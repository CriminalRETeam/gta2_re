# VC6 x87 scheduler tools

When a function with float code differs from the original only in how integer instructions are interleaved
with the x87 ones (a load, a dead `mov %ebx,hi` store or a `push` issued a few slots earlier or later),
the cause is usually not the expression order but where VC6's scheduler cut the basic block into
**windows**. The rule below was reversed from `C2.DLL` (VC6 SP4, 12.00.8804) with a patched copy that logs
the scheduler, and checked by changing the window limit: 8 MapRenderer functions then match exactly.

## The rule

The list scheduler is at C2 `0x1072a20f`, called once per function.

- **The x87 order is fixed before scheduling.** Code generation emits the x87 instructions in expression
  tree order (Sethi-Ullman style). The scheduler only moves integer instructions around them.
- **Windows.** It works per basic block, in windows of at most **81 instruction nodes**
  (`cmp $0x50,%esi; jg` at `0x1072a7ba`, window built at `0x1072a7a8`). A window ends at a label or a
  branch, not at a call. Nothing moves across a window boundary.
- **Priority** is the latency-weighted height of the node (the longest path to the end of the window)
  `<< 13`, plus flag bits. The ready list is sorted by priority, highest first; ties keep IL order. An x87
  instruction issues alone; two integer instructions can pair in one cycle.
- **The exact formula** (`0x1072B71C`; terms combined by `0x1072C018`, a shift by a signed weight from the
  per-CPU table at `0x107A1EF0`, row picked at `0x1072A402`):
  `priority = height << 13 + mem << 16 + ([node+0x22] >> 5)` (+ one x87-only bit), with
  `height = 1 + max over successors (height + edge latency)` and `mem` = any operand is a memory
  reference (`0x1072AC9F`: operand kinds 2 and 6), so loads, stores' address reads and calls.
  `sv.py` prints `priority >> 12`, i.e. `2*height + 16*mem`: in `GetLayout_4D6000` the byte load
  `mov dl,[pwszKLID+6]` is `2*15 + 16 = 46`, `lea ecx,&v2` `2*14 = 28`.
- **Calls don't pair**: nothing issues in a call's cycle (0 of 2705 pairs in `sound_obj.cpp`, bar a few
  `imul`).
- **No-op nodes count.** Op 354 (0x162) emits nothing but takes a node in the window and a cycle on the
  x87 dependency chain. The front end creates one for each parenthesised float subexpression, each
  `(f32)` cast of a float expression and each store to an `f32` local (even an optimised-away one).
  `((a * b))` gives two. `Fix16::ToFloat()`'s `(mValue / 16384.0f)` gives one per call that isn't CSE'd.
  The parentheses also stop reassociation. VC7 (9.6f) ignores them, so a 9.6f check can't tell the
  paren forms apart.

What follows: where the 81st node falls decides what can move where. If the original had a few more no-op
nodes earlier in a long straight block, its window broke earlier, and an integer instruction that ours
hoists to the top of a window waits for its x87 neighbour in the original (or the reverse). In
MapRenderer the original had about 9-11 more no-op nodes per window than our source.

## Tools

One-time: `python3 Scripts/x87_sched/setup.py` writes the patched compilers to `build_vc6/x87_c2/`
(git-ignored; `sched.sh` runs it on demand). It applies `c2_patch.json` to the submodule's `C2.DLL` after
checking its sha256. There are two variants: `log` logs the scheduler to stderr, `fast` doesn't. Both have
a window-limit table, which is stock (80) unless `LIM` is set. With the stock table, both give the same
code as stock C2 (checked on all of MapRenderer.cpp).

`VC6_TOOLS` points the scripts at another `gta2_re_compile_tools` checkout. The default is this repo's
`3rdParty/gta2_re_compile_tools`. For a git worktree without submodules, use
`VC6_TOOLS=/path/to/main/checkout/3rdParty/gta2_re_compile_tools`. The `3rdParty/GTA2Hax` headers are then
also taken from that checkout.

