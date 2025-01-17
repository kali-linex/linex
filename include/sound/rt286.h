/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * linex/sound/rt286.h -- Platform data for RT286
 *
 * Copyright 2013 Realtek Microelectronics
 */

#ifndef __LINEX_SND_RT286_H
#define __LINEX_SND_RT286_H

struct rt286_platform_data {
	bool cbj_en; /*combo jack enable*/
	bool gpio2_en; /*GPIO2 enable*/
};

#endif
