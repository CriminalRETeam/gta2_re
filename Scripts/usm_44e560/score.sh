#!/bin/bash
# score.sh [C2DIR] : copies Source/ to build_vc6/usm_exp, applies experiment.patch, compiles CarAI_78.cpp with
# the toolchain in C2DIR (default: the stock one) and prints UpdateStateMachine_44E560's permuter score (0 = match).
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/../.." && pwd)"
RT="$ROOT/3rdParty/gta2_re_compile_tools"; TC="${1:-$RT}"; W="$ROOT/build_vc6/usm_exp"
winpath() { printf 'Z:%s' "$(realpath -m "$1")" | tr / '\\'; }
rm -rf "$W"; mkdir -p "$W"; cp -r "$ROOT/Source" "$W/"
(cd "$W" && patch -s -p1 < "$HERE/experiment.patch") || exit 1
export WINEDEBUG=-all WINEPATH="$(winpath "$TC/VC98/Bin");$(winpath "$RT/Common/MSDev98/Bin")"
export INCLUDE="$(winpath "$RT/VC98/ATL/Include");$(winpath "$RT/VC98/Include");$(winpath "$RT/VC98/MFC/Include")"
FLAGS="/DWIN32 /D_WINDOWS /D_CRT_SECURE_NO_WARNINGS /D_CRT_NON_CONFORMING_SWPRINTFS /DIMGUI_DLL /W3 /EHsc /GX /ML /O2 /DNDEBUG"
INCS="/I$(winpath "$W/Source") /I$(winpath "$ROOT") /I$(winpath "$RT")"
nice -n 19 wine "$TC/VC98/Bin/CL.EXE" /nologo /TP /c $INCS $FLAGS /Zm1000 /Fo"$(winpath "$W/CarAI_78.obj")" \
  "$(winpath "$W/Source/CarAI_78.cpp")" > "$W/cl.log" 2>&1
[ -s "$W/CarAI_78.obj" ] || { grep " error " "$W/cl.log"; exit 1; }
PY="$ROOT/venv/bin/python3"; [ -x "$PY" ] || PY=python3
"$PY" "$ROOT/Scripts/bin_comp/permuter_score.py" --no-callees "$W/CarAI_78.obj" 44e560 UpdateStateMachine_44E560 | tail -1
