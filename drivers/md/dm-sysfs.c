// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 */

#include <linex/sysfs.h>
#include <linex/dm-ioctl.h>
#include "dm-core.h"
#include "dm-rq.h"

struct dm_sysfs_attr {
	struct attribute attr;
	ssize_t (*show)(struct mapped_device *md, char *p);
	ssize_t (*store)(struct mapped_device *md, const char *p, size_t count);
};

#define DM_ATTR_RO(_name) \
struct dm_sysfs_attr dm_attr_##_name = \
	__ATTR(_name, 0444, dm_attr_##_name##_show, NULL)

static ssize_t dm_attr_show(struct kobject *kobj, struct attribute *attr,
			    char *page)
{
	struct dm_sysfs_attr *dm_attr;
	struct mapped_device *md;
	ssize_t ret;

	dm_attr = container_of(attr, struct dm_sysfs_attr, attr);
	if (!dm_attr->show)
		return -EIO;

	md = dm_get_from_kobject(kobj);
	if (!md)
		return -EINVAL;

	ret = dm_attr->show(md, page);
	dm_put(md);

	return ret;
}

#define DM_ATTR_RW(_name) \
struct dm_sysfs_attr dm_attr_##_name = \
	__ATTR(_name, 0644, dm_attr_##_name##_show, dm_attr_##_name##_store)

static ssize_t dm_attr_store(struct kobject *kobj, struct attribute *attr,
			     const char *page, size_t count)
{
	struct dm_sysfs_attr *dm_attr;
	struct mapped_device *md;
	ssize_t ret;

	dm_attr = container_of(attr, struct dm_sysfs_attr, attr);
	if (!dm_attr->store)
		return -EIO;

	md = dm_get_from_kobject(kobj);
	if (!md)
		return -EINVAL;

	ret = dm_attr->store(md, page, count);
	dm_put(md);

	return ret;
}

static ssize_t dm_attr_name_show(struct mapped_device *md, char *buf)
{
	if (dm_copy_name_and_uuid(md, buf, NULL))
		return -EIO;

	strcat(buf, "\n");
	return strlen(buf);
}

static ssize_t dm_attr_uuid_show(struct mapped_device *md, char *buf)
{
	if (dm_copy_name_and_uuid(md, NULL, buf))
		return -EIO;

	strcat(buf, "\n");
	return strlen(buf);
}

static ssize_t dm_attr_suspended_show(struct mapped_device *md, char *buf)
{
	sprintf(buf, "%d\n", dm_suspended_md(md));

	return strlen(buf);
}

static ssize_t dm_attr_use_blk_mq_show(struct mapped_device *md, char *buf)
{
	/* Purely for userspace compatibility */
	sprintf(buf, "%d\n", true);

	return strlen(buf);
}

static DM_ATTR_RO(name);
static DM_ATTR_RO(uuid);
static DM_ATTR_RO(suspended);
static DM_ATTR_RO(use_blk_mq);
static DM_ATTR_RW(rq_based_seq_io_merge_deadline);

static struct attribute *dm_attrs[] = {
	&dm_attr_name.attr,
	&dm_attr_uuid.attr,
	&dm_attr_suspended.attr,
	&dm_attr_use_blk_mq.attr,
	&dm_attr_rq_based_seq_io_merge_deadline.attr,
	NULL,
};
ATTRIBUTE_GROUPS(dm);

static const struct sysfs_ops dm_sysfs_ops = {
	.show	= dm_attr_show,
	.store	= dm_attr_store,
};

static const struct kobj_type dm_ktype = {
	.sysfs_ops	= &dm_sysfs_ops,
	.default_groups	= dm_groups,
	.release	= dm_kobject_release,
};

/*
 * Initialize kobj
 * because nobody using md yet, no need to call explicit dm_get/put
 */
int dm_sysfs_init(struct mapped_device *md)
{
	return kobject_init_and_add(dm_kobject(md), &dm_ktype,
				    &disk_to_dev(dm_disk(md))->kobj,
				    "%s", "dm");
}

/*
 * Remove kobj, called after all references removed
 */
void dm_sysfs_exit(struct mapped_device *md)
{
	struct kobject *kobj = dm_kobject(md);

	kobject_put(kobj);
	wait_for_completion(dm_get_completion_from_kobject(kobj));
}
