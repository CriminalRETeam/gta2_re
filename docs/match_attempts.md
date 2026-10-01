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

## BurgerKing_1::sub_498CB0 (STUB)

Target: `mov 4(%esp),%eax; shr $7,%al; mov %al,byte_67B80C; ret $4`. It loads the whole
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

These have `STUB_FUNC` markers but are compiler-generated in the original, so there is
no source to write for them:

- `PedGroup::sub_4C8E60` (0x4C8E60): the static destructor for a global array of 20
  `PedGroup`s (`eh vector destructor iterator` on 0x67EF20, size 0x44).
- `NetPlay::static_dtor_5E4DD0`: the `atexit` destructor for `gNetPlay_7071E8`
  (`mov $gNetPlay,%ecx; jmp ~NetPlay`). VC6 now generates it, since `NetPlay` has a real
  constructor.
- `NetPlay::vdtor_51D7B0`: NetPlay's scalar deleting destructor (`??_G`), generated from
  the virtual destructor.
- The `crt_stubs.cpp` functions (`malloc`, `free`, `fopen`, ...) are the static CRT.

The markers can't be checked either way: there's no function body to put after them.

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
`num_doors`, then `door_info[]`), turned into `Fix16` with `dword_6F6850.sub_41FE70`,
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

Left-to-right version of the WIP `sub_492260`: draws the counter digits, skipping
leading zeros (a leading zero that is still rolling, `field_13_offset != 0`, is drawn at
its offset height), and returns the x after the last digit. The x/y `Fix16` arguments
go through the out-of-line constructor `FromInt_4926F0` that this file emits.

Ratio 0.457. Structure and calls are the same, but registers differ throughout. The original keeps
`field_27_sprite_w` in `cl` across iterations (reloaded after each draw for
`curr_xpos +=`, then reused as the next width argument) and keeps `bFirst` at 0x13, not
in an arg slot. Worth another try together with `sub_492260`.

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
`(xpos, ypos)`, starting from `dword_6F5B8C` (255.0). Ratio 0.644.

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
- The original calls `Ang16::sub_406C20` for both normalisations: build the angle with
  `Ang16(s32)` (no inline `Normalize`) and call `sub_406C20()` explicitly.
- `dword_6FD540 * dword_6FD4A8` (operand order picks the load order), and
  `dword_6FD4A0 * dword_6FD540` in the `<= 40` case.
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

The projection is the `Camera_0xBC::sub_40CFC0` formula with this TU's copies of the
constants (`dword_6FFC7C / (u + dword_6FFC9C)`). Written out in the function, VC6
inlines every `Fix16` operator (0.259). Moved into a `static inline` helper, VC6 inlines
only the first multiply and calls `Multiply_408680`/`Add_408660`/`Subtract_436A00` for the
rest, as the original does (0.349). So the original most likely called an inline
projection helper (see `matching_quirks.md`).

Still different:
- The original calls an out-of-line `Fix16(u8)` (`0x45C4E0`) for the x argument of
  `sub_58CF10`, ours inlines both.
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

- The car block uses the inline helper `sub_4BA2C0` (moved above this function). Its font
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
`sub_571350`, with the car's `field_76_last_seen_timer > 200` checks and the despawn state
set to 4. Returns nothing (now `void`). Ratio 0.867.

The code is identical apart from three `je`s in the "car, no ped" branch: the original jumps
to the `field_2C = 1` tail right after the `if`, ours to the identical tail of the last block
in the function, which makes them near jumps (12 bytes longer). Tried: `field_2C = 1` in
both branches (0.812), an explicit `return` after it (no change).

## PoliceCrew_38::sub_571A30 (WIP, was STUB)

Shut-down for the other crew kinds. If the player is driving, the crew either gets back
into its car (group members flagged `field_238 = 3`) or, once everyone is ready, leaves
and despawns; otherwise like `sub_571350`, with `field_7C_uni_num == 2` cars kept. Now
`void`. Ratio 0.748.

- `pPed_6FEDDC == pCar->field_54_driver` (the global on the left) gives the original's
  `cmp 0x54(%eax),%ecx`.

Still different:
- The original keeps the `field_28 = 5; field_2C = 1` return block right after the first
  group path and the other paths jump back to it; ours puts the shared copy elsewhere.
- In the first member loop the original does `inc %dl` and the store before reading
  `field_4_ped_list[i]` (same loop as `sub_571350` otherwise). Rewriting it as a `while`
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

- 0.856. Added globals 0x6FF540 (u8 static volume), 0x6FF542 (u16 timer), 0x625010 / 0x625014 (u32 rates).
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

- 0.429. Added `gNetworkGameSettings_707098` (NetworkGameSettings). It overlaps the existing `dword_7071A0` /
  `dword_7071B0` in Game_0x40.cpp (game_speed / police_on fields).
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
  `field_C` 2 starts a thread (`sub_5120C0`) and 3 calls `sub_511A70` + `sub_5C8680`.
- In case 4 (cmd types 0xD4/0xD6/else) each branch must have its own full `GetBasePointer` + id compare.
  VC6 then tail-merges the compare into the case 6/7 code as the target does (0.36 → 0.43).
- Remaining: register/scheduling differences in most cases. Our case 3 car branch is tail-merged into case 5,
  and the target keeps them separate. The target also sets edi = pSprite on the "no match" paths, an
  uninitialised `pCar` coalesced with the param.
