#!/usr/bin/env python3
"""Driving benchmark: the nearest parked car drives to several targets, over a few seeds.
Prints result and frames per run and a total, to compare driver changes.
python3 drive_bench.py [--seeds 1 2] [--jobs 3]"""
import argparse
import concurrent.futures
import re
import sys

import botlib

TARGETS = ["178,193", "120,100", "100,160", "60,150", "200,60", "150,50"]


def one(root, target, seed):
    name = f"bench_{target.replace(',', '_')}_s{seed}"
    out, out_arg = botlib.out_paths(root, f"bot_out/bench/{name}")
    botlib.write_exclusions(botlib.MapFile(), out)
    for f in ("bot_report.txt", "bot_log.txt"):
        if (out / f).exists():
            (out / f).unlink()
    args = ["-bot=drive_test", "-bot_map=wil", f"-bot_goto={target}", f"-bot_out={out_arg}", "-bot_budget=9000", f"-bot_seed={seed}"]
    botlib.run_game(root, args, timeout=900, log=out / "wine_stdout.txt")
    rep = botlib.read_report(out)
    frames = int(rep.get("frame", 0) or 0)
    resets = botlib.count_resets(out)
    result = rep.get("result", "NONE")
    if result == "PASS" and resets:
        result = "RESET"  # reached it only by being rewound after getting stuck
    return target, seed, result, frames, f"{resets} resets; " + rep.get("detail", "")[:60] if resets else rep.get("detail", "")[:70]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--seeds", nargs="*", type=int, default=[1, 2])
    ap.add_argument("--jobs", type=int, default=3)
    a = ap.parse_args()
    root = botlib.gta2_root(a.root)
    botlib.install_exe(root)
    jobs = [(t, s) for t in TARGETS for s in a.seeds]
    ok = 0
    total = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as ex:
        for target, seed, res, frames, detail in ex.map(lambda j: one(root, *j), jobs):
            print(f"{target:9s} seed {seed} {res:5s} {frames:6d} frames {detail if res != 'PASS' else ''}", flush=True)
            ok += res == "PASS"
            total += frames if res == "PASS" else 9000
    print(f"{ok}/{len(jobs)} reached; frames summed (failures count 9000): {total}")


if __name__ == "__main__":
    sys.exit(main())
