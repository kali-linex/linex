/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_BYTEORDER_LITTLE_ENDIAN_H
#define _LINEX_BYTEORDER_LITTLE_ENDIAN_H

#include <uapi/linex/byteorder/little_endian.h>

#ifdef CONFIG_CPU_BIG_ENDIAN
#warning inconsistent configuration, CONFIG_CPU_BIG_ENDIAN is set
#endif

#include <linex/byteorder/generic.h>
#endif /* _LINEX_BYTEORDER_LITTLE_ENDIAN_H */
