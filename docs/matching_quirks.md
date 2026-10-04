# VC6 matching quirks

Things we have run into while matching functions against `10.5.exe` with MSVC 6. Each entry
lists what you see in the diff, what caused it, and the function where it was found, so you
can look at a real example.

Add to this file when you find something new. Keep entries short and point at a function.

## Before you start: things that hide or fake a match

**A marker at an address that isn't in `og_function_data_v105.csv` is never verified.** Three
`MATCH_FUNC`s at unlisted addresses (0x419CD0, 0x4AD0D0, 0x4ADDE0) turned out not to match. Add
the address to the csv (size from the target asm dump: `dump_target_asm.py` dumps the bytes of
marked unlisted addresses into `target_extra.json`) before trusting the marker.

**`WIP_IMPLEMENTED` and `NOT_IMPLEMENTED` add code.** The macros inject a static flag and a
logging `call` into the function body, so a function containing one can never match. Remove
the line (or compile it out locally) before comparing. Several WIP functions were already
matching apart from this, for example `Crane_15C::TargetTransporter_47F7F0` and
`CarPhysics_B0::DispatchCollision_55CA70`.

**`MATCH_FUNC` is only checked if the address is in `og_function_data_v105.csv`.** Functions
whose address is missing from the csv are silently skipped by `compare_all_functions.py`.
Some of those claimed matches were wrong (`Frontend::DrawLoading_4AD0D0`, `Frontend::DrawDeletePlayerDialog_4ADDE0`).
When you add a function that isn't in the csv (for example one split out of a thunk, see
below), add a row for it: name, address, file offset (address - 0x400000) and size up to the
final `ret`, excluding padding and any jump table after the code.

**The asm normaliser has blind spots.** `post_process_asm.py` replaces absolute addresses so
relocated code compares equal. It used to miss x87 memory operands (`fildl 0x6F633C`),
`and`/`or`/`xor` memory operands, and it crashed on instructions without operands
(`pushaw`). Those are fixed, but if a function looks identical by eye and still fails, check
whether the normaliser handles every instruction in it. `MapRenderer::ProjectVertBottom_4EAEA0` carried a
comment blaming "wrong global offsets" for years when it was this.

**The normaliser also hides real differences.** It renames call targets and globals in
order of first use, so calling the wrong function or touching the wrong global can still
pass. Before promoting, check calls and globals resolve to the same things (objdiff does
this). Examples it would not catch on its own: `Player::Hud_Controls_565890` called
`DoBrianTest_42D870` on the wrong global object, and `Car_BC::DetachTrailer_442760` pushed
onto the pool's free list instead of the active list.

A case found this way: in `Particle_8::EmitFireTruckSprayParticle_53FAE0` the asm matched
with `dword_6FD508 * dword_6FD554` while the original multiplies `dword_6FD554 * dword_6FD508`
(the two globals were swapped, the instructions identical). Comparing the referenced
globals per instruction catches this.

**Single-operand memory instructions weren't normalised.** `divl 0x705334` kept its raw
address, so dividing by a global could never match. `div`/`idiv`/`mul`/`neg`/`not` are now
handled. A `target_asm.json` dumped before that fix still has the old `pp` text, so recompute
`pp` with the current `post_process_asm.py` when one of those instructions is involved
(`sub_5BEED0`).

**Some `MATCH_FUNC`s were never verified.** `sound_obj::sound_obj` (0x419CD0) and
`Frontend::DrawLoading_4AD0D0`/`DrawDeletePlayerDialog_4ADDE0` have no csv row. Checked against the raw bytes in
`target_extra.json`, they are at 0.987, 0.957 and 0.931, so they're not matches.
`sound_obj::Release_41A290` and `Char_B4::IsThreatToSearchingPed_553330` were real matches and
now have rows.

**A build that hangs after "Built target" while the permuter runs.** Wine starts
`explorer.exe /desktop` on demand. When a build happens to start it, it inherits `build.py`'s
output pipe, and `build.py` waits for EOF forever. Kill the `explorer.exe /desktop` process,
and the build carries on with correct results.

## Control flow and layout

**`if (flag) call(); return flag;` after an if/else, not a call in each branch.** With one shared
tail, VC6 keeps the flag in a dead parameter slot and reloads it, and pushes `ebx`/`edi` late, as
in `Ped::HandlePickupCollision_45DE80`. A call plus `return 1` in each branch lets it propagate
the constant instead.

**`||` of a range test and an equality follows the grouping.** `(s >= 7 && s <= 8) || s == 11`
and `s >= 7 && (s <= 8 || s == 11)` are the same logic but branch differently: in the second the
`jl` on `cmp $7` skips the `== 11` test (`sound_obj::ChooseRadioEmitterForVehicle_57E6C0`).

**Case bodies are laid out in source order.** If the original's `mov $N,%eax; ret` blocks come
in a different order from yours, reorder the `case` groups to match (`sub_417AC0`,
`sub_417BA0`, `GetExplosionTypeForWallSide_528E00`).

**One shared `return true` block comes from nesting, not early returns.** In an EH-frame
function VC6 gives every `return` its own epilogue copy. If several failure checks in the
original all `je` to one `mov $1,%al` epilogue, nest the success path and put one `return true`
at the end. For example, `if (a && b) { ...; if (z < limit) { ...; return 0; } } return true;`
(`Particle_4C::UpdateDirectedBurst_state_13_14_36_539480`, 0.420 -> 0.453). When the original
repeats the epilogue after each check, early returns are right (`Particle_4C::UpdateCircularBurst_state_5_539890`).

**A one-case `switch` gives `mov/dec/jne`.** `if (notify == 1)` compiles to `cmpl $1,mem`;
`switch (notify) { case 1: ... }` loads the value and tests it with `dec %eax; jne`. Use
the switch form when the original has the load and `dec`
(`Network_20324::OnWmCommand_519FE0`).

**A redundant null test that VC6 doesn't fold.** The original tests the driver twice in a
row (`cmp %ebx,%ecx; je; cmp %ebx,0x15C(%ecx); je; cmp %ebx,%ecx; jne`). Testing the same
local twice folds away. Reloading the pointer through another path keeps the second test:
`if (pCar->is_driven_by_player()) { if (!pTrain->field_C_carriages[0]->field_54_driver) ...`.
VC6 CSEs the load into the same register but doesn't thread the branch
(`PublicTransport_181C::PublicTransportService_57A7A0`).

**Invert an `if`/`else` to let a case share a block with a later case.** In
`Police_7B8::sub_56FBD0`, case 0's `field_8_state = 3` is a `jne` into an identical block in
case 5. With `if (n) { state = 3; } else { ... }`, VC6 kept its own copy. With
`if (!n) { ... } else { state = 3; }`, the block moves to the end and merges (0.690 -> 0.802).

**Case groups out of numeric order.** The same function lays out state 5 before state 4, and
the case's own `if`/`else` follows the source too (`if (occ != 43) { angle } else { reset }`
in `Char_B4::HandlePedCollision_548BD0`). Read the jump table targets in address order to get
the source order of the cases.

**A countdown that returns goes in the `else`.** In `sound_obj::ProcessObject_Type12_41E850`,
each "play only now and then" case is `cmpw $0,w; jne <far>`, then the reload and a fall
through to the code after the switch. The `w--; return` blocks come at the very end of the
function. `if (w) { w--; return; } w = ...;` laid the decrement inline (0.716). With
`if (!w) { w = ...; } else { w--; return; }` the layout matched (0.907).

**Loops that count down separately.** `mov $0x5A0,%edi ... dec %edi; jne`, with the array
walked by pointer and the value counter kept in memory, is
`for (s32 i = 1440; i != 0; i--) { ...arg...; arg++; p++; }`. A `for (i = 0; i < 1440; i++)`
gives a pointer compare against the end (`cmp $end,%esi`), and a separate up-counter gets merged
with `arg` (`arc_tan_table_init_4052D0`).

**Two constant `fmul`s need two statements.** `i * 3.141592654 * (1.0 / 720.0)` folds into one
`fmull`. `f64 r = i * 3.141592654; ... tan(r * 0.001388888888888889)` keeps the two
(`arc_tan_table_init_4052D0`).

**`if/else` around a call vs a ternary argument.** `f(x == 0 ? 1 : 0)` gives `sete`, but
two `push`es that branch to one `call` come from `if (x == 0) f(1); else f(0);`
(`PedGroup::RemovePed_4C9970`).

**Merged `case` labels give a byte index table.** The original often has one jump table
entry per case, so write each case out.

**VC6 copies a small shared tail into both branches.** If both branches of an `if/else` end
with the same call and the original has that call twice, write it once after the `if/else`,
not in each branch. VC6 duplicates it, and it pops the callee-saved registers the
branches used before the copies (`Net_Send_Our_Inputs_4DACB0`: two `SendToAll_521B20`
calls in the asm, one in the source).

**A switch range that runs past the last real case.** If the index table covers values
that all go to `default`, a case at the top of the range exists in the source but does
nothing. An empty `case N: break;` is dropped, even with an explicit `default`. A dead
store in it keeps the range (`sound_obj::HandleTruckCorneringAudio_417FD0`, case 86).

**A case that only returns is dropped from the table.** VC6 removes a `case` whose body is
identical to `default`, which shrinks the switch range (`cmp $0x53` instead of `cmp $0x57`).
A dead store in that case keeps it in the table:

```cpp
case car_model_enum::none:
    car_name_word = 111; // dead store, keeps 'none' as its own table entry
    return;
default:
    return;
```

(`sound_obj::GenerateRadioVehicleDescription_426F20`, checked against the original jump and
index tables byte for byte.)

**Identical case bodies in two switches get merged** into the one that falls through to the
shared return (the last case of the later switch), so case order decides which copy survives.

**Cases matching `default` still need their own `case`.** A case whose body is the same as
`default` is dropped from the jump table unless it is written out. When the original table has
an entry for it, keep the case and put it where the original lays it out
(`ReturnAngleFromRoadDirection_4F7940`, `ObjectTypeToWeaponType_443CB0`). Register choice
(`cx` vs `dx`) across case blocks also follows the source order of the cases.

**A statement duplicated in both branches is hoisted after the test.** If the original
schedules a store between `test` and `je` (`mov byte,%al; test %al,%al; mov ..; mov ..,(..); je`),
write the store at the top of both the `if` and the `else` block rather than once before the
`if` (`Object_2C::HandleSpriteGroundAndCollisionSimple_523770`). The hoisted value can then share a
register with one computed for the condition: `a2_ = a2;` at the top of both branches put it in `ebx` with
the half constant in `CarPhysics_B0::UpdateZPosition_55B4F0`.

**`if/else` block order follows the condition.** The `then` block is usually laid out first.
If the original has your `else` block first, invert the condition and swap the blocks
(`Car_BC::sub_440510`, `GetDamageMultiplier_45CF90`, `Ang16::SnapToAng4_405640`). VC6 sometimes normalises
both spellings to the same code, in which case this won't help (`Ped::ProcessInCarObjective_463FB0`).

**`je tail; jmp next`** for an `if/else` whose branches share a tail comes from a `goto`.
In `frosty_pasteur_0xC1EA8::sub_512BA0`/`RecordWeaponHit_512C00`, early `continue`/`return`, the body
written in both branches, a ternary condition and an inline helper all failed to produce it.

The `goto` entries in this file were only kept after `goto`-free forms had been tried (see the
`goto` rule in `CLAUDE.md`). Try restructuring first. Plain returns or a flag sometimes give
the same code (`NetPlay::WaitForPlayersSync_5213E0`, `PedGroup::CoordinateGroupCarEntry_4C9F00`).

**`return a <= b` vs branches.** `return a <= b;` gives `xor eax,eax; cmp; setle al`. If the
original has `setle al` without the `xor`, write it as `if (a <= b) return true; return false;`
(`Ang16::IsAngleAhead_405C60`). The reverse also happens: branching `return 1`/`return 0`
code in the original may need an explicit `if` with `return true`/`return false`
(`sound_obj::IsTrainOrBoxcar_57F120`).

