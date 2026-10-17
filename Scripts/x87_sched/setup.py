"""
Builds the scheduler-logging VC6 compilers used by sched.sh / regsearch.py in build_vc6/x87_c2/
(git-ignored): VC98/Bin dirs with symlinks to the real toolchain and a C2.DLL patched with
c2_patch.json. No dependencies.

    python3 Scripts/x87_sched/setup.py                         # build_vc6/x87_c2/{log,fast}, stock window limit
    python3 Scripts/x87_sched/setup.py --variant fast --out DIR --lim SPEC   # one copy with a limit table

SPEC (what-if window limits; a window holds at most limit + 1 nodes, stock 80):
    69              every function, every window
    12:70,70,69     the 12th scheduled function (the #12 of `@F #12` in the log), per window; later windows 80
VC6_TOOLS overrides the compiler location (default 3rdParty/gta2_re_compile_tools).
"""
import argparse, hashlib, json, os, shutil, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
RT = os.environ.get('VC6_TOOLS') or os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools')
BIN = os.path.join(RT, 'VC98', 'Bin')
OUT = os.path.join(ROOT, 'build_vc6', 'x87_c2')
SP4_SHA256 = '5649bd68ed833fcf4396c4a0a834e94ab1ec162316b3cbc3d8a9beabdc54e517'


def parse_lim(spec):
    """'' -> (0, []) stock; '69' -> (-1, [69]); '12:70,69' -> (12, [70, 69])."""
    if not spec:
        return 0, []
    if ':' in spec:
        f, l = spec.split(':', 1)
        return int(f.lstrip('#')), [int(x) for x in l.split(',') if x]
    return -1, [int(spec)]


_stock = None


def stock():
    global _stock
    if _stock is None:
        src = os.path.join(BIN, 'C2.DLL')
        if not os.path.exists(src):
            sys.exit('no C2.DLL in %s (set VC6_TOOLS to a gta2_re_compile_tools checkout)' % BIN)
        _stock = open(src, 'rb').read()
        if hashlib.sha256(_stock).hexdigest() != SP4_SHA256:
            sys.exit('unexpected C2.DLL (the patch is for VC6 SP4, 12.00.8804): ' + src)
    return _stock


def make(variant, out_bin, lim=''):
    """Write out_bin/ (a VC98/Bin dir) with the toolchain symlinked and a patched C2.DLL."""
    cfg = json.load(open(os.path.join(HERE, 'c2_patch.json')))
    data = bytearray(stock())
    if variant != 'stock':   # 'stock': the unpatched DLL (for drc/dr.sh coverage runs)
        v = cfg[variant]
        for off, hexbytes in v['runs']:
            b = bytes.fromhex(hexbytes)
            data[off:off + len(b)] = b
        ordinal, lims = parse_lim(lim)
        if len(lims) > cfg['maxlim']:
            sys.exit('at most %d window limits' % cfg['maxlim'])
        tab = struct.pack('<ii', ordinal, len(lims)) + b''.join(struct.pack('<i', x) for x in lims)
        data[v['limtab']:v['limtab'] + len(tab)] = tab
    elif lim:
        sys.exit('the stock variant has no window limit')
    os.makedirs(out_bin, exist_ok=True)
    for name in os.listdir(BIN):
        dst = os.path.join(out_bin, name)
        if name.upper() == 'C2.DLL':
            continue
        src = os.path.join(BIN, name)
        if os.path.lexists(dst):
            if os.path.islink(dst) and os.readlink(dst) == src:
                continue
            if not os.path.islink(dst) and os.path.getmtime(dst) == os.path.getmtime(src):
                continue
            os.remove(dst)
        try:
            os.symlink(src, dst)
        except OSError:
            # Windows without the symlink privilege (or developer mode).
            shutil.copy2(src, dst)
    trim_text_vsize(data)
    dll = os.path.join(out_bin, 'C2.DLL')
    if not (os.path.exists(dll) and open(dll, 'rb').read() == data):
        tmp = dll + '.tmp'
        open(tmp, 'wb').write(data)
        os.replace(tmp, dll)
    return out_bin


def trim_text_vsize(data):
    """Keep .text's VirtualSize within its raw size.

    The hooks live in .text's slack space and the patch grows VirtualSize to cover them, which
    makes .text overlap the next section's virtual address. Wine maps that image anyway; the
    Windows loader refuses it, and CL then falls back to spawning C2.DLL as a process and dies
    with "D2027 : cannot execute c2.dll". The hooks are below the raw end either way.
    """
    e = struct.unpack_from('<I', data, 0x3C)[0]
    sec = e + 24 + struct.unpack_from('<H', data, e + 20)[0]
    vsize, _va, rawsize, _raw = struct.unpack_from('<IIII', data, sec + 8)
    if vsize > rawsize:
        struct.pack_into('<I', data, sec + 8, rawsize)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--variant', choices=('log', 'fast', 'stock'))
    ap.add_argument('--out', help='directory that gets VC98/Bin')
    ap.add_argument('--lim', default='', help='window limit spec, see the module docstring')
    a = ap.parse_args()
    if a.variant:
        print(make(a.variant, os.path.join(a.out or os.path.join(OUT, a.variant), 'VC98', 'Bin'), a.lim))
        return
    for variant in ('log', 'fast'):
        make(variant, os.path.join(OUT, variant, 'VC98', 'Bin'))
    print('scheduler compilers in', OUT)


if __name__ == '__main__':
    main()
