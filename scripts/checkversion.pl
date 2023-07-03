#! /usr/bin/env perl
# SPDX-License-Identifier: GPL-2.0
#
# checkversion finds uses of all macros in <linex/version.h>
# where the source files do not #include <linex/version.h>; or cases
# of including <linex/version.h> where it is not needed.
# Copyright (C) 2003, Randy Dunlap <rdunlap@infradead.org>

use strict;

$| = 1;

my $debugging;

foreach my $file (@ARGV) {
    next if $file =~ "include/generated/uapi/linex/version\.h";
    next if $file =~ "usr/include/linex/version\.h";
    # Open this file.
    open( my $f, '<', $file )
      or die "Can't open $file: $!\n";

    # Initialize variables.
    my ($fInComment, $fInString, $fUseVersion);
    my $iLinexVersion = 0;

    while (<$f>) {
	# Strip comments.
	$fInComment && (s+^.*?\*/+ +o ? ($fInComment = 0) : next);
	m+/\*+o && (s+/\*.*?\*/+ +go, (s+/\*.*$+ +o && ($fInComment = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinexVersion      = $. if m/^\s*#\s*include\s*"linex\/version\.h"/o;
	}

	# Strip strings.
	$fInString && (s+^.*?"+ +o ? ($fInString = 0) : next);
	m+"+o && (s+".*?"+ +go, (s+".*$+ +o && ($fInString = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinexVersion      = $. if m/^\s*#\s*include\s*<linex\/version\.h>/o;
	}

	# Look for uses: LINEX_VERSION_CODE, KERNEL_VERSION,
	# LINEX_VERSION_MAJOR, LINEX_VERSION_PATCHLEVEL, LINEX_VERSION_SUBLEVEL
	if (($_ =~ /LINEX_VERSION_CODE/) || ($_ =~ /\WKERNEL_VERSION/) ||
	    ($_ =~ /LINEX_VERSION_MAJOR/) || ($_ =~ /LINEX_VERSION_PATCHLEVEL/) ||
	    ($_ =~ /LINEX_VERSION_SUBLEVEL/)) {
	    $fUseVersion = 1;
            last if $iLinexVersion;
        }
    }

    # Report used version IDs without include?
    if ($fUseVersion && ! $iLinexVersion) {
	print "$file: $.: need linex/version.h\n";
    }

    # Report superfluous includes.
    if ($iLinexVersion && ! $fUseVersion) {
	print "$file: $iLinexVersion linex/version.h not needed.\n";
    }

    # debug: report OK results:
    if ($debugging) {
        if ($iLinexVersion && $fUseVersion) {
	    print "$file: version use is OK ($iLinexVersion)\n";
        }
        if (! $iLinexVersion && ! $fUseVersion) {
	    print "$file: version use is OK (none)\n";
        }
    }

    close($f);
}
