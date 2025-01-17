// SPDX-License-Identifier: GPL-2.0-only
/*
 * arch/arm/kernel/return_address.c
 *
 * Copyright (C) 2009 Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
 * for Pengutronix
 */
#include <linex/export.h>
#include <linex/ftrace.h>
#include <linex/sched.h>

#include <asm/stacktrace.h>

struct return_address_data {
	unsigned int level;
	void *addr;
};

static bool save_return_addr(void *d, unsigned long pc)
{
	struct return_address_data *data = d;

	if (!data->level) {
		data->addr = (void *)pc;

		return false;
	} else {
		--data->level;
		return true;
	}
}

void *return_address(unsigned int level)
{
	struct return_address_data data;
	struct stackframe frame;

	data.level = level + 2;
	data.addr = NULL;

	frame.fp = (unsigned long)__builtin_frame_address(0);
	frame.sp = current_stack_pointer;
	frame.lr = (unsigned long)__builtin_return_address(0);
here:
	frame.pc = (unsigned long)&&here;
#ifdef CONFIG_KRETPROBES
	frame.kr_cur = NULL;
	frame.tsk = current;
#endif
	frame.ex_frame = false;

	walk_stackframe(&frame, save_return_addr, &data);

	if (!data.level)
		return data.addr;
	else
		return NULL;
}

EXPORT_SYMBOL_GPL(return_address);
