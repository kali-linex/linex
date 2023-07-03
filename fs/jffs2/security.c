/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright © 2006  NEC Corporation
 *
 * Created by KaiGai Kohei <kaigai@ak.jp.nec.com>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <linex/kernel.h>
#include <linex/slab.h>
#include <linex/fs.h>
#include <linex/time.h>
#include <linex/pagemap.h>
#include <linex/highmem.h>
#include <linex/crc32.h>
#include <linex/jffs2.h>
#include <linex/xattr.h>
#include <linex/mtd/mtd.h>
#include <linex/security.h>
#include "nodelist.h"

/* ---- Initial Security Label(s) Attachment callback --- */
static int jffs2_initxattrs(struct inode *inode,
			    const struct xattr *xattr_array, void *fs_info)
{
	const struct xattr *xattr;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		err = do_jffs2_setxattr(inode, JFFS2_XPREFIX_SECURITY,
					xattr->name, xattr->value,
					xattr->value_len, 0);
		if (err < 0)
			break;
	}
	return err;
}

/* ---- Initial Security Label(s) Attachment ----------- */
int jffs2_init_security(struct inode *inode, struct inode *dir,
			const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
					    &jffs2_initxattrs, NULL);
}

/* ---- XATTR Handler for "security.*" ----------------- */
static int jffs2_security_getxattr(const struct xattr_handler *handler,
				   struct dentry *unused, struct inode *inode,
				   const char *name, void *buffer, size_t size)
{
	return do_jffs2_getxattr(inode, JFFS2_XPREFIX_SECURITY,
				 name, buffer, size);
}

static int jffs2_security_setxattr(const struct xattr_handler *handler,
				   struct mnt_idmap *idmap,
				   struct dentry *unused, struct inode *inode,
				   const char *name, const void *buffer,
				   size_t size, int flags)
{
	return do_jffs2_setxattr(inode, JFFS2_XPREFIX_SECURITY,
				 name, buffer, size, flags);
}

const struct xattr_handler jffs2_security_xattr_handler = {
	.prefix = XATTR_SECURITY_PREFIX,
	.set = jffs2_security_setxattr,
	.get = jffs2_security_getxattr
};
