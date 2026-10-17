#!/usr/bin/env python3
"""fullexp.py BASE_DIR MOD_DIR TU.cpp [TU.cpp ...]

Compiles each TU from BASE_DIR and MOD_DIR (two copies of Source/, for example before and after
replacing an inline variant), and for every MATCH/WIP function whose asm differs between the two,
prints the permuter score of both (0 = match). A quick check across TUs before the full build.
Needs Scripts/bin_comp/target_asm.json and matched_asm.json (see CLAUDE.md). Run with venv/bin/python3.
"""
import sys, os, re, json, subprocess, tempfile
REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
sys.path.insert(0, REPO + '/Scripts/bin_comp')
import permuter_score as ps

T = json.load(open(REPO + '/Scripts/bin_comp/target_asm.json'))
M = json.load(open(REPO + '/Scripts/bin_comp/matched_asm.json'))
EXTRA = {'Network_20324.cpp': '/Gz', 'FpsCounter_54.cpp': '/GX-', 'gbh_graphics.cpp': '/Od /ZI'}

def compile_(src, obj):
    env = dict(os.environ, GTA2_RE=REPO, WINEDEBUG='-all', NO_PCH='1', EXTRA_CFLAGS=EXTRA.get(os.path.basename(src), ''))
    r = subprocess.run([REPO + '/3rdParty/cpp_permuter/examples/gta2/compile.sh', src, obj], env=env,
                       capture_output=True, text=True)
    errs = [l for l in r.stdout.splitlines() if ' error ' in l]
    if errs or not os.path.exists(obj):
        print('COMPILE ERROR', src, errs[:5]); return None
    return ps.Coff(open(obj, 'rb').read())

def score(coff, addr, symidx):
    target = T.get(hex(addr)) or M.get(hex(addr))
    callees = hex(addr) in T
    ml = ps.function_lines(coff, symidx); tl = ps.target_lines(target)
    s = ps.score_lines(tl, ml)
    if callees:
        s += ps.callee_penalty(coff, symidx, target, addr)[0]
    return s


def markers(text):
    """(kind, addr, symbol needles) per MATCH_FUNC/WIP_FUNC: '_ADDR@' first, then the mangled name
    of the definition that follows (for symbols without the address, like ctors)."""
    lines = text.split('\n')
    out = []
    for i, l in enumerate(lines):
        m = re.match(r'\s*(MATCH|WIP)_FUNC\((0x[0-9A-Fa-f]+)\)', l)
        if not m:
            continue
        needles = ['_%X@' % int(m.group(2), 16)]
        for l2 in lines[i + 1:i + 4]:
            d = re.search(r'([A-Za-z_]\w*)::(~?)(\w+)\s*\(', l2)
            if d and not l2.strip().startswith('//'):
                cls, tilde, fn = d.groups()
                if fn == cls:
                    needles.append('??%s%s@@' % ('1' if tilde else '0', cls))
                else:
                    needles.append('?%s@%s@@' % (fn, cls))
                break
            d = re.search(r'\b([A-Za-z_]\w*)\s*\(', l2)
            if d and not l2.strip().startswith('//') and d.group(1) not in ('if', 'while', 'for', 'switch'):
                needles.append('?%s@@' % d.group(1))
                break
        out.append((m.group(1), m.group(2), needles))
    return out

base, mod = sys.argv[1], sys.argv[2]
NCMP = [0]
tmp = tempfile.mkdtemp()
for tu in sys.argv[3:]:
    cb = compile_(os.path.join(base, tu), tmp + '/b.obj'); cm = compile_(os.path.join(mod, tu), tmp + '/m.obj')
    if not cb or not cm: continue
    src_text = open(os.path.join(mod, tu), encoding='latin-1').read()
    for kind, a, needles in markers(src_text):
        addr = int(a, 16)
        ib = im = None
        for n in needles:
            try:
                ib = ps.find_function(cb, n); im = ps.find_function(cm, n)
                break
            except BaseException:
                ib = im = None
        if ib is None:
            print(tu, kind, a, 'NOT FOUND in the object (check by hand)'); continue
        NCMP[0] += 1
        if ps.function_lines(cb, ib) == ps.function_lines(cm, im):
            continue
        if not (T.get(hex(addr)) or M.get(hex(addr))):
            print(tu, kind, a, 'changed (no target)'); continue
        print('%-22s %-5s %s  base=%d  mod=%d' % (tu, kind, a, score(cb, addr, ib), score(cm, addr, im)), flush=True)
print("compared", NCMP[0])
import shutil; shutil.rmtree(tmp, ignore_errors=True)
