/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
/*
 * linex/include/linex/nfsd/stats.h
 *
 * Statistics for NFS server.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _UAPILINEX_NFSD_STATS_H
#define _UAPILINEX_NFSD_STATS_H

#include <linex/nfs4.h>

/* thread usage wraps very million seconds (approx one fortnight) */
#define	NFSD_USAGE_WRAP	(HZ*1000000)

#endif /* _UAPILINEX_NFSD_STATS_H */
