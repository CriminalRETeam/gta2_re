#!/bin/bash
# build.sh: builds the DynamoRIO clients (bbc.c, ct.c, ww.c) into build_vc6/x87_c2/drc/.
# Needs gcc with -m32 and a DynamoRIO release unpacked at $DYNAMORIO
# (https://github.com/DynamoRIO/dynamorio/releases, DynamoRIO-Linux-10.0.0 was used).
# Without the 32-bit libc headers (gcc-multilib), the 64-bit ones are used with an empty gnu/stubs-32.h:
# the clients only need DynamoRIO's API and link against libdynamorio, not libc.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
: "${DYNAMORIO:?set DYNAMORIO to an unpacked DynamoRIO-Linux release}"
OUT="$ROOT/build_vc6/x87_c2/drc"; mkdir -p "$OUT"
INC=
if ! echo '#include <stdio.h>' | gcc -m32 -E -x c - > /dev/null 2>&1; then
  mkdir -p "$OUT/inc/gnu"; : > "$OUT/inc/gnu/stubs-32.h"
  INC="-I$OUT/inc -I/usr/include/$(gcc -print-multiarch)"
fi
for c in bbc ct ww; do
  gcc -m32 -O2 -shared -fPIC -nostdlib -DLINUX -DX86_32 -I"$DYNAMORIO/include" $INC -o "$OUT/lib$c.so" \
    "$ROOT/Scripts/x87_sched/drc/$c.c" -L"$DYNAMORIO/lib32/release" -ldynamorio || exit 1
done
echo "clients in $OUT"
