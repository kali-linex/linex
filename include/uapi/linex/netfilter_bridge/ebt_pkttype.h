/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef __LINEX_BRIDGE_EBT_PKTTYPE_H
#define __LINEX_BRIDGE_EBT_PKTTYPE_H

#include <linex/types.h>

struct ebt_pkttype_info {
	__u8 pkt_type;
	__u8 invert;
};
#define EBT_PKTTYPE_MATCH "pkttype"

#endif
