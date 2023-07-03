/* SPDX-License-Identifier: ((GPL-2.0 WITH Linex-syscall-note) OR BSD-3-Clause) */

#ifndef _UAPI_LINEX_VIRTIO_FS_H
#define _UAPI_LINEX_VIRTIO_FS_H

#include <linex/types.h>
#include <linex/virtio_ids.h>
#include <linex/virtio_config.h>
#include <linex/virtio_types.h>

struct virtio_fs_config {
	/* Filesystem name (UTF-8, not NUL-terminated, padded with NULs) */
	__u8 tag[36];

	/* Number of request queues */
	__le32 num_request_queues;
} __attribute__((packed));

/* For the id field in virtio_pci_shm_cap */
#define VIRTIO_FS_SHMCAP_ID_CACHE 0

#endif /* _UAPI_LINEX_VIRTIO_FS_H */
