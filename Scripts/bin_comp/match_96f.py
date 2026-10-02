"""
Pair 10.5 functions with their 9.6f versions using fingerprints.json (dump_fingerprints.py,
written by the "Dump target asm" workflow) and the known pairs in Scripts/ida/functions_data.json.

Seeds: known pairs, unique names, unique strings / imports. Then the pairs are grown along the
call graph and through globals: the call and global lists of a matched pair are aligned and the
counterparts in the same position are paired when their fingerprints agree.

Writes match_96f.json: {"pairs": {v105: v96f}, "inlined": {v96f: [v105 callers]}}. A 9.6f
function with no 10.5 partner whose 9.6f callers are paired is an inline candidate: 10.5
inlined it into those callers.
"""
import difflib
import json
import re
from collections import Counter, defaultdict

fp = json.load(open("fingerprints.json"))
A, B = fp["v105"], fp["v96f"]  # hex addr -> fingerprint


def h(x):
    return hex(x) if isinstance(x, int) else x


for side in (A, B):
    for k, v in side.items():
        v["c"] = [h(c) for c in v["c"]]
        v["g"] = [h(g) for g in v["g"]]

pairs, rpairs = {}, {}
gmap = {}  # 10.5 global -> 9.6f global
why = {}


def base_name(n):
    n = re.sub(r"_[0-9A-Fa-f]{6}$", "", n)
    return None if re.match(r"^(sub|nullsub|j|unknown|loc)_|^nullsub", n) else n


def sim(a, b):
    fa, fb = A[a], B[b]
    ma, mb = fa["m"].split(), fb["m"].split()
    if not ma or not mb:
        return 0.0
    sm = difflib.SequenceMatcher(None, ma[:600], mb[:600], autojunk=False)
    r = sm.quick_ratio()
    if r < 0.3:
        return r
    return sm.ratio()


def plausible(a, b):
    """Small functions all look alike: demand near identical code for them."""
    ia = {c for c in A[a]["c"] if not c.startswith("0x")}
    ib = {c for c in B[b]["c"] if not c.startswith("0x")}
    if ia != ib and (ia or ib) and not (ia & ib):
        return False
    sa, sb = A[a]["size"], B[b]["size"]
    if min(sa, sb) < 48:
        return abs(sa - sb) <= 6 and sim(a, b) >= 0.85
    return True


def add(a, b, reason):
    if a in pairs or b in rpairs or a not in A or b not in B:
        return False
    pairs[a], rpairs[b] = b, a
    why[a] = reason
    return True


# 1. known pairs
for e in json.load(open("../ida/functions_data.json")):
    if e["v96f_address"] and e["v105_address"] and not e.get("v96f_auto"):
        add(hex(int(e["v105_address"], 16)), hex(int(e["v96f_address"], 16)), "known")

# 2. unique names
na, nb = defaultdict(list), defaultdict(list)
for k, v in A.items():
    if base_name(v["name"]):
        na[base_name(v["name"])].append(k)
for k, v in B.items():
    if base_name(v["name"]):
        nb[base_name(v["name"])].append(k)
for n, l in na.items():
    if len(l) == 1 and len(nb.get(n, [])) == 1:
        add(l[0], nb[n][0], "name")


# 3. unique string / import signatures
def keyset(v, kind):
    if kind == "s":
        return frozenset(v["s"])
    return frozenset(c for c in v["c"] if not c.startswith("0x"))


for kind in ("s", "i"):
    ka, kb = defaultdict(list), defaultdict(list)
    for k, v in A.items():
        s = keyset(v, kind)
        if s:
            ka[s].append(k)
    for k, v in B.items():
        s = keyset(v, kind)
        if s:
            kb[s].append(k)
    for s, l in ka.items():
        if len(l) == 1 and len(kb.get(s, [])) == 1 and sim(l[0], kb[s][0]) > 0.4:
            add(l[0], kb[s][0], "strings" if kind == "s" else "imports")


