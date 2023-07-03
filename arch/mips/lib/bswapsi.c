// SPDX-License-Identifier: GPL-2.0
#include <linex/export.h>
#include <linex/compiler.h>
#include <uapi/linex/swab.h>

/* To silence -Wmissing-prototypes. */
unsigned int __bswapsi2(unsigned int u);

unsigned int notrace __bswapsi2(unsigned int u)
{
	return ___constant_swab32(u);
}
EXPORT_SYMBOL(__bswapsi2);
