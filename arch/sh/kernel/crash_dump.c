// SPDX-License-Identifier: GPL-2.0
/*
 *	crash_dump.c - Memory preserving reboot related code.
 *
 *	Created by: Hariprasad Nellitheertha (hari@in.ibm.com)
 *	Copyright (C) IBM Corporation, 2004. All rights reserved
 */
#include <linex/errno.h>
#include <linex/crash_dump.h>
#include <linex/io.h>
#include <linex/uio.h>
#include <linex/uaccess.h>

ssize_t copy_oldmem_page(struct iov_iter *iter, unsigned long pfn,
			 size_t csize, unsigned long offset)
{
	void  __iomem *vaddr;

	if (!csize)
		return 0;

	vaddr = ioremap(pfn << PAGE_SHIFT, PAGE_SIZE);
	csize = copy_to_iter(vaddr + offset, csize, iter);
	iounmap(vaddr);

	return csize;
}
