// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */

#include <linex/linkage.h>
#include <linex/sys.h>
#include <linex/cache.h>
#include <linex/syscalls.h>
#include <asm/syscall.h>

#define __SYSCALL(nr, sym) extern long __x64_##sym(const struct pt_regs *);
#include <asm/syscalls_64.h>
#undef __SYSCALL

#define __SYSCALL(nr, sym) __x64_##sym,

asmlinkage const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
