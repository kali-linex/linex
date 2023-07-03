/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  include/linex/anon_inodes.h
 *
 *  Copyright (C) 2007  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#ifndef _LINEX_ANON_INODES_H
#define _LINEX_ANON_INODES_H

struct file_operations;
struct inode;

struct file *anon_inode_getfile(const char *name,
				const struct file_operations *fops,
				void *priv, int flags);
struct file *anon_inode_getfile_secure(const char *name,
				       const struct file_operations *fops,
				       void *priv, int flags,
				       const struct inode *context_inode);
int anon_inode_getfd(const char *name, const struct file_operations *fops,
		     void *priv, int flags);
int anon_inode_getfd_secure(const char *name,
			    const struct file_operations *fops,
			    void *priv, int flags,
			    const struct inode *context_inode);

#endif /* _LINEX_ANON_INODES_H */

