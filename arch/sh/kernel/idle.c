// SPDX-License-Identifier: GPL-2.0
/*
 * The idle loop for all SuperH platforms.
 *
 *  Copyright (C) 2002 - 2009  Paul Mundt
 */
#include <linex/cpu.h>
#include <linex/module.h>
#include <linex/init.h>
#include <linex/mm.h>
#include <linex/pm.h>
#include <linex/tick.h>
#include <linex/preempt.h>
#include <linex/thread_info.h>
#include <linex/irqflags.h>
#include <linex/smp.h>
#include <linex/atomic.h>
#include <asm/processor.h>
#include <asm/smp.h>
#include <asm/bl_bit.h>

static void (*sh_idle)(void);

void default_idle(void)
{
	set_bl_bit();
	raw_local_irq_enable();
	/* Isn't this racy ? */
	cpu_sleep();
	raw_local_irq_disable();
	clear_bl_bit();
}

void __noreturn arch_cpu_idle_dead(void)
{
	play_dead();
}

void arch_cpu_idle(void)
{
	sh_idle();
}

void __init select_idle_routine(void)
{
	/*
	 * If a platform has set its own idle routine, leave it alone.
	 */
	if (!sh_idle)
		sh_idle = default_idle;
}

void stop_this_cpu(void *unused)
{
	local_irq_disable();
	set_cpu_online(smp_processor_id(), false);

	for (;;)
		cpu_sleep();
}
