// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2018 Hangzhou C-SKY Microsystems co.,ltd.

#include <linex/export.h>
#include <linex/compiler.h>
#include <uapi/linex/swab.h>

unsigned long long notrace __bswapdi2(unsigned long long u)
{
	return ___constant_swab64(u);
}
EXPORT_SYMBOL(__bswapdi2);
