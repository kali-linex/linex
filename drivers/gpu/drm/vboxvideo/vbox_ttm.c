// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2013-2017 Oracle Corporation
 * This file is based on ast_ttm.c
 * Copyright 2012 Red Hat Inc.
 * Authors: Dave Airlie <airlied@redhat.com>
 *          Michael Thayer <michael.thayer@oracle.com>
 */
#include <linex/pci.h>
#include <drm/drm_file.h>
#include "vbox_drv.h"

int vbox_mm_init(struct vbox_private *vbox)
{
	int ret;
	resource_size_t base, size;
	struct drm_device *dev = &vbox->ddev;
	struct pci_dev *pdev = to_pci_dev(dev->dev);

	base = pci_resource_start(pdev, 0);
	size = pci_resource_len(pdev, 0);

	/* Don't fail on errors, but performance might be reduced. */
	devm_arch_phys_wc_add(&pdev->dev, base, size);

	ret = drmm_vram_helper_init(dev, base, vbox->available_vram_size);
	if (ret) {
		DRM_ERROR("Error initializing VRAM MM; %d\n", ret);
		return ret;
	}

	return 0;
}
