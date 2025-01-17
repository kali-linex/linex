// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022, 2023 Linaro Ltd.
 */
#include <linex/bitfield.h>
#include <linex/clk.h>
#include <linex/clk-provider.h>
#include <linex/of.h>
#include <linex/module.h>
#include <linex/platform_device.h>
#include <linex/regmap.h>

#include "clk-alpha-pll.h"
#include "clk-regmap.h"

/* Need to match the order of clocks in DT binding */
enum {
	DT_XO,
	DT_APCS_AUX,
};

enum {
	CBF_XO_INDEX,
	CBF_PLL_INDEX,
	CBF_DIV_INDEX,
	CBF_APCS_AUX_INDEX,
};

#define DIV_THRESHOLD		600000000

#define CBF_MUX_OFFSET		0x18
#define CBF_MUX_PARENT_MASK		GENMASK(1, 0)
#define CBF_MUX_AUTO_CLK_SEL_ALWAYS_ON_MASK GENMASK(5, 4)
#define CBF_MUX_AUTO_CLK_SEL_ALWAYS_ON_GPLL0_SEL \
	FIELD_PREP(CBF_MUX_AUTO_CLK_SEL_ALWAYS_ON_MASK, 0x03)
#define CBF_MUX_AUTO_CLK_SEL_BIT	BIT(6)

#define CBF_PLL_OFFSET 0xf000

static const u8 cbf_pll_regs[PLL_OFF_MAX_REGS] = {
	[PLL_OFF_L_VAL] = 0x08,
	[PLL_OFF_ALPHA_VAL] = 0x10,
	[PLL_OFF_USER_CTL] = 0x18,
	[PLL_OFF_CONFIG_CTL] = 0x20,
	[PLL_OFF_CONFIG_CTL_U] = 0x24,
	[PLL_OFF_TEST_CTL] = 0x30,
	[PLL_OFF_TEST_CTL_U] = 0x34,
	[PLL_OFF_STATUS] = 0x28,
};

static const struct alpha_pll_config cbfpll_config = {
	.l = 72,
	.config_ctl_val = 0x200d4828,
	.config_ctl_hi_val = 0x006,
	.test_ctl_val = 0x1c000000,
	.test_ctl_hi_val = 0x00004000,
	.pre_div_mask = BIT(12),
	.post_div_mask = 0x3 << 8,
	.post_div_val = 0x1 << 8,
	.main_output_mask = BIT(0),
	.early_output_mask = BIT(3),
};

static struct clk_alpha_pll cbf_pll = {
	.offset = CBF_PLL_OFFSET,
	.regs = cbf_pll_regs,
	.flags = SUPPORTS_DYNAMIC_UPDATE | SUPPORTS_FSM_MODE,
	.clkr.hw.init = &(struct clk_init_data){
		.name = "cbf_pll",
		.parent_data = (const struct clk_parent_data[]) {
			{ .index = DT_XO, },
		},
		.num_parents = 1,
		.ops = &clk_alpha_pll_hwfsm_ops,
	},
};

static struct clk_fixed_factor cbf_pll_postdiv = {
	.mult = 1,
	.div = 2,
	.hw.init = &(struct clk_init_data){
		.name = "cbf_pll_postdiv",
		.parent_hws = (const struct clk_hw*[]){
			&cbf_pll.clkr.hw
		},
		.num_parents = 1,
		.ops = &clk_fixed_factor_ops,
		.flags = CLK_SET_RATE_PARENT,
	},
};

static const struct clk_parent_data cbf_mux_parent_data[] = {
	{ .index = DT_XO },
	{ .hw = &cbf_pll.clkr.hw },
	{ .hw = &cbf_pll_postdiv.hw },
	{ .index = DT_APCS_AUX },
};

struct clk_cbf_8996_mux {
	u32 reg;
	struct notifier_block nb;
	struct clk_regmap clkr;
};

