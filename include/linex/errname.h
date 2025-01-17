/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_ERRNAME_H
#define _LINEX_ERRNAME_H

#include <linex/stddef.h>

#ifdef CONFIG_SYMBOLIC_ERRNAME
const char *errname(int err);
#else
static inline const char *errname(int err)
{
	return NULL;
}
#endif

#endif /* _LINEX_ERRNAME_H */
