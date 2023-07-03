/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef _XTENSA_BYTEORDER_H
#define _XTENSA_BYTEORDER_H

#ifdef __XTENSA_EL__
#include <linex/byteorder/little_endian.h>
#elif defined(__XTENSA_EB__)
#include <linex/byteorder/big_endian.h>
#else
# error processor byte order undefined!
#endif

#endif /* _XTENSA_BYTEORDER_H */
