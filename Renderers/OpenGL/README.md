# GTA2 OpenGL renderer (prototype)

Two drop-in DLLs that replace DMA's `d3ddll.dll` (gbh_* API) and `dmavideo.dll` (Vid_* API) with
OpenGL 3.3 core. They work with the decomp build (`decomp_main.exe`) and with the original exe:
the game picks its render and video DLLs from the registry, so no game code changes.

This directory is not part of the VC6 build and has no effect on matching.

By default the output follows DMA's Direct3D renderer: the draw calls it sends are identical to the
ones DMA's DLL sends to Direct3D, and the frontend is bit exact. The remaining in-game
differences are rasteriser rounding (see Fidelity). Optional enhancements are off unless enabled in
`gta2gl.ini`.

## Build

Needs MinGW-w64 (i686). `make test` needs a host g++ and the SDL2 development headers.

```bash
make                  # build/glvideo.dll build/glrender.dll build/capvideo.dll
make test             # native tests of the portable core
make OUT=/path/to/gta2
```

The DLLs link the MinGW runtime statically and only need system DLLs.

## Install and switch

Copy `glvideo.dll` and `glrender.dll` next to the game exe (keep DMA's DLLs), then set in
`HKLM\SOFTWARE\DMA Design Ltd\GTA2\Screen` (32-bit view):

| value | OpenGL | DMA's Direct3D |
|---|---|---|
| `rendername` | `glrender.dll` | `d3ddll.dll` |
| `videoname` | `glvideo.dll` | `dmavideo.dll` |
| `do_play_movie` | `0` (movies are not supported) | `1` |

From a command prompt run as administrator (under wine, in `wine cmd`); `/reg:32` also picks
`WOW6432Node` on 64-bit Windows:

```bat
set K="HKLM\SOFTWARE\DMA Design Ltd\GTA2\Screen"
reg add %K% /reg:32 /f /v rendername /d glrender.dll & reg add %K% /reg:32 /f /v videoname /d glvideo.dll & reg add %K% /reg:32 /f /v do_play_movie /t REG_DWORD /d 0
```

Back to DMA's renderer: the same with `d3ddll.dll`, `dmavideo.dll` and `do_play_movie` 1.

The resolution is the game's own setting, `full_width`/`full_height` in the same key (`start_mode`
1 is full screen, 0 windowed). Modes offered: 640x480, 800x600, 1024x768, 1280x960, 1280x1024,
1600x1200, 1440x1080, 1920x1080. Full screen is a borderless window over the monitor, the frame is
scaled with letterboxing. 1440x1080 keeps the original 4:3 framing; in 1920x1080 the unmodified
game shows less of the city vertically and the mission text goes off screen.

## Bonus: optional enhancements

Not in the original. All off by default; with nothing set the output is bit for bit the faithful
renderer's. Copy `gta2gl.ini.example` next to `glvideo.dll` as `gta2gl.ini`:

```ini
[graphics]
truecolor = 1            ; 8-bit palette colours, 32-bit target (the original is 16-bit only)
bloom = 0.5              ; 0..1, glow of additive draws (fire, explosions, lights)
per_pixel_lighting = 1   ; the level's lights evaluated per pixel with the original's formula
```

The file is re-read while the game runs. **Scroll Lock** switches between the faithful renderer and
these options. Environment variables override the file: `GTA2GL_TRUECOLOR`, `GTA2GL_BLOOM`,
`GTA2GL_PPL`. The code is in `core/gl_enhanced.cpp`, `core/options.*` and the places marked
"enhancement".

## Keyboard under wine

Under wine on XWayland, wine's lock key resync can be blocked by DirectInput's keyboard hook and is then
retried on every key, filling the keyboard buffer with NumLock or Scroll Lock pairs (Scroll Lock is on after
switching to the faithful renderer). GTA2 reads one buffered keyboard event per frame, so held keys kept
acting after release. `glvideo.dll` drops NumLock and Scroll Lock events from the game's keyboard reads
(`dll/dinput_hook.cpp`); `GTA2GL_KEEP_LOCK_KEYS=1` turns that off.

## Development tools

Used to compare with DMA's renderer; all inactive unless their variable is set.

| variable | effect |
|---|---|
| `GTA2GL_CAPTURE=<dir>,<frame>,<a>-<b>,...` | write those frames (Vid_FlipBuffers count) as BMP |
| `GTA2GL_CMDLOG=<dir>,<frames>` | log every gbh call and the primitives it produced |
| `GTA2GL_INPUT=<frame>:<dik>[:<held>],...` | feed DirectInput key events at fixed frames |
| `GTA2GL_TOGGLE_AT=<frame>,...` | press the Scroll Lock switch at those frames |
| `GTA2GL_KEYLOG=<file>` | log every keyboard event the game reads and the backlog left |
| `GTA2GL_STATS=<n>` | log fps, draw calls, uploads and frame times every n frames |

`capvideo.dll` is a proxy for DMA's `Dmavideo.dll` (register it as `videoname`, keep
`rendername = d3ddll.dll`): the same variables give reference frames and the D3D calls of DMA's
DLL in the same log format. Frames are deterministic when booting straight into a level with the
debug keys (`skip_frontend`, `mapname`...), so the two runs can be compared frame by frame with
`tools/compare_frames.py` and the logs with `diff`.

## Layout

- `core/` portable C++ and OpenGL, no Windows headers. `gbh_api.cpp` is the gbh_* layer,
  `gl_renderer.*` the faithful renderer, `gl_enhanced.cpp` the optional enhancements,
  `vid_system.*` the Vid_* logic, `renderer.hpp` the interface between the two DLLs.
- `platform/` the GL context: WGL on the game's window for the DLLs, SDL2 for a future native port.
- `dll/` Windows glue: Vid_* exports, window handling, the DirectInput hook.
- `tools/` development tools, `tests/` native tests.

The video DLL owns the window, the GL context and the renderer. The game passes the SVideo it got
from `Vid_Init_SYS` to `gbh_InitDLL`; that SVideo heads a larger object carrying an `IRenderer*`,
which is how the render DLL reaches the renderer.

## Fidelity

Behaviour taken from DMA's `d3ddll.dll` (disassembled) where GTA2Hax differs:

- blending: 0x200 is ONE/ONE, 0x80/0x100 blend with the filtered texture alpha, no alpha test;
- 16-bit back buffer, RGB565 textures, ARGB4444 for textures drawn with alpha (DMA picks a 4-bit
  alpha format first), 8..256 texel cache slots with the original's UV normalisation;
- off-screen rejection, fit-to-texture quads (`w - 0.001`), exact image blits;
- the lighting's table driven square root, and the x87 control word DMA's DLL sets for the whole
  process (single precision, round down);
- D3D pixel centres: coverage at the centre + 1/2048 px, texture sampling at the centre - 1/1024 px.

Against captures of DMA's DLLs under wine, over 40-frame sequences at two points per city:

| sequence | MAE | bad% | exact% |
|---|---|---|---|
| city 1 | 0.28 | 0.01 | 82.9 |
| city 2 | 0.32 | 0.02 | 80.3 |
| city 3 | 0.14 | 0.03 | 91.4 |
| frontend | 0 | 0 | 100 |

Every remaining bad pixel samples exactly on a texel boundary or lies exactly on a triangle edge,
where the result depends on the rasteriser's internal rounding. The non exact pixels are one step
of a 5/6-bit channel.
