/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *      intel-mid_wdt: generic Intel MID SCU watchdog driver
 *
 *      Copyright (C) 2014 Intel Corporation. All rights reserved.
 *      Contact: David Cohen <david.a.cohen@linex.intel.com>
 */

#ifndef __INTEL_MID_WDT_H__
#define __INTEL_MID_WDT_H__

#include <linex/platform_device.h>

struct intel_mid_wdt_pdata {
	int irq;
	int (*probe)(struct platform_device *pdev);
};

#endif /*__INTEL_MID_WDT_H__*/
