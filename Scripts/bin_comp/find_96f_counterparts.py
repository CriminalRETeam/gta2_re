"""
Finds probable 9.6f counterparts of WIP/STUB functions that match_96f.py left unpaired.

For each such function, its callees in the original 10.5 asm (target_asm.json) are mapped to 9.6f
through the existing pairs, and compared (as a multiset) with the calls of every unpaired 9.6f
function. Prints the best candidates: (shared / mapped callees, shared, 9.6f addr, name, size).
A high share with several mapped callees is usually the real counterpart (it found
miss2_0x11C::GetSpeed_50E190 -> 0x47D070 and Ped::FindBestTargetPed_466BF0 -> 0x437BE0).
Check the candidate's asm, then register it with add_96f_target.py.

    python find_96f_counterparts.py [addr ...]     # default: every WIP/STUB in target_asm.json
"""
import bisect
import csv
import json
import re
import subprocess
import sys
from collections import Counter

pairs = {int(k, 16): int(v, 16) for k, v in json.load(open("match_96f.json"))["pairs"].items()}
paired96 = set(pairs.values())
targets = json.load(open("target_asm.json"))
f96 = sorted((int(r[1], 16), int(r[3], 16), r[0]) for r in csv.reader(open("og_function_data_v96f.csv")) if r[1].startswith("0x"))
starts = [f[0] for f in f96]

calls96 = {}
out = subprocess.run(["objdump", "-d", "--no-show-raw-insn", "9.6f.exe"], capture_output=True, text=True).stdout
for line in out.split("\n"):
    m = re.match(r"\s+([0-9a-f]+):\s+call\s+0x([0-9a-f]+)", line)
    if not m:
        continue
    at = int(m.group(1), 16)
    i = bisect.bisect_right(starts, at) - 1
    if i >= 0 and at < f96[i][0] + f96[i][1]:
        calls96.setdefault(f96[i][0], []).append(int(m.group(2), 16))

addrs = [int(a, 16) for a in sys.argv[1:]] or [int(a, 16) for a in targets]
for a in sorted(addrs):
    t = targets.get(hex(a))
    if not t or a in pairs:
        continue
    c105 = [(a + int(m, 16)) & 0xFFFFFFFF for m in re.findall(r"call[l]? 0x([0-9A-Fa-f]+)", t["asm"])]
    mapped = Counter(pairs[c] for c in c105 if c in pairs)
    n = sum(mapped.values())
    if n < 2:
        continue
    best = []
    for s, size, name in f96:
        if s in paired96:
            continue
        shared = sum((mapped & Counter(calls96.get(s, []))).values())
        if shared >= 2:
            best.append((round(shared / n, 2), shared, hex(s), name, size))
    best.sort(reverse=True)
    if best:
        print(hex(a), t["name"], "mapped", n, best[:3])
