#!/bin/bash
# cl.sh [-tc TC] FILE.cpp [flags...]: compile FILE (cwd-relative or absolute) with /FAs listing next to it.
#   TC: vc6 (default, the repo toolchain, SP4), msvc6.0 ... msvc6.6, msvc7.0 (from fetch_compilers.sh)
#   VC6 default flags: /O2 /GX /EHsc (as the build). VC7 (9.6f) flags must be passed, e.g. /O2 /Ob0 /G5 /GX.
. "$(dirname "$0")/common.sh"
TC=vc6; [ "$1" = -tc ] && { TC=$2; shift 2; }
f=$(realpath "$1"); shift
T=$REPO/3rdParty/gta2_re_compile_tools
case $TC in
  vc6) B=$T/VC98/Bin; INC=$T/VC98/Include; DEF="/O2 /GX /EHsc" ;;
  msvc7.0) B=$COMPILERS/msvc7.0/Bin; INC=$COMPILERS/msvc7.0/Include; DEF="" ;;
  *) B=$COMPILERS/$TC/Bin; INC=$T/VC98/Include; DEF="/O2 /GX /EHsc" ;;
esac
cd "$(dirname "$f")"; b=$(basename "${f%.*}")
export WINEDEBUG=-all WINEPATH="$(winpath $B);$(winpath $T/Common/MSDev98/Bin)" INCLUDE="$(winpath $INC)"
rm -f $b.obj
wine cmd /c "cl.exe /nologo /TP /W0 /Zm1000 /DNDEBUG $DEF $* /c /FAs /Fa$b.asm /Fo$b.obj $(basename $f)" 2>&1 \
  | tr -d '\r' | grep -v "^$(basename $f)\s*$" | grep -E "error|warning C4[0-9]*: .*(uninit|not all)" | head -5
[ -f $b.obj ]
