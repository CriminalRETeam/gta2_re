# Known crashes and original bugs

`decomp_main.exe` doesn't have the memory layout of `10.5.exe`, so a bug the original survives can crash it. The game
code stays as the original wrote it, bugs included: a crash from such a bug is fixed outside the matched code (for
example in the renderer DLLs), and a crash from a decomp mistake (wrong struct size, wrong global, missing init) is
fixed at its cause.

## Original bugs that can reach outside the game code

- `TextureCache_15D8::ReadTextures_5B92E0` registers 1024 tile textures, but `bil.sty`, `ste.sty` and `wil.sty`
  hold 992 tiles (a 4063232 byte `TILE` chunk). For tiles 992 to 1023 it passes pointers up to 128 KB past the tile
  buffer, and `Aligned_malloc_4FE510` leaves at most 64 KB of slack. The GTA2Hax renderer copies the pixels when a
  texture is registered (`InternalRegisterTexture`, `SetTexture`), so that is where the read happens; deferring the
  copy to the first draw would avoid it, since no map uses those tiles. Not reproduced under wine 11.19: the game
  ran 90 s in `wil` without a fault.

## Decomp mistakes, fixed

"Global crashing standalone" meant `GlobalsRegistry::CheckVars`: `test.cpp` runs it at every start and it aborts
when two `DEFINE_GLOBAL` regions overlap. `decomp_main.exe --check_vars_only` runs only that check. An overlap
means a size in the decomp is wrong.

- `gTrainStationList_6FEE68` was `TrainStation_34* [101]` plus a count at `+0x194`, covering 0x6FEE68 to
  0x6FF000, so the nine train globals in that range (and three next to it) were left unregistered. The original has 5 pointers at
  0x6FEE68 (`SetupTrainAndBusStops_5794B0` clears five) and a byte count at 0x6FEFFC (`gTrainStopCount_6FEFFC`).
  The train globals are `DEFINE_GLOBAL_INIT` again, with the values of their static initializers.
- `sThetaText` was `wchar_t[32]`, running into `kAngZero` and `dword_66A924`. The next global the
  original uses is at 0x66A90C, so it is `wchar_t[16]`.
- Earlier ones of the same kind: `gNetPlayerDropped_6F8470` (really `gTagGame_6F8450.field_20_bHasQuit`) and
  `gNetworkGameSettings_707098`.

## Fixed in the game code

- `ErrorLog`: outside VC6, `field_0_ofstr` is a 0x3C-byte placeholder. Casting it to `std::ostream` in
  `Write_4D9620` read a null vbptr inside the static initializers that build both logs, so non-VC6 builds (the
  VS2022 CI exe) crashed before WinMain. The stream code is compiled only for VC6, where the member is a real
  classic `ofstream`; VC6 builds never crashed there.

## Not looked at

- `log_on_line_written_cb_4D9690` (`error.cpp`) is said to crash the patched exe.

## Original bugs kept as they are

No crash seen from these; they stay as the original wrote them.

- `Ped::FindNearestPed_466F60`, `Ped::FindNearbyPed_466FB0`: `AsCharB4_40FEA0()->field_7C_pPed` without a null check.
- `text_0x14::Wide2PesudoAscii_5B5D10` loops on `pSrc` instead of `*pSrc`; only the length check stops it.
- `registry.cpp` closes keys that failed to open.
- `Frontend.cpp`: the cheat string length check compares with 16.

## Running decomp_main.exe under wine

Seen with wine 11.19 (wow64) on Oct 20, with a GTA2 folder that has the windowed-mode and widescreen mods:

- The folder's `ddraw.dll` (windowed-mode wrapper) patches the `CoCreateInstance` import, then
  `GetDirectXVersion_4C4EC0` unloads it with `FreeLibrary`. The `CoCreateInstance` in `GetDirectMusicVer_4A0650`
  then jumps into the unloaded DLL. `10.5.exe` crashes there too. Without the wrapper (wine's `ddraw`, and a
  wine virtual desktop for a window) `decomp_main.exe` starts.
- The folder's `mss32.dll` is an ASI loader for `scripts/GTA2.WidescreenFix.asi`, which patches `10.5.exe`
  addresses. Move `scripts/` away for `decomp_main.exe`.
- `skip_frontend` (DWORD) and `mapname`, `stylename`, `scriptname` (strings such as `wil.gmp`) under
  `HKLM\SOFTWARE\DMA Design Ltd\GTA2\Debug` (32-bit view, `wine reg add ... /reg:32`) boot straight into a level.
- `EnableBoot2MapDebugOptions` (`ImGuiDebug.cpp`, called from `test.cpp`) sets `bSkip_trains_67D550`, so standalone runs don't exercise the
  train code.
