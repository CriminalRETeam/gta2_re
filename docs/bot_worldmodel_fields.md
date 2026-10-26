# Bot WorldModel: game data field map

Reference for `Source/Bot/WorldModel` (docs/BOT_DESIGN.md, phase 1): where each entity kind lives, how to iterate it safely and which fields to read. Produced by reading the headers on branch `bot` at 6ae3ccbd; re-check line numbers against the tree.

GTA2 bot WorldModel field map. Everything below was read from source; anything I could not find is marked NOT FOUND. All paths are under /home/snake/dev/gta2_re/Source/.

Units used throughout:
- Fix16 (fix16.hpp:20, `s32 mValue` at :502): 14-bit fixed point, 1.0 = 16384. Use `ToInt()` (= `mValue>>14`, :183), `ToUInt8()` or `ToFloat()` (:208). One map tile = 1.0.
- Ang16 (ang16.hpp, `s16 rValue` at :374): `Normalize()` keeps it in 0..1439, so 1440 = 360 deg (4 units per degree).
- Fix16_Point_POD has `.x` and `.y`. Fix16_Vec (Fix16_Vec.hpp:5) is `struct { Fix16 x, y, z; }`.

Cross-cutting warnings (read these before coding):
- (a) Several engine iterators keep their position in shared state. If you call them, you can corrupt an iteration the game is in the middle of:
  - `Game::IterateFirstPlayer_4B9CD0` / `IterateNextPlayer_4B9D10` use `Game::field_20_player_iter_idx`.
  - `GangPool::FirstGang_4BECA0` / `NextGang_4BECE0` use the global `gGangIdx_6206B8` (Gang.cpp:10).
  - All the `Map_0x370` zone queries use `field_364_cur_zone_idx`, `field_368_zone_type`, `field_36A_zone_x`, `field_36B_zone_y` and `field_36C_bMatchPos`.
  - Read the arrays directly instead (snippets below).
- (b) Some pools have an "active list" that does not hold every live object. Cars, objects and peds below each say which test catches all of them.
- (c) Run the bot on the game thread, e.g. hooked from `Game::UpdateGame_4B9410`.

=====================================================================
1. PLAYER
=====================================================================
Game.hpp:142 `EXTERN_GLOBAL(Game*, gGame_0x40_67E008);`
Game fields:
- :117 `Player* field_4_players[MAX_PLAYERS];` (MAX_PLAYERS = 6, :7)
- :123 `u8 field_24_local_player_idx;`
- :127 `s32 field_28_exit_timer;`
- :128 `s32 field_2C_game_exit_type;`
- :134 `Player* field_38_local_player;`

```
Player* P = gGame_0x40_67E008->field_38_local_player;
Ped* me = P->field_2C4_player_ped;   // Player.hpp:416 (NOT field_38)
```
- Also: `P->field_2C8_aux_ped` (:417, remote-control ped). `GetActivePed_4A5150()` (:319) returns the aux ped when `field_68_camera_mode` is 2 or 3.
- Player index (used for gang respect): `u8 field_2E_idx` (:359), or `get_idx_4219D0()`.

Score / cash: score and money are the same counter.
- `PlayerScoreTracker field_2D4_scores;` (Player.hpp:420)
- `P->GetScore_421980()` → `field_2D4_scores.GetScore_592370()`, which returns `field_0_money.field_0_value` (PlayerScoreTracker.cpp:297).
- PlayerScoreTracker.hpp:56 `RollingDigitCounter field_0_money;`. RollingDigitCounter has `s32 field_0_value` (RollingDigitCounter.hpp:47) and `get_value()` (:42).
- Other tracker fields: :86 `u16 field_86_total_kills;`, :88 `s16 field_88_cop_cars_destroyed;`, :67 `u16 field_8A_cars_stolen_count;`, `field_38_multiplayer_frags`.
- Score multiplier: Player.hpp:426 `RollingDigitCounter field_6BC_multipliers;`, or `get_multiplier_4766A0()`.

Lives:
- Player.hpp:425 `RollingDigitCounter field_684_lives;`
- Read with `P->get_lives_4766C0()` or `P->field_684_lives.field_0_value`.

Wanted level (stored on the ped):
- Ped.hpp:901 `s16 field_20A_wanted_points;`
- Stars 0..6: `me->get_wanted_star_count_46EF00()` (Ped.hpp:338, EXPORT, u8). It compares against `cop_level_ped_enum` (enums.hpp:190): 0, 600, 1600, 3000, 5000, 8000, 12000 → 1..6 stars.
- Return values are `cop_level_enum` (enums.hpp:176): none_0, police_1, police_2, police_3 (roadblocks), swat_4, fbi_5, army_6.
- The HUD copy of the same value: `gHud_2B00_706620->field_1028_wanted_level.field_48_cop_level` (Hud.hpp:229, s32), set in Hud.cpp:511.

Gang respect: see item 7 for the gang pool. `gang->GetRespectForPlayer_4BEEF0(P->field_2E_idx)` returns `field_11C_respect[idx]` (Gang.hpp:121, `s8 [6]`). The game clamps it at 100. `IsRespectNegativeForPlayer_4BEF10` means respect < -19.

Current weapon / ammo:
- Player.hpp:429 `Weapon* field_718_weapons[28];`, :430 `s16 field_788_curr_weapon_idx;` (-1 = none).
- `P->GetCurrPlayerWeapon_5648F0()` returns `field_718_weapons[curr]` or NULL (Player.cpp:335).
- Ped side: Ped.hpp:864 `Weapon* field_170_selected_weapon;`, :865 `field_174_pWeapon`, :866 `field_178_car_weapon`.
- Weapon fields:
  - Weapon.hpp:166 `u16 field_0_ammo;` (0xFFFF = infinite; `get_ammo_4A4FE0()`)
  - :176 `s32 field_1C_idx;` (`weapon_type` enum, enums.hpp:580: pistol=0 … dual_pistol=10, car_bomb=15 … army_gun_jeep=22; indices >= 15 are car weapons)
  - :181 `Ped* field_24_pPed;`
  - :174 `Car* field_14_car;`
