# VC6 cross-jumping (tail merging) tools

When two blocks end in identical code and jump to the same place, VC6 deletes the tail of one and jumps into
the other ("cross-jumping"). Which copy survives decides the layout, so a WIP can have every instruction right
and still differ in which `jmp` goes where. The rules below were reversed from `C2.DLL` (VC6 SP4, 12.00.8804)
with a patched copy that logs the pass, and checked by forcing other orders in the patched compiler
(`--rev`, `--rank`): `Map_0x370::sub_4E6190` then matches exactly.

## The rules

- **When.** FlowOpts (`0x10725453`) runs after register allocation (colour pass and local allocation) and before
  dupB and the scheduler. It's called twice per function (`0x10765c86`, `0x10765cc9`), and each call repeats its
  sweep (`0x10725478`) until nothing changes (at most 17 times). So the tails are compared **in codegen order
  with the final registers, before scheduling**: the source order of the statements, not the order in the
  binary. Two blocks that end up identical after scheduling may not have been identical at cross-jump time,
  and the reverse.
- **Where.** A sweep walks the instructions in layout order. At each label it calls `0x1072f529` for that label's
  predecessor list (`[label+0x1C]`).
- **Candidates.** Only unconditional jumps to the label take part (kind 0x11, not conditional, op != 0x18B). A
  block that falls through into the label doesn't.
- **The order.** The list is in **reverse creation order**. A jump is added at the head of its target's list when
  codegen emits it (`0x10703d77` -> `0x10703cd3`), or when a pass retargets it. Codegen emits in source order,
  with two exceptions:
  - a switch lowered to a `dec/je` chain emits its cases in value order, whatever the source order;
  - when codegen reaches a label whose block is only a jump (an inner `break` right before an outer
    `break`), it retargets the label's jumps to the final target. They are re-added one by one from the head of
    the old list, so they end up ahead of that jump, in source order.
- **The pairing** (`0x1072f529`):
  1. The first candidate in the list (normally the **jump created last, i.e. the last copy in the source**) is
     the target P0.
  2. Each later candidate is compared with P0, backwards from the two jumps, instruction by instruction (labels
     and op 0x1BC are skipped, so the match can run across a fall-through label). This is `0x1073049c`, which
     calls `0x1072f67d` for each instruction pair.
  3. If enough matches (more than 2 bytes, or 20 when `[0x107AC0B4]` is set), the later candidate's matched
     tail is deleted and it jumps into P0 at the start of the match. The length doesn't matter: **the first
     candidate that matches at all wins**, not the longest.
  3a. **Cost check** (`0x10730632`..`0x107306E5`): with `[0x107AC0B4]` set (it is in our builds) the encoded sizes
     (`0x10727887`) of the jumper's matched instructions are summed, first to last, stopping once the sum reaches
     20, plus the larger of the two jumps' `[jmp+0x12]` totals (reset per list, grown on a target by each merge
     into it). The merge is done only if the result is **> 20**. The size estimate treats an operand field
     `[op+0xC]` (a running tuple count on some stores) as a displacement: below 0x80 such a store costs 5 bytes,
     from 0x80 on 8. So adding statements earlier in a function can turn a merge on or off
     (`sound_obj::ProcessOtherObjects_41F520`).
  4. Swap rule: if P0's whole block matched (the instruction before the match is an unconditional jump or a
     return) and the other's didn't, P0 jumps into the other instead.
  5. When P0 has been compared with every later candidate, the next candidate becomes the target, and so on.

What follows in practice:

- Of identical tails, the copy written **last in the source** is kept and the earlier ones jump to it. That's
  the opposite of "keep the first in layout". A chain switch lays its cases out by descending value, so there
  the kept copy is often earlier in the binary.
- To make the original's copy survive, its jump must be created later than the others. Inner `switch`
  case order doesn't change this (value order), but statements that change which jumps exist (a result
  variable and one `return` vs `return` in each case) do.
- To stop a pair merging, make the **last** instruction before the jump differ at cross-jump time. Only
  the unscheduled order counts, so swapping two independent stores in the source can break or create a
  merge without changing the final order.

## Tools

`patch_c2.py` builds `build_vc6/x87_c2/<variant>/` (git-ignored) from the stock variant
(`Scripts/x87_sched/setup.py --variant stock`). The generated code is unchanged unless `--rev`/`--rank` is
given. Lines are source lines relative to the function's first line.

