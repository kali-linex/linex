/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_GLOB_H
#define _LINEX_GLOB_H

#include <linex/types.h>	/* For bool */
#include <linex/compiler.h>	/* For __pure */

bool __pure glob_match(char const *pat, char const *str);

#endif	/* _LINEX_GLOB_H */