**Loop shape.** If the original re-reads `next` through the current node (`mov 0xC(%ecx),%eax`
where yours uses the loop variable's register), walk a single pointer:

```cpp
for (v3 = head; v3->next && v3->next->key < k; v3 = v3->next) {}
```

(`RouteFinder::InsertIntoOpenList_589420`, `Hud_Brief_704::AllocBrief_5D33F0`.)

**A jump table whose default lands on a case block.** VC6 builds a table only from four
explicit cases. `case 4: default:` counts as three cases plus a default, so you get a
`dec`/`je` chain. If the table entry for one case equals the `ja` default target, write that
case out with its own `return` and put the same `return` after the switch, so the two blocks
tail-merge. Case order in the source still sets the `cx`/`dx` alternation between blocks, even
when the case lands last in the layout. The permuter found `case 4` first (`sub_5345E0`).

**A `switch` whose cases all return the same value still loads the operand.** A stray
`mov 0x28(%ecx),%ecx` with no compare after it comes from `switch (field_28_state)` where every case
does `return true`: VC6 drops the compares and keeps the load (`Kfc_30::PedIsValid_5CBC60`; the case
values are a guess).

**Two returns can turn branchless.** `if (ok) return x; return 0;` came out as `neg/sbb/not/and`. The
original's `test/je` with two epilogues came from setting `x = 0` on the failure path and returning `x`
once (`Registry::Get_Int_Setting_5874E0`).

**`if (a > b) {...} else if (a < b) {...}`, not nested `<=`/`<`.** Same compares, different block order
(`CarPhysics_B0::SyncZWithTrailer_55B3F0`).

**A function can tail call a chunk that isn't in the function list.** `Ped::BecomeDummyOnPlayerDisconnect_470300`
ends in a jump into code at 0x43AA20 that IDA counts as a chunk of another function. It is written as its own
method (`Car_BC::sub_43AA20`, no marker since 0x43AA20 isn't in `og_function_data_v105.csv`), called in tail
position. `Hud_2B00::UpdatePauseSection_5D69C0` is the thunk form of the same thing (`add $0x2A1C,%ecx; jmp
0x5D6300`): the body moved to `Garox_12E4_sub::UpdatePauseSection_5D6300`, which is unverified for the same reason.

**`test; je L; jne end; L:` comes from `(a & m) == 0 || (b & m) == 0` with `a == b`.** When both
operands are the same value in one register, VC6 merges the two tests but keeps both branches, so the `je`
lands right after the following `jne`. In `BurgerKing_67F8B0::get_input_bits_4CEAC0`, `a` is `saved_input`,
which equals `*control_status` there. `&&`, a one-case `switch` or an inline helper all fold it away.

**Insert-into-list blocks follow the source order of the cases.** `Hud_Brief_704::SetHudBrief_5D3F10` matched
with the blocks in the original's order (empty list, insert by priority, replace current), a single walking
pointer as in 9.6f, and the tests reading the field directly: a function-scope iterator local swapped
`eax`/`ecx` everywhere.

**`Fix16::Abs` can merge two epilogues the original keeps.** The inline let VC6 merge the two sign cases
into one store; `if (v.mValue > 0) return v; return -v;` written out keeps one epilogue per case, as the
original has (`CarPhysics_B0::vec_len_552DE0`).

**Using the result of `+=` in a compare gives copy-then-compare.** `if ((ypos += gap) > X) break;` reproduced
the original's loop shape in `Frontend::ManageCredits_4B7A10` (with the `u16` timer/index fields).

**`if (n) { do {...} while (n); test }` skips a post-loop test for an empty loop.** A plain `for` inside an
`if` doesn't give the original's layout (`PedGroup::FindNearestOtherMember_4CAE80`). There, reading one
operand through an inline getter (`get_cam_x()`) and the other directly also set the load order of `a - b`.

**A goto loop that returns the same value from several places is a `for` with `continue`.** That gave the
shared `return 10` in `sad_mirzakhani::find_431EC0` (which also read the wrong field before).

**Search loops that return a pointer or NULL were inline helpers.** When the original tests `&array[i] == NULL`
and gives every `return false` its own epilogue, write each search as a file-local inline that returns the
found item or NULL. Open-coded loops make VC6 send all the returns to one shared block
(`Police_7B8::PromptCrewAtCarToPurseCriminal_5707B0`).

**`return a < N;` per case vs a bool local.** Returning the comparison in each case gives
`xor eax; mov field,edx; cmp; setl`; setting a bool local gives `cmpl $N,mem; setl` with no `xor`
(`Car_6C::CanAllocateOfType_446930`, cases in the original block order).

**`if (a || b)` through a `bool` local.** Written directly, VC6 laid the branches out inverted; computing
`bool aligned = ...; if (aligned)` first gave the original layout (`CarPhysics_B0::HandleUserInputs_55A860`,
which also inlines `IsVelocityAlignedWithHeading_40F840` and, inside it, `Fix16_Point::atan2_40ACD0`).

## Types and signedness

**A 2-byte global defined in the same file loads as 32 bits.** With the `DEFINE_GLOBAL` of an
`Ang16`/`s16`/`u16` global in the function's own .cpp, VC6 can load it with a 32-bit `mov` and
add it with `lea`, where the original has a 16-bit `mov`/`add`. With only an `EXTERN_GLOBAL` in
that file it emits the original's code, so move the definition to another .cpp that uses it
(`kAng180_6FD3EE` moved from `Wolfy_3D4.cpp` to `Particle_4C.cpp`: `Wolfy_30::state_3_12_540D30`
181 -> 32 lines). It can go the other way too (`gFaceCollisionMask_6F6002` and `kAng180_676772`
had to move *into* `sprite.cpp`), so compare the load width in the target first.
The move can also fix the order in which globals are reloaded after a call, not only the load
width: moving `gBlockLeft_6F62F6`/`gBlockRight_6F63C6` to `map_0x370.cpp` matched all nine
`MapRenderer::DrawPartialBlock*` functions. Try each global separately: there, moving
`gBlockTop` changed nothing and moving `gBlockBottom` made it worse.

**A flag returned with no `setne` is `char_type`, not `bool`.** If the original returns a
`char` local as is, a `bool` return makes VC6 normalise it (`Ped::HandlePickupCollision_45DE80`).

**`jae`/`jb` vs `jge`/`jl` means unsigned vs signed.** Fix the field or parameter type, not the
comparison (`RouteFinder_10::field_2` is `u16`).

**`test al,al; jbe`** on a byte means `if (x > 0)` with an unsigned `x`, not `if (x)`
(`Cooldown_4236C0`).

**`mov mem,%edx; and $1,%edx; cmp $1,%dl` is `(x & 1) == 1` with no cast.** A `(u8)` or
`(char)` cast, or an inline returning `char`, narrows the load to `mov mem,%dl; and $1,%dl`.
`!(x & 1)` gives `testb $1,mem` (`sound_obj::HandlePedVoiceEvent_423080`). Likewise,
`(field_21C & 0x20) == 0x20` (`Police_7B8::sub_56FBD0`).

**Unsigned compares on `char_type` counters.** `cmp $1,%al; jae` or `test %al,%al; ja` on a
counter field means the field is `u8`. `Police_7C`'s `field_70`..`field_73_next_tile_y` crew counts were
`char_type`, and changing them to `u8` moved no other function.

**`and $0xFFFF,%eax` vs `movswl`** is a `u16` vs `s16` parameter (`PedManager::DoIanTest_471060`).

**Return width.** `xor al,al`/`mov $1,al` returns a byte; `xor eax,eax`/`mov $1,eax` returns
32 bits. A function whose first path returns another call's `bool` unextended while other
paths set all of `eax` is still unsolved (`Car_BC::IsDoorLockedForPed_43B2B0`).

**`sub $C` vs `add $-C`.** `x -= 0x100;` (or `x = x - 256;`, `x += -256;`) compiles to
`sub $0x100`. `return x - 0x100;` from an inline helper gives `add $0xFFFFFF00`
(`SeqDiff` in NetPlay.cpp, `NetPlay::MakeSendData_51F420`).

**`flag ? '1' : '0'` vs `(flag != 0) + '0'`.** Storing to a `char`, the ternary gives
`setne %cl; add $0x30,%ecx` (32-bit add); the explicit bool sum gives a byte `add $0x30,%cl`
(`BurgerKing_67F8B0::AppendReplayHeader_4CDF70`).

**Adding a bool.** `setne al; add $0xE,%eax` comes from `(b != 0) + 14`, not `b + 14`
(`GetSirenSampleIdx_417B80`).

**Returning a class adds a flag local.** A zeroed stack slot (`push %ecx` and `movl $0,..(%esp)`)
in a function that returns a point means the return type has a destructor: return
`Fix16_Point`, not `Fix16_Point_POD` (`Fix16_Point_POD::Multiply_438FE0`, `Divide_442CB0`).
A function the decomp wrote as `Fix16_Point* f(Fix16_Point* out)` usually returned by value
in the original. Return a local filled in place instead (`Char_B4::sub_545580` uses
`FromPolar_41E210`).

**Passing by value vs by reference.** `mov 0x19(%edi),%al; push %eax` passes the byte;
`lea 0x19(%edi),%eax; push %eax` passes a pointer. The decomp had `gbh_DrawTriangle` and
`MapRenderer::draw_4E9EE0` taking the colour as `u8&`, which also passed a pointer to the
game's d3d dll where it expects the value.

**A u8 parameter type shows up as a missing `xor`.** Passing a `u8` field to an `s32`
parameter gives `xor %eax,%eax; mov 0x24C(%esi),%al; push %eax`, with the `xor` scheduled early. With the
parameter declared `u8` the `xor` goes and the load moves next to the push (`Ped::ExitTrainStateMachine_46D240`
calling `Car_BC::IsStoppedWithPavementAtDoor_43B140(u8)`). A shared prototype's parameter type moves code in every
caller, so check `compare_builds` after changing one.

**A u8 stored to a stack slot and then pushed as a dword is a `u8` local.** (`sound_obj::Tank_414A50`,
`Ped::IncreaseWantedLevelFromDebugKeys_46EFD0`, where the current and maximum star counts are read into u8 locals,
in that order.)

**A u8 loop index can give separate pointers and a count-down counter.** `for (u8 i = 0; i < 17; i++)`
over two `u16` arrays gives the original's two pointers plus a counter in a stack slot. An `s32` index gives
indexed addressing, and hand-written pointers merge into one pointer and a difference register
(`Player::RestorePowerUpsFromSave_5651F0`).

## Evaluation order and registers

**Read through the pointer, not a local copy.** `lea (%eax,%ecx)` where yours gives
`lea (%ecx,%eax)`, with no other difference, can come from a local copy of `*p`
(`Fix16 t = *pTarget; ... t + k`). Using `*pTarget` directly each time fixed the operand order
in `sub_405E80`. The permuter found it.

**Assigning wrapper vs returning wrapper for an out-of-line by-value call.** An inline wrapper
`a = a.Multiply_408680(b);` reads the result through `%eax` (`mov (%eax),%eax`), while a wrapper
that returns the product by value copies it from its stack temporary. A direct call also reads
through eax but saves one inline expansion, which can push another helper over the inline budget.
And `x *= f` loads the factor into eax, while a product temporary
(`(__int64)x.mValue * f.mValue`) loads `x` into eax first and does `imull f`. That holds only
while `x` isn't already in a register (`CalculateRearWheelForce_5620D0` didn't move).
Both were needed in `CarPhysics_B0::StabilizeVelocityAtSpeed_562910`.

**The destination's signedness can pick the add order.** In `field = (u32 expr) + s32 field`,
an `s32` destination adds a u32->s32 conversion, and that decides which register holds the sum
(and so the add operand order and where the store goes). No spelling of the expression moved
it; making the two destination fields `u32` matched `Map_0x370::OnModifiedMapDataLoaded_4E8C00`.
When an add's operand order won't move, check the destination field's type.

**Copy the 9.6f store order for a run of field stores.** VC6 doesn't reorder independent
stores much, so their source order shows in the scheduling. `UpdateCarEngineAudio_57E220`
matched once the second sample's stores followed 9.6f exactly (rate, volume, `Fix16(0)`
distance, pan, type). Earlier `operator=` "barrier" workarounds were no longer needed.

**A dead store can change register allocation.** In `HandlePedVoiceEvent_423080`, a repeated
`field_58_type = 20;` just before the rate store is deleted by VC6, but the sum then stays in
edi like the original. Any dead store to the sample works. 9.6f has none, so treat it as a
last-resort workaround and say so in a comment. It did not help the similar 42A500.

**Write out `RotateVector_41FC90` with an `Ang16&`.** Taking the sprite angle as `Ang16&`
(not a copy) keeps sin in ebp and spills cos, as in `GetDoorWorldPos_43B420`. Also keep the
`old_xpos` copy and write `x*cos + y*sin`. A static inline version doesn't match.

