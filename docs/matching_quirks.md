# VC6 matching quirks

Things we have run into while matching functions against `10.5.exe` with MSVC 6. Each entry
lists what you see in the diff, what caused it, and the function where it was found, so you
can look at a real example.

Add to this file when you find something new. Keep entries short and point at a function.

## Before you start: things that hide or fake a match

**`WIP_IMPLEMENTED` and `NOT_IMPLEMENTED` add code.** The macros inject a static flag and a
logging `call` into the function body, so a function containing one can never match. Remove
the line (or compile it out locally) before comparing. Several WIP functions were already
matching apart from this, for example `Crane_15C::sub_47F7F0` and
`CarPhysics_B0::DispatchCollision_55CA70`.

**`MATCH_FUNC` is only checked if the address is in `og_function_data_v105.csv`.** Functions
whose address is missing from the csv are silently skipped by `compare_all_functions.py`.
Some of those claimed matches were wrong (`Frontend::sub_4AD0D0`, `Frontend::sub_4ADDE0`).
When you add a function that isn't in the csv (for example one split out of a thunk, see
below), add a row for it: name, address, file offset (address - 0x400000) and size up to the
final `ret`, excluding padding and any jump table after the code.

**The asm normaliser has blind spots.** `post_process_asm.py` replaces absolute addresses so
relocated code compares equal. It used to miss x87 memory operands (`fildl 0x6F633C`),
`and`/`or`/`xor` memory operands, and it crashed on instructions without operands
(`pushaw`). Those are fixed, but if a function looks identical by eye and still fails, check
whether the normaliser handles every instruction in it. `MapRenderer::sub_4EAEA0` carried a
comment blaming "wrong global offsets" for years when it was this.

**The normaliser also hides real differences.** It renames call targets and globals in
order of first use, so calling the wrong function or touching the wrong global can still
pass. Before promoting, check calls and globals resolve to the same things (objdiff does
this). Examples it would not catch on its own: `Player::Hud_Controls_565890` called
`DoBrianTest_42D870` on the wrong global object, and `Car_BC::DetachTrailer_442760` pushed
onto the pool's free list instead of the active list.

## Control flow and layout

**Case bodies are laid out in source order.** If the original's `mov $N,%eax; ret` blocks come
in a different order from yours, reorder the `case` groups to match (`sub_417AC0`,
`sub_417BA0`, `sub_528E00`).

**Merged `case` labels give a byte index table.** The original often has one jump table
entry per case, so write each case out.

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
`if` (`Object_2C::HandleSpriteGroundAndCollisionSimple_523770`).

**`if/else` block order follows the condition.** The `then` block is usually laid out first.
If the original has your `else` block first, invert the condition and swap the blocks
(`Car_BC::sub_440510`, `sub_45CF90`, `Ang16::SnapToAng4_405640`). VC6 sometimes normalises
both spellings to the same code, in which case this won't help (`Ped::ProcessInCarObjective_463FB0`).

**`je tail; jmp next`** for an `if/else` whose branches share a tail comes from a `goto`.

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

(`RouteFinder::sub_589420`, `Hud_Brief_704::sub_5D33F0`.)

## Types and signedness

**`jae`/`jb` vs `jge`/`jl` means unsigned vs signed.** Fix the field or parameter type, not the
comparison (`RouteFinder_10::field_2` is `u16`).

**`test al,al; jbe`** on a byte means `if (x > 0)` with an unsigned `x`, not `if (x)`
(`Cooldown_4236C0`).

**`and $0xFFFF,%eax` vs `movswl`** is a `u16` vs `s16` parameter (`PedManager::DoIanTest_471060`).

**Return width.** `xor al,al`/`mov $1,al` returns a byte; `xor eax,eax`/`mov $1,eax` returns
32 bits. A function whose first path returns another call's `bool` unextended while other
paths set all of `eax` is still unsolved (`Car_BC::sub_43B2B0`).

**Adding a bool.** `setne al; add $0xE,%eax` comes from `(b != 0) + 14`, not `b + 14`
(`sub_417B80`).

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

## Evaluation order and registers

**Store and load order follows the source statement order** and inline getters, so try
reordering statements and using the existing inline accessors.

