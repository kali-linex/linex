/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINEX_DECOMPRESS_INFLATE_H
#define LINEX_DECOMPRESS_INFLATE_H

int gunzip(unsigned char *inbuf, long len,
	   long (*fill)(void*, unsigned long),
	   long (*flush)(void*, unsigned long),
	   unsigned char *output,
	   long *pos,
	   void(*error_fn)(char *x));
#endif
