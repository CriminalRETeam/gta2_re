#!/bin/bash
# tu.sh SRC.cpp [ADDR...]: preprocess SRC (a Source/*.cpp or a modified copy of one; it is compiled as if it
# were in Source/) with WIP_IMPLEMENTED/NOT_IMPLEMENTED stripped, compile with VC6 into $WORK/q.obj
# (about 3 s), and print the diff line count against the 10.5 target for each ADDR (0 = same asm,
# stack offsets ignored). Use diff.py for the diff itself.
. "$(dirname "$0")/common.sh"
S=$(realpath "$1"); shift
tmp=$REPO/Source/_tu_harness_tmp_$$.cpp
sed -E 's/^\s*(WIP|NOT)_IMPLEMENTED;\s*$//' "$S" > $tmp
T=$REPO/3rdParty/gta2_re_compile_tools
export WINEDEBUG=-all WINEPATH="$(winpath $T/VC98/Bin);$(winpath $T/Common/MSDev98/Bin)" INCLUDE="$(winpath $T/VC98/Include)"
R=$(winpath $REPO)
(cd $WORK && wine cmd /c "cl.exe /nologo /TP -DIMGUI_DLL -D_CRT_NON_CONFORMING_SWPRINTFS -D_CRT_SECURE_NO_WARNINGS -I$R\\Source -I$R -I$(winpath $T) /DWIN32 /D_WINDOWS /DNDEBUG /EP $(winpath $tmp)" 2>/dev/null | tr -d '\r' > q.cpp)
rm -f $tmp
"$(dirname "$0")/cl.sh" $WORK/q.cpp || exit 1
for a in "$@"; do echo "$a $($PY "$(dirname "$0")/diff.py" $WORK/q.obj $a -n)"; done