- Weapon pool: WeaponManager.hpp:103 `EXTERN_GLOBAL(WeaponPool*, gWeapon_30_Pool_707014);` holding `Pool<Weapon,255> field_0_pool`.
- Max ammo per weapon: WeaponManager.hpp:107 `max_ammo_capacity_5FF75C[28]`.

In car:
- Ped.hpp:863 `Car* field_16C_car;` (`has_car_403B80()` / `get_car_416B60()`).
- `P->GetPlayerCar_5698E0()` (EXPORT) also covers the aux ped.

Position:
- Best source: Ped.hpp:879 `Fix16_Vec field_1AC_cam;`, via `get_cam_x()`, `get_cam_y()`, `get_cam_z()`. The game sets it from the Char sprite when on foot (Ped.cpp:1285/4264) and from the car sprite when in a car (`UpdatePositionFromCar_45C4B0`, Ped.cpp:1297), so it is valid in both cases.
- Raw path: Ped.hpp:862 `Char* field_168_game_object;` → char.hpp:142 `Sprite* field_80_sprite_ptr;` → sprite.hpp:601 `Fix16_Point_POD field_14_xy;` and :602 `Fix16 field_1C_zpos;`.
  - Char inlines: `get_sprite_xpos()`, `get_sprite_ypos()`, `get_sprite_zpos()` (char.hpp:182-196).
  - `field_168_game_object` can be NULL (e.g. when in a car), so check before use.
- Helper: `P->get_pos_569920(Fix16*, Fix16*, Fix16*)` (EXPORT).

Facing angle:
- `me->GetRotation()` (EXPORT, Ped.cpp:1505): Char `field_40_rotation` (char.hpp:103) when on foot, car sprite `field_0_rotation` when in a car.
- Ped also has `Ang16 field_12C_facing_angle` (:844), `field_12E_aim_angle` (:845), `field_134_rotation` (:848).

Health: Ped.hpp:907 `s16 field_216_health;` (`get_health_433B70()`; full = 100).

Death / arrest:
- Player.hpp:354 `char_type field_28_wasted_or_busted;`, :358 `s16 field_2C_death_countdown;`
  - On death or arrest the game sets them to 1 and 70, then counts down to 0 and respawns (Player.cpp:1760-1850).
- Player.hpp:421 `char_type field_640_busted;`
  - Set when the ped's `field_21C_bf.bBusted` is set (Player.cpp:2173, Police.cpp:439). `PedFlags32 field_21C_bf` is Ped.hpp:911; bits are in ped_flags.hpp (bActive bit0, bBusted bit5, bOnFire bit24, bInvisible bit25 …).
- `P->IsWastedOrBustedNotObjective54_476700()` = wasted. `P->IsBustedObjective54_476730()` = busted (objective_54).
- Ped dead test: `field_278_ped_state_1 == ped_state_1::dead_9` (`isDead_403B60()`). `ped_state_1` enum is at enums.hpp:925 (in_car_10, entering_car_3, …).
- Other Player fields:
  - :363 `Gang* field_34_gang_curr_location;` (updated per frame, Player.cpp:1946)
  - :364 `gmp_map_zone* field_38_local_navigation_zone;`
  - :365 `gmp_map_zone* field_3C_navigation_zone;`
  - :427 `u16 field_6F4_power_up_timers[17];` (`power_up_indices`, Player.hpp:24)

=====================================================================
2. PEDS
=====================================================================
Char_Pool.hpp:192 `EXTERN_GLOBAL(PedPool*, gPedPool_6787B8);`
- PedPool holds `Pool<Ped,200> field_0_pool` (:187). `Pool` is in Pool.hpp:56, with fields `field_0_pStart` (free list), `field_4_pPrev` (active list head), `field_8_pool[N]` and `s16 field_X_count` (live count, rebuilt each update).
- Other globals: `gPedManager_6787BC` (:190, `PedManager::PedById(s32)` EXPORT), `gChar_B4_Pool_6FDB44` (`PoolBasic<Char,400>`), `gThreateningPedsList_678468` (Ped_List.hpp:58), `gPedRefTable_7F8_703398` (PedRefTable.hpp:39; maps a u8 ref index to a ped id via `GetPedId_420F10`).

```
for (Ped* p = gPedPool_6787B8->GetFirstPed_435530(); p; p = p->mpNext) { ... }
```
- Everything on this list is allocated (pattern used by `PedManager::PedById`, Char_Pool.cpp:1020). Dead peds stay on it until removed; check `isDead_403B60()`.
- No code that takes peds off the active list was found.

Per-Ped fields (Ped.hpp; sizeof 0x294):

