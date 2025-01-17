// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)
/* Copyright(c) 2014 - 2020 Intel Corporation */
#include <linex/types.h>
#include <linex/mutex.h>
#include <linex/slab.h>
#include <linex/iopoll.h>
#include <linex/pci.h>
#include <linex/dma-mapping.h>
#include "adf_accel_devices.h"
#include "adf_common_drv.h"
#include "icp_qat_fw_init_admin.h"

#define ADF_ADMIN_MAILBOX_STRIDE 0x1000
#define ADF_ADMINMSG_LEN 32
#define ADF_CONST_TABLE_SIZE 1024
#define ADF_ADMIN_POLL_DELAY_US 20
#define ADF_ADMIN_POLL_TIMEOUT_US (5 * USEC_PER_SEC)

static const u8 const_tab[1024] __aligned(1024) = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x02, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,
0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76,
0x54, 0x32, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x45, 0x23, 0x01, 0xef, 0xcd, 0xab,
0x89, 0x98, 0xba, 0xdc, 0xfe, 0x10, 0x32, 0x54, 0x76, 0xc3, 0xd2, 0xe1, 0xf0,
0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x05, 0x9e,
0xd8, 0x36, 0x7c, 0xd5, 0x07, 0x30, 0x70, 0xdd, 0x17, 0xf7, 0x0e, 0x59, 0x39,
0xff, 0xc0, 0x0b, 0x31, 0x68, 0x58, 0x15, 0x11, 0x64, 0xf9, 0x8f, 0xa7, 0xbe,
0xfa, 0x4f, 0xa4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x09, 0xe6, 0x67, 0xbb, 0x67, 0xae,
0x85, 0x3c, 0x6e, 0xf3, 0x72, 0xa5, 0x4f, 0xf5, 0x3a, 0x51, 0x0e, 0x52, 0x7f,
0x9b, 0x05, 0x68, 0x8c, 0x1f, 0x83, 0xd9, 0xab, 0x5b, 0xe0, 0xcd, 0x19, 0x05,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xcb, 0xbb, 0x9d, 0x5d, 0xc1, 0x05, 0x9e, 0xd8, 0x62, 0x9a, 0x29,
0x2a, 0x36, 0x7c, 0xd5, 0x07, 0x91, 0x59, 0x01, 0x5a, 0x30, 0x70, 0xdd, 0x17,
0x15, 0x2f, 0xec, 0xd8, 0xf7, 0x0e, 0x59, 0x39, 0x67, 0x33, 0x26, 0x67, 0xff,
0xc0, 0x0b, 0x31, 0x8e, 0xb4, 0x4a, 0x87, 0x68, 0x58, 0x15, 0x11, 0xdb, 0x0c,
0x2e, 0x0d, 0x64, 0xf9, 0x8f, 0xa7, 0x47, 0xb5, 0x48, 0x1d, 0xbe, 0xfa, 0x4f,
0xa4, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x6a, 0x09, 0xe6, 0x67, 0xf3, 0xbc, 0xc9, 0x08, 0xbb,
0x67, 0xae, 0x85, 0x84, 0xca, 0xa7, 0x3b, 0x3c, 0x6e, 0xf3, 0x72, 0xfe, 0x94,
0xf8, 0x2b, 0xa5, 0x4f, 0xf5, 0x3a, 0x5f, 0x1d, 0x36, 0xf1, 0x51, 0x0e, 0x52,
0x7f, 0xad, 0xe6, 0x82, 0xd1, 0x9b, 0x05, 0x68, 0x8c, 0x2b, 0x3e, 0x6c, 0x1f,
0x1f, 0x83, 0xd9, 0xab, 0xfb, 0x41, 0xbd, 0x6b, 0x5b, 0xe0, 0xcd, 0x19, 0x13,
0x7e, 0x21, 0x79, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x15, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x02, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25,
0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x2B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

struct adf_admin_comms {
	dma_addr_t phy_addr;
	dma_addr_t const_tbl_addr;
	void *virt_addr;
	void *virt_tbl_addr;
	void __iomem *mailbox_addr;
	struct mutex lock;	/* protects adf_admin_comms struct */
};

