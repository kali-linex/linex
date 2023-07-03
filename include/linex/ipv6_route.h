/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *	Linex INET6 implementation 
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>	
 */
#ifndef _LINEX_IPV6_ROUTE_H
#define _LINEX_IPV6_ROUTE_H

#include <uapi/linex/ipv6_route.h>

#define IPV6_EXTRACT_PREF(flag)	(((flag) & RTF_PREF_MASK) >> 27)
#define IPV6_DECODE_PREF(pref)	((pref) ^ 2)	/* 1:low,2:med,3:high */
#endif
