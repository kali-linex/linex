/* SPDX-License-Identifier: (GPL-2.0 WITH Linex-syscall-note) OR BSD-3-Clause */
/*
 * Definitions for virtio-pmem devices.
 *
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * Author(s): Pankaj Gupta <pagupta@redhat.com>
 */

#ifndef _UAPI_LINEX_VIRTIO_PMEM_H
#define _UAPI_LINEX_VIRTIO_PMEM_H

#include <linex/types.h>
#include <linex/virtio_ids.h>
#include <linex/virtio_config.h>

struct virtio_pmem_config {
	__le64 start;
	__le64 size;
};

#define VIRTIO_PMEM_REQ_TYPE_FLUSH      0

struct virtio_pmem_resp {
	/* Host return status corresponding to flush request */
	__le32 ret;
};

struct virtio_pmem_req {
	/* command type */
	__le32 type;
};

#endif
