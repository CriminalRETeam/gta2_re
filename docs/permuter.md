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
  The fake addresses are 0x10000 apart. With 0x10, a `symbol+offset` operand
  (`gTrainStationList+0x10`) could land on the next symbol's fake address and shift every later
  `stable_name`.
- Remove `NOT_IMPLEMENTED` / `WIP_IMPLEMENTED` from the function first.
- Results go to `permuter_out/output-<score>-<n>/` in the current directory: `source.cpp`,
  `function.cpp`, `diff.txt`, `asm_diff.txt`. Copy the function back, then run `build.py` and
  `compare_builds.py` as usual. The passes assume fields don't alias, so read the diff.
- About 400 candidates a minute with `-j 8` on 4 cores.
- `permute.sh` passes `--op-alias "*=Multiply_408680" --op-alias "+=Add_408660" --op-alias
  "neg=Negate_4086A0"`. The `named_op` pass then tries the named out-of-line `Fix16` versions of
  `*`, `+` and unary `-`, for
  the inline-budget problems (see matching_quirks.md). `cast_operand` tries integer casts on the
  operands of `/ % >> < > + -`, for signedness problems.
- Each improvement's `score_output.txt` has the unified diff of target vs candidate asm.
- `cache_member` (a member path read into a local: by value, `T&` or `T*`, typed from the member
  declarations in the included headers), `ref_local` (`T x = e` vs `const T& x = e`), `incdec`,
  `cond_temp` and `pow2_shift` are the rewrites most often tried by hand. So are `bool_assign`
  (`x = c;` vs `if (c) x = 1; else x = 0;`), `reuse_local` / `split_local` (one local for two
  values, or two) and `inline_use` (read a local's initializer again at one use).
- `permuter_score.py` adds 2 for each call to a function named after an address (`Name_ADDRESS`)
  that the original doesn't call: the asm comparison alone only checks that call targets map one
  to one. Calls to an inline's COMDAT copy (`??DFix16@@...`) are free, as in the build's verifier.
- `permute.sh` passes `--extern-globals`: each global the function reads can also be switched
  between its `DEFINE_GLOBAL...` definition and an `EXTERN_GLOBAL` declaration in that file (or the
  other way, with the definition copied from another .cpp). See the 2-byte global entry in
  matching_quirks.md. If a candidate wins with a switch, move the definition by hand: an extern
  needs the `DEFINE_GLOBAL` line in another .cpp that uses the global.
- Every run keeps a checkpoint in `permuter_out/`. Run the same command again with `--resume` to
  carry on after a timeout or Ctrl-C (`-n` then counts the new run's compiles). With `-j 1` a seed
  always gives the same candidates.
- Run one permuter at a time if you share the machine: `flock -o /tmp/permute.lock Scripts/permute.sh ...`.

## Sweeping many functions

`Scripts/permute_sweep.py --out DIR --minutes 15 --parallel 2 --max-base 60` runs `permute.sh` over
every `WIP_FUNC` (or a `--list` of "Source/File.cpp Class::Func addr" lines), skipping those whose
unmodified score is above `--max-base`, nearest first. Runs resume from their checkpoints when the
sweep is run again, and it prints each function's base and best score. Empty `WIP_IMPLEMENTED` /
`NOT_IMPLEMENTED` locally while it runs.

## Against 9.6f (`--96f`)

`Scripts/permute.sh <File.cpp> <Class::Func> <96f_addr> --96f ...` compiles each candidate with VC7.0
(`Scripts/compile_vc7.sh`: VC6 preprocesses, VC7 `/O2 /Ob0 /G5 /GX` compiles, about 1 s) and scores it
against `target_96f.json` (`permuter_score.py --96f`). The 9.6f address of a WIP is in
`Scripts/bin_comp/match_96f.json` (`pairs`) and `docs/inlines_96f.md`. 9.6f has inlining off, so its
functions are smaller and the source shape (statement order, helper calls, temporaries) is easier to
settle there; the same source compiled with VC6 is then the best 10.5 candidate. 9.6f is an older build,
so a few functions really changed between the versions. Get VC7 once with
`Scripts/tu_harness/fetch_compilers.sh` (only `msvc7.0` is needed).

### WIPs that `match_96f.json` left unpaired

`match_96f.json` misses some real counterparts. `Scripts/bin_comp/find_96f_counterparts.py` maps a WIP's
callees through the existing pairs and compares them with the calls of every unpaired 9.6f function;
`add_96f_target.py <105>=<96f>` then adds the pair and the 9.6f asm to the local `match_96f.json` /
`target_96f.json`, after which `show_96f.py`, `permuter_score.py --96f` and `permute.sh --96f` work for it
(both files are git-ignored: rerun it after fetching them again). Found so far (callees shared / mapped):

| 10.5 | 9.6f | shared |
|---|---|---|
| `miss2_0x11C::GetSpeed_50E190` (matched) | 0x47D070 | 6/14 |
| `Ped::FindBestTargetPed_466BF0` | 0x437BE0 | 6/6 |
| `Ped::BusCustomer_AI_461290` | 0x4427E0 | 14/19 |
| `Ped::HandlePedHitByObject_45D000` | 0x441A30 | 15/28 |
| `sub_5DF270` (Weapon_30.cpp) | 0x4CEF40 | 9/13 |
| `sound_obj::ProcessOtherObjects_41F520` | 0x41A3C0 | 9/9 |
| `sound_obj::HandleVocalStreamSwitching_57DF10` | 0x4B25D0 | 12/16 |
| `Sprite::Draw_59EFF0` | 0x4BE060 | 13/16 |
| `TrafficLight_20::Init_5C1D00` | 0x4C3C70 | 22/26 |
| `Map_0x370::sub_4E6660` | 0x467110 | 56/56 |
| `Particle_4C::PoolUpdate` 0x53D260 | 0x490760 | 25/28 |
| `Particle_4C::UpdateObjectBeamLink_state_38_538AC0` | 0x48F230 | 12/24 |
| `Particle_8::EmitElectricArcParticle` 0x540320 | 0x48DDC0 | 9/18 |

Matching the source against 9.6f first settles its shape fast (VC7 compiles in about 1 s): for
`FindBestTargetPed_466BF0`, `Fix16 dz; dz = Abs(...); if (dz >= kFpOne) return 0;` (9.6f calls
`greater_than_or_equal`, so the source says `>=`) and `Fix16 dist; dist = MaxAbsDistance(...)` took the 9.6f
score from 201 to 50.

## Related tools

- `Scripts/quick_score.sh <Source/File.cpp> <addr> <symbol_substring> [-q]` compiles one TU into a
  private obj and scores one function with `permuter_score.py`, without touching `build_vc6/`, so
  several can run at once. Use it for hand experiments instead of `build.py --single_cpp`.
- `Scripts/decl_shuffle.py <Source/File.cpp> <func_name> <addr> [-n 300] [--seed 1]` tries random
  orders of the declaration lines at the top of one function (on a temp copy of `Source/`, scored
  with `quick_score.sh`) and prints the best block. Declaration order decides stack slots and dead
  parameter slot reuse, and the permuter rarely tries a full shuffle; it took
  `InsertLineBreaksAndGetNumLines_5B5BC0` from 232 to 56.
- `Scripts/bin_comp/show_96f.py <addr>` prints the 9.6f version of a 10.5 function and the 9.6f
  bodies of the callees 10.5 inlined into it.
- Logic-bug finders (from `Scripts/bin_comp`, after a build and `msvc_dump_new_data.py`):
  `compare_globals.py` lists globals and float constants a WIP reads where the original reads
  others (the asm normaliser hides these behind `stable_name` numbers), and
  `compare_callees_multiset.py` lists calls to real functions that the original makes and ours
  doesn't, or the reverse. Both found real bugs (`SetupTrainAndBusStops_5794B0`,
  `Weapon_30::throwable_5DDFC0`). Run them before tuning registers.

## Rejected candidates

- **`PoliceCrew_38::sub_571A30`, score 129 → 72.** The candidate casts `(s8)pCar->field_76_last_seen_timer <= 200`, which is always true. VC6 then drops the compare and the score falls, but the original does `cmpw $0xC8`. With `(u8)` instead it scores 0.659 (base 0.748).
- **`MapRenderer::DrawDiagonalDownRightFace_4ECE40`, exhaustive run.** `(s8)right_word >> 13` changes the result from 0–7 to 0 or -1. The random run's candidate (a `u32` temp) was used instead.
- **`PedGroup::CoordinateGroupCarEntry_4C9F00` and `Ped::IncreaseWantedLevelFromDebugKeys_46EFD0`.** The permuter score went down but the real ratio got worse. Always re-check with the real ratio before committing.

- **`remove_stmt` and `move_stmt` can win by breaking the logic.** On `Map_0x370::sub_4E8370`
  `remove_stmt` "improved" the score by deleting `field_0_height--`, and on
  `ProcessOtherObjects_41F520` by deleting `sample_index = 1;`. Read the diff of every candidate
  from these passes, or leave them out (`-p`).

A cast that narrows a compared value into a range where the compare is always true or always false is a red flag: check the target asm's compare width.
