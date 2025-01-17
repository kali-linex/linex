// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *	Dong Aisheng <aisheng.dong@nxp.com>
 */

#include <linex/err.h>
#include <linex/firmware/imx/sci.h>
#include <linex/module.h>
#include <linex/of_address.h>
#include <linex/pinctrl/pinctrl.h>
#include <linex/platform_device.h>

#include "../core.h"
#include "pinctrl-imx.h"

#define IMX_SC_PAD_FUNC_GET_WAKEUP	9
#define IMX_SC_PAD_FUNC_SET_WAKEUP	4
#define IMX_SC_IRQ_GROUP_WAKE           3   /* Wakeup interrupts */
#define IMX_SC_IRQ_PAD			2   /* Pad wakeup */

enum pad_func_e {
	IMX_SC_PAD_FUNC_SET = 15,
	IMX_SC_PAD_FUNC_GET = 16,
};

struct imx_sc_msg_req_pad_set {
	struct imx_sc_rpc_msg hdr;
	u32 val;
	u16 pad;
} __packed __aligned(4);

struct imx_sc_msg_req_pad_get {
	struct imx_sc_rpc_msg hdr;
	u16 pad;
} __packed __aligned(4);

struct imx_sc_msg_resp_pad_get {
	struct imx_sc_rpc_msg hdr;
	u32 val;
} __packed;

struct imx_sc_msg_gpio_set_pad_wakeup {
	struct imx_sc_rpc_msg hdr;
	u16 pad;
	u8 wakeup;
} __packed __aligned(4);

static struct imx_sc_ipc *pinctrl_ipc_handle;

int imx_pinctrl_sc_ipc_init(struct platform_device *pdev)
{
	imx_scu_irq_group_enable(IMX_SC_IRQ_GROUP_WAKE,
					 IMX_SC_IRQ_PAD, true);
	return imx_scu_get_handle(&pinctrl_ipc_handle);
}
EXPORT_SYMBOL_GPL(imx_pinctrl_sc_ipc_init);

int imx_pinconf_get_scu(struct pinctrl_dev *pctldev, unsigned pin_id,
			unsigned long *config)
{
	struct imx_sc_msg_req_pad_get msg;
	struct imx_sc_msg_resp_pad_get *resp;
	struct imx_sc_rpc_msg *hdr = &msg.hdr;
	int ret;

	hdr->ver = IMX_SC_RPC_VERSION;
	hdr->svc = IMX_SC_RPC_SVC_PAD;
	hdr->func = IMX_SC_PAD_FUNC_GET;
	hdr->size = 2;

	msg.pad = pin_id;

	ret = imx_scu_call_rpc(pinctrl_ipc_handle, &msg, true);
	if (ret)
		return ret;

	resp = (struct imx_sc_msg_resp_pad_get *)&msg;
	*config = resp->val;

	return 0;
}
EXPORT_SYMBOL_GPL(imx_pinconf_get_scu);

int imx_pinconf_set_scu(struct pinctrl_dev *pctldev, unsigned pin_id,
			unsigned long *configs, unsigned num_configs)
{
	struct imx_pinctrl *ipctl = pinctrl_dev_get_drvdata(pctldev);
	struct imx_sc_msg_req_pad_set msg;
	struct imx_sc_rpc_msg *hdr = &msg.hdr;
	unsigned int mux = configs[0];
	unsigned int conf;
	unsigned int val;
	int ret;

	if (num_configs == 1) {
		struct imx_sc_msg_gpio_set_pad_wakeup wmsg;

		hdr = &wmsg.hdr;
		hdr->ver = IMX_SC_RPC_VERSION;
		hdr->svc = IMX_SC_RPC_SVC_PAD;
		hdr->func = IMX_SC_PAD_FUNC_SET_WAKEUP;
		hdr->size = 2;
		wmsg.pad = pin_id;
		wmsg.wakeup = *configs;
		ret = imx_scu_call_rpc(pinctrl_ipc_handle, &wmsg, true);

		dev_dbg(ipctl->dev, "wakeup pin_id: %d type: %ld\n",
				pin_id, *configs);
		return ret;
	}

	/*
	 * Set mux and conf together in one IPC call
	 */
	WARN_ON(num_configs != 2);
	conf = configs[1];

	val = conf | BM_PAD_CTL_IFMUX_ENABLE | BM_PAD_CTL_GP_ENABLE;
	val |= mux << BP_PAD_CTL_IFMUX;

	hdr->ver = IMX_SC_RPC_VERSION;
	hdr->svc = IMX_SC_RPC_SVC_PAD;
	hdr->func = IMX_SC_PAD_FUNC_SET;
	hdr->size = 3;

	msg.pad = pin_id;
	msg.val = val;

	ret = imx_scu_call_rpc(pinctrl_ipc_handle, &msg, true);

	dev_dbg(ipctl->dev, "write: pin_id %u config 0x%x val 0x%x\n",
		pin_id, conf, val);

	return ret;
}
EXPORT_SYMBOL_GPL(imx_pinconf_set_scu);

void imx_pinctrl_parse_pin_scu(struct imx_pinctrl *ipctl,
			       unsigned int *pin_id, struct imx_pin *pin,
			       const __be32 **list_p)
{
	const struct imx_pinctrl_soc_info *info = ipctl->info;
	struct imx_pin_scu *pin_scu = &pin->conf.scu;
	const __be32 *list = *list_p;

	pin->pin = be32_to_cpu(*list++);
	*pin_id = pin->pin;
	pin_scu->mux_mode = be32_to_cpu(*list++);
	pin_scu->config = be32_to_cpu(*list++);
	*list_p = list;

	dev_dbg(ipctl->dev, "%s: 0x%x 0x%08lx", info->pins[pin->pin].name,
		pin_scu->mux_mode, pin_scu->config);
}
EXPORT_SYMBOL_GPL(imx_pinctrl_parse_pin_scu);

MODULE_AUTHOR("Dong Aisheng <aisheng.dong@nxp.com>");
MODULE_DESCRIPTION("NXP i.MX SCU common pinctrl driver");
MODULE_LICENSE("GPL v2");
