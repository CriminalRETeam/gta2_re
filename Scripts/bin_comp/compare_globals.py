"""
For each WIP/STUB function in target_asm.json, compare the data the original references (globals,
float constants, other absolute addresses) with what the build references.

The verifier's post processing renames every absolute address to stable_name_N, so reading
kFpOne_6FF07C where the original read kFpHalf_6FEEE8 still "matches" line for line and only
shows up as a logic bug in game. This lists such differences.

Build side: addresses are named through build_vc6/output.map, and a name is mapped to its
original address through new_data.json's variables (DEFINE_GLOBAL) or an _ADDRESS name suffix.
Float constants (__real@...) are compared by value with target_data.json. Data the build has no
original address for (strings, vtables, ...) is shown by name but not paired.

Score per function (lower = more likely a bug): 0 same typed globals at aligned positions,
1-3 other aligned pairs, 4 same typed globals used more often on opposite sides, 6 the original
reads data we have no global for. Aligned pairs are kept only when the original's global is used
more often there and ours more often in the build, so register caching alone doesn't show up.
MATCH functions are not in target_asm.json and can't be checked this way.

Usage (from Scripts/bin_comp, after a build and msvc_dump_new_data.py):
    python3 compare_globals.py              # all WIP/STUB functions, likely bugs first
    python3 compare_globals.py 4b8f30 ...   # only these functions
    python3 compare_globals.py --all        # also list functions that only use globals a
                                            # different number of times (mostly noise)
    --root DIR                              # repo root holding build_vc6/ (default ../..)
"""
import argparse, bisect, collections, json, re, struct, difflib
import compare_function

ap = argparse.ArgumentParser()
ap.add_argument('addrs', nargs='*')
ap.add_argument('--root', default='../..')
ap.add_argument('--all', action='store_true', help='also show functions with only unpaired differences')
args = ap.parse_args()

t = json.load(open('target_asm.json'))
tdata = json.load(open('target_data.json'))
nd = json.load(open('new_data.json'))

ogfunc = {}
for l in open('og_function_data_v105.csv'):
    r = l.strip().split(',')
    if len(r) >= 4:
        try: ogfunc[int(r[1], 16)] = r[0]
        except ValueError: pass

# original addresses of our globals
og_of_name = {}       # plain name -> og address
var_at_og = {}        # og address -> (name, type)
def mangled_type(m):
    x = re.match(r'\?[^@]+@(?:[^@]+@)*@3(.*)A$', m or '')
    return x.group(1) if x else '?'
for v in nd['variables']:
    try: a = int(v['og_address'], 16)
    except (ValueError, TypeError): continue
    if a < 0x400000: continue
    og_of_name[v['mangled_name']] = a
    og_of_name[v['name']] = a
    var_at_og[a] = (v['name'], mangled_type(v['mangled_name']))
var_og_sorted = sorted(var_at_og)

mine = {}; va2og = {}
for f in nd['functions']:
    try: og = int(f['og_addr'], 16)
    except (ValueError, TypeError): continue
    mine[og] = f; va2og[int(f['func_va'], 16)] = og

# build symbols from the map
code_syms, data_syms = {}, {}
for l in open(args.root + '/build_vc6/output.map', errors='ignore'):
    m = re.match(r'\s*000(\d):[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8}) ', l)
    if not m: continue
    (code_syms if m.group(1) == '1' else data_syms)[int(m.group(3), 16)] = m.group(2)
code_starts = sorted(code_syms); data_starts = sorted(data_syms)
# globals not defined with DEFINE_GLOBAL: take the original address from an _ADDRESS name suffix
for n in data_syms.values():
    sm = re.match(r'\?([A-Za-z_]\w*_([0-9A-F]{6}))@@3', n)
    if sm and n not in og_of_name:
        a = int(sm.group(2), 16)
        og_of_name[n] = a
        if a not in var_at_og: var_at_og[a] = (sm.group(1), mangled_type(n))
var_og_sorted = sorted(var_at_og)
exe = args.root + '/build_vc6/decomp_main.exe'

def our_size(va):
    i = bisect.bisect_right(code_starts, va)
    return (code_starts[i] - va) if i < len(code_starts) else 0x1000

def demangle_short(n):
    m = re.match(r'\?([^@]+)@(?:([^@]+)@)?', n)
    if not m or n.startswith('??'): return n
    return (m.group(2) + '::' if m.group(2) else '') + m.group(1)

def ext80(h):
    se, mant = int(h[:4], 16), int(h[4:], 16)
    if se & 0x7fff == 0 and mant == 0: return 0.0
    v = mant / (1 << 63) * 2.0 ** ((se & 0x7fff) - 16383)
    return -v if se & 0x8000 else v

