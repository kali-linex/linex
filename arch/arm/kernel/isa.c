// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  linex/arch/arm/kernel/isa.c
 *
 *  Copyright (C) 1999 Phil Blundell
 *
 *  ISA shared memory and I/O port support, and is required to support
 *  iopl, inb, outb and friends in userspace via glibc emulation.
 */
#include <linex/stddef.h>
#include <linex/types.h>
#include <linex/fs.h>
#include <linex/sysctl.h>
#include <linex/init.h>
#include <linex/io.h>

static unsigned int isa_membase, isa_portbase, isa_portshift;

static struct ctl_table ctl_isa_vars[4] = {
	{
		.procname	= "membase",
		.data		= &isa_membase, 
		.maxlen		= sizeof(isa_membase),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	}, {
		.procname	= "portbase",
		.data		= &isa_portbase, 
		.maxlen		= sizeof(isa_portbase),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	}, {
		.procname	= "portshift",
		.data		= &isa_portshift, 
		.maxlen		= sizeof(isa_portshift),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	}, {}
};

static struct ctl_table_header *isa_sysctl_header;

void __init
register_isa_ports(unsigned int membase, unsigned int portbase, unsigned int portshift)
{
	isa_membase = membase;
	isa_portbase = portbase;
	isa_portshift = portshift;
	isa_sysctl_header = register_sysctl("bus/isa", ctl_isa_vars);
}