| Line | Declaration | Notes |
|---|---|---|
| :898 | `u32 field_200_id;` | `get_id()` |
| :925 | `s32 field_240_occupation;` | `ped_ocupation_enum`, enums.hpp:332-390 (player=0, empty=1, dummy=3, driver=5, mugger=15, car_thief=16, armed_gang_member_19, elvis=22, paramedic_23, police=24, swat=25, fbi=26, army_army=27, guard=28, walking_guard_29, roadblock_cop_37, fireman=38, gang_driver_42 …) |
| :934 | `s32 field_258_objective;` | `objectives_enum`, enums.hpp:816-883 (no_obj_0, flee_*, goto_area_on_foot_12, goto_area_any_means_13, goto_area_in_car_14, kill_char_any_means_19, kill_char_on_foot_20, enter_car_as_driver_35, leave_car_36, …) |
| :935 | `s32 field_25C_internal_objective;` | |
| :915 | `u8 field_225_objective_status;` | `objective_status`, enums.hpp:806: not_finished_0, passed_1, failed_2 |
| :923 | `s32 field_238_ped_type;` | `ped_type`, enums.hpp:899: player_2, dummy_3, special_ped_4, script_created_5, dummy_with_occupation_6 |
| :867 | `Gang* field_17C_pGang;` | |
| :875 | `Gang* field_19C_dummy_gang;` | |
| :926 | `char_type field_244_remap;` | `get_remap_433BA0()` |
| :907 | `s16 field_216_health;` | |
| :863 | `Car* field_16C_car;` | |
| :854 | `Ped* field_148_objective_target_ped;` | |
| :855 | `Ped* field_14C_internal_target_ped;` | |
| :853 | `Ped* field_144_attacker;` | |
| :859 | `Player* field_15C_player;` | non-NULL = player ped; `is_player_41B0A0()` |
| :862 | `Char* field_168_game_object;` | position, see item 1; prefer `field_1AC_cam` |
| :950 | `s32 field_278_ped_state_1;` | |
| :951 | `s32 field_27C_ped_state_2;` | |

- Cop test: `p->IsLawEnforcement_45B4E0()` (EXPORT, Ped.cpp:718). True for police, swat, fbi, army_army, walking_guard_29, unknown_cop_occu_30, unknown_cop_occu_31, roadblock_cop_37.
- `IsEmergencyOccupation_45B590()` is occupation 23..27 or 37.

=====================================================================
3. CARS
=====================================================================
Car.hpp:1467 `EXTERN_GLOBAL(CarPool*, gCar_BC_Pool_67792C);`
- CarPool holds `Pool<Car,306> field_0_pool` (:1412). Accessors: `GetFirstCar_420E50()` (:1401), `get_cars_count_45AD30()`.
- Also `gCarManager_6C_677930` (:328).

```
for (Car* c = gCar_BC_Pool_67792C->GetFirstCar_420E50(); c; c = c->mpNext) { ... }
```
- Caveat: deactivated cars (`Deactivate_43AA60`, Car.cpp:2342; sets `field_88_despawn_status = deactivated_7`), trailers (Car.cpp:1522) and train carriages (PublicTransport.cpp:182/793) are taken off the active list.
- To catch every car, scan the whole array:
```
for (int i = 0; i < 306; i++) {
    Car* c = &gCar_BC_Pool_67792C->field_0_pool.field_8_pool[i];
    if (c->field_50_car_sprite && c->field_6C_car_id) { ... }
}
```
  PoolDeallocate clears both fields (Car.cpp:~6801-6820).

Per-Car fields (Car.hpp:1325-1372; sizeof 0xBC):

| Line | Declaration | Notes |
|---|---|---|
| :1344 | `s32 field_84_car_info_idx;` | car model (`car_model_enum`, enums.hpp:65; e.g. COPCAR, TANK) |
| :1331 | `Ped* field_54_driver;` | `GetDriver_4118B0()`, `IsDrivenByPlayer_4118D0()` |
| :1326 | `Ped_List field_4_passengers_list;` | see snippet below |
| :1339 | `s16 field_74_damage;` | 32001 = destroyed (`IsMaxDamage_40F890`) |
| :1346 | `u8 field_8C_damage_level;` | |
| :1327 | `DamageAreaFlags field_8_damaged_areas;` | |
| :1352 | `s32 field_98_door_lock;` | `Car_Door_Lock`, enums.hpp:161: no_lock_0, locked_1, lockout_thief_2, unlocked_3, locked_permanently_4, lockout_player_5, none_6 |
| :1354 | `s32 field_A0_car_kind;` | `car_kind`, Car.hpp:41: police_6, roadblock_car_7, mission_car_8, parked_car_9 … |
| :1342 | `s32 field_7C_ped_type;` | |
| :1341 | `u16 field_78_flags;` | `car_flag`, car_flags.hpp (Bulletproof 0x100, Rocketproof 0x200, Flameproof 0x400 …) |
| :1330 | `Sprite* field_50_car_sprite;` | position: `->field_14_xy.x/.y`, `->field_1C_zpos`; heading: `->field_0_rotation` (`GetCarRotation_416BB0`); remap: Sprite `s16 field_24_remap` (sprite.hpp:605), set by `SetCarRemap` (Car.cpp:2118) |
| :1332 | `CarPhysics* field_58_physics;` | |
| :1337 | `s32 field_6C_car_id;` | |
| :1345 | `s32 field_88_despawn_status;` | |
| :1353 | `s32 field_9C_engine_status;` | |

- Passenger iteration:
```
for (PedListNode* n = c->field_4_passengers_list.field_0_pFirstPed; n; n = n->mpNext) {
    Ped* p = n->field_0_char_ped;
}
```
  PedListNode is Char_Pool.hpp:28-46.
- Mission car: `c->IsMissionRelated_4421B0()` (EXPORT, Car.cpp:5551) = `car_kind == mission_car_8`, or `ped_type == script_created_5`, or a script-created passenger.
- Police car: `IsPoliceCar_439EC0()` (COPCAR / SWATVAN / EDSELFBI), or `field_A0_car_kind == car_kind::police_6`.
- Speed: `c->GetVelocity_43A4C0()` (Fix16, length of `field_58_physics->field_0_vel_read_only`, 0 without physics) or `GetCarLinearSpeed_43A240()`; also `GetMaxSpeed_439F30()`.
- Gang: NOT FOUND as a Car field. Gangs carry `field_13C_gang_car_model` and `field_140_gang_car_remap`, and `GangPool::FindGangByCarModel_4BF2F0(s32)` returns a gang index.