**A tail call after a null test comes from an early return.** `test; jne L; xor al,al; ret;
L: jmp callee` is `if (!p) return 0; return p->F();`, not if/else, which puts the `jmp` elsewhere
(`Car_BC::IsThreatToSearchingPed_43AAE0`, `Object_2C::sub_525100`).

**`u8 + int` passed to a `u8` parameter is added in a byte register.** An `s32` temporary keeps
the `mov cl; mov eax,ecx; add eax` shape (`Car_BC::TurnToWreck_4436A0`).

**`Fix16(0, 0)` vs `Fix16(0)` as a by-value argument.** `Fix16(0)` goes through the
out-of-line `Fix16(s32)` ctor with stack temporaries; `Fix16(0, 0)` is an `xor` and a push
(`sound_obj::InitMusicAndCopRadio_57E960`).

**A one-case `switch` on a byte: `xor eax,eax; mov al,[x]; dec eax; jne`.** An `if (x == 1)`
gives `cmpb $1` instead (`Frontend::DrawDeletePlayerDialog_4ADDE0`).

**A `const T&` local can move its load.** `Mike_A80::DebugDrawProfiling_4FF250` loaded the five
frame averages in the wrong order whatever the order or grouping of the sum. The fix was in an
unrelated statement above it: `const s32& polys_drawn = pGlobals[0];` instead of
`s32 polys_drawn = pGlobals[0];` defers that load, which frees the register the sum needs.
The permuter's `ref_local` pass found it (exhaustive, depth 1).

**Both calls run, first result kept: `b = f(); b |= g();`.** When the original calls both
helpers and keeps the first result in a byte register, `f() || g()` short-circuits and a single
`f() | g()` defers the first compare. Two statements match (`Sprite::ShrinkSprite_59E390`).

**`Fix16(u8)` delays the shift.** With the `Fix16(u8)` constructor VC6 keeps the u8 around and
shifts at the use; `Fix16(v << 14, 0)` shifts at once like the original (`Car_BC::CarShrinkSprite_43DC80`).

**Declaration order of `Fix16` locals picks which product goes first.** In `Trailer::sub_407BD0`
swapping the operands of `+` didn't change the multiply order, declaring `cos` before `sin` did.

**Call arguments are evaluated right to left, inline expressions included.** Writing the
offset math inside the call (`set_xyz_lazy_420600(x + sin*v, y + cos*v, z)`) gives the original's
y-first order and early load of z. A separate `PolarToCartesian` statement into locals computes x
first (`Char_B4::state_1_5504F0`).

**Ctor EH frame missing when the member ctors come first in the TU.** If the member
constructors are defined earlier in the same .cpp, VC6 infers they can't throw and drops the
ctor's EH frame. Moving the ctor above them restored it (`Hud_2B00::ctor_5D6CD0`).

**`A && (B || C || D) ? x : y` gets normalised.** Only a nested `if` with `y` repeated in the
outer `else` (VC6 tail-merges the copies) gave the original block order (`Ped::ReactToAttacker_465B20`).

**Ternaries into one local stored after a switch.** One store after the switch instead of one per
case, with `b ? 4 : 3` as setne/add and `b ? 1 : 2` as neg/sbb/add (`Char_B4::sub_54C090`).

**A switch default that sets a value then shares a fix-up.** `default: v = 127;` plus one fix-up
after the switch lets VC6 jump-thread the default (`ChooseRadioEmitterForVehicle_57E6C0`, closer).

**Fix16 constant parameter by `const Fix16&` in an inline helper.** A per-TU `Fix16(1)` passed by
value reorders the loads; a const reference matches (`Player::RestoreCarsFromSave_56A0F0`).

**Indexed store vs pointer local in a loop.** `arr[idx++] = v` puts the strength-reduced `lea`
in the preheader, after the loop entry check; an explicit pointer local is initialised before the
check (`CarPhysics_B0::StepMovementAndCollisions_55E470`).

**Fix16 compares can push a small function over the inline budget.** In `sub_4F76A0` the
`Fix16_Point()` ctor went out of line until the compares were written on the raw `mValue`.

**Ctor defined earlier in the TU drops the EH frame around `new`.** Same cause as the Hud ctor
above: with the member/callee ctor body visible earlier, VC6 knows it can't throw. Put the
definitions in original address order (`Montana_4::ctor_5C5E70`).

**A small constant `memset` looks like field stores.** A separate `xor`'d zero register storing
three dwords and a word was an inline `memset` of 14 bytes (`InitializeGame_4DA4D0`).

**An inline bool helper as a condition can change block layout** where the expanded `&&` test
doesn't (`Car_BC::IsSwatVanOrBankVan_403BC0` in `PedGroup::CoordinateGroupCarEntry_4C9F00`).

**u8 shift-or: `a <<= 4; a |= b; return a;`** gives byte ops (`shl %al`, `or %cl,%al`);
`return (a << 4) | b` promotes to int ops (`Frontend::GetNextUnlockedBonusStage_4B7360`).

**`Fix16::operator=` on a member is a scheduling barrier** and blocks constant CSE across it
(`UpdateCarEngineAudio_57E220`, 6 -> 1 diff lines). With `field_28_distance = 0;` VC6 can't hoist
the following `mov ecx, esi` or a constant load above the neighbouring stores; `.mValue = 0` or
`= Fix16(0, 0)` removes the barrier. `operator=` on a dead local is not a barrier. By-value Fix16 returns assigned through
`operator=` on a named local give the ecx return slot + `mov eax,ecx` (`Car_14::GetRandomTrafficSpeed_583750`).

**Pick the GetLength variant from the call targets.** Resolve the calls in the og csv: one
variant has out-of-line Negate/Abs/Multiply for x*x and inline y*y (`CarPhysics_B0::ScarePedsOnDrivingFast_559C30`).

**`A + B` in one return expression evaluates the right operand first** for a member
`operator+`, which can be the original order (`CarPhysics_B0::ComputeCombinedCenterOfMass_559EC0`,
which also needed `Fix16 m; m = f();` to copy the return into a register).

**An EH state above 0 at entry means extra named locals with constructors.** In `pistol_5DD860`
the frame size and entry state showed two `Fix16_Point` locals where we had one (0.721 -> 0.931).

**An if/else-if chain with nested returns, not a switch returning a compare.** `if (a3 == 1) { if (a2 == N) return 1; } else if ...` with one shared `return 0`, in the original test order (`sad_mirzakhani::sub_432170`).

**Write `base + k*i`, not a running local.** VC6's strength reduction of `ypos + 40*i` gives the
original's induction variables and slots; hand-written running sums don't (`DrawScoreTable_4B5430`).

**`and al,0xFE` comes from `&= ~1u` on a 32-bit field.** `x & 0xFE` also clears the upper bytes
(`CanStepForwardWithRegionCheck_54ECB0`, also a bug fix).

**A dead `cmp $6; mov 0x6C(...)` with no test after it** is a condition folded into a default with
the same result (`Bink::OpenSlot2_5133E0`, `OpenSlot1_513560`). IDA cuts such functions off after the
`FatalError` call when the error path is the else branch laid out last.

**A store repeated in each branch vs once after the if/else.** Writing `field_C = 0` in both
branches changed which register holds zero and gave per-exit stores like the original
(`Object_2C::ReleaseSubObjects_527F10`).

**Ang16 Normalize inlined on a register turns into a closed form.** With an `Ang16(s16, u8)`
by-value ctor VC6 replaces the Normalize loops with `(1439 - v) / 1440 * 1440`; the const-ref
`Fix16_To_Ang16_40F540` ctor left Normalize out of line (`CarPhysics_B0::ApplyMovementStep_560F20`).

**A car_info flag method instead of `(flags & N) == N` inline** removed a `sete` in a `!a && !b`
chain (`PickUpCar_47F930`, closer).

**A parameter slot reused for a local means a logic bug can be hiding.** In
`Car_BC::UpdateTrainCarriagesOnTrack_4413B0` the original reused a param slot and reloaded registers
from the out-params at the end of the loop: each carriage starts where the previous one was placed
(params copied into x/y/z locals before the loop, updated each pass).

**Zero-initialised byte locals and declaration order.** A byte local declared right after one VC6
keeps in a zero register is initialised from that register; earlier-declared ones get `movb $0`
(`Ped::AttackTargetStateMachine_46D460`, closer).

**10.5 can call the real function where 9.6f inlined a copy** (`GetSpeedVector_52ADF0`, not 9.6f's
482BA0, in `Object_3C::GetMovementSpeedAndAngle_521FD0`).

**A default that sets a value plus one check after the switch** (`if (cur == 1) {...; return;}`) lets
jump threading produce the original's `cmp $1; je` (`Wolfy_7A8::sub_543690`, 113 -> 12).

**By-value returns: stack slot or eax.** If the original reads a by-value (hidden pointer) return
back from its stack slot, use a named local in its own block; it gets built in a dead parameter slot
(`Object_2C::NewObj3C_528130`). If it reads it through eax, write `T x; x = call();` or pass the
temporary to an inline taking `const T&`; a `const T&` local doesn't work (`HandleWorldCollision_55FD00`).

**Some TUs inline GetLength with their own zero constant** and their own mix of out-of-line
Negate/Abs/Multiply/Add; give them file-local helpers (`Object_2C::SetMovementVector_5224E0`, 528130).

**Jump tables need the explicit cases plus a separate default.** `case 4: default:` merged gives a
dec chain (`Char_B4::sub_54C3E0`). Paths that all jump to one shared `xor al; ret` are a `break` out of
the switch to a `return 0` after it (`Object_2C::ShouldCollideWithSprite_525370`).

**An inline helper whose operators stay out of line can't be called as is.** Our build would call
its own local copy of the operator, not the original's `Negate_4086A0`; use a copy of the helper with
the explicit `..._4086A0` call (`Sprite::RotatedRectCollisionSAT_5A0380`). Explicit calls to
declared-only `EXPORT` functions add an EH frame while an object with a destructor is alive; `throw()`
on those declarations removes it (`CarPhysics_B0::UpdateReferencePoint_563460`).

**Calling a small inline helper costs an inline expansion.** Writing out `HalfWH_4BA0A0`'s two
divisions kept both inline (`Sprite::FindOverlappingBoundingBoxCorners_5A0150`). A Fix16 multiply is
`imul` when it is the only inline one, `__allmul` when several share a sign-extended operand.

**Byte bit read: `((u8)field & 1) == 1`** gives `mov %cl; and $1,%cl; cmp $1,%cl`
(`Ped_List_4::FindClosestPedInViewCone_4713C0`).

**Callee-saved pushes in the middle of a function.** VC6 sinks `push ebx/edi` to the path that
uses them only when the other path returns from the switch: a switch whose default returns, with the
rest after the switch, replaced a goto into a case (`CarPhysics_B0::ComputeSlopeCorrection_55AB50`).

**Merged case labels and jump tables.** `case 2` plus `case 3: case 4: case 5:` gave a compare chain;
giving `case 3` its own copy of the body produced the 2..5 jump table, and VC6 still merged the identical
blocks (`Object_2C::UpdateMovementAndEffects_527070`). Writing each case separately also keeps a
constant in a register: VC6 counts constant uses before merging (`Wolfy_7A8::sub_543690`).

**Take an inline's `this` into a local before the call** to set the prologue load order and the
register for a constant (`ExplodingScore_50::DrawNumbers_596C90`, with a private out-of-line copy of
the 9.6f inline 4B90E0).

**Implicit `u8 -> Fix16` arguments call the ctor out of line** (`FromInt_45C4E0`); `(s32)u8` builds it
in place inline; an explicit `Fix16(u8)` pushes the value (`TryCreateRoadblockAt_577370`).

**`Fix16(x + 1)` per iteration** is strength-reduced to an induction variable initialised after the
loop guard (`RectHitsDiagonalWall_4E11E0`, 175 -> ~12).

**One call after the switch, not one per case.** VC6 duplicates the call + ret tail back into each
case itself (`Ped::SetObjective_463570`, with a u8 mode local) or tail-merges the calls
(`Ped::SpawnWeaponOnDeath_45E080`). In 45E080 reading `field_1AC_cam.x/y/z` directly instead of the
by-value getters gave the per-case register rotation.

**EH state stores around temporaries depend on how the callee is defined.** VC6 dropped the state store
before a call to our inline `operator-()` that wasn't inlined; the original calls a real out-of-line
function and keeps it (`Object_2C::ResolveCollisionWithPed_5229B0`). `ApplyExplosionImpulse_443710` is
the reverse: `throw()` on `Divide_442CB0` would fix it but breaks Divide's own match.

