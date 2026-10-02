"""
Regenerate the tables in docs/inlines_96f.md from match_96f.json (match_96f.py).

For each marked 10.5 function paired with its 9.6f version, the 9.6f callees that the 10.5
code no longer calls are inlines (or calls the pairing missed). The Status and Notes columns of
the existing doc are kept, keyed by address, so progress survives a regeneration.
Also fills the missing v96f addresses in Scripts/ida/functions_data.json with --json, and
writes dump_96f_addrs.txt (the 9.6f functions the workflow dumps the asm of) with --addrs.
"""
import glob
import json
import re
import sys
from collections import defaultdict

DOC = "../../docs/inlines_96f.md"
LIB96 = (0x4D3550, 0x4F7000)  # 9.6f Bink/Miles thunks and the CRT
LIB105 = 0x5ED000

fp = json.load(open("fingerprints.json"))
A, B = fp["v105"], fp["v96f"]
m = json.load(open("match_96f.json"))
pairs = m["pairs"]
rpairs = {b: a for a, b in pairs.items()}


def is_lib96(b):
    return LIB96[0] <= int(b, 16) < LIB96[1] or (b in rpairs and int(rpairs[b], 16) >= LIB105)


# markers in Source/
status, where = {}, {}
for f in sorted(glob.glob("../../Source/*.cpp")):
    for i, line in enumerate(open(f, encoding="utf-8", errors="ignore"), 1):
        mm = re.match(r"^\s*(MATCH|WIP|STUB)_FUNC\((0x[0-9A-Fa-f]+)\)", line)
        if mm:
            a = hex(int(mm.group(2), 16))
            status[a] = mm.group(1)
            where[a] = f"{f[6:]}:{i}"

# source text of each marked function, from its marker to the next one
body = {}
for f in sorted(glob.glob("../../Source/*.cpp")):
    text = open(f, encoding="utf-8", errors="ignore").read()
    ms = list(re.finditer(r"^\s*(?:MATCH|WIP|STUB)_FUNC\((0x[0-9A-Fa-f]+)\)", text, re.M))
    for k, mm in enumerate(ms):
        end = ms[k + 1].start() if k + 1 < len(ms) else len(text)
        body[hex(int(mm.group(1), 16))] = text[mm.end():end]

# basic Fix16/ang16/point operators: used through operators, so not tracked per function
CORE = re.compile(r"^(Fix16|ang16|Ang16|Fix16_Point|Fix16_2|Fix16_rect)::")


# helpers in the headers marked with the 9.6f function they come from ("// 9.6f 0x403A00" on the
# line of the definition or the line above it): their names count as uses of that 9.6f function
alias = defaultdict(set)
for f in sorted(glob.glob("../../Source/*.hpp")):
    lines = open(f, encoding="utf-8", errors="ignore").read().split("\n")
    for i, line in enumerate(lines):
        if "9.6f" not in line and "96f" not in line:
            continue
        for x in re.findall(r"0x([0-9A-Fa-f]{6})\b", line):
            code = line.split("//")[0]
            if not re.search(r"\w+\s*\(", code) and i + 1 < len(lines):
                code = lines[i + 1].split("//")[0]
            mm = re.search(r"(\w+)\s*\(", code)
            if mm and mm.group(1) not in ("if", "while", "for", "return", "switch"):
                alias[hex(int(x, 16))].add(mm.group(1))


def used_in(a, c):
    """Does the source of 10.5 function a already use the inline for 9.6f function c?"""
    t = body.get(a, "")
    pats = [c[2:]]
    if c in rpairs:
        pats.append(rpairs[c][2:])
    if any(re.search(r"_" + x + r"(?![0-9A-Fa-f])", t, re.I) for x in pats):
        return True
    return any(re.search(r"\b" + n + r"\s*\(", t) for n in alias.get(c, ()))


# 9.6f addresses already noted in Source/ comments ("9.6f 0x401C10", "inlined v9.6f, 0x432850", ...)
noted = defaultdict(list)
noted96_ident = defaultdict(list)  # inline methods named after an address, maybe the 9.6f one
for f in sorted(glob.glob("../../Source/*.[ch]pp")):
    for i, line in enumerate(open(f, encoding="utf-8", errors="ignore"), 1):
        if "9.6f" in line or "96f" in line:
            for a in re.findall(r"0x([0-9A-Fa-f]{6})\b", line):
                noted[hex(int(a, 16))].append(f"{f[6:]}:{i}")
        if f.endswith(".hpp"):
            for a in re.findall(r"\b\w+_([0-9A-Fa-f]{6})(?:_\w+)?\s*\(", line):
                if "FUNC" not in line:
                    noted96_ident[hex(int(a, 16))].append(f"{f[6:]}:{i}")

# per 10.5 function: the 9.6f callees it inlined
inl = {}
users = defaultdict(list)
for a, cs in m["per_func"].items():
    if int(a, 16) >= LIB105:
        continue
    cs = [c for c in cs if not is_lib96(c) and not CORE.match(B[c]["name"])]
    if cs:
        inl[a] = cs
        for c in cs:
            users[c].append(a)


