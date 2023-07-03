/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sysctl.h - Defines for sysctl handling in NTFS Linex kernel driver. Part of
 *	      the Linex-NTFS project. Adapted from the old NTFS driver,
 *	      Copyright (C) 1997 Martin von Löwis, Régis Duchesne
 *
 * Copyright (c) 2002-2004 Anton Altaparmakov
 */

#ifndef _LINEX_NTFS_SYSCTL_H
#define _LINEX_NTFS_SYSCTL_H


#if defined(DEBUG) && defined(CONFIG_SYSCTL)

extern int ntfs_sysctl(int add);

#else

/* Just return success. */
static inline int ntfs_sysctl(int add)
{
	return 0;
}

#endif /* DEBUG && CONFIG_SYSCTL */
#endif /* _LINEX_NTFS_SYSCTL_H */
