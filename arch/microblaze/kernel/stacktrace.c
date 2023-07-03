/*
 * Stack trace support for Microblaze.
 *
 * Copyright (C) 2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2009 PetaLogix
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linex/export.h>
#include <linex/sched.h>
#include <linex/stacktrace.h>
#include <linex/thread_info.h>
#include <linex/ptrace.h>
#include <asm/unwind.h>

void save_stack_trace(struct stack_trace *trace)
{
	/* Exclude our helper functions from the trace*/
	trace->skip += 2;
	microblaze_unwind(NULL, trace, "");
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	microblaze_unwind(tsk, trace, "");
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
