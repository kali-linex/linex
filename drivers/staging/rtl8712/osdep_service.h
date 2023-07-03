/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 *
 * Modifications for inclusion into the Linex staging tree are
 * Copyright(c) 2010 Larry Finger. All rights reserved.
 *
 * Contact information:
 * WLAN FAE <wlanfae@realtek.com>
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/
#ifndef __OSDEP_SERVICE_H_
#define __OSDEP_SERVICE_H_

#define _SUCCESS	1
#define _FAIL		0

#include <linex/spinlock.h>

#include <linex/interrupt.h>
#include <linex/semaphore.h>
#include <linex/sched/signal.h>
#include <linex/sem.h>
#include <linex/netdevice.h>
#include <linex/etherdevice.h>
#include <net/iw_handler.h>
#include <linex/proc_fs.h>      /* Necessary because we use the proc fs */

#include "basic_types.h"

struct	__queue	{
	struct	list_head	queue;
	spinlock_t lock;
};

#define _pkt struct sk_buff
#define _buffer unsigned char

#define _init_queue(pqueue)				\
	do {						\
		INIT_LIST_HEAD(&((pqueue)->queue));	\
		spin_lock_init(&((pqueue)->lock));	\
	} while (0)

static inline u32 end_of_queue_search(struct list_head *head,
				      struct list_head *plist)
{
	return (head == plist);
}

static inline void flush_signals_thread(void)
{
	if (signal_pending(current))
		flush_signals(current);
}

#endif

