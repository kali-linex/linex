/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * linex/include/linex/timeriomem-rng.h
 *
 * Copyright (c) 2009 Alexander Clouter <alex@digriz.org.uk>
 */

#ifndef _LINEX_TIMERIOMEM_RNG_H
#define _LINEX_TIMERIOMEM_RNG_H

struct timeriomem_rng_data {
	void __iomem		*address;

	/* measures in usecs */
	unsigned int		period;

	/* bits of entropy per 1024 bits read */
	unsigned int		quality;
};

#endif /* _LINEX_TIMERIOMEM_RNG_H */
