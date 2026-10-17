"""
Builds build_vc6/x87_c2/sinklog/ from the ildump variant (run ildump.py first): ildump's pass-boundary dump plus a
log of the loop sink pass (0x10740251, which moves blocks that are not part of a loop out of the loop body).
Generated code is unchanged.

    @SINK run <first id>/<index>..<last id>/<index> after <id>/<index>   a run of non-loop blocks is unlinked
    @SINKCMP blk <id>/<index> runidx <index>                           a block after the loop it is compared with
    @SINKINS before <id>/<index>                                       the run is inserted before this block

<index> is the block's layout index [blk+0x6C] (0x107048CA numbers the blocks in list order; -1 for a block created
after that). <id> is the low 16 bits of the block's first instruction, as in ildump's @S lines.

    venv/bin/python3 Scripts/flowopt/ildump.py && venv/bin/python3 Scripts/flowopt/sinklog.py
    X87_C2=sinklog X87_OUT=/tmp/sk Scripts/x87_sched/sched.sh -l Source/Explosion_30.cpp
    # the @SINK lines of a function come just before its "@S 107659c0" line (the boundary after 0x10706181)
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import patch_c2 as P

CODE = 0x10798D00  # after ildump's cave (P.CAVE .. +0x600, strings after it)
STRS = 0x10798E40  # .text raw data ends at 0x10799000

def main():
    var = sys.argv[1] if len(sys.argv) > 1 else 'sinklog'
    src = os.path.join(P.X, 'ildump', 'VC98', 'Bin', 'C2.DLL')
    if not os.path.exists(src):
        sys.exit('build the ildump variant first (ildump.py)')
    pe = pefile.PE(src)
    d = bytearray(open(src, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - P.BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    sva = STRS
    strs = {}
    for k, s in [('R', b'@SINK run %x/%d..%x/%d after %x/%d\n'), ('C', b'@SINKCMP blk %x/%d runidx %d\n'),
                 ('I', b'@SINKINS before %x/%d\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    assert sva < 0x10799000

    def blk(reg):  # pushes index, then id: printed as id/index
        return f"""
          movsx eax, word ptr [{reg}+0x6c]
          push eax
          mov eax, dword ptr [{reg}+0x1c]
          mov eax, dword ptr [eax]
          and eax, 0xffff
          push eax
        """
    # 0x107402D7: esi = last loop block before the run, ebp = first block of the run, ebx = its last block
    # 0x10740355: compare the run's index with the block esi after the loop
    # 0x10740302: insert the run before esi
    hooks = [
        ('h1', 0x107402d7, 6, f"""
          {blk('esi')}
          {blk('ebx')}
          {blk('ebp')}
          {P.pr(strs['R'], 6)}
        """, "mov ecx,[esp+0x14]\n mov edx,[ebx]", 0x107402dd),
        ('h2', 0x10740355, 8, f"""
          movsx eax, word ptr [ebp+0x6c]
          push eax
          {blk('esi')}
          {P.pr(strs['C'], 3)}
        """, "mov di,[ebp+0x6c]\n mov ax,[esi+0x6c]", 0x1074035d),
        ('h3', 0x10740302, 6, f"""
          {blk('esi')}
          {P.pr(strs['I'], 2)}
        """, "mov edi,[esp+0x10]\n test esi,esi", 0x10740308),
    ]
    va = CODE
    for name, at, n, body, orig, back in hooks:
        code = f"""
          pushfd
          pushad
          {body}
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
    assert va < STRS
    dst = os.path.join(P.X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(os.path.join(P.X, 'ildump'), dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
