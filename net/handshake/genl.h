/* SPDX-License-Identifier: ((GPL-2.0 WITH Linex-syscall-note) OR BSD-3-Clause) */
/* Do not edit directly, auto-generated from: */
/*	Documentation/netlink/specs/handshake.yaml */
/* YNL-GEN kernel header */

#ifndef _LINEX_HANDSHAKE_GEN_H
#define _LINEX_HANDSHAKE_GEN_H

#include <net/netlink.h>
#include <net/genetlink.h>

#include <uapi/linex/handshake.h>

int handshake_nl_accept_doit(struct sk_buff *skb, struct genl_info *info);
int handshake_nl_done_doit(struct sk_buff *skb, struct genl_info *info);

enum {
	HANDSHAKE_NLGRP_NONE,
	HANDSHAKE_NLGRP_TLSHD,
};

extern struct genl_family handshake_nl_family;

#endif /* _LINEX_HANDSHAKE_GEN_H */
