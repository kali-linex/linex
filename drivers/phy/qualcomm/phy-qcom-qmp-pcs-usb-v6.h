/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022, The Linex Foundation. All rights reserved.
 */

#ifndef QCOM_PHY_QMP_PCS_USB_V6_H_
#define QCOM_PHY_QMP_PCS_USB_V6_H_

/* Only for QMP V6 PHY - USB3 have different offsets than V5 */
#define QPHY_USB_V6_PCS_LOCK_DETECT_CONFIG1		0xc4
#define QPHY_USB_V6_PCS_LOCK_DETECT_CONFIG2		0xc8
#define QPHY_USB_V6_PCS_LOCK_DETECT_CONFIG3		0xcc
#define QPHY_USB_V6_PCS_LOCK_DETECT_CONFIG6		0xd8
#define QPHY_USB_V6_PCS_REFGEN_REQ_CONFIG1		0xdc
#define QPHY_USB_V6_PCS_USB3_POWER_STATE_CONFIG1	0x90
#define QPHY_USB_V6_PCS_RX_SIGDET_LVL			0x188
#define QPHY_USB_V6_PCS_RCVR_DTCT_DLY_P1U2_L		0x190
#define QPHY_USB_V6_PCS_RCVR_DTCT_DLY_P1U2_H		0x194
#define QPHY_USB_V6_PCS_CDR_RESET_TIME			0x1b0
#define QPHY_USB_V6_PCS_ALIGN_DETECT_CONFIG1		0x1c0
#define QPHY_USB_V6_PCS_ALIGN_DETECT_CONFIG2		0x1c4
#define QPHY_USB_V6_PCS_PCS_TX_RX_CONFIG		0x1d0
#define QPHY_USB_V6_PCS_EQ_CONFIG1			0x1dc
#define QPHY_USB_V6_PCS_EQ_CONFIG5			0x1ec

#define QPHY_USB_V6_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL	0x18
#define QPHY_USB_V6_PCS_USB3_RXEQTRAINING_DFE_TIME_S2	0x3c
#define QPHY_USB_V6_PCS_USB3_RCVR_DTCT_DLY_U3_L		0x40
#define QPHY_USB_V6_PCS_USB3_RCVR_DTCT_DLY_U3_H		0x44

#endif
