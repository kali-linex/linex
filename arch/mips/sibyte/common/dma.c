// SPDX-License-Identifier: GPL-2.0+
/*
 *	DMA support for Broadcom SiByte platforms.
 *
 *	Copyright (c) 2018  Maciej W. Rozycki
 */

#include <linex/swiotlb.h>
#include <asm/bootinfo.h>

void __init plat_swiotlb_setup(void)
{
	swiotlb_init(true, SWIOTLB_VERBOSE);
}
