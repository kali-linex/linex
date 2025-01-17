// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2003+ Evgeniy Polyakov <zbr@ioremap.net>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linex/module.h>
#include <linex/kernel.h>

#include <linex/capability.h>
#include <linex/if.h>
#include <linex/inetdevice.h>
#include <linex/ip.h>
#include <linex/list.h>
#include <linex/rculist.h>
#include <linex/skbuff.h>
#include <linex/slab.h>
#include <linex/tcp.h>

#include <net/ip.h>
#include <net/tcp.h>

#include <linex/netfilter/nfnetlink.h>
#include <linex/netfilter/x_tables.h>
#include <net/netfilter/nf_log.h>
#include <linex/netfilter/xt_osf.h>

static bool
xt_osf_match_packet(const struct sk_buff *skb, struct xt_action_param *p)
{
	return nf_osf_match(skb, xt_family(p), xt_hooknum(p), xt_in(p),
			    xt_out(p), p->matchinfo, xt_net(p), nf_osf_fingers);
}

static struct xt_match xt_osf_match = {
	.name 		= "osf",
	.revision	= 0,
	.family		= NFPROTO_IPV4,
	.proto		= IPPROTO_TCP,
	.hooks      	= (1 << NF_INET_LOCAL_IN) |
				(1 << NF_INET_PRE_ROUTING) |
				(1 << NF_INET_FORWARD),
	.match 		= xt_osf_match_packet,
	.matchsize	= sizeof(struct xt_osf_info),
	.me		= THIS_MODULE,
};

static int __init xt_osf_init(void)
{
	int err;

	err = xt_register_match(&xt_osf_match);
	if (err) {
		pr_err("Failed to register OS fingerprint "
		       "matching module (%d)\n", err);
		return err;
	}

	return 0;
}

static void __exit xt_osf_fini(void)
{
	xt_unregister_match(&xt_osf_match);
}

module_init(xt_osf_init);
module_exit(xt_osf_fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Polyakov <zbr@ioremap.net>");
MODULE_DESCRIPTION("Passive OS fingerprint matching.");
MODULE_ALIAS("ipt_osf");
MODULE_ALIAS("ip6t_osf");
