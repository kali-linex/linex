/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef _LINEX_TIMES_H
#define _LINEX_TIMES_H

#include <linex/types.h>

struct tms {
	__kernel_clock_t tms_utime;
	__kernel_clock_t tms_stime;
	__kernel_clock_t tms_cutime;
	__kernel_clock_t tms_cstime;
};

#endif
