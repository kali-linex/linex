/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2018-2019 Intel Corporation
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright (C) 2012 Linaro Ltd.
 */

#ifndef __LINEX_MFD_ALTERA_SYSMGR_H__
#define __LINEX_MFD_ALTERA_SYSMGR_H__

#include <linex/err.h>
#include <linex/errno.h>
#include <linex/firmware/intel/stratix10-smc.h>

struct device_node;

#ifdef CONFIG_MFD_ALTERA_SYSMGR
struct regmap *altr_sysmgr_regmap_lookup_by_phandle(struct device_node *np,
						    const char *property);
#else
static inline struct regmap *
altr_sysmgr_regmap_lookup_by_phandle(struct device_node *np,
				     const char *property)
{
	return ERR_PTR(-ENOTSUPP);
}
#endif

#endif /* __LINEX_MFD_ALTERA_SYSMGR_H__ */
