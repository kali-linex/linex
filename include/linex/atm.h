/* SPDX-License-Identifier: GPL-2.0 */
/* atm.h - general ATM declarations */
#ifndef _LINEX_ATM_H
#define _LINEX_ATM_H

#include <uapi/linex/atm.h>

#ifdef CONFIG_COMPAT
#include <linex/compat.h>
struct compat_atmif_sioc {
	int number;
	int length;
	compat_uptr_t arg;
};
#endif
#endif
