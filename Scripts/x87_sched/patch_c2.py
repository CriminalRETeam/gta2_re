"""
Regenerates c2_patch.json: byte patches for the VC6 (SP4, 12.00.8804) C2.DLL list scheduler
(0x1072a20f) that setup.py applies. Only needed to change the hooks; needs
`pip install pefile keystone-engine`.

    python3 Scripts/x87_sched/patch_c2.py

Two variants, both with a per-window limit table (see README.md):
  log   logs to stderr: @F #ordinal function, @L window node list (op/kind), @R ready list,
        @S scheduled node (cycle, priority, earliest, seq = pre-schedule order, successors/latency),
        @E emitted instruction (to align with the /FAs listing)
  fast  no logging, only the window limit (for regsearch.py)

The window limit is `cmp $0x50,%esi; jg` at 0x1072a7ba (a window holds at most 81 nodes). The hooks
replace 0x50 with a value from a table in the patched DLL:
  int ordinal   0: stock limit everywhere, -1: lims[0] for every function,
                n: lims[i] for window i+1 of the n-th scheduled function (1-based), 0x50 for the others
  int count     number of entries in lims
  int lims[MAXLIM]
setup.py writes the table at the file offset stored in the json ("limtab"). With the default table
the generated code is the same as stock.
"""
import json, os, struct, sys
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
RT = os.environ.get('VC6_TOOLS') or os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools')
SRC = os.path.join(RT, 'VC98', 'Bin', 'C2.DLL')
MAXLIM = 64

BASE = 0x10700000
IOB, FFLUSH, VFPRINTF = 0x107a0074, 0x107a0094, 0x107a00c4
GETNAME, INDIR = 0x1073ffc7, 0x10783bcf          # symbol name (ecx=sym, edx=1); indirection
CAVE, CAVE_END = 0x10798400, 0x10799000          # zero padding at the end of .text
FCOUNT, WCOUNT, LIMIT = 0x1079f910, 0x1079f914, 0x1079f904   # unused .bss dwords
READY = 0x1079f278       # scheduler ready list head
CYCLE = 0x1079f238       # current cycle
WINDOW = 0x1079f268      # current window list head
ks = Ks(KS_ARCH_X86, KS_MODE_32)
REGOFF = dict(edi=0, esi=4, ebp=8, esp=12, ebx=16, edx=20, ecx=24, eax=28)


def R(r):
    """The register's value at hook entry (after pushfd/pushad, before any push of the hook body)."""
    return 'dword ptr [esp+%d]' % REGOFF[r]


def asm(src, addr):
    enc, _ = ks.asm(src, addr)
    return bytes(enc)


class Patcher:
    def __init__(self):
        self.pe = pefile.PE(SRC)
        self.orig = open(SRC, 'rb').read()
        self.data = bytearray(self.orig)
        self.text = self.pe.sections[0]
        self.hooks = []
        self.strings = {}

    def off(self, va):
        return va - BASE - self.text.VirtualAddress + self.text.PointerToRawData

    def string(self, name, s):
        self.strings[name] = s + b'\0'

    def hook(self, at, nbytes, displaced, body, back=None):
        """Jump from `at` (nbytes overwritten) to body (run between pushfd/pushad and popad/popfd),
        then the displaced instructions, then back to `back` (default at + nbytes)."""
        self.hooks.append((at, nbytes, displaced, body, back if back is not None else at + nbytes))

    def code(self, S):
        src = f"""
        logf:
          push ebp
          mov ebp, esp
          lea eax, [ebp+12]
          push eax
          push dword ptr [ebp+8]
          mov eax, dword ptr [{IOB:#x}]
          add eax, 0x40
          push eax
          call dword ptr [{VFPRINTF:#x}]
          add esp, 12
          mov eax, dword ptr [{IOB:#x}]
          add eax, 0x40
          push eax
          call dword ptr [{FFLUSH:#x}]
          add esp, 4
          pop ebp
          ret
        name:
          push edx
          test word ptr [ecx+0x36], 0x1000
          je name1
          call {INDIR:#x}
          mov ecx, eax
        name1:
          mov edx, 1
          call {GETNAME:#x}
          pop edx
          ret
        """
        for i, (at, n, disp, body, back) in enumerate(self.hooks):
            for k, v in S.items():
                body = body.replace('{' + k + '}', '%#x' % v)
            src += f"\nhook{i}:\n pushfd\n pushad\n{body}\n popad\n popfd\n{disp}\n jmp {back:#x}\n"
        return src

    def build(self):
        S = {k: 0x10798f00 for k in self.strings}
        code = asm(self.code(S), CAVE)
        p = CAVE + len(code) + 16
        S, blob = {}, b''
        for k, v in self.strings.items():
            S[k] = p + len(blob)
            blob += v
        src = self.code(S)
        code = asm(src, CAVE)
        assert CAVE + len(code) <= p and p + len(blob) < CAVE_END, (hex(CAVE + len(code)), hex(p + len(blob)))
        self.data[self.off(CAVE):self.off(CAVE) + len(code)] = code
        self.data[self.off(p):self.off(p) + len(blob)] = blob
        for i, (at, n, disp, body, back) in enumerate(self.hooks):
            tgt = CAVE + len(asm(src.split(f'\nhook{i}:')[0], CAVE))
            j = asm(f'jmp {tgt:#x}', at)
            assert len(j) <= n
            self.data[self.off(at):self.off(at) + n] = j + b'\x90' * (n - len(j))
        struct.pack_into('<I', self.data, self.text.get_file_offset() + 8, 0x99000)   # .text virtual size
        runs, i = [], 0
        while i < len(self.data):
            if self.data[i] != self.orig[i]:
                j = i
                while j < len(self.data) and (self.data[j] != self.orig[j] or
                                              (j + 1 < len(self.data) and self.data[j + 1] != self.orig[j + 1])):
                    j += 1
                runs.append([i, self.data[i:j].hex()])
                i = j
            else:
                i += 1
        return {'runs': runs, 'limtab': self.off(S['limtab'])}