def fkey(v):
    return ('f', float('%.6g' % v))

def float_size(mn):
    # x87 memory operand size from the GAS suffix; plain moves treated as 4 bytes
    if mn.startswith('f'):
        if mn.endswith('l'): return 8
        if mn.endswith('t'): return 10
    return 4

def og_name(a):
    if a in var_at_og: return var_at_og[a][0]
    i = bisect.bisect_right(var_og_sorted, a) - 1
    if i >= 0:
        b = var_og_sorted[i]; n, ty = var_at_og[b]
        if a - b < 0x400: return '%s+0x%x?' % (n, a - b)
    if a in ogfunc: return ogfunc[a]
    return hex(a)

def og_type(a):
    return var_at_og.get(a, (None, None))[1]

# one ref = (instruction index, key, display, type[, maybe WIP flag])
OG_CODE_END = 0x5FE000
OG_IAT_END = 0x5FE3C8
DATA_START = min(data_starts)
ADDR = re.compile(r'(\$|\*)?0x([0-9A-Fa-f]+)(\()?')
def refs_of(asm, side):
    out = []
    lines = asm.split('\n')
    wip_flags = set()
    if side != 'og':
        for line in lines:
            fm = re.match(r'(?:movb \$(?:0x)?1|mov %[a-d]l),0x([0-9A-F]+)$', line)
            if fm: wip_flags.add(int(fm.group(1), 16))
    for i, line in enumerate(lines):
        parts = line.split(None, 1)
        if len(parts) < 2: continue
        mn, ops = parts
        if mn.startswith('j') or mn.startswith('call') or mn.startswith('loop'): continue
        if mn == 'push' and i == 1 and lines[0].startswith('push $0xFFFFFFFF'): continue  # SEH handler
        for m in ADDR.finditer(ops):
            v = int(m.group(2), 16)
            if not 0x400000 <= v < 0x800000: continue
            if side == 'og':
                if v < OG_CODE_END and v not in ogfunc: continue   # jump tables, flag immediates
                if OG_CODE_END <= v < OG_IAT_END and hex(v) not in tdata: continue  # imports
                if m.group(1) == '$' and v not in var_at_og and v not in ogfunc and hex(v) not in tdata: continue
                if hex(v) in tdata and m.group(1) != '$' and (mn.startswith('f') or 'ss' in mn or 'sd' in mn):
                    raw = bytes.fromhex(tdata[hex(v)])
                    sz = float_size(mn)
                    if sz == 8 and len(raw) >= 8: fv = struct.unpack('<d', raw[:8])[0]
                    elif len(raw) >= 4: fv = struct.unpack('<f', raw[:4])[0]
                    else: fv = None
                    if fv is not None:
                        out.append((i, fkey(fv), '%s=%g' % (hex(v), fv), 'float')); continue
                k = ('a', v)
                out.append((i, k, og_name(v), og_type(v)))
            else:
                if v in va2og:
                    out.append((i, ('a', va2og[v]), ogfunc.get(va2og[v], hex(va2og[v])), 'code')); continue
                j = bisect.bisect_right(data_starts, v) - 1
                if j < 0 or v < DATA_START: continue
                base = data_starts[j]; n = data_syms[base]; off = v - base
                if n.startswith('__imp_'): continue
                if m.group(1) == '$' and off: continue
                flag = bool(off) and v in wip_flags   # maybe WIP_IMPLEMENTED's static done___ flag
                fm = re.match(r'__real@(\d+)@([0-9a-f]{20})$', n)
                if fm and off == 0:
                    fv = ext80(fm.group(2)); out.append((i, fkey(fv), '%s=%g' % ('real', fv), 'float')); continue
                og = og_of_name.get(n)
                plain = demangle_short(n)
                if og is None:
                    sm = re.search(r'_([0-9A-Fa-f]{6})(?:@|$)', plain)
                    if sm: og = int(sm.group(1), 16)
                disp = plain + ('+0x%x' % off if off else '')
                if og is not None and og >= 0x400000:
                    out.append((i, ('a', og + off), disp, mangled_type(n) if og == og_of_name.get(n) else og_type(og + off), flag))
                else:
                    out.append((i, ('n', disp), disp, None, flag))
    return out

def score_pair(o, m):
    # lower = more likely a real logic bug
    ko, km = o[1], m[1]
    if ko[0] == 'f' and km[0] == 'f': return 0
    if ko[0] == 'a' and km[0] == 'a':
        if o[3] and o[3] == m[3] and o[3] not in ('?', 'code'): return 0
        if ko[1] in var_at_og and km[1] in var_at_og: return 1
        if abs(ko[1] - km[1]) < 0x40: return 3   # likely struct/array field offset
        return 2
    return 5

