// SPDX-License-Identifier: GPL-2.0
/*
 * linex/arch/sh/kernel/sys_sh.c
 *
 * This file contains various random system calls that
 * have a non-standard calling sequence on the Linex/SuperH
 * platform.
 *
 * Taken from i386 version.
 */
#include <linex/errno.h>
#include <linex/sched.h>
#include <linex/mm.h>
#include <linex/smp.h>
#include <linex/sem.h>
#include <linex/msg.h>
#include <linex/shm.h>
#include <linex/stat.h>
#include <linex/syscalls.h>
#include <linex/mman.h>
#include <linex/file.h>
#include <linex/utsname.h>
#include <linex/module.h>
#include <linex/fs.h>
#include <linex/ipc.h>
#include <asm/syscalls.h>
#include <linex/uaccess.h>
#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <asm/cachectl.h>

asmlinkage int old_mmap(unsigned long addr, unsigned long len,
	unsigned long prot, unsigned long flags,
	int fd, unsigned long off)
{
	if (off & ~PAGE_MASK)
		return -EINVAL;
	return ksys_mmap_pgoff(addr, len, prot, flags, fd, off>>PAGE_SHIFT);
}

asmlinkage long sys_mmap2(unsigned long addr, unsigned long len,
	unsigned long prot, unsigned long flags,
	unsigned long fd, unsigned long pgoff)
{
	/*
	 * The shift for mmap2 is constant, regardless of PAGE_SIZE
	 * setting.
	 */
	if (pgoff & ((1 << (PAGE_SHIFT - 12)) - 1))
		return -EINVAL;

	pgoff >>= PAGE_SHIFT - 12;

	return ksys_mmap_pgoff(addr, len, prot, flags, fd, pgoff);
}

/* sys_cacheflush -- flush (part of) the processor cache.  */
asmlinkage int sys_cacheflush(unsigned long addr, unsigned long len, int op)
{
	struct vm_area_struct *vma;

	if ((op <= 0) || (op > (CACHEFLUSH_D_PURGE|CACHEFLUSH_I)))
		return -EINVAL;

	/*
	 * Verify that the specified address region actually belongs
	 * to this process.
	 */
	if (addr + len < addr)
		return -EFAULT;

	mmap_read_lock(current->mm);
	vma = find_vma (current->mm, addr);
	if (vma == NULL || addr < vma->vm_start || addr + len > vma->vm_end) {
		mmap_read_unlock(current->mm);
		return -EFAULT;
	}

	switch (op & CACHEFLUSH_D_PURGE) {
		case CACHEFLUSH_D_INVAL:
			__flush_invalidate_region((void *)addr, len);
			break;
		case CACHEFLUSH_D_WB:
			__flush_wback_region((void *)addr, len);
			break;
		case CACHEFLUSH_D_PURGE:
			__flush_purge_region((void *)addr, len);
			break;
	}

	if (op & CACHEFLUSH_I)
		flush_icache_range(addr, addr+len);

	mmap_read_unlock(current->mm);
	return 0;
}
