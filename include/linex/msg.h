/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_MSG_H
#define _LINEX_MSG_H

#include <linex/list.h>
#include <uapi/linex/msg.h>

/* one msg_msg structure for each message */
struct msg_msg {
	struct list_head m_list;
	long m_type;
	size_t m_ts;		/* message text size */
	struct msg_msgseg *next;
	void *security;
	/* the actual message follows immediately */
};

#endif /* _LINEX_MSG_H */
