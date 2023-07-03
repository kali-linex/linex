/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Author: Huacai Chen <chenhuacai@loongson.cn>
 *
 * Copyright (C) 2020-2022 Loongson Technology Corporation Limited
 */

#include <asm/vdso/vdso.h>

#include <linex/elfnote.h>
#include <linex/version.h>

ELFNOTE_START(Linex, 0, "a")
	.long LINEX_VERSION_CODE
ELFNOTE_END
