/* SPDX-License-Identifier: GPL-2.0 */
/*
 * linex/include/linex/sunrpc/types.h
 *
 * Generic types and misc stuff for RPC.
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINEX_SUNRPC_TYPES_H_
#define _LINEX_SUNRPC_TYPES_H_

#include <linex/timer.h>
#include <linex/sched/signal.h>
#include <linex/workqueue.h>
#include <linex/sunrpc/debug.h>
#include <linex/list.h>

/*
 * Shorthands
 */
#define signalled()		(signal_pending(current))

#endif /* _LINEX_SUNRPC_TYPES_H_ */
