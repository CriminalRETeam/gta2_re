"""
Builds build_vc6/x87_c2/opdump/ from the stock variant: a C2.DLL that prints a function's instructions with their
operands at the pass boundaries given (return addresses of the 0x107034AB calls in 0x107657E2, as in ildump's
@S lines; none = every boundary). Generated code is unchanged.

    @O <boundary> <function>
    @I <op>.<line>.<kind>.<addr>  D <operand>...  S <operand>...

Operands are printed for instructions of kind 12 (ordinary) and 14 only; other kinds (labels, jumps, entry/exit)
just get the @I part. D is the instruction's use list [ins+0x18], S its definition list [ins+0x1C] (both linked
through [op+0]). An operand is `<kind>/<[op+0x14]>/<[op+0x18]>`, and for a symbol reference (kind 2) or a placeholder
(kind 1 with [op+0x18] = 0x107AE040) also `/<symbol kind>/<[sym+0x30]>/<size>`. Operand kinds: 1 register value (an IL
temp: [op+0x18] is its own record until registers are assigned; the placeholder: a symbol that is a colour
candidate), 2 symbol in memory, 5 scale, 6 memory, 7 immediate. Symbol kinds: 3 compiler temp, 4 local, 5 parameter,
7 global. See README.md, "Which values get colour live ranges".

    venv/bin/python3 Scripts/regalloc/opdump.py 10765baa 10765bbd     # before and after 0x10711F93
    X87_C2=opdump X87_OUT=/tmp/od Scripts/x87_sched/sched.sh -l Source/Explosion_30.cpp
    awk '/^@O/{p=($0 ~ /sub_543690/)} p' /tmp/od/last.log
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'flowopt'))
import patch_c2 as P

STRS = 0x10798E00  # .text raw data ends at 0x10799000

def main():
    rets = [int(a, 16) for a in sys.argv[1:]]
    pe = pefile.PE(P.SRC)
    d = bytearray(open(P.SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = STRS
    strs = {}
    for k, s in [('S', b'@O %08x %s\n'), ('I', b'@I %x.%d.%d.%x'), ('D', b' D'), ('R', b' S'), ('P', b' %d/%x/%x'),
                 ('Q', b'/%d/%x/%x'), ('NL', b'\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    assert sva < 0x10799000
    cmps = '\n'.join(f"cmp eax, {r:#x}\n je go" for r in rets)
    # hook 0x107034AB (the boundary, esi = the function); first instruction and end as in ildump.py
    code = f"""
      pushfd
      pushad
      mov eax, dword ptr [esp+0x24]
      {cmps}
      {'jmp out' if rets else ''}
    go:
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
      push dword ptr [esp+0x28]
      {P.pr(strs['S'], 2)}
      mov ebp, 4000
    lp:
      test esi, esi
      je dn
      cmp esi, ebx
      je dn
      push esi
      movzx eax, byte ptr [esi+8]
      push eax
      movzx eax, word ptr [esi+0x10]
      push eax
      push dword ptr [esi+4]
      {P.pr(strs['I'], 4)}
      cmp byte ptr [esi+8], 12
      je doops
      cmp byte ptr [esi+8], 14
      jne skipops
    doops:
      push 0
      {P.pr(strs['D'], 1)}
      mov edi, dword ptr [esi+0x18]
      call opl
      push 0
      {P.pr(strs['R'], 1)}
      mov edi, dword ptr [esi+0x1c]
      call opl
    skipops:
      push 0
      {P.pr(strs['NL'], 1)}
      mov esi, dword ptr [esi]
      dec ebp
      jne lp
    dn:
      {P.flush()}
    out:
      popad
      popfd
      mov eax, dword ptr [0x107AC2E8]
      jmp 0x107034b0
    opl:
      test edi, edi
      je opr
      push dword ptr [edi+0x18]
      push dword ptr [edi+0x14]
      movzx eax, byte ptr [edi+8]
      push eax
      {P.pr(strs['P'], 3)}
      movzx eax, byte ptr [edi+8]
      cmp eax, 2
      je sym
      cmp eax, 1
      jne nx
      cmp dword ptr [edi+0x18], 0x107AE040
      jne nx
    sym:
      mov eax, dword ptr [edi+0x14]
      test eax, eax
      je nx
      push dword ptr [eax+0x20]
      push dword ptr [eax+0x30]
      movzx ecx, byte ptr [eax+4]
      push ecx
      {P.pr(strs['Q'], 3)}
    nx:
      mov edi, dword ptr [edi]
      jmp opl
    opr:
      ret
    """
    c = bytes(ks.asm(code, P.CAVE)[0]); assert P.CAVE + len(c) < STRS, len(c)
    put(P.CAVE, c)
    put(0x107034ab, bytes(ks.asm(f'jmp {P.CAVE:#x}', 0x107034ab)[0]))
    dst = os.path.join(P.X, 'opdump')
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'stock'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