```bash
venv/bin/python3 Scripts/flowopt/patch_c2.py                  # variant cjlog: logs to stderr
X87_C2=cjlog X87_OUT=/tmp/o Scripts/x87_sched/sched.sh -l Source/map_0x370.cpp
grep -a '^@' /tmp/o/last.log | awk '/^@F/{p=index($0,"sub_4E6190")>0} p'
#   @F <function>                      (the cross-jumps of that function follow its @F line)
#   @P join=<label>/0 preds: <jump>/<line>/<kind> ...   predecessor list, head (target) first
#   @J a=<jump> line 44  b=<jump> line 46  pass=1     try to merge a's tail into b
#      <jump>: kind:op ...             the instructions before each jump, last first, back to a label
#   @K jumper=<jump> target=<jump> n=<matched instructions>

# What-if experiments (to find the order the original must have had):
venv/bin/python3 Scripts/flowopt/patch_c2.py --rev cjrev       # reverse every list (--rev 7:20: only lists with 7..20 entries)
venv/bin/python3 Scripts/flowopt/patch_c2.py --rank 5:0,17:1,46:2 rk   # sort lists by these lines' ranks
X87_C2=cjrev X87_OUT=/tmp/r Scripts/x87_sched/sched.sh -l Source/map_0x370.cpp
venv/bin/python3 Scripts/bin_comp/permuter_score.py /tmp/r/last.obj 4e6190 sub_4E6190

# Score every WIP under two variants (what a global change would do):
venv/bin/python3 Scripts/flowopt/sweep.py stock cjrev -j 8

# Check the jump targets too: the score renames forward jmp targets, so a 0 can still jump into the wrong copy
cd Scripts/bin_comp && ../../venv/bin/python3 ../flowopt/jumps.py /tmp/r/last.obj 4e6190 sub_4E6190
```

Instruction kinds seen in the log: 0x0C plain instruction, 0x0E call, 0x11 jump (op 15 `jcc`, 16 `jmp`), 0x1A
label (op 430), 0x13 switch dispatch (op 397). Op names are in `Scripts/x87_sched/optab.txt`.

## Hoisting common code out of both branches (head merging)

The same FlowOpts pass also merges the **heads** of the two successors of a conditional jump (`0x1072F221`, called
for each `jcc` from the sweep's jump handler). It walks the fall-through block and the target block in step,
skipping op 0x1BC and stopping at a label or a mismatch, compares instructions with the cross-jump matcher
(`0x1072F67D`) and checks each one with `0x1073033F` (it must be movable past the `jcc`). The matched run is moved
to **just before the `jcc`** (`0x10702CC8`), and the target's copy is deleted (`0x1072FDDC`). Disabling it (patch
the entry to `xor eax,eax; ret`) leaves the code in both arms.

What follows:

- The hoisted code always lands after the compare or `test` that sets the flags. The scheduler can't lift it
  above the test when they share a register (`mov al,[x]; test al,al` then `mov eax,..` has an `eax` dependency).
- The hoist runs after register allocation, so the allocator saw two copies: two live ranges, double the use
  weights. Writing the code once before the `if` gives one live range and can change the whole allocation.
- In the probes it was the second FlowOpts call (`0x10765CC9`) that did it.

## Memory compares split into a register load (`0x10723BFD`)

After local allocation, pass `0x10723BFD` walks each block backwards keeping the set of live registers (per
block live-out `[blk+0x28]`, minus each instruction's destinations, plus its sources). For an instruction whose
op has flag 0x400 in `0x107A0494` (a compare or test with a memory operand, for example `cmp byte [mem],0`) it
asks `0x10729437` for a free register: first a round robin from a cursor reset per block (`0x107AC2DC`), then
the first register of the list at `0x107ADFF4` (eax ecx edx esi edi ebx ebp) that isn't live. If it finds one,
the operand is loaded into it (`mov al,[mem]; cmp al,0`, later `test al,al`); if not, the compare stays
`cmpb $0,mem`. So the same `if (flag)` gives `cmpb` when every byte register is live at the compare (for example
a value computed just before it and used in both branches), and `mov al; test al` otherwise.

## IL at every pass

`ildump.py` builds a C2 variant that prints each function's instruction list at every pass boundary (`0x107034AB`,
called between the passes of `0x107657E2`), and `ilshow.py` prints the boundaries where it changed:

