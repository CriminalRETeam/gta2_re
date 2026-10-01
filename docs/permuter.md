# Using cpp_permuter

`3rdParty/cpp_permuter` (submodule) brute-forces source rewrites of one function (reorder
locals, invert ifs, scope blocks, swap operands, ternary ↔ if, ...) and keeps the ones whose
code gets closer to the original. Use it for the "right logic, wrong shape" part of matching
before spending time on manual permutations. See its README for the passes and options.

```sh
git submodule update --init 3rdParty/cpp_permuter
Scripts/permute.sh Source/sound_obj.cpp sound_obj::UpdateCarEngineAudio_57E220 57e220 -j 4 -n 400
Scripts/permute.sh Source/Foo.cpp Foo::Bar_123456 123456 -m exhaustive -p reorder_saves+invert_if -j 4
Scripts/permute.sh Source/Foo.cpp Foo::Bar_123456 123456 --base-only   # just print the score
```

- The wrapper builds the permuter into `build_permuter/` on first use. It compiles candidates
  with the VC6 flags (and the per-file flags from `cmake/vc6.cmake`), and scores them with
  `Scripts/bin_comp/permuter_score.py`.
- `permuter_score.py` doesn't need the original exe. It disassembles the function from the
  candidate `.obj`, patches each relocation to a fake address unique per symbol, post processes it
  like `target_asm.json`, and prints the number of differing lines (0 = match). On every
  `MATCH_FUNC` that has target asm (83 of them), it scores 0 from the build's own objects.
- Remove `NOT_IMPLEMENTED` / `WIP_IMPLEMENTED` from the function first.
- Results go to `permuter_out/output-<score>-<n>/` in the current directory: `source.cpp`,
  `function.cpp`, `diff.txt`, `asm_diff.txt`. Copy the function back, then run `build.py` and
  `compare_builds.py` as usual. The passes assume fields don't alias, so read the diff.
- About 400 candidates a minute with `-j 8` on 4 cores.
