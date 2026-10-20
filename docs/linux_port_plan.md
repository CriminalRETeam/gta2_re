# Cross-platform port plan (Linux first, as a fork)

Goal: a modern GTA2 that builds and runs natively on Linux (then macOS and Windows), in a
**fork** of this repo. Upstream stays the 1:1 matching reference; the fork drops VC6 and
is free to diverge, including quality-of-life changes that no longer match.

## Two phases

**Phase A, tracking upstream.** The decomp still moves (names, structures, fixes), so the
fork merges `upstream/master` regularly. To keep those merges trivial:

1. **Don't edit game code.** The port lives in `Source/port/` and the build files, and
   replaces what the game calls (Win32, DirectX, Miles, Bink) instead of its callers.
2. **Send portability fixes upstream.** A dialect fix that is VC6-neutral (checked with
   `compare_builds.py`) goes to upstream as a PR, so the fork's diff on game code stays
   near zero. What upstream can't take (inline asm replacements) stays a short, listed set
   of fork commits.
3. **32-bit first** (`-m32`), so struct layouts and pointer-sized fields stay as they are.

**Phase B, after the freeze.** Once upstream has settled, the fork freezes a base, stops
merging and refactors freely: modern C++, 64-bit, `char16_t` text, no matching artefacts
(markers, global addresses, reconstructed dead code, raw `mValue` arithmetic). Two rules:

1. **Every gameplay change is an option, off by default.** With all options off, the game
   behaves exactly like `10.5.exe`.
2. **The replay test gates refactors.** Recorded replays must produce the same per-frame
   game-state hashes as the original exe (see Verification). It must exist before phase B
   starts.

## Starting point

- The game code is complete (3419 `MATCH_FUNC`, 6 `WIP_FUNC`, 9 `STUB_FUNC`) and
  `build.py --run_standalone` runs it.
- CI already builds with MSVC 2022 (`Windows_VC_Latest`): the code compiles with a modern compiler.
- `Source/Function.hpp` already has a non-MSVC branch: markers, `EXPORT` and the global
  macros reduce to plain C++, so phase A can keep them in game code at no cost.
- Renderer and video are DLLs behind function tables loaded with `GetProcAddress`: 36
  `gbh_*` in `gbh_graphics.cpp`, 22 `Vid_*` in `dma_video.cpp`. GTA2Hax already has open
  D3D9 versions of both (`3rdParty/GTA2Hax/d3ddll`, `3rdParty/GTA2Hax/DmaVideo`).
- Miles and Bink are already stub libraries in the repo (`Source/3rdParty/mss.cpp`,
  `bink.cpp`): the natural place for real implementations.
- `HookLoader` already injects decomp code into the original exe: the base for the replay harness.
- The whole OS surface is the import table of `10.5.exe` (appendix).

## Architecture

```
game code (unchanged in phase A)
  | Win32, DirectInput, DirectPlay, AIL_*, Bink*, gbh_*, Vid_*
  v
Source/port/
  win32_shim     LoadLibrary/GetProcAddress to built-in modules, registry to ini,
                 timers, paths, MessageBox, wsprintf
  window_sdl     SDL2 window and event pump behind CreateWindowExA/PeekMessageA/WndProc
  dinput_sdl     IDirectInput + IDirectInputDevice over SDL keyboard and game controller
  mss_openal     the 52 AIL_* functions over OpenAL Soft (or miniaudio)
  bink_ffmpeg    the 18 Bink* functions over libavcodec
  gbh_gl, vid_gl the two renderer tables over OpenGL 3.3
  dplay_enet     (later) DirectPlay over ENet
```

In phase B the shims can be folded into the game code (direct SDL calls, no fake
`HWND`/`WM_*`), one subsystem at a time, each behind the replay test.

## Work items

Size: S a few days, M one to two weeks, L several weeks.

