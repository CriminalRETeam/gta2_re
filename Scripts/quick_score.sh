#!/bin/bash
# Compiles one .cpp into a private obj and scores one function against target_asm.json,
# without touching build_vc6/ (safe to run several at once).
#
#   Scripts/quick_score.sh <Source/File.cpp> <og_addr> <symbol_substring> [-q]
#
# Prints the post processed asm diff, then the score (0 = match). -q prints the score only.
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC=$1; ADDR=$2; NEEDLE=$3; QUIET=$4
case "$(basename "$SRC")" in
    Network_20324.cpp) export EXTRA_CFLAGS="/Gz" ;;
    sharp_bose_0x54.cpp) export EXTRA_CFLAGS="/GX-" ;;
    gbh_graphics.cpp) export EXTRA_CFLAGS="/Od /ZI" ;;
esac
export GTA2_RE="$ROOT" WINEDEBUG=-all
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
NAME=$(basename "$SRC" .cpp)
if ! "$ROOT/3rdParty/cpp_permuter/examples/gta2/compile.sh" "$SRC" "$TMP/$NAME.obj" > "$TMP/log" 2>&1; then
    grep -E "error|fatal" "$TMP/log" | head -20
    exit 2
fi
grep -E "warning C4(700|701|715|716|05[0-9])" "$TMP/log" | head -5 || true
PY="$ROOT/venv/bin/python3"
if [ "$QUIET" = "-q" ]; then
    "$PY" "$ROOT/Scripts/bin_comp/permuter_score.py" "$TMP/$NAME.obj" "$ADDR" "$NEEDLE" | tail -1
else
    "$PY" "$ROOT/Scripts/bin_comp/permuter_score.py" "$TMP/$NAME.obj" "$ADDR" "$NEEDLE"
fi
