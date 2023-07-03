// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *
 * Copyright (C) Jonathan Naylor G4KLX (g4klx@g4klx.demon.co.uk)
 */
#include <linex/errno.h>
#include <linex/types.h>
#include <linex/socket.h>
#include <linex/in.h>
#include <linex/kernel.h>
#include <linex/timer.h>
#include <linex/string.h>
#include <linex/sockios.h>
#include <linex/net.h>
#include <net/ax25.h>
#include <linex/inet.h>
#include <linex/netdevice.h>
#include <linex/skbuff.h>
#include <net/sock.h>
#include <linex/uaccess.h>
#include <linex/fcntl.h>
#include <linex/mm.h>
#include <linex/interrupt.h>

/*
 * The following routines are taken from page 170 of the 7th ARRL Computer
 * Networking Conference paper, as is the whole state machine.
 */

void ax25_std_nr_error_recovery(ax25_cb *ax25)
{
	ax25_std_establish_data_link(ax25);
}

void ax25_std_establish_data_link(ax25_cb *ax25)
{
	ax25->condition = 0x00;
	ax25->n2count   = 0;

	if (ax25->modulus == AX25_MODULUS)
		ax25_send_control(ax25, AX25_SABM, AX25_POLLON, AX25_COMMAND);
	else
		ax25_send_control(ax25, AX25_SABME, AX25_POLLON, AX25_COMMAND);

	ax25_calculate_t1(ax25);
	ax25_stop_idletimer(ax25);
	ax25_stop_t3timer(ax25);
	ax25_stop_t2timer(ax25);
	ax25_start_t1timer(ax25);
}

void ax25_std_transmit_enquiry(ax25_cb *ax25)
{
	if (ax25->condition & AX25_COND_OWN_RX_BUSY)
		ax25_send_control(ax25, AX25_RNR, AX25_POLLON, AX25_COMMAND);
	else
		ax25_send_control(ax25, AX25_RR, AX25_POLLON, AX25_COMMAND);

	ax25->condition &= ~AX25_COND_ACK_PENDING;

	ax25_calculate_t1(ax25);
	ax25_start_t1timer(ax25);
}

void ax25_std_enquiry_response(ax25_cb *ax25)
{
	if (ax25->condition & AX25_COND_OWN_RX_BUSY)
		ax25_send_control(ax25, AX25_RNR, AX25_POLLON, AX25_RESPONSE);
	else
		ax25_send_control(ax25, AX25_RR, AX25_POLLON, AX25_RESPONSE);

	ax25->condition &= ~AX25_COND_ACK_PENDING;
}

void ax25_std_timeout_response(ax25_cb *ax25)
{
	if (ax25->condition & AX25_COND_OWN_RX_BUSY)
		ax25_send_control(ax25, AX25_RNR, AX25_POLLOFF, AX25_RESPONSE);
	else
		ax25_send_control(ax25, AX25_RR, AX25_POLLOFF, AX25_RESPONSE);

	ax25->condition &= ~AX25_COND_ACK_PENDING;
}
