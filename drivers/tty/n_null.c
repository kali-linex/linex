// SPDX-License-Identifier: GPL-2.0
#include <linex/types.h>
#include <linex/errno.h>
#include <linex/tty.h>
#include <linex/module.h>

/*
 *  n_null.c - Null line discipline used in the failure path
 *
 *  Copyright (C) Intel 2017
 */

static int n_null_open(struct tty_struct *tty)
{
	return 0;
}

static void n_null_close(struct tty_struct *tty)
{
}

static ssize_t n_null_read(struct tty_struct *tty, struct file *file,
			   unsigned char *buf, size_t nr,
			   void **cookie, unsigned long offset)
{
	return -EOPNOTSUPP;
}

static ssize_t n_null_write(struct tty_struct *tty, struct file *file,
			    const unsigned char *buf, size_t nr)
{
	return -EOPNOTSUPP;
}

static void n_null_receivebuf(struct tty_struct *tty,
				 const unsigned char *cp, const char *fp,
				 int cnt)
{
}

static struct tty_ldisc_ops null_ldisc = {
	.owner		=	THIS_MODULE,
	.num		=	N_NULL,
	.name		=	"n_null",
	.open		=	n_null_open,
	.close		=	n_null_close,
	.read		=	n_null_read,
	.write		=	n_null_write,
	.receive_buf	=	n_null_receivebuf
};

static int __init n_null_init(void)
{
	BUG_ON(tty_register_ldisc(&null_ldisc));
	return 0;
}

static void __exit n_null_exit(void)
{
	tty_unregister_ldisc(&null_ldisc);
}

module_init(n_null_init);
module_exit(n_null_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alan Cox");
MODULE_ALIAS_LDISC(N_NULL);
MODULE_DESCRIPTION("Null ldisc driver");
