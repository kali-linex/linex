/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Qualcomm #define SC7280 interconnect IDs
 *
 * Copyright (c) 2021, The Linex Foundation. All rights reserved.
 */

#ifndef __DRIVERS_INTERCONNECT_QCOM_SC7280_H
#define __DRIVERS_INTERCONNECT_QCOM_SC7280_H

#define SC7280_MASTER_GPU_TCU			0
#define SC7280_MASTER_SYS_TCU			1
#define SC7280_MASTER_APPSS_PROC			2
#define SC7280_MASTER_LLCC			3
#define SC7280_MASTER_CNOC_LPASS_AG_NOC			4
#define SC7280_MASTER_CDSP_NOC_CFG			5
#define SC7280_MASTER_QDSS_BAM			6
#define SC7280_MASTER_QSPI_0			7
#define SC7280_MASTER_QUP_0			8
#define SC7280_MASTER_QUP_1			9
#define SC7280_MASTER_A1NOC_CFG			10
#define SC7280_MASTER_A2NOC_CFG			11
#define SC7280_MASTER_A1NOC_SNOC			12
#define SC7280_MASTER_A2NOC_SNOC			13
#define SC7280_MASTER_COMPUTE_NOC			14
#define SC7280_MASTER_CNOC2_CNOC3			15
#define SC7280_MASTER_CNOC3_CNOC2			16
#define SC7280_MASTER_CNOC_A2NOC			17
#define SC7280_MASTER_CNOC_DC_NOC			18
#define SC7280_MASTER_GEM_NOC_CFG			19
#define SC7280_MASTER_GEM_NOC_CNOC			20
#define SC7280_MASTER_GEM_NOC_PCIE_SNOC			21
#define SC7280_MASTER_GFX3D			22
#define SC7280_MASTER_CNOC_MNOC_CFG			23
#define SC7280_MASTER_MNOC_HF_MEM_NOC			24
#define SC7280_MASTER_MNOC_SF_MEM_NOC			25
#define SC7280_MASTER_ANOC_PCIE_GEM_NOC			26
#define SC7280_MASTER_SNOC_CFG			27
#define SC7280_MASTER_SNOC_GC_MEM_NOC			28
#define SC7280_MASTER_SNOC_SF_MEM_NOC			29
#define SC7280_MASTER_VIDEO_P0			30
#define SC7280_MASTER_VIDEO_PROC			31
#define SC7280_MASTER_QUP_CORE_0			32
#define SC7280_MASTER_QUP_CORE_1			33
#define SC7280_MASTER_CAMNOC_HF			34
#define SC7280_MASTER_CAMNOC_ICP			35
#define SC7280_MASTER_CAMNOC_SF			36
#define SC7280_MASTER_CRYPTO			37
#define SC7280_MASTER_IPA			38
#define SC7280_MASTER_MDP0			39
#define SC7280_MASTER_CDSP_PROC			40
#define SC7280_MASTER_PIMEM			41
#define SC7280_MASTER_GIC			42
#define SC7280_MASTER_PCIE_0			43
#define SC7280_MASTER_PCIE_1			44
#define SC7280_MASTER_QDSS_DAP			45
#define SC7280_MASTER_QDSS_ETR			46
#define SC7280_MASTER_SDCC_1			47
#define SC7280_MASTER_SDCC_2			48
#define SC7280_MASTER_SDCC_4			49
#define SC7280_MASTER_UFS_MEM			50
#define SC7280_MASTER_USB2			51
#define SC7280_MASTER_USB3_0			52
#define SC7280_SLAVE_EBI1			53
#define SC7280_SLAVE_AHB2PHY_SOUTH			54
#define SC7280_SLAVE_AHB2PHY_NORTH			55
#define SC7280_SLAVE_AOSS			56
#define SC7280_SLAVE_APPSS			57
#define SC7280_SLAVE_CAMERA_CFG			58
#define SC7280_SLAVE_CLK_CTL			59
#define SC7280_SLAVE_CDSP_CFG			60
#define SC7280_SLAVE_RBCPR_CX_CFG			61
#define SC7280_SLAVE_RBCPR_MX_CFG			62
#define SC7280_SLAVE_CRYPTO_0_CFG			63
#define SC7280_SLAVE_CX_RDPM			64
#define SC7280_SLAVE_DCC_CFG			65
#define SC7280_SLAVE_DISPLAY_CFG			66
#define SC7280_SLAVE_GFX3D_CFG			67
#define SC7280_SLAVE_HWKM			68
#define SC7280_SLAVE_IMEM_CFG			69
#define SC7280_SLAVE_IPA_CFG			70
#define SC7280_SLAVE_IPC_ROUTER_CFG			71
#define SC7280_SLAVE_LLCC_CFG			72
#define SC7280_SLAVE_LPASS			73
#define SC7280_SLAVE_LPASS_CORE_CFG			74
#define SC7280_SLAVE_LPASS_LPI_CFG			75
#define SC7280_SLAVE_LPASS_MPU_CFG			76
#define SC7280_SLAVE_LPASS_TOP_CFG			77
#define SC7280_SLAVE_MSS_PROC_MS_MPU_CFG			78
#define SC7280_SLAVE_MCDMA_MS_MPU_CFG			79
#define SC7280_SLAVE_CNOC_MSS			80
#define SC7280_SLAVE_MX_RDPM			81
#define SC7280_SLAVE_PCIE_0_CFG			82
#define SC7280_SLAVE_PCIE_1_CFG			83
#define SC7280_SLAVE_PDM			84
#define SC7280_SLAVE_PIMEM_CFG			85
#define SC7280_SLAVE_PKA_WRAPPER_CFG			86
#define SC7280_SLAVE_PMU_WRAPPER_CFG			87
#define SC7280_SLAVE_QDSS_CFG			88
#define SC7280_SLAVE_QSPI_0			89
#define SC7280_SLAVE_QUP_0			90
#define SC7280_SLAVE_QUP_1			91
#define SC7280_SLAVE_SDCC_1			92
#define SC7280_SLAVE_SDCC_2			93
#define SC7280_SLAVE_SDCC_4			94
#define SC7280_SLAVE_SECURITY			95
#define SC7280_SLAVE_TCSR			96
#define SC7280_SLAVE_TLMM			97
#define SC7280_SLAVE_UFS_MEM_CFG			98
#define SC7280_SLAVE_USB2			99
#define SC7280_SLAVE_USB3_0			100
#define SC7280_SLAVE_VENUS_CFG			101
#define SC7280_SLAVE_VSENSE_CTRL_CFG			102
#define SC7280_SLAVE_A1NOC_CFG			103
#define SC7280_SLAVE_A1NOC_SNOC			104
#define SC7280_SLAVE_A2NOC_CFG			105
#define SC7280_SLAVE_A2NOC_SNOC			106
#define SC7280_SLAVE_CNOC2_CNOC3			107
#define SC7280_SLAVE_CNOC3_CNOC2			108
#define SC7280_SLAVE_CNOC_A2NOC			109
#define SC7280_SLAVE_DDRSS_CFG			110
#define SC7280_SLAVE_GEM_NOC_CNOC			111
#define SC7280_SLAVE_GEM_NOC_CFG			112
#define SC7280_SLAVE_SNOC_GEM_NOC_GC			113
#define SC7280_SLAVE_SNOC_GEM_NOC_SF			114
#define SC7280_SLAVE_LLCC			115
#define SC7280_SLAVE_MNOC_HF_MEM_NOC			116
#define SC7280_SLAVE_MNOC_SF_MEM_NOC			117
#define SC7280_SLAVE_CNOC_MNOC_CFG			118
#define SC7280_SLAVE_CDSP_MEM_NOC			119
#define SC7280_SLAVE_MEM_NOC_PCIE_SNOC			120
#define SC7280_SLAVE_ANOC_PCIE_GEM_NOC			121
#define SC7280_SLAVE_SNOC_CFG			122
#define SC7280_SLAVE_QUP_CORE_0			123
#define SC7280_SLAVE_QUP_CORE_1			124
#define SC7280_SLAVE_BOOT_IMEM			125
#define SC7280_SLAVE_IMEM			126
#define SC7280_SLAVE_PIMEM			127
#define SC7280_SLAVE_SERVICE_NSP_NOC			128
#define SC7280_SLAVE_SERVICE_A1NOC			129
#define SC7280_SLAVE_SERVICE_A2NOC			130
#define SC7280_SLAVE_SERVICE_GEM_NOC_1			131
#define SC7280_SLAVE_SERVICE_MNOC			132
#define SC7280_SLAVE_SERVICES_LPASS_AML_NOC			133
#define SC7280_SLAVE_SERVICE_LPASS_AG_NOC			134
#define SC7280_SLAVE_SERVICE_GEM_NOC_2			135
#define SC7280_SLAVE_SERVICE_SNOC			136
#define SC7280_SLAVE_SERVICE_GEM_NOC			137
#define SC7280_SLAVE_PCIE_0			138
#define SC7280_SLAVE_PCIE_1			139
#define SC7280_SLAVE_QDSS_STM			140
#define SC7280_SLAVE_TCU			141

#endif
