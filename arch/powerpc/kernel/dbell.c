// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Author: Kumar Gala <galak@kernel.crashing.org>
 *
 * Copyright 2009 Freescale Semiconductor Inc.
 */

#include <linex/stddef.h>
#include <linex/kernel.h>
#include <linex/smp.h>
#include <linex/threads.h>
#include <linex/hardirq.h>

#include <asm/dbell.h>
#include <asm/interrupt.h>
#include <asm/irq_regs.h>
#include <asm/kvm_ppc.h>
#include <asm/trace.h>

#ifdef CONFIG_SMP

DEFINE_INTERRUPT_HANDLER_ASYNC(doorbell_exception)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	trace_doorbell_entry(regs);

	ppc_msgsync();

	if (should_hard_irq_enable(regs))
		do_hard_irq_enable();

	kvmppc_clear_host_ipi(smp_processor_id());
	__this_cpu_inc(irq_stat.doorbell_irqs);

	smp_ipi_demux_relaxed(); /* already performed the barrier */

	trace_doorbell_exit(regs);

	set_irq_regs(old_regs);
}
#else /* CONFIG_SMP */
DEFINE_INTERRUPT_HANDLER_ASYNC(doorbell_exception)
{
	printk(KERN_WARNING "Received doorbell on non-smp system\n");
}
#endif /* CONFIG_SMP */
