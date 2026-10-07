"""
Builds a logging copy of the VC6 (SP4, 12.00.8804) C2.DLL for cross-jumping (tail merging):
build_vc6/x87_c2/<variant>/VC98/Bin/C2.DLL (default variant: cjlog), so
`X87_C2=cjlog Scripts/x87_sched/sched.sh -l file.cpp` compiles with it. Generated code is unchanged.
Needs `pip install pefile keystone-engine`.

Hooks (stderr):
    @F <name>                       colour pass entry (after the flow passes of that function)
    @J a=<blk> b=<blk> pass=<n>     CrossJump(a, b) at 0x1073049c: try to merge a's tail into b
       then each block's instructions from the last back: kind:op ...
    @K jumper=<blk> target=<blk> n=<matched>   a merge was done (0x10730739)
"""
import os, sys, shutil
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.join(HERE, '..', '..')
SRC = os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools', 'VC98', 'Bin', 'C2.DLL')
X = os.path.join(ROOT, 'build_vc6', 'x87_c2')
BASE = 0x10700000
IOB, FFLUSH, VFPRINTF = 0x107a0074, 0x107a0094, 0x107a00c4
GETNAME, INDIR = 0x1073ffc7, 0x10783bcf
CAVE = 0x10798400
NODES = 24

def pr(fmt, nargs):
    # print fmt with nargs dwords already pushed (last pushed = first arg)
    return f"""
      mov eax, esp
      push eax
      push {fmt:#x}
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{VFPRINTF:#x}]
      add esp, {12 + 4 * nargs}
    """

def flush():
    return f"""
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{FFLUSH:#x}]
      add esp, 4
    """

RANK = None  # --rank line:rank,...
REV = None   # --rev [min:max]: reverse the predecessor lists (with min..max entries) before cross-jumping (experiments)

