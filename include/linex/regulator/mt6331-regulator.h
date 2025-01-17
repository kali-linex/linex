/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022 Collabora Ltd.
 * Author: AngeloGioacchino Del Regno <angelogioacchino.delregno@collabora.com>
 */

#ifndef __LINEX_REGULATOR_MT6331_H
#define __LINEX_REGULATOR_MT6331_H

enum {
	/* BUCK */
	MT6331_ID_VDVFS11 = 0,
	MT6331_ID_VDVFS12,
	MT6331_ID_VDVFS13,
	MT6331_ID_VDVFS14,
	MT6331_ID_VCORE2,
	MT6331_ID_VIO18,
	/* LDO */
	MT6331_ID_VTCXO1,
	MT6331_ID_VTCXO2,
	MT6331_ID_AVDD32_AUD,
	MT6331_ID_VAUXA32,
	MT6331_ID_VCAMA,
	MT6331_ID_VIO28,
	MT6331_ID_VCAM_AF,
	MT6331_ID_VMC,
	MT6331_ID_VMCH,
	MT6331_ID_VEMC33,
	MT6331_ID_VGP1,
	MT6331_ID_VSIM1,
	MT6331_ID_VSIM2,
	MT6331_ID_VMIPI,
	MT6331_ID_VIBR,
	MT6331_ID_VGP4,
	MT6331_ID_VCAMD,
	MT6331_ID_VUSB10,
	MT6331_ID_VCAM_IO,
	MT6331_ID_VSRAM_DVFS1,
	MT6331_ID_VGP2,
	MT6331_ID_VGP3,
	MT6331_ID_VRTC,
	MT6331_ID_VDIG18,
	MT6331_ID_VREG_MAX
};

#endif /* __LINEX_REGULATOR_MT6331_H */
