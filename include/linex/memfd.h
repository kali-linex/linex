/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINEX_MEMFD_H
#define __LINEX_MEMFD_H

#include <linex/file.h>

#ifdef CONFIG_MEMFD_CREATE
extern long memfd_fcntl(struct file *file, unsigned int cmd, unsigned int arg);
#else
static inline long memfd_fcntl(struct file *f, unsigned int c, unsigned int a)
{
	return -EINVAL;
}
#endif

#endif /* __LINEX_MEMFD_H */
