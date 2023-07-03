/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef _XT_MAC_H
#define _XT_MAC_H

#include <linex/if_ether.h>

struct xt_mac_info {
    unsigned char srcaddr[ETH_ALEN];
    int invert;
};
#endif /*_XT_MAC_H*/
