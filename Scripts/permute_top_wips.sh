#!/bin/bash
# Ranks every WIP_FUNC by its unmodified permuter score, then runs every pass (exhaustive) on the
# best N, one after the other. --pairs adds every ordered pair of passes ("a+b" = a, b and a-then-b):
# that is millions of candidates per function at ~130 compiles/min, so only use it with --max-candidates.
#
#   Scripts/permute_top_wips.sh [-n 3] [-j N] [-o /some/dir] [--depth 1] [--max-candidates N] [--pairs]
#   Scripts/permute_top_wips.sh -f sub_4E6660 [-f Class::Func] [-f 4d6000] ...
#
# -f NAME permutes that WIP_FUNC instead of the top N (no ranking). NAME is the function name
# (with or without Class::) or its original address; repeat it for several.
# -j N is the number of parallel compiles (default: all cores, nproc).
# --max-candidates N caps the candidates per function; 0 (the default) means no limit.
# Needs the same setup as Scripts/permute.sh (a build, wine, Scripts/bin_comp/target_asm.json).
# The WIP/NOT_IMPLEMENTED logging guards are switched off with /DNO_WIP_LOGGING (see Function.hpp),
# so nothing in the source needs editing. Results: <out>/<addr>/permuter_out/output-<score>-<n>/.
# Ctrl-C stops cleanly and saves a checkpoint (also saved every 2 s); running the same command again
# resumes where it left off, per function. Delete <out>/<addr>/permuter_out to start that one over.
# Base scores are cached in <out>/base_scores.json; delete it to re-rank.
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
N=3; JOBS=$(nproc); OUT="$ROOT/permuter_top_wips"; DEPTH=1; MAXC=0; ONLY=""; PAIRS=""
while [ $# -gt 0 ]; do
    case "$1" in
        -f) ONLY="$ONLY$2"$'\n'; shift 2 ;;
        --pairs) PAIRS=1; shift ;;
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

# 1. rank the WIPs (or pick the ones named with -f) (builds cpp_permuter on first use through permute.sh --base-only)
ONLY="$ONLY" "$PY" - "$ROOT" "$OUT" "$N" <<'EOF'
import json, os, sys
root, out, n = sys.argv[1], sys.argv[2], int(sys.argv[3])
sys.path.insert(0, os.path.join(root, "Scripts"))
import permute_sweep as ps
cache = os.path.join(out, "base_scores.json")
bases = json.load(open(cache)) if os.path.exists(cache) else {}
funcs = ps.wip_functions()
only = [x.strip() for x in os.environ.get("ONLY", "").split("\n") if x.strip()]
if only:
    picked = []
    for name in only:
        key = name.lower().removeprefix("0x")
        hits = [f for f in funcs if f[2] == key or f[1] == name or f[1].endswith("::" + name)]
        if not hits:
            sys.exit("no WIP_FUNC matches %r" % name)
        if len(hits) > 1:
            sys.exit("%r is ambiguous: %s" % (name, ", ".join(h[1] for h in hits)))
        picked.append(hits[0])
    funcs = picked
for i, (src, func, addr) in enumerate(funcs):
    if addr not in bases:
        bases[addr] = ps.base_score(src, func, addr, os.path.join(out, addr))
        json.dump(bases, open(cache, "w"), indent=1)
        print("[%d/%d] %s %s" % (i + 1, len(funcs), func, bases[addr]), file=sys.stderr, flush=True)
ranked = funcs if only else sorted((f for f in funcs if bases.get(f[2])), key=lambda f: bases[f[2]])[:n]
with open(os.path.join(out, "top.txt"), "w") as fh:
    for src, func, addr in ranked:
        fh.write("%s %s %s %s\n" % (src, func, addr, bases[addr]))
EOF
echo "functions to permute:"; cat "$OUT/top.txt"

# 2. every pass on its own (and, with --pairs, every ordered pair "a+b": millions of candidates)
PASSES=$("$BIN" --list-passes | grep -E '^  [a-z_0-9]+$' | tr -d ' ')
LIST=""
for a in $PASSES; do
    LIST="$LIST,$a"
    if [ -n "$PAIRS" ]; then
        for b in $PASSES; do
            [ "$a" != "$b" ] && LIST="$LIST,$a+$b"
        done
    fi
done
LIST="${LIST#,}"

# 3. run them
while read -r SRC FUNC ADDR BASE; do
    echo "=== $FUNC ($ADDR, base $BASE) ==="
    mkdir -p "$OUT/$ADDR"
    RESUME=""
    [ -f "$OUT/$ADDR/permuter_out/checkpoint.txt" ] && RESUME="--resume" && echo "resuming from checkpoint"
    (cd "$OUT/$ADDR" && "$ROOT/Scripts/permute.sh" "$ROOT/$SRC" "$FUNC" "$ADDR" \
        -m exhaustive -p "$LIST" --depth "$DEPTH" --max-candidates "$MAXC" $RESUME -j "$JOBS" \
        2>&1 | tee run.log)
done < "$OUT/top.txt"
echo "done; best candidates: ls -d $OUT/*/permuter_out/output-* | sort -t- -k2 -n"
