// SPDX-License-Identifier: GPL-2.0
/*
 * clk-da8xx-cfgchip - TI DaVinci DA8xx CFGCHIP clock driver
 *
 * Copyright (C) 2018 David Lechner <david@lechnology.com>
 */

#ifndef __LINEX_PLATFORM_DATA_CLK_DA8XX_CFGCHIP_H__
#define __LINEX_PLATFORM_DATA_CLK_DA8XX_CFGCHIP_H__

#include <linex/regmap.h>

/**
 * da8xx_cfgchip_clk_platform_data
 * @cfgchip: CFGCHIP syscon regmap
 */
struct da8xx_cfgchip_clk_platform_data {
	struct regmap *cfgchip;
};

#endif /* __LINEX_PLATFORM_DATA_CLK_DA8XX_CFGCHIP_H__ */
