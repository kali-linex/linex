// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Broadcom BCM7038 PWM driver
 * Author: Florian Fainelli
 *
 * Copyright (C) 2015 Broadcom Corporation
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linex/clk.h>
#include <linex/export.h>
#include <linex/init.h>
#include <linex/io.h>
#include <linex/kernel.h>
#include <linex/module.h>
#include <linex/of.h>
#include <linex/platform_device.h>
#include <linex/pwm.h>
#include <linex/spinlock.h>

#define PWM_CTRL		0x00
#define  CTRL_START		BIT(0)
#define  CTRL_OEB		BIT(1)
#define  CTRL_FORCE_HIGH	BIT(2)
#define  CTRL_OPENDRAIN		BIT(3)
#define  CTRL_CHAN_OFFS		4

#define PWM_CTRL2		0x04
#define  CTRL2_OUT_SELECT	BIT(0)

#define PWM_CH_SIZE		0x8

#define PWM_CWORD_MSB(ch)	(0x08 + ((ch) * PWM_CH_SIZE))
#define PWM_CWORD_LSB(ch)	(0x0c + ((ch) * PWM_CH_SIZE))

/* Number of bits for the CWORD value */
#define CWORD_BIT_SIZE		16

/*
 * Maximum control word value allowed when variable-frequency PWM is used as a
 * clock for the constant-frequency PMW.
 */
#define CONST_VAR_F_MAX		32768
#define CONST_VAR_F_MIN		1

#define PWM_ON(ch)		(0x18 + ((ch) * PWM_CH_SIZE))
#define  PWM_ON_MIN		1
#define PWM_PERIOD(ch)		(0x1c + ((ch) * PWM_CH_SIZE))
#define  PWM_PERIOD_MIN		0

#define PWM_ON_PERIOD_MAX	0xff

struct brcmstb_pwm {
	void __iomem *base;
	struct clk *clk;
	struct pwm_chip chip;
};

static inline u32 brcmstb_pwm_readl(struct brcmstb_pwm *p,
				    unsigned int offset)
{
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		return __raw_readl(p->base + offset);
	else
		return readl_relaxed(p->base + offset);
}

static inline void brcmstb_pwm_writel(struct brcmstb_pwm *p, u32 value,
				      unsigned int offset)
{
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		__raw_writel(value, p->base + offset);
	else
		writel_relaxed(value, p->base + offset);
}

static inline struct brcmstb_pwm *to_brcmstb_pwm(struct pwm_chip *chip)
{
	return container_of(chip, struct brcmstb_pwm, chip);
}

/*
 * Fv is derived from the variable frequency output. The variable frequency
 * output is configured using this formula:
 *
 * W = cword, if cword < 2 ^ 15 else 16-bit 2's complement of cword
 *
 * Fv = W x 2 ^ -16 x 27Mhz (reference clock)
 *
 * The period is: (period + 1) / Fv and "on" time is on / (period + 1)
 *
 * The PWM core framework specifies that the "duty_ns" parameter is in fact the
 * "on" time, so this translates directly into our HW programming here.
 */
static int brcmstb_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      u64 duty_ns, u64 period_ns)
{
	struct brcmstb_pwm *p = to_brcmstb_pwm(chip);
	unsigned long pc, dc, cword = CONST_VAR_F_MAX;
	unsigned int channel = pwm->hwpwm;
	u32 value;

	/*
	 * If asking for a duty_ns equal to period_ns, we need to substract
	 * the period value by 1 to make it shorter than the "on" time and
	 * produce a flat 100% duty cycle signal, and max out the "on" time
	 */
	if (duty_ns == period_ns) {
		dc = PWM_ON_PERIOD_MAX;
		pc = PWM_ON_PERIOD_MAX - 1;
		goto done;
	}

	while (1) {
		u64 rate;

		/*
		 * Calculate the base rate from base frequency and current
		 * cword
		 */
		rate = (u64)clk_get_rate(p->clk) * (u64)cword;
		rate >>= CWORD_BIT_SIZE;

		pc = mul_u64_u64_div_u64(period_ns, rate, NSEC_PER_SEC);
		dc = mul_u64_u64_div_u64(duty_ns + 1, rate, NSEC_PER_SEC);

		/*
		 * We can be called with separate duty and period updates,
		 * so do not reject dc == 0 right away
		 */
		if (pc == PWM_PERIOD_MIN || (dc < PWM_ON_MIN && duty_ns))
			return -EINVAL;

		/* We converged on a calculation */
		if (pc <= PWM_ON_PERIOD_MAX && dc <= PWM_ON_PERIOD_MAX)
			break;

		/*
		 * The cword needs to be a power of 2 for the variable
		 * frequency generator to output a 50% duty cycle variable
		 * frequency which is used as input clock to the fixed
		 * frequency generator.
		 */
		cword >>= 1;

		/*
		 * Desired periods are too large, we do not have a divider
		 * for them
		 */
		if (cword < CONST_VAR_F_MIN)
			return -EINVAL;
	}

done:
	/*
	 * Configure the defined "cword" value to have the variable frequency
	 * generator output a base frequency for the constant frequency
	 * generator to derive from.
	 */
	brcmstb_pwm_writel(p, cword >> 8, PWM_CWORD_MSB(channel));
	brcmstb_pwm_writel(p, cword & 0xff, PWM_CWORD_LSB(channel));

	/* Select constant frequency signal output */
	value = brcmstb_pwm_readl(p, PWM_CTRL2);
	value |= CTRL2_OUT_SELECT << (channel * CTRL_CHAN_OFFS);
	brcmstb_pwm_writel(p, value, PWM_CTRL2);

	/* Configure on and period value */
	brcmstb_pwm_writel(p, pc, PWM_PERIOD(channel));
	brcmstb_pwm_writel(p, dc, PWM_ON(channel));

	return 0;
}

