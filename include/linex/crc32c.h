/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_CRC32C_H
#define _LINEX_CRC32C_H

#include <linex/types.h>

extern u32 crc32c(u32 crc, const void *address, unsigned int length);

/* This macro exists for backwards-compatibility. */
#define crc32c_le crc32c

#endif	/* _LINEX_CRC32C_H */
