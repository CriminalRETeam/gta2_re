"""
Regression check that does NOT need the original 10.5.exe.

Compares every function marked MATCH_FUNC (func_status 0x1) in a saved baseline
build against the current build. The asm is post processed the same way
compare_function.py does it, so relocated addresses are ignored. If a matching
function's asm is identical to the baseline then its match status can't have
changed (assuming the baseline passed compare_all_functions.py).

Usage (from Scripts/bin_comp, after a build + msvc_dump_new_data.py):
    python compare_builds.py --save BASELINE_DIR    # snapshot current build
    python compare_builds.py BASELINE_DIR           # compare current build to snapshot

Exit code 1 if any matching function changed or went missing.
"""
import argparse
import json
import os
import shutil
import sys

import compare_function
import post_process_asm

NEW_EXE = "../../build_vc6/decomp_main.exe"
NEW_DATA = "new_data.json"


def load_og_sizes():
    sizes = {}
    with open("og_function_data_v105.csv") as file:
        for line in file:
            rec = line.rstrip().split(",")
            if len(rec) >= 4:
                sizes[int(rec[1], 16)] = (rec[0], int(rec[3], 16))
    return sizes


def matching_funcs(new_data_path):
    with open(new_data_path, "rt") as file:
        new_data = json.load(file)
    ret = {}
    for rec in new_data["functions"]:
        if rec["func_status"] == "0x1":
            ret[int(rec["og_addr"], 16)] = int(rec["func_fo"], 16)
    return ret


def func_asm(exe, offset, size):
    func_bytes = compare_function.get_bytes_from_file(exe, offset, size)
    return post_process_asm.post_process_asm(compare_function.dism_func(func_bytes))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("baseline_dir")
    parser.add_argument("--save", action="store_true", help="save the current build as the baseline")
    args = parser.parse_args()

    base_exe = os.path.join(args.baseline_dir, "decomp_main.exe")
    base_data = os.path.join(args.baseline_dir, "new_data.json")

    if args.save:
        os.makedirs(args.baseline_dir, exist_ok=True)
        shutil.copy2(NEW_EXE, base_exe)
        shutil.copy2(NEW_DATA, base_data)
        print(f"Saved baseline to {args.baseline_dir}")
        return 0

    og_sizes = load_og_sizes()
    base_funcs = matching_funcs(base_data)
    cur_funcs = matching_funcs(NEW_DATA)

    changed = []
    removed = []
    for og_addr, base_fo in sorted(base_funcs.items()):
        name, size = og_sizes.get(og_addr, (hex(og_addr), None))
        if size is None:
            continue
        if og_addr not in cur_funcs:
            removed.append(name)
            continue
        if func_asm(base_exe, base_fo, size) != func_asm(NEW_EXE, cur_funcs[og_addr], size):
            changed.append(name)

    added = [og_sizes.get(a, (hex(a),))[0] for a in sorted(set(cur_funcs) - set(base_funcs))]

    for name in changed:
        print(f"CHANGED: {name}")
    for name in removed:
        print(f"NO LONGER MARKED MATCH: {name}")
    for name in added:
        print(f"NEWLY MARKED MATCH (verify against 10.5.exe): {name}")
    print(f"baseline matching: {len(base_funcs)}, current matching: {len(cur_funcs)}, "
          f"changed: {len(changed)}, removed: {len(removed)}, added: {len(added)}")

    return 1 if changed or removed else 0


if __name__ == "__main__":
    sys.exit(main())
