#!/usr/bin/env python3
"""Runs a part of a bot run again from a WTF! save (the button of the bot's overlay).

python3 wtf_replay.py bot_out/watch/wtf_1 [--show] [--budget N]

The WTF directory holds state.snap (the game's state some seconds before the button was pressed) and notes.txt (the goal
chain, the state and the decisions at the button, and the command line of the run). This loads the state into a new game
started with the same bot arguments and lets the bot play from there; the log goes to <wtf dir>/replay/bot_log.txt."""
import argparse
import os
import re
import sys
from pathlib import Path

import botlib


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("wtf_dir", help="a wtf_N directory, relative to the GTA2 dir or absolute")
    ap.add_argument("--root")
    ap.add_argument("--show", action="store_true", help="draw on your screen (fast), with the overlay")
    ap.add_argument("--budget", type=int, default=20000, help="frames to play after the load")
    ap.add_argument("--timeout", type=int, default=1800)
    args = ap.parse_args()

    root = botlib.gta2_root(args.root)
    wtf = Path(args.wtf_dir)
    if not wtf.is_absolute():
        wtf = root / wtf
    snap = wtf / "state.snap"
    notes = (wtf / "notes.txt").read_text(errors="ignore")
    m = re.search(r"^command line: (.*)$", notes, re.M)
    if not snap.exists() or not m:
        sys.exit(f"{wtf}: no state.snap / notes.txt with a command line")
    keep = []
    for tok in m.group(1).split():
        if tok.startswith("-bot") and not re.match(r"-bot_(pad|out|load|budget|draw|fast|save_at|wtf_at)\b", tok):
            keep.append(tok)
    out_dir, out_arg = botlib.out_paths(root, str((wtf / "replay").relative_to(root)))
    botlib.write_exclusions(botlib.MapFile(), out_dir)
    rel_snap = str(snap.relative_to(root)).replace("/", "\\")
    bot_args = keep + [f"-bot_out={out_arg}", f"-bot_load={rel_snap}", f"-bot_budget={args.budget}"]
    if args.show and botlib.screen_unlocked():
        os.environ["BOT_USE_DISPLAY"] = "1"
        bot_args += ["-bot_draw", "-bot_fast"]
    print("replaying from", snap)
    botlib.run_game(root, bot_args, timeout=args.timeout, log=out_dir / "wine_stdout.txt")
    print("log:", out_dir / "bot_log.txt")


if __name__ == "__main__":
    main()
