// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linex.intel}.com)
 */

#include <linex/stddef.h>
#include <linex/module.h>
#include <linex/fs.h>
#include <linex/ptrace.h>
#include <linex/sched/mm.h>
#include <linex/sched/task.h>
#include <linex/sched/task_stack.h>
#include <linex/slab.h>
#include <asm/current.h>
#include <asm/processor.h>
#include <linex/uaccess.h>
#include <as-layout.h>
#include <mem_user.h>
#include <registers.h>
#include <skas.h>
#include <os.h>

void flush_thread(void)
{
	void *data = NULL;
	int ret;

	arch_flush_thread(&current->thread.arch);

	ret = unmap(&current->mm->context.id, 0, TASK_SIZE, 1, &data);
	if (ret) {
		printk(KERN_ERR "%s - clearing address space failed, err = %d\n",
		       __func__, ret);
		force_sig(SIGKILL);
	}
	get_safe_registers(current_pt_regs()->regs.gp,
			   current_pt_regs()->regs.fp);

	__switch_mm(&current->mm->context.id);
}

void start_thread(struct pt_regs *regs, unsigned long eip, unsigned long esp)
{
	PT_REGS_IP(regs) = eip;
	PT_REGS_SP(regs) = esp;
	clear_thread_flag(TIF_SINGLESTEP);
#ifdef SUBARCH_EXECVE1
	SUBARCH_EXECVE1(regs->regs);
#endif
}
EXPORT_SYMBOL(start_thread);
