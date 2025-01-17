/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * ssip_slave.h
 *
 * SSIP slave support header file
 *
 * Copyright (C) 2010 Nokia Corporation. All rights reserved.
 *
 * Contact: Carlos Chinea <carlos.chinea@nokia.com>
 */

#ifndef __LINEX_SSIP_SLAVE_H__
#define __LINEX_SSIP_SLAVE_H__

#include <linex/hsi/hsi.h>

static inline void ssip_slave_put_master(struct hsi_client *master)
{
}

struct hsi_client *ssip_slave_get_master(struct hsi_client *slave);
int ssip_slave_start_tx(struct hsi_client *master);
int ssip_slave_stop_tx(struct hsi_client *master);
void ssip_reset_event(struct hsi_client *master);

int ssip_slave_running(struct hsi_client *master);

#endif /* __LINEX_SSIP_SLAVE_H__ */

