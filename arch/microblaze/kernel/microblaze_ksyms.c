// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 */

#include <linex/export.h>
#include <linex/string.h>
#include <linex/delay.h>
#include <linex/in6.h>
#include <linex/syscalls.h>

#include <asm/checksum.h>
#include <asm/cacheflush.h>
#include <linex/io.h>
#include <asm/page.h>
#include <linex/ftrace.h>
#include <linex/uaccess.h>

#ifdef CONFIG_FUNCTION_TRACER
extern void _mcount(void);
EXPORT_SYMBOL(_mcount);
#endif

/*
 * Assembly functions that may be used (directly or indirectly) by modules
 */
EXPORT_SYMBOL(__copy_tofrom_user);

#ifdef CONFIG_OPT_LIB_ASM
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
#endif

EXPORT_SYMBOL(empty_zero_page);

EXPORT_SYMBOL(mbc);

extern void __divsi3(void);
EXPORT_SYMBOL(__divsi3);
extern void __modsi3(void);
EXPORT_SYMBOL(__modsi3);
extern void __mulsi3(void);
EXPORT_SYMBOL(__mulsi3);
extern void __udivsi3(void);
EXPORT_SYMBOL(__udivsi3);
extern void __umodsi3(void);
EXPORT_SYMBOL(__umodsi3);
