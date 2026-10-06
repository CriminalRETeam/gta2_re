# Shared by sched.sh and regsearch.py: paths and the wine environment for the scheduler-logging compiler.
# VC6_TOOLS overrides the toolchain (default: this checkout's 3rdParty/gta2_re_compile_tools).
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
HERE="$ROOT/Scripts/x87_sched"
RT="${VC6_TOOLS:-$ROOT/3rdParty/gta2_re_compile_tools}"
export VC6_TOOLS="$RT"
X="$ROOT/build_vc6/x87_c2"
winpath() { printf 'Z:%s' "$(realpath -m "$1")" | tr / '\\'; }
[ -f "$X/log/VC98/Bin/C2.DLL" ] && [ -f "$X/fast/VC98/Bin/C2.DLL" ] || python3 "$HERE/setup.py" >&2 || exit 1
export WINEDEBUG=-all
