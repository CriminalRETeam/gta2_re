# Bot: design and implementation plan

A bot that plays GTA2 on its own, headless, for coverage and regression of the decompiled game.
It is given a run profile (reach the level exit, every mission of every gang, every bonus, with or
without dying) and works out the rest: which phone to answer, how to get there, what the mission
wants, when to give up on a plan. It plans with GOAP (goal-oriented action planning, as F.E.A.R.
does) and, unlike F.E.A.R., can test a plan step for real before committing to it, because the
whole game state can be snapshotted and put back.

This is the GTA2 counterpart of the ALIVE bot design (`alive_reversing`,
`Source/relive_lib/Bot/BOT_DESIGN.md`). The layers are the same; what differs is forced by this
codebase: the game is a matching decompilation, so **no matched function can change**, there is no
mid-level save to build a snapshot on, and the game already has a deterministic input replay
system that the bot should produce and consume rather than reinvent.

## What the codebase gives us (verified unless marked)

- **Deterministic logic.** Game logic runs in fixed 33 ms frames (`Game_0x40::GetFrameDurationMs`),
  gated by `timeGetTime` only for pacing (`RunGameFrame_4DA850`). The game's own replay system
  (`InputRecorder_67F8B0`) records input changes keyed by the RNG index and plays them back with a
  sync check (`ReplaySyncError`, `bSkip_replay_sync_check`), which is the original developers'
  evidence that logic is a pure function of inputs and the RNG. The rng (`rng.hpp`) is two
  words of state.
