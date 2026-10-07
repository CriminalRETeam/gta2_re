# VC6 register allocation (C2.DLL), work in progress

Goal: the rule VC6 (`C2.DLL`, SP4 12.00.8804) uses to give values registers, so near misses
that differ only in register choice (see "Still unexplained" in `docs/matching_quirks.md`) can be
predicted instead of permuted. Same approach as `Scripts/inline_budget/`: find the code, add
logging hooks with a byte patch, then write a model that reproduces the log.

Status: the colour (select) step, the priority and the tie-break are reversed and logged; see "Priority and
tie-break (reversed)".

## The rule (colour step)

VC6's global allocator is a priority-based colouring (Chow/Hennessy style) in `color.c`:

- Live ranges sit in a list at `0x1079D864`, sorted by priority `[lr+0x0C]` descending, then by a
  tie-break `[lr+0x40]` descending (insert: `0x1072215B`). The colour pass (`0x1071ACDF`) pops
  them in that order. A live range that can't be coloured goes to the splitter (`0x10722650`) and
  comes back as smaller pieces.
- The select function `0x107233B6(lr, class)` gives each register r a score, starting at 0:
  - minus the weight of each of the live range's own preferences for r (`[lr+0x34]` list);
  - plus the weight of each preference for r of every interfering live range not yet coloured;
  - plus 100 x priority for an interfering live range that can only use r.
- It then scans the class's register order (`0x107A0A0C[class]`; class 0 is `0x107A09E8`:
  **eax, ecx, edx, esi, edi, ebx, ebp**) and takes the allowed register (bit set in `[lr+0x20]`) with
  the lowest score. The compare is a strict `<`, so the earlier register in the order wins a tie.
  The result goes to `[lr+0x10]` (pointer into the per-register table at `0x107AC730 + reg*0x54`).

Checked by patching the order table (esi and edi swapped): every variable that had esi got edi and
the other way round.

C2 register numbers: 0 none, 1 eax, 2 ecx, 3 edx, 4 ebx, 5 esp, 6 ebp, 7 esi, 8 edi (name table
`0x107A91A4`, then ax.., al..).

## The rule (local temps, code generator)

Values the colour pass leaves alone (expression temps inside a block) get registers in the next
pass (`0x10723B05`), which walks the instruction list in IL order (`0x1072830C` -> `0x1072EB08`
per value; `0x1072EE00` records the choice in `0x1079D6EC[reg]`):

1. the value's preferred register `[x+0x2C]` (a copy hint) if it is free;
2. otherwise **round robin over eax, ecx, edx**: a cursor (`0x1079D710`) into the list at
   `0x107ADFF4` (eax ecx edx esi edi ebx ebp) takes the next free register that doesn't conflict
   and moves past it; it wraps after edx. The cursor is reset to eax once per function
   (`0x10723B6F`), not per block;
3. if eax, ecx and edx are all busy, the first free register in the whole list (so esi, edi, ebx,
   ebp get used for temps only then).

Per instruction (`0x10723B05` loop): first the registers of source temps that die at it are freed
(`0x10728271`), then each destination is placed (`0x1072830C`): a destination operand that already
has a register (the optimizer merged it with a dying source, e.g. `mov 4(%eax),%eax`) keeps it without
a pick; otherwise steps 1-3 above. A dying source therefore counts as free for the round robin of
its own instruction.

Which values are colour-pass live ranges (`@R k=` gives the kind: `0x0A0804`/`0x020A04` named
variables, `3`/`0x103` optimizer temps, `0x100D` constants): a variable's definition gets a live range
only if its uses reach another basic block; a variable defined and used within one block is a plain
temp (probe: `o = ped->f184; rot = o->f4->ang;` has no live range for `o`, with
`if (k) rot = o->f4->ang; else rot = o->a;` it gets one). Optimizer temps are coloured even when
block-local.

Blocks merged after allocation (identical early-return blocks, tail merging) still made their picks:
a block that disappears from the final asm can move the cursor.

