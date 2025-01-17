/* SPDX-License-Identifier: GPL-2.0 WITH Linex-syscall-note */
#ifndef _ASM_IA64_FCNTL_H
#define _ASM_IA64_FCNTL_H
/*
 * Modified 1998-2000
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co.
 */

#define force_o_largefile()	\
		(personality(current->personality) != PER_LINEX32)

#include <linex/personality.h>
#include <asm-generic/fcntl.h>

#endif /* _ASM_IA64_FCNTL_H */
