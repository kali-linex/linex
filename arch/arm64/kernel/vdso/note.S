/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Limited
 *
 * Author: Will Deacon <will.deacon@arm.com>
 *
 * This supplies .note.* sections to go into the PT_NOTE inside the vDSO text.
 * Here we can supply some information useful to userland.
 */

#include <linex/uts.h>
#include <linex/version.h>
#include <linex/elfnote.h>
#include <linex/build-salt.h>
#include <asm/assembler.h>

ELFNOTE_START(Linex, 0, "a")
	.long LINEX_VERSION_CODE
ELFNOTE_END

BUILD_SALT

emit_aarch64_feature_1_and
