"""
Builds build_vc6/x87_c2/slotlog/ from the stock variant: a C2.DLL that prints, per function, the stack symbol list
the frame pass (0x10723F8C -> 0x10724009) builds before it assigns slots. Generated code is unchanged.

    @SLOTS <function>
    @SLOT <name> size=<bytes> refs=<count>      in list order: the first one gets the lowest address

The list (0x1079F220: records whose [+0] is the symbol, linked through [+0x2C], size [+0x20], count [+0x34]) is built by 0x1072EE45, called for every symbol operand of every
instruction in IL order (0x10724286). Order: size ascending, then reference count descending (each operand counts
1, loops don't weigh more), then first reference. See README.md, "Stack slots".

    venv/bin/python3 Scripts/regalloc/slotlog.py
    X87_C2=slotlog X87_OUT=/tmp/sl Scripts/x87_sched/sched.sh -l Source/CarAI_78.cpp
    awk '/^@SLOTS/{p=index($0,"sub_452060")} p' /tmp/sl/last.log | grep -a '^@SLOT'
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'flowopt'))
import patch_c2 as P

SYMNAME = 0x10731B0E  # ecx = symbol -> eax = listing name (as in the "%s = %ld" lines), in a static buffer

def main():
    var = sys.argv[1] if len(sys.argv) > 1 else 'slotlog'
    pe = pefile.PE(P.SRC)
    d = bytearray(open(P.SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = P.CAVE + 0x400
    strs = {}
    for k, s in [('F', b'@SLOTS %s\n'), ('S', b'@SLOT %s size=%d refs=%d\n'), ('Q', b'?')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    # hook 0x1072401A (mov eax,[0x1079F20C], 5 bytes) in 0x10724009, esi = the function
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
    c = bytes(ks.asm(code, P.CAVE)[0]); assert len(c) < 0x400
    put(P.CAVE, c)
    put(0x1072401a, bytes(ks.asm(f'jmp {P.CAVE:#x}', 0x1072401a)[0]))
    dst = os.path.join(P.X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'stock'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
