"""
Scores every WIP function compiled with two C2.DLL variants (build_vc6/x87_c2/<variant>), per TU in parallel:

    venv/bin/python3 Scripts/flowopt/sweep.py stock cjrev [-j 8] [files...]

Prints `addr name score_a score_b` for each WIP whose score differs, then the totals.
"""
import os, re, subprocess, sys, glob
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
PY = os.path.join(ROOT, 'venv', 'bin', 'python3')
OUT = os.path.join(ROOT, 'build_vc6', 'x87_c2', 'sweep')

def wips(path):
    lines = open(path, encoding='latin-1').read().split('\n')
    res = []
    for i, l in enumerate(lines):
        m = re.match(r'\s*WIP_FUNC\((0x[0-9A-Fa-f]+)\)', l)
        if not m:
            continue
        for l2 in lines[i + 1:i + 6]:
            n = re.search(r'([A-Za-z_][A-Za-z0-9_]*)\s*\(', l2)
            if n:
                res.append((m.group(1).lower(), n.group(1))); break
    return res

def compile_(var, src):
    d = os.path.join(OUT, var, os.path.basename(src))
    os.makedirs(d, exist_ok=True)
    env = dict(os.environ, X87_C2=var, X87_OUT=d)
    subprocess.run([os.path.join(ROOT, 'Scripts/x87_sched/sched.sh'), '-l', src], env=env,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=900)
    return os.path.join(d, 'last.obj')

def score(obj, addr, name):
    if not os.path.exists(obj):
        return None
    r = subprocess.run([PY, os.path.join(ROOT, 'Scripts/bin_comp/permuter_score.py'), obj, addr, name],
                       capture_output=True, text=True, cwd=os.path.join(ROOT, 'Scripts/bin_comp'))
    try:
        return int(r.stdout.strip().split('\n')[-1])
    except ValueError:
        return None

def main():
    args = sys.argv[1:]
    j = 8
    if '-j' in args:
        i = args.index('-j'); j = int(args[i + 1]); del args[i:i + 2]
    va, vb = args[0], args[1]
    files = args[2:] or sorted(f for f in glob.glob(os.path.join(ROOT, 'Source', '*.cpp'))
                               if 'WIP_FUNC(' in open(f, encoding='latin-1').read() and not f.endswith('_q.cpp'))
    jobs = [(v, f) for f in files for v in (va, vb)]
    with ThreadPoolExecutor(j) as ex:
        objs = dict(zip(jobs, ex.map(lambda a: compile_(*a), jobs)))
    ta = tb = 0
    for f in files:
        for addr, name in wips(f):
            a, b = score(objs[(va, f)], addr, name), score(objs[(vb, f)], addr, name)
            if a is None or b is None:
                continue
            ta += a; tb += b
            if a != b:
                print(f'{addr} {name} {a} {b}' + ('  MATCH' if b == 0 else ''), flush=True)
    print(f'total {ta} {tb}')

if __name__ == '__main__':
    main()
