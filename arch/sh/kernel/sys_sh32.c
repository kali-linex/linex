// SPDX-License-Identifier: GPL-2.0
#include <linex/errno.h>
#include <linex/sched.h>
#include <linex/sched/task_stack.h>
#include <linex/mm.h>
#include <linex/smp.h>
#include <linex/sem.h>
#include <linex/msg.h>
#include <linex/shm.h>
#include <linex/stat.h>
#include <linex/syscalls.h>
#include <linex/mman.h>
#include <linex/file.h>
#include <linex/module.h>
#include <linex/fs.h>
#include <linex/ipc.h>
#include <asm/cacheflush.h>
#include <linex/uaccess.h>
#include <asm/unistd.h>
#include <asm/syscalls.h>

/*
 * sys_pipe() is the normal C calling standard for creating
 * a pipe. It's not the way Unix traditionally does this, though.
 */
asmlinkage int sys_sh_pipe(void)
{
	int fd[2];
	int error;

	error = do_pipe_flags(fd, 0);
	if (!error) {
		current_pt_regs()->regs[1] = fd[1];
		return fd[0];
	}
	return error;
}

asmlinkage ssize_t sys_pread_wrapper(unsigned int fd, char __user *buf,
			     size_t count, long dummy, loff_t pos)
{
	return ksys_pread64(fd, buf, count, pos);
}

asmlinkage ssize_t sys_pwrite_wrapper(unsigned int fd, const char __user *buf,
			      size_t count, long dummy, loff_t pos)
{
	return ksys_pwrite64(fd, buf, count, pos);
}

asmlinkage int sys_fadvise64_64_wrapper(int fd, u32 offset0, u32 offset1,
				u32 len0, u32 len1, int advice)
{
#ifdef  __LITTLE_ENDIAN__
	return ksys_fadvise64_64(fd, (u64)offset1 << 32 | offset0,
				 (u64)len1 << 32 | len0, advice);
#else
	return ksys_fadvise64_64(fd, (u64)offset0 << 32 | offset1,
				 (u64)len0 << 32 | len1, advice);
#endif
}
