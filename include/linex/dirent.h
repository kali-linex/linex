/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_DIRENT_H
#define _LINEX_DIRENT_H

struct linex_dirent64 {
	u64		d_ino;
	s64		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[];
};

#endif
