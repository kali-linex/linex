#
# gdb helper commands and functions for Linex kernel debugging
#
#  loader module
#
# Copyright (c) Siemens AG, 2012, 2013
#
# Authors:
#  Jan Kiszka <jan.kiszka@siemens.com>
#
# This work is licensed under the terms of the GNU GPL version 2.
#

import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + "/scripts/gdb")

try:
    gdb.parse_and_eval("0")
    gdb.execute("", to_string=True)
except:
    gdb.write("NOTE: gdb 7.2 or later required for Linex helper scripts to "
              "work.\n")
else:
    import linex.constants
    if linex.constants.LX_CONFIG_DEBUG_INFO_REDUCED:
        raise gdb.GdbError("Reduced debug information will prevent GDB "
                           "from having complete types.\n")
    import linex.utils
    import linex.symbols
    import linex.modules
    import linex.dmesg
    import linex.tasks
    import linex.config
    import linex.cpus
    import linex.lists
    import linex.rbtree
    import linex.proc
    import linex.timerlist
    import linex.clk
    import linex.genpd
    import linex.device
    import linex.vfs
    import linex.mm
    import linex.radixtree
    import linex.interrupts
