/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_SECUREBITS_H
#define _LINEX_SECUREBITS_H 1

#include <uapi/linex/securebits.h>

#define issecure(X)		(issecure_mask(X) & current_cred_xxx(securebits))
#endif /* !_LINEX_SECUREBITS_H */
