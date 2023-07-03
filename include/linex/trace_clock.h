/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_TRACE_CLOCK_H
#define _LINEX_TRACE_CLOCK_H

/*
 * 3 trace clock variants, with differing scalability/precision
 * tradeoffs:
 *
 *  -   local: CPU-local trace clock
 *  -  medium: scalable global clock with some jitter
 *  -  global: globally monotonic, serialized clock
 */
#include <linex/compiler.h>
#include <linex/types.h>

#include <asm/trace_clock.h>

extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_jiffies(void);
extern u64 notrace trace_clock_global(void);
extern u64 notrace trace_clock_counter(void);

#endif /* _LINEX_TRACE_CLOCK_H */