**A pointer local can be rematerialised while a repeated expression is CSE'd into a slot.** In
`Ped::FollowCarOnFootWithOffset_46A350` a `Sprite*` local was recomputed from `field_150->field_50`
each use; writing the expression out each time gave the original's stack slot.

**A one-case switch gives `dec; je`** where an `if` gives `sbb` (`ExplodingScore_50::DrawSingleNumber_597100`).

**A trivial inline getter instead of a direct field read changes load order**: with the getter the
global was loaded before the pointer chain (`Weapon_30::car_mine_5E2550`, `GetH_447E10`).

**Calling a non-`throw()` function while a `Fix16_Point` is alive creates the EH frame.** The original
`ComputeRelativePointVelocity_561130` has none, which suggests `operator-` (0x40AC80) and `Normalize_406C20`
were known not to throw in that file; marking ours `throw()` breaks 5 matches, so it stays WIP.

**A block scope around a written-out rotation** (`{ Fix16 sin, cos, x_old; ... }`) lets the sin/cos
temporaries share stack slots with the argument temporaries (`EmitElectricArcParticle_540320`, 202 -> 21).

**`Fix16(rng(3) - 1)` in the Particle_4C jitter** is `movswl; add $0x3FFFF; shl $0xE` in all five
originals; ours is `shl; sub $0x4000`. Only an inline taking `s32` that does `v * 16384` gave the
`add` form, and only when VC6 hoisted the value (still unexplained).

**Implicit `s32 -> Fix16` by-value argument is built in the argument slot** (`push ecx; mov esp,eax;
shl; mov ecx,(eax)`); an explicit `Fix16(z)` is computed before the pushes
(`Car_BC::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0`).

**`IsFlagSet_411930(N)` vs `field & N`.** The helper gives `mov/shr/test $1`; the direct test gives the
original's `testb` (`ApplyImpactForcesAndDamage_55FA60`, 205 -> 19).

**A decrement in each branch, not once after.** Writing `--timer` in both branches gave the shared
`decb mem` tail and all pushes at entry; once after the if/else gave `mov/dec/mov` and late pushes
(`Train_58::UpdatePassengerAI_578390`, also a logic fix: an inverted `field_1818` test).

**Parameter slots reused as locals.** When the original keeps grid indices or loop counters in a
parameter's stack slot, assign them to the parameter (`Orca_2FD4::ComputePath_554AB0`, 216 -> 48).

**A zero-initialised local declared at the top becomes a zero register.** In
`PoliceCrew_38::State6_ShutDown_574720`, `u8 i = 0;` at the top made VC6 keep 0 in `ebp` for every null test,
zero store and `push 0` in the function. Declaring it just before its loop removed the zero register (410->119).

**`Fix16(s32)` and `Fix16(u32)` are separate out-of-line copies.** 0x4369F0 and 0x4926F0 have identical code but
are distinct COMDATs; passing a u32 where the original calls 0x4926F0 matched `Garox_12E4_sub::DrawPause_5D63B0`.

**A switch split on an odd value was two switches.** VC6 splits a sparse switch on the median case. When the
original splits elsewhere (110 in `sound_obj::SelectObjectImpactSound_413120`), write `if (x <= 110) { switch } else
{ switch }`.

**Identical bodies in an else-if chain are tail-merged** with a `jmp` into the first copy
(`CarAI_78::ManageCollisions_452A20`). Stacked case labels give a `cmp/jl/jle` range test, while separate
identical case bodies give the `sub/dec/je` chain (`Char_B4::state_7_551CB0`).

**Moving a callee to its own TU brings back EH state stores.** With `GetBoundingBoxCorner_562450` defined earlier
in sprite.cpp, VC6 knew it couldn't throw and dropped the state stores around its calls in
`Sprite::FindCollisionIntersectionPoint_5A2710`. Defining it in CarPhysics_B0.cpp (its address range) restored them
and gave the match. Worth checking in any WIP whose EH state stores are missing.

**EH state at entry counts the destructible locals declared up front.** When the original sets the trylevel to N
on entry, declare N+1 `Fix16_Point` (or other destructible) locals at the top, even one that stays unused, and copy
call results into them (`TryHitchTrailer_442810`, `ProcessPedImpact_560B40`, `SpawnSkidSegment_55D200`).

**`__forceinline` on pool constructors.** In `PedManager::PedManager` (0x470650) VC6 stopped inlining the pool
constructors once out-of-line Fix16 conversions appeared in the function; `__forceinline` on them restored the match.

**A by-reference argument stops tail merging of inlined calls.** In `Ped::FollowTargetStateMachine_46AC20` two
inlined velocity regulators with different arguments were merged into one tail by VC6; passing the argument by
reference (`RegulateVelocityByRef_433970`) kept them separate like the original.

**Normalize out of line from inline depth, not budget.** `ang + k` through `Ang16::operator+` leaves `Normalize` out of
line (operator+ -> ctor -> Normalize is too deep); the ctor form `Ang16(a.rValue + k.rValue, 0)` inlines it
(`Wolfy_30::state_13_14_5411E0`). To force the rotation operators out of line, write `Multiply_408680`/`Negate_4086A0`
calls explicitly and pass `(const Fix16&)` to get the const out-of-line `operator+` 0x408660.

**A shared local in the first test makes VC6 skip the whole chain.** When one local feeds both the first type check
and a later chain of type checks, VC6 jumps past the entire chain when the first test fails. Reading the field
directly in the first check keeps the original jump target (`Car_BC::CanCarCollideWithSprite_43AAF0`).

**`xor eax,eax; mov ax,..; mov al,..` is a 3-byte `memset(arr, 0, 3)`** (`GetMainAndBonusStagesFromSeqFile_4B4440`).

**Default-constructed `Fix16_Point` locals count against the inline budget** (`SpawnCabAndTrailerHelper_408370`).

**Block-scoped locals in different switch cases share a stack slot; function-scope ones get their own.** If the original gives each case's temporary a separate slot, move the declarations to the top of the function. Example: `WindowProc_5E4EE0`.

**Pass an inline's result straight into the call when the original does.** `mov (%eax),%edx; push %edx` right after the inline means `Call(..., *sub_4E4E50(&tmp, ...))`, not a local assigned first (`Ped::ExitCarStateMachine_46C250`).

**`speed * Ang16::sine_40F500(a)` vs `sine(a) * speed`.** With the by-value temporary on the right, VC6 loads both into registers (`mov table,%ecx; mov speed,%eax; imul %ecx`); the other order or `PolarToCartesian` gives `imull mem` (`Particle_4C::PoolUpdate_53D260`).

**Mixed inline/out-of-line operators inside one expression.** When the inline budget runs out mid-expression, some operands of a rotation or length stay inline (`x.Multiply_408680(cos) + y * sin`), so each site may need its own helper variant. The order of the out-of-line calls in the asm shows which operand stayed inline (`EmitFlameStreamSegment_53F4C0`, `SpawnDamageFireEffect_43B870`). Note: `compare_target_asm` can normalise an immediate `0` into a stable name, which makes its ratio unreliable (0x5D0850).

**Identical switch cases are not always cross-jumped.** In `UpdateCircularBurst_state_5_539890` cases 4 and 5 have the same source, but our first copy gets a different schedule and isn't merged into the second as in the original. Operand, statement and case order didn't help. Unexplained.

