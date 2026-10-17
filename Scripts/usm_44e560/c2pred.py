"""c2pred.py NAME LINE: copy of build_vc6/inline_c2_cap0 (c2cap.py 0) whose predicate 0x107129B0 keeps the 0 of
`cmp X, 0` as an immediate for the compare at IL line LINE only, written to build_vc6/inline_c2_NAME.
Emulation only: it shows what one compare would change. Needs keystone-engine and pefile."""
import os, shutil, sys
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
CAVE = 0x10798F00
name, line = sys.argv[1], int(sys.argv[2])
src = os.path.join(ROOT, "build_vc6", "inline_c2_cap0")
p = os.path.join(src, "VC98", "Bin", "C2.DLL")
pe = pefile.PE(p)
d = bytearray(open(p, "rb").read())


def at(va):
    return pe.get_offset_from_rva(va - 0x10700000)


assert not any(d[at(CAVE):at(CAVE) + 64]) and d[at(0x10717AE6)] == 0xE8
ks = Ks(KS_ARCH_X86, KS_MODE_32)
cave = f"""
  cmp dword ptr [ecx+4], 0x2f
  jne orig
  cmp word ptr [ecx+0x10], {line}
  jne orig
  xor eax, eax
  ret
orig:
  jmp 0x107129B0
"""
code = bytes(ks.asm(cave, CAVE)[0])
d[at(CAVE):at(CAVE) + len(code)] = code
call = bytes(ks.asm(f"call {CAVE:#x}", 0x10717AE6)[0])
d[at(0x10717AE6):at(0x10717AE6) + len(call)] = call
dst = os.path.join(ROOT, "build_vc6", "inline_c2_" + name)
shutil.rmtree(dst, ignore_errors=True)
shutil.copytree(src, dst, symlinks=True)
out = os.path.join(dst, "VC98", "Bin", "C2.DLL")
os.remove(out)
open(out, "wb").write(d)
print(dst)
