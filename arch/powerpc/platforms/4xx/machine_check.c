// SPDX-License-Identifier: GPL-2.0-or-later
/*
 */

#include <linex/kernel.h>
#include <linex/printk.h>
#include <linex/ptrace.h>

#include <asm/reg.h>

int machine_check_4xx(struct pt_regs *regs)
{
	unsigned long reason = regs->esr;

	if (reason & ESR_IMCP) {
		printk("Instruction");
		mtspr(SPRN_ESR, reason & ~ESR_IMCP);
	} else
		printk("Data");
	printk(" machine check in kernel mode.\n");

	return 0;
}
