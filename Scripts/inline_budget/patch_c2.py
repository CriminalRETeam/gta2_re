"""
Regenerates c2_patch.json: the byte patches that add logging hooks to the VC6 (SP4, 12.00.8804) C2.DLL
inliner (function 0x1073b633, "InlineCalls(ctx, depth, budget, flag)"). setup.py applies them. Only
needed to change the hooks; needs `pip install pefile keystone-engine`.

    python3 patch_c2.py [hook,hook...]

The patched C2.DLL logs to stderr (lines starting with "@I") and turns every inline decision into a
C4711 warning with the real source line. The code it generates is unchanged.
"""
import json, os, struct, sys
import pefile
from keystone import Ks, KS_ARCH_X86, KS_MODE_32

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, '..', '..', '3rdParty', 'gta2_re_compile_tools', 'VC98', 'Bin', 'C2.DLL')
pe = pefile.PE(SRC)
orig = bytes(open(SRC, 'rb').read())
data = bytearray(orig)
BASE = 0x10700000
text = pe.sections[0]
def off(va): return va - BASE - text.VirtualAddress + text.PointerToRawData

IOB, FFLUSH, VFPRINTF = 0x107a0074, 0x107a0094, 0x107a00c4
GETNAME = 0x1073ffc7     # ecx=sym, edx=1 -> char*
INDIR = 0x10783bcf       # ecx=sym -> sym (used when sym->0x36 & 0x1000)
CAVE = 0x10798400

ks = Ks(KS_ARCH_X86, KS_MODE_32)

# strings placed at the end of the cave
strings = {
 'fmt_enter': b'@I ENTER depth=%d budget=%d flag=%d caller=%s callersize=%d global=%d\n\x00',
 'fmt_site':  b'@I SITE depth=%d line=%d callee=%s size=%d flags73=%08x budget=%d count=%d maxdepth=%d argok=%d global=%d\n\x00',
 'fmt_acc':   b'@I ACCEPT budget=%d\n\x00',
 'fmt_rej':   b'@I REJECT\n\x00',
 'fmt_skip':  b'@I SKIP\n\x00',
 'fmt_used':  b'@I NESTED used=%d\n\x00',
 'fmt_w':     b'depth=%d [%x] %s size=%d flags=%x budget=%d sites_left=%d maxdepth=%d argok=%d total=%d\x00',
 'fmt_ab1': b'@I ABORT udt\n\x00',
 'fmt_ab2': b'@I ABORT eh1\n\x00',
 'fmt_ab3': b'@I ABORT post\n\x00',
}
GBUF = 0x1079f800   # unused tail of .bssbe (writable, zero filled)
WARN = 0x1073ff68   # warning(level, number - 4000, string)

def asm(src, addr):
    enc, _ = ks.asm(src, addr)
    return bytes(enc)

