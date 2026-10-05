"""
Prints the 9.6f version of a 10.5 function and the 9.6f bodies of the callees that 10.5
inlined into it (a guide to the original source shape and its inline helpers).

    python show_96f.py <10.5 addr> [--no-callees]

Needs match_96f.json (match_96f.py) and target_96f.json (claude/target-asm branch).
9.6f is a reference only: VC7, /Ob0, its code never has to match.
"""
import csv
import json
import os
import re
import sys

here = os.path.dirname(os.path.abspath(__file__))
m = json.load(open(os.path.join(here, "match_96f.json")))
t96 = json.load(open(os.path.join(here, "target_96f.json")))
names = {}
with open(os.path.join(here, "og_function_data_v96f.csv")) as f:
    for row in csv.reader(f):
        if len(row) > 1:
            names[row[1].lower()] = row[0]

a105 = hex(int(sys.argv[1], 16))
a96 = m["pairs"].get(a105)
if not a96:
    sys.exit(f"{a105}: no 9.6f partner in match_96f.json")
if a96 not in t96:
    sys.exit(f"{a105} -> 9.6f {a96}: not in target_96f.json")


def show(addr):
    r = t96[addr]
    print(f"== 9.6f {addr} {names.get(addr, r['name'])} size {r['size']}")
    asm = r["asm"]
    for a in sorted(set(re.findall(r"call\s+0x0*([0-9a-fA-F]+)\b", asm))):
        asm = re.sub(rf"call\s+0x0*{a}\b", f"call 0x{a.lower()}<{names.get(hex(int(a, 16)), '?')}>", asm)
    print(asm)


show(a96)
if "--no-callees" in sys.argv:
    sys.exit()
inl = {k for k, users in m["inlined"].items() if a105 in users}
calls = re.findall(r"call\s+0x0*([0-9a-fA-F]+)\b", t96[a96]["asm"])
seen = set()
for c in calls:
    c = hex(int(c, 16))
    if (c in inl or c in t96) and c not in seen:
        seen.add(c)
        print()
        print("-- inlined in 10.5:" if c in inl else "-- callee (10.5 may or may not inline it):")
        if c in t96:
            show(c)
        else:
            print(f"== 9.6f {c} {names.get(c, '?')} (asm not dumped)")
