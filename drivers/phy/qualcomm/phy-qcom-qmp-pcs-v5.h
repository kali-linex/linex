/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2017, The Linex Foundation. All rights reserved.
 */

#ifndef QCOM_PHY_QMP_PCS_V5_H_
#define QCOM_PHY_QMP_PCS_V5_H_

/* Only for QMP V5 PHY - USB/PCIe PCS registers */
#define QPHY_V5_PCS_SW_RESET				0x000
#define QPHY_V5_PCS_PCS_STATUS1				0x014
#define QPHY_V5_PCS_POWER_DOWN_CONTROL			0x040
#define QPHY_V5_PCS_START_CONTROL			0x044
#define QPHY_V5_PCS_LOCK_DETECT_CONFIG1			0x0c4
#define QPHY_V5_PCS_LOCK_DETECT_CONFIG2			0x0c8
#define QPHY_V5_PCS_LOCK_DETECT_CONFIG3			0x0cc
#define QPHY_V5_PCS_LOCK_DETECT_CONFIG6			0x0d8
#define QPHY_V5_PCS_REFGEN_REQ_CONFIG1			0x0dc
#define QPHY_V5_PCS_G3S2_PRE_GAIN			0x170
#define QPHY_V5_PCS_RX_SIGDET_LVL			0x188
#define QPHY_V5_PCS_RCVR_DTCT_DLY_P1U2_L		0x190
#define QPHY_V5_PCS_RCVR_DTCT_DLY_P1U2_H		0x194
#define QPHY_V5_PCS_RATE_SLEW_CNTRL1			0x198
#define QPHY_V5_PCS_CDR_RESET_TIME			0x1b0
#define QPHY_V5_PCS_RX_CONFIG				0x1b0
#define QPHY_V5_PCS_ALIGN_DETECT_CONFIG1		0x1c0
#define QPHY_V5_PCS_ALIGN_DETECT_CONFIG2		0x1c4
#define QPHY_V5_PCS_PCS_TX_RX_CONFIG			0x1d0
#define QPHY_V5_PCS_EQ_CONFIG1				0x1dc
#define QPHY_V5_PCS_EQ_CONFIG2				0x1e0
#define QPHY_V5_PCS_EQ_CONFIG3				0x1e4
#define QPHY_V5_PCS_EQ_CONFIG5				0x1ec

#endif
