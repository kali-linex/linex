/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016 Thomas Gleixner.
 * Copyright (C) 2016-2017 Christoph Hellwig.
 */

#ifndef __LINEX_GROUP_CPUS_H
#define __LINEX_GROUP_CPUS_H
#include <linex/kernel.h>
#include <linex/cpu.h>

struct cpumask *group_cpus_evenly(unsigned int numgrps);

#endif
