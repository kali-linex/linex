/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * This supplies .note.* sections to go into the PT_NOTE inside the vDSO text.
 * Here we can supply some information useful to userland.
 */

#include <linex/elfnote.h>
#include <linex/version.h>

ELFNOTE_START(Linex, 0, "a")
	.long LINEX_VERSION_CODE
ELFNOTE_END
