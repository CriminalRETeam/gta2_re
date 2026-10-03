"""diff.py OBJ ADDR [-n] [--96f] [--needle NAME] [-c N] [--regs]

Diff one function of a COFF object against the original asm.
  default: 10.5 target (Scripts/bin_comp/target_asm.json, WIP/STUB functions only); the function is
           found by the `_<ADDR>@` part of its mangled name, or --needle.
  --96f:   9.6f target (target_96f.json from the claude/target-asm branch, fetched on first use).
Globals, call targets and jump targets are normalised, stack offsets are ignored (`S`) unless --regs.
  -n       print only the number of differing lines (0 = same)
"""
import sys, os, re, json, difflib, subprocess
HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(REPO, 'Scripts', 'bin_comp'))
import permuter_score as ps

args = sys.argv[1:]
def opt(name, default=None):
    if name in args:
        i = args.index(name); v = args[i + 1]; del args[i:i + 2]; return v
    return default
def flag(name):
    if name in args: args.remove(name); return True
    return False
needle = opt('--needle'); ctx = int(opt('-c', 2)); count = flag('-n'); v96 = flag('--96f'); keep_stack = flag('--regs')
obj, addr = args[0], int(args[1], 16)

if v96:
    p = os.path.join(REPO, 'build_vc6', 'tu_harness', 'target_96f.json')
    if not os.path.exists(p):
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, 'w').write(subprocess.run(['git', '-C', REPO, 'show', 'origin/claude/target-asm:target_96f.json'],
                                          capture_output=True, text=True, check=True).stdout)
    t = json.load(open(p))[hex(addr)]
    if not needle:
        needle = t['name'].split('::')[-1]
else:
    t = json.load(open(os.path.join(REPO, 'Scripts', 'bin_comp', 'target_asm.json')))[hex(addr)]
    needle = needle or '_%X@' % addr
tgt = [re.sub(r'^[0-9a-f]+:\s*', '', x).strip() for x in t['asm'].split('\n') if x.strip()]
c = ps.Coff(open(obj, 'rb').read())
ours = [x.strip() for x in ps.function_asm(c, ps.find_function(c, needle)).split('\n') if x.strip()]
while ours and ours[-1] in ('nop', 'int3'):
    ours.pop()

def norm(s):
    m = s.split()[0] if s else ''
    if m.startswith('call'): return 'call'
    if m.startswith('j'): return m
    s = re.sub(r'0x[0-9A-Fa-f]{5,8}\b', 'G', s)
    if not keep_stack:
        s = re.sub(r'(0x[0-9A-Fa-f]+|[0-9])\(%esp\)', 'S', s)
    return s

A = [norm(x) for x in tgt]; B = [norm(x) for x in ours]
sm = difflib.SequenceMatcher(None, A, B, autojunk=False)
ops = [o for o in sm.get_opcodes() if o[0] != 'equal']
n = sum(max(i2 - i1, j2 - j1) for _, i1, i2, j1, j2 in ops)
if count:
    print(n); sys.exit(0)
print('%s %s: %d differing lines, ratio %.3f' % (hex(addr), t['name'], n, sm.ratio()))
for tag, i1, i2, j1, j2 in ops:
    print('--- %s orig[%d:%d] ours[%d:%d]' % (tag, i1, i2, j1, j2))
    for k in range(max(0, i1 - ctx), i1): print('    ' + tgt[k])
    for k in range(i1, i2): print('  - ' + tgt[k])
    for k in range(j1, j2): print('  + ' + ours[k])
