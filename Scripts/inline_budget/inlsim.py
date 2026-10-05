#!/usr/bin/env python3
"""Re-implementation of the VC6 (C2.DLL 12.00.8168/8804) inline budget, driven by the patched-C2 log.

LOG is the raw log of inl.sh (build_vc6/inline_c2/last.log) or probe.sh.

usage:
  inlsim.py LOG --verify                  re-simulate every function in LOG and compare with the log
  inlsim.py LOG NEEDLE [--scan A:B]       simulate NEEDLE's function; with --scan, vary the caller's
                                          FE size by A..B and print the out-of-line call multiset per delta
  inlsim.py LOG NEEDLE --size NAME=+D     change the FE size of callee NAME (substring) by D before simulating
  inlsim.py LOG NEEDLE --extra N@K[,..]   add N free (size<=40) sites before top-level site index K
  inlsim.py LOG NEEDLE --list             list the top-level sites with their index

Model (C2.DLL 0x1073b588 / 0x1073b633):
  budget(top) = clamp(2 * callersize, 1000, 35000); total = callersize
  walk the call sites of the function body in IL order; sites_left = sites not yet visited (incl. this one)
    reject if depth > inline_depth (8) or arg count mismatch
    if not __forceinline:
        reject if callee.size > 40 and callee.size > budget
        reject if total > 35000
    accept:
        if not __forceinline: (if callee.size > 40: budget -= callee.size); total += callee.size
        used = walk(callee body, depth + 1, budget / sites_left)        (C division)
        if not __forceinline: budget -= used; total += used
  walk returns budget_in - budget_out
"""
import re, sys
from collections import Counter

FREE = 40
CAP = 35000


class Site:
    def __init__(self, name, size, force, argok, maxdepth, src=None):
        self.name, self.size, self.force, self.argok, self.maxdepth = name, size, force, argok, maxdepth
        self.children = None  # None: never expanded in the log (body unknown)
        self.logged = None    # 'INLINE' / 'OUT' as logged
        self.src = src


class Func:
    def __init__(self, name, size):
        self.name, self.size, self.sites = name, size, []


def parse(path):
    funcs = []
    stack = []  # list of site lists being filled; stack[-1] is the current body
    cur_site = []  # per depth: the last SITE seen
    last = None
    for ln in open(path, errors='replace'):
        m = re.search(r'([^\\/]+)\((\d+)\) : warning C4711: function \'(INLINE|OUTLINE|SKIP)', ln)
        if m and last is not None:
            last.src = '%s:%s' % (m.group(1), m.group(2))
            continue
        if not ln.startswith('@I '):
            continue
        b = ln[3:].strip()
        kind = b.split()[0]
        kv = dict(re.findall(r'(\w+)=(-?\w+)', b))
        if kind == 'ENTER':
            depth = int(kv['depth'])
            if depth == 1:
                f = Func(re.search(r'caller=(.*) callersize=', b).group(1), int(kv['callersize']))
                funcs.append(f)
                stack = [f.sites]
            else:
                # expanding the last accepted site at depth-1
                s = last_acc[depth - 1]
                s.children = []
                stack = stack[:depth - 1] + [s.children]
        elif kind == 'SITE':
            depth = int(kv['depth'])
            name = re.search(r'callee=(.*) size=', b).group(1)
            s = Site(name, int(kv['size']), bool(int(kv['flags73'], 16) & 0x2000), kv['argok'] == '1',
                     int(kv['maxdepth']))
            s.depth = depth
            s.log_budget = int(kv['budget'])
            s.log_left = int(kv['count'])
            stack = stack[:depth]
            stack[depth - 1].append(s)
            last = s
        elif kind == 'ACCEPT':
            last.logged = 'INLINE'
            last_acc_set(last)
        elif kind == 'REJECT':
            last.logged = 'OUT'
        elif kind == 'SKIP':
            last.logged = 'SKIP'
    return funcs


last_acc = {}


def last_acc_set(s):
    last_acc[s.depth] = s


def catalog(funcs):
    """callee name -> child site list from any logged expansion"""
    cat = {}
    def visit(sites):
        for s in sites:
            if s.children is not None:
                if s.name not in cat or len(s.children) > len(cat[s.name]):
                    cat[s.name] = s.children
                visit(s.children)
    for f in funcs:
        visit(f.sites)
    return cat


def cdiv(a, b):
    q = abs(a) // abs(b)
    return q if (a >= 0) == (b > 0) else -q


