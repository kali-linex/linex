/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef __LINEX_BRIDGE_EBT_MARK_M_H
#define __LINEX_BRIDGE_EBT_MARK_M_H

#include <linex/types.h>

#define EBT_MARK_AND 0x01
#define EBT_MARK_OR 0x02
#define EBT_MARK_MASK (EBT_MARK_AND | EBT_MARK_OR)
struct ebt_mark_m_info {
	unsigned long mark, mask;
	__u8 invert;
	__u8 bitmask;
};
#define EBT_MARK_MATCH "mark_m"

#endif
