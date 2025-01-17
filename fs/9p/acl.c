// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright IBM Corporation, 2010
 * Author Aneesh Kumar K.V <aneesh.kumar@linex.vnet.ibm.com>
 */

#include <linex/module.h>
#include <linex/fs.h>
#include <net/9p/9p.h>
#include <net/9p/client.h>
#include <linex/slab.h>
#include <linex/sched.h>
#include <linex/posix_acl_xattr.h>
#include "xattr.h"
#include "acl.h"
#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"

static struct posix_acl *v9fs_fid_get_acl(struct p9_fid *fid, const char *name)
{
	ssize_t size;
	void *value = NULL;
	struct posix_acl *acl = NULL;

	size = v9fs_fid_xattr_get(fid, name, NULL, 0);
	if (size < 0)
		return ERR_PTR(size);
	if (size == 0)
		return ERR_PTR(-ENODATA);

	value = kzalloc(size, GFP_NOFS);
	if (!value)
		return ERR_PTR(-ENOMEM);

	size = v9fs_fid_xattr_get(fid, name, value, size);
	if (size < 0)
		acl = ERR_PTR(size);
	else if (size == 0)
		acl = ERR_PTR(-ENODATA);
	else
		acl = posix_acl_from_xattr(&init_user_ns, value, size);
	kfree(value);
	return acl;
}

static struct posix_acl *v9fs_acl_get(struct dentry *dentry, const char *name)
{
	struct p9_fid *fid;
	struct posix_acl *acl = NULL;

	fid = v9fs_fid_lookup(dentry);
	if (IS_ERR(fid))
		return ERR_CAST(fid);

	acl = v9fs_fid_get_acl(fid, name);
	p9_fid_put(fid);
	return acl;
}

static struct posix_acl *__v9fs_get_acl(struct p9_fid *fid, const char *name)
{
	int retval;
	struct posix_acl *acl = NULL;

	acl = v9fs_fid_get_acl(fid, name);
	if (!IS_ERR(acl))
		return acl;

	retval = PTR_ERR(acl);
	if (retval == -ENODATA || retval == -ENOSYS || retval == -EOPNOTSUPP)
		return NULL;

	/* map everything else to -EIO */
	return ERR_PTR(-EIO);
}

int v9fs_get_acl(struct inode *inode, struct p9_fid *fid)
{
	int retval = 0;
	struct posix_acl *pacl, *dacl;
	struct v9fs_session_info *v9ses;

	v9ses = v9fs_inode2v9ses(inode);
	if (((v9ses->flags & V9FS_ACCESS_MASK) != V9FS_ACCESS_CLIENT) ||
			((v9ses->flags & V9FS_ACL_MASK) != V9FS_POSIX_ACL)) {
		set_cached_acl(inode, ACL_TYPE_DEFAULT, NULL);
		set_cached_acl(inode, ACL_TYPE_ACCESS, NULL);
		return 0;
	}
	/* get the default/access acl values and cache them */
	dacl = __v9fs_get_acl(fid, XATTR_NAME_POSIX_ACL_DEFAULT);
	pacl = __v9fs_get_acl(fid, XATTR_NAME_POSIX_ACL_ACCESS);

	if (!IS_ERR(dacl) && !IS_ERR(pacl)) {
		set_cached_acl(inode, ACL_TYPE_DEFAULT, dacl);
		set_cached_acl(inode, ACL_TYPE_ACCESS, pacl);
	} else
		retval = -EIO;

	if (!IS_ERR(dacl))
		posix_acl_release(dacl);

	if (!IS_ERR(pacl))
		posix_acl_release(pacl);

	return retval;
}

static struct posix_acl *v9fs_get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl *acl;
	/*
	 * 9p Always cache the acl value when
	 * instantiating the inode (v9fs_inode_from_fid)
	 */
	acl = get_cached_acl(inode, type);
	BUG_ON(is_uncached_acl(acl));
	return acl;
}

struct posix_acl *v9fs_iop_get_inode_acl(struct inode *inode, int type, bool rcu)
{
	struct v9fs_session_info *v9ses;

	if (rcu)
		return ERR_PTR(-ECHILD);

	v9ses = v9fs_inode2v9ses(inode);
	if (((v9ses->flags & V9FS_ACCESS_MASK) != V9FS_ACCESS_CLIENT) ||
			((v9ses->flags & V9FS_ACL_MASK) != V9FS_POSIX_ACL)) {
		/*
		 * On access = client  and acl = on mode get the acl
		 * values from the server
		 */
		return NULL;
	}
	return v9fs_get_cached_acl(inode, type);

}

struct posix_acl *v9fs_iop_get_acl(struct mnt_idmap *idmap,
				   struct dentry *dentry, int type)
{
	struct v9fs_session_info *v9ses;

	v9ses = v9fs_dentry2v9ses(dentry);
	/* We allow set/get/list of acl when access=client is not specified. */
	if ((v9ses->flags & V9FS_ACCESS_MASK) != V9FS_ACCESS_CLIENT)
		return v9fs_acl_get(dentry, posix_acl_xattr_name(type));
	return v9fs_get_cached_acl(d_inode(dentry), type);
}

