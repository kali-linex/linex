/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * linex/include/net/ethoc.h
 *
 * Copyright (C) 2008-2009 Avionic Design GmbH
 *
 * Written by Thierry Reding <thierry.reding@avionic-design.de>
 */

#ifndef LINEX_NET_ETHOC_H
#define LINEX_NET_ETHOC_H 1

#include <linex/if.h>
#include <linex/types.h>

struct ethoc_platform_data {
	u8 hwaddr[IFHWADDRLEN];
	s8 phy_id;
	u32 eth_clkfreq;
	bool big_endian;
};

#endif /* !LINEX_NET_ETHOC_H */
