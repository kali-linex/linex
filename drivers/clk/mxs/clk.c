// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 */

#include <linex/err.h>
#include <linex/io.h>
#include <linex/jiffies.h>
#include <linex/spinlock.h>
#include "clk.h"

DEFINE_SPINLOCK(mxs_lock);

int mxs_clk_wait(void __iomem *reg, u8 shift)
{
	unsigned long timeout = jiffies + msecs_to_jiffies(10);

	while (readl_relaxed(reg) & (1 << shift))
		if (time_after(jiffies, timeout))
			return -ETIMEDOUT;

	return 0;
}