So a temp's register depends on how many round-robin picks came before it in the function, in
code generation order. A run of temps after a call rotates eax -> ecx -> edx; a shifted rotation
in one block (the original's eax/ecx/edx where ours has ecx/edx/eax) means one more or one fewer
pick earlier, or blocks generated in another order. Code generation order is C2's block order at
that point, which source order does not set directly (a `goto` to a block written last still
generates it first).

## Using it

```bash
python3 Scripts/regalloc/patch_c2.py              # once: build_vc6/x87_c2/ralog (needs the stock variant)
Scripts/regalloc/ralog.sh Source/sound_obj.cpp HandleCarTireScrubSound
```

prints each colour decision in order (priority, tie-break, weight, register, scores). The raw
log (`build_vc6/x87_c2/ralog_out/last.log`) also has one `@L` line per local pick: register, path
(`via=1072ebd5` round robin, `1072ec84` preference, others the fallback), cursor and an
approximate source line (offset by a constant per function). When two
values have the wrong registers, see whether they have equal priority (then the tie-break order
decides, see below) or whether a score (a preference) decides.

What decides the tie-break, from probes (`probe_loop.cpp`): in a loop, the live range updated first in
the loop body gets the higher tie-break and is coloured first. Reordering independent statements
in the loop rotates the registers of equally weighted variables. The log for `probe_loop.cpp`:

```
  prio=40  tie=17  -> edx    a  (a += p[i] first in the loop)
  prio=40  tie=15  -> esi    b
  prio=40  tie=14  -> edi    c
```

## Stack slots (frame pass `0x10723F8C`)

Stack locals (address-taken variables, `$T` temporaries, spilled variables) and the parameters get their frame
offsets in the pass after local register allocation and the memory-compare split (`0x10723F8C` -> `0x10724009`):

1. **The list.** `0x10724286` walks every instruction in IL order and calls `0x1072EE45` for each stack symbol
   operand. That keeps a list (`0x1079F220`; records with the symbol at `[+0]`, next `[+0x2C]`, size `[+0x20]`,
   count `[+0x34]`) sorted by
   - **size ascending**,
   - then **reference count descending**: each operand adds 1, with no loop weighting. The list is built after
     register allocation, so only memory references count;
   - then **first reference** (a symbol only moves ahead of symbols with a strictly smaller count).
2. **Packing.** The list is walked in that order. A local takes the first slot, in creation order (a parameter's
   own slot included), whose members don't overlap it, if its size is at most **twice the slot's current size**
   (the slot then grows to it). Otherwise it gets a new slot. Lifetimes for this are lexical: an
   address-taken local overlaps everything in its scope (two function-scope locals never share, even when
   their uses don't overlap). A parameter's slot is free after the parameter's last read, which is usually its
   load into a register.
3. **Offsets.** New slots go from the bottom of the frame up, in creation order, so the list's front gets the
   lowest addresses (`[esp+0]`). `0x1075F8E9` then rebases them against the frame base symbol (`0x10799020`).

Probes (`int` unless noted; `{}` is a separate scope):

| Source | Slots, lowest first |
|---|---|
| `int a, b, c; ext(&c); ext(&a); ext(&b);` | c, a, b (declaration order is ignored) |
| `ext2(&a, &b, &c)` | c, b, a (the pushes are evaluated right to left) |
| `ext(&b); ext(&a); ext(&a); ext(&a); ext(&a);` | a (4 refs), b |
| `char b3[3]; int a; double d; char b16[16];` | b3, a, d, b16 (by size) |
| `f(int n) { int a, b; ext(&b); for (...n...) ext(&a); }` | b, a (`n` is read after `ext(&b)`, `a` overlaps it) |
| `f(int n) { int a, b; for (...n...) ext(&a); ext(&b); }` | `a` in `n`'s slot, then b |
| `{ char c; } { short s; } { int i; }` | one slot (1 -> 2 -> 4) |
| `{ char c; } { int i; }` | two slots (4 > 2 x 1) |

`slotlog.py` builds a variant that prints the list per function:

```bash
venv/bin/python3 Scripts/regalloc/slotlog.py
X87_C2=slotlog X87_OUT=/tmp/sl Scripts/x87_sched/sched.sh -l Source/CarAI_78.cpp
awk '/^@SLOTS/{p=index($0,"sub_452060")} p' /tmp/sl/last.log | grep -a '^@SLOT'
#   @SLOT _v82$59045 size=2 refs=3     (slot 0, at [esp+0])
#   @SLOT _new_z$ size=4 refs=4        (slot 1)
#   @SLOT _v85$ size=4 refs=3          (slot 2)
#   @SLOT _v7$59027 size=4 refs=2      (shares slot 0: its block and v82's don't overlap)
```

The listing (`last.asm`) names only the declared locals (`_v7$59027 = -28`); `$T` temporaries are in the list
but not in the listing. To get a local lower in the frame: give it more memory references than the ones
below it, or make it smaller. To make two locals share a slot, put them in disjoint scopes. A local declared
at function scope never shares with another function-scope local.

## Tools

`c2dis.py` disassembles C2.DLL (needs `iced-x86` and `pefile` in the venv):

```bash
python3 Scripts/regalloc/c2dis.py d 1071acdf 40              # disassemble 40 instructions
python3 Scripts/regalloc/c2dis.py x 1078e69f 1078e6ae        # who branches/calls to these addresses
```

## What is known

- C2's asserts pass the source file name, so `E:\8799\vc98\p2\src\P2\<file>.c` strings locate each
  source file. The assert calls sit in cold blocks (`mov edx,line; mov ecx,file; jmp 0x1076AB67`)
  after `0x1078xxxx`; find the hot code with `c2dis.py x <cold block>`.
- Allocator files: `color.c` (string `0x107A97DC`, graph colouring, asserts up to line 5616) and
  `regasg.c` (string `0x107A95CC`, register assignment). `color.c` code is at about
  `0x1071B000-0x10723400`.
- Per-function pass pipeline: `0x107657E2`. It calls each pass in turn with `0x107034AB` between
  them. The inliner (`0x1073B588`) is one of the first passes. The `color.c` phase is the call to
  `0x1071ACDF`, near the middle, after `0x10717684` and `0x10711F93` and before `0x10723B05`.
- `0x1071ACDF` calls, among others, `0x1071B48F`, `0x1071B848`, `0x1071BBC7`, `0x1071B079`,
  `0x1071BC3E` (asserts at `color.c` lines 0x331/0x3F3), `0x107203FD`, `0x10721578`, `0x1072243B`
  and `0x10722650` (0xD66 bytes, asserts at lines 0xE44/0xF17; most likely the select/colour step).
  `0x1072035D` (12 callers) holds the assert at line 0x15F0.
- Tables used in that range: `0x107A09BC` (.rdata, small int table, compared against the current
  value in several loops), `0x10799034` (.bssbe, writable, indexed like a register number: a
  "who holds register r" table is the first guess), `0x107A0494` (.rdata, flag word per entry,
  bit 0x40 tested). Their meaning is not confirmed.

## Priority and tie-break (reversed)

Both are now exact (`priolog.py` logs every term; summing its terms gives the final priority of every live
range in `Wolfy_3D4.cpp`, 329 of 329, and the tie of 373 of 377, the rest being constants and split pieces).

**Priority** `[lr+0x0C]` is built by `0x107203FD` (before the colour pass) in one walk over the blocks in list order:

- Each reference of the live range in block b adds its saving `c` to the block's tally `[lr+0x18]` and `c * bw`
  to the weight `[lr+0x3C]`. `c` is 2 for a load or store of a variable (`0x10721029`/`0x107213B5` per operand;
  constants have 0). `bw = 1 << [blk+0x6E]`, i.e. **2 per loop level** (1, 2 in a loop, 4 in a nested loop).
- At the end of block b, `N` = the number of live ranges referenced in b. Then every live range **referenced** in
  b gets `prio += N * bw * tally` (and the tally is cleared), and every live range only **live through** b gets
  `prio -= N * bw`.

So the priority is `sum over blocks of N_b * bw_b * (savings in b, or -1 if only live through)`. The weight `w`
(printed by ralog) only scales the scores. What follows in practice:

- One more use of a value in a busy block (many live ranges referenced) raises its priority more than the same
  use in a quiet one. A use inside a loop counts double.
- A value that is live across blocks it doesn't touch loses priority there, more so when those blocks are busy.
  Moving a use earlier, so the live range ends sooner, or a load later, so it starts later, raises it.
- Probe (`int x = g; c(); h = x; ...` with k uses, one block, x the only live range): N = 1, so the priority is
  the savings, 2 + 2k. In `while (k1) { c(); h = x; }` the loop block has N = 2 (x and `k1`), bw = 2: +8.

**Tie-break** `[lr+0x40]` is set by `0x10711F93`'s walk (cold part at `0x1071A919`, write at `0x1071A7EE`): a
counter runs over the blocks in list order and, **within each block, over the instructions backwards**. When a
definition of the live range is reached, the tie becomes the counter. A later block's definition overwrites an
earlier one's. Constants have 0. So among equal priorities (colour order is priority, then tie, both descending):

- in the same block, the value **defined earlier** gets the higher tie and is coloured first;
- a value (re)defined in a later block (a loop body after the init block) beats one defined only earlier.

`probe_loop.cpp`: `a`, `b`, `c` are initialised in that order (ties 6, 5, 4), then redefined in the loop
body, which overwrites them: `a += ..` is defined earliest in the body (17), `b ^= p[i] * 3` (15, the `imul`
comes after it in reverse order), `c |= ..` (14). So a, b, c are coloured in that order: edx, esi, edi.

```bash
venv/bin/python3 Scripts/regalloc/patch_c2.py && venv/bin/python3 Scripts/regalloc/priolog.py
X87_C2=priolog X87_OUT=/tmp/pl Scripts/x87_sched/sched.sh -l Source/Wolfy_3D4.cpp
grep -a '^@[PTR]' /tmp/pl/last.log      # @T ties, @P priority terms, @R colour decisions (ralog)
```

## Open

- **Block order at code generation.** Both register-only WIPs come down to it (see
  `docs/match_attempts.md`): `Wolfy_7A8::sub_543690` needs its final tail generated before the
  in-loop return (or one more round-robin temp in the in-loop tail), `Char_B4::state_8_5520A0`
  has the rotation shifted between two blocks only. What orders blocks before `0x10723B05` is now
  known: the loop sink pass `0x10740251` (`Scripts/flowopt/README.md`, "Code generation order"), which
  keeps the source order of out-of-loop code. For `sub_543690` that rules out a different block order
  with the original's layout, so it needs one more round-robin pick before the in-loop `lea`. Which
  values become colour-pass live ranges (optimizer temps, kind 3) rather than local temps is not
  reversed. Leads: kind 3 is any IL temporary (constructors `0x107014F6`, 7 callers, and `0x10702BC5`,
  17 callers, which also sets bit 1 of `[t+5]`); in `sub_543690` the coloured copy (`mov %edi,%eax`) first
  appears in pass `0x10713E23` (boundary `10765aff`, the `field_0[i]` index expansion), while the round-robin
  `lea` is formed later (`0x10714007`, then code selection `0x1071744C`). Telling the two constructors'
  temps apart, and which passes use which, is the next step.

- The log has no variable names: `[lr+0]` is not a symbol (C2 asserts in `p2symtab.c` when its
  name is read). Match live ranges to variables by the register they get in the listing.
- First real case tried, `sound_obj::HandleCarTireScrubSound_418720` (eax/ecx swap), turned out
  not to be a colouring difference: the original keeps the call's returned pointer and dereferences
  it after loading the divisor. The `__int64` raw form and `GetCarLinearSpeed() / max` both made it
  worse (4 -> 44 and 4 -> ~70 lines).
- The near misses that read as register swaps in `docs/match_attempts.md` are worth a `ralog.sh`
  run each: equal priorities point at statement order, a nonzero score at a preference.
