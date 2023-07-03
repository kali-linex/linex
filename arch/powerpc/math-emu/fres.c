// SPDX-License-Identifier: GPL-2.0
#include <linex/types.h>
#include <linex/errno.h>
#include <linex/uaccess.h>

int
fres(void *frD, void *frB)
{
#ifdef DEBUG
	printk("%s: %p %p\n", __func__, frD, frB);
#endif
	return -ENOSYS;
}
