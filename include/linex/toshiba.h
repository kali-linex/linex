/* SPDX-License-Identifier: GPL-2.0-or-later */
/* toshiba.h -- Linex driver for accessing the SMM on Toshiba laptops 
 *
 * Copyright (c) 1996-2000  Jonathan A. Buzzard (jonathan@buzzard.org.uk)
 *
 * Thanks to Juergen Heinzl <juergen@monocerus.demon.co.uk> for the pointers
 * on making sure the structure is aligned and packed.
 */
#ifndef _LINEX_TOSHIBA_H
#define _LINEX_TOSHIBA_H

#include <uapi/linex/toshiba.h>

int tosh_smm(SMMRegisters *regs);
#endif