def build(str_addr):
    S = str_addr
    code = f"""
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

    hook_enter:      
      pushfd
      pushad
      mov esi, ecx
      mov edi, edx
      push dword ptr [0x1079f234]
      mov eax, dword ptr [esi]
      movsx eax, word ptr [eax+0x6d]
      push eax
      mov ecx, dword ptr [esi]
      call name
      push eax
      push dword ptr [esp+36+12+8]
      push dword ptr [esp+36+16+4]
      push edi
      push {S['fmt_enter']:#x}
      call logf
      add esp, 28
      popad
      popfd
      sub esp, 0x34
      mov eax, dword ptr [esp+0x38]
      jmp 0x1073b63a

    hook_site:       
      pushfd
      pushad
      
      push dword ptr [0x1079f234]
      push ecx
      mov eax, dword ptr [ebx+8]
      and eax, 0xff
      push eax
      push dword ptr [esp+36+12+0x30]
      push dword ptr [esp+36+16+0x48]
      push dword ptr [edi+0x73]
      movsx eax, word ptr [edi+0x6d]
      push eax
      mov ecx, edi
      call name
      push eax
      push dword ptr [0x107ac354]
      push dword ptr [esp+36+36+0x34]
      push {S['fmt_site']:#x}
      call logf
      mov dword ptr [esp], {S['fmt_w']:#x}
      push {GBUF+8:#x}
      call dword ptr [0x107a0064]
      add esp, 4
      add esp, 44
      popad
      popfd
      mov eax, dword ptr [ebx+8]
      mov edx, dword ptr [esp+0x34]
      jmp 0x1073bbb0

    hook_acc:        
      pushfd
      pushad
      push dword ptr [esp+36+0x48]
      push {S['fmt_acc']:#x}
      call logf
      add esp, 8
      mov dword ptr [{GBUF:#x}], 0x494c4e49
      mov dword ptr [{GBUF+4:#x}], 0x2020454e
      push {GBUF:#x}
      push 0x2c7
      push 1
      call {WARN:#x}
      add esp, 12
      popad
      popfd
      mov edx, dword ptr [edi+0x73]
      mov ecx, edx
      jmp 0x1073bc07

    hook_rej:        
      pushfd
      pushad
      push {S['fmt_rej']:#x}
      call logf
      add esp, 4
      mov dword ptr [{GBUF:#x}], 0x4c54554f
      mov dword ptr [{GBUF+4:#x}], 0x20454e49
      push {GBUF:#x}
      push 0x2c7
      push 1
      call {WARN:#x}
      add esp, 12
      popad
      popfd
      test dword ptr [edi+0x73], 0x2080
      jmp 0x1073b6d4

    hook_skip:
      pushfd
      pushad
      push {S['fmt_skip']:#x}
      call logf
      add esp, 4
      mov dword ptr [{GBUF:#x}], 0x50494b53
      mov dword ptr [{GBUF+4:#x}], 0x20202020
      push {GBUF:#x}
      push 0x2c7
      push 1
      call {WARN:#x}
      add esp, 12
      popad
      popfd
      test dword ptr [edi+0x73], 0x2080
      jmp 0x10793e69

    hook_used:       
      pushfd
      pushad
      push eax
      push {S['fmt_used']:#x}
      call logf
      add esp, 8
      popad
      popfd
      mov esi, eax
      mov eax, dword ptr [0x107ac0b4]
      jmp 0x1073bd22

    hook_ab1:
      pushfd
      pushad
      push {S['fmt_ab1']:#x}
      call logf
      add esp, 4
      popad
      popfd
      mov ecx, ebp
      call 0x1075a568
      jmp 0x1073b8a8

    hook_ab2:
      pushfd
      pushad
      push {S['fmt_ab2']:#x}
      call logf
      add esp, 4
      popad
      popfd
      mov ecx, ebp
      call 0x1075a568
      jmp 0x10793f25

    hook_ab3:
      pushfd
      pushad
      push {S['fmt_ab3']:#x}
      call logf
      add esp, 4
      popad
      popfd
      mov ecx, ebp
      call 0x1075a568
      jmp 0x10794019
    """
    return code

# two-pass: first compute code size with dummy string addr
S = {k: 0x10798f00 for k in strings}
code = asm(build(S), CAVE)
p = CAVE + len(code) + 16
S = {}
blob = b''
for k, v in strings.items():
    S[k] = p + len(blob); blob += v
code = asm(build(S), CAVE)
assert CAVE + len(code) <= S['fmt_enter']
end = S['fmt_enter'] + len(blob)
assert end < 0x10799000, hex(end)
data[off(CAVE):off(CAVE) + len(code)] = code
data[off(S['fmt_enter']):off(S['fmt_enter']) + len(blob)] = blob

# label addresses: assemble prefix to find labels
def label_addr(lbl):
    src = build(S)
    pre = src.split(lbl + ':')[0]
    return CAVE + len(asm(pre, CAVE))

hooks = {0x1073b633: ('hook_enter', 7), 0x1073bba9: ('hook_site', 7), 0x1073bc02: ('hook_acc', 5),
         0x1073b6cd: ('hook_rej', 7), 0x10793e62: ('hook_skip', 7), 0x1073bd1b: ('hook_used', 7), 0x1073b8a1: ('hook_ab1', 7), 0x10793f1e: ('hook_ab2', 7), 0x10794012: ('hook_ab3', 7)}
which = sys.argv[1].split(',') if len(sys.argv) > 1 else None
for at, (lbl, n) in hooks.items():
    if which and lbl not in which: continue
    tgt = label_addr(lbl)
    j = asm(f'jmp {tgt:#x}', at)
    j = j + b'\x90' * (n - len(j))
    data[off(at):off(at) + n] = j

# grow .text virtual size to cover the cave
hdr = text.get_file_offset()
struct.pack_into('<I', data, hdr + 8, 0x98000)
# store the differing byte runs
runs = []
i = 0
while i < len(data):
    if data[i] != orig[i]:
        j = i
        while j < len(data) and (data[j] != orig[j] or (j + 1 < len(data) and data[j + 1] != orig[j + 1])):
            j += 1
        runs.append([i, data[i:j].hex()])
        i = j
    else:
        i += 1
json.dump({'note': 'byte patches for VC6 SP4 C2.DLL, made by patch_c2.py', 'runs': runs},
          open(os.path.join(HERE, 'c2_patch.json'), 'w'), indent=0)
print('ok', hex(CAVE), hex(end), len(runs), 'runs')
