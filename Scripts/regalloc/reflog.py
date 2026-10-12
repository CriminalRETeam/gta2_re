"""
Builds build_vc6/x87_c2/reflog/ from the stock variant: slotlog.py plus one line per symbol reference the
frame pass's walk (0x10724286 -> 0x1072EE45) counts, in walk order. That is the order that decides the
tie-break of the stack slot list (README.md, "Stack slots"): of two same-size symbols with the same final
count, the one that reached that count first in this walk comes first and gets the lower address.
Generated code is unchanged.

    @REF <name>                                  one per counted operand, in walk (block list) order
    @SLOTS <function>                            printed after the function's walk
    @SLOT <name> size=<bytes> refs=<count>       the resulting list

    venv/bin/python3 Scripts/regalloc/reflog.py
    X87_C2=reflog X87_OUT=/tmp/rl Scripts/x87_sched/sched.sh -l Source/Frontend.cpp
    # the @REF lines of a function are the ones between the previous function's @SLOTS line and its own
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'flowopt'))
import patch_c2 as P

SYMNAME = 0x10731B0E  # ecx = symbol -> eax = listing name, in a static buffer

def main():
    var = sys.argv[1] if len(sys.argv) > 1 else 'reflog'
    pe = pefile.PE(P.SRC)
    d = bytearray(open(P.SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = P.CAVE + 0x500
    strs = {}
    for k, s in [('F', b'@SLOTS %s\n'), ('S', b'@SLOT %s size=%d refs=%d\n'), ('Q', b'?'), ('R', b'@REF %s\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    # hook 0x1072401A (mov eax,[0x1079F20C], 5 bytes) in 0x10724009, esi = the function: print the list
    code = f"""
      pushfd
      pushad
      mov ecx, dword ptr [esi]
      test word ptr [ecx+0x36], 0x1000
      je direct
      call {P.INDIR:#x}
      mov ecx, eax
    direct:
      mov edx, 1
      call {P.GETNAME:#x}
      push eax
      {P.pr(strs['F'], 1)}
      mov ebx, dword ptr [0x1079F220]
    lp:
      test ebx, ebx
      je dn
      push dword ptr [ebx+0x34]
      push dword ptr [ebx+0x20]
      mov ecx, dword ptr [ebx]
      mov eax, {strs['Q']:#x}
      test ecx, ecx
      je noname
      cmp byte ptr [ecx+4], 1
      jne noname
      call {SYMNAME:#x}
    noname:
      push eax
      {P.pr(strs['S'], 3)}
      mov ebx, dword ptr [ebx+0x2c]
      jmp lp
    dn:
      {P.flush()}
      popad
      popfd
      mov eax, dword ptr [0x1079F20C]
      jmp 0x1072401f
    """
    c = bytes(ks.asm(code, P.CAVE)[0]); assert len(c) < 0x300, len(c)
    put(P.CAVE, c)
    put(0x1072401a, bytes(ks.asm(f'jmp {P.CAVE:#x}', 0x1072401a)[0]))
    # hook 0x1072EE45 (push ebp; push esi; push edi; mov edi,edx: 5 bytes): ecx = the list record, [ecx] = symbol
    cave2 = P.CAVE + 0x300
    code2 = f"""
      pushfd
      pushad
      mov eax, {strs['Q']:#x}
      test ecx, ecx
      je noname
      mov ecx, dword ptr [ecx]
      test ecx, ecx
      je noname
      cmp byte ptr [ecx+4], 1
      jne noname
      call {SYMNAME:#x}
    noname:
      push eax
      {P.pr(strs['R'], 1)}
      popad
      popfd
      push ebp
      push esi
      push edi
      mov edi, edx
      jmp 0x1072ee4a
    """
    c2 = bytes(ks.asm(code2, cave2)[0]); assert len(c2) < 0x200, len(c2)
    put(cave2, c2)
    put(0x1072ee45, bytes(ks.asm(f'jmp {cave2:#x}', 0x1072ee45)[0]))
    dst = os.path.join(P.X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'stock'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    if os.path.islink(out):
        os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
