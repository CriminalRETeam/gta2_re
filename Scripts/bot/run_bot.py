#!/usr/bin/env python3
"""Runs the bot once: python3 run_bot.py --profile walk_test --map wil [-- extra -bot_* args]

Copies the current build into the GTA2 dir, runs it under wine, then copies the game's replay
(test/replay.rep) next to the bot's logs as bot_run.rep and prints the end of bot_log.txt."""
import argparse
import os
import shutil
from pathlib import Path

import botlib


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", help="GTA2 dir (default $GTA2_ROOT)")
    ap.add_argument("--profile", default="idle")
    ap.add_argument("--map", default="wil")
    ap.add_argument("--out", default="bot_out", help="out dir, relative to the GTA2 dir")
    ap.add_argument("--budget", type=int, default=0)
    ap.add_argument("--timeout", type=int, default=600)
    ap.add_argument("--draw", action="store_true")
    ap.add_argument("--show", action="store_true", help="draw on your own screen at game speed so you can watch (falls back to off screen when the screen is locked)")
    ap.add_argument("--desktop", action="store_true", help="run in a wine virtual desktop (did not start the exe here)")
    ap.add_argument("extra", nargs="*", help="extra -bot_* arguments")
    args = ap.parse_args()

    root = botlib.gta2_root(args.root)
    botlib.install_exe(root)
    out_dir, out_arg = botlib.out_paths(root, args.out)
    if botlib.MAP_FILE.exists():
        botlib.write_exclusions(botlib.MapFile(), out_dir)

    if args.show:
        if botlib.screen_unlocked():
            os.environ["BOT_USE_DISPLAY"] = "1"
            args.draw = True
            args.extra = list(args.extra) + ["-bot_fast"]
            print(f"showing the run on {os.environ['DISPLAY']}")
        else:
            print("screen locked or no display: running off screen instead")
    bot_args = [f"-bot={args.profile}", f"-bot_map={args.map}", f"-bot_out={out_arg}"]
    if args.budget:
        bot_args.append(f"-bot_budget={args.budget}")
    if args.draw:
        bot_args.append("-bot_draw")
    bot_args += args.extra

    code = botlib.run_game(root, bot_args, timeout=args.timeout, desktop="gta2bot,800x600" if args.desktop else None, log=out_dir / "wine_stdout.txt")

    rep = root / "test" / "replay.rep"
    if rep.exists():
        shutil.copy2(rep, out_dir / "bot_run.rep")
        print(f"replay: {rep.stat().st_size} bytes -> {out_dir / 'bot_run.rep'}")
    print("--- bot_log.txt")
    print(botlib.tail(out_dir / "bot_log.txt", 40))
    return code


if __name__ == "__main__":
    raise SystemExit(main())
