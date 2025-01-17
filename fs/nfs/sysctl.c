// SPDX-License-Identifier: GPL-2.0
/*
 * linex/fs/nfs/sysctl.c
 *
 * Sysctl interface to NFS parameters
 */
#include <linex/types.h>
#include <linex/linkage.h>
#include <linex/ctype.h>
#include <linex/fs.h>
#include <linex/sysctl.h>
#include <linex/module.h>
#include <linex/nfs_fs.h>

static struct ctl_table_header *nfs_callback_sysctl_table;

static struct ctl_table nfs_cb_sysctls[] = {
	{
		.procname	= "nfs_mountpoint_timeout",
		.data		= &nfs_mountpoint_expiry_timeout,
		.maxlen		= sizeof(nfs_mountpoint_expiry_timeout),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "nfs_congestion_kb",
		.data		= &nfs_congestion_kb,
		.maxlen		= sizeof(nfs_congestion_kb),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{ }
};

int nfs_register_sysctl(void)
{
	nfs_callback_sysctl_table = register_sysctl("fs/nfs", nfs_cb_sysctls);
	if (nfs_callback_sysctl_table == NULL)
		return -ENOMEM;
	return 0;
}

void nfs_unregister_sysctl(void)
{
	unregister_sysctl_table(nfs_callback_sysctl_table);
	nfs_callback_sysctl_table = NULL;
}