How a ped enters a car:
- The ped gets objective `enter_car_as_driver_35` (`IsPedGoingToEnterCar_492FD0`, Ped.hpp:803).
- Fields set: `Car* field_154_target_to_enter` (:857), `u8 field_24C_target_car_door` (:928), `s32 field_248_enter_car_as_passenger` (:927, 1 = passenger), `Car* field_150_target_objective_car` (:856).
- `field_278_ped_state_1` goes to `entering_car_3`, then `in_car_10`.
- `GetCarBeingEnteredOrExited_45BBF0()` is EXPORT.

=====================================================================
4. PHONES / SCRIPT STATE
=====================================================================
ScriptManager.hpp:750 `EXTERN_GLOBAL(ScriptManager*, gScriptManager_6F8060);`

ThreadTrigger (:491-498):
```
s32 field_0_ped_id;
s32 field_4_target_id;
u16 field_8_cmd_line;
```

ScriptManager fields:
- :694 `s16 field_184_phone_trigger_count;`
- :696 `ThreadTrigger field_188_phone_triggers[20];`
- :743 `u16 field_C1E32_phone_ids[31];` (script pointer indices of gang phones)
- :702 `s32 field_310_finish_score;`
- :703 `s32 field_314_total_missions;`
- :704 `u32 field_318_total_secrets;`
- :705-707 `field_31C/320/324_gang_N_missions_total`
- :708 `s32* field_328_passed_flag;`
- :709-711 `field_32C/330/334_N_passed_flag`
- :712 `field_338_secrets_passed`, :713 `field_33C_secrets_failed`
- :715 `s32* field_344_mission_flag;`
- :716-718 `field_348/34C/350_gang_N_mission_flag`
- :740 `char_type field_C1E2D_bKillFrenzyActive;`
- :741 `char_type field_C1E2E_death_arrest_flag;`
- All the `s32*` fields point at script counter variables (`SCR_POINTER::field_8_counter`) and can be NULL.

Phone representation:
- A phone is an `Object` (Object_5C.hpp:48) whose `field_18_model` is in 176..181 (`IsModel176To181_475AA0`, :246).
- Colour from `GetPhoneTypeFromObjModel_5D1260` (Hud.cpp): 176/177 = red (2), 178/179 = yellow (1), 180/181 = green (3). Minimum respect: yellow 40, red 80, green 237 (`GetMinRespectForPhoneType_5D12E0`).
- objects enum (enums.hpp:612) also has blue_phone_164.
- Dead phone: the game switches it to model 174 (`SET_PHONE_DEAD`, ScriptThread.cpp:4195). `STOP_PHONE_RING` switches it to 163.

Ringing / answerable:
- A phone is live if some `field_188_phone_triggers[i].field_4_target_id == obj->field_14_id` with `field_8_cmd_line != 0`. `field_0_ped_id` is the ped (`field_200_id`) that may answer it.
- Added by script commands `ANSWER_PHONE` and `THREAD_DECLARE4` (ScriptThread.cpp:818/3604).
- Lookup helpers: `FindPhoneThreadByObjId_512AD0(s32)` and `FindPhoneThread_512980(ped_id, obj_id)`.
- The engine answers when the player ped touches the phone: `Char::PhoneTouched_5535B0` → `AnswerPhone_5129F0(ped_id, obj_id)`.
- Thread started: `GetBasePointer_512770(trigger.field_8_cmd_line)` gives an `SCR_THREAD*` (ScriptThread.hpp:167).
  - If `field_2_type == SCRCMD_THREAD_DECLARE4`, it spawns a thread at `field_E_thread_start_cmd`; afterwards `field_8_script_thread` and `field_C_thread_id` are set.
  - If it is `SCRCMD_ANSWER_PHONE`, the waiting thread gets `field_12_answered = 1`.
- Mission names: there is no phone → mission-name table. After launch, `ScriptThread::field_14_str[255]` holds the mission script name (strcpy in `launch_mission_5119A0`, ScriptThread.cpp:7709).

Script threads:
- ScriptThread.hpp:1204 `EXTERN_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool_6F8064);` holding `Pool<ScriptThread,8> field_0_pool`.
- Iterate: `for (ScriptThread* t = miss2_0x11C_Pool_6F8064->field_0_pool.field_4_pPrev; t; t = t->mpNext)` (pattern in `LogAll_47F4D0`, :1193).
- ScriptThread fields (:1151-1167): `u16 field_4_next_cmd`, `s32 field_8_cond_result`, `s16 field_E_phone_timer`, `char_type field_10_stopped`, `char_type field_14_str[255]`, `s16 field_11A_thread_id`.

On mission:
- `gScriptManager_6F8060->field_344_mission_flag && *field_344_mission_flag == 1` (the check used at ScriptThread.cpp:82; the save-game code also treats nonzero as on mission, :5059).
- Gang-specific: `*field_348_gang_1_mission_flag == 1` etc.

Mission counters:
- Passed: `*field_328_passed_flag` out of `field_314_total_missions` (HUD stats, Hud.cpp:3302).
- Per gang: `*field_32C_1_passed_flag` and so on.

Mission result: NOT FOUND as a single engine field; pass/fail is up to the script. What is visible:
- Pass: the `*field_328_passed_flag` counter goes up and `*field_344_mission_flag` goes back to 0.
- Fail on death/arrest (engine, `MissionFailOnDeathOrArrest_503200`, ScriptThread.cpp:80-133): sets `*field_344_mission_flag = 0` and `field_C1E2E_death_arrest_flag = 1`, and shows the "mfail" text in `gHud_2B00_706620->field_111C_message`.

