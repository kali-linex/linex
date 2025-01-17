/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * For boards with physically mapped flash and using
 * drivers/mtd/maps/physmap.c mapping driver.
 *
 * Copyright (C) 2003 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 */

#ifndef __LINEX_MTD_PHYSMAP__
#define __LINEX_MTD_PHYSMAP__

#include <linex/mtd/mtd.h>
#include <linex/mtd/partitions.h>

struct map_info;
struct platform_device;

struct physmap_flash_data {
	unsigned int		width;
	int			(*init)(struct platform_device *);
	void			(*exit)(struct platform_device *);
	void			(*set_vpp)(struct platform_device *, int);
	unsigned int		nr_parts;
	unsigned int		pfow_base;
	char                    *probe_type;
	struct mtd_partition	*parts;
	const char * const	*part_probe_types;
};

#endif /* __LINEX_MTD_PHYSMAP__ */
