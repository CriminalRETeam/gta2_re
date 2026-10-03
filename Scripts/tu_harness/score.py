"""score.py SRC.cpp [--save]

Compile a variant of a whole TU (tu.sh) and score it: total differing lines over every WIP_FUNC in it,
plus the MATCH_FUNCs whose code changed against the saved base object. Run once with --save on the
unmodified file, then on each variant; prints per-function deltas against the base.
"""
import sys, os, re, json, difflib, subprocess, shutil
HERE = os.path.dirname(os.path.abspath(__file__)); REPO = os.path.dirname(os.path.dirname(HERE))
WORK = os.path.join(REPO, 'build_vc6', 'tu_harness')
sys.path.insert(0, os.path.join(REPO, 'Scripts', 'bin_comp'))
import permuter_score as ps
src_path = sys.argv[1]; save = '--save' in sys.argv
name = os.path.basename(src_path).split('.')[0]
r = subprocess.run([os.path.join(HERE, 'tu.sh'), src_path], capture_output=True, text=True)
if r.returncode: print('compile failed', r.stdout[-500:]); sys.exit(1)
T = json.load(open(os.path.join(REPO, 'Scripts', 'bin_comp', 'target_asm.json')))
marks = re.findall(r'(MATCH|WIP)_FUNC\((0x[0-9A-Fa-f]+)\)', open(src_path, encoding='latin-1').read())
def norm(s):
    m = s.split()[0] if s else ''
    if m.startswith('call'): return 'call'
    if m.startswith('j'): return m
    return re.sub(r'(0x[0-9A-Fa-f]+|[0-9])\(%esp\)', 'S', re.sub(r'0x[0-9A-F]{5,8}\b', 'G', s))
def fasm(coff, needle):
    l = [x.strip() for x in ps.function_asm(coff, ps.find_function(coff, needle)).split('\n') if x.strip()]
    while l and l[-1] in ('nop', 'int3'): l.pop()
    return l
c = ps.Coff(open(os.path.join(WORK, 'q.obj'), 'rb').read())
base_obj = os.path.join(WORK, 'base_%s.obj' % name); base_json = os.path.join(WORK, 'base_%s.json' % name)
b = ps.Coff(open(base_obj, 'rb').read()) if os.path.exists(base_obj) and not save else None
per = {}; changed = []
for kind, a in marks:
    n = '_%X@' % int(a, 16)
    try: ours = fasm(c, n)
    except Exception: continue
    if kind == 'WIP':
        t = T.get(hex(int(a, 16)))
        if not t: continue
        A = [norm(x) for x in t['asm'].split('\n') if x.strip()]; B = [norm(x) for x in ours]
        per[a] = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in
                     difflib.SequenceMatcher(None, A, B, autojunk=False).get_opcodes() if tag != 'equal')
    elif b:
        try:
            if fasm(b, n) != ours: changed.append(a)
        except Exception: pass
print('total', sum(per.values()), 'zero', [a for a, d in per.items() if d == 0], 'changed', changed)
if save:
    shutil.copy(os.path.join(WORK, 'q.obj'), base_obj); json.dump(per, open(base_json, 'w'))
elif os.path.exists(base_json):
    bp = json.load(open(base_json))
    print('deltas', {a: per[a] - bp[a] for a in per if a in bp and per[a] != bp[a]})