addrs = [int(a, 16) for a in args.addrs] if args.addrs else None
rows = []
for k, v in t.items():
    a = int(k, 16)
    if addrs is not None and a not in addrs: continue
    if addrs is None and v['status'] not in ('0x2', '0x0'): continue
    f = mine.get(a)
    if not f or not v.get('asm'): continue
    va = int(f['func_va'], 16)
    ours_asm = compare_function.dism_func(compare_function.get_bytes_from_file(exe, int(f['func_fo'], 16), our_size(va)))
    to = refs_of(v['asm'], 'og'); tm = refs_of(ours_asm, 'ours')
    ogkeys = {r[1] for r in to}
    tm = [r for r in tm if not (len(r) > 4 and r[4] and r[1] not in ogkeys)]
    ko = [r[1] for r in to]; km = [r[1] for r in tm]
    if ko == km: continue
    pairs, only_o, only_m = [], [], []
    sm = difflib.SequenceMatcher(None, ko, km, autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal': continue
        if op == 'replace' and i2 - i1 == j2 - j1:
            for x, y in zip(to[i1:i2], tm[j1:j2]):
                if x[1][0] == 'n' or y[1][0] == 'n':
                    if not (y[1][0] == 'n' and x[1][0] == 'a' and x[1][1] not in var_at_og): only_o.append(x)
                    only_m.append(y)
                else: pairs.append((score_pair(x, y), x, y))
        else:
            # unknown og data (strings, ...) against unnamed build data (??_C@ strings, ...)
            nn = sum(1 for y in tm[j1:j2] if y[1][0] == 'n')
            for x in to[i1:i2]:
                if nn and x[1][0] == 'a' and x[1][1] not in var_at_og and x[1][1] not in ogfunc:
                    nn -= 1; continue
                only_o.append(x)
            only_m += tm[j1:j2]
    # a key used as often on both sides only moved (block order, register allocation)
    co = collections.Counter(ko); cm = collections.Counter(km)
    # a real swap A -> B leaves A used more often in the original and B more often in ours
    has_n = any(r[1][0] == 'n' for r in tm)
    known = lambda key: key[0] != 'a' or key[1] in var_at_og or key[1] in ogfunc
    # unknown original data (mostly string literals) against unnamed build data
    only_o = [r for r in to if co[r[1]] > cm[r[1]] and (known(r[1]) or not has_n)]
    only_m = [r for r in tm if cm[r[1]] > co[r[1]] and r[1][0] != 'n']
    pairs = [p for p in pairs if co[p[1][1]] > cm[p[1][1]] and cm[p[2][1]] > co[p[2][1]]]
    # same typed globals used more often on opposite sides, but not at aligned positions
    loose = []
    if not pairs:
        uo = {r[1]: r for r in only_o}; um = {r[1]: r for r in only_m}
        for x in uo.values():
            for y in um.values():
                if x[3] and x[3] == y[3] and x[3] not in ('?', 'code'):
                    loose.append((x, y))
                elif not known(x[1]) and y[3] not in (None, 'code', 'float'):
                    loose.append((x, y))   # original reads data we have no global for
    if not pairs and not loose and not (args.all or addrs is not None): continue
    if not pairs and not only_o and not only_m: continue
    best = min([p[0] for p in pairs] or [min(4 if known(x[1]) else 6 for x, y in loose) if loose else 9])
    cnt = lambda r: '%s (%d/%d)' % (r[2], co[r[1]], cm[r[1]])
    only_o = [r[:2] + (cnt(r),) + r[3:] for r in only_o]
    only_m = [r[:2] + (cnt(r),) + r[3:] for r in only_m]
    rows.append((best, len(pairs), k, v['name'], v['status'], pairs, only_o, only_m, loose))

rows.sort(key=lambda r: (r[0], r[1], r[2]))
for best, n, k, name, st, pairs, only_o, only_m, loose in rows:
    print('%s %s [%s] score %d' % (k, name, 'WIP' if st == '0x2' else 'STUB' if st == '0x0' else st, best))
    for s, x, y in sorted(pairs, key=lambda p: p[0]):
        print('    insn %3d/%3d: og %-34s ours %s' % (x[0], y[0], x[2], y[2]))
    for x, y in loose:
        print('    possible swap (%s): og %s -> ours %s' % (x[3] or 'unknown og data', x[2], y[2]))
    if only_o or only_m: print('    used a different number of times (og/ours):')
    if only_o: print('      more in og  :', ', '.join(sorted({r[2] for r in only_o})))
    if only_m: print('      more in ours:', ', '.join(sorted({r[2] for r in only_m})))
print(len(rows), 'functions')