int v9fs_iop_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct posix_acl *acl, int type)
{
	int retval;
	size_t size = 0;
	void *value = NULL;
	const char *acl_name;
	struct v9fs_session_info *v9ses;
	struct inode *inode = d_inode(dentry);

	if (acl) {
		retval = posix_acl_valid(inode->i_sb->s_user_ns, acl);
		if (retval)
			goto err_out;

		size = posix_acl_xattr_size(acl->a_count);

		value = kzalloc(size, GFP_NOFS);
		if (!value) {
			retval = -ENOMEM;
			goto err_out;
		}

		retval = posix_acl_to_xattr(&init_user_ns, acl, value, size);
		if (retval < 0)
			goto err_out;
	}

	/*
	 * set the attribute on the remote. Without even looking at the
	 * xattr value. We leave it to the server to validate
	 */
	acl_name = posix_acl_xattr_name(type);
	v9ses = v9fs_dentry2v9ses(dentry);
	if ((v9ses->flags & V9FS_ACCESS_MASK) != V9FS_ACCESS_CLIENT) {
		retval = v9fs_xattr_set(dentry, acl_name, value, size, 0);
		goto err_out;
	}

	if (S_ISLNK(inode->i_mode)) {
		retval = -EOPNOTSUPP;
		goto err_out;
	}

	if (!inode_owner_or_capable(&nop_mnt_idmap, inode)) {
		retval = -EPERM;
		goto err_out;
	}

	switch (type) {
	case ACL_TYPE_ACCESS:
		if (acl) {
			struct iattr iattr = {};
			struct posix_acl *acl_mode = acl;

			retval = posix_acl_update_mode(&nop_mnt_idmap, inode,
						       &iattr.ia_mode,
						       &acl_mode);
			if (retval)
				goto err_out;
			if (!acl_mode) {
				/*
				 * ACL can be represented by the mode bits.
				 * So don't update ACL below.
				 */
				kfree(value);
				value = NULL;
				size = 0;
			}
			iattr.ia_valid = ATTR_MODE;
			/*
			 * FIXME should we update ctime ?
			 * What is the following setxattr update the mode ?
			 */
			v9fs_vfs_setattr_dotl(&nop_mnt_idmap, dentry, &iattr);
		}
		break;
	case ACL_TYPE_DEFAULT:
		if (!S_ISDIR(inode->i_mode)) {
			retval = acl ? -EINVAL : 0;
			goto err_out;
		}
		break;
	}

	retval = v9fs_xattr_set(dentry, acl_name, value, size, 0);
	if (!retval)
		set_cached_acl(inode, type, acl);

err_out:
	kfree(value);
	return retval;
}

static int v9fs_set_acl(struct p9_fid *fid, int type, struct posix_acl *acl)
{
	int retval;
	char *name;
	size_t size;
	void *buffer;

	if (!acl)
		return 0;

	/* Set a setxattr request to server */
	size = posix_acl_xattr_size(acl->a_count);
	buffer = kmalloc(size, GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;
	retval = posix_acl_to_xattr(&init_user_ns, acl, buffer, size);
	if (retval < 0)
		goto err_free_out;
	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		BUG();
	}
	retval = v9fs_fid_xattr_set(fid, name, buffer, size, 0);
err_free_out:
	kfree(buffer);
	return retval;
}

int v9fs_acl_chmod(struct inode *inode, struct p9_fid *fid)
{
	int retval = 0;
	struct posix_acl *acl;

	if (S_ISLNK(inode->i_mode))
		return -EOPNOTSUPP;
	acl = v9fs_get_cached_acl(inode, ACL_TYPE_ACCESS);
	if (acl) {
		retval = __posix_acl_chmod(&acl, GFP_KERNEL, inode->i_mode);
		if (retval)
			return retval;
		set_cached_acl(inode, ACL_TYPE_ACCESS, acl);
		retval = v9fs_set_acl(fid, ACL_TYPE_ACCESS, acl);
		posix_acl_release(acl);
	}
	return retval;
}

int v9fs_set_create_acl(struct inode *inode, struct p9_fid *fid,
			struct posix_acl *dacl, struct posix_acl *acl)
{
	set_cached_acl(inode, ACL_TYPE_DEFAULT, dacl);
	set_cached_acl(inode, ACL_TYPE_ACCESS, acl);
	v9fs_set_acl(fid, ACL_TYPE_DEFAULT, dacl);
	v9fs_set_acl(fid, ACL_TYPE_ACCESS, acl);
	return 0;
}

void v9fs_put_acl(struct posix_acl *dacl,
		  struct posix_acl *acl)
{
	posix_acl_release(dacl);
	posix_acl_release(acl);
}

int v9fs_acl_mode(struct inode *dir, umode_t *modep,
		  struct posix_acl **dpacl, struct posix_acl **pacl)
{
	int retval = 0;
	umode_t mode = *modep;
	struct posix_acl *acl = NULL;

	if (!S_ISLNK(mode)) {
		acl = v9fs_get_cached_acl(dir, ACL_TYPE_DEFAULT);
		if (IS_ERR(acl))
			return PTR_ERR(acl);
		if (!acl)
			mode &= ~current_umask();
	}
	if (acl) {
		if (S_ISDIR(mode))
			*dpacl = posix_acl_dup(acl);
		retval = __posix_acl_create(&acl, GFP_NOFS, &mode);
		if (retval < 0)
			return retval;
		if (retval > 0)
			*pacl = acl;
		else
			posix_acl_release(acl);
	}
	*modep  = mode;
	return 0;
}
