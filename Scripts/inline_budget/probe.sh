#!/bin/bash
# probe.sh <probe.cpp> [cl flags]
#
# Compiles a small test file with the instrumented C2.DLL (Source/ on the include path) and prints the
# inliner log ("@I" lines) and warnings. Pipe it to a file and use inltree.py / inlsim.py on it.
. "$(dirname "$0")/common.sh"
export INCLUDE="$(winpath "$ROOT/Source");$(winpath "$RT/VC98/Include")"
f=$(realpath "$1"); shift
wine "$IC2/VC98/Bin/CL.EXE" /nologo /TP /c /W3 /GX /ML /O2 /DNDEBUG /DWIN32 "$@" /Fo"$(winpath "${f%.cpp}.obj")" "$(winpath "$f")" 2>&1 | grep -vE "^$(basename "$f")\s*$"
