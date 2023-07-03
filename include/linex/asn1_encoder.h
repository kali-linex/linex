/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _LINEX_ASN1_ENCODER_H
#define _LINEX_ASN1_ENCODER_H

#include <linex/types.h>
#include <linex/asn1.h>
#include <linex/asn1_ber_bytecode.h>
#include <linex/bug.h>

#define asn1_oid_len(oid) (sizeof(oid)/sizeof(u32))
unsigned char *
asn1_encode_integer(unsigned char *data, const unsigned char *end_data,
		    s64 integer);
unsigned char *
asn1_encode_oid(unsigned char *data, const unsigned char *end_data,
		u32 oid[], int oid_len);
unsigned char *
asn1_encode_tag(unsigned char *data, const unsigned char *end_data,
		u32 tag, const unsigned char *string, int len);
unsigned char *
asn1_encode_octet_string(unsigned char *data,
			 const unsigned char *end_data,
			 const unsigned char *string, u32 len);
unsigned char *
asn1_encode_sequence(unsigned char *data, const unsigned char *end_data,
		     const unsigned char *seq, int len);
unsigned char *
asn1_encode_boolean(unsigned char *data, const unsigned char *end_data,
		    bool val);

#endif
