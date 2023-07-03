/* SPDX-License-Identifier: GPL-2.0-only OR MIT */
/*
 * Apple mailbox message format
 *
 * Copyright (C) 2021 The Asahi Linex Contributors
 */

#ifndef _LINEX_APPLE_MAILBOX_H_
#define _LINEX_APPLE_MAILBOX_H_

#include <linex/types.h>

/* encodes a single 96bit message sent over the single channel */
struct apple_mbox_msg {
	u64 msg0;
	u32 msg1;
};

#endif
