# Match attempts

A log of functions that were worked on and still don't match, with what was tried.
Read the entry for a function before working on it, so the same ideas aren't tried
again. General VC6 patterns belong in `matching_quirks.md`; this file is per function.

For each entry: the closest the function got, what the remaining diff looks like, and
each thing that was tried and what it changed. When a function finally matches, delete
its entry here and add the trick that did it to `matching_quirks.md`.

Target asm comes from the `claude/target-asm` branch (`compare_target_asm.py`, see
`CLAUDE.md`).

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
in before the `memset`. That doesn't work for a plain `char_type`. What fixed `SendPing_51EF60`
was putting the payload's `memset` after the header stores. The equivalent here,
`keep_alive = 2` after the `field_D` store, gives 0.619.

## NetPlay::MovePlayerToGroup_520040 (WIP)

Ratio 0.692. Moves player `toFind` from `pStru` to `pDst` through `AddPlayer_51E9C0` and
`FreePlayerSlot_5201A0`. Branch layout matches with the nested form (the call inside the loop's found
branch, `break` on failure, one `return 0` at the end). The registers don't: the original
has `this` in `ebx`, `pStru` in `ebp` and the result 1 in `edi` across the `FreePlayerSlot_5201A0`
call (`mov $1,%edi` ... `mov %edi,%eax`). Ours has `this` in `edi`, `pStru` in `ebx`, and
`mov $1,%eax` after the call.

Tried:
- Early `return 0`s with the call after the loop (the failure blocks get laid out
  inline).
- `s32 bMoved = 1;` before the `FreePlayerSlot_5201A0` call and `return bMoved;` (no change), and
  the same with the call after the loop (0.615).

## NetPlay::EnumAddress_cb_51E030 (WIP)

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

Oct 5 (86 lines): `lpContext` used directly and `pWide` at function scope: no change.

Now 16: a plain (non-volatile) `BOOL&` flag, `while (!bDone)` and two trailing `lstrlenA` ifs give the
original's registers and entry push. Left: VC6 drops the entry flag test and keeps the bottom one, the
original the reverse. Non-volatile with the entry `if (!bDone)` folds it (70); volatile gives the right
flow but the wrong prologue (86).

## BurgerKing_1::SetAltKeyState_498CB0 (WIP, was STUB)

Target: `mov 4(%esp),%eax; shr $7,%al; mov %al,gAltKeyDown_67B80C; ret $4`. It loads the whole
dword, then shifts only `al`. Every spelling below compiles to a byte load
(`mov 4(%esp),%al; shr $7,%al`), or to a dword shift plus `and $1,%al`. Tested in a scratch
TU with `build.py --single_cpp ../build_vc6/scratch_t.cpp`:

- byte load: `(u8)a1 >> 7` with `a1` as `u32`, `s32`, `u8` or `char_type`;
  `(u8)(a1 & 0x80) >> 7`; `((u8)a1 & 0x80) >> 7`; `(u8)a1 / 128`; `*(u8*)&a1 >> 7`;
  a copy through a `u32` local; an inline `u8 HighBit(u8)`; a 4-byte struct/union
  parameter (`S4.b0`, `U4.b[0]`, `(u8)S2.w0`); a `u8` bitfield.
- dword shift + `and $1,%al`: `(a1 & 0x80) != 0`, `(u8)(a1 >> 7) & 1`,
  `(a1 & 0xFF) >> 7`, a `u32` bitfield.
- `mov %al` + `test` + `setl`: `(char_type)a1 < 0`.

Now an implemented WIP at 2 diff lines. About 20 more spellings also give 2.

## NetPlay::EnumSessions_51E650 (WIP)

Ratio 0.915. Enumerates sessions: with `field_4` set it loops on `DPERR_CONNECTING`
(PeekMessage/Translate/Dispatch, `Sleep(500)`), then calls again with
`DPENUMSESSIONS_STOPASYNC`. Otherwise it calls once with `AVAILABLE | ASYNC`. Returns
the session count, 0 on `DPERR_USERCANCEL`, -1 on failure. `EnumSessions_cb_51EAE0` had to
become `static __stdcall` to be passed as the callback (it never used `this`, and it
still matches).

The code is right. Only the layout of the return blocks differs: the original has a
single `or $-1,%eax` return block at the very end, which every "not DP_OK" check jumps
to. The "`hr != DP_OK` -> -1, else return count" check sits right after the
STOPASYNC call, and the else branch's `jge` jumps back up into it. Ours gives each
failure its own return block.

Tried:
- Separate `if (hr != DP_OK) return -1;` in each branch (0.915, best).
- One shared `if (hr != DP_OK) return -1; return count;` after the if/else, with the
  STOPASYNC call under `if (hr == DP_OK)` (0.901, the shared check goes to the end).
- The shared check at the end of the `if` branch under a label, with the else branch
  doing `if (hr >= 0) goto check_result; return -1;` (0.894).
- The same with every "not DP_OK" check as `goto failed` and `failed: return -1;` at the
  very end (0.894: VC6 copies the check into the else path instead of jumping back).

## Stubs that aren't normal functions

These had `STUB_FUNC` markers but are compiler-generated in the original:

- `NetPlay::vdtor_51D7B0` (NetPlay's scalar deleting destructor) and
  `NetPlay::static_dtor_5E4DD0` (the `atexit` destructor for `gNetPlay_7071E8`) now match,
  written out by hand: `this->NetPlay::~NetPlay(); if (flags & 1) operator delete(this); return this;`
  and `gNetPlay_7071E8.NetPlay::~NetPlay();`. The qualified call stops a virtual dispatch,
  and VC6 turns the second into `mov $gNetPlay,%ecx; jmp ~NetPlay`.
- `PedGroup::sub_4C8E60` (0x4C8E60): the static destructor for the global array of 20
  `PedGroup`s (`push ~PedGroup; push 0x14; push 0x44; push pedGroups_67EF20; call ??_M`).
  Our build generates the same code as `_$E5` for `DEFINE_GLOBAL_ARRAY(PedGroup, ...)`, but there
  is no way to put a marker on it: a `MATCH_FUNC` before the array definition is followed by
  `_$E7` (the init wrapper), and the verifier skips `$E` symbols ("not in the build"). Plain
  C++ can't name `??_M` or take a destructor's address. Now an implemented WIP (12 lines): a
  wrapper struct (`PedGroupArray_4C8E60`) whose destructor holds the `??_M` call, but VC6 doesn't
  inline it, so ours is `mov ecx; jmp`.
- `cSampleManager`'s marker was on 0x58D400, which is the static init thunk for
  `gSampManager_6FFF00` (`mov ecx; jmp ctor`; 0x58D410 registers the `atexit` destructor and
  0x58D420 jumps to `~cSampleManager`). The constructor is at 0x58D430, which had no csv row.
  With the row and the marker moved, it matched once its three handle arrays were cleared with
  loops instead of `memset`. The `memset`s picked other zero registers and instruction order.
- The `crt_stubs.cpp` functions (`malloc`, `free`, `fopen`, ...) are the static CRT.

## NetPlay::CalcPacketLen_51F210 (WIP)

Rewrites a received packet in place: copies it to a 64-byte local, writes a 5-byte header
(bytes 0-4) chosen by the first byte (cases 1-9, unknown is `FatalError` 1073), copies
the payload to offset 5 and returns the new length. The fatal error's source file name
is a guess.

Ratio 0.628 (both versions below). The logic is right, the registers aren't. The
original keeps the constant 3 in `ebx` (for `and $3` after the `rep movs`, the type-3
header byte and the case 4 length), the result length in `ebp` (zeroed at the top,
`mov %ebp,%eax` at every exit), and spills `pPacket + 5` into `pPacket`'s own stack slot.
It also reloads `pBytes[4]` from memory before computing `pBytes[2] = pBytes[4] + 2`.

Tried:
- `return` in each case (the default then gets its own epilogue after the fatal error).
- A single `len` result with `break`s (VC6 still turns it into constants per case).
- A packed 5-byte header struct instead of `u8*` indexing (0.628, the `pBytes[4]` reload
  still doesn't appear, the header stores get reordered).

The missing code after the `FatalError_4A38C0` call in the target is only the csv size stopping
there: the real epilogue follows, so it isn't `noreturn`. Oct 5: 182 lines, still only registers
(`len` in `ebp` zeroed and copied at the exits, 3 in `ebx`, the `pBytes[4]` reload).

## NetPlay::WaitForPlayersSync_5213E0 (WIP)

Ratio 0.903. Fills the sync check data (`sub_4DB2E0`, a new stub), sends it
(`Send_521E40`) plus a type-4 packet (`Send_521370`), then loops on `Receive_51F010` until
every other player has acked (type 2) and sent their sync data (type 1/2/5, checked by
`CompareRemotePlayers_4DB440`, also a new stub) or 20 s pass.

Everything matches except where the two final return blocks go. The original: after the
loop `test %bl,%bl; je <return true>`, then the `return false` block (which the
timeout check and the two `IndexOf` failures also jump to), then `return true` last.
Ours always puts `return true` first and the shared `return false` at the end.

Tried (all 0.903 or the plain `sete` version):
- `if (bTimedOut) return false; return true;` (VC6 turns it into `sete`).
- `if (bTimedOut) { failed: return false; } return true;` with the inner failures as
  `goto failed`.
- `if (!bTimedOut) goto succeeded; failed: return false; succeeded: return true;`
- `if (!bTimedOut) return true; failed: return false;`
- A `while (1)` with the exit test inside and `failed: return false;` at the end of the
  body (the trick that fixed `Receive_51F010`).

Also tried: a `char_type` return type (no change). Not tried: a result variable.

## NetPlay::ReceiveGameMessage_521890 (WIP)

The in-game message pump. It first drains buffered out-of-order packets (`sub_521770`, which
matches now),
otherwise reads new ones with `Receive_51F010`. Game packets (type 3) are compared with
the sender's and our own 8-bit sequence numbers (`SeqDiff`, an inline helper in NetPlay.cpp): old ones
are dropped, the next one is accepted (`sub_521820`, sequence++), anything later is
buffered (`Add_5216E0`). It stops on a message, `field_8F0` or after 500 ms, and stores
the time taken in `field_8F4_time_diff`.

Ratio 0.419 (0.284 before `SeqDiff` returned `diff - 0x100` directly). The logic lines up,
but our frame is 0x1C bytes instead of
0x18, so every stack offset differs. The original has six dwords: one holding the three
byte locals (`bGotMessage` 0x11, `bCheckBuffered` 0x12, `seq` 0x13), then pData,
senderId, the start time (also kept in `ebp`), recvId and the length. Ours keeps
`pType` in `ebp` and gives the start time its own slot.

Tried: declaring the byte locals together before the others (no change).

Oct 5 (240 lines): the original reloads `pPlayerIdx` into `edi` each iteration (ours hoists it into
`ebp`) and keeps the start time in `ebp`. Stack: pData 0x14, senderId 0x18, startTime 0x1C, recvId
0x20, dataLen 0x24.

## Network_20324::EnumerateMaps_51BFA0 (WIP)

Finds `data\*.mmp`, reads each file's GMP/STY/SCR/description/player count, keeps
the maps whose files exist, and bubble-sorts them by description. Rewriting it from the
asm also fixed `Network_Enumerated_Map`: the description is a 260-char string at 0x30C
(there is no player count there), 0x410 is the .mmp file name and 0x514 the player count.
The old body's sort also swapped with element 0 instead of `[j]`.

Ratio 0.891 (0.480 before). Left:
- `this` is spilled to the slot at pre-push offset 8 in the original and 4 in ours: the
  original has the second loop's down counter at 4.
- The first loop's `map_count++` is a load before the `strcpy` plus `inc`/store after it
  in the original, and a single `incl` at the end in ours.

Tried for the second point: `for (; FindNextFileA(...); map_count++)` (no change),
`map_count++` before the `strcpy` (0.879), `strcpy((pIter++)->...)` (0.852).

## PedGroup::MergeWithOtherGroup_4C9B60 (WIP)

Makes the members of this group follow `pPed` (objective 20), or pair them up with the
members of `pPed`'s group. The original never sets a return value (`char_type` return,
C4716 silenced). The bit-2 tests are `mov/shr $2/test $1`, which comes from an inline
returning the bit (`Ped::GetBit2()`), not from `field_21C_bf.b2` directly (`test $4`).
The `or $4` and `field_14C` stores are `SetBit2_403950` and `set_field_14C_403AE0`.

Ratio 0.724. Left: register allocation in the first loop. The original keeps `pPed` in
`edi` and spills the list pointer the loop walks to `pPed`'s arg slot. Ours keeps the list
pointer in `edi` and reads `pPed` from the stack.

Tried: `pMember` at function scope (no change); reading `pPed->field_164_ped_group` in
both branches instead of a shared `pOther` (no change); `s8 i` at function scope (no
change); no `pMember` local in the first loop, just `field_4_ped_list[i]->` (0.758). That
last one gives `pPed` `edi` but reloads the list entry every time.

Oct 5 (104 lines): `field_30 = 1` before the branch matches the prologue but the first loop still keeps
the list pointer in `edi` (122).

## PedGroup::CoordinateGroupCarEntry_4C9F00 (WIP)

When the leader is getting into a car, sends up to passenger-count members to the car
doors, picking a door with an inlined search over `byte_620838` (one variant for
`SWATVAN`/`bank_van`, which starts one door further on and also tries door 1). The rest
are told to wait (objective 8). Otherwise, handles the leader waiting for the farthest
member and updates `field_30`. The rest of the code matches instruction for instruction.

Ratio 0.861. Left: block order. The original lays out the van search (then-block) first,
with its door-0/door-1 fallback right after it, then the other search. We get the other
search first and the van's door-1 fallback at the very end.

Tried: `if (van) {...} else {...}` and the inverted `if (!van) {...} else {...}` (VC6
emits the same code for both); `switch` with the van cases first (0.837, `default` still
first). `Fix16 distance` at function scope instead of in its block fixed the frame size (the
original doesn't overlap it with the spilled `this`).

## RouteFinder::NoRefs_589210 (WIP)

Checks whether the tile one step from (x, y) in `direction` (1, 2, 4 or 8) is inside a
junction's bounds. For any other direction `dx`/`dy` stay uninitialised and the original
"loads" them from the `y` arg slot (C4701 silenced). The bounds check is an inline
`Junction_10::ContainsPoint(s16 x, s16 y)` (arguments evaluated right to left, so `y` first).

Ratio 0.923. Left: inside each case the original sets `dx` (`edx`) before `dy` (`ecx`).
Writing `dx = ...; dy = ...;` gives `dx` `ecx` and changes most of the function (0.385), so
the source has `dy` first and the two stores come out swapped.

Tried: swapping the declaration order (no change either way); locals instead of the
inline (same ratio).

## struct_4::TakeClosestSprite_5A6EA0 (Object_3C.cpp, WIP)

Removes the list entry nearest to (x, y) and returns its sprite. The distance is
`Fix16::Max_44E540(Abs_negate_out_of_line(xd), Abs(yd))`: the original inlines the `y` abs
but calls `Negate_4086A0` for `x`. The `Max_44E540` result goes into a `distance`
declared before the loop, and `pPrev` is declared before `pClosest`/`pBeforeClosest` (both
needed for the registers).

Ratio 0.975. Left: `xd` is stored to its stack slot after the `y` load in the original,
before it in ours.

Tried: `yd` computed first (0.924); a `Sprite*` local for the entry (no change); `xd`/`yd`
declared before the loop (0.855); `yd` or both passed as temporaries (0.975 / 0.861).

## Map_0x370::sub_4E4820 (WIP)

Returns false if any block in a `Fix16_Rect` (rounded with `Round_To_Int_410BF0`, z from
`field_10_low_z.ToInt()`) has a slope type (`field_B & 3`) other than `slope_type`. Also
stores each block in `gBlockInfo0_6F5EB0`. The code is the same as the original's
apart from registers.

Ratio 0.614, almost all of it register rotation: the original keeps `pRect` in `edi` and
the inner `x` in `ebp`; ours has them the other way round. That also rotates the `al`/`cl`/`dl`
temps in the four cases.

Tried: 7 of the 24 orders of the four rounding statements (0.614 or 0.489); `x`/`y`
declared at function scope (no change).

## Police_7B8::TryCreateRoadblockAt_577370 (WIP)

Sets the roadblock guard type from the wanted level, finds the ground z at (x, y) and
creates the roadblock in the first free slot (type 3 for road types 1 and 2, else 2; the
second slot always uses 3).

Ratio 0.376. The main difference: the original builds the `y` argument of
`FindGroundZForCoord_4E5B60` inline (`and $0xFF; shl $0xE`) but `x` with a call to the
out-of-line `Fix16` constructor at 0x45C4E0 (`FromInt_45C4E0`), constructing straight into
the argument slot. Both constructors are inlined in our build, which moves every register
after that point. It looks like the "only the first call gets inlined" quirk, but here it
doesn't happen.

Tried: splitting the `FindGroundZ` call and `.ToUInt8()` into two statements (0.367);
`x` as `s32` so it uses `Fix16(s32)` (0.367).

Since then 113 -> 30 lines (implicit u8 -> Fix16 gives `FromInt_45C4E0`, separate case 3/4 bodies). Oct 5:
the two identical `r2.create(3)` calls should merge as in the original, but the argument is in `eax` in
one and `edx` in the other. 400 permuter iterations: nothing.

## Ped::sub_45EA00 (WIP)

Removes a ped that has been off screen for a while (`GetOffscreenCounter() > 30`, compared
unsigned). A group leader takes its whole group with it when all members are in cars or
far away. Other members leave the group. Then the ped is deallocated and bit 10 of
`field_21C` is cleared.

Ratio 0.619. Left: in the "all far away" branch the original keeps the first member in
`edi` across the counting loop (so it pushes `edi`) and keeps the counter in `cl` as well
as storing it to its slot each time round. Ours re-reads the member and does `incb` on the
slot. The locals also sit one dword lower (`bool` at 0xF and the counter at 0x10 in the
original).

Tried: one `u8 i` shared by all three loops (no change); separate block-scoped counters
declared counter first (no change).

## Frontend::sub_4B7E10 (WIP)

Draws one of 12 frontend key labels (`Find_5B5F90` keys, the last four prefixed with ": "),
with `DrawText_4B87A0` or, if `palette != 0xFFFF`, the paletted `DrawText_5D8A10`, and
returns `GetMaxTextWidth_5D8990`. It's a static `__stdcall` (5 args, no `this`), despite the
old note in `Frontend.cpp` saying it isn't static. Writing each case out in full gives the
original's shared `push key; jmp common` tails. The `Fix16` x/y arguments are built by the
out-of-line `Fix16(u16)` constructor in both our build and the original.

Ratio 0.981. Left: in both branches the original loads `text_xpos` before the `push ecx`
that reserves its argument slot, and we load it after.

Tried: `text_xpos` as `s32` with a `(u16)` cast at the calls (no change).

## Map_0x370::FindNearbyBlockOfType_4E4930 (WIP, was sub_4E4930)

Spirals out from (x, y) through the search globals `dword_6F6164`/`6F6148`/`6F613C` until
`IsSearchBlockOfType_4E4AC0` finds a block of the type, then writes the position back.
Both are static `__stdcall` in the original (no `ecx` is set up for the call, and this
function takes 4 stack args). The switch cases are in source order 3, 2, 4, 1.

Ratio 0.816. Left: only the prologue. We load the `pY` argument into `edx` before the
pushes and use `eax`/`ecx` for the zero-extended bytes. The original uses `ecx`/`edx` and
loads `pY` from the stack after storing the first global.

Tried: declaring `step`/`direction` first; setting `step = 1` between the global stores
(no change either way).

## sound_obj::ProcessType3_CopRadioAndMusic_57DD50 (WIP)

Per-frame radio/music update: counts down the radio switch cooldown, runs police radio
messages (when not paused), and, if the player is in a car that isn't a train or wrecked,
picks or updates the radio emitter and switches the vocal stream. Otherwise it stores the
emitter position in the last car, frees the old emitter and resets the vocal stream.
`field_54F4`/`54F5`/`54F6` are `field_54F2[2..4]`.

Ratio 0.854, and all that's left is the "train or wrecked" check. The original calls
`IsTrainOrBoxcar_57F120`, then computes `field_74_damage == 32001` into `cl` with `sete`,
and only then tests `al` and `cl`. We branch on each one straight away.

Tried: `!a && !b`; `!(a || b)` and `a == false && b == false` with bool locals; `u8`
locals; the damage compare written inline or with `!=`; `!(a | b)` (0.661); the damage
check before the call (0.641); a file-local inline helper returning `a || b`. None
of them produce the `sete`.

## Map_0x370::sub_4DF3E0 (WIP, was STUB)

Returns the zone of `zone_type` whose centre is nearest (max-abs distance) to the block
`(xpos, ypos)`, starting from `kFp255_6F5B8C` (255.0). Ratio 0.644.

The distance is computed in plain registers: `(w & 0xFE) << 13` is `(w >> 1) << 14`
folded into one shift, which VC6 only produces when the shifts are in one integer
expression. Going through `Fix16(u8)`/`Fix16(s32)` (any cast, initializer list or `*
16384` constructor) gives `shr $1; shl $0xE`, and `Fix16::MaxAbsDistance_42A6B0`/`Max`
take references, so they spill to the stack (`lea`), unlike the original. So the body
uses `s32` maths: `(xpos << 14) - ((w >> 1) << 14) - (x << 14)` (VC6 reassociates
`a - (x + w/2)` to the same thing).

Still different: the prologue loads the global before `mov %ecx,%edi` (original after),
the loop counter is pushed from `edx` (original `eax`), the zone type byte is loaded
after `mov %eax,%esi` (original before), and the x part uses `and $0xFFFFFFFE,%ecx`
where the original copies to `eax` and uses `and $0xFE,%al` (the y part matches).
The `cmp` for the max uses the `diff_y` register rather than a separate result register.

Tried: `Fix16` locals + `Fix16::Abs` + ternary (0.533, spills), `dist = |dy|; if (|dx| >
dist) dist = |dx|` (0.644, best), the `x + w/2` grouping (same code).

## Trailer::sub_407BD0 (WIP, was STUB)

Returns `gTrailerHitchOffset_66AAC8` rotated by the cab's `field_58_theta`, plus the
cab's `get_cp1_40B560()`, as a `Fix16_Point` by value (hidden pointer, `ret $4`).
`RotateByAngle_40F6B0` is right: the original inlines the x line (`imul`/`__allshr`, `add`)
and calls `Multiply_408680`/`Negate_4086A0`/`Add_408660` for the y line.

The tail is the problem. The original adds the returned point field by field through
the pointer `get_cp1_40B560` returned (`mov (%eax),%edi; add`), with no calls. Ours:

- `return offset + cp1;` calls `Fix16_Point::operator+` out of line (0.476, kept).
- `offset += cp1; return offset;` (POD `operator+=`) inlines the tail but then the
  rotation's first `Multiply`/`+` stop being inlined (0.319).
- A named `cp1` local with `offset.x += cp1.x` (0.294), or with a static inline helper
  that adds `mValue`s (0.319): same effect.
- A named `cp1` local with `offset.x.mValue += cp1.x.mValue` (no inline call at all):
  rotation inlined again, tail close (0.458).

So every extra inline expansion in the tail pushes VC6 past an inlining limit for the
rotation. The original probably spends one fewer inline expansion somewhere else.

## Particle_4C::UpdateAttachedEmitter_state_9_10_53B670 (WIP, was STUB)

**Matched (Oct 7).** The 9.6f version shows the shape: `PolarToCartesian_41FC20(field_28_pSprite->field_0 + jitter,
radius, ...)`, the sum an `operator+` temporary passed straight in (no named `angle`). That gives the original's 0x12
slot. Then `Fix16 radius; radius = ...;` (assigned) instead of initialised: 8 more caller size units, which
`inlsim.py --scan` showed is what PolarToCartesian's first `operator*` needs to stay inline. The notes below are
from before.

Smoke/flame particle attached to a ped sprite (`field_28_pSprite`, type `ped_3`). State 9
sets sprite id `+3` and spawns a cigarette puff; otherwise it offsets the particle by a
polar vector (`FromPolar_41E210`) chosen by `field_2C_counter` (>= 60, 41..59 with a
random jitter, <= 40). Ratio 0.817.

What got it there:
- No null check on `field_8_char_b4_ptr`: test the sprite type and read the pointer, not
  `AsCharB4_40FEA0()`.
- The original calls `Ang16::Normalize_406C20` for both normalisations: build the angle with
  `Ang16(s32)` (no inline `Normalize`) and call `Normalize_406C20()` explicitly.
- `dword_6FD540 * dword_6FD4A8` (operand order picks the load order), and
  `kFP16One_6FD4A0 * dword_6FD540` in the `<= 40` case.
- `zpos += dword_6FD470` written in both the 41..59 and the `<= 40` blocks (0.695 -> 0.817);
  written once after them VC6 lays the tail out differently.

Still different:
- Stack slots: the original has the angle at `0x12(%esp)` (upper half of a dword) and
  `zpos` at `0x14`, ours has `zpos` at `0x10` and the angle at `0x14`. Moving the
  `zpos`/`offset` declarations didn't change it.
- `angle = sprite->field_0 + angle` is `add mem,%dx; mov %dx,mem` in the original, ours
  folds it into `add %dx,mem` (tried `rValue +=`, `rValue = a + b`, `angle = Ang16(a + b)`).
  A second named `Ang16` gets the right instructions but its own slot (0.667).
- The `<= 40` block copies the `zpos` add instead of jumping into the 41..59 block's copy;
  ours schedules the `zpos` load before the flags store, so the tails differ.
- Oct 5 (61): the original puts `field_0 + angle` into a new `Ang16` that shares `angle`'s slot; the
  `zpos +=` cross-jump is still missing.

Oct 5: 61 lines, still the angle add (load-add-store in the original, add to memory in ours).

## TagGameHudUpdate_4DADA0 (WIP, was STUB)

Network tag game clock: counts frames to seconds to minutes, ends the game with `g_over`
when time runs out, and flashes the HUD timer (pager) in the last seconds of each 5 minute
block and near the limit `dword_67ED24`. The minute/second globals had to become `s32`
(`jns`, `idiv`), no other function changed. Ratio 0.710.

What helped:
- `s32 rem = minutes % 5;` before the condition: the original does the `idiv` before
  testing `minutes == 0`.
- `(rem == 4 && s >= 50) || (rem == 0 && s == 0)`: VC6 threads the `rem == 4` failure past
  the `rem == 0` test itself, like the original. The ternary `rem == 4 ? s >= 50 : ...`
  gives `setge` (0.612).
- Negating the whole condition so the "not flashing" block comes first (0.647 -> 0.710).

Still different: the original lays out the first-flash path (`if (!byte_6F59C0)`) right after
the condition, then the "not flashing" block and the shared pager-clear return, then the
rest of the flash code. Ours puts the whole flash block after the not-flashing one. Also
`test $1,%dl` vs ours `test %dl,%bl` (VC6 reuses the `bShow = 1` register). Tried: the
clear code written in both branches (0.518), the flash's tail moved after the if/else with an
early `return` in the else (0.664), `% 2 == 0` (0.550).

Oct 5 (54 lines): merging the clear and return tails into one form scored 209.

## RouteFinder::ShowJunctionIds_588620 (WIP, was STUB)

Debug overlay: for each junction (1..544) with a non-zero `field_C_min_x` that the view
camera can see, projects its corner to the screen and draws its index with `%d`. Ratio 0.349.

The projection is the `Camera_0xBC::WorldToScreen_40CFC0` formula with this TU's copies of the
constants (`dword_6FFC7C / (u + dword_6FFC9C)`). Written out in the function, VC6
inlines every `Fix16` operator (0.259). Moved into a `static inline` helper, VC6 inlines
only the first multiply and calls `Multiply_408680`/`Add_408660`/`Subtract_436A00` for the
rest, as the original does (0.349). So the original most likely called an inline
projection helper (see `matching_quirks.md`).

Still different:
- The original calls an out-of-line `Fix16(u8)` (`0x45C4E0`) for the x argument of
  `IsPointInBoundaries_58CF10`, ours inlines both.
- VC6 adds a count-down register for the loop (`movl $0x220`), the original only has the
  `u16` index compared with `0x221`.
- Stack slots for the saved x/y bytes and the frame size (0x40 vs 0x3C).

## Mike_A80::sub_4FFD90 (WIP, was STUB)

Draws the profiler history as 1 pixel wide bars (5 per sample) at the bottom right of
the screen. The 5th bar reads `field_758_ary` again, not `field_8E8_ary` (a bug in the
original). Now `void`, nothing reads a result. Ratio 0.831.

The bar is an inline helper `DrawProfileBar(x, value, colour)` (0.708 written out, 0.831
as a helper). The remaining diff is in the x87 code for `left = 630.0f - x`: the original
does `fildl x; flds 630.0; fsub %st(1),%st` and later pops the unused `x` with
`fstp %st(0)`, ours folds it into `fsubrs`. Tried: an `f32` parameter (0.812),
`630.0f - x` written twice with no `left` local (0.812).

**MATCH.** `f32 fx = x; left = 630.0f - fx; right = 630.0f - fx + 1.0f;`: the kept `x` is the
second use of `fx`, folded away by CSE (see matching_quirks.md, x87 code). The clamp is
`count = field_A7C_count < 100 ? field_A7C_count : 100`: with `if (count >= 100)` the `jl` skips the
`count` reload, the ternary makes it land on it.

## Functions without target asm

These are called by stubs worked on above but have no entry in the target asm dump,
because they weren't in the source when it was made. They now have `STUB_FUNC` markers,
so the next "Dump target asm" run will include them:

- `sub_5BEED0` (Montana.cpp): converts a cycle count for `DebugDrawProfiling_4FF250`. The
  body is a guess.
- `Net_4DA9B0` (winmain.cpp, 64 bytes, `__stdcall` with 3 arguments): called by the matched
  `Net_4DA9F0` to re-send an earlier frame's inputs to one player. Empty for now.
- `frosty_pasteur_0xC1EA8::sub_511A70(s32 car_model, SCR_CMD_HEADER*)`: a guessed STUB, called from `Car_214::sub_5C8780`.

## sound_obj::ChooseRadioEmitterForVehicle_57E6C0 (WIP, was STUB)

Picks the radio emitter for the player's car: service vehicles get none (101), cars with
`field_B0` use that station, a few models have a fixed station (status 5..11 through
`FindEmitterByStatus_57F050`), and if that one is too quiet a random rule picks another.
Now `void` (the caller ignores the result). Ratio 0.724.

Notes:
- The model switch has no `default` code: the emitter local is left uninitialised and
  still passed to `ComputeRadioEmitterVolume_57EB90`. The `127 -> 0` fix-up is inside each
  case (VC6 merges the tails after the `push`).
- `if (volume < 50) { switch } else { store }` puts the store block at the end like the
  original (0.689 -> 0.724).
- `field_544C[i + 1].field_8` is read as `u32` status and `u16` `field_8`, like
  `FindEmitterByStatus_57F050`; the struct there is still wrong.

Still different:
- On the `default` path ours reloads the emitter byte from its stack slot (`jmp; mov
  0xC(%esp),%bl`), the original just uses `bl`. A separate loop counter instead of reusing
  the emitter variable made no difference.
- The search loops end with `jb top` in ours; the original has `jae <shared return>;
  jmp top` and the "found" block right after case 0's loop (case 1 jumps back to it).

## Sprite::ShowId_59EB30 (WIP, was STUB)

Debug ids drawn at a sprite's screen position: car ids, ped ids and `model:id` for objects.
Ratio 0.333 (low because of register allocation; the instruction sequence is mostly
right).

- The car block uses the inline helper `DrawTextScaled_4BA2C0` (moved above this function). Its font
  parameter must be `u16` (stored with `mov %dx`), and it computes `scale_y` first and
  `x * scale` inside the `DrawText_5D8A10` argument list: then VC6 inlines the y multiply
  (`imull (%esi)`) and calls `Multiply_408680` for x after pushing the other arguments,
  like the original.
- The ped and object blocks call `DrawText_5D8A10` directly with `xpos * scale`; the
  original uses `__allmul` there (both operands widened), which is what VC6 produces
  for this form.
- The screen size globals are converted as unsigned (`fildll` with a zero high dword):
  `(f32)(u32)window_width_706630`.

Still different: the original keeps the object pointer in `ebx` and a zero in `ebp`, and
does the `<< 14` right after each `__ftol`. Ours keeps a zero in `ebx` for the whole
function, reloads the car pointer from `this`, and delays the shifts. The three
`DrawText_5D8A10` calls also get tail merged in ours.

## PoliceCrew_38::sub_571540 (WIP, was STUB)

Shut-down for a crew with a car (`EmergencyCrew_30::field_24 == 2`): same shape as the matched
`TryDespawnOffscreenCrew_571350`, with the car's `field_76_last_seen_timer > 200` checks and the despawn state
set to 4. Returns nothing (now `void`). Ratio 0.867.

The code is identical apart from three `je`s in the "car, no ped" branch: the original jumps
to the `field_2C = 1` tail right after the `if`, ours to the identical tail of the last block
in the function, which makes them near jumps (12 bytes longer). Tried: `field_2C = 1` in
both branches (0.812), an explicit `return` after it (no change).

## PoliceCrew_38::sub_571A30 (WIP, was STUB)

Shut-down for the other crew kinds. If the player is driving, the crew either gets back
into its car (group members flagged `field_238 = 3`) or, once everyone is ready, leaves
and despawns; otherwise like `TryDespawnOffscreenCrew_571350`, with `field_7C_uni_num == 2` cars kept. Now
`void`. Ratio 0.748.

- `gCurrentCrewPed_6FEDDC == pCar->field_54_driver` (the global on the left) gives the original's
  `cmp 0x54(%eax),%ecx`.

Still different:
- The original keeps the `field_28 = 5; field_2C = 1` return block right after the first
  group path and the other paths jump back to it; ours puts the shared copy elsewhere.
- In the first member loop the original does `inc %dl` and the store before reading
  `field_4_ped_list[i]` (same loop as `TryDespawnOffscreenCrew_571350` otherwise). Rewriting it as a `while`
  with `u8 idx = i++` made it worse (0.655).

## PoliceCrew_38::sub_575310 (WIP, was STUB)

Crew state handler when it has caught up with the criminal: on foot the player ped gets
objective 27 once close enough, in a car it depends on the car's `Hamburger_40` path
kind (`field_C`: 15 ends the chase, 0/1/2/14 keep going, others set `field_78`). Ratio 0.382
(mostly stack slot and register differences).

- The four positions are loaded before the `field_168_game_object` test in the original,
  so they are locals (player y/x first, then the criminal's).
- The distance uses the out-of-line `Abs_436A50`/`Max_44E540` in the on-foot branch and
  `Abs_negate_out_of_line` + `Max_44E540` in the car branch.

Still different: the frame is 0x20 vs 0x14 (the original keeps the max result in `eax`
and never stores `dist`), and the criminal's x/y go to the other registers. Writing the
`Max` call inside the `if` made VC6 evaluate the threshold first (0.345).

## Weapon_30::fire_truck_gun_5E0E70 (WIP, was STUB)

Fire truck water cannon: gun sprite (model 114) angle plus `word_706DFA`, muzzle offset
`dword_706CDC` rotated by it plus the car offset `dword_706CD8` rotated by the car angle,
then the spray particle (`field_4 == 0`) or bullet 199. Ratio 0.375.

The structure is right; the difference is inlining. In the original both
`RotateByAngle_40F6B0` expansions call `Multiply_408680`/`Add_408660` out of line, and only
the first rotation's `-x_old` is folded (inline `Negate`, known 0); the second calls
`Negate_4086A0`. Ours inlines the first rotation's x line like the other Weapon_30 car
guns. Not tried yet: writing the rotations with explicit `Multiply_408680` calls.

Oct 5 (10 lines): an `operator+` shape like `fire_truck_flamethrower_5E0B10` matches the start, but past
the budget it calls the unnamed `Ang16::Normalize` copy.

## Ambulance_110::ProcessPatientQueue_4FA500 (WIP, was STUB)

- 0.986 after cpp_permuter (scope_block): everything from the `x/y/z` locals to the new-task code went into its own `{ }` block, and `1 == field_18` / `get_cam_y()` were applied.

Takes the next patient off the queue: dead/invalid ones are re-queued, then it finds a
road tile near the patient and either hands the patient to an active ambulance that is
close enough (`dword_6F6FC0`) or starts a new paramedic crew. Ratio 0.867.

- `if (f28 != 6) { if (f28 == 5) ... }` as two tests, like the original.
- The distance is `Max_44E540(Abs_negate_out_of_line(dx), Abs_negate_out_of_line(dy))`
  (out-of-line `Negate_4086A0` and `Max_44E540`).

Still different: the scheduling of the `dx`/`dy` loads (the original loads the ped's y before
`field_1`, ours after) and of the `lea`s around the two `Negate_4086A0` calls; named
`abs_dy`/`abs_dx` locals gave the same code. Ours is 1 byte longer.

## Particle_8::EmitElectricArcParticle (0x540320, WIP, was STUB)

Spawns one state 37 particle at a random angle (the first `get_int_4F7AE0(3)` result is
unused in the original too). Ratio 0.456; everything up to the rotation matches.

The rotation is where it differs: the original inlines `y * sin` in the x line, keeps a
stack copy of `x_old`, and calls `Negate_4086A0`. Ours calls `Multiply_408680` for all four
products and inlines the negate (and the frame is 4 bytes smaller). Same inlining-budget
problem as `Trailer::sub_407BD0` and `Weapon_30::fire_truck_gun_5E0E70`; the matched
`EmitBloodBurst`/`EmitWaterSplash` siblings are still WIP for probably the same reason.

Oct 5: 20 lines. The rng/word multiply operands sit in `eax`/`ecx` swapped: the original moves the rng
result into `ecx` before loading `word_6FD5CC` into `eax`.

## Map_0x370::sub_4E8370 (WIP, was STUB)

Removes block `z` from a map column for `RemoveBlock_4E8940`: either into a new copy of
the column (original map columns) or in place (columns added at run time), dropping the
blocks above (`do_drop`) or leaving a hole. Returns the column index or -1. Ratio 0.342.

- Using a `u16** pColumns = field_0_pDmap->field_40008_pColumn;` local for both the old and
  the new column, and the column's `field_0_height` directly instead of a `u8 height`
  local, keeps the column base in a register like the original (0.245 -> 0.342).

Oct 5: 392 -> 104 lines, with the prologue and frame matching: `s32 height` instead of `u8` (no
extra slot), `offset` declared first, no `pColumns` local, the `pNew`/`field_360` updates written in
each `do_drop` branch.

Still different: the loop bound `h - off` loads `off` first in the original, and the `do_drop` test
is `cmpb mem` there vs ours `mov al; test al`. 600 permuter iterations: nothing (`remove_stmt` "won"
by deleting `field_0_height--`, rejected).

## Garage_48::ParkCarAtDoor_534700 (WIP, was STUB)

Sets up a PARK command: stores the car and door, picks a half size from the car sprite's
height, and builds the parking rectangle (`field_18..field_24`) and target point
(`field_30`/`field_34`, now `Fix16`) for the door's face (1..4), with an extra
`dword_6FD124` margin for double doors. Returns `field_3E` (the caller ignores it).
Ratio 0.239; the arithmetic in each case matches, the rest is register allocation.

Still different:
- The original keeps the half size in two registers (`eax` and `ebp`) and `dword_6FD124`
  in `edx`; ours merges the two half sizes and keeps the constant in `ebp`. Written as two
  locals set in both branches.
- The original stores the zeros and ones at the top from `eax`/`ebx`, ours uses
  immediates (it doesn't use `ebx` at all).

## SpawnCabAndTrailerHelper_408370 (WIP, was STUB)

- 0.226. Added global `gTrailerCabOffset_66AAE0` (Fix16_Point). The logic is three `RotateByAngle_40F6B0`
  rotations (hitch by rot, the cab offset by rot, negate it, then by uknown_rot), two `Fix16_Point +` and an
  `atan2_fixed_405320(-off.y, -off.x)`.
- Our out-of-line call sequence matches the target except:
  - Target inlines the first `Fix16 +` (the x of rotation 1). That keeps the first product in edi, so there is
    an extra `push edi` and the frame is 0x3C instead of 0x40. We never inline any `+` inside RotateByAngle.
  - Target calls `Negate_4086A0` twice for the offset negation. Ours inlines `-x`/`-y` when written at
    function level. `= -offset` (Point operator-) gives a call to 40ACB0 instead, and
    `= Fix16_Point(-x, -y)` calls the POD ctor.
- Writing rotation 1 by hand inlines everything (0.04).

## sound_obj::HandleVocalStreamSwitching_57DF10 (WIP, was STUB)

- 0.543. The `a2` param is unused. The station voc index ternary must be written as an if/else of two
  `PlayVocal_58E510` calls. VC6 then tail-merges the call and pushes `bFast` / the immediate `101` directly
  (0.28 → 0.48). The `field_14 * 1000 / field_8` needs a `(u32)` cast for `div`.
- Two bools are zeroed at entry (al/cl): `bFast` (reused for `field_54FC == 1`) and `bStationChanged`, both
  set to true when `field_54F7[1] != field_54F7[0]`. The target then jump-threads: on `field_5500 != field_54FC`
  it tests al and jumps straight to the station-restart block.
- Remaining: target block layout is [equal → test cl → update | station block] then [test al → station | mode
  block]. Tried `if (changed) … else if (!=) …`, `(== && changed) || (!= && fast)`, and a nested form; all ≤ 0.543.
  Target also keeps `bFast` spilled to the stack (reloads it before `push`), while ours keeps it in bl.

## Frontend::sub_4B4EC0 (MATCH, was STUB)

- Reads the plyslot svg header and checks its map/sty/scr names against `"data\\" + field_C9E8_blocks[main][bonus]`.
  Then it passes them to gLucid_hamilton. Retyped `svg_stru` 0..0x4A as three `char[25]` names.
- Every instruction matched at 0.867 except the frame size. The block-scoped `plySlotIdx` and `len` made it 1.0
  (see matching_quirks.md, "Stack slot sharing needs block scopes").

## Start_NetworkGame_5E5A30 (WIP, was STUB)

- 0.429. Added `gNetworkGameSettings_707098` (NetworkGameSettings). It overlaps the existing `gNetworkGameSpeed_7071A0` /
  `gNetworkPolice_7071B0` in Game_0x40.cpp (game_speed / police_on fields).
- The registry part reads `UseProtocol` (GUID) and `UseConnection` (a `Connection_Unknown` buffer via
  `operator new`), and `ModemNumber` when `gNetPlay.field_4` is set. Then it runs `Network_20324` UI →
  `CopyGameSettings`, the map/sty/scr names, and `SetMultiplayerParams`.
- Remaining 1: every `return 0` in the target is `xor al,al; jmp <shared epilogue>`, while ours duplicates the
  EH epilogue at each return. Wrapping in `if (bStartNetworkGame)` or moving the registry part into a
  `static inline` helper did not change it.
- Remaining 2: target frame is 4 bytes bigger. There is an unused dword between the GUID (0x10) and the
  `data\%s` path buffer (0x24).

## Car_214::sub_5C8780 (WIP, was STUB)

- 0.431. Now `void` (callers ignore the result). `Car_18::field_C` is now `s32` (it's a 2/3 action code).
  Added `kZero_705DD8`, a local `Car_18_Cmd` struct (the idx fields at 8/0x10/0x24) and the
  `sub_511A70` stub.
- It is an 8-case switch on the touch-point type (car / ped / driver / stopped ped id checks). Then
  `field_C` 2 starts a thread (`SpawnThread_5120C0`) and 3 calls `sub_511A70` + `FreeTrigger_5C8680`.
- In case 4 (cmd types 0xD4/0xD6/else) each branch must have its own full `GetBasePointer` + id compare.
  VC6 then tail-merges the compare into the case 6/7 code as the target does (0.36 → 0.43).
- Remaining: register/scheduling differences in most cases. Our case 3 car branch is tail-merged into case 5,
  and the target keeps them separate. The target also sets edi = pSprite on the "no match" paths, an
  uninitialised `pCar` coalesced with the param.

## sound_obj::ProcessOtherObjects_41F520 (WIP, was STUB)

- 0.854. A switch on the object model (map obj 5, explosion 113 with a nested switch on its
  `field_10_type_or_state`, rocket 128, conveyor 139, phones, skids 185..191, fire 197). It sets
  sample idx / volume / distances / loop, then one shared `CalculateDistance` + `VolCalc` + sample fill.
  Added `dword_61A6CC` / `dword_61A6D0` (stored by the explosion 18/19/20 cases).
- The phone rate displacement is `((u32)a2 * 8) % 760`: it really uses the `Sound_Params_8` pointer.
- Case bodies are laid out in source order. Ordering the outer cases as the target lays them out took it
  from 0.24 → 0.75, and `cpp_permuter -m exhaustive -p reorder_cases` on the inner switch → 0.854.
- A random permuter run got further by deleting `sample_index = 1;` from a branch (remove_stmt). That
  changes behaviour, so it was rejected.
- Oct 5: 68 -> 46 lines. Left: the case-18 else (`samp`, `bLoop = 0`, `vol`) should jump into case 19's
  else at `sample_index = 1`; ours cross-jumps only when the tail includes `bLoop = 0`. Also the case 13/14
  store order and a register in the sample fill. All 24 orders tried.

## RouteFinder::NoRefs_589210 (MATCH, was WIP)

- Matched by cpp_permuter in two steps. A random run reordered the case assignments and cast
  `(s16)dx` (0.923 → 0.981). Then `-m exhaustive -p reorder_saves,move_stmt,local_type,cast_operand --depth 2`
  found `s16 dx` plus `dx = 0;` before `dy = -1;` in case 1. Either change alone was worse (0.385).

## DMA_Video_LoadDll_5EB970 (WIP, was STUB)

- 0.198. The frame is 0xF8 = `Text[120]` + `Buffer[128]`, so both buffers are declared at function scope rather than inside the macro's error block. That fixed the stack size.
- What's left: each `GetProcAddress` failure block in the original jumps (`jmp`) into the identical tail of another block. VC6 is cross-jumping tails that end in `ret`.
  - With `/O2` our build keeps every block separate.
  - `/O1`, `/Os` and `/Ogs` add an ebp frame and merge every block into one path, so they're no better.
  - The other `/O2` variants we tried change nothing (`/Ox`, `/Oy-`, `/Gy`, `/GX-`, `/Ob0`).
- It may come from a library shared with other tools (the level editor?) and built with a different compiler version. gbh_graphics.cpp has the same loader macro and builds with `/Od /ZI`.

## Particle_4C::UpdateDirectedBurst_state_13_14_36_539480 (WIP, was STUB)

- 0.420 on the first write-up. It follows `field_40_pUnknown`'s object for speed and angle, rotates `(0, speed)` by `field_24_angle` and adds rng jitter (`Fix16(get_int(3) - 1) / 50`). It moves only inside the map bounds.
- In the original, all three `return true` paths share one return block; ours duplicates it each time. The original frame is also 16 bytes bigger.
- Its `x` multiply and the `-old_x` negate call `Multiply_408680` / `Negate_4086A0`, while the `y * sin` multiply is inline. That looks like the inline budget running out.

## Particle_4C::UpdateDirectedBurstSweep_state_4_539040 (WIP, was STUB)

- 0.455. This is `539480` with different constants: z offset `dword_6FD45C`, speed `dword_6FD2F4`, jitter `/ 30`. It also has an extra `ApplyScaleToDimensions_59E4C0(1 + sub_state * dword_6FD2E8, 0)`.
- Both functions got their shared `return true` block by nesting the success path under `if (sub_state != 16 && counter != 0) { ... if (zpos < limit) { ...; return 0; } } return true;`. With early returns, each failure check got its own epilogue.
- What's left in both: ebp vs edi as the zero register, and the original frame is 0x10 bytes bigger.

## Map_0x370::FindNearbyBlockOfType_4E4930 (MATCH, was WIP)

- Found by moving declarations around:
  - `step` must be declared before `direction`, and both before the three coordinate stores.
  - That order makes VC6 put `mov $1,%esi` before `mov $3,%eax`.
- The permuter's depth 2 run got it to 0.984 with `direction` first. Trying the other orders by hand found the rest.

## Particle_4C::UpdateCircularBurst_state_5_539890 (WIP, was STUB)

- 0.381 on the first write-up. Same frame as `5384C0`, with these differences:
  - `field_20` is reset to `kFP16Zero_6FD49C` after the follow block on every path.
  - There is no x offset; `off_y = -dword_6FD45C` always.
  - Cases 6 and 7 take x/y/z from the followed sprite.
  - `default` uses `field_20 / kFP16Two_6FD4A4` (inline `__allshl` / `__alldiv`). On `rng(7) == 4` it spawns a smoke particle through `Particle_8::New_53E3C0(0, ...)` with `SetType_4206F0(8)`.
- The original keeps the jitter flag in `bl` and returns `bl` (still 1) from the first early return. Writing `return bJitter;` there made no difference.
- Oct 5: 71 -> 8 lines. Case 4 dropped a redundant store, so VC6 merges it into case 5. Left: only the
  position of the `dir.x = 0` store (the original's is after the call in the shared tail); `dir.x = 0` after
  `set_id` in both 4 and 5 schedules case 4's tail differently and loses the cross-jump (71).

## Particle_4C::UpdateObjectBeamLink_state_38_538AC0 (WIP, was STUB)

- 0.147, but the structure is right:
  - Draws a beam from this sprite to `field_28_pSprite` (it must be a `code_obj1_4` sprite).
  - Splits the delta into `max(|dx|, |dy|) / dword_6FD364` steps and spawns a state-43 beam segment particle at each step's midpoint.
  - Then puts this sprite on a circle around the target. The angle is the target angle + `kAng180_6FD3EE`, plus rng jitter in sub-states 4/5.
- **Shared returns:** all three `return true` paths share one block, so it uses the nested form.
- **Point slots:** the original reuses the `src` slot for the circle offset and `dst` for the final position.
- **What's left:**
  - The original sets EH state 5 before anything else, so five `Fix16_Point`-like locals are constructed with no code.
  - Our build calls `Fix16_Point()` out of line for two of them.
  - With `Fix16_Point_POD` locals instead it drops to 0.106. A first draft without the nesting scored 0.168.
- Added `Fix16::operator/=` (out-of-line copy at 0x539F90) to fix16.hpp.
- Oct 5: 574 lines. The frame is 0x1C too big (scalar temps), and the original passes the `Ang16` sum to
  `AssignNormalized_409300` as a 32-bit temporary.

## Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280 (MATCH, see "Particle pass (9.6f unpaired counterparts)")

- 0.161. The spark sits on `field_28_pSprite`:
  - **Car:** the spark goes at a corner of the car's box (half width/height plus constants, sub-states 1–4), rotated by the car's angle. State 40 and 41 use mirrored corners. The id is base + sub_state + 200.
  - **Ped:** an offset rotated by the ped's angle plus `Char_B4::field_98`. The id is base + sub_state + 197.
- Needed `#include "Car_BC.hpp"` (compare_builds is unchanged) and three new globals (`dword_6FD3C0`, `dword_6FD5A8`, `dword_6FD2F8`).
- **Open question:** our build calls `Fix16_Point_POD::Fix16_Point_POD()` out of line for the `Fix16_Point offset` local. The original constructs it inline, as it does for all five points in `538AC0` (where we call it twice). The ctor is empty and the struct isn't exported. Unexplained so far.
- Oct 5: 160 lines. The original's two rotation branches put sin/cos in swapped stack slots and aren't
  tail-merged; ours are merged.

## Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0 (WIP, was STUB)

- 0.313 on the first write-up. An impact burst attached to a car or ped (`field_28_pSprite`):
  - **Ped:** the source velocity is the ped's `GetVelocityVector_45B520`.
  - **Car:** the velocity is `GetPointVelocity_561350` at the gun-model (114) or fallback-model (248) attachment point. That is the same attachment code as `Particle_8::EmitImpactParticles_53FE40`.
  - The burst moves along a random spread angle (`/ 71`, `Normalize_406C20`).
  - **On a hit:** it moves `Particle_8`'s `field_0` (state 31) or `field_4` (state 34) to the hit. It also sets the damage owner from the shooter's `field_267_varrok_idx`. On a ped hit it calls `HandleGenericImpact_553E00` and `frosty_pasteur::RecordWeaponHit_512C00(id, 194/198, 1)`.
- **Return block:** every failure check in the original jumps to one shared `return true` block. Ours uses early returns, so the next step is the nested form.
- Uses `Fix16_Point::RotateByAngle_40F6B0` for the rotations. The other `Particle_4C` WIPs write the same rotation out by hand.
- Oct 5: 860 -> 488 lines. Left: stack slot order, frame 4 bytes bigger (the original spills `pB4`/`pCar` to
  0x1C/0x20).

## Particle_8::GunMuzzelFlash_53E970 (WIP, was STUB)

- 0.120, with the right structure. It spawns the corner sparks that `Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280` then updates:
  - **Car:** two sparks (states 40 and 41) at the mirrored corners of the car box.
  - **Ped:** one spark (state 40) at the `Char_B4::field_98`-relative offset.
  - It does nothing when `bSkip_particles_67D64D` is set.
- The original computes `sin * dword_6FD2E8` and `cos * dword_6FD2E8` into a temporary and never uses them in the car branch. That is kept as `unused`.
- **What's left:**
  - The original's EH state is 4 before the first branch and 7 inside the car branch, so there are more destructible locals declared at the top than ours.
  - Register choice for the zero registers (ebp/ebx swapped).
- Oct 5: like `EmitFlameStreamSegment_53F4C0`, the original has EH state 4 with five `Fix16_Point`s (frame
  0x4C). All 120 declaration orders were worse.

## Weapon_30::sub_5DE4F0 (WIP, was STUB)

- 0.262. This is the electro-baton beam: it aims `gObject_5C_6F8F84->field_58` from the owner ped to `field_198`, its target.
  - If the distance is more than `dword_706EC4`, it drops the target.
  - Otherwise it steps the beam along the line (`dist / dword_706CF0` steps, at least `dword_706EBC`), stopping at map walls (`sub_5A2440`).
  - On a car hit, or a hit on a ped not in states 8–9, it sets the weapon's `field_4`.
  - Then it marks the target as shot by the owner and calls `sub_5DE910`.
- `sub_5DE910` (0x772 bytes) was missing from the source entirely. It's `void __stdcall (Fix16_Point by value, Fix16_Point&, Fix16 z)`, worked out from the call site, and now matches. Both point arguments are `get_x_y_443580()` of the target's sprite.
- Changed the return type to `void`, since nothing sets one. `electro_batton_5E0740` still matches.
- Oct 5: 230 -> 192 lines. Left: frame 0x28 vs 0x24; the original's atan2 temp, `dist` and `u8 i` share
  0x10, and the angle is in `bx`.

## Weapon_30::sub_5DFB60 (WIP, was STUB)

- 0.146. This is the shocker's chain lightning:
  - It collects the sprites in a box around `a3` (`gPurpleDoom_1_679208->CollectRectCollisions_477F30`). The box is `kFP16Two_706EC0` wide for the first link and `dword_706EBC` after that.
  - For each ped or car within the angle window (`word_706D6C` / `word_706E28` around `a4`), that has map line of sight (`sub_4E5640`) and isn't already hit (`gWeapon_8_707018` list), it does three things. It draws the arc (`sub_5DE910`), recurses once (`a2 < 1`), and shocks the ped or damages the car. The car damage is `AccumulateDamage_43DA90(300)`, with scoring for the player.
  - At the end it takes one ammo with a 1/2 chance.
- The ped branch normalises the angle difference out of line (`Normalize_406C20`). The car branch normalises it inline (the `Ang16 operator-` normalising ctor). Written to match.
- Both branches compute `Max(Abs(back_x), Abs(back_y))` and never use it. In the ped branch the y subtraction is the out-of-line `Fix16::Subtract_436A00`.
- Changed the return type to `void` (nothing sets one). `shocker_5E06B0` ignores it.
- **What's left:** the original frame is 0x1C bytes bigger.

## Ped::CopyStatsFromPed_45B5B0 (MATCH, was STUB)

- About 100 field copies in the original's order, plus single-bit copies of `field_21C_bf`. The bit copies on `field_224` (declared `char_type`, used as a byte of flags) are written as `((dst ^ src) & mask) ^ dst`. Then three `Ped` helper calls, and `field_0_patrol_points[0].field_0/1 = 0`.
- Generated from the target asm, using a VC6 `offsetof` dump of `Ped` to get the field names.
- `field_1AC_cam` has to be copied one component at a time: a struct assignment copies through pointers.
- Changed the return type to `void`. The PedGroup callers ignore it.

## sub_5DF270 (WIP, was STUB)

- 0.135. This is the shocking hit search, called by `Ped::ManageShocking_45BC70`:
  - It collects the sprites in a box of width `a2` around sprite `a1` and walks them nearest first (`TakeClosestSprite_5A6EA0`).
  - With `a6` set, every hit nearer than `a6` must be inside the angle window. Otherwise it gives up and sets the weapon's `field_4`.
  - Then it shocks the peds outside the window (or every ped when `a3 == 0`) that have line of sight, and draws the arc with `sub_5DE910`.
  - With `a4` set it uses `TakeDamage(3)`. Otherwise it adds 3 to the shock counter and stops after the first ped.
- Moved from Ped.cpp to Weapon_30.cpp, where its address falls (between `sub_5DE910` and `sub_5DFB60`). It's declared in Weapon_30.hpp.
- Now `void`, with `a6` typed as `Sprite*` (it's compared with the hit sprites). `ManageShocking_45BC70` still matches.

## Garage_48::GaragesService_5349D0 (WIP, was STUB)

- 0.417. The park-in-garage state machine, driven by `field_C`:
  - **1, drive in:** after the car-box collision test with the garage rect (`CollisionCheck_5A0320`), lock the car and the player's controls. Push the car toward the midpoint of the two touching box corners, normalised and scaled by `dword_6FCF10`.
  - **2, settle:** keep pushing until the box is fully inside, or until 300 ticks pass. Then zero the velocity for `field_3C` (30) ticks.
  - **3, parked:** close the doors, kill the passengers, walk the driver (or `field_14`) out to the target, and reset the garage.
- `field_28` / `field_2C` were two `s32`s, but the code uses them as the push vector. They're now `Fix16_Point_POD field_28_push_dir`. The ctor (MATCH 0x534E80) still matches with `.x` / `.y`.
- `sub_5345E0` (0x55: door face to ped heading, an `Ang16` returned through a hidden pointer) was missing. It's now a `STUB_FUNC`.

### sub_5345E0 (0x5345E0): MATCH
- Door face to `Ang16` heading: a 4-entry jump table over four `Ang16` globals, with the default sharing case 4's block.
- `case 4: default:` gave an if chain (0.57). A separate `case 4` plus `return` after the switch gave the table (0.62). The permuter then moved `case 4` to the top to fix the register alternation (1.0).

### sub_405DA0 (0x405DA0): MATCH, first try
- Turns an angle toward a target by at most `*pSpeed`, the short way round, then wraps it into [0, 2pi). It's the same shape as `SmoothApproachAngle_405CE0`, written with the same `Fix16` globals and the same two wrap loops that `WrapAngle_40E790` uses.

### sub_405E20, sub_405E80 (0x405E20, 0x405E80): MATCH
- `sub_405E80` clamps an angle into a +-`dword_669140` window around a target, allowing for the wrap. It then returns whether the angle sits on either edge, tested with `sub_405E20`.
- `sub_405E20` (unmarked until now) only matched with an `s32` return (`mov $1,%eax`). The caller still tests `%al`, so the calls are cast to `(u8)`. `sub_405E80` returns `s32` too.
- The permuter did the rest of 405E80: assign `lo` before `hi` in every case so the shared `*pCur = lo` block tail-merges (0.63 to 0.93), and read `*pTarget` directly instead of through a local `t`, which fixes the `lea` operand order (to 1.0).

### Fix16_Point_POD::AddAssign_5E40C0 / DivAssign_5E40E0 / MaxAbs_5E4140: MATCH, first try
- These are out-of-line copies emitted after Weapon_30.cpp's functions, used by `sub_5DE910`: `+=`, `/= Fix16` and max(|x|, |y|). The csv names them `Fix16::sub_...`, but they're `Fix16_Point` members (`this` is a point).

## Round: remaining non-CRT stubs (Oct 2)

### Net_4DA9B0 (0x4DA9B0): MATCH, first try
- Re-sends one earlier frame's inputs to one player: fills `gInputSendData_6F5B18` (length 8
  with the sync check, else 4) and calls `NetPlay::SendToPlayer_521630`.

### frosty_pasteur_0xC1EA8::sub_511A70 (0x511A70): MATCH
- Looks a car model up in the script's car list (`field_340_car_list`, read as bytes) and sets
  the generator's type from `kDecidePowerupGenTypes_6212F0` (0x41 when it isn't in the list). The second argument
  is a `Generator_2C`, not a `SCR_CMD_HEADER`.
- It uses `gfrosty_pasteur_6F8060` instead of `this`. 0.957 to match by writing the compare as
  `car_model == *pList` and the increments as `pList++, i++`.

### FileByteSum_4DB120, sub_4DB2E0, FatalErrorMsg_4DB410, CompareRemotePlayers_4DB440: MATCH, first try
- The network sync check. `sub_4DB2E0` fills a packed 0x1F-byte `SyncCheckData_1F`: debug
  flags, level file size from `_stat`, byte sums of the script and `data\nyc.gci`, language and
  player index. `CompareRemotePlayers_4DB440` quits through `FatalErrorMsg_4DB410` (video off,
  message box, `exit(1)`) naming the player and file that differ. The original passes an extra
  unused argument to the last `sprintf`.
- The format strings and the two helpers needed the asm dump. `dump_target_asm.py` on
  `claude/target-asm-request` now also dumps strings (and other data) referenced by
  `push $imm`/`mov $imm` into `target_data.json`, cut at the NUL when it looks like text.

### Car_6C::SpawnCarOnRoadNetwork_4458B0 (0x4458B0): WIP 0.580
- Finds a junction near the point (`RouteFinder::sub_58A130`, whose `s16` result the original
  narrows to `char`), heads out of it, then walks the road like `SpawnBusAtValidRoadPosition_4453E0`
  until a free, off-screen spot is found. The car gets an AI following the route.
- Left: the dead-parameter-slot assignment (see matching_quirks.md). A `dir` local for the
  junction switch, `ToInt()` written inline and the permuter all made it worse.
- Oct 5 (449 lines): the junction switch on a `dir` local (edi) with `default: dir = road_direction` (469
  alone). Blocker: the corner switch's merged case tails (the original's down_2 jumps into up_1, ours the
  reverse).

### sound_obj::PoliceRadioMessageGeneration_426790 (0x426790): MATCH
- Picks the most serious crime reported this frame (`CrimeReportQueue_CC::TryPopOldestReport`, crime 9
  only when nothing else was reported), counts down the radio timers, then queues the dispatcher
  lines for a new wanted level or one random chatter line.
- 0.998 on the first build. It matched once `best_crime = 0` was declared before the two
  `u8 = 0`s. It also needed the empty `sound_obj::nullsub_4` (0x427330) and CrimeReportQueue_CC.hpp.

### Police_7B8::UpdatePursuitTargets_56FBD0 (0x56FBD0): MATCH
- Updates each call for service: wanted level from the criminal's stars, then the state
  (send crews, add SWAT at 4 stars, stand down at 5/6, clean up when the criminal is gone, and
  case 5 re-sends crews when the criminal gets away from the searched spot).
- 0.590 -> 0.629: share one `count` and one index `j` across the loops (the frame was 0x2C).
  0.690: state 5 before state 4. 0.802: case 0's inverted `if`. 0.924: the distance check
  written out. Match: `if (!(dx > dy)) dx = dy;` instead of the ternary.

### eager_benz::OnPedKilled_592660 (0x592660): WIP 0.343
- Scores a ped kill, the ped counterpart of `OnCarDestroyed_592DD0`. Points depend on the victim's
  occupation (cop, army, SWAT, FBI, gang members, Elvis with his own counter) and the kill type
  (`field_290`); network kills of players use a separate table. Then the exploding score, cash
  and the crime report (6/7/8/9).
- The original lays the kill-type switch out inside the occupation switch, and the Elvis case
  jumps past it. There's a `goto` for now. The six occupation flags live in two dead parameter
  slots and two stack bytes. `this` and the victim ped get each other's registers.
- Oct 5 (376 lines): the occupation switch layout (kill-type switch after the army case, the others jump back
  into army's zero stores) and `movb $0` vs ours a zeroed `bl`. Seven case orders and flag types didn't help.

### Char_B4::HandlePedCollision_548BD0 (0x548BD0): WIP 0.592
- A 5x5 switch on both peds' `field_238` types. It covers Elvis followers, pushing the other ped
  away (`atan2` + 180 degrees, with `Normalize_406C20` or the inline `Normalize`), slowing down,
  turning 30 degrees, stepping back to the saved position, and cops arresting a wanted ped.
- 0.457 -> 0.592: case order from the jump tables, the `occ != 43` branch first, and the
  differences passed straight to `atan2`.
- Oct 5: 518 -> 162 lines with the atan2 angle written out at each site (0x4C frame). Left: angle2/3
  slots (0x10/0x12 vs 0x20), site 3's operand order, the last site's angle in `pOther`'s slot, and the
  `field_10 = 1` tail merged into the type 4/6 copy (the original keeps type 5's).

### MapRenderer::DrawDiagonalWall{UpLeft,UpRight,DownLeft,DownRight} (0x4EE7D0..0x4EEA40): MATCH
- The bodies were already written, but the addresses weren't in the csv, so the markers were
  commented out. They match the raw bytes in `target_extra.json` (0x64 bytes each). The calls
  and globals check out. Each now has a csv row and `MATCH_FUNC`.

### sub_5BEED0 (0x5BEED0): MATCH
- `return (u32)cycles / dword_705334;`. It needed the normaliser fix for `divl mem`.

### arc_tan_table_init_4052D0 (0x4052D0): MATCH
- The tangent table initialiser. The dump now reads the x87 constants of the `target_extra.json`
  functions too: pi and 1/720, then 16384 from `Fix16(f64)`. The two multiplies and the
  separate down counter needed the spellings in matching_quirks.md. Its csv row is new.

### sound_obj::Release_41A290, Char_B4::IsThreatToSearchingPed_553330: now verified
- Both were `MATCH_FUNC` without a csv row. They're tail-jump thunks and match.

### sound_obj::ProcessObject_Type12_41E850 (0x41E850): WIP 0.970
- A map object's sound by kind (`Object_2C::field_26`). 40 case blocks set the sample, release,
  range, distance and volume. VC6 merges their tails, so each case is written out in full, in
  the jump table's address order. Then come countdowns for the occasional kinds, the sample
  counter `byte_6751E4`, and the queue.
- 0.716 -> 0.907: the countdown `if (!w) {...} else { w--; return; }`. 0.970: the countdown
  cases in address order and an unsigned sample index. One register difference is left.
- Oct 5 (40 lines): the rate line. The original copies the first argument to `eax` and reloads `field_14`
  for `RandomDisplacement`; splitting the statement reloads but spoils registers (74). 300 permuter
  iterations: nothing.

### Ambulance_20::HandleObjectiveState_4FAAC0 (0x4FAAC0): WIP 0.961
- The paramedic crew loop. Each ped of the crew, through `gParamedicCrewPed_6F6D60`, walks to a patient in
  `field_10`, revives them (cops come back as cops), then gets back in and leaves.
- 0.485 -> 0.724: case order 14, 0, 28, 36, 16, 35. 0.835: the `field_225 == 1` branch of
  case 14 first, with an explicit `else` for the no-car path. 0.892: one `field_8 = pPatient`
  after the whole `field_23C == 99` branch, which VC6 copies into each path without folding the
  NULL. 0.961: case 35 as three branches. Left: the load order of the distance check (the
  original loads `y` first) and the copy of the objective target. A `Fix16_Vec` struct copy made
  that worse.

### PoliceCrew_38::State5_PursueOrChase_572920 (0x572920): WIP 0.396
- The chasing crew, built on the matched `State3_AlertedSearch_572340`. Members follow the criminal on foot, or
  get back in the car when the criminal is far away or fast. The crew-state check goes in the
  sibling's order (`!= 3` first). Left: the original puts the two "timer ran out" blocks at
  the end of the function, and the case tails merge differently.
- Oct 5: 217 -> 138 lines. Left: which block keeps the merged `SetObjective` tail, and the 0x8000000 mask
  in a register vs an immediate.

### Map_0x370::sub_4E5D10, sub_4E5D70, sub_4E5E00: match
- Small road helpers of the two road followers below: move x or y along a road direction, and
  the distance to the block edge across or along an angle's face. Their bytes came from the
  dump's extra address list. They take `this` but don't use it, so they're members. Their csv
  rows got the `Map_0x370::` prefix.

### Map_0x370::sub_4E6660 (0x4E6660): WIP, one instruction pair off
- Moves a point `dist` along the road, following the arrows, and returns the final direction.
- 0.56 -> 0.99 (registers counted): the "block under or at z" lookup as an inline that writes
  through a `gmp_block_info*&`. Then the frame: the reversed angle as a temporary through
  `Ang16::Normalized_406C20()`, the final z in a block scope, and `bTurned` declared before
  `x`. The early exits jump straight to the shared exit, so they're `goto done` (the
  original's tail code is shared, not duplicated).
- Left: in the `side == 0.5` branch, `sub_4E65A0(x, y, &z, 1, 1)` pushes `%ebx`, which still
  holds the 1 loaded at the top, where the original pushes `$1` twice. Literal type (`true`,
  `TRUE`, casts), an inline wrapper, a variable for the first 1 and declaration order made no
  difference. Moving `pPrev = pBlock` before the call fixes the pushes but moves the `mov`.
  The permuter's best results only shuffled jump offsets.
- Oct 5 (4 lines): a variable 1 or an inline param 1 both fold back into `ebx`.

### Map_0x370::sub_4E7190 (0x4E7190): WIP
- The reverse road follower. When it runs off the road it looks for a turn in the neighbouring
  blocks, via `gMap_0x370_6F6268` rather than `this`, and returns the opposite direction.
- Same helpers as sub_4E6660, plus the null-checking lookup. The function runs out of inline
  expansions (see matching_quirks.md), so the neighbour z offsets use raw `mValue` arithmetic
  and the arrow check and `dist` update are written out. Left: `dist` and `pPrev` swap `%ebx`
  and `%ebp`, the constant cached in `%ebp` before the first switch, and the neighbour arrow
  check's `xor`/`test`, which needs an inline the budget can't afford.
- Oct 5 (196 lines): a ternary keeps the unfolded test but moves things to `ebp` (272).
- Round 8 (skeleton 4 -> 0, structure 18 -> 6, full 196 -> 218): `KeepDir(d, want)` inline for the
  neighbour arrow check, paid for by `(last_z - kFpOne_6F6110).mValue >> 14` instead of `.ToInt()` in the
  four neighbour lookups (with all four `ToInt()` the inline pushes two `operator-` out of line, 756).
  Left: the original keeps the KeepDir result in `ebp` (`mov %eax,%ebp; cmp $3,%ebp`), ours tests `eax`
  and copies after; `dist`/`pPrev` swap `ebx`/`ebp` again; the cached switch constant is 2 instead of 1;
  a few slots. A by-reference KeepDir, a void one, `d = Get(); d = KeepDir(d)` and a result copy inside
  the inline all fold or change nothing. Case order in the two direction switches is canonicalised
  (no effect). The fully raw `(last_z.mValue - k.mValue) >> 14` keeps the registers (202) but loads the
  neighbour args in the wrong order (structure 32).

### PublicTransport_181C::SpawnTrainsFromStations_578860 (0x578860): WIP 0.90
- For each of the first 10 stations with wagons: takes a train, places the wagons and the engine
  behind the stop zone along its green arrow, takes the wagons off the car pool's active list,
  and gives the engine AI, a driver and a light. The spawns are `SpawnCarAtCorrectZ_426E40` with
  the scale passed in (`Car_6C::SpawnCarAtCorrectZ_Scaled`, with `const&` rotation and scale so
  the globals are read at the push).
- Left: the original keeps the byte of the axis that only gets the 0.5 offset in a stack temp
  across `GetWagonType_577f80` and adds it after the call. That's 4 temps, the 16-byte frame
  difference.

### PoliceRoadblock_A4::CreateRoadblock_575FF0 (0x575FF0): WIP 0.80
- Scans for the road's edges along y (orientation 2) or x, checks the rect, then fills the lanes:
  cars on the odd lanes, barrier pairs and guards on the even ones. Three inline "first free slot"
  helpers.
- 0.73 -> 0.80: z passed to the barrier and guard spawns as a plain `u8` (see matching_quirks.md).
  Left: the scan switches' case layout, the register for `dist`-like temps, and parts of the
  barrier and guard position arithmetic.
- Oct 5 (529 lines): the original puts `bEdge` in the dead `z` parameter slot (0x88); ours gives it to the
  block-type switch temp.

### DrawDebugLine_5D7DD0, sub_5A5690: match
- See matching_quirks.md for the line plotter's parameter reuse and zero step.

### NoRefs_sub_5B1170 (0x5B1170): match
- A 5 KB unreferenced test-scene builder. Transcribed with a small interpreter over the listing
  (track pushes and registers, turn each call into a statement), then one fix: the cab position
  passed as plain ints.

### Fix16 out-of-line operator copies, Fix16_Rect::MakeRect_4E6280: match
- See matching_quirks.md. `MakeRect_4E6280` stores left, top, right, bottom in that order.

### Near-miss pass, batch B (2026-10-02)

Each was a few asm lines away from the original. What is left and what was tried:

- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (0x427220): known unexplained cmp-before-volatile-load; tried volatile u32, index locals, precomputed bool, >14, cast-volatile store.
- `keybrd_0x204::GetLayout_4D6000` (0x4d6000): lea &v2 scheduled before the two KLID byte loads in orig; tried decl orders, u16 copy, store order, result local, temps.
- `jolly_poitras_0x2BC0::sub_56BA60` (0x56ba60): len=126 store vs outer loop counter init order; tried len at every position, decl order, struct copy, pStats local, 600 permuter iterations.
- `menu_option_0x82::sub_4B6330` (0x4b6330): orig reloads field_6E (cmp 0x6E(%ecx),%ax) in loop cond though nothing in the loop stores; VC6 CSEs it to old_count's reg whatever the spelling (s16 old_count, casts, volatile worse, 800 permuter iters). Same issue in sibling 0x4b6390.
- `sub_4B7E10` (0x4b7e10): xpos load before the arg-slot push; tried s32/u32 ypos/xpos params with casts, branch inversion, explicit Fix16 (inlines ctor, worse).
- `CarPhysics_B0::ShowPhysicsDebug_559430` (0x559430): lea 0x818 (field_650 this) placement around pushes; theta local, pText reorder, 600 permuter iters.
- `SetGamma_5D9910` (0x5d9910): matches (0) just by uncommenting gErrorLog_67C530.Write_4D9620, but commit e538f1b8 (Valps, 2026-09-30) demoted it from MATCH on purpose because that call crashes the standalone exe at boot. Main session/user should decide.
- `Car_BC::sub_43B2B0` (0x43b2b0): known unsolved return width (bool call result returned unextended, other paths eax); not retried beyond analysis (PedTypeIs_45EDE0 has 96 callers testing al).
- `MapRenderer::Set_UV_4F4190` (0x4f4190): fmuls (1/16384) scheduled after the idx load in orig; AsFloat/ToFloat/mValue*k/local float, 400 permuter iters (x87 scheduling, cf. Draw*Sided* note).
- `Hud_Brief_704::ClearAllBriefsWithPriority_5D4890` (0x5d4890): known: ebp shrink-wrap (pushed after null check). Code otherwise identical. Tried if+do/while, early return, break, if(pIter) Start(), decl order, 500 permuter iters. VC6 does shrink-wrap in similar matched loops (struct_4::RemoveByRngValue_5A6C40).
- `Car_BC::sub_43B850` (0x43b850): known: u16 load then test $6,%ch; tried IsFlagSet_411930 inline, local copy, shifts, casts, != 0.
- `RouteFinder::sub_589E20` (0x589e20): known: loop-top test of a flag known 0 on entry (unrotated while). Tried for(;;)+break, && condition, if/else, return in loop, char/s32 flag.
- `Object_2C::sub_526830` (0x526830): known: switch clobbers value (add $-39) and reloads param in default; tried direct returns, default return, param reassign, switch(a1-39), (u32) switch, result=a1 init.
- `Map_0x370::do_process_loaded_zone_data_4E8E30` (0x4e8e30): base+index operand order in two places (zone-info loop: offset reg then add field_334; mov %bl,(base,idx)); 9.6f has our order. Tried + operand swaps, byte offsets, for loop, local placement.

### Near-miss pass, batches A2 and B2 (2026-10-02)

- `Camera_0xBC::UpdateBoundaries_435B90` (0x435b90): no change. only diff: reg alloc in final field_20 = field_78 +/- dword_67691C block (right: field in eax/dword in edi swapped); operand order swaps have no effect; 9.6f-style rewrite (one v, checks on fields, v*=) much worse
- `Car_BC::HandleUserInput_4418D0` (0x4418d0): no change. only diff: 'cmp %bl,%al' (bl=0) vs our 'test %al,%al' after HandleRoofTurretRotation call; tried !=0, !=(char)0, ==true, !=field_B8, local zero var, combined condition; same in 9.6f
- `Car_6C::dtor_446DC0` (0x446dc0): no change. only diff: last delete (gSprite_Unused_677938) loads ptr into ecx then mov ecx->esi, ours esi then esi->ecx (Car_14 delete just above uses esi in both). Dropping the if made it worse
- `Ped::ComputeAimAngle_45C9D0` (0x45c9d0): no change. VC6 duplicates the return tail into the atan2 branch (forwarding the stored angle) where orig jmps to the shared reload tail; tried branch inversion, rValue stores, named temp, getter, 500 permuter iters
- `Ped::Deallocate_45EB60` (0x45eb60): no change. bitfield b0 clear: orig dword load, and $0xFE,%al, dword store, scheduled after the timer store; ours and $-2 on edx with field_16C load hoisted. Tried &= masks, u8 cast, reorder, inline setter (bool/u8/s32)
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (0x4b6390): no change. original reloads field_6E in the loop compare (cmp 0x6E(%ecx),%ax), VC6 CSEs it to si in ours; same in 9.6f and in sibling 4B6330. Tried s16 old, swapping old/new init
- `MapRenderer::Draw4SidedDiagonalUpLeft_4EF880` (0x4ef880): no change. (skipped) known MapRenderer Draw*Sided* x87/vertex store scheduling, not attempted
- `Ambulance_110::ProcessPatientQueue_4FA500` (0x4fa500): no change. only diff: original interleaves load/sar/store for x,y,z (as if stores may alias), ours hoists the 3 loads; tried separate decl/assign, ToUInt8, stores through u8* pointers
- `Ambulance_20::UpdateState_4FB330` (0x4fb330): no change. only diff: case 3 '>500' false path should jle back to shared epilogue (0x3F) rather than the adjacent duplicate pop/ret; tried return after state=5, inverted if, break in default
- `youthful_einstein::SetNewFugitive_516590` (0x516590): no change. original reloads field_0 into edx (not esi/ecx) before SetPlayerArrowColour; with local pPlayer VC6 reuses esi, without it uses ecx and the else branch's gHud reg shifts too. Tried field/GetPlayerPed/pPed local/ref
- `NetPlay::EnumSessions_51E650` (0x51e650): closer, 18->7. wrong flag (orig 0x80 RETURNSTATUS, not STOPASYNC); else only fails on hr<0; nested success + single return -1. Left: else jge into the modem's shared return-count block
- `struct_4::CleanupSpriteList_5A7080` (0x5a7080): no change. keep-branch block (pLast = pIter) laid out between the two unlink branches in orig; tried inverted conds, continue forms, if+do/while, nested ifs, 600 permuter iters
- `gtx_0x106C::GetSpriteTrueIndex_5AA460` (0x5aa460): no change. known unexplained (quirks list): default 'mov 8(%esp),%eax'. Tried default return direct, (s32) cast, s32 param (still ax and breaks 13 callers)
- `Montana_4::dtor_5C5F10` (0x5c5f10): no change. same pattern as 0x446dc0: original looks like an inlined scalar deleting dtor (ptr tested in ecx, push esi + mov ecx->esi inside the if), ours keeps ptr in esi; tried moving ~Montana_2EE4 after use, an inline 'delete this' helper
- `SetWindowedMode_5D9510` (0x5d9510): no change. push $0x316 scheduled before the height arithmetic in orig; all operand orders compile the same, locals much worse, 500 permuter iters

### Near-miss pass, batch D (2026-10-02)

- `Map_0x370::sub_4E6190` (0x4E6190): no change. only diff: original cross-jumps case 3/4 inner switch tails into case 2/1 (jmp into other case's dec/jne); ours duplicates. Inner case order, default:return 0, casts, if-returns: no effect
- `Gang_144::ApplyKillRespectChange_4BEF70` (0x4BEF70): closer 23->21. compare swapped. Left: original moves pGang to ecx and computes respect*reaction before the lea of field_11C[p], then reloads via full address; inline member helper, locals, casts, pointer: no change
- `Car_BC::IsSpriteShrunk_43DC00` (0x43DC00): closer 28->22. return a != b (Fix16 op in first branch, .mValue in second: matches setne/xor shapes). Left: load order/regs (esi popped early in orig)
- `Player::AddCarToHistory_5645B0` (0x5645B0): closer 29->23. for(;i<3;p++,i++){ if(!*p){*p=new;return;} } then shift. Left: esi/edi swap of the two history pointers, shift stores through a copied pointer (mov %esi,%eax) in orig; decl orders, pointer alias, struct copy, memcpy: no change
- `Map_0x370::HasGreenArrowForPathDirection_4E5E90` (0x4E5E90): no change. same as 0x4E6190: original tail-merges identical HasBlockDesiredArrow calls across switch cases (case4-else = case3-if, case1-else push 2; jmp into it). Ternary/if-chain/compiler flags don't reproduce; likely the same unexplained cross-case merge
- `miss2_0x11C::Locate_509FD0` (0x509FD0): no change. register rotation only (eax/ecx/edx) in the STOP_LOCATE_CHAR_FOOT/CAR case blocks; pObj/typed local/getter/default position/velocity local: no change
- `frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0` (0x5121E0): closer 24->22. str_count before if, single store after (empty path stores ax=0 like orig). Left: dead (len+9)&~1 in edi, tableSize test via bx instead of zero-extended stack slot, first-loop regs
- `Mike_A80::sub_4FFD90` (0x4FFD90): no change. x87 'fildl x; flds 630; fsub %st(1),%st ... fstp %st(0)' (x kept and popped) vs our fsubrs; double/f64 x, dead right/top locals, casts: no change (already in match_attempts)
- `Car_BC::ManageTVAntenna_4425D0` (0x4425D0): closer 39->11. Ang16 towerAng; declared at top (9.6f), towerAng = Get...() (T x; x = f()). Left: sprite rot is loaded into cx and reused for the conversion, orig compares against memory and reloads; operand order, s32 NotEqual (9.6f 0x41CFF0 returns s32), towerFp local: no change

### Near-miss pass, batch F (2026-10-02)

- `Type_3_HandleCarImpactSound_4174C0` (0x4174C0): closer 0.822->0.915. if chain -> switch gives the original's `sub $0`/`dec`. Left: cases 0 and 1 compute the 0x7FFFFFE3/0x7FFFFFE7 multiply in ecx, original in edx
- `TrainCab_414710` (0x414710): closer 0.684->0.961. zone read through `pTrainStation->field_10_pZone` each time, not a local. Left: original's failure jumps go to the success path's epilogue, ours to the shared one after the else
- `0x5D3B80` (DrawBrief): no change. identical except the y Fix16 ctor calls the duplicate copy FromInt_4926F0 instead of 4369F0 (duplicate-helper-copies quirk)
- `0x57DD50`: skipped, the known sete issue above
- `0x4D94E0`: no change. needs field_0 as a real ofstream member, which crashes the standalone build (see the source note)
- `0x554710`: no change. original pushes/pops ebp inside one branch (late-ebp-push quirk)
- `0x5552B0`: no change. layout only: original puts the shared `return 0` epilogue right after the loop's bottom test
- `0x541430`: no change. Ang16 sum is a 16-bit add placed before the point ctor, plus an epilogue-sharing difference

### Near-miss pass, batch G (2026-10-02)

- `Crane_15C` dtor (0x47E5B0): matches if field_0..field_20 become Fix16_Point, but then the matched ctor 0x47E610 calls the Fix16_Point_POD ctor out of line. One-for-one trade, not taken
- `Frontend::DrawBackground_4B6E10` (0x4B6E10): no change. VC6 merges the two final retry blits into one tail (see Still unexplained)
- `Ped::HandlePickupCollision_45DE80` (0x45DE80): no change. ebx/edi pushed only after the early returns (late push quirk)
- `Frontend::GetNextUnlockedMainStage_4B7270` (0x4B7270): no change. ours goes branchless and doesn't hoist the loop flag load; 1800 permuter iterations, 54 -> 44 at best
- `Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0`: skipped, on the Still unexplained list

### Near-miss pass, batch C (2026-10-02)

- `Car_14::GetRandomTrafficSpeed_583750` (0x583750): closer 14->7. shared lo/hi/factor locals and one final lerp. Left: final sum in ecx, orig eax; by-value return, swapped operands, +=, a Lerp inline, `&(*p = x)`: same code
- `0x463FB0`: order of two blocks in the switch default; every spelling gives the same layout, 9.6f has the original's
- `ProjectVert_4EB940`: the MapRenderer x87 scheduling quirk
- `0x5AA9A0`: the door part fixes with a separate offset variable, but that shifts registers across the whole function (82 diff lines)
- `0x4CDF30`: ebx/edi swap, nothing moved it
- `0x5D4A10`: calls the duplicate Fix16(s32) ctor copy (documented)
- `0x5213E0`: return block order; while(cond) loop and a result variable also tried, both failed

### Near-miss pass, batches D2 and E2 (2026-10-02)

- `CarPhysics_B0::ApplyReverseEngineForce_55EF20` (0x55EF20): closer 46->2, logic fix (y > 0 branch: negative impulse for 90..180, positive for 180..270; reversed angle through a temporary and Normalize_406C20). Left: the temporary is at 6(%esp) in the original, 4(%esp) in ours
- `sound_obj::UpdateCarEngineAudio_57E220` (0x57E220): closer 21->6. clamp to 100 after the paused if/else fixes the block layout. Left: scheduling only
- `DrawPlayerStatsHelper_5D61A0` (0x5D61A0): closer 23->16. no x_offset local fixes registers; any local for the 18/22 ternary swaps ebx/ebp back
- `Door_4D4::dtor_49D570` (0x49D570): closer 21->20. ~DoorData_10_Pool clears the pool head (9.6f 0x44C7F0). Left: missing EH frame, see the `<new>` quirk
- `Char_B4::HandleObjectCollision_548840` (0x548840): no change. original EH state is 4 at entry (ours 1): three more destructible locals. Three dummy Fix16_Point locals get it to 11 diff lines (not kept, a guess). Success path also shares the epilogue in the original
- `sharp_pare_0x15D8::ReadTextures_5B92E0` (0x5B92E0): matches exactly without the standalone guard `if (i > 992) return;`. Kept for now (decision: document, don't match): removing it brings back the original's out-of-range read. If a standalone-only guard macro is added later, this becomes a match
- `UpdateStatsForKiller_46F720`: closer 14->4. test the loop pointer after the loop instead of a flag. Left: one register in the group-respect branch
- `HandleCollision_522E10`: closer 17->16. ResolveCollisionWithPed_5229B0's third param is u8. Left: the original sends both arms of the inlined As2C check into one shared call
- `AdjustPlaybackRate_41A580`: closer 43->39. fixed-point maths follows the asm now. Left: this/difference register swap throughout
- `ResolveCollisionWithWorld_522B20`: closer 48->41. bug fix: the collision point is *f18 (was an uninitialised local); Divide_442CB0. Left: EH state numbering
- `EnumAddress_cb_51E030`: no change. late ebp push
- `sub_4DF3E0`: no change. by-value abs/max helpers didn't help
- `sub_575650`: no change. switch value clobber (add $-3) vs our lea
- `EnumerateMaps_51BFA0`: no change. one `if (map_count > 0)` around the rest: 15 -> 51, reverted

### Near-miss pass, batch I (2026-10-02)

- `MapRenderer::sub_4F4250`, `sub_4F49B0`: closer 31->19. `T_gbh_DrawTile` takes the diffuse colour as u8 (pushed without zero extension). Left: x87 scheduling around the ProjectVert inlines
- `MapRenderer::sub_4EC450`, `sub_4ECAF0`, `sub_4ECE40` (and `sub_4EC7A0`): closer 47->28, 49->30, ->28. atan2(dy, dx) order, angle test `> 45 && < 225`, Fix16(f32) via 16384.0f, 4ECE40 uses GetTexture_46BB50. Left: x87 scheduling
- `MapRenderer::draw_lid_4EE130`: no change
- `Weapon_30::oil_stain_5E1DC0` matched once the z bound was `k_dword_706EDC - k_dword_706F70` and the half depth/lower bound got named locals (inline budget)

### Near-miss pass, batch H (2026-10-02)

- `MapRenderer::draw_left_4F3C00`, `sub_4F4600`: closer 31->16 with the u8 `T_gbh_DrawTile` colour. Left: the ProjectVertTop_46BD40 scheduling (Still unexplained)
- `sub_4DADA0` (0x4DADA0): no change. original puts the clear block and pager-clear tail in the middle of the show path; inverted if, early returns, bShow at outer scope: same or worse

### Near-miss pass, batch J (2026-10-02)

- `Sprite::ShrinkSprite_59E390` matched with `b = ReduceWidthBy(x); b |= ReduceHeightBy(y);`
- `Car_BC::CarShrinkSprite_43DC80` matched with `Fix16(w << 14, 0)` and the sprite w/h read into locals (w first)
- `ConvertColourBanks_5D7CB0`: closer 0.146->0.638. Only a 10-byte `jmp +8; nop...` gap after the `pgbh_SetColourDepth` call is left; looks like a binary patch in the exe, likely unmatchable
- `Trailer::sub_407BD0`: closer 0.476->0.843. Explicit out-of-line y line (see "Big functions run out of inline expansions"). Left: eax/edi swap for offset.x/y and multiply order
- `GetDoorWorldPos_43B420`: no change. The `(const Fix16&)` cast reaches 0x408660 and sin/cos locals fix part 1, but then the arg copies are lost; best 0.55 < 0.564
- `Particle_4C` 0x53B670: no change (known stack slot / add-to-memory issue)
- `SCRCMD_STORE_CAR_INFO` 0x509180: no change. eax/ecx swap for pCar vs gStoredCar; four restructurings didn't move it

### Near-miss pass, batches K, L, M and Fable trial (2026-10-02)

- Matched: `CarPhysics_B0::ApplyReverseEngineForce_55EF20` (`theta = field_58_theta + kAng180`), `Ped::UpdateStatsForKiller_46F720` (Ped* local, no Player* local), `get_skid_obj_type_55D490` (per-case returns, cases 0,2,1,3), `SmoothApproachAngle_405CE0` and `SmoothApproachClamped_4F75D0` (missing else branch, single `*curr += *vel` at the end), `Ped::ReactToAttacker_465B20`, `sound_obj::EnqueueRadioLocationPhrase_426E10` (local order mid_x, mid_y, halves), `Crane_15C::ComputeHookPos_47E620`/`_47E730` (rotation written out), `Hud_2B00::ctor_5D6CD0` (ctor moved to the top of Hud.cpp), `Car_6C::SpawnCarAtRoadDirection_444CF0` (u8 z, else-if chain with a pCar=0 local), `Char_B4::sub_54C090`, `Player::RestoreCarsFromSave_56A0F0` (SpawnCar inline with const Fix16& scale, u8 loop index, u16 model)
- `ChooseRadioEmitterForVehicle_57E6C0`: closer 94->49. Left: the found block of the search loops sits after the case 0 loop with `jae ret; jmp top`; for/do/goto/break variants failed
- `sad_mirzakhani::ProcessBonusEvent_4320D0`: closer 69->12. Left: eax/ecx/edx swap in the inlined AddScore_41DC40

### Near-miss pass, batches N2, K2, M2, L2 (2026-10-03, partial)

- Matched: `sound_obj::ProcessType8_Crane_412820` (u8 loop + switch with `continue` cases, shared tail), `sound_obj::HandleTrainCabRollingFrictionSound_4143A0` (same shape as 4140C0), `CarPhysics_B0::StepMovementAndCollisions_55E470` (indexed store)
- `sub_4F76A0`: closer 96->~35. Left: original keeps a separate success store + jmp per branch, ours tail-merges
- `sound_obj::ProcessOtherObjects_41F520`: closer (field_1_age u8, jle->jbe). Left: VC6 tail merging of identical case tails picks other blocks than the original
- `DrawScoreTable_4B5430`: closer 0.729->0.978. palette s32, `y = ypos + 40*i`. Left: ebx/ebp load order and a temp slot
- Matched (second part): `sound_obj::HandleAICarHornBeep_413D10` (`fAC > 2` first, rate as one expression), `Frontend::GetNextUnlockedBonusStage_4B7360`, `PedGroup::CoordinateGroupCarEntry_4C9F00` (9.6f inline IsSwatVanOrBankVan_403BC0), `InitializeGame_4DA4D0` (restored global resets, 14-byte memset), `Bink::OpenSlot1_513560` (if/else FatalError, missing 5/6/5 format case), `CarPhysics_B0::ScarePedsOnDrivingFast_559C30`, `CarPhysics_B0::HandleGravityOnSlope_55AA00` (Fix16_Point local, cases break to one call), `Car_14::GetRandomTrafficSpeed_583750` (Fable), `Montana_4::ctor_5C5E70` (definition order)
- `UpdateCarEngineAudio_57E220` (Fable): closer 6->1. Left: order of the 0x58 store in the 2nd sample
- `EnforceGearSensitiveMaxSpeed_562D00`: 0.699->0.944 (atan2_40F790 in the inline). Left: shared store at the end of the y clamp
- `GetPrevUnlockedStageBonusCode_4B7800`: 0.312->0.530; `pistol_5DD860`: 0.721->0.931 (two Fix16_Point locals); `GetTaxiNear_457BF0`: 0.330->0.705 (MaxAbsDistance_42A6B0 calls Max_44E540); `GetPreviousUnlockedBonusStage_4B7120`: 0.239->0.432; `TurnTowardsAngle_54CAE0`: 71->61 (subtraction order fixed; original shares two add+normalize blocks)
- No change: `4174C0` (Fable; product reg ecx vs edx), `4C9B60` (register allocation), `43B5A0` (x' product sunk past the ypos calls), `GetSpeed_50E190` (since matched)
- Matched: `CarPhysics_B0::ComputeCombinedCenterOfMass_559EC0` (`Fix16 m; m = f();`, one return expression)
- `ComputeScanlineIntersectionX_4F77D0`: 0.533->0.822, same restructure as 4F76A0. `SmoothApproach_4F7540`: 0.233->0.434 (9.6f statement order; left: clamp/delta tails merge differently).
- `Camera_0xBC::ComputeTargetFacingAngle_4358D0`: no change. Original keeps the rotation in dx and adds kAng180 as a 16-bit add + jns; ours spills the Ang16 and uses lea. Operand order, s16 sum in operator+, rValue ctor, block local, shared helper all tried
- Fable worker, second list: matched `Sprite::IsTouchingSlopeBlock_5A1EB0` (9.6f inline `Map_0x370::IsGradientSlopeAt_466CF0` for the eight open-coded slope checks) and `sad_mirzakhani::sub_432170` (if/else-if chain). No change: `TrainCab_414710` (VC6 threads the failure `je`s to the else epilogue; early return, inverted if, goto all identical), `DrawPlayerStatsHelper_5D61A0` (original calls two Fix16(int) copies, 4369F0 and 4926F0; we link one), `Map_0x370::sub_4E6190` (case 3/4 tails merging into 2/1 unexplained)
- `UpdateSteeringAngle_562560`: 0.496->0.857 with `GetLength_out_of_line_x_squared`. Left: GetTrailerAwareTurnRatio_55A100 called before the division
- `CanStepForwardWithRegionCheck_54ECB0`: 0.752->0.931 (case order 1,3,2,4 from the jump table, one shared check after the switch; fixed an inverted bit-0 test and `&= ~1u`). Left: shared return blocks in the other order
- `Bink::OpenSlot2_5133E0`: 0.443->0.838 (BinkOpen failure as the else). Left: `mov $2,%ebx` not hoisted above the `je`
- `Car_BC::GetDoorWorldPosition_43B5A0`: no change; the explicit out-of-line y line (as in 407BD0) made it worse locally and did nothing inside RotateVector_41FC90

### Near-miss pass, batches O, P, Q, R (2026-10-03, partial)

- Matched: `Hud_CopHead_C_Array::DrawWantedLevel_5D0110` (inline DrawFigureScaled_4C71B0 rotation by const&, explicit pointer loop), `Object_2C::ReleaseSubObjects_527F10` (field_C = 0 in each branch), `Crane_15C::ComputeHookOffset_47E840` (RotateByAngle_40F6B0 with out-of-line Multiply/Add), `Object_3C::GetMovementSpeedAndAngle_521FD0` (Fix16_Point local declared at the top, out-of-line GetSpeedVector_52ADF0, file-local GetLength variant), `CarPhysics_B0::ApplyMovementStep_560F20` (Ang16(s16,u8) by value)
- `ProcessType7_Weapon_42A500`: 111->28. Left: rate add reassociation and GetLoopStart arg load order
- `Ped::RecruitNearbyPeds_46E080`: 116->56. sub_4204D0 inline stays a comment (worse). Left: z stored in desiredCount's param slot
- `DoorData_10::Init_49C340`: 116->16 (flip flag is 0x3C00, old source had 0x2C00). Left: v8 stored before the tile load
- `Frontend::DrawCredits_4B7AE0`: draw_x u16. Left: case 0 jumps into case 1's tail; goto duplicates it
- `PickUpCar_47F930`: closer (car_info::is_0x10). Left: EH epilogue copied into 3 exits
- `Ped::BusCustomer_AI_461290`: 134->41 (case order 38,35,31,34). Left: whole-function register rotation
- No change: `571A30` (shared ret block placement), `51F210` (register allocation)
- Matched: `Sprite::RotatedRectCollisionSAT_5A0380` (inline budget: last point via out-of-line ProjectOntoAxis_5A5AA0/GetNegatedAngle_5A26E0), `Car_BC::UpdateTrainCarriagesOnTrack_4413B0` (logic fix, carriage positions chain), `Object_2C::NewObj3C_528130` (file-local GetLength, block-scoped Ang16 local for the atan2 result)
- `Wolfy_7A8::sub_543690`: 113->12. `Sprite::MinDistanceToAnySpriteBBoxCorner_5A22B0`: 116->4 (only the first Abs inline, the others out of line; KeepMin helper). `Ped::AttackTargetStateMachine_46D460`: closer (declaration order). Left: shared `b11 = false` block placement
- Matched: `Object_2C::ShouldCollideWithSprite_525370` (gotos to nested ifs, case 12/13 sub-chain), `Object_2C::SetMovementVector_5224E0` (file-local GetLength, Fix16_Point declared before the if), `CarPhysics_B0::HandleWorldCollision_55FD00` (`Fix16 damage; ... = damage = call()`, particles through a const& inline)
- `TryCreateRoadblockAt_577370`: 113->30 (implicit u8->Fix16 is the out-of-line FromInt_45C4E0; separate case 3/4 bodies). `Char_B4::sub_54C3E0`: 116->59 (also fixed which local is passed per branch). `CanStepForward_54FEC0`: 136->32. Left: return block merging
- Matched: `CarPhysics_B0::UpdateReferencePoint_563460` and `UpdateCenterOfMassPoint_563350` (rotation written out, out-of-line y line, `throw()` on the out-of-line Fix16 copies), `Sprite::FindOverlappingBoundingBoxCorners_5A0150` (HalfWH written out), `Ped_List_4::FindClosestPedInViewCone_4713C0` (by-ref MaxAbsDistance, re-read pIter->ped)
- `Object_2C::HandleImpact_528E50`: 119->99 (case 1/2 fold into the shared PoolGive tail elsewhere)
- No change: `446530` (x/y/rotation loaded before the train check)
- Matched: `CarPhysics_B0::ComputeSlopeCorrection_55AB50` (switch with returning default instead of a goto into a case)
- `CarPhysics_B0::ApplyDriveForce_5615D0`: no change; only the two `Fix16_Point()` ctors called out of line (inline budget)
- Matched: `Hud_Arrow_7C::UpdateTargets_5D0620` (file-local GetLength with kFpZero_7064C0), `ExplodingScore_50::DrawNumbers_596C90` (private out-of-line copy of 9.6f inline 4B90E0), `CarAI_78::UpdateDrivingAI_452DF0` (kF16Zero_677B90 in a file-local GetLength), `Object_2C::ResolveCollisionWithPed_5229B0` (and the out-of-line `Fix16_Point::Negate_40ACB0`, now emitted and verified)
- Closer: `GetNearestZoneOfType_4DF240` 185->28, `CarAI_78::CheckRoadAhead_448770` 186->60, `RectHitsDiagonalWall_4E11E0` 175->~12, `UpdateCarEngineAudio_57E220` (distance store order)
- No change: `4320D0` (Fable: eax/ecx/edx roles in the inlined AddScore_41DC40), `5238B0` (goto-shaped layout), `446530`
- Matched: `Ped::SetObjective_463570` (one call after the switch, case order from the jump table), `Ped::SpawnWeaponOnDeath_45E080` (one SetO8Timer after the switch, direct cam field reads, electro_batton case in enum order)
- `NormalizeSafe_442AD0`: 170->52 (two inline length variants with the original's out-of-line calls). Left: stack packing (0x20 vs 0x28). `IsSpriteInView_435630`: 188->134. `tank_main_gun_5E10E0`: 181->95 (left: EH state 3 around the offset+pos temp)
- Fable, closest leftovers (no matches): `57E220` now 1 line, the `mov $0x81020409,%eax` load scheduled after the div; only `operator=` on a member is a barrier (a temp, a local or a static inline helper are not). `5A22B0`: corner.y load scheduled above the xd subtraction. `543690`: lea temp register swapped between the two returns. `4320D0`: AddScore_41DC40 register roles unchanged by every variant. `49C340`: 10.5 doesn't hoist the tile-idx load above the v8 store (9.6f and ours do). `4B5430`: param load order and the const s32& temp slot. `562D00`: VC6 duplicates the y-clamp store + epilogue. `54ECB0`: the original keeps `return true` inline after the range check and one shared `return false` at the end; VC6 merges identical `return false` tails into the first earlier identical block but not a later `return true` into an earlier `mov $1,%al`
- Matched: `Ped::FollowCarOnFootWithOffset_46A350` (Ang16 -=, repeated expression instead of a Sprite* local), `sound_obj::ProcessType6_Rozza_C88_413760` (goto loop as a for loop, `p88->pool[i].x` reads), `ExplodingScore_50::DrawSingleNumber_597100` (Camera local, one-case switch, ProjectWorldToScreen_4B90E0 with a scale local), `Weapon_30::car_mine_5E2550` (Fix16_Point at the top, `p += get_x_y_443580()`, GetH_447E10 getter)
- Closer: `FindUsableCarDoor_467090` 182->76, `Type_10_HandleCarSkidSound_418940` 183->98, `ShowJunctionIds_588620` 190->111, `Char_B4::state_8_5520A0` 115->26 (logic fix: the electrocuted >100 check is outside the net if/else), `4E1520` 162->48
- No change: `539040` (all 5 Particle_4C jitter originals build `Fix16(rng(3)-1)` as `movswl; add $0x3FFFF; shl $0xE`, ours `shl; sub $0x4000`), `5406B0` (needs out-of-line Fix16(0) ctors with an object with a dtor alive)
- Matched: `Car_BC::UpdateAttachedToSprite_443360` (logic fix: the rotation uses pSprite->field_0, not rot; out-of-line Multiply/operator+ rotation)
- Closer: `Ped::sub_469FE0` 0.313->0.608, `SetObjective2_463830` 185->151 (left: VC6 copies the call + epilogue into each case, original shares one tail)
- `RectHitsDiagonalWall_4E11E0` (~12 lines): only `return 1` jumping into the shared EH epilogue is left, same unexplained case as `Start_NetworkGame_5E5A30`
- No change: `46F1E0` (original tests the angle with jns right after the 16-bit sub/add)
- `EmitElectricArcParticle_540320`: 202->21 (explicit Multiply/Negate rotation, block scope). Left: one operand order in MultiplyByFix16_401CB0
- Matched: `Car_BC::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` (params modified in place, pos_z declared at the call), `Object_2C::ResolveCollisionWithObject_522710` (Fix16_Point results declared up front, Negate_40ACB0 out of line, angle += through an Ang16& + Normalize_406C20), `Train_58::UpdatePassengerAI_578390` (new global gTargetCarDoor_6FF1D8 as the loop counter, inverted field_1818 test fixed)
- Closer: `ApplyImpactForcesAndDamage_55FA60` 205->19 (left: three return tails should jump to one shared epilogue), `PoliceCrew_38::sub_575310` 233->49, `Orca_2FD4::ComputePath_554AB0` 216->48, `Garox_110C_sub::Update_5CF730` 225->60, `ApplyCarVelocityCameraOffset_436200` 233->119
- No change: `4B0220` (5 KB of stores, register rotation everywhere), `521890` (ours hoists pPlayerIdx into ebp), `5DDA70` (rotation in %bx), `57DF10` (original keeps the restart tail unmerged)

### Near-miss pass, batches AA-AE (2026-10-03, partial)

- Matched: `Sprite::ShowHorn_59EE40` (Fix16 from `(s32)(x/(f32)(u32)width*640)` like the other Show* functions, not `Fix16(f32)`), `sound_obj::HandleCarEngineSound_4157C0` (gotos removed; separate per-path `Round(...)+25` statements, which VC6 tail-merges into the shared div/mul code like the original)
- Closer: `Wolfy_30::state_18_33_541D60` 306->35 and `state_19_32_542060` 279->20 (else branch first, PolarToCartesian written out with the second multiply as out-of-line `Multiply_408680` because of the inline budget; left: register rotation), `HandleCarTireScrubSound_418720` 261->4 (u8 volume, structured ifs, `field_AC > 0`), `StabilizeVelocityAtSpeed_562910` 0.41->0.96 (9.6f inline `MultiplyByFix16_49E3A0` as `*=` by reference, `RotateVelocity_562C20` on Fix16_Point_POD)
- Matched: `Char_B4::state_7_551CB0` (both block lookups call `GetBlockTypeAtCoord_420420`/`get_block_4DFE10` in each branch; the seven `z == N` tests compare raw `mValue` to stay inside the inline budget; cases 8 and 9 as separate identical bodies), `PedManager::PedManager` 0x470650 (sprite setup and statics restored, implicit conversions for `set_xyz(0,0,0)`, `__forceinline` on the pool ctors because of the budget), `Garox_12E4_sub::DrawPause_5D63B0` (pass a u32 where the original calls the `Fix16(u32)` copy 0x4926F0)
- Closer: `SelectObjectImpactSound_413120` 0.35->0.99 (two switches inside `if (model <= 110)`), `fire_truck_gun_5E0E70` 241->15 (out-of-line rotation helpers, `Add_40AC50`, Fix16_Point locals before the field_24 store), `SpawnSkidSegment_55D200` 0.23->0.84, `Draw_59EFF0` 0.75->0.92, `PoliceCrew_38::State6_ShutDown_574720` 410->119, `army_gun_jeep_5E13E0` 0.43->0.55, `FindBestTargetPed_466BF0` 0.31->0.43, `HandleCarDoorSounds_4182E0` 0.15->0.33, smaller gains in `4410D0`, `561E50`, `561380`, `5D8A10`, `4672E0` (path switch fix)
- No change: `46EB60` (rewritten without gotos, regalloc left), `5E5A30` (shared EH epilogue)
- Matched: `Ped::CarThief_AI_45FF60` (case order 0, 35, 31, 36, 1; Kill written out at each site; inline bus model compare; `*(xd > yd ? &xd : &yd)` instead of `Max_44E540`), `Sprite::FindCollisionIntersectionPoint_5A2710` (logic fixes: restore paths use the saved angle, second-half corners from pOther, bOutSideSelf/bOutSideOther were swapped; one named result; `GetBoundingBoxCorner_562450` moved to CarPhysics_B0.cpp)
- Closer: `ProcessPedImpact_560B40` 0.26->0.95 (left: an EH state around the negated temporary; `throw()` on `operator/` removes it but breaks `ResolveCollisionWithObject_522710`), `state_18_33_541D60` 306->35
- No change: `59EB30` ShowId, `465D00` IsPedAThreat (register rotation)
- Unverified data: `word_61A898` now defined with initial value 40, a guess (marked TODO in Char_Pool.cpp)
- Matched: `Hud_CarName_4C::DrawCarName_5D4A10` and `Hud_Brief_704::DrawBrief_5D3B80` (the y/x positions passed as u32, so the call goes to the `Fix16(u32)` copy 0x4926F0), `CarPhysics_B0::UpdateWheelSkidEffects_55DC00` (SpawnSkidSegment param by value, a temporary per call with an EH state; assign rather than init for the inline ApplyScale results), `Ped::FollowTargetStateMachine_46AC20` (`RegulateVelocityByRef_433970`: a by-reference argument stops VC6 tail-merging inlined calls with different arguments), `CarAI_78::DoShortcutsUsingJunctions_447970` (u8 x/y/z locals, `(u16)route_pos`, if/else per case, 9.6f `ContainsPoint` 0x40CEE0 used)
- Closer: `state_20_542340` 239->20 (timer > 8 branch first, cos product through `Multiply_408680`), `DrawPlayerNames_5CFE40` 247->172 (WorldToScreen_40CFC0 written out), `DrawDigits_492260` 246->198, `Car_214::sub_5C8780` 250->229, `DrawPlayerStatsHelper_5D61A0` (Fix16(u32) for the text x), `GetNearest{Horizontal,Vertical}EdgeToCoordinate` 5A0A70/5A1030 (one reused diff local)
- Still unexplained: in `DrawDigits_492260` the original re-tests `c != '0'` and `idx == 8` after the `idx == 8` branch where ours threads the jump (`sub_492430` matched by re-reading `field_9_str[idx]` at each test); `5CFE40` tests only `al` of `IsCoordsPosVisible_435A70` (bool return?); `5C8780` case 4 keeps three calls jumping to a shared compare where ours merges them
- Matched: `Car_BC::CanCarCollideWithSprite_43AAF0` (u16 flag locals; `if (pSprite) {...} else {null case}`; the first type test reads `field_30_sprite_type_enum` directly, not the shared local; model 182 rather than `rocket_bullet_128`), `Ped::PullDriverOutOfCarStateMachine_46B2F0` (u8 loop index declared before pCar, which puts it in memory and pCar in ebx; split sub/Abs statements; `SetMaxSpeedByRef`)
- Closer: `ComputeCarMassAndInertia_454410` 339->147, `GetNearest*EdgeToCoordinate` 325->150 / 327->142, plus `55AD90`, `5A1490`, `4E1A30`
- Logic fix: `RobbedDriver_AI_461630` tested `field_140_stolen_car` the wrong way round and could dereference null (diff count unchanged; zero register is ebp in the original, ebx in ours)
- No change: `SpawnCabAndTrailerHelper_408370` (call sequence now identical, but the two extra destructible locals needed for EH state 4 use up the inline budget so the first `+` goes out of line)
- Closer: `Ped::Reset_45AFC0` 0.02->0.80 (b0/b1 cleared first, `field_130 = -gPedAng_6787A0`), `Init_AI_Chase_44E0C0` 0.41->0.94 (logic fix: the found tile is converted with `Fix16(u8)`, the old code stored raw bytes; `if (v == 0) ... else if ((u16)v > 0)` gives the `jne`/`jbe` pair), `UpdateZPhysics_55AD90` 0.25->0.82 (one commented goto into the shared `zpos = cp3` block; `/=` reloads the field), `PointInsideRotatedBounds_5A1490` 0.40->0.42, `Particle_4C::sub_538060`
- Not committed: `MeleeAttackStateMachine_46B670` has a logic bug (the punch-to-death path falls through into `b5`/`Set_F250(18)` after the mugger block where the original returns). A rewrite fixes it but the diff grows 422->440 because one mugger block gets tail-merged
- No change: `MakeTrafficForCurrCamera_5832C0` (original multiplies with width loaded first into edx; ours ends up 2 bytes short)
- Systemic: the missing EH state around binary `operator-` (0x40AC80) temporaries was fixed by giving `operator-` an inline body (see matching_quirks.md); `ProcessPedImpact_560B40` still has one around the `Negate_40ACB0` temporary
- Closer (Fable): `Map_0x370::sub_4E1A30` 366->40 (the hit-test arguments were wrong: x+1/y+1 face coordinates, and a u32 first argument for the Fix16(u32) copy), `Car_BC::SpawnDamageFireEffect_43B870` 0.20->0.82 (logic fixes: both x/y scaled after the switch, case 2 condition, default uses the raw argument), `ProcessGroundCollisionAndSurfaceType_55B970` 0.26->0.47, `Particle_4C::sub_538060` 0.44->0.55 (position/rotation logic fixed)

### Near-miss pass, batches AJ-AN (2026-10-03, partial)

- 0x5349D0 `Garage_48::GaragesService_5349D0`: MATCH. Index locals and the `Fix16_Point car_pos` declared at the top (EH state 0 at entry), inlines `Door_38::CloseDoors_476A30`, `CarPhysics_B0::StopMoving_4895D0`, `Garage_48::Reset_489650`, u8 collision flag, `field_44` u32. The corner checks return through separate inline epilogues, done with a goto.
- 0x561970 `ComputeEngineTorque`: closer 572->543. Out-of-line torque helpers, gear order inverted, logic fix (half thrust was added twice). Left: inline budget (gear 1 multiply goes out of line) and tail merging.
- 0x550F60 `GetNextRotationToward`: closer 576->513. Unused `Ang16(&Fix16,0)` from 9.6f, case 0 is a nested switch. Left: register/slot choices.
- 0x542E30 `state_22_23_24_25`: closer 595->439. Left: the original runs out of inline budget in cases 2/3 (out-of-line Ang16 ctor 0x409300).
- 0x4E5640: closer 596->464. Static inline GetLength/PolarToCartesian with out-of-line helpers. Left: Fix16_Point_POD ctor out of line (inline budget).
  Round 8 (480, skeleton 9): with `Fix16_Point_POD()` forced inline (header experiment only) it's 275 and
  skeleton 7, and the rest of the skeleton is only the shared `xor %al,%al; jmp epilogue` for `return 0`
  (ours copies the EH epilogue into the last `return 0`). A `result` local with `break`s gets the skeleton
  to 5 but spills `result` (313). Not committed: none of these frees the ctor. ToInt -> raw shifts,
  `__forceinline` on the GetLength/Polar helpers, `set_xyz_lazy_inlined_420600`, moving `pos_diff`, and
  initialised `vec_x/vec_y/ground_z` all keep it out of line.
- 0x498DA0 `read_input_device`: closer 461->356. `acquire_input_device_498730` is a thiscall member, Poll() not Acquire(). Left: zero register held across the whole function.
- 0x422B70 `ProcessPed`: closer 445->165. Case order from the jump table, case 26 falls into default. Left: ebx vs ebp allocation.
- 0x539890 `UpdateCircularBurst_state_5`: closer 0.543->0.769 (5384C0 shape). Left: identical cases 4/5 not cross-jumped.
- 0x53E450 `EmitBloodBurst`: closer 0.148->0.495. Source bug: `rng(50) + 25` was missing.
- 0x53F060: closer 489->464. Source bug: the loop rotation must use `angle_2`.
- 0x53D260 `Particle_4C::PoolUpdate`: closer 0.49->0.885. Case order, lazy set_xyz/set_z inlines, missing `|= 4`. Left: constant 1 kept in bl by the original.
- 0x5D0850 `Hud_Arrow_7C::UpdateScreenPos`: closer 0.678->0.772. Note: compare_target_asm maps a value to 0 here, so its ratio is unreliable for this function.
- 0x53F4C0 `Particle_8::EmitFlameStreamSegment`: closer 0.414->0.533 with `RotateByAngle_MixOOL_40F6B0` (`y * sin` inline, rest out of line). Call sequence now identical. Left: frame 0x34 vs 0x4C, EH state at entry 1 vs 4, esi/edi swapped.
- 0x461A60 `Ped::UpdateFacingAngle`: no change. The original lays cases out 2, 3, 1, 7, default with case 3 reusing case 2's atan2 tail; ours merges them into case 1's tail.
- 0x5E4EE0 `WindowProc`: MATCH. `switch((u8)wParam)`, (u8) casts on Bink BOOL results, locals moved to function scope.
- 0x53A280 `Particle_4C`: no change. The original enters EH state 2 after `Remove_477B00` (two more Fix16_Point locals); adding them makes our build call the Fix16_Point ctor out of line. The original also repeats the `type == car` compare (inline `AsCar_40FEB0`), which ours folds.
- 0x4482C0 `CarAI_78`: closer 0.287->0.691. Source bugs: the loops must move the probe sprite, and the flag clear is `&= ~0x80`. gotos rewritten as early-out and loops.
- 0x541850 `TimerAfter50Handler`: closer 0.275->0.351. Source bug: rect left/top were `r - x`/`r - y`.
- 0x55F3B0 `ComputeLineLineIntersection`: closer 0.101->0.354. Out-of-line operators, `DotProductOOL_49E500`, three Fix16_Point locals up front (EH state 2).
- 0x5620D0 `CalculateRearWheelForce`: closer 0.234->0.933. `Fix16_Point(Fix16(0), y)` and v25 up front (EH state 1), uninitialised locals declared in the order the default path loads them, helpers `GetLength_inline_5620D0` and `MultiplyByFix16_inline_5620D0`. Left: `v25.x *= stability` register form and the `(6FE228 - len)` schedule; any extra inline in the tail pushes GetLength's `y*y` out of line. Permuter best (0.715) was worse than the hand version.
- 0x5DDFC0 `throwable`: 0.470->0.485. The `a4 == 96` branch jumps to the shared "thrown" reload block with a goto (the original's LABEL_36). Left: ours keeps 0 in ebx, the original keeps 1 in bl.
- 0x53E860 `Fix16::DivideInt_53E860`: MATCH. Out-of-line copy of `operator/(const s32&)`, guessed from its size by batch AJ and verified once the target asm dump included it.
- Batch AJ (g1), all closer: 0x53FE40 `EmitImpactParticles` 596->416 (`RotateByAngle_40F6B0_all_out_of_line`, normalizing copy via 0x409300, also got the `Fix16_Point_POD` ctor inlined again), 0x524630 `IntegrateHorizontalMovementAndCollisions` 603->501 (division branch first, `PolarToCartesian` written out), 0x5D7EC0 `DrawFigure` 603->519. Permuter runs on 5D7EC0 only found statement deletions (rejected).
- 0x5D2AB0 `Hud_Pager_C::DrawPager`: MATCH (Fable). Early return on `timer < 0 && !counter`, then `timer >= 0 && counter` / `timer >= 0` / else instead of the goto (VC6 folds the second timer test on the fall-through edge only), `v10 / 2` signed instead of `>> 1`, x through the Fix16(u32) ctor.
- Batch AL (g3), closer: 0x442D70 `TrainUpdate` 481->338 (case order 2/1/3/4, `Abs_negate_out_of_line`, out-of-line const `operator+` for high_z in cases 2/1/3 but inline in case 4, u8 car index), 0x416260 `Type_1_6` 455->165 (if/else without gotos, single `return 0`), 0x5283C0 `TickObject` 464->412 (case order from the block layout, `!obj_type` first), 0x5DE4F0 `Weapon_30::sub_5DE4F0` 467->254 (behaviour fix: the first `sub_5DE910` argument is the owner's sprite position, not the target's). 0x45E4A0 `StartCrossingRoad`: no change (about 0.22), permuter 418->408 not applied.
- 0x579CA0 `PublicTransport_181C::BusesService`: MATCH. Road type neighbour checks take u8 coords; North/East call `get_block_452980` while South/West inline `get_block_42A850`; u16 loop counter; the lead carriage is read again for the first driver check so the driver pointer is reloaded.
- Batch AO (g5): 0x451980 `ReactToNearbyCar` 0.382->0.557 (`MaxAbsDistanceOOL_42A6B0`; the original tests `flag1` twice, so probably two `if (flag1)` blocks), 0x5D8470 `DrawTexture` 0.207->0.672 (one Fix16_Point up front for the EH frame, `| 0x20000` was missing; permuter supplied a store order), 0x5DF270 0.135->0.199 (`Fix16_Rect::ComputeShockPrism` by value, duplicated angle check). No change: 0x465270 (constant registers; permuter gains were noise or a dropped `else`).
- Batches AP/AQ/AR (stopped early on the usage limit):
  - 0x5E2940 `Weapon_30::car_smg`: 0.287->0.997 (4 diff lines) with `RotateByAngle_40F6B0_out_of_line` for both points. Only the EH entry state is left (original 4, ours 3): an unused `Fix16_Point` up front gives 4 but changes the inline `+` registers (0.925). Matched later (slow pass) with the permuter: an unused 5th `Fix16_Point` for EH state 4, `Ang16` declared first with `tmpx`/`tmpy` at function scope, and an `Ang16` copy temp before the first flamethrower spawn.
  - 0x460820 `Ped::TaxiCustomer_AI`: 0.230->0.824. `switch (objective)` cases 35, 31, 0; `MaxAbsDistanceByRef_42A6B0` in case 0 with `pSprite->GetXPos()` (permuter); branch inversions; `break` for shared-epilogue exits. Left: then-block placement (VC6 cross-jumps the duplicated `sub_43AF40` tail) and three register choices.
  - 0x538AC0 `UpdateObjectBeamLink_state_38`: 0.147->0.426. Out-of-line `DivideAssign_539F90`/`Multiply_408680` freed enough inline budget that the `Fix16_Point_POD` ctors inline again; sixth point local up front (EH state 5).
  - 0x5538A0 `HandleCarImpact`: 0.135->0.536. Nested if/else in block order, file-local GetLength; `Ang16 tanVec; tanVec = ...` fixes the frame (permuter). Left: per-branch epilogue copies where the original shares one.
  - 0x5DD290 `shotgun`: 0.242->0.346. `word + ped_rotation` order. Left: a 16-bit `add %bx,%di; jns` vs our `lea`+`test`.
  - 0x46E380 `SpawnPedestrianAt`: no change. The original splits the shared tail between the two halves of case 4.
  - 0x5DFB60 `Weapon_30::sub_5DFB60`: 294->255 lines. One `SetRect` with a ternary width, velocity copied as a struct.
  - 0x4645B0 `Ped::CalcApproachPointNearTargetPed_4645B0`: 718->438 lines. Each case of the first switch does its own polar step and adds.
  - 0x54B8F0 `ContinueMovementAfterCollision`: 0.241->0.371. Behaviour fix: `field_24 = 2; field_40_rotation = field_28;` were missing after the 180 degree turn.
  - 0x53E970 `GunMuzzelFlash`: no change (EH state issue; our `vel` calls `Fix16_Point_POD()` out of line).
  - 0x452060 `CarAI_78::ScanAheadForObstacles_452060`: 0.31->0.81. Out-of-line cosine (and in the last two rotations sine) multiplies, `Normalize_406C20` on the angle sum, a bogus `f10 * 4` removed (table index scale), `field_24_bf` bitfields, locals assigned after declaration, gotos replaced except one shared `react:` switch that four checks jump to. Logic fix: `ReactToNearbyPed` runs when bit 0x80 is clear.
  - 0x45D000 `Ped::HandlePedHitByObject`: no change. 10.5 inlines all of `IsPedAThreat_465D00` (itself a WIP), so that has to match first.
  - 0x5620D0 `CalculateRearWheelForce`: 0.715->0.976 (the earlier 0.933 was not reproducible). The function hits the VC6 inline budget: the `Ang16` ctors and `y*y` get outlined. Fixed by building the tail by hand, with `Normalize_406C20` in a block scope, `y*y` written out (`__allmul`), `v` declared up front, and `6FE340*6FE228`.
  - 0x460820 `Ped::TaxiCustomer_AI_460820`: no change (0.824). What's left is register allocation in the Fix16 compares (lhs in ecx, constant in eax), plus the Max>2 else block placed after the `ret`. The permuter's only gain removed the `pTargetObjCar_` load, so it was rejected.
  - 0x5504F0 `Char_B4::state_1`: 0.567->0.825. Logic fixes (`b3` set when negating velocity, electrified check vs state 15), `Ang16(a+b,(u8)0)` for the inlined Normalize, u8 tile args to `CanReachTile_550090`, `MaxAbsDistanceOOL_42A6B0`, offset math inside the `set_xyz_lazy` call. Left: the original keeps 0 in ebp throughout.
  - 0x53BAC0 `Particle_4C::sub_53BAC0` (0.31), 0x4661F0 `Ped::sub_4661F0` (0.34), 0x448CE0 `ManageTrafficCarDirection` (0.31), 0x44D1D0 `CarAI_78::DetectCarAhead_44D1D0` (0.11): no change, these need rewrites rather than tweaks (block layout or frame differ from the start).
  - 0x5CBD50 `EmergencyCrew_30::UpdateStateMachine`: MATCH. `if (v36) {...} else {dead}` block order, a shared u8 index counter and a reused flag variable fix the stack slots, new `MaxAbsDistanceNegOOL_42A6B0` (out-of-line `Negate_4086A0` for x), a missing `dword_706148` compare.
  - 0x4626B0 `Ped::StateMachineTick`: MATCH. Restructured without the IDA gotos (if/else layout, u16 local, `return 0` last); one goto kept to the shared final `Deallocate` block.
  - 0x4AD140 `Frontend::DrawMenu`: MATCH. A missing call to `Frontend::sub_4B7D60` (added as STUB), u8/u16 field types, u16 palette param on `DrawTextFixedWidth_4B78B0`, block-scoped locals for the frame size. Also matched 0x4B7E10 (thiscall, not static stdcall; u16 font param).
  - 0x447D40 `CarAI_78::TrySetTargetDirectionFromArrows_447D40`: 0.146->0.415. Angle copied to a local at entry. Left: VC6 cross-jumps shared call tails in ours that the original keeps duplicated.
  - 0x523BF0 `IntegrateMovementAndCollisions`: 0.352->0.465. Logic fix: the loop saves the previous xyz/angle; radius halved once; a second `Fix16_Point` local (EH state 1 at entry). Left: `this` kept in a stack copy, `__allmul` in the halving loop vs our imul.
  - 0x54CC40 `Char_B4::ApplyMovement`: MATCH. Outer switch in up/right/down/left order and octant case groups in the order of the jump-table targets; past the inline budget, so every polar step goes through a helper calling `Multiply_408680`, plus `Divide_436A20` and the out-of-line `operator+`.
  - 0x54A530 `HandleGenericCollision`: 0.358->0.339, kept because it fixes the logic (compare against `kAng180` not `kAng90`, cascade nested the other way, missing `field_2A == kAng90` branch); the call sequence now equals the original, the rest is frame and registers.
  - 0x54EF60 `CanStepDiagonal`: 0.532->0.587 (permuter). Left: the original keeps `sprite_zpos` in ebp and pushes the direction constants as immediates.
  - 0x545AF0 `CarDoorAlignmentSolver`: 0.077->0.249. Left: the first inner case sits before the else block; temporaries reuse dead argument slots.
  - 0x4B3170 `Frontend::ChangeMenuPage`: 0.223->0.295. Both switches became one else-if chain (the original has no jump table). Left: the 2||11 (DEAD/QUIT) body is placed at the end in ours, inline in the original; frame 0x100 vs 0x108.
  - 0x44A1F0 `CarAI_78::FollowRoadDirection_44A1F0`: 0.448->0.573. Angle compares as `Ang16(a.rValue ± b.rValue, 0)` so Normalize inlines. Left: register allocation of the three kAng0 copies; the permuter's gains changed behaviour.
  - 0x546360 `Char_B4::UpdateAnimState`: 0.108->0.117. Separate `case 1`, `s8`/`u8` locals. Still far.
  - 0x469060 `Ped::GotoAreaByAnyMeans`: no change. All four MaxAbsDistance sites use out-of-line `Abs_436A50`; switching to `MaxAbsDistanceOOL_42A6B0` fixes those blocks but shrinks the frame 0x24->0x20 (0.348->0.253).
  - 0x54DDF0 `Char_B4::state_0`: no change (0.459 without WIP_IMPLEMENTED), diffs spread over the whole 3.7 KB.
  - 0x554110 `Orca_2FD4::Internel_CanMoveDiagonally`: MATCH (fable). A flat if chain, `IsGradientSlopeAt_466CF0` on the SW/NW branches, the `yd == 1` branch first, negations written `f() ? false : true`.
  - 0x5C1D00 `TrafficLight_20::Init`: 0.410->0.557. Logic fix: two loops did `h++` instead of `i++`; s32 countdown counters. Left: register allocation.
  - 0x41AB80 `sound_obj::ProcessActiveQueues`: 0.265->0.353. Doppler logic fix (old distance passed, new one stored, both truncated); `sound_0x68` fields 14/20/30 u32. Left: register allocation.
  - 0x44AF00 `CarAI_78::AlignToLaneCenter_44AF00`: 0.189->0.265. Logic fix: lane offset was `x - x`. Left: `Ang16 a - b` is a 16-bit `sub` from memory in the original.
  - 0x4E1E00 `Map_0x370::CanSpriteEnterTile`: 2022->2018 diff lines. `!gSprite || Check()` for all 23 checks gets the size right but VC6 then shares one `return 1`; the original also uses two copies of the Fix16 int ctor (`FromInt_4369F0`, `FromInt_4926F0`).
  - 0x4626B0 `Ped::StateMachineTick` (fable): one goto kept, the null-ped-group exit into dummy_3's last `Deallocate` block; no structured spelling produced that forward jump.
  - 0x43F130 `Car_BC::HandleCarHitByObject`: 0.130->0.090 by the ratio, kept because the frame (0x118), EH numbering, compare-tree case order (128/138, 10, 194, 210, default, 265, 198) and the first case's calls now match; the ratio sees one big hunk either way. New inline `const Fix16_Point&` overload of `AccumulateDamage_43DA90`. Left: damage in ebx, RotateByAngle operator variants, tail merges.
  - 0x5A3550 `Sprite_4C::UpdateRotatedBoundingBox`: not attempted (>2 KB, ratio <0.04).

## x87 scheduling pass (MapRenderer)

- 0x4F4190 `Set_UV_4F4190`: **MATCH**. Products summed as `Edge01*a1 + Edge03*a2` (right operand
  first), u/v converted through `f32` locals (a rounding node that moves `fmuls` after the index load).
- 0x4FFD90 `Mike_A80::sub_4FFD90`: **MATCH** (see its section).
- The Draw*Sided* / ProjectVert cluster: still unexplained. All VC6 builds (RTM to SP6, Processor Pack)
  give the same code, about 150 helper and call-site variants scored over every MapRenderer WIP: details
  in matching_quirks.md, "Still unexplained".
- Round 5 (tu_harness counts on c783089, total over all MapRenderer WIPs 1246 -> 1235):
  - `ProjectVert_4EB940` is a `MapRenderer` member (thiscall that ignores ecx): every original call
    site does `mov %ebp,%ecx` first. Made it a member: 4EAF40 -4, 4ED290 -4, 4F0420/4F1660/4F33B0 -1,
    no MATCH changed. 4EB940 itself unchanged (11).
  - Address order says Top/Bottom/set_vert/ProjectVert are inline functions whose out-of-line copies
    follow the first function that called them out of line: 4EAD90/4EAE00/4EAEA0 sit right after
    DrawLeftSide_4EA390 (which calls Top/Bottom out of line twice each), 4EB940 right after
    DrawRightSide_4EAF40 (its only out-of-line caller before it).
  - DrawTopSide_4EBA60 (MATCH) inlines the same Top: every Top body change breaks it (compound `+=`,
    x copy, u32 centre locals, z line first), so the helper body is settled and the cluster cause is
    in the caller or TU context.
  - No effect on draw_left/right/top/bottom: named x/y locals (either order), `Fix16::Add_ref` or other
    add forms for every sum, swapped sum operands, inline MapRenderer members PVTop/PVBottom/PV called
    on this (cluster same, 4EA390 +67), `#pragma optimize("a"/"w")` around draw_left, /GX-.
  - 4EB940 y line: u32 local, `(f32)` cast, operand orders, local camera pointer: no effect. Plain x line
    (no `tmp` block) +2, body = `gVertProjector.ProjectVert_46BC70` +55.
  - ProjectVert_46BC70 signature: const refs no effect; by-value x/y or z break 4EBA60.
  - Adding member declarations to `MapRenderer` (3-9 dummies) flips `lea (%eax,%ecx)` operand order in
    4EF880/4EF520/4EEE60/4EFDB0 (+1/+2): register tie-breaks depend on the class's symbol table. No
    x87 order moved.
  - What differs, concretely (draw_left): site 1 Top (x temp and the u32 temp share slot 0x18) the
    original stores the dead zero hi dword of the x conversion after `fmul`, ours before `fildl x`, and
    the y one right after the x `fiaddl` (ours just before the y `fiaddl`); site 4 the original hoists the
    next statement's `mov 0x28(%esp),%edi; xor %eax,%eax` above `fstps x`, ours waits for the y temp store.
    4EB940: the original pops ebx/ebp/esi/edi between the y line's x87 ops and reads the temp as
    `fiaddl (%esp)`; ours pops after `fiaddl` (VC6 can do it: ProjectVertTop_4EAE00 does).

- Round 6 (14aa33a and after): 14aa33a matched 4EE130, 4EEE60, 4EF520, 4EF880, 4EFDB0, 4F3C00, 4F4250,
  4F4600, 4F49B0 and 4F4D60 with VertProjector2 (paren no-op nodes that move the 81-node window breaks).
  Then:
  - **MATCH** 4EC450, 4EC7A0, 4ECAF0, 4ECE40, 4EEAF0, 4EF1C0, 4EFB20, 4F0030 (2 lines each before). The
    VertProjector2 y lines convert through an `f32` local (`Fix16ToF32_Rounded`, a free inline so it costs
    no inline size). That puts the no-op node between `fildl` and `fmuls` instead of after the `fmuls`,
    with the same node count. regsearch showed two kinds of misses: six needed exactly one more node in
    the first full window (limit 79), and 4EC450/4ECE40 couldn't be fixed by any window limit (the
    rounding-node delay). The Top y line fixes 4EC450/4EC7A0, the Bottom y line fixes the other six, and
    the ten earlier matches don't change. With the local written in the helper body, Top/Bottom grow
    146 -> 152 and draw_left_4F3C00 & co. (budget ends at exactly 0) call Top out of line.
  - **MATCH** 4EA390. In the Draw*Side functions ProjectVert_46BC70 loads `field_60` before x/y. That
    needs the field_60 conversion to be the heavier subtree, which `Fix16ToFloat_Paren` (`((v / 16384.0f))`,
    a free inline) does. Operand order (`f60 * x`) makes no difference, and parens in the body break
    4EA390's budget (80 -> 398). The slopes load x first, so they now use a plain copy,
    VertProjector2::ProjectVert_46BC70. 4EBA60 is still a MATCH.
  - ProjectVert_4EB940 made a `MapRenderer` member again (the original does `mov %ebp,%ecx`): slopes
    -2/-8/0/-2, 4EAF40/4ED290 +2.
  - Left in 4EAF40 (125) / 4ED290 (123): register allocation (eax/ecx/edx swapped) in the inlined
    VertProjector Bottom/Top sums and around the out-of-line 4EB940 call (`add` vs `lea` for the z sum,
    `imul %esi` vs `imull mem` after it), plus a gradient compare hoisted one line early. Operand swaps
    of the z sum, the x sum, the `* kTileTexSize` product and `this->` did nothing. Also tried and ruled
    out for the 2-line WIPs: call-site sum operand orders (all 16 combos for 4EEAF0), `Fix16` locals for
    the args, u/v statement order, double/int/paren/cast u,v constants, and `f32` u locals (+6).

## Near-miss pass, 9.6f compare and exhaustive permuter (Oct 4)

Matched here: `StabilizeVelocityAtSpeed_562910`, `OnModifiedMapDataLoaded_4E8C00`,
`UpdateCarEngineAudio_57E220`, `HandlePedVoiceEvent_423080`, `GetDoorWorldPos_43B420`,
`DebugDrawProfiling_4FF250`, `do_process_loaded_zone_data_4E8E30`, `MakeTrafficForCurrCamera_5832C0` (see matching_quirks.md and the commit messages). Random-mode
permuter runs of 6 minutes (2700-4100 compiles) made no progress on 418720, 57E220, 4E6660,
516590, 427220, 56BA60, 4D6000, 414710, 4B6390 or 440D90. An exhaustive depth 1 run (every
single pass) also gave nothing on 418720. Scores are differing lines from `permuter_score.py`.

- `CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00` (26): with the 9.6f nested ifs in
  `Fix16_Point::ClampTowardsZero_49E480`, VC6 copies the y store and the epilogue into the first
  branch. `(y >= 0 && y > lim.y) || (y < 0 && y < lim.y)` gives 14, but it re-tests `y < 0`, so
  it was not kept. A ternary gives 20 (`setg`/`setl`). A per-component helper goes out of line
  (inline budget), and `__forceinline` brings it back at 26.
- `CarPhysics_B0::CalculateRearWheelForce_5620D0` (10): in `v25.x *= stability`, x is already in
  ecx. The original does `mov %ecx,%eax; imull (%edi)`. Every spelling stays at 10: both operand
  orders, product temp, reference/pointer locals, `MultiplyAssign_562430`.
- `CarPhysics_B0::ShowPhysicsDebug_559430` (8): only the scheduling of `lea 0x818` (this for
  field_650). No change from a `pFront` local or a `const Ang16&` param on ThetaText_49E240.
- `CarPhysics_B0::ProcessPedImpact_560B40` (16): EH state around the `Negate_40ACB0` temporary
  before `Divide_442CB0`. A visible inline body called out of line keeps the state. 9.6f
  0x40F640 is Fix16_Point negate and 0x4202E0 Fix16_Point divide by Fix16; inlines_96f maps
  0x40F640 to 0x43D5D0, which is wrong (that is ApplyImpactDamage).
- `Map_0x370::sub_4E6660` (4): `mov %edi,%ebx` (pPrev) before the `sub_4E65A0(x,y,&z,1,1)` call.
  Written after the call, VC6 pushes ebx (constant 1) for the two 1s (48). The a5/a6 types
  (s32/u8/bool/u32) and a block-local pOld don't change it.

- `Map_0x370::RectHitsDiagonalWall_4E11E0` (22): `return 1` gets its own EH epilogue copy. A
  `goto` to a single `return result` gets one too.
- `Map_0x370::sub_4E6190` (60): the original cross-jumps case 3/4's inner switch tails into
  case 1/2; ours does the reverse. Inner case order has no effect, and outer orders only move
  the layout (4321: 76, 3412: 68, 2143: 62).
- `ProjectVert_4EB940` (22): `Camera::field_70/74` as u32 had no effect. In the original it is a
  `__thiscall` member of `MapRenderer` (`DrawRightSide`/`draw_bottom` call it with `ecx = this`).
- `sound_obj::ProcessType7_Weapon_42A500` (32): the add order of `rate + rate_adjust + random`.
  No change from regrouping, `+=`, u32/s16 types, a random local, or a dead store (the trick
  that matched 423080).
- `sound_obj::HandleCarTireScrubSound_418720` (4): the original sign-extends the divisor first,
  then reads the dividend through the call's returned pointer. A const ref plus an `__int64`
  divisor first gets the order right but reads from the stack slot (8).
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (4): `cmp $0xF,%al` lands after the
  volatile load whatever the local's type or placement. Non-volatile locals lose the store (40).
- `sound_obj::TrainCab_414710` (6): storing in both branches fixes the epilogue jumps, but VC6
  merges the stores and drops the else's `field_13C = 0` (10).
- `Weapon_30::fire_truck_gun_5E0E70` (10): one register left, the sprite pointer in eax (ours ecx).

## Near-miss pass (Oct 5)

Matched this round: see `git log` and matching_quirks.md. Scores are differing lines from
`permuter_score.py` / `quick_score.sh`; "permuter N" means N random-mode iterations that found
nothing.

Shared epilogue (the original jumps to one pure epilogue, VC6 copies it; see the tail duplication
entry in matching_quirks.md, it can't be fixed by restructuring):
- `ApplyImpactForcesAndDamage_55FA60` (32): three per-path return stores into one epilogue. One `return`
  gives the same; an initialised `Fix16 ImpulseIntensity = ...` gives the three stores but copies the EH
  epilogue into each (60).
- `TrainCab_414710` (6): early returns, an inverted if, an explicit else `return` don't move the failure
  jumps; storing the field in both branches gets the jump targets but VC6 hoists both stores above the `cmp`.
- `RectHitsDiagonalWall_4E11E0` (22): only `return 1` into the shared EH epilogue; goto, result variable
  and a bool flag all duplicate it. `SpriteHitsDiagonalWall_4E1520` (64): the same, plus the original calls
  the out-of-line `Fix16(u32)` (0x4926F0) for the x/y block centres where ours inlines; z is an implicit
  `s32 -> Fix16` built in the argument slot.
- `ComputeScanlineIntersectionY_4F76A0`/`X_4F77D0` (52): `mov $1,%al; jmp` to the shared epilogue.
- `PointInsideRotatedBounds_5A1490` (60), `TickObject_5283C0` (282), `HandleCarImpact_5538A0` (446, also
  `pCar` in `ebx`): the same per check.
- `Char_B4::HandleObjectCollision_548840` (36): success path into the shared EH epilogue, plus EH state; three
  dummy locals fix the state (34).
- `Wolfy_30::state_5_541430` (20): tried `if (!pNew) return`, inverted cooldown, explicit return, permuter.
- `Ped::SetObjective2_463830` (166): the `ChangePedStatesByMode` call + epilogue copied into every case;
  `/Os /O1 /Ob0` don't help.
- `Ped::ComputeAimAngle_45C9D0` (12): return tail copied into the atan2 branch; a `Fix16_Point_POD` local and
  `atan2_40F790` give 16.
- `EnforceGearSensitiveMaxSpeed_562D00` (26): y clamp store duplicated; ternary or early return in
  `ClampTowardsZero_49E480` don't help.
- `Char_B4::CanStepForward_54FEC0` (28): the reverse: the original's first `return false` keeps its own
  epilogue (global store after the pops), ours shares.
- `Crane_15C::PickUpCar_47F930` (90): EH epilogue copied after tail calls.

Tail merging and block layout:
- `Ped::FindUsableCarDoor_467090` (72): a `for` driver loop fixes the shape; VC6 merges the post-loop
  `return 0` into the shared fail block, the original keeps a copy with `found` after it.
- `Frontend::DrawBackground_4B6E10` (72): the two final retry blits share one tail in ours.
- `Frontend::DrawCredits_4B7AE0` (20): case 0 should jump into case 1's tail; VC6 copies the shared stores
  whatever the shape. Permuter 300.
- `Ambulance_20::UpdateState_4FB330` (2): all 24 case orders and a no-default switch + tail give the same
  `jle` target (the original's goes to default's earlier pop/ret copy).
- `Wolfy_7A8::sub_543690` (12): `edx`/`eax` temp swapped in two return tails; `for (u8 i)` the same; permuter 400.
- `Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0` (54): the `return 0` epilogue sits after the loop's
  bottom test, before the out-of-line `return 1` block.
- `Ped::AttackTargetStateMachine_46D460` (46): placement of the shared `b11 = false` block.
- `Ped::MeleeAttackStateMachine_46B670` (440): the second `health >= 20` section's mugger block is merged
  into the canonical one in ours, the original keeps it inline.
- `Object_2C::HandleSpriteZCollision_5238B0` (146): the `field_50 == 1` branch (the original's
  `mov %al,(%ecx)` reuses the compared value) and else-path block order.
- `Sprite_4C::UpdateRotatedBoundingBox_5A3550` (55): one instruction scheduled differently in the 90/270
  branches, so no cross-jump.
- `CarAI_78::FollowRoadDirection_44A1F0` (525 -> 140): which merged `SetGoStraight` copy VC6 keeps (the original keeps the
  second), one add operand load order.
- `Ped::TaxiCustomer_AI_460820` (86): `eax`/`ecx` swaps in Fix16 compares (the original has the left side in
  `ecx`, the constant in `eax`; maybe the header's compare operators), else block placement.
  `Max(dx, dy) > kFpTwo` gives `jg`.
- `TagGameHudUpdate_4DADA0`, `PublicTransport` 57A7A0 and 572920: see their entries.
- `ManageTrafficCarDirection_448CE0` (2005 -> 1897, second switch in the 9.6f layout): nested temps packed
  in the original, separate 4-byte slots in ours; the turn-exit tails merge the other way.
- `CarAI_78::DetectCarAhead_44D1D0` (150): identical case tails merge into east in ours, north in the original.

Registers only:
- `sound_obj::HandleCarDoorSounds_4182E0` (286): the original keeps 0 in `ebx` and `a2` on the stack.
- `Ped::sub_469FE0` (102): the original has 0 in `ebx`, 10 in `ebp`; ours no zero register. Permuter 300.
- `Ped::BusCustomer_AI_461290` (12): `field_150` in `edx`, door byte `al`/`edx`.
- `Ped::IsPedAThreat_465D00` (142): register choice in the two `player_idx` blocks.
  `Ped::HandlePedHitByObject_45D000` inlines it, so it needs 465D00 first.
- `Ped::FindBestTargetPed_466BF0` (224): `pClosest` in `ebp` and `bestPed` at 0x34 in the original, ours
  the reverse.
- `Ped::Reset_45AFC0` (96): now returns `void`; bitfield clears rotate registers.
- `Ped::GotoAreaByAnyMeans_469060` (22): `cmp %bl,%al` on the `FindNearbyTileMatchingSlopeType` result,
  ours `test`.
- `Ped::Deallocate_45EB60` (16): the bit 0 clear should be a dword load to `eax`, the timer store, then
  `and $0xFE,%al`. The permuter's 12 needs a bogus `(u16)` cast.
- `CarAI_78::CheckRoadAhead_448770` (38): the original zeroes `ebx` right after the third `get_block`
  (`xor ebx,ebx; cmp ebx,edi`), ours `test edi,edi` and zeroes at the join.
- `Char_B4::state_8_5520A0` (32 -> 12): two rotations (`ped->184` in `eax` vs `edx` before `set_xyz`; the
  rot load registers).
- `Weapon_30::tank_main_gun_5E10E0` (34), `army_gun_jeep_5E13E0` (30): opening register choice (angle in
  `cx`, length in `eax` in the original). Permuter 300-400.
- `Player::AddCarToHistory_5645B0` (32 -> 16): the shift loop matches; only `esi` (base) / `edi` (iterator)
  swapped. Permuter 300.
- `TrafficLight_20::Init_5C1D00` (742): control flow matches, no 9.6f; the original has x in `ebx`, y in
  `edi` from the prologue, frame 4 bytes bigger.
- `PoliceCrew_38::State6_ShutDown_574720` (46): only the entry store of the loop index; a top-level
  declaration gives the `ebp` zero register (128).
- `Garox_13C0_sub::DrawPlayerNames_5CFE40` (182 -> 180, `(u8)` cast on `IsCoordsPosVisible_435A70`): the
  original keeps `pIter` in `ebx`, `pCam` in `edi`, adds 320 early; a static inline `WorldToScreen_40CFC0`
  is worse (318).
- `DrawPlayerStatsHelper_5D61A0` (26): a local for the 18/22 offset fixes the tail but swaps `ebx`/`ebp` at
  the top (30-32); if/else keeps the registers but branches.
- `Hud_Brief_704::ClearAllBriefsWithPriority_5D4890` (10): `ebp` pushed only after the null check; permuter 500.
- `Orca_2FD4::Internel_UpdateBehaviorGrid_554710` (42): the original uses `dx` first and pushes `ebp` only
  inside the branch.
- `Frontend::GetNextUnlockedMainStage_4B7270` (24): ours does `mov dl,al` before the `== 2` test, the
  original only on the return path. Permuter 400.
- `Object_2C::HandleCollision_522E10` (41): a ternary for the As2C check gives the same.
- `Particle_4C::PoolUpdate_53D260` (112): the original keeps 1 in `bl` for `timer = 1` and `return 1`.
- `Char_B4::state_0_54DDF0` (428): an explicit `test %bl` after `and $3` on the non-null `pAhead` path; the
  map pointer goes to `edx` after z.
- `BurgerKing_1::read_input_device_498DA0` (201): the original reuses one flag for the first pass and
  `handled`.
- `gtx_0x106C::BuildCarInfoContainer_5AA9A0` (16): the original keeps `num_remaps + 0xE` in `edi` (shared
  with the cached `this`); every door CSE form moves `this` to `ebp` (76-84). The permuter segfaults on it.
- `Char_B4::HandleGenericCollision_54A530` (514 -> 403, one `DoJump` call with a ternary condition): the
  original keeps the scaled index in `edi` across `__allshr` and the velocity pointer in `ebx`.
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (4): the original reloads `field_6E` in the loop
  compare, VC6 CSEs it.

Frame and stack slots:
- `Fix16_Point::NormalizeSafe_442AD0` (52): frame 0x28 vs 0x20, nothing else.
- `CarAI_78::ScanAheadForObstacles_452060` (268 -> 34, new `word_677A3A`, `gSin_table_667A80` indexed directly, `u8 field_2C`):
  `zpos_`/`v85` at +0/+8 vs the original's +8/+0xC, plus the `u8` temp of the third inlined
  `IsBlockRoadType`. `word_677A3A`'s initial value is a guess (TODO, read it from the exe).
- `CarAI_78::BrakeForBlockedRoadAhead_4482C0` (225): the top `v33 = 0` goes into `v32`'s slot in the original, ours swapped.
- `CarAI_78::AlignToLaneCenter_44AF00` (609): each angle temp has its own 2-byte packed slot in a 0x1C frame.
- `Particle_8::EmitBloodBurst_53E450` (291): multiply temps in other slots (frame 4 short); ours keeps 0 in
  `ebx`, the original uses immediates.
- `Particle_4C::UpdateFloatingParticle_538060` (367): the original keeps a dead store `rng_1 = zpos`.
- `CarPhysics_B0::ComputePointVelocity_561380` (110): only slots (cos in the `point` param slot, `local_pos`
  below `old_pos`): the three points' slot order is exactly reversed; declaration order and renames don't move it.
- `Ped::CalcApproachPointNearTargetPed_4645B0` (847 -> 395): sine/cos temp at 0xC and the case 2 `Ang16` at 2 in the original, ours 0
  and 0xC; the top `angle = k180 + rot` goes through a temp in ours.
- `Wolfy_30::state_22_23_24_25_542E30` (533 -> 252): the sin/cos temp sits after the case locals and is shared
  by cases 0-2; case 3's value temps have their own slots.
- `Wolfy_30::TimerAfter50Handler_541850` (264 -> 194): two slots (0x24, 0x40) shared across branches, and a
  `setle` in the `timer == 99` compare.
- `Char_B4::GetNextRotationToward_550F60` (452): the original's `v12` is in the temps area at 0xE, frame 0x24
  vs 0x28.
- `Char_B4::ContinueMovementAfterCollision_54B8F0` (392): the original saves `ebx`/`ebp`/`edi` after the
  `Jumping_15` early return.
- `Frontend::ChangeMenuPage_4B3170` (447 -> 354, gotos removed): three local pairs swapped and loop registers.
- `Car_BC::HandleCarHitByObject_43F130`: frame 0x110 vs 0x118 (without `WIP_IMPLEMENTED`).
- `Camera_0xBC::ApplyCarVelocityCameraOffset_436200` (156 -> 58, block-scoped atan2 copy): only the original's
  dead store of `offset.x` to 0x30 (frame 0x28 vs 0x20).
- `Orca_2FD4::ComputePath_554AB0` (55 -> 12): the lazy `cur_z` store at LABEL_52 (a `new_z` split gives the
  shape but the wrong register, 82); the else branch's store order (reading `field_4` late gives an
  `eax`/`edx` swap, 82; the original's else-branch order gives the instruction order but `new_z` in `al`,
  not `dl`, 78).
- `Hud_Arrow_7C::UpdateScreenPos_5D0850` (86, Oct 6): its 9.6f copy is the unpaired 0x4C7FC0 (between the
  counterparts of 0x5D0620 and 0x5D0C90). It calls exactly the helpers our source uses (vec_len, get_camera,
  sine/cosine, ProjectWorldToScreen_Hud 0x4B90E0), in the same order, so it does not show where the 66..103
  missing caller-size units are. The plain `GetLength_41E260` gives the original's out-of-line Abs/Multiply/
  Add/SquareRoot but then the projection's operators stay inline (195): 9.6f 302 -> 212.
- `Hud_Arrow_7C::UpdateScreenPos_5D0850`: a static inline `GetLength` with a return per branch puts the
  results in one slot but pushes the `Fix16_Point_POD` ctor, one `+` and one `/` out of line (189 vs 304).
- `DrawTexture_5D8470` (76 -> 68): `rotation.rValue` brings the point ctor inline but the last `+` goes out of
  line (197).

Evaluation order and scheduling:
- `Car_6C::SpawnCabAndTrailer_446530` (180 -> 162): the x/y loads of the first spawn are inside each branch
  in the original; references for the inline params are worse (338).
- `sound_obj::AdjustPlaybackRate_41A580` (54 -> 36): registers in the last
  `Fix16(snd_rate) * (speed / (v5 + speed))`; `Fix16::Abs` fixes the abs but moves the `ebx` push (58).
  Permuter 900. The original multiplies with the difference in `eax`; every form gives `imul %esi`.
- `sound_obj::Type_10_HandleCarSkidSound_418940` (108): `&gCarInfo->field_28` cached across a call in ours,
  reloaded in the original; `>= 1` on `field_AC` fixes the `pPhysics` register but gives `cmpb`.
- `sound_obj::HandleCarTireScrubSound_418720` (4): the deferred `(%ecx)` deref of the
  `GetCarLinearSpeed` result. `operator/` on the temporary computes the `field_28` address before the call
  (120); a `Fix16&` to the result gives 8.
- `sound_obj::ProcessType7_Weapon_42A500` (32): the original loads `field_14` for `GetLoopStart` before the
  rate store; all orders and permuter 400 failed.
- `sound_obj::ProcessActiveQueues_41AB80` (131): the 3D sample-manager calls load `ecx` before the
  argument registers, the 2D ones after.
- `CarPhysics_B0::UpdateSteeringAngle_562560` (44): VC6 moves single-use division temps into their use and
  hoists the `Ratio()` call above the inlined `__allshl`/`__alldiv`; no form keeps the division first.
- `ComputeCarMassAndInertia_454410` (30 -> 22): a const-ref `inertiaBase` gives the slots; the `__allmul`
  operands stay swapped whatever the source order.
- `Char_B4::UpdateAnimState_546360` (84 -> 80): `field_40 + kAng180` loads the constant first; a `(u8)` timer
  compare reorders but the registers are wrong.
- `Camera_0xBC::IsSpriteInView_435630` (233 -> 160): the original computes the numerator first and spills
  the denominator.
- `Particle_8::EmitElectricArcParticle_540320`, `Particle_4C` 53B670: see their entries.
- `DrawText_5D8A10` (229 -> 20, u/v were swapped): the zero u/v stores come before the `DrawQuad` pushes in
  ours, after them in the original (420 orders tried).
- `thirsty_lamarr::DrawDigits_492260` (286): the original keeps `idx` in a stack slot cached in `ebx`,
  `curr_char` spilled to 0x12, and `height = (c == '0' && idx != 8) ? field_13[idx] : w`. Permuter best 199,
  unnatural. Try the `sub_492430` tricks (matching_quirks.md, dead parameter slots).
- `CarPhysics_B0::UpdateZPhysics_55AD90` (84, `lea` vs `add` on `cp3 + k`), `ComputeLineLineIntersection_55F3B0`
  (68), `ProcessGroundCollisionAndSurfaceType_55B970` (244, epilogue duplication): no progress. 55F3B0's
  original constructs no `Fix16_Point` up front and sets the EH state with `movl`; ours has two ctors and `movb`.
- `MapRenderer` diagonals (`Draw*Sided*`): a u32/f32 inline getter for `field_70/74` and moving the uv
  stores before or after the project calls: no change or worse. 4ECE40, 4ECAF0, 4EC7A0, 4EC450 and
  `draw_lid`/`draw_left`/`right`/`top`/`bottom`: only the x87 scheduling blocker is left.

## Map_0x370::CanMoveOntoSlopeTile_4E0130 (WIP)

Rewritten from the original asm: ratio 0.029 -> 0.509 (permuter score 1606 -> 528). The case layout
matches now. One `switch (path_direction)`; `break` means blocked and lands on a shared
`return true` after the switch (a `return true` in a then-block gets its own epilogue copy; one
reached by a jump is shared, like the original's `je 992`). Up/down dispatch with
`if (dir != NORTH) {...} else`, right/left with a `switch (dir)` (cases EAST/WEST, then NORTH/SOUTH,
default). `pSlope`/`pBaseSlope` start at 0 (the original's two zeroed slots). The below-block test is
`dir > NO_GRADIENT_SLOPE_0` (`test; jbe`), not `>= NORTH_1`. 9.6f 0x4656D0 is the same function with
no extra helper.

Left:
- y/z registers swapped (original: z in ebp, y in edi). Moving single uses between x, y and z flips
  the allocation with no obvious rule.
- After the first GetEffectiveBlock call the original uses the block from eax; ours reloads it.
- Up case: the below-block check should jump into the down case's identical code (`jmp 47B`).
- Right case, east climb wall check: should jump to the shared `return true`; ours gets an
  epilogue (no `break` possible inside the inner switch).
- Round 8 (528, unchanged): the up and down below-block sources are identical and in ours VC6 already
  shares their `pBaseSlope` check block, but not the run from `mov %cl,%al` to the `0xFF` epilogue that the
  original's up case reaches with `jmp 47B`. `z >= 1`, declaration order and an assignment in the `if`
  don't move the y/z registers.

Renames: `field_36E` -> `field_36E_bBlockedByTerrain` (no usable ground ahead, as opposed to a wall),
`field_36F` -> `field_36F_bLowerBlockHasArrows`, params `bByRefUnk` -> `pSlopeZDelta` (1 stepping up,
0xFF stepping down onto a slope) and `bNotifyByRefRet` -> `bReportStepUp`.

### Near-miss pass, round 5 (Ped / Char_B4 / Orca_2FD4)

- Matched: `Ped::Deallocate_45EB60` (4 -> 0): the bit 0 clear goes through a file-local
  `CompilerBitField32 ClearBit0_45EB60(CompilerBitField32 bf)` that takes and returns the flags word by
  value (`field_21C_bf = ClearBit0_45EB60(field_21C_bf);`). The by-reference helper, a direct
  `b0 = 0`, `&= ~1` masks and a local copy all keep `and $-2,%edx`; the by-value one loads into `eax`
  before the timer store and gives the original's `and $0xFE,%al`.
- `Char_B4::state_8_5520A0` (6, unchanged score, real call fix): the two `-k_dword_6FD868` arguments to
  `NewUnknown_52A240` are `k_dword_6FD868.Negate_4086A0()` in the original (out-of-line call; the
  scorer masked the COMDAT `??GFix16` copy). The `-dword_6FD87C` jitter stays inline (`neg`). Tried for
  the `ped->184` register rotation: a `Sprite*` local for `field_4` (16-27), no `field_184_pObj2C`
  reassignment (27), `field_40_rotation` set directly / via `Ang16 rot; rot = ...` (17): all worse.
- `Orca_2FD4::ComputePath_554AB0` (7 -> 4): else branch `t = ypos; dir = idx2; new_z = xpos;
  idx2 = t; idx1 = 0; xpos = zpos;`. Left: the `ypos` load is the first instruction of both branches so
  VC6 hoists it above the `jge`; every order that loads `idx2` first (like the original) flips
  `new_z`/the switch register to `al`/`edx` (32-34). Temp type (u8/char), declaration position, block
  scope: no effect.
- `Orca_2FD4::Internel_UpdateBehaviorGrid_554710` (19): distance written as y-part first, `* v12` last,
  s16/u16/s32 dx/dy temps (37-55), inline `DistSq`/`Sq` helpers with s16/u16/s32/u8 params (37-93): no
  gain. The late `push ebp` stays.
- `Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0` (15): not a logic bug (the in-loop empty list
  returns 0 like the original; ours just places that block last). `for(;;)` after an entry test (23),
  explicit `return 0`, inverted `++j > 6 && !maybe_timer` early return, `if (!field_18) return 1`
  first: all 15.
- `Ped::GotoAreaByAnyMeans_469060` (12): the original compares the `FindNearbyTileMatchingSlopeType`
  result with `cmp %bl,%al` (zero register) at both sites while the `CanAllocateOfType` result next to
  it is `test`. Return type u8/char_type (header), `(u8)` cast, `!= 0`, `!= false`, `== true`,
  assigning into a new u8/bool local (inline, `&&` statement, ternary 67): no change. The 4
  `FromInt_45C4E0` calls the callee checker reports are the COMDAT `??0Fix16@@QAE@E@Z`.
- `Ped::ComputeAimAngle_45C9D0` (4): the original stores `field_130` in each branch (`cx`/`dx`) and
  re-reads it at the join, but per-branch stores make VC6 copy the return tail into the atan2 branch
  (5); `Ang16&` to the field (5), ternary (15), inverted if (26), return through a local (5-9).
- `Ped::BusCustomer_AI_461290` (6): `IsDespawning_4215B0()` at any of the three despawn checks, door
  passed directly / as `field_24C`, door local type and position: no change.
- `Ped::AttackTargetStateMachine_46D460` (18): `== 15` first at the first jumping test gives 16 but
  moves the wrong block; at the second test 104.
- `Char_B4::CanStepForward_54FEC0` (tu.sh 2): the global store after the epilogue pops in the original;
  an inline `SetPathDir` returning false, `result = false`, dropping the `else`: no change.
## Near-miss pass, round 5 (Oct 5)

No new matches. Scores below are `permuter_score.py` lines.
- `sound_obj::HandleCarTireScrubSound_418720`: no 9.6f partner. Tried a static inline helper taking
  the speed by reference (28: the inline budget pushes `operator*` out of line) or by value (14),
  `/=` on the returned temporary (47: `lea field_28` before the call), an explicit
  `((__int64)call().mValue << 14) / max` (14: the dividend is loaded first), `Fix16 speed(call().mValue, 0)`
  (unchanged), a wrapper returning the call by value (= the `Fix16 speed = call()` form, stack slot read),
  a user copy constructor in fix16.hpp (19, RVO into the slot), and a const by-value member divide (49: argument
  evaluated before the call). The original reads the dividend late through the returned pointer, which only a
  temporary gives, but every temporary form evaluates the divisor's operands too early.
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220`: the dead store has to be `volatile`. A local array,
  a `Fix16` local, `(void)&old`, `s32* pOld = &old`, a do-nothing inline taking `s32&`/`s32*`: the store is
  dropped (13). A `volatile` pointer to it gives 32. The load in both branches of the clamp gives 23/26, a comma in the
  condition 2, a precomputed `cur >= 15` bool 4, `> 14` 3, a `*(volatile s32*)&old =` store 2. The 9.6f build (0x41C000) has no dead load.
- `sound_obj::TrainCab_414710` (6): `&&` for the two checks, early returns for each check, `if (!pDriver)
  return;`, `return` in the else, the else storing `pTrainStation` (null) instead of 0: all 6. Storing in the if
  branch only: 10.
- `CarPhysics_B0::ShowPhysicsDebug_559430` (4): `ThetaText_49E240` as an `Ang16` member (9.6f 0x49E240
  is `__thiscall` on the Ang16): no change.
- `CarPhysics_B0::CalculateRearWheelForce_5620D0` (10): `MultiplyByFix16_inline_5620D0` returning
  `Fix16_Point&` (as 9.6f 0x49E3A0 does), explicit `__int64` products in both orders, `x = x * f`, a
  `const Fix16*` parameter, a file-local helper (16: budget): all unchanged. The original multiplies `imull (%edi)` (the
  factor through its reference pointer); ours loads the factor into eax from the cached `gCarInfo` base.
- `CarPhysics_B0::ProcessPedImpact_560B40` (16): declaring `Divide_442CB0` `throw()` drops the EH state
  stores around it (16 -> 14), but breaks `Object_2C::ResolveCollisionWithPed_5229B0` (MATCH, same
  `Negate_40ACB0().Divide_442CB0()` pattern with the state stores), so it was not kept. The rest is temp slot placement:
  ours puts the `ComputeRelativePointVelocity_561130` return temporary in v16's slot (0x20, so v16.y's store is
  dropped), the original at 0x18. Declaration order of the 4 points and v16 makes no difference. v16 as a `Fix16_Point` in place of
  `unused` gives 126; an early `v16.x` store changes nothing (16).
### Near-miss pass, round 5 batch (2026-10-05), no new matches

- `BurgerKing_1::SetAltKeyState_498CB0` (1): still a byte load. Also tried `(u8)(u16)a1`, `(u8)(s16)a1`,
  `(s32)(u8)a1`, `(u8)(a1 & 0xFFFF)`, store then `>>=` on the global, `u8 v; v = a1; v >>= 7`, static inline
  helpers taking `const u8&`, `u32`, `s32`, ternaries `((u8)a1 & 0x80) ? 1 : 0`, and `u16`/`s16` parameters
  (header). `(a1 & 0x80) >> 7` and `(a1 & 0x80) ? 1 : 0` give `shr eax` + `and $1,%al`.
- `keybrd_0x204::GetLayout_4D6000` (3): `char Buffer[4] = "  "` (21, copies 3 bytes), sscanf in a static
  inline `HexToInt` returning the value, a `char*` walking pwszKLID+6, an `s32* pV = &v2` argument: all 3.
- `jolly_poitras_0x2BC0::SavePlySlotDat_56BA60` (2): all locals up top, `len = 126` just before the call (4),
  outer `do/while(--k)` count-down (18), `size_t len = 126` initialiser: no change.
- `youthful_einstein::SetNewFugitive_516590` (2): a static inline `SetArrowColour(Hud_Arrow_7C*, Player*)`
  holding the null check (2), if/else inverted with the message first (24), `Ped* pPed` local / no local /
  `Player*&` / `this->` + pPed (all 5: reload goes to `ecx` and the else's gHud moves to `edx`).
- `Wolfy_7A8::sub_543690` (6): `pObj->field_1A_timer = 0` in the in-loop return (14), `field_0[next_idx]`
  there (38).
- `Player::AddCarToHistory_5645B0` (8): indexing `field_54_car_history[i]` instead of the iterator (21).
- `DoorData_10::Init_49C340` (8): a `gmp_block_info* pBlock = &blockData` for the case stores (8, VC6 still
  hoists the tile load above the v8 store).
- `PedGroup::sub_4C8E60` (6): it is the `_$E` atexit destructor of `pedGroups_67EF20`; not attempted.
- `BurgerKing_67F8B0::modify_inputs_4CDF30` (8): the toggle as a static inline `ToggleBits(s32&, s32)` (12).
- `Car_14::SpawnTrafficCar_582480` (11): both case 1 and case 2 written `if (!field_8) {x_step = 1 ...} else
  {... x_step = -1}`: with identical statement order VC6 merges the whole case tails (23); with case 1's then
  block as `x_step = 1; xpos = ...` (or case 2's) the else blocks merge but the surviving `-1` block is
  case 2's, laid out after case 2 (11, same as now); case 1 normal + case 2 inverted (12). The original keeps
  case 1's copy. VC6 keeps the later copy in every form tried (also true for case 4 -> case 3).
- `Frontend::GetNextUnlockedMainStage_4B7270` (7): 9.6f (0x453230) has the same shape as 10.5. Writing the
  param in place with an `old_idx` copy (21, new slot), `result = main_stage_idx` only on the `== 2` path
  (20: VC6 propagates the constant 2), `return main_stage_idx` there (20, branchless), testing `result == 2`
  (7).
- `gtx_0x106C::BuildCarInfoContainer_5AA9A0` (6): `u32 doors_off = num_remaps + 0xE` indexing
  `((u8*)p)[doors_off]` twice gives the door code but moves `this` to `ebp` (26), as noted before.
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (1): the matched sibling's `u16& selected_idx` (+ `BYTE
  tmp` flag) trick does not carry over (10): here `field_7E` is not hoisted, so `ebp` is free and VC6 copies
  `si` into `di` instead of reloading. `*(u16*)((u8*)this + 0x6E)` still CSEs (1), `*(volatile u16*)&` (22).

## Round 6: "missing callee" audit (callee multisets via COFF relocations)
- Only `TrafficLight_20::Init_5C1D00` called different functions. In 10.5 the pavement checks also go
  through `get_block_452980` (new inline `Map_0x370::IsBlockPavementTypeAt_452980`), and the four lights
  call the out-of-line `Light_1D4CC::Alloc_5C2B70` and `sub_5C5CD0` (= out-of-line
  `LightIntensityRadius::SetRadius_463F10`, now `SetRadius_5C5CD0`, WIP, our body is 26 bytes like the
  original). New inline `Light_1D4CC::InitOutOfLine_469010`. Callee multiset now equal, 742 -> 738.
- No missing logic in the others: their source already has every call, VC6 tail-merges identical call tails
  that the original keeps apart (`MeleeAttackStateMachine_46B670` AddCash 1 vs 2, `HandleCarImpact_5538A0`
  Kill 5 vs 6 / ChangeNextPedState2 3 vs 5, `HandleImpact_528E50` PoolGive 2 vs 3,
  `ContinueMovementAfterCollision_54B8F0` set_xyz_lazy_451950 1 vs 2 / DispatchCollision 7 vs 8,
  `FindBestTargetPed_466BF0` IsSpriteInView 1 vs 2), or the other way (`Car_214::sub_5C8780`: the original
  merges case 8 into case 6's GetPedVelocity_45C920 call, ours keeps 2). Tried without effect: 5538A0 branch 3
  with the state/blood-burst tail copied into each arm (482), 528E50 `if (done) { PoolGive; break; } return;`
  (61), 466BF0 `bInView` local set in both arms (224).

### Round 6 (9.6f-at-0 WIPs, tail/epilogue leftovers)
All counts are `tu.sh` diff lines (stack offsets ignored).
- `HandleObjectCollision_548840` (9): three extra `Fix16_Point` locals at the top give the original EH state 4
  (8 lines), the "v19 = 4" in the old Hex-Rays comment agrees. Left: the success path `jmp`s to the shared EH
  epilogue. `goto END`, if/else around `field_5C = 10`, `point` declared in the block (18, copy elided) don't help.
- `HandleCollision_522E10` (16): As2C_40FEC0 as a result local or a ternary, and passing `As2C()` straight as
  the argument (9.6f pushes `&v13` first) change nothing. The matched `DispatchCollision_55CA70` has the same
  As2C shape and there VC6 copies the call into both arms, as ours does here.
- `PickUpCar_47F930` (16): only the two `call; jmp shared-EH-epilogue` exits differ (same class as 548840).
- `DoorData_10::Init_49C340` (8): a `gmp_block_info*` alias, an inline member setter for the pair, `u16` tile
  idx field, `v8` as s16/s32/u32 (10) don't stop VC6 hoisting the tile-idx load above the v8 store.
- `ClearAllBriefsWithPriority_5D4890` (4): 10.5 is byte-identical to 9.6f; only `push %ebp` is shrink-wrapped
  after the `pIter` test in the original. `if (p) do {} while (p)`, `break` for the inner `return`, if around
  StartCurrentBrief, local order and a `u32` param don't help; `priority == field` is worse (5).
- `EnforceGearSensitiveMaxSpeed_562D00` (8): only the y clamp store + epilogue copy (see earlier entries).
- `TryCreateRoadblockAt_577370` (12): writing the bBothSides arm as `if (r1.active) { if (!r2.active)
  r2.create(3); } else r1.create(3);` makes VC6 merge the two `r2.create(3)` calls (9 lines), but with the
  wrong block order. Early `return` after each r1 create plus one shared r2 check after the if/else also
  gives 9 (the r2 test isn't copied into the bBothSides arm). The else arm inverted the same way gives 40.

### Round 7 (structure score, control flow and call order)
Scores are `permuter_score.py --structure` (normal score in brackets).
- `ComputeScanlineIntersectionY_4F76A0` 46 -> 34 (76 -> 52): `pd = p1.Sub_40AC80(p0)` (operator- called by
  name) gives the original's EH frame for `pd`. Left: success returns `mov $1,%al; jmp` to the shared EH
  epilogue (tail duplication class). `4F77D0` scores the same both ways (34), left as is.
- `HasGreenArrowForPathDirection_4E5E90` 44 -> 40 (56 -> 58): no `default:` label, `return a3` after the switch.
  `if (a3) return X; return Y;` merges the wrong calls (40 too), `if (!a3)` 46. Original cross-jumps case 4's
  false call into case 3's true call and case 1's false call into the tail of that block.
- `sub_4E6190` (48): logic checked against the asm (all four directions correct). No outer default, inner
  `switch (a5 - 2)` (82), `return 0` vs break: no change; the original jumps case 3/4 into case 2/1's dispatch.
- `SpriteHitsDiagonalWall_4E1520` (58): the original calls `Fix16::FromInt_4926F0` (out-of-line Fix16(u32))
  + `Add_408660` for the block centre. `static_cast<const Fix16&>(Fix16((u32)x))` and `SetXY_432860` for the
  points leave the ctor inline (no change).
- `DrawDigits_492260` (156): ternary height, `offset_byte` local as in `sub_492430`, `(u8*)` reload of
  `field_13_offset[idx]`: all still thread the second `c == '0' && idx != 8` test (no change).
- `DrawBackground_4B6E10` (48): `blitRet =` on the first retry, explicit `return;`: no change.
- Control flow and calls already match, only scheduling/regalloc left: `IsSpriteInView_435630` (56; num/den
  locals no change), `SpawnCabAndTrailer_446530` (64; zero kept in a register), `CalcPacketLen_51F210` (124;
  `mov $3,%ebx`, `pBytes[4]` reloaded after the `pBytes[1]` store), `ShowJunctionIds_588620` (68),
  `sub_469FE0` (48; original keeps 0 in `ebx` and compares call results with `cmp %eax,%ebx`).
- `SetObjective2_463830` (146), `PickUpCar_47F930` (54): tail duplication class, not retried.

### Near-miss round (Oct 6)
Scores are `sc.sh` lines. Matched: `ProcessType7_Weapon_42A500` (see matching_quirks.md, "A call result summed
in one expression"); the same form replaced the dead-store workaround in the matched `HandlePedVoiceEvent_423080`.
- `TryCreateRoadblockAt_577370` (36 -> 32): the `bRoadblock2Active` local dropped, field read in each arm. Left:
  the bBothSides arm's `r2.create(3)` should cross-jump into the else arm's copy. Tried `return` after any subset of
  the creates (32), one shared `r2.create(3)` after the if/else with early returns (50-77, VC6 copies the call back
  or merges r1's create instead), `PoliceRoadblock_A4*` locals (32-105), a ternary orientation (106). Permuter 1200.
- `Ambulance_20::UpdateState_4FB330` (2): `HandleObjectiveState` once after the switch with `return` in the other
  paths (18: the join goes last), `<= 500` + break/return, `++` in the condition, `return` after the state store,
  no trailing `return`, `default` with break/return: all 2. Permuter 1500.
- `TagGameHudUpdate_4DADA0` (54): the original never stores 59 to `dword_6F5B74` on the first flash (jump-threaded
  into the `> 0` test with 59 in `ecx`), so the timer is likely a local (`timer = byte ? dword : 59`), but layout
  is the gap: the original puts the `if (!byte_6F59C0)` block right after the condition, then the not-flashing
  block + pager clear, then the rest. Tried: if/else with the rest after (B goes first, 56-211), a `byte_6F59C0`
  test after the if/else (threaded, 56), inline `IsTimerFlashTime` (54/142), goto (93/211). VC7 (`sc7.sh`, 9.6f has
  the same layout) puts the first-flash block after the first `||` term with the early-return form (58).
- `gtx_0x106C::BuildCarInfoContainer_5AA9A0` (8): 9.6f has `mov %edx,%eax` before the `lea 1(%eax,%eax)`, a
  conversion on the door count; static inline `DoorsLen` helpers (u8/s32/u32/u16 params and returns), `s16`/`u8`
  casts, `u16`/`s16`/`s32` `off`/length types, `remap[num_remaps]` indexing: all 8 or 76+ (`this` moves to `ebp`).
- `DrawPlayerStatsHelper_5D61A0` (10): `s32 width` puts width in `ebp`, which has no byte register, so it is loaded
  through `ecx` (32); separate assign, `-width + base`, `base -= width`, `s32 x_offset`, field read without the
  getter: 26-44. Permuter 1100 found nothing natural.
- `PoliceCrew_38::State6_ShutDown_574720` (46): `u8 i = 0` at the top in every spelling (`char_type`, assign,
  `i++`) gives the `ebp` zero register (128).
- `frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0` (52): the original's `test %ecx; jbe` before the second loop is a
  rotated `while (total < tableSize)`, not `if (tableSize) do {} while`, and the empty-table store reuses eax from
  the memset; a `while` gives the shape but shifts registers (94; VC7 76). The dead `(len + 9) & ~1` in the first
  loop is also in 9.6f; an unused aligned-length local is removed (52).
- `sound_obj::ProcessOtherObjects_41F520` (30): the vol 50 else-if of case 18 jumps into case 19/20's else-if tail
  at `sample_index = 1` in the original (so its own `bLoop = 0` stays). All six orders of its first three stores:
  only the current one cross-jumps (from `bLoop`), the others don't merge at all (34-43).
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (4): a volatile read (`*(volatile s32*)&field_552C[cur]`)
  puts the `cmp` before the load like the original but loses the stack store (24); volatile on both: 4. Uses
  folded away later (`old - old`, `old * 0`, `*p = *p`, `old != old`) lose the store (40).
- `sound_obj::HandleCarTireScrubSound_418720` (4): `Fix16& speed = call()` (8), `Fix16 speed = call() /= max` (worse).
- `sound_obj::TrainCab_414710` (6): store only in the if branch + `= pTrainStation` in the else (10), early returns
  for the two checks (6), no `pDriver` local (22), a `pCar` local (10).
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (4): `u16&`/`u16*` to the field for the loop compare (28),
  `(s16)` casts on either side (4-50), a `bool` flag (4), comparing against the field at the end (28). VC7 also CSEs.
- `keybrd_0x204::GetLayout_4D6000` (4): swapped byte stores (12), `u32 v2`, a `char*` to `pwszKLID[6]`, `u16` copy
  (26), `strncpy`/`memcpy` (26-28), an inline returning the buffer as the sscanf argument, a comma expression: 4.
- `Frontend::DrawCredits_4B7AE0` (20): a Duff-style shared tail for cases 0/1 (98), store order swap (20).

### Near-miss pass (after the Fix16_Point split)
Scores are `sc.sh` lines. No new matches.
- `Orca_2FD4::ComputePath_554AB0` 12 -> 8: `new_z` loaded first in the `abs < 1` branch (so the two branches
  don't both start with the `ypos` load, which VC6 hoisted above the `jge`), `t = ypos` first in the else
  branch (keeps `new_z` in `dl`). Left: in both branches the original stores `field_1B` before the second
  load; every order that does that (dir first, `t` after dir, `t` in the old `field_4_zpos` char) swaps
  `al`/`dl` for `new_z` and the switch index (82). Permuter 980 iterations from 8: nothing.
- `Particle_4C::UpdateAttachedEmitter_state_9_10_53B670` 61 -> 43: `Ang16 jitter; jitter = Fix16_To_Ang16(...)`,
  then `Ang16 angle = sprite->field_0; angle += jitter;` gives the original's load/add/store. Left: that slot
  is 0x10 (original 0x12, a dead 2-byte object below it), and the `<= 40` block's `zpos +=` (VC6 schedules
  the zpos load above the flags store, so no cross-jump into the 41..59 copy). `zpos = zpos + d`, `d + zpos`,
  `mValue +=`, an address-taken zpos: no change.
- `CarAI_78::AlignToLaneCenter_44AF00` (9): the only call difference is the 6th `PolarToCartesian` (else switch, west_4):
  the original calls 0x408680 for its cos product, as for the first five; written as `PolarToCartesian` VC6
  inlines both products there (nested budget 142, needs < 114). `inlsim --scan` gives exactly the original's
  6 out-of-line products for caller size -415..-472, or with >= 7 extra free sites after that site. Writing
  the 13 `Ang16(a - b).Normalized_406C20()` as `Ang16` operator+/- (ctor -> Normalize two levels down) gives
  exactly the original's 13 out-of-line / 2 inline Normalizes AND the 6 products with caller size +70..+212,
  but every operator form computes the sum in 32 bits (`mov mem,%edx; sub`, original `sub mem,%dx`): (s16)
  cast, s16 local, const method, by-value param, `(s32,s32,s32)` ctor, `.Normalized_406C20()` inside the
  operator all 32-bit. Only the top-level `Ang16(int)` form is 16-bit. Getters in the set_xyz args give the
  right count but change the tail merges (294).
- `CarAI_78::ScanAheadForObstacles_452060` (32): `zpos_` uninitialised (258), the `v7` block removed (126), cos/sin order (210),
  `Fix16 c = cos; c.Multiply(v7)` / assigned v7 (32). Permuter 500: nothing.
- `Weapon_30::fire_truck_gun_5E0E70` (10): moving the function (and its globals) to the top of the TU, an
  `EXTERN` `word_706DFA`, `Ang16(...).Normalized_406C20()`, `AddNormalized`, `operator+`, no turret local,
  a `Sprite*` local: all 10 or worse. Permuter 800: nothing.
- `Map_0x370::sub_4E6660` (4): `sub_4E65A0` parameter types s32/char, char/s32, u8/bool, s32/s32 change
  nothing; `pPrev = pBlock` after the call caches 1 in `ebx` (48), before `dist +=` / the `get_block`/the
  `SetRoadBlockAt` (82-94).
- `Particle_4C::UpdateCircularBurst_state_5_539890` (8): cases 4/5 both `set_id; dir.x = 0; dir.y = ...`
  (73: case 5 is then exactly the original's, but case 4 is scheduled `mov $0xE,%ecx` first and not merged),
  `dir.y` before `dir.x` (69), mixed orders (73/94), case 5 before 4 (77/126).
- `Ped::ComputeAimAngle_45C9D0` (12): per-branch `field_130` stores (14, tail copied into the atan2 branch).
- `CarAI_78::CheckRoadAhead_448770` (38): `!(a && b ...)`, `||` of the negated tests, `pBlock_____ = 0` before the
  get_block: same IL, no change.
- `Wolfy_7A8::sub_543690` (12): the in-loop return as `smallestVal_idx = last_idx/next_idx; break;` (88/68).
- `Car_214::sub_5C8780` (84): `field_30_sprite_type_enum` / swapped compare in case 1 do not stop the pSprite
  load being hoisted above the jump table.
- `Car_14::SpawnTrafficCar_582480` (8): six other if/else and statement orders for cases 1/2: 142-176.
- `PedGroup::sub_4C8E60`: still the `_$E` atexit thunk, not reachable from source.

### Ang16 operator pass (AlignToLaneCenter_44AF00)
- `CarAI_78::AlignToLaneCenter_44AF00` 9 -> 0 (MATCH): the 15 lane angles as `Ang16` operators with the angle global as
  the left operand (`kAng180_677ADE + dword_677A2E`; with `dword_677A2E` as `this` in the adds VC6 loads both
  globals as 32 bits, see matching_quirks "Inlined Ang16 operators on globals"), all eight rotations as
  `PolarToCartesian_41FC20`, and `Fix16 x_off, y_off` declared once per switch instead of per case (the
  inline budget then cuts exactly where the original does).
- Same operator rewrite (both add operand orders) on other WIPs, all worse, left as they were:
  `CarAI_78::FollowRoadDirection_44A1F0` 122 -> 494/707, `Char_B4::HandleGenericCollision_54A530` 267 -> 349,
  `Char_B4::state_1_5504F0` 310 -> 396, `CarDoorAlignmentSolver_545AF0` 875 -> 879,
  `Map_0x370::sub_4E7190` 218 -> 655, `Particle_8::EmitWaterSplash_53F060` 458 -> 459;
  `Map_0x370::sub_4E6660` 4 -> 4.

### Near-miss pass (CarPhysics open again)
Scores are `sc.sh` lines.
- `CarPhysics_B0::UpdateSteeringAngle_562560` 44 -> 0 (**MATCH**): new inline member
  `GetScaledTurnRatio(Fix16 scale, s32 turn_direction)` called with the division as the first (by-value)
  argument, see matching_quirks "A computation ahead of a call in one expression".
- `Sprite::PointInsideRotatedBounds_5A1490` 60 -> 4: `char_type result` set on every path and returned once
  (see matching_quirks "A `char_type` result variable keeps the jumps to one shared EH epilogue"). Left: the
  `bool` conversion sits in the shared epilogue instead of on the last path. `bool`/`s32` result, early
  returns, `char_type` return type (the only caller is `SpriteHitsDiagonalWall_4E1520`), a `||` chain through
  an inline "rotate and test" helper (758): worse.
- `CarPhysics_B0::SpawnSkidSegment_55D200` 58 -> 54: `*pBoxCorner = arg_4` (struct copy: both loads, then both
  stores) and `/ 2` instead of the `box_idx = 2` IDA artefact (the length then takes the dead `box_idx` slot as
  in the original). Left: the original also puts the `2` temporary in that slot (ours: a frame slot), and the
  clear branch falls into the shared EH epilogue while the main path jumps to it (ours copies the epilogue).
  Inverted condition, `return` after the copy, `return` in the else: same or worse (144). No `obj_x/obj_y`
  locals, declaring them after `len`: same 54; `len` assigned (136), `len` before `r` (96).
- `CarPhysics_B0::UpdateZPhysics_55AD90` (84): `Fix16::Add_ref` for the `cp3 + k` compares: no change (the
  original computes the sum with `lea` into a third register). Permuter 800: best 66, only by using the
  `get_cp3_40F800()` getter for the first `field_6C_cp3` read (not applied).
- `ComputeLineLineIntersection_55F3B0` (68): `RelVel * -(k + offset)`, `-(offset + k)`, a `restitution` local:
  no change (the original loads RelVel into eax and the negated sum into ecx; ours `imull` from memory);
  the dot product written into the expression or assigned: 283-548 (inline budget).
- `Char_B4::sub_54C3E0` (58): non-const / `u32` / assigned `face`, `char_type unknown`, `unknown` declared
  after `face`: 58-66. Permuter 700: nothing. The original's tail copies use different registers per copy
  (B: ecx/edx, C: eax/ecx), so only one of them cross-jumps one instruction earlier.
- `Sprite_4C::UpdateRotatedBoundingBox_5A3550` (57): height declared first (302), depth first (782), declare
  then assign (57). Permuter 600: nothing.
- `Ped::sub_469FE0` (102): the original keeps 0 in `ebx` (9.6f and VC7 too). `u8 x = 0, y = 0, z = 0;` at the
  top (then assigned) gives the zero register and 30 lines, but adds three byte stores the original doesn't
  have; not applied. `pCar = NULL`, `!= 0`/`!= false`/`== true`/casts on the `SpawnCrewInCar_5703E0` test: no
  `cmp %bl,%al` (always `test`).
- `CarAI_78::ReactToNearbyCar_451980` (96): `kAng180 + field_10_angle` (global as `this` in both sums) 96,
  global on the right 106; the real gap is `v21` spilled while the original keeps it in `bp` (and spills
  `field_0_car`), which also makes `kAng180 + v21` a 32-bit add.
- `Map_0x370::SpriteHitsDiagonalWall_4E1520` (64): `Fix16((u32)x).Add_408660(half)` / `+` (66/234). The
  original calls the out-of-line Fix16(u32) copy (an argctor) and constructs the z argument in place with
  `mov %esp,..` (EH arg address), plus the shared-epilogue jump.
- `Camera_0xBC::ApplyCarVelocityCameraOffset_436200` (58): rewritten in its 9.6f shape (0x41EBF0, 186 -> 8 there;
  only the bool vs s32 Ang16 compares are left): `Ang16 angle` declared first, `(> 45 && < 135) || (> 225 && < 315)`
  picks `Fix16(240)`, `*=` for the trailer and suspicion scaling, `(Fix16(8) - z) + *pZ`, `radius` assigned into a
  function-scope local. Same 58 in 10.5, without the block-scoped copy trick.
- `Camera_0xBC::ApplyCarVelocityCameraOffset_436200` (58, older): `offset` declared in the block, `PolarToCartesian`
  with `offset.x/.y` as the outputs: `offset` stays in registers (62/150), the original keeps it in a frame
  slot and stores `offset.x` before the second `Multiply_408680` call.
- `PedGroup::MergeWithOtherGroup_4C9B60` (104): `pOther = pPed->field_164_ped_group` before the test gives
  the original's `ebx` for it but loses `pPed` in `edi` (132).

### Near-miss pass (byte bit fields)
Scores are `sc.sh` lines.
- `Ped::Reset_45AFC0` 96 -> 0 (**MATCH**): `field_224` is now a union with a `CompilerBitField8`, and its
  three clears/sets are bit field stores; the `0xEF` byte clear no longer shares the `0xFFFFFFEF` mask
  register with `field_21C_bf.b4 = 0` (see matching_quirks "Bit clears on byte flag fields"). Plus
  `field_1F8_run_speed` stored before `field_1A0`, as in the asm.
- `Char_B4::state_1_5504F0` 310 -> 260: logic fix `field_58_flags &= 0x7F` -> `field_58_flags_bf.b7 = 0`
  (the old form cleared bits 8..31), and no `(u8)` cast in the slope bit copy, which gives back the `ebp` zero
  register. Left: the slope flag is an `int` in `ecx` in the original (an `s32` local gives that but loses
  `ebp`, 318), the frame slots of the IDA locals, and the argument load order of the first `get_block` call.
- `Start_NetworkGame_5E5A30` (170): a nested single-exit version with a `char_type result` and a
  `bConnected` flag (the `char_type` result trick of 5A1490) still copies the EH epilogue into the
  `result = 0` paths (380). The original also has an unused dword between the GUID (0x10) and the path
  buffer (0x24) in both 10.5 and 9.6f; a 20-byte GUID holder fixes the slots (170 -> 106) but is not
  natural, so not applied.
- `Sprite::Draw_59EFF0` (104): only x87 scheduling inside the four `ProjectWorldPointToScreen_4BA4D0`
  expansions (where the `(u32)` centre conversions and the vertex index load go) and a one-byte size
  difference; not retried.
- `Ped::TaxiCustomer_AI_460820` (86): `Max(dx, dy) > kFpTwo || bit || !passengers.IsEmpty() ||
  IsDespawning()` as one condition (the original's block shape) 88; `Fix16 dmax = Max(...)` (assigned or
  initialised) 86; `kFpZero != GetVelocity()` 106, `.mValue` compare / `!(==)` 88, a `vel` local 120-130.
  The original loads the compared value into `ecx` and the constant into `eax`, the opposite of
  `TrainCustomer_AI_461530` (matched, same expression).
- `Char_B4::UpdateAnimState_546360` (80): `(u8)` on the `field_68 > 5 ? 3 : 4` ternary gives the
  original's byte compare (`jbe`) but the ternary result lands in `cl` and `field_68` in `al` (original:
  the reverse plus a `mov %cl,%al` copy before `inc`), 118; `frame_limit = ...` local, `<= 5 ? 4 : 3`,
  `4 - (f > 5)`, a `FrameDelay(u8)` inline (u8/s32/const ref), operands swapped: 118-124. VC7 (`sc7.sh`)
  shows the same swap. `kAng180 + field_40`, `Ang16(a.rValue + b.rValue, (u8)0)` either order,
  `AddNormalized`: the constant is still loaded first (80).
- `sound_obj::Type_10_HandleCarSkidSound_418940` (108): the original divides with the dividend shifted
  first and `gCarInfo_48->field_28` loaded fresh after the call; `call() / max` caches `&field_28` before
  the call (ours), and `/=`, a `speed` local, `v4 = v4 / max` or the division inside the multiply all load
  the divisor first (108-122).
- `Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280` (166): `++field_46_sub_state == 5` with
  `field_46_sub_state < 4` gives the original's `inc %bl` but no `mov %bl,%dl` copy (240); `u8/s8/char/s32
  sub = ++field` 240-244; `u16/s16/s32/u32 sub` 166. The original has `this` in `edi`, ours `esi`.
  Permuter 800 from 166: 96, only by inlining the state-40 multiply (`corner.x * cos`) that the original
  calls out of line.
- `Garage_48::ParkCarAtDoor_534700` (116): the original pushes `&field_38` before the `field_40` compare;
  reading `field_C_sprite_4c_ptr->field_4_height` directly gives that (106), but 9.6f calls
  `IsLongerThanOneBlock_447ED0` -> `GetH_447E70`, so not applied (an inline copy of 447ED0 is 116).
  Permuter 1000 from 106: 73, by regrouping the `SetXY` sums (`w1 + Fix16(y) + d`,
  `kFpTwo + (Fix16(y) + w1)`) and swapping case 3's branches; noise, not applied.
- `miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180` (121; the earlier 266 was a wrong needle): no `pChar`
  local, `Car_BC* pCar` declared first, `gStoredCar->Reassign(8)` in the else: 121.
  `pParam2->field_8_car->Reassign(8)` gives the original's `eax` for `pCar` (107) but VC6 then folds the
  `four` local into immediates.
- `RouteFinder::ShowJunctionIds_588620` (136): the original stores the junction x/y bytes to the stack
  around the `FindGroundZ` call and converts them again after it (no CSE of `Fix16(x)`/`Fix16(y)`).
  `ProjectToScreen` taking `u8 x, u8 y` (converted inside) is worse (186).
- `NetPlay::ReceiveGameMessage_521890` (168): the original loads `timeGetTime` into `edi` and calls
  through it (three calls); ours calls the import directly each time. Not retried.

## x87 scheduler pass (Draw.cpp, sprite.cpp, ProjectVert_4EB940)

- `DrawText_5D8A10`: **MATCH** (20 -> 0). regsearch: window 38 (the glyph quad) needed 4 more nodes
  (limit 76) so that the zero u/v stores fall into the next window, after the `DrawQuad` pushes. Two
  per `Fix16 u/v(((w - 0.0001f)))` argument does it. The last `ecx`/`edx` swap in `cur_xpos + sprite_w`
  went away with `sprite_w`/`sprite_h` declared before `cur_xpos`. Parens in the `Fix16(f32)` ctor body,
  an `f32` local for the argument, `(0.0f)` zero stores, swapped sum operands or a `right` local: no
  effect or worse.
- `DrawTexture_5D8470` (78, Oct 6): its 9.6f copy is the unpaired 0x4CBDB0 (right after DrawFigure's 0x4CBA50).
  The 9.6f shape: `if (scale == one && rotation == zero) flags = 0x10000; else flags = 0;`, the half sizes assigned
  into locals, `point.SetXY_432860(+-v12, +-v13)` for the corners, `(x_pos + point.x).ToFloat()`. That scores 12
  against 9.6f (from 361; only the u/v/z store order and the `a9` load) and replaces the raw-negate inline-budget
  hack, but 10.5 goes 68 -> 78: everything left is x87/integer interleaving and regsearch's best is 17 (2 nodes
  per window), so not only the window breaks.
- `DrawTexture_5D8470` (68, older): regsearch best 19 (its metric) at limit 78 in every window, so it is not
  only the window breaks: the first `RotateByAngle` push order and vertex 1/2 load placement also
  differ. Greedy over 0-2 parens on each vertex `ToFloat()`, u/v and the `[3].z` store position: best
  21 (from 28; stack offsets ignored). The natural `[3].z` order (z right after y) scores 74.
- `Sprite::Draw_59EFF0` (104): regsearch says 15 nodes short per window (limit 65 -> 4). Greedy and
  all-pairs searches over parens in `ProjectWorldPointToScreen_4BA4D0` got to 2 lines with
  `pVert->x = ((((f60)) * (((px) - (cx)))) * (pVert->z)) + cx)` and the y line the same minus the
  `point.y` paren plus an outer one (ugly, not applied). What is left: the expansion-1 y-line `fmulp`
  at the start of a window issues before `mov %edx,0x14(%esp); mov $5,%edx` (see matching_quirks.md).
  With free double-paren/`f32`-local conversion helpers in place of `ToFloat()` the best is 10.
  `u32` temp for the screen centre (as in 4EB940): no effect.
- `ProjectVert_4EB940` (11 -> 4 with parens, not applied): regsearch limit 71 (9 nodes) gives 4;
  greedy over parens in the z/x/y lines and `set_vert_xyz_relative_to_cam_inlined` also 4 (`(1.0f / ...)`,
  `((xpos.ToFloat())) * ((f60))`, and four pairs round the y line's f60). Left: `mov %ecx,%eax` one
  slot early at the function start and `pop %ebx` before the last `fmuls`.
- `DrawFigure_5D7EC0` (430), `4EAF40`/`4ED290` (register allocation), the slopes (356-617) and
  `Draw_4F6A20` (structure): not x87 scheduling problems, not worked on.
### FPO marker follow-up (near misses)
- Matched with the FPO markers: `Char_B4::HandleObjectCollision_548840` (three unused `Fix16_Point` locals give
  EH state 4, the shared-epilogue jump now comes for free), `CarPhysics_B0::SpawnSkidSegment_55D200` (`Fix16 len`
  declared at the top: it and the `/ 2` temporary then share the dead `box_idx` slot), `Sprite_4C::UpdateRotatedBoundingBox_5A3550`
  (left/right/top/bottom declared, then assigned through the `Fix16_Rect` getters 9.6f calls; initialised they
  interleave), `Map_0x370::SpriteHitsDiagonalWall_4E1520` (`Fix16((u32)x).Add_408660(half)` and plain `z_pos`
  for the by-value z argument), `Start_NetworkGame_5E5A30` (`char path[MAX_PATH]`, the frame was 4 bytes short).
- `Car_14::SpawnTrafficCar_582480` 161 -> 8: cases 3/4 need `ypos = ...; y_step = 1;` in the `!field_8` arm, then
  case 4 cross-jumps into case 3 again. Left: case 2's `-1` block. Of two identical tails VC6 drops the copy
  that is a whole label block (its label is retargeted); with both whole the later copy survives. The original
  keeps case 1's `-1` label block, so case 2's copy must have been the whole block while case 1's was not; no
  if/else polarity or statement order (64 x 64 combinations, all case orders) or ypos written in both arms does it.
- `sound_obj::ProcessOtherObjects_41F520` (4): `max_distance` before `calc_distance` in case 13/14 (the original
  order) keeps 4/12 and 13/14 apart as in the original, but fire then cross-jumps into 13/14 (from `xor bl`)
  instead of 4/12 (from the volume store). All 5040 orders of the 13/14 statements after `samp_idx`, and 42
  joint orders of 4/12 + fire: no 0. Source order of the inner cases swaps the 4/12 and 13/14 blocks.
- `sound_obj::TrainCab_414710` (6) and `Ambulance_20::UpdateState_4FB330` (2): both originals have the exit
  block right after the first jumper to it, with the remaining block copying it. dupB's first loop does that
  (`jmp L` forward to a block whose predecessor ends in jmp/ret: the block, up to its ret, is moved after the jmp;
  `Ped::PunchChar_467FD0` matches that way). C2's layout moves a jmp-ending else arm right before its target, so
  the exit's predecessor always falls through in ours. Tried for Ambulance: `HandleObjectiveState` once after the
  switch with `return` in the other paths (18), plus the car path or case 6 returning on its own (6-20), four
  case 3 shapes x two case 5 shapes x all 24 case orders (case order changes nothing). TrainCab: the play code
  after the if/else with early returns (6, same pre-dupB list as now).
- `Particle_4C::UpdateAttachedEmitter_state_9_10_53B670` (12): only the jitter/angle slot (0x10, original 0x12).
  An extra unused `Ang16`, `jitter` at function scope, `radius` declared first, `angle(field_0)`, `AddNormalized`,
  adding into `jitter`, a separate `Fix16` for the jitter: 12-119.

### Near-miss pass (Particle_8 / sound_obj / misc owners)
Scores are `sc.sh` lines.
- `Char_B4::GetNextRotationToward_550F60` 452 -> 164: `word_6FDB2E` is an `Ang16` and the unused step angle is
  `word_6FDB2E.MultiplyByFix16_401CB0_ctor_ool(field_38_velocity)`. Initialised from an inline's return value the
  local lands in the temporaries area like the original's `v12` at 0xE (frame 0x28 -> 0x24); a named
  `Fix16 unused_vel` + `Ang16 v12(&unused_vel, 0)` gives it a named slot above the `u8` locals. Left: the
  original rotates the scratch registers from case to case (Delta args edx/eax, ecx/edx, eax/ecx; the sum in
  dx, cx, ...), ours uses the same ones in every case. Comparison forms, `Ang16(v12 + rot)`, operator vs
  `.rValue` arithmetic, a reference/temporary `v12`, a block scope: no change or worse; permuter 100 iterations
  nothing.
- `sound_obj::HandleVocalStreamSwitching_57DF10` 600 -> 254: control flow from the asm. Station restart after the
  `field_5500 == field_54FC` test (`if (!bStationChanged) { Update; return; }`, `else if (!bFast) { mode; return; }`,
  station code last), and the old position saved through `pos` before it is written to
  `RadioEmitter(...).field_18` (the original calls `GetVocalPosMs` before computing the emitter address).
  Left: the original lays the station block right after the `==` test and jumps back into it from the `!=`
  path; a goto into that block, two station copies, `(eq && !changed)` + `(eq || bFast)` (VC6 doesn't thread
  the re-test) are all worse. The original keeps 0/1 out of registers (ours keeps 1 in `ebx`).
- `Particle_8` (53E450, 53F060): the 9.6f source (`velocity.RotateByAngle_40F6B0(rotation)`,
  `word_6FD5CC.MultiplyByFix16_401CB0(...)`, `angle_1 + angle_2 - word.MultiplyByFix16_401CB0(Fix16(8))`,
  `-(velocity.x / 15)`) gives every inline decision the original has only with a larger caller: `inlsim --scan`
  needs +49..+76 size units for 53E450 and +80..+113 for 53F060 (then the out-of-line counts equal the
  original's exactly). Natural versions score 746/611 as is. 9.6f's 53E450 (0x48C9C0) aligns its frame
  (`and $-8,%esp`), so it had a `double`/`__int64` local that was optimised away: probably dead code that
  also explains the missing size. Not committed.
- `Car_214::sub_5C8780` (84): 9.6f writes case 7's ped check out in full (no fallthrough into case 6), and the
  10.5 `jmp` into case 6 is a cross-jump of that copy. With the copy (and the case 3 car path as if/else, two
  `GetBasePointer` calls like the ped path) ours gets other registers in cases 5/6/7 and merges case 5 into
  case 3 instead (96). Not committed.
- `Ped::StartCrossingRoad_45E4A0` (414): 9.6f calls four helpers 0x433470/0x4334A0/0x4334D0/0x433500 (N/E/S/W)
  that take the ped's (x, y, z) and offset inside. Written as such they give the same code as the current
  offset arguments. Left: the original spills x/z and the by-value block lookups' arguments to stack temps
  (frame 0x2C vs 0x1C) and reloads x/z before each jump to the next test.
- `Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280` (166): only `this` in `edi` (original) vs `esi`;
  the original also copies the incremented sub state to `dl`. `++field_46_sub_state == 5`, `u8 sub = ++...`,
  half_w/half_h or pB4 at function scope: no change or worse.
- `Particle_4C::UpdateObjectBeamLink_state_38_538AC0` (493): the original reloads `ang` after the first
  `Multiply_408680` (its address escapes), ours CSEs `ang * 4` in `esi` across the call, and VC6 then merges
  the two cases' second multiply. A by-reference PolarToCartesian helper, one function-scope `ang`, `base` as a
  temporary: no change or worse.
- `sound_obj::HandleCarDoorSounds_4182E0` (286): declaration order of the locals changes nothing.
- `Frontend::SetupMenuStringsOptionsElements_4B0220` (254): `regsearch.py` finds no window limit that helps
  (127 -> 125), so the store order differs in the IL, not at a window break.
## Near-miss pass 2 (VC7/9.6f first, then VC6)
Scores are `sc.sh` lines (VC6 vs 10.5); `sc7` is `sc7.sh` (VC7 vs 9.6f).
- `PoliceCrew_38::sub_571A30` 129 -> 0 (**MATCH**): `Car_BC* pCar` loaded inside each branch (9.6f) instead of
  once before `if (pGroup)`; the shared local let both compilers hoist `Get_F76` and moved the state tail.
- `RouteFinder::ShowJunctionIds_588620` 136 -> 0 (**MATCH**): 9.6f's `WorldToScreen_40CFC0` shape with two out
  pointers (file-local copy for this TU's constants) and `Fix16(pJunction->field_C_min_x)` at each use.
- `Ped::TaxiCustomer_AI_460820` 86 -> 0 (**MATCH**): VC7-guided rebuild (`Fix16 dx; dx = ...`, `field_150` re-read
  in the passed branch, `SetField238` before `field_150 = field_16C`, `Fix16 dist; dist = MaxAbs(...)` with the
  sprite fields read through `pTaxi->field_50_car_sprite`), the max written as a ternary inside the condition, and
  the abort code written twice (early `return` after the first copy) for the original's reload stub. The last
  register tweak (no `pSprite` local) came from the permuter in 101 compiles.
- `PoliceCrew_38::State5_PursueOrChase_572920` 142 -> 123 (sc7 381 -> 240): `field_0_car = 0` before the state
  store, `get_cam_y()` for the criminal's y, `field_278_ped_state_1` read directly in the loop test, status read
  twice, all four MaxAbs getters, `GetPedState_403990` in the enter-car test, `obj_43` with the `52` path written
  in both arms. Left: kill-char's `status == 2` block loses its `SetObjective` pair to `obj_28`'s copy (orig keeps
  the earlier one); `obj_28` polarity, `pPed` vs `gCurrentCrewPed`, merging cases 12/51: no effect. The inliner
  sends the wrong `Abs` out of line in kill-char's MaxAbs (nested budget 61 < 64). `inlsim.py --scan` asks for +20
  caller size, and `{}` padding there proves it (sc 123 -> 101), but no natural change gives it: `u8 status`,
  `else if`, `pCriminal` (no change) and `pPursuitTarget` (worse, 248) locals all failed.
- `eager_benz::OnPedKilled_592660` 269 -> 77: second switch with cases 9..20 first (layout), a `pCar ? model : 87`
  local like 9.6f, swat case store order (sc only). Left: the first switch's layout (army falls into the
  dispatch in 10.5 and 9.6f); all 8! case orders sampled (45 random), store orders from 9.6f: worse.
- `Garage_48::ParkCarAtDoor_534700` 116 (unchanged; sc7 575 -> 97): 9.6f store order, file-local
  `IsLongerThanOneBlock_447ED0`, no point locals, 9.6f operand order. 9.6f's `Fix16(y) - (d + w1)` is two subs in
  10.5 (kept `- d - w1`). Left: VC6 gives `w2` and `&field_38` swapped callee-saved registers; permuter 1700: 93
  with operand-order noise only.
- `Weapon_30::throwable_5DDFC0` 148 -> 0 (**MATCH**; sc7 391 -> 124): one `speed = e74 + (a3/60) * (cf0 + e80)` per
  branch, `!vector.IsNull_420360()` (reads `FIX16_POINT_ZERO`), and `field_24_pPed->field_21C_bf.b22 = true` written
  in both arms of the reload-speed `if` instead of once after it (permuter, 164 compiles). That one store moved the
  constant 1 into `bl`, which was the 141-point register difference. VC6 doesn't merge the two identical stores.
- `Map_0x370::sub_4E5640` 257 -> 233: `Fix16 distance; distance = GetLength()`, zero stores in source order.
  Left: stack slots (pos_diff and the subtraction temporaries). VC7 prefers the initialised form (263 vs 363).
- No change: `Char_B4::UpdateAnimState_546360` (`(u8)` ternary forms; `kAng180 + field_40` load order),
  `HandleCarImpact_5538A0` (zero register in `ebx`), `CarAI_78::DetectCarAhead_44D1D0` (north/east tail keeper),
  `NetPlay::ReceiveGameMessage_521890` (loop forms: `for(;;)` differs from `while(1)`, none closer).

## Particle pass (9.6f unpaired counterparts)
Scores are `quick_score.sh` lines. `match_96f.json` lacked the 9.6f copies of several Particle functions; found by
address order and callee overlap: `PoolUpdate_53D260` -> 0x490760, `UpdateObjectBeamLink_538AC0` -> 0x48F230,
`EmitElectricArcParticle_540320` -> 0x48DDC0, `UpdateSkidOrScrapeSpark_53A280` -> 0x48BEE0 (size 0x38b),
`UpdateSimpleBallisticMotion_53ABA0` -> 0x48C270, `UpdateLargeBallisticDebris_53AE60` -> 0x48C3E0,
`UpdateDebrisArc_53B1A0` -> 0x48C590, and probably `UpdateCollisionBurst_53BAC0` -> 0x490130 (size 0x628).
- `Particle_4C::PoolUpdate_53D260` 112 -> 0 (**MATCH**). The only difference was the constant 1 kept in `bl`
  (`timer = 1` store and `return 1`) in the four animation cases. VC6 counts the uses of a constant before it
  merges identical code, and 4 more byte uses were needed (k extra `x = 1` stores: 3 no, 4 yes; a dword use makes
  it `mov $1,%ebx` instead). `-= 1`, `+= 1`, `x = x - 1`, `>= 1`, `(u8)1`, `true`, a `u8 one = 1` local all count
  0 (folded first). Writing the frame-delay condition as `if (sub < 6) timer = 1; else if (sub > 10) timer = 1;`
  adds one store per case and VC6 merges the two branches back: 0. Two separate `if`s don't merge (86).
  Duplicating whole case bodies per label is not merged either (392/1026). Same trick as
  `Weapon_30::throwable_5DDFC0`.
- `Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280` 166 -> 0 (**MATCH**). 9.6f 0x48BEE0 reads the car box
  through `get_car_width()`/`get_car_height()` (9.6f 0x48A930/0x48A950, `GetW_420740`/`GetH_447E70` inlines)
  instead of `field_50_car_sprite->field_C_sprite_4c_ptr->field_0_width`; with the inlines `this` goes to `edi`
  like the original and `half_w` to `esi` (166 -> 66). Then `u8 sub = ++field_46_sub_state;` (66 -> 0): the copy
  (`dl`) outlives the stored value (`bl`, reused for the angle). With the old register layout the same spelling
  scored 240, which is why it was rejected before; `s32 sub` keeps 66. `IsDespawning_4215B0()`, an `Ang16 angle`
  declared after `Remove_477B00` (9.6f default-constructs one there), `corner` declared after the call: no change
  or worse.
- `Particle_8::EmitElectricArcParticle_540320` (20, was 12 by the old count): the 9.6f copy (0x48DDC0) has the same
  shape (`Fix16(s16)` ctor 0x401AE0 for both rng values, `MultiplyByFix16_401CB0` result copied into `angle`,
  `RotateByAngle_40F6B0(angle)`, `SetFlags_4337D0(2, 20)` for the `0xA2` store, now written that way). Left: the
  original copies the raw rng result (`mov %eax,%ecx`) and sign-extends it after loading the word
  (`movswl %cx,%ecx`); ours extends in place and loads the word into `ecx`. `Fix16((s32)rng)`, an `s16`/`s32`
  helper parameter, `s16 r` local: the register choice becomes right but the `Ang16(Fix16*, 0)` ctor tail changes
  (product stored before the pushes, 26-42; `regsearch.py`: not a window-limit effect). `Fix16(Ang16(rng).rValue)`
  gives everything but a 16-bit struct copy `mov %ax,%cx` for the 32-bit `mov %eax,%ecx` (16). Swapping the operand
  order inside `Fix16::operator*` changes nothing (VC6 canonicalises it). Permuter 600: nothing.
- `Particle_4C::UpdateAttachedEmitter_state_9_10_53B670` (12): 15 more forms of the jitter block. Both named `Ang16`s
  pack into one dword in ours as `jitter` 0x12 / `angle` 0x10; the original has the live object in 0x12 and a dead
  2-byte one in 0x10. `Ang16 angle(a.rValue + jitter.rValue, 0)` (12), `field_0 + jitter` passed as a temporary
  (165, frame +4), `jitter = Ang16(field_0.rValue + jitter.rValue, 0)` (28-72), `angle` declared first (52, VC6
  then stores the default ctor's 0), `angle.rValue += ...; Normalize_406C20()` (13, the ctor's inline Normalize
  copy and the explicit call become different symbols).
- `Particle_8::EmitBloodBurst_53E450` (325): `inl.sh` shows one inline decision off: `Fix16(word) * Fix16(8)` is
  inline, the original calls `Multiply_408680`. Through `MulAng16_401CB0(word_6FD5CC, Fix16(8))` (9.6f's shape) it
  goes out of line but the score gets worse (405). The frame is 0x58 vs 0x68 and the original zeroes a third dword
  at entry (0x7C) that ours doesn't have; 9.6f (0x48C9C0) counts the loop down (`count = 6; ... while (--count)`)
  and keeps the two `15` divisors in dword locals (10.5: `edi`), both already the same in ours. Loop forms,
  `Fix16 zero(0)` locals, `Fix16(0, 0)`, a dead `f64` local: no change.
- `Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0` 562 -> 463: the original calls `Abs_436A50` once
  (`compare_callees_multiset.py`), so `GetLength_OOL_6FD49C` has the first `Abs` (x == 0) inline and the second
  out of line, like `MinDistanceToAnySpriteBBoxCorner_5A22B0`. The original also keeps the two
  `RecordWeaponHit_512C00(id, 194/198, 1)` calls separate (`push $1` hoisted, the id loaded into `eax`/`ecx`),
  ours merges them into one. Left: stack slots and frame from the start; 9.6f copy probably 0x490130.
- `Particle_4C::UpdateObjectBeamLink_state_38_538AC0` (493): not attempted this pass; 9.6f copy 0x48F230.
## DrawGradientSlope* (MapRenderer, Oct 6)

Scores are `quick_score.sh` lines. `DrawGradientSlopeNorthwards_4F0420` 399 -> 0 and `Westwards_4F22F0`
617 -> 0 (**MATCH**), `Southwards_4F1660` 356 -> 8, `Eastwards_4F33B0` 524 -> 16. What it took, in order:

- `u16 side_word` block-scoped (one per `if (gBlockX) { if (gBlockY) {` block): the original reuses its
  slot for the lid temporaries, so the frame is 0x14, not 0x18.
- The coordinate adjustment before the `side_word` assignment, written `gXCoord = gXCoord + k` /
  `- k` instead of `+=`/`-=`. The statement order keeps `eax` (the side word) live across the add, so
  the add uses ecx/edx. The operator form matters for the **inline budget**: `operator+`/`operator-`
  (52 each, six sites) use up what `+=` (free) does not, and the last `ProjectVert_4EB940` call's two sums
  then go out of line as `Add_408660` calls, as in 10.5 (`inl.sh`: budget 317 -> 65 before that site).
- Lid colour: `u16 colour_sel = (gLidType >> 10) & 3; if (colour_sel == 0) colour = field; else colour =
  GetColour_4F0BD0(colour_sel);` (16-bit compare, `and $0xFFFF` before the call, field in the fallthrough).
  A ternary changes the inlining (221); a nested block for `colour` changes nothing.
- Two bugs: Northwards' right side word is `| 0x1000` (was `| 0x10`), Eastwards' lid flag is `| 4`.
- The inlined `ProjectVert_46BC70` x/y lines: both operands converted through an `f32` local with
  parentheses (`Fix16ToF32_Rounded3` / `Rounded2`). regsearch had said window 35 (the first lid else
  branch) needed ~10 more nodes, and the `idiv`/`fmuls` order needed a node *between* `fildl` and `fmuls`
  (the f32 local), not after. The heavier operand is loaded first, and that order differs per function
  in 10.5 (N, W: x first; S, E: field_60 first; the out-of-line 4EB940 copy: field_60 first). With equal
  weights VC6 breaks the tie by something in the function's context that we don't reproduce for W (3 of
  4 agree with a plain body; renaming the function, the lid flag constant, the colour field, and the
  single side blocks' order don't flip it, dropping the first block does). So S and E have their own copy
  (`VertProjector3`, field_60 heavier). Left for S (8) and E (16): `fildl y` one or two integer
  instructions later than the original in the y line after the next vertex's `idiv`, and the final `fstps`
  sunk below the next call's `add`/`mov`/`push`. Tried ~40 conversion-form combinations (parens 0-5,
  one or two f32 locals, `(f32)` casts, different forms for the x and y lines): best stays 8/16, and the
  forms that help E (R1/Q1: 12) hurt S (32).
- `ProjectVert_4EB940` 22 -> 4: the same conversion forms as the S/E slopes (`Fix16ToF32_Rounded2(xpos) *
  Fix16ToF32_Rounded3(field_60)`, field_60 first as in 10.5) and `Fix16ToF32_Rounded` for the x/y
  conversions in `set_vert_xyz_relative_to_cam_inlined` (puts the index `mov %ecx,%eax` behind the z
  temp store). Left: `pop %ebx` before the y line's `fmuls 8(%ecx)` instead of after it. The y line's
  node count (0-3 parens, with or without the f32 local, x and y lines separately) doesn't move it.
- `MapRenderer::Draw_4F6A20` 448 -> 442, but the logic now follows 10.5: `x_semi_distance++` when
  `(max_x - min_x) % 2 != 1` (the increment was commented out, and the test was on the half-width), the
  block loops start at `semi_distance - 1`, the four `AddToDrawList_46BB90` calls go right/down, left/down,
  right/up, left/up, and the z factors are `cam.z + Fix16(8 - zLayer)` (the `- Fix16(zLayer) + Fix16(8)`
  form folds differently). Left: zLayer in `ebp` instead of `ebx` from the top, `Fix16(zLayer)` kept in a
  register instead of a spill slot (frame 0x24 vs 0x20), the lights block's tail duplicated into the loop
  init, the first inlined `AddToDrawList` storing y through the `lea`'d pointer. Indexed stores in the
  helper (746) and `Fix16(gZCoord_6F63E0)`/`Fix16(gZCoordTop_6F62B0)` for the z factors (728) are worse.

## Police_38 / CarPhysics_B0 pass (9.6f first)
Scores are `quick_score.sh` lines (10.5) and `permuter_score.py --96f` lines (VC7 vs 9.6f).
- `CarPhysics_B0::UpdateZPhysics_55AD90` 84 -> 0 (**MATCH**), 9.6f 258 -> 24 first. 9.6f 0x4A2240 settled:
  `field_6C_cp3` read directly everywhere (no `cp3` copies, no reference: a function-scope `Fix16&` hoists the
  `lea` in VC7), `g_ZPos_6FE0AC * a2` (global as left operand), `pCar->IsFlagSet_411930(0x2000)`, the slope test
  as `!(slope && frac != 0 && zpos <= cp3 + k)` (9.6f calls `not_equals` then `<=`), `GetFracValue` as the 9.6f
  static `Fix16::GetFracValue_42A630(const Fix16&)` (a file-local static with the same body changed VC7's register
  allocation: it has to be a class member), and `ComputeSlopeCorrection_55AB50` with two real outputs: the old
  code overwrote the parameter. For 10.5 the parameter is `Fix16 a2_ = a2` at the top (so it lives in `ebp` from
  entry) and its dead stack slot is the second output (`&a2`), giving the original's single `push ecx` frame.
- `PoliceCrew_38::State6_ShutDown_574720` (46, 9.6f 78 -> 0 with `u8 i = 0` at the top and
  `get_objective_403A80()` for the first objective test; the getter is applied, same 10.5 code). With the index at
  the top VC6 makes `ebp` a zero register (128). Probing it: changing any one of the zero stores (`i = 0`,
  `field_60 = 0`, `field_28_state = 0`, `gCurrentCrewPed = NULL`, either `byte_6FEB48 = 0`) to a non-zero value
  removes `xor ebp,ebp`; the enum zeros pushed to `SetObjective*` and the `char_type field_29 = 0` store don't
  count; a zero store inside an inlined helper still counts; `bool`/`char_type` typing of `byte_6FEB48`, `true`,
  `!= NULL`, `while` loop, inverted if/else, statement order: no change. So the original has one zero store fewer
  in VC6's count with the same code; not found. A dead `u8 x = 0` at the top is eliminated.
- `PoliceCrew_38::State5_PursueOrChase_572920` (275, 9.6f 73): the 9.6f gap is only VC7 keeping the kill-char
  `status == 2` SetObjective pair as the surviving copy (ours jumps to obj_28's copy at the end); 10.5 keeps that
  copy too (case 12/51 and 28 jump into kill-char at 0x4ba). The 10.5 diff is the `MaxAbsDistance` cut-off:
  the original inlines Abs(dx) with its negate, calls `Negate_4086A0` for Abs(dy) and `Max_44E540`; ours sends
  both negates out of line. `inlsim --scan`: +144 caller size, or exactly one top-level inline site fewer after
  the MaxAbs call (nested budget 5166/18 = 287 vs 271). All 18 later sites are 9.6f calls, so none can be a
  field access. `pPed = gCurrentCrewPed_6FEDDC` reassignments (folded, no change), `pPed->SetObjective2` in the
  pair (198/446), inverted `if (!criminal)` or a ternary for `field_8` (292/290, 9.6f worse).
- `ComputeLineLineIntersection_55F3B0` (68, 9.6f 520): the natural 9.6f form (operators, `DotProductInlined`,
  `Square_49E0E0`, `Dir * (vel / mass)`) scores 470 against 9.6f and 348 against 10.5: 9.6f's `IsNull` returns
  `int` (0x420360, `test %eax`), its `DotProduct_49E500` is a static with two by-reference points and a hidden
  return, and the sums are evaluated right operand first. Not finished.
- `PoliceRoadblock_A4::CreateRoadblock_575FF0` (435), `ApplyImpactForcesAndDamage_55FA60` (179),
  `ProcessGroundCollisionAndSurfaceType_55B970` (210): not attempted.
## Ped.cpp / sound_obj.cpp pass with the unpaired 9.6f counterparts (Oct 6)
Scores are `permuter_score.py` lines (10.5) and `--96f` lines (9.6f) unless noted.
- `Ped::BusCustomer_AI_461290` 12 -> 0 (**MATCH**; 9.6f 0x4427E0: 128 -> 14). 9.6f shape: case 38 holds a full
  copy of case 34's "leave bus and flee" block (VC6 tail-merges them), `SetField238_403920` /
  `set_occupation_403970` / `is_driven_by_player` inlines. The two eax/edx swaps (`field_150` pointer, door byte)
  disappeared with the duplicate block; the merge then kept case 38's copy until case 34 was written as if/else
  (`if (36 && status) { block } else { pCar_ && IsDespawning ... }`): the copy whose block is a jump target is the
  one dropped, so case 38's then-arm copy is the one that jumps forward. Case 31 got its own despawn check
  instead of the goto into case 34.
- `Ped::FindBestTargetPed_466BF0` 86 -> 0 (**MATCH**; 9.6f 0x437BE0: 50 -> 47). Two IsSpriteInView calls (the
  shared `push $1` is VC6 hoisting the identical heads, a ternary argument pushes the 1 after the join). The
  shared `return 0` block sits at the first `return 0` in source order, so the dz / same ped / distance early
  returns became one `&&` condition and the IsSpriteInView arm's `return 0` is the first. The `switch` on the
  sprite type is guarded by `if (pNear)` with the final `return 0` after it: with `switch {...} return 0;`
  VC6 lays the trailing `return 0` out right after the dispatch (`dec; je PED; [ret 0]`) and that fall-through
  copy wins the merge; `default: return 0;` alone leaves the car case falling into the epilogue, so the
  epilogue isn't moved into the return-0 block (`return pBestPed` then gets its own copy). Only the permuter's
  COMDAT `Max_41E130` penalty remained (4); the build verifier accepts it.
- `sound_obj::ProcessOtherObjects_41F520` (4, unchanged; 9.6f 0x41A3C0 is far: Fix16 ctor/div/mul calls for the
  constants). Fire merges into whichever of the identical 4/12 and 13/14 blocks is later in layout (13/14 first in
  source: fire -> 4/12 but the blocks swap); the original keeps both blocks, fire in 4/12 (first). Inner case
  orders, `default` position, fire after the explosion case, statement orders (stores stay in source order, so no
  order gives identical post-scheduling blocks): no 0. Permuter 2500: 4.
- `Ped::HandlePedHitByObject_45D000` (74, unchanged; 9.6f 0x441A30 added as pair) and `IsPedAThreat_465D00` (142;
  9.6f 0x4371D0 added, 604). Left in 45D000: inside the inlined IsPedAThreat, the first
  `IsRespectNegativeForPlayer(player->field_2E)` site goes through a stack byte temp in the original (a `u8` local
  or `get_idx_4219D0()`, as 9.6f calls it at both sites), and the `MaxAbs <= kFpOne` compare has eax/ecx
  swapped. A getter or `u8` local at the first site makes the two IsRespect blocks identical and VC6 merges them
  (203); `pMyGang->` instead of `this->field_17C_pGang->` (187-237) and the gang block restructured in 9.6f order
  (256, 9.6f 530) are worse. 9.6f: `sub_4614E0` is `MaxAbsDistance_42A6B0`, the 9.6f source uses the getter.
- `sound_obj::HandleVocalStreamSwitching_57DF10` 254 -> 294 but 9.6f 0x4B25D0 600 -> 328: the parameter is used.
  `0x18(%esp)` after the prologue (push ecx + 4 saves) is the argument, and every `test %al,%al` before
  `SetVocalSpeed` reloads it: `if (bFastForward)` doubles the speed, the local `field_54FC == 1` is only
  PlayVocal's append flag. With that every instruction matches and the constant-1 `ebx` is gone. Left: the
  original merges every duplicate tail into the first copy (`Update; ret`, `push 0; SetVocalPosMs ...`,
  `xor edx; div`), ours into the later ones, so the `!changed` Update stays in place and the station head isn't
  moved up into the `jmp STATION` slot. Tried: `char_type bFast`, a separate `bAppend` local, `eq ? changed :
  bFast` ternary (384), two flat `&&` tests (372), `!=` test first (288), two station copies (388, 508).


## char.cpp / Wolfy_3D4 pass (9.6f first)
Scores are `quick_score.sh` lines (10.5) / `permuter_score.py --96f` lines (VC7 vs 9.6f). No new match.
- `Char_B4::HandlePedCollision_548BD0` (16): the two spots are `kAng180 + atan2` at the type 5 / 3-4-6 site
  (original `mov kAng180,%cx; add (%eax),%cx`, ours loads the result first) and `word_6FD888 + angle` in
  `TurnBy16Deg_548BD0` (`lea (%edx,%eax)` vs `(%eax,%edx)`). Operand order at the site (both ways), the
  result in a named `Ang16`, `atan2() + kAng180` with/without `Normalized_406C20()`, `angle + word` in the
  helper, the sum written out at the call, and `EXTERN_GLOBAL` declarations for `kAng180_6FD936` /
  `word_6FD888` (the 2-byte-global quirk): all 16 or worse (84-184). Value numbering ("the operand numbered
  later is loaded first") does not explain it: the atan2 call result should always be the earlier value.
- `Char_B4::sub_54C3E0` (58, 9.6f 183): the paired 9.6f 0x4994D0 is a different function (a switch on the
  face comparing `field_40` against four angle globals, then 0x491F10/0x4725B0); the pairing is wrong, so
  9.6f gives no shape for this one.
- `Char_B4::state_0_54DDF0` (188, 9.6f 982): only two real spots, the rest is jump-offset noise. (1) the
  `pAhead` block type: the original loads `gMap` into `edx` before pushing z and re-tests `bl` after
  `and $3` (the written-out ternary, the `GetBlockTypeAtCoord_420420` inline (204), an if/else with the null
  case first, a `u8` temporary (705): no change). (2) the conveyor adds: the original loads `Saved_Xpos`
  before `field_4C_conveyor_dx` (both `+=` orders: no change). 9.6f default-constructs the two `Ang16`s and
  builds two zero `Fix16` pairs up front (frame 0x30 vs our 0x24); not pursued.
- `GetNextRotationToward_550F60` (164, 9.6f 437), `Wolfy_7A8::sub_543690` (12, 9.6f 16): looked at only; the
  10.5 diffs are the per-case scratch-register rotation / the `edx`/`eax` temp swap already in the notes.

## winmain / eager_benz / misc owners pass (9.6f first)
Scores are `quick_score.sh` lines (10.5) / `permuter_score.py --96f` lines (VC7 vs 9.6f). No new match.
- `SetWindowedMode_5D9510` (14 / 14): both compilers put `push $0x316` before the height arithmetic in the
  original and after both size expressions in ours, so it is source shape, not scheduling. Tried: the
  original's evaluation order (`window_height - top - bottom + top + bottom`, 34: swaps the two RECT slots),
  WinMain_5E53F0's matched form `w + (r - l) - (r - l)` inline (14) and as `s32 w, h` locals (85), the flags
  as a `UINT` local or as `SWP_*` names, both RECTs declared at the top, the call result in a `BOOL`, the
  sizes through file-local inline helpers (34): nothing moves the push.
- `eager_benz::OnPedKilled_592660` (77 / 222): 9.6f 0x4B7EB0 lays the whole non-network block (occupation
  switch with the kill-type switch inside it, flag stores tail-merged across cases in the order
  bFbi, bCop, bSwat, bArmy, bGangA, bGangB) *after* the scoring block, jumping back to it; the French test
  calls `get_occupation_403980` once per compare. Writing every case's six flag stores in that fixed order
  is worse (260 / 300), with the per-compare getters 260 / 232. The current (tuned) store orders stay.
- Triage by real (non-jump) differing lines: `BurgerKing_1::read_input_device_498DA0` 104 (53 lines, all
  stack-slot shuffles of the four input dwords, see its entry), `Object_2C::HandleSpriteZCollision_5238B0`
  146 (82, the `field_50 == 1` branch and tail layout), `NetPlay::ReceiveGameMessage_521890` 168 (115,
  `call *%edi` through a register and base/index order in `0x760(%esi,%ebp)`). `TagGameHudUpdate_4DADA0`
  (54 / 58): the first-flash block layout, already covered in its entry.

### SpawnTrainsFromStations_578860 (Oct 6)
- 10.5 601 / 9.6f 877. The call sequence matches 9.6f 0x4AFE20 one for one. `SpawnCarAtCorrectZ_Scaled` exists only
  because this TU's copy of the spawn scale constant is `kFpOne_6FF07C` (9.6f's `SpawnCarAtCorrectZ_426E40` reads one
  global, 0x5E4D4C, and takes the rotation by value). The plain inline with a per-TU `CAR_6C_SPAWN_SCALE` define
  (like `FIX16_POINT_ZERO`) gives 9.6f 747 but 10.5 935: the frame grows 0x68 -> 0x78 (by-value `Ang16` copies and
  `temp_z`), so not applied.
## Ped.cpp / sound_obj.cpp / Car_BC.cpp, second pass (Oct 6)
- `Ped::ComputeAimAngle_45C9D0` 12 -> 0 (**MATCH**; 9.6f 0x43E3A0 16). `field_130` stored in each arm as 9.6f
  does. The old note that per-arm stores copy the return tail predates the `Marker_<addr>_fpo` fix: the tail is
  21 bytes with the `[esp+x]` SIB bytes counted, so dupB jumps to it.
- `Ped::MeleeAttackStateMachine_46B670` 442 -> 454 lines, structure 44 -> 54, 9.6f 0x4436A0 182 -> 104: shape from
  9.6f (hurt blocks `IsField238(2) && mugger` with one TakeDamage else; health >= 20 nested with two TriggerVoiceEventRateLimited_433E50
  sites, which the 10.5 asm shows too; knock-out tails `network ? Kill : (b5, Set_F250(18))`), and the
  punch-to-death fallthrough bug is gone. Left: the original keeps two mugger (AddCash) blocks, the first hurt
  block's copy merged into the else-if's; VC6 merges all three of ours into one. Hurt block as `&&` in the health
  branch too: structure 74.
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (4; 9.6f 0x41C000 40): 9.6f has no dead load at all, so
  the `cmp $0xF,%al` before the load/store is a scheduler move: the dead local isn't volatile. Unexplained how it
  survives dead-store elimination.
- `Car_14::SpawnTrafficCar_582480` (8): 9.6f 0x4B34E0 writes cases 1 and 2 identically (`if (!field_8) {+1}
  else {-1}`), so the earlier 64 x 64 polarity/order search covered the real shape; the survivor of the two `-1`
  label blocks (original: case 1's) stays unexplained.
- `Ped::GotoAreaByAnyMeans_469060` (30): `cmp %bl,%al` (bl = 0) on the FindNearbyTileMatchingSlopeType result at
  both sites, ours `test`: a `char_type` return type, `!= 0`, a `u8 bFound` assigned in the condition: no change.
  kill_char_20's MaxAbsDistance loads the target's y first: getters (348) and a cached `pTarget` (no change).
- New pairs scored: `Ped::CalcApproachPointNearTargetPed_4645B0` 266 / 9.6f 842, `Ped::IsThreatToSearchingPed_4661F0` 418 / 1759,
  `AttackTargetStateMachine_46D460` 46 / 307, `StartCrossingRoad_45E4A0` 414 / 166, `IsPedAThreat_465D00` 142 / 604,
  `Car_BC::HandleCarHitByObject_43F130` 882, `Car_214::sub_5C8780` 84 / 422, `SpawnCarOnRoadNetwork_4458B0` 469 / 855.

## Weapon_30.cpp / sprite.cpp / Ped.cpp helper-variant pass (Oct 6)
- `Sprite::Draw_59EFF0` (104; 9.6f 0x4BE060 388): 9.6f calls all three helpers as functions (`sub_4BA4D0` x4,
  `sub_4B9BC0` x2, `sub_4B9C70` x1), so they existed. `ProjectWorldPointToScreen_4BA4D0` compiled with VC7 against
  9.6f 0x4BA4D0: the body is the same (VC7 keeps the reciprocal in a temp either way; an explicit `f32 z` local
  changes nothing for VC7 and makes 10.5 worse, 158). 9.6f passes `zpos` on the stack (`ret $4`, `lea 0x18(%esp)`
  before its ToFloat) with the other two arguments in registers, so the original took `Fix16 zpos` by value: the
  helper's 9.6f diff goes 74 -> 62 (frame and VC7's register convention left), Draw's 10.5 score stays 104. So
  the remaining 10.5 difference is only the x87 window problem already documented.
- `Weapon_30::sub_5DE4F0` (222) has no 9.6f counterpart: 9.6f 0x4CE970 (throwable, 0x5CC bytes) runs straight into
  0x4CEF40 (sub_5DF270), so the electro-baton beam code between `throwable_5DDFC0` and `sub_5DF270` is new in 10.5.
- `Ped::CalcApproachPointNearTargetPed_4645B0` (266; 9.6f 0x436BF0 842): 9.6f calls `PolarToCartesian_41FC20` x4 (cross-jumped from the
  per-case sites), `compound_add_41FA70` x13, `angle_plus_40E5A0` once (case 2 is `angle = kAng180 + angle`) and
  `ctor_40E590` once. The real `Ang16::PolarToCartesian_41FC20` at every site: 10.5 409 / 9.6f 487; plus
  `operator+` in case 2: 892 / 475; `angle = rotation; angle += kAng180;` for the first assignment: 612 / 846.
  The per-site variants encode the 10.5 inline budget (inl.sh: Normalize goes out of line at nested budget
  52-63 and inline at 67+), so this needs the budget tools, not shape changes.
- `Weapon_30::fire_truck_gun_5E0E70` (10): unchanged, no 9.6f pair (the function is new in 10.5 or renamed).
- `Ped::HandlePedHitByObject_45D000` scores 137 after the merge (74 before; the merged fix16/ang16 header changes
  moved the inlined IsPedAThreat code).

## Particle pass 2 (9.6f first, bracketed pairs)
Scores are `quick_score.sh` lines (10.5) / `permuter_score.py --96f` lines (VC7 vs 9.6f).
- New 9.6f pairs by address bracketing between the paired neighbours (added with `add_96f_target.py`):
  `EmitWaterSplash_53F060` -> 0x48D1F0, `EmitFlameStreamSegment_53F4C0` -> 0x48D4E0,
  `EmitFireTruckSprayParticle_53FAE0` -> 0x48D8B0.
- `Particle_8::EmitWaterSplash_53F060` 458 -> 0 (**MATCH**). The plain 9.6f form: `word.MultiplyByFix16_401CB0(
  Fix16(rng))`, `(angle_1 + angle_2) - word.MultiplyByFix16_401CB0(Fix16(8))`, `RotateByAngle_40F6B0`, a count-down
  loop (`for (count = 6; count != 0; count--)`), `DivideInt_53E860(15).Negate_4086A0()` for the New_53E3C0 args.
  That scored 2: the rotations' x_old negate was the COMDAT copy of `operator-` while the arguments called
  `Negate_4086A0`, and the two are not folded in our build (the build verifier failed on it). A file-local
  `RotateNegExport_` (operators inline, `x_old.Negate_4086A0()`) gives one symbol for all three: 0.
  `-v.DivideInt(15)` instead inlines the negates (563); `RotateByAngle_NegOOL_40F6B0` changes the budget (433).
- `Particle_8::EmitBloodBurst_53E450` (325 / 317): the same plain form scores 524 / 251; with `RotateNegExport_`
  646. Its original frame is 0x68 vs our 0x54-0x58 and 9.6f (0x48C9C0) aligns the stack (`and $-8`) and keeps the
  two `15` divisors in dword locals before the loop: it has locals we don't see.
- `Particle_8::EmitFlameStreamSegment_53F4C0` (710 / 595 -> 685 / 572 with `PolarToCartesian_41FC20` as the real
  call, `vector += vector_2 + get_x_y()`, `Set_2C_0x4_Flag_4337F0()`): 9.6f constructs only `Ang16 angle` up front
  (no `Fix16_Point(Fix16(0), Fix16(0))`), builds the New_53E3C0 x/y as two `Fix16(0)` locals, and the car branch
  `Fix16(0)`s in place; not rewritten.
- `Particle_8::GunMuzzelFlash_53E970` (428 / 933): 9.6f has no unused sin/cos multiplies in the car branch (ours
  needs them for the 10.5 Multiply count), `AsCharB4_40FEA0()` inline in the `offset + ...` expression (579 /
  933), removing the multiplies 998 / 841. Not pursued.
- `Particle_4C::UpdateObjectBeamLink_state_38_538AC0` 493 -> 343 (9.6f 881 -> 436), see the commit: 9.6f's
  0x48A270/0x48A250 as `Fix16_Point::MaxAbs_48A270` (with `Abs_436A50`) / `DivideAssign_48A250`, `SetXY_432860`,
  point operators, `delta.atan2_40F790()`, `SetFlags_4337D0(2, 20)`, u8 `Fix16(field_46_sub_state)` radius.
  The case bodies keep `NormalizedAng` + `MulInto` (SetFromPolar inline: 578). Left: frame 0x5C vs 0x54 and the
  slot order of i/segments/the points; `Fix16_Point_POD target` + a named `Ang16` for the atan2 result give the
  original's frame size by accident (Ang16 temporaries) but not the layout (351).

## Small-WIP pass (Oct 6, agent/small)
Scores are `quick_score.sh` lines. One new match: `LightIntensityRadius::SetRadius_5C5CD0` already compiled to
the original's 26 bytes, it only had no `target_asm.json` entry (disassembled 0x1c5cd0 from `10.5.exe` by hand).
- `PedGroup::sub_4C8E60` (12, the `_$E` atexit destructor of `pedGroups_67EF20`): VC6 does map the special name
  `__ehvec_dtor` to `??_M`, so `void __stdcall __ehvec_dtor(void*, unsigned, int, DtorFn)` declared in a .cpp
  compiles `__ehvec_dtor(pedGroups_67EF20, 0x44, 20, (DtorFn)0x4CB870)` to exactly the original's five
  instructions. But the mangled name follows the declared pointer type: the CRT defines `??_M@YGXPAXIHP6EX0@Z@Z`
  (`P6E` = thiscall pointer) and VC6 has no `__thiscall` keyword (`C4234`, the keyword is ignored, `_thiscall`,
  `__pascal`, `__fortran`, `__fastcall`, `__stdcall` give `P6A`/`P6I`/`P6G`), so ours is the unresolved
  `??_M@YGXPAXIHP6AX0@Z@Z`. VC6's link.exe has no `/alternatename` (LNK4044). A destructor's address can't be
  taken either. Dead end from source without a tool change.
- `Door_4D4::dtor_49D570` (92): the dropped EH frame reproduces in a 12-line TU: a member array of a class with
  an out-of-line dtor plus `delete` of a pointer whose class has an *inline* dtor, after `#include <new>`; with an
  out-of-line pool dtor the frame stays. In Door_4D4.cpp `<new>` comes through Garage_48.hpp -> ... -> Ped.hpp
  -> char.hpp -> sprite.hpp -> gbh_graphics.hpp -> d3ddll.hpp -> DmaVideo.hpp (`<set>`, `<vector>`), so dropping
  the Ped.hpp/Object_5C.hpp includes doesn't help. `#define _SET_`/`_VECTOR_` before the includes removes the
  frame problem in the test TU but DmaVideo.hpp's `Renderer` uses `std::vector`/`std::set` members (line 282),
  so the TU doesn't compile. Pre-declaring `operator delete` without `throw()` or including `<new.h>` first
  changes nothing. Only the shared fix in the quirks doc (keep DmaVideo.hpp out of gbh_graphics.hpp) is left.
- `struct_4::CleanupSpriteList_5A7080` (20): 9.6f 0x4BF070 has the same A / keep / B block order (VC7 even hoists
  the shared `push %esi` of the two DeAllocate calls). 13 shapes all leave the keep block as the fall-through
  into the loop test: both `continue` forms with B after the if/else, `goto head_unlink` with the label after
  `continue`, `for (;;)` + `break`, `if (pIter) while`, `if (!pIter) return` at the end of the body, inverted
  outer/inner conditions (44-58), B-with-continue before A (44).
- `Wolfy_7A8::sub_543690` (12): the two `lea` temps are swapped (orig in-loop `edx`, final `eax`); 9.6f lays the
  final tail first, so the original may have allocated it first. `goto found` with the label after the final
  store (12), `Wolfy_30* pW` locals in either tail (12), swapped `smallestVal`/`_idx` stores (16), swapped
  declarations (16).
- `jolly_poitras_0x2BC0::SavePlySlotDat_56BA60` (4): 9.6f 0x4A89E0 also has `mov $3,%ebp` before the `len` store,
  so the counter init precedes `len = 126` in the original IL. `s32 k = 0` declared before the memcpy with
  `for (; k < 3; k++)` or `while (k < 3)` (4: the count-down init still goes to the preheader), `k = 0, len = 126`
  in the for-init (4), pDst before the memcpy (80), a `stage_stats*` walk (34), count-down with `3 - k` (190).
- `keybrd_0x204::GetLayout_4D6000` (4), `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (4): reviewed only, the
  earlier notes cover every spelling tried.
- `SetGamma_5D9910`, `sharp_pare_0x15D8::ReadTextures_5B92E0`, `ErrorLog::ErrorLog` (0x4D94E0): maintainer
  decisions (crashing `Write_4D9620` call, standalone guard, real `ofstream` member), not retried.

## CarAI_78 / map_0x370 / Orca_2FD4 / Draw pass (Oct 6, 9.6f first)
Scores are `quick_score.sh` lines (10.5) / `permuter_score.py --96f` lines (VC7 vs 9.6f).
- **Matched** `CarAI_78::CheckRoadAhead_448770` (38 -> 0) and `DrawFigure_5D7EC0` (430 -> 0), see their comments and the
  commit messages. For `CheckRoadAhead_448770` the 9.6f shape (globals compared directly, the `== 3` gtx helper) did not
  change the 10.5 score; the last 38 lines were the zero register's start, fixed by an else-if tail with an
  explicit `!pBlock_____` arm. For `DrawFigure` the 9.6f shape (`Fix16 v12; v12 = ...`, `Fix16(u8)`, one
  equality condition) gave 9.6f 312 -> 42 but 10.5 382 -> 540: the two extra statements grew the front-end
  size and pushed `RotateByAngle`'s nested operators the other way. `inlsim.py --scan` found the size range
  with the original's x16/x8/x3 out-of-line counts; the distribution (`Negate_4086A0` inline only in the
  second rotation) then needed +2 size units, which `u32 flags` (as in DrawTexture) gives.
- `DrawTexture_5D8470` (78 -> 8 / 12): `((x_pos + point.x).ToFloat())` on all eight vertex conversions (one
  x87 no-op node each, regsearch had said 2 extra nodes per window) and `verts[3].z` stored right after
  `verts[3].y`. Left: one window where ours issues vertex 2's `fmuls`/`fstps` two integer instructions early;
  regsearch's best is now 4 at the stock limit (so not a window break), and extra parens on vertex 1/2's lines
  (`(((..)))`, `((a + (b)))`, `(f32)`) are 20-53.
- `Orca_2FD4::Internel_UpdateBehaviorGrid_554710` (42 -> 8 / 44): `++field_8_pNode` before `++field_C_node_count`
  gives the original's late `push ebp` (ebp then lives only in the distance branch); `v7 = dy² + dx²; v7 *= v12;`
  as two statements keeps the product in the sum's register. Left: the original loads the x difference first
  (edi) and the y difference second (esi); both VC6 and VC7 compute the y term first for every sum order, cast,
  temp type (u16/s32/s16/u8 temps for dx/dy change the whole function's allocation, 76-228) and `Sq()` helper
  tried. A `u16 dx` temp alone flips the order but mirrors the copy/in-place squaring (14).
- `Orca_2FD4::ComputePath_554AB0` (8 / 136): the first `abs < 1` branch in the original's order (`field_1B`
  store, then `new_z` load) and the else branch in 9.6f's order (`field_1B = idx2` first) both swap the roles
  of eax/edx for `new_z` and the switch index (82-90) whatever the declaration order/type of `new_z` or `t`
  (also `cur_z` assigned directly, 26). The 9.6f shape is exactly that order, so the register choice is the
  remaining puzzle.
- `Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0` (54 / 158): the call sequence matches 9.6f 0x49D7A0 one
  for one; the 9.6f diff is a zero register for the five `= 0` stores. 10.5: the loop's bottom `jne top` must
  fall into the shared `return 0` epilogue with the `return 1` block last. `if (!node_count) return 1; do {...}
  while (node_count); return 0;` puts the `return 0` right but then the entry test gets its own `return 1`
  epilogue copy (82).
- `CarAI_78::ReactToNearbyCar_451980` (96 / 503): 9.6f 0x431770 shows `v27 = v21 + kAng180` (this = v21), which
  gives the original's 16-bit `add %bp,%di` form (106, the diff then is only v21 in `bp` with `field_0_car`
  spilled vs ours the reverse), two `Ang16` locals initialised from `kAng0_677CE8` at the top (dead stores VC6
  drops) and no default `Ang16` ctors in the turn block (ours has two). `Ang16 v21; v21 = atan2(..)` copies the
  result out like the original but v21 still goes to a slot; `= kAng0` initialisers (in place or at the top)
  move cBC to edi (106-140); a `Car_BC* pCar` local for the switch and turn calls, `Car_6C* p60`, `v21 += 180`
  in place, `v26/v27 = kAng0` inits: 96-150. The real `MaxAbsDistance_42A6B0` is 281 (budget), `RawY` stays.
- `CarAI_78::FollowRoadDirection_44A1F0` (122): the two `SetGoStraight` tail copies are at `if (v3 > (v2 + word_677CE2))` (dropped
  in the original, `jmp`) and `if (v3 < (v2 - word_677CE2))` (kept). Flipping either if/else so SetGoStraight is
  the else arm: 176-196.
- `Map_0x370::sub_4E8370` (104 / 108): the three `height - offset` loop bounds load the offset first in the
  original (and in 9.6f), ours the height; the matched `CloneColumnExtendedToZ_4E8220` has both orders for the
  same expression, so it is allocation context. Casts (u32/u16/int/char), `(u8*)` indexing, `i + offset < height`:
  104-818. `do_drop` is `mov al; test al` in the original, `cmpb $0` in ours.
- `Map_0x370::sub_4E6190`: the original computes `a5 - 3` in cases 3/4 and jumps *back* into case 2/1's inner
  switch (`je` past the `dec`), i.e. the earlier copies are kept; VC6 keeps the later ones for every outer
  order we can write. Not retried.
- `CarAI_78::ScanAheadForObstacles_452060` (32): slot offsets only. The original's frame (0x1C) has the cosine temporary at 0x0 and
  `v7`/`new_z`/`v1` at 0x4/0x8/0xC; ours has the three locals at 0x0/0x4/0x8 and the temporary at 0xC.
  `decl_shuffle.py` cannot run on it (interleaved declarations). `Fix16 v7;` at the top or outside its block
  (126), `v7` assigned after its declaration, `new_z` or `v9`/`v10` declared at the top: 32. `v85` and `zpos_`
  are live later, so they cannot be dropped.
  Slot rule (Oct 7, `Scripts/regalloc/slotlog.py`): our list is v82 (size 2), new_z (4 refs), v85 (3), v7 (2),
  then the `$T`s. v82 opens slot 0 and v7 shares it (disjoint blocks); new_z, v85 get slots 1, 2; the cosine
  temporary gets a later one. That's our layout exactly. The original (temp 0x0, v7 0x4, new_z 0x8, v85 0xC) needs
  v7 ahead of new_z in the list (more memory references than new_z, or new_z fewer), v7 not sharing slot 0 (its
  scope overlapping v82's, e.g. v82 declared at function scope), and the cosine temporary sharing slot 0.
### Mid-list pass (Oct 6, Fable worker)
Scores are `quick_score.sh` lines.
- Matched `gtx_0x106C::BuildCarInfoContainer_5AA9A0`: `u32 door_len = doors * sizeof(door_info) + 1;` added to
  `off` at both the total and the pointer advance. One expression folds into a single lea; two uses of
  `off + door_len` keep the original's lea / add / add and the `this` register (the earlier u16 cast is gone).
- Matched `sound_obj::sound_obj` (0x419CD0): `field_1468_v1 = Fix16(0)` (the Fix16(s32) constructor) for the three
  Fix16 fields. `operator=(s32)` returns `*this`, and that reference chained the four stores in the scheduler,
  so `mov %esi,%ecx` for GenerateIntegerRandomNumberTable_41BA90 landed after three of them. `sched.sh` shows the
  chain (each store's successors include the next); with the constructor the stores are independent.
- `Fix16_Point::NormalizeSafe_442AD0` (52): slot map of the original: first GetLength temps yy/Add/xx at
  0xC/0x10/0x14, and the if-branch reuses them (sqrt temp 0xC, the 128 const 0x10, second yy 0x14, Add 0x18, xx
  0x1C, `scaled` 0x20). Ours never overlaps the two expansions' temps (0x10..0x18 and 0x1C..0x24, frame +8). The
  `length = scaled.GetLength()` reassignment is right (the original copies the sqrt temp into slot 8 before the
  divide). Tried: named `xx`/`yy`/`sum` locals inside either inline (52-182), a `length2` local (86: it takes
  slot 8), `s32 scale = 128` (58), a static inline for the scaled branch (127), early return (86), plain
  `GetLength_41E260` (124: budget gives Abs(x) and x*x inline, y*y out of line, the reverse of the original),
  permuter 1500 (best 40, noise).
- `DrawPlayerStatsHelper_5D61A0` (10): 9.6f (0x4C9B40) does `and $0xFF,%eax; mov %eax,%ebp` after the width
  getter and a signed `/2`, so width is an `s32` from a `u8`. With `s32 width` VC6 gives ebp to width and ebx
  to base_xpos and loads the byte through ecx (32); `s16`/`s8` keep the registers but convert in two steps
  (10/12); `u8`/`u16` spill or `shr` (58-60). `const s32&`, `register`, a static inline s32 getter, `& 0xFF`,
  a `half_width` local, `base_xpos -= width`, a `sprite_num` local, `const` param: all 32.
- `Ambulance_20::UpdateState_4FB330` (2): with `HandleObjectiveState` once after the switch and `return` in
  the ClearTask paths and the case 3 else arm, the layout is byte for byte the original's except that the
  conditional jumps to the join / exit go to the end (18): ours lays out [join][inc arm][exit], the original
  [inc arm][join][exit] (the inc arm ends in `jmp exit`, so dupB's first loop moves join+exit into default's
  `jmp join`). All 24 case orders, inverted case 3 condition (50), and every break/return mix of the
  per-case HOS form (2-18) leave the inc arm after the join.
- `sound_obj::TrainCab_414710` (6): `if (!pTrainStation) { store; return; }` early return inlines the store
  block after the test (26); `&&` for the two checks, early returns inside the success path, success path
  `return` + store after the if: 6.
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (4): the original has a 4-byte local (`push %ecx`) that
  holds `field_552C[cur]` and 9.6f has none. `const s32`, `const s32&`, `old = old`, `*(s32*)&`, `if (old !=
  old)`: store dropped (40). `volatile u32`: 4.
- `miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180`: `find_96f_counterparts.py` finds no convincing 9.6f pair (best
  0.5 on a 139-byte function), so no 9.6f shape to follow.
## CarPhysics_B0 / big-WIP pass (Oct 6, 9.6f-guided)
Scores are `quick_score.sh` lines. Matched: `ApplyImpactForcesAndDamage_55FA60` (179 -> 0), `ComputeLineLineIntersection_55F3B0`
(68 -> 0), `ProcessGroundCollisionAndSurfaceType_55B970` (210 -> 0); see their commits and the new quirks entries.
- `Garage_48::ParkCarAtDoor_534700` (116): 9.6f 0x489BC0 confirms the shape (one `w` value stored to two locals,
  `Fix16(char)` ctors, `SetXY_432860` storing x then y, `Fix16(y) - (d + w1)` for the double door) and has an
  aligned 0x134 frame (one hidden-return slot per out-of-line operator). Left: the case bodies compute y, then x
  with the constant loaded into the x register (`mov k,%ebx; lea (%ecx,%ebx),%edi; ...; add %edx,%ebx`), ours
  interleaves them with ebx/edi swapped, and the prologue pushes `&field_38` before the inlined
  IsLongerThanOneBlock compare. A block scope around y/z/w1/w2/switch, `field_44 = 0` first, `255 == field_3E`,
  a `bool bLong = field_40` local: 116-120 each and combined. Permuter 730: 70, only with regrouped sums
  (`w1 + Fix16(y) + k`, `Fix16(y) + (k + w1)`) that fix the prologue as a side effect; not applied.
- `PedGroup::MergeWithOtherGroup_4C9B60` (104): 9.6f 0x404EF0 has the same first loop as 10.5 (the list pointer
  spilled to pPed's slot, `i` homed at 0x10, pPed in edi) so it is register priority, not a source form: ours
  gives the strength-reduced pointer edi and reloads pPed. The second loop already matches (pOther/pTarget take the
  registers). `pOther = pPed->field_164_ped_group` before the test (126/132, pPed then lives in eax only),
  `field_30 = 1` before the branch (122). Permuter 1500: 78 only by re-reading `field_4_ped_list[i]` for the first
  GetBit2 (changes the `test k,%eax` into `testl k,mem`; rejected).
- `NetPlay::ReceiveGameMessage_521890` (168): the original reloads pOut/pPlayerIdx from their slots in the loop and
  keeps the `timeGetTime` import in edi for the two calls at the loop end (`mov __imp__,%edi; call *%edi` twice);
  ours hoists the two parameters into ebp/edi and calls through memory. Our build does CSE the import when a
  register is free (`NetPlay 0x51ED00`), so again register priority. Permuter 650: 130, only by deleting the
  `sub_521820` call (rejected).
- `Object_2C::HandleSpriteZCollision_5238B0` (146): the `field_50 == 1` arm. 9.6f 0x4843A0 lays the slope path's
  `cmp $1; je` out to the non-slope `*a5 = 1; z = 0; speed = 0` block (VC7 merged the identical blocks) and the
  `!= 1` arm (`cmp $4`, default) right before the final CheckSpriteMovementRegion. In 10.5 the then-arm is a
  separate inline copy that stores `*a5` from `al` (eax == 1 after the compare), which keeps it from merging with
  the non-slope copy; ours merges the two copies (22 bytes + jmp, over the 20-byte merge limit) and so inverts
  the branch. `switch (field_50)` in three case orders (164), `s32`/`u8 v15` locals for field_50 (146/148),
  `*a5 = field_8->field_50` (168: `mov %cl,%dl; mov %dl,(%eax)` and only the tail merges), `*a5 = true` (146).
- `Car_214::sub_5C8780` (84): reviewed only (see the earlier entries): ours loads pSprite into eax before the
  `jmpl` (and `lea -1(%eax)` instead of `dec`), case 3's car branch is tail-merged into case 5 and its
  GetBasePointer argument is one push after a ternary where the original pushes in each arm.
### Mid-list pass 2 (Oct 6, Fable worker: sound_obj / Ped / Police_38 WIPs)
Scores are `quick_score.sh` lines. No new matches; each target stopped at one of the mechanisms below.
- **Duplicate whole blocks: VC6 keeps the later copy, the original keeps the earlier one.**
  `Ped::AttackTargetStateMachine_46D460` (46) and `PoliceCrew_38::State5_PursueOrChase_572920` (275) both have
  two identical blocks (`b11 = false; ret` in the inner `else` and the else-if's `else`; the
  `SetObjective2(0,9999); SetObjective(0,9999); break` pair in the kill_char `status == 2` test and in
  `case objective_28`). The original lays the block out at the *first* site, as the fall-through of its test,
  and the second site jumps backward into it (`jmp 0x4BA`, `je 0x5e9`). Ours keeps the second site and the
  first jumps forward. Probes (switch cases in/out of a loop, nested if/else, `break` inside or after the
  if, member calls on a global) always make VC6 keep the later copy; a `goto` to a label in the first block
  makes VC6 float the labelled block to the second site anyway (same code). Negating either `state != 15`
  test, an early-return form, or an inner `if (dist >= half) ... else if ...` chain change far more (66-376).
  In 9.6f (VC7, 0x4ac580) objective_52's else-pair jumps *forward* into the kill_char pair, so the kill_char
  block is a merge target there too. Unexplained; the only probe where the earlier block survived was a later
  duplicate that is a whole `case` body entered straight from the jump table (p2 in the scratch probes).
- `State5_PursueOrChase_572920`: besides the pair placement, the original inlines the first nested
  `Negate` of `MaxAbsDistance` (`neg %eax`) and calls the second and `Max_44E540`, frame 0x20 instead of our
  0x24. `inlsim.py --scan`: caller size +144..+300 gives exactly that; padding the function with 80 `(void)0;`
  confirms it (everything but the pair block then matches). So the original source has about 150-300 more
  front-end size units than ours while compiling to the same VC7 code (9.6f score 73, only the pair block and
  a 1-byte shift differ). Not found: a `switch` on `GetObjectiveStatus()` for the 1/2 tests swaps ebx/ebp.
- `sound_obj::HandleCarTireScrubSound_418720` (4): after `GetCarLinearSpeed_43A240` the original keeps the
  returned pointer in ecx, loads `gCarInfo_48->field_28_max_speed` (cltd) and only then derefs `(%ecx)`; ours
  derefs first. `speed = call(); speed /= max`, `speed = speed / max`, `(speed / max)` in the Round argument,
  `speed *= ...` as a statement: all 4. `Fix16 speed = call()` (NRVO) reads the stack slot (8); `call() / max`
  in one expression evaluates `&max` before the call and keeps it in a register (114).
- `sound_obj::Type_10_HandleCarSkidSound_418940` (108): whole function identical up to register roles: the
  original has a2 in esi, pPhysics in ebx, the first-division temp in edi and `push edi` after the switch;
  ours a2 in ebx, pPhysics in edi and `lea esi,[ecx+0x28]` (the address of `max_speed` CSE'd across the
  GetCarLinearSpeed call; the original reloads `gCarInfo_48`). 9.6f (0x414bc0, paired with
  `add_96f_target.py 418940=414bc0`) shows getters for field_9C/AC/84/88, `Fix16(6000) * (speed / max)` and
  `Fix16(6000) * max(front, rear)` written in each branch with the multiply tail-merged; that shape in VC6
  moves the stack slots (148). `Fix16 speed = call(); speed / max` variants: 122 (reads the local).
- `sound_obj::ProcessOtherObjects_41F520` (4): only case 13/14's `max_distance`/`calc_distance` store order.
  The TODO in the source was wrong: with the original order both full blocks still exist; what changes is
  the cross-jump pairing. The original has fire -> case 4/12 at its `movb 0x32` (7 instructions shared) and
  rocket -> case 13/14 at `movb 1,0x13(%esp)`; ours sends fire to 13/14's `xor bl,bl` (6 shared). Disabling
  rocket's merge (different release_mod) still sends fire to 13/14, so VC6 here picks the *last* identical
  block regardless of match length; swapping the two cases in the source swaps the layout (4 again). Dword
  stores moved after the inner switch (with gotos), `default:` placement, `Fix16(7)`/`.mValue` spellings:
  no effect (28-199). Permuter 500: 4 (the case swap).
- `Ped::GotoAreaByAnyMeans_469060` (30): 9.6f (0x43c480) calls `get_cam_y`/`get_cam_x` on
  `field_14C_internal_target_ped` for the kill_char `MaxAbsDistance`, and with the getters that site matches
  (the original computes dy first). But the two getter temporaries flip the whole register allocation: the
  zero register moves from ebx to ebp, `mov $4,%dl` for the b2 bit test disappears and gDistanceToTarget /
  kFpFour are no longer kept in ebp/edi (348). `Ped* pTarget` local, getters on `this` too, `Fix16(...)`
  copies instead of getters: 348-352; a getter for only x or only y: 836-890. The two remaining
  `cmp %bl,%al` (ours `test %al,%al`) after `FindNearbyTileMatchingSlopeType_5552B0` are not the return
  type (`u8` instead of `bool` changes nothing); 9.6f also uses `test`.
- `Ped::HandlePedHitByObject_45D000` (137): the one real diff is the first `IsRespectNegativeForPlayer`
  site in the inlined `IsPedAThreat_Inline_465D00`: the original spills `field_2E_idx` through
  `[esp+0x20]` (the `flag` slot) at both sites. A `u8 player_idx` local at the first site too gives both
  spills, but then the two sites get each other's register pattern (`edx/al` vs `eax/cl`, gang pointer
  loaded before or after the push) and `sub_4614E0(...) <= kFpOne` puts the result in ecx instead of eax, a
  1-byte encoding difference that shifts every later jump (203). `pMyGang->` at one site (237), `!= 0` / `!`
  spellings (203), a `Fix16 dist` local for the compare reads the slot (139), getters for `this` coords
  (179), permuter 400 (171, an s8 cast and an extern switch: noise).
- `sound_obj::ProcessActiveQueues_41AB80` (131): all register choice around three `gSampManager` calls:
  the original uses eax for the `u8 j` reload (`and eax,0xFF`, 1 byte shorter) and loads `ecx = gSampManager`
  before the `and`; ours uses ecx/edx and loads ecx after the push. The first divergence is
  `mov edx,[esi+0x20]` (ours eax) right after the two argument `Fix16(s32)` constructor calls of
  `AdjustPlaybackRate_41A580`. Permuter 400: 118 (noise).

## Car_BC / CarAI_78 big-WIP pass (Oct 6, agent/big2)

Scores are `quick_score.sh` lines (WIP_IMPLEMENTED emptied).
- `CarAI_78::DetectCarAhead_44D1D0` 150 -> 16 (not committed: goto variant kept out of the branch, see agent/big2 a459f4bf): both probe switches leave through `goto tail_N;` instead of `break`,
  which makes VC6 keep the north copy of the merged second-probe tail like the original (see matching_quirks,
  "Which copy survives can also depend on how the cases leave the switch"). Left: the final block has
  `arrow_idx + 1` in `al` and `arrow_count - 1` in `cl`, ours the reverse (same instructions, `jbe`/`jae`
  flipped). Tried: both declaration orders, `next > last` / `last < next`, the compare on `field_2F` itself,
  ternaries, `arrow_idx++; arrow_count--` in place (with and without `u8` casts), expressions instead of
  locals: all 16-96, `al` always goes to `arrow_count - 1`. `default:` first in the switch (150), a range check
  instead of `default: return;` (284). Permuter 1000: 12 only by moving the `field_2F = next_idx` store after the
  if (behaviour change).
- `Car_214::sub_5C8780` (84): 9.6f 0x4C4FE0 calls `AsCar_40FEB0` in every car case (10.5 folds the type check
  when no store sits between the compare and the inline, as in case 2), so `pSprite->AsCar_40FEB0()` replaces
  `field_8_car_bc_ptr` without changing the code. Case 3's car path as if/else with two `GetBasePointer` calls
  (the original pushes in each arm) plus case 7 written out in full: 96, and with case 5 reading
  `((Car_18_Cmd*)pEntry->field_0_pScriptCmd)->field_8_idx` directly (no `pCmd` local, which changes its registers
  so it no longer merges into case 3's else arm): 104 where everything matches except that case 7's body is not
  merged into case 6's (see matching_quirks). The fallthrough form (current source) hoists the pSprite load above
  the `jmpl` whatever the case 6 test looks like (`!= ped` first 84, a `switch` on the type 78 with a different
  jump table, `goto` into case 6's body 75 but AsCharB4 no longer folds). Not committed (84 vs 104, different
  remaining problems).
- `CarAI_78::ReactToNearbyCar_451980` (96): the whole diff is one spill choice: the original copies the atan2
  result into `bp` (`v21`) and spills the `field_0_car` CSE to 0x14(%esp), ours keeps `field_0_car` in `ebp` and
  `v21` in memory. `Ang16 v21; v21 = atan2(...)` copies the result (`mov (%eax),%ax; mov %ax,slot`) but still to
  a slot, `v27 = v21 + kAng180` gives the original's `add %bp,%di` form (106 with the spill still wrong). No
  effect: `v21`/`v26`/`v27` declared at function scope, a `Car_BC* pCar = field_0_car` local after `v21` used
  for the switch, the turn calls or both (96-106), before the atan2 (180), `v27.rValue = v21.rValue`, an `s16`
  `v21` (178), `v26`/`v27` initialised then overwritten (216). Permuter 1200: 82 only by inverting the final
  `v26 < v27` (equivalent) plus a cached switch index; the spill is the same.
- `CarAI_78::FollowRoadDirection_44A1F0` (122): the two `SetGoStraight(); return;` tails. `return;` inside either or both arms
  (122), SetGoStraight as the fall-through of an inverted test (180), `goto ret_n1;` to a label on the case's
  `return` from one or both sites (122), the outer `v12 <= A` branch pair swapped (210, changes the layout).
- `Car_14::SpawnTrafficCar_582480` (8): identical case 1/2 bodies (notes above) keep case 2's -1 block; `goto`
  instead of `break` in the a2 switch: still 8.
- `Car_6C::SpawnCarOnRoadNetwork_4458B0` (469, skeleton 0): the corner switch's down_2 -> up_1 merge is already
  the original's; what is left is register permutation from the start (`dir` in edi vs esi, junction index in
  bx vs di, FindArrowBlockInJunction's pushes) and the dead-parameter-slot frame. A `goto` out of the corner
  switch: 482.
- Not attempted this pass: `HandleCarHitByObject_43F130` (882), `ManageTrafficCarDirection_448CE0` (1897),
  `UpdateCollisionBurst_state_31_34_53BAC0` (463), `EmitBloodBurst_53E450` (325), `Object_2C::
  IntegrateHorizontalMovementAndCollisions_524630` (584), `DMA_Video_LoadDll_5EB970` (1034).
### Far-list pass (Oct 6, Fable worker: map_0x370 / Frontend / TrafficLights / PublicTransport / Ped WIPs)
Scores are `quick_score.sh` lines (10.5) and `--96f` lines (9.6f, VC7). Baseline 3294, one improvement, no new match.
- `Map_0x370::sub_4E8370` 104 -> 8 (committed). Two source changes, both confirmed with VC7 against 9.6f
  0x463C30 (108 -> 12), so they are shape, not a VC6 quirk:
  - `new_idx` assigned in both `do_drop` branches (VC6 hoists the load/store/`lea` above the `je`). Assigned
    once before the `if`, the `do_drop` test is a `cmpb $0,0x20(%esp)` scheduled before the `new_idx` store
    (eax is live, so no `mov al; test al`).
  - The copied height read through `const u8& new_height = pColumn->field_0_height` at the four
    `pNew->field_0_height = ... - 1` sites and the plain copy. With `pColumn->field_0_height` directly, every
    `pNew->field_0_height - pNew->field_1_offset` loop bound in that branch loads height first (`mov (%edx),%bl;
    mov 1(%edx),%al; sub %eax,%ebx`), the original offset first with the result in the other register. Any
    `const T&` local in scope toggles it (`const s32& new_idx = field_360_column_words` (a conversion
    temporary), `const s32& height`, `const u8& offset`, `const gmp_col_info& col = *pColumn`), but the
    toggle is scoped: a function-scope one also flips the in-place loop at the end, which must stay. Two of
    them cancel. Casts, `i + offset < height`, `bound > i`, inline getters, a `Len()` inline: no effect.
  - Left: the original has the hoisted `mov 0x360(%ecx),%eax; mov %eax,0x1C(%esp); lea` *before*
    `mov 0x20(%esp),%al; test %al,%al; je`, ours after. `pNew` computed before the `if (do_drop)` gives that
    order but swaps eax/ecx for `column_idx`/`this` at the top (the `this` spill moves to the first
    instruction) and `offset`/`new_idx` swap their dead-parameter slots, 320-330, in VC6 and VC7 alike; no
    toggle, `else`-wrapping of the in-place branch, `u16** pColumns` local, `s32`/`bool` `do_drop`, `!= 0`,
    swapped branches or a `bDrop` copy changes that. The permuter (3 runs, 4000 compiles) only "fixed" the top
    by moving E's zero store or the `field_1_offset` copies before the loops (wrong), and its 44 was the
    `const s32&` conversion temporary.
- `Map_0x370::sub_4E6660` (4): `pPrev = pBlock` after `sub_4E65A0(x, y, &z, 1, 1)` is 48 with every
  `(s32|bool|u8|char_type)` pair for the two parameters; VC6 keeps the 1 in ebx for both pushes whenever ebx
  is free across the call. Assigned at the top of the else block (64) or after `dist +=` (82).
- `Map_0x370::sub_4E6190` (60): cases 3/4 as `switch (a5 - 2)` with cases 1/2, `rel = a5 - 2` or `a5 -= 2`
  before the inner switch: 116 (the cross-jump into case 1/2 still goes the other way).
- `Map_0x370::sub_4E5640` (233): all 13 locals declared at the top (233 unchanged), then 400 random
  declaration orders (own shuffler, `decl_shuffle.py` stops at the `Fix16_Point pos_diff(...)` line): not one
  changes the score. `Fix16_Point pos_diff; pos_diff.x = ...` 329. The slot layout (pos_diff 0x10/0x14 in the
  original, 0x18/0x1C ours; `i`, `value_1.ToInt()` and the z temporaries in other dead parameter slots) does
  not come from declaration order.
- `Frontend::ChangeMenuPage_4B3170` (353, structure 46): same experiment, 20 declarations at the top
  (`saved_main`, `pPage`, `playerSlotSetting`, `best_opponent`, `user_value`, `time` moved up), 400 orders: 353
  throughout. `u16`/`s32 playerSlotSetting`, `(u8)`/`(char_type)` casts on the `SetPlySlotIdx` argument: 353
  (the original's `mov %al,%cl; push %ecx` vs our `push %eax` is not the type). Left besides slots: the
  frags/points/times loops rotate ebx/ebp/edi (`best_opponent` in edi in the original, ebp + a spill ours),
  frame 0x108 vs 0x100, `setne %dl` vs `%cl`.
- `Ped::IsThreatToSearchingPed_4661F0` (418, skeleton 4): 9.6f 0x437670 default-constructs three `Fix16`
  locals at the top (frame 0x50 vs our 0x40 fits them plus a `player_idx` byte at 0x2C, which the original
  stores and reloads at the *first* `IsRespectNegativeForPlayer` site). Separately: `u8 mode` switch copy 418,
  `Fix16 candX/candY/dist` at function scope 438, `u8 player_idx` at the first site 446. Prologue: original
  `xor eax,eax; mov al,gTargetSearchMode` before `push ebx`, ours `mov eax,...; and $0xFF` before `sub esp`.
- `TrafficLight_20::Init_5C1D00` (738): the original keeps `gMap_0x370_6F6268` in esi, x in ebx and `y`
  (`and $0xFF`) in edi from the prologue and spills `this` to 0x14. A `Map_0x370* pMap` local: 800,
  `+ Object_5C* pObj`: 1052, `s32` copies of x/y: 800.
- `Frontend::SetupMenuStringsOptionsElements_4B0220` (254), `PublicTransport_181C::SpawnTrainsFromStations_578860`
  (601, frame 0x68 vs 0x58), `Ped::UpdateMovementTowardsTarget_4672E0` (560), `CarDoorAlignmentSolver_545AF0`
  (875), `Map_0x370::CanMoveOntoSlopeTile_4E0130` (548), `Map_0x370::sub_4E7190` (218, structure 6; `tmp`
  declared first/before `last_x`, `last_*` declared then assigned: 218): scored only.

## MapRenderer / rest pass (Oct 6, worktree agent/rest)
Scores are `quick_score.sh` lines.
- `MapRenderer::ProjectVert_4EB940` 4 -> 0 (**MATCH**). The last diff was `pop %ebx` before `fmuls 8(%ecx)` in the
  epilogue. `sched.sh` showed the y line's x87 chain in the final window: the two paren no-ops *before* `fmulp`
  gave the pops free cycles ahead of `fmul [ecx+8]`. One no-op *after* `fmulp` instead (parenthesised product
  `(A * B) * (pVert->z)`) blocks the last `pop` for one cycle, so `fmuls` is issued first. 256 forms of the two
  conversions (0-4 parens, f32 local or not) alone never moved it: the position of the no-op matters, not the count.
- `DrawGradientSlopeSouthwards_4F1660` 8 -> 4 and `Eastwards_4F33B0` 16 -> 8 with `VertProjector3`'s y line as
  `Rounded2(ypos) * P3(field_60) * (pVert->z)` (`P3` = `(((v.mValue / 16384.0f)))`, no f32 local). Left in both (one
  site in S, two in E): `fmuls k` of `field_60` is issued *before* `idiv` (cycle 153 vs 154); the original has it
  after. With an f32-local store no-op before that `fmul` (the `Rounded*` forms) the no-op waits until the idiv
  completes (cycle 176) and the `fmul` follows at 177, but then the `lea` of the `Add_408660` return slot (ready
  since the idiv) takes the two-cycle gap before `fildl gYCoord` (the original has it after). Both `fmul -> nop`
  edges carry the fmul's latency 3 and `fmul -> fild` is 0, so the original must have no node between `fmuls k`
  and `fildl gYCoord`; any unparenthesised `f / 16384.0f` is then reassociated (constant hoisted past `fmulp`:
  `fmulp; fmuls z; fmuls k`), so the shape is not found. Swept ~1500 forms: x and y lines separately over
  {`Rounded`(no paren), `Rounded2/3/4`, `ToFloat()`, `P0..P3` (no local), `(f32)` casts `C1..C5`, f32 store after
  the division} x {`pVert->z`, `(pVert->z)`} x parens around the product / the whole line. `Rounded`/`P0`/`C4`
  forms (no paren) reassociate; `ToFloat()` for ypos makes ypos heavier so it is loaded first. Nothing below 4/8.
- `TagGameHudUpdate_4DADA0` (54): five more structures (bShow at outer scope with if/else; the clear block as the
  else arm with its own ClearPager copy + return (211, no tail merge); `if (!bShow) {clear; return}`; show+return
  inside the then arm): 54-211. The 9.6f copy (0x462530) has the same layout as 10.5 (first-flash block, then
  clear + ClearPager, then the `> 0` block, then show), so the shape is in the source, not in VC6.
- `SetWindowedMode_5D9510` (14): `sched.sh` shows `push $0x316` (p40) losing the two-per-cycle slots at cycles 23-27
  to the eight RECT/global loads (p56-74) that are ready at the GetClientRect call (their call->load edge is 0
  latency); the original issues the push with the `window_height` load, so there its RECT loads are not ready at
  that cycle. `volatile` RECTs (62), the sizes as `s32 w, h` locals (85), both RECTs through `RECT*` locals (16),
  a `UINT flags` local (14): no.
- `Hud_Arrow_7C::UpdateScreenPos_5D0850` (86): the diff is one block: ours reads `field_10_radius_pos` once
  before the first `__alldiv` and keeps it in a stack slot for both the `* 64` and the `/ distance` use (the
  frame is the same size, `distance` moves to another slot); the original reloads it twice (into `ebx` after the
  `imul`, then from the field), and loads `field_60.x` into `eax` for `imul %ecx` where ours does `imull 0x60(%edi)`
  with factor in eax. Removing the second use makes ours load late too, so it is the cross-block CSE of the two
  field reads. `factor * field_60.x`, `Fix16(mValue * 64)`, `mValue << 6`, a `radius64` local before/after
  `factor`, `prod` local, `*=` for the ui scale, `const Fix16&`/`this` pointer locals: 86 or worse (134-146).
- `Char_B4::sub_54C3E0` (58): the paired 9.6f 0x4994D0 is an older version of the logic (face switch over four
  angle globals + 0x491F10), confirmed; `find_96f_counterparts.py` offers nothing else. The original's four
  `lea`/`push` tails rotate registers continuously (edx/eax, ecx/edx, eax/ecx, edx/eax) and only the first and
  last merge; ours allocates all four alike. Not retried.
- `Char_B4::ContinueMovementAfterCollision_54B8F0` 392 -> 220: `volatile char_type bMoved = false` declared after
  the `Jumping_15` early return (its store then follows the compare, the original has it before, but the tail
  layout and a `push edi` move closer). ebx/ebp/edi are still pushed at the top (the original pushes them after
  `mov 0x18(%esi),%eax` in the next block, the late-push quirk); a non-volatile `bMoved` is far worse (524),
  `x_vec`/`y_vec` declaration order and `bMoved` assigned after the return: 220.
- `BurgerKing_1::read_input_device_498DA0` (104): the original's entry stores are `0 -> 0x14` (the slot
  `padItems` later uses) and `1 -> 0x18` (the slot `kbResult` later uses), ours `bReleased = 0` and `padItems = 1`
  in other slots. `DWORD padItems = 0; HRESULT kbResult = 1;` at the top, with `bReleased` uninitialised or
  declared in the loop, and `padResult` hoisted: 134-148, so the slot sharing is VC6's, not the source's.
- `eager_benz::OnPedKilled_592660` (77), `GetNextRotationToward_550F60` (164, per-case ax/cx/dx rotation),
  `Draw_4F6A20`, `DrawRightSide_4EAF40` (125), `draw_bottom_4ED290` (123): looked at only.

## Fresh pass (Oct 6): DrawBackground, LoadStringTbl, STORE_CAR_INFO, the Particle/Object/Car big three

Scores are `quick_score.sh` lines. Matched: `Frontend::DrawBackground_4B6E10` (72 -> 0, see the new tail-merge
entry in matching_quirks.md). The 9.6f copy (0x453020) showed the way: it ends with `mov %eax,8(%esp)`, the last
right-hand blit's result stored into the slot that was passed as the second out pointer of
`GetTgaIdxsForMenuScreen_4B6B00`. So the second "tga index" is a dword local (`(BYTE*)&ret`, read as `(u8)ret`)
that also takes that blit result, and a read of it (`if (ret == -10) { }`) follows. VC6 drops the store after
the tail merge, so the two retry blits stay apart; without the read the store is dead early and the tails merge.
Tried before finding it: an `s32` return type with `return blit()` on one side and a discarded call on the other
(24: breaks the merge, but then `blitRet` needs a register), assign-then-return on either side (72), a volatile
store (20), the dword local without the read (72).

- `frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0` (52): still the dead `(len + 9) & ~1` in `edi` in the first
  loop and the register/slot swap that goes with it. 9.6f (0x475D30, VC7 /Ob0) has the very same dead `and`, so
  it is in the source and both compilers fail to remove it. VC7 does not inline `__forceinline` under /Ob0
  (checked), so it is not a helper. Spellings that VC6 removes completely: `aligned = len & ~1` with the local in
  or out of the loop, `len &= ~1` after the uses, `(void)`, comma, `switch (x) { default: }`, folded reads after
  the loop (`if (a == 0) {}`, `a != a`, `a - a`, `a == 0 && a != 0`, `+= 0 * a`), a `str_count` reused as scratch,
  `volatile` (50: a store). What VC6 keeps: a dead loop-carried accumulator `x += len & ~1` (56, `and $0xFE,%al;
  add %eax,%edi`) or `p += len & ~1` on a pointer (46): the `and` stays but so does the `add`, and with that
  third loop variable the pointer and total take the original's ecx/edx. The second loop as a rotated `while`
  (the original's `test %ecx,%ecx; jbe` on the zero-extended size) still shifts the registers (94).
- `miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180` 121 -> 102 (committed). The original's `this` for both
  `ReassignAllocatedCarType_443EE0(8)` calls is the value just stored to `gStoredCar` (`mov 8(%ebx),%ecx; ... mov
  %ecx,gStoredCar; ... call` in the inner block, `mov %eax,%ecx` from `pCar` in the else), so both are written
  `gStoredCar_6F7560->Reassign(8)` and VC6 forwards the store; `pParam2->field_8_car->Reassign(8)` reloads. The
  operand order `pCar != gStoredCar` (not `gStoredCar != pCar`) is worth 23 lines on its own. Left, all register
  allocation: `pCar` is loaded into `ecx` (original: `mov 0x16C(%eax),%eax`, reusing `pChar`'s register) and
  `four` is folded into immediates (original `mov $4,%edi`). The permuter's "improvements" (76, 52, 50) are a
  wrong store order in the else branch (it loses the original's reload of `pParam2->field_8_car` after the
  `gStoredCar` store) or deleted statements; they do show that the `pParam2->field_8_car = pCar` store is what
  makes `pCar` outrank `gStoredCar` for `eax`/`ecx`, and that the else-branch reload is what makes VC6 fold
  `four` (with `gStoredCarId = pCar->field_6C` instead, `four` is in `edi` and ebx/esi/edi are the original's).
  No `pChar` local, nested `if`s, hoisted declarations, `four` declared one or two scopes out, `pParam2->field_8_car`
  instead of `pCar` in the compare (forwarded, identical asm): no change. No 9.6f pair.
- `Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0` (463): the diff is stack slots and one allocation
  choice. The original keeps `max_sub_state` in `bl` (`mov $0xC,%bl` at its first use, `cmp %bl,%al`, `mov
  %bl,0x46(%esi)`) and spills `pCar` (stored at entry and at the switch, reloaded from 0x20 for the late
  `else if (pCar)` reads); ours keeps `pCar` in `ebx` throughout and puts `max_sub_state` in a byte slot.
  Removing the late `pCar` reads (diagnostic only) gives `bl` to `max_sub_state`, so it is a weight decision.
  `max_sub_state` declared at the top: 453, but the 12 is then stored to memory at entry. 9.6f copy registered:
  0x490130 (`add_96f_target.py 53BAC0=490130`); it also stores both `pB4` and `pCar` at entry (VC7), and has two
  `Ang16` default ctors at the top (`angle` and one more). Not finished.
- `Object_2C::IntegrateHorizontalMovementAndCollisions_524630` (584) and `Car_BC::HandleCarHitByObject_43F130`
  (882): scored and looked at only; both differ in register assignment from the first block (`this` in edi vs
  ebx, the zero register) and need a rewrite rather than tweaks.
## Near-miss pass (Oct 6, worktree agent/near4)
Scores are `quick_score.sh` lines. One new match.
- `jolly_poitras_0x2BC0::SavePlySlotDat_56BA60` 4 -> 0 (**MATCH**). `sched.sh -r` showed the `len = 126` store and
  the outer counter init `mov $3,%ebp` both at priority 4.0, the store first by IL order because the loop optimiser
  appends the counter init to the preheader. Accumulating `len` in the inner body (`len++; len += 4; len += 4;`,
  the style of the matched `SaveHiScores_56BF20`) makes VC6 fold the induction variable's final value into a store
  emitted after the counter init. `len = 126` after the memcpy / in the loop body (hoisted) / in the for-init /
  before the call / as the declaration initialiser: 4; after the loop or `pDst - this`: 12-18; count-down loops
  and a running `stage_stats*` pointer break the pointer strength reduction (12-46).
- `Ambulance_20::UpdateState_4FB330` (2): scratch-TU experiments show chain-lowered switch bodies are laid out and
  ordered default, 6, 5, 3 whatever the source order, and the exit block always follows the lowest case; tried
  per-case HOS and HOS-after-switch forms with `return`/`break` mixes in the inc arm (`return` inside the `> 500`
  if, both, `<= 500 return`), `case 0/2/4: default:`, no default + tail, dead code after the switch, `for (;;)` and
  `do {} while (0)` wrappers, if/else followed by `break`, then-arm `break` + inc arm after the if: all keep the `jle`
  on the adjacent exit copy. An if/else chain and a jump-table switch give the original's shape (`jle` far, own
  `ret` copy after `movl $5`), but with `cmp`/table dispatch.
- `sound_obj::ProcessOtherObjects_41F520` (4): with the original 13/14 store order the only difference is the fire
  cross-jump target (13/14 at `xor bl,bl`, the original 4/12 at `movb $0x32`). VC6 takes the last identical block:
  making 4/12 identical to fire's whole block (`samp_idx = 189`) still goes to 13/14; making 13/14 differ
  (`release_mod = 16`) goes to 4/12 (2). Fire with volume 85 scores 0 but jumps into the wrong block (verifier blind
  spot, see matching_quirks.md). Label order, inner/outer `default` position and `break` form, fire after the
  explosion case, rocket made different: no change.
- `sound_obj::ProcessPoliceRadioWordsPlayback_427220` (4): the volatile store's barrier edge to the `cmp` is the
  cause (sched log). Non-volatile forms that might survive DSE: `struct {s32 a;}`, `union`, `Fix16.mValue`,
  `memcpy(&old, ...)`, `__asm {}`, `const s32&` to the element, `s32* p = &old; *p = ...`, `volatile s32& r = old`
  (4, same edge), a dead conditional use: all drop the store (40) or keep the edge. A later `*(volatile s32*)&old`
  read keeps a plain store with the compare in the right place but adds the reload (34-36). `u32 cur` local (56),
  compare precomputed in a `u8`/`bool`, comma, `| (old & 0)`: 4.
- `sound_obj::HandleCarTireScrubSound_418720` (4): the `[eax]` load's WAR edges (sched log) force it before the
  divisor's `eax` use. A `Fix16&` inline parameter bound to the call with `/=` inside copies the pointer and loads
  late (right order) but stores back (`mov %eax,(%esi)`, 48); with `/` inside (needs budget padding to stay inline,
  `inl.sh`: nested budget 56 < 57) the dividend is evaluated first (54); `Fix16&`/copy-ctor/NRVO locals read the
  slot (8); `/=` on the temporary itself evaluates `&max` before the call (120).
- `keybrd_0x204::GetLayout_4D6000` (4): sched log: the two KLID byte loads (p46/p38) beat `lea &v2` (p28) at the
  first cycle after the call; `char*` to Buffer, `const char*` to KLID+6, `v2` declared first, `s32*`/`s32&` to v2:
  4; a 4th sscanf argument: 34.
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (4): `BYTE tmp` flag alone or with the sibling's `u16&` (28:
  copies si to di), raw `*(u16*)((u8*)this + 0x6E)` at the top and/or in the condition, `(s16)` compare, `pThis`
  local, reversed `&&`, `volatile bFound` (30): the loop compare still CSEs to `si`.
- `sound_obj::TrainCab_414710` (6): `return` in both arms, early `!pDriver` return + then-arm `return` + store after,
  `return` after the if/else, else `return` + outer `return`: 6.
- `DrawGradientSlopeSouthwards_4F1660` (4): `regsearch.py` finds no window limit below the stock score, so it is
  not a window break; not retried after the ~1500-form sweep above. `Car_14::SpawnTrafficCar_582480` (8): not retried.

### Fresh pass over the near misses (Oct 6, agent/fresh2)
Scores are `quick_score.sh` lines. Variants were compiled in a mirror of `Source/` (symlinks plus the edited
file, so a header could be swapped per variant) without touching `build_vc6/`.
- `ErrorLog::ErrorLog_4D94E0` 32 -> 0 (**MATCH**, 3296 -> 3297). The original constructs the member
  `ofstream` (`??0ofstream@@QAE@XZ`, `push $1` is the virtual-base flag) and registers EH state 0 before
  `Open_4D9470`. The `fake_ofstream` buffer hack hid both; `sizeof(ofstream)` is 0x3C under VC6 (probe:
  `char c[sizeof(ofstream) == 0x3C ? 1 : -1]` compiled with `compile.sh`), so the real member replaces the hack
  for `!defined(__clang__) && _MSC_VER <= 1200` and `field_3C_pLen` stays put.
- `Garox_12E4_sub::DrawPlayerStatsHelper_5D61A0` (10): the role swap of `ebx`/`ebp` (width vs base_xpos) with
  `s32 width` is insensitive to `u8` (58, slot + `edi`), `s32 width;` then assign, the combined
  `get_sprite_width_4C7220` inline, `(s16)` on the getter, `s16 sprite_idx`, a `half` local (76), an explicit
  `Fix16(base_xpos - width / 2)` (62): all 32 except where noted.
- `Weapon_30::fire_truck_gun_5E0E70` (10): the `pTurret->field_0->field_0` chain through `eax` with
  `lea 8(%esp),%ecx` before the load. `rValue +=` plus `Normalize()` (300, the loop inlines), `Ang16(a + b)`
  ctor (74), no `pTurret` local (20), a `Sprite*` local (26), `gun_ang = word; gun_ang += sprite angle` (228).
  `find_96f_counterparts.py` gives no 9.6f partner (best share 0.38).
- `Particle_4C::UpdateAttachedEmitter_state_9_10_53B670` (12): the Ang16 slot 0x10 vs 0x12. An `s16 rnd`
  local for `get_int(8)` (12; `rnd - 4` folded into it: 16, the `sub` moves above the `movswl`), `angle`
  declared before/after `jitter` at function scope (52), `jitter +=` (54), a `Fix16 half` local (12),
  `operator+=` returning void (12), `Fix16_To_Ang16` through the `(s16, u8)` ctor (12), `angle` built with the
  `const s16&` ctor (12) or the `(s16, u8)` ctor (12). Whatever holds 0x10 in the original is not a visible
  temp of these expressions.
- `Wolfy_7A8::sub_543690` (12): the two `lea` temps swapped between the in-loop and final tails.
  `smallestVal_idx = last_idx` before the in-loop store, a `u8 idx` for the final tail, a `Wolfy_30*` local
  in the in-loop tail: all 12.
- `Particle_8::EmitElectricArcParticle_540320` (20): the rng/word `imul` operand registers. A left-wide
  multiply (`(__int64)a.mValue * b.mValue`) in a static inline, with the `Fix16*` ctor via a `tmp` (20) or via
  `MultiplyLeftWide` (20); the same inside `Ang16(Fix16(...), 0)` directly: 168.
- `struct_4::CleanupSpriteList_5A7080` (20): VC6 lays the keep block out as the fall-through into the loop
  test whatever the source order: `if (...) { A; continue; } else { K; continue; } B` (20), `goto head_unlink`
  with the label after `continue` (20), `if (!pIter) return` after the unlink branches (28). The matched
  `PruneNonCollidingSprites_5A7240` (`if (keep) K else if (pLast) A else B`) is laid out in source order K, A, B,
  so the original's A, K, B order is still unexplained.
- `Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0` (54): the `return 0` epilogue placed right after the
  loop's bottom `jne`. Flat `if (Process()) return 0;` chains (54), `if (count) continue; return 0;` (54),
  `for (;;)` with an explicit top `return 1` (54), `if (!count) return 1; do {} while (count); return 0;` (82),
  `goto fail` for the end-of-body `return 0` only or for every in-loop `return 0` with `fail: return 0;` after
  the final `return 1` (54): VC6 ignores the textual position of the shared epilogue here.
- `PoliceCrew_38::State6_ShutDown_574720` (46): with `u8 i = 0` at the top, writing the three literal-0
  `SetObjective2_463830(0, 9999)` calls as `objectives_enum::no_obj_0` does not remove the `ebp` zero register
  (128), so enum vs literal zeros in pushes is not the missing count.
- `Fix16_Point::NormalizeSafe_442AD0` (52): `Fix16 length; length = GetLength...()` (209) and additionally
  `Fix16_Point scaled;` hoisted (204) are far worse; the slot reuse of the second GetLength result with the
  first expansion's `y*y` (0xC) stays unexplained.
- `Camera_0xBC::ApplyCarVelocityCameraOffset_436200` (58): `compare_callees_multiset.py` reports a missing
  `ErrorLog::Write_4D9620` call, but mapping every original `call` through the csv shows none (the targets are
  `Multiply/Add_40866x`, `atan2_fixed_405320`, `Abs_436A50`, `SquareRoot_436A70`, `get_linvel_43A450`,
  `Multiply_438FE0` and the `__all*` helpers), so that report is a tool artefact. 9.6f 0x41EBF0 calls
  `SetFromPolar_41E210` on `offset` by reference and then two `add_40E530`, i.e. our shape. The one dead
  `offset.x` store: `SetFromPolar_41E210` overload (62), the two assignments written out (162), by-value
  `Fix16_Point offset(sin * r, cos * r)` (174).
- `Garage_48::ParkCarAtDoor_534700` (116) and `PedGroup::MergeWithOtherGroup_4C9B60` (104): re-scored and read;
  nothing new beyond the existing notes (register priority between `pPed` and the list cursor; `w2`/`&field_38`
  swap). `compare_globals.py` flags nothing on any of the targets above.
## sound_obj / winmain / Hud pass (Oct 6, worktree agent/fresh3)
Scores are `quick_score.sh` lines. One new match.
- `sound_obj::Type_10_HandleCarSkidSound_418940` 108 -> 0 (**MATCH**). 9.6f 0x414BC0 paired by hand
  (`add_96f_target.py 418940=414bc0`): getters for field_9C/AC/84/88 (no effect in 10.5), `(speed / max) *
  Fix16(6000)` with the 6000 built first. Steps: the raw 64-bit division expression (108 -> 82: the
  `const Fix16&` divisor is otherwise bound before the call, see matching_quirks), then the permuter's `>= 1`
  showed the register permutation (a2/pPhysics/late temp) could flip (30, but `cmpb $1`), and storing
  `field_20_rate` per branch instead of through `new_rate` gave 0 with `> 0`. No effect: `v4 = call(); v4 /=
  max` / `v4 = v4 / max` (122, divisor evaluated first), `Fix16 rear` locals, cached thresholds alone,
  by-value skid getters, `rate` or the skid locals declared elsewhere, `s32 rate = 16000` without default,
  `Fix16 v4` per branch, no pPhysics local (142).
- `sound_obj::HandleCarDoorSounds_4182E0` 286 -> 138 (not committed: the shape that gets there is wrong). The
  original holds 0 in ebx (flags, `field_3C`/`field_34` stores, `push`, byte compares), pCar in edi,
  `(u32)i` in ebp across the open block (so `displacement` spills to 0x20) and a2 on the stack. Ours has no zero
  register until the close block's release-mod if/else is written through a local or a ternary (138/154); then
  pCar/`(u32)i` have ebp/edi swapped and `displacement` stays in a register. Tried on top: flags before pCar,
  `char_type` flags, `== 0` tests, written-out `IsStartingToOpen/Close`, a shared inline for the 3/5 store
  (286), `mod` at function scope (138), `5` then override (296), `model` read from pCar (198), a `pDoor` local.
  Permuter 1000 + 1000: 118 only with the ternary, `&&`-merged distance/volume tests and a bogus `u8 rate`.
  No 9.6f pair (`show_96f.py`: unpaired).
- `TagGameHudUpdate_4DADA0` (54): `if (cond) { if (!byte) {...} } else { byte = 0; dword = 0; bShow = false; }`
  followed by the `dword > 0` block and `if (bShow)` (the structure the original's layout suggests: the
  not-flashing stores sit between the first-flash block and the `dword` load, with the clear block moved into
  the else arm by dupB) scores 93: VC6 lays the else arm first and hoists `mov $1,%bl` above the tests.
- `Hud_Arrow_7C::UpdateScreenPos_5D0850` (86): the two `field_10_radius_pos` reads are CSE'd into a slot in
  ours, reloaded in the original. A `Fix16 r` local for the second read, the raw `<< 14` division, `Fix16(mValue
  << 6, 0)` for the first: 86; a `radius` local for the first: 146. `frame_slots.py`: the original's `distance`
  is the hidden-return slot of all three GetLength arms (0x10); ours gives each ternary arm its own temp and
  copies into 0x20, which is the `Fix16 d = inline_with_returns()` shape whose budget breaks the projection.
- `Map_0x370::sub_4E6660` (4), `SetWindowedMode_5D9510` (14), `sub_4E6190` (60), `eager_benz::OnPedKilled_592660`
  (77, the occupation/kill-type switch interleaving): reviewed against the notes only. `ErrorLog::ErrorLog` was
  matched by another worker meanwhile.

## Register-only WIPs (regalloc pass, Oct 7)

`Scripts/regalloc/regonly.py` finds the WIPs that match except for registers: only
`Wolfy_7A8::sub_543690` and `Char_B4::state_8_5520A0` (6 lines each). Both are local-temp
round-robin differences (`Scripts/regalloc/README.md`), not colour-pass ones.

- `sub_543690`: our in-loop tail's `lea` is the first round-robin pick (eax), the final tail's the
  second (cursor at ecx, which holds `this`, so edx). The original needs the final tail generated
  first, or one more round-robin temp in the in-loop tail (the `mov %edi,%eax` copy is a colour-pass
  live range in ours). No change to that order: in-loop `goto` to a block after the final tail,
  the final tail written before the loop with gotos, `if (cv != 1) ... else`, and index spellings
  `(s32)`, `(u32)`, `& 0xFF`, `u8`/`s32` idx locals, `smallestVal_idx = last_idx` inside the index.
  `for (;;)` with the exit test (and final tail) at the top does generate it first and gives the
  right registers, but lays the final tail out first (30 lines).
  Live range kinds (`@R k=`): the two tail copies are optimizer temps (kind 3, ids 0x144 final and
  0x14a in-loop), so (b) would need the optimizer not to create the in-loop one. The 99 constant at
  entry is a colour-pass constant (kind 0x100d); `smallestVal_idx = smallestVal = 99` and the other
  initialiser chains don't change that. A `default:` case with its own copy of the in-loop tail gives
  the final tail eax but leaves an extra copy (not merged).
  Block order (Oct 7, `Scripts/flowopt/sinklog.py`): the in-loop return gets its place from the loop
  sink pass `0x10740251` (index 44 at numbering, final tail 48, so it goes between the latch and the
  final tail, as in the original layout). For the final tail to be generated first, its index would
  have to be lower (written earlier, as in the `for (;;)` variant), and in the pass dump nothing
  after that pass moves the two blocks again. So the original's layout rules out (a). It made one more round-robin pick before
  the in-loop `lea`. The likely candidate is the `mov %edi,%eax` copy as a local temp rather than optimizer
  temp 0x14a. That copy is already in the IL at code selection (boundary `10765b5c`), so what decides it
  is in the optimizer, not the block order. 9.6f's layout (final tail first) presumably comes from VC7's own rule.
- `state_8_5520A0`: the rotation differs only from asm line 157 to 179 (post processed): source lines
  5797 (`field_184_pObj2C = field_7C_pPed->field_184_pObj2C;` reload, orig `edx`, ours `eax`) to 5807
  (`Ang16 rot = ...`, orig eax/ecx/edx, ours ecx/edx/eax). Moving the reload before the
  `field_6C_animation_state = 12` store, after the `pMySprite` local, or dropping that local all move
  other registers too (listing only, not built).
  Scored with `Scripts/quick_score.sh` (base 12): the round-robin model reproduces ours exactly
  (5800-5807 picks edx, eax, ecx, edx, eax, then ax by copy hint; the `mov 4(%eax),%eax` reuse is not a
  pick). The original follows the same rules only if eax is still busy when the 5797 reload and the
  5799 sprite load are allocated, and its `mov 4(%edx),%eax` doesn't reuse the dying reload register,
  so there the reload behaves like a coloured variable (edx), not a round-robin temp. Tried, all 12 or
  worse: the reload as its own local (function-wide, before or after the other declarations), a copy
  local for the set_xyz arguments, a case-scoped `pObj` for the first load (12), `rot` assigned
  directly (34), no `pMySprite` local (18), the reload dropped and the arguments read through
  `field_7C_pPed` (162), and combinations.
  With the full local rules (README: dying sources freed first, pre-merged destinations keep their
  register): the original fits if the 5797 reload is a coloured variable (edx), so `->field_4` can't be
  merged into it and is a round-robin pick (eax). That needs the reload's uses to reach another block;
  reading it at 5807 or 5810 instead of `field_7C_pPed->field_184_pObj2C` does that but changes the code
  (140, 136). The merged early-return block (`Kill_46F9D0 ... return`, kept copy at 6034) makes its
  picks before the reload in both builds.

## Integer scheduling near misses (scheduler pass, Oct 7)

The priority formula is now exact (`Scripts/x87_sched/README.md`). Checked against it:

- `keybrd_0x204::GetLayout_4D6000` (4): after `GetKeyboardLayoutNameA` the original issues `lea ecx,&v2`
  before the two `pwszKLID` byte loads. With our graph the loads score 46/38 (memory +16, height via the
  `dl`/`edx` write-after-read chain into `lea edx,Buffer`), the `lea` 28, so the original's dependency graph
  differs. Not from: swapping the two byte copies (12), `v2` declared first, a `pv = &v2` pointer (before or
  after the copies), a `Buffer` pointer, `u32 v2` (all 4); the permuter, 1,600 iterations: nothing below 4.
- `sound_obj::ProcessOtherObjects_41F520` (2): not the scheduler. Case 13/14 stores `calc_distance` before
  `max_distance` on purpose; the original's order makes VC6 cross-jump fire's tail into this block (TODO in
  the source).
- `Map_0x370::sub_4E6660` (4): `mov %edi,%ebx` is a copy inserted by the colour pass when it splits a live
  range, placed at the start of the `if (pBlock != pPrev)` block; the original has it after the
  `sub_4E65A0` call. Source `pPrev = pBlock` copies there are deleted as dead (4). Moving the earlier
  `pPrev = pBlock` after `sub_4E5D10` moves the other copy instead (16); `SetRoadBlockAt` after the `if`
  (1668).
- `Orca_2FD4::ComputePath_554AB0` (4): the `[ecx+4]` load is IL-first in our block (pairs at cycle 0); the
  original has it IL-after the `field_1B_direction` store (a may-alias edge holds it). Writing the store
  first makes VC6 hoist the shared `ypos` load above the `jge` (comment in the source), so this is the
  branch-hoisting optimisation, not the scheduler.

## Cross-jump near misses (FlowOpts pass, Oct 7)

The cross-jump rules are in `Scripts/flowopt/README.md`. Both functions below were checked with the patched C2
(`--rev`/`--rank`) and `jumps.py`.

- **`Map_0x370::sub_4E6190` (60 -> 2 with a source change, not applied).**
  - The cause is only the order of the exit's jump list. Reversing it in C2 (`--rev 7:20`) gives an exact match,
    and so does any order with the case 1 returns created last and case 2 after case 3 (outer creation
    orders 3241, 3421, 4321 are all 0).
  - Our source creates them in source order (case 1 first), so case 3/4's copies survive.
  - The 16 inner case orders, `default: return 0`, `return 0` instead of `break`, and outer case order all
    change nothing or also move the layout. VC7 with our source gives the same as VC6, and 9.6f has the
    original's shape, so the difference is in the source.
  - Best so far: `s16 r;`, cases 1 and 2 `r = a6 ? ..;` with `default: r = 0; break;`, cases 3 and 4 `return`
    directly, then `r = 0; break;` after their inner switch, `default: r = 0; break;`, and `return r;`. Every
    jump then matches. Only the shared exit is `xor %eax,%eax` (from `r = 0`) where the original has
    `xor %ax,%ax` (a `return 0`). Any `return 0` in place of an `r = 0` changes the merges (22-68). `r` as
    s32/u16/u32/int and casts change nothing; `s16 r = 0;` hoists the zero (62).
- **`sound_obj::ProcessOtherObjects_41F520` (4).**
  - With the original 13/14 order (`max_distance` first, 28), the join list is
    [default, case 5, 13/14, 4/12, fire, phones, rocket]. Fire is compared with 13/14 first and merges there.
  - Forcing 4/12 ahead of 13/14 (`--rank 215:0,199:1,168:2,188:3,60:4,42:5,24:6`) gives score 0, but `jumps.py`
    shows one `jmp` going to 0x300 instead of 0x32d. So that order is not the whole answer.
  - Joint orders of rocket's and 13/14's last four stores (24): no better than 4. `default` before, between or
    first, and dead statements after the inner switch: 28.
- **`Map_0x370::sub_4E8370` (8).** The original has the hoisted `new_idx = field_360; pNew = ...` before
  `mov 0x20(%esp),%al; test`, ours after.
  - The scheduler (`sched.sh`) shows why: both loads are ready at once, and they are tied by an anti-dependency
    on `eax`. Ours has the `do_drop` load first in IL (the optimizer hoists the code common to both arms in
    after the condition), so it goes first.
  - Writing the hoisted statements before the `if` in source gives the original's order (`mov 0x20(%esp),%cl`
    after the `lea`) but a different global allocation (`column_idx` in `ecx`, `this` at `(%esp)`): 323.
    `do_drop != 0`, `(u8)do_drop` and the combined `[new_idx = ...]` form change nothing.
  - Oct 7, reversed (`Scripts/flowopt/README.md`, "head merging" and "memory compares"): the shared code is hoisted
    by FlowOpts after allocation, always to just before the `je`, after `mov al; test al` (split by `0x10723BFD`
    earlier). The scheduler can't swap them (`eax`). Our colour pass already equals the original's (`ralog.sh`:
    same registers for every live range), and that needs `pNew` computed in both arms.
  - `new_idx = field_360_column_words;` once before the `if`, `pNew` in both arms: 1 instruction off (score 44,
    the shifted jumps): everything matches except `cmpb $0,0x20(%esp)` for `mov 0x20(%esp),%al; test %al,%al`.
    `eax` (new_idx, read by both arms' `lea`) is live at the compare, so `0x10723BFD` finds no free byte register.
  - `pNew` before the `if` in any form (`[field_360]`, `[new_idx = ...]`, both statements, either order): 298-323,
    one `pNew` live range changes the allocation. So no form found gives both; the original's IL is unexplained.
- **`Ambulance_20::UpdateState_4FB330` (2).** Only the `jle` of `field_1C > 500` targets the exit at the end
  instead of the copy after `default`. dupB's first loop didn't move the exit block, because the block before
  it (`state = 5`) falls through. The `<= 500` break/return forms and `break` inside the `if` change nothing (2);
  `return` after the `if` gives 18.

### Exit block placement (dupB, Oct 7)

Rules in `Scripts/flowopt/README.md` ("Duplicating and moving exit blocks").
- **`sound_obj::TrainCab_414710` (6)** and **`Ambulance_20::UpdateState_4FB330` (2)**: in both, **TrainCab matched later (Oct 7, see below).** The original's
  `jcc`s to the exit go to the copy after the success path / `default`, ours to the one at the end. Our last block
  (the `else` store / `state = 5`) falls into the exit, so dupB copies the exit instead of moving it. In the original
  the exit wasn't fallen into when dupB ran. The only matched example found (`Ped::PunchChar_467FD0`, found by
  scanning the build for this exit shape) gets that from an out-of-line `if (A || B) { X; return; }` block
  that FlowOpts later empties by cross-jumping. TrainCab tried: `return;` in the `else` and/or the success path,
  early `if (!pDriver) return;`, `do {} while (0)`, dead statements at the end, `||` and `&&` forms of the distance
  checks, and the store as an `if (!p || <always false>) { store; return; }` block (6); the plain inverted `if`
  (26). The source form that gives a block that is emptied later is not found.

### Permuter sweep and pattern mining (Oct 7)

`permute_sweep.py`, 12 min (about 1,000-1,300 candidates) each on the 16 closest WIPs (4FB330, 4B6390, 4F1660, 4D6000,
4E6660, 418720, 41F520, 427220, 414710, 582480, 5D8470, 4F33B0, 554710, 554AB0, 4E8370, and 4E6190 from its score-2
variant): no improvement on any of them.
- **`SelectPrevHorizontalIdx_4B6390` (4).** The original rereads `field_6E` in the loop condition; ours (and VC7 on our
  source, against 9.6f) reuses `oldCount`'s `si`. The matched `SelectNextHorizontalIdx_4B6330` gets the reload from a
  `u16&` to the field, but there all seven registers are taken (`field_7E` is hoisted into `di`). In Prev the reference
  is hoisted into a spare register instead (28). `s16` old count, `volatile`, a pointer local, a copy from `new_count`,
  the swapped compare, `bool` flag and a `break`: 4-52. A permuter run from the reference form went back to the copy (4).
- **`HandleCarTireScrubSound_418720` (4).** The original applies the inlined `/=` to `GetCarLinearSpeed_43A240()`'s
  returned temporary (the divisor is loaded first, the speed read through the returned pointer).
  `Fix16 speed = pCar->GetCarLinearSpeed_43A240(); speed /= max;` gets the order but reads the stack slot (8);
  `speed = pCar->GetCarLinearSpeed_43A240() /= max;` CSEs `&max` into a register and changes the allocation (120);
  `operator/` forms 114.
- **`sub_4E6190` (2).** Matched `s16` functions get `xor %ax,%ax` from a direct `return 0;`. Every mix of `return 0`
  and `r = 0` on the three zero paths (inner defaults, after the case 3/4 inner switches, outer default): 22-90, the
  cross-jumps change.

### Unpaired 9.6f counterparts (Oct 7)

Several WIPs had no 9.6f partner in `match_96f.json` (or a wrong identity pair), so `docs/inlines_96f.md` never
listed their inlines. Two cheap ways to find the partner: the unpaired 9.6f function between the 9.6f versions of
the 10.5 neighbours, and the 9.6f call at the same position in each paired caller (align the callers' call lists).
Three matches came out of it:
- **`Char_B4::sub_54C3E0`** = 9.6f 0x495470 (between 0x495220 = 54C1A0 and 0x495540 = 54C500). Its two switches
  are an inline `RotateFace_491F10(s32* face, bool* clockwise)` with the cases in 1, 3, 2, 4 order (that order
  fixed the case block layout), and the three stores a setter (0x492400). 59 -> 0.
- **`sound_obj::TrainCab_414710`** = 9.6f 0x412A20 (call position in 0x413BF0). A get-and-clear helper
  (`Ped::PopTrainStation_4117D0`) is tried twice; the second call always returns 0 in 10.5, but its `return`
  path is what gives the exit block placement described above. 6 -> 0.
- **`ConvertColourBanks_5D7CB0`** was a csv row covering two functions: `call; jmp 0x5D7CC0` plus padding, then the
  body. Split into the thunk and `ConvertColourBanks_5D7CC0` (new csv row), as 9.6f has it (0x4CAEB0 -> 0x4CADE0).
  No other WIP target has code after padding.

Partners found but no gain yet: `CarAI_78::DetectCarAhead_44D1D0` = 0x42C8B0, `Fix16_Point::NormalizeSafe_442AD0` = 0x420390
(the plain `GetLength_41E260` for both lengths: 124, the per-site variants stay at 52), `DrawGradientSlope*`
4F1660/4F33B0 = 0x46F370/0x46FC10, `MapRenderer::Draw_4F6A20` = 0x472110, `Particle_4C` 53BAC0 = 0x490130,
`Char_B4::ContinueMovementAfterCollision_54B8F0` = 0x49A080.

`LoadStringTbl_5121E0` (52): the original's first loop keeps a dead `edi = (len + 9) & ~1` (9.6f too) that no
source form tried reproduces; a `u32` copy of the parameter and `while` loops give the param-slot reuse but 94.

Follow-up the same day, no gain:
- `Sprite::Draw_59EFF0`: regsearch still 65 (15 nodes short per window, 38 -> 4); already the documented x87 case.
- `CarAI_78::DetectCarAhead_44D1D0` (134, 16 with the agent/big2 goto variant): 9.6f 0x42C8B0 cross-jumps the per-case probe
  tails too (VC7), and ends with the same `field_2F = idx + 1; if (> count - 1)` clamp, so it adds nothing new.
- `Particle_4C::UpdateAttachedEmitter_state_9_10_53B670` (12): the original's jitter `Ang16` is at 0x12 with 0x10 free;
  a second `Ang16` local (either declaration order), or adding into the jitter itself: 52-100.
- `Ambulance_20::UpdateState_4FB330` (2): 9.6f 0x473CE0 has no extra helper on the case 3 `else` path. `else if
  (++f1C > 500)`, `<= 500` with `return` or `break`, `return` after the store: all 2; the success path with one
  `HandleObjectiveState_4FAAC0()` after the `if (car)`: 6.

### Larger WIPs and the near misses again (Oct 7, later)

All 96 WIPs scored, plain and `--structure`; the listed 9.6f partners of the big ones (DrawGradientSlope*,
`Draw_4F6A20`, 53BAC0, 54B8F0) add no missing inlines: their unpaired 9.6f callees are the 10.5 MapRenderer
helpers under other names, or plain Fix16/Ang16 operators.
- **`Weapon_30::fire_truck_gun_5E0E70` (10 -> 0, MATCH).** The flamethrower's shape with the plain
  `RotateByAngle_40F6B0` and `operator+` (171), then `Get_F4_41CC70()` for `field_4` (30: the second
  rotation's negate goes out of line), a `pTurret` local (16), `get_driver_4118B0()` (0 when placed before
  the flamethrower; the permuter found the getter). In source order the flamethrower's inline `operator+`
  had made the add nothrow, so the flamethrower now calls `Add_40AC50`, with `Get_F4_41CC70()` for its own
  budget split. See matching_quirks.md, "Two fire truck guns, one opaque add".

### Markers, function order and global initialisers (Oct 7)

- Empty markers: no matched function changes, no WIP score changes.
- The 12 WIPs out of address order in their file (582480, 4B6390, 534700, 540320, 53F4C0, 5121E0, 4E5640,
  427220, 41AB80, 4182E0, 418720, 5D9510) moved into address order: every score unchanged.
- `check_global_inits.py`: kAngZero fixed (0x6FE3C0). Left as found (no code reads them differently):
  `k_word_678656` (copied from 0x61A898 at startup), `dword_67BBE0` (dynamic 0, probably `Fix16`),
  `gCharB4_Saved_TileX/Y` (`kFP16Zero.ToInt()`), `dword_705334` (Montana's rdtsc init),
  `gCollisionDamage_6FE33C` (no original initialiser), the debug bools (one 79-store init at 0x4AB950).
- Address order for whole files: 34 files reordered (pure moves), 3304/3304 and every WIP score unchanged.
- PCH: `/Yc`/`/Yu` builds of Weapon_30.cpp change nothing. Defining `Fix16_Point::operator+` at the end of
  the TU does reproduce 10.5's EH stores in all Weapon_30 callers (see matching_quirks.md).


## Oct 8: imul operand order from the argument temporary (EmitElectricArcParticle)

- **`Particle_8::EmitElectricArcParticle_540320` (20 -> 0, MATCH).** The operand of an inlined `imul` that is
  computed first in IL order goes into `eax` (opdump). With `Fix16(s16)` by value, the rng's `movsx`/`shl` stays
  where the argument temporary is defined, before `word_6FD5CC`'s load, so the rng got `eax`. Binding the call
  result as `(const s16&)` makes a 2-byte temporary whose conversion is substituted at its use, after the word:
  the word gets `eax`, and the short temporary gets its own register (`mov %eax,%ecx`, the original's extra copy).
  `Fix16((s32)rng)` gets the order but drops the copy. A `Fix16(const s16&)` constructor also matches here but
  breaks DrawSavedStage_4B5270, EmitWaterSplash_53F060 and Wolfy_30::state_22_23_24_25_542E30.
- `Char_B4::HandlePedCollision_548BD0` (16, unchanged): 9.6f writes every site as `atan2(..).operator+(kAng180)`
  (rhs loaded first), but 10.5 calls `Normalize_406C20` out of line at site 3 and the budget always inlines it
  (122). `(const s16&)` casts, `s16 sum` locals, `Add2` helpers, operator and `+=` forms: 16-178.
- `DrawTexture_5D8470` (8 -> 6 with paren changes on vertex 0/1, not applied): no window limit gives less; the
  rest is priority (vertex 1's `fmul` 184 vs the `y_pos`/`sin` loads 178/160).
- `PoliceCrew_38::State6_ShutDown_574720` (46, unchanged): 9.6f also keeps `i` in memory; the missing `ebp`
  zero register is decided after colouring (constants get `ebp` in both variants in ralog).

## Oct 8: flags folded after layout (UpdateState_4FB330, TagGameHudUpdate_4DADA0)

Jump threading on a constant flag runs after the reverse-postorder layout (`Scripts/flowopt/README.md`), so a
flag test that later disappears still shapes the block order and the exit placement.
- **`Ambulance_20::UpdateState_4FB330` (2 -> 0, MATCH).** `bool bHandle = true;`, cleared on the ReInit and
  inc paths, and one `if (bHandle) HandleObjectiveState_4FAAC0();` after the switch. At layout time the inc arm's
  `jle` targets that test block X; the DFS visits X first, so it sits between the `state = 5` store and the exit
  and the store has to `jmp exit` over it. Threading then empties X, so nothing falls into the exit when dupB runs,
  and dupB moves the exit up after `default`'s call (the original's `jle 0x3F`) and copies Handle+ret into the
  cases. Same mechanism as TrainCab_414710.
- **`TagGameHudUpdate_4DADA0` (54 -> 6).** `bShow = true` and the first-flash block (59 stored before `= 1`) inside
  the condition's `if`, then `if (!bShow) { clear } else { ... }`: the clear block is reached by the DFS from the
  first-flash block, which gives the original's F1 | F2 | clear | pager clear | rest layout; threading removes
  the second test. Left: `test %dl,%bl` for `test $1,%dl`. The constant 1 web (`bShow = true`, `byte = 1`, `& 1`)
  is coalesced with `bShow` in `ebx`; the original keeps the immediate in the test but `bl` in the store. Casts,
  `% 2`, other flag types (98 for 32-bit), separate flags (22-148): no.
- **`Car_214::sub_5C8780` (84 -> 70).** Case 3's car branch as two `GetBasePointer_512770` calls (if/else, each
  pushing its own argument, cross-jumped at the call) instead of a ternary argument; case 5 without the `pCmd`
  local, so the `field_8_idx` load takes a fresh round-robin register (`%cx`) and case 5 keeps its own tail. Left:
  the switch head (`dec %eax`) and case 7, which should cross-jump into case 6's ped check; written as a full copy
  (as 9.6f has it) the head matches but case 7 doesn't merge (104): the cjlog shows case 7 failing the first
  instruction compare against case 6 although opdump shows the instruction identical.
- `CleanupSpriteList_5A7080` (20), `OnPedKilled_592660` (77), `MergeWithOtherGroup_4C9B60` (104),
  `ProcessPoliceRadioWordsPlayback_427220` (4): no gain. For 5A7080 the original's A, K, B order is not a reverse
  postorder of the plain loop CFG (K is a leaf, A and B both successors of the `pLast` test), so something folded
  after layout must be involved.
- **`struct_4::CleanupSpriteList_5A7080` (20 -> 0, MATCH).** The head unlink moved behind a `bUnlinkHead` flag
  tested after the if/else. The DFS reaches the keep block K first (the condition's fail jump); K now leads to the
  flag test and on to the head unlink B, so B finishes before K, and A (unlink after `pLast`) only later from the
  `pLast` test: finish order B, K, A gives the original's A, K, B. Threading then removes the flag test.
