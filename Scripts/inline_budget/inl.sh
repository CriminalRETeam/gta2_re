#!/bin/bash
# inl.sh <file.cpp> [function name substring] [-a]
#
# Compiles a Source/ TU with the gta2 flags and the instrumented C2.DLL (setup.py; the code is the same)
# and prints the inliner's decision for every call site of the matching functions:
#   INLINE (charged), free (callee size <= 40, shown with -a), force (__forceinline),
#   OUT-OF-LINE (over budget), SKIP(argctor) (a by-value argument constructed in place)
# with the callee's size, the budget before the site, the sites left and the nested budget.
# The raw log goes to build_vc6/inline_c2/last.log (LOG=... to change) for inlsim.py.
. "$(dirname "$0")/common.sh"
SRC=$(realpath "$1"); shift
LOG=${LOG:-$IC2/last.log}
OBJ=${OBJ:-$IC2/last.obj}
case "$(basename "$SRC")" in Network_20324.cpp) X="/Gz";; sharp_bose_0x54.cpp) X="/GX-";; gbh_graphics.cpp) X="/Od /ZI";; esac
export INCLUDE="$(winpath "$RT/VC98/ATL/Include");$(winpath "$RT/VC98/Include");$(winpath "$RT/VC98/MFC/Include")"
FLAGS="/DWIN32 /D_WINDOWS /D_CRT_SECURE_NO_WARNINGS /D_CRT_NON_CONFORMING_SWPRINTFS /DIMGUI_DLL /W3 /EHsc /GX /ML /O2 /DNDEBUG $X"
INCS="/I$(winpath "$(dirname "$SRC")") /I$(winpath "$ROOT") /I$(winpath "$RT")"
wine "$IC2/VC98/Bin/CL.EXE" /nologo /TP /c $INCS $FLAGS /Zm1000 /Fo"$(winpath "$OBJ")" "$(winpath "$SRC")" > "$LOG" 2>&1
grep -E " error " "$LOG"
python3 "$HERE/inltree.py" "$LOG" "$@"
