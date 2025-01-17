/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/*
 * Copyright (c) 2022, The Linex Foundation. All rights reserved.
 * Copyright (c) 2022, Linaro Limited
 */

#ifndef _DT_BINDINGS_CLK_QCOM_CAMCC_SM6350_H
#define _DT_BINDINGS_CLK_QCOM_CAMCC_SM6350_H

/* CAMCC clocks */
#define CAMCC_PLL2_OUT_EARLY		0
#define CAMCC_PLL0			1
#define CAMCC_PLL0_OUT_EVEN		2
#define CAMCC_PLL1			3
#define CAMCC_PLL1_OUT_EVEN		4
#define CAMCC_PLL2			5
#define CAMCC_PLL2_OUT_MAIN		6
#define CAMCC_PLL3			7
#define CAMCC_BPS_AHB_CLK		8
#define CAMCC_BPS_AREG_CLK		9
#define CAMCC_BPS_AXI_CLK		10
#define CAMCC_BPS_CLK			11
#define CAMCC_BPS_CLK_SRC		12
#define CAMCC_CAMNOC_ATB_CLK		13
#define CAMCC_CAMNOC_AXI_CLK		14
#define CAMCC_CCI_0_CLK		15
#define CAMCC_CCI_0_CLK_SRC		16
#define CAMCC_CCI_1_CLK		17
#define CAMCC_CCI_1_CLK_SRC		18
#define CAMCC_CORE_AHB_CLK		19
#define CAMCC_CPAS_AHB_CLK		20
#define CAMCC_CPHY_RX_CLK_SRC		21
#define CAMCC_CSI0PHYTIMER_CLK		22
#define CAMCC_CSI0PHYTIMER_CLK_SRC	23
#define CAMCC_CSI1PHYTIMER_CLK		24
#define CAMCC_CSI1PHYTIMER_CLK_SRC	25
#define CAMCC_CSI2PHYTIMER_CLK		26
#define CAMCC_CSI2PHYTIMER_CLK_SRC	27
#define CAMCC_CSI3PHYTIMER_CLK		28
#define CAMCC_CSI3PHYTIMER_CLK_SRC	29
#define CAMCC_CSIPHY0_CLK		30
#define CAMCC_CSIPHY1_CLK		31
#define CAMCC_CSIPHY2_CLK		32
#define CAMCC_CSIPHY3_CLK		33
#define CAMCC_FAST_AHB_CLK_SRC		34
#define CAMCC_ICP_APB_CLK		35
#define CAMCC_ICP_ATB_CLK		36
#define CAMCC_ICP_CLK			37
#define CAMCC_ICP_CLK_SRC		38
#define CAMCC_ICP_CTI_CLK		39
#define CAMCC_ICP_TS_CLK		40
#define CAMCC_IFE_0_AXI_CLK		41
#define CAMCC_IFE_0_CLK		42
#define CAMCC_IFE_0_CLK_SRC		43
#define CAMCC_IFE_0_CPHY_RX_CLK	44
#define CAMCC_IFE_0_CSID_CLK		45
#define CAMCC_IFE_0_CSID_CLK_SRC	46
#define CAMCC_IFE_0_DSP_CLK		47
#define CAMCC_IFE_1_AXI_CLK		48
#define CAMCC_IFE_1_CLK		49
#define CAMCC_IFE_1_CLK_SRC		50
#define CAMCC_IFE_1_CPHY_RX_CLK	51
#define CAMCC_IFE_1_CSID_CLK		52
#define CAMCC_IFE_1_CSID_CLK_SRC	53
#define CAMCC_IFE_1_DSP_CLK		54
#define CAMCC_IFE_2_AXI_CLK		55
#define CAMCC_IFE_2_CLK		56
#define CAMCC_IFE_2_CLK_SRC		57
#define CAMCC_IFE_2_CPHY_RX_CLK	58
#define CAMCC_IFE_2_CSID_CLK		59
#define CAMCC_IFE_2_CSID_CLK_SRC	60
#define CAMCC_IFE_2_DSP_CLK		61
#define CAMCC_IFE_LITE_CLK		62
#define CAMCC_IFE_LITE_CLK_SRC		63
#define CAMCC_IFE_LITE_CPHY_RX_CLK	64
#define CAMCC_IFE_LITE_CSID_CLK	65
#define CAMCC_IFE_LITE_CSID_CLK_SRC	66
#define CAMCC_IPE_0_AHB_CLK		67
#define CAMCC_IPE_0_AREG_CLK		68
#define CAMCC_IPE_0_AXI_CLK		69
#define CAMCC_IPE_0_CLK		70
#define CAMCC_IPE_0_CLK_SRC		71
#define CAMCC_JPEG_CLK			72
#define CAMCC_JPEG_CLK_SRC		73
#define CAMCC_LRME_CLK			74
#define CAMCC_LRME_CLK_SRC		75
#define CAMCC_MCLK0_CLK		76
#define CAMCC_MCLK0_CLK_SRC		77
#define CAMCC_MCLK1_CLK		78
#define CAMCC_MCLK1_CLK_SRC		79
#define CAMCC_MCLK2_CLK		80
#define CAMCC_MCLK2_CLK_SRC		81
#define CAMCC_MCLK3_CLK		82
#define CAMCC_MCLK3_CLK_SRC		83
#define CAMCC_MCLK4_CLK		84
#define CAMCC_MCLK4_CLK_SRC		85
#define CAMCC_SLOW_AHB_CLK_SRC		86
#define CAMCC_SOC_AHB_CLK		87
#define CAMCC_SYS_TMR_CLK		88

/* GDSCs */
#define BPS_GDSC			0
#define IPE_0_GDSC			1
#define IFE_0_GDSC			2
#define IFE_1_GDSC			3
#define IFE_2_GDSC			4
#define TITAN_TOP_GDSC			5

#endif
