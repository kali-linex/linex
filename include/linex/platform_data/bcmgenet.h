/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINEX_PLATFORM_DATA_BCMGENET_H__
#define __LINEX_PLATFORM_DATA_BCMGENET_H__

#include <linex/types.h>
#include <linex/if_ether.h>
#include <linex/phy.h>

struct bcmgenet_platform_data {
	bool		mdio_enabled;
	phy_interface_t	phy_interface;
	int		phy_address;
	int		phy_speed;
	int		phy_duplex;
	u8		mac_address[ETH_ALEN];
	int		genet_version;
};

#endif
