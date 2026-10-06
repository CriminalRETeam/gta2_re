#!/bin/bash
# dr.sh <bbc|ct|ww> <out.txt> [hexaddr,...] <file.cpp> [function]
#
# Compiles a TU as sched.sh -l does, with CL.EXE running under a DynamoRIO client (build.sh), and writes
# the client's output (all processes, sorted for bbc) to <out.txt>. ww takes the dwords to watch.
# X87_C2=stock|fast|log picks the C2.DLL (default stock, so coverage addresses are the original code).
# Coverage diffing: run bbc on two variants of a source, then `diff` the two files (or join them on the
# address) to see which C2 blocks, and how often, one variant takes and the other doesn't.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
: "${DYNAMORIO:?set DYNAMORIO to an unpacked DynamoRIO-Linux release}"
C=$1; OUT=$(realpath -m "$2"); shift 2
OPTS="$OUT.part"
[ "$C" = ww ] && { OPTS="$OPTS $(echo "$1" | tr , ' ')"; shift; }
LIB="$ROOT/build_vc6/x87_c2/drc/lib$C.so"
[ -f "$LIB" ] || "$ROOT/Scripts/x87_sched/drc/build.sh" >&2 || exit 1
rm -f "$OUT".part.*.txt
X87_C2=${X87_C2:-stock} WINE=${DR_WINE:-/usr/lib/wine/wine} \
  WRAP="$DYNAMORIO/bin32/drrun -c $LIB $OPTS --" "$ROOT/Scripts/x87_sched/sched.sh" -l "$@"
if [ "$C" = bbc ]; then cat "$OUT".part.*.txt | sort > "$OUT"; else cat "$OUT".part.*.txt > "$OUT"; fi
rm -f "$OUT".part.*.txt
wc -l < "$OUT"