**Out-of-line copies by address:** `Abs_436A50` is `Fix16::Abs`, `AssignNormalized_409300` is the `Ang16(const s16&, s32)` ctor (9.6f 0x401C60), `sub_53E860` is a COMDAT copy of `Fix16::operator/(const s32&)` emitted by Particle_8.cpp and also called from sprite.cpp (no EXPORT yet, so those divides can't match).

**Hoisting a rotation angle into a local changes evaluation order.** Storing the spread angle in the Ang16 local 9.6f uses gave the original's left-first `(A + ang) - B` (`EmitBloodBurst_53E450`, `EmitWaterSplash_53F060`).

**`if (c) goto ok; return;` gives an inline epilogue copy, `if (!c) return;` jumps to the shared one.** A return that is the fall-through statement after a conditional goto gets its own epilogue; use it when the original repeats `pop/ret` per check (`Garage_48::GaragesService_5349D0`, corner checks).

**Returns without RVO.** Where the original copies a call's result into the return slot (`mov (%eax),%eax; mov %eax,(%ecx)`) instead of building it in place, `return Fix16(x.mValue, 0);` or `Ang16 r = ...; return r;` reproduces it (`ComputeEngineTorque_561970`, `GetNextRotationToward_550F60`).

**`T x = f();` vs `T x; x = f();`.** Initialising constructs the result in the local's slot, so the value stays in memory; assigning goes through a temporary and lets it live in a register (`Type_1_6_416260` speed, `Weapon_30::sub_5DE4F0` angle and steps).

**Use the labelled per-file asm for control flow.** Branch targets in `target_asm.json` are numeric offsets; `asm/<File>.cpp.asm` on the `claude/target-asm` branch has labels and is much easier for rebuilding loops and gotos (`read_input_device_498DA0`).

**Out-of-line Fix16 operators whose `this` is a copy point to a by-value inline helper.** If the out-of-line `Subtract_436A00`/`Add_408660` are called on a stack copy of the variable rather than the variable itself, the original passed it to an inline helper by value (`Fix16_Rect::ComputeShockPrism` in `sub_5DF270`).

**Constant registers in big functions** (ebp = 0 from a pointer local initialised to 0, ebx = 2, edi = 0xF) are often all that is left; they come from the original holding a local or constant across a region (`Ped::Threat_Reaction_AI_465270`, `Particle_4C::PoolUpdate_53D260`). No source form found yet.

**Out-of-line helpers can be mixed within one family.** `Is{North,East}BlockRoadType` call `get_block_452980` while `Is{South,West}BlockRoadType` inline `get_block_42A850` (`PublicTransport_181C::BusesService_579CA0`). Check each call target separately.

**Reading a pointer through a different expression forces a reload.** `field_C_carriages[0]->field_54_driver` instead of a local `pBusCar->field_54_driver` stops VC6 reusing the loaded field (579CA0).

**Values held in registers going into a shared tail were computed per case.** When each case ends with `add %edi,%edx; jmp tail`, do the computation inside each case (`Ped::sub_4645B0`).

**A dominated repeated test folds to a jmp.** `if (t < 0) { ...; if (t < 0) goto X; }` loses the second test; `if (t < 0 && !c) return; if (t >= 0 && c) A else if (t >= 0) C else B` keeps the original's test on the fall-through edge (`Hud_Pager_C::DrawPager_5D2AB0`).

**Freeing inline budget brings inlines back.** Writing the out-of-line calls the original makes (`DivideAssign_539F90`, `Multiply_408680`) let the `Fix16_Point_POD` ctors inline again (`UpdateObjectBeamLink_state_38_538AC0`, `EmitImpactParticles_53FE40`).

**The EH entry state counts every object with a destructor or EH-tracked ctor, used or not.** When only the initial `mov [ebp-4], N` differs, add the missing object (an unused `Fix16_Point` local in `Weapon_30::car_smg_5E2940`). If that shifts registers, move declarations around: there `Ang16` first, the temporaries at function scope and an `Ang16` copy before the first call fixed them. The permuter found the order.

**Large functions can hit the VC6 inline budget.** When normally inlined ctors (`Ang16`, `Fix16_Point_POD`) or a small multiply show up as calls only in one big function, and moving code pushes *other* inlines out of line, the function is at the budget. Free budget by writing the helper body out by hand (`Ang16(...)` and `Normalize_406C20()` in a block scope, `y*y` written out) in `CarPhysics_B0::CalculateRearWheelForce_5620D0` (0.715 to 0.976). See also 538AC0.

**Store and load order follows the source statement order** and inline getters, so try
reordering statements and using the existing inline accessors.

**`memcmp`/`operator==` operand order picks `esi`/`edi`.** For an inlined 16-byte compare
(`repe cmpsl`), the left operand goes in `esi` and the right in `edi`. Swap the sides
of `==` if they are the wrong way round (`NetPlay::InitializeConnection_51E5C0`).

**`a > b ? a : b` on `Fix16` goes through memory.** VC6 picks the address of the larger
operand (`lea ...; jg; lea ...; mov (%eax),%edx`). `Fix16::Max` and `MaxAbsDistance_42A6B0`
do the same. Keep it in registers with `if (!(dx > dy)) { dx = dy; }`, which gives the
original's `cmp %eax,%edx; jg; mov %eax,%edx` (`Police_7B8::sub_56FBD0`, 0.924 -> match).

**A value picked by an `if`/`else` chain vs a ternary chain moves a later sum to another
register.** In `sound_obj::HandlePedVoiceEvent_423080`,
`else { vol = voice == 4 ? 35 : bTank ? 62 : 40; }` instead of two more `else if`s fixed which
register the following `rate + RandomDisplacement(...)` used. The permuter found it.

**One expression can forward a field across a call.**
`field_20 = Get(samp) + Disp(field_14_samp_idx)`, right after `field_14_samp_idx = samp`,
pushes `samp`'s register for `Disp`. Split into
`s32 rate = Get(samp); field_20 = rate + Disp(field_14_samp_idx);` and `field_14` is
reloaded after the call, as in the original (`sound_obj::HandlePedVoiceEvent_423080`).

**Put the zero-initialised accumulator first.** `xor %ebp,%ebp` before two `mov %bl,mem`
stores means the `s32` was declared before the two `u8 = 0`s
(`sound_obj::PoliceRadioMessageGeneration_426790`).

**Field-index locals: try every declaration order.** Four `u16` locals loaded from the same
struct (the junction link indices) gave 0.930, 0.585 or a match depending only on their
declaration order (`RouteFinder::sub_5895C0` needed north, south, east, west; its sibling
`sub_589BB0` needed north, south, west, east). With 4 locals it's 24 builds, so script it.
The same function also needed `RouteFinder_10* pStart = field_861C_nodes;` (used for the memset,
`field_A82C_open_list` and the `field_4` store) for VC6 to reuse the `lea` register, and the
"primary direction" test written as `if (!x) { fallbacks } else { primary }`.

**The order of local saves decides register rotation later on.** When a function saves
some fields to locals before calls, the statement order of those saves can leave the load
schedule the same and still rotate the registers for the rest of the function. Try every
order (`PedGroup::PromoteMemberToLeader_4C9680`: four saves, one order of 24 matched).

**A hidden return pointer means a by-value return.** A class with a constructor (`Fix16_Rect`,
`Ang16`, ...) is returned through a hidden pointer that the caller pushes after the other
arguments. The function then returns that pointer in `eax`, and the caller uses the slot it
passed. So a function that fills a pointer argument and returns it, with `ret $4` for one
"argument", is really `T Func()`. Declare it that way. Build the object in the `return`
statement (`return Fix16_Rect(...)`, adding an inline constructor if needed): VC6 has no
named return value optimisation, so `T t; ...; return t;` adds a copy
(`Car_BC::NoRefs_441600` went from 0.712 with a named local to a match;
`Ang16::SubtractNormalized_409340` with `return Ang16(rValue - toSub.rValue, 0)`). On the caller's side
this also removes the need for a raw buffer to get around a zeroing default constructor
(`sound_obj::HandleTruckCorneringAudio_417FD0`).

**Nested member access instead of a local pointer.** `a2->field_0->field_8->Get()` written
out in full can schedule its loads differently from a `Car_BC* pCar` local
(`sound_obj::HandleAICarEngineSound_418190`: the local loaded the car before a global,
unlike the original).

**Read-then-reset through an inline method.** `s32 d = t.b - t.a; t.init();` written out
lets VC6 hoist the next object's loads above the zero stores. The original had an inline
method (`s32 TakeElapsed() { s32 e = field_4 - field_0; init(); return e; }`), which keeps
each load pair before its own stores (`Mike_A80::sub_4FFA90`, 0.667 to a match). The same
function also needed `wsprintfA` (an import, `calll *0x5FE1BC`) rather than `sprintf`.

**Getters vs direct field reads change register choice.** Reading `p->field_1AC_cam.x`
directly instead of through an inline `get_cam_x()` that returns `Fix16` by value can give
the same instructions with different registers (`this` in `edi` rather than `ebx` in
`PedGroup::UpdateMemberTightFollowState_4CA820`). Try both.

**Bitfield reads through an inline getter.** `if (!p->field_21C_bf.b2)` gives `test $4,%al`.
An inline that returns the bit (`u8 GetBit2() { return field_21C_bf.b2; }`) gives
`mov %eax,%ecx; shr $2,%ecx; test $1,%cl` (`PedGroup::MergeWithOtherGroup_4C9B60`).

**Operand order matters.** `a + b` vs `b + a` changes which value is loaded first and which
register holds the result (`ProjectOntoAxis_5A5AA0`). Writing `x |= f()` instead of
`return f() | x` keeps the result in the first value's register
(`CarPhysics_B0::CheckAndHandleCarAndTrailerCollisions_55EB80`).

**Block-scoped locals share stack slots.** VC6 overlaps the stack slots of locals declared in
different blocks (for example different `case`s). If the original reuses one slot for
unrelated values, declare them inside their own blocks rather than at function scope. If
instead two identical blocks use the same slots, declare the locals once at function
scope (`NetPlay::OnPacketReceived_51F870` for the first, `NetPlay::NetworkTick_51ED00`
for the second).

**Declaration position moves a zero store.** A loop counter declared before an `if` gets its
`= 0` store scheduled before the test, not inside the block (`Kfc_30::CleanupExpiredEntities_5CC1C0`).

**A variable index blocks load hoisting.** VC6 moves a later load above a store with a
constant array index, but not above one through a variable. Inside `case 6:`, writing
`timers[power_up_idx] = 1200` instead of `timers[6] = 1200` keeps the following
`field_2C4_player_ped` load after the store (`Player::CollectPowerUp_564D60`).

**VC6 picks the call order in `a() + b()` itself.** Swapping the operands doesn't change it.
A temporary (`s32 r = b(); r += a();`) forces the order, but the result register can still
differ. See the include order note below too (`sound_obj::sub_412D40` and siblings).

**Include order can change codegen in a TU.** In sound_obj.cpp, the order VC6 emits two calls
in `a() + b()` depended on where `cSampleManager.hpp` was included, and the effect isn't
monotonic. If a function matches when compiled alone (copy it into a small .cpp under
`build_vc6/` and use `build.py --single_cpp ../build_vc6/x.cpp`) but not in its TU, try
moving includes and check `compare_builds.py` for regressions.

**An inline helper changes what else gets inlined.** If the original inlines the first
`Fix16` operator of a formula and calls the out-of-line copies (`Multiply_408680`,
`Add_408660`, ...) for the rest, while yours inlines all of them, the formula was probably
inside an inline helper in the original. Moving it into one (a `static inline` function
or a class inline) made VC6 stop inlining after the first operator in
`RouteFinder::ShowJunctionIds_588620`. The reverse also happens: adding an inline call
before a formula can push the formula's operators out of line (`Trailer::sub_407BD0`).

**Inline functions: often only the first call gets inlined.** When a function calls the same
inline function several times, VC6 often inlines the first call and emits real `call`s for
the rest. Since those calls need a body, an out-of-line copy of the "inline" function is
emitted too. That copy is a function in the original binary as well, often a small `Fix16`
helper with no obvious caller of its own. So a function whose first use of a helper is
expanded and the later ones are calls isn't necessarily written differently: it can be
the same inline method used several times. Before rewriting it by hand, try calling the
existing inline method (or making the helper `inline`). Note that the out-of-line copy
lives in whatever TU emits it, which is also why the duplicate helper copies mentioned
under "Duplicate helper copies" exist.

**Tail merging across `case`s needs a shared statement.** VC6 merges the identical tails of
the two branches of an `if` into one call, but not the tails of two different `case`s. If
the original jumps from one case into the middle of another (`push $1; jmp <other case's
call>`), end the first case with a `goto` to a label in front of the other case's shared
code (`ParseTokenAndPush_430C70`).

**A "point" local that lives half in a register.** If the original keeps one coordinate of a
constant pair in a register and the other in a stack slot, with no EH state for it, the pair
was two plain `Fix16` locals, not a `Fix16_Point` (which has a destructor and adds an EH state
and a slot). `Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60` went from 0.839
(`Fix16_Point`) to 0.924 (`Fix16_Point_POD`) to a match (`Fix16 x, y`), keeping the zeroed
`Fix16_Point point2(0, 0)` that the original also constructs and never uses.

**Stack slot order isn't declaration order.** Two local arrays or a set of scalars can come out
in a different order from the original whatever order they're declared in. If the
original's slots look like one block, try one array. In `FatalError_4A07C0`, six route
coordinates had to be one `s32[6]` with the second triple stored back to front.

**Copy through a local to get a spill.** `mov (%edx),%eax; mov %eax,X(%esp); fildl X(%esp)`
instead of `fildl (%edx)` comes from copying the value into a local object first
(`Fix16 value = *va_arg(va, Fix16*); value.AsFloat()`, `FatalError_4A07C0`).

**Failure path laid out before the success path.** If the original falls through into the
cleanup/failure block after a check and jumps forward to the success code, make the failure
code the body of `if (hr != DP_OK) { failed: ...; return 0; }` and have earlier checks
`goto failed;`. A success label at the end doesn't work: VC6 moves it back up
(`NetPlay::CreateModemAddress_51E2B0`).

**Placing a shared failure block right after a loop.** When the original's loop ends with
`je <loop top>` and falls straight into a `return 0` block that earlier checks also jump
to, with the success code after it, put the label and the return at the end of a
`while (1)` body: `if (!Receive(...)) continue; failed: return 0; }`, with `break` at the
top for the success case. A `failed:` label after the success code, or a
`goto success` from the loop, gets laid out the other way round (`NetPlay::Receive_51F010`).

**Uninitialised locals are "loaded" from argument slots.** An uninitialised local can be
given a stack home that overlaps an argument, so its first use shows up as
`mov N(%esp),%reg` reading that argument. It isn't a real read of the parameter, so
leave the local uninitialised (`NetPlay::sub_521770`).

**A dword read at an odd offset.** `mov 1(%ecx),%ecx` followed by a shift by `cl` is a 32-bit
field at offset 1 of a packed message, not a byte (`Net_4DA9F0` reads the player index as
`*(s32*)((u8*)p + 1)`). Reading it as `p[1]` gives `xor; mov 1(%eax),%cl`. In the same
function, repeating the global (`*(u8*)gpInputBuffer_6F58C0`) instead of a `u8* pMsg` local
put the pointer and the value in the original's registers.

**Struct copies load through a pointer register.** `mov (%edx),%esi; mov (%esi),%ebp; ... mov 4(%esi),%esi`
into consecutive fields is a struct assignment (`entry.inputs = *pData->p`), not two
separate field copies (`NetPlay::Add_5216E0`).

**`memset` position moves register choice.** Where a `memset` of a local sits relative to
other stores decides which register holds the zero and which holds addresses. Try it
before and after the neighbouring field stores (`NetPlay::SendPing_51EF60`: the payload
`memset` goes after the header stores; `Send_521DB0`: the payload is filled in before the
header `memset`).

**A flag VC6 should have optimised away.** If the original zeroes a local, tests it once and
sets it, without VC6 folding any of that, the flag may be a `volatile` alias of a dead
parameter's slot: `volatile BOOL& bDone = *(volatile BOOL*)&lpData;` (`NetPlay::EnumAddress_cb_51E030`,
still WIP for other reasons). Might be worth trying on `RouteFinder::sub_589E20` below.

**VC6 drops tests it can prove.** If the original tests a flag at the top of a loop that is
known to be 0 on entry, VC6 won't reproduce it however you write the loop
(`RouteFinder::sub_589E20`, unsolved).

**Stack slot sharing needs block scopes.** If the target puts two short-lived locals in the same stack
slot, e.g. a `u8` index used once and a later `u32 len = sizeof(x)` read-size, and your frame is 4 bytes
bigger, wrap each one in its own `{ }` block. VC6 only overlaps slots for variables in disjoint scopes; the
original probably had them inside inline helpers. `Frontend::sub_4B4EC0` went 0.867 → 1.0 from this alone.
A `static inline bool` helper does the same job and reads better: `CarAI_78::sub_453C00` matched once its
Ang16 angle test moved into one, so the test's temporaries got their own scope and a later speed temporary
reuses their slot.

