# x87 / MapRenderer handoff

Status of the MapRenderer x87 scheduling investigation, the tools built for it, and where to pick it up.
Read `docs/matching_quirks.md` ("x87 code: the scheduler works in 81-node windows" and the MapRenderer
entry under "Still unexplained") for the details behind each point.

## Where it stands

- **Solved mechanism: window breaks.** C2's list scheduler cuts each basic block into windows of at most 81
  nodes and moves integer instructions only within a window. The x87 order is fixed. No-op nodes (op 354)
  count toward the 81: parenthesised float subexpressions, `(f32)` casts of float expressions, stores to
  `f32` locals. The original had about 9-11 more of them per window in the cluster, so its windows break
  earlier. A patched window limit makes `draw_left/right/top/bottom` (`4F3C00`/`4F4250`/`4F4600`/`4F49B0`,
  limit 71), `4EEE60`, `4EF880` and `draw_lid_4EE130` (69) match exactly, and brings the others in the
  cluster to 2-6 lines. The table is in the MapRenderer entry in `docs/matching_quirks.md`. The tools are in
  `Scripts/x87_sched/` (README there).
- **Open: the source change.** Find where the original had the extra no-op nodes (most likely parentheses,
  casts or `f32` locals in the inlined `ProjectVertTop_46BD40` / `ProjectVertBottom_46BDF0` /
  `ProjectVert_46BC70` helpers or `set_vert_xyz_relative_to_cam`). It has to give about 9-11 nodes per window
  without changing the matched functions. A global limit of 69 changes `set_shading_lev_4E9DB0`, so the
  nodes are not spread evenly over the TU.
- Matched along the way: `Set_UV_4F4190` (f32 locals instead of `ToFloat()`: one more no-op node),
  `Mike_A80::sub_4FFD90` (a repeated `630 - fx` that VC6 CSEs; a ternary clamp).

## What is known

1. **The compiler build is not the cause.** decomp.me's VC6 builds were compiled against the same TU:
   RTM, SP3, SP4 (ours, byte-identical: CL 8804, C1XX 8867, C2 8799), SP5 and SP6 give the same code for the
   cluster; the Processor Pack is worse. The 10.5 Rich header matches our objects (Utc12_CPP 8799/8797,
   Linker600 8447). The scheduler is deterministic and doesn't depend on TU-global counters.
2. **The schedule follows the expression tree and the window breaks.** Assigning to an `f32` local, an
   `(f32)` cast of a product or a parenthesised float subexpression adds a no-op node. The node moves the
   next x87 op one cycle later and counts toward the 81-node window. A repeated float subexpression keeps
   a value on the x87 stack (`fsub %st(1),%st` ... `fstp %st(0)`). Commutative operand order and the
   spelling of the u32 conversion make no difference. VC7 drops the parentheses, so 9.6f can't show them.
3. **The helper bodies are the original source, as 9.6f compiled them.** 9.6f.exe is VC7.0
   (Utc13 13.00.9466, VS.NET 2002), and decomp.me's msvc7.0 is that exact build. With
   `/O2 /Ob0 /G5 /GX` (/G3 /G4 the same; the default /GB and /G6 differ: they convert u32 with
   `test/jge/fadds 2^32` instead of `fildll`) our current `ProjectVertTop_46BD40`, `ProjectVertBottom_46BDF0`
   and `ProjectVert_46BC70` bodies give the 9.6f code exactly (ratio 1.000). The only changes needed were
   external linkage and `__stdcall`: VC7 gives static functions a custom register convention.
4. **In 9.6f the helpers are members of `Nanobotz`, called with the caller's `this`** (`mov %ebx,%ecx` before
   `call 0x46BD40`). `Nanobotz` is MapRenderer's 9.6f name (it also has `set_shading_lev`, `GetColour`,
   `TransformTriangleUVs`, `ClearDrawnTileCount`), so in 10.5 they're probably inline MapRenderer members.
   The out-of-line `MapRenderer::ProjectVertTop_4EAE00`/`Bottom_4EAEA0` (both matched) would then be
   copies VC6 didn't inline (see "Big functions run out of inline expansions").
5. The remaining cluster difference: in the y line of the inlined Top, the original issues the centre load
   and the dead zero high dword of the u32 -> float temp (`mov 0x74(%eax),%edx; mov %esi,0x1C(%esp)`) only
   after `fstps x; fildl y; fmuls; fmul`, ours right after the x `fiaddl`. In the original those
   instructions fall into the next scheduling window (point 2 and "Where it stands").

## Experiments and their scores

Scored with `score.py` over all 30 MapRenderer WIPs (differing lines, lower is better; base 1505):

