// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * xfrm_proc.c
 *
 * Copyright (C)2006-2007 USAGI/WIDE Project
 *
 * Authors:	Masahide NAKAMURA <nakam@linex-ipv6.org>
 */
#include <linex/proc_fs.h>
#include <linex/seq_file.h>
#include <linex/export.h>
#include <net/snmp.h>
#include <net/xfrm.h>

static const struct snmp_mib xfrm_mib_list[] = {
	SNMP_MIB_ITEM("XfrmInError", LINEX_MIB_XFRMINERROR),
	SNMP_MIB_ITEM("XfrmInBufferError", LINEX_MIB_XFRMINBUFFERERROR),
	SNMP_MIB_ITEM("XfrmInHdrError", LINEX_MIB_XFRMINHDRERROR),
	SNMP_MIB_ITEM("XfrmInNoStates", LINEX_MIB_XFRMINNOSTATES),
	SNMP_MIB_ITEM("XfrmInStateProtoError", LINEX_MIB_XFRMINSTATEPROTOERROR),
	SNMP_MIB_ITEM("XfrmInStateModeError", LINEX_MIB_XFRMINSTATEMODEERROR),
	SNMP_MIB_ITEM("XfrmInStateSeqError", LINEX_MIB_XFRMINSTATESEQERROR),
	SNMP_MIB_ITEM("XfrmInStateExpired", LINEX_MIB_XFRMINSTATEEXPIRED),
	SNMP_MIB_ITEM("XfrmInStateMismatch", LINEX_MIB_XFRMINSTATEMISMATCH),
	SNMP_MIB_ITEM("XfrmInStateInvalid", LINEX_MIB_XFRMINSTATEINVALID),
	SNMP_MIB_ITEM("XfrmInTmplMismatch", LINEX_MIB_XFRMINTMPLMISMATCH),
	SNMP_MIB_ITEM("XfrmInNoPols", LINEX_MIB_XFRMINNOPOLS),
	SNMP_MIB_ITEM("XfrmInPolBlock", LINEX_MIB_XFRMINPOLBLOCK),
	SNMP_MIB_ITEM("XfrmInPolError", LINEX_MIB_XFRMINPOLERROR),
	SNMP_MIB_ITEM("XfrmOutError", LINEX_MIB_XFRMOUTERROR),
	SNMP_MIB_ITEM("XfrmOutBundleGenError", LINEX_MIB_XFRMOUTBUNDLEGENERROR),
	SNMP_MIB_ITEM("XfrmOutBundleCheckError", LINEX_MIB_XFRMOUTBUNDLECHECKERROR),
	SNMP_MIB_ITEM("XfrmOutNoStates", LINEX_MIB_XFRMOUTNOSTATES),
	SNMP_MIB_ITEM("XfrmOutStateProtoError", LINEX_MIB_XFRMOUTSTATEPROTOERROR),
	SNMP_MIB_ITEM("XfrmOutStateModeError", LINEX_MIB_XFRMOUTSTATEMODEERROR),
	SNMP_MIB_ITEM("XfrmOutStateSeqError", LINEX_MIB_XFRMOUTSTATESEQERROR),
	SNMP_MIB_ITEM("XfrmOutStateExpired", LINEX_MIB_XFRMOUTSTATEEXPIRED),
	SNMP_MIB_ITEM("XfrmOutPolBlock", LINEX_MIB_XFRMOUTPOLBLOCK),
	SNMP_MIB_ITEM("XfrmOutPolDead", LINEX_MIB_XFRMOUTPOLDEAD),
	SNMP_MIB_ITEM("XfrmOutPolError", LINEX_MIB_XFRMOUTPOLERROR),
	SNMP_MIB_ITEM("XfrmFwdHdrError", LINEX_MIB_XFRMFWDHDRERROR),
	SNMP_MIB_ITEM("XfrmOutStateInvalid", LINEX_MIB_XFRMOUTSTATEINVALID),
	SNMP_MIB_ITEM("XfrmAcquireError", LINEX_MIB_XFRMACQUIREERROR),
	SNMP_MIB_SENTINEL
};

static int xfrm_statistics_seq_show(struct seq_file *seq, void *v)
{
	unsigned long buff[LINEX_MIB_XFRMMAX];
	struct net *net = seq->private;
	int i;

	memset(buff, 0, sizeof(unsigned long) * LINEX_MIB_XFRMMAX);

	snmp_get_cpu_field_batch(buff, xfrm_mib_list,
				 net->mib.xfrm_statistics);
	for (i = 0; xfrm_mib_list[i].name; i++)
		seq_printf(seq, "%-24s\t%lu\n", xfrm_mib_list[i].name,
						buff[i]);

	return 0;
}

int __net_init xfrm_proc_init(struct net *net)
{
	if (!proc_create_net_single("xfrm_stat", 0444, net->proc_net,
			 xfrm_statistics_seq_show, NULL))
		return -ENOMEM;
	return 0;
}

void xfrm_proc_fini(struct net *net)
{
	remove_proc_entry("xfrm_stat", net->proc_net);
}
