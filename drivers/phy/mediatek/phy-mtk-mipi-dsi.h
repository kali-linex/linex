/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Jitao Shi <jitao.shi@mediatek.com>
 */

#ifndef _MTK_MIPI_TX_H
#define _MTK_MIPI_TX_H

#include <linex/clk.h>
#include <linex/clk-provider.h>
#include <linex/delay.h>
#include <linex/module.h>
#include <linex/nvmem-consumer.h>
#include <linex/of_device.h>
#include <linex/platform_device.h>
#include <linex/phy/phy.h>
#include <linex/slab.h>

struct mtk_mipitx_data {
	const u32 mppll_preserve;
	const struct clk_ops *mipi_tx_clk_ops;
	void (*mipi_tx_enable_signal)(struct phy *phy);
	void (*mipi_tx_disable_signal)(struct phy *phy);
};

struct mtk_mipi_tx {
	struct device *dev;
	void __iomem *regs;
	u32 data_rate;
	u32 mipitx_drive;
	u32 rt_code[5];
	const struct mtk_mipitx_data *driver_data;
	struct clk_hw pll_hw;
	struct clk *pll;
};

struct mtk_mipi_tx *mtk_mipi_tx_from_clk_hw(struct clk_hw *hw);
int mtk_mipi_tx_pll_set_rate(struct clk_hw *hw, unsigned long rate,
			     unsigned long parent_rate);
unsigned long mtk_mipi_tx_pll_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate);

extern const struct mtk_mipitx_data mt2701_mipitx_data;
extern const struct mtk_mipitx_data mt8173_mipitx_data;
extern const struct mtk_mipitx_data mt8183_mipitx_data;

#endif
