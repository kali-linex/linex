// SPDX-License-Identifier: GPL-2.0
#include <linex/types.h>
#include <linex/errno.h>
#include <linex/uaccess.h>

int
fabs(u32 *frD, u32 *frB)
{
	frD[0] = frB[0] & 0x7fffffff;
	frD[1] = frB[1];

#ifdef DEBUG
	printk("%s: D %p, B %p: ", __func__, frD, frB);
	dump_double(frD);
	printk("\n");
#endif

	return 0;
}
