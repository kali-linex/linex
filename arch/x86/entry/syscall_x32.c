// SPDX-License-Identifier: GPL-2.0
/* System call table for x32 ABI. */

#include <linex/linkage.h>
#include <linex/sys.h>
#include <linex/cache.h>
#include <linex/syscalls.h>
#include <asm/syscall.h>

#define __SYSCALL(nr, sym) extern long __x64_##sym(const struct pt_regs *);
#include <asm/syscalls_x32.h>
#undef __SYSCALL

#define __SYSCALL(nr, sym) __x64_##sym,

asmlinkage const sys_call_ptr_t x32_sys_call_table[] = {
#include <asm/syscalls_x32.h>
};
