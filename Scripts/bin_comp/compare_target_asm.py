"""
Compare functions in the current build against the original asm dumped by the
"Dump target asm" workflow (branch claude/target-asm), without needing 10.5.exe.

The comparison uses the same post processing as compare_function.py, so MATCH here
means compare_all_functions.py will pass for that function too.

Setup (from Scripts/bin_comp):
    git show origin/claude/target-asm:target_asm.json > target_asm.json
    git show origin/claude/target-asm:target_data.json > target_data.json

Usage (from Scripts/bin_comp, after a build + msvc_dump_new_data.py):
    python compare_target_asm.py                 # list every WIP/STUB function that matches
    python compare_target_asm.py 4ff990 57edb0   # MATCH or a diff of the post processed asm
    python compare_target_asm.py --raw 4ff990    # original vs build asm side by side
    python compare_target_asm.py --target 4ff990 # original asm only, with switch jump tables

WIP_IMPLEMENTED/NOT_IMPLEMENTED add code to a function, so remove them while comparing.
Only the dumped functions (WIP/STUB when the dump was made) are available.
"""
import argparse
import difflib
import json
import re

import compare_function
import post_process_asm

NEW_EXE = "../../build_vc6/decomp_main.exe"


def load_json(path):
    with open(path) as f:
        return json.load(f)


def build_asm(func, size):
    b = compare_function.get_bytes_from_file(NEW_EXE, int(func["func_fo"], 16), size)
    return compare_function.dism_func(b)


def jump_tables(target, data, addr):
    lo, hi = addr, addr + target["size"]
    for line in target["asm"].split("\n"):
        m = re.search(r"jmpl? \*0x([0-9a-fA-F]+)\(", line)
        if not m:
            continue
        table = hex(int(m.group(1), 16))
        raw = bytes.fromhex(data.get(table, ""))
        entries = []
        for i in range(0, len(raw), 4):
            e = int.from_bytes(raw[i : i + 4], "little")
            if not lo <= e < hi:
                break
            entries.append(hex(e))
        print(f"  jump table {table}: {entries}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("addrs", nargs="*")
    parser.add_argument("--raw", action="store_true", help="show original and build asm side by side")
    parser.add_argument("--target", action="store_true", help="show the original asm only")
    parser.add_argument("--target-json", default="target_asm.json")
    parser.add_argument("--target-data", default="target_data.json")
    args = parser.parse_args()

    targets = load_json(args.target_json)
    new_data = load_json("new_data.json")
    funcs = {}
    for rec in new_data["functions"]:
        if rec["og_addr"] and rec["og_addr"].startswith("0x"):
            funcs[int(rec["og_addr"], 16)] = rec

    if args.addrs:
        addrs = [int(a, 16) for a in args.addrs]
    else:
        addrs = [a for a, r in funcs.items() if r["func_status"] != "0x1"]

    matches = 0
    for addr in sorted(addrs):
        target = targets.get(hex(addr))
        if not target:
            if args.addrs:
                print(f"{hex(addr)}: not in {args.target_json}")
            continue

        if args.target:
            print(f"== {target['name']} {hex(addr)} size {target['size']}")
            print(target["asm"])
            try:
                jump_tables(target, load_json(args.target_data), addr)
            except FileNotFoundError:
                pass
            continue

        func = funcs.get(addr)
        if not func or target["pp"] is None:
            if args.addrs:
                print(f"{hex(addr)}: not in the build")
            continue

        mine = build_asm(func, target["size"])
        if args.raw:
            t_lines = target["asm"].split("\n")
            m_lines = mine.split("\n")
            for i in range(max(len(t_lines), len(m_lines))):
                t = t_lines[i] if i < len(t_lines) else ""
                m = m_lines[i] if i < len(m_lines) else ""
                print(("   " if t == m else ">> ") + f"{t:<45}{m}")
            continue

        mine_pp = post_process_asm.post_process_asm(mine)
        if mine_pp == target["pp"]:
            matches += 1
            print(f"MATCH {hex(addr)} {target['name']} (status {func['func_status']})")
        elif args.addrs:
            t_lines = target["pp"].split("\n")
            m_lines = mine_pp.split("\n")
            ratio = difflib.SequenceMatcher(None, t_lines, m_lines).ratio()
            print(f"DIFF {hex(addr)} {target['name']} ratio {ratio:.3f}")
            for line in difflib.unified_diff(t_lines, m_lines, "original", "build", n=2, lineterm=""):
                print(line)

    if not args.raw and not args.target:
        print(f"matches: {matches}")


if __name__ == "__main__":
    main()
