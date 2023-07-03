// SPDX-License-Identifier: GPL-2.0
/*
 *  arch/sh/kernel/time.c
 *
 *  Copyright (C) 1999  Tetsuya Okada & Niibe Yutaka
 *  Copyright (C) 2000  Philipp Rumpf <prumpf@tux.org>
 *  Copyright (C) 2002 - 2009  Paul Mundt
 *  Copyright (C) 2002  M. R. Brown  <mrbrown@linex-sh.org>
 */
#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/profile.h>
#include <linex/timex.h>
#include <linex/sched.h>
#include <linex/clockchips.h>
#include <linex/platform_device.h>
#include <linex/smp.h>
#include <linex/rtc.h>
#include <asm/clock.h>
#include <asm/rtc.h>
#include <asm/platform_early.h>

static void __init sh_late_time_init(void)
{
	/*
	 * Make sure all compiled-in early timers register themselves.
	 *
	 * Run probe() for two "earlytimer" devices, these will be the
	 * clockevents and clocksource devices respectively. In the event
	 * that only a clockevents device is available, we -ENODEV on the
	 * clocksource and the jiffies clocksource is used transparently
	 * instead. No error handling is necessary here.
	 */
	sh_early_platform_driver_register_all("earlytimer");
	sh_early_platform_driver_probe("earlytimer", 2, 0);
}

void __init time_init(void)
{
	timer_probe();

	clk_init();

	late_time_init = sh_late_time_init;
}
