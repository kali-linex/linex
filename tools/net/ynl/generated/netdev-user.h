/* SPDX-License-Identifier: ((GPL-2.0 WITH Linex-syscall-note) OR BSD-3-Clause) */
/* Do not edit directly, auto-generated from: */
/*	Documentation/netlink/specs/netdev.yaml */
/* YNL-GEN user header */

#ifndef _LINEX_NETDEV_GEN_H
#define _LINEX_NETDEV_GEN_H

#include <stdlib.h>
#include <string.h>
#include <linex/types.h>
#include <linex/netdev.h>

struct ynl_sock;

extern const struct ynl_family ynl_netdev_family;

/* Enums */
const char *netdev_op_str(int op);
const char *netdev_xdp_act_str(enum netdev_xdp_act value);

/* Common nested types */
/* ============== NETDEV_CMD_DEV_GET ============== */
/* NETDEV_CMD_DEV_GET - do */
struct netdev_dev_get_req {
	struct {
		__u32 ifindex:1;
	} _present;

	__u32 ifindex;
};

static inline struct netdev_dev_get_req *netdev_dev_get_req_alloc(void)
{
	return calloc(1, sizeof(struct netdev_dev_get_req));
}
void netdev_dev_get_req_free(struct netdev_dev_get_req *req);

static inline void
netdev_dev_get_req_set_ifindex(struct netdev_dev_get_req *req, __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}

struct netdev_dev_get_rsp {
	struct {
		__u32 ifindex:1;
		__u32 xdp_features:1;
	} _present;

	__u32 ifindex;
	__u64 xdp_features;
};

void netdev_dev_get_rsp_free(struct netdev_dev_get_rsp *rsp);

/*
 * Get / dump information about a netdev.
 */
struct netdev_dev_get_rsp *
netdev_dev_get(struct ynl_sock *ys, struct netdev_dev_get_req *req);

/* NETDEV_CMD_DEV_GET - dump */
struct netdev_dev_get_list {
	struct netdev_dev_get_list *next;
	struct netdev_dev_get_rsp obj __attribute__ ((aligned (8)));
};

void netdev_dev_get_list_free(struct netdev_dev_get_list *rsp);

struct netdev_dev_get_list *netdev_dev_get_dump(struct ynl_sock *ys);

/* NETDEV_CMD_DEV_GET - notify */
struct netdev_dev_get_ntf {
	__u16 family;
	__u8 cmd;
	struct ynl_ntf_base_type *next;
	void (*free)(struct netdev_dev_get_ntf *ntf);
	struct netdev_dev_get_rsp obj __attribute__ ((aligned (8)));
};

void netdev_dev_get_ntf_free(struct netdev_dev_get_ntf *rsp);

#endif /* _LINEX_NETDEV_GEN_H */
