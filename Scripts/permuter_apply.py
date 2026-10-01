"""
Copies the best cpp_permuter result for a function back into the source file.

    python Scripts/permuter_apply.py <permuter_out dir> <Source/File.cpp> <Class::Function>

Takes function.cpp from the lowest scoring output-<score>-<n> directory and splices the target
function (the first definition in function.cpp) over its definition in the source file, using
cpp_permuter --list-definitions for the byte ranges. Only that function changes, so results for
several functions of one file can be applied one after another. Read diff.txt first: the
passes don't prove the rewrite keeps the behaviour (remove_stmt in particular).
"""

import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PERMUTER = os.path.join(ROOT, "build_permuter", "cpp_permuter")


def definitions(path):
    out = subprocess.run([PERMUTER, "--list-definitions", path], capture_output=True, text=True, check=True).stdout
    defs = []
    for line in out.splitlines():
        m = re.match(r"(\d+) (\d+) (.*)", line)
        if m:
            defs.append((int(m.group(1)), int(m.group(2)), m.group(3)))
    return defs


def main():
    outdir, src, func = sys.argv[1:4]
    outs = []
    for d in os.listdir(outdir):
        m = re.match(r"output-(\d+)-(\d+)$", d)
        if m:
            outs.append((int(m.group(1)), int(m.group(2)), d))
    if not outs:
        raise SystemExit("no outputs in " + outdir)
    score, _, best = min(outs)
    fpath = os.path.join(outdir, best, "function.cpp")
    new_defs = [d for d in definitions(fpath) if d[2] == func]
    if not new_defs:
        raise SystemExit(f"{func} not in {fpath}")
    new_text = open(fpath, encoding="latin-1", newline="").read()
    nb, ne, _ = new_defs[0]
    body = new_text[nb:ne]

    old = open(src, encoding="latin-1", newline="").read()
    old_defs = [d for d in definitions(src) if d[2] == func]
    if len(old_defs) != 1:
        raise SystemExit(f"{func}: {len(old_defs)} definitions in {src}")
    ob, oe, _ = old_defs[0]
    if "\r\n" in old and "\r\n" not in body:
        body = body.replace("\n", "\r\n")
    open(src, "w", encoding="latin-1", newline="").write(old[:ob] + body + old[oe:])
    print(f"applied {best} (score {score}) to {func} in {src}")


if __name__ == "__main__":
    main()
