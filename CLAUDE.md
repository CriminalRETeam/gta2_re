# CLAUDE.md

GTA2 matching decompilation. The goal is C++ that compiles with **MSVC 6 (VC98)** to
byte-for-byte the same machine code as the original `10.5.exe`. The game is useful, but
every change here is judged first by whether it keeps or grows the match count.

## Rules

- **Do not break matching functions.** A function marked `MATCH_FUNC(0x...)` must still
  match after your change. The one exception: breaking one is OK only if the same change
  makes more functions match, so the **overall match count goes up**. Call out any such
  trade in the commit message.
- **The build must always work**: `python3 build.py` compiles, links and verifies with
  no errors.
- Keep commits small and self-contained, for example **one commit per warning fix**.
  VC6 codegen is fragile, so an innocent cast or signedness change in a shared header
  can change code in many functions. Small commits make it easy to bisect and revert.
- Never push to `master`. Work on a branch.
- Code style follows the surrounding code: `field_XX_name` members, `sub_ADDRESS`/
  `Name_ADDRESS` functions, types from `types.hpp` (`s32`, `u8`, `Fix16`, ...).

## Function markers (`Source/Function.hpp`)

Each reimplemented function is preceded by a marker giving its original address:

| Marker            | status | Meaning                                         |
|-------------------|--------|-------------------------------------------------|
| `MATCH_FUNC(addr)`| 0x1    | Claimed to match; **verified by the build**     |
| `WIP_FUNC(addr)`  | 0x2    | Implemented but not matching yet                |
| `STUB_FUNC(addr)` | 0x0    | Not implemented                                 |

Only `MATCH_FUNC` functions are checked. Promote a `WIP_FUNC` to `MATCH_FUNC` only once
it verifies.

## Building (Linux, wine)

One-time setup:

```bash
git submodule update --init --recursive   # VC6 toolchain lives in 3rdParty/gta2_re_compile_tools
sudo dpkg --add-architecture i386 && sudo apt-get update
sudo apt-get install -y wine64 wine32:i386 xvfb binutils-mingw-w64-i686
python3 -m venv venv && . venv/bin/activate && pip install -r requirements.txt
python3 vc6_setup.py                      # registers VC6 in the wine registry
```

Build and verify (a full build takes about 1 minute; output goes to `build_vc6/`):

```bash
. venv/bin/activate
export WINEDEBUG=-all
python3 build.py                  # compile + link + verify all MATCH_FUNCs
python3 build.py --ignore_no_match  # compile + link, don't fail on mismatches
python3 build.py --single_cpp Camera.cpp  # compile a single TU quickly
```

`build.py` checks for duplicated globals, builds with cmake/jom under wine, then runs
`Scripts/bin_comp/msvc_dump_new_data.py` (parses `build_vc6/output.map` to produce
`new_data.json`) and `compare_all_functions.py`. That last step prints
`<func> FAIL!` for each mismatch and `[N/M] funcs OK`. **N is the match count to protect.**

Compiler warnings show up as `...(line) : warning Cxxxx:` lines in the build output.
Tee it to a file to grep them: `python3 build.py 2>&1 | tee build.log`.

### Original executables

Verification needs the original `Scripts/bin_comp/10.5.exe` (and `9.6f.exe`), which
`build.py` downloads from `mouzedrift.s-ul.eu`. They are git-ignored and must never be
committed. If that host is unreachable (for example, a sandbox with restricted network),
place the files there by hand or use the baseline check below.

### Regression check without the original exe

`Scripts/bin_comp/compare_builds.py` compares each `MATCH_FUNC` in the current build with
a saved baseline build, using the same asm normalisation as `compare_function.py`. If the
baseline verified, identical asm means the function still matches.

```bash
# on a known-good commit
python3 build.py --ignore_no_match
cd Scripts/bin_comp && python3 msvc_dump_new_data.py
python3 compare_builds.py --save /some/dir/baseline
# after your change
python3 build.py --ignore_no_match
cd Scripts/bin_comp && python3 msvc_dump_new_data.py
python3 compare_builds.py /some/dir/baseline   # exit 1 + "CHANGED: <func>" on regressions
```

Functions it lists as `NEWLY MARKED MATCH` still have to be verified against `10.5.exe`.
Keep the baseline outside the repo, or under the git-ignored `build_vc6/`.

### Sanity-check the verifier

Before trusting a green run, break a known match and confirm the run turns red. For
example, in `Source/Camera.cpp`, `Camera_0xBC::sub_4357F0`, change `<` to `<=`. Both
`build.py` (`FAIL!`) and `compare_builds.py` (`CHANGED: Camera_0xBC::sub_4357F0`) must
report that function and only that one. Revert afterwards.

## Figuring out why a function doesn't match

Prefer **objdiff** to the Python asm dumps. It diffs at the object level with relocations
and symbols resolved, and it highlights the exact differing instructions and registers.
The `diff/*_asm*.txt` files written by `compare_function.py` are post-processed text
that is much harder to read.

1. Install `objdiff-cli` (<https://github.com/encounter/objdiff/releases>) or the GUI.
2. Build a target object from the original asm (needs `10.5.exe`):
   ```bash
   python3 Scripts/generate_target_asm_for_objs.py Camera.cpp   # writes Scripts/asm/Camera.cpp.asm + make_objs.sh
   (cd Scripts/asm && ./make_objs.sh)                           # i686-w64-mingw32-as -> Camera.cpp.obj
   ```
   This covers the `WIP_FUNC`/`STUB_FUNC` functions in that .cpp. For a single function,
   use `python3 Scripts/generate_function_decompme.py --objdiff <name|addr>` (or `--asm`).
3. Diff it against the compiled object:
   ```bash
   objdiff-cli diff -1 Scripts/asm/Camera.cpp.obj \
                    -2 build_vc6/CMakeFiles/gta2_lib.dir/Source/Camera.cpp.obj \
                    '?sub_4357F0@Camera_0xBC@@QAEXXZ'
   ```
   Without `-o` this opens the interactive TUI. `-o - --format json-pretty` gives
   one-shot output. Mangled names come from `nm -C <obj>`.
4. Iterate with `python3 build.py --single_cpp <file>.cpp` for fast rebuilds.

decomp.me scratches (`generate_function_decompme.py <name>`) use the `msvc6.4` preset
with `/TP /O2 /GX /EHsc`.

## Layout

- `Source/`: reimplemented game code (one class per `.cpp`/`.hpp`, named after the
  original class and its size, for example `Camera_0xBC`).
- `Source/Function.hpp`: `MATCH_FUNC`/`WIP_FUNC`/`STUB_FUNC` and global definition macros.
- `Scripts/bin_comp/`: verification scripts. `og_function_data_v105.csv` holds the
  original function addresses, offsets and sizes.
- `reccmp/`: config for the reccmp tool (used in CI, `python3 build.py --reccmp`).
- `3rdParty/gta2_re_compile_tools`: VC6 compiler, cmake and jom (submodule).