static int adf_put_admin_msg_sync(struct adf_accel_dev *accel_dev, u32 ae,
				  void *in, void *out)
{
	int ret;
	u32 status;
	struct adf_admin_comms *admin = accel_dev->admin;
	int offset = ae * ADF_ADMINMSG_LEN * 2;
	void __iomem *mailbox = admin->mailbox_addr;
	int mb_offset = ae * ADF_ADMIN_MAILBOX_STRIDE;
	struct icp_qat_fw_init_admin_req *request = in;

	mutex_lock(&admin->lock);

	if (ADF_CSR_RD(mailbox, mb_offset) == 1) {
		mutex_unlock(&admin->lock);
		return -EAGAIN;
	}

	memcpy(admin->virt_addr + offset, in, ADF_ADMINMSG_LEN);
	ADF_CSR_WR(mailbox, mb_offset, 1);

	ret = read_poll_timeout(ADF_CSR_RD, status, status == 0,
				ADF_ADMIN_POLL_DELAY_US,
				ADF_ADMIN_POLL_TIMEOUT_US, true,
				mailbox, mb_offset);
	if (ret < 0) {
		/* Response timeout */
		dev_err(&GET_DEV(accel_dev),
			"Failed to send admin msg %d to accelerator %d\n",
			request->cmd_id, ae);
	} else {
		/* Response received from admin message, we can now
		 * make response data available in "out" parameter.
		 */
		memcpy(out, admin->virt_addr + offset +
		       ADF_ADMINMSG_LEN, ADF_ADMINMSG_LEN);
	}

	mutex_unlock(&admin->lock);
	return ret;
}

static int adf_send_admin(struct adf_accel_dev *accel_dev,
			  struct icp_qat_fw_init_admin_req *req,
			  struct icp_qat_fw_init_admin_resp *resp,
			  const unsigned long ae_mask)
{
	u32 ae;

	for_each_set_bit(ae, &ae_mask, ICP_QAT_HW_AE_DELIMITER)
		if (adf_put_admin_msg_sync(accel_dev, ae, req, resp) ||
		    resp->status)
			return -EFAULT;

	return 0;
}

static int adf_init_ae(struct adf_accel_dev *accel_dev)
{
	struct icp_qat_fw_init_admin_req req;
	struct icp_qat_fw_init_admin_resp resp;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	u32 ae_mask = hw_device->ae_mask;

	memset(&req, 0, sizeof(req));
	memset(&resp, 0, sizeof(resp));
	req.cmd_id = ICP_QAT_FW_INIT_AE;

	return adf_send_admin(accel_dev, &req, &resp, ae_mask);
}

static int adf_set_fw_constants(struct adf_accel_dev *accel_dev)
{
	struct icp_qat_fw_init_admin_req req;
	struct icp_qat_fw_init_admin_resp resp;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	u32 ae_mask = hw_device->admin_ae_mask ?: hw_device->ae_mask;

	memset(&req, 0, sizeof(req));
	memset(&resp, 0, sizeof(resp));
	req.cmd_id = ICP_QAT_FW_CONSTANTS_CFG;

	req.init_cfg_sz = ADF_CONST_TABLE_SIZE;
	req.init_cfg_ptr = accel_dev->admin->const_tbl_addr;

	return adf_send_admin(accel_dev, &req, &resp, ae_mask);
}

static int adf_get_dc_capabilities(struct adf_accel_dev *accel_dev,
				   u32 *capabilities)
{
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	struct icp_qat_fw_init_admin_resp resp;
	struct icp_qat_fw_init_admin_req req;
	unsigned long ae_mask;
	unsigned long ae;
	int ret;

	/* Target only service accelerator engines */
	ae_mask = hw_device->ae_mask & ~hw_device->admin_ae_mask;

	memset(&req, 0, sizeof(req));
	memset(&resp, 0, sizeof(resp));
	req.cmd_id = ICP_QAT_FW_COMP_CAPABILITY_GET;

	*capabilities = 0;
	for_each_set_bit(ae, &ae_mask, GET_MAX_ACCELENGINES(accel_dev)) {
		ret = adf_send_admin(accel_dev, &req, &resp, 1ULL << ae);
		if (ret)
			return ret;

		*capabilities |= resp.extended_features;
	}

	return 0;
}

/**
 * adf_send_admin_init() - Function sends init message to FW
 * @accel_dev: Pointer to acceleration device.
 *
 * Function sends admin init message to the FW
 *
 * Return: 0 on success, error code otherwise.
 */
int adf_send_admin_init(struct adf_accel_dev *accel_dev)
{
	u32 dc_capabilities = 0;
	int ret;

	ret = adf_get_dc_capabilities(accel_dev, &dc_capabilities);
	if (ret) {
		dev_err(&GET_DEV(accel_dev), "Cannot get dc capabilities\n");
		return ret;
	}
	accel_dev->hw_device->extended_dc_capabilities = dc_capabilities;

	ret = adf_set_fw_constants(accel_dev);
	if (ret)
		return ret;

	return adf_init_ae(accel_dev);
}
EXPORT_SYMBOL_GPL(adf_send_admin_init);