- **Unthrottled headless.** `bSkip_draw_67D4EA` makes `RunGameFrame` run a logic frame on every
  loop iteration and never draw; `bSkip_audio_67D6BE` skips sound. Both are plain globals set from
  registry debug settings (`Debug::Init_4ABBD0`). `bSkip_frontend_67D53B` skips the menus.
  *To verify:* starting straight into a chosen map/script without the frontend (what
  `InitializeGame_4DA4D0` needs in `gGameSession_67E8E0`), and running under xvfb+wine with
  the D3D9 `d3ddll` never asked to draw (there is a `dma_video` software path if it won't start).
- **An input seam.** Every frame `do_network_and_local_inputs_4DAF30` calls
  `InputRecorder_67F8B0::get_input_bits_4CEAC0`, which reads the devices through
  `InputDevices_1::read_input_device_498DA0(s32* input_bits, u8)` and records the result as a
  replay in `Live_0` state. The player code then reads `gInputRecorder_67F8B0.field_4_input_bits`
  through the 12 `field_8_input_masks`. *To verify:* `Live_0` reads through
  `read_input_device_498DA0` as `Playback_1` does.
- **Detours in tree.** `Source/HookLoader.cpp` already builds Microsoft Detours (x86) and swaps
  functions at runtime. A detour patches compiled code at startup; it changes no source, so the
  match count is untouched.
- **Non-original code has a home.** `ImGuiDebug.cpp`, `LoadBeginSceneCBPtr` in `winmain.cpp`,
  `WinMainProxy.cpp`/`test.cpp` (our entry points) show the pattern: separate TUs, called before
  `WinMain_5E53F0`, reading globals and calling `EXPORT` methods directly.
- **World data and the game's own path finders.** `Map_0x370` (blocks, slopes, road arrows,
  zones by type, gang at coords), `RouteFinder` (road junction graph + A* the car AI uses),
  `PathFinder_2FD4` (the ped grid walker), the ped/car pools, `Player` (score, lives, wanted
  level, gang respect, weapons), `ScriptManager_C1EA8` (phone triggers, mission flag, counters,
  briefs shown via `Hud_BriefEntry_18`'s gxt keys), the HUD arrow target.
- **Pool allocation.** The pools (`PedPool`, `Car_BC_Pool`, `SpritePool`, script pools...) are
  allocated once at game start (`Game_0x40::Game_0x40` and friends) and reused. *To verify:* no
  `new`/`malloc` happens during play (124 sites, all look init-time; phase 0 checks by logging).
- **Symbols for every byte.** `build_vc6/output.map` names every global, which turns a raw memory
  diff into a list of global names.

## Principles

Same as the ALIVE bot, plus two that this codebase forces:

- **Never touch a matched function.** The bot is non-original code in its own TUs. It reaches the
  game through globals, `EXPORT` methods and two Detours. If something needs a seam the game
  doesn't have, add a detour, not a line in a `MATCH_FUNC`.
- **The game's replay is the bot's output.** A run produces a `.rep` the unmodified game (and
  the original `10.5.exe`) can play back with its own sync check. That is the regression artefact
  and the cross-check that the decompiled logic still matches the original's.

And the shared ones: simulate rather than guess (snapshot, try, look, restore), white box, own
RNG only for ties, fails loudly with a cheap text view and a repro.

## Architecture

```
 Profile / Goals        all_missions,no_death,seed=7  ->  prioritised goals
 Planner (GOAP A*)      goal  ->  [GoTo(phone#3), AnswerPhone, FollowArrow, EnterCar(#12), ...]
 Executors              one state machine per action, input bits per frame, retry via snapshots
 Navigation             foot: block grid + PathFinder; road: RouteFinder junction graph; driving
 WorldModel             observe pools/script/player -> entities, facts; the cheap text view
 Snapshot               writable PE sections + heap blocks, hash, diff by symbol
 Hooks                  Detours on ExecuteGame_4DA780 (frame) and read_input_device_498DA0 (input)
```

### Hooks

Two detours, attached in the entry point (`GameMain`/`test.cpp`) when `-bot` is on the command
line, before `WinMain_5E53F0`:

- `ExecuteGame_4DA780` (plain `EXPORT` function, called once per logic frame right after the
  inputs are read): the detour runs `Bot::Step()` and then the trampoline. This is the only place
  the bot thinks, observes, snapshots or restores. Nothing is mid-frame here, so a restore is a
  plain memory copy.
- `InputDevices_1::read_input_device_498DA0`: the detour writes the bot's bits into
  `*input_bits` instead of reading DirectInput. The game then records them into its replay as if
  a person had pressed the keys, so `test\replay.rep` (or wherever `SaveReplay_4CDED0` writes) is
  the run's input log, for free, in a format the stock game plays back. `__thiscall` is detoured
  with the usual `__fastcall(this, edx, ...)` thunk.

No other game code changes. If the bot needs a third seam later (a mission event, a brief being
shown), it gets a third detour.

### Snapshot

There is no mid-level save, so the snapshot is the process state itself:

- The writable sections of the exe module (`.data`, `.bss`, from the PE section headers of
  `GetModuleHandle(NULL)`): every `DEFINE_GLOBAL` lives there.
- Every block of the CRT heap (`HeapWalk` on `_crtheap`): the pools and everything else `new`ed
  at init. Pointers stay valid across a restore because the blocks are never freed during play
  (phase 0 verifies).
- Not the stack (the hook runs at a fixed depth with nothing live) and not the bot's own memory.

Minus an **exclusion list** of ranges that must survive a restore: DirectX/video (`gVidSys`,
`d3ddll` state), sound (`gRoot_sound`, Miles handles), input devices, `gFpsCounter`, the frame
pacing globals (`gMatchStartTime_6F5A28`, `gFramesSinceDraw`, `bInputsPending`), and the replay
recorder's buffer (the bot keeps its own view of what it committed). The list is data, named by
symbol, and grows as `restore_check` finds things.

The bot keeps itself out of the snapshot by having one global (a pointer in the exclusion list)
to a `BotState` in a private Win32 heap (`HeapCreate`); bot classes allocate from it through a
class-scope `operator new`. The bot's memory survives a restore; the world rewinds.

A **restore_check** (the ALIVE tool, same semantics): hash all snapshotted memory at frame N,
run k frames, hash; restore N, check the hash is N's, run k frames, check the hash is the second
one. A mismatch means something that matters is outside the snapshot or in the exclusion list.
The diff tool maps differing bytes to names via `output.map` and prints `<global> <offset>: a vs b`.

Snapshots are per build (addresses); the `.rep` is the durable artefact. A snapshot is ~ the
size of `.data` + heap (tens of MB); a restore is a memcpy, so a run can afford thousands.

### WorldModel

`Observe()` every frame the bot is awake, producing:

- **Entities** keyed by pool index (stable while the object lives) and kind: Player ped, Peds
  (occupation, objective, gang, weapon, health, in car), Cars (model, driver, damage, locked,
  mission car), Phones (ringing, which mission thread they trigger via `field_188_phone_triggers`),
  Objects (weapons, bonuses, kill frenzy tokens), Zones (gang, navigation, arrow blocker), the HUD
  arrow target, Police (wanted level, cop peds/cars near), current brief (gxt key + text).
- **Facts** for the planner: `AtPhone(id)`, `OnMission(id)`, `MissionDone(id)`, `Respect(gang)>=n`,
  `InCar(id)`, `CarAt(id, zone)`, `WantedLevel(n)`, `Score>=exit`, `BonusCollected(id)`,
  `Alive`, `Lives(n)`. Taken from the game's own counters and script state, never the bot's
  bookkeeping.

#### The cheap view

```
bot f=4812 wil residential profile=all_missions,no_death seed=7 deaths=0 restores=31 budget=40%
goal Mission(zaibatsu#3) plan=[GoTo phone#9, AnswerPhone, FollowArrow, EnterCar, DriveTo zone] step=3
brief "2_14" "Steal the Zaibatsu car from the compound and bring it to the garage"
player x=112.5 y=44.2 z=2 hp=25 lives=3 wanted=1 cash=18400 respect z=+2 l=-1 y=0 oncar=0 weapon=pistol
arrow -> car#44 x=140.0 y=51.0 (dist 28)
car#44 zaibatsu_z x=140.0 y=51.0 locked=0 driver=none mission=1
ped#17 cop x=118 y=46 objective=arrest target=player
ped#31 zaibatsu x=139 y=50 objective=guard weapon=smg
phone#9 x=110 y=44 ringing=0 answered=1
nav foot: at block (112,44,2) -> (140,51) 31 steps via (120,44)(132,48); road: j14 -> j22 -> j31
blocked: EnterCar(#44) tried x3: shot by ped#31 (f+40, f+38, f+61)
```

Under 40 lines, integers, no sprite or animation data.

### Navigation

Three modes, each with the game doing the heavy lifting:

- **On foot**: block grid from `Map_0x370::GetBlock`, walkable = pavement/road/slope rules the
  ped AI uses; `PathFinder_2FD4::ComputePath` for the route, the bot's own executor turns steps
  into direction bits (8 directions + run). Hazards as attributes: roads (cars), gang zones with
  low respect, cop proximity.
- **Driving**: `RouteFinder::InitSearch_589480` from the current block to the destination block
  gives the junction path the car AI would take; the bot's driving executor follows it with
  steer/accelerate/brake bits and the road arrows for lane direction. The physics is the hard
  part; the retry search tunes it (approach speed, when to brake) instead of a perfect model.
- **Transitions**: `EnterCar`, `ExitCar`, `StealCar(nearest)`. Cars are the default for distance
  > N blocks when one is reachable.

A `-bot_nav_dump` writes the block grid around the player as text, and the junction path.

### Planner (GOAP)

As in the ALIVE design: `WorldState` of `(FactKey{kind, id}, value)`, action templates
instantiated per entity each plan, regressive A*, cost in frames (foot/road distance from the nav
layer), replan on action end/failure/fact change/every N frames, own RNG for ties.

GTA2 actions: `GoTo(place)` (foot or car, planner picks by cost), `AnswerPhone(id)`,
`FollowArrow` (the mission's current target, whatever it is), `EnterCar(id)`, `StealCar`,
`DriveTo(zone)`, `DeliverCar(zone)`, `Kill(ped)`, `KillGroup(zone)`, `PickUp(object)`,
`BuyAt(shop)`, `LoseWanted` (pay'n'spray / cop bribe / wait), `RaiseRespect(gang)` (kill rivals),
`Wait(n)`, `Die` (profiles that allow it), `StartKillFrenzy(id)`, `ExitLevel`.

Goals from the profile: `StayAlive` (always), `StayUnarrested` with `no_death`,
`Mission(gang, n)` per phone, `Bonus(id)`, `Score>=exit` then `ExitLevel`, `Explore` as the
fallback when nothing has a plan.

Missions are the long pole. GTA2 tells the player what to do through briefs (text) and the HUD
arrow (a target). The bot's generic mission handling is: answer the phone, then alternate
`FollowArrow` with the action the arrow's target suggests (a car: enter it; a ped: kill or
follow it; a zone: go there; nothing: wait) until the mission flag clears, reading the outcome
from the script (completed/failed brief, cash). That generic loop plus the retry search is
expected to clear the simple missions. For the rest there is a **mission knowledge** table keyed
by (map, phone/mission id) holding sub-goal sequences, filled first by what the bot learns (which
arrow targets came in which order on a successful try) and, only where the stuck report shows the
generic loop can't do it, by a hand-written hint. Hints are data in `bot_runs/`, not code.

### Executors

One class per action, `Start/Step/Abort`, `Step` once per logic frame from the hook, setting the
input bits for the next frame. Each watches the state it expects to change (ped objective, car
driver, mission flag) rather than elapsed time, and has a frame budget and a no-progress check.
Retry is generic: a risky step (near cops, gang zones, a mission car, any driving) is run under a
snapshot; on failure or death the bot restores and picks the next variant (RNG-ordered: wait,
different car, approach from the other side, on foot instead), up to the action's retry count.
With `no_death`, dying inside a rewound try is fine; only the committed run is death-free.

### Determinism and replay

- The committed run is exactly the `.rep` the game recorded. Replaying it in the stock game
  (`play_replay`) must reach the same end state: that is the regression test, and it also runs on
  the original `10.5.exe`, which makes it a logic-match test between the original and the decomp.
- `hash_log`: the snapshot hash per frame, to find the first frame two runs diverge.
- `BotRng` seeded from `-bot_seed`; the game's rng is never touched. No wall clock in the bot;
  budgets in frames and restores.

### Stuck detection and the report

The same four conditions (`no_plan`, `action_failed`, `no_progress`, `budget`) and the same
outputs: `bot_report.txt` (condition, cheap view, last 50 decisions, nearest miss for `no_plan`),
`bot_stuck.snap` (the snapshot at the start of the failing action, for same-build repro),
`bot_run.rep` (the committed inputs), `bot_log.txt`. Repro: `-bot=<profile> -bot_seed=N
-bot_load=bot_stuck.snap`, seconds. A challenge is named (map, mission/goal) so it can be tracked.

## Integration

- Command line (parsed by the entry point, not by `WinMain_5E53F0`): `-bot=<profile>`,
  `-bot_seed=N`, `-bot_map=<map> -bot_script=<scr>`, `-bot_out=<dir>`, `-bot_budget=<frames>`,
  `-bot_load=<snap>`, `-bot_nav_dump`, `-bot_hash_log`. `-bot` implies `bSkip_draw`,
  `bSkip_audio`, `bSkip_frontend`.
- Build: `Source/Bot/` is a static library (`gta2_bot`) compiled with VC6 like the rest, linked
  into `decomp_main` only. It includes the game headers directly (as `ImGuiDebug.cpp` does) and
  the Detours sources `HookLoader` already builds. Keep the C++ VC6-friendly: no templates beyond
  what the STL in `HookLoader.cpp` already proves works, no C++11.
- Tests: `Scripts/bot/` holds Python drivers (the repo's test style): `restore_check.py`,
  `replay_check.py` (bot run, then stock playback, compare hash logs), `bot_runs/` with a script
  per (map, profile) and its expected end state. They run under xvfb+wine like CI's build.

## Code layout

```
Source/Bot/
  Bot.hpp/.cpp             the hook target: profile, budgets, goal loop, retry, logs, report
  BotOptions.hpp/.cpp      command line and profile parsing
  BotHeap.hpp/.cpp         private heap, class-scope operator new base
  BotRng.hpp
  Hooks.hpp/.cpp           the two detours and their thunks
  Snapshot.hpp/.cpp        sections + heap walk, exclusion list, hash, restore, diff by symbol
  WorldModel.hpp/.cpp      Observe, entities, facts, Text()
  NavFoot.hpp/.cpp         block grid + PathFinder wrapper
  NavRoad.hpp/.cpp         RouteFinder wrapper, junction path
  Driving.hpp/.cpp         the driving controller
  Goap/WorldState, Action, Planner, Goals
  Actions/*.cpp            one executor per action
  Missions.hpp/.cpp        generic arrow-follow loop + knowledge table
  BotLog.hpp/.cpp          decisions, report
Scripts/bot/               drivers, restore_check, replay_check, bot_runs/
```

## Implementation plan

### Phase 0: seams and prerequisites

1. `-bot` command line in the entry point; `bSkip_draw/audio/frontend`; boot straight into a
   map + script (find what `gGameSession` needs). Check: the game runs a level headless under
   xvfb+wine at thousands of frames a second.
2. The two detours. Check: a hard-coded "hold forward for 100 frames" moves the player, and the
   game writes a `.rep` that plays back in the stock game to the same position.
3. `Snapshot`: sections + heap walk, hash, restore, `restore_check` driver, diff by `output.map`
   symbol. Log any `new`/`malloc` during play (a detour on the CRT allocator for the check).
   Check: `restore_check` passes over 300 frames of traffic and peds with nothing excluded but the
   known device/timing ranges; the exclusion list is written down with the reason for each entry.

### Phase 1: skeleton end to end

`Bot`, `BotOptions`, `BotHeap`, `BotRng`, `WorldModel` with every entity kind and `Text()`,
`BotLog`, the stuck report, `-bot_load`. One hard-coded behaviour: walk towards the arrow target;
report `action_failed` when blocked.

Check: a report with the cheap view and a `.snap`; `-bot_load` reproduces the view; the `.rep`
replays to the same `hash_log`.

### Phase 2: navigation

`NavFoot` (grid, `PathFinder`, 8-direction executor, slopes, roads as hazards), `NavRoad`
(`RouteFinder` junction path), `Driving` (follow a junction path; retry-tuned parameters),
`EnterCar`/`ExitCar`/`StealCar`. Generic retry with snapshots.

Check: `GoTo` on foot between two far blocks in Downtown; `GoTo` by car across the map; a nav dump
eyeballed; a crash during a driving variant is rewound and the next variant tried.

### Phase 3: planner and the `any` profile

`WorldState`, action templates, regressive A*, goal selection, replan triggers, `Explore`.
Actions: `GoTo`, `PickUp`, `Kill`, `StartKillFrenzy`, `LoseWanted`, `ExitLevel`. Profile `any`:
reach the exit score by frenzies and bonuses, then exit.

Check: planner unit tests (toy states, nearest miss, deterministic ties); `any` on Downtown
reaches `AreaCompleted_4`; the `.rep` replays; a `no_plan` report when the exit is made
unreachable names the missing fact.

### Phase 4: missions

`AnswerPhone`, `FollowArrow`, `DeliverCar`, `KillGroup`, `RaiseRespect`, the generic mission loop,
mission outcome from script state, the knowledge table and learning. Profile `all_missions`
with respect ordering as a planner cost.

Check: every mission of one gang in Downtown; the stuck reports for the others each become a
tracked challenge; `no_death` variant replays death-free.

### Phase 5: hazards and the rest of the profiles

Cops and wanted levels, gang zones, trains and buses, kill frenzies as `all_bonuses`,
`max_score`, `Die` where allowed, pay'n'spray and shops.

Check: `all_missions` across Downtown; the remaining stuck reports listed.

### Phase 6: whole-game runs and regression

Residential and Industrial, the level transition, `bot_runs/` with per-(map, profile) scripts,
a replay-only mode (fast regression: play the stored `.rep` in the decomp and check the hash
log and end state) and a re-solve mode. Run the stored `.rep` through the original `10.5.exe`
as the match test.

## Status

### Phase 0: done (branch `bot`)

What is in the tree: `Source/Bot/` (`gta2_bot` static library, linked into `decomp_main` only;
`test.cpp` calls `Bot_Init(lpCmdLine)` before `WinMain_5E53F0`), `Scripts/bot/` (drivers). Run from
the GTA2 install dir (`$GTA2_ROOT`, the dir `build.py` copies the exe to):

```bash
cd Scripts/bot && export GTA2_ROOT=/path/to/GTA2
python3 run_bot.py --profile walk_test --map wil     # any profile; logs in $GTA2_ROOT/bot_out
python3 replay_check.py                              # walk_test, then the game plays the .rep back
python3 restore_check.py [--perturb]                 # snapshot/restore check, --perturb must FAIL
```

Measured on Downtown (`wil`) under wine 8 on the desktop display:

- Headless boot straight into the level, no frontend, no drawing, no sound: about 2 s to load,
  then **~10,000 logic frames/s** (700 frames in 69 ms).
- Input seam: `walk_test` holds forward for 100 frames, the player moves, the game writes
  `test\replay.rep` (header + 2 records) at shutdown, and the same build plays it back
  (`-bot=replay`) to an identical per-frame trace of rng index and player position.
- Snapshot: 11.7 MB (`.data` 0.9 MB + 186 CRT heap blocks, 10.8 MB), restore well under 1 ms,
  `restore_check` passes over 300 frames with 12 exclusions. `--perturb` (one byte of player
  health changed after the restore) turns it red and the report names
  `heap block ... <- gPedPool_6787B8`, so the check is live. No allocation happens during play
  (the frees logged at exit are the game's teardown).

Decisions that differ from the plan above, and why:

- **Three detours, not two.** The boot overrides (skip frontend/draw/audio, map, style, script,
  replay recording) are applied in a detour on `ParseCommandLine_4DA320`, which `WinMain_5E53F0`
  calls right after `Debug::Init_4ABBD0` and `GameSession::LoadDebugSettings_4C53D0` have read the
  registry. Writing the registry instead would change the user's normal runs.
- **CRT allocator hooks instead of `HeapWalk`.** VC6's small-block heap keeps blocks under 1 KB in
  `VirtualAlloc`'d regions that no heap walk sees, so `HeapTracker` detours `malloc`, `calloc`,
  `realloc` and `free` and keeps the block table itself. The heap's own metadata is not
  snapshotted; the CRT variables that point into it (`_crtheap`, `__sbh_*`) are on the exclusion
  list for that reason, together with `_iob` (the game's `error.log` `FILE`) and the two wall
  clock ranges (`gMatchStartTime_6F5A28`, the `FpsCounter` block). The list with reasons is
  `DEFAULT_EXCLUSIONS` in `Scripts/bot/botlib.py`, resolved against `output.map` into
  `bot_out/bot_exclude.txt`.
- **The replay recorder is inside the snapshot.** Rewinding `gInputRecorder_67F8B0` with the world
  keeps the recorded `.rep` equal to the committed run with no bookkeeping in the bot.
- **The bot never uses CRT stdio or `malloc`.** Its files go through Win32 handles and its memory
  through a private heap, since the CRT's state is part of what a restore rewinds.
- The caller logged for an allocation during play is the `crt::malloc`/`crt::free` wrapper, not
  the game function; walk one more frame when that matters.
- `wine explorer /desktop=...` never started the exe here; the drivers run `wine decomp_main.exe`
  directly (a small window appears on the desktop). Xvfb is not installed on this machine, so
  the CI style `xvfb-run` path is untested. The process sometimes segfaults inside wine during
  teardown, after the replay was written; the drivers ignore the exit code.

### Phase 1: done

`WorldModel` (`Observe()` copies plain numbers out of the game pools, `Text()` is the cheap view),
`BotRng`, the decision ring, the stuck report with `bot_stuck.snap`, `-bot_load`, and the
`walk_arrow` profile: walk to the HUD arrow target, or to `-bot_goto=x,y` when there is none.

```bash
python3 run_bot.py --profile walk_arrow --budget 1500 -- -bot_goto=162,141   # PASS, arrived
python3 run_bot.py --profile walk_arrow --budget 1500 -- -bot_goto=170,150   # FAIL no_progress
```

- The controller calibrates itself: it walks 10 frames, turns right 8, walks again, and fits
  `heading = sign * rotation + offset` from the two moves. On foot the ped's rotation runs the
  opposite way to the world heading (`heading = -rot + 180`, rot 180 is up the map), which a
  single move could not show.
- Failure conditions are `no_progress` (the distance did not shrink by half a tile in 150
  frames) and `no_plan` (no arrow and no goto). The second goto above is a real wall near
  (164,140); the report holds the cheap view, the last decisions and the path of the snapshot.
- `bot_stuck.snap` is the state at the start of the failing action. Loading it in a fresh process
  (`-bot_load`) resumes at the same rng and player position as the walk trace shows for that frame.
- The recorded `.rep` of the arrival run plays back to the same per-frame trace. The `hash_log`
  comparison of the plan was not done: the recorder state (live vs playback) is inside the hashed
  memory and differs by design, so the trace (rng, player position) is the replay check.
- Not yet exercised: the arrow branch of the walk (no mission arrow is active at the start of
  Downtown). The `WorldModel` arrow, phone, brief and gang fields are read but only the player,
  ped and car lines have been looked at against live data.

### Phases 2 to 6: what exists and what it does

All of it is in `Source/Bot/` and `Scripts/bot/`; the match count is unchanged (3430/3430).

**Running.** `Scripts/bot/botlib.py` starts a private Xvfb (`:99`, found on `PATH`, `$BOT_XVFB` or
`~/.local/bin/Xvfb`; `BOT_USE_DISPLAY=1` uses the session display). A locked X session blocks window
creation, and wine 8's null driver cannot create windows at all, so the game needs a real X server.
The command line is padded to a fixed length and the environment is fixed: the CRT mallocs both before
the allocator hooks, and their sizes decide where every later heap block lands, which a `.snap` needs.

| Layer | Files | State |
|---|---|---|
| WorldModel | `WorldModel` | players, peds, cars (speed), objects (tokens, frenzy icons, phones), the HUD arrow, script flags, passed/failed counters, the active bonus rule, brief text, tracked ped |
| Foot navigation | `NavFoot` | A* over (x,y,z) tiles whose edges are the game's own `PathFinder::TestMoveToAdjacentTile`; bans; closest reachable tile when the target is not walkable (a phone on a wall) |
| Driving navigation | `NavCar` | A* over drivable tiles (roads preferred, lane arrows respected, clearance, no corner cutting), string pulled. The `RouteFinder` junction path was tried first and dropped: it needs both ends on arrow blocks and a parked car starts in a walled yard |
| Planner | `Goap` | Dijkstra over (done mask, location), goal counts per kind, seed-driven tie break, replanned after every action, failed entities banned |
| Actions | `WalkAction`, `EnterCarAction`, `ExitCarAction`, `DriveAction`, `PickUpAction`, `KillFrenzyAction`, `KillPedAction`, `DestroyCarAction`, `PhoneAction`, `MissionAction` | one state machine each (`Start/Step`, `Fail(condition, detail)`) |
| Reflexes | `Bot::SelfDefense`, traffic wait and car yielding in `WalkAction`, obstacle braking in `DriveAction` | |
| Retry | snapshot at every action start; a walk takes rolling checkpoints; a mission takes one per step | restore, change something (ban a tile, a wait, a speed cap, another target), try again |
| Profiles | `walk_arrow`, `drive_test`, `collect`, `phone_test`, `restore_check`, `replay`, `idle`, `walk_test` | |
| Regression | `Scripts/bot/regress.py`, `bot_runs/runs.json` | each entry: result, condition, and the stock-game replay of its `.rep` compared per frame |

**Results on Downtown (`wil`).**

- Foot trips of 20 to 130 tiles, including detours around walls, road crossings with traffic, and
  trips that needed several snapshot retries. Parked car: walk to it, enter, drive 28 to 90 tiles
  (short drives work; long ones can still get stuck on building corners or take collision damage, no success rate was measured).
- **Secret tokens:** 6 collected in one session (the planner orders them by walking cost).
- **Kill frenzies:** two of the 16 pass, by the script's own passed counter: 25 kills with the
  silenced SMG (icon 413) and 10 kills with the flamethrower (icon 417). A single session
  collected 6 tokens and passed both (26,651 frames, one failed frenzy attempt rewound and
  retried), and its recording replays frame-identically in the stock game (19,982 committed frames).
- **Missions:** none completed. The mission loop gets a long way: it answers the phone, reads the
  brief, and follows the arrow wherever it points (a place on foot or by car, a car to get in, a
  pedestrian to kill, an object to walk onto, "sound the horn", "destroy that car"). It reads
  mission success from the script's passed counter. The three Loonies missions stop at:
  - Johnny Zoo / Doctor ZitZaki (phone 220): walk 50 tiles, take the marked car, drive to the
    hospital, honk, then the script waits for something the bot does not know (5000 frames idle).
  - Johnny Zoo / money transfer (phone 221): the drive to the City Bank works; the two bank guards
    kill the player.
  - Elmo / Pizza Cake (phone 226): explosives picked up and planted, then the delivery truck has to
    be destroyed and it drives off through traffic faster than a pedestrian can follow.
  - Elmo / Zaibatsu drug (phone 225): a crowd of crazed pedestrians kills the player on the way.
  The other phones refuse until gang respect is high enough ("kill some Zaibatsu before you return
  for work"); raising respect is the missing `RaiseRespect` action.

**What the bot learned the hard way** (found by experiment; the code has them as comments too):

- A held attack button stops firing: pulse it (6 frames on, 2 off).
- While the attack button is held the game turns the ped slowly; line up first, fire second.
- Thrown weapons (molotov, grenade) fly when the button is let go, the range is the time held.
- A rocket bursts on the first thing in its path: a pedestrian two tiles away burns the player.
- A kill frenzy counts only what its `BonusRule` says (kind of ped or car, weapon, count), and some
  rules cancel on a mismatch; the HUD counter is not the progress.
- Pedestrians three tiles away can be behind a fence: compare the walking path with the line.
- The phone and pickup objects sit on building tiles; walk to the closest reachable tile.
- Cars and pedestrians at a crossing: wait on the pavement, never walk into a moving car.
- Rotation and heading run opposite ways on foot and in cars (`heading = 180 - rotation`), both
  measured by the calibrator and identical.

**Not done** (tracked as `regress.py` entries with `expect: FAIL`, so a regression in them shows):
phase 5's hazards beyond the reflexes above (wanted level handling, gang zones, trains, buses,
pay'n'spray, shops), the `RaiseRespect` action, driving that avoids all collisions, vehicle chases,
the other 14 kill frenzies (rockets and cop kills in particular), the other maps, a re-solve mode,
and the run through the original `10.5.exe` (the `.rep` files are in `bot_out/regress/*/bot_run.rep`
and play in the stock `decomp_main.exe`; the original exe was not tried).

### Goal tree (added after phases 0 to 6)

Phases 2 to 6 used a flat list of actions and one GOAP plan for tokens and frenzies. The mission
work needed nesting, so a goal is now an `Action` that owns child actions (`Source/Bot/Goals.*`,
`ChaseActions.*`). A node can be a leaf (walk) or a whole subtree, and each child starts under a
snapshot of its own, so a child's retries rewind to its start and not to the start of the tree.

```
MissionsGoal              root: which phone next. Tier (green, amber, red), distance, what it has learned
  MissionGoal             one phone
    PrepareGoal           health, armour, a better weapon: GOAP over the pickups in reach, then select the gun
    TravelGoal            on foot or by car, whichever the cost estimate says; takes a parked car, or a hijack
                          when a car waits beside us; falls back to walking
    PhoneAction           refusal = a brief but no mission within 200 frames
    MissionAction         follows the arrow; makes children for each arrow
      SequenceGoal(PrepareGoal, KillPedAction)         a ped arrow
      DestroyCarGoal      shoot it or ram it, by cost; tries the other way if one fails
        RamCarAction      aims where the target will be, hand brake round corners, stops when it stops or burns
      SequenceGoal(DriveAction, ParkAction)            a place arrow in a car: park on the spot
  raise_respect           when a phone refuses: PrepareGoal, then KillGangGoal (whom: the brief, or the game's kill table)
```

- **Costed alternatives.** `DestroyCarGoal` estimates frames for each way (walk to it and shoot, with
  a penalty for a weak gun or a fast target; or reach a fast car and ram, cheaper when the target is
  already damaged), picks the cheaper, and after a failure decides again with that way struck out.
  Entering a car to ram is therefore only chosen when it beats shooting, and a car that is too
  damaged (burning at 16000) ends the ram and sends the goal back to Decide, which picks another car.
- **Failure is local.** A failed child restores its own snapshot and the parent picks something else
  (`MissionsGoal` bans the phone for a while; `KillGangGoal` bans a ped or a zone). A refusal is not a
  failure: the world is left as it is and the bot does what the brief says first.
- **Respect.** The refusal brief names the gang to kill ("kill some Zaibatsu before you return for
  work"); when it does not, the kill reaction table of the gangs (`Gang::field_122_gang_kill_reaction`)
  says whose death raises the phone owner's respect. The owner is the gang whose map zone holds the phone.
  The number of kills asked doubles after each completed attempt. Respect is compared as a vector:
  one kill moves the victim's gang and its enemies in opposite directions, so the sum stays near zero.
- **Cars.** `NavCar` is an A* over drivable tiles with lane arrows, one tile of clearance and the
  walls of block faces (face flag 0x400). A target that cannot be driven to (a phone on the pavement
  beside a wall) ends the route at the nearest tile the car can reach. A drive that gets stuck bans the
  tiles ahead of the car and replans. `ParkAction` puts the car on an arrow to 0.3 tiles: a script
  `LOCATE_CHAR_BY_CAR` is a small box round the arrow, which is what held the zoo mission.
- **Reading the script.** `-bot_script_dump` writes every script command (index, name, raw words),
  `-bot_miss_log` makes the game write `test\MISS_LOG.TXT` (which command each thread runs each
  frame), and the stuck report lists the script threads with the next commands, and the peds near
  the player with their objectives. A stalled mission can be read off these in a few minutes.

- **Gang hunting.** `KillGangGoal` goes to the territory by car and stays in it (`TravelGoal` with
  `keep_car`), runs gang peds over (`RunOverAction`), shoots the ones on foot, picks up the better
  guns they drop, and takes another car when it loses one. A car stolen from its owner can be taken
  back, so a drive that ends with the player thrown out goes back for the car (the thief is beside it).
  Kills go fast once the gang is in sight and attacking; the slow part is getting there (140 tiles
  through traffic from the start). `-bot=kill_gang -bot_gang=N -bot_kills=K` runs it alone.
- **Trains.** Checked with `-bot_zones_dump`: Downtown has 8 train tracks, each with its own entry,
  stop and exit point and one platform, so a train is a short shuttle of about 30 tiles, not a line
  between stations. Not worth a travel option on this map.
- **Watching.** `run_bot.py --show` draws the run on your own screen at game speed; if the session is
  locked or there is no display it runs off screen as before.

Missions passed by the bot (`Scripts/bot/bot_runs/runs.json`, each replays identically in the
stock game): phone 220 (the zoo: pick up a car, park on the arrow, honk, drive ZitZaki), phone 226
(destroy the delivery truck: ram it with a fast car until it stops, then shoot it).

Open: the bank guards (phone 221) stand in a walled pocket behind a traffic jam; wanted level and
police; mission goals that need a respect level the bot cannot reach quickly; the other maps.

## Risks and open points

- Headless D3D9 under xvfb+wine may not initialise; fall back to the `dma_video` path or stub the
  video globals before `WinMain`. Phase 0 settles it.
- `HeapWalk` under wine: if it misbehaves, detour the CRT allocator at startup and keep the block
  list ourselves (same list the alloc-during-play check needs anyway).
- Anything the logic reads from the clock or the sound system (the RNG seed, `timeGetTime` in
  script commands) shows up as a `restore_check` failure; each one is either excluded with a
  reason or detoured to a deterministic source.
- Driving is the biggest executor; expect most retry budget to go there early.
- Missions that need the bot to lose something on purpose (die to reset, abandon a car) are left
  until a run needs them.
- Alternative kept in reserve: build the bot as a separate modern-C++ DLL reading globals by
  address from `output.map`. More comfortable language, but it would need `__thiscall` thunks for
  every game method it calls and a regenerated address header per build; the VC6 static lib
  shares the headers and needs neither.
