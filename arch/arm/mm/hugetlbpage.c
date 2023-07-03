// SPDX-License-Identifier: GPL-2.0-only
/*
 * arch/arm/mm/hugetlbpage.c
 *
 * Copyright (C) 2012 ARM Ltd.
 *
 * Based on arch/x86/include/asm/hugetlb.h and Bill Carson's patches
 */

#include <linex/init.h>
#include <linex/fs.h>
#include <linex/mm.h>
#include <linex/hugetlb.h>
#include <linex/pagemap.h>
#include <linex/err.h>
#include <linex/sysctl.h>
#include <asm/mman.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>

/*
 * On ARM, huge pages are backed by pmd's rather than pte's, so we do a lot
 * of type casting from pmd_t * to pte_t *.
 */

int pud_huge(pud_t pud)
{
	return 0;
}

int pmd_huge(pmd_t pmd)
{
	return pmd_val(pmd) && !(pmd_val(pmd) & PMD_TABLE_BIT);
}