/**
 * adf_init_admin_pm() - Function sends PM init message to FW
 * @accel_dev: Pointer to acceleration device.
 * @idle_delay: QAT HW idle time before power gating is initiated.
 *		000 - 64us
 *		001 - 128us
 *		010 - 256us
 *		011 - 512us
 *		100 - 1ms
 *		101 - 2ms
 *		110 - 4ms
 *		111 - 8ms
 *
 * Function sends to the FW the admin init message for the PM state
 * configuration.
 *
 * Return: 0 on success, error code otherwise.
 */
int adf_init_admin_pm(struct adf_accel_dev *accel_dev, u32 idle_delay)
{
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	struct icp_qat_fw_init_admin_resp resp = {0};
	struct icp_qat_fw_init_admin_req req = {0};
	u32 ae_mask = hw_data->admin_ae_mask;

	if (!accel_dev->admin) {
		dev_err(&GET_DEV(accel_dev), "adf_admin is not available\n");
		return -EFAULT;
	}

	req.cmd_id = ICP_QAT_FW_PM_STATE_CONFIG;
	req.idle_filter = idle_delay;

	return adf_send_admin(accel_dev, &req, &resp, ae_mask);
}

int adf_init_admin_comms(struct adf_accel_dev *accel_dev)
{
	struct adf_admin_comms *admin;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	void __iomem *pmisc_addr = adf_get_pmisc_base(accel_dev);
	struct admin_info admin_csrs_info;
	u32 mailbox_offset, adminmsg_u, adminmsg_l;
	void __iomem *mailbox;
	u64 reg_val;

	admin = kzalloc_node(sizeof(*accel_dev->admin), GFP_KERNEL,
			     dev_to_node(&GET_DEV(accel_dev)));
	if (!admin)
		return -ENOMEM;
	admin->virt_addr = dma_alloc_coherent(&GET_DEV(accel_dev), PAGE_SIZE,
					      &admin->phy_addr, GFP_KERNEL);
	if (!admin->virt_addr) {
		dev_err(&GET_DEV(accel_dev), "Failed to allocate dma buff\n");
		kfree(admin);
		return -ENOMEM;
	}

	admin->virt_tbl_addr = dma_alloc_coherent(&GET_DEV(accel_dev),
						  PAGE_SIZE,
						  &admin->const_tbl_addr,
						  GFP_KERNEL);
	if (!admin->virt_tbl_addr) {
		dev_err(&GET_DEV(accel_dev), "Failed to allocate const_tbl\n");
		dma_free_coherent(&GET_DEV(accel_dev), PAGE_SIZE,
				  admin->virt_addr, admin->phy_addr);
		kfree(admin);
		return -ENOMEM;
	}

	memcpy(admin->virt_tbl_addr, const_tab, sizeof(const_tab));
	hw_data->get_admin_info(&admin_csrs_info);

	mailbox_offset = admin_csrs_info.mailbox_offset;
	mailbox = pmisc_addr + mailbox_offset;
	adminmsg_u = admin_csrs_info.admin_msg_ur;
	adminmsg_l = admin_csrs_info.admin_msg_lr;

	reg_val = (u64)admin->phy_addr;
	ADF_CSR_WR(pmisc_addr, adminmsg_u, upper_32_bits(reg_val));
	ADF_CSR_WR(pmisc_addr, adminmsg_l, lower_32_bits(reg_val));

	mutex_init(&admin->lock);
	admin->mailbox_addr = mailbox;
	accel_dev->admin = admin;
	return 0;
}
EXPORT_SYMBOL_GPL(adf_init_admin_comms);

void adf_exit_admin_comms(struct adf_accel_dev *accel_dev)
{
	struct adf_admin_comms *admin = accel_dev->admin;

	if (!admin)
		return;

	if (admin->virt_addr)
		dma_free_coherent(&GET_DEV(accel_dev), PAGE_SIZE,
				  admin->virt_addr, admin->phy_addr);
	if (admin->virt_tbl_addr)
		dma_free_coherent(&GET_DEV(accel_dev), PAGE_SIZE,
				  admin->virt_tbl_addr, admin->const_tbl_addr);

	mutex_destroy(&admin->lock);
	kfree(admin);
	accel_dev->admin = NULL;
}
EXPORT_SYMBOL_GPL(adf_exit_admin_comms);
