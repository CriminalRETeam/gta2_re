# How a near miss gets closed

A checklist distilled from the rounds that worked (and from a long round that didn't). The
reversed compiler rules live in `Scripts/regalloc/README.md`, `Scripts/x87_sched/README.md` and
`Scripts/flowopt/README.md`; this is about which lever to reach for, and when.

## 1. Measure the real diff

`WIP_IMPLEMENTED` compiles to a logging guard, so a WIP's diff against `10.5.exe` is meaningless
until it is gone. Define both guards empty in `Function.hpp` for the duration:

```c
#define NOT_IMPLEMENTED do {} while (0)
#define WIP_IMPLEMENTED do {} while (0)
```

With the guards off, every `WIP_FUNC` can be ranked by post-processed diff lines straight out of
the two exes (`Scripts/bin_comp/gen_target_asm.py` writes `target_asm.json` from `10.5.exe` when
the `claude/target-asm` branch is not reachable). Restore the guards before committing.

## 2. Classify the residue before touching the source

| What the diff shows | What it is | Lever |
|---|---|---|
| instructions the original has and we don't (or the reverse) | source structure | recover the missing statement, local or block |
| our frame is a different size (`sub $N,%esp`) | slot sharing | block scopes, or a missing/extra local |
| same instructions, different registers | register allocation | priority tipping (below) |
| same registers, different order | list scheduling | node count (x87 no-op nodes), or leave it |
| one block where the original has several copies | cross-jumping | see below - it is a regalloc symptom |
| EH state numbers differ (`movl $N,0x..(%esp)`) | objects with destructors | count the by-value temporaries and scoped objects |

Compare call multisets, not call sequences: `compare_callees.py` aligns the two sequences, so a
different call order reads as an insert plus a delete. `compare_callees_multiset.py`, or a direct
count, tells you whether a call is really missing.

## 3. The priorities are not fixed - this is the key

The mistake that wasted a whole round here: assuming that because the emitted code has to stay
the same, the register allocator's inputs are out of reach. They are not.

- A local **assigned in both arms of an `if`** (instead of initialised once and updated) gets an
  extra definition and outranks its competitors.
- A **fail tail written out per branch**, or a label-shared tail written out per case, adds blocks
  for priority counting. VC6 cross-jumps the copies back together *after* allocation, so the code
  is unchanged and only the priorities move.
- The same works for a **constant's** register: more references, or fewer blocks it is only live
  through, and the zero moves from "no register" to `ebx`.
- `Scripts/regalloc/ralog.sh` prints each colour decision (priority, tie, register, scores) and
  `priolog.py` every term that built them, so the gap is a number, not a guess: "width 30 (tie 33)
  against base_xpos 32 (tie 13), width needs +2".
- Equal priorities are broken by the tie, which is a definition counter running **backwards within
  a block**: in the same block the value defined earlier wins, and a value redefined in a later
  block beats one defined only earlier.

## 4. Levers that have landed matches

- **Explicit early `return;`** to choose which of several identical tails cross-jumping keeps.
- **Block scopes** so two locals share a stack slot and the frame size matches.
- **Declaring a local at the top** of the function instead of at first use (and the reverse).
- **A per-site copy of an inline helper** so the sites differ and are not merged.
- **An `f32` local, a parenthesised subexpression or a `(f32)` cast** to add an x87 no-op node;
  removing a no-op `Fix16(x, 0)` construction to remove a round-robin pick.
- **9.6f getters instead of raw field access** (`get_cam_x()`, `get_idx_4219D0()`,
  `get_wanted_points_433DC0()`): the inline changes the live ranges, not just the loads.
- **`u8 bFound = 0; if (...) bFound = Call(); if (bFound)`** gives the original's `cmp %bl,%al`.
- **Combining two levers that are each worse alone.** This is common; do not discard a variant on
  its own diff count without trying it together with the next one.

## 5. What to expect

Cross-jumping differences are usually a *symptom*: our copies are identical where the original's
differ by one register, so ours merge. Blocks merged after allocation still consumed round-robin
picks, which is why a single extra pick early in a function shows up as "every temp from here on
is in the next register". Fix the register, not the merge.

Scheduling-only residue (one `lea`, `push` or copy a slot earlier) is the hardest: priority is
`2*height + 16*mem`, so a load beats an address computation and no source form reorders them. A
window-limit sweep (`regsearch.py`) tells you whether a window break is involved at all - on the
five smallest near misses in this tree the stock limit 80 was already optimal, which rules the
window out and saves a lot of fruitless paren juggling.
