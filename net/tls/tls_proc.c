// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
/* Copyright (C) 2019 Netronome Systems, Inc. */

#include <linex/proc_fs.h>
#include <linex/seq_file.h>
#include <net/snmp.h>
#include <net/tls.h>

#include "tls.h"

#ifdef CONFIG_PROC_FS
static const struct snmp_mib tls_mib_list[] = {
	SNMP_MIB_ITEM("TlsCurrTxSw", LINEX_MIB_TLSCURRTXSW),
	SNMP_MIB_ITEM("TlsCurrRxSw", LINEX_MIB_TLSCURRRXSW),
	SNMP_MIB_ITEM("TlsCurrTxDevice", LINEX_MIB_TLSCURRTXDEVICE),
	SNMP_MIB_ITEM("TlsCurrRxDevice", LINEX_MIB_TLSCURRRXDEVICE),
	SNMP_MIB_ITEM("TlsTxSw", LINEX_MIB_TLSTXSW),
	SNMP_MIB_ITEM("TlsRxSw", LINEX_MIB_TLSRXSW),
	SNMP_MIB_ITEM("TlsTxDevice", LINEX_MIB_TLSTXDEVICE),
	SNMP_MIB_ITEM("TlsRxDevice", LINEX_MIB_TLSRXDEVICE),
	SNMP_MIB_ITEM("TlsDecryptError", LINEX_MIB_TLSDECRYPTERROR),
	SNMP_MIB_ITEM("TlsRxDeviceResync", LINEX_MIB_TLSRXDEVICERESYNC),
	SNMP_MIB_ITEM("TlsDecryptRetry", LINEX_MIB_TLSDECRYPTRETRY),
	SNMP_MIB_ITEM("TlsRxNoPadViolation", LINEX_MIB_TLSRXNOPADVIOL),
	SNMP_MIB_SENTINEL
};

static int tls_statistics_seq_show(struct seq_file *seq, void *v)
{
	unsigned long buf[LINEX_MIB_TLSMAX] = {};
	struct net *net = seq->private;
	int i;

	snmp_get_cpu_field_batch(buf, tls_mib_list, net->mib.tls_statistics);
	for (i = 0; tls_mib_list[i].name; i++)
		seq_printf(seq, "%-32s\t%lu\n", tls_mib_list[i].name, buf[i]);

	return 0;
}
#endif

int __net_init tls_proc_init(struct net *net)
{
#ifdef CONFIG_PROC_FS
	if (!proc_create_net_single("tls_stat", 0444, net->proc_net,
				    tls_statistics_seq_show, NULL))
		return -ENOMEM;
#endif /* CONFIG_PROC_FS */

	return 0;
}

void __net_exit tls_proc_fini(struct net *net)
{
	remove_proc_entry("tls_stat", net->proc_net);
}
