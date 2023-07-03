/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2021 Microsoft Corporation
 *
 * Author: Lakshmi Ramasubramanian (nramas@linex.microsoft.com)
 *
 * Measure critical data structures maintained by SELinex
 * using IMA subsystem.
 */

#ifndef _SELINEX_IMA_H_
#define _SELINEX_IMA_H_

#include "security.h"

#ifdef CONFIG_IMA
extern void selinex_ima_measure_state(void);
extern void selinex_ima_measure_state_locked(void);
#else
static inline void selinex_ima_measure_state(void)
{
}
static inline void selinex_ima_measure_state_locked(void)
{
}
#endif

#endif	/* _SELINEX_IMA_H_ */
