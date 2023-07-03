/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_MSDOS_FS_H
#define _LINEX_MSDOS_FS_H

#include <uapi/linex/msdos_fs.h>

/* media of boot sector */
static inline int fat_valid_media(u8 media)
{
	return 0xf8 <= media || media == 0xf0;
}
#endif /* !_LINEX_MSDOS_FS_H */
