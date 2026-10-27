#!/usr/bin/env python3
"""Regression driver for the bot (docs/BOT_DESIGN.md, phase 6).

Runs every entry of bot_runs/runs.json: the bot plays, the report must say what the entry expects,
and for entries with "replay": true the recorded test/replay.rep is played back by the same build
(without the bot's inputs) and must follow the same per-frame trace (rng index and player position).
The committed timeline of a run that used snapshot retries is its last monotone suffix by rng index.

python3 regress.py [--only name ...] [--no-replay]
Exit code 0 when every entry did what it expects; expected failures are tracked challenges, not errors."""
import argparse
import json
import shutil
import sys
from pathlib import Path

import botlib

HERE = Path(__file__).resolve().parent


def committed(trace):
    out = []
    for row in trace:
        while out and out[-1][1] >= row[1]:
            out.pop()
        out.append(row)
    return out


def run_entry(root, entry, do_replay, seed=1):
    name = entry["name"]
    out, out_arg = botlib.out_paths(root, f"bot_out/regress/{name}")
    botlib.write_exclusions(botlib.MapFile(), out)
    for f in ("bot_report.txt", "bot_log.txt", "bot_trace.txt", "bot_run.rep"):
        if (out / f).exists():
            (out / f).unlink()
    rep = root / "test" / "replay.rep"
    if rep.exists():
        rep.unlink()
    args = list(entry["args"]) + [f"-bot_out={out_arg}", f"-bot_budget={entry['budget']}", f"-bot_seed={seed}"]
    botlib.run_game(root, args, timeout=entry.get("timeout", 1500), log=out / "wine_stdout.txt")
    report = botlib.read_report(out)
    result = report.get("result", "NONE")
    condition = report.get("condition", "")
    ok = result == entry["expect"] and (not entry.get("condition") or condition == entry["condition"])
    detail = report.get("detail", "")
    if rep.exists():
        shutil.copy2(rep, out / "bot_run.rep")
    line = f"{name:22s} seed={seed} result={result} expect={entry['expect']}"
    if condition:
        line += f" condition={condition}"
    status = "ok" if ok else "MISMATCH"

    replay_note = ""
    if ok and do_replay and entry.get("replay") and result == "PASS":
        trace = committed(botlib.read_trace(out))
        map_name = [a.split('=', 1)[1] for a in entry['args'] if a.startswith('-bot_map=')][0]
        rout, rout_arg = botlib.out_paths(root, f"bot_out/regress/{name}_replay")
        shutil.copy2(out / "bot_run.rep", root / "test" / "replay.rep")
        botlib.run_game(root, ["-bot=replay", f"-bot_map={map_name}", f"-bot_out={rout_arg}", f"-bot_budget={len(trace) + 300}"], timeout=600, log=rout / "wine_stdout.txt")
        rtrace = botlib.read_trace(rout)
        n = min(len(trace), len(rtrace))
        bad = [i for i in range(n) if trace[i][1:6] != rtrace[i][1:6]]
        if n == 0 or bad:
            ok = False
            status = "REPLAY MISMATCH"
            replay_note = f" replay: first difference at frame {bad[0] if bad else 0} of {n}"
        else:
            replay_note = f" replay: {n} frames identical"
    print(f"{status:16s} {line} | {detail[:90]}{replay_note}", flush=True)
    if entry.get("note"):
        print(f"{'':16s} note: {entry['note']}")
    return ok


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--only", nargs="*")
    ap.add_argument("--no-replay", action="store_true")
    args = ap.parse_args()
    root = botlib.gta2_root(args.root)
    botlib.install_exe(root)
    runs = json.loads((HERE / "bot_runs" / "runs.json").read_text())["runs"]
    if args.only:
        runs = [r for r in runs if r["name"] in args.only]
    results = []
    for r in runs:
        ok = False
        for seed in r.get("seeds", [1]):
            ok = run_entry(root, r, not args.no_replay, seed)
            if ok:
                break
        results.append(ok)
    print(f"{sum(results)}/{len(results)} entries behaved as expected")
    return 0 if all(results) else 1


if __name__ == "__main__":
    sys.exit(main())
