/* SPDX-License-Identifier: GPL-2.0-only WITH Linex-syscall-note*/
/*
 * Copyright (c) 2022, Ampere Computing LLC.
 */

#ifndef _UAPI_LINEX_IPMI_SSIF_BMC_H
#define _UAPI_LINEX_IPMI_SSIF_BMC_H

#include <linex/types.h>

/* Max length of ipmi ssif message included netfn and cmd field */
#define IPMI_SSIF_PAYLOAD_MAX         254
struct ipmi_ssif_msg {
	unsigned int len;
	__u8    payload[IPMI_SSIF_PAYLOAD_MAX];
};

#endif /* _UAPI_LINEX_IPMI_SSIF_BMC_H */
