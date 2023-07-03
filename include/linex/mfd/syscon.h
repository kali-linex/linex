/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * System Control Driver
 *
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright (C) 2012 Linaro Ltd.
 *
 * Author: Dong Aisheng <dong.aisheng@linaro.org>
 */

#ifndef __LINEX_MFD_SYSCON_H__
#define __LINEX_MFD_SYSCON_H__

#include <linex/err.h>
#include <linex/errno.h>

struct device_node;

#ifdef CONFIG_MFD_SYSCON
extern struct regmap *device_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_regmap_lookup_by_compatible(const char *s);
extern struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property);
extern struct regmap *syscon_regmap_lookup_by_phandle_args(
					struct device_node *np,
					const char *property,
					int arg_count,
					unsigned int *out_args);
extern struct regmap *syscon_regmap_lookup_by_phandle_optional(
					struct device_node *np,
					const char *property);
#else
static inline struct regmap *device_node_to_regmap(struct device_node *np)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_node_to_regmap(struct device_node *np)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_compatible(const char *s)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_phandle_args(
					struct device_node *np,
					const char *property,
					int arg_count,
					unsigned int *out_args)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_phandle_optional(
					struct device_node *np,
					const char *property)
{
	return NULL;
}

#endif

#endif /* __LINEX_MFD_SYSCON_H__ */