| # | Area | Where | Port | Size |
|---|---|---|---|---|
| 1 | Build | new top-level `CMakeLists.txt` | One executable from the game sources + `Source/port`, gcc/clang `-m32`, SDL2. Drop the VC6 toolchain, `build.py`, `Scripts/bin_comp`, reccmp, permuter, `HookLoader`, `ExePatcher`, `cl_proxy`, `WinMainProxy`, `gta2_dll_*`, Detours, Manual-DLL-Loader. Keep the upstream file list in sync on merges. | S |
| 2 | Compiler dialect | all TUs | `__stdcall`/`__cdecl`/`__fastcall`/`__thiscall`/`__declspec` as macros, `__int64`, VC6 for-scope, missing `typename`, implicit conversions. Order: MinGW gcc i686 (still Win32, runs in Wine), then Linux. Fixes go upstream when VC6-neutral (phase A rule 2). | M |
| 3 | Inline asm | `SpriteRenderer_1C.cpp` (`get_rdtsc_5BEE90`), `gtx_0x106C.cpp` (`sprite_delta::Delta_5ABA00`, `Delta_5ABA40`) | `__rdtsc()` and two C loops. Fork-only commits (or a `#if !defined(_MSC_VER)` fallback upstream accepts). | S |
| 4 | Wide strings | 270 `wchar_t`, 292 `L""`; `swprintf` 149 (VC6 form, no size), `wcsncpy` 85, `wcscpy` 26, `wcslen` 17, `wcstombs` 9, `wcscmp` 9, `_itow` 3 | GXT text is UTF-16 read straight into `wchar_t` buffers. Phase A: `-fshort-wchar` and our own versions of these 7 functions (glibc's assume a 32-bit `wchar_t`). Phase B: a `char16_t` typedef and `u""`. | M |
| 5 | Window, loop, timers | `winmain.cpp` (`CreateWindowExA`, `RegisterClassA`, `PeekMessageA` loop, `SetWindowPos`, `ShowCursor`), `FpsCounter_54`, `Mike_A80`, `ProfilerTimer_C` | SDL2 window. The shim turns SDL events into the `WM_*` messages the WndProc handles (activate, close, keys). `timeGetTime` to `SDL_GetTicks`. | M |
| 6 | Renderer | `gbh_graphics.cpp`, `dma_video.cpp`, DLL selection in `winmain.cpp` | OpenGL 3.3 implementation of both tables, ported from the GTA2Hax D3D9 code. `LoadLibraryA("d3ddll.dll")` returns the built-in table. Build it first as Windows DLLs: they drop in next to the original exe and get validated before any Linux work. | L |
| 7 | Input | `input.cpp`, `InputRecorder_67F8B0.cpp`, `Frontend.cpp` (`DirectInputCreateA` from `DInput.dll`); `DIK_` codes in `Player.cpp` (49), `Frontend.cpp` (18), `InputRecorder` (8), `Hud.cpp` (7), `Char_Pool.cpp` (3); `keybrd_0x204.cpp` | Built-in `DInput.dll`: `CreateDevice`, `EnumDevices`, `SetDataFormat`, `SetCooperativeLevel`, `Acquire`, `GetDeviceState` over SDL. A `SDL_Scancode` to `DIK_` table keeps key configs and replays. `GetKeyboardLayoutNameA` from the locale or the config. | M |
| 8 | Audio | `cSampleManager.cpp`, `Source/3rdParty/mss.cpp` | The 52 `AIL_*` imports (2D samples, 3D samples, streams) over OpenAL Soft. `cSampleManager` unchanged. | M |
| 9 | Movies | `Bink.cpp` (18 `Bink*` imports, blits to DirectDraw) | First `BinkOpen` returns 0 and the game skips movies. Then libavcodec's Bink decoder, frames uploaded as a GL texture. | S, then M |
| 10 | Files | `crt::fopen` in `crt_stubs.hpp` (single entry for game file I/O), `"data\\..."` literals, `_findfirst` (`Frontend`, `PlyDat_2BC0`, `InputRecorder`), `FindFirstFileA` (`Network_20324`), `GetFileAttributesA`, `GetDriveTypeA` (CD check), `_getcwd`/`_chdir` | Normalise in the shim: `\` to `/`, case-insensitive lookup in the data dir, `_findfirst` over `opendir` + `fnmatch`, CD check answers "no CD needed". | S |
| 11 | Config | `registry.cpp` (`HKCU\Software\DMA Design Ltd\GTA2\{Screen,Sound,Player,Option,Control,Debug}`), `Network_20324.cpp`, `cSampleManager.cpp` | The 6 `Reg*` functions over an ini file in `$XDG_CONFIG_HOME/gta2/`. QoL options go in the same file. | S |
| 12 | Network | `NetPlay.cpp` (DirectPlay 3 and lobby, via `CoCreateInstance` and `DPLAYX` ordinals 1 and 5), `Network_20324.cpp` (Win32 dialog lobby from `resources.rc`) | First disabled (single player). Then a transport over ENet behind `NetPlay` and a lobby in ImGui or the frontend. No compatibility with original clients. | L |
| 13 | Misc | `error.cpp`, `Frontend.cpp`, `winmain.cpp` | `MessageBoxA` to `SDL_ShowSimpleMessageBox`, `OutputDebugStringA` to stderr, `wsprintfA` to `vsnprintf`, `ShellExecuteA` ("GTA2 Manager.EXE", "WebLaunch.exe") no-op, `GetVersionExA` and `GetFileVersionInfo*` fixed values, `GetUserNameA` to `getlogin`, mutexes to a lock file, `CoInitialize` no-op, the DirectX version probe in `winmain.cpp` returns a fixed version. | S |
| 14 | Debug tools | `ImGuiDebug.cpp`, `dear_imgui` | ImGui SDL2 + OpenGL3 backends, without the HookLoader parts. | S |
| 15 | Replay harness | upstream `HookLoader`, `InputRecorder_67F8B0.cpp`, new `tests/replays/` in the fork | A hook DLL plays a replay in `10.5.exe` and dumps a hash of the game state each frame (pools of peds, cars, objects, player, RNG, mission state). The fork plays the same replay headless and compares. Reference hashes are stored in the fork, so CI never needs the original exe. | M |

## Milestones

**Phase A**

1. **M1 Fork and build.** Fork repo with an `upstream` remote, new CMake, VC6 tooling removed.
   MinGW i686 build of the same Win32 game. Done when it plays under Wine.
2. **M2 OpenGL renderer.** `gbh_*`/`Vid_*` over SDL2 + OpenGL as Windows DLLs, tested with
   `10.5.exe` and with the M1 build.
3. **M3 Native Linux, silent.** Shims (files, registry, timers, window, input, wchar),
   Miles and Bink stubs, no network. Done when a level plays on Linux.
4. **M4 Audio**, Miles over OpenAL.
5. **M5 Movies**, Bink over libavcodec.
6. **M6 Replay harness** (item 15), wired into CI on the Linux build.
7. **M7 Network**, ENet and a new lobby.

**Phase B**

8. **M8 Freeze.** Last merge from upstream; tag the base.
9. **M9 Modernisation.** Fold the shims into the game code, `char16_t`, 64-bit (audit
   pointer/int casts with `-Wpointer-to-int-cast -Wint-to-pointer-cast`, pointers in structs
   with `GTA2_ASSERT_SIZEOF` (17), save, replay and network formats), remove matching
   artefacts. Each step keeps the replay hashes.
10. **M10 Quality of life**, each as an option off by default: widescreen and high
    resolution, UI scaling, remapping and controllers, unlocked framerate (needs a fixed
    simulation tick with interpolated rendering, since the game advances one step per frame).
11. **M11 Packaging.** AppImage or Flatpak; Windows build from the same tree; macOS through
    SDL2, where OpenGL is deprecated (Vulkan via MoltenVK or SDL_GPU later).

## Verification

- **CI**: a Linux job (Ubuntu, gcc `-m32`, SDL2 i386), then a Windows job on the same tree.
- **Replay test**: vanilla mode (all options off) must reproduce the reference hashes of
  `10.5.exe` frame by frame. Rendering-only changes don't touch them; gameplay changes are
  checked with their option off. Game logic is mostly `Fix16` integer math; if float code
  diverges, try `-mfpmath=387` on the 32-bit build.
- **Short wchar** (phase A): `nm` the executable and check that no glibc `wcs*`/`swprintf`
  symbol is referenced (the linker won't warn).

## Risks

- Merges from upstream conflict as soon as the fork edits game code: keep phase A edits
  rare, listed, and upstream them when possible.
- Freezing too early loses upstream naming and structure work; freezing too late delays
  the refactor. Freeze when upstream changes are mostly renames.
- WndProc semantics (focus loss, Alt-Tab, fullscreen switch) are where window shims usually break.
- GTA2 data still comes from the user's install, never from the repo.

## Appendix: OS surface (imports of 10.5.exe)

Regenerate with `i686-w64-mingw32-objdump -p Scripts/bin_comp/10.5.exe`.

- **USER32 (43)**: window (`RegisterClassA`, `CreateWindowExA`, `DestroyWindow`, `DefWindowProcA`,
  `CallWindowProcA`, `ShowWindow`, `UpdateWindow`, `SetWindowPos`, `GetWindowRect`,
  `GetClientRect`, `SetForegroundWindow`, `FindWindowExA`, `GetSystemMetrics`, `ShowCursor`,
  `LoadIconA`, `LoadCursorA`, `GetDC`, `ReleaseDC`), messages (`PeekMessageA`,
  `TranslateMessage`, `DispatchMessageA`, `PostMessageA`, `SendMessageA`, `PostQuitMessage`,
  `SetTimer`, `KillTimer`), dialogs, network lobby only (`DialogBoxParamA`, `EndDialog`,
  `GetDlgItem`, `GetDlgCtrlID`, `GetParent`, `SetDlgItemTextA`, `GetDlgItemTextA`,
  `SendDlgItemMessageA`, `EnableWindow`, `GetWindowTextA`, `SetWindowTextA`,
  `GetWindowLongA`, `SetWindowLongA`), misc (`MessageBoxA`, `wsprintfA`, `wvsprintfA`,
  `GetKeyboardLayoutNameA`).
- **KERNEL32, game side**: `LoadLibraryA`, `GetProcAddress`, `FreeLibrary`, `FindFirstFileA`,
  `FindNextFileA`, `FindClose`, `GetFileAttributesA`, `GetDriveTypeA`,
  `GetPrivateProfileStringA`, `CreateMutexA`, `OpenMutexA`, `ReleaseMutex`, `CreateEventA`,
  `CloseHandle`, `GetTickCount`, `Sleep`, `GetVersionExA`, `GetLastError`, `FormatMessageA`,
  `LocalFree`, `MultiByteToWideChar`, `WideCharToMultiByte`, `lstrlenA`, `lstrcatA`,
  `OutputDebugStringA`. The rest is the statically linked CRT, replaced by libc.
- **ADVAPI32 (8)**: `RegOpenKeyA`, `RegOpenKeyExA`, `RegCreateKeyExA`, `RegQueryValueExA`,
  `RegSetValueExA`, `RegDeleteValueA`, `RegCloseKey`, `GetUserNameA`.
- **Others**: GDI32 `GetDeviceCaps`; SHELL32 `ShellExecuteA`; ole32 `CoInitialize`,
  `CoUninitialize`, `CoCreateInstance`; VERSION `GetFileVersionInfoSizeA`,
  `GetFileVersionInfoA`, `VerQueryValueA`; WINMM `timeGetTime`; DINPUT `DirectInputCreateA`;
  DPLAYX ordinals 1 and 5 (`DirectPlayCreate`, `DirectPlayLobbyCreateW`).
- **mss32 (52)**: `AIL_*` (startup and shutdown, `waveOut` open and close, 2D samples,
  3D providers and samples, streams, memory).
- **binkw32 (18)**: `BinkOpen`, `BinkOpenMiles`, `BinkClose`, `BinkDoFrame`, `BinkNextFrame`,
  `BinkWait`, `BinkCopyToBuffer`, `BinkGetSummary`, `BinkSetIOSize`, `BinkSetSoundSystem`,
  and the `BinkBuffer*` DirectDraw helpers.
- **Loaded at runtime**: the renderer named in the registry (`d3ddll.dll`, `softdll.dll`
  or `3dfx.dll`) and `dmavideo.dll`; `DInput.dll` (`input.cpp`); `DDRAW.DLL`, `DINPUT.DLL`
  and `DMUSIC.DLL` only to probe the DirectX version (`winmain.cpp`).