def limit_hooks(p, log):
    p.string('limtab', struct.pack('<ii', 0, 0) + b'\0' * 4 * MAXLIM)
    # scheduler entry (once per function): count functions, reset the window counter, log the name
    body = f"  inc dword ptr [{FCOUNT:#x}]\n  mov dword ptr [{WCOUNT:#x}], 0\n"
    if log:
        p.string('fmtF', b'@F #%d %s\n')
        body += f"""
          mov ecx, {R('ecx')}
          mov ecx, dword ptr [ecx]
          call name
          push eax
          push dword ptr [{FCOUNT:#x}]
          push {{fmtF}}
          call logf
          add esp, 12
        """
    p.hook(0x1072a20f, 7, "push ecx\n push ebp\n push edi\n mov edi, edx\n mov ebp, ecx", body)
    # start of a window: pick its limit
    p.hook(0x1072a2a6, 8, f"mov ecx, esi\n mov dword ptr [{WINDOW:#x}], esi", f"""
      inc dword ptr [{WCOUNT:#x}]
      mov dword ptr [{LIMIT:#x}], 0x50
      mov edx, {{limtab}}
      mov eax, dword ptr [edx]
      test eax, eax
      je ldone
      cmp eax, -1
      je allf
      cmp eax, dword ptr [{FCOUNT:#x}]
      jne ldone
      mov ecx, dword ptr [{WCOUNT:#x}]
      cmp ecx, dword ptr [edx+4]
      ja ldone
      mov eax, dword ptr [edx+ecx*4+4]
      mov dword ptr [{LIMIT:#x}], eax
      jmp ldone
     allf:
      mov eax, dword ptr [edx+8]
      mov dword ptr [{LIMIT:#x}], eax
     ldone:
    """)
    # the window size check itself (stock: cmp esi, 0x50; jg 0x1072a7d3)
    p.hook(0x1072a7ba, 5, f"cmp esi, dword ptr [{LIMIT:#x}]\n jg 0x1072a7d3", "", back=0x1072a7bf)


