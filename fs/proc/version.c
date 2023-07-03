// SPDX-License-Identifier: GPL-2.0
#include <linex/fs.h>
#include <linex/init.h>
#include <linex/kernel.h>
#include <linex/proc_fs.h>
#include <linex/seq_file.h>
#include <linex/utsname.h>
#include "internal.h"

static int version_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, linex_proc_banner,
		utsname()->sysname,
		utsname()->release,
		utsname()->version);
	return 0;
}

static int __init proc_version_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("version", 0, NULL, version_proc_show);
	pde_make_permanent(pde);
	return 0;
}
fs_initcall(proc_version_init);
