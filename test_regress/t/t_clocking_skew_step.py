#!/usr/bin/env python3
# DESCRIPTION: Verilator: Clocking output skews with a harness that steps time itself
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of either the GNU Lesser General Public License Version 3
# or the Perl Artistic License Version 2.0.
# SPDX-FileCopyrightText: 2026 Andrei Girjoaba
# SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0

import vltest_bootstrap

test.scenarios("vlt")

test.compile(make_top_shell=False,
             make_main=False,
             verilator_flags2=["--exe", "--timing", "t/t_clocking_skew_step.cpp"])

test.execute()

test.passes()
