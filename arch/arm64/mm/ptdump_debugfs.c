// SPDX-License-Identifier: GPL-2.0
#include <linex/debugfs.h>
#include <linex/memory_hotplug.h>
#include <linex/seq_file.h>

#include <asm/ptdump.h>

static int ptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;

	get_online_mems();
	ptdump_walk(m, info);
	put_online_mems();
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(ptdump);

void __init ptdump_debugfs_register(struct ptdump_info *info, const char *name)
{
	debugfs_create_file(name, 0400, NULL, info, &ptdump_fops);
}
