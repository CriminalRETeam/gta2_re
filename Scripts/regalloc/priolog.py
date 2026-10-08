"""
Builds build_vc6/x87_c2/priolog/ from the ralog variant (patch_c2.py): ralog's colour log plus how C2 computes each
live range's priority [lr+0x0C] (0x107203FD) and tie-break [lr+0x40] (0x1071A7EE). Generated code is unchanged.

    @P enter                              0x107203FD starts (once per function, before the colour pass)
    @P ref lr=<lr> c=<saving> w=<weight>  a reference: weight [lr+0x3C] += saving * block weight
    @P blk N=<n> bw=<bw>                  end of a block: n live ranges referenced in it, block weight 2^loop depth
    @P add lr=<lr> prio=<new> +<d>        a referenced live range: prio += n * bw * (its savings in the block)
    @P add2 ...                           the same, other list
    @P sub lr=<lr> <d>                    live through the block but not referenced: prio -= n * bw
    @T lr=<lr> cnt=<n> line=<line>        a definition binds the live range: tie = n (see README.md)

<lr> is the live range record ([lr] is ralog's l0); @R lines (ralog) give the final priority, tie and register.

    venv/bin/python3 Scripts/regalloc/patch_c2.py && venv/bin/python3 Scripts/regalloc/priolog.py
    X87_C2=priolog X87_OUT=/tmp/pl Scripts/x87_sched/sched.sh -l Scripts/regalloc/probe_loop.cpp
    grep -a '^@[PTR]' /tmp/pl/last.log
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'flowopt'))
import patch_c2 as P

CODE = 0x10798A00  # after ralog's caves (0x10798400..0x10798600)
STRS = 0x10798E00  # .text raw data ends at 0x10799000

def main():
    var = sys.argv[1] if len(sys.argv) > 1 else 'priolog'
    src = os.path.join(P.X, 'ralog', 'VC98', 'Bin', 'C2.DLL')
    if not os.path.exists(src):
        sys.exit('build the ralog variant first (Scripts/regalloc/patch_c2.py)')
    pe = pefile.PE(src)
    d = bytearray(open(src, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = STRS
    strs = {}
    for k, s in [('E', b'@P enter\n'), ('B', b'@P blk N=%d bw=%d\n'), ('A', b'@P add lr=%x prio=%d +%d\n'),
                 ('A2', b'@P add2 lr=%x prio=%d +%d\n'), ('M', b'@P sub lr=%x %d\n'),
                 ('W', b'@P ref lr=%x c=%d w=%d\n'), ('T', b'@T lr=%x cnt=%d line=%d\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    assert sva < 0x10799000
    # (address, bytes replaced, pushes for the printf, format, nargs, replaced instructions, return address)
    # esp-relative operands in the pushes are shifted by pushfd + pushad (0x24).
    hooks = [
        (0x107203fd, 5, "", 'E', 0, "sub esp,0x2c\n push ebx\n push ebp", 0x10720402),
        (0x10720545, 5, "push dword ptr [esp+0x24+0x24]\n push esi", 'B', 2, "imul esi,[esp+0x24]", 0x1072054a),
        (0x10720cf4, 6, "push edi\n push ebx\n push eax", 'A', 3, "mov [eax+0xc],ebx\n mov bl,[eax+6]", 0x10720cfa),
        (0x10720d16, 7, "push ecx\n push edx\n push eax", 'A2', 3, "mov dword ptr [eax+0x18],0", 0x10720d1d),
        (0x10720d33, 5, "push esi\n push eax", 'M', 2, "sub [eax+0xc],esi", 0x10720d20),
        (0x1072095c, 5, "push ebx\n push eax\n push esi", 'W', 3, "mov [esi+0x3c],ebx\n add edx,eax", 0x10720961),
        (0x10720bcd, 6, "push ebx\n push edi\n push esi", 'W', 3, "mov [esi+0x3c],ebx\n mov [esi+0x18],edx", 0x10720bd3),
        # 0x1071A7EE: mov [edi+0x40],eax (eax = instruction counter, edi = live range, [esp+0x14] = instruction)
        (0x1071a7ee, 6, "mov ecx,[esp+0x24+0x14]\n movzx ecx, word ptr [ecx+0x10]\n push ecx\n push eax\n push edi",
         'T', 3, "mov [edi+0x40],eax\n mov eax,[ecx+4]", 0x1071a7f4),
    ]
    va = CODE
    for at, n, pushes, k, na, orig, back in hooks:
        code = f"""
          pushfd
          pushad
          {pushes}
          {P.pr(strs[k], na)}
          {P.flush()}
          popad
          popfd
          {orig}
          jmp {back:#x}
        """
        c = bytes(ks.asm(code, va)[0])
        put(va, c)
        j = bytes(ks.asm(f'jmp {va:#x}', at)[0])
        put(at, j + b'\x90' * (n - len(j)))
        va += len(c)
    assert va < STRS, hex(va)
    dst = os.path.join(P.X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'ralog'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
