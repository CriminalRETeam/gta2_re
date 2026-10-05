"""
Like compare_callees.py, but compares the call targets of each WIP as multisets and lists only
missing or extra calls to real functions (Fix16 operator and inline copies are left out). A call
the original makes that ours lacks is often missing code: it found the missing reload block in
Weapon_30::throwable_5DDFC0. Tail merging can also add or remove a call, so check the asm.

Usage (from Scripts/bin_comp, after a build and msvc_dump_new_data.py):
python3 compare_callees_multiset.py
"""
import collections, json, re, sys, difflib
from iced_x86 import Decoder, FlowControl, OpKind
B = ''
t = json.load(open(B + 'target_asm.json'))
nd = json.load(open(B + 'new_data.json'))
names = {}
for l in open(B + 'og_function_data_v105.csv'):
    r = l.strip().split(',')
    if len(r) >= 4: names[int(r[1], 16)] = r[0]
va2og = {}
mine = {}
for f in nd['functions']:
    try: og = int(f['og_addr'], 16)
    except: continue
    va2og[int(f['func_va'], 16)] = og
    mine[og] = f
mapname = {}
for l in open('../../build_vc6/output.map', errors='ignore'):
    m = re.match(r'\s*0001:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8}) f( i)?', l)
    if m: mapname[int(m.group(2), 16)] = m.group(1) + (' [inline copy]' if m.group(3) else '')
starts = sorted(mapname)
import bisect
def our_size(va):
    i = bisect.bisect_right(starts, va)
    return (starts[i] - va) if i < len(starts) else 0x1000
exe = open('../../build_vc6/decomp_main.exe', 'rb').read()
def calls_of(code, ip):
    out = []
    for ins in Decoder(32, code, ip=ip):
        if ins.flow_control == FlowControl.CALL and ins.op0_kind == OpKind.NEAR_BRANCH32:
            tg = ins.near_branch_target
            if tg == ins.next_ip or mapname.get(tg, '').startswith('?LogFuncAddr'):
                continue
            out.append(tg)
    return out
rows = []
for k, v in t.items():
    if v['status'] != '0x2': continue
    a = int(k, 16)
    f = mine.get(a)
    if not f: continue
    # target: raw asm has relative call targets
    tc = []
    for line in v['asm'].split('\n'):
        m = re.match(r'call 0x([0-9A-F]+)$', line)
        if m: tc.append((a + int(m.group(1), 16)) & 0xffffffff)
    fo = int(f['func_fo'], 16); va = int(f['func_va'], 16)
    oc = [va2og.get(x, -x) for x in calls_of(exe[fo:fo + our_size(va)], va)]
    norm = lambda n: n.lstrip('_')
    tn = sorted(norm(names.get(x, hex(x))) for x in tc)
    on = sorted(norm(names.get(x, hex(x))) if x >= 0 else norm(mapname.get(-x, 'new')) for x in oc)
    if tn != on:
        ta = [names.get(x, hex(x)) for x in tc]
        oa = [names.get(x, hex(x)) if x >= 0 else mapname.get(-x, 'new:' + hex(-x)) for x in oc]
        sm = difflib.SequenceMatcher(None, ta, oa, autojunk=False)
        d = [(op, ta[i1:i2], oa[j1:j2]) for op, i1, i2, j1, j2 in sm.get_opcodes() if op != 'equal']
        C=collections.Counter
        bad=lambda x: 'inline copy' in x or x.startswith('_') or x.startswith('?') or 'new' in x
        ct=C(x for x in ta if not bad(x)); co=C(x for x in oa if not bad(x))
        miss=ct-co; extra=co-ct
        if miss or extra: print(k, v['name'], '\n    missing', dict(miss), '\n    extra  ', dict(extra))
import collections
for n,k,name,d in rows: pass

for n,k,name,d in sorted(rows):
    pass
