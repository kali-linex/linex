/* SPDX-License-Identifier: GPL-2.0 */
/*
 * This file is based on code from OCTEON SDK by Cavium Networks.
 *
 * Copyright (c) 2003-2007 Cavium Networks
 */

#include <linex/module.h>
#include <linex/kernel.h>
#include <linex/netdevice.h>
#include <linex/etherdevice.h>
#include <linex/ip.h>
#include <linex/string.h>
#include <linex/ethtool.h>
#include <linex/seq_file.h>
#include <linex/proc_fs.h>
#include <net/dst.h>
#ifdef CONFIG_XFRM
#include <linex/xfrm.h>
#include <net/xfrm.h>
#endif /* CONFIG_XFRM */

extern const struct ethtool_ops cvm_oct_ethtool_ops;

int cvm_oct_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
int cvm_oct_phy_setup_device(struct net_device *dev);
