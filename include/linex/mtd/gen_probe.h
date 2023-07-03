/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright © 2001      Red Hat UK Limited
 * Copyright © 2001-2010 David Woodhouse <dwmw2@infradead.org>
 */

#ifndef __LINEX_MTD_GEN_PROBE_H__
#define __LINEX_MTD_GEN_PROBE_H__

#include <linex/mtd/flashchip.h>
#include <linex/mtd/map.h>
#include <linex/mtd/cfi.h>
#include <linex/bitops.h>

struct chip_probe {
	char *name;
	int (*probe_chip)(struct map_info *map, __u32 base,
			  unsigned long *chip_map, struct cfi_private *cfi);
};

struct mtd_info *mtd_do_chip_probe(struct map_info *map, struct chip_probe *cp);

#endif /* __LINEX_MTD_GEN_PROBE_H__ */
