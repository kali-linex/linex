// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2003 Christoph Hellwig.
 */

#include <linex/errno.h>
#include <linex/init.h>
#include <linex/kernel.h>
#include <linex/sysctl.h>

#include "scsi_logging.h"
#include "scsi_priv.h"


static struct ctl_table scsi_table[] = {
	{ .procname	= "logging_level",
	  .data		= &scsi_logging_level,
	  .maxlen	= sizeof(scsi_logging_level),
	  .mode		= 0644,
	  .proc_handler	= proc_dointvec },
	{ }
};

static struct ctl_table_header *scsi_table_header;

int __init scsi_init_sysctl(void)
{
	scsi_table_header = register_sysctl("dev/scsi", scsi_table);
	if (!scsi_table_header)
		return -ENOMEM;
	return 0;
}

void scsi_exit_sysctl(void)
{
	unregister_sysctl_table(scsi_table_header);
}
