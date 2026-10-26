# What the verifier misses, and the checks that see it

`compare_all_functions.py` (the `build.py` verifier) says whether each `MATCH_FUNC` has the same
instructions as 10.5.exe once absolute addresses are renamed. This page lists what that leaves
unchecked, the checks added for it (`Scripts/bin_comp/run_extra_checks.py`, `build.py --extra_checks`),
and what they found on master at d26e830b (3430/3430 MATCH functions verified).

## What each existing tool checks

| Tool | Checks | Doesn't check |
|---|---|---|
| `compare_all_functions.py` / `compare_function.py` | Each MATCH function with a row in `og_function_data_v105.csv`: the csv size of bytes from both exes, disassembled and normalised by `post_process_asm.py`, must be equal | Anything outside the function's bytes (EH funclets and tables, jump tables, data); what renamed operands stand for (see below); functions without a csv row |
| `post_process_asm.py` | Renames, in order of first use: every `call` and `push` operand and every `jmp` target, whatever its value; every other immediate or displacement above 0x400000 in `mov`/`cmp`/`add`/`sub`/`test`/`imul`/x87/`and`/`or`/`xor`/`inc`/`dec`/`div`... | Two different values used once each look the same: a different float pushed as an argument (`push 43EF0000h`), a different negative constant (`cmp eax, -14` for `-12`: negative numbers are above 0x400000 unsigned), a different callee, a `jmp` into the wrong copy of a block |
| `compare_builds.py` | The same normalisation, against a baseline build | The same as above |
| `check_match_refs.py` | Absolute operands that have an original identity (marker, `DEFINE_GLOBAL`, `_ADDRESS` name) at the exact symbol start; callees with an identity (with alias witnesses); switch jump tables and byte index tables | Addresses with no identity (strings, float constants, vtables, GUIDs, COMDAT copies of inlines, CRT functions, imports); offsets into a known global; non-address immediates; EH; data. Not run by CI |
| `check_global_inits.py` | Reads 10.5.exe's static initialisers: store widths, and values of `DEFINE_GLOBAL_INIT` in the forms `Fix16(n)`, `Fix16(raw, 0)`, `Ang16(n)` | Other initialisers (tables, copies, constructors, compile-time data), globals not declared with `DEFINE_GLOBAL`; prints only, exit code always 0. Not run by CI |
| `check_duplicate_globals.py` (in `build.py`) | Two `DEFINE_GLOBAL`s at one address | Plain declarations named after an address (`ErrorLog gFile_67C530` next to `DEFINE_GLOBAL_INIT(ErrorLog, gErrorLog_67C530, ...)`) |
| CI reccmp job | `reccmp-reccmp` HTML report of a RelWithDebInfo (`/Z7`) build, `reccmp-datacmp` (continue-on-error) | Doesn't fail the build; it is a different build from the one `build.py` verifies |

`decomp_main.exe --check_vars_only` (handled in `test.cpp` at the repo root) runs the globals registry's
`CheckVars()` and exits; it doesn't compare values with 10.5.exe.

## The new checks

All run from `Scripts/bin_comp` after a build, exit with 1 on differences and have a `REVIEWED` dict
for differences checked as equivalent. `run_extra_checks.py` runs `check_match_refs.py` and these
three in about 4 s.

`run_extra_checks.py` compares what they find with `Scripts/bin_comp/extra_checks_known.txt`, the
findings below that master still has. It fails only on a new difference, or on a known one whose
details changed (build addresses are left out of the details, so code moving elsewhere doesn't change
them). A known one it no longer finds is reported as fixed: remove it from the list in the same change,
or run `run_extra_checks.py --update-known` and review the diff. CI runs it after the Linux build with
a notice when nothing is new, a warning per new difference and the lists in the job summary; it fails
the job only once `EXTRA_CHECKS_BLOCKING` is set to 1 in `.github/workflows/ci.yml`.

- `ref_values.py` (shared): what a build address stands for in 10.5.exe. Identities from
  `check_match_refs.load()`, file-local objects named after their address (`const` tables like
  `kDoorFirstDeltaIdx_61A808` are static, so the map doesn't list them: they are read from the
  object files and placed through their relocations), CRT names from `reccmp/library.csv`, and
  imports by name. Anything else is compared by content: strings to their terminator, float
  constants by their size, vtables entry by entry, functions by their code with absolute addresses
  left out, other data up to the next object.
- `check_match_eh.py`: per MATCH function with `/GX` EH, the handler (`__CxxFrameHandler`),
  `FuncInfo` magic and `maxState`, each unwind entry (`toState`, and the funclet: frame offsets
  exact, destructor mapped as above), try blocks and catch handlers. When the same funclets are
  only on other states it says so.
