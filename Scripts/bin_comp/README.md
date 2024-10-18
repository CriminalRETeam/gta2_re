## Auto function matching check/comparison tools
This directory contains a bunch of scripts that are executed manually or during a build to check if a reimplemented functions asm matches the original function.

## ida_dump_func_data.py
Run from within ida to generate `og_function_data.csv`. This is checked into the repo so generally you won't need to run this locally.

## generate_og_asm_hashes.py
More statically generated data like `og_function_data.csv` which is a record of function virtual addresses and hashes of that functions asm after it has been passed through `post_process_asm.py`. This is required by ` compare_function.py` when checking if functions are matching during a CI build when the original executable isn't available. This output is also checked into the repo so again generally you won't need to run this locally. It does however require an update if `post_process_asm.py` is updated.

## msvc_dump_func_data.py
Parses the linker map file from the build to generate `new_function_data.csv` which is the same as the output from `ida_dump_func_data.py` but pulls data from the linker map `output.map` in the `build_vc6` dir instead of the ida database.

## post_process_asm.py
Rewrites an asm file in place stripping anything that appears to be a reference to anything that can be relocated - for example data and other function references. This is because the recompiled binary will have changed offsets/virtual address locations. And hence comparing the asm directly would always result in it appearing to not actually match.

## collect_matching_functions.py
Outputs `functions_to_compare.txt` after processing the source code to find annotations that indicate a function has been implemented and is expected to be matching in its current impl.

## compare_function.py
Compares a function from the og and recompiled binary to check if they appear to match at the asm level. This is done by passing in the functions file offset and size for both og and rebuilt binaries. It will dump the asm from both and run `post_process_asm.py` on both and then check if they are equal. Since we can't distributed the original executable during a CI build the new asm will be hashed and compared to an existing hash of the post processed asm from the og binary by loading the record from `og_asm_hashes.txt`.

## compare_all_functions.py
Much like `compare_function.py` but will check all functions in `functions_to_compare.txt` match.

