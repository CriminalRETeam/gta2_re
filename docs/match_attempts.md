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