class Sim:
    def __init__(self, cat, size_delta=None):
        self.cat = cat
        self.size_delta = size_delta or {}
        self.unknown = set()

    def size(self, s):
        d = 0
        for k, v in self.size_delta.items():
            if k in s.name:
                d += v
        return s.size + d

    def walk(self, sites, depth, budget, out, path):
        b0 = budget
        n = len(sites)
        for i, s in enumerate(sites):
            left = n - i
            size = self.size(s)
            if s.logged == 'SKIP':   # call constructing a by-value argument in place: never inlined, free
                out.append((path + (i,), s, 'SKIP', budget, left))
                continue
            ok = depth <= s.maxdepth and s.argok
            if ok and not s.force:
                if size > FREE and size > budget:
                    ok = False
                elif self.total > CAP:
                    ok = False
            if not ok:
                out.append((path + (i,), s, 'OUT', budget, left))
                continue
            if not s.force:
                if size > FREE:
                    budget -= size
                self.total += size
            out.append((path + (i,), s, 'INLINE', budget, left))
            body = s.children if s.children is not None else self.cat.get(s.name)
            if body is None:
                self.unknown.add(s.name)
                body = []
            used = self.walk(body, depth + 1, cdiv(budget, left), out, path + (i,))
            if not s.force:
                budget -= used
                self.total += used
        return b0 - budget

    def run(self, f, size_delta=0):
        S = f.size + size_delta
        self.total = S
        out = []
        self.walk(f.sites, 1, min(max(2 * S, 1000), CAP), out, ())
        return out


def ool(out):
    return Counter(s.name for p, s, d, b, l in out if d != 'INLINE')


def verify(funcs):
    cat = catalog(funcs)
    bad = 0; nsites = 0
    for f in funcs:
        sim = Sim(cat)
        out = sim.run(f)
        # compare only the logged sites (walk the logged tree in the same order)
        logged = []
        def visit(sites, path):
            for i, s in enumerate(sites):
                logged.append((path + (i,), s))
                if s.logged == 'INLINE' and s.children is not None:
                    visit(s.children, path + (i,))
        visit(f.sites, ())
        simd = {p: (d, b, l) for p, s, d, b, l in out}
        for p, s in logged:
            nsites += 1
            d = simd.get(p)
            if d is None or d[0] != s.logged:
                bad += 1
                print('MISMATCH', f.name, p, s.name, 'log', s.logged, 'sim', d)
    print('verified %d functions, %d sites, %d mismatches' % (len(funcs), nsites, bad))


def main():
    a = sys.argv[1:]
    funcs = parse(a[0])
    if '--verify' in a:
        verify(funcs); return
    needle = a[1]
    cand = [f for f in funcs if needle in f.name]
    if not cand:
        print('no function', needle); return
    f = cand[0]
    cat = catalog(funcs)
    sd = {}
    if '--size' in a:
        for item in a[a.index('--size') + 1].split(','):
            k, v = item.split('=')
            sd[k] = int(v)
    if '--extra' in a:
        # --extra N@K[,N@K...]: insert N free (size 20) sites before top-level site index K
        for item in sorted(a[a.index('--extra') + 1].split(','), key=lambda x: -int(x.split('@')[1])):
            n, k = map(int, item.split('@'))
            for _ in range(n):
                d = Site('<extra free site>', 20, False, True, 8); d.children = []
                f.sites.insert(k, d)
    if '--list' in a:
        for i, s in enumerate(f.sites):
            print(i, s.src, s.name[:80], s.size)
        return
    if '--scan' in a:
        lo, hi = map(int, a[a.index('--scan') + 1].split(':'))
        prev = None
        for d in range(lo, hi + 1):
            sim = Sim(cat, sd)
            o = ool(sim.run(f, d))
            key = sorted(o.items())
            if key != prev:
                print('caller size %+d (%d):' % (d, f.size + d), ', '.join('%s x%d' % (k, v) for k, v in key) or '(all inlined)')
                prev = key
        return
    sim = Sim(cat, sd)
    out = sim.run(f)
    print('%s  size=%d budget=%d' % (f.name, f.size, min(max(2 * f.size, 1000), CAP)))
    for p, s, d, b, l in out:
        if d != 'INLINE' or s.size > FREE or s.force:
            print('%-24s %s%-8s %-60s size=%-4d budget=%-6d left=%d' % (s.src or '', '  ' * len(p), d, s.name[:60], sim.size(s), b, l))
    if sim.unknown:
        print('bodies unknown (treated as empty):', ', '.join(sorted(sim.unknown)))


if __name__ == '__main__':
    main()
