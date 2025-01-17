/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SELINEX_POLICYCAP_NAMES_H_
#define _SELINEX_POLICYCAP_NAMES_H_

#include "policycap.h"

/* Policy capability names */
const char *const selinex_policycap_names[__POLICYDB_CAP_MAX] = {
	"network_peer_controls",
	"open_perms",
	"extended_socket_class",
	"always_check_network",
	"cgroup_seclabel",
	"nnp_nosuid_transition",
	"genfs_seclabel_symlinks",
	"ioctl_skip_cloexec"
};

#endif /* _SELINEX_POLICYCAP_NAMES_H_ */