**`memcmp`/`operator==` operand order picks `esi`/`edi`.** For an inlined 16-byte compare
(`repe cmpsl`), the left operand goes in `esi` and the right in `edi`. Swap the sides
of `==` if they are the wrong way round (`NetPlay::sub_51E5C0`).

**Operand order matters.** `a + b` vs `b + a` changes which value is loaded first and which
register holds the result (`ProjectOntoAxis_5A5AA0`). Writing `x |= f()` instead of
`return f() | x` keeps the result in the first value's register
(`CarPhysics_B0::CheckAndHandleCarAndTrailerCollisions_55EB80`).

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

**Tail merging across `case`s needs a shared statement.** VC6 merges the identical tails of
the two branches of an `if` into one call, but not the tails of two different `case`s. If
the original jumps from one case into the middle of another (`push $1; jmp <other case's
call>`), end the first case with a `goto` to a label in front of the other case's shared
code (`sub_430C70`).

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
(`NetPlay::NoRefs_51E2B0`).

**Struct copies load through a pointer register.** `mov (%edx),%esi; mov (%esi),%ebp; ... mov 4(%esi),%esi`
into consecutive fields is a struct assignment (`entry.inputs = *pData->p`), not two
separate field copies (`NetPlay::Add_5216E0`).

**`memset` position moves register choice.** Where a `memset` of a local sits relative to
other stores decides which register holds the zero and which holds addresses. Try it
before and after the neighbouring field stores (`NetPlay::Send_51EF60`: the payload
`memset` goes after the header stores; `Send_521DB0`: the payload is filled in before the
header `memset`).

**A flag VC6 should have optimised away.** If the original zeroes a local, tests it once and
sets it, without VC6 folding any of that, the flag may be a `volatile` alias of a dead
parameter's slot: `volatile BOOL& bDone = *(volatile BOOL*)&lpData;` (`NetPlay::sub_51E030`,
still WIP for other reasons). Might be worth trying on `RouteFinder::sub_589E20` below.

**VC6 drops tests it can prove.** If the original tests a flag at the top of a loop that is
known to be 0 on entry, VC6 won't reproduce it however you write the loop
(`RouteFinder::sub_589E20`, unsolved).

## Functions, thunks and calling conventions

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

**Duplicate helper copies.** The original has two identical copies of some small functions,
for example the `Fix16(int)` constructor at `0x4369F0` and `0x4926F0`. Our link has one, so a
function that calls the "other" copy can't match (`Hud_CarName_4C::sub_5D4A10`).

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

## Inline asm

**16-bit `pushaw`/`popaw`.** The inline assembler can't spell them. Put `_emit 0x66` before
`pushad`/`popad`: the compiler still sees `pushad` and saves `ebx`/`esi`/`edi` as the original
does (`get_rdtsc_5BEE90`). Emitting the whole instruction as bytes loses those saves.

## Still unexplained

These came up more than once and nothing tried so far reproduces them. Notes on what was
tried are in the WIP status report.

- A `u16` field loaded whole and then tested on its high byte (`mov 0x78(%ecx),%cx; test $6,%ch`)
  where we get `testb $6,0x79(%ecx)` (`Car_BC::sub_43B850`).
- A dword load followed by a byte shift (`mov 4(%esp),%eax; shr $7,%al`) (`bk_1::sub_498CB0`).
- A stack slot reused for a later temporary (`CarAI_78::sub_453C00`).
- `ebp` pushed only after an early null check (`Hud_Brief_704::ClearAllBriefsWithPriority_5D4890`).
- x87 instruction scheduling around the inlined vertex helpers in the `MapRenderer::Draw*Sided*`
  functions.
- A compare scheduled before a volatile load instead of after it (`cmp $0xF,%al` in
  `sound_obj::ProcessPoliceRadioWordsPlayback_427220`).
- A store scheduled before the `lea` of an out pointer rather than after it
  (`sound_obj::InterrogateAudioEntities_41A730`, `Car_14::sub_583750`).
- A `switch` that clobbers its value (`add $-39,%eax`) and reloads the parameter for
  `default`, where ours uses `lea` into another register (`Object_2C::sub_526830`).
- An `s16` parameter returned with a 32-bit `mov` in `default` (`gtx_0x106C::GetSpriteTrueIndex_5AA460`).
- Global load register choice in a run of similar statements (`Camera_0xBC::sub_435B90`).
