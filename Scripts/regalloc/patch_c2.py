"""
Builds a logging copy of the VC6 (SP4, 12.00.8804) C2.DLL for the global register allocator (color.c):
build_vc6/x87_c2/<variant>/VC98/Bin/C2.DLL, next to the x87_sched variants, so
`X87_C2=<variant> Scripts/x87_sched/sched.sh -l file.cpp` compiles with it. The code it generates
is unchanged. Needs `pip install pefile keystone-engine`.

    python3 Scripts/regalloc/patch_c2.py [variant]        # default variant: ralog

Each function prints `@F <name>` when the colour pass starts, then each colour decision (end of the select function 0x107233b6) prints a line to stderr:
    @R lr=<live range> l0=<[lr]> prio=<[lr+0C]> tie=<[lr+40]> w=<[lr+3C]> cls=<class> reg=<chosen>
       s=<score eax ecx edx ebx esp ebp esi edi>
Each local (code generator) register assignment prints
    @L reg=<reg> via=<return address: 0x1072ebd5 round robin, 0x1072ec84 preference, others> cur=<round-robin cursor index after> x=<temp> line=<source line>
Registers are C2 numbers: 1 eax, 2 ecx, 3 edx, 4 ebx, 5 esp, 6 ebp, 7 esi, 8 edi (0 none).
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
GETNAME = 0x1073ffc7     # ecx=sym, edx=1 -> char*
INDIR = 0x10783bcf       # ecx=sym -> sym (when sym->0x36 & 0x1000)
SCORE = 0x1079d868       # s32 score[9], indexed by register
HOOK, BACK = 0x1072353b, 0x10723544   # lea eax,[edi*8]; sub eax,edi  (edi = reg, ebx = lr)
CAVE = 0x10798400

FMT = b'@R lr=%08x l0=%08x k=%08x %08x %08x %08x prio=%d tie=%d w=%d cls=%d reg=%d s=%d %d %d %d %d %d %d %d\n\x00'
NONAME = b'?\x00'

FORCE = None   # (prio, tie, reg): give that live range this register (experiments, --force p:t:r)

def build(cave):
    fmt = cave + 0x300; nn = fmt + len(FMT)
    pushes = '\n'.join(f'push dword ptr [{SCORE + 4*r:#x}]' for r in range(8, 0, -1))
    src = f"""
      pushfd
      pushad
      {pushes}
      push edi
      push dword ptr [esp+0x24+0x24+0x10]
      push dword ptr [ebx+0x3c]
      push dword ptr [ebx+0x40]
      push dword ptr [ebx+0x0c]
      mov eax, dword ptr [ebx]
      push dword ptr [eax+0x1c]
      push dword ptr [eax+0x10]
      push dword ptr [eax+8]
      push dword ptr [eax+4]
      push dword ptr [ebx]
      push ebx
      mov eax, esp
      push eax
      push {fmt:#x}
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{VFPRINTF:#x}]
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      mov dword ptr [esp], eax
      call dword ptr [{FFLUSH:#x}]
      add esp, 4*22
      popad
      popfd
      {{force}}
      lea eax, [edi*8]
      sub eax, edi
      jmp {BACK:#x}
    """
    force = ''
    if FORCE:
        force = f"""
      cmp dword ptr [ebx+0x0c], {FORCE[0]}
      jne nof
      cmp dword ptr [ebx+0x40], {FORCE[1]}
      jne nof
      mov edi, {FORCE[2]}
    nof:"""
    src = src.replace('{force}', force)
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    code = bytes(ks.asm(src, cave)[0])
    return code, fmt, nn

FFMT = b'@F %s\n\x00'
FHOOK, FBACK = 0x1071acdf, 0x1071ace4   # color pass entry: mov eax,[0x1079d854]; ecx = function context

def build_f(cave, fmt):
    src = f"""
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
      mov eax, esp
      push eax
      push {fmt:#x}
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{VFPRINTF:#x}]
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      mov dword ptr [esp], eax
      call dword ptr [{FFLUSH:#x}]
      add esp, 16
      popad
      popfd
      mov eax, dword ptr [0x1079d854]
      jmp {FBACK:#x}
    """
    return bytes(Ks(KS_ARCH_X86, KS_MODE_32).asm(src, cave)[0])

LFMT = b'@L reg=%d via=%08x cur=%d x=%08x line=%d\n\x00'
LHOOK, LBACK = 0x1072ee00, 0x1072ee0a   # assign(reg=ecx, x=edx): push esi; mov esi,ecx; mov [esi*4+0x1079d6ec],edx
CURSOR, RRLIST = 0x1079d710, 0x107adff4

def build_l(cave, fmt):
    src = f"""
      pushfd
      pushad
      push dword ptr [0x107ac354]
      push edx
      mov eax, dword ptr [{CURSOR:#x}]
      sub eax, {RRLIST:#x}
      sar eax, 2
      push eax
      push dword ptr [esp+0x24+12]
      push ecx
      mov eax, esp
      push eax
      push {fmt:#x}
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{VFPRINTF:#x}]
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      mov dword ptr [esp], eax
      call dword ptr [{FFLUSH:#x}]
      add esp, 32
      popad
      popfd
      push esi
      mov esi, ecx
      mov dword ptr [esi*4+0x1079d6ec], edx
      jmp {LBACK:#x}
    """
    return bytes(Ks(KS_ARCH_X86, KS_MODE_32).asm(src, cave)[0])

def main():
    global FORCE
    args = sys.argv[1:]
    if '--force' in args:
        i = args.index('--force'); FORCE = tuple(int(v, 0) for v in args[i + 1].split(':')); del args[i:i + 2]
    var = args[0] if args else 'ralog'
    stock = os.path.join(X, 'stock')
    if not os.path.isdir(stock):
        sys.exit('build_vc6/x87_c2/stock missing: run Scripts/x87_sched/setup.py --variant stock first')
    dst = os.path.join(X, var)
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(stock, dst, symlinks=True)
    out = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    os.remove(out)
    pe = pefile.PE(SRC)
    d = bytearray(open(SRC, 'rb').read())
    def put(va, b):
        o = pe.get_offset_from_rva(va - BASE)
        d[o:o + len(b)] = b
    code, fmt, nn = build(CAVE)
    assert len(code) < 0x300
    put(CAVE, code); put(fmt, FMT); put(nn, NONAME)
    fcave = CAVE + 0x380; ffmt = fcave + 0x70
    fcode = build_f(fcave, ffmt)
    assert len(fcode) < 0x70
    put(fcave, fcode); put(ffmt, FFMT)
    lcave = CAVE + 0x400; lfmt = lcave + 0x80
    lcode = build_l(lcave, lfmt)
    assert len(lcode) < 0x80
    put(lcave, lcode); put(lfmt, LFMT)
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    lj = bytes(ks.asm(f'jmp {lcave:#x}', LHOOK)[0])
    put(LHOOK, lj + b'\x90' * (10 - len(lj)))
    put(FHOOK, bytes(ks.asm(f'jmp {fcave:#x}', FHOOK)[0]))
    j = bytes(ks.asm(f'jmp {CAVE:#x}', HOOK)[0])
    put(HOOK, j + b'\x90' * (9 - len(j)))
    open(out, 'wb').write(d)
    print(out)

if __name__ == '__main__':
    main()
