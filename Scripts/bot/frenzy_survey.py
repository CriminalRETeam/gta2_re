#!/usr/bin/env python3
"""Tries every kill frenzy icon of a map one by one and prints its rule and whether the bot passes.
python3 frenzy_survey.py [--map wil] [--ids 408 409 ...]"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

import botlib


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--map", default="wil")
    ap.add_argument("--ids", type=int, nargs="*")
    ap.add_argument("--budget", type=int, default=14000)
    ap.add_argument("--timeout", type=int, default=240)
    ap.add_argument("--extra", nargs="*", default=[])
    args = ap.parse_args()
    root = botlib.gta2_root(args.root)
    botlib.install_exe(root)
    ids = args.ids or list(range(408, 424))
    for fid in ids:
        out, out_arg = botlib.out_paths(root, f"bot_out/frenzy_{fid}")
        botlib.write_exclusions(botlib.MapFile(), out)
        for f in ("bot_log.txt", "bot_report.txt"):
            if (out / f).exists():
                (out / f).unlink()
        botlib.run_game(root, ["-bot=collect", f"-bot_map={args.map}", f"-bot_out={out_arg}", "-bot_tokens=0", "-bot_frenzies=1", f"-bot_frenzy_id={fid}", f"-bot_budget={args.budget}"] + args.extra, timeout=args.timeout, log=out / "wine_stdout.txt")
        log = (out / "bot_log.txt").read_text(errors="replace") if (out / "bot_log.txt").exists() else ""
        rule = re.findall(r"kill_frenzy: started after \d+ frames; rule: (.*)", log)
        over = re.findall(r"kill_frenzy: over after \d+ frames: (\w+)", log)
        rep = botlib.read_report(out)
        print(f"frenzy {fid}: rule {rule[0] if rule else 'never started'} | outcomes {over} | result {rep.get('result')} {rep.get('detail', '')[:100]}", flush=True)


if __name__ == "__main__":
    sys.exit(main())
