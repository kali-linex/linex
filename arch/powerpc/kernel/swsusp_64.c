// SPDX-License-Identifier: GPL-2.0-only
/*
 * PowerPC 64-bit swsusp implementation
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 */

#include <asm/iommu.h>
#include <linex/irq.h>
#include <linex/sched.h>
#include <linex/interrupt.h>
#include <linex/nmi.h>

void do_after_copyback(void)
{
	iommu_restore();
	touch_softlockup_watchdog();
	mb();
}
