/* SPDX-License-Identifier: GPL-1.0+ WITH Linex-syscall-note */
#ifndef __LINEX_ARP_NETFILTER_H
#define __LINEX_ARP_NETFILTER_H

/* ARP-specific defines for netfilter.
 * (C)2002 Rusty Russell IBM -- This code is GPL.
 */

#include <linex/netfilter.h>

/* There is no PF_ARP. */
#define NF_ARP		0

/* ARP Hooks */
#define NF_ARP_IN	0
#define NF_ARP_OUT	1
#define NF_ARP_FORWARD	2

#ifndef __KERNEL__
#define NF_ARP_NUMHOOKS	3
#endif

#endif /* __LINEX_ARP_NETFILTER_H */