- `check_match_consts.py`: each absolute operand without an identity, paired by instruction as in
  `check_match_refs.py`, compared by content; offsets into known globals; immediates the normaliser
  renames although they aren't addresses; jumps inside the function by offset. An indexed access is
  compared from the first entry its bounds checks let it reach: `table[ch - 0x80]` after
  `cmp ax, 80h; jb` is `[eax*2 + table - 100h]`, and `table - 0x100` can be any other object (an
  import slot in `TextTable::ToUpper_5B5AD0`, the Russian table in `RemapExtendedChar_5B58D0`).
- `check_global_values.py`: runs both exes' CRT startup (`_ioinit`, `__xi_a..__xi_z`,
  `__xc_a..__xc_z`) under unicorn (`static_init_emu.py`) and compares every global the build maps
  to a 10.5.exe address, byte for byte, pointers by what they point at. Sizes come from the mangled
  type, the `DEFINE_GLOBAL`/declaration in `Source/`, or else the object file, bounded by the next
  address 10.5.exe's code uses. A difference that starts where 10.5.exe has another object (another
  mapped global, or the lowest address one of its initialisers writes) is flagged as such: the
  global is declared bigger than in 10.5.exe. Compared with `check_global_inits.py`, this checks the
  values of all globals however they are computed; `check_global_inits.py` still adds the store
  width check and the "POD but dynamically initialised" report.

### The emulator against the real 10.5.exe