**A local can live in a parameter's slot.** Once VC6 has a parameter in a register (here
`hInstance` in `esi`), it may put an address-taken local in that parameter's stack slot
(`lea 0x7C(%esp)` above the return address). If your frame is 4 bytes too small and one `&local`
points at the wrong slot, add the out-parameter local the original had instead of passing one
local twice, and try both declaration orders: one of them goes in the parameter slot
(`WinMain_5E53F0`: `GetDirectXVersion_4C4EC0(&dxVer, &osKind)` with `dxVer` declared first).

**Try the permuter's depth 2 before hand-editing.** Two changes that are each worse alone can match
together (`RouteFinder::NoRefs_589210`: a local's type and the order of two assignments). That's
`Scripts/permute.sh ... -m exhaustive -p <passes> --depth 2`; see docs/permuter.md.

**`T x; x = f();` vs `T x = f();` for a by-value return.** The assignment form returns into a
temporary and keeps the value in a register; the initialiser form has the call write straight into the
local's stack slot (`sound_obj::Tank_414A50`, with a `Fix16`).

**A `volatile` local keeps a flag in its stack slot.** When the original stores a flag to the stack and
reads it back where VC6 would keep it in a register, `volatile bool found = 0;` reproduces that
(`Ped::ExitTrainStateMachine_46D240`, from upstream).

**A by-value max helper instead of `Fix16::Max`.** `Fix16::Max` reads through memory. A file-local
by-value `if (a > b) b = a; return b;` gives the original's register use and keeps the call nesting
(`CarPhysics_B0::ComputeRequiredSweepSteps_55A6A0`).

**`new T()` without an EH state: declare T's constructor `throw()`.** When the original calls T's
constructor out of line with no EH state around `new`, `T() throw();` removes the frame. Write
`T* p = new T(); g = p; if (!p)`: assigning straight to the global let VC6 jump past the store
(`frosty_pasteur_0xC1EA8` ctor 0x512CE0 with `Miss2_25C`).

**Operand order inside a shared inline helper matters, and 9.6f shows it.** `Ang16::PolarToCartesian_41FC20`
computing `sine(angle) * radius` (the 9.6f order) instead of `radius * sine(angle)` fixed
`Car_BC::IsStoppedWithPavementAtDoor_43B140`. The helper has 76 call sites, so run `compare_builds` after such a change.

**A by-value class argument pushed as plain dwords, with an EH frame in the callee.** The caller pushes
the members directly, but the callee still destroys the parameter. That is a class with a destructor
and no user-defined copy constructor: `Fix16_Point`'s inline copy constructor builds the copy in place
instead. `Fix16_Point_ByValue` in `CarPhysics_B0.hpp` is the parameter type of
`ApplyForceAndIntegrate_55F7A0`, which matched its caller `ApplyForceWithTrailerRedirect_55F740`.
A POD parameter matched the caller but lost the callee's EH frame.

**A result flag with one return after the loop.** `result = 0` at the top, `result = 1` on the
found path and one `return result` after the loop fixed every register in
`NetPlay::MovePlayerToGroup_520040`, where returning from inside the loop didn't.

**A by-value return from one local keeps one register across cases.** `Wolfy_30::sub_541680` returns `Fix16`
through the hidden pointer. Assigning one local in each case and `break`ing to a single `return k` keeps
the same register in every case block; a `return` per case alternated `ecx`/`edx`. Leaving the local
unset in `default` reproduces the original reading the argument slot.

**A `const Fix16` picks the out-of-line `operator+`.** On a non-const `Fix16` the inline operator is used;
the original called the exported const one at 0x408660. `Garage_48::ValidateParkCommand_534650` matched with
the unused sum on a `const Fix16` in its own block, so a later `u8` temporary reuses its stack slot.

**`return T(tmp.field)` copies out of a by-value call's temporary.** The original copies from the temporary
atan2 returns into, straight into the hidden return slot; a named local copies from its own slot instead
(`Car_BC::GetCornerAngle_4403A0`: `return Ang16(atan2(...).rValue);`).

**A temporary that only gets an `init` call is raw storage.** `GangPool_CA8::SwapGangSlots_4BF230` calls
only `init_4BED70` on its swap temporary, with no Gang_144 ctor or dtor: a `u8` buffer plus a reference to
it, with the init called explicitly.

**Explicit `Fix16(113)` vs an implicit `113` argument.** In a big function the explicit form is built out of
line into a reused stack temporary and copied; the implicit conversion is built straight in the argument slot.
One call can mix both (`Wolfy_30::state_18_19_20_32_33_542790`: explicit x and y, implicit z).

**Which value you pass can decide the whole function's registers.** Passing the stored field
(`pCar->field_68_scale`) instead of the parameter it was just set from fixed `Car_6C::SpawnCarAt_446230`.
A trivial getter instead of a direct field read does the same (`GetCarInfoIdx_411940()` in
`sound_obj::HandleHeavyVehicleStopSound_417E30`).

**A private copy of an inline that calls the out-of-line Fix16 helpers.** When a function's Fix16 operators are
calls in the original but the shared inline expands them, a file-local copy of the inline written with
`Negate_4086A0`, `Multiply_408680`, `operator+` and `SquareRoot_436A70` matched `Car_BC::ManageDrowning_43E560`.

**Original inline asm.** `sprite_delta::Delta_5ABA00` and `Delta_5ABA40` use `lodsw`/`rep movsb`/`loop`,
which VC6 never emits from C: they are `__asm` blocks.

**`<new>` pulled in through `sprite.hpp` can drop destructor EH frames.** `sprite.hpp` includes
`gbh_graphics.hpp`, which includes GTA2Hax's `DmaVideo.hpp`, which includes `<set>`/`<vector>` and so `<new>`
with its `throw()` `operator delete`. That is why `Door_4D4::dtor_49D570` lacks the original's EH frame, and it
can affect destructors in every TU that includes `sprite.hpp`. `sprite.hpp` only needs `Vert` from it, so
keeping `DmaVideo.hpp` out is the fix to try (it touches many TUs; check compare_builds).

**Arguments are evaluated right to left, so the first one computed is the last parameter.** The diagonal
MapRenderer faces call `atan2_fixed_405320(dy, dx)`: `dx` is computed first. Getting the order wrong also
flips which angle range the face tests (`MapRenderer::sub_4EC450` and siblings).

**`Fix16(f32)` multiplies by `16384.0f`.** The original uses `fmuls` with a float constant; the constructor in
`fix16.hpp` now does too (no matched function changed).

### x87 code: rounding points and kept values decide the schedule

VC6's x87 scheduler is deterministic: it only reorders what the IL gives it, so flags and compiler builds
don't change it (see the MapRenderer note under "Still unexplained"). What changes it is the shape of the
float expression tree:

- **Assigning to an `f32` local adds a rounding node, even if the local is optimised away.** That makes
  the next x87 op one step deeper, so the scheduler fills the gap with integer work. `Set_UV_4F4190`
  only matched with `f32 u = uv.x.mValue; ... = u / 16384.0f;`: `Fix16::ToFloat()` (`mValue / 16384.0f`)
  put `fmuls` before the index load `mov (%eax),%ecx`, the local put it after. An explicit `(f32)` cast
  of the int does not add the node; `(f32)(a * b)` of a product does. Changing `ToFloat()` itself
  breaks `ProjectVertTop_4EAE00`/`Bottom_4EAEA0`, so the original used both forms.
- **`fild x; flds c; fsub %st(1),%st` ... `fstp %st(0)`** (a converted int kept on the x87 stack and
  popped unused later) means the same float value appears twice in the tree and one use was folded by
  CSE. `Mike_A80::sub_4FFD90` matched with `left = 630.0f - fx; right = 630.0f - fx + 1.0f;`: the second
  `630 - fx` becomes the stored `left`, the kept `fx` is popped. `right = left + 1.0f` gives `fsubrs`.
- Operand order of commutative `*` and `+` makes no difference (the compiler canonicalises it), and
  neither do (u32)/(unsigned)/`*(u32*)&` variants of the u32 -> float conversion.

A quick way to test such variants: `Scripts/tu_harness/tu.sh` compiles a preprocessed copy of the TU
(about 3 seconds) and diffs single functions, `score.py` scores a whole TU. Status and next steps for the
MapRenderer cluster, and how to check helpers against 9.6f with VC7: `docs/x87_handoff.md`.

## Functions, thunks and calling conventions

**An EH frame for a member in only one owner's ctor.** If one class's ctor has an EH frame for
a member and the other owners of that member type have none, the member's type has a
destructor only there: give that member a derived type with an empty destructor
(`struct_4_dtor` in `char.hpp`, `Char_B4::ctor_544FF0`). An empty destructor on `struct_4`
itself adds frames to the `Car_BC`, `Object_5C` and `Weapon_8` ctors and dtors.

**A store through a reference keeps a later load after it.** Clearing a bitfield through an
inline helper that takes the bitfield by reference stops VC6 from hoisting the next field load
above the store (`Ped::Deallocate_45EB60`).

**`mov $1,%eax` in the callee but `test %al,%al` in the caller.** That's an `s32` (BOOL-style)
return, cast to `u8` at the call: `if ((u8)sub_405E20(...) || (u8)sub_405E20(...))`. With a
`bool` return, VC6 emits `mov $1,%al` in the callee instead (`sub_405E20`, `sub_405E80`).

**A member that ignores `ecx`.** `frosty_pasteur_0xC1EA8::sub_511A70` is called with
`ecx = gfrosty_pasteur_6F8060`, but reads the global (`mov 0x6F8060,%eax`) instead of `this`.
Write the body against the global. Similarly, `Police_7B8::sub_56FBD0` calls
`gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(...)` and writes `gPolice_7B8_6FEE40->field_65C` while using
`this` for everything else.

**A byte load from a live parameter's slot is an uninitialised local.** In
`sound_obj::HandlePedVoiceEvent_423080`, `mov 0x1C(%esp),%bl` (the `Sound_Params_8*`
argument) on the player path is the `bTank` flag. Only the non-player path assigns it, and VC6
reads the "value" from wherever the variable's home is. Declare it without an initialiser and
assign it on the one path.

**Tail-call thunks.** A tiny original function that is just `mov ...,%ecx; jmp <addr>` or
`if (x) jmp A; else jmp B` means the real code is a separate function the decomp had
inlined. Split the body into its own function at the jump target address and call it:
`Game_0x40::TogglePause_4B9700` calls `Pause_4B96B0`/`Unpause_4B96C0`,
`Car_BC::GetEffectiveDriver_43E990` calls `Trailer::GetTruckCabDriver_407B80`. Add the new
address to the csv (see the top of this file).

**A "member" that doesn't use `this` may be static `__stdcall`.** If the caller overwrites
`ecx` with the argument just before the call (`mov 0x3C(%eax),%ecx; push %ecx; call`) and the
callee ends in `ret $N` without reading `ecx`, declare it `static ... __stdcall`
(`Object_2C::sub_526830`, which fixed its caller `TriggerCarExplosionIfApplicable_526790`).

**A variadic member is `__cdecl` with `this` on the stack.** A plain `ret` hints at `...`.

**Duplicate helper copies.** The original has two identical copies of some small functions. For the `Fix16(int)`
constructor they are really two constructors: `0x4369F0` is `Fix16(s32)` and `0x4926F0` is `Fix16(u32)`, with
identical code. Passing a `u32` where the original calls `0x4926F0` matched `Garox_12E4_sub::DrawPause_5D63B0`; the
same probably applies to `Hud_CarName_4C::DrawCarName_5D4A10`, `DrawBrief_5D3B80`, `DrawPlayerStatsHelper_5D61A0` and
`0x492430`. Check the other duplicate pairs for a type difference before assuming they can't match.

**EH state stores between member destructor calls.** If the original calls several member
destructors in a row without the `movb $N,X(%esp)` state stores between them, VC6 knew
those destructors can't throw. It only knows that if their (empty) bodies come earlier in
the same TU, so define the destructor after them
(`jolly_poitras_0x2BC0::~jolly_poitras_0x2BC0` after `~high_score_table_0xF0`).

**Array construction: inline loop vs `??_H`.** For an array of objects with a constructor,
VC6 calls `vector constructor iterator` (`??_H`) when the constructor is out of line, and
writes an inline loop when the constructor is an inline function whose body it can see
(calling it, or inlining it when it's small enough). `Car_6C::ctor_4469F0` needs the inline
loop for `CarAI_78`, see `match_attempts.md`.

**Inline `memset` of 12 bytes.** `lea X(%esi),%ecx; xor %eax,%eax; mov %eax,(%ecx); mov %eax,4(%ecx);
mov %eax,8(%ecx)` with a second zero register is `memset(arr, 0, sizeof(arr))`, not three
stores (`Player::~Player`).

**An EH frame missing from a destructor:** `<new>` declares `operator delete` as `throw()`,
so don't include C++ std headers from widely used headers.

### Per-file compiler flags are real

Checked 2026-10-01: dropping either per-file flag in `cmake/vc6.cmake` breaks every matched
function in that file.

- **`sharp_bose_0x54.cpp` with `/GX-`.** The ctor builds five `distracted_einstein_0xC`
  members that have real dtors, yet the original ctor has no EH frame. Under `/GX`, VC6 can't
  drop that frame, so the file really was built without exception handling.
- **`gbh_graphics.cpp` with `/Od /ZI`.** The DLL loader there is a debug / edit-and-continue
  build. In contrast, the similar `DMA_Video_LoadDll_5EB970` in dma_video.cpp is optimised.

When a whole file looks unoptimised, or lacks EH frames it should have, suspect a per-file
flag before rewriting the code.

### Use the existing inline helper, not its expansion

Writing out what an inline helper does is not the same as calling it. The inliner counts the
call, and that changes how later `*` / unary `-` calls get inlined and how registers are
allocated. `x' = x*cos + y*sin; y' = -old_x*sin + y*cos` written by hand in the `Particle_4C`
burst functions scored about 0.45. Replacing it with `Fix16_Point::RotateByAngle_40F6B0(angle)`
gave about 0.70 (`UpdateDirectedBurst_state_13_14_36_539480`, `UpdateCircularBurst_state_5_539890`,
`UpdateSkidOrScrapeSpark_state_40_41_53A280`). It is not always better:
`UpdateDirectedProjectile_state_3_12_5384C0` dropped slightly. Before hand-writing maths,
grep `Fix16_Point.hpp`, `fix16.hpp` and `ang16.hpp` for an inline that does it.

### Let the 9.6f version show the structure and the inlines

`docs/inlines_96f.md` lists, per function, the 9.6f calls that 10.5 inlined. The 9.6f code is
not the same compiler and never has to match, but it is often the same source, so it shows
which helper was called and in which order things happened:

- `PoliceCrew_38::sub_571540`: one branch open-coded the despawn check that 9.6f calls as
  `Car_BC::MarkForDespawn_421470`, and every path stored `field_28` before `field_2C`.
- `Firefighter_28::sub_4A7FC0`: 9.6f compares `get_car_velocity_4211C0()`, which is
  `GetLength_41E260`, not the `GetLength_453590` the source used.
- `Garox_2A25_sub::DrawChatMessages_5D16B0`: 9.6f calls the line spacing wrapper (0x4539B0),
  which 10.5 inlines (`GetLineSpacingFromFontType_5D7700_inlined`).

**A getter that returns a copy is not a reference getter.** The 9.6f `Fix16_Rect` getters
(0x45ADA0-0x45ADD0) return a `Fix16` by value. Returning `Fix16&` gave different scheduling of
the four rect reads in `Map_0x370::sub_4E4820`; by value, read in the original order (left,
right, top, bottom), it matched. Check the 9.6f getter's `ret $4` and hidden return pointer.

**A missing EH state store can mean the wrong callee.** `ApplyTurningForce_55F020` lacked the
`movb $1,N(%esp)` before multiplying the `NormalizeSafe_442AD0()` temporary. The source used the
inline `Fix16_Point::operator*(Fix16&)` (called out of line, but its body is visible, so VC6
knows it can't throw); the original calls the exported `Multiply_438FE0`. Check the call target
address against the csv before chasing the state store.

**Trivial getters and setters are free; a by-value `Fix16` setter is not.** Replacing a raw
field access with a one-line inline helper (a scalar or pointer field get/set, `field == K`)
left the code of every matching function unchanged, so those can be used wherever 9.6f calls
them. Adding these helpers made `Object_2C::sub_526B40` match (`Sprite::get_type_416B40`,
`Char_B4::get_velocity_41B080`, `Sprite::set_num_40F7B0`). The exception is
`Char_B4::SetMaxSpeed_433920(Fix16)`: as a by-value `Fix16` parameter it changed six matching
`Ped` functions (`sub_46C770` and others), although 9.6f calls it there. Those keep the plain
assignment for now.

**Adding unused inline methods to a header can still move code in other TUs.** Ten small
`field_A6` bit helpers added to `Car_BC.hpp` (for `CarAI_78::sub_447710`) leave every
`MATCH_FUNC` alone but make five `MapRenderer.cpp` WIPs (`Draw3SidedDiagonal*`,
`Draw4SidedDiagonal*`) 2 to 4 lines worse, by swapping the operands of one `lea`.
`MapRenderer.cpp` gets `Car_BC.hpp` through `Camera.hpp` and never calls the helpers. So when a
WIP's register choice is close, the set of inline functions VC6 has seen in the TU is a
suspect too, not just the ones it uses.

**A by-reference inline helper changes the load order.** When 9.6f calls a helper that takes
its operands by reference (`MaxAbsDistance_42A6B0(Fix16&, ...)`), VC6 10.5 inlines it but still
loads all the operands before computing, where the open-coded form interleaves loads and
subtractions. Returning the result by value adds a temporary copy; writing it through an out
parameter does not. `struct_4::TakeClosestSprite_5A6EA0` matched with a file-local
`MaxAbsDistance_5A6EA0(Fix16& out, Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)`.

**Search loops: put the unlink inside the loop body.** `Car_BC::AttachTrailer_4427A0` matched
once the search-then-unlink was one `for (p = head; p; p = p->mpNext)` loop with the unlink and
the `return` in its body, which gives the original's `pLast = 0` before the null test.

**Even a global's name can change code.** Renaming `word_70643E` to `gChatFont_70643E`, with
every token of `Hud.cpp` and its headers otherwise the same, makes
`Garox_2A25_sub::DrawChatMessages_5D16B0` load the `u16` global with a 32-bit `mov` (`%eax`
instead of `%ax`), so it stops matching. With the old name it matches again. So when a
rename branch is merged, `compare_builds.py` still has to run, and a global can keep its
`word_`/`dword_` name, with a comment, where the new name changes the code.

**Two different callees for the same constructor mean two types.** If the original calls one
`Fix16` constructor twice and you call two, an argument has the wrong type (a `u16` position
that went through `Fix16(u16)` instead of `Fix16(s32)`, `DrawChatMessages_5D16B0`).

### Big functions run out of inline expansions

VC6 stops inlining once a function has made a certain number of inline expansions. The calls
past the limit stay as real calls, for example `call ??GFix16@@QBE?AV0@ABV0@@Z`
(`Fix16::operator-`) where the original has `sub %ecx,%eax`. Which calls lose out isn't source
order, and removing one expansion doesn't always free exactly one: test each change. In
`Map_0x370::sub_4E7190` three `operator-` calls were left out of line until two helper inlines
were written out by hand (score 825 -> 196). If a big function calls an inline that its
smaller sibling inlines fine, count the inline helpers you added that the original may not have
had. Check by grepping the object's relocations for inline member names.

**An inline that writes through a reference keeps the target in a register.**
`p = GetBlock(x, y, z)` with an inline that returns its own local spilled `pBlock` to the stack
in `Map_0x370::sub_4E6660`. `SetBlock(pBlock, x, y, z)`, which assigns `pBlock` in both lookups,
gave the original's `mov %eax,%edi` after each `get_block_4DFE10` and fixed the whole register
allocation (0.56 -> 0.99). The opposite holds for a value that must not be constant-propagated:
`if (d != want) d = 0; if (!d)` folds into one `cmp`, but the original's
`cmp; je; xor; test; jne` comes from an inline that returns `d`.

**Temporaries share slots, named locals don't.** A named `Ang16 back(...)` and a named
`Fix16 found_z` each got their own stack slot, so the frame was 4-8 bytes too big. The original
puts both in slots it reuses, including the dead `dist` parameter's slot. A temporary
(`GetAngleFace_4F78F0(Ang16(...).Normalized_406C20())`, an inline that returns `*this`) and a
block-scoped `{ Fix16 found_z; ... }` gave the original's frame (`Map_0x370::sub_4E6660`).

**An implicit conversion into a by-value argument calls the constructor out of line.**
`Call(98, 179)` to a function taking `Fix16` by value builds each argument in its stack slot
with `mov %esp,%ecx; push $98; call Fix16::FromInt_4369F0` (the `Fix16(s32)` constructor out of
line), while `Call(Fix16(98), Fix16(179))` gets the constructor inlined and the constant folded.
The two spellings gave score 0 against 1123 on the 5 KB `NoRefs_sub_5B1170`. The same goes for
a `u8` passed to a `Fix16` parameter (`FromInt_45C4E0`, the roadblock barriers' z in
`PoliceRoadblock_A4::CreateRoadblock_575FF0`). When the original has `FromInt_...` calls right
before a call, pass the plain value.

**Out-of-line operator copies are functions too.** Functions that run out of inline expansions call
real copies of the `Fix16` inline operators: `operator-` at 0x436A00, `operator/` at 0x436A20,
`<`/`>` at 0x451670/0x451690, `/=`, `*=` and `*(const s32&)` at 0x539F90, 0x562430 and 0x561DB0.
Each is matched as an `EXPORT` member with the operator's body (`Fix16::Subtract_436A00`, ...),
like `Add_408660` and `Multiply_408680`. The `Fix16(s32)` constructor copies (0x41B480, 0x4369F0,
0x4926F0) can't be written that way.

**Parameters reused as working variables.** If the original stores a computed value into a
parameter's stack slot and keeps another parameter in a register for the whole function, the
source probably reassigned the parameters (`x2 -= x1; y2 -= y1;`, then `y2` becomes the y step).
Locals for the same values gave a register rotation (`DrawDebugLine_5D7DD0`).

**Keep VC6 from folding a known zero.** `if (n == 0) { step = n; } else { step = d / n; }`
gets the zero folded, and VC6 then keeps 0 in a register for the other zero tests. The original's
`test`/`mov %ecx,...` came from a small inline returning `count` for a zero count
(`StepFor_5D7DD0`).

Not checked yet: `CarAI_78.cpp` has many `sine_40F500(a) * r` / `cosine_40F520(a) * r` pairs
that may be `FromPolar_41E210` or `Ang16::PolarToCartesian_41FC20`.


Before blaming the budget, check whether the out-of-line calls look written by name. In
`Trailer::sub_407BD0` the rotation's y line calls `Negate_4086A0`, `Multiply_408680` and the
out-of-line `operator+` (0x408660) while the x line is inlined; writing those calls explicitly
kept the rest inlined (0.476 -> 0.843). It matched `Crane_15C::ComputeHookPos_47E620` and
`_47E730`. `GetDoorWorldPos_43B420` has the same shape, and it may help `fire_truck_gun_5E0E70`
and the `EmitBloodBurst`/`EmitWaterSplash` siblings.

**Use one out-of-line callee per operator throughout the function.** Past the budget VC6 calls a
COMDAT copy of `operator*`, which is a different target from the original's `Multiply_408680`.
Mixing the two fails, so every site in the function must go through the named helper: a local
inline `PolarToCartesian` that calls `Multiply_408680`, and `Normalize_406C20`, `Subtract_436A00`,
`Add_408660`, `Negate_4086A0`, `Divide_436A20` instead of the operators
(`Char_B4::ApplyMovement_54CC40`, `HandleGenericCollision_54A530`).

**No jump table and compares in source order mean an if-chain, not a switch.** If the original
tests the value in one else-if chain (14, 9, 3, 6, 2||11, ...) and builds no jump table, write the
chain; a switch reorders the compares (`Frontend::ChangeMenuPage_4B3170`).

**`case 1:` sharing a label with `default:` loses its test.** VC6 drops the compare for a case that
falls into `default`. Give it its own body, even an identical one, to keep it in the compare chain
(`Char_B4::UpdateAnimState_546360`).

**One shared loop counter and flag across separate loops.** Separate variables get separate stack
slots; reusing one `u8 idx` for four loops and one flag for several checks gave the original's frame
(`Kfc_30::UpdateStateMachine_5CBD50`).

**Write switch cases in jump-table order and let VC6 merge the tails.** It merges shared tails
itself, even across different call arguments (`push 1; jmp` into a shared call). Gotos into another
case's block wreck register allocation (`Frontend::UpdatePageFromUserInput_4AE2D0`).

**Test the in-range case first.** `if (v >= lo && v <= hi) mid; else if (v < lo) A; else B;` gives
`cmp lo; jl A; cmp hi; jg B` with `mid` as the fallthrough (`CarAI_78::sub_44AF00`).

**A `dec; jne` countdown loop needs an int counter.** A u8 counter gives a compare instead
(`TrafficLight_20::Init_5C1D00`).

**A u8 min against a constant, written inline.** Two arms that each zero-extend and double with a
byte compare come from `x < 63 ? x : 63` written out; an s32 or u8 `Min` helper gives another shape
(`sound_obj::ProcessActiveQueues_41AB80`).

**`f() ? false : true` gives `test/sete`; `!f()` gives `neg/sbb/inc`.** A flat if chain lets VC6
thread repeated register tests, which can move a case to the end of the function as in the original
(`Orca_2FD4::Internel_CanMoveDiagonally_554110`).

## Inline asm

**16-bit `pushaw`/`popaw`.** The inline assembler can't spell them. Put `_emit 0x66` before
`pushad`/`popad`: the compiler still sees `pushad` and saves `ebx`/`esi`/`edi` as the original
does (`get_rdtsc_5BEE90`). Emitting the whole instruction as bytes loses those saves.


## Still unexplained

These came up more than once and nothing tried so far reproduces them. Notes on what was
tried are in the WIP status report.

**VC6 merges identical tails the original keeps separate.** The reverse of the cross-case tail merging:
in `Frontend::DrawBackground_4B6E10` the two final retry blits share one tail in ours, but the original has
both copies. Only a meaningless cast changed it.

- Identical code merged across `switch` cases, with one case jumping into another's block (`push $2; jmp`)
  where ours duplicates it (`Map_0x370` 0x4E6190 and 0x4E5E90; case order, default, ternaries, if chains and
  `/Os /O1 /Ob0 /Ob2 /Oy- /Gy` didn't help).
- A `u16` field loaded whole and then tested on its high byte (`mov 0x78(%ecx),%cx; test $6,%ch`)
  where we get `testb $6,0x79(%ecx)` (`Car_BC::sub_43B850`).
- A dword load followed by a byte shift (`mov 4(%esp),%eax; shr $7,%al`) (`bk_1::SetAltKeyState_498CB0`).
- What looks like an inlined scalar deleting destructor: the pointer is tested in `ecx` and
  `push %esi; mov %ecx,%esi` happen inside the `if`, where ours keeps the pointer in `esi` from the
  start (0x446DC0, `0x5C5F10`).
- `ebp` pushed only after an early null check (`Hud_Brief_704::ClearAllBriefsWithPriority_5D4890`).
- x87 instruction scheduling around the inlined vertex helpers in the `MapRenderer::Draw*Sided*`
  functions.
  The same `ProjectVertTop_46BD40` y line is now nearly the whole diff of `MapRenderer::sub_4EC450`, `sub_4EC7A0`,
  `sub_4ECAF0`, `sub_4ECE40`, `draw_left_4F3C00` and `sub_4F4600`: the original loads the camera centre y after
  the multiply and orders the u32 high-dword stores differently. Expression order in either line has no effect.
  Solving it could match several functions at once (also the Draw3Sided*/Draw4Sided* functions, 22-42 lines each).
  Details from a focused attempt (8 functions, about 22 rebuilds):
  - The u32 -> float conversion of the camera centre is a lo store, a zero hi store (`mov %ebx,0x1C(%esp)`) and
    `fiaddl`. Ours hoists the hi store as early as byte-level aliasing allows; the original places it differently
    per site (after `lo; fmuls; fmul` when the temp shares a slot with the by-ref x temp in draw_left, between
    `mov 0x74(%eax),%edx` and `fmuls` in sub_4EC450, right before `fiaddl` in Draw4SidedDiagonalUpLeft_4EF880).
  - The original also hoists the next statement's integer code (param loads, `xor %eax,%eax`, global loads,
    pushes of the next inline call) above `fstps vert.y/z`, the uv stores to gTileVerts and stack temp stores.
    Ours never moves a load above a store to a different global or stack slot. It looks like the original
    had more precise alias information for globals and stack slots in this TU.
  - No effect or worse: operand order in Top/Bottom, a local `f32` scale, a local camera pointer, pre-converted
    `f32` centre locals, by-value params, swapped (y, x) params, gTileVerts by index or template index, per-file
    flags /G3-/G6 /Oa /Ow /Os /Ot /Op /Oi- /Oy- /Og-. 9.6f has the same helper shapes; no 9.6f pairs for these.
  - Untested: compiling the Draw* functions with the member `ProjectVertTop_4EAE00`/`Bottom_4EAEA0` inlined instead
    of separate helpers, and whether something in the TU (an address-taken global, a pragma) lowers alias precision.
  - A second focused experiment (17 probes, 303 diff lines in total, draw_left_4F3C00 at 16) found nothing that
    gets closer:
    - Flags: about 40 per-file combinations. /G3 /G4 /G5 /GB give identical code, as do /QIfist /Ob1 /Ob2 /Ox /Ot
      /Oi- /QIfdiv /Zp. /Op /Oa /Ow /G6 /Oy- /Os are much worse.
    - float vs double: `(__int64)`, `(f32)` and `(f32)(f64)` casts of the u32 centre give identical code;
      `(double)(u32)` gives `fildll; faddp` (worse); the sum or product in double is worse; `* (1/16384.0f)` is
      the same as `/ 16384.0f`.
    - 192 variants of the Top helper (operand order, local camera pointer, local scale, by-value or by-ref
      params) and the member `ProjectVertTop_4EAE00`/`Bottom_4EAEA0`, `__forceinline`, const refs: none better.
    - Alias: gTileVerts extern, volatile global pointers, template-index stores: no effect. Dropping the
      `GLOBAL()` registrations (which take the address of every global in the TU) changes only other functions.
  - Third attempt (fast single-TU harness, about 400 compiles):
    - **Not the compiler build.** decomp.me's VC6 packages were compiled against the same TU: RTM (8168),
      SP3 (c1xx 8472, c2 8447), SP4 (c1xx 8867, c2 8799: byte-identical to ours), SP5 (8964/8966) and SP6
      (9782) give the same code for the cluster; the Processor Pack c2 is worse. The scheduler is
      deterministic on its input and doesn't depend on TU-global counters (dummy functions before it).
    - The y-line difference: the original issues `mov 0x74(%eax),%edx; mov %esi,0x1C(%esp)` (centre load
      and the dead zero high dword of the u32 -> float temp) only after `fstps x; fildl y; fmuls; fmul`,
      ours right after the x `fiaddl`. In the original the high-dword store waits for the load, as if it
      depended on it; in ours it is independent. Even the out-of-line `ProjectVertTop_4EAE00` stores it
      before `fstps (%esi)`, so it isn't pointer aliasing.
    - Helper forms that help other functions (scored over all 30 MapRenderer WIPs):
      `pVert->y = (f32)(y * scale) + c` or an `f32` local for the y product: -79 to -88 lines in total, but
      7 functions get worse (`DrawTopSide_4EBA60` +32); a local `Camera_0xBC*`: -94 (4EAF40 -67, 4ED290
      -60, 4EBA60 +32). None fixes the 0.93 cluster, so none is committed.
  - Lead: probably the compiler build rather than the source. The original isn't self-consistent the way a source
    cause would be: the same inlined Top stores the zero hi dword early at one site and late at another with the
    same stack layout (draw_lid_4F4D60), and late in sub_4EC450 but early in draw_left. That looks like a scheduler
    tie-break. In `Set_UV_4F4190` (one `fmuls`/`mov (%eax),%ecx` swap, the smallest case) /G6 flips exactly that
    pair but breaks push order elsewhere. Our toolchain mixes C2.DLL 12.00.8799, C1XX 12.00.8867 and CL 12.00.8804.
    Rich headers (`Scripts/bin_comp/rich_header.py`, dumped by the target-asm CI as `rich.txt`): 10.5.exe's game
    objects are Utc12_CPP build 8799 (102) and 8797 (11), Utc12_C 8797, Linker600 8447, the same builds as
    our build (Utc12_CPP 8799 x110, 8797 x11, Utc12_C 8797, Linker600 8447). So the back end (C2.DLL 8799,
    which stamps @comp.id) and the linker match, and a different c2.dll is ruled out. The Rich header does
    not record the C1XX front-end build (ours is 12.00.8867), so a different front end, which could hand c2
    the IL in a different order, is the remaining compiler-side possibility. Otherwise the cause is in the
    source or TU context (declaration order, what else is in MapRenderer.cpp).
- A compare scheduled before a volatile load instead of after it (`cmp $0xF,%al` in
  `sound_obj::ProcessPoliceRadioWordsPlayback_427220`).
- A store scheduled before the `lea` of an out pointer rather than after it
  (`sound_obj::InterrogateAudioEntities_41A730`). In `Car_14::GetRandomTrafficSpeed_583750` it went away
  once each branch only set lo, hi and the random factor and one shared `*pRet = lo + t*(hi-lo)` ended the
  function; the final sum is still in `ecx` instead of `eax` there.
- A `switch` that clobbers its value (`add $-39,%eax`) and reloads the parameter for
  `default`, where ours uses `lea` into another register (`Object_2C::sub_526830`).
  Also `Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0`. There each case also repeats the whole
  `SetDlgItemTextA` call where we share one tail. 200 permuter compiles found nothing.
- An `s16` parameter returned with a 32-bit `mov` in `default` (`gtx_0x106C::GetSpriteTrueIndex_5AA460`).
- Global load register choice in a run of similar statements (`Camera_0xBC::UpdateBoundaries_435B90`).
- A dead parameter slot given to a different local. In `Car_6C::SpawnCarOnRoadNetwork_4458B0`
  the original puts an unused `u8` out byte in `xpos`'s slot and the y integer in `ypos`'s.
  Ours gives `found_z` the `xpos` slot, which shifts the frame (0x34 vs 0x30). Declaration
  order, passing `(u8*)&xpos` and the permuter didn't help.
- Every inlined angle computation with its own stack slots, so the frame is 0x4C where ours is
  0x14 (`Char_B4::HandlePedCollision_548BD0`). Named locals, an inline helper,
  temporaries bound straight to `atan2`'s references, and function-scope locals for each site
  all either shared the slots or lost elsewhere.
- One register left in `sound_obj::HandlePedVoiceEvent_423080`: `add %eax,%edi` (the sum stays
  in `edi`, stored after `xor %eax,%eax`) where ours does `add %edi,%eax`. Six spellings of the
  sum and 3,000 permuter iterations didn't find it.
- In `sound_obj::ProcessObject_Type12_41E850` the original copies the sample index into `eax`
  for `GetPlayBackRateIdx` (`mov %edi,%eax; push %eax`) and reloads `field_14` for
  `RandomDisplacement`. Every spelling we tried either pushes `edi` directly or swaps the call
  order.
- Error blocks that cross-jump into each other's identical `ret` tail
  (`DMA_Video_LoadDll_5EB970`: the `load_gbh_func` failure blocks). With `/O2` our VC6 keeps
  every block separate. No compiler flag reproduces it:
  - `/O1`, `/Os` and `/Ogs` add an ebp frame and merge every block into one path.
  - The `/O2` variants change nothing (`/Ox`, `/Oy-`, `/Gy`, `/Gf`, `/GX-`, `/Ob0`, `/Zi`, `/Z7`).
  - A debug or edit-and-continue build is ruled out: the target has no frame pointer and keeps
    values in registers, and VC6 rejects `/O2` with `/ZI` (D2016).

  It may be from a library built with another compiler version (the loader macro also appears
  in gbh_graphics.cpp, which builds with `/Od /ZI`).
- An empty `Fix16_Point()` / `Fix16_Point_POD()` default ctor called out of line
  (`??0Fix16_Point_POD@@QAE@XZ`) for locals declared at the top of big functions
  (`Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280`,
  `Particle_4C::UpdateObjectBeamLink_state_38_538AC0`, `sub_5DE910`). The original constructs
  them with no code. Partly explained by an inline budget per function. In a test TU, 11
  `Fix16_Point` locals inline, 12 leave one ctor call, and 14 leave five. It takes both a
  destructor (EH) and `Fix16` members: the same struct with `int` members, or without the
  dtor, never calls. The ctors seem to get whatever budget other inline expansions leave, and
  the first-declared locals lose. In `sub_5DE910`, dropping a `static inline` length helper
  took the calls from 6 to 2, and switching to inline `Fix16_Point` operators raised them
  again. So the original spends less of the budget elsewhere, and it isn't known where.
