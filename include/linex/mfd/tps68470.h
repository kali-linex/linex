/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2017 Intel Corporation */
/* Functions to access TPS68470 power management chip. */

#ifndef __LINEX_MFD_TPS68470_H
#define __LINEX_MFD_TPS68470_H

/* Register addresses */
#define TPS68470_REG_POSTDIV2		0x06
#define TPS68470_REG_BOOSTDIV		0x07
#define TPS68470_REG_BUCKDIV		0x08
#define TPS68470_REG_PLLSWR		0x09
#define TPS68470_REG_XTALDIV		0x0A
#define TPS68470_REG_PLLDIV		0x0B
#define TPS68470_REG_POSTDIV		0x0C
#define TPS68470_REG_PLLCTL		0x0D
#define TPS68470_REG_PLLCTL2		0x0E
#define TPS68470_REG_CLKCFG1		0x0F
#define TPS68470_REG_CLKCFG2		0x10
#define TPS68470_REG_GPCTL0A		0x14
#define TPS68470_REG_GPCTL0B		0x15
#define TPS68470_REG_GPCTL1A		0x16
#define TPS68470_REG_GPCTL1B		0x17
#define TPS68470_REG_GPCTL2A		0x18
#define TPS68470_REG_GPCTL2B		0x19
#define TPS68470_REG_GPCTL3A		0x1A
#define TPS68470_REG_GPCTL3B		0x1B
#define TPS68470_REG_GPCTL4A		0x1C
#define TPS68470_REG_GPCTL4B		0x1D
#define TPS68470_REG_GPCTL5A		0x1E
#define TPS68470_REG_GPCTL5B		0x1F
#define TPS68470_REG_GPCTL6A		0x20
#define TPS68470_REG_GPCTL6B		0x21
#define TPS68470_REG_SGPO		0x22
#define TPS68470_REG_GPDI		0x26
#define TPS68470_REG_GPDO		0x27
#define TPS68470_REG_VCMVAL		0x3C
#define TPS68470_REG_VAUX1VAL		0x3D
#define TPS68470_REG_VAUX2VAL		0x3E
#define TPS68470_REG_VIOVAL		0x3F
#define TPS68470_REG_VSIOVAL		0x40
#define TPS68470_REG_VAVAL		0x41
#define TPS68470_REG_VDVAL		0x42
#define TPS68470_REG_S_I2C_CTL		0x43
#define TPS68470_REG_VCMCTL		0x44
#define TPS68470_REG_VAUX1CTL		0x45
#define TPS68470_REG_VAUX2CTL		0x46
#define TPS68470_REG_VACTL		0x47
#define TPS68470_REG_VDCTL		0x48
#define TPS68470_REG_RESET		0x50
#define TPS68470_REG_REVID		0xFF

#define TPS68470_REG_MAX		TPS68470_REG_REVID

/* Register field definitions */

#define TPS68470_REG_RESET_MASK		GENMASK(7, 0)
#define TPS68470_VAVAL_AVOLT_MASK	GENMASK(6, 0)

#define TPS68470_VDVAL_DVOLT_MASK	GENMASK(5, 0)
#define TPS68470_VCMVAL_VCVOLT_MASK	GENMASK(6, 0)
#define TPS68470_VIOVAL_IOVOLT_MASK	GENMASK(6, 0)
#define TPS68470_VSIOVAL_IOVOLT_MASK	GENMASK(6, 0)
#define TPS68470_VAUX1VAL_AUX1VOLT_MASK	GENMASK(6, 0)
#define TPS68470_VAUX2VAL_AUX2VOLT_MASK	GENMASK(6, 0)

#define TPS68470_VACTL_EN_MASK		GENMASK(0, 0)
#define TPS68470_VDCTL_EN_MASK		GENMASK(0, 0)
#define TPS68470_VCMCTL_EN_MASK		GENMASK(0, 0)
#define TPS68470_S_I2C_CTL_EN_MASK	GENMASK(1, 0)
#define TPS68470_VAUX1CTL_EN_MASK	GENMASK(0, 0)
#define TPS68470_VAUX2CTL_EN_MASK	GENMASK(0, 0)
#define TPS68470_PLL_EN_MASK		GENMASK(0, 0)

#define TPS68470_CLKCFG1_MODE_A_MASK	GENMASK(1, 0)
#define TPS68470_CLKCFG1_MODE_B_MASK	GENMASK(3, 2)

#define TPS68470_CLKCFG2_DRV_STR_2MA	0x05
#define TPS68470_PLL_OUTPUT_ENABLE	0x02
#define TPS68470_CLK_SRC_XTAL		BIT(0)
#define TPS68470_PLLSWR_DEFAULT		GENMASK(1, 0)
#define TPS68470_OSC_EXT_CAP_DEFAULT	0x05

#define TPS68470_OUTPUT_A_SHIFT		0x00
#define TPS68470_OUTPUT_B_SHIFT		0x02
#define TPS68470_CLK_SRC_SHIFT		GENMASK(2, 0)
#define TPS68470_OSC_EXT_CAP_SHIFT	BIT(2)

#define TPS68470_GPIO_CTL_REG_A(x)	(TPS68470_REG_GPCTL0A + (x) * 2)
#define TPS68470_GPIO_CTL_REG_B(x)	(TPS68470_REG_GPCTL0B + (x) * 2)
#define TPS68470_GPIO_MODE_MASK		GENMASK(1, 0)
#define TPS68470_GPIO_MODE_IN		0
#define TPS68470_GPIO_MODE_IN_PULLUP	1
#define TPS68470_GPIO_MODE_OUT_CMOS	2
#define TPS68470_GPIO_MODE_OUT_ODRAIN	3

#endif /* __LINEX_MFD_TPS68470_H */
