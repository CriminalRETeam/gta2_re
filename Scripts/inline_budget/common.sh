# Shared by inl.sh and probe.sh: paths and the wine environment for the instrumented compiler.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
HERE="$ROOT/Scripts/inline_budget"
RT="$ROOT/3rdParty/gta2_re_compile_tools"
IC2="$ROOT/build_vc6/inline_c2"
winpath() { printf 'Z:%s' "$(realpath -m "$1")" | tr / '\\'; }
[ -f "$IC2/VC98/Bin/C2.DLL" ] || python3 "$HERE/setup.py" >&2 || exit 1
export WINEDEBUG=-all
export WINEPATH="$(winpath "$IC2/VC98/Bin");$(winpath "$RT/Common/MSDev98/Bin")"
