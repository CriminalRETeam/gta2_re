#!/bin/bash
# sched.sh [-q] [-r] <file.cpp> [function name substring]
#
# Compiles a Source/ TU (a stripped copy: WIP_IMPLEMENTED/NOT_IMPLEMENTED removed, as for the target
# comparison) with the gta2 flags and the scheduler-logging C2.DLL (setup.py; same code as stock), then
# for the functions matching the substring prints the window node counts (nodes.py) and the per-window
# schedule (sv.py; -r also prints the ready list at each cycle). Without a substring: nodes.py for all.
#   -q        compile only, with the non-logging variant (last.obj), for scripts
#   -l        compile with logging (last.log, last.asm) but print nothing
#   LIM=69    what-if: window limit 69 for every function; LIM=12:80,70 per window of function #12
#   X87_OUT   output dir (default build_vc6/x87_c2/out): last.log, last.asm, last.obj
#   X87_C2    log|fast|stock: which C2.DLL; WRAP: command prefix for wine (drc/dr.sh uses it)
. "$(dirname "$0")/common.sh"
Q=; RD=; SILENT=
while :; do case "$1" in -q) Q=1; shift;; -l) SILENT=1; shift;; -r) RD=-r; shift;; *) break;; esac; done
[ -f "$1" ] || { sed -n '2,12s/^# \{0,1\}//p' "$0"; exit 1; }
SRC=$(realpath "$1"); shift
OUTD=${X87_OUT:-$X/out}; mkdir -p "$OUTD"
V=log; [ -n "$Q" ] && V=fast; V=${X87_C2:-$V}
if [ -n "$LIM" ]; then
  B="$OUTD/c2"; python3 "$HERE/setup.py" --variant $V --out "$B" --lim "$LIM" > /dev/null || exit 1
else
  B="$X/$V"
  [ -f "$B/VC98/Bin/C2.DLL" ] || python3 "$HERE/setup.py" --variant $V > /dev/null || exit 1
fi
b=$(basename "$SRC")
sed -E 's/^\s*(WIP|NOT)_IMPLEMENTED;\s*$//' "$SRC" > "$OUTD/$b"
case "$b" in Network_20324.cpp) XF="/Gz";; FpsCounter_54.cpp) XF="/GX-";; gbh_graphics.cpp) XF="/Od /ZI";; esac
if [ -n "$MSYSTEM" ]; then
  export PATH="$B/VC98/Bin:$RT/Common/MSDev98/Bin:$PATH"
else
  export WINEPATH="$(winpath "$B/VC98/Bin");$(winpath "$RT/Common/MSDev98/Bin")"
fi
export INCLUDE="$(winpath "$RT/VC98/ATL/Include");$(winpath "$RT/VC98/Include");$(winpath "$RT/VC98/MFC/Include")"
FLAGS="/DWIN32 /D_WINDOWS /D_CRT_SECURE_NO_WARNINGS /D_CRT_NON_CONFORMING_SWPRINTFS /DIMGUI_DLL /W3 /EHsc /GX /ML /O2 /DNDEBUG $XF"
INCS="/I$(winpath "$(dirname "$SRC")") /I$(winpath "$ROOT/Source") /I$(winpath "$ROOT") /I$(winpath "$RT")"
# a worktree without submodules: also take 3rdParty/GTA2Hax from the checkout VC6_TOOLS lives in
RT_ROOT=$(realpath -m "$RT/../..")
[ "$RT_ROOT" != "$ROOT" ] && INCS="$INCS /I$(winpath "$RT_ROOT")"
LST=; [ -z "$Q" ] && LST="/FAs /Fa$(winpath "$OUTD/last.asm")"
rm -f "$OUTD/last.obj"
$WRAP ${MSYSTEM:+env MSYS2_ARG_CONV_EXCL=*} ${WINE-wine} "$B/VC98/Bin/CL.EXE" /nologo /TP /c $INCS $FLAGS /Zm1000 $LST /Fo"$(winpath "$OUTD/last.obj")" \
  "$(winpath "$OUTD/$b")" 2>&1 | tr -d '\r' > "$OUTD/last.log"
grep -E " error " "$OUTD/last.log"
[ -f "$OUTD/last.obj" ] || exit 1
[ -n "$Q$SILENT" ] && exit 0
python3 "$HERE/nodes.py" "$OUTD/last.log" "$@"
[ $# -gt 0 ] && python3 "$HERE/sv.py" "$OUTD/last.log" "$OUTD/last.asm" "$@" $RD
exit 0
