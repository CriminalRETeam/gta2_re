#!/bin/bash
# ralog.sh <file.cpp> [function name substring]
#
# Compiles a TU with the colour-logging C2.DLL (patch_c2.py, same code as stock) and prints, per
# function, each global register allocation decision in the order C2 makes them:
#   prio tie weight -> register   scores (eax ecx edx ebx esp ebp esi edi; lowest allowed wins)
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
[ -f "$1" ] || { sed -n '2,6s/^# \{0,1\}//p' "$0"; exit 1; }
PY="$ROOT/venv/bin/python3"; [ -x "$PY" ] || PY=python3
[ -f "$ROOT/build_vc6/x87_c2/ralog/VC98/Bin/C2.DLL" ] || "$PY" "$ROOT/Scripts/regalloc/patch_c2.py" > /dev/null || exit 1
OUT="$ROOT/build_vc6/x87_c2/ralog_out"
X87_C2=ralog X87_OUT="$OUT" "$ROOT/Scripts/x87_sched/sched.sh" -l "$1" > /dev/null
awk -v f="$2" '
  BEGIN { split("none eax ecx edx ebx esp ebp esi edi", R, " ") }
  /^@F / { p = (f == "" || index($0, f)); if (p) print substr($0, 4); next }
  p && /^@R / {
    for (i = 2; i <= NF; i++) { split($i, kv, "="); v[kv[1]] = kv[2] }
    printf "  prio=%-5d tie=%-5d w=%-4d -> %s   s=%s", v["prio"], v["tie"], v["w"], R[v["reg"] + 1], v["s"]
    for (i = 10; i <= NF; i++) printf " %s", $i
    printf "\n"
  }' "$OUT/last.log"
