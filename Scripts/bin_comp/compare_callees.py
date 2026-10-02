"""
For each WIP function in target_asm.json, compare the original's call targets with the build's,
named through og_function_data_v105.csv and build_vc6/output.map.

The verifier normalises call targets, so a different callee alone doesn't break a match (an
inline COMDAT copy of an operator has the same code as the exported copy 10.5 calls). The list
is for finding out-of-line copies that have no marker yet (Fix16_Point::operator- at 0x40AC80),
calls that should be inlined or not, and wrong callees.

Usage (from Scripts/bin_comp, after a build and msvc_dump_new_data.py): python3 compare_callees.py
"""
import json, re, sys, difflib
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
        rows.append((len(d), k, v['name'], d))
rows.sort()
for n, k, name, d in rows:
    print(k, name)
    for op, x, y in d[:4]: print('   ', op, x, '->', y)
print(len(rows))
