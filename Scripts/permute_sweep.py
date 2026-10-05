"""
Runs Scripts/permute.sh over many WIP functions, several at a time, and summarises the results.

    python3 Scripts/permute_sweep.py --minutes 20 --parallel 3 --out /some/dir [--max-base 60]
                                     [--list funcs.txt] [-- <extra cpp_permuter options>]

Without --list it takes every WIP_FUNC in Source/*.cpp ("Source/File.cpp Class::Func addr" per line
in a list). Each function gets --minutes of random search (resumed on the next sweep: the runs keep
their checkpoints in <out>/<addr>/permuter_out) and stops early on a match. --max-base skips functions
whose unmodified score is above it (scored once and cached in <out>/base_scores.json), so the time goes
to the near misses. Prints, per function, the base and best scores and where the best candidate is;
apply one with Scripts/permuter_apply.py after reading its diff.txt.

Like permute.sh, this needs WIP_IMPLEMENTED / NOT_IMPLEMENTED to add no code (empty them in
Source/Function.hpp locally while sweeping, and don't commit that).
"""

import argparse
import concurrent.futures
import json
import os
import re
import signal
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PERMUTE = os.path.join(ROOT, "Scripts", "permute.sh")


def wip_functions():
    funcs = []
    src = os.path.join(ROOT, "Source")
    for name in sorted(os.listdir(src)):
        if not name.endswith(".cpp"):
            continue
        lines = open(os.path.join(src, name), errors="replace").read().split("\n")
        for i, l in enumerate(lines):
            m = re.match(r"\s*WIP_FUNC\(0x([0-9A-Fa-f]+)\)", l)
            if not m:
                continue
            for l2 in lines[i + 1 : i + 4]:
                d = re.search(r"([A-Za-z_]\w*(?:::~?\w+)*)\s*\(", l2)
                if d and not l2.strip().startswith("//"):
                    funcs.append(("Source/" + name, d.group(1), m.group(1).lower()))
                    break
    return funcs


def best_output(outdir):
    best = None
    if os.path.isdir(outdir):
        for d in os.listdir(outdir):
            m = re.match(r"output-(\d+)-(\d+)$", d)
            if m and (best is None or int(m.group(1)) < best[0]):
                best = (int(m.group(1)), os.path.join(outdir, d))
    return best


def base_score(src, func, addr, workdir):
    os.makedirs(workdir, exist_ok=True)
    r = subprocess.run([PERMUTE, os.path.join(ROOT, src), func, addr, "--base-only"], cwd=workdir,
                       capture_output=True, text=True)
    nums = re.findall(r"base score:? (\d+)", r.stdout + r.stderr)
    return int(nums[-1]) if nums else None


def run_one(src, func, addr, workdir, minutes, extra):
    os.makedirs(workdir, exist_ok=True)
    args = [PERMUTE, os.path.join(ROOT, src), func, addr, "-j", "1", "-n", "1000000"] + extra
    if os.path.exists(os.path.join(workdir, "permuter_out", "checkpoint.txt")):
        args.append("--resume")
    with open(os.path.join(workdir, "run.log"), "a") as log:
        p = subprocess.Popen(args, cwd=workdir, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
        try:
            p.wait(timeout=minutes * 60)
        except subprocess.TimeoutExpired:
            os.killpg(p.pid, signal.SIGINT)  # cpp_permuter writes its checkpoint on Ctrl-C
            try:
                p.wait(timeout=60)
            except subprocess.TimeoutExpired:
                os.killpg(p.pid, signal.SIGKILL)
    return best_output(os.path.join(workdir, "permuter_out"))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", required=True)
    ap.add_argument("--list")
    ap.add_argument("--minutes", type=float, default=20)
    ap.add_argument("--parallel", type=int, default=2)
    ap.add_argument("--max-base", type=int)
    ap.add_argument("extra", nargs="*", help="cpp_permuter options, after --")
    a = ap.parse_args()

    if a.list:
        funcs = [tuple(l.split()[:3]) for l in open(a.list) if l.strip() and not l.startswith("#")]
        funcs = [(os.path.relpath(s, ROOT) if os.path.isabs(s) else s, f, ad.lower()) for s, f, ad in funcs]
    else:
        funcs = wip_functions()
    os.makedirs(a.out, exist_ok=True)

    cache_path = os.path.join(a.out, "base_scores.json")
    bases = json.load(open(cache_path)) if os.path.exists(cache_path) else {}
    if a.max_base is not None:
        todo = [f for f in funcs if f[2] not in bases]
        with concurrent.futures.ThreadPoolExecutor(a.parallel) as ex:
            for f, s in zip(todo, ex.map(lambda f: base_score(*f, os.path.join(a.out, f[2])), todo)):
                bases[f[2]] = s
                json.dump(bases, open(cache_path, "w"), indent=1)
        funcs = [f for f in funcs if bases.get(f[2]) is not None and 0 < bases[f[2]] <= a.max_base]
        funcs.sort(key=lambda f: bases[f[2]])
    print("%d functions, %g min each, %d at a time" % (len(funcs), a.minutes, a.parallel), flush=True)

    def job(f):
        t = time.time()
        best = run_one(*f, os.path.join(a.out, f[2]), a.minutes, a.extra)
        line = "%-60s base %-5s best %-5s %4.0fs %s" % (f[1], bases.get(f[2], "?"), best[0] if best else "-",
                                                        time.time() - t, best[1] if best else "")
        print(line + ("  MATCH" if best and best[0] == 0 else ""), flush=True)
        return f, best

    with concurrent.futures.ThreadPoolExecutor(a.parallel) as ex:
        results = list(ex.map(job, funcs))
    print("\nmatches:")
    for f, best in results:
        if best and best[0] == 0:
            print("  %s %s %s" % (f[0], f[1], best[1]))


if __name__ == "__main__":
    sys.exit(main())