```bash
# Windows and schedule of one function (a stripped copy of the TU is compiled; Source/ isn't touched)
Scripts/x87_sched/sched.sh Source/MapRenderer.cpp draw_left_4F3C00       # -r: also the ready lists
#   #238 void __thiscall MapRenderer::draw_left_4F3C00(...)     <- #238: ordinal for LIM
#      window 2   n=81  nop=5  FULL at [23, 33, 43, 67, 77]     <- cut by the limit; positions of no-op nodes
#   ---------- window 2: 81 nodes, 5 nop
#    25 s30  p 210.0  e25  fiadd DWORD PTR -20+[esp+44]  -> s50/0 ...
#   cycle, seq (IL order), priority, earliest cycle, instruction, successors seq/latency

# What-if: a different window limit (a window holds limit + 1 nodes)
LIM=69 Scripts/x87_sched/sched.sh Source/MapRenderer.cpp 4EEE60           # every function
LIM=238:71,71 Scripts/x87_sched/sched.sh Source/MapRenderer.cpp 4F3C00    # function #238, per window

# Search the limits against the 10.5 target (needs Scripts/bin_comp/target_asm.json and the venv)
venv/bin/python3 Scripts/x87_sched/regsearch.py Source/MapRenderer.cpp draw_left_4F3C00 0x4f3c00 -j 8
#   stock limit 80: 8 differing lines ... all windows 71: 0
#   best 0 with LIM=238:71,...;  extra nodes before each window break (80 - limit): [9, ...]
venv/bin/python3 Scripts/x87_sched/regsearch.py Source/MapRenderer.cpp --tu --range 68:71 -j 4
#   limit 80: 1246 lines ...;  limit 69: 983 lines, zero: 4EEE60 4EF880, MATCH changed: 4E9DB0

# Node counts of an existing log: windows, no-op nodes and their positions
python3 Scripts/x87_sched/nodes.py build_vc6/x87_c2/out/last.log 4F3C00
```

`sched.sh -q` compiles with the `fast` variant and prints nothing (`build_vc6/x87_c2/out/last.obj`, or
`$X87_OUT`). `Scripts/tu_harness/diff.py` diffs that object against the target.

The window limit is a diagnostic, not a fix: the original was built with the stock limit. A best limit
`L < 80` means the original had about `80 - L` more nodes before that window's break. Find them in the
source: missing parentheses around float subexpressions, an `(f32)` cast, an `f32` local, a `ToFloat()`
that the original wrote out (or the reverse when `L > 80`). `nodes.py` counts the no-op nodes, so check
the count after each change.

## Workflow

1. The function differs only in integer/x87 interleaving, from some point in a long straight block on.
2. `sched.sh` it. Find the window that holds the differing instructions and check whether it is `FULL`.
3. `regsearch.py` the function. If a limit makes it match (or nearly), the window break is the cause. The
   difference to 80 is how many nodes ours is short (or over) before that break.
4. Add the missing parentheses/casts/locals where the original most plausibly had them. Prefer forms that
   also explain sibling functions (inline helpers shared by a cluster). Recount with `sched.sh` and
   verify with `Scripts/tu_harness/tu.sh`, then a full `build.py` and `compare_builds.py`.

## Changing the hooks

`patch_c2.py` regenerates `c2_patch.json` (needs `pip install pefile keystone-engine`). The hooks: the
scheduler entry (`@F`, function ordinal), the window start (`@L` node list, picks the limit), the window
size check, the ready list (`@R`), each scheduled node (`@S`: cycle, priority, earliest cycle, IL seq,
successors with latency) and instruction emission (`@E`, used to align with the `/FAs` listing). The
opcode names in `optab.txt` are C2's own table (op numbers below 270, read from `0x107a5a98`).

## Coverage diffing (DynamoRIO)

The scheduler was found by diffing which C2 basic blocks two source variants execute. `drc/` has the
client sources: `bbc.c` (basic-block hit counts), `ct.c` (call/return trace) and `ww.c` (writes to given
addresses, with a C2 call stack). To build them, unpack a DynamoRIO release (10.0.0 was used) and run
`DYNAMORIO=/path/to/DynamoRIO-Linux-10.0.0 Scripts/x87_sched/drc/build.sh`. This needs gcc with `-m32`.
Without the 32-bit libc headers it falls back to the 64-bit ones and an empty `gnu/stubs-32.h`.

```bash
export DYNAMORIO=/path/to/DynamoRIO-Linux-10.0.0
Scripts/x87_sched/drc/dr.sh bbc /tmp/a.txt variant_a.cpp      # "addr hits" per C2 block, stock C2.DLL
Scripts/x87_sched/drc/dr.sh bbc /tmp/b.txt variant_b.cpp
diff /tmp/a.txt /tmp/b.txt                                     # blocks only one variant takes / hit counts
Scripts/x87_sched/drc/dr.sh ct /tmp/ct.txt probe.cpp           # C call target / R ret retaddr / X external
Scripts/x87_sched/drc/dr.sh ww /tmp/ww.txt 1079f238 probe.cpp  # who writes the scheduler's cycle counter
```

Keep the probes small: a one-function `.cpp` (Source/ is on the include path) runs in about a second under
DynamoRIO.

## Troubleshooting

`sched.sh` can hang when wine starts a fresh `winedevice` that inherits its `| tr -d '\r'` pipe. Kill the
`tr` process to unblock it, or start a persistent wine server first (`wineserver -p`) to avoid it.
