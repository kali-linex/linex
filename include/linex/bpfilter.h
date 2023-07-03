/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_BPFILTER_H
#define _LINEX_BPFILTER_H

#include <uapi/linex/bpfilter.h>
#include <linex/usermode_driver.h>
#include <linex/sockptr.h>

struct sock;
int bpfilter_ip_set_sockopt(struct sock *sk, int optname, sockptr_t optval,
			    unsigned int optlen);
int bpfilter_ip_get_sockopt(struct sock *sk, int optname, char __user *optval,
			    int __user *optlen);

struct bpfilter_umh_ops {
	struct umd_info info;
	/* since ip_getsockopt() can run in parallel, serialize access to umh */
	struct mutex lock;
	int (*sockopt)(struct sock *sk, int optname, sockptr_t optval,
		       unsigned int optlen, bool is_set);
	int (*start)(void);
};
extern struct bpfilter_umh_ops bpfilter_ops;
#endif