def addr105(a):
    return f"0x{int(a, 16):X}"


def callee_str(a, c):
    s = f"`{B[c]['name']}`"
    if used_in(a, c):
        s = "✓ " + s
    if c in rpairs:
        s += f" (10.5 {addr105(rpairs[c])})"
    return s


# keep Status/Notes columns of the existing doc
kept = {}
try:
    for line in open(DOC, encoding="utf-8"):
        cells = [x.strip() for x in line.strip().strip("|").split("|")]
        if line.startswith("| 0x") and len(cells) >= 3:
            kept[(cells[0], len(cells))] = cells[-2:]
except FileNotFoundError:
    pass


def row(cells, ncols):
    old = kept.get((cells[0], ncols))
    if old:
        cells = cells[:-2] + old
    return "| " + " | ".join(cells) + " |"


out = []
for st in ("WIP", "STUB", "MATCH"):
    rows = sorted((a for a in inl if status.get(a) == st), key=lambda a: int(a, 16))
    out.append(f"<!-- table {st} -->")
    out.append(f"| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |")
    out.append("|---|---|---|---|---|---|")
    for a in rows:
        b = pairs[a]
        out.append(row([addr105(a), f"`{A[a]['name']}`", addr105(b),
                        ", ".join(callee_str(a, c) for c in inl[a]), "todo", ""], 6))
    out.append("")

# the inlines themselves
out.append("<!-- table inlines -->")
out.append("| 9.6f | 9.6f name | Size | 10.5 copy | Noted in Source | WIP/MATCH users | Status | Notes |")
out.append("|---|---|---|---|---|---|---|---|")
for c in sorted(users, key=lambda c: (-sum(status.get(u) == "WIP" for u in users[c]), int(c, 16))):
    us = users[c]
    nw = sum(status.get(u) == "WIP" for u in us)
    nm = sum(status.get(u) == "MATCH" for u in us)
    copy = ""
    if c in rpairs:
        r = rpairs[c]
        copy = f"{addr105(r)} {status.get(r, 'unmarked')}"
    out.append(row([addr105(c), f"`{B[c]['name']}`", str(B[c]["size"]), copy,
                    ", ".join((noted.get(c, []) or [x for x in noted96_ident.get(c, []) if c not in rpairs])[:2]), f"{nw}/{nm}", "todo", ""], 8))
out.append("")

unpaired = sorted((a for a, s in status.items() if s == "WIP" and a not in pairs), key=lambda a: int(a, 16))
out.append("<!-- table unpaired -->")
out.append(", ".join(f"`{addr105(a)}`" for a in unpaired))
out.append("")

doc = open(DOC, encoding="utf-8").read()
head = doc.split("<!-- generated -->")[0]
stats = (f"Paired: {len(pairs)}/{len(A)} 10.5 functions. Marked functions with inlined callees: "
         + ", ".join(f"{st} {sum(status.get(a) == st for a in inl)}" for st in ("WIP", "STUB", "MATCH"))
         + f". Distinct inlined 9.6f functions: {len(users)}. WIPs without a 9.6f partner: {len(unpaired)}.")
open(DOC, "w", encoding="utf-8").write(head + "<!-- generated -->\n\n" + stats + "\n\n" + "\n".join(out))
print(stats)

if "--json" in sys.argv:
    path = "../ida/functions_data.json"
    data = json.load(open(path))
    n = 0
    have = {hex(int(e["v105_address"], 16)) for e in data if e["v105_address"]}
    used = {hex(int(e["v96f_address"], 16)) for e in data if e["v96f_address"] and not e.get("v96f_auto")}
    for e in data:
        if e.get("v96f_auto"):
            e["v96f_address"] = None
            del e["v96f_auto"]
        if e["v96f_address"] or not e["v105_address"]:
            continue
        a = hex(int(e["v105_address"], 16))
        b = pairs.get(a)
        if b and int(a, 16) < LIB105 and b not in used:
            e["v96f_address"] = b
            e["v96f_auto"] = True  # from match_96f.py, not checked by hand
            used.add(b)
            n += 1
    with open(path, "w") as f:
        f.write(json.dumps(data, indent=4) + "\n")
    print(f"functions_data.json: filled {n} v96f addresses")

if "--addrs" in sys.argv:
    # the list the workflow dumps the 9.6f asm for (copy it to the claude/target-asm-request branch)
    want = {pairs[a] for a, st in status.items() if st in ("WIP", "STUB", "MATCH") and a in pairs}
    for a, cs in m["per_func"].items():
        if int(a, 16) < LIB105:
            want.update(c for c in cs if not is_lib96(c))
    with open("dump_96f_addrs.txt", "w") as f:
        f.write("# 9.6f functions to dump: the 9.6f versions of marked functions and the 9.6f callees\n"
                "# inlined in 10.5 (gen_inline_tracking.py --addrs)\n")
        f.write("\n".join(sorted(want, key=lambda x: int(x, 16))) + "\n")
    print(f"dump_96f_addrs.txt: {len(want)} functions")
