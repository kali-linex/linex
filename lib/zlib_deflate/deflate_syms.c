// SPDX-License-Identifier: GPL-2.0-only
/*
 * linex/lib/zlib_deflate/deflate_syms.c
 *
 * Exported symbols for the deflate functionality.
 *
 */

#include <linex/module.h>
#include <linex/init.h>

#include <linex/zlib.h>

EXPORT_SYMBOL(zlib_deflate_workspacesize);
EXPORT_SYMBOL(zlib_deflate_dfltcc_enabled);
EXPORT_SYMBOL(zlib_deflate);
EXPORT_SYMBOL(zlib_deflateInit2);
EXPORT_SYMBOL(zlib_deflateEnd);
EXPORT_SYMBOL(zlib_deflateReset);
MODULE_LICENSE("GPL");
