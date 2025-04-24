#!/bin/sh

# This file is part of frame-logger project.
# Copyright (C) 2025 Rishitha Minol
#
# This file is dual-licensed:
#
# - Under the GNU General Public License v3.0 or later (GPLv3),
#   as published by the Free Software Foundation; and
#
# - Under the Business Source License 1.1 (BSL),
#   as published by MariaDB Corporation.
#
# You may choose either license for use, subject to the terms below.
#
# GPLv3: See the LICENSE.gpl file in the project root or
#        <https://www.gnu.org/licenses/gpl-3.0.html>
#
# BSL:  See the LICENSE.bsl file in the project root or
#       <https://mariadb.com/bsl11>
#
# Under the BSL, production/commercial use of this script is restricted
# until April 23, 2028. After that date, this script will be permanently
# licensed under the terms of GPLv3.
#
# For licensing inquiries, contact: rishithaminol@gmail.com

UNAME_OUTPUT=$(uname -a)
if echo "$UNAME_OUTPUT" | grep -q "FreeBSD"; then
    # "This is FreeBSD."
    PYTHON_INTERPRETER=$(ls /usr/local/bin/python* | head -n1)
    ${PYTHON_INTERPRETER} ./main.py
else
    python3 ./main.py
fi
