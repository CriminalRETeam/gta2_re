"""
Writes matched_asm.json: the asm of every MATCH_FUNC in the current build, in the same format
as target_asm.json ({addr: {name, size, asm, pp}}).

A MATCH_FUNC verifies byte for byte against 10.5.exe, so its asm in our build is the original's.
permuter_score.py falls back to this file for addresses that target_asm.json doesn't have
(target_asm.json only holds WIP/STUB functions), so matched neighbours can be re-scored after
a change without the original exe. Only trust it for a build whose MATCH_FUNCs verified.

Usage (after build.py and msvc_dump_new_data.py):
    python dump_matched_asm.py
"""

import json

import compare_function
import post_process_asm
from compare_builds import NEW_DATA, NEW_EXE, load_og_sizes, matching_funcs


def main():
    sizes = load_og_sizes()
    out = {}
    for og_addr, fo in sorted(matching_funcs(NEW_DATA).items()):
        name, size = sizes.get(og_addr, (hex(og_addr), None))
        if size is None:
            continue
        asm = compare_function.dism_func(compare_function.get_bytes_from_file(NEW_EXE, fo, size))
        out[hex(og_addr)] = {"name": name, "size": size, "asm": asm,
                             "pp": post_process_asm.post_process_asm(asm)}
    with open("matched_asm.json", "w") as file:
        json.dump(out, file)
    print(f"wrote {len(out)} functions to matched_asm.json")


if __name__ == "__main__":
    main()