Brief text:
- Hud.hpp:728 `Hud_Brief field_DC_brief;`
- Current entry: `gHud_2B00_706620->field_DC_brief.field_6F8_curr_briefs` (Hud.hpp:660, a list linked through `field_C_pNext`).
- Hud_BriefEntry (:367-375): `char_type field_0_brief_id_str[8]` (gxt key, e.g. "1_3"), `s32 field_8_brief_priority`, `u8 field_10_was_displayed`, `s32 field_14_cost_param`.
- Formatted wide text: `field_DC_brief.field_0_str[640]` (:648).
- Big centre messages: `field_111C_message` (Hud_Message :107-121: `u8 field_0_time_to_show; wchar_t field_2_str[221]; s32 field_1C4_priority;`).
- Pagers/timers: `field_620_pagers.field_0_pagers_array[4]` (Hud_Pager: `s32 field_0_timer; s32* field_4_ptr_counter;`).

=====================================================================
5. HUD ARROW
=====================================================================
Hud.hpp:750 `EXTERN_GLOBAL(Hud*, gHud_2B00_706620);`

There is no per-player arrow instance. The single array is `gHud_2B00_706620->field_1F18_arrows` (Hud.hpp:731, `Hud_Arrow_7C_Array`):
- :622 `Hud_Arrow field_0_array[17];`
- :623 `char_type field_83C_show_gang_arrows;`
- :624 `Hud_Arrow* field_840_visible_gang_arrow;`

Hud_Arrow (:582-588): `Fix16 field_0_screen_pos_x`, `field_4_screen_pos_y`, `Ang16 field_8_rotation`, …, `Hud_ArrowTargets field_18_targets;`

Hud_ArrowTargets (:489-495):
- `s32 field_28_arrow_colour;` (4 = default; 1/2/3 = yellow/red/green phone arrow)
- `s16 field_2C_arrow_sprt_idx;`
- `Hud_ArrowGangInfo field_10_gang_info;` → `Gang* field_30_gang; char_type field_34_min_respect; char_type field_5_is_visible; char_type field_6_in_use;`
- `ArrowTrace field_18_primary_target;`
- `ArrowTrace field_3C_secondary_target;`
- `ArrowTrace* field_60_curr_target;`

ArrowTrace (:466-474):
- `Ped* field_0_ped; Car* field_4_car; Object* field_8_obj; Player* field_C_player;`
- `s32 field_10_target_type;`
- `Fix16 field_14_aim_x, field_18_aim_y, field_1C_aim_z;`
- `char_type field_20_bIsTargetVisible;`

ArrowTargetType (:378-389): Nothing_0, XYZ_Coord_1, Ped_2, Car_3, Object_4, InfoPhone_5, Player_6.

Getting the target:
- `field_14/18/1C_aim_*` hold the world position. For Ped/Car/Object/Player targets they are refreshed every frame (`ArrowTrace::UpdateAimCoordinates_5D03F0`, Hud.cpp:607). For XYZ and InfoPhone they are fixed.
- The type resets to Nothing_0 when the ped goes inactive, the car despawns, or the object becomes model 174.
- Script mission arrows are allocated with `AllocArrow_5D1050` (ScriptThread.cpp:2249-2332). Reset clears `field_30_gang` to NULL and sets colour 4.
- Gang/phone arrows have `field_30_gang != NULL` (Hud.cpp:1253, 1310).

```
for (int i = 0; i < 17; i++) {
    Hud_Arrow& a = gHud_2B00_706620->field_1F18_arrows.field_0_array[i];
    Hud_ArrowTargets& t = a.field_18_targets;
    if (!t.field_10_gang_info.field_6_in_use || a.IsType0_4C6F80()) continue;
    bool mission = (t.field_10_gang_info.field_30_gang == NULL);
    ArrowTrace* tr = &t.field_18_primary_target;
    // tr->field_10_target_type, tr->field_14_aim_x/_18/_1C, tr->field_0_ped / field_4_car / field_8_obj
}
```

=====================================================================
6. OBJECTS / BONUSES
=====================================================================
ObjectPool.hpp:8 `EXTERN_GLOBAL(TObjectPool*, gObject_2C_Pool_6F8F80);`
- `typedef Pool<Object, 3825> TObjectPool;` (:6)
- Manager: Object_5C.hpp:440 `gObject_5C_6F8F84`. Its `u8 field_20_uncollected_tokens[50]` (:434) tracks secret tokens.
- `Object_3C` (Object_3C_Pool.hpp:21 `gObject_3C_Pool_6F8F7C`, `PoolBasic<Object_3C,385>`) and `Object_8` (Object_8_Pool.hpp:21 `gObject_8_Pool_6F8F78`, `PoolBasic<Object_8,1101>`) are free-list-only pools with no active list. They are sub-objects:
  - `Object::field_10_obj_3c` holds motion: angle, speed, z speed.
  - `field_C_pAny.o8` holds animation state.
  - You cannot iterate them on their own.

Iteration caveat: objects whose definition has `field_61_in_pool_list == 0` are kept out of the active list (`AllocateUntracked` / `PoolTake_522360`; `field_20_pool_list_state = out_of_pool_list_2`, Object_5C.cpp:3393-3398, 2550). Static props and pickups can be among them; they live only in the SpriteGrid buckets.
- Active list only (incomplete): `for (Object* o = gObject_2C_Pool_6F8F80->field_4_pPrev; o; o = o->mpNext)`
- Complete scan:
```
for (int i = 0; i < 3825; i++) {
    Object* o = &gObject_2C_Pool_6F8F80->field_8_pool[i];
    if (o->field_4_sprite && o->field_18_model) { ... }
}
```
  PoolDeallocate zeroes `field_18_model`, `field_4_sprite` and `field_14_id` (Object_5C.cpp:130-163). `field_20_pool_list_state` is `object_pool_state` (:28): none_0, in_pool_list_1, out_of_pool_list_2.

