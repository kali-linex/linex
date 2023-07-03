// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2018 Hangzhou C-SKY Microsystems co.,ltd.

#include <linex/init.h>
#include <linex/interrupt.h>
#include <linex/irq.h>
#include <linex/irqchip.h>
#include <asm/traps.h>
#include <asm/smp.h>

void __init init_IRQ(void)
{
	irqchip_init();
#ifdef CONFIG_SMP
	setup_smp_ipi();
#endif
}