A copy of 10.5.exe with `EB FE` (jmp $) at `WinMain_5E53F0` was run under wine (scratch prefix, no
display), and once it spun, a small VC6-built tool read its `.data`/`.bss` (0x607000..0x70A000)
with `ReadProcessMemory` (`/proc/<pid>/mem` is refused by Yama ptrace scope 1, since the Windows
process isn't a child of the launcher). Compared with the emulated startup:

- whole range: 551 of 1,060,864 bytes differ, all in CRT state the emulation doesn't set up
  (`_setenvp`/`_setargv`/`_tzset`/code page tables, `_osver`, heap internals), in heap pointers,
  in a time stamp and in the three globals below;
- the 1689 globals `check_global_values.py` compares in that range: 1682 identical, 4 differ only
  by heap addresses (the `ErrorLog` objects at 0x67C530, counted twice as `gErrorLog_67C530` and
  `gFile_67C530`, at 0x67CF58 and `gMiss2Log_6F7698`), 3 differ:
  `dword_705334` (10.5.exe measures it with `rdtsc` at startup; the emulated run divides by zero,
  it is in `REVIEWED`), `gTmpBuffer_67C598` (the log constructor writes the date), and two entries of
  `gTanTable_6663C8` (90 and 270 degrees, where tan overflows: unicorn's `fptan` uses the host's
  double precision `tan`, not the x87's).

So the emulated values are the real ones apart from run dependent data, and both sides go through
the same emulator, so the check doesn't need wine.

## Findings on master (d26e830b)

These are the entries of `extra_checks_known.txt`. Cross-checked by hand against 10.5.exe where marked.

### EH data: 36 MATCH functions differ (174 have an EH frame)

All their funclets call an empty destructor (10.5.exe's `ret` at 0x40AC40, the build's
`??1Fix16_Point@@QAE@XZ` and `??1SpriteList_4_dtor@@QAE@XZ`), so the game behaves the same. They
show that locals or temporaries are declared or constructed in another order, or sit in another
frame slot, than in the original source. Hand-checked: `SpawnCabAndTrailerHelper_408370` (handler
0x5FAED0, FuncInfo 0x602780, unwind map 0x6027A0: states 1 and 2 destroy `[ebp-14h]` and
`[ebp-1Ch]`, the build `[ebp-1Ch]` and `[ebp-14h]`), and `Object::IntegrateMovementAndCollisions_523BF0`.

Same funclets on other states (21): `Trailer::UpdateTrailerAlignment_407CE0`,
`SpawnCabAndTrailerHelper_408370`, `Car::TryHitchTrailer_442810`, `Car::ApplyExplosionImpulse_443710`,
`Object_3C::GetMovementSpeedAndAngle_521FD0`, `Object::ResolveCollisionWithObject_522710`,
`Object::ResolveCollisionWithPed_5229B0`, `Object::ResolveCollisionWithHorizontalEdge_522BE0`,
`Object::ResolveCollisionWithVerticalEdge_522D00`, `Particle_4C::UpdateObjectBeamLink_state_38_538AC0`,
`Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0`, `ParticleManager::EmitFlameStreamSegment_53F4C0`,
`CarPhysics::SetVelocityTowardTarget_55A1D0`, `CarPhysics::SpawnSkidSegment_55D200`,
`CarPhysics::HandleCarCollision_55FF20`, `CarPhysics::HandleObjectCollision_5606C0`,
`CarPhysics::ProcessPedImpact_560B40`, `CarPhysics::ApplyDriveForce_5615D0`,
`SpriteShape::UpdateRotatedBoundingBox_5A3550`, `Weapon::flamethrower_5DD0F0`, `Weapon::pistol_5DD860`.

Other frame slots (13): `Car::EmitExplosion_43D690` (10.5.exe destroys `[ebp+8]`, an argument),
`Object::IntegrateMovementAndCollisions_523BF0` (`[ebp-44h]`, `[ebp-14h]` vs `[ebp-14h]`, `[ebp-1Ch]`),
`Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280`, `ParticleManager::GunMuzzelFlash_53E970`,
`ParticleManager::EmitImpactParticles_53FE40`, `Char::HandleObjectCollision_548840`,
`CarPhysics::ProcessGroundCollisionAndSurfaceType_55B970`, `ComputeCollisionImpulse_55F3B0`,
`CarPhysics::ComputePointVelocity_561380`, `Sprite::FindCollisionIntersectionPoint_5A2710`,
`Weapon::dual_pistol_5DDA70`, `DrawElectricArc_5DE910` (10.5.exe destroys `[ebp+4]` in state 0),
`Weapon::car_smg_5E2940`.

Another member destroyed in a constructor (2): `Camera::ctor_4368E0` (10.5.exe destroys
`this+0x48`, the build `this+0x60`) and `Char::ctor_544FF0` (`this+0x98`, the build `this+0x88`):
the member with a destructor is at another offset, or another member has one.

### Constants, callees and global values: none

8 functions with a wrong constant or callee (a `Fix16` constructor, `IID_IDirectDrawSurface3`,
`DPAID_Modem`, copies of SetGamma and DeinitializeAudio) and 6 globals with wrong values or sizes,
found on 5c33a937, were fixed in #869. `PedGroup::DestroyGroupArray_4C8E60` calls the hand-written
`__ehvec_dtor` in `crt_stubs.cpp` where 10.5.exe calls the CRT's `??_M`: it is in `REVIEWED`.
8608 references without an original identity are compared; 2 are left out because where the object
starts is unknown.

## Proof that each check sees an injected defect

In a throwaway worktree, with these changes the verifier still printed `[3426/3426] funcs OK`:

| Change | Reported by |
|---|---|
| `"SOFTWARE\\DMA Design Ltd\\GTA2\\Sound"` to `...\\Sounds` in both uses in `Registry::Open_Sound_Root_586A00` | `check_match_consts.py`: `string '...Sound', the build '...Sounds'` |
| `478.0f` to `477.0f` for the `bottom` argument of `FrameProfiler::DrawProfileBar` | `check_match_consts.py`: `FrameProfiler::DrawProfileHistory_4FFD90: immediate 0x43ef0000, the build 0x43ee8000` |
| `kDoorFirstDeltaIdx_61A808` `{7, 24, 11, 28}` to `{..., 29}` (a static `const` table) | `check_global_values.py` (`check_global_inits.py` doesn't see it) |
| `kFpOne_67B434` `Fix16(1)` to `Fix16(2)` | `check_global_values.py` and `check_global_inits.py` |
| in the built exe, the unwind funclet of `Car::GetHitchPoint_439FB0` patched from `lea ecx, [ebp-14h]` to `[ebp-18h]` | `check_match_eh.py`: `state 0: frame offset` |
| in the built exe, entry 5 of `word_626590`, which `TextTable::ToUpper_5B5AD0` reads through `[eax*2 + word_626590 - 100h]` | `check_global_values.py`: `word_626590 +10`; `check_match_consts.py` leaves the table to it, as it has an identity |
| in the built exe, entry 0x21 of `kExtendedWesternCharMap` (no identity), read through `[eax*2 + table - 100h]` where the build's displacement is the Russian table | `check_match_consts.py`: `RemapExtendedChar_5B58D0: 2a: 626390 holds b000..., the build _kExtendedWesternCharMap holds b100...` |

Each check reported exactly the injected difference on top of the master findings. Swapping the
declarations of two `Fix16_Point` locals in `CarPhysics::CalculateFrontWheelForce_561E50` and
`Crane::SetTarget1_480900` was reported by `check_match_eh.py` too, but also changed their code.

## Limits

- `check_match_consts.py` doesn't follow a callee reached by a tail `jmp`, so a jump into a wrong
  copy of a function isn't reported (a SetGamma copy reached from `Game::UpdatePaused_4B93C0` was missed).
- An indexed access whose bounds can't be derived (a register set by a load, a path around the
  checks, two registers) is compared from its raw displacement, as the object there or the next
  object with an identity; when that is an import slot, it is left out.
- The checks pair instructions one to one; a function whose instruction count differs is left to
  the verifier.
- Data with no symbol in the build and no reference from code isn't compared, nor are globals the
  build doesn't name after an address.
- Struct globals of unknown size are compared up to the next address 10.5.exe's code uses, which
  can be one of their fields.
- Heap pointers set by initialisers are compared only as being heap pointers.
- `compare_all_functions.py` raises (and stops checking the remaining functions) when a MATCH
  function gets shorter and the csv size runs into bytes that don't decode: `Crane::SetTarget1_480900`
  did this after the local swap above. `build.py` still fails, without a `FAIL!` line.
