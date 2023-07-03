// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * OpenRISC or32_ksyms.c
 *
 * Linex architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 */

#include <linex/export.h>
#include <linex/elfcore.h>
#include <linex/sched.h>
#include <linex/in6.h>
#include <linex/interrupt.h>
#include <linex/vmalloc.h>
#include <linex/semaphore.h>
#include <linex/pgtable.h>

#include <asm/processor.h>
#include <linex/uaccess.h>
#include <asm/checksum.h>
#include <asm/io.h>
#include <asm/hardirq.h>
#include <asm/delay.h>

#define DECLARE_EXPORT(name) extern void name(void); EXPORT_SYMBOL(name)

/* compiler generated symbols */
DECLARE_EXPORT(__udivsi3);
DECLARE_EXPORT(__divsi3);
DECLARE_EXPORT(__umodsi3);
DECLARE_EXPORT(__modsi3);
DECLARE_EXPORT(__muldi3);
DECLARE_EXPORT(__ashrdi3);
DECLARE_EXPORT(__ashldi3);
DECLARE_EXPORT(__lshrdi3);
DECLARE_EXPORT(__ucmpdi2);

EXPORT_SYMBOL(empty_zero_page);
EXPORT_SYMBOL(__copy_tofrom_user);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(memset);
