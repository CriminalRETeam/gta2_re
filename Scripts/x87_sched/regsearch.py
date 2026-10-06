"""regsearch.py: window-limit what-if against the original asm (run with the repo venv's python).

    regsearch.py SRC.cpp FUNC ADDR [--range 62:80] [--min-window 40] [-j N]
        One function. Scans one limit for all its windows, then per window (coordinate descent, windows of
        at least --min-window nodes and a few extra for windows that split) for the limits that minimise
        its diff against the 10.5 target. A best limit L below 80 says the original had about 80 - L more
        nodes (no-op nodes: parentheses, (f32) casts, f32 locals) before that window's break.
    regsearch.py SRC.cpp --tu [--range 62:80] [-j N]
        One limit for every function of the TU: total diff lines over its WIP_FUNCs, the ones that reach 0,
        and the MATCH_FUNCs whose code changes against the stock limit.

FUNC is a substring of the decorated name as `sched.sh` prints it (`draw_left_4F3C00`); ADDR the function's
address for the target lookup. The target is Scripts/bin_comp/target_asm.json (WIP/STUB functions), from
`git show origin/claude/target-asm:target_asm.json`. Each compile takes a few seconds; -j runs that many in
parallel (separate build_vc6/x87_c2/job<N> dirs).
"""
import argparse, difflib, json, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(ROOT, 'Scripts', 'bin_comp'))
import nodes  # noqa: E402
import permuter_score as ps  # noqa: E402

STOCK = 80
X = os.path.join(ROOT, 'build_vc6', 'x87_c2')


def compile_tu(src, job, lim='', log=False):
    out = os.path.join(X, 'job%d' % job)
    env = dict(os.environ, X87_OUT=out, LIM=lim)
    r = subprocess.run([os.path.join(HERE, 'sched.sh'), '-l' if log else '-q', src], env=env,
                       capture_output=True, text=True)
    obj = os.path.join(out, 'last.obj')
    if r.returncode or not os.path.exists(obj):
        sys.exit('compile failed (LIM=%s): %s' % (lim, (r.stdout + r.stderr)[-800:]))
    return out


def norm(s):
    m = s.split()[0] if s else ''
    if m.startswith('call'):
        return 'call'
    if m.startswith('j'):
        return m
    return re.sub(r'(0x[0-9A-Fa-f]+|[0-9])\(%esp\)', 'S', re.sub(r'0x[0-9A-Fa-f]{5,8}\b', 'G', s))


def fasm(coff, addr):
    l = [x.strip() for x in ps.function_asm(coff, ps.find_function(coff, '_%X@' % addr)).split('\n') if x.strip()]
    while l and l[-1] in ('nop', 'int3'):
        l.pop()
    return l


_target = None


def target(addr):
    global _target
    if _target is None:
        p = os.path.join(ROOT, 'Scripts', 'bin_comp', 'target_asm.json')
        if not os.path.exists(p):
            sys.exit('no %s: git show origin/claude/target-asm:target_asm.json > %s' % (p, p))
        _target = json.load(open(p))
    t = _target.get(hex(addr))
    return None if t is None else [norm(re.sub(r'^[0-9a-f]+:\s*', '', x).strip()) for x in t['asm'].split('\n') if x.strip()]


def diff_lines(coff, addr):
    A = target(addr)
    B = [norm(x) for x in fasm(coff, addr)]
    return sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in
               difflib.SequenceMatcher(None, A, B, autojunk=False).get_opcodes() if tag != 'equal')


def load(out):
    return ps.Coff(open(os.path.join(out, 'last.obj'), 'rb').read())


def run_parallel(jobs, specs, fn):
    """fn(job, spec) for each spec, at most `jobs` at a time, each worker with its own job dir."""
    if jobs <= 1:
        return [fn(1, s) for s in specs]
    free = list(range(1, jobs + 1))
    import threading
    lock = threading.Lock()

    def wrap(s):
        with lock:
            j = free.pop()
        try:
            return fn(j, s)
        finally:
            with lock:
                free.append(j)
    with ThreadPoolExecutor(jobs) as ex:
        return list(ex.map(wrap, specs))


