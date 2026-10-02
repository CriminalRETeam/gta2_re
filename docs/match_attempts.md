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

## BurgerKing_1::SetAltKeyState_498CB0 (STUB)

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
  C++ can't name `??_M` or take a destructor's address, so it would need a linker
  `/alternatename` hack. Left as a stub.
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

Also: the original has no code after the `FatalError_4A38C0` call in the default case.
Check whether the original declared it `__declspec(noreturn)` somewhere.

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

## Car_BC::GetDoorWorldPos_43B420 (WIP, was sub_43B420)

World position of door `door_idx`: the door offset from `get_car_remap_5AA3D0` (+1 skips
`num_doors`, then `door_info[]`), turned into `Fix16` with `gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70`,
rotated by the sprite angle and added to the sprite position.

Ratio 0.564. The first rotated coordinate uses the inline `Fix16 operator*` (`__allshr`) and
computes `sin * door_y` first. The second calls the out-of-line `Multiply_408680` twice and
the out-of-line `operator+` (0x408660). Tracing the arguments gives
`(-door_x).Multiply(sin_copy) + door_y.Multiply(cos_copy)`, with the right-hand side done
first and copies of sin/cos made into temporaries (cos into the `door_idx` arg slot).

Tried: `sin`/`cos` locals (0.411/0.464, depending on the operand order of the first sum);
`Ang16::sine_40F500`/`cosine_40F520` at every use (0.564); the traced operand order for the
second sum (0.553).

## Ped::sub_45EA00 (WIP)

