// SPDX-License-Identifier: GPL-2.0
#include <linex/types.h>
#include <linex/errno.h>
#include <linex/uaccess.h>

#include <asm/sfp-machine.h>
#include <math-emu/soft-fp.h>

int
mffs(u32 *frD)
{
	frD[1] = __FPU_FPSCR;

#ifdef DEBUG
	printk("%s: frD %p: %08x.%08x\n", __func__, frD, frD[0], frD[1]);
#endif

	return 0;
}
