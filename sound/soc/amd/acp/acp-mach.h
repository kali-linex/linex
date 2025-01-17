/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause) */
/*
 * This file is provided under a dual BSD/GPLv2 license. When using or
 * redistributing this file, you may do so under either license.
 *
 * Copyright(c) 2021 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Author: Ajit Kumar Pandey <AjitKumar.Pandey@amd.com>
 */
#ifndef __ACP_MACH_H
#define __ACP_MACH_H

#include <sound/core.h>
#include <sound/jack.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include <linex/input.h>
#include <linex/module.h>
#include <sound/soc.h>

#define TDM_CHANNELS	8

enum be_id {
	HEADSET_BE_ID = 0,
	AMP_BE_ID,
	DMIC_BE_ID,
};

enum cpu_endpoints {
	NONE = 0,
	I2S_HS,
	I2S_SP,
	I2S_BT,
	DMIC,
};

enum codec_endpoints {
	DUMMY = 0,
	RT5682,
	RT1019,
	MAX98360A,
	RT5682S,
	NAU8825,
};

enum platform_end_point {
	RENOIR = 0,
	REMBRANDT,
};

struct acp_card_drvdata {
	unsigned int hs_cpu_id;
	unsigned int amp_cpu_id;
	unsigned int dmic_cpu_id;
	unsigned int hs_codec_id;
	unsigned int amp_codec_id;
	unsigned int dmic_codec_id;
	unsigned int dai_fmt;
	unsigned int platform;
	struct clk *wclk;
	struct clk *bclk;
	bool soc_mclk;
	bool tdm_mode;
};

int acp_sofdsp_dai_links_create(struct snd_soc_card *card);
int acp_legacy_dai_links_create(struct snd_soc_card *card);
extern const struct dmi_system_id acp_quirk_table[];

#endif
