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
- `permute.sh` passes `--op-alias "*=Multiply_408680" --op-alias "neg=Negate_4086A0"`. The
  `named_op` pass then tries the named out-of-line `Fix16` versions of `*` and unary `-`, for
  the inline-budget problems (see matching_quirks.md). `cast_operand` tries integer casts on the
  operands of `/ % >> < > + -`, for signedness problems.
- Each improvement's `score_output.txt` has the unified diff of target vs candidate asm.
- `cache_member` (a member path read into a local: by value, `T&` or `T*`, typed from the member
  declarations in the included headers), `ref_local` (`T x = e` vs `const T& x = e`), `incdec`,
  `cond_temp` and `pow2_shift` are the rewrites most often tried by hand.
- Every run keeps a checkpoint in `permuter_out/`. Run the same command again with `--resume` to
  carry on after a timeout or Ctrl-C (`-n` then counts the new run's compiles). With `-j 1` a seed
  always gives the same candidates.
- Run one permuter at a time if you share the machine: `flock -o /tmp/permute.lock Scripts/permute.sh ...`.

## Rejected candidates

- **`PoliceCrew_38::sub_571A30`, score 129 → 72.** The candidate casts `(s8)pCar->field_76_last_seen_timer <= 200`, which is always true. VC6 then drops the compare and the score falls, but the original does `cmpw $0xC8`. With `(u8)` instead it scores 0.659 (base 0.748).
- **`MapRenderer::DrawDiagonalDownRightFace_4ECE40`, exhaustive run.** `(s8)right_word >> 13` changes the result from 0–7 to 0 or -1. The random run's candidate (a `u32` temp) was used instead.
- **`PedGroup::CoordinateGroupCarEntry_4C9F00` and `Ped::IncreaseWantedLevelFromDebugKeys_46EFD0`.** The permuter score went down but the real ratio got worse. Always re-check with the real ratio before committing.

A cast that narrows a compared value into a range where the compare is always true or always false is a red flag: check the target asm's compare width.
