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


## Optional numeric evidence in check_match_refs.py

From this directory after building, run:

```sh
python check_match_refs.py --numeric-json numeric.json
python check_match_refs.py 0x430ac0 --numeric-json car-bound.json
```

The existing global/callee/switch verdict and exit status stay unchanged. The
optional JSON adds review candidates for differing numeric operands hidden by
normalization, including small pushed arguments and sign-extended bounds.
It records raw instruction bytes, decoded operands, input/implementation hashes
and coverage/exclusion counts, deterministically for unchanged inputs and tools.
Numeric candidates do not become reference-check failures or prove a game bug.

This diagnostic requires complete file-backed function bytes, normalized-equal
assembly and identical instruction encodings/operand kinds. It excludes values
inside either image's span, including headers/gaps, to avoid treating relocated
addresses as numbers. Unknown runtime pointers can still become candidates.
Traversal follows both direct conditional edges and call fallthrough, without
executing branch conditions or callees; this is a syntactic overapproximation.
It stops at indirect jumps and does not follow switch tables. Unreached pairs,
invalid/truncated bytes and unsupported alignment are excluded and counted.
The existing REVIEWED reference waivers are unchanged; numeric evidence is a
separate review-only result. A clean numeric report is not an equivalence proof.

Original-free regression tests:

```sh
python -m unittest discover -s . -p test_check_match_refs_numeric.py
```
