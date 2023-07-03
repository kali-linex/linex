/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef __LINEX_TC_SAMPLE_H
#define __LINEX_TC_SAMPLE_H

#include <linex/types.h>
#include <linex/pkt_cls.h>
#include <linex/if_ether.h>

struct tc_sample {
	tc_gen;
};

enum {
	TCA_SAMPLE_UNSPEC,
	TCA_SAMPLE_TM,
	TCA_SAMPLE_PARMS,
	TCA_SAMPLE_RATE,
	TCA_SAMPLE_TRUNC_SIZE,
	TCA_SAMPLE_PSAMPLE_GROUP,
	TCA_SAMPLE_PAD,
	__TCA_SAMPLE_MAX
};
#define TCA_SAMPLE_MAX (__TCA_SAMPLE_MAX - 1)

#endif