def main():
    global REV, RANK
    args = sys.argv[1:]
    if '--rank' in args:
        # --rank line:rank,...: sort every predecessor list by rank (lowest first) of the jump's line
        # field (low byte; unlisted lines rank 128, ties keep their order)
        i = args.index('--rank')
        RANK = {int(a): int(b) for a, b in (kv.split(':') for kv in args[i + 1].split(','))}
        del args[i:i + 2]
    if '--rev' in args:
        # --rev [min:max]: only lists with min..max predecessors
        i = args.index('--rev'); REV = (2, 1 << 30)
        if i + 1 < len(args) and ':' in args[i + 1]:
            REV = tuple(int(v) for v in args[i + 1].split(':')); del args[i + 1]
        del args[i]
    var = args[0] if args else 'cjlog'
    stock = os.path.join(X, 'stock')
    if not os.path.isdir(stock):
        sys.exit('build_vc6/x87_c2/stock missing: run Scripts/x87_sched/setup.py --variant stock first')
    pe = pefile.PE(SRC)
    d = bytearray(open(SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - BASE)
        d[o:o + len(b)] = b
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    strs = {}
    sva = CAVE + 0x600
    for k, s in [('J', b'@J a=%08x line %d  b=%08x line %d  pass=%d\n'), ('P', b'@P join=%08x/%d preds:'), ('E', b' %08x/%d/%x'), ('B', b'   %08x:'), ('I', b' %x:%d'),
                 ('NL', b'\n'), ('K', b'@K jumper=%08x target=%08x n=%d\n'), ('F', b'@F %s\n')]:
        strs[k] = sva; put(sva, s + b'\0'); sva += len(s) + 1
    # print block: esi = block
    pb = f"""
    pblk:
      push esi
      {pr(strs['B'], 1)}
      mov ecx, {NODES}
      mov esi, dword ptr [esi+0xc]
    pl:
      test esi, esi
      je pdone
      push ecx
      push dword ptr [esi+4]
      movzx eax, byte ptr [esi+8]
      push eax
      {pr(strs['I'], 2)}
      pop ecx
      cmp byte ptr [esi+8], 0x1a
      je pdone
      mov esi, dword ptr [esi+0xc]
      dec ecx
      jne pl
    pdone:
      push 0
      {pr(strs['NL'], 1)}
      ret
    """
    j = f"""
      pushfd
      pushad
      push dword ptr [esp+0x24+4]
      push dword ptr [edx+0x10]
      push edx
      push dword ptr [ecx+0x10]
      push ecx
      {pr(strs['J'], 5)}
      mov esi, dword ptr [esp+0x18]
      call pblk
      mov esi, dword ptr [esp+0x14]
      call pblk
      {flush()}
      popad
      popfd
      sub esp, 0x20
      push ebx
      push ebp
      jmp 0x107304a1
    {pb}
    """
    jc = bytes(ks.asm(j, CAVE)[0]); assert len(jc) < 0x200
    put(CAVE, jc)
    kc_va = CAVE + 0x200
    k = f"""
      pushfd
      pushad
      push dword ptr [esp+0x24+0x18]
      push ebx
      push ebp
      {pr(strs['K'], 3)}
      {flush()}
      popad
      popfd
      mov eax, 1
      jmp 0x1073073e
    """
    kc = bytes(ks.asm(k, kc_va)[0]); assert len(kc) < 0x100
    put(kc_va, kc)
    pc_va = CAVE + 0x500
    pc = f"""
      pushfd
      pushad
      {{rev}}
      push dword ptr [ecx+0x10]
      push ecx
      {pr(strs['P'], 2)}
      mov ebx, dword ptr [esp+0x18]
      mov ebx, dword ptr [ebx+0x1c]
    pel:
      test ebx, ebx
      je ped
      mov eax, dword ptr [ebx+0xc]
      movzx ecx, byte ptr [eax+8]
      push ecx
      push dword ptr [eax+0x10]
      push eax
      {pr(strs['E'], 3)}
      mov ebx, dword ptr [ebx]
      jmp pel
    ped:
      push 0
      {pr(strs['NL'], 1)}
      {flush()}
      popad
      popfd
      sub esp, 0xc
      mov eax, dword ptr [ecx+0x1c]
      jmp 0x1072f52f
    """
    rev = ''
    if REV:
        rev = f'''
      mov ebx, dword ptr [ecx+0x1c]
      xor edx, edx
    cnl:
      test ebx, ebx
      je cnd
      inc edx
      mov ebx, dword ptr [ebx]
      jmp cnl
    cnd:
      cmp edx, {REV[0]}
      jb rvx
      cmp edx, {REV[1]}
      ja rvx
      mov ebx, dword ptr [ecx+0x1c]
      xor edx, edx
    rvl:
      test ebx, ebx
      je rvd
      mov eax, dword ptr [ebx]
      mov dword ptr [ebx], edx
      mov edx, ebx
      mov ebx, eax
      jmp rvl
    rvd:
      mov dword ptr [ecx+0x1c], edx
    rvx:
      '''
    if RANK:
        tbl = CAVE + 0x700
        t = bytearray([0x80] * 256)
        for ln, r in RANK.items():
            t[ln & 0xff] = r
        put(tbl, bytes(t))
        # selection sort: repeatedly move the (last) max-rank edge to the front of a new list
        rev += f'''
      push ecx
      mov edi, ecx
      xor ebp, ebp
    sso:
      mov esi, dword ptr [edi+0x1c]
      test esi, esi
      je ssd
      xor ebx, ebx
      mov edx, -1
      lea ecx, [edi+0x1c]
      push ecx
    ssl:
      mov eax, dword ptr [esi+0xc]
      mov eax, dword ptr [eax+0x10]
      and eax, 0xff
      movzx eax, byte ptr [eax+{tbl:#x}]
      cmp eax, edx
      jl ssn
      mov edx, eax
      mov ebx, esi
      mov dword ptr [esp], ecx
    ssn:
      lea ecx, [esi]
      mov esi, dword ptr [esi]
      test esi, esi
      jne ssl
      pop ecx
      mov eax, dword ptr [ebx]
      mov dword ptr [ecx], eax
      mov dword ptr [ebx], ebp
      mov ebp, ebx
      jmp sso
    ssd:
      mov dword ptr [edi+0x1c], ebp
      pop ecx
      '''
    pc = pc.replace('{rev}', rev)
    pcode = bytes(ks.asm(pc, pc_va)[0]); assert len(pcode) < 0x100
    put(pc_va, pcode)
    put(0x1072f529, bytes(ks.asm(f'jmp {pc_va:#x}', 0x1072f529)[0]) + b'\x90')
    fc_va = CAVE + 0x300
    f = f"""
      pushfd
      pushad
      mov ecx, dword ptr [ecx]
      test word ptr [ecx+0x36], 0x1000
      je direct
      call {INDIR:#x}
      mov ecx, eax
    direct:
      mov edx, 1
      call {GETNAME:#x}
      push eax
      {pr(strs['F'], 1)}
      {flush()}
      popad
      popfd
      mov eax, dword ptr [0x1079d854]
      jmp 0x1071ace4
    """
    fc = bytes(ks.asm(f, fc_va)[0]); assert len(fc) < 0x100
    put(fc_va, fc)
    put(0x1073049c, bytes(ks.asm(f'jmp {CAVE:#x}', 0x1073049c)[0]))
    put(0x10730739, bytes(ks.asm(f'jmp {kc_va:#x}', 0x10730739)[0]))
    put(0x1071acdf, bytes(ks.asm(f'jmp {fc_va:#x}', 0x1071acdf)[0]))
    dst = os.path.join(X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(stock, dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
