"""
Builds the instrumented VC6 compiler used by inl.sh / probe.sh in build_vc6/inline_c2/ (git-ignored):
VC98/Bin with symlinks to the real toolchain and a C2.DLL patched with c2_patch.json. No dependencies.

    python3 Scripts/inline_budget/setup.py
"""
import hashlib, json, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
BIN = os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools', 'VC98', 'Bin')
OUT = os.path.join(ROOT, 'build_vc6', 'inline_c2', 'VC98', 'Bin')
SP4_SHA256 = '5649bd68ed833fcf4396c4a0a834e94ab1ec162316b3cbc3d8a9beabdc54e517'


def main():
    src = os.path.join(BIN, 'C2.DLL')
    data = bytearray(open(src, 'rb').read())
    if hashlib.sha256(data).hexdigest() != SP4_SHA256:
        sys.exit('unexpected C2.DLL (the patch is for VC6 SP4, 12.00.8804): ' + src)
    for off, hexbytes in json.load(open(os.path.join(HERE, 'c2_patch.json')))['runs']:
        b = bytes.fromhex(hexbytes)
        data[off:off + len(b)] = b
    os.makedirs(OUT, exist_ok=True)
    for name in os.listdir(BIN):
        dst = os.path.join(OUT, name)
        if os.path.lexists(dst):
            os.remove(dst)
        if name.upper() != 'C2.DLL':
            os.symlink(os.path.join(BIN, name), dst)
    open(os.path.join(OUT, 'C2.DLL'), 'wb').write(data)
    print('instrumented compiler in', OUT)


if __name__ == '__main__':
    main()
