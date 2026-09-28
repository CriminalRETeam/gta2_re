# Match attempts

A log of functions that were worked on and still don't match, with what was tried.
Read the entry for a function before working on it, so the same ideas aren't tried
again. General VC6 patterns belong in `matching_quirks.md`; this file is per function.

For each entry: the closest the function got, what the remaining diff looks like, and
each thing that was tried and what it changed. When a function finally matches, delete
its entry here and add the trick that did it to `matching_quirks.md`.

Target asm comes from the `claude/target-asm` branch (`compare_target_asm.py`, see
`CLAUDE.md`).

## BurgerKing_67F8B0::modify_inputs_4CDF30 (WIP)

Closest: ratio 0.714. The code is right apart from register allocation: the original
keeps `match_mask` in `ebx` (loaded before `push %esi`) and the loop counter in `edi`,
ours swaps them. The original also computes the tail as `mov %ebx,%eax; and $0xFFFFF000,%eax`
after the loop.

Tried, none moved the registers:
- `if ((match_mask & 0xFFFFF000) != 0) field_4 |= match_mask & 0xFFFFF000;` (tests then
  ands again, worse).
- A local `s32 high_bits = match_mask & 0xFFFFF000;` (best so far), `const u32`, and
  `match_mask &= 0xFFFFF000;` in place.
- `u32 match_mask` parameter.
- Loading `field_8_input_masks[i]` into a local once per iteration (worse: changes the
  `test` operand order and duplicates the store).
- Walking a pointer with a count-down `for (i = 12; i != 0; i--)` (same as the local).

## NetPlay::RemovePlayerByName_520F80 (WIP)

Closest: ratio 0.788 with a `while (1)` loop (`if (i >= count) break;`, as in the matched
`IndexOf_520E30`) and an `s32 bRemoved` result. The original spills the result to a stack
slot (`push %ecx` in the prologue, `mov %esi,0x10(%esp)` to store the 0, `mov 0x10(%esp),%eax`
on the not-found return) and returns `ebx = 1` on the found path. Ours keeps both returns
as constants.

Tried:
- `for` loop with `return 1` / `return 0` (0.645, the loop gets rotated).
- `for` loop with `bRemoved = 1; break;` (0.624).
- `while (1)` with `bRemoved = 1; break;` (0.788, best).
- `while (1)` with `bRemoved = 1; return bRemoved;` in the loop (0.740).

Not tried yet: an inline helper for the delete that returns 1, so the outer result is a
separate variable.

## Car_6C::ctor_4469F0 (STUB)

Ratio 0.589. Everything up to the `CarAI_78_Pool` allocation matches. The rest differs
because of how two pool arrays are constructed:

- `CarAI_78_Pool` (0x131 x 0x78): the original builds the array with an inline loop
  (`movl $0x131,0x10(%esp)` / `call ??0CarAI_78` / `add $0x78,%edi` / `dec`), ours calls
  `vector constructor iterator` (`??_H`). VC6 uses the inline loop when the element
  constructor is an inline function whose body is visible but doesn't get inlined; for an
  out-of-line constructor it calls `??_H`. So in the original `CarAI_78::CarAI_78` was
  defined inline in the header and emitted out of line (at 0x453CB0).
- `TrailerPool` (10 x 0x10): the original inlines `Trailer::Trailer` into the loop, ours
  calls the out-of-line copy. `__forceinline` on `Trailer()` fixes this part (and the
  frame size and EH state numbers follow), removing the stray `0;` statement doesn't.

Tried:
- `EXPORT inline CarAI_78();` in the header with the body still in CarAI_78.cpp: Car_BC.cpp
  still calls `??_H` (the body must be visible), and the link fails because CarAI_78.cpp
  no longer emits the constructor.

Blocker: moving the `CarAI_78` constructor body into the header would give the inline
loop, but its `MATCH_FUNC(0x453cb0)` marker has to sit in front of the definition in a
.cpp, so that function would stop being verified. Needs a way to mark inline functions
defined in headers.

## Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0 (STUB)

The existing body is right. The original has a full copy of the
`GetString_519A00` + `SetDlgItemTextA` + epilogue sequence in every case, lays the cases
out 2, 1, 0, default, and reloads `game_speed` from the stack (`mov 0xC(%esp),%eax`) for
the default, although it is still in `esi`. Ours tail-merges the cases into one call and
puts the default first.

Tried, all still tail-merged:
- `switch` with a `SetDlgItemTextA` per case and `break` (the existing body).
- One `pText` local set in each case and a single `SetDlgItemTextA` after the switch.
- `return` in each case with the default call after the switch.

Ideas not tried: `game_speed` as a by-value struct or a different type (to explain the
reload), or the function being a static `__stdcall` (it never reads `ecx`).

## frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0 (WIP)

Ratio 0.732 with an `s32 str_count` (was `u16`, 0.497): the original keeps the count in a
32-bit stack slot and strength-reduces `field_4[str_count]` into an offset register
(`mov $4,%ebp` ... `mov %esi,-4(%ecx,%ebp)`).

Still different:
- The first (length) loop also computes `(len + 9) & ~1` into `edi` in the original,
  a value that is never used. Nothing in our source produces it.
- The `if (tableSize)` test before the second loop reads the zero-extended size back from
  its stack slot (`mov 0x10(%esp),%ecx; test %ecx,%ecx; jbe`), ours tests `bx`.
- The empty-table branch stores `ax` (0 from the memset) rather than an immediate.

Tried:
- A `u32 table_size = tableSize;` local used by both loops and the `if`: worse, the
  register assignment shifts.
- Declaring `str_count` (and the total) before the `if` and storing `str_count` in the
  else branch: worse, `str_count` stops being kept in the 32-bit slot.

## NetPlay::SendKeepAlive_521D20 (WIP)

Ratio 0.690. Same shape as the matched `Send_521DB0`, `Send_521E40` and
`NoRefs_Send_521C80`: `memset` a `Packet_SubType_3`, fill it in, `MakeSendData_51F420`, then
`IDirectPlay3::Send`. The payload is a single byte, 2. The original keeps the 2 in `ecx` for
both the `keep_alive` byte and `field_4_sub_type`, and does all the stores before the
`lea`s and pushes for `MakeSendData_51F420`. Ours computes the `lea`s and pushes early and
stores the 2 through `eax` after the first push.

Tried (0.690 unless noted):
- `keep_alive = 2` before or after `field_4_sub_type = 2`, both after the `memset`.
- `char_type keep_alive = 2;` declared after the `memset`.
- `keep_alive` declared before the other locals, assigned after the `memset`.
- `keep_alive = 2` before the `memset` (0.357, the 2 goes into `eax` and the zero into
  `ecx`).
- `char_type keep_alive[1]` set before the `memset` (0.357).

What fixed the similar `Send_521DB0` was declaring the payload struct first and filling it
in before the `memset`. That doesn't work for a plain `char_type`. What fixed `Send_51EF60`
was putting the payload's `memset` after the header stores. The equivalent here,
`keep_alive = 2` after the `field_D` store, gives 0.619.

## NetPlay::sub_520040 (WIP)

Ratio 0.692. Moves player `toFind` from `pStru` to `pDst` through `sub_51E9C0` and
`sub_5201A0`. Branch layout matches with the nested form (the call inside the loop's found
branch, `break` on failure, one `return 0` at the end). The registers don't: the original
has `this` in `ebx`, `pStru` in `ebp` and the result 1 in `edi` across the `sub_5201A0`
call (`mov $1,%edi` ... `mov %edi,%eax`). Ours has `this` in `edi`, `pStru` in `ebx`, and
`mov $1,%eax` after the call.

Tried:
- Early `return 0`s with the call after the loop (the failure blocks get laid out
  inline).
- `s32 bMoved = 1;` before the `sub_5201A0` call and `return bMoved;` (no change), and
  the same with the call after the loop (0.615).

## NetPlay::sub_51E030 (WIP)

The `EnumAddress` callback. For `DPAID_INet` it walks the double-null-terminated ANSI
address list, widens each entry with `MultiByteToWideChar` (IAT 0x5FE054; 0x5FE058 is
`lstrlenA`) and passes it to `PushConnection_51E0E0`.

The original keeps a "done" flag in `lpData`'s stack slot. It zeroes the flag at entry,
tests it once before the loop and sets it when the list ends, but the loop condition is
a second `lstrlenA` call. VC6 normally optimises such a flag away completely. What
reproduces it: `volatile BOOL& bDone = *(volatile BOOL*)&lpData;` plus
`if (!bDone) do { ... } while (lstrlenA(pAddress));`. This may also help with the
"flag tested at the top of a loop" entry in `matching_quirks.md`
(`RouteFinder::sub_589E20`).

Still different: the original pushes `ebp` in the prologue and loads `lpContext` into it
before the flag test. Ours delays `push %ebp` until after the flag test. `lstrlenA` and the
new buffer also use `esi`/`edi` the other way round. Tried: moving
`NetPlay* pThis = (NetPlay*)lpContext;` to the top of the function (no change).

Tried before the volatile trick (the flag gets optimised away in all of these):
- `while (!bDone) { ...; if (!lstrlenA(p)) bDone = TRUE; }` with a plain local.
- `if (!bDone) do { ... } while (lstrlenA(p));` with a plain local.
- Reusing `lpData` itself as the flag (`lpData = NULL;` ... `lpData = (LPCVOID)1;`).
