"""
Flags 16-bit globals whose offset in their object file has a different alignment (mod 4) than the original's address.

Why: VC6 copies a 2-byte integer (u16, s16, wchar_t, or an element of an array of them) from a global it defines in
the same TU into a 4-byte stack slot with a 32-bit move when it knows the address is 4-aligned
(`mov eax, DWORD PTR g; mov [esp+x], eax` instead of `mov ax, WORD PTR g; mov [esp+x], ax`). It knows the address
from the global's offset in the object's .bss/.data. A TU's uninitialized and constructor-initialized globals are laid
out in .bss in the order of C1XX's symbol hash (bucket_key() below), not by declaration. So renaming any global defined
in a TU can move the others and change code (docs/matching_quirks.md, "Even a global's name can change code"). Bytes
are never widened, and globals defined in another TU (extern) never are.

The original address comes from the `_gRef_<name>_0x<addr>` symbol that DEFINE_GLOBAL emits.

    python3 Scripts/bin_comp/global_align_check.py            # all objects in build_vc6
    python3 Scripts/bin_comp/global_align_check.py Hud.cpp    # one TU (after build.py or --single_cpp)
    python3 Scripts/bin_comp/global_align_check.py --key gChatFont_70643E word_70643E   # bucket of candidate names

Exit code 1 when something is flagged. A flagged global only changes code where it is copied to the stack as
described above, so check the functions that use it before acting on a flag.
"""
import glob, os, re, subprocess, sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
OBJDIR = os.path.join(ROOT, 'build_vc6', 'CMakeFiles', 'gta2_lib.dir', 'Source')
OBJDUMP = 'i686-w64-mingw32-objdump'

# MSVC decoration of a global's type after '@@3': 2-byte integers, plain or as a one-dimensional array
SCALAR16 = ('F', 'G', '_W')
ARRAY16 = ('PAF', 'PAG', 'PA_W')
SYM = re.compile(r'\(sec\s+(\d+)\).*\(scl\s+(\d+)\).*\s(0x[0-9a-f]+)\s+(\S+)$')

def bucket_key(name):
    # C1XX hashes identifiers with h = 4*h + c + (h >> 4) (lexer, 0x10409122) and keeps a scope's symbols in a
    # 1024-bucket table indexed by ((h >> 16) ^ h) & 0x3FF. It writes them to the IL in bucket order (within a bucket,
    # the later declaration first), and C2 lays out .bss in that order. Globals written `= 0` follow that run in
    # declaration order, then function-local statics; .data (non-zero initializers) is in declaration order. The
    # name is the C++ identifier, so DEFINE_GLOBAL's `gRef_<name>_<addr>` object sorts in the same run.
    h = 0
    for c in name.encode():
        h = (4 * h + c + (h >> 4)) & 0xffffffff
    return ((h >> 16) ^ h) & 0x3ff

def check(obj):
    out = subprocess.run([OBJDUMP, '-t', obj], capture_output=True, text=True).stdout
    defs, orig = {}, {}
    for line in out.splitlines():
        m = SYM.search(line)
        if not m:
            continue
        sec, scl, off, name = int(m[1]), int(m[2]), int(m[3], 16), m[4]
        if sec == 0:
            continue  # undefined (extern)
        g = re.match(r'_gRef_(.+)_0x([0-9a-fA-F]+)$', name)
        if g:
            orig[g[1]] = int(g[2], 16)
            continue
        d = re.match(r'\?(\w+)@@3(.+?)A$', name)
        if d and scl == 2:
            defs[d[1]] = (off, d[2])
    bad = []
    for name, (off, ty) in sorted(defs.items()):
        if name not in orig:
            continue
        addr = orig[name]
        if ty in SCALAR16:
            if (off % 4 == 0) != (addr % 4 == 0):
                bad.append((name, ty, off, addr, 'scalar'))
        elif ty in ARRAY16:
            if off % 4 != addr % 4:
                bad.append((name, ty, off, addr, 'array'))
    return bad

def main():
    if len(sys.argv) > 1 and sys.argv[1] == '--key':
        for n in sys.argv[2:]:
            print(f'{n}: {bucket_key(n)}')
        return
    objs = sorted(glob.glob(os.path.join(OBJDIR, '*.obj')))
    if len(sys.argv) > 1:
        objs = [o for o in objs if any(os.path.basename(o).startswith(a) for a in sys.argv[1:])]
    if not objs:
        sys.exit(f'no objects in {OBJDIR}: run build.py first')
    n = 0
    for obj in objs:
        for name, ty, off, addr, kind in check(obj):
            n += 1
            print(f'{os.path.basename(obj)}: {name} ({kind}) object offset {off:#x} (mod 4 = {off % 4}), '
                  f'original {addr:#x} (mod 4 = {addr % 4}), bucket {bucket_key(name)}')
    print(f'{n} misaligned 16-bit globals in {len(objs)} objects')
    sys.exit(1 if n else 0)

if __name__ == '__main__':
    main()