static struct clk_cbf_8996_mux *to_clk_cbf_8996_mux(struct clk_regmap *clkr)
{
	return container_of(clkr, struct clk_cbf_8996_mux, clkr);
}

static int cbf_clk_notifier_cb(struct notifier_block *nb, unsigned long event,
			       void *data);

static u8 clk_cbf_8996_mux_get_parent(struct clk_hw *hw)
{
	struct clk_regmap *clkr = to_clk_regmap(hw);
	struct clk_cbf_8996_mux *mux = to_clk_cbf_8996_mux(clkr);
	u32 val;

	regmap_read(clkr->regmap, mux->reg, &val);

	return FIELD_GET(CBF_MUX_PARENT_MASK, val);
}

static int clk_cbf_8996_mux_set_parent(struct clk_hw *hw, u8 index)
{
	struct clk_regmap *clkr = to_clk_regmap(hw);
	struct clk_cbf_8996_mux *mux = to_clk_cbf_8996_mux(clkr);
	u32 val;

	val = FIELD_PREP(CBF_MUX_PARENT_MASK, index);

	return regmap_update_bits(clkr->regmap, mux->reg, CBF_MUX_PARENT_MASK, val);
}

static int clk_cbf_8996_mux_determine_rate(struct clk_hw *hw,
					   struct clk_rate_request *req)
{
	struct clk_hw *parent;

	if (req->rate < (DIV_THRESHOLD / 2))
		return -EINVAL;

	if (req->rate < DIV_THRESHOLD)
		parent = clk_hw_get_parent_by_index(hw, CBF_DIV_INDEX);
	else
		parent = clk_hw_get_parent_by_index(hw, CBF_PLL_INDEX);

	if (!parent)
		return -EINVAL;

	req->best_parent_rate = clk_hw_round_rate(parent, req->rate);
	req->best_parent_hw = parent;

	return 0;
}

static const struct clk_ops clk_cbf_8996_mux_ops = {
	.set_parent = clk_cbf_8996_mux_set_parent,
	.get_parent = clk_cbf_8996_mux_get_parent,
	.determine_rate = clk_cbf_8996_mux_determine_rate,
};

static struct clk_cbf_8996_mux cbf_mux = {
	.reg = CBF_MUX_OFFSET,
	.nb.notifier_call = cbf_clk_notifier_cb,
	.clkr.hw.init = &(struct clk_init_data) {
		.name = "cbf_mux",
		.parent_data = cbf_mux_parent_data,
		.num_parents = ARRAY_SIZE(cbf_mux_parent_data),
		.ops = &clk_cbf_8996_mux_ops,
		/* CPU clock is critical and should never be gated */
		.flags = CLK_SET_RATE_PARENT | CLK_IS_CRITICAL,
	},
};

static int cbf_clk_notifier_cb(struct notifier_block *nb, unsigned long event,
			       void *data)
{
	struct clk_notifier_data *cnd = data;

	switch (event) {
	case PRE_RATE_CHANGE:
		/*
		 * Avoid overvolting. clk_core_set_rate_nolock() walks from top
		 * to bottom, so it will change the rate of the PLL before
		 * chaging the parent of PMUX. This can result in pmux getting
		 * clocked twice the expected rate.
		 *
		 * Manually switch to PLL/2 here.
		 */
		if (cnd->old_rate > DIV_THRESHOLD &&
		    cnd->new_rate < DIV_THRESHOLD)
			clk_cbf_8996_mux_set_parent(&cbf_mux.clkr.hw, CBF_DIV_INDEX);
		break;
	case ABORT_RATE_CHANGE:
		/* Revert manual change */
		if (cnd->new_rate < DIV_THRESHOLD &&
		    cnd->old_rate > DIV_THRESHOLD)
			clk_cbf_8996_mux_set_parent(&cbf_mux.clkr.hw, CBF_PLL_INDEX);
		break;
	default:
		break;
	}

	return notifier_from_errno(0);
};