Removes a ped that has been off screen for a while (`get_field_20e() > 30`, compared
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

## thirsty_lamarr::sub_492430 (WIP)

Left-to-right version of the WIP `DrawDigits_492260`: draws the counter digits, skipping
leading zeros (a leading zero that is still rolling, `field_13_offset != 0`, is drawn at
its offset height), and returns the x after the last digit. The x/y `Fix16` arguments
go through the out-of-line constructor `FromInt_4926F0` that this file emits.

Ratio 0.457. Structure and calls are the same, but registers differ throughout. The original keeps
`field_27_sprite_w` in `cl` across iterations (reloaded after each draw for
`curr_xpos +=`, then reused as the next width argument) and keeps `bFirst` at 0x13, not
in an arg slot. Worth another try together with `DrawDigits_492260`.

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

## Mike_A80::DebugDrawProfiling_4FF250 (WIP, was STUB)

The profiler overlay: per texture-size cache stats, totals, polys/texture swaps, memory,
the 30 frame averages, a "LARGE" flash, the Montana display timings, then
`sub_4FFD90` for the history bars. Ratio 0.992.

What it needed:
- `DrawText_4B87A0(buf, 0, ypos, word_703BAA, 1)` with plain ints: the implicit
  conversion builds each `Fix16` argument in place through the out-of-line `Fix16(s32)`
  (`0x4369F0`) like the original. `Fix16(0)` written out is constructed and pushed instead.
- The row loop runs on `ypos` (20..260), with a separate row counter. VC6 then tests
  `ypos < 140` for the size shift.
- The averages are recomputed for each print (no named locals, 0.171 with them); the total
  is one sum of the five.
- `large_timer = total > 30 ? 15 : g; if (large_timer) { g = large_timer - 1; print; }`:
  the original never stores the 15.
- `Mike_A80::sub_4FF970` is a static `__stdcall` (the caller doesn't set `ecx`). It still
  matches.

Still different: only the load order of the five averages in the total. Ours loads
m80_1, m80_5, m80_4, m80_2, m80_3 whatever the source order or grouping (tried 8
orderings and groupings and an inline `Average()`); the original loads 1, 2, 3, 5, 4.

Two things are guessed: the wide format strings at 0x621100 and 0x6210DC (not in
`reccmp/widechar.csv`), and `sub_5BEED0` (15 bytes, near `get_rdtsc_5BEE90`, never dumped,
see "Functions without target asm" below).

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

## CarPhysics_B0::ShowPhysicsDebug_559430 (WIP, was STUB)

Debug text for the car physics (CM/CP, velocities, theta, mass, skids with highlighted
text past the thresholds, surface). Ratio 0.991.

- `swprintf(..., CalculateMass_559FF0().AsDouble())` inline in the argument list: VC6 then
  stores the double to its own slot before pushing it, like the original. A named `f64`
  shares the slot with the returned `Fix16` and the frame is 4 bytes smaller (0.955).
- The highlighted lines keep `DisplayText_5D1F50`'s returned `Garox_C4*` and set
  `field_B0_drawKind = 8; field_B4 = 5`.

Still different: where VC6 puts `lea 0x818(%eax),%ecx` (the `field_650` this pointer) for two
of the `DisplayText_5D1F50` calls: before the pushes for the "theta" line in the original
(ours after), after them for the "front skid" line (ours before). Declaring `pText` at
the top didn't change it. The format string at 0x623FFC is not in `widechar.csv`, guessed.

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

Shut-down for a crew with a car (`Kfc_30::field_24 == 2`): same shape as the matched
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

## Map_0x370::sub_4E8370 (WIP, was STUB)

Removes block `z` from a map column for `RemoveBlock_4E8940`: either into a new copy of
the column (original map columns) or in place (columns added at run time), dropping the
blocks above (`do_drop`) or leaving a hole. Returns the column index or -1. Ratio 0.342.

- Using a `u16** pColumns = field_0_pDmap->field_40008_pColumn;` local for both the old and
  the new column, and the column's `field_0_height` directly instead of a `u8 height`
  local, keeps the column base in a register like the original (0.245 -> 0.342).

Still different: the original keeps `column_idx` in `eax` and spills `this`, and reuses the
`column_idx` argument slot for the offset and the `z` slot for the new index; ours does
the reverse. Splitting the range check into two `if`s with an `offset` local was worse
(0.274, separate return blocks).

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

## sound_obj::UpdateCarEngineAudio_57E220 (WIP, was STUB)

- 0.884 after one cpp_permuter run (400 random candidates). It wrapped everything after the `!pCar`
  return in an `else` (early_return pass) and hoisted `u32 rate` to the top. Before that it was 0.856. Added globals 0x6FF540 (u8 static volume), 0x6FF542 (u16 timer), 0x625010 / 0x625014 (u32 rates).
  `field_54F4` is `field_54F2[2]`.
- The vocal volume and both rates must be unsigned (`shr`, not `sar`, on `>>`). The second sample's volume
  needs `(u8)(a / 254) + (u8)(rand % 3)` to get the byte `add dl,cl`.
- Remaining diff 1: block layout of `paused ? 0 : min(127 - v, 100)`. The target places the `0` branch after
  the ±5/−10 smoothing code and tail-merges the store. `if (paused)` / `== 0` / ternaries were all the same or worse.
- Remaining diff 2: target keeps 0 in ebx across the whole function, while ours re-materialises it in ebp
  after the first sample. The register-allocation knock-on also covers the sample field stores.

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

## Weapon_30::fire_truck_flamethrower_5E0B10 (WIP, was STUB)

- 0.526. Same shape as `fire_truck_gun_5E0E70`, with a fallback to the army-jeep gun sprite (model 248,
  offsets `dword_706EA4` / `dword_706EE8`) when the model 114 turret isn't there. It emits
  `EmitFlameStreamSegment_53F4C0` or spawns bullet 195.
- Turret angle: target calls `Ang16::AssignNormalized_409300` (a normalising "ctor") on the `operator+` result. Added an inline
  `Ang16(Ang16&, s32)` ctor wrapper; `gun_ang = Ang16(a + b, 0)` gives the 32-bit temp store (0.21 → 0.53).
- Remaining: the out-of-line `Ang16(const s16&, s32)` ctor from `operator+` is inlined in the target, and
  the EH state starts at 2 vs our 1.

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

## RouteFinder::NoRefs_589210 (MATCH, was WIP)

- Matched by cpp_permuter in two steps. A random run reordered the case assignments and cast
  `(s16)dx` (0.923 → 0.981). Then `-m exhaustive -p reorder_saves,move_stmt,local_type,cast_operand --depth 2`
  found `s16 dx` plus `dx = 0;` before `dy = -1;` in case 1. Either change alone was worse (0.385).

## Firefighter_28::Service_4A81F0 (WIP, was STUB)

- 0.750. A five-state switch:
  1. Spawn the fire truck, its CarAI and its fireman driver.
  2. Drive to the burning car.
  3. Set the put-out-fire objective.
  4. Wait for the objective result.
  5. Despawn.
- Uses the `SetUniNum_421560`, `IsDespawning_4215B0` and `IsMarkedForDespawn_4214B0` inline helpers instead of raw field writes.
- `dword_67D384` (Fix16) is the arrival distance.
- Not tuned yet. Next step: run the permuter, and check the case 2 `Max_44E540(Abs...)` distance test against the target.

## Car_6C::ctor_4469F0 (WIP, was STUB)

- The body was already written; it was only marked STUB. Now 0.589.
- The logic and stores match. The diffs are in how the pool arrays get built:
  - **CarAI_78_Pool:** the original builds the array with an explicit loop that calls the `CarAI_78` ctor. That is VC6's form for an inline ctor it chose not to inline. We emit the `??_H` vector-constructor iterator, because our ctor (MATCH 0x453CB0) is an out-of-line `EXPORT`.
  - **TrailerPool:** the original inlines `Trailer()` into its array loop. We call it out of line, probably because the function has used up its inline budget.

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

## Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0 (WIP, was STUB)

- 0.556 on the first write-up. It has the same follow / rotate / jitter frame as `539480`, plus two switches:
  - **`(u8)(field_2C_counter >> 2)`**, cases 2–7: sets the sprite id (base + 102 … 97) and the speed (`dword_6FD300` / `304` / `308`). Case 2 and `default` return early on `rng(2)`. Case 2 turns the jitter off.
  - **`field_46_sub_state`**, cases 0–7: sets the sprite flags. Cases 0/1 and case 2 are separate bodies with the same value.
- Its returns duplicate the epilogue, so early returns are right here.
- `dword_6FD304` and `dword_6FD308` are new and set by a static init.

## Particle_4C::UpdateCircularBurst_state_5_539890 (WIP, was STUB)

- 0.381 on the first write-up. Same frame as `5384C0`, with these differences:
  - `field_20` is reset to `kFP16Zero_6FD49C` after the follow block on every path.
  - There is no x offset; `off_y = -dword_6FD45C` always.
  - Cases 6 and 7 take x/y/z from the followed sprite.
  - `default` uses `field_20 / kFP16Two_6FD4A4` (inline `__allshl` / `__alldiv`). On `rng(7) == 4` it spawns a smoke particle through `Particle_8::New_53E3C0(0, ...)` with `SetType_4206F0(8)`.
- The original keeps the jitter flag in `bl` and returns `bl` (still 1) from the first early return. Writing `return bJitter;` there made no difference.

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

## Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280 (WIP, was STUB)

- 0.161. The spark sits on `field_28_pSprite`:
  - **Car:** the spark goes at a corner of the car's box (half width/height plus constants, sub-states 1–4), rotated by the car's angle. State 40 and 41 use mirrored corners. The id is base + sub_state + 200.
  - **Ped:** an offset rotated by the ped's angle plus `Char_B4::field_98`. The id is base + sub_state + 197.
- Needed `#include "Car_BC.hpp"` (compare_builds is unchanged) and three new globals (`dword_6FD3C0`, `dword_6FD5A8`, `dword_6FD2F8`).
- **Open question:** our build calls `Fix16_Point_POD::Fix16_Point_POD()` out of line for the `Fix16_Point offset` local. The original constructs it inline, as it does for all five points in `538AC0` (where we call it twice). The ctor is empty and the struct isn't exported. Unexplained so far.

## Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0 (WIP, was STUB)

- 0.313 on the first write-up. An impact burst attached to a car or ped (`field_28_pSprite`):
  - **Ped:** the source velocity is the ped's `GetVelocityVector_45B520`.
  - **Car:** the velocity is `GetPointVelocity_561350` at the gun-model (114) or fallback-model (248) attachment point. That is the same attachment code as `Particle_8::EmitImpactParticles_53FE40`.
  - The burst moves along a random spread angle (`/ 71`, `Normalize_406C20`).
  - **On a hit:** it moves `Particle_8`'s `field_0` (state 31) or `field_4` (state 34) to the hit. It also sets the damage owner from the shooter's `field_267_varrok_idx`. On a ped hit it calls `HandleGenericImpact_553E00` and `frosty_pasteur::RecordWeaponHit_512C00(id, 194/198, 1)`.
- **Return block:** every failure check in the original jumps to one shared `return true` block. Ours uses early returns, so the next step is the nested form.
- Uses `Fix16_Point::RotateByAngle_40F6B0` for the rotations. The other `Particle_4C` WIPs write the same rotation out by hand.

## Particle_8::GunMuzzelFlash_53E970 (WIP, was STUB)

- 0.120, with the right structure. It spawns the corner sparks that `Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280` then updates:
  - **Car:** two sparks (states 40 and 41) at the mirrored corners of the car box.
  - **Ped:** one spark (state 40) at the `Char_B4::field_98`-relative offset.
  - It does nothing when `bSkip_particles_67D64D` is set.
- The original computes `sin * dword_6FD2E8` and `cos * dword_6FD2E8` into a temporary and never uses them in the car branch. That is kept as `unused`.
- **What's left:**
  - The original's EH state is 4 before the first branch and 7 inside the car branch, so there are more destructible locals declared at the top than ours.
  - Register choice for the zero registers (ebp/ebx swapped).

## Weapon_30::sub_5DE4F0 (WIP, was STUB)

- 0.262. This is the electro-baton beam: it aims `gObject_5C_6F8F84->field_58` from the owner ped to `field_198`, its target.
  - If the distance is more than `dword_706EC4`, it drops the target.
  - Otherwise it steps the beam along the line (`dist / dword_706CF0` steps, at least `dword_706EBC`), stopping at map walls (`sub_5A2440`).
  - On a car hit, or a hit on a ped not in states 8–9, it sets the weapon's `field_4`.
  - Then it marks the target as shot by the owner and calls `sub_5DE910`.
- `sub_5DE910` (0x772 bytes) was missing from the source entirely. It's now a `STUB_FUNC`: `void __stdcall (Fix16_Point by value, Fix16_Point&, Fix16 z)`, worked out from the call site. Both point arguments are `get_x_y_443580()` of the target's sprite. It still needs target asm (it was not in the dump).
- Changed the return type to `void`, since nothing sets one. `electro_batton_5E0740` still matches.

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

## Trailer::UpdateTrailerAlignment_407CE0 (WIP, was STUB)

- 0.155, with the right structure.
  - It rotates the trailer's rear-wheel point by the trailer angle and the cab's hitch point (`gTrailerHitchOffset_66AAC8`) by the cab angle, and adds each body's `get_cp1_40B560()`.
  - It takes the angle between the two points, then eases the trailer's angle toward it with `sub_405E80`. When the cab drives straight with the gas pressed, it uses `sub_405DA0` with the cab speed instead.
  - Finally it puts the trailer at hitch minus the rotated `gTrailerCabOffset_66AAE0`.
- Angles go through the `* 71` / `/ 71` `Fix16` radian conversions (`Ang16_to_Fix16`, `Ang16(Ang16(raw / 71), 0)` via `AssignNormalized_409300`).
- `sub_405DA0` (0x76) and `sub_405E80` (0x110) were missing from the source. They're now `STUB_FUNC`s in fix16.cpp, with signatures from this call site. They still need target asm.
- **What's left:** the original's EH state is 3 at entry (three point locals constructed with no code), and `esi`/`edi` are swapped.
- Changed the return type to `void`. Its caller, `CarPhysics_B0`, ignores it.

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

### sub_5DE910 (0x5DE910): WIP 0.149
- The electro-beam draw. It starts at `a1`, or at the muzzle offset rotated by `word_707004` plus `stru_706E58` when `byte_706C94` is clear. It first lays `kFP16Quarter_706CF4`-long segments, each with a random kink (`rng(rng(4)+1)*32`, scaled by `dword_706D34`, in `Ang16`). It then lays straight segments the rest of the way to `a2`, using max(|x|,|y|) for the count. Each segment's midpoint goes to `Particle_8::EmitElectricArcParticle`.
- The call sequence is almost the target's. The distance is computed three times, the first `atan2` result is dead, and `rng(2)` is called and dropped.
- The EH state is 0xA at entry and never changes. So 10 `Fix16_Point` locals are declared at the top, plus the by-value `a1`. That took it from 0.075 to 0.149.
- What's left is VC6's inline budget. Our build calls `Fix16_Point_POD()` out of line for the first two locals. The original instead inlines the loop-1 `mid /= k; mid += cur` with `Fix16 /=` as calls (0x539F90), and calls the copies at 0x5E40E0/0x5E40C0 in loop 2. Using inline `Fix16_Point` operators for both loops gave the right loop-1 code but inlined loop 2 and left 6 ctor calls (0.072). Writing the length out by hand gave 2 calls (0.067). See the ctor note in matching_quirks.md.

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

### PublicTransport_181C::PublicTransportService_57A7A0 (0x57A7A0): WIP 0.710
- Buses, then for each train: the player driving it controls it. `field_50` (the train's
  motion state) gives whether it's stopped, and `field_48` steps through approach, stop,
  doors, wait, leave for the current station.
- The redundant driver null test needed a reload through `field_C_carriages[0]` (see
  matching_quirks.md). `field_4C` is read once into a local when moving to the next station.
- Left: register choice in the "train at zone" checks. 10,000 permuter iterations only shuffled
  scopes (188 -> 140).

### sound_obj::HandlePedVoiceEvent_423080 (0x423080): WIP 0.992
- Picks a ped's voice sample by voice event (`Ped::field_250`), with cooldown bytes
  `byte_67554A/B/C` and `word_675548` for repeated shouts (Elvis gets his own), then queues it.
  `bTank` (tank driver, or `Ped::IsLawEnforcement_45B4E0`) is uninitialised on the player path, as in the
  original.
- 0.809 -> 0.974: the entity index is used unsigned (`(u32)... % 5`). 0.974 -> 0.987: split the
  rate sum so `field_14` is reloaded. 0.987 -> 0.992: the permuter's ternary for the volume and
  a plain `(x & 1) == 1`. One register is left, see matching_quirks.md.

### Car_6C::SpawnCarOnRoadNetwork_4458B0 (0x4458B0): WIP 0.580
- Finds a junction near the point (`RouteFinder::sub_58A130`, whose `s16` result the original
  narrows to `char`), heads out of it, then walks the road like `SpawnBusAtValidRoadPosition_4453E0`
  until a free, off-screen spot is found. The car gets an AI following the route.
- Left: the dead-parameter-slot assignment (see matching_quirks.md). A `dir` local for the
  junction switch, `ToInt()` written inline and the permuter all made it worse.

### sound_obj::PoliceRadioMessageGeneration_426790 (0x426790): MATCH
- Picks the most serious crime reported this frame (`Shooey_CC::GetLatestReportedCrime`, crime 9
  only when nothing else was reported), counts down the radio timers, then queues the dispatcher
  lines for a new wanted level or one random chatter line.
- 0.998 on the first build. It matched once `best_crime = 0` was declared before the two
  `u8 = 0`s. It also needed the empty `sound_obj::nullsub_4` (0x427330) and Shooey_CC.hpp.

### Police_7B8::sub_56FBD0 (0x56FBD0): MATCH
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

### Char_B4::HandlePedCollision_548BD0 (0x548BD0): WIP 0.592
- A 5x5 switch on both peds' `field_238` types. It covers Elvis followers, pushing the other ped
  away (`atan2` + 180 degrees, with `Normalize_406C20` or the inline `Normalize`), slowing down,
  turning 30 degrees, stepping back to the saved position, and cops arresting a wanted ped.
- 0.457 -> 0.592: case order from the jump tables, the `occ != 43` branch first, and the
  differences passed straight to `atan2`. The frame-size difference is under "Still
  unexplained".

### sub_5DE910 (0x5DE910): moving the arc into an inline helper is wrong
- The permuter scored nested scopes for the `Fix16_Point` locals (836 -> 542). An inline helper
  owning the arc locals gave 0.056: the original constructs all ten locals at entry
  (`movl $0xA` EH state), so they belong to the function itself.

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

### Map_0x370::sub_4E7190 (0x4E7190): WIP
- The reverse road follower. When it runs off the road it looks for a turn in the neighbouring
  blocks, via `gMap_0x370_6F6268` rather than `this`, and returns the opposite direction.
- Same helpers as sub_4E6660, plus the null-checking lookup. The function runs out of inline
  expansions (see matching_quirks.md), so the neighbour z offsets use raw `mValue` arithmetic
  and the arrow check and `dist` update are written out. Left: `dist` and `pPrev` swap `%ebx`
  and `%ebp`, the constant cached in `%ebp` before the first switch, and the neighbour arrow
  check's `xor`/`test`, which needs an inline the budget can't afford.

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

### Sprite_4C::DrawCollisionBox_5A4DA0 (0x5A4DA0): WIP 0.97
- Projects the bounding box corners and the rendering rect points with a static inline copy of
  the projection and joins them with `DrawDebugLine_5D7DD0`. Like the original, the function runs
  out of inline expansions: the projection's `Fix16` operators are calls and the eighth projection
  is the out-of-line `sub_5A5690`, which matches on its own. Left: the stack slots of the inline's
  argument copies (4 bytes of frame).

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
- `Car_BC::sub_43B2B0` (0x43b2b0): known unsolved return width (bool call result returned unextended, other paths eax); not retried beyond analysis (IsField238_45EDE0 has 96 callers testing al).
- `MapRenderer::Set_UV_4F4190` (0x4f4190): fmuls (1/16384) scheduled after the idx load in orig; AsFloat/ToFloat/mValue*k/local float, 400 permuter iters (x87 scheduling, cf. Draw*Sided* note).
- `Hud_Brief_704::ClearAllBriefsWithPriority_5D4890` (0x5d4890): known: ebp shrink-wrap (pushed after null check). Code otherwise identical. Tried if+do/while, early return, break, if(pIter) Start(), decl order, 500 permuter iters. VC6 does shrink-wrap in similar matched loops (struct_4::RemoveByRngValue_5A6C40).
- `Car_BC::sub_43B850` (0x43b850): known: u16 load then test $6,%ch; tried IsFlagSet_411930 inline, local copy, shifts, casts, != 0.
- `Map_0x370::sub_4E8C00` (0x4e8c00): add operand order (result in the field's reg, edx); 9.6f has our order. Tried operand swaps, locals, param reassign, pDmap local, inline getters, /4 and /12 spellings, 400 permuter iters.
- `RouteFinder::sub_589E20` (0x589e20): known: loop-top test of a flag known 0 on entry (unrotated while). Tried for(;;)+break, && condition, if/else, return in loop, char/s32 flag.
- `Object_2C::sub_526830` (0x526830): known: switch clobbers value (add $-39) and reloads param in default; tried direct returns, default return, param reassign, switch(a1-39), (u32) switch, result=a1 init.
- `Map_0x370::do_process_loaded_zone_data_4E8E30` (0x4e8e30): base+index operand order in two places (zone-info loop: offset reg then add field_334; mov %bl,(base,idx)); 9.6f has our order. Tried + operand swaps, byte offsets, for loop, local placement.

### Near-miss pass, batches A2 and B2 (2026-10-02)

- `sound_obj::HandlePedVoiceEvent_423080` (0x423080): no change. skipped: documented unexplained (add %eax,%edi; 3000 permuter iterations)
- `Camera_0xBC::UpdateBoundaries_435B90` (0x435b90): no change. only diff: reg alloc in final field_20 = field_78 +/- dword_67691C block (right: field in eax/dword in edi swapped); operand order swaps have no effect; 9.6f-style rewrite (one v, checks on fields, v*=) much worse
- `Car_BC::HandleUserInput_4418D0` (0x4418d0): no change. only diff: 'cmp %bl,%al' (bl=0) vs our 'test %al,%al' after HandleRoofTurretRotation call; tried !=0, !=(char)0, ==true, !=field_B8, local zero var, combined condition; same in 9.6f
- `Car_6C::dtor_446DC0` (0x446dc0): no change. only diff: last delete (gSprite_Unused_677938) loads ptr into ecx then mov ecx->esi, ours esi then esi->ecx (Car_14 delete just above uses esi in both). Dropping the if made it worse
- `Ped::ComputeAimAngle_45C9D0` (0x45c9d0): no change. VC6 duplicates the return tail into the atan2 branch (forwarding the stored angle) where orig jmps to the shared reload tail; tried branch inversion, rValue stores, named temp, getter, 500 permuter iters
- `Ped::Deallocate_45EB60` (0x45eb60): no change. bitfield b0 clear: orig dword load, and $0xFE,%al, dword store, scheduled after the timer store; ours and $-2 on edx with field_16C load hoisted. Tried &= masks, u8 cast, reorder, inline setter (bool/u8/s32)
- `menu_option_0x82::SelectPrevHorizontalIdx_4B6390` (0x4b6390): no change. original reloads field_6E in the loop compare (cmp 0x6E(%ecx),%ax), VC6 CSEs it to si in ours; same in 9.6f and in sibling 4B6330. Tried s16 old, swapping old/new init
- `MapRenderer::Draw4SidedDiagonalUpLeft_4EF880` (0x4ef880): no change. (skipped) known MapRenderer Draw*Sided* x87/vertex store scheduling, not attempted
- `Ambulance_110::ProcessPatientQueue_4FA500` (0x4fa500): no change. only diff: original interleaves load/sar/store for x,y,z (as if stores may alias), ours hoists the 3 loads; tried separate decl/assign, ToUInt8, stores through u8* pointers
- `Ambulance_20::UpdateState_4FB330` (0x4fb330): no change. only diff: case 3 '>500' false path should jle back to shared epilogue (0x3F) rather than the adjacent duplicate pop/ret; tried return after state=5, inverted if, break in default
- `Mike_A80::DebugDrawProfiling_4FF250` (0x4ff250): no change. skipped: documented (load order of five averages; 8 orderings tried before)
- `youthful_einstein::SetNewFugitive_516590` (0x516590): no change. original reloads field_0 into edx (not esi/ecx) before SetPlayerArrowColour; with local pPlayer VC6 reuses esi, without it uses ecx and the else branch's gHud reg shifts too. Tried field/GetPlayerPed/pPed local/ref
- `NetPlay::EnumSessions_51E650` (0x51e650): closer, 18->7. wrong flag (orig 0x80 RETURNSTATUS, not STOPASYNC); else only fails on hr<0; nested success + single return -1. Left: else jge into the modem's shared return-count block
- `NetPlay::RemovePlayerByName_520F80` (0x520f80): no change. original spills bRemoved=0 to a stack slot (not const-propagated), found path returns ebx=1; tried bRemoved=1 after the call, while(!bRemoved && i<count) loop: VC6 const-props both
- `struct_4::CleanupSpriteList_5A7080` (0x5a7080): no change. keep-branch block (pLast = pIter) laid out between the two unlink branches in orig; tried inverted conds, continue forms, if+do/while, nested ifs, 600 permuter iters
- `gtx_0x106C::GetSpriteTrueIndex_5AA460` (0x5aa460): no change. known unexplained (quirks list): default 'mov 8(%esp),%eax'. Tried default return direct, (s32) cast, s32 param (still ax and breaks 13 callers)
- `Montana_4::dtor_5C5F10` (0x5c5f10): no change. same pattern as 0x446dc0: original looks like an inlined scalar deleting dtor (ptr tested in ecx, push esi + mov ecx->esi inside the if), ours keeps ptr in esi; tried moving ~Montana_2EE4 after use, an inline 'delete this' helper
- `SetWindowedMode_5D9510` (0x5d9510): no change. push $0x316 scheduled before the height arithmetic in orig; all operand orders compile the same, locals much worse, 500 permuter iters

### Near-miss pass, batch D (2026-10-02)

- `Map_0x370::sub_4E6190` (0x4E6190): no change. only diff: original cross-jumps case 3/4 inner switch tails into case 2/1 (jmp into other case's dec/jne); ours duplicates. Inner case order, default:return 0, casts, if-returns: no effect
- `Car_BC::GetHitchPoint_439FB0` (0x439FB0): no change. this in esi vs edi, sum of x*cos+y*sin kept in eax in original, missing EH state before final operator+ (original calls 0x40AC50 non-visible?); ternary/local sprite/hand-written rotation/non-inline add all worse
- `Gang_144::ApplyKillRespectChange_4BEF70` (0x4BEF70): closer 23->21. compare swapped. Left: original moves pGang to ecx and computes respect*reaction before the lea of field_11C[p], then reloads via full address; inline member helper, locals, casts, pointer: no change
- `Car_BC::IsSpriteShrunk_43DC00` (0x43DC00): closer 28->22. return a != b (Fix16 op in first branch, .mValue in second: matches setne/xor shapes). Left: load order/regs (esi popped early in orig)
- `Player::AddCarToHistory_5645B0` (0x5645B0): closer 29->23. for(;i<3;p++,i++){ if(!*p){*p=new;return;} } then shift. Left: esi/edi swap of the two history pointers, shift stores through a copied pointer (mov %esi,%eax) in orig; decl orders, pointer alias, struct copy, memcpy: no change
- `Map_0x370::HasGreenArrowForPathDirection_4E5E90` (0x4E5E90): no change. same as 0x4E6190: original tail-merges identical HasBlockDesiredArrow calls across switch cases (case4-else = case3-if, case1-else push 2; jmp into it). Ternary/if-chain/compiler flags don't reproduce; likely the same unexplained cross-case merge
- `miss2_0x11C::Locate_509FD0` (0x509FD0): no change. register rotation only (eax/ecx/edx) in the STOP_LOCATE_CHAR_FOOT/CAR case blocks; pObj/typed local/getter/default position/velocity local: no change
- `frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0` (0x5121E0): closer 24->22. str_count before if, single store after (empty path stores ax=0 like orig). Left: dead (len+9)&~1 in edi, tableSize test via bx instead of zero-extended stack slot, first-loop regs
- `Mike_A80::sub_4FFD90` (0x4FFD90): no change. x87 'fildl x; flds 630; fsub %st(1),%st ... fstp %st(0)' (x kept and popped) vs our fsubrs; double/f64 x, dead right/top locals, casts: no change (already in match_attempts)
- `Car_BC::ManageTVAntenna_4425D0` (0x4425D0): closer 39->11. Ang16 towerAng; declared at top (9.6f), towerAng = Get...() (T x; x = f()). Left: sprite rot is loaded into cx and reused for the conversion, orig compares against memory and reloads; operand order, s32 NotEqual (9.6f 0x41CFF0 returns s32), towerFp local: no change
- `Car_BC::EmitExplosion_43D690` (0x43D690): no change. original has an EH frame around building the implicit Fix16(2) z argument (out-of-line Fix16 ctor 0x4369F0 treated as throwing, arg address saved for cleanup); ours has none because the inline Fix16(s32) body is visible. Same family as the 'Fix16(s32) ctor copies can't be written' note

### Near-miss pass, batch F (2026-10-02)

- `Type_3_HandleCarImpactSound_4174C0` (0x4174C0): closer 0.822->0.915. if chain -> switch gives the original's `sub $0`/`dec`. Left: cases 0 and 1 compute the 0x7FFFFFE3/0x7FFFFFE7 multiply in ecx, original in edx
- `TrainCab_414710` (0x414710): closer 0.684->0.961. zone read through `pTrainStation->field_10_pZone` each time, not a local. Left: original's failure jumps go to the success path's epilogue, ours to the shared one after the else
- `0x5D3B80` (DrawBrief): no change. identical except the y Fix16 ctor calls the duplicate copy FromInt_4926F0 instead of 4369F0 (duplicate-helper-copies quirk)
- `0x57DD50`: skipped, the known sete issue above
- `0x4D94E0`: no change. needs field_0 as a real ofstream member, which crashes the standalone build (see the source note)
- `0x554710`: no change. original pushes/pops ebp inside one branch (late-ebp-push quirk)
- `0x5552B0`: no change. layout only: original puts the shared `return 0` epilogue right after the loop's bottom test
- `0x4C1AB0`: no change. u16 fields right, eax/ecx roles swapped throughout
- `0x5D1B10`: no change. original loads its s16 params with a 16-bit mov (9.6f too) and keeps 0 in ebx for three stores
- `0x541430`: no change. Ang16 sum is a 16-bit add placed before the point ctor, plus an epilogue-sharing difference
- `0x5DCF60`: no change. ped pointer kept in ebx with y loaded later; failure path does `xor eax,eax; mov %al,global`

### Near-miss pass, batch G (2026-10-02)

- `Montana_4::AddSprite_5C5CF0` (0x5C5CF0): closer. `field_2EE0_free_indx` is u32 (jb), dropped an (s16) cast (movswl). Left: ebx/ebp swap; a (u8) cast on the first compare gets the permuter to 4 but is wrong
- `Crane_15C` dtor (0x47E5B0): matches if field_0..field_20 become Fix16_Point, but then the matched ctor 0x47E610 calls the Fix16_Point_POD ctor out of line. One-for-one trade, not taken
- `Frontend` ctor (0x4AF2A0): no change. original zeroes the name/password/bonus-stage arrays with direct disp(%esi) stores; ours shares 0 in ebx or uses a lea base. Permuter segfaults on it
- `Frontend::DrawBackground_4B6E10` (0x4B6E10): no change. VC6 merges the two final retry blits into one tail (see Still unexplained)
- `Ped::HandlePickupCollision_45DE80` (0x45DE80): no change. ebx/edi pushed only after the early returns (late push quirk)
- `Frontend::GetNextUnlockedMainStage_4B7270` (0x4B7270): no change. ours goes branchless and doesn't hoist the loop flag load; 1800 permuter iterations, 54 -> 44 at best
- `Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0`: skipped, on the Still unexplained list

### Near-miss pass, batch C (2026-10-02)

- `Car_14::GetRandomTrafficSpeed_583750` (0x583750): closer 14->7. shared lo/hi/factor locals and one final lerp. Left: final sum in ecx, orig eax; by-value return, swapped operands, +=, a Lerp inline, `&(*p = x)`: same code
- `0x442520`: only the EH state stores around `Fix16_Point::operator-` (0x40AC80) differ. `throw()` on it matches this one but changes 5 matched functions (Crane_15C x3, draw_4F3FB0, 0x40AC80), not kept
- `0x463FB0`: order of two blocks in the switch default; every spelling gives the same layout, 9.6f has the original's
- `0x4EB940`: the MapRenderer x87 scheduling quirk
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
- `PushCarInfo_564680`: no change. original shifts with rep movsl alone; ours movsl + movsb
- `EnumerateMaps_51BFA0`: no change. one `if (map_count > 0)` around the rest: 15 -> 51, reverted

### Near-miss pass, batch I (2026-10-02)

- `MapRenderer::sub_4F4250`, `sub_4F49B0`: closer 31->19. `T_gbh_DrawTile` takes the diffuse colour as u8 (pushed without zero extension). Left: x87 scheduling around the ProjectVert inlines
- `MapRenderer::sub_4EC450`, `sub_4ECAF0`, `sub_4ECE40` (and `sub_4EC7A0`): closer 47->28, 49->30, ->28. atan2(dy, dx) order, angle test `> 45 && < 225`, Fix16(f32) via 16384.0f, 4ECE40 uses GetTexture_46BB50. Left: x87 scheduling
- `MapRenderer::draw_lid_4EE130`: no change
- `Weapon_30::oil_stain_5E1DC0` matched once the z bound was `k_dword_706EDC - k_dword_706F70` and the half depth/lower bound got named locals (inline budget)
