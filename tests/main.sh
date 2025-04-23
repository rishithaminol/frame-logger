#!/bin/sh

# This file is part of frame-logger.
# Copyright (C) 2025 Rishitha Minol
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

UNAME_OUTPUT=$(uname -a)
if echo "$UNAME_OUTPUT" | grep -q "FreeBSD"; then
    # "This is FreeBSD."
    PYTHON_INTERPRETER=$(ls /usr/local/bin/python* | head -n1)
    ${PYTHON_INTERPRETER} ./main.py
else
    python3 ./main.py
fi
