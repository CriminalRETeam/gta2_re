#!/usr/bin/env python3
"""Phase 0 check of the Snapshot: snapshot at frame W, run k frames, restore, run k frames again,
the memory must be identical. Prints the differing bytes as symbols from output.map when not.

python3 restore_check.py [--map wil] [--warmup 100] [--frames 300]"""
import argparse
from collections import defaultdict
from pathlib import Path

import botlib


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root")
    ap.add_argument("--map", default="wil")
    ap.add_argument("--warmup", type=int, default=100)
    ap.add_argument("--frames", type=int, default=300)
    ap.add_argument("--out", default="bot_out")
    ap.add_argument("--timeout", type=int, default=600)
    ap.add_argument("--exclude", action="append", default=[], help="extra symbol to exclude (repeatable)")
    ap.add_argument("--max-print", type=int, default=60)
    args = ap.parse_args()

    root = botlib.gta2_root(args.root)
    botlib.install_exe(root)
    out_dir = root / args.out
    mapfile = botlib.MapFile()
    missing = botlib.write_exclusions(mapfile, out_dir, [(n, "command line") for n in args.exclude])
    if missing:
        print("not in output.map (not excluded):", ", ".join(missing))

    for f in ("bot_report.txt", "bot_diff.txt"):
        p = out_dir / f
        if p.exists():
            p.unlink()

    bot_args = ["-bot=restore_check", f"-bot_map={args.map}", f"-bot_out={args.out}", f"-bot_warmup={args.warmup}", f"-bot_frames={args.frames}"]
    botlib.run_game(root, bot_args, timeout=args.timeout, log=out_dir / "wine_stdout.txt")

    rep = botlib.read_report(out_dir)
    print("--- report")
    for k, v in rep.items():
        print(f"{k:20s} {v}")
    if not rep:
        print("no report; bot_log.txt:")
        print(botlib.tail(out_dir / "bot_log.txt", 40))
        return 2

    diff = out_dir / "bot_diff.txt"
    if diff.exists():
        by_symbol = defaultdict(lambda: [0, 0, None])
        for line in diff.read_text().splitlines():
            parts = line.split()
            if len(parts) < 2:
                continue
            addr, size = int(parts[0], 16), int(parts[1])
            sym, off = mapfile.lookup(addr)
            key = sym.name if sym and sym.va >= mapfile.image_base and off < max(sym.size, 1) else f"heap/unknown {addr:08x}"
            if sym and key.startswith("heap"):
                key = f"heap block near {sym.name}" if addr < mapfile.image_base + 0x1000000 else "heap block"
            entry = by_symbol[key]
            entry[0] += 1
            entry[1] += size
            if entry[2] is None:
                entry[2] = line
        print(f"--- {len(by_symbol)} differing symbols/areas ({diff})")
        for key, (runs, nbytes, first) in sorted(by_symbol.items(), key=lambda kv: -kv[1][1])[: args.max_print]:
            print(f"{nbytes:8d} bytes {runs:5d} runs  {key:50s}  e.g. {first}")

    print("--- bot_log.txt")
    print(botlib.tail(out_dir / "bot_log.txt", 25))
    return 0 if rep.get("result") == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