```bash
venv/bin/python3 Scripts/flowopt/ildump.py
X87_C2=ildump X87_OUT=/tmp/il Scripts/x87_sched/sched.sh -l Source/map_0x370.cpp
venv/bin/python3 Scripts/flowopt/ilshow.py /tmp/il/last.log 10 25 sub_4E8370
#   10765c1a ... mov@19 cmp@19 _jcc/17@19 mov@21 mov@21 lea@22 ...     (after 0x10723BFD: do_drop loaded into al)
#   10765cda ... mov@19 test@19 mov@21 mov@21 lea@22 _jcc/17@19 ...    (after the 2nd FlowOpts: hoisted before the jcc)
```

The first column is the return address of the boundary call, so the pass that just ran is the call before it in
`0x107657E2` (for example `10765c1a` follows `0x10723BFD`, `10765cda` the second FlowOpts). Lines are relative to the
function's first line.

## Duplicating and moving exit blocks (dupB, `0x10726A33`)

After both FlowOpts calls, this pass removes unconditional jumps by moving or copying their target block. It walks
the instruction list twice, looking at each unconditional `jmp L` (kind 0x11, not conditional, op != 0x18B):

1. **Move forward targets** (first loop, handler `0x10726B01`): if the `jmp` is followed by a label, `L` comes
   later in the layout, and the instruction just before `L` is an unconditional `jmp` or a `ret` (so nothing
   falls into `L`), the block from `L` to its own `jmp`/`ret` is moved to just after the `jmp`
   (`0x1072EFA3`), and the `jmp` is deleted.
2. **Move or copy** (second loop, `0x10726BDA`): for the remaining jumps, scan `L`'s block. If it holds no call,
   and the last real instruction before `L` (`0x10703C06`, which stops at a label) is an unconditional `jmp`
   elsewhere, the block is moved after this `jmp` (`0x1070AC7F`). Otherwise it is **copied** after the `jmp` when it
   is small (up to 2, or 20 with `[0x107AC0B4]`, weighted instructions), and the `jmp` is deleted.

So the function's exit block (`pop`s and `ret`) stays where it is, and gets copied to each `jmp exit`, as long as
the last block in the layout **falls into it**. Conditional jumps never move: every `jcc exit` keeps pointing at
the original exit. If the original's `jcc`s go to an exit copy in the middle of the function, with the last
block ending in its own copy, then the exit was moved up there, and nothing fell into it when dupB ran.

How a source gets that (`Ped::PunchChar_467FD0`, verified match): the then-block of an `if (A || B) { X; return; }`
is laid out out of line, after the rest of the function, just before the exit. The last `else`'s `return` then
jumps over it (not a jump to the next label, so the early cleanups at `0x1073C33A`/`0x10706181` keep it). Later
FlowOpts cross-jumps `X` into an identical store elsewhere, which leaves only a label between that `jmp` and the
exit, and dupB moves the exit up. Probes: `if (!a || b == 9) {...}` goes out of line, `if (a == 5) {...}` and
`&&` conditions don't. `TrainCab_414710` and `UpdateState_4FB330` need this (see `docs/match_attempts.md`).

## Block layout: reverse postorder (`0x1070483E`, `0x10734C6A`)

The block order that codegen, FlowOpts and dupB start from is set early, in the third pass of `0x107657E2`
(`0x10706181`, boundary `107659c0` in `ildump.py`), before the loop sinking described below:

1. `0x1070483E` (called from `0x107047BC`, which then drops unreachable blocks) is an iterative depth-first search
   from the entry block. It clears the visited bit (bit 0 of `[blk+0x18]`), keeps the DFS stack in `[blk+0x10]` and
   each block's successor iterator in `[blk+0x14]` (from the edge list `[blk+0x0C]`: next `[e]`, target block
   `[e+0x0C]`), and threads the blocks in **postorder** (the order they finish) through `[blk+0x10]`, head at
   `[hdr+8]`, tail at `[hdr+0x0C]` (`hdr = [func+8]`).
2. `0x10734C6A` empties the block list and walks that postorder list, prepending each block (`0x10733880`), so the
   new order is the **reverse postorder**. The exit block (the old last block) is put back at the end.