def one_function(a, lo, hi):
    addr = int(a.addr, 16)
    if target(addr) is None:
        sys.exit('%s is not in target_asm.json (only WIP/STUB functions are)' % a.addr)
    base = compile_tu(a.src, 0, log=True)
    F = [f for f in nodes.parse(os.path.join(base, 'last.log')) if a.func in f['name'] and 'Marker_0x' not in f['name']]
    if len(F) != 1:
        sys.exit('%d functions match %r: %s' % (len(F), a.func, [f['name'] for f in F][:8]))
    f = F[0]
    sizes = [len(w) for w in f['win']]
    n = min(len(sizes) + 4, 64)
    print('#%d %s' % (f['ord'], f['name']))
    print('windows', sizes, flush=True)

    def score(job, lims):
        out = compile_tu(a.src, job, '%d:%s' % (f['ord'], ','.join(map(str, lims))))
        return diff_lines(load(out), addr)

    best = score(1, [STOCK] * n)
    lims = [STOCK] * n
    print('stock limit 80: %d differing lines' % best, flush=True)
    Ls = list(range(lo, hi + 1))
    for L, s in zip(Ls, run_parallel(a.j, [[L] * n for L in Ls], score)):
        print('  all windows %d: %d' % (L, s), flush=True)
        if s < best:
            best, lims = s, [L] * n
    cand = [i for i in range(n) if i >= len(sizes) or sizes[i] >= a.min_window]
    for i in cand:
        if best == 0:
            break
        tries = [L for L in Ls if L != lims[i]]
        res = run_parallel(a.j, [lims[:i] + [L] + lims[i + 1:] for L in tries], score)
        for L, s in zip(tries, res):
            if s < best:
                best, lims = s, lims[:i] + [L] + lims[i + 1:]
        print('  window %d -> %s: %d' % (i + 1, lims[i], best), flush=True)
    while len(lims) > 1 and lims[-1] == STOCK:
        lims.pop()
    print('best %d with LIM=%d:%s' % (best, f['ord'], ','.join(map(str, lims))))
    print('extra nodes before each window break (80 - limit):', [STOCK - x for x in lims])


def whole_tu(a, lo, hi):
    text = open(a.src, encoding='latin-1').read()
    marks = [(k, int(x, 16)) for k, x in re.findall(r'(MATCH|WIP)_FUNC\((0x[0-9A-Fa-f]+)\)', text)]
    base = load(compile_tu(a.src, 0))
    wips = [x for k, x in marks if k == 'WIP' and target(x) is not None]
    matches = [x for k, x in marks if k == 'MATCH']

    def safe(fn, *args):
        try:
            return fn(*args)
        except BaseException:
            return None
    base_match = {x: safe(fasm, base, x) for x in matches}

    def evaluate(job, L):
        c = load(compile_tu(a.src, job, str(L))) if L != STOCK else base
        per = {x: safe(diff_lines, c, x) for x in wips}
        changed = [x for x in matches if base_match[x] is not None and safe(fasm, c, x) != base_match[x]]
        return per, changed

    Ls = [STOCK] + [L for L in range(lo, hi + 1) if L != STOCK]
    res = run_parallel(a.j, Ls, evaluate)
    print('%d WIP functions with a target, %d MATCH functions' % (len(wips), len(matches)))
    for L, (per, changed) in zip(Ls, res):
        tot = sum(v for v in per.values() if v is not None)
        print('limit %d: %d lines, zero: %s, MATCH changed: %s' % (
            L, tot, ' '.join('%X' % x for x, v in per.items() if v == 0) or '-',
            ' '.join('%X' % x for x in changed) or '-'), flush=True)


def main():
    ap = argparse.ArgumentParser(usage=__doc__)
    ap.add_argument('src')
    ap.add_argument('func', nargs='?')
    ap.add_argument('addr', nargs='?')
    ap.add_argument('--tu', action='store_true')
    ap.add_argument('--range', default='62:80')
    ap.add_argument('--min-window', type=int, default=40)
    ap.add_argument('-j', type=int, default=1)
    a = ap.parse_args()
    a.src = os.path.abspath(a.src)
    lo, hi = [int(x) for x in a.range.split(':')]
    if a.tu:
        whole_tu(a, lo, hi)
    elif a.func and a.addr:
        one_function(a, lo, hi)
    else:
        ap.error('FUNC ADDR or --tu')


if __name__ == '__main__':
    main()
