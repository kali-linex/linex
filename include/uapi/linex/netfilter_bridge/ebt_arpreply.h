/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef __LINEX_BRIDGE_EBT_ARPREPLY_H
#define __LINEX_BRIDGE_EBT_ARPREPLY_H

#include <linex/if_ether.h>

struct ebt_arpreply_info {
	unsigned char mac[ETH_ALEN];
	int target;
};
#define EBT_ARPREPLY_TARGET "arpreply"

#endif
