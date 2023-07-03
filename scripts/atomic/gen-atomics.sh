#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Generate atomic headers

ATOMICDIR=$(dirname $0)
ATOMICTBL=${ATOMICDIR}/atomics.tbl
LINEXDIR=${ATOMICDIR}/../..

cat <<EOF |
gen-atomic-instrumented.sh      linex/atomic/atomic-instrumented.h
gen-atomic-long.sh              linex/atomic/atomic-long.h
gen-atomic-fallback.sh          linex/atomic/atomic-arch-fallback.h
EOF
while read script header args; do
	/bin/sh ${ATOMICDIR}/${script} ${ATOMICTBL} ${args} > ${LINEXDIR}/include/${header}
	HASH="$(sha1sum ${LINEXDIR}/include/${header})"
	HASH="${HASH%% *}"
	printf "// %s\n" "${HASH}" >> ${LINEXDIR}/include/${header}
done
