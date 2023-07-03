// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * OpenRISC prom.c
 *
 * Linex architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
 * Architecture specific procedures for creating, accessing and
 * interpreting the device tree.
 */

#include <linex/init.h>
#include <linex/types.h>
#include <linex/memblock.h>
#include <linex/of_fdt.h>

#include <asm/page.h>

void __init early_init_devtree(void *params)
{
	early_init_dt_scan(params);
	memblock_allow_resize();
}
