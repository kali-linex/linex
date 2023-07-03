// SPDX-License-Identifier: GPL-2.0
#include <uapi/linex/unistd.h>
#include <linex/kbuild.h>

#define SYSNR(_NR) DEFINE(SYS ## _NR, _NR)

void syscall_defines(void)
{
	COMMENT("Linex system call numbers.");
	SYSNR(__NR_write);
	SYSNR(__NR_read);
#ifdef __NR_mmap2
	SYSNR(__NR_mmap2);
#endif
#ifdef __NR_mmap
	SYSNR(__NR_mmap);
#endif

}
