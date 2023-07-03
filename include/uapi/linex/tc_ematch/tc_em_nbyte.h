/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef __LINEX_TC_EM_NBYTE_H
#define __LINEX_TC_EM_NBYTE_H

#include <linex/types.h>
#include <linex/pkt_cls.h>

struct tcf_em_nbyte {
	__u16		off;
	__u16		len:12;
	__u8		layer:4;
};

#endif