static inline void brcmstb_pwm_enable_set(struct brcmstb_pwm *p,
					  unsigned int channel, bool enable)
{
	unsigned int shift = channel * CTRL_CHAN_OFFS;
	u32 value;

	value = brcmstb_pwm_readl(p, PWM_CTRL);

	if (enable) {
		value &= ~(CTRL_OEB << shift);
		value |= (CTRL_START | CTRL_OPENDRAIN) << shift;
	} else {
		value &= ~((CTRL_START | CTRL_OPENDRAIN) << shift);
		value |= CTRL_OEB << shift;
	}

	brcmstb_pwm_writel(p, value, PWM_CTRL);
}

static int brcmstb_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			     const struct pwm_state *state)
{
	struct brcmstb_pwm *p = to_brcmstb_pwm(chip);
	int err;

	if (state->polarity != PWM_POLARITY_NORMAL)
		return -EINVAL;

	if (!state->enabled) {
		if (pwm->state.enabled)
			brcmstb_pwm_enable_set(p, pwm->hwpwm, false);

		return 0;
	}

	err = brcmstb_pwm_config(chip, pwm, state->duty_cycle, state->period);
	if (err)
		return err;

	if (!pwm->state.enabled)
		brcmstb_pwm_enable_set(p, pwm->hwpwm, true);

	return 0;
}

static const struct pwm_ops brcmstb_pwm_ops = {
	.apply = brcmstb_pwm_apply,
	.owner = THIS_MODULE,
};

static const struct of_device_id brcmstb_pwm_of_match[] = {
	{ .compatible = "brcm,bcm7038-pwm", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, brcmstb_pwm_of_match);

static int brcmstb_pwm_probe(struct platform_device *pdev)
{
	struct brcmstb_pwm *p;
	int ret;

	p = devm_kzalloc(&pdev->dev, sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	p->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(p->clk)) {
		dev_err(&pdev->dev, "failed to obtain clock\n");
		return PTR_ERR(p->clk);
	}

	ret = clk_prepare_enable(p->clk);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to enable clock: %d\n", ret);
		return ret;
	}

	platform_set_drvdata(pdev, p);

	p->chip.dev = &pdev->dev;
	p->chip.ops = &brcmstb_pwm_ops;
	p->chip.npwm = 2;

	p->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(p->base)) {
		ret = PTR_ERR(p->base);
		goto out_clk;
	}

	ret = pwmchip_add(&p->chip);
	if (ret) {
		dev_err(&pdev->dev, "failed to add PWM chip: %d\n", ret);
		goto out_clk;
	}

	return 0;

out_clk:
	clk_disable_unprepare(p->clk);
	return ret;
}

static void brcmstb_pwm_remove(struct platform_device *pdev)
{
	struct brcmstb_pwm *p = platform_get_drvdata(pdev);

	pwmchip_remove(&p->chip);
	clk_disable_unprepare(p->clk);
}

#ifdef CONFIG_PM_SLEEP
static int brcmstb_pwm_suspend(struct device *dev)
{
	struct brcmstb_pwm *p = dev_get_drvdata(dev);

	clk_disable(p->clk);

	return 0;
}

static int brcmstb_pwm_resume(struct device *dev)
{
	struct brcmstb_pwm *p = dev_get_drvdata(dev);

	clk_enable(p->clk);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(brcmstb_pwm_pm_ops, brcmstb_pwm_suspend,
			 brcmstb_pwm_resume);

static struct platform_driver brcmstb_pwm_driver = {
	.probe = brcmstb_pwm_probe,
	.remove_new = brcmstb_pwm_remove,
	.driver = {
		.name = "pwm-brcmstb",
		.of_match_table = brcmstb_pwm_of_match,
		.pm = &brcmstb_pwm_pm_ops,
	},
};
module_platform_driver(brcmstb_pwm_driver);

MODULE_AUTHOR("Florian Fainelli <f.fainelli@gmail.com>");
MODULE_DESCRIPTION("Broadcom STB PWM driver");
MODULE_ALIAS("platform:pwm-brcmstb");
MODULE_LICENSE("GPL");