# 4. propagate along calls and globals
def align(la, lb, known):
    """Pair up items of la/lb that sit between equal known items."""
    ta = [known.get(x, "?a" + x) for x in la]
    tb = [x if x in known.values() else "?b" + x for x in lb]
    out = []
    sm = difflib.SequenceMatcher(None, ta, tb, autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "replace":
            ua = [la[i] for i in range(i1, i2) if la[i] not in known]
            ub = [lb[j] for j in range(j1, j2) if lb[j] not in known.values()]
            if len(ua) == len(ub):
                out += list(zip(ua, ub))
            elif ua and ub and len(ua) * len(ub) <= 64:
                out += best_pairs(ua, ub)
    return out


def best_pairs(ua, ub, thresh=0.0):
    """Greedy pairing of two candidate lists by similarity."""
    sc = []
    for x in dict.fromkeys(ua):
        for y in dict.fromkeys(ub):
            if x in A and y in B and plausible(x, y):
                r = sim(x, y)
                if r > thresh:
                    sc.append((r, x, y))
    sc.sort(reverse=True)
    used_a, used_b, out = set(), set(), []
    for r, x, y in sc:
        if x not in used_a and y not in used_b:
            used_a.add(x); used_b.add(y); out.append((x, y))
    return out


A_sorted = sorted(A, key=lambda k: int(k, 16))
B_sorted = sorted(B, key=lambda k: int(k, 16))
B_pos = {k: i for i, k in enumerate(B_sorted)}


def fill_gaps():
    """Unpaired functions between two paired neighbours whose partners are close in 9.6f too."""
    n = 0
    prev = None
    run = []
    for a in A_sorted:
        if a not in pairs:
            run.append(a)
            continue
        if prev is not None and run:
            i, j = B_pos[pairs[prev]], B_pos[pairs[a]]
            if 0 < j - i <= len(run) + 40:
                ub = [b for b in B_sorted[i + 1:j] if b not in rpairs]
                for x, y in best_pairs(run, ub, 0.6):
                    n += add(x, y, f"between {prev} {a}")
        prev, run = a, []
    return n


changed = True
rounds = 0
votes = defaultdict(set)
ncall_a = Counter(c for v in A.values() for c in set(v["c"]))
ncall_b = Counter(c for v in B.values() for c in set(v["c"]))
while changed:
    changed = False
    rounds += 1
    for a, b in list(pairs.items()):
        fa, fb = A[a], B[b]
        for ga, gb in align(fa["g"], fb["g"], gmap):
            if ga not in gmap:
                gmap[ga] = gb
        for ca, cb in align([c for c in fa["c"] if c in A], [c for c in fb["c"] if c in B], pairs):
            if ca in pairs or cb in rpairs:
                continue
            if ncall_b[cb] > 2 * ncall_a[ca] + 3:
                continue  # a 9.6f helper called from far more places: an inline, not this callee
            s = sim(ca, cb)
            if not plausible(ca, cb):
                if s > 0.5:
                    votes[(ca, cb)].add(a)
                continue
            if s > 0.6 or (s > 0.45 and abs(A[ca]["size"] - B[cb]["size"]) < 0.3 * A[ca]["size"]):
                changed |= add(ca, cb, f"call of {a}")
            elif s > 0.3 and max(A[ca]["size"], B[cb]["size"]) <= 3 * min(A[ca]["size"], B[cb]["size"]):
                votes[(ca, cb)].add(a)
    # weaker pairs (the callee inlines different things in each build) that several callers agree on
    best = defaultdict(list)
    for (ca, cb), vs in votes.items():
        if ca not in pairs and cb not in rpairs:
            best[ca].append((len(vs), cb))
    for ca, l in best.items():
        l.sort(reverse=True)
        cb = l[0][1]
        if min(A[ca]["size"], B[cb]["size"]) < 32 and sim(ca, cb) < 0.75:
            continue
        if l[0][0] >= 2 and (len(l) == 1 or l[1][0] < l[0][0]):
            changed |= add(ca, l[0][1], f"votes {l[0][0]}")
    # unique global signatures
    ga_sig = defaultdict(list)
    for k, v in A.items():
        if k in pairs:
            continue
        s = frozenset(gmap[g] for g in v["g"] if g in gmap)
        if len(s) >= 2:
            ga_sig[s].append(k)
    bidx = defaultdict(set)  # 9.6f global -> unpaired 9.6f functions using it
    for k, v in B.items():
        if k not in rpairs:
            for g in v["g"]:
                bidx[g].add(k)
    for s, l in ga_sig.items():
        if len(l) != 1:
            continue
        cands = set.intersection(*(bidx.get(g, set()) for g in s))
        cands = [k for k in cands if k not in rpairs]
        if len(cands) == 1 and plausible(l[0], cands[0]) and sim(l[0], cands[0]) > 0.5:
            changed |= add(l[0], cands[0], "globals")
    changed |= fill_gaps() > 0
    print(f"round {rounds}: {len(pairs)} pairs, {len(gmap)} globals")

# inlines: 9.6f callees of a paired function whose 10.5 partner (if any) the 10.5 function
# doesn't call, so 10.5 inlined them there
inlined = defaultdict(list)  # 9.6f callee -> 10.5 functions that inlined it
per_func = {}
for a, b in pairs.items():
    ca = set(A[a]["c"])
    miss = []
    for c in dict.fromkeys(B[b]["c"]):
        if not c.startswith("0x") or c not in B:
            continue
        p = rpairs.get(c)
        if p is None or p not in ca:
            miss.append(c)
            inlined[c].append(a)
    if miss:
        per_func[a] = miss
score = {a: round(sim(a, b), 2) for a, b in pairs.items()}
json.dump({"pairs": pairs, "score": score, "why": why, "inlined": inlined, "per_func": per_func, "globals": gmap},
          open("match_96f.json", "w"), indent=1)
print(f"{len(pairs)}/{len(A)} 10.5 functions paired, {len(inlined)} inline candidates")
print(Counter(w.split(" ")[0] for w in why.values()))
