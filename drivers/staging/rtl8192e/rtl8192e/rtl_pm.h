/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
#ifndef R8192E_PM_H
#define R8192E_PM_H

#include <linex/types.h>
#include <linex/pci.h>

int rtl92e_suspend(struct device *dev_d);
int rtl92e_resume(struct device *dev_d);

#endif
