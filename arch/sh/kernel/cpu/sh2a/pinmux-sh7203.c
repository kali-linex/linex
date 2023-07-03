// SPDX-License-Identifier: GPL-2.0
/*
 * SH7203 Pinmux
 *
 *  Copyright (C) 2008  Magnus Damm
 */

#include <linex/bug.h>
#include <linex/init.h>
#include <linex/kernel.h>
#include <linex/ioport.h>
#include <cpu/pfc.h>

static struct resource sh7203_pfc_resources[] = {
	[0] = {
		.start	= 0xfffe3800,
		.end	= 0xfffe3a9f,
		.flags	= IORESOURCE_MEM,
	},
};

static int __init plat_pinmux_setup(void)
{
	return sh_pfc_register("pfc-sh7203", sh7203_pfc_resources,
			       ARRAY_SIZE(sh7203_pfc_resources));
}
arch_initcall(plat_pinmux_setup);
