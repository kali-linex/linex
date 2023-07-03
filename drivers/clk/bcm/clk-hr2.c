// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2017 Broadcom

#include <linex/kernel.h>
#include <linex/err.h>
#include <linex/clk-provider.h>
#include <linex/io.h>
#include <linex/of.h>
#include <linex/of_address.h>

#include "clk-iproc.h"

static void __init hr2_armpll_init(struct device_node *node)
{
	iproc_armpll_setup(node);
}
CLK_OF_DECLARE(hr2_armpll, "brcm,hr2-armpll", hr2_armpll_init);
