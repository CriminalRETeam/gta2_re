"""
Builds build_vc6/x87_c2/<variant>/ (default ildump) from the stock variant: a C2.DLL that prints a function's
instruction list at every pass boundary (0x107034AB, called between the passes of the pipeline at 0x107657E2,
esi = the function). Generated code is unchanged.

    @S <return address> <first instr> <function>: kind.op.line.id ...

id: a label's own address and a jump's target label (low 16 bits), other instructions their address.

The return address names the pass that just ran (the call before it in 0x107657E2). `ilshow.py` prints only
the boundaries where the list changed.
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import patch_c2 as P

def main():
    var = sys.argv[1] if len(sys.argv) > 1 else 'ildump'
    pe = pefile.PE(P.SRC)
    d = bytearray(open(P.SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = P.CAVE + 0x600
    strs = {}
    for k, s in [('S', b'@S %08x %08x %s:'), ('I', b' %d.%d.%d.%x'), ('NL', b'\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    # first instruction: [[[f+8]]+0x1c] -> next; end: [[f+8]+4]+0x20 (as FlowOpts' sweep 0x10725478)
    code = f"""
      pushfd
      pushad
      mov ebp, esi
      test ebp, ebp
      je out
      mov eax, dword ptr [ebp+8]
      test eax, eax
      je out
      mov edx, dword ptr [eax+4]
      test edx, edx
      je out
      mov ebx, dword ptr [edx+0x20]
      mov eax, dword ptr [eax]
      test eax, eax
      je out
      mov eax, dword ptr [eax+0x1c]
      test eax, eax
      je out
      mov esi, dword ptr [eax]
      mov ecx, dword ptr [ebp]
      test word ptr [ecx+0x36], 0x1000
      je direct
      call {P.INDIR:#x}
      mov ecx, eax
    direct:
      mov edx, 1
      call {P.GETNAME:#x}
      push eax
      push esi
      push dword ptr [esp+0x2c]
      {P.pr(strs['S'], 3)}
      mov edi, 4000
    lp:
      test esi, esi
      je dn
      cmp esi, ebx
      je dn
      movzx eax, byte ptr [esi+8]
      mov ecx, esi
      cmp al, 0x11
      jne notj
      xor ecx, ecx
      mov edx, dword ptr [esi+0x18]
      test edx, edx
      je notj
      cmp byte ptr [edx+8], 4
      jne notj
      mov edx, dword ptr [edx+0x14]
      mov ecx, dword ptr [edx+0x32]
    notj:
      and ecx, 0xffff
      push ecx
      push dword ptr [esi+0x10]
      push dword ptr [esi+4]
      push eax
      {P.pr(strs['I'], 4)}
      mov esi, dword ptr [esi]
      dec edi
      jne lp
    dn:
      push 0
      {P.pr(strs['NL'], 1)}
      {P.flush()}
    out:
      popad
      popfd
      mov eax, dword ptr [0x107AC2E8]
      jmp 0x107034b0
    """
    c = bytes(ks.asm(code, P.CAVE)[0]); assert len(c) < 0x600
    put(P.CAVE, c)
    put(0x107034ab, bytes(ks.asm(f'jmp {P.CAVE:#x}', 0x107034ab)[0]))
    dst = os.path.join(P.X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'stock'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