Object fields (Object_5C.hpp:341-366; sizeof 0x2C):
- `Object* mpNext;`
- `Sprite* field_4_sprite;` → `GetX_4340D0()`, `GetY_4340E0()`, `GetZ_4340F0()`
- `ObjectDefinition* field_8_definition;` → `field_34_behavior_type` (`object_behavior_type`, ObjectDefinitions.hpp:153: pickup_6..pickup_physics_animated_9, shop_10 …; `IsPickup_421060()`)
- `s32 field_14_id;`
- `s32 field_18_model;` (`objects` enum, enums.hpp:612-793)
- `u8 field_26_ped_ref_idx;` (for pickups this is the item parameter, e.g. ammo)
- `char_type field_27_thread_trigger_idx;`

Pickup decoding (Ped::HandlePickupCollision_45DE80, Ped.cpp:2285):
- `model == 266` (secret_token_266): token; its index is `field_26`.
- Models <= 108 (moving_collect_96..108) get +136 first.
- After that, model <= 227: weapon pickup, weapon_type = model-200, ammo = `field_26`.
- Above 227: power-up, `power_up_indices` = model-228 (health_powerup_230, armor_231, get_out_jail_232, invulnerability_234, invisibility_239).

Kill frenzy / gang icons: `kill_frenzy_icon_286`, `loonies_icon_287` … `russian_mafia_icon_293`; `IsGangIcon_4BE850()`. Phones: see item 4. Savepoint: `savepoint_161`. Car shops: 130/131/162/163.

Spatial grids (alternative): SpriteGrid.hpp `gSpriteGrid_1_679208`, `gSpriteGrid_2_67920C`, `gSpriteGrid_3_679210`. Each has `GridCell* field_0_rows[256]`; a GridCell is `{u8 field_0_x; GridSpriteLink* field_4_pSpriteLinks; GridCell* mpNext;}`, and GridSpriteLink (collide.hpp) is `{Sprite* field_0_pSprite; GridSpriteLink* mpNext;}`. Read `Sprite::field_30_sprite_type_enum` (sprite.hpp:612) and the union `field_8_object_2C_ptr` / `field_8_car_bc_ptr` / `field_8_char_b4_ptr` (:452-459).

=====================================================================
7. ZONES / MAP / GANGS
=====================================================================
map_0x370.hpp:824 `EXTERN_GLOBAL(Map_0x370*, gMap_0x370_6F6268);`

Map_0x370 fields:
- :795 `gmp_compressed_map_32* field_0_pDmap;` (`u32 field_0_base[256][256]`, so the map is 256x256 tiles)
- :797 `gmp_map_zone* field_328_pZoneData;`
- :798 `gmp_zone_list* field_32C_pZones;`
- :800 `gmp_zone_info* field_334_pZoneInfo;`

gmp_zone_list (:348-353): `u16 field_0_num_zones; u16 field_2_pad; gmp_map_zone* field_4_zone_array[2];` (variable length).

gmp_map_zone (:187-205):
```
u8 field_0_zone_type;
u8 field_1_x, field_2_y, field_3_w, field_4_h;
u8 field_5_name_length;
char_type field_6_name[2];   // variable length
```
- `ContainsPoint_463020(u8, u8)`; `get_zone_str_4DEF00()` gives the display wide string.

Zone types: `gmp_zone_type` (:66-89, e.g. Navigation_1, Arrow_Blocker_5, Information_10, Gang_14, Local_Navigation_15, Restart_16, Arrest_Restart_20). There is a duplicate namespace `gmp_zone_type_enum` at enums.hpp:205.

Safe iteration (does not touch the shared cursor):
```
gmp_zone_list* zl = gMap_0x370_6F6268->field_32C_pZones;
if (gMap_0x370_6F6268->field_328_pZoneData) {
    for (u16 i = 0; i < zl->field_0_num_zones; i++) {
        gmp_map_zone* z = zl->field_4_zone_array[i];
    }
}
```
- Engine queries (EXPORT, but they move the shared cursor): `GetZone_4DFB30(u16)`, `GetZoneAtPosOfType_4DF4D0(u8 x, u8 y, u8 type)`, `GetNavZoneAtPos_4DF5C0`, `GetFirstZoneAtPos_4DF6A0` / `GetNextZone_4DF770`, `FindZoneByName_4DEFD0`, `GetZoneInfoAt_4DF890`.

Gang at coordinates:
- `Gang* Map_0x370::GetGangAtCoords_4DFB50(Fix16 x, Fix16 y)` (map_0x370.hpp:421): finds the Gang_14 zone, then `gGangPool_CA8_67E274->FindGangByName_4BF100(zone->field_6_name)`. It moves the cursor.
- Or read `Player::field_34_gang_curr_location`.

