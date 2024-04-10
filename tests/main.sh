#!/bin/sh

UNAME_OUTPUT=$(uname -a)
if echo "$UNAME_OUTPUT" | grep -q "FreeBSD"; then
    # "This is FreeBSD."
    PYTHON_INTERPRETER=$(ls /usr/local/bin/python* | head -n1)
    ${PYTHON_INTERPRETER} ./main.py
else
    python3 ./main.py
fi