static struct clk_hw *cbf_msm8996_hw_clks[] = {
	&cbf_pll_postdiv.hw,
};

static struct clk_regmap *cbf_msm8996_clks[] = {
	&cbf_pll.clkr,
	&cbf_mux.clkr,
};

static const struct regmap_config cbf_msm8996_regmap_config = {
	.reg_bits		= 32,
	.reg_stride		= 4,
	.val_bits		= 32,
	.max_register		= 0x10000,
	.fast_io		= true,
	.val_format_endian	= REGMAP_ENDIAN_LITTLE,
};

static int qcom_msm8996_cbf_probe(struct platform_device *pdev)
{
	void __iomem *base;
	struct regmap *regmap;
	struct device *dev = &pdev->dev;
	int i, ret;

	base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(base))
		return PTR_ERR(base);

	regmap = devm_regmap_init_mmio(dev, base, &cbf_msm8996_regmap_config);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	/* Select GPLL0 for 300MHz for the CBF clock */
	regmap_write(regmap, CBF_MUX_OFFSET, 0x3);

	/* Ensure write goes through before PLLs are reconfigured */
	udelay(5);

	/* Set the auto clock sel always-on source to GPLL0/2 (300MHz) */
	regmap_update_bits(regmap, CBF_MUX_OFFSET,
			   CBF_MUX_AUTO_CLK_SEL_ALWAYS_ON_MASK,
			   CBF_MUX_AUTO_CLK_SEL_ALWAYS_ON_GPLL0_SEL);

	clk_alpha_pll_configure(&cbf_pll, regmap, &cbfpll_config);

	/* Wait for PLL(s) to lock */
	udelay(50);

	/* Enable auto clock selection for CBF */
	regmap_update_bits(regmap, CBF_MUX_OFFSET,
			   CBF_MUX_AUTO_CLK_SEL_BIT,
			   CBF_MUX_AUTO_CLK_SEL_BIT);

	/* Ensure write goes through before muxes are switched */
	udelay(5);

	/* Switch CBF to use the primary PLL */
	regmap_update_bits(regmap, CBF_MUX_OFFSET, CBF_MUX_PARENT_MASK, 0x1);

	for (i = 0; i < ARRAY_SIZE(cbf_msm8996_hw_clks); i++) {
		ret = devm_clk_hw_register(dev, cbf_msm8996_hw_clks[i]);
		if (ret)
			return ret;
	}

	for (i = 0; i < ARRAY_SIZE(cbf_msm8996_clks); i++) {
		ret = devm_clk_register_regmap(dev, cbf_msm8996_clks[i]);
		if (ret)
			return ret;
	}

	ret = devm_clk_notifier_register(dev, cbf_mux.clkr.hw.clk, &cbf_mux.nb);
	if (ret)
		return ret;

	return devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get, &cbf_mux.clkr.hw);
}

static const struct of_device_id qcom_msm8996_cbf_match_table[] = {
	{ .compatible = "qcom,msm8996-cbf" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, qcom_msm8996_cbf_match_table);

static struct platform_driver qcom_msm8996_cbf_driver = {
	.probe = qcom_msm8996_cbf_probe,
	.driver = {
		.name = "qcom-msm8996-cbf",
		.of_match_table = qcom_msm8996_cbf_match_table,
	},
};

/* Register early enough to fix the clock to be used for other cores */
static int __init qcom_msm8996_cbf_init(void)
{
	return platform_driver_register(&qcom_msm8996_cbf_driver);
}
postcore_initcall(qcom_msm8996_cbf_init);

static void __exit qcom_msm8996_cbf_exit(void)
{
	platform_driver_unregister(&qcom_msm8996_cbf_driver);
}
module_exit(qcom_msm8996_cbf_exit);

MODULE_DESCRIPTION("QCOM MSM8996 CPU Bus Fabric Clock Driver");
MODULE_LICENSE("GPL");
