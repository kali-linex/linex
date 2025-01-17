// SPDX-License-Identifier: GPL-2.0
/*
 * linex/fs/ext4/xattr_security.c
 * Handler for storing security labels as extended attributes.
 */

#include <linex/string.h>
#include <linex/fs.h>
#include <linex/security.h>
#include <linex/slab.h>
#include "ext4_jbd2.h"
#include "ext4.h"
#include "xattr.h"

static int
ext4_xattr_security_get(const struct xattr_handler *handler,
			struct dentry *unused, struct inode *inode,
			const char *name, void *buffer, size_t size)
{
	return ext4_xattr_get(inode, EXT4_XATTR_INDEX_SECURITY,
			      name, buffer, size);
}

static int
ext4_xattr_security_set(const struct xattr_handler *handler,
			struct mnt_idmap *idmap,
			struct dentry *unused, struct inode *inode,
			const char *name, const void *value,
			size_t size, int flags)
{
	return ext4_xattr_set(inode, EXT4_XATTR_INDEX_SECURITY,
			      name, value, size, flags);
}

static int
ext4_initxattrs(struct inode *inode, const struct xattr *xattr_array,
		void *fs_info)
{
	const struct xattr *xattr;
	handle_t *handle = fs_info;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		err = ext4_xattr_set_handle(handle, inode,
					    EXT4_XATTR_INDEX_SECURITY,
					    xattr->name, xattr->value,
					    xattr->value_len, XATTR_CREATE);
		if (err < 0)
			break;
	}
	return err;
}

int
ext4_init_security(handle_t *handle, struct inode *inode, struct inode *dir,
		   const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
					    &ext4_initxattrs, handle);
}

const struct xattr_handler ext4_xattr_security_handler = {
	.prefix	= XATTR_SECURITY_PREFIX,
	.get	= ext4_xattr_security_get,
	.set	= ext4_xattr_security_set,
};