| Change | Total | Notes |
|---|---|---|
| Top/Bottom/ProjectVert as members of another struct, called through a global instance | 1293 | 4EA390 -67, 4EAF40 -63, 4EBA60 -27, 4ED290 -55, nothing worse, cluster unchanged |
| local `Camera_0xBC*` in Top | 1411 | 4EAF40 -67, 4ED290 -60, 4EBA60 +32 |
| `(f32)(y * scale)` or an f32 local for the Top y product | 1417-1426 | cluster -12, 7 functions worse |
| inline MapRenderer members (`inline void MapRenderer::ProjectVertTop_46BD40`) | 1467 | 4EA390 -45, cluster +5 |
| static/extern/`__stdcall` inline free helpers, `__forceinline` | 1505 | no change |
| about 150 other helper and call-site forms (centre conversions, z local, atan2 argument temps, pointer forms) | >= 1505 for the cluster | |

None of these is committed.

## Tools (`Scripts/tu_harness/`; scheduler windows: `Scripts/x87_sched/`)

```bash
Scripts/tu_harness/fetch_compilers.sh                 # once: VC6 RTM..SP6 and VC7.0 into build_vc6/compilers/
Scripts/tu_harness/tu.sh Source/MapRenderer.cpp 0x4ecaf0 0x4ece40   # ~3 s: preprocess + compile one TU, diff counts
venv/bin/python3 Scripts/tu_harness/diff.py build_vc6/tu_harness/q.obj 0x4ecaf0      # the diff itself
venv/bin/python3 Scripts/tu_harness/score.py Source/MapRenderer.cpp --save           # base for a TU
venv/bin/python3 Scripts/tu_harness/score.py /path/to/variant/MapRenderer.cpp       # total + deltas + broken MATCHes
Scripts/tu_harness/cl.sh -tc msvc6.5 file.cpp         # any single file with another compiler build
```

`tu.sh` takes a modified copy of a .cpp anywhere on disk (it is compiled as if it were in `Source/`), so a
script can write variants to a temp file and score them without touching the tree. `score.py` also lists
`MATCH_FUNC`s whose code changed against the saved base. Still confirm a find with a full `build.py` and
`compare_builds.py`, and edit headers only temporarily.

9.6f checks (target: `target_96f.json` from the `claude/target-asm` branch, fetched by `diff.py --96f`):

```bash
W=build_vc6/tu_harness
Scripts/tu_harness/tu.sh Source/MapRenderer.cpp                       # writes $W/q.cpp (preprocessed)
n=$(grep -n "^static inline void set_vert_xyz_relative_to_cam_inlined" $W/q.cpp | cut -d: -f1)
head -$((n-1)) $W/q.cpp | sed 's/^static inline void ProjectVert/inline void __stdcall ProjectVert/' > $W/h96.cpp
echo 'void __stdcall W1(Fix16& x, Fix16& y, Vert* p) { ProjectVertTop_46BD40(x, y, p); }' >> $W/h96.cpp
Scripts/tu_harness/cl.sh -tc msvc7.0 $W/h96.cpp /O2 /Ob0 /G5 /GX
venv/bin/python3 Scripts/tu_harness/diff.py $W/h96.obj 46bd40 --96f   # 0 differing lines
```

VC7 rejects things VC6 accepts (case labels that skip an initialisation: C2360/C2361), so whole-file VC7
builds fail; cut the TU before the first such function and append only what you test.
`target_96f.json` holds only the 9.6f functions paired with a 10.5 function (3818). For MapRenderer that is
0x470250 (`DrawRightSide_4EAF40`), 0x46D9A0 (`draw_bottom_4ED290`), 0x470800 (`draw_lid_4F4D60`), 0x46EE40,
0x46BEA0 and the helpers 0x46BBF0/0x46BC70/0x46BD40/0x46BDF0.

## Next steps

0. **Find the missing no-op nodes.** For one function at a time: `Scripts/x87_sched/sched.sh
   Source/MapRenderer.cpp draw_left_4F3C00` shows the windows and where ours breaks. `regsearch.py` gives
   the limit, and so the node shortfall, per window. Add parentheses, `(f32)` casts or `f32` locals to the
   helpers until the counts agree, then check the whole TU with `regsearch.py --tu` or `score.py`. Start
   with the four `draw_left/right/top/bottom` functions: they need the same 9 nodes. The steps below still
   apply to the helper and call shapes.
1. **Match a 9.6f Draw function under VC7** (start with 0x470800 `draw_lid` = 10.5 `draw_lid_4F4D60`). Make
   Top/Bottom/ProjectVert MapRenderer members called on `this`, as in 9.6f (`thiscall`, ret $0xC); a first
   try with them as `__stdcall` free functions was at 0.452 because of the missing `this`. Once a 9.6f Draw
   function matches, its source shape (member calls, temporaries, argument order) is settled, and the same
   source compiled with VC6 is the best 10.5 candidate.
2. **Then the VC6 side**: build the cluster with that member form. The "struct + global instance" row above
   suggests that how `this` reaches the inline changes the schedule; the plain inline member form only
   helped 4EA390. Try `this` passed through (non-static members of MapRenderer called from MapRenderer
   members), a `MapRenderer*` local, and `gMapRenderer->`.
3. Use the same 9.6f route for other inline helpers: any 9.6f callee listed in `docs/inlines_96f.md` can now be
   compiled with VC7 and checked exactly (`/O2 /Ob0 /G5 /GX`, external linkage, the right calling convention).
