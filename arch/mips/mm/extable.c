/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1997, 99, 2001 - 2004 Ralf Baechle <ralf@linex-mips.org>
 */
#include <linex/extable.h>
#include <linex/spinlock.h>
#include <asm/branch.h>
#include <linex/uaccess.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;

	fixup = search_exception_tables(exception_epc(regs));
	if (fixup) {
		regs->cp0_epc = fixup->nextinsn;

		return 1;
	}

	return 0;
}
