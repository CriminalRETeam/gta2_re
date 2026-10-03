#!/bin/bash
# Downloads decomp.me's VC6 service-pack builds and VC7.0 (the 9.6f compiler) into build_vc6/compilers/.
#   msvc6.0 RTM (8168), msvc6.3 SP3, msvc6.4 SP4 (= our toolchain), msvc6.5 SP5, msvc6.5pp, msvc6.6 SP6
#   msvc7.0 = 13.00.9466, the exact build in 9.6f.exe's Rich header
. "$(dirname "$0")/common.sh"
mkdir -p "$COMPILERS" && cd "$COMPILERS"
for v in 6.0 6.3 6.4 6.5 6.5pp 6.6; do
  [ -d msvc$v ] && continue
  curl -sfL -o msvc$v.tar.gz https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc$v.tar.gz &&
    mkdir -p msvc$v && tar xzf msvc$v.tar.gz -C msvc$v && rm msvc$v.tar.gz && echo "msvc$v ok"
done
if [ ! -d msvc7.0 ]; then
  curl -sfL -o msvc7.0.tar.gz https://github.com/roblabla/MSVC-7.0-Portable/releases/download/release/msvc7.0.tar.gz &&
    mkdir -p msvc7.0 && tar xzf msvc7.0.tar.gz -C msvc7.0 && rm msvc7.0.tar.gz && echo "msvc7.0 ok"
fi
