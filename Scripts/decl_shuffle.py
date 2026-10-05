"""
Random search over the order of the local declarations at the top of one function.

VC6 picks stack slots (and which local reuses a dead parameter slot) partly from declaration
order. With 6+ locals there are too many orders to try by hand, and cpp_permuter's
reorder_saves rarely picks a full shuffle, so this tries random shuffles of the leading run of
declaration lines and keeps the best score. It took text_0x14::InsertLineBreaksAndGetNumLines
from 232 to 56 before the permuter found the rest.

    python3 Scripts/decl_shuffle.py Source/text_0x14.cpp InsertLineBreaksAndGetNumLines_5B5BC0 5b5bc0 [-n 300] [--seed 1]

The declaration run is the block of lines right after the function's opening brace that look
like declarations (end in ';', no '(' except in an initializer, not a statement keyword). The
source is copied to a temp dir with the rest of Source/, so the real file is never written.
WIP_IMPLEMENTED/NOT_IMPLEMENTED are stripped in the copy. Prints each improvement and writes the best
version of the function's declaration block to stdout at the end.
"""
import argparse
import os
import random
import re
import shutil
import subprocess
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DECL = re.compile(r"^\s*(const\s+)?[A-Za-z_][\w:<>]*[\s\*&]+[\*&]*\w+(\[[^\]]*\])?(\s*=\s*[^;]*)?;\s*(//.*)?$")
KEYWORDS = ("return", "if", "for", "while", "switch", "do", "else", "goto", "case", "break")

ap = argparse.ArgumentParser()
ap.add_argument("src")
ap.add_argument("func")
ap.add_argument("addr")
ap.add_argument("-n", type=int, default=300)
ap.add_argument("--seed", type=int, default=1)
args = ap.parse_args()

text = open(args.src).read()
# only the temp copy is compiled, so the logging macros can go everywhere
text = re.sub(r"\n[ \t]*(WIP_IMPLEMENTED|NOT_IMPLEMENTED);[ \t]*(?=\n)", "", text)
m = re.search(re.escape(args.func) + r"\s*\([^)]*\)[^{;]*\{\n", text)
if not m:
    raise SystemExit("function not found: " + args.func)
start = m.end()
lines = text[start:].split("\n")
decls = []
for line in lines:
    s = line.strip()
    if not s or s.startswith("//"):
        if decls:
            break
        decls.append(line)
        continue
    if s.split()[0].rstrip("(") in KEYWORDS or not DECL.match(line):
        break
    decls.append(line)
while decls and not decls[-1].strip():
    decls.pop()
end = start + len("\n".join(decls))
real = [d for d in decls if d.strip() and not d.strip().startswith("//")]
if len(real) < 2:
    raise SystemExit("fewer than 2 declarations found at the top of the function")
print("declarations:", [d.strip() for d in real])

tmp = tempfile.mkdtemp()
src_dir = os.path.join(tmp, "Source")
shutil.copytree(os.path.join(ROOT, "Source"), src_dir)
tmp_src = os.path.join(src_dir, os.path.basename(args.src))
needle = args.func.split("::")[-1]


def score(order):
    open(tmp_src, "w").write(text[:start] + "\n".join(order) + text[end:])
    out = subprocess.run([os.path.join(ROOT, "Scripts", "quick_score.sh"), tmp_src, args.addr, needle, "-q"],
                         capture_output=True, text=True).stdout.strip().split("\n")[-1]
    try:
        return int(float(out))
    except ValueError:
        return None


rng = random.Random(args.seed)
best = (score(real), real)
print("base", best[0], flush=True)
try:
    for _ in range(args.n):
        order = real[:]
        rng.shuffle(order)
        sc = score(order)
        if sc is not None and (best[0] is None or sc < best[0]):
            best = (sc, order)
            print(sc, [d.strip() for d in order], flush=True)
            if sc == 0:
                break
finally:
    shutil.rmtree(tmp, ignore_errors=True)
print("best", best[0])
print("\n".join(best[1]))