Successors are visited in edge list order, which is **the jump target first, then the fall-through**. Checked
by simulating it on the IL before the pass (`@S 10765998`) for every function of `winmain.cpp`: 298 of 305 give
the actual order (the rest are loop sinking and merged empty blocks); fall-through first gives 283. A switch
visits its cases in ascending value order with `default` last, so the cases are laid out `default` first, then
by descending value (`Ambulance_20::UpdateState_4FB330`).

What follows:

- A block is laid out after everything that the DFS reaches from it before its last successor is finished. In
  `if (c) A; else B; C`, the `jcc !c -> B` target B is visited first and finishes first (with C), so the order is
  `A`, `B`, `C`, the usual source order.
- In `if (a || b) { X; return; }`, the first term's `jcc a -> X` makes X the first successor of the first test.
  X only leads to the exit, so it finishes before the rest of the function is even visited, and is laid out
  **after** everything that follows, just before the exit. That's the "out-of-line `||` block"
  (`Ped::PunchChar_467FD0`). Without the `return`, X's DFS walks the code after the `if` first and X stays in
  place; `if (a) X` and `&&` jump past X, so X stays in place too.
- A join reached first through one arm is laid out right after that arm's last block, and before any sibling arm
  visited earlier (the earlier sibling finishes first, so it comes later). For example, a `HandleObjectiveState()`
  join after a switch that the case 3 success path reaches is laid out before case 3's `else` arm, which the DFS
  visited first.

## Code generation order: sinking non-loop blocks out of loops (`0x10740251`)

The local register allocator (`0x10723B05`, round robin over eax/ecx/edx, see `Scripts/regalloc/README.md`) walks the
function's instruction list in order, so "codegen order" is simply the block order at that point. From the pass
dump (`ildump.py`), only one early pass reorders whole blocks: `0x10706181`, which lays the blocks out in reverse
postorder (previous section) and then runs the loop pass (the third pass of
`0x107657E2`, boundary `107659c0`). Everything after it keeps the order until FlowOpts and dupB.

How it works (patching the call to `0x107063B2` out leaves the order as written):

1. `0x107048CA` numbers the blocks in list order (`[blk+0x6C]`, a word; a block created later has -1).
2. `0x107063B2` walks the loop tree, inner loops first, and calls `0x10740251` for each loop L.
3. `0x10740251` walks L's blocks from its first (`[L+0x14]`) to its last (`[L+0x18]`). A block counts as in the
   loop when its loop (`[blk+0x68]`) is L or nested in L (`0x10740084`). Each run of consecutive blocks that are
   not in L (typically an `if (...) { ...; return; }` inside the loop body) is unlinked (`0x10733880`).
4. The run is reinserted after L's last block: walking forward from there, past blocks of L, it goes **before
   the first block whose index is greater than the run's first index** (a block with index -1 uses the index of
   the next numbered block, `0x1074D045`). Otherwise the walk continues.

So sinking keeps the source order among out-of-loop code. An in-loop `return` written before the code after the
loop is generated (and laid out) between the loop latch and that code. Code that comes *earlier* in the source than
the sunk run, but sits after the loop at this point, is passed over: in `for (;;) { if (i >= 40) { tail; return; } ... if (x) { ret2; return; } ... }`
both runs are sunk and `tail` stays first.

`sinklog.py` builds a variant that logs each run, the blocks it is compared with and where it goes:

```bash
venv/bin/python3 Scripts/flowopt/ildump.py && venv/bin/python3 Scripts/flowopt/sinklog.py
X87_C2=sinklog X87_OUT=/tmp/sk Scripts/x87_sched/sched.sh -l Source/Explosion_30.cpp
#   @SINK run a8f8/44..a8f8/44 after 2ed0/43    (ExplosionPool_7A8::FreeLowestPriority_543690: the in-loop return, index 44)
#   @SINKCMP blk 6e7c/48 runidx 44              (the final tail, index 48: 44 < 48)
#   @SINKINS before 6e7c/48                     (so the return is generated before the final tail)
```

Consequence for register-only near misses: with this pass, a block's place in codegen order follows from its place
in the final layout (no later pass moves these blocks, dupB aside). When the layout already matches but the
round-robin rotation doesn't, the difference is the number of picks, not the block order (`docs/match_attempts.md`,
`ExplosionPool_7A8::FreeLowestPriority_543690`).
