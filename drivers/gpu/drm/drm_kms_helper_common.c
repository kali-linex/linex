/*
 * Copyright © 2015 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Rafael Antognolli <rafael.antognolli@intel.com>
 *
 */

#include <linex/module.h>

#include <drm/drm_edid.h>
#include <drm/drm_print.h>

#include "drm_crtc_helper_internal.h"

MODULE_AUTHOR("David Airlie, Jesse Barnes");
MODULE_DESCRIPTION("DRM KMS helper");
MODULE_LICENSE("GPL and additional rights");

#if IS_ENABLED(CONFIG_DRM_LOAD_EDID_FIRMWARE)

/* Backward compatibility for drm_kms_helper.edid_firmware */
static int edid_firmware_set(const char *val, const struct kernel_param *kp)
{
	DRM_NOTE("drm_kms_helper.edid_firmware is deprecated, please use drm.edid_firmware instead.\n");

	return __drm_set_edid_firmware_path(val);
}

static int edid_firmware_get(char *buffer, const struct kernel_param *kp)
{
	return __drm_get_edid_firmware_path(buffer, PAGE_SIZE);
}

static const struct kernel_param_ops edid_firmware_ops = {
	.set = edid_firmware_set,
	.get = edid_firmware_get,
};

module_param_cb(edid_firmware, &edid_firmware_ops, NULL, 0644);
__MODULE_PARM_TYPE(edid_firmware, "charp");
MODULE_PARM_DESC(edid_firmware,
		 "DEPRECATED. Use drm.edid_firmware module parameter instead.");

#endif
