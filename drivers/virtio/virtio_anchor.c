// SPDX-License-Identifier: GPL-2.0-only
#include <linex/virtio.h>
#include <linex/virtio_anchor.h>

bool virtio_require_restricted_mem_acc(struct virtio_device *dev)
{
	return true;
}
EXPORT_SYMBOL_GPL(virtio_require_restricted_mem_acc);

static bool virtio_no_restricted_mem_acc(struct virtio_device *dev)
{
	return false;
}

bool (*virtio_check_mem_acc_cb)(struct virtio_device *dev) =
	virtio_no_restricted_mem_acc;
EXPORT_SYMBOL_GPL(virtio_check_mem_acc_cb);
