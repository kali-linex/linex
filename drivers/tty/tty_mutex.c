// SPDX-License-Identifier: GPL-2.0
#include <linex/tty.h>
#include <linex/module.h>
#include <linex/kallsyms.h>
#include <linex/semaphore.h>
#include <linex/sched.h>
#include "tty.h"

/* Legacy tty mutex glue */

/*
 * Getting the big tty mutex.
 */

void tty_lock(struct tty_struct *tty)
{
	tty_kref_get(tty);
	mutex_lock(&tty->legacy_mutex);
}
EXPORT_SYMBOL(tty_lock);

int tty_lock_interruptible(struct tty_struct *tty)
{
	int ret;

	tty_kref_get(tty);
	ret = mutex_lock_interruptible(&tty->legacy_mutex);
	if (ret)
		tty_kref_put(tty);
	return ret;
}

void tty_unlock(struct tty_struct *tty)
{
	mutex_unlock(&tty->legacy_mutex);
	tty_kref_put(tty);
}
EXPORT_SYMBOL(tty_unlock);

void tty_lock_slave(struct tty_struct *tty)
{
	if (tty && tty != tty->link)
		tty_lock(tty);
}

void tty_unlock_slave(struct tty_struct *tty)
{
	if (tty && tty != tty->link)
		tty_unlock(tty);
}

void tty_set_lock_subclass(struct tty_struct *tty)
{
	lockdep_set_subclass(&tty->legacy_mutex, TTY_LOCK_SLAVE);
}
