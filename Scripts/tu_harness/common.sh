# Sourced by the tu_harness scripts.
REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
WORK=${TU_WORK:-$REPO/build_vc6/tu_harness}      # git-ignored
COMPILERS=$REPO/build_vc6/compilers               # filled by fetch_compilers.sh
PY=${PY:-$REPO/venv/bin/python3}
mkdir -p "$WORK"
winpath() { echo "Z:$(echo "$1" | sed 's#/#\\#g')"; }
