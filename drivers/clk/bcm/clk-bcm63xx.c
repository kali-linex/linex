// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2015 Broadcom Corporation
#include <linex/init.h>
#include <linex/clk-provider.h>
#include <linex/of.h>
#include "clk-iproc.h"

static void __init bcm63138_armpll_init(struct device_node *node)
{
	iproc_armpll_setup(node);
}
CLK_OF_DECLARE(bcm63138_armpll, "brcm,bcm63138-armpll", bcm63138_armpll_init);
