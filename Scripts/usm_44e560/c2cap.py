"""c2cap.py K: copy of build_vc6/inline_c2 (Scripts/inline_budget/setup.py) whose global optimizer tracks
0x400 + K entries per function instead of 0x400, written to build_vc6/inline_c2_capK. K=0 is the stock limit."""
import os, shutil, sys
import pefile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
K = int(sys.argv[1])
src = os.path.join(ROOT, "build_vc6", "inline_c2")
dst = os.path.join(ROOT, "build_vc6", "inline_c2_cap%d" % K)
shutil.rmtree(dst, ignore_errors=True)
shutil.copytree(src, dst, symlinks=True, ignore=shutil.ignore_patterns("*.log", "*.obj"))
p = os.path.join(dst, "VC98", "Bin", "C2.DLL")
d = bytearray(open(p, "rb").read())
pe = pefile.PE(p)
v = (0x400 + K).to_bytes(4, "little")
for va, op in ((0x10755778, "3d"), (0x1073AD34, "b8")):  # cmp eax,400h / mov eax,400h
    o = pe.get_offset_from_rva(va - 0x10700000)
    assert d[o:o + 5] == bytes.fromhex(op + "00040000"), d[o:o + 5].hex()
    d[o + 1:o + 5] = v
os.remove(p)
open(p, "wb").write(d)
print(dst)
