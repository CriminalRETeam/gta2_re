#!/usr/bin/env python3
"""Runs the bot once: python3 run_bot.py --profile walk_test --map wil [-- extra -bot_* args]

Copies the current build into the GTA2 dir, runs it under wine, then copies the game's replay
(test/replay.rep) next to the bot's logs as bot_run.rep and prints the end of bot_log.txt."""
import argparse
import os
import subprocess
import threading
import time
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
            args.extra = list(args.extra) + ["-bot_fast", "-bot_pause_route"]
            print(f"showing the run on {os.environ['DISPLAY']}")
        else:
            print("screen locked or no display: running off screen instead")
    bot_args = [f"-bot={args.profile}", f"-bot_map={args.map}", f"-bot_out={out_arg}"]
    if args.budget:
        bot_args.append(f"-bot_budget={args.budget}")
    if args.draw:
        bot_args.append("-bot_draw")
    bot_args += args.extra

    shown = bool(args.show and os.environ.get("BOT_USE_DISPLAY"))
    hung = {"v": False}
    if shown:
        # A screen that locks after the start leaves the game stuck before its first frame: watch for that
        def watchdog():
            log = out_dir / "bot_log.txt"
            deadline = time.time() + 90
            while time.time() < deadline:
                if log.exists() and "game running" in log.read_text(errors="ignore"):
                    return
                time.sleep(3)
            hung["v"] = True
            print("no first frame after 90 s on your screen (locked?): stopping, running off screen instead")
            subprocess.run("pkill -f 'decomp_main.exe'", shell=True)

        threading.Thread(target=watchdog, daemon=True).start()
    code = botlib.run_game(root, bot_args, timeout=args.timeout, desktop="gta2bot,800x600" if args.desktop else None, log=out_dir / "wine_stdout.txt")
    if hung["v"]:
        subprocess.run("wineserver -k", shell=True)  # the killed game leaves wine's server in a bad state
        time.sleep(4)
        os.environ.pop("BOT_USE_DISPLAY", None)
        bot_args = [a for a in bot_args if a not in ("-bot_draw", "-bot_fast", "-bot_pause_route")]
        code = botlib.run_game(root, bot_args, timeout=args.timeout, desktop=None, log=out_dir / "wine_stdout.txt")

    rep = root / "test" / "replay.rep"
    if rep.exists():
        shutil.copy2(rep, out_dir / "bot_run.rep")
        print(f"replay: {rep.stat().st_size} bytes -> {out_dir / 'bot_run.rep'}")
    print("--- bot_log.txt")
    print(botlib.tail(out_dir / "bot_log.txt", 40))
    return code


if __name__ == "__main__":
    raise SystemExit(main())
