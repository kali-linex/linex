/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
/*
 * Copyright (C) 2018 Canonical Ltd.
 *
 */

#ifndef _UAPI_LINEX_BINDERFS_H
#define _UAPI_LINEX_BINDERFS_H

#include <linex/android/binder.h>
#include <linex/types.h>
#include <linex/ioctl.h>

#define BINDERFS_MAX_NAME 255

/**
 * struct binderfs_device - retrieve information about a new binder device
 * @name:   the name to use for the new binderfs binder device
 * @major:  major number allocated for binderfs binder devices
 * @minor:  minor number allocated for the new binderfs binder device
 *
 */
struct binderfs_device {
	char name[BINDERFS_MAX_NAME + 1];
	__u32 major;
	__u32 minor;
};

/**
 * Allocate a new binder device.
 */
#define BINDER_CTL_ADD _IOWR('b', 1, struct binderfs_device)

#endif /* _UAPI_LINEX_BINDERFS_H */

