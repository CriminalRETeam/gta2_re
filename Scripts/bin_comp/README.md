## Auto function matching check/comparison tools
This directory contains a bunch of scripts that are executed manually or during a build to check if a reimplemented functions asm matches the original function.

## ida_dump_func_data.py
Run from within ida to generate `og_function_data_v105.csv` or `og_function_data_v96f`. This is checked into the repo so generally you won't need to run this locally.

## msvc_dump_new_data.py
Parses the linker map file, executable and the gta2 lib from the build to generate `new_data.json` from the `build_vc6` dir instead of the ida database. This dumps the mangled name, virtual address, file offset, original function addr and function match status (if it has been annotated in the RE'd source).

## post_process_asm.py
Rewrites an asm file in place stripping anything that appears to be a reference to anything that can be relocated - for example data and other function references. This is because the recompiled binary will have changed offsets/virtual address locations. And hence comparing the asm directly would always result in it appearing to not actually match.

## compare_function.py
Compares a function from the og and recompiled binary to check if they appear to match at the asm level. This is done by passing in the functions file offset and size for both og and rebuilt binaries. It will dump the asm from both and run `post_process_asm.py` on both and then check if they are equal. Since we can't distributed the original executable during a CI build the new asm will be hashed and compared to an existing hash of the post processed asm from the og binary by loading the record from `og_asm_hashes.txt`.

## compare_all_functions.py
Much like `compare_function.py` but will check all functions in `new_data.json` match that have been marked as matching.

## Checks beyond the normalised asm
`compare_all_functions.py` renames every absolute address and stops at the function's last byte, so these check what it can't see. All are run from this directory after a build, exit with 1 on differences and have a `REVIEWED` dict for differences reviewed as equivalent. `run_extra_checks.py` runs the first four (`python3 build.py --extra_checks` does it after verifying).

- `check_match_refs.py`: the same globals and callees at the same instructions, and the same block for each switch value.
- `check_match_eh.py`: the EH data of each MATCH function (FuncInfo, unwind entries and their funclets: the frame slot destroyed and the destructor, try blocks and catch handlers).
- `check_match_consts.py`: for addresses with no original identity (float constants, string literals, vtables, GUIDs, COMDAT copies of inlines, CRT functions, imports, offsets into known globals), what they hold compared with what 10.5.exe uses at the same instruction.
- `check_global_values.py`: every global with an original address, after both exes' static initialisers ran under unicorn (`static_init_emu.py`), compared byte for byte, pointers by what they point at.
- `check_global_inits.py`: reads 10.5.exe's initialiser code for the store widths and simple `Fix16(n)`/`Ang16(n)` values of the `DEFINE_GLOBAL`s (no exit code, report only).

`ref_values.py` is the shared part: it maps a build address to what it stands for in 10.5.exe (markers, `DEFINE_GLOBAL`s, `_ADDRESS` names, file-local objects found in the object files, `reccmp/library.csv`, import names).
