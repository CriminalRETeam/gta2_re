"""
Builds build_vc6/x87_c2/alog: the `log` variant (setup.py) plus a log of the scheduler's memory alias tests,
so `X87_C2=alog Scripts/x87_sched/sched.sh file.cpp func` also shows why two memory nodes got an edge.
Needs `pip install pefile keystone-engine`. Same generated code as stock.

    python3 Scripts/x87_sched/alias_log.py

Each alias test of the dependence-graph builder prints (stderr, so in last.log):
    @A t=<1 may-alias 0x1070283e from 0x1072b509 (RAW/WAW), 2 same-location 0x1070166c, 3 0x1070283e from
          0x1072caad (WAR)> r=<result, nonzero = edge> n=<seq of the new node> m=<seq of the earlier node>
          flag=<edge kind for t 1/2> A:<new operand> B:<earlier operand>
    operand = k=<kind: 2 symbol, 6 indirect> f=<flags> <+14> <+18> <+1c points-to index> <+20> <+24 displacement>
For an indirect operand whose index is a per-base location entry (index >= [0x1079d670]) it also prints
    @D idx=<index> base=<points-to index of the base pointer> bit=<location bit> mask=<bits it overlaps>
"""
import os, shutil, sys
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32

HERE = os.path.dirname(os.path.abspath(__file__))
X = os.path.join(HERE, '..', '..', 'build_vc6', 'x87_c2')
BASE = 0x10700000
IOB, VFPRINTF, FFLUSH = 0x107a0074, 0x107a00c4, 0x107a0094
CAVE = 0x10798b00                     # after the `log` variant's hooks, before the end of .text
NLOC, LOCTAB, PTSTAB = 0x1079d670, 0x1079d6bc, 0x1079d678
# call site, callee, tag, register holding the new node in the caller
SITES = [(0x1072b530, 0x1070283e, 1, 'ebp'), (0x1072b556, 0x1070166c, 2, 'ebp'), (0x1072cac2, 0x1070283e, 3, 'edi')]
PUSHAD = dict(edi=0, esi=4, ebp=8, esp=12, ebx=16, edx=20, ecx=24, eax=28)
STRS = {
    'fmtA': b'@A t=%d r=%d n=%d m=%d flag=%x A:k=%d f=%02x %08x %08x %08x %08x %08x '
            b'B:k=%d f=%02x %08x %08x %08x %08x %08x\n',
    'fmtD': b'@D idx=%x base=%x bit=%d mask=%08x\n',
}


def printf(fmt, nargs):
    """vfprintf(stderr, fmt, esp) for the nargs dwords on top of the stack, then pops them."""
    return f"""
      mov eax, esp
      push eax
      push {fmt}
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      push eax
      call dword ptr [{VFPRINTF:#x}]
      mov eax, dword ptr [{IOB:#x}]
      add eax, 0x40
      mov dword ptr [esp], eax
      call dword ptr [{FFLUSH:#x}]
      add esp, {12 + 4 * nargs}
    """


def operand(reg):
    s = ''.join(f' push dword ptr [{reg}+{o:#x}]\n' for o in (0x24, 0x20, 0x1c, 0x18, 0x14))
    return s + f' movzx eax, byte ptr [{reg}+0x10]\n push eax\n movzx eax, byte ptr [{reg}+8]\n push eax\n'


def source(addr):
    src = f"""
    locentry:
      cmp byte ptr [ebx+8], 6
      jne ldone
      mov eax, dword ptr [ebx+0x1c]
      mov edx, dword ptr [{NLOC:#x}]
      cmp eax, edx
      jb ldone
      mov ecx, eax
      sub ecx, edx
      lea ecx, [ecx+ecx*2]
      mov esi, dword ptr [{LOCTAB:#x}]
      lea esi, [esi+ecx*4]
      push dword ptr [esi+8]
      push dword ptr [esi+4]
      push dword ptr [esi]
      push eax
      {printf(addr['fmtD'], 4)}
    ldone:
      ret
    """
    for i, (site, callee, tag, nreg) in enumerate(SITES):
        # stack after pushfd/pushad: [esp+36] = new operand (edx), [esp+40] = earlier operand (ecx)
        src += f"""
    w{i}:
      push ecx
      push edx
      call {callee:#x}
      pushfd
      pushad
      mov esi, dword ptr [esp+{PUSHAD['esi']}]
      mov ebx, dword ptr [esp+36]
      mov edi, dword ptr [esp+40]
      {operand('edi')}
      {operand('ebx')}
      push dword ptr [esp+{PUSHAD['ebx'] + 56}]
      mov eax, dword ptr [esi+4]
      movzx eax, word ptr [eax+0x36]
      push eax
      mov eax, dword ptr [esp+{PUSHAD[nreg] + 64}]
      movzx eax, word ptr [eax+0x36]
      push eax
      push dword ptr [esp+{PUSHAD['eax'] + 68}]
      push {tag}
      {printf(addr['fmtA'], 19)}
      mov ebx, dword ptr [esp+36]
      call locentry
      mov ebx, dword ptr [esp+40]
      call locentry
      popad
      popfd
      add esp, 8
      ret
    """
    return src


def main():
    ks = Ks(KS_ARCH_X86, KS_MODE_32)
    log = os.path.join(X, 'log')
    if not os.path.isdir(log):
        sys.exit('build_vc6/x87_c2/log missing: run Scripts/x87_sched/setup.py --variant log first')
    dummy = {k: CAVE for k in STRS}
    size = len(ks.asm(source(dummy), CAVE)[0])
    addr, p = {}, CAVE + size + 16
    for k, v in STRS.items():
        addr[k] = p
        p += len(v) + 1
    assert p < 0x10799000
    src = source({k: hex(v) for k, v in addr.items()})
    code = bytes(ks.asm(src, CAVE)[0])
    assert len(code) == size
    dst = os.path.join(X, 'alog')
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(log, dst, symlinks=True)
    dll = os.path.join(dst, 'VC98', 'Bin', 'C2.DLL')
    data = bytearray(open(os.path.realpath(dll), 'rb').read())
    os.remove(dll)
    pe = pefile.PE(data=bytes(data))

    def put(va, b):
        o = pe.get_offset_from_rva(va - BASE)
        data[o:o + len(b)] = b
    o = pe.get_offset_from_rva(CAVE - BASE)
    assert not any(data[o:o + (p - CAVE)]), 'cave in use'
    put(CAVE, code)
    for k, v in STRS.items():
        put(addr[k], v + b'\0')
    for i, (site, callee, tag, nreg) in enumerate(SITES):
        w = CAVE + len(ks.asm(src.split(f'\n    w{i}:')[0], CAVE)[0])
        assert data[pe.get_offset_from_rva(site - BASE)] == 0xe8
        put(site, bytes(ks.asm(f'call {w:#x}', site)[0]))
    open(dll, 'wb').write(data)
    print(dll)


if __name__ == '__main__':
    main()
