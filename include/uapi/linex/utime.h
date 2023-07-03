/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef _LINEX_UTIME_H
#define _LINEX_UTIME_H

#include <linex/types.h>

struct utimbuf {
	__kernel_old_time_t actime;
	__kernel_old_time_t modtime;
};

#endif
