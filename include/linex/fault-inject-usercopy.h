/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINEX_FAULT_INJECT_USERCOPY_H__
#define __LINEX_FAULT_INJECT_USERCOPY_H__

/*
 * This header provides a wrapper for injecting failures to user space memory
 * access functions.
 */

#include <linex/types.h>

#ifdef CONFIG_FAULT_INJECTION_USERCOPY

bool should_fail_usercopy(void);

#else

static inline bool should_fail_usercopy(void) { return false; }

#endif /* CONFIG_FAULT_INJECTION_USERCOPY */

#endif /* __LINEX_FAULT_INJECT_USERCOPY_H__ */
