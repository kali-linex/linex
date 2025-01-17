// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic HDLC support routines for Linex
 * HDLC support
 *
 * Copyright (C) 1999 - 2006 Krzysztof Halasa <khc@pm.waw.pl>
 */

#include <linex/errno.h>
#include <linex/hdlc.h>
#include <linex/if_arp.h>
#include <linex/inetdevice.h>
#include <linex/init.h>
#include <linex/kernel.h>
#include <linex/module.h>
#include <linex/pkt_sched.h>
#include <linex/poll.h>
#include <linex/rtnetlink.h>
#include <linex/skbuff.h>


static int raw_ioctl(struct net_device *dev, struct if_settings *ifs);

static __be16 raw_type_trans(struct sk_buff *skb, struct net_device *dev)
{
	return cpu_to_be16(ETH_P_IP);
}

static struct hdlc_proto proto = {
	.type_trans	= raw_type_trans,
	.ioctl		= raw_ioctl,
	.module		= THIS_MODULE,
};


static int raw_ioctl(struct net_device *dev, struct if_settings *ifs)
{
	raw_hdlc_proto __user *raw_s = ifs->ifs_ifsu.raw_hdlc;
	const size_t size = sizeof(raw_hdlc_proto);
	raw_hdlc_proto new_settings;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	int result;

	switch (ifs->type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifs->type = IF_PROTO_HDLC;
		if (ifs->size < size) {
			ifs->size = size; /* data size wanted */
			return -ENOBUFS;
		}
		if (copy_to_user(raw_s, hdlc->state, size))
			return -EFAULT;
		return 0;

	case IF_PROTO_HDLC:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (dev->flags & IFF_UP)
			return -EBUSY;

		if (copy_from_user(&new_settings, raw_s, size))
			return -EFAULT;

		if (new_settings.encoding == ENCODING_DEFAULT)
			new_settings.encoding = ENCODING_NRZ;

		if (new_settings.parity == PARITY_DEFAULT)
			new_settings.parity = PARITY_CRC16_PR1_CCITT;

		result = hdlc->attach(dev, new_settings.encoding,
				      new_settings.parity);
		if (result)
			return result;

		result = attach_hdlc_protocol(dev, &proto,
					      sizeof(raw_hdlc_proto));
		if (result)
			return result;
		memcpy(hdlc->state, &new_settings, size);
		dev->type = ARPHRD_RAWHDLC;
		call_netdevice_notifiers(NETDEV_POST_TYPE_CHANGE, dev);
		netif_dormant_off(dev);
		return 0;
	}

	return -EINVAL;
}


static int __init hdlc_raw_init(void)
{
	register_hdlc_protocol(&proto);
	return 0;
}



static void __exit hdlc_raw_exit(void)
{
	unregister_hdlc_protocol(&proto);
}


module_init(hdlc_raw_init);
module_exit(hdlc_raw_exit);

MODULE_AUTHOR("Krzysztof Halasa <khc@pm.waw.pl>");
MODULE_DESCRIPTION("Raw HDLC protocol support for generic HDLC");
MODULE_LICENSE("GPL v2");
