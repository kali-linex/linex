/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Platform Data for LTC4245 hardware monitor chip
 *
 * Copyright (c) 2010 Ira W. Snyder <iws@ovro.caltech.edu>
 */

#ifndef LINEX_LTC4245_H
#define LINEX_LTC4245_H

#include <linex/types.h>

struct ltc4245_platform_data {
	bool use_extra_gpios;
};

#endif /* LINEX_LTC4245_H */