def log_hooks(p):
    p.string('nl', b'\n')
    # emitted instruction, in final order
    p.string('fmtE', b'@E %08x op=%d\n')
    p.hook(0x1078cee3, 6, "sub esp, 0x44\n push edi\n mov edi, ecx",
           "mov esi, " + R('ecx') + "\n push dword ptr [esi+4]\n push esi\n push {fmtE}\n call logf\n add esp, 12\n")
    # scheduled node (esi = node)
    p.string('fmtS', b'@S cyc=%d node=%08x ins=%08x op=%d prio=%d earliest=%d seq=%d f39=%02x f3a=%02x npred=%d '
                     b'opA=%08x/%08x/%08x opB=%08x/%08x/%08x succ:')
    p.string('fmtEdge', b' %08x/%d')
    p.hook(0x1072c103, 8, "mov ecx, dword ptr [esi+0x1c]\n call 0x1072c701", f"""
      mov esi, {R('esi')}
      mov edx, dword ptr [esi+0x1c]
      xor eax, eax
      push eax
      push eax
      push eax
      push eax
      push eax
      push eax
      test edx, edx
      je nops
      mov eax, dword ptr [edx+0x18]
      test eax, eax
      je nopb
      push dword ptr [eax+20]
      push dword ptr [eax+16]
      push dword ptr [eax+4]
      pop dword ptr [esp+20]
      pop dword ptr [esp+20]
      pop dword ptr [esp+20]
     nopb:
      mov eax, dword ptr [edx+0x1c]
      test eax, eax
      je nops
      push dword ptr [eax+20]
      push dword ptr [eax+16]
      push dword ptr [eax+4]
      pop dword ptr [esp+8]
      pop dword ptr [esp+8]
      pop dword ptr [esp+8]
     nops:
      movzx eax, word ptr [esi+0x20]
      push eax
      movzx eax, byte ptr [esi+0x3a]
      push eax
      movzx eax, byte ptr [esi+0x39]
      push eax
      movzx eax, word ptr [esi+0x36]
      push eax
      push dword ptr [esi+0x30]
      push dword ptr [esi+0x2c]
      mov eax, dword ptr [esi+0x1c]
      mov ecx, -1
      test eax, eax
      je snull
      mov ecx, dword ptr [eax+4]
     snull:
      push ecx
      push eax
      push esi
      push dword ptr [{CYCLE:#x}]
      push {{fmtS}}
      call logf
      add esp, 68
      mov edi, dword ptr [esi+0xc]
     eloop:
      test edi, edi
      je edone
      movzx eax, word ptr [edi+0x14]
      push eax
      push dword ptr [edi+0xc]
      push {{fmtEdge}}
      call logf
      add esp, 12
      mov edi, dword ptr [edi]
      jmp eloop
     edone:
      push {{nl}}
      call logf
      add esp, 4
    """)
    # ready list when a cycle starts
    p.string('fmtR', b'@R cyc=%d ready:')
    p.string('fmtRn', b' %08x(op=%d,p=%d,e=%d,s=%d)')
    p.hook(0x1072c20e, 5, "sub esp, 0xc\n push ebx\n push ebp", f"""
      push dword ptr [{CYCLE:#x}]
      push {{fmtR}}
      call logf
      add esp, 8
      mov esi, dword ptr [{READY:#x}]
     rloop:
      test esi, esi
      je rdone
      movzx eax, word ptr [esi+0x36]
      push eax
      push dword ptr [esi+0x30]
      push dword ptr [esi+0x2c]
      mov eax, dword ptr [esi+0x1c]
      mov ecx, -1
      test eax, eax
      je rnull
      mov ecx, dword ptr [eax+4]
     rnull:
      push ecx
      push esi
      push {{fmtRn}}
      call logf
      add esp, 24
      mov esi, dword ptr [esi+0x10]
      jmp rloop
     rdone:
      push {{nl}}
      call logf
      add esp, 4
    """)
    # window node list once built: @L op/kind ...
    p.string('fmtL', b'@L')
    p.string('fmtLn', b' %d/%x')
    p.hook(0x1072a2b3, 6, f"mov ecx, dword ptr [{WINDOW:#x}]", f"""
      push {{fmtL}}
      call logf
      add esp, 4
      mov esi, dword ptr [{WINDOW:#x}]
      mov edi, {R('eax')}
     lloop:
      test esi, esi
      je ldone2
      movzx eax, byte ptr [esi+8]
      push eax
      push dword ptr [esi+4]
      push {{fmtLn}}
      call logf
      add esp, 12
      cmp esi, edi
      je ldone2
      mov esi, dword ptr [esi]
      jmp lloop
     ldone2:
      push {{nl}}
      call logf
      add esp, 4
    """)


def main():
    out = {'note': 'byte patches for VC6 SP4 C2.DLL, made by patch_c2.py', 'maxlim': MAXLIM}
    for variant in ('log', 'fast'):
        p = Patcher()
        limit_hooks(p, variant == 'log')
        if variant == 'log':
            log_hooks(p)
        out[variant] = p.build()
        print(variant, len(out[variant]['runs']), 'runs, limit table at file offset', hex(out[variant]['limtab']))
    with open(os.path.join(HERE, 'c2_patch.json'), 'w') as f:
        json.dump(out, f, indent=0)


if __name__ == '__main__':
    main()
