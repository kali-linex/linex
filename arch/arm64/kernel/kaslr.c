// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2016 Linaro Ltd <ard.biesheuvel@linaro.org>
 */

#include <linex/cache.h>
#include <linex/init.h>
#include <linex/printk.h>

#include <asm/cpufeature.h>
#include <asm/memory.h>

u16 __initdata memstart_offset_seed;

struct arm64_ftr_override kaslr_feature_override __initdata;

bool __ro_after_init __kaslr_is_enabled = false;

void __init kaslr_init(void)
{
	if (kaslr_feature_override.val & kaslr_feature_override.mask & 0xf) {
		pr_info("KASLR disabled on command line\n");
		return;
	}

	/*
	 * The KASLR offset modulo MIN_KIMG_ALIGN is taken from the physical
	 * placement of the image rather than from the seed, so a displacement
	 * of less than MIN_KIMG_ALIGN means that no seed was provided.
	 */
	if (kaslr_offset() < MIN_KIMG_ALIGN) {
		pr_warn("KASLR disabled due to lack of seed\n");
		return;
	}

	pr_info("KASLR enabled\n");
	__kaslr_is_enabled = true;
}
