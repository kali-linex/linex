// SPDX-License-Identifier: GPL-2.0
/*
 * WMI Thunderbolt driver
 *
 * Copyright (C) 2017 Dell Inc. All Rights Reserved.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linex/acpi.h>
#include <linex/device.h>
#include <linex/fs.h>
#include <linex/kernel.h>
#include <linex/module.h>
#include <linex/string.h>
#include <linex/sysfs.h>
#include <linex/types.h>
#include <linex/wmi.h>

#define INTEL_WMI_THUNDERBOLT_GUID "86CCFD48-205E-4A77-9C48-2021CBEDE341"

static ssize_t force_power_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct acpi_buffer input;
	acpi_status status;
	u8 mode;

	input.length = sizeof(u8);
	input.pointer = &mode;
	mode = hex_to_bin(buf[0]);
	dev_dbg(dev, "force_power: storing %#x\n", mode);
	if (mode == 0 || mode == 1) {
		status = wmi_evaluate_method(INTEL_WMI_THUNDERBOLT_GUID, 0, 1,
					     &input, NULL);
		if (ACPI_FAILURE(status)) {
			dev_dbg(dev, "force_power: failed to evaluate ACPI method\n");
			return -ENODEV;
		}
	} else {
		dev_dbg(dev, "force_power: unsupported mode\n");
		return -EINVAL;
	}
	return count;
}

static DEVICE_ATTR_WO(force_power);

static struct attribute *tbt_attrs[] = {
	&dev_attr_force_power.attr,
	NULL
};
ATTRIBUTE_GROUPS(tbt);

static const struct wmi_device_id intel_wmi_thunderbolt_id_table[] = {
	{ .guid_string = INTEL_WMI_THUNDERBOLT_GUID },
	{ },
};

static struct wmi_driver intel_wmi_thunderbolt_driver = {
	.driver = {
		.name = "intel-wmi-thunderbolt",
		.dev_groups = tbt_groups,
	},
	.id_table = intel_wmi_thunderbolt_id_table,
};

module_wmi_driver(intel_wmi_thunderbolt_driver);

MODULE_DEVICE_TABLE(wmi, intel_wmi_thunderbolt_id_table);
MODULE_AUTHOR("Mario Limonciello <mario.limonciello@dell.com>");
MODULE_DESCRIPTION("Intel WMI Thunderbolt force power driver");
MODULE_LICENSE("GPL v2");
