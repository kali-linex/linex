#!/bin/sh
# SPDX-License-Identifier: LGPL-2.1

if [ $# -ne 1 ] ; then
	linex_header_dir=tools/include/uapi/linex
else
	linex_header_dir=$1
fi

linex_mman=${linex_header_dir}/mman.h

printf "static const char *mremap_flags[] = {\n"
regex='^[[:space:]]*#[[:space:]]*define[[:space:]]+MREMAP_([[:alnum:]_]+)[[:space:]]+((0x)?[[:xdigit:]]+)[[:space:]]*.*'
grep -E -q $regex ${linex_mman} && \
(grep -E $regex ${linex_mman} | \
	sed -r "s/$regex/\2 \1 \1 \1 \2/g"	| \
	xargs printf "\t[ilog2(%s) + 1] = \"%s\",\n#ifndef MREMAP_%s\n#define MREMAP_%s %s\n#endif\n")
printf "};\n"
