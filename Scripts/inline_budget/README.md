# VC6 inline budget tools

VC6 inlines `inline` functions until the caller runs out of an **inline budget**. In a big
function, some calls to an inline are expanded and later ones (or ones nested inside an earlier
expansion) are left as out-of-line calls. The rule below was reversed from `C2.DLL` (VC6 SP4,
12.00.8804) and checked with a patched copy that logs every decision: a re-implementation
(`inlsim.py`) reproduces all of more than 15,000 logged decisions over about 4,500 functions.

Use these tools instead of writing per-call-site variants of an inline (`*_ool`, `*OOL*`,
`*_forced`, ...): they show why a call goes out of line and what source change moves the cut-off.

## The rule

The driver is at C2 `0x1073b588`; the recursive walker `InlineCalls(ctx, depth, budget, flag)` is
at `0x1073b633`.

- **Size.** Every function has a front-end size (roughly its expression and statement node count,
  computed by C1XX before any inlining, read by C2 at `0x107327ca`, never re-measured). Examples:
  `Fix16` binary `operator-`/`+` 52, `operator*` 57, unary `operator-` 42, `Fix16::Abs` (if/else)
  64, `Fix16(s32, u8)` 30, `Fix16()` 20, `MaxAbsDistance_42A6B0` 123.
- **Start.** A function gets `budget = clamp(2 * size, 1000, 35000)`; a running total starts at its
  size.
- **Order.** Candidate call sites are walked in IL evaluation order: call arguments right to left
  (`Max(Abs(dx), Abs(dy))` visits the `dy` Abs first), binary operands left to right, inner calls
  before outer ones, `for (init; cond; inc)` as init, inc, cond. `sites_left` is the number of
  candidates not yet visited, including the current one.
- **Per site**:
  - rejected if the nesting depth exceeds the site's `inline_depth` (default 8) or the argument count
    doesn't match;
  - unless `__forceinline`: a callee of size <= 40 is **free** (always inlined, never charged); a
    bigger one is rejected if `size > budget`; anything non-forced is rejected once the total is over
    35000;
  - on accept: `budget -= size` (size > 40, not forced), `total += size`; then the callee's own sites
    are walked with **`budget / sites_left`** (C division) at depth + 1, and the nested amount used is
    subtracted from `budget` afterwards (not for `__forceinline`);
  - a rejected site costs nothing, but it (and every free site) still counts in `sites_left`.
- **Never inlined**: a constructor that builds a by-value class argument in its outgoing slot
  (`Call(Fix16(113))`, "SKIP(argctor)", free, counts in `sites_left`), and callee bodies that need an
  EH frame (a `try`, or a local with a non-trivial destructor under `/GX`); the latter are not
  candidates at all.

- **Charged but never expanded** (under `/GX`): an accepted callee that returns a class with a
  destructor by value (hidden `___$ReturnUdt`, C2 `0x1073b7f7`) is charged like any other site and
  then called out of line, with no nested walk. All `Fix16_Point` operators are like this. The log has
  `@I ABORT udt` and `inltree.py` shows `CALL(udt)`. See "Inline calls and EH states" in
  `docs/matching_quirks.md`.

What follows:

- Nested sites of a big inline only get `budget / sites_left`, so they are cut off long before the
  top-level budget is used up. That is why `Abs` or `operator-` inside `MaxAbsDistance` stays inline
  at one site and not at the next.
- Free sites (getters, empty constructors) *after* a site shrink its share: a getter vs a field
  access, or an extra `Fix16 x;`, can move a cut-off.
- Caller code raises the budget by 2 per size unit. Some costs: `{}` or `(void)0;` +2 (no code, handy
  as test padding), `x = y;` +5, `if (x) y = 1;` +12, `Fix16 b;` +11 and one site,
  `Fix16 b = a;` +7, `r = a - r;` +17, a ternary about 6 less than if/return.
- `__forceinline` is never charged, but its own sites still get only `budget / sites_left`.

## Tools

One-time: `python3 Scripts/inline_budget/setup.py` builds the instrumented compiler in
`build_vc6/inline_c2/` (git-ignored; `inl.sh` runs it on demand). It applies `c2_patch.json` to the
submodule's `C2.DLL` and checks its hash first. The patch only adds logging; the code is the same.

```bash
# Decision tree of one function: each site's result, callee size, budget, sites left, nested budget
Scripts/inline_budget/inl.sh Source/Explosion_30.cpp TimerAfter50Handler      # -a also lists free sites

# What-if, on the log inl.sh just wrote
L=build_vc6/inline_c2/last.log
venv/bin/python3 Scripts/inline_budget/inlsim.py $L TimerAfter50 --scan -200:200   # caller size +-N
venv/bin/python3 Scripts/inline_budget/inlsim.py $L TimerAfter50 --list            # top-level site indices
venv/bin/python3 Scripts/inline_budget/inlsim.py $L TimerAfter50 --extra 2@40      # 2 free sites before site 40
venv/bin/python3 Scripts/inline_budget/inlsim.py $L TimerAfter50 --size Abs=+8     # change a callee's size
venv/bin/python3 Scripts/inline_budget/inlsim.py $L --verify                       # model vs log

# Small experiments (Source/ on the include path); log on stdout
Scripts/inline_budget/probe.sh /tmp/t.cpp > /tmp/t.log; python3 Scripts/inline_budget/inltree.py /tmp/t.log

# Which MATCH/WIP functions change between two copies of Source/ (before/after an inline change)
venv/bin/python3 Scripts/inline_budget/fullexp.py /tmp/src_base /tmp/src_mod CarAI_78.cpp Ped.cpp
```

`inlsim.py` only knows the callee bodies that were expanded somewhere in the log; it lists the
others as unknown (treated as empty).

Without the patched compiler, `#pragma warning(1:4710)` (for example through `/FI` of a header
holding it) makes stock VC6 print a C4710 warning for every call it didn't inline, at the line of
the call.

## Workflow for an out-of-line call in the original

1. Write the natural single inline (check its body against 9.6f with `Scripts/tu_harness/`, when
   9.6f has it out of line).
2. `inl.sh` the function and compare the OUT-OF-LINE sites with the original's calls.
3. If they differ, `inlsim.py --scan` / `--extra` tells how much caller size or how many sites are
   missing or extra. Look for the source difference that explains it: a getter written as a field
   access (or the reverse), a missing 9.6f helper call, a by-value vs by-reference parameter, an
   initialised vs assigned local. Prefer what 9.6f shows.

To change the hooks, `patch_c2.py` regenerates `c2_patch.json` (needs `pip install pefile keystone-engine`).
