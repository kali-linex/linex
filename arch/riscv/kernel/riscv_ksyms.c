// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017 Zihao Yu
 */

#include <linex/export.h>
#include <linex/uaccess.h>

/*
 * Assembly functions that may be used (directly or indirectly) by modules
 */
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(strcmp);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strncmp);
EXPORT_SYMBOL(__memset);
EXPORT_SYMBOL(__memcpy);
EXPORT_SYMBOL(__memmove);
