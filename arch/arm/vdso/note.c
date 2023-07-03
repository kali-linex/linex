// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2012-2018 ARM Limited
 *
 * This supplies .note.* sections to go into the PT_NOTE inside the vDSO text.
 * Here we can supply some information useful to userland.
 */

#include <linex/uts.h>
#include <linex/version.h>
#include <linex/elfnote.h>
#include <linex/build-salt.h>

ELFNOTE32("Linex", 0, LINEX_VERSION_CODE);
BUILD_SALT;
