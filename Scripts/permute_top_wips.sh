#!/bin/bash
# Ranks every WIP_FUNC by its unmodified permuter score, then runs every pass and every pair of
# passes (exhaustive, "a+b" = a, b and a-then-b) on the best N, one after the other.
#
#   Scripts/permute_top_wips.sh [-n 3] [-j 4] [-o /some/dir] [--depth 1] [--max-candidates N]
#
# --max-candidates N caps the candidates per function; 0 (the default) means no limit.
# Needs the same setup as Scripts/permute.sh (a build, wine, Scripts/bin_comp/target_asm.json).
# The WIP/NOT_IMPLEMENTED logging guards are switched off with /DNO_WIP_LOGGING (see Function.hpp),
# so nothing in the source needs editing. Results: <out>/<addr>/permuter_out/output-<score>-<n>/.
# Base scores are cached in <out>/base_scores.json; delete it to re-rank.
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
N=3; JOBS=4; OUT="$ROOT/permuter_top_wips"; DEPTH=1; MAXC=0
while [ $# -gt 0 ]; do
    case "$1" in
        -n) N=$2; shift 2 ;;
        -j) JOBS=$2; shift 2 ;;
        -o) OUT=$2; shift 2 ;;
        --depth) DEPTH=$2; shift 2 ;;
        --max-candidates) MAXC=$2; shift 2 ;;
        *) echo "unknown option $1" >&2; exit 2 ;;
    esac
done
mkdir -p "$OUT"
export EXTRA_CFLAGS="/DNO_WIP_LOGGING"
PY="$ROOT/venv/bin/python3"; [ -x "$PY" ] || PY=python3
BIN="$ROOT/build_permuter/cpp_permuter"

# 1. rank the WIPs (builds cpp_permuter on first use through permute.sh --base-only)
"$PY" - "$ROOT" "$OUT" "$N" <<'EOF'
import json, os, sys
root, out, n = sys.argv[1], sys.argv[2], int(sys.argv[3])
sys.path.insert(0, os.path.join(root, "Scripts"))
import permute_sweep as ps
cache = os.path.join(out, "base_scores.json")
bases = json.load(open(cache)) if os.path.exists(cache) else {}
funcs = ps.wip_functions()
for i, (src, func, addr) in enumerate(funcs):
    if addr not in bases:
        bases[addr] = ps.base_score(src, func, addr, os.path.join(out, addr))
        json.dump(bases, open(cache, "w"), indent=1)
        print("[%d/%d] %s %s" % (i + 1, len(funcs), func, bases[addr]), file=sys.stderr, flush=True)
ranked = sorted((f for f in funcs if bases.get(f[2])), key=lambda f: bases[f[2]])[:n]
with open(os.path.join(out, "top.txt"), "w") as fh:
    for src, func, addr in ranked:
        fh.write("%s %s %s %s\n" % (src, func, addr, bases[addr]))
EOF
echo "top $N WIPs:"; cat "$OUT/top.txt"

# 2. every pass alone and every pair of passes
PASSES=$("$BIN" --list-passes | grep -E '^  [a-z_0-9]+$' | tr -d ' ')
LIST=""
for a in $PASSES; do
    LIST="$LIST,$a"
    for b in $PASSES; do
        [ "$a" != "$b" ] && LIST="$LIST,$a+$b"
    done
done
LIST="${LIST#,}"

# 3. run them
while read -r SRC FUNC ADDR BASE; do
    echo "=== $FUNC ($ADDR, base $BASE) ==="
    mkdir -p "$OUT/$ADDR"
    (cd "$OUT/$ADDR" && "$ROOT/Scripts/permute.sh" "$ROOT/$SRC" "$FUNC" "$ADDR" \
        -m exhaustive -p "$LIST" --depth "$DEPTH" --max-candidates "$MAXC" -j "$JOBS" \
        2>&1 | tee run.log | tail -n 15)
done < "$OUT/top.txt"
echo "done; best candidates: ls -d $OUT/*/permuter_out/output-* | sort -t- -k2 -n"
