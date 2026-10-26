#!/usr/bin/env python3
"""Phase 0 check of the input seam: the bot walks forward (walk_test), the game records the
inputs into test/replay.rep, then the same build plays the replay back without the bot's inputs
and must follow the same trace (player position and rng index per frame).

python3 replay_check.py [--map wil] [--walk 100]"""
import argparse
import shutil

import botlib


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--map", default="wil")
    ap.add_argument("--walk", type=int, default=100)
    ap.add_argument("--out", default="bot_out")
    ap.add_argument("--timeout", type=int, default=600)
    args = ap.parse_args()

    root = botlib.gta2_root(args.root)
    botlib.install_exe(root)
    out_run, out_run_arg = botlib.out_paths(root, f"{args.out}/run")
    out_replay, out_replay_arg = botlib.out_paths(root, f"{args.out}/replay")
    mapfile = botlib.MapFile() if botlib.MAP_FILE.exists() else None
    if mapfile:
        botlib.write_exclusions(mapfile, out_run)
        botlib.write_exclusions(mapfile, out_replay)
    rep = root / "test" / "replay.rep"
    if rep.exists():
        rep.unlink()

    print("=== 1. bot run (walk_test)")
    botlib.run_game(root, ["-bot=walk_test", f"-bot_map={args.map}", f"-bot_out={out_run_arg}", f"-bot_walk={args.walk}"], timeout=args.timeout, log=out_run / "wine_stdout.txt")
    print(botlib.tail(out_run / "bot_log.txt", 12))
    if not rep.exists():
        print("FAIL: the game wrote no test/replay.rep")
        return 1
    saved = out_run / "bot_run.rep"
    shutil.copy2(rep, saved)
    trace_run = botlib.read_trace(out_run)
    print(f"replay {saved.stat().st_size} bytes, trace {len(trace_run)} frames")

    print("=== 2. playback of the recorded replay")
    shutil.copy2(saved, rep)
    botlib.run_game(root, ["-bot=replay", f"-bot_map={args.map}", f"-bot_out={out_replay_arg}", f"-bot_budget={len(trace_run) + 200}"], timeout=args.timeout, log=out_replay / "wine_stdout.txt")
    print(botlib.tail(out_replay / "bot_log.txt", 12))
    trace_replay = botlib.read_trace(out_replay)
    print(f"trace {len(trace_replay)} frames")

    n = min(len(trace_run), len(trace_replay))
    if n == 0:
        print("FAIL: empty trace")
        return 1
    for i in range(n):
        a, b = trace_run[i], trace_replay[i]
        if a[1:6] != b[1:6]:
            print(f"FAIL: traces diverge at frame {i}: run {a} replay {b}")
            return 1
    moved = trace_run[0][2:4] != trace_run[-1][2:4]
    print(f"PASS: {n} frames identical, player moved={moved} from {trace_run[0][2:4]} to {trace_run[-1][2:4]}")
    return 0 if moved else 1


if __name__ == "__main__":
    raise SystemExit(main())