Gangs:
- Gang.hpp:160 `EXTERN_GLOBAL(GangPool*, gGangPool_CA8_67E274);` holding `Gang field_0_gang_list[10];` (:156).
- `FirstGang_4BECA0` / `NextGang_4BECE0` only return gangs with `field_0_used && field_139_kill_respect_change > 0`, and they use the global cursor. Iterate directly:
```
for (int i = 0; i < 10; i++) {
    Gang* g = &gGangPool_CA8_67E274->field_0_gang_list[i];
    if (!g->field_0_used) continue;
    // ...
}
```
- Gang fields (Gang.hpp:105-134):
  - `u8 field_0_used; u8 field_1_gang_idx; char field_2_name[4];`
  - `char field_101_remap;`
  - `u32 field_104_basic_weapon, field_108_angry_weapon, field_10C_hate_weapon;`
  - `char field_112_hostile_to_gang[10];`
  - `s8 field_11C_respect[6];`
  - `Fix16 field_12C_info_phone_x, field_130_info_phone_y, field_134_info_phone_z;`
  - `u8 field_138_arrow_colour;`
  - `u32 field_13C_gang_car_model; char field_140_gang_car_remap;`
- Other helpers: `GangByIdx_4BF1C0(u8)`, `GetNameWide_4BED30()`.

Blocks:
- `gmp_block_info* GetBlockOrNull_452980(u8 x_coord, u8 y_coord, u8 z_coord);` (map_0x370.hpp:358, impl map_0x370.cpp:208). Returns NULL outside the column's offset..height.
- Same with s32 args: `GetBlock_4DFE10(s32, s32, s32)` (:427), plus `GetEffectiveBlock_4DFE60` and `GetBlockClamped_4DFEE0`.
- gmp_block_info (:43-64): `u16 field_0_left, field_2_right, field_4_top, field_6_bottom, field_8_lid; u8 field_A_arrows; u8 field_B_slope_type;`
- Face words: `block_face` (block_face.hpp): tile_idx_mask 0x3FF, wall 0x400, bullet_wall 0x800, flat 0x1000, flip 0x2000.
- Arrows: `block_arrow` (:22-40): green_left 0x1, green_right 0x2, green_up 0x4, green_down 0x8, red_* 0x10..0x80.
- Slope byte macros (:339-346):
  - `GET_BLOCK_TYPE(b) = b & 3` → `gmp_slope_type` (AIR 0, ROAD 1, PAVEMENT 2, FIELD 3)
  - `GET_SLOPE_IDX = b >> 2` (0..63)
  - `IS_GRADIENT_SLOPE` = slope index 1..44
  - `MapSlopes` enum (:91-110): diagonal 0xB4..0xC0, tri 0xC4..0xD0, partial 0xD4..0xF4
  - `gGmpSlopes_6F5BA8[64]` (:324)
- Ground height: `FindGroundZForCoord_4E5B60(Fix16 x, Fix16 y)`.
- Map size: 256x256 (`field_0_base[256][256]`; SpriteGrid rows 0..255). Z: no named constant found; `PathFinder::FindNearbyTileMatchingSlopeType` rejects z > 6 or z == 0.

=====================================================================
8. POLICE
=====================================================================
Police.hpp:20 `EXTERN_GLOBAL(Police*, gPolice_7B8_6FEE40);` (also declared again at :22)

Police fields:
- :77 `PoliceCrew field_4_cop_crew[MAX_POLICE_CREWS];` (20)
- :78 `PolicePursuitTarget field_464_pursuit_targets[MAX_PURSUIT_TARGETS];` (4)
- :79 `s32 field_654_max_wanted_level;`
- :80 `u8 field_658_police_car_count;`
- :83 `u8 field_660_max_wanted_stars;`
- :91 `char_type field_7AD_police_peds_in_range_screen;`

PolicePursuitTarget (Police_38.hpp:98-138):
- `Ped* field_0_criminal_ped; s32 field_4_wanted_level; s32 field_8_state;`
- `Fix16 field_10_x, field_14_y, field_18_z;` (last-seen position)
- `PoliceCrew* field_20_crews[6];`
- `u8 field_70_num_police_crews … field_75_num_crews;`
- `char_type field_78_is_actively_chased;`
- The list ends at the first NULL `field_0_criminal_ped`.

PoliceCrew (:67-94): `s32 field_1C_used; s32 field_20_crew_kind; s32 field_24_state; EmergencyCrew* field_10_subObj;` (EmergencyCrew members were not explored).

Exported queries: `IsBeingPursued_56F800(Ped*)`, `IsActivelyChased_56F880(Ped*)`.

Wanted number 0..6: `me->get_wanted_star_count_46EF00()` (item 1). Cop peds: ped loop with `IsLawEnforcement_45B4E0()`. Cop cars: car loop with `field_A0_car_kind == police_6` or `IsPoliceCar_439EC0()`. Ped.hpp also has `gNumberWalkingCopsSpawned_6787CD`, and Char_Pool.hpp:198 has `gNumPolicePedsInRangeScreen_6787EE`.

=====================================================================
9. PATHFINDING
=====================================================================
PathFinder.hpp:111 `EXTERN_GLOBAL(PathFinder*, gPathFinder_6FDEF0);`

Signature (PathFinder.hpp:50):
```
EXPORT bool ComputePath_554AB0(s32 ped_id, Ped* pPed, u8 x_start, u8 y_start, u8 z_start,
                               u8 x_end, u8 y_end, u8 z_end, s32 angle_face, u8* pOutPathFailCount);
```

Behaviour (PathFinder.cpp:521-888):
- It is a local best-first search in a 34x33 tile window centred on the start (grid origin 16,16), so it only reaches about ±16 tiles.
- It runs across several frames with a budget of 100 per call. It returns 0 = still computing (call again next frame) and 1 = finished (success, failure, or timeout after 200 calls).
- On success it writes the path into `pPed->field_0_patrol_points[100]` (Ped.hpp:843; PatrolPoint is `{u8 field_0_x, field_1_y, field_2_z}`, PatrolRoutePool.hpp:7). The list ends at `x == 0`, and the count goes to `*pOutPathFailCount`.
- On failure it calls `RemovePed_554620` and does not write points.
- Ped caller protocol (Ped.cpp:6950-6975):
  1. `gPathFinder_6FDEF0->field_3C_ped_list.AddPedToBackIfMissing_471160(ped)` and set `ped->field_21C_bf.bInPathList`.
  2. Only call ComputePath when `IsFirstInPathQueue_554A90(ped) == 1`.
  3. When it finishes, `RemovePed_4711F0`.
  4. Pass `angle_face = Ang16::GetAngleFace_4F78F0(angle)`.
