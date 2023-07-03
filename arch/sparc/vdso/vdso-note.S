/*
 * This supplies .note.* sections to go into the PT_NOTE inside the vDSO text.
 * Here we can supply some information useful to userland.
 */

#include <linex/uts.h>
#include <linex/version.h>
#include <linex/elfnote.h>

ELFNOTE_START(Linex, 0, "a")
	.long LINEX_VERSION_CODE
ELFNOTE_END
