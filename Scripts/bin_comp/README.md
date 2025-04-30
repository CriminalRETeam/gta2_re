## Auto function matching check/comparison tools
This directory contains a bunch of scripts that are executed manually or during a build to check if a reimplemented functions asm matches the original function.

## ida_dump_func_data.py
Run from within ida to generate `og_function_data.csv`. This is checked into the repo so generally you won't need to run this locally.

## msvc_dump_new_data.py
Parses the linker map file, executable and the gta2 lib from the build to generate `new_data.json` from the `build_vc6` dir instead of the ida database. This dumps the mangled name, virtual address, file offset, original function addr and function match status (if it has been annotated in the RE'd source).

## post_process_asm.py
Rewrites an asm file in place stripping anything that appears to be a reference to anything that can be relocated - for example data and other function references. This is because the recompiled binary will have changed offsets/virtual address locations. And hence comparing the asm directly would always result in it appearing to not actually match.

## compare_function.py
Compares a function from the og and recompiled binary to check if they appear to match at the asm level. This is done by passing in the functions file offset and size for both og and rebuilt binaries. It will dump the asm from both and run `post_process_asm.py` on both and then check if they are equal. Since we can't distributed the original executable during a CI build the new asm will be hashed and compared to an existing hash of the post processed asm from the og binary by loading the record from `og_asm_hashes.txt`.

## compare_all_functions.py
Much like `compare_function.py` but will check all functions in `new_data.json` match that have been marked as matching.

