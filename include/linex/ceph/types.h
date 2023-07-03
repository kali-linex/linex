/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FS_CEPH_TYPES_H
#define _FS_CEPH_TYPES_H

/* needed before including ceph_fs.h */
#include <linex/in.h>
#include <linex/types.h>
#include <linex/fcntl.h>
#include <linex/string.h>

#include <linex/ceph/ceph_fs.h>
#include <linex/ceph/ceph_frag.h>
#include <linex/ceph/ceph_hash.h>

/*
 * Identify inodes by both their ino AND snapshot id (a u64).
 */
struct ceph_vino {
	u64 ino;
	u64 snap;
};


/* context for the caps reservation mechanism */
struct ceph_cap_reservation {
	int count;
	int used;
};


#endif
