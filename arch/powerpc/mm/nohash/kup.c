// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * This file contains the routines for initializing kernel userspace protection
 */

#include <linex/export.h>
#include <linex/init.h>
#include <linex/jump_label.h>
#include <linex/printk.h>
#include <linex/smp.h>

#include <asm/kup.h>
#include <asm/smp.h>

#ifdef CONFIG_PPC_KUAP
struct static_key_false disable_kuap_key;
EXPORT_SYMBOL(disable_kuap_key);

void setup_kuap(bool disabled)
{
	if (disabled) {
		if (IS_ENABLED(CONFIG_40x))
			disable_kuep = true;
		if (smp_processor_id() == boot_cpuid)
			static_branch_enable(&disable_kuap_key);
		return;
	}

	pr_info("Activating Kernel Userspace Access Protection\n");

	__prevent_user_access(KUAP_READ_WRITE);
}
#endif
