#!/bin/bash
# Runs cpp_permuter (3rdParty/cpp_permuter) on one function, scored against target_asm.json.
#
#   Scripts/permute.sh <File.cpp> <Class::Function> <og_addr> [cpp_permuter options...]
#
# e.g.
#   Scripts/permute.sh Source/sound_obj.cpp sound_obj::HandleVocalStreamSwitching_57DF10 57df10 \
#       -m exhaustive -p reorder_saves+invert_if -j 4
#
# Needs: a build (for Scripts/bin_comp/target_asm.json and the venv), wine, cmake + a C++17
# compiler (cpp_permuter is built into build_permuter/ on first use).
# Remove NOT_IMPLEMENTED / WIP_IMPLEMENTED from the function first, they add code.
# Results land in permuter_out/output-<score>-<n>/ (function.cpp, diff.txt, asm_diff.txt).
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC=$1
FUNC=$2
ADDR=$3
shift 3

# --96f: compile with VC7.0 (Scripts/compile_vc7.sh) and score against the 9.6f build. ADDR is then
# the 9.6f address (docs/inlines_96f.md, match_96f.json).
COMPILE="$ROOT/3rdParty/cpp_permuter/examples/gta2/compile.sh {src} {obj}"
SCORE_FLAG=""
if [ "$1" = "--96f" ]; then
    shift
    COMPILE="$ROOT/Scripts/compile_vc7.sh {src} {obj}"
    SCORE_FLAG="--96f"
fi

PERMUTER_DIR="$ROOT/3rdParty/cpp_permuter"
BIN="$ROOT/build_permuter/cpp_permuter"
if [ ! -x "$BIN" ] || [ -n "$(find "$PERMUTER_DIR/src" -newer "$BIN" -print -quit)" ]; then
    cmake -S "$PERMUTER_DIR" -B "$ROOT/build_permuter" -DCMAKE_BUILD_TYPE=Release >/dev/null
    cmake --build "$ROOT/build_permuter" -j 4 >/dev/null
fi

# Per-file flags, as in cmake/vc6.cmake.
case "$(basename "$SRC")" in
    Network_20324.cpp) export EXTRA_CFLAGS="/Gz" ;;
    sharp_bose_0x54.cpp) export EXTRA_CFLAGS="/GX-" ;;
    gbh_graphics.cpp) export EXTRA_CFLAGS="/Od /ZI" ;;
esac

export GTA2_RE="$ROOT"
export WINEDEBUG=-all
# Start the persistent wineserver without our open files (fds above 2): if it inherited the
# fd of a lock the caller holds around this script (flock), it would keep it after we exit.
(for fd in /proc/self/fd/*; do n=${fd##*/}; [ "$n" -gt 2 ] 2>/dev/null && eval "exec $n>&-"; done
 exec wineserver -p) 2>/dev/null || true

PY="$ROOT/venv/bin/python3"
[ -x "$PY" ] || PY=python3
NEEDLE="${FUNC##*::}"

exec "$BIN" -s "$SRC" -f "$FUNC" \
    -c "$COMPILE" \
    --score-cmd "$PY $ROOT/Scripts/bin_comp/permuter_score.py $SCORE_FLAG {obj} $ADDR $NEEDLE" \
    --op-alias "*=Multiply_408680" --op-alias "neg=Negate_4086A0" --extern-globals \
    "$@"
