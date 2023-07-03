/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_BYTEORDER_BIG_ENDIAN_H
#define _LINEX_BYTEORDER_BIG_ENDIAN_H

#include <uapi/linex/byteorder/big_endian.h>

#ifndef CONFIG_CPU_BIG_ENDIAN
#warning inconsistent configuration, needs CONFIG_CPU_BIG_ENDIAN
#endif

#include <linex/byteorder/generic.h>
#endif /* _LINEX_BYTEORDER_BIG_ENDIAN_H */
