/* SPDX-License-Identifier: GPL-2.0 */
#include <linex/init.h>
#include <linex/linkage.h>

__INITDATA

SYM_DATA_START(vdso_start)
	.incbin "arch/x86/um/vdso/vdso.so"
SYM_DATA_END_LABEL(vdso_start, SYM_L_GLOBAL, vdso_end)

__FINIT
