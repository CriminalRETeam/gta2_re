#!/bin/sh
# Compiles one gta2_re translation unit with VC7.0 13.00.9466 and the 9.6f flags (/O2 /Ob0 /G5 /GX),
# for scoring against the 9.6f build (permuter_score.py --96f, Scripts/permute.sh --96f).
# Same interface as cpp_permuter's examples/gta2/compile.sh:
#
#   compile_vc7.sh <src.cpp> <out.obj>
#
# Needs build_vc6/compilers/msvc7.0 (Scripts/tu_harness/fetch_compilers.sh). The VC6 headers are used,
# as the code is written against them. The source's own directory stands in for Source/.
set -e
SRC=$1
OBJ=$2
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TOOLS="$ROOT/3rdParty/gta2_re_compile_tools"
VC7="$ROOT/build_vc6/compilers/msvc7.0"
[ -f "$VC7/Bin/CL.EXE" ] || { echo "error: $VC7 missing, run Scripts/tu_harness/fetch_compilers.sh"; exit 2; }

winpath() { printf 'Z:%s' "$(realpath -m "$1")" | tr / '\\'; }

export WINEDEBUG=-all
export INCLUDE="$(winpath "$TOOLS/VC98/Include");$(winpath "$TOOLS/VC98/MFC/Include")"
# Preprocess with VC6 (the headers test _MSC_VER), then compile the result with VC7.
DEFS="/DWIN32 /D_WINDOWS /D_CRT_SECURE_NO_WARNINGS /D_CRT_NON_CONFORMING_SWPRINTFS /DIMGUI_DLL /DNDEBUG"
INCS="/I$(winpath "$(dirname "$SRC")") /I$(winpath "$ROOT") /I$(winpath "$TOOLS")"
PRE="${OBJ%.*}_pp.cpp"
rm -f "$OBJ"
WINEPATH="$(winpath "$TOOLS/VC98/Bin");$(winpath "$TOOLS/Common/MSDev98/Bin")" \
    wine cl.exe /nologo /TP $INCS $DEFS /EP "$(winpath "$SRC")" 2>/dev/null | tr -d '\r' > "$PRE"
WINEPATH="$(winpath "$VC7/Bin");$(winpath "$TOOLS/Common/MSDev98/Bin")" \
    wine cl.exe /nologo /TP /c /W0 /O2 /Ob0 /G5 /GX /Zm1000 /Fo"$(winpath "$OBJ")" "$(winpath "$PRE")" \
    | tr -d '\r' | grep -E "error|fatal" | head -20
[ -f "$OBJ" ] || exit 2
