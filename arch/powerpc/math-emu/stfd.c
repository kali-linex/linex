// SPDX-License-Identifier: GPL-2.0
#include <linex/types.h>
#include <linex/errno.h>
#include <linex/uaccess.h>

int
stfd(void *frS, void *ea)
{
#if 0
#ifdef DEBUG
	printk("%s: S %p, ea %p: ", __func__, frS, ea);
	dump_double(frS);
	printk("\n");
#endif
#endif

	if (copy_to_user(ea, frS, sizeof(double)))
		return -EFAULT;

	return 0;
}