- `field_38_bComputePathInProgress` is set while a search is running.

RouteFinder (road junction graph):
- RouteFinder.hpp:158 `EXTERN_GLOBAL(RouteFinder*, gRouteFinder_6FFDCC8);` — exact name is `gRouteFinder_6FFDC8`.
- `EXPORT bool InitSearch_589480(u8 start_x, u8 start_y, u8 start_z, u8 dest_x, u8 dest_y, u8 dest_z, s32 arrow_type);` (:120). This only seeds the search.
- Easiest entry point: `EXPORT s16 DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type);` (:134). It runs InitSearch, then `SearchToDest_589E20`, then `StoreRoute_589EB0`, and returns a route index 1..49, or -1 on failure. `StartRoute_58A190` is the same.
- `arrow_type`: `gmp_arrow_type` (map_0x370.hpp): green_1, red_2, green_or_red_3.
- Reading the result:
```
u16* j = gRouteFinder_6FFDC8->field_2218_routes[idx].field_0_junctions;   // JunctionRoute :94, u16[256]
// ordered start → dest, 0-terminated; length also in global gLastRouteLength_6FFDCC (no header EXTERN found)
Junction* J = gRouteFinder_6FFDC8->GetJunction_58A0B0(j[k]);              // or &field_8_junctions[j[k]]
// free afterwards:
gRouteFinder_6FFDC8->CancelRoute_589930(idx);
```
- Junction (:42-63): `Link field_0_n, field_2_s, field_4_e, field_6_w; s32 field_8_type; u8 field_C_min_x, field_D_min_y, field_E_max_x, field_F_max_y;`
- Link (:7-39): `u16 field_0`. `GetIndex_0040CE90()` = `field_0 & 0x1ff` (neighbour junction), `GetLength()` = `(field_0 >> 9) & 0x3F`, `IsEnabled()` = bit 15.
- RouteFinder fields (:139-155): `u16 field_0_route_count; Junction field_8_junctions[545]; JunctionRoute field_2218_routes[50]; u16 field_8618_idx; u16 field_861A_dest_idx;` (start and dest junction), plus `JunctionSegment field_A830_horz_segments[545]`, `field_B938_vert_segments[545]` (`{u16 junction_num1, junction_num2; u8 min_x, min_y, max_x, max_y}`) and the counts `field_CC62` / `field_CC64`.
- Also available: `FindArrowBlockInJunction_5890D0(u16 junction_idx, s32 direction, u8* x, u8* y)`.
- Example call in the game: Player.cpp:81 `DoStartRoute_58A0D0(3,3,1,0xD5,0xAB,1,1)`.

=====================================================================
10. LEVEL FINISH / EXIT
=====================================================================
Exit score: `gScriptManager_6F8060->field_310_finish_score` (ScriptManager.hpp:702). Set by `SCRCMD_FINISH_SCORE_505710` (ScriptThread.cpp:1310).

`Player::field_60_finish_score_reached` (Player.hpp:372, s32):
- `Player::SetScoreTextColour_569C20` (Player.cpp:2490-2518) sets it to 1 when `GetScore >= field_310_finish_score` (score digits turn red) and back to 0 if the score drops below. This happens in single player only.

AreaCompleted_4 callers:
- `ScriptThread::SCRCMD_FINISH_LEVEL_50EA40` (ScriptThread.cpp:5758-5793). It maps the command's `field_A_bonus_type` to 0..3, forces bonus 2 if `*field_328_passed_flag == field_314_total_missions`, then calls `ExitGame_4B8BD0(0, GameExitType::AreaCompleted_4, bonus)` or `ExitGameNoBonus_4B8C00(0, AreaCompleted_4)`. So the level ends only when the script runs FINISH_LEVEL (normally when the player reaches the exit after hitting the score).
- Debug `UnknownDebugClass::DoBrianTest_42D870` case 8 (Player.cpp:78).
- Handled in winmain.cpp:2491.

`Game::ExitGame_4B8BD0(s32 new_timer, s32 exit_type, s8 bonus_type)` (Game.cpp:105):
- Sets `field_28_exit_timer` (-1 = none; `IsExitTimerSet_4B7590()`), `field_2C_game_exit_type` (`GetExitType_4D09C0()`) and `gGameSession_67E8E0.SetLevelFinishBonusType_4C5930(bonus)`.
- `GameExitType` (Game.hpp:25): None 0, CloseGameImmediately_1, PlayerQuit_2, GameOverRIP_3, AreaCompleted_4, MultiplayerExit_5, ReplayExit_6.
- For the bot, "level complete" means `gGame_0x40_67E008->field_2C_game_exit_type == GameExitType::AreaCompleted_4` once `field_28_exit_timer != -1`.

=====================================================================
NOT FOUND / unverified
=====================================================================
- A car → gang field.
- A single engine "mission passed/failed" result field (it is script-driven, see item 4).
- A header EXTERN for `gLastRouteLength_6FFDCC`.
- A named map Z-level constant.
- What the script assigns to Hud_Message for "mission complete" (not traced).
- EmergencyCrew members.
- Whether ammo is stored ×10 relative to the HUD display (not checked).
