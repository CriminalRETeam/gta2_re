# Inlines recovered from 9.6f

`9.6f.exe` was built with inlining mostly turned off, so many helpers that 10.5 inlines are real
functions there. This doc tracks, function by function, which 9.6f calls became inlines in
10.5, and whether `Source/` has them yet. WIP functions come first, since a missing inline is a
common reason for a near miss.

## How the tables are made

1. The "Dump target asm" workflow runs `Scripts/bin_comp/dump_fingerprints.py` on both original
   exes and pushes `fingerprints.json` to the `claude/target-asm` branch. It holds call
   targets, globals, strings, float constants and mnemonics of every function, but no code bytes.
2. `Scripts/bin_comp/match_96f.py` pairs 10.5 functions with their 9.6f versions. It starts
   from the known pairs in `Scripts/ida/functions_data.json`, unique names, strings and
   imports, then grows the pairs along the call graph, through shared globals, and between
   paired neighbours (functions of one .cpp stay together in both builds). It writes
   `match_96f.json`.
3. `Scripts/bin_comp/gen_inline_tracking.py` writes the tables below, keeping the Status and
   Notes columns. With `--json` it also fills the missing 9.6f addresses in
   `Scripts/ida/functions_data.json`, marked `"v96f_auto": true` so they can be told from
   the hand checked ones (and redone on the next run).
4. The workflow also dumps the 9.6f asm of the functions listed in
   `Scripts/bin_comp/dump_96f_addrs.txt` on the request branch (`gen_inline_tracking.py --addrs`) (the 9.6f versions of the
   WIP/STUB functions and every inlined callee) to `target_96f.json`, with absolute
   addresses so calls can be followed. Regenerate that list from `match_96f.json` when the
   pairs change.

```bash
cd Scripts/bin_comp
git show origin/claude/target-asm:fingerprints.json > fingerprints.json
git show origin/claude/target-asm:target_96f.json > target_96f.json
python3 match_96f.py && python3 gen_inline_tracking.py
```

A ✓ in front of a callee means the function's source already uses an inline named after that
address (`MaxAbsDistance_42A6B0`). The basic `Fix16`/`ang16`/point operators are left out of
the per-function lists, since the source uses them as operators.

A 9.6f callee is listed when the 10.5 version of the caller doesn't call its 10.5 partner. That
is usually an inline, but can also be a pairing miss, so check the asm. "10.5 copy" means 10.5
still has an out-of-line copy of the inline (often an unmarked COMDAT). Library code (CRT,
Bink and Miles thunks) is left out.

## Callee check

`Scripts/bin_comp/compare_callees.py` lists, per WIP, where the build's call targets differ
from the original's (both resolved to names). The verifier doesn't compare call targets, so this
is not a list of mismatches. It does show out-of-line copies that still lack a marker (it found
`Fix16_Point::operator-` at 0x40AC80), and calls that 10.5 inlines or doesn't.

## 9.6f is a reference only

9.6f was built with a different compiler and different settings, so its code never has to match.
It tells us which helpers exist and roughly what their bodies do, nothing more. Only 10.5
codegen counts: an inline is right when the 10.5 code that uses it matches.

The function order in 9.6f differs from 10.5 too, and some files aren't contiguous. Pairing
by neighbours only fires when both neighbours are already paired and their 9.6f partners are
close together, but a pair from it can still be wrong where the order changed.

## How to verify an inline

An inline has no body of its own in 10.5, so it can only be checked through code that uses it:

- **Out-of-line copy.** If 10.5 still has a copy ("10.5 copy" column), give it a marker. When it
  matches, the inline body is right.
- **A matched user.** Use the inline in a `MATCH_FUNC` that inlined it ("WIP/MATCH users"
  column). If the function still matches, the body agrees with 10.5 there.
- **A WIP user.** Adding the inline to a WIP is the goal. It counts once the WIP matches, or at
  least gets closer.

Note what was done in the Status column (`todo`, `present`, `added`, `verified`, `n/a`) and
anything worth keeping in Notes. Mention the 9.6f address in a comment next to the inline in
`Source/` (`// 9.6f 0x401B20`), so the "Noted in Source" column picks it up.

<!-- generated -->

Paired: 3081/4433 10.5 functions. Marked functions with inlined callees: WIP 220, STUB 0, MATCH 877. Distinct inlined 9.6f functions: 1373. WIPs without a 9.6f partner: 137.

<!-- table WIP -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|
| 0x407CE0 | `Car_A4_10::UpdateTrailerAlignment_407CE0` | 0x40F900 | ✓ `sub_40F820`, `sub_40F580`, `sub_421CB0`, ✓ `sub_40F6B0`, `sub_40F7C0`, `sub_40F680`, ✓ `sub_40F540`, `sub_40F600`, ✓ `sub_40F790`, `sub_40F0A0`, ✓ `sub_40F840`, `sub_40EE60`, ✓ `sub_40F830`, ✓ `sub_40F7E0`, ✓ `sub_40F800`, ✓ `sub_40F810` | inlines added | 318->235; get/set_theta_40F820/40F830, set_cp1_40F7E0, +=, Fix16_To_Ang16_40F540, atan2_40F790 used; 40F840 commented (different globals); 40F0A0/40EE60/421CB0 are pairing noise. vtall also showed DIFF WIP in CarAI_78 0x44E560 and Particle_8 0x53F4C0 (not checked) |
| 0x413120 | `sound_obj::sub_413120` | 0x4150E0 | ✓ `sub_414F60` | commented | Car_BC sub_414F60/sub_411930 (f78 & 0x100) as bool inline makes 196->198 (value materialised in dl) |
| 0x413760 | `sound_obj::ProcessType6_Rozza_C88_413760` | 0x415F90 | `sub_411730` | matched | sub_411730 = ComputeEmitterDistanceSquared_4190B0, 10.5 still calls it (pairing noise) |
| 0x413A10 | `sound_obj::sub_413A10` | 0x4121F0 | `Car_BC::sub_421D90`, ✓ `sub_410BF0` | checked | Car_BC::sub_421D90 = GetCarLinearSpeed_43A240 (still called in 10.5); fixed inverted field_10 null check, 98->92 |
| 0x41AB80 | `sound_obj::ProcessActiveQueues_41AB80` | 0x417410 | `sub_414FA0`, `sub_4B6320`, `sub_4B62B0`, `sub_4B6150`, `sub_4B6110`, `sub_4B6130`, `sub_416F20`, `sub_4168E0`, `sub_416940`, `sub_4B62D0`, `sub_416E50`, `sub_4B6200`, `sub_4B6360`, `sub_4B6340`, `sub_4B63B0`, `sub_4B6190`, `sub_4B6170`, `sub_4B61E0` | checked | all pairing noise: 10.5 still calls cSampleManager wrappers, sub_412260 (=414FA0), VecDiff_41B4E0 (=4168E0), FromFix16_41B520 (=416940), AdjustPlaybackRate_41A580 (=416F20), sub_41A4A0 (=416E50) |
| 0x422B70 | `sound_obj::sub_422B70` | 0x41B7B0 | `sub_4A65E0`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_41B090`, ✓ `sub_41B0A0`, ✓ `sub_410BF0` | done | get_velocity_41B080, is_player_41B0A0, Round_To_Int_410BF0 x3 (get_ped_state_2_41B090 already used); 192->175; sub_4A65E0 = GetPlayerCar_5698E0 (still called) |
| 0x423080 | `sound_obj::sub_423080` | 0x41B0D0 | ✓ `sub_41B0B0`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::get_occupation_403980` | matched | Ped::TakeF250_41B0B0 (new) + is_player_41B0A0; 6->6 (remaining diff is register choice for rate + RandomDisplacement) |
| 0x42A500 | `sound_obj::ProcessType7_Weapon_42A500` | 0x41CCA0 | ✓ `sub_41CC80`, ✓ `sub_41CC70`, ✓ `sub_41CC90`, `sub_4CD8C0`, `sub_411730` | done | Weapon_30::TakeF2C_41CC80 (new), Get_F4_41CC70, GetWeaponType_41CC90, position fields like 9.6f; 92->67; 4CD8C0/411730 still called in 10.5 |
| 0x4320D0 | `sad_mirzakhani::sub_4320D0` | 0x41DEE0 | ✓ `sub_41DC40` | done | Player::Add_2D4 = 9.6f 0x41DC40 (comment added); 51->51 |
| 0x4358D0 | `Camera_0xBC::ComputeTargetFacingAngle_4358D0` | 0x41E620 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_416BB0`, ✓ `CarPhysics_B0::is_backward_gas_on_411810` | done | all 9.6f inlines used |
| 0x436200 | `Camera_0xBC::ApplyCarVelocityCameraOffset_436200` | 0x41EBF0 | ✓ `Car_BC::sub_403BA0`, ✓ `Car_BC::sub_411900`, ✓ `sub_40F790`, ✓ `Car_BC::has_trailer_41E460`, ✓ `sub_41E210` | done | atan2_40F790, is_trailer_cab_41E460, FromPolar_41E210; 98->97 |
| 0x43B2B0 | `Car_BC::sub_43B2B0` | 0x422360 | ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_target_to_enter_403B10` | done | GetInternalObjective_403A90, get_target_to_enter_403B10 used (6 unchanged) |
| 0x43B5A0 | `Car_BC::GetDoorWorldPosition_43B5A0` | 0x422500 | ✓ `sub_41FE70`, ✓ `RotateVector_41FC90` | done | all 9.6f inlines used |
| 0x43D690 | `Car_BC::EmitExplosion_43D690` | 0x425FD0 | `Object_5C::sub_4852E0` (10.5 0x5299B0), `sub_424220` | checked | 9.6f 0x425FD0 is a different function (NewPhysicsObj + radio tower angle), pairing miss |
| 0x4413B0 | `Car_BC::UpdateTrainCarriagesOnTrack_4413B0` | 0x429680 | `sub_467110`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690` | matched | 467110 = Map_0x370::sub_4E6660 (called in 10.5) |
| 0x4418D0 | `Car_BC::HandleUserInput_4418D0` | 0x429810 | `Car_BC::sub_423720`, `Car_BC::sub_4260C0`, ✓ `Car_BC::sub_425780` | commented | 4->2 (bool test of HandleRoofTurretRotation result); sub_425780/411930 flag helper worsens (4->14) so commented; 423720/4260C0 = HandleRoofTurretRotation/HandleSpecialInput (called) |
| 0x4425D0 | `Car_BC::ManageTVAntenna_4425D0` | 0x424280 | `sub_424220`, `angle::angle_not_equal_41CFF0`, `sub_40F580`, ✓ `sub_40F540` | done | 44->39; Ang16_to_Fix16 temp + zero copy order as in 9.6f; 424220 = GetRadioTowerAngle_442520 |
| 0x442810 | `Car_BC::sub_442810` | 0x4262E0 | ✓ `sub_40F7B0`, `sub_421D80`, ✓ `IsMaxDamage_40F890`, ✓ `sub_4214B0`, ✓ `sub_4215B0`, ✓ `sub_40FEB0`, `sub_421C40`, `sub_40F600`, `sub_4262B0`, `Car_BC::sub_425DF0`, ✓ `sub_421130`, `sub_420390`, `sub_4207B0`, `sub_40F820`, `sub_40F580` | inlines added | 133->129; new CarPhysics_B0::IsRngBelowDamage_421130; get_theta_40F820 worsens (134) so not used; 421C40/4262B0/425DF0/420390/421D80 = GetHitchPoint_439FB0/AttachTrailer_4427A0/UpdateDamageFlags_43BD40/NormalizeSafe_442AD0/HasSpriteZoom_43A230 |
| 0x443710 | `Car_BC::sub_443710` | 0x426580 | ✓ `sub_4214D0`, `sub_4207B0`, `sub_40F600`, `sub_420390` | checked | sub_4214D0, get_x_y, operator-, NormalizeSafe already used; 9.6f order same |
| 0x444FC0 | `DoGetNearestCarFromCoord_444FC0` | 0x424BD0 | ✓ `sub_420E50`, ✓ `IsMaxDamage_40F890`, ✓ `sub_421640`, `sub_421D80`, ✓ `sub_4214D0`, `sub_421DF0`, ✓ `sub_411940`, ✓ `Car_BC::sub_403BA0` | inlines added | 218->216; new Car_BC_Pool::GetFirstCar_420E50; fixed TRAINFB test (!=); 421D80/421DF0 = HasSpriteZoom_43A230/IsCarInAir_43A3C0; MapRenderer 4EFDB0 +2 jitter |
| 0x4458B0 | `Car_6C::SpawnCarOnRoadNetwork_4458B0` | 0x428540 | `sub_40C810`, ✓ `sub_41FE40`, ✓ `EnqueueRadioLocationPhrase_426E10`, `sub_421510` | done | 239->205; SpawnCar_426E10_v2 used; 40C810 = switch around sub_588DE0 (already open), SwapIf3or4_41FE40 already used, 421510 open-coded |
| 0x445EC0 | `Car_BC::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` | 0x445EC0 | `sub_420B70` | matched | 181->148; pairing wrong (9.6f addr=10.5 addr), but used SwapIf3or4_41FE40, SetRect_41E350/SetHiLowZ_41E370, Accelerate_421210 |
| 0x446530 | `Car_6C::SpawnCabAndTrailer_446530` | 0x428EC0 | ✓ `sub_426E40`, `sub_40FD40`, ✓ `sub_420F30`, `sub_425570` | checked | 426E40/420F30/425570 already used; 40FD40 = sub_408370 (called) |
| 0x4469F0 | `Car_6C::ctor_4469F0` | 0x428FA0 | ✓ `Car_BC_Pool::ctor_426DB0`, `CarPhyisicsPool::ctor_420F80`, ✓ `CarAI_78_Pool::ctor_420EB0`, ✓ `TrailerPool::ctor_425500`, ✓ `struct_4::sub_4207E0`, `Car_3C::ctor_4205A0` | checked | already 0 diff (candidate match?); CarPhyisicsPool ctor already inline, Car_3C ctor = Sprite ctor (new Sprite()) |
| 0x446DC0 | `Car_6C::dtor_446DC0` | 0x45A9D0 | `Car_6C::dtor_429250` | checked | 9.6f 0x45A9D0 is the deleting-dtor wrapper of 0x429250 = this dtor; already 0 diff |
| 0x447970 | `CarAI_78::sub_447970` | 0x430320 | matched | inlines added | TurnAntiClockwise_42AB90 used (286 unchanged); ContainsPoint (40CEE0) commented, worsens 286->298 |
| 0x4482C0 | `CarAI_78::sub_4482C0` | 0x42B350 | `sub_4221A0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, ✓ `sub_416B40`, `Car_BC::sub_421D90`, ✓ `Car_3C::set_ang_lazy_420690` | done | 393->315; PolarToCartesian_41FC20, get_type_416B40 used; 4221A0/421D90 are DoBreak_43A950/GetCarLinearSpeed_43A240 (pairing, called in 10.5) |
| 0x448770 | `CarAI_78::sub_448770` | 0x430650 | ✓ `PolarToCartesian_41FC20`, `sub_42A5D0`, `sub_42A810`, `sub_42FDF0`, ✓ `sub_42AC30`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, `sub_4221A0`, ✓ `sub_42AC20`, `sub_42ADA0` | inlines added | 447->48; PolarToCartesian_41FC20 x3, IsA6Bit0/1Set used; 42A810 (gtx spec==3 check) not added yet; 42A5D0/42FDF0/42ADA0 = IsClockwiseTurning_448270/ChooseRandomTurn_4537D0/sub_4482C0 (called in 10.5) |
| 0x448CE0 | `CarAI_78::ManageTrafficCarDirection_448CE0` | 0x430CC0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, `sub_42FE40`, ✓ `sub_42AB90`, ✓ `sub_42ABB0`, ✓ `sub_42ABA0`, `sub_42B8A0` | commented | 42FE40=sub_4538B0 (10.5 calls once, rest inlined); inline copy for calls 2-4 worsens 1295->1346; 42B8A0 big callee not identified |
| 0x44E0C0 | `CarAI_78::Init_AI_Chase_44E0C0` | 0x42D390 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `MaxAbsDistance_42A6B0` | done | Ped get_cam_x/y/z used (285 unchanged); MaxAbsDistance_42A6B0 noted only: 10.5 calls Abs_436A50/Max_44E540 out of line |
| 0x44E560 | `CarAI_78::UpdateStateMachine_44E560` | 0x42D820 | `sub_4463C0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, `MaxAbsDistance_42A6B0`, `sub_40E8D0`, `sub_42A5B0`, `sub_421C00`, `sub_446690`, `sub_446550`, `sub_446480`, `sub_4221A0`, `sub_40EC80`, ✓ `sub_42AB90`, ✓ `sub_42ABA0`, `sub_40EAB0`, ✓ `sub_421250`, ✓ `sub_421540`, `sub_42A660`, ✓ `sub_42ABB0`, `sub_42A620`, `sub_4BD670`, `Car_BC::sub_421D90`, `sub_421210`, `sub_446410`, `sub_446620`, `sub_4464E0`, `sub_4465B0` | inlines added | 5010->4843; TurnClockwise/AntiClockwise, SetGoStraight_42ABB0, new SetHandBrakeOn_421250, SetA6Bit5_421540; Accelerate_421210 worsens (4863) so not used; PolarToCartesian sites (many += into globals) and 4463C0..4465B0 helpers not done |
| 0x451980 | `CarAI_78::ReactToNearbyCar_451980` | 0x431770 | `sub_40EAB0`, ✓ `MaxAbsDistance_42A6B0`, `sub_42C8B0`, `sub_4221A0`, ✓ `sub_42AC00` (10.5 0x453F90), `sub_423940`, `Car_BC::sub_421D90`, ✓ `sub_42ABC0` (10.5 0x453F50), ✓ `sub_421210`, ✓ `sub_421470`, `sub_40E8D0`, ✓ `sub_42ABA0`, ✓ `sub_42AB90`, `sub_430090`, `sub_4221B0` | checked | inlines already used; remaining (40EAB0/40E8D0/42C8B0/423940/421D90/4221A0/4221B0/430090) are 10.5 out-of-line calls (4056C0/405320/44D1D0/4416D0/43A240/43A950/43A970/453C00) |
| 0x452A20 | `CarAI_78::ManageCollisions_452A20` | 0x42FB80 | ✓ `sub_4215B0`, ✓ `sub_41E210`, ✓ `sub_42A720`, ✓ `sub_42AC00` (10.5 0x453F90), ✓ `sub_42AB90`, ✓ `sub_42ABA0`, ✓ `sub_421210` | done | all 9.6f inlines used |
| 0x452DF0 | `CarAI_78::sub_452DF0` | 0x432370 | ✓ `sub_42ABB0`, ✓ `sub_42AB90`, ✓ `sub_42ABA0`, `sub_431C10`, ✓ `sub_416B40`, ✓ `sub_40FEC0` | matched | only 431C10 (1855 bytes) missing, = a CarAI_78 method 10.5 still calls |
| 0x457BF0 | `Taxi_4::GetTaxiNear_457BF0` | 0x4330A0 | ✓ `MaxAbsDistance_42A6B0`, ✓ `sub_4215B0` | done | all 9.6f inlines used |
| 0x45C9D0 | `Ped::ComputeAimAngle_45C9D0` | 0x43E3A0 | `sub_445CC0`, `sub_43BEC0`, `sub_437EB0`, `sub_40E8D0` | checked | all missing are pairing noise (4713C0/466BB0/466F40 wrappers, atan2) |
| 0x45DE80 | `Ped::HandlePickupCollision_45DE80` | 0x43E550 | ✓ `sub_434B10`, ✓ `sub_434B20`, ✓ `sub_434B60`, ✓ `sub_434A10`, `sub_421050`, `sub_4340A0`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | diff 24 unchanged; new youthful_einstein/lucid_hamilton helpers; 421050/4340A0 = get_field_26 already |
| 0x45E080 | `Ped::SpawnWeaponOnDeath_45E080` | 0x436250 | ✓ `sub_434130` | matched | diff 149->145; Object_5C.hpp includes Object_8.hpp now (CarAI_78 WIP 44E560 5009->5010 from that include alone) |
| 0x45E4A0 | `Ped::sub_45E4A0` | 0x43B7C0 | ✓ `sub_433470`, ✓ `sub_4334A0`, ✓ `sub_4334D0`, ✓ `sub_433500` | done | all 9.6f inlines used |
| 0x45EB60 | `Ped::Deallocate_45EB60` | 0x43E650 | ✓ `sub_434070`, ✓ `Car_BC::sub_4343B0` | done | all 9.6f inlines used |
| 0x45FF60 | `Ped::CarThief_AI_45FF60` | 0x442050 | matched | done | diff 134 unchanged |
| 0x460820 | `Ped::TaxiCustomer_AI_460820` | 0x442420 | ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `Char_8::sub_420EA0`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403920`, `Car_BC::sub_421EC0`, ✓ `MaxAbsDistance_42A6B0` | inlines added | diff 318 unchanged; MaxAbsDistance commented (worse); 421EC0 = GetVelocity_43A4C0 (noise) |
| 0x461630 | `Ped::RobbedDriver_AI_461630` | 0x442A40 | ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_403920`, ✓ `Car_BC::sub_421560` | done | diff 130 unchanged |
| 0x461A60 | `Ped::UpdateFacingAngle_461A60` | 0x436460 | `sub_40E8D0`, `sub_40EAB0`, ✓ `Char_B4::sub_433920`, ✓ `sub_40F540` | checked | 40E8D0 = atan2_fixed_405320, 40EAB0 = ComputeShortestAngleDelta_4056C0 (pairing noise) |
| 0x4626B0 | `Ped::StateMachineTick_4626B0` | 0x43ECC0 | ✓ `sub_433810`, ✓ `Char_B4::sub_433A80`, ✓ `sub_4338F0`, `sub_491EA0`, `ApplyCarVelocityCameraOffset_436200`, `sub_436140`, `angle::angle_not_equal_41CFF0`, ✓ `sub_433C20`, ✓ `sub_4215B0` | matched | diff 187->177; 491EA0/436200/436140 pairing noise |
| 0x463570 | `Ped::SetObjective` | 0x43BBC0 | `cool_nash_0x294::sub_433580` | matched | cool_nash_0x294::sub_433580 is ChangePedStatesByMode_463300, which 10.5 calls (pairing noise) |
| 0x463830 | `Ped::SetObjective2_463830` | 0x436920 | ✓ `Car_10::set_obj_421380`, `cool_nash_0x294::sub_433580` | checked | cool_nash_0x294::sub_433580 is ChangePedStatesByMode_463300, which 10.5 calls (pairing noise) |
| 0x463FB0 | `Ped::ProcessInCarObjective_463FB0` | 0x444D00 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, `sub_436BF0`, `sub_493940`, ✓ `sub_4340D0`, ✓ `sub_4340E0`, ✓ `sub_4340F0`, `cool_nash_0x294::sub_439D30` | checked | already uses the cam/Object_2C getters; 436BF0/493940/439D30 are sub_4645B0/CarDoorAlignmentSolver_545AF0/ChaseTargetStateMachine_46B170 (pairing noise) |
| 0x465270 | `Ped::Threat_Reaction_AI_465270` | 0x43F340 | `sub_43BED0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `sub_41CC90`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `cool_nash_0x294::sub_403AE0`, `sub_40E8D0`, ✓ `sub_433A30`, `sub_43BEB0`, `Car_BC::sub_421EC0`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, `Mouze_44::sub_403D20`, `Mouze_44::sub_403D10`, ✓ `cool_nash_0x294::get_car_416B60`, `sub_404900`, ✓ `sub_403950`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::sub_433DA0`, ✓ `sub_41B0A0`, ✓ `Char_B4::sub_433A80` | checked | 43BEB0/43BED0 = FindBestTargetPed_Mode1/Mode5, Mouze_44 403D20/403D10 = PedGroup::IsLeaderEnteringCarOrUnknown5_4C9220/IsLeaderInCar_4C9210, 404900 = MergeWithOtherGroup, 421EC0 = GetVelocity_43A4C0, 40E8D0 = atan2 (pairing noise) |
| 0x467090 | `Ped::FindUsableCarDoor_467090` | 0x437EC0 | `Car_BC::sub_421EC0`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, ✓ `sub_4214D0`, ✓ `Car_10::set_obj_421380` | checked | 421EC0 = GetVelocity_43A4C0 (already called) |
| 0x4672E0 | `Ped::UpdateMovementTowardsTarget_4672E0` | 0x4380D0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_4340D0`, ✓ `sub_4340E0`, ✓ `sub_4340F0`, `sub_40E8D0` | done | diff 256->244; 40E8D0 = atan2 |
| 0x469060 | `Ped::GotoAreaByAnyMeans_469060` | 0x43C480 | ✓ `sub_4215B0`, ✓ `MaxAbsDistance_42A6B0`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `cool_nash_0x294::get_cam_x_403A00`, ✓ `Get_F3C_433370`, ✓ `Car_BC::sub_421560`, ✓ `sub_421510`, ✓ `sub_426E00`, ✓ `sub_433900`, `Car_BC::sub_421EC0`, ✓ `cool_nash_0x294::sub_403AE0` | inlines added | diff 484 unchanged; only Get_F3C used, others comment (each made diff worse); 421510 commented |
| 0x469FE0 | `Ped::sub_469FE0` | 0x43CFC0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x46AC20 | `Ped::FollowTargetStateMachine_46AC20` | 0x439970 | matched | done | all 9.6f inlines used |
| 0x46B2F0 | `Ped::PullDriverOutOfCarStateMachine_46B2F0` | 0x439E60 | matched | done | diff 177->164; listed missing were noise, used SetMaxSpeed/SetCharState |
| 0x46B670 | `Ped::MeleeAttackStateMachine_46B670` | 0x4436A0 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40`, `sub_435610`, ✓ `sub_433BF0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0`, ✓ `sub_416B40`, ✓ `Char_B4::sub_433920`, ✓ `Char_B4::sub_433A80`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_41DC40`, ✓ `sub_433B70`, ✓ `sub_433B60`, ✓ `sub_433E50`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_433970`, ✓ `Car_10::set_obj_421380` | done | diff 363->292; 435610 noise |
| 0x46BDC0 | `Ped::EnterCarStateMachine_46BDC0` | 0x43D1C0 | ✓ `sub_434AF0`, ✓ `sub_433900`, `Car_BC::sub_421EC0`, ✓ `sub_433560`, ✓ `Char_B4::sub_433920`, ✓ `Car_10::set_obj_421380`, `sub_426F00`, ✓ `Char_B4::sub_433930`, ✓ `sub_4118B0`, `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | diff 109->86; gotos removed; 426F00 = ApplyVisualDamage_43A9F0 |
| 0x46C250 | `Ped::ExitCarStateMachine_46C250` | 0x443C30 | ✓ `sub_433C10`, ✓ `sub_433900`, ✓ `Char_B4::sub_433920`, ✓ `sub_433A30`, ✓ `sub_4341B0`, ✓ `Car_10::set_obj_421380`, ✓ `sub_4215B0`, ✓ `sub_433C40` | done | diff 374->358; new Ped::DoJump_433C40 |
| 0x46D460 | `Ped::AttackTargetStateMachine_46D460` | 0x4441B0 | ✓ `sub_4340F0`, ✓ `sub_434140`, ✓ `Car_BC::sub_41E450`, ✓ `IsMaxDamage_40F890`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_car_416B60`, `Car_BC::sub_421EC0`, ✓ `sub_433BF0`, ✓ `sub_433BD0`, ✓ `sub_433810`, `sub_435610`, ✓ `sub_41CC70`, ✓ `sub_433AA0`, ✓ `sub_433970`, ✓ `Char_B4::sub_433920`, ✓ `sub_420420`, ✓ `Char_B4::sub_433A80` | commented | 421EC0/435610 noise; SetMaxSpeed worse (23->30), commented |
| 0x46E080 | `Ped::RecruitNearbyPeds_46E080` | 0x444930 | `struct_4::ctor_424620`, ✓ `sub_4204D0`, ✓ `sub_40FEA0`, ✓ `sub_433A20`, `sub_4402C0` | commented | closer 116->56; sub_4204D0 inline left as a comment (made it worse); diff 93->50; z fix (cam.z not searchRadius); ComputeCollisionPrism commented (different k constant in 10.5) |
| 0x46E380 | `SpawnPedestrianAt_46E380` | 0x4404F0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_433B80`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433BC0`, ✓ `sub_433C80`, `sub_43D640`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_433C10`, ✓ `sub_433C00`, `cool_nash_0x294::sub_433DF0` | inlines added | diff 323 unchanged; SetRemap_433C10 commented (+2); others noise |
| 0x46EB60 | `Char_C::SpawnDummies_46EB60` | 0x440CC0 | `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_433430` | done | diff 148->147; new Map_0x370::IsSlopeB4ToD0At_433430; 420600 = set_xyz_lazy_451950 (noise) |
| 0x4703F0 | `Char_C::PedsService_4703F0` | 0x445A20 | `sub_445960`, `Ped_Unknown_4::sub_4460D0` | checked | 445960 = Pool::UpdatePool, 4460D0 = Ped_List_4::ClearPeds_4712F0 (both already used) |
| 0x470650 | `Char_C::ctor_470650` | 0x4416B0 | matched | checked | pool ctors are the new expressions, set_xyz_lazy_420600 is ResetCharStatics_553F90 / 451950 (noise); w.sh already 0 |
| 0x47E5B0 | `Crane_15C::dtor_47E5B0` | 0x447F70 | ✓ `root_sound::DestroySoundObj_40FE60` | done | all 9.6f inlines used |
| 0x47F930 | `Crane_15C::PickUpCar_47F930` | 0x448A80 | ✓ `sub_4215B0`, `sub_4BEA60` (10.5 0x5A6AD0), ✓ `sub_447EC0`, ✓ `sub_447EB0`, `sub_423A70`, ✓ `sub_447F00`, `sub_4207B0`, `sub_448900`, `Zheal_15C::sub_448150`, `sub_4BED60` | checked | inlines already used or still called in 10.5 (TagSpriteWithRng_5A6C10, AreAllDoorsClosed_441A40, AddSprite_5A6CD0, ComputeHookPolar_47F6C0, IsTarget1PositionClear_47EB00) |
| 0x492260 | `thirsty_lamarr::sub_492260` | 0x44B500 | `sub_44B490` | checked | 9.6f sub_44B490 = sub_495470 (10.5 calls it directly; 9.6f had an extra wrapper level) |
| 0x492430 | `thirsty_lamarr::sub_492430` | 0x44B6E0 | `sub_44B490` | checked | 9.6f sub_44B490 = sub_495470 (10.5 calls it directly) |
| 0x498DA0 | `BurgerKing_1::read_input_device_498DA0` | 0x44C0F0 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_44C050` | done | get_cur_rng_41CFE0, new IsInputSet_44C050; 293->258 |
| 0x49C340 | `DoorData_10::sub_49C340` | 0x44C8A0 | ✓ `gmp_block_info::init_44C840` | done | new gmp_block_info::init_44C840; diff 0 before and after (CANDIDATE MATCH) |
| 0x49CFA0 | `Door_4D4::RegisterDoubleDoorNoCheck_49CFA0` | 0x44D430 | `sub_44CDD0` | checked | sub_44CDD0 is a 9.6f-only door setup variant; 10.5 calls Door_38::sub_49CC00 instead (pairing difference) |
| 0x49D570 | `Door_4D4::dtor_49D570` | 0x44D7D0 | `Door_10_Pool::gdtor_44D3A0` | checked | gdtor_44D3A0 is the pool delete in GTA2_DELETE_AND_NULL |
| 0x4A81F0 | `Firefighter_28::Service_4A81F0` | 0x450F10 | ✓ `sub_4215B0`, ✓ `Car_BC::sub_421560`, `sub_421510`, `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `sub_4118B0`, `MaxAbsDistance_42A6B0`, ✓ `sub_450CB0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `sub_4214B0`, `cool_nash_0x294::sub_403A40` | inlines added | Case 1 now shares the state=6 block like 9.6f/10.5 (jump layout matches); left: ebx vs ebp for 0, and the MaxAbsDistance part of case 2 (10.5 partly out of line) |
| 0x4AD140 | `Frontend::DrawMenu_4AD140` | 0x457920 | ✓ `sub_453A60` | matched | DecodeStage_453A60 x2; unchanged (76) |
| 0x4AE2D0 | `Frontend::UpdatePageFromUserInput_4AE2D0` | 0x4597C0 | ✓ `sub_453A60` | done | DecodeStage_453A60 x2; unchanged (377) |
| 0x4AF2A0 | `Frontend::ctor_4AF2A0` | 0x456A60 | ✓ `laughing_blackwell_0x1EB54::sub_453A30`, `sub_453D40` | inlines added | new Frontend::SetField10D_453A30; sub_453D40 (sets field_11C by language) has no 10.5 form, sub_4AF0E0 used; diff 0 (CANDIDATE MATCH, was 0 already) |
| 0x4B0220 | `Frontend::SetupMenuStringsOptionsElements_4B0220` | 0x453E20 | `?do_always_noconv@codecvt_base@std@@MBE_NXZ` | checked | do_always_noconv (9.6f 0x4538E0, returns true) guards dead code that 10.5 dropped; nothing to use |
| 0x4B3170 | `Frontend::sub_4B3170` | 0x4587B0 | ✓ `lucid_hamilton::sub_453A80`, ✓ `sub_453A60`, ✓ `sub_453AB0`, ✓ `sub_434B20`, ✓ `sub_453A90`, ✓ `sub_453AA0`, `sub_4529C0`, `sub_452990` | inlines added | new IsTagGame_434B20, youthful_einstein HasQuit_453A90/GetTime_453AA0/GetLeaderIdx_453AB0, DecodeStage once; 2 decodes commented (500->532); 4529C0/452990 pairing; 502->500 |
| 0x4B4440 | `Frontend::GetMainAndBonusStagesFromSeqFile_4B4440` | 0x455340 | `sub_4527A0` | checked | sub_4527A0 is Frontend::sub_4B4BC0, already called (10.5 merged the two calls) |
| 0x4B7120 | `Frontend::sub_4B7120` | 0x456180 | ✓ `sub_453A60`, ✓ `sub_453A40` | done | new lucid_hamilton::DecodeStage_453A60/EncodeStage_453A40; code unchanged (83) |
| 0x4B7800 | `Frontend::GetPrevUnlockedStageBonusCode_4B7800` | 0x4565E0 | ✓ `sub_453A40` | done | EncodeStage_453A40; 91->90 |
| 0x4C1AB0 | `next_cycle_4C1AB0` | 0x45E0B0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C9B60 | `PedGroup::MergeWithOtherGroup_4C9B60` | 0x404EF0 | `sub_403B90` (10.5 0x4CA3E0), `sub_404900`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `cool_nash_0x294::has_car_403B80`, `sub_404450`, ✓ `cool_nash_0x294::sub_403990` | done | diff 35 unchanged; 403B90 = GetBit2, 404450 = sub_4C9ED0 |
| 0x4DADA0 | `sub_4DADA0` | 0x462530 | ✓ `sub_461DC0`, `sub_4C93B0` | done | GetTimeLimit_461DC0 x2 (was dword_67ED24); sub_4C93B0 = Hud ClearPager_5D3280 (out of line in 10.5); unchanged (18) |
| 0x4DF240 | `Map_0x370::GetNearestZoneOfType_4DF240` | 0x469110 | `Map_0x370::sub_462E40`, ✓ `MaxAbsDistance_42A6B0` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4E11E0 | `Map_0x370::sub_4E11E0` | 0x465FE0 | `sub_4BA5E0`, ✓ `sub_463760`, ✓ `sub_432860`, ✓ `sub_463690` | inlines added | new Fix16_Rect GetMidZ_463760 (const; fixes missing /2) + EdgesCrossSegment_463690; x reset per row bug fixed; SetXY_432860 commented; 131->76 |
| 0x4E1520 | `Map_0x370::sub_4E1520` | 0x466170 | ✓ `sub_432860`, `sub_4BB9C0`, `sub_4828F0`, `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_40FEE0` | done | SetXY_432860; 4BB9C0=PointInsideRotatedBounds_5A1490, 4828F0=GetDirectionalObject_5298E0, set_xyz_lazy_420600=451950 all called; unchanged (162) |
| 0x4E1A30 | `Map_0x370::sub_4E1A30` | 0x466430 | ✓ `sub_4634B0`, ✓ `sub_463480` | checked | sub_463480 is the existing IsWestOrEastGradSlope_4634B0 (address comment added), already used |
| 0x4E1E00 | `Map_0x370::CanSpriteEnterTile_4E1E00` | 0x46A570 | ✓ `sub_466CF0` | done | IsGradientSlopeAt_466CF0; 1042->993 |
| 0x4E5640 | `Map_0x370::sub_4E5640` | 0x469F90 | ✓ `sub_4637B0`, `sub_40E8D0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_420420`, ✓ `sub_466CF0`, `sub_4BD670` | checked | sub_40E8D0 = atan2_fixed_405320, sub_4BD670 = 10.5 0x5A2500, both called |
| 0x4E6190 | `Map_0x370::sub_4E6190` | 0x466D30 | ✓ `sub_466CF0` | done | all 9.6f inlines used |
| 0x4E7190 | `Map_0x370::sub_4E7190` | 0x467F80 | `sub_463150`, `sub_463210`, `ProcessObjective_4632E0`, ✓ `sub_42A660` | commented | ReverseRoadDirection_42A660 helper 103->301, noted only; 463150/463210/4632E0 pair with called 4E5Dxx |
| 0x4E8E30 | `Map_0x370::do_process_loaded_zone_data_4E8E30` | 0x464330 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4EAF40 | `MapRenderer::DrawRightSide_4EAF40` | 0x470250 | `sub_46BEA0` (10.5 0x4F3FB0), `Nanobotz::Set_UV_46C0C0` (10.5 0x4F4190), ✓ `sharp_pare_0x15D8::sub_46BB50` | inlines added | GetTexture_46BB50 (198 unchanged); sub_46BEA0 = draw_4F3FB0 and Set_UV_46C0C0 = Set_UV_4F4190 (called) |
| 0x4ED290 | `MapRenderer::draw_bottom_4ED290` | 0x46D9A0 | ✓ `sub_46BC70`, ✓ `sharp_pare_0x15D8::sub_46BB50` | done | GetTexture_46BB50; unchanged (192) |
| 0x4F4D60 | `MapRenderer::draw_lid_4F4D60` | 0x470800 | ✓ `Nanobotz::sub_46BD40` (10.5 0x4EAE00), ✓ `Nanobotz::sub_46B5E0`, ✓ `sharp_pare_0x15D8::sub_46BB50` | commented | GetColour_46B5E0 43->105 |
| 0x4F66C0 | `MapRenderer::sub_4F66C0` | 0x471D60 | `Nanobotz::sub_470060`, `Nanobotz::sub_470440`, `Nanobotz::sub_470620`, `Nanobotz::sub_46C2C0`, `Nanobotz::sub_46C7F0`, `Nanobotz::sub_46CE30`, `Nanobotz::sub_46DFE0` | checked | Nanobotz draw callees (470060.., 46C2C0..) pair with the out-of-line 10.5 draw functions (called) |
| 0x4FA500 | `Ambulance_110::ProcessPatientQueue_4FA500` | 0x473E00 | `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::sub_416B50`, `Kfc_30::sub_4C54F0`, `MaxAbsDistance_42A6B0` | checked | get_cam_x/y/z (9.6f calls) give no change; diff is load/store scheduling of x/y/z |
| 0x4FAAC0 | `Ambulance_20::HandleObjectiveState_4FAAC0` | 0x473410 | `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_objective_403A80`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0`, ✓ `cool_nash_0x294::get_objective_timer_403B30`, `sub_472FD0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `sub_450CB0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403B40`, ✓ `cool_nash_0x294::set_health_4039A0`, `sub_403960` | inlines added | get_cam_x/y/z for the objective target copy (score 64->48); MaxAbsDistance spot: 10.5 calls Abs_436A50/Max_44E540 out of line, load order still differs |
| 0x4FB330 | `Ambulance_20::UpdateState_4FB330` | 0x473CE0 | ✓ `Char_8::sub_420EA0`, ✓ `sub_421470` | inlines added | IsEmpty_420EA0 added, no codegen change; case 3 else should share the default case's epilogue |
| 0x4FF250 | `Mike_A80::DebugDrawProfiling_4FF250` | 0x474530 | `sub_4744C0`, `sub_4740F0`, `sub_474490`, `unknown_libname_28`, `sub_4741F0` | matched | Fixed by `const s32& polys_drawn` (ref_local), not by the sum's term order |
| 0x516590 | `youthful_einstein::SetNewFugitive_516590` | 0x4820D0 | `sub_4C8620`, ✓ `sub_41D020`, `cool_nash_0x294::sub_435F00`, ✓ `sub_482080`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_4820A0`, `sub_4C83D0` | checked | 4C8620=ReleaseAllArrows_5D10B0, 435F00=ClearInvulnerable_45C050, 4C83D0=SetPlayerArrowColour_5D0DC0, all called in 10.5 |
| 0x51CFC0 | `Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0` | 0x4068C0 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x5213E0 | `NetPlay::sub_5213E0` | 0x40BFA0 | ✓ `sub_409C40` | done | GetPlayerIdx_409C40; unchanged (15) |
| 0x521890 | `NetPlay::sub_521890` | 0x40C120 | `sub_409C50`, ✓ `sub_409C40` | done | GetPlayerIdx_409C40 x2; SeqDiff is 409C50 (comment added); unchanged (220) |
| 0x521FD0 | `GetMovementSpeedAndAngle_521FD0` | 0x482D90 | ✓ `sub_482BA0`, ✓ `sub_40F790`, ✓ `sub_482BD0` | matched | GetSpeedVector_482BA0 added; VC6 keeps it out of line (COMDAT), diff 82->82 |
| 0x5224E0 | `Object_2C::SetMovementVector_5224E0` | 0x4849B0 | ✓ `sub_40F790`, `sub_4847D0` | matched | GetSpeedVector already inlined in ours; 4847D0 = NewObj3C_528130, which 10.5 still calls (pairing noise) |
| 0x522710 | `Object_2C::ResolveCollisionWithObject_522710` | 0x486580 | `sub_482C30`, `sub_40F600`, ✓ `sub_482C90`, ✓ `sub_482C80`, `sub_40F640` (10.5 0x43D5D0), `sub_40EAB0` | matched | GetMass_482C80 added, sub_482C90 used; 482C30/40EAB0/40F640 are real calls in 10.5; diff 235->177 |
| 0x522B20 | `Object_2C::ResolveCollisionWithWorld_522B20` | 0x486950 | ✓ `sub_482C80`, `sub_482C30` | done | GetMass_482C80 x2 (482C30 = GetXY_52AE70, still called in 10.5); diff 53->48 |
| 0x522E10 | `Object_2C::HandleCollision_522E10` | 0x486C60 | ✓ `sub_40FEB0`, ✓ `sub_482CC0`, ✓ `sub_40FEA0`, `sub_4867E0`, ✓ `sub_40FEC0` | done | HandleObjectCollisionAt_482CC0 added (CarPhysics_B0.hpp), v13 hoisted; 4867E0=ResolveCollisionWithPed (called); diff 52->17 (left: 5229B0 a4 is s32 but orig passes unextended, tail merge) |
| 0x5238B0 | `Object_2C::HandleSpriteZCollision_5238B0` | 0x4843A0 | ✓ `sub_466CF0`, ✓ `sub_420420`, `sub_4699A0` (10.5 0x4E4F40), ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_483100`, `sub_4BD670`, `sub_4207B0`, `sub_483500`, ✓ `sub_482BE0` | done | set_xyz_lazy_420600 x2; 4699A0/483100/4BD670/4207B0/483500 are called in 10.5 (pairing noise); diff 85->85 |
| 0x523BF0 | `Object_2C::sub_523BF0` | 0x486E90 | ✓ `sub_482BE0`, ✓ `sub_4637B0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `sub_466CF0`, ✓ `PolarToCartesian_41FC20`, `sub_484260`, `sub_483100`, `sub_4BD670`, `sub_4207B0` | checked | 484260=HandleSpriteGroundAndCollisionSimple_523770, 483100=522FA0, 4BD670=5A2500, 4207B0=get_x_y_443580: all still called in 10.5 (pairing noise) |
| 0x524630 | `Object_2C::IntegrateHorizontalMovementAndCollisions_524630` | 0x4874D0 | ✓ `sub_4637B0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_466CF0`, `sub_483100`, ✓ `sub_420420`, `sub_4BD670`, `sub_4824E0`, `sub_4207B0`, `sub_483500` | done | 4637B0, 416B40, 4206F0, 482A30, 41FC20, 466CF0 x3, 420420 used; Fix16::operator++(int) added (4824E0); 483100/4BD670/4207B0/483500 called in 10.5; diff 518->349 |
| 0x525370 | `Object_2C::ShouldCollideWithSprite_525370` | 0x4835E0 | `sub_483570`, `sub_421050`, ✓ `sub_40FEB0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_482C90`, ✓ `sub_416B40`, ✓ `sub_40FEC0`, ✓ `check_is_shop_421060` | matched | sub_482C90 used; 483570=525290 (called), 421050=get_field_26 (already); diff 49->43 |
| 0x527070 | `Object_2C::UpdateMovementAndEffects_527070` | 0x486130 | `sub_482F80`, ✓ `RotateVector_41FC90`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_482D30`, ✓ `sub_416B40`, ✓ `sub_420660` | checked | 482F80 = PoolTake_522360, still called in 10.5 |
| 0x527F10 | `Object_2C::sub_527F10` | 0x484760 | ✓ `sub_47F4F0`, `sub_483FC0`, `sub_484000` | matched | 483FC0/484000 are the pool DeAllocate inlines, already used |
| 0x5283C0 | `Object_2C::TickObject_5283C0` | 0x485760 | ✓ `sub_482400`, ✓ `sub_420F10`, `sub_482790`, ✓ `sub_41E210`, `Object_2C::sub_4826A0` (10.5 0x525AE0), `Object_3C_Pool::sub_483FE0`, `sub_483FC0`, `sub_484000`, `Object_8_Pool::sub_483FA0`, ✓ `sub_4206C0`, `sub_482F80` | done | 482400 (branches swapped), 420F10, 41E210 used; pool Allocate/DeAllocate already used; 482790/482F80/4826A0 are calls in 10.5; diff 321->301 |
| 0x528E50 | `Object_2C::HandleImpact_528E50` | 0x486410 | `sub_482F60`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `check_is_shop_421060`, `sub_410480`, `sub_410460` | checked | 482F60=PoolGive_522340, 410480/410460=Rozza Type3_40BDD0: all calls in 10.5 |
| 0x534700 | `Garage_48::ParkCarAtDoor_534700` | 0x489BC0 | ✓ `sub_447ED0`, ✓ `sub_489600`, `sub_432860` | inlines added | diff 191->187; 447ED0 commented (different constant), 432860 point setter commented |
| 0x5349D0 | `Garage_48::GaragesService_5349D0` | 0x489680 | matched | inlines added CloseDoors_476A30, StopMoving_4895D0, Reset_489650; matched 2026-10-03 | diff 283->208; f78 clears, Door 476A30, 4895D0 physics stop, 489650 reset, point helpers left |
| 0x5384C0 | `Particle_4C::sub_5384C0` | 0x48AE70 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | inlines added | Sprite::SetFlags_4337D0 added+used; RotateByAngle_40F6B0 commented (192->198); diff 192->192 |
| 0x539040 | `Particle_4C::sub_539040` | 0x48B540 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0`, `sub_4BDEF0` | done | SetFlags_4337D0 used; 4BDEF0 = ApplyScaleToDimensions_59E4C0 (called); diff 125->125 |
| 0x539480 | `Particle_4C::sub_539480` | 0x48B9F0 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | SetFlags_4337D0 used; diff 127->127 |
| 0x539890 | `Particle_4C::sub_539890` | 0x48F650 | ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_433800`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_40F6B0`, ✓ `sub_4337F0` | done | SetFlags_4337D0 + Clear_2C_0x4_Flag_433800 (added) used; diff 188->184 |
| 0x53B670 | `Particle_4C::sub_53B670` | 0x48FE90 | ✓ `sub_416B40`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `sub_4206C0`, `sub_40F540`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_4337D0` | inlines added | SetFlags_4337D0, CheckBit0_433B40, PolarToCartesian_41FC20 x3 used; Fix16_To_Ang16_40F540 left open-coded (7->9); diff 7->7 |
| 0x53E450 | `Particle_8::sub_53E450` | 0x48C9C0 | ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0` | done | all 9.6f inlines used |
| 0x53E970 | `Particle_8::GunMuzzelFlash_53E970` | 0x48CD10 | ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, `sub_48A930`, `sub_48A950`, ✓ `sub_432860`, ✓ `sub_40F6B0`, `sub_4207B0`, `sub_40F680`, ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_4337F0`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_40FEA0` | inlines added | get_car_width/height (48A930/48A950), SetXY_432860, operator+= (40F680), PolarToCartesian_41FC20 used; 4337F0 commented (both uses together 672->818); diff 819->658 |
| 0x53FE40 | `sub_53FE40` | 0x48DB00 | `sub_40F790`, ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | checked | 40F790 is atan2 on (sinv,cosv), which 9.6f passes as one Fix16_Point by value; ours has two Fix16 params (signature not changed), atan2_fixed_405320(cosv, sinv) is the same body |
| 0x5406B0 | `Particle_8::SpawnCigaretteSmokePuff_5406B0` | 0x48E060 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_4337F0` | done | all 9.6f inlines used |
| 0x541430 | `Wolfy_30::state_5_541430` | 0x48E8B0 | ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | RotateByAngle_40F6B0 on a Fix16_Point, SetFlags_4337D0; branch/store order fixed; diff 161->19 |
| 0x541850 | `Wolfy_30::TimerAfter50Handler_541850` | 0x490D60 | `struct_4::ctor_424620`, `sub_48A420`, `sub_48A390`, `sub_48EA50`, `sub_4BEE10`, ✓ `sub_40FEA0`, ✓ `sub_48A4C0`, ✓ `sub_420F10`, `sub_40E8D0`, ✓ `MaxAbsDistance_42A6B0`, ✓ `sub_40FEB0`, ✓ `IsMaxDamage_40F890`, ✓ `Car_BC::sub_403BA0`, `sub_425D60`, `sub_426F00`, `sub_4207B0`, ✓ `sub_40FEC0` | inlines added | get_ped_state_1_48A4C0 added; MaxAbsDistance_42A6B0 x2 + GetPedId_420F10 x1 used, 1 MaxAbs + 1 GetPedId commented (worse); 48A420/48A390/48EA50/4BEE10/425D60/426F00 are calls in 10.5; Fix16::Abs/Max now emitted as COMDAT calls; diff 246->240 |
| 0x541D60 | `Wolfy_30::state_18_33_541D60` | 0x48EB00 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `Sprite_Pool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0`, `sub_4BDEF0` | done | has_pStart_48A8F0, Fix16_To_Ang16_40F540 (was 482740) used; 48A900 = pool Allocate (already), 4BDEF0 = ApplyScale (called); diff 246->156 |
| 0x542060 | `Wolfy_30::state_19_32_542060` | 0x48ED40 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `Sprite_Pool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0`, `sub_4BDEF0` | done | has_pStart_48A8F0, Fix16_To_Ang16_40F540, PolarToCartesian_41FC20; diff 270->264 |
| 0x542340 | `Wolfy_30::state_20_542340` | 0x48EF90 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `Sprite_Pool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0` | done | has_pStart_48A8F0, Fix16_To_Ang16_40F540, PolarToCartesian_41FC20; diff 260->248 |
| 0x543800 | `Wolfy_7A8::New_543800` | 0x48A6C0 | `Wolfy_30::sub_48A550` | checked | 48A550 = Wolfy_30::Init_543650, still called in 10.5 |
| 0x544FF0 | `Char_B4::ctor_544FF0` | 0x497A60 | `struct_4::ctor_424620` | checked | struct_4 ctor (424620) already inline; remaining diff (19, with needle ??0Char_B4) is an EH frame: orig struct_4 seems to have a destructor (not changed) |
| 0x546360 | `Char_B4::UpdateAnimState_546360` | 0x497DF0 | ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_420700`, ✓ `sub_4206C0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, `sub_493940`, ✓ `sub_421360`, ✓ `sub_40F7B0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_492CB0`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::sub_4039D0`, ✓ `cool_nash_0x294::sub_403B50`, ✓ `sub_433A30`, ✓ `sub_433910`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `sub_433A50`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `Car_10::set_obj_421380`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70), `sub_472C60`, `sub_4BDEF0` | done | IsStateActive_421360, SetPaletteSprites_420700, set_field_140_492CB0 added; set_field_14C_403AE0, get_field_226_4039D0, IsGradientSlopeAt_466CF0 x2 used; 493940/466B70/472C60/4BDEF0 are calls in 10.5; diff 2285->1931 |
| 0x548840 | `Char_B4::HandleObjectCollision_548840` | 0x4993B0 | ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_482C90`, ✓ `sub_482C80`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_4867E0` | done | is_not_type6_to_12_421080, sub_482C90, GetMass_482C80 used; 4867E0 = ResolveCollisionWithPed (called); diff 55->13 |
| 0x54A530 | `Char_B4::HandleGenericCollision_54A530` | 0x4948C0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_41FAC0`, ✓ `sub_493540`, ✓ `sub_420590`, ✓ `sub_447E10`, `sub_40E8D0`, ✓ `sub_492170`, ✓ `sub_492C20`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_42A720`, ✓ `sub_492CE0`, ✓ `sub_492CF0`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `set_xy_lazy_447E20` | done | get_field_230_492C20 added; 41FAC0=GetCornerAngle_4403A0, 40E8D0=atan2 (called); code unchanged (716) |
| 0x54DDF0 | `Char_B4::state_0_54DDF0` | 0x49A560 | ✓ `sub_42A630`, ✓ `sub_420420`, ✓ `sub_491F80`, `ApplyCarVelocityCameraOffset_436200`, ✓ `PolarToCartesian_41FC20`, `sub_436140`, ✓ `cool_nash_0x294::sub_403A40`, `sub_433970`, `sub_491FA0`, ✓ `cool_nash_0x294::get_objective_403A80`, `sub_4995A0`, `sub_495470` (10.5 0x495470), ✓ `sub_4923D0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_466CF0`, `sub_49A080`, ✓ `sub_4923A0`, `sub_4994D0` | inlines added | ClearBit11_403A40 x2 used; RegulateVelocity_433970 skipped (uses kFpOne256th_678620, code uses dword_6FD99C); rest are 10.5 calls; diff 556->556 |
| 0x54ECB0 | `Char_B4::CanStepForwardWithRegionCheck_54ECB0` | 0x495980 | ✓ `sub_466CF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_4BD670`, ✓ `sub_492130`, ✓ `sub_420420`, ✓ `sub_42A630`, `sub_492190` | checked | 492190 = CanStepForward_54FEC0, 4BD670 = CheckSpriteMovementRegion_5A2500: both called in 10.5 |
| 0x54EF60 | `Char_B4::CanStepDiagonal_54EF60` | 0x495BF0 | ✓ `PolarToCartesian_41FC20`, `sub_491EE0`, ✓ `sub_491F00`, ✓ `sub_491EF0` | checked | 491EE0 = Fix16::ToInt, already used |
| 0x5504F0 | `Char_B4::state_1_5504F0` | 0x49B0D0 | ✓ `sub_42A630`, ✓ `sub_420420`, ✓ `sub_491F80`, ✓ `sub_492140`, ✓ `sub_492CC0`, ✓ `sub_492FD0`, ✓ `MaxAbsDistance_42A6B0`, `sub_40E8D0`, ✓ `PolarToCartesian_41FC20`, `sub_496500`, `sub_492C30`, `angle::angle_not_equal_41CFF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_466CF0`, `sub_49A080` | done | get_field_130_492CC0 added x2; 41CFF0 = Ang16 operator!= (already); 492C30=CanStepInDirection_551350, 496500/49A080 called; diff 387->387 |
| 0x550F60 | `Char_B4::GetNextRotationToward_550F60` | 0x4928B0 | `sub_40EAB0` | checked | 40EAB0 = ComputeShortestAngleDelta_4056C0 wrapper, called in 10.5 (note: orig also calls Fix16::FromFix16_4516B0, the commented-out 'unused' line) |
| 0x551CB0 | `Char_B4::state_7_551CB0` | 0x49BD10 | matched | done | get_field_130_492CC0 used; 436140 = get_field8_45C900 (called); diff 99->91 |
| 0x5520A0 | `Char_B4::state_8_5520A0` | 0x496880 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `sub_40F7B0`, `sub_4937D0`, `sub_493810`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_48D1F0`, `sub_4BDDD0`, ✓ `sub_420660`, ✓ `sub_433B70`, `sub_4211A0`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_433B60`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `PolarToCartesian_41FC20`, ✓ `Car_3C::set_ang_lazy_420690`, `sub_4105B0`, ✓ `sub_434950` | inlines added | ClearBit11_403A40, get_health_433B70 used; Set_B4_F16_To_1_433B50 commented (115->192); 4937D0/493810/48D1F0/4BDDD0/4211A0/4105B0 map to 10.5 calls; diff 115->115 |
| 0x5538A0 | `Char_B4::HandleCarImpact_5538A0` | 0x497570 | ✓ `sub_40F790`, `sub_482790` | checked | 482790 = SetDamageOwner_529080, called in 10.5 |
| 0x554110 | `Orca_2FD4::Internel_CanMoveDiagonally_554110` | 0x49CB60 | ✓ `sub_466CF0` | matched | all 9.6f inlines used |
| 0x554AB0 | `Orca_2FD4::save_find_554AB0` | 0x49CF70 | ✓ `sub_466CF0` | done | all 9.6f inlines used |
| 0x5552B0 | `Orca_2FD4::maybe_find_5552B0` | 0x49D7A0 | ✓ `sub_466CF0`, ✓ `sub_420420` | done | all 9.6f inlines used |
| 0x559430 | `CarPhysics_B0::ShowPhysicsDebug_559430` | 0x4A1DA0 | ✓ `sub_49E240`, ✓ `Garox_C4::sub_45AFD0` | inlines added | ThetaText_49E240 + Garox_C4::SetDrawKind8_45AFD0 (no change); left: the lea of the DisplayText this is scheduled early in the theta call and late in the rear-skid call |
| 0x55A1D0 | `CarPhysics_B0::SetVelocityTowardTarget_55A1D0` | 0x49F760 | ✓ `sub_40F6B0`, `sub_40F600`, `sub_40F580` | checked | 40F600 = Point - (40AC80 called), 40F580 = Ang16_to_Fix16 (already) |
| 0x55AB50 | `CarPhysics_B0::ComputeSlopeCorrection_55AB50` | 0x49FBE0 | ✓ `is_on_trailer_421720`, `sub_40F600`, `sub_4634E0`, ✓ `sub_42A630` | matched | is_on_trailer_421720 used; 4634E0 skipped (map globals, 10.5 uses CarPhysics copies); 40F600 = Point - ; diff 96->96 |
| 0x55B7E0 | `EmitImpactParticles_55B7E0` | 0x49FF80 | ✓ `IsMaxDamage_40F890`, `sub_4102A0` (10.5 0x40BD10) | checked | 4102A0 pairs with Rozza Type5_40BD10; 10.5 calls Type4_40BC40 (pairing noise) |
| 0x55B970 | `CarPhysics_B0::ProcessGroundCollisionAndSurfaceType_55B970` | 0x4A26C0 | `sub_4BD490`, `sub_40F600`, ✓ `sub_42A630`, ✓ `sub_49EBE0` | done | GetBlockSurfaceType_49EBE0 (static inline in CarPhysics_B0.cpp) + gtx::sub_49E570 added; 4BD490 = CheckCornerZCollisions? (called), 40F600 = Point -; diff 308->308 |
| 0x55C5C0 | `CarPhysics_B0::HandleMapBoundaryCollisionY_55C5C0` | 0x4A3DF0 | `sub_4BCD00`, ✓ `sub_432860`, `sub_49E5A0`, `sub_4828C0`, ✓ `sub_40F6B0` | done | SetXY_432860 + operator-= (4828C0); 4BCD00/49E5A0 = edge / ComputeRelativePointVelocity (called); diff 88->87 |
| 0x55C820 | `CarPhysics_B0::HandleMapBoundaryCollisionX_55C820` | 0x4A3FB0 | `sub_4BCFA0`, ✓ `sub_432860`, `sub_49E5A0`, `sub_4828C0`, ✓ `sub_40F6B0` | checked | SetXY/-= already used; 4BCFA0/49E5A0 called in 10.5 |
| 0x55CBB0 | `CarPhysics_B0::ReplayAndDispatchCollision_55CBB0` | 0x4A4270 | ✓ `sub_40FEB0` | done | AsCar_40FEB0 used; diff 51->51 |
| 0x55F3B0 | `sub_55F3B0` | 0x4A05C0 | ✓ `sub_420360`, `sub_40F600`, `sub_49E420`, `sub_420390`, ✓ `sub_49E500`, `sub_49E0E0` | checked | 49E420 = Rotate90CCW_5605E0, 420390 = NormalizeSafe_442AD0 (called), 49E0E0/40F600 = Fix16/Point operators |
| 0x55FA60 | `CarPhysics_B0::ApplyImpactForcesAndDamage_55FA60` | 0x4A31B0 | ✓ `Car_BC::sub_411930`, ✓ `sub_4118D0`, `sub_4211C0`, `sub_426F00`, ✓ `sub_49EF50`, ✓ `sub_421260` | done | IsFlagSet_411930 added, is_driven_by_player (4118D0), AddDamage_49EF50, ClearHandBrake_421260 used; 4211C0 = GetLength_453590 & 426F00 = ApplyVisualDamage (called); diff 180->140 |
| 0x55FD00 | `CarPhysics_B0::HandleWorldCollision_55FD00` | 0x4A32D0 | `sub_420360`, `sub_4292F0`, ✓ `IsMaxDamage_40F890` | matched | IsNull (420360) used; 4292F0 = ApplyImpactDamage_43D5D0 (called); diff 63->63 |
| 0x55FF20 | `CarPhysics_B0::HandleCarCollision_55FF20` | 0x4A34A0 | `sub_49E5A0`, `sub_40F600`, `sub_420390`, `sub_49E360`, ✓ `sub_49EFE0`, ✓ `sub_4216E0`, ✓ `sub_49EFC0`, `sub_4A09B0`, ✓ `sub_4211A0`, `sub_423480`, ✓ `sub_41B0A0`, `sub_4292F0`, ✓ `Car_BC::sub_403BA0`, ✓ `IsMaxDamage_40F890` | done | GetLinearSpeed_4211A0, IsMaxDamage_40F890 used; 423480 = GetEffectiveDriver_43E990, 49E360/4A09B0/49E5A0 = called CarPhysics funcs/ops; diff 447->445 |
| 0x5606C0 | `CarPhysics_B0::HandleObjectCollision_5606C0` | 0x4A3A40 | `sub_482C30`, `sub_40F600`, ✓ `sub_482C90`, ✓ `sub_482C80`, `sub_49E5A0`, `sub_420390`, `sub_49E360`, `sub_4A09B0`, `sub_4292F0`, ✓ `IsMaxDamage_40F890` | done | GetMass_482C80 used; 482C30 = GetXY_52AE70 (called), rest operators/called funcs; diff 260->260 |
| 0x560B40 | `CarPhysics_B0::ProcessPedImpact_560B40` | 0x4A0A30 | ✓ `sub_4339E0`, ✓ `sub_4339C0`, `sub_49E5A0`, `sub_40F600`, ✓ `sub_433A00`, `sub_40F640` (10.5 0x43D5D0), ✓ `sub_49EF40` | done | get_sprite_xpos/ypos/zpos (4339C0/4339E0/433A00, noted) + get_field_140_49EF40 (added) used; 49E5A0 called, 40F600/40F640 operators; diff 207->189 |
| 0x560F20 | `CarPhysics_B0::ApplyMovementStep_560F20` | 0x4A2E30 | ✓ `sub_40F540`, `sub_40F680`, `sub_49ED00`, ✓ `Car_BC::sub_403BA0`, `sub_49FF70` | matched | operator+= (40F680) worse 103->109; 49ED00/49FF70 = UpdateCp1FromCm1/UpdateTrailerAlignment (called) |
| 0x561E50 | `CarPhysics_B0::CalculateFrontWheelForce_561E50` | 0x4A1130 | `sub_4A0F30`, ✓ `sub_432860`, ✓ `sub_40F540`, `sub_4A0D40` | commented | SetXY_432860 worse (214->216); 4A0F30/4A0D40 = ComputeEngineTorque/ApplyDriveForce (called) |
| 0x5620D0 | `CarPhysics_B0::CalculateRearWheelForce_5620D0` | 0x4A1360 | `sub_4A0F30`, ✓ `sub_432860`, ✓ `sub_49E3A0`, ✓ `sub_40F540`, `sub_4A0D40` | inlines added | SetXY_432860 used; MultiplyByFix16_49E3A0 commented (196->222); 4A0F30/4A0D40 called; diff 196->196 |
| 0x562910 | `CarPhysics_B0::StabilizeVelocityAtSpeed_562910` | 0x49EA70 | ✓ `sub_49E3A0`, ✓ `sub_40F6B0` | matched | all 9.6f inlines used; matched with an assigning `Multiply_408680` wrapper and a product-temp `x *=` |
| 0x562D00 | `CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00` | 0x4A1B20 | ✓ `sub_40F840`, ✓ `sub_40F790`, ✓ `sub_41E210`, ✓ `sub_49E480` | inlines added | IsVelocityAlignedWithHeading_40F840 now uses atan2_40F790 (0.699->0.944); all 9.6f inlines used |
| 0x563350 | `CarPhysics_B0::UpdateCenterOfMassPoint_563350` | 0x49ED60 | ✓ `sub_40F6B0` | matched | all 9.6f inlines used |
| 0x563460 | `CarPhysics_B0::UpdateReferencePoint_563460` | 0x49EDC0 | ✓ `sub_40F6B0` | matched | all 9.6f inlines used |
| 0x571A30 | `PoliceCrew_38::sub_571A30` | 0x4AB610 | ✓ `sub_4A9AD0`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `sub_421470`, ✓ `cool_nash_0x294::sub_4039F0` | inlines added | No codegen change; rest is block layout/tail merging of the 28/2C tails |
| 0x572920 | `PoliceCrew_38::State5_Searching_572920` | 0x4AC580 | ✓ `sub_421470`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, `sub_4048A0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, `sub_403960`, `cool_nash_0x294::sub_403AF0`, `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `sub_450CB0`, `MaxAbsDistance_42A6B0`, `Car_BC::sub_421D90`, ✓ `cool_nash_0x294::sub_403990` | done | diff 287->281; 4048A0/421D90 noise; MaxAbsDistance not used (10.5 out-of-line Negate/Max) |
| 0x574720 | `PoliceCrew_38::State6_ShutDown_574720` | 0x4ACE80 | ✓ `sub_414F20`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, `sub_403960`, ✓ `sub_450CB0` | done | diff 43 unchanged |
| 0x575310 | `PoliceCrew_38::sub_575310` | 0x4ABAE0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0`, `Car_BC::sub_421EC0` | done | diff 159 unchanged; MaxAbsDistance not used (10.5 out-of-line calls); 421EC0 noise |
| 0x575FF0 | `PoliceRoadblock_A4::CreateRoadblock_575FF0` | 0x4ADB70 | ✓ `sub_420420`, ✓ `sub_40F540`, `sub_426E40`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `Car_BC::sub_421560`, ✓ `sub_414F20` | done | diff 759 unchanged; 426E40 already used (Scaled variant) |
| 0x578390 | `Train_58::UpdatePassengerAI_578390` | 0x4AFB30 | ✓ `sub_411940`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `Char_8::sub_420EA0`, ✓ `cool_nash_0x294::set_occupation_403970` | matched | 122->93; IsEmpty_420EA0 with branches swapped to 9.6f order |
| 0x578860 | `PublicTransport_181C::SpawnTrainsFromStations_578860` | 0x4AFE20 | `sub_426E40`, ✓ `sub_420F30`, `sub_421510`, ✓ `Car_BC::sub_421560`, ✓ `sub_475C30`, ✓ `sub_426E00` | done | new Car_BC::SetField98To4_475C30 used (318 unchanged); 426E40/420F30/421560 already used, 421510 open-coded |
| 0x5794B0 | `PublicTransport_181C::SetupTrainAndBusStops_5794B0` | 0x4B08A0 | ✓ `sub_433470`, ✓ `sub_4334A0`, ✓ `sub_4334D0`, ✓ `sub_433500` | done | all 9.6f inlines used |
| 0x579CA0 | `PublicTransport_181C::BusesService_579CA0` | 0x4B0F20 | matched | checked | only 421D90 listed = GetCarLinearSpeed_43A240 (called in 10.5) |
| 0x57A7A0 | `PublicTransport_181C::PublicTransportService_57A7A0` | 0x4B1560 | ✓ `sub_4118D0`, ✓ `Car_BC::sub_421560`, `ApplyCarVelocityCameraOffset_436200`, `sub_4AF290`, `sub_4AF860`, `sub_4AF880` | done | new Ped::SetTrainStation_4AF860 (4AF880 identical) used (95 unchanged); 436200=get_fieldC_45C9B0, 4AF290=sub_5782D0 (called in 10.5) |
| 0x57DD50 | `sound_obj::ProcessType3_CopRadioAndMusic_57DD50` | 0x4B2D50 | `sub_4A65E0`, `sub_4B25A0`, ✓ `IsMaxDamage_40F890`, `sub_4B25D0` | checked | pairing noise: 10.5 still calls GetPlayerCar_5698E0 (4A65E0), IsTrainOrBoxcar_57F120 (4B25A0), sub_57DF10 (4B25D0) |
| 0x57E220 | `sound_obj::sub_57E220` | 0x4B1E40 | `sub_4A65E0`, `Car_BC::sub_41E450`, `Car_BC::sub_41E440`, `Car_BC::sub_41E430` | matched | paused/static-volume block layout differs (else placed later in 10.5); more diffs further down |
| 0x57E6C0 | `sound_obj::sub_57E6C0` | 0x4B2830 | `sub_4A65E0`, ✓ `sub_411940` | checked | 9.6f sub_4A65E0 = Player::GetPlayerCar_5698E0, still called in 10.5 |
| 0x582480 | `Car_14::SpawnTrafficCar_582480` | 0x4B34E0 | ✓ `sub_41FE40`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `Zone_144::sub_45DD50`, `sub_4B3230`, `sub_4B33F0`, `sub_4B30A0`, `sub_42A620`, ✓ `sub_426E40`, ✓ `sub_420700`, ✓ `sub_426E00`, ✓ `sub_421490` | inlines added | 893 unchanged; new SetPaletteSprites_420700, HasKillRespectChange_45DD50, Car_BC::sub_421490, + sub_426E00 used; SpawnCarAtCorrectZ (426E40) commented (893->907); 4B3230/4B33F0/4B30A0/42A620 not done |
| 0x5832C0 | `Car_14::MakeTrafficForCurrCamera_5832C0` | 0x4B4A60 | ✓ `sub_4B3110`, ✓ `sub_4B3130` | done | all 9.6f inlines used |
| 0x588620 | `RouteFinder::ShowJunctionIds_588620` | 0x40D120 | `DrawUnk_0xBC::sub_40CF60`, `DrawUnk_0xBC::sub_40CFC0` | commented | ProjectToScreen = WorldToScreen_40CFC0 but original uses RouteFinder's own constants 0x6FFC7C/9C, so noted; 40CF60 = IsPointInBoundaries_58CF10 called |
| 0x592660 | `eager_benz::sub_592660` | 0x4B7EB0 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_41DC40`, `Shooey_CC::sub_44A370`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier_4766A0 (new), IsUser_41DC70, GetPlayerPed_4A5130, Add_2D4, Ped get_remap/get_cam_x/y/z; 259->125; Shooey sub_44A370 = sub_485140 (still called); get_car_model already used |
| 0x596C90 | `ExplodingScore_50::DrawNumbers_596C90` | 0x4B94B0 | ✓ `DrawUnk_0xBC::sub_4B90E0`, `CokeZero_50::sub_4B92B0` | matched | private out-of-line copy of 9.6f inline 4B90E0 (Camera::ProjectWorldToScreen_OutOfLine_4B90E0); CokeZero_50::sub_4B92B0 is DrawSingleNumber_597100, called |
| 0x59FB10 | `Sprite::IntersectsRectSAT_59FB10` | 0x4BB020 | ✓ `sub_4BA0A0`, ✓ `sub_45ADD0`, ✓ `sub_45ADB0`, ✓ `sub_42A720`, ✓ `sub_45ADA0`, ✓ `sub_45ADC0` | done | all 9.6f inlines used |
| 0x5A0380 | `Sprite::RotatedRectCollisionSAT_5A0380` | 0x4BB560 | ✓ `sub_41E390`, ✓ `sub_4BA0A0`, ✓ `sub_42A720` | matched | all 9.6f inlines used |
| 0x5A2710 | `Sprite::FindCollisionIntersectionPoint_5A2710` | 0x4BD8A0 | matched | checked | 4207B0=get_x_y_443580 and 4BB3C0=FindOverlappingBoundingBoxCorners_5A0150 are called in 10.5; 49E360 is Fix16_Point operator/ (used) |
| 0x5A3550 | `Sprite_4C::UpdateRotatedBoundingBox_5A3550` | 0x4BBD40 | ✓ `sub_432860`, ✓ `sub_40F6B0`, ✓ `sub_45ADB0`, `sub_45ADA0`, `sub_45ADD0`, `sub_45ADC0`, `sub_4B9E60` | inlines added | SetXY_432860 x21 (836->638); rect getters 45ADB0.. commented (->648); 4B9E60 = FindMinMax_5A57E0 |
| 0x5A7080 | `struct_4::CleanupSpriteList_5A7080` | 0x4BF070 | ✓ `sub_416B40`, ✓ `sub_4BE830`, `sub_485260`, `Sprite_18_Pool::sub_4BEC50` | checked | 485260 = Object_5C::RemoveAndFree_52A610 (called), 4BEC50 = DeAllocate (already) |
| 0x5AA9A0 | `gtx_0x106C::load_car_info_5AA9A0` | 0x4C0410 | `sub_4C03F0` | checked | Door count/offset arithmetic: 10.5 keeps num_remaps in edi then adds 0xE after the first read; not reproduced yet |
| 0x5B5BC0 | `text_0x14::InsertLineBreaksAndGetNumLines_5B5BC0` | 0x4C2450 | ✓ `sub_4C23D0`, ✓ `sub_4539D0` | inlines added | new GetSpaceWidth_4C23D0 (gtx_0x106C.hpp) 70->62; GetCharWidth_4539D0 noted (Draw.hpp include changes static init/dtor code) |
| 0x5B92E0 | `sharp_pare_0x15D8::ReadTextures_5B92E0` | 0x4C3040 | ✓ `gtx_0x106C::has_tiles_4C2EE0`, ✓ `gtx_0x106C::get_tile_4C2EB0` | done | all 9.6f inlines used |
| 0x5C1D00 | `TrafficLight_20::sub_5C1D00` | 0x4C3C70 | ✓ `sub_42A8C0`, `sub_483C20`, ✓ `sub_469010` (10.5 0x52B2A0), ✓ `sub_433530` | done | 482->466; IsBlockPavementTypeInlined_433530 used; 483C20 = Object_2C::SetSpriteIdOffset_5290C0 (called in 10.5) |
| 0x5C5CF0 | `Montana_4::AddSprite_5C5CF0` | 0x4C4BF0 | ✓ `sub_4C4B40` | done | all 9.6f inlines used |
| 0x5C5F10 | `Montana_4::dtor_5C5F10` | 0x4C4E60 | `Montana_2EE4::gdtor_4C4D80`, `Montana_FA4::gdtor_4C4DA0` | checked | gdtors 4C4D80/4C4DA0 are compiler scalar deleting dtors from delete |
| 0x5C8780 | `Car_214::sub_5C8780` | 0x4C4FE0 | ✓ `sub_416B40`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_433A20`, ✓ `sub_4C4F20`, ✓ `sub_4118B0`, ✓ `sub_433C20`, `sub_47ED20` | done | 150->145; AsCharB4_40FEA0, new Ped::GetGameObjectVelocity_433C20, get_id used; 47ED20 = sub_511A70 (called in 10.5) |
| 0x5CBD50 | `Kfc_30::UpdateStateMachine_5CBD50` | 0x4C55D0 | ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, `sub_421D80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `Car_BC::sub_421560` | matched | 442->439; SetUniNum_421560 used; 421D80 = Car_BC::HasSpriteZoom_43A230 (called in 10.5) |
| 0x5CFA70 | `Garox_107C_sub::DrawGangRespectBars_5CFA70` | 0x4C74F0 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x5D0620 | `Hud_Arrow_7C::sub_5D0620` | 0x4C7E60 | ✓ `sub_4C6F20`, ✓ `sub_4C7060`, ✓ `sub_4C6FB0`, ✓ `sub_432860` | matched | SetXY_432860 on one point x2; 67->53 |
| 0x5D1B10 | `Garox_C4::FormatAndSetupText_5D1B10` | 0x4C8AA0 | ✓ `Garox_C4::sub_4C70E0` | done | new Garox_C4::ClearAlpha_4C70E0 (byte store matches now); 15 unchanged |
| 0x5D2AB0 | `Hud_Pager_C::DrawPager_5D2AB0` | 0x4C9040 | matched | checked | sub_4C8CA0 pairs with 10.5 out-of-line 5D2380/5D2680 (called) |
| 0x5D3B80 | `Hud_Brief_704::DrawBrief_5D3B80` | 0x4C9430 | matched | done | GetLineSpacingFromFontType_5D7700_inlined + s32 ypos; 20->0 CANDIDATE MATCH (w.sh only) |
| 0x5D4A10 | `Hud_CarName_4C::sub_5D4A10` | 0x4C94F0 | matched | done | get_sprite_width_4C7220; stays 0 (candidate, w.sh only) |
| 0x5D61A0 | `DrawPlayerStatsHelper_5D61A0` | 0x4C9B40 | ✓ `sub_420220` | done | all 9.6f inlines used |
| 0x5D63B0 | `Garox_12E4_sub::DrawPause_5D63B0` | 0x4C9FA0 | matched | done | new Phi_8CA8::GetObjectPalette_4C6E30, get_sprite_width_420220; unchanged (37) |
| 0x5D7EC0 | `DrawFigure_5D7EC0` | 0x4CBA50 | ✓ `sub_432860`, ✓ `sub_40F6B0` | done | single point + SetXY_432860 x4; 412->345 |
| 0x5D8A10 | `DrawText_5D8A10` | 0x4CC100 | `magical_germain_0x8EC::sub_460DA0`, `magical_germain_0x8EC::sub_4CBA40`, `magical_germain_0x8EC::sub_4CBA00`, `sub_4CBA10`, `sub_460CC0`, `magical_germain_0x8EC::sub_4CBA20`, `sub_4CBA30`, `sub_460D30`, `sub_4BF550` | checked | 9.6f magical_germain helpers use a different layout (glyph v1/v2 at 0x24/0x25); no clear 10.5 open-coded form, left |
| 0x5DCF60 | `Weapon_30::spawn_bullet_5DCF60` | 0x4CDA90 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `sub_420B50`, `sub_482790`, `sub_483C20`, `sub_4BD670` | checked | 482790/483C20/4BD670 = SetDamageOwner_529080/SetSpriteIdOffset_5290C0/CheckSpriteMovementRegion_5A2500, all called in 10.5 (noise) |
| 0x5DD860 | `Weapon_30::pistol_5DD860` | 0x4CE070 | ✓ `Weapon_30::sub_4CCA80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_41E210`, ✓ `Weapon_30::sub_4CCA30`, ✓ `sub_41B0A0`, `sub_4CD000`, ✓ `sub_4CCA90` | done | diff 58 unchanged; 4CD000 = TickReloadSpeed_5DCF40 (10.5 calls it twice, once per branch) |
| 0x5DDA70 | `Weapon_30::dual_pistol_5DDA70` | 0x4CE270 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_41E210`, ✓ `Weapon_30::sub_4CCA80`, ✓ `Weapon_30::sub_4CCA30`, ✓ `sub_41B0A0`, `sub_4CD000` | checked | 4CD000 = TickReloadSpeed_5DCF40 (noise) |
| 0x5DDD20 | `Weapon_30::smg_5DDD20` | 0x4CE4B0 | ✓ `Weapon_30::sub_4CCA80`, ✓ `sub_40F6B0`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_4CCA60`, ✓ `sub_41B0A0`, `sub_4CD000`, ✓ `sub_4CCA90` | done | diff 117->87 |
| 0x5DDFC0 | `Weapon_30::throwable_5DDFC0` | 0x4CE970 | ✓ `Weapon_30::sub_4CCA80`, ✓ `sub_4CCA90`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `sub_435C20` (10.5 0x408680), ✓ `sub_41B0A0`, ✓ `Weapon_30::sub_4CCA30`, ✓ `sub_420B50`, `sub_482960`, `sub_485500`, ✓ `sub_420360`, ✓ `sub_434130`, `sub_4CD000` | done | diff 178->159 |
| 0x5DFB60 | `Weapon_30::sub_5DFB60` | 0x4CF380 | `struct_4::ctor_424620`, `sub_435C20` (10.5 0x408680), `sub_4BEE10`, ✓ `sub_416B40`, `sub_40E8D0`, `MaxAbsDistance_42A6B0`, `sub_4207B0`, `sub_4CCBD0`, ✓ `sub_433BF0`, ✓ `sub_41B0A0`, `sub_425770`, `sub_426F00`, ✓ `Weapon_30::sub_4CCA80`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_4CCA60` | done | diff 465 unchanged; most listed are noise |
| 0x5E4EE0 | `WindowProc_5E4EE0` | 0x4D0A00 | matched | checked | 4D09D0=Frontend::SetInputEnabled_5E53C0, 481DC0/481D80/481DA0 = Bink 513760/5136D0/513720, all called in 10.5 |
| 0x5E5A30 | `Start_NetworkGame_5E5A30` | 0x4D0ED0 | `sub_409F90`, ✓ `sub_409C40`, `sub_405A40` | done | GetPlayerIdx_409C40 (57->45); 409F90=GetMaxPlayers_521350; 405A40 is a vtable store (vdtor) |

<!-- table STUB -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|

<!-- table MATCH -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|
| 0x405CE0 | `sub_405CE0` | 0x40ECB0 | ✓ `sub_40E790` | matched | all 9.6f inlines used |
| 0x408140 | `Car_A4_10::sub_408140` | 0x40FB20 | `sub_427450` | checked | 427450 = a called Car_BC method (ManageDrowning_43E560 or physics state machine) |
| 0x4081B0 | `Car_A4_10::DeAllocateCarPhysics_4081B0` | 0x40F470 | `sub_426120` | checked | 426120 = DeAllocateCarPhysics_441A10 (called) |
| 0x408220 | `Car_A4_10::sub_408220` | 0x40FBE0 | `sub_40F490`, ✓ `sub_40F7B0`, `sub_40FB70` | checked | 40F490 = Car_BC_Pool::Remove (already), 40FB70 = ExplodeBothIfOneDestroyed_4081D0 (called), set_num_40F7B0 already |
| 0x40B890 | `Rozza_A::sub_40B890` | 0x40FF20 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40B980 | `Rozza_A::sub_40B980` | 0x410010 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BA60 | `Rozza_A::sub_40BA60` | 0x4100F0 | ✓ `sub_40FF00`, ✓ `sub_40FEF0`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0` | done | Object_2C::GetDefField63_40FF00 (second use); Still matches. |
| 0x40BBA0 | `Rozza_C88::OtherType_40BBA0` | 0x410210 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FF10` | done | all 9.6f inlines used |
| 0x40BC40 | `Rozza_C88::Type4_40BC40` | 0x410370 | ✓ `sub_40FF10`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BD10 | `Rozza_C88::Type5_40BD10` | 0x4102A0 | ✓ `sub_40FF10`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BE00 | `Rozza_C88::ctor_40BE00` | 0x4104B0 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler vector ctor helper |
| 0x40BE40 | `Rozza_C88::dtor_40BE40` | 0x410440 | ✓ `root_sound::DestroySoundObj_40FE60` | done | all 9.6f inlines used |
| 0x40EF40 | `root_sound::CreateSoundObject_40EF40` | 0x410750 | ✓ `root_sound::sub_410730` | done | root_sound::PopFree_410730 (new); its 9.6f callee infallible_turing::sub_4106D0 isn't dumped, only field_8 = 0 assumed (TODO); code unchanged |
| 0x40F010 | `root_sound::sub_40F010` | 0x410560 | `sound_obj::sub_4B2F20` | checked | 9.6f sound_obj::sub_4B2F20 = InitMusicAndCopRadio_57E960 (10.5 calls it) |
| 0x412490 | `sound_obj::ProcessType2_412490` | 0x411A50 | `sub_4B6700` | checked | 9.6f sub_4B6700 = cSampleManager::OpenStream_58E320 (still called) |
| 0x412740 | `sound_obj::ProcessType1_Sprite_412740` | 0x4162C0 | ✓ `sub_4117B0`, `sub_411730`, ✓ `sub_40FEA0`, ✓ `sub_40FEB0`, ✓ `sub_40FEC0`, `sub_41B030` | checked | 9.6f sub_41B030 is a Char_B4 dispatcher (4181E0/41A3C0, not paired), 10.5 calls ProcessPed_422B70; GetXYZ_4117B0 already used; 411730 still called |
| 0x412820 | `sound_obj::ProcessType8_Crane_412820` | 0x411D20 | ✓ `sub_411A00`, ✓ `sub_4117B0`, `sub_411730` | matched | GetRotorSprite_411A00 + GetXYZ_4117B0, 86->52; sub_411730 = ComputeEmitterDistanceSquared_4190B0 (still called in 10.5, pairing noise) |
| 0x412A60 | `sound_obj::ProcessType9_Crusher_412A60` | 0x412010 | ✓ `sub_411A10`, ✓ `unknown_libname_20`, ✓ `sub_411A30`, `sub_411730` | done | Crusher_30::get_state_411A10 + get_ypos_411A30 (new); code unchanged; 411730 still called |
| 0x412B80 | `sound_obj::sub_412B80` | 0x415DF0 | ✓ `sub_4118D0`, ✓ `sub_411940`, `sub_4156B0` | checked | 9.6f sub_4156B0 is a thunk to ProcessTrain_413BE0 (4143B0); GetCarInfoIdx_411940 already used |
| 0x413B90 | `sound_obj::sub_413B90` | 0x415C70 | `sub_412680`, `sub_412830`, `sub_4143B0` (10.5 0x413BE0), `sub_412A20` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413BE0 | `sound_obj::sub_413BE0` | 0x4143B0 | `sub_411940`, `sub_4BF210` (10.5 0x5AA3D0), `Car_BC::sub_41F880` (10.5 0x43B360), `sub_411870`, `sub_4116B0` (10.5 0x419020), `sub_4116F0` (10.5 0x419070), `sub_413DF0` (10.5 0x417D70), `sub_4166E0` (10.5 0x41A650), `sub_4B6020` (10.5 0x58DBF0), `sub_4171A0` (10.5 0x41A850), `sub_411890` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413BF0 | `sound_obj::sub_413BF0` | 0x415CC0 | `sub_412BD0`, `sub_413160`, `sub_4156C0`, `sub_412D80`, `sub_4143B0` (10.5 0x413BE0) | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413C50 | `sound_obj::sub_413C50` | 0x415D20 | `sub_413160`, `sub_414030`, `sub_414200`, `sub_4148D0`, `sub_4143B0` (10.5 0x413BE0), `sub_415A90`, `sub_4139A0`, `sub_414780`, `sub_413EB0`, `sub_412EB0` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413D10 | `sound_obj::sub_413D10` | 0x4123C0 | ✓ `sub_411950`, ✓ `IsMaxDamage_40F890`, ✓ `sub_411940` | matched | Car_BC::TakeFieldAC_411950 added; 54->54 |
| 0x4145E0 | `sound_obj::GetCar_4145E0` | 0x4129D0 | ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x415480 | `sound_obj::sub_415480` | 0x412FE0 | ✓ `sub_4119D0` | done | Car_BC::TakeWeaponKind_4119D0 (new); code unchanged |
| 0x415570 | `sound_obj::sub_415570` | 0x415880 | ✓ `sub_4118F0`, ✓ `sub_414F20`, ✓ `sub_414F80`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `?_GetResult@?$_Task_impl@_N@details@Concurrency@@QAE_NXZ`, ✓ `sub_411970`, ✓ `sub_411940`, ✓ `sub_411990` | done | Car_BC::get_B8_4119F0 (new); code unchanged |
| 0x4177D0 | `sound_obj::sub_4177D0` | 0x413BD0 | ✓ `Car_BC::sub_4118C0` | matched | all 9.6f inlines used |
| 0x417A00 | `sound_obj::Type_4_417A00` | 0x413D40 | ✓ `sub_411940`, ✓ `sub_411970` | done | all 9.6f inlines used |
| 0x417D70 | `sub_417D70` | 0x413DF0 | ✓ `sub_411940` | matched | Use GetCarInfoIdx_411940; 'a2 ? 27 : 28' gives the neg/sbb/add |
| 0x418B60 | `sound_obj::ProcessType11_HudPager_418B60` | 0x414E50 | ✓ `sub_411A40` | done | Hud_Pager_C::get_timer_411A40 (new); code unchanged |
| 0x419FA0 | `sound_obj::AddSoundObject_419FA0` | 0x417DB0 | ✓ `sub_416B40`, ✓ `ctor_416C00` | done | sound_unknown_0xC empty ctor (9.6f 0x416C00); get_type_416B40 already used; code unchanged (ImGuiDebug.cpp shows a DIFF in an unmarked function that also showed before, unrelated) |
| 0x41A090 | `sound_obj::FreeSoundEntry_41A090` | 0x416C10 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x41A3F0 | `sound_obj::CalcVolume_41A3F0` | 0x416DA0 | ✓ `sub_410BF0` | done | Round_To_Int_410BF0 + s32 clamp + reuse a3 like 9.6f; 31->0 CANDIDATE MATCH |
| 0x41A730 | `sound_obj::InterrogateAudioEntities_41A730` | 0x417030 | ✓ `sub_416B70`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::sub_416B80`, ✓ `sub_416BB0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | Car_BC::GetXYZ_416B80 (new) + get_car_rotation_416BB0 + get_car_416B60 + get_cam_z; 4->0 CANDIDATE MATCH |
| 0x41B540 | `sound_obj::AdjustSamplesVolume_41B540` | 0x417CE0 | `sub_416940` | checked | 9.6f sub_416940 = FromFix16_41B520, already called |
| 0x426790 | `sound_obj::sub_426790` | 0x41C600 | `nullsub_4` (10.5 0x426670), `sub_4A65E0` | checked | nullsub_4 is an empty method (10.5 nullsub_74 0x426670, no body to add); sub_4A65E0 = GetPlayerCar_5698E0, already called |
| 0x426F20 | `sound_obj::sub_426F20` | 0x41C350 | ✓ `sub_411940`, ✓ `sub_41C1F0` | done | all 9.6f inlines used |
| 0x42D870 | `DoBrianTest_42D870` | 0x41D0B0 | `sub_40E030` (10.5 0x58A190), ✓ `sub_41D020`, `sub_451510` | checked | sub_40E030 is a StartRoute copy and sub_451510 a large callee; source already calls the 10.5 functions, nothing open-coded |
| 0x430C70 | `sub_430C70` | 0x41D620 | `sub_41D580` | checked | 41D580 (160 bytes) is a helper 10.5 keeps out of line / pairing |
| 0x431E30 | `sad_mirzakhani::sub_431E30` | 0x41DCC0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x431FE0 | `sad_mirzakhani::alloc_next_431FE0` | 0x41DE40 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4355D0 | `Camera_0xBC::IsSpriteTheCameraSubject_4355D0` | 0x41E480 | ✓ `sub_40FEB0`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_40FEA0` | done | AsCar_40FEB0, get_car_416B60, AsCharB4_40FEA0; still matches |
| 0x435A20 | `Camera_0xBC::ReturnOwnerVelocity_435A20` | 0x41DFC0 | `Car_BC::sub_421D90` | checked | sub_421D90 is GetCarLinearSpeed_43A240 (pairing error, 10.5 calls it) |
| 0x435A70 | `Camera_0xBC::IsCoordsPosVisible_435A70` | 0x41E710 | ✓ `DrawUnk_0xBC::sub_40CFC0` | done | all 9.6f inlines used |
| 0x435D20 | `Camera_0xBC::sub_435D20` | 0x41EA10 | `sub_41E540` | checked | sub_41E540 is 10.5 sub_435840 (called out of line) |
| 0x435FF0 | `Camera_0xBC::sub_435FF0` | 0x41F2F0 | `sub_4727E0`, `sub_41EB00` | checked | sub_4727E0 = SmoothApproachClamped_4F75D0, sub_41EB00 = sub_436140, both called in 10.5 |
| 0x4364A0 | `Camera_0xBC::sub_4364A0` | 0x41EE30 | ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `sub_41E130`, ✓ `sub_41E3D0` | matched | get_x/y/z, sub_41E130 used; SetCamera_41E3D0 commented (26->38); stays 26 |
| 0x436540 | `Camera_0xBC::UpdateFollowPedCamera_436540` | 0x41F410 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `sub_41E130`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_41E3D0` | done | GetPedState_403990, sub_41E130 in ped branch; still matches |
| 0x436860 | `Camera_0xBC::ApplyZOffsetToScreenPosition_436860` | 0x41F0D0 | ✓ `cool_nash_0x294::sub_416B50` | done | all 9.6f inlines used |
| 0x4368E0 | `Camera_0xBC::ctor_4368E0` | 0x41F580 | `sub_41E3D0`, `DrawUnk_0xBC::CommitCameraTarget_41E410` | checked | sub_41E3D0 = out-of-line sub_4397D0 in 10.5; CommitCameraTarget_41E410 called directly from the ctor |
| 0x439CD0 | `Car_Door_10::sub_439CD0` | 0x421A00 | `maybe_flags::clear_420DE0`, ✓ `maybe_flags::set_420DC0` | commented | BitSet32 clear_bit/set_bit, but the param is u32* (changing it would change the signature) |
| 0x439D40 | `Car_Door_10::sub_439D40` | 0x421A80 | `maybe_flags::clear_420DE0`, `maybe_flags::set_420DC0` | commented | same as 0x439CD0 |
| 0x439DA0 | `Car_Door_10::sub_439DA0` | 0x421AF0 | ✓ `cool_nash_0x294::sub_403B60` | done | all 9.6f inlines used |
| 0x43A120 | `Car_BC::get_mass_43A120` | 0x421CF0 | ✓ `sub_4215C0`, ✓ `sub_421910` | done | all 9.6f inlines used |
| 0x43A3E0 | `Car_BC::GetOrientationAngle_43A3E0` | 0x421E00 | ✓ `sub_4211E0` | done | new CarPhysics_B0::GetLinvelAngle_4211E0, code unchanged |
| 0x43A450 | `Car_BC::get_linvel_43A450` | 0x421E70 | ✓ `Car_BC::sub_403BA0`, `sub_421150` | checked | 421150 = get_linvel_447010 (out-of-line in 10.5) |
| 0x43A6F0 | `Car_BC::IsNotCurrentRemap` | 0x421F40 | ✓ `sub_41C1F0` | done | get_remap_41C1F0, code unchanged |
| 0x43A7D0 | `Car_BC::AssignRandomRemap_43A7D0` | 0x422020 | ✓ `sub_4212D0`, ✓ `sub_420700` | done | GetRngValue_4212D0, SetPaletteSprites_420700, code unchanged |
| 0x43A850 | `Car_BC::GetCarModelForPhysics_43A850` | 0x4220A0 | ✓ `Car_BC::sub_4214F0`, ✓ `Car_BC::sub_403BA0` | done | sub_4214F0 used, code unchanged |
| 0x43A9A0 | `Car_BC::SetDriver` | 0x4221D0 | ✓ `sub_421270` | done | all 9.6f inlines used |
| 0x43ADC0 | `Car_BC::ProcessCarToCarImpact_43ADC0` | 0x42A360 | ✓ `sub_40FEB0`, ✓ `IsMaxDamage_40F890`, `sub_4230D0`, `sub_422F00`, ✓ `Car_BC::sub_411900`, `sub_4292F0`, ✓ `sub_4118B0`, ✓ `sub_41B0A0`, ✓ `sub_421170`, `sub_4BED60` | done | AsCar_40FEB0, is_player_41B0A0, new CarPhysics_B0::set_linvel_421170; code unchanged; 4230D0/422F00/4292F0 = IsAreaDamaged/DamageArea/ApplyImpactDamage (called) |
| 0x43AF10 | `Car_BC::CanExitCar_43AF10` | 0x4222A0 | `sub_421D80` | checked | 421D80 = HasSpriteZoom_43A230 (called) |
| 0x43AF40 | `Car_BC::sub_43AF40` | 0x4222D0 | ✓ `sub_421550` | done | new Car_BC::ClearA6Bit5_421550, code unchanged |
| 0x43AF60 | `Car_BC::sub_43AF60` | 0x4222F0 | ✓ `sub_421540` | done | SetA6Bit5_421540, code unchanged |
| 0x43AFE0 | `Car_BC::IsDoorAccessible_43AFE0` | 0x425A40 | ✓ `sub_4204D0`, `sub_4BA7E0` | checked | 4BA7E0 is a 10.5 out-of-line call (pairing) |
| 0x43B140 | `Car_BC::sub_43B140` | 0x425B60 | `Car_BC::sub_421D90`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_420420` | matched | PolarToCartesian already used; 421D90 = GetCarLinearSpeed_43A240 |
| 0x43B540 | `Car_BC::sub_43B540` | 0x4224A0 | ✓ `sub_41FE70` | done | all 9.6f inlines used |
| 0x43B770 | `Car_BC::sub_43B770` | 0x422670 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x43B7B0 | `Car_BC::AssignDriverBlameForExplosion_43B7B0` | 0x425CA0 | ✓ `sub_420C30` | done | CANDIDATE MATCH (92->0); file-static IsPlayerPedId_420C30 (9.6f method of unknown global 0x5E5BC0) + rewrite without gotos |
| 0x43BC30 | `Car_BC::SetupCarPhysicsAndSpriteBinding_43BC30` | 0x425D90 | `Car_BC::sub_423A50`, `sub_4A1D30`, `CarPhysics_B0::sub_49EE10` (10.5 0x563670), ✓ `Car_BC::sub_403BA0` | checked | 423A50/4A1D30/49EE10 = AllocCarPhysics_4419E0/SetCar_5638C0/SetSprite_563560 (called in 10.5) |
| 0x43BD00 | `Car_BC::DeAllocateCarPhysics_43BD00` | 0x426F40 | `sub_426120` | checked | 426120 = DeAllocateCarPhysics_441A10 (called) |
| 0x43C1C0 | `Car_BC::PrepareForExplosion_43C1C0` | 0x426F60 | `sub_425ED0`, ✓ `sub_414F20`, `sub_425E60` | inlines added | HasEmergencyLights_414F20 used, code unchanged; 425ED0/425E60 = StopLightFlashing_43CBE0/HeadlightsOff_43C0C0 (called) |
| 0x43C310 | `Car_BC::ResetTopRightRoofLight_43C310` | 0x422980 | ✓ `sub_4216A0`, ✓ `sub_420E20`, ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00` | checked | inline_check_0x80/mask_bit/clear_bit already used; 4BE9C0/483A00 = FindObject2CByModel_5A6A90/Light_527990 (called) |
| 0x43C650 | `Car_BC::ResetRoofLights_43C650` | 0x422B70 | ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00`, ✓ `sub_4217A0`, ✓ `sub_4118F0` | checked | same as 0x43C310 |
| 0x43C840 | `Car_BC::ResetBottomLeftRoofLight_43C840` | 0x422A40 | `sub_4216A0`, `sub_420E20`, ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00` | checked | pairing to a different 9.6f function; calls match 10.5 |
| 0x43C920 | `Car_BC::ActivateEmergencyLights_43C920` | 0x422D20 | ✓ `sub_411920`, ✓ `maybe_flags::set_420DC0` | done | all 9.6f inlines used |
| 0x43C9D0 | `Car_BC::DeactivateEmergencyLights_43C9D0` | 0x422D80 | `sub_422CB0`, ✓ `sub_411920`, ✓ `IsMaxDamage_40F890`, ✓ `maybe_flags::set_420DC0` | checked | is_FBI/IsMaxDamage/set_bit used; 422CB0 = a called reset function |
| 0x43D2C0 | `Car_BC::TryDamageArea_43D2C0` | 0x423180 | ✓ `IsMaxDamage_40F890`, `Car_BC::sub_411930`, `sub_422F00` | commented | sub_411930 flag helper (bool) breaks the match; 422F00 = DamageArea_43CF30 (called) |
| 0x43D400 | `Car_BC::sub_43D400` | 0x425F20 | ✓ `maybe_flags::clear_420DE0`, `sub_425590`, `sub_4213D0`, ✓ `sub_421570` | done | inline_check_0x2_info_421700 x2, inline_check_0x4_info_421660, new Car_BC::sub_421570; code unchanged |
| 0x43D7B0 | `Car_BC::TriggerExplosion_43D7B0` | 0x429370 | ✓ `IsMaxDamage_40F890`, `sub_423230` | done | CANDIDATE MATCH (16->0); 9.6f branch order, pass kZeroPoint_6778A8.x/.y as Fix16 (was .mValue); 423230 = EmitExplosion_43D690 |
| 0x43D840 | `Car_BC::HandleCarExplosion_43D840` | 0x426FA0 | ✓ `IsMaxDamage_40F890`, `sub_423830`, `sub_423230`, ✓ `cool_nash_0x294::get_occupation_403980`, `sub_4207B0`, `sub_4B8A60` (10.5 0x5935D0), ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41B0A0` | done | CANDIDATE MATCH (21->0); is_player_41B0A0, temp passed to ApplyExplosionImpulse_443710, unused Fix16_Point local removed (EH state); 423830=StartWreckCountdown_441380, 4B8A60 etc pairing |
| 0x43DA90 | `Car_BC::AccumulateDamage_43DA90` | 0x427180 | ✓ `IsMaxDamage_40F890`, `sub_4226C0` | matched | 4226C0 = SpawnDamageFireEffect_43B870 (pairing); fixed clamp logic per 9.6f, still 4 (anti_s load order) |
| 0x43DB80 | `Car_BC::KillContainedPeds_43DB80` | 0x423290 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_4212A0` | done | is_player_41B0A0, SetField8C_to_1; code unchanged |
| 0x43DD60 | `Car_BC::sub_43DD60` | 0x427290 | `sub_4232E0`, `sub_4233B0`, ✓ `cool_nash_0x294::get_occupation_403980`, `sub_4B8A60` (10.5 0x5935D0), ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41B0A0`, ✓ `sub_421590`, ✓ `sub_420660`, ✓ `sub_421470` | done | CANDIDATE MATCH (24->0); new Car_BC::sub_421590, set_z_lazy_420660, MarkForDespawn_421470, is_player_41B0A0; s16 gang idx, branch orders; 4232E0/4233B0 = StopMovement_43DBD0/CarShrinkSprite_43DC80 |
| 0x43EA60 | `Car_BC::OnObjectTouched_43EA60` | 0x4293D0 | ✓ `check_is_shop_421060`, `sub_421050`, ✓ `sub_421490`, `sub_423480` | done | sub_421490 used (if !driver), code unchanged; 421050 = get_field_26_420FF0, 423480 = GetEffectiveDriver_43E990 |
| 0x440510 | `Car_BC::sub_440510` | 0x4234A0 | `Car_BC::sub_421D90` | checked | 421D90 = GetCarLinearSpeed_43A240 (called) |
| 0x440590 | `Car_BC::InitCarAIControl_440590` | 0x4279E0 | ✓ `sub_421260`, ✓ `sub_420B70`, `sub_421510`, ✓ `sub_426E00` | done | new CarPhysics_B0::SetHandBrakeOff_421260, sub_426E00; 421510 open-coded (AI alloc); code unchanged |
| 0x4405F0 | `Car_BC::SpawnDriverPed` | 0x423510 | ✓ `sub_421960`, ✓ `sub_421970` | done | new Car_6C::GetSpareDriver_421960/SetSpareDriver_421970; code unchanged |
| 0x4406E0 | `Car_BC::sub_4406E0` | 0x427A20 | ✓ `sub_420B70`, ✓ `Car_BC::sub_421560`, ✓ `sub_4215F0`, ✓ `sub_4218F0`, ✓ `sub_420B60`, ✓ `sub_4218E0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_4212B0` | done | SetUniNum_421560, new sub_4215F0, GetField8DBit0_4218F0, ClearField8DBit0_4218E0, SetField8C_to_2; code unchanged |
| 0x4407F0 | `Car_BC::ClearDriver_4407F0` | 0x4235D0 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `angry_lewin_0x85C::sub_41DC70`, `Garox_2B00::sub_4219F0`, ✓ `sub_4212A0`, ✓ `Car_BC::sub_421460` | inlines added | new ClearHorn_421460, SetField8C_to_1; Garox_2B00::sub_4219F0 (hud display time = 0) left open-coded; code unchanged |
| 0x440C10 | `Car_BC::RotateRoofObjectTowardTarget_440C10` | 0x423630 | ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, `sub_40EAB0`, `sub_40EC80` | done | 96->6; IsFireTruck/IsTank/IsGunJeep if-chain, 9.6f branch order; left: Ang16 -= normalize codegen in one branch; 40EAB0/40EC80 = 4056C0/IsAngleAhead_405C60 |
| 0x440F90 | `Car_BC::do_car_bomb_440F90` | 0x4295C0 | ✓ `sub_420B50`, ✓ `sub_420F10`, `sub_482790` | done | GetPedId_420F10 used, code unchanged; 482790 = SetDamageOwner_529080 (called) |
| 0x441030 | `Car_BC::GoToBlockTest_441030` | 0x4237B0 | `sub_421510` | checked | 421510 AI alloc left open-coded (Car_BC.hpp can't see gCarAI_78_Pool); no change |
| 0x441330 | `Car_BC::GetZPos_441330` | 0x420270 | `sub_49F510` | checked | 49F510 = ComputeZPosition_559E90 (called) |
| 0x441520 | `Car_BC::sub_441520` | 0x426030 | `sub_425E30`, `sub_425E60` | checked | 425E30/425E60 = HeadlightsOn_43BFE0/HeadlightsOff_43C0C0 (called) |
| 0x4418B0 | `Car_BC::DetachTrailerAndUpdateDamage_4418B0` | 0x427B20 | `Car_BC::sub_425DF0` | done | add_f78_bits_421890(1), code unchanged; 425DF0 = FlashHeadlights_43BD40 (called) |
| 0x441A70 | `Car_BC::sub_441A70` | 0x423AA0 | ✓ `Car_10::set_obj_421380`, ✓ `sub_421340` | done | new Car_Door_10::ResetState_421340, code unchanged |
| 0x441E70 | `Car_BC::sub_441E70` | 0x426140 | `sub_423B80`, `sub_425ED0`, ✓ `sub_425590`, ✓ `sub_4213D0`, ✓ `sub_425650`, ✓ `sub_421430`, ✓ `inline_check_0x2_info_421700`, `sub_423C30`, ✓ `Car_BC::inline_check_0x20_info_4216C0`, `sub_423B50`, ✓ `sub_411920` | checked | 423B80/425ED0/423C30/423B50 = called UpdateRoofLightFlasher/StopLightFlashing_43CBE0 etc (pairing) |
| 0x4421B0 | `Car_BC::sub_4421B0` | 0x420450 | `TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` (10.5 0x445EC0) | checked | 445EC0 listed is HasPassengerWith_F238_Is_5_471710 (pairing) |
| 0x442200 | `Car_BC::sub_442200` | 0x424030 | ✓ `IsMaxDamage_40F890`, ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x442310 | `Car_BC::ManageDespawning_442310` | 0x424090 | ✓ `Car_BC::sub_403BA0`, ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_421790`, ✓ `sub_4118F0`, ✓ `sub_4217A0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `sub_411920`, ✓ `sub_4214B0`, ✓ `sub_421470` | done | all 9.6f inlines used |
| 0x4424C0 | `Car_BC::UpdateCarDespawnStatus_4424C0` | 0x4241C0 | `sub_424010` | checked | 424010 = IsCarInConditionsToDespawn_442170 (called) |
| 0x4426D0 | `Car_BC::sub_4426D0` | 0x426220 | ✓ `IsMaxDamage_40F890`, `sub_423850`, ✓ `sub_4217E0`, ✓ `Car_BC::sub_4118C0` | checked | 423850 = UpdateHorn_4417D0 (called) |
| 0x442760 | `Car_BC::DetachTrailer_442760` | 0x426270 | ✓ `Car_BC_Pool::sub_420F20`, ✓ `TrailerPool::sub_425580` | done | Car_BC_Pool::UpdateNextPrev, new TrailerPool::Remove_425580; code unchanged |
| 0x443170 | `Car_BC::Update_443170` | 0x429E50 | ✓ `Car_BC::sub_403BA0`, `Car_BC::sub_4298B0`, ✓ `sub_420B70`, `sub_414F70`, `sub_427450`, `sub_421760`, `Car_BC::sub_421830` | checked | 4298B0/427450/421760/421830 are big or called helpers; 414F70 (flag 0x2000 via 411930) left open-coded (flag helper changes code elsewhere) |
| 0x4435B0 | `Car_BC::GetCrashSoundCategory_4435B0` | 0x4243C0 | `Car_BC::sub_411930`, `sub_4216E0` | checked | CanCollideOver_4216E0 (via get_car_info_4BF1F0) and the 411930 flag test both change the code; left as is |
| 0x4435F0 | `Car_BC::sub_4435F0` | 0x424400 | ✓ `sub_421640`, `Car_BC::sub_411930`, ✓ `sub_40F7B0`, `sub_4216E0` | inlines added | inline_check_0x10_info_421640 used; CanCollideOver_4216E0 breaks the match; code unchanged |
| 0x4438C0 | `Car_BC:sub_4438C0` | 0x4244A0 | ✓ `sub_421870`, `sub_4219A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, `sub_421980`, `sub_421990`, ✓ `sub_421950` | inlines added | new GetDriverPlayer_421870; 4219A0/421980/421990 Player helpers not added; code unchanged |
| 0x443AB0 | `Car_BC::sub_443AB0` | 0x424470 | ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x443AE0 | `Car_BC::ResprayOrChangePlates` | 0x427BD0 | ✓ `sub_421870`, `sub_421980`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_421950`, `sub_421990`, `cool_nash_0x294::sub_420B80` | inlines added | GetDriverPlayer_421870; code unchanged |
| 0x443BD0 | `Car_BC::ResprayOrCleanPlates` | 0x427CA0 | `sub_4257B0`, ✓ `sub_421870`, ✓ `angry_lewin_0x85C::sub_41DC70` | inlines added | GetDriverPlayer_421870; 4257B0 not checked; code unchanged |
| 0x443C40 | `Car_BC::HandleShops_443C40` | 0x427D10 | ✓ `sub_4215C0`, `sub_421020`, `sub_420760` | done | sub_4215C0, GetDriverPlayer_421870; code unchanged; 421020/420760 = get_field_26_420FF0/ObjectTypeToWeaponType_443CB0 |
| 0x443D00 | `Car_BC::sub_443D00` | 0x4207F0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `CarPhysics_B0::sub_49EE10` (10.5 0x563670) | checked | 49EE10 = SetSprite_563560 (called), set_xyz_lazy already used |
| 0x443D70 | `Car_BC::IncrementCarStats_443D70` | 0x424630 | ✓ `Car_BC::has_trailer_41E460` | done | is_trailer_cab_41E460, code unchanged |
| 0x443F30 | `Car_BC::sub_443F30` | 0x420950 | `sub_483A00` | checked | 483A00 = Light_527990 (called) |
| 0x444090 | `Car_BC::GetEffectiveDriverPedId_444090` | 0x4246A0 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_420C30` | done | IsPlayerPedId_420C30 used, code unchanged |
| 0x4441B0 | `Car_BC::SetSirens_4441B0` | 0x424700 | ✓ `sub_421780`, ✓ `sub_421790`, ✓ `sub_4217A0`, ✓ `sub_4118F0`, ✓ `sub_411920` | done | IsMediCar/IsCopCar/IsSwatVan/IsFireTruck/is_FBI_car tests, code unchanged |
| 0x444490 | `Car_BC::PoolAllocate` | 0x424880 | ✓ `sub_420D90`, ✓ `Ped_Unknown_4::ClearList_420E90`, ✓ `Car_BC::sub_421460` | done | ClearAllBits/ClearList already, ClearHorn_421460 now used; code unchanged |
| 0x4446E0 | `Car_BC::DeAllocateAI_4446E0` | 0x4266F0 | `sub_425460` | checked | 425460 = pool DeAllocate (already) |
| 0x4447D0 | `Car_BC::sub_4447D0` | 0x426730 | `sub_426120`, ✓ `Sprite_Pool::sub_421030`, `Car_BC::sub_4D0700` | checked | 426120 = DeAllocateCarPhysics_441A10, 421030 = Sprite_Pool::remove (already) |
| 0x444860 | `Car_BC::ctor_444860` | 0x4267B0 | `struct_4::ctor_424620`, `Ped_Unknown_4::ctor_425450`, `maybe_flags::ctor_420D80`, ✓ `Car_BC::sub_421460` | inlines added | ClearHorn_421460 used; struct_4/Ped_Unknown_4/maybe_flags ctors are member ctors (implicit); code unchanged |
| 0x444980 | `Car_6C::DistributeCarsByRating_444980` | 0x420A10 | ✓ `gtx_0x106C::sub_420200`, `sub_4BFFE0` | done | does_car_exist (420200) used; 4BFFE0 = IsCarModelInRecycleList_5AB380 (called); code unchanged |
| 0x444AB0 | `Car_6C::SelectTrafficCarModel_444AB0` | 0x424980 | ✓ `sub_4212D0` | done | GetRngValue_4212D0 used; code unchanged |
| 0x444E40 | `Car_BC::SnapCarToGreenArrow_444E40` | 0x4268C0 | `sub_424FF0` | checked | 424FF0 (948 bytes) = TrySnap/other big call (pairing) |
| 0x4451E0 | `sub_4451E0` | 0x424E30 | ✓ `sub_421780` | done | CANDIDATE MATCH (5->0); IsMediCar = 9.6f 0x421780 (comment), return via ternary |
| 0x445210 | `GetNearestFrontVehicle_445210` | 0x424E70 | ✓ `PolarToCartesian_41FC20`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_421610`, `Car_BC::sub_421D90` | checked | PolarToCartesian_41FC20 already used; 421D90 = GetCarLinearSpeed_43A240 |
| 0x445360 | `sub_445360` | 0x424F80 | ✓ `IsMaxDamage_40F890`, ✓ `sub_421640`, `sub_421D80`, ✓ `sub_4214D0`, `sub_421DF0` | done | CANDIDATE MATCH (6->0); inline_check_0x10_info_421640; model check is boxcar/TRAINFB/TRAINCAB only (not IsTrainModel) |
| 0x4453E0 | `sub_4453E0` | 0x427D60 | `sub_40C810`, ✓ `sub_41FE40`, ✓ `EnqueueRadioLocationPhrase_426E10` | checked | 40C810 (switch around sub_588DE0) open; SwapIf3or4_41FE40 already used |
| 0x446230 | `Car_6C::SpawnCarAt_446230` | 0x426AC0 | ✓ `sub_41FEA0`, `sub_4254A0`, ✓ `sub_41FF00`, `sub_425480`, ✓ `Sprite_Pool::sub_421000`, `sub_466B70` (10.5 0x466B70), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_420710`, `sub_4BDEF0`, ✓ `inline_check_0x40_info_421680`, ✓ `sub_4217D0`, ✓ `sub_421810`, ✓ `sub_421790`, ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `sub_421890`, ✓ `sub_4218C0`, ✓ `sub_4218D0`, ✓ `sub_4218A0`, ✓ `Car_BC::sub_411910`, ✓ `sub_4217E0`, `sub_425FD0` (10.5 0x43D690), ✓ `Car_BC::sub_403BA0`, ✓ `sub_4218B0`, ✓ `sub_4212D0`, `sub_422E00`, ✓ `sub_411920` | matched | all listed callees already used or called in 10.5 (pool alloc, ApplyScaleToDimensions, PutTV_Antenna, SetCarGraphic_43CDF0) |
| 0x446790 | `Car_6C::CarsService_446790` | 0x42A4C0 | `sub_433160`, `Car_BC_Pool::sub_42A2D0` | checked | 433160/42A2D0 = Taxi PopAll/pool UpdatePool (called/inline already) |
| 0x447710 | `CarAI_78::sub_447710` | 0x4300E0 | ✓ `sub_42ACA0`, ✓ `sub_42ACB0`, ✓ `sub_42AC80`, ✓ `sub_42AC20`, ✓ `sub_42AC40`, `sub_42FE60`, ✓ `sub_42AC90`, ✓ `sub_42AC70`, ✓ `sub_42AC30`, ✓ `sub_42AC50`, ✓ `sub_421530` | done | CANDIDATE MATCH (59->0); field_A6 bit helpers added to Car_BC.hpp, also |
| 0x447CA0 | `CarAI_78::GoToBlock_447CA0` | 0x42ACF0 | `sub_40E0F0` | checked | 40E0F0 (5 bytes) is a Fix16 ctor (operator) |
| 0x453470 | `CarAI_78::sub_453470` | 0x4326E0 | ✓ `sub_421260`, `sub_4211A0`, ✓ `sub_421540`, ✓ `sub_421550`, ✓ `sub_42AC60`, `sub_4221A0`, `sub_4221B0`, ✓ `sub_42AC00` (10.5 0x453F90) | done | SetHandBrakeOff_421260, SetA6Bit5/ClearA6Bit5, new IsA6Bit5Set_42AC60, NeutralGear_42AC00; code unchanged; GetLength_453590 kept (out-of-line in 10.5); 4221A0/4221B0 = DoBreak/DoBrakeAndHandbrake (called) |
| 0x453A40 | `CarAI_78::sub_453A40` | 0x42FF20 | `sub_467110`, `sub_40F580`, `sub_40E790`, `sub_42FE80` | checked | 467110/40E790/42FE80 = sub_4E6660/... (called); Ang16_to_Fix16 already used |
| 0x4546D0 | `CarInfo_808::LoadModelPhysics_4546D0` | 0x432D10 | ✓ `gtx_0x106C::get_car_info_count_432850`, `sub_402B20` (10.5 0x4A6DB0), `array_constuctor_401CF0` | checked | get_number_of_cars (432850) used; 402B20 = File::sub_4A6DB0 copy; array ctor is compiler-generated |
| 0x454850 | `CarInfo_808::CalculateAllCarInfo_454850` | 0x432EA0 | ✓ `gtx_0x106C::get_car_info_count_432850`, ✓ `gtx_0x106C::sub_420200`, `sub_432AA0` | checked | get_number_of_cars, does_car_exist used; 432AA0 = CalculateCarInfo_4542A0 (called) |
| 0x4549C0 | `CarInfo_808::ConvertAllMass_4549C0` | 0x432CD0 | ✓ `gtx_0x106C::get_car_info_count_432850`, `sub_432C60` | checked | get_number_of_cars used; 432C60 = ConvertMass_454680 |
| 0x454AA0 | `CarInfo_808::Free_454AA0` | 0x433000 | `Monster_2C::gdtor_432F70` | checked | 432F70 is the compiler vector deleting dtor |
| 0x454B00 | `CarInfo_808::ctor_454B00` | 0x432830 | `Monster_808::sub_432810` | checked | 432810 = Clear_454A80 |
| 0x457BA0 | `Taxi_4::PushTaxi_457BA0` | 0x433140 | `sub_433120` | checked | 433120 = pool Allocate (already inline) |
| 0x45AE40 | `abs_sub_less_than_epislon_45AE40` | 0x45AE40 | `Zone_144::init_45D960` (10.5 0x4BED70) | checked | Zone_144::init_45D960 is a pairing error |
| 0x45BC10 | `Ped::TeleportToCoord_45BC10` | 0x435C80 | ✓ `cool_nash_0x294::get_car_416B60` | done | all 9.6f inlines used |
| 0x45BC70 | `Ped::ManageShocking_45BC70` | 0x435CE0 | ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_4338D0`, `sub_4CEF40` | done | code unchanged |
| 0x45BD20 | `Ped::sub_45BD20` | 0x435D90 | `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity_43A4C0 |
| 0x45BE30 | `Ped::sub_45BE30` | 0x435E40 | ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x45BEC0 | `Ped::ManageBurning_45BEC0` | 0x444A70 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `sub_434950`, ✓ `sub_41B0A0` | done | code unchanged |
| 0x45C350 | `Ped::RespawnPed_45C350` | 0x43E140 | `Char_B4_Pool::DeAllocate`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0` | commented | SetRemap_433C10 breaks the match |
| 0x45C410 | `Ped::sub_45C410` | 0x435FA0 | ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::sub_403920` | done | all 9.6f inlines used |
| 0x45C830 | `Ped::AllocCharB4_45C830` | 0x4360C0 | `Char_B4_Pool::sub_4355A0`, `Char_B4::sub_433880`, ✓ `Char_B4::sub_4338E0` | checked | Char_B4_Pool 4355A0 = field_0_pool.Allocate; Char_B4::sub_433880 (sprite xyz + alloc + region buckets) not added, it needs PurpleDoom and Ped.cpp globals in char.hpp |
| 0x45C920 | `Ped::GetPedVelocity_45C920` | 0x436160 | ✓ `sub_41B080` (10.5 0x41B480), `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity_43A4C0 |
| 0x45C960 | `Ped::GetRotation` | 0x4361B0 | ✓ `sub_433A40` | done | code unchanged |
| 0x45CAA0 | `Ped::HandleClosePedInteraction_45CAA0` | 0x444B50 | ✓ `cool_nash_0x294::sub_416B50`, `sub_435610`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_433E50`, ✓ `sub_433B70`, ✓ `sub_41DC40`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_433A60`, ✓ `cool_nash_0x294::sub_403AE0` | matched | diff 36->6; sub_433E50 fixed to unsigned compare; 435610 = abs_sub_less_than_epislon_45AE40 |
| 0x45DD30 | `Ped::AddWeaponWithAmmo_45DD30` | 0x43E4B0 | ✓ `sub_433820` | done | code unchanged |
| 0x45EA00 | `Ped::sub_45EA00` | 0x441F10 | `cool_nash_0x294::sub_403A30` | checked | Target keeps the 'all gone' loop index in cl and pFirst in edi; ours keeps the index in memory |
| 0x45EE70 | `Ped::EnterPublicTransport_45EE70` | 0x43B9D0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x45F360 | `Ped::Mugger_AI_45F360` | 0x43E8B0 | ✓ `cool_nash_0x294::sub_4039F0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::has_car_403B80` | done | code unchanged |
| 0x461530 | `Ped::TrainCustomer_AI_461530` | 0x43EAF0 | ✓ `cool_nash_0x294::sub_403920`, `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity_43A4C0 |
| 0x4619F0 | `Ped::RoadBlockTank_AI_4619F0` | 0x43EBC0 | ✓ `sub_421540` | done | code unchanged |
| 0x461F20 | `Ped::Occupation_AI_461F20` | 0x442DE0 | ✓ `sub_434970`, `sub_4427E0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::set_occupation_403970`, `sub_403960`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0` | done | code unchanged; new Taxi_4::IsEmpty_434970 |
| 0x462280 | `Ped::sub_462280` | 0x445330 | ✓ `Get_F3C_433370` | done | all 9.6f inlines used |
| 0x462510 | `Ped::RemovePedWeapons_462510` | 0x436830 | ✓ `cool_nash_0x294::sub_403A40`, `Weapon_8::sub_4D06E0` | done | code unchanged; 4D06E0 = deallocate_5E3CB0 |
| 0x462550 | `Ped::sub_462550` | 0x436860 | ✓ `cool_nash_0x294::sub_403A40`, `Weapon_8::sub_4D06E0` | done | code unchanged; 4D06E0 = deallocate_5E3CB0 |
| 0x462620 | `Ped::sub_462620` | 0x436890 | ✓ `Char_B4::sub_433A80`, ✓ `sub_4338F0`, `sub_491EA0` | done | code unchanged |
| 0x462B80 | `Ped::sub_462B80` | 0x43F180 | ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `sub_4339C0`, ✓ `sub_4339E0`, ✓ `sub_433A00`, ✓ `sub_433A40`, ✓ `sub_403900`, `Char_B4_Pool::DeAllocate`, `sub_423590`, ✓ `Car_10::set_obj_421380` | done | code unchanged; new Char_B4::Get_F84_403900 |
| 0x462E70 | `Ped::Update_462E70` | 0x4454E0 | ✓ `sub_433C90`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | code unchanged |
| 0x4633E0 | `Ped::sub_4633E0` | 0x433650 | `cool_nash_0x294::sub_433580` | checked | cool_nash_0x294::sub_433580 is ChangePedStatesByMode_463300, which 10.5 calls (pairing noise) |
| 0x463AA0 | `Ped::ProcessOnFootObjective_463AA0` | 0x443170 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_4215B0`, `sub_493940`, `sub_4340D0`, `sub_4340E0`, `sub_4340F0`, `sub_433EB0`, `sub_433F40`, `sub_433FE0`, `sub_43BEE0`, `sub_43BF60`, `sub_4388E0`, `sub_434380`, `sub_439640` | checked | the big callees (433EB0..439640, 493940) are case bodies already open-coded in the matching switch; Object_2C getters already used |
| 0x465B20 | `Ped::sub_465B20` | 0x437010 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_433DA0`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::sub_416B50` | matched | diff 74->70 |
| 0x466B70 | `sub_466B70` | 0x466B70 | `sub_4653C0` (10.5 0x4DFE10), `sub_42A630`, `sub_4634E0` | checked | pairing miss: 10.5 0x466B70 is the small Ped::sub_466B70 flag test, 9.6f 0x466B70 is an unrelated map function |
| 0x466FB0 | `Ped::FindNearbyPed_466FB0` | 0x433D00 | `MaxAbsDistance_42A6B0`, ✓ `sub_40FEA0` | done | diff 55->23 (rest: abs order / Max_44E540 call form inside MaxAbsDistance) |
| 0x467E20 | `Ped::KillCharAnyMeans_467E20` | 0x43FD10 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433DA0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | all 9.6f inlines used |
| 0x467FD0 | `Ped::sub_467FD0` | 0x438A30 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x468040 | `Ped::ProcessAirborneMovement_468040` | 0x438AB0 | ✓ `Char_B4::sub_433A80`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | follow-up: branch order, diff 20->0 CANDIDATE MATCH |
| 0x468310 | `Ped::sub_468310` | 0x438C40 | ✓ `sub_421550`, ✓ `sub_421540`, `Car_BC::sub_421EC0` | done | diff 12 unchanged; new Car_BC::SetA6Bit20_421540/ClearA6Bit20_421550 |
| 0x4686C0 | `Ped::EnterTargetObjectiveCar_4686C0` | 0x43C010 | ✓ `sub_433900`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890` | done | code unchanged |
| 0x468930 | `Ped::EnterTrain_468930` | 0x438E80 | ✓ `sub_433900`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890` | done | code unchanged |
| 0x468A00 | `Ped::LeaveTrain_468A00` | 0x43C220 | ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x468BD0 | `Ped::sub_468BD0` | 0x43C3E0 | ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `cool_nash_0x294::set_target_to_enter_403B00` | done | code unchanged |
| 0x468C70 | `Ped::PatrolOnFoot_468C70` | 0x438F60 | ✓ `sub_433970` | done | all 9.6f inlines used |
| 0x468DE0 | `Ped::GotoAreaOnFoot_468DE0` | 0x4390F0 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x468E80 | `Ped::UpdateFollowPedObjective_468E80` | 0x439190 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::sub_416B50`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | follow-up: branch order, diff 35->0 CANDIDATE MATCH |
| 0x469BD0 | `Ped::sub_469BD0` | 0x4341D0 | `sub_4046F0` | checked | 4046F0 = PedGroup::UpdateMemberTightFollowState_4CA820 |
| 0x469BF0 | `Ped::GuardSpot_469BF0` | 0x439360 | ✓ `Char_B4::sub_433A80`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | diff 26->0 CANDIDATE MATCH (RegulateVelocityByRef, u8 flag, inverted state test) |
| 0x469D60 | `Ped::sub_469D60` | 0x4394C0 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x469E50 | `Ped::sub_469E50` | 0x434230 | ✓ `Car_BC::sub_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x469F30 | `Ped::sub_469F30` | 0x434300 | ✓ `Car_BC::sub_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x46A1F0 | `Ped::sub_46A1F0` | 0x439590 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x46A290 | `Ped::FollowCarInCurrCar_46A290` | 0x4343C0 | ✓ `Car_BC::sub_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x46A530 | `Ped::FireAtObject_46A530` | 0x434470 | `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x46A5E0 | `Ped::FireAtPlayer_46A5E0` | 0x434530 | ✓ `cool_nash_0x294::sub_433DA0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `sub_40E8D0` | done | diff 84->0 CANDIDATE MATCH (also fixed bit 8 -> 0x800, restructured like 9.6f) |
| 0x46A6D0 | `Ped::AimVehicleTurretStateMachine_46A6D0` | 0x434640 | ✓ `sub_4215B0`, `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x46A850 | `Ped::DestroyTargetCar_46A850` | 0x439810 | ✓ `IsMaxDamage_40F890` | done | all 9.6f inlines used |
| 0x46A8F0 | `Ped::FleeOnFootTillSafe_46A8F0` | 0x434740 | ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433920` | done | code unchanged; new Char_B4::Get_F44_433A90 |
| 0x46A9C0 | `Ped::FleeFromPedTillSafe_46A9C0` | 0x4347D0 | ✓ `cool_nash_0x294::sub_433BE0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433930` | done | code unchanged; new Ped::ClearF144_433BE0 |
| 0x46AAE0 | `Ped::sub_46AAE0` | 0x434870 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_433BE0`, ✓ `Char_B4::sub_433930` | done | code unchanged |
| 0x46AB50 | `Ped::sub_46AB50` | 0x4398B0 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_433BE0`, ✓ `Char_B4::sub_433930` | done | code unchanged |
| 0x46BD50 | `Ped::sub_46BD50` | 0x4349A0 | `sub_4341B0` | done | all 9.6f inlines used |
| 0x46C770 | `Ped::sub_46C770` | 0x43A210 | ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433920` | done | code unchanged (SetMaxSpeedByRef_433920) |
| 0x46C7E0 | `Ped::sub_46C7E0` | 0x43A290 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x46C8A0 | `Ped::sub_46C8A0` | 0x43A340 | ✓ `Char_B4::sub_433920` | done | code unchanged (SetMaxSpeedByRef_433920) |
| 0x46C910 | `Ped::sub_46C910` | 0x43A3C0 | ✓ `Char_B4::sub_433920` | done | code unchanged (SetMaxSpeedByRef_433920) |
| 0x46C9B0 | `Ped::sub_46C9B0` | 0x43A490 | `sub_434960`, ✓ `Char_B4::sub_433920`, ✓ `Char_B4::sub_433A90` | done | code unchanged (SetMaxSpeedByRef_433920) |
| 0x46CA70 | `Ped::sub_46CA70` | 0x434A30 | ✓ `Car_BC::sub_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x46CB30 | `Ped::StartPedCrossingAtTrafficLight_Y_Backward_46CB30` | 0x43A550 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CC70 | `Ped::StartPedCrossingAtTrafficLight_X_Forwards_46CC70` | 0x43A660 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CDB0 | `Ped::StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0` | 0x43A770 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CEF0 | `Ped::StartPedCrossingAtTrafficLight_X_Backwards_46CEF0` | 0x43A880 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46D030 | `Ped::sub_46D030` | 0x43A990 | ✓ `sub_411940`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, ✓ `sub_433900`, ✓ `Char_B4::sub_433920` | done | code unchanged (SetMaxSpeedByRef_433920) |
| 0x46D0D0 | `Ped::sub_46D0D0` | 0x43FEE0 | `Char_B4::sub_433930` | done | diff 62 unchanged |
| 0x46D300 | `Ped::sub_46D300` | 0x43AA10 | ✓ `Char_B4::sub_433A80`, `Car_BC::sub_421EC0`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970`, ✓ `sub_433940` | done | diff 111->0 CANDIDATE MATCH |
| 0x46DD70 | `sub_46DD70` | 0x4400A0 | ✓ `sub_41D020`, `sub_404400`, ✓ `sub_433360`, ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::sub_416B50`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433BC0` | inlines added | SetCounts_433360, sub_433BC0 x2, get_remap_433BA0 x2; get_cam_x/y/z and SetRemap_433C10 break the match (commented) |
| 0x46E200 | `Ped::SpawnPedGroupFollowers_46E200` | 0x440350 | `sub_404400`, ✓ `sub_433360`, ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433BB0`, ✓ `sub_433BC0`, `sub_404420` | inlines added | code unchanged; new PedGroup::SetCounts_433360; SetRemap_433C10 breaks the match (left out) |
| 0x46F110 | `Ped::GetWeaponFromPed_46F110` | 0x434D70 | `sub_433820`, ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910` | done | diff 15 unchanged |
| 0x46F490 | `Ped::sub_46F490` | 0x434E60 | `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity_43A4C0 (noise) |
| 0x46F600 | `Ped::ForceWeapon_46F600` | 0x43D830 | ✓ `sub_433810` | done | code unchanged |
| 0x46F650 | `Ped::GiveWeapon_46F650` | 0x43AD10 | ✓ `sub_433810` | done | code unchanged |
| 0x46F680 | `Ped::sub_46F680` | 0x434FF0 | ✓ `Zone_144::sub_433B30`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x46F720 | `Ped::UpdateStatsForKiller_46F720` | 0x43D880 | `Shooey_CC::sub_44A370`, ✓ `angry_lewin_0x85C::sub_4219D0` | matched | Shooey_CC::sub_44A370 = ShouldReportPedCrime_485140, which 10.5 calls (noise) |
| 0x46F9D0 | `Ped::Kill_46F9D0` | 0x4411B0 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_4338F0`, ✓ `cool_nash_0x294::set_health_4039A0` | done | code unchanged |
| 0x46FE20 | `Ped::ProcessWeaponHitResponse_46FE20` | 0x4350C0 | `sub_4340E0`, ✓ `sub_4340D0`, `MaxAbsDistance_42A6B0`, `sub_433810` | matched | diff 41->26; MaxAbsDistance commented (10.5 out-of-line Abs/Max) |
| 0x46FF00 | `Ped::NotifyWeaponHit_46FF00` | 0x435180 | `MaxAbsDistance_42A6B0`, `sub_433810` | done | diff 58->30; MaxAbsDistance not used (10.5 out-of-line) |
| 0x46FFF0 | `Ped::HandleWeaponFireEnd_46FFF0` | 0x435220 | ✓ `sub_433810` | done | code unchanged |
| 0x470050 | `Ped::AimRoofGun_470050` | 0x435280 | ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x4701D0 | `Ped::sub_4701D0` | 0x435430 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x470200 | `Ped::StartPedWalking_470200` | 0x43AD50 | ✓ `sub_433C10`, ✓ `Char_B4::sub_433920` | done | inlines added; SetMaxSpeedByRef_433920 used, SetRemap_433C10 breaks match |
| 0x470330 | `Dummies_470330` | 0x4415E0 | ✓ `sub_433E90` | done | code unchanged |
| 0x470A50 | `Char_C::SpawnPedAt` | 0x43DB40 | ✓ `PedPool::sub_403890`, ✓ `sub_433C00`, ✓ `sub_433B90`, `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0` | done | follow-up: SetRemap_433C10 too, code unchanged |
| 0x470B00 | `Char_C::SpawnDriver_470B00` | 0x43DBD0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470BA0 | `Char_C::SpawnGangDriver_470BA0` | 0x43DC60 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433BC0` | done | code unchanged |
| 0x470CC0 | `Char_C::sub_470CC0` | 0x43DD80 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470D60 | `Char_C::SpawnRunAwayGuy_470D60` | 0x43DE10 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470E30 | `Char_C::SpawnTrainLeaver_470E30` | 0x43DEB0 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470F30 | `Char_C::sub_470F30` | 0x436070 | `Car_BC::sub_4221D0` (10.5 0x43A9A0), `Car_BC::sub_421560`, `sub_4215F0` | checked | pairing error (Car_BC callees in a 2-line alloc function) |
| 0x470F90 | `Char_C::sub_470F90` | 0x43DFB0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `sub_433C00`, ✓ `cool_nash_0x294::sub_433B50` | inlines added | code unchanged; SetRemap_433C10 breaks the match (commented) |
| 0x4710C0 | `Char_C::PedById` | 0x43AE10 | ✓ `PedPool::sub_435530` | done | code unchanged; new PedPool::GetFirstPed_435530 |
| 0x471140 | `Ped_List_4::AddPed_471140` | 0x445F10 | ✓ `sub_445EF0` | done | code unchanged |
| 0x471160 | `Ped_List_4::AddPedToBackIfMissing_471160` | 0x445F30 | ✓ `sub_445EF0` | done | code unchanged |
| 0x4711B0 | `Ped_List_4::AddPedToFrontIfMissing_4711B0` | 0x445F80 | ✓ `sub_445EF0` | done | code unchanged |
| 0x4711F0 | `Ped_List_4::RemovePed_4711F0` | 0x445FC0 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471240 | `Ped_List_4::RemovePed_471240` | 0x446010 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471290 | `Ped_List_4::RemovePedsInSpecificState_471290` | 0x446060 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_8_Pool::sub_445F00` | done | follow-up: CheckBit0_433B40, code unchanged |
| 0x471320 | `Ped_List_4::RemoveFirstPed_471320` | 0x446100 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471340 | `Ped_List_4::GetFromListClosestPedToPoint_471340` | 0x445C30 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0` | matched | all 9.6f inlines used |
| 0x4715A0 | `Ped_List_4::KillAllPedsFromList_4715A0` | 0x446120 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x4715E0 | `Ped_List_4::KillAllPedsAndClearCarRef_4715E0` | 0x446160 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471630 | `Ped_List_4::ApplyPassengerBusStopBehavior_471630` | 0x445E40 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x474850 | `Hamburger_500::ArePedsCompatible_474850` | 0x4462F0 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x477C90 | `PurpleDoom::FindNearestSprite_SpiralSearch_477C90` | 0x447540 | `sub_4BAA70` | checked | sub_4BAA70 = Sprite::GetPed_59E1B0 (called) |
| 0x477F60 | `PurpleDoom::CheckRectForCollisions_477F60` | 0x4477B0 | `sub_4BA5E0` | checked | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect_59DD60, called in 10.5 |
| 0x478060 | `PurpleDoom::CheckTileSpritesForClosestMatch_478060` | 0x446B80 | ✓ `sub_446940`, `sub_4BBC80`, ✓ `sub_416B40`, ✓ `sub_446960` | done | New Sprite::ManhattanDistance_446960; Still matches. |
| 0x478160 | `PurpleDoom::SearchTileColumnForClosestSprite_478160` | 0x446CB0 | ✓ `PurpleDoom::sub_446820` | done | all 9.6f inlines used |
| 0x478240 | `PurpleDoom::AddToDrawList_478240` | 0x446D60 | ✓ `sub_446950` | done | Sprite::IsTypeAbove1_446950; Still matches. |
| 0x4782C0 | `PurpleDoom::DoRemove_4782C0` | 0x447850 | `sub_447360`, `sub_447380` | checked | sub_447350/60/70/80 are the Collide_8 / PurpleDoom_C pool Allocate/DeAllocate (template, already used) |
| 0x478370 | `PurpleDoom::AddToColumnBuckets_478370` | 0x447900 | `sub_447360`, `sub_447380` | checked | sub_447350/60/70/80 are the Collide_8 / PurpleDoom_C pool Allocate/DeAllocate (template, already used) |
| 0x478440 | `PurpleDoom::AddToSingleBucket_478440` | 0x4479D0 | `sub_447350`, `sub_447370` | checked | sub_447350/60/70/80 are the Collide_8 / PurpleDoom_C pool Allocate/DeAllocate (template, already used) |
| 0x4784D0 | `PurpleDoom::AddToRowBuckets_4784D0` | 0x447A60 | `sub_447350`, `sub_447370` | checked | sub_447350/60/70/80 are the Collide_8 / PurpleDoom_C pool Allocate/DeAllocate (template, already used) |
| 0x4785D0 | `PurpleDoom::CheckRowForRectCollisions_4785D0` | 0x446DE0 | ✓ `sub_446940`, ✓ `CanAllocateOfType_446930`, `sub_4B9A30`, ✓ `sub_41E390`, `sub_4BED60`, ✓ `sub_446920` | checked | sub_4B9A30 = ShouldCollideWithSprite_59E850, sub_4BED60 = AddSprite_5A6CD0 (called) |
| 0x478750 | `PurpleDoom::CheckAndHandleCollisionsInStrip_478750` | 0x446F30 | ✓ `CanAllocateOfType_446930`, ✓ `sub_446920` | done | all 9.6f inlines used |
| 0x4787E0 | `PurpleDoom::CheckAndHandleRowCollisionsForSprite_4787E0` | 0x446FD0 | ✓ `sub_446940`, ✓ `CanAllocateOfType_446930`, `sub_4B9A80`, ✓ `sub_446920` | checked | sub_4B9A80 = ProcessCarToCarImpactIfCar_59E910 (called) |
| 0x478880 | `PurpleDoom::FindNearestSpriteInRow_478880` | 0x4470B0 | ✓ `sub_446940`, ✓ `CanAllocateOfType_446930`, `sub_4B9A30`, ✓ `sub_446920` | checked | sub_4B9A30 = ShouldCollideWithSprite_59E850 (called) |
| 0x478A30 | `Collide_C::ctor_478A30` | 0x4471B0 | `Collide_8_Pool::ctor_4468C0`, `PurpleDoom_C_Pool::ctor_4468F0` | checked | pool ctors are compiler-generated (new) |
| 0x478BF0 | `Collide_C::dtor_478BF0` | 0x447B20 | `PurpleDoom::gdtor_4472F0`, `Collide_8_Pool::gdtor_447310`, `PurpleDoom_C_Pool::gdtor_447330` | checked | pool dtors are compiler-generated (delete) |
| 0x47E610 | `Crane_15C::ctor_47E610` | 0x449860 | `struct_4::ctor_424620` | checked | struct_4 ctor is the member's implicit ctor |
| 0x47E730 | `Crane_15C::ComputeHookPos_47E730` | 0x447FE0 | ✓ `sub_432860`, ✓ `sub_40F6B0`, `sub_4207B0`, `sub_40F680` | matched | all inlines already used; 4207B0 = Sprite::get_x_y_443580 (called in 10.5) |
| 0x47E920 | `Crane_15C::sub_47E920` | 0x448090 | ✓ `sub_40F540`, `sub_447F90`, `sub_40F680`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, `sub_4BD670` | checked | Fix16_To_Ang16_40F540 already used; 447F90/40F680/4BD670 = ComputeHookPos_47E620/+=/... already |
| 0x47ECC0 | `Crane_15C::sub_47ECC0` | 0x4481F0 | ✓ `sub_40FEB0`, ✓ `Car_BC_Pool::sub_420F20`, ✓ `sub_447EA0`, `sub_4BED60` | done | SetF_88_447ea0 used; AsCar/UpdateNextPrev already; 4BED60 = AddSprite_5A6CD0 (called) |
| 0x47EDF0 | `Crane_15C::sub_47EDF0` | 0x448300 | ✓ `sub_40FEB0`, `sub_4207B0`, `sub_40F600`, ✓ `sub_447DF0`, ✓ `sub_40E790` | checked | AsCar, get_x_y, operator- already used |
| 0x47EF80 | `Crane_15C::sub_47EF80` | 0x448450 | ✓ `sub_40FEB0`, ✓ `sub_41B0A0`, ✓ `sub_420F30`, ✓ `sub_4214E0`, `sub_4207B0`, `sub_40F600`, ✓ `sub_447DF0`, ✓ `sub_447EB0` | done | is_player_41B0A0 used; AsCar/UnlinkFromActiveList_420F30 already |
| 0x47F220 | `Crane_15C::sub_47F220` | 0x4485D0 | `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F290 | `Crane_15C::sub_47F290` | 0x448650 | `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F2F0 | `Crane_15C::sub_47F2F0` | 0x4486C0 | `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F350 | `Crane_15C::sub_47F350` | 0x448730 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F3D0 | `Crane_15C::sub_47F3D0` | 0x4487D0 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, `sub_40F580`, `sub_423A70` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F450 | `Crane_15C::sub_47F450` | 0x448870 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, `sub_40F580`, `sub_4BEA90` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47F7F0 | `Crane_15C::sub_47F7F0` | 0x448980 | `sub_4BEA90`, `sub_4207B0`, `sub_448900` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed_441A40/FirstSpriteOfType_5A6CA0/ComputeHookPolar (called) |
| 0x47FB40 | `Crane_15C::sub_47FB40` | 0x448C00 | ✓ `sub_40E790` | done | all 9.6f inlines used |
| 0x47FE10 | `Crane_15C::UpdateCraneSprites_47FE10` | 0x448E30 | ✓ `sub_40F540`, `Object_2C::sub_4826A0` (10.5 0x525AE0), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `set_xy_lazy_447E20`, `Zheal_15C::sub_448030`, `sub_447F90`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_40F680` | checked | Fix16_To_Ang16_40F540, set_ang_lazy etc already used; Object_2C::sub_4826A0/Zheal sub_448030/447F90 = called helpers |
| 0x480310 | `Crane_15C::Service_480310` | 0x449BA0 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_447F40`, ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x4803B0 | `Crane_15C::InitCrane_4803B0` | 0x449130 | ✓ `Zheal_15C::sub_447F60`, ✓ `sub_447E90`, ✓ `Sprite_Pool::sub_421000`, ✓ `struct_4::sub_4207E0` | done | new Crane_15C::set_field_148_447F60, struct_4::ResetHead_4207E0; code unchanged |
| 0x484CF0 | `Shooey_14::ReportCrimeForPedAtLocation` | 0x44A060 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | all 9.6f inlines used |
| 0x484FE0 | `Shooey_CC::ReportCrimeForPed` | 0x44A200 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_41B0A0` | done | Ped::is_player_41B0A0; Still matches. |
| 0x485090 | `Shooey_CC::sub_485090` | 0x44A2C0 | `angry_lewin_0x85C::sub_4219D0` | checked | get_idx_4219D0 vs 10.5 Player::GetIdx_4881E0 (separate out-of-line copy, called) |
| 0x488310 | `Crusher_30::sub_488310` | 0x44A420 | ✓ `sub_4215B0`, `sub_423310`, ✓ `sub_447EB0`, `sub_44A3E0` | done | add_f78_bits_421890(2) (9.6f 0x44A3E0); IsDespawning/Is_F9_Eq7 already; code unchanged |
| 0x4887F0 | `CrusherPool_94::CrushersService_4887F0` | 0x44A990 | `sub_44A470` | checked | 44A470 = Service_488350 (called) |
| 0x488820 | `CrusherPool_94::CreateCrusher_488820` | 0x44A9C0 | `sub_44A720` | checked | 44A720 = InitCrusher_4885A0 (called) |
| 0x48B920 | `rng::ShowCycle_48B920` | 0x44AA90 | ✓ `sub_44AA60`, ✓ `sub_44AA80` | done | inlined_check (44AA60), new GetLastRecRngIdx_44AA80; 2->0 CANDIDATE MATCH |
| 0x48F710 | `Sprite_3CC::InvalidateAllMasks_48F710` | 0x44AFE0 | ✓ `sub_44AF70` | done | all 9.6f inlines used |
| 0x48F730 | `Sprite_3CC::ctor_48F730` | 0x44B000 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler vector ctor helper |
| 0x48F8B0 | `sub_48F8B0` | 0x44B0C0 | `sub_44AF90` | checked | sub_44AF90 is Sprite_14::MarkUsed_48F5A0 (called) |
| 0x495470 | `sub_495470` | 0x495470 | `sub_472C00` (10.5 0x4F78F0), `sub_495220` (10.5 0x54C1A0), `sub_491F10`, `sub_4725B0` (10.5 0x4F7940), `sub_492400` | checked | pairing error (9.6f entry is a different function at the same address) |
| 0x495630 | `Montana::dtor_495630` | 0x44B9E0 | `Montana_4::gdtor_44B970` | checked | Montana_4::gdtor_44B970 is the compiler scalar deleting dtor |
| 0x498D20 | `bk_1::game_pad_read_498D20` | 0x44C070 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x49C6D0 | `Door_38::CanOpen_49C6D0` | 0x44CA70 | ✓ `sub_4118D0`, ✓ `sub_44C870` | done | all 9.6f inlines used |
| 0x49C7F0 | `Door_38::sub_49C7F0` | 0x44CB80 | ✓ `sub_41B0A0` | done | all 9.6f inlines used |
| 0x49CC00 | `Door_38::sub_49CC00` | 0x44CC30 | ✓ `sub_447E90`, `Object_5C::sub_4852E0` (10.5 0x5299B0) | checked | Object_5C::sub_4852E0 is NewPhysicsObj_5299B0 (out of line in 10.5); 10.5 calls NewTouchPoint_529950; set_field_26 already used |
| 0x49CF10 | `Door_4D4::sub_49CF10` | 0x44D140 | `Door_10_Pool::sub_44C830` | checked | Door_10_Pool::sub_44C830 is the pool Allocate() already used |
| 0x49CF50 | `Door_4D4::RegisterSingleDoorDataNoCheck_49CF50` | 0x44DB00 | `sub_44D840` | checked | sub_44D840 corresponds to Door_38::sub_49C8D0, called in 10.5 |
| 0x49D170 | `Door_4D4::RegisterSingleDoorData_49D170` | 0x44D670 | `sub_44CC30` (10.5 0x49CC00) | checked | sub_44CC30 (=sub_49CC00) vs 10.5 call to sub_49CAC0: variant pairing difference |
| 0x49D1F0 | `Door_4D4::RegisterDoubleDoor_49D1F0` | 0x44D6F0 | `sub_44CDD0` | checked | sub_44CDD0: same as 0x49CFA0, 10.5 calls sub_49CC00 |
| 0x49D3C0 | `Door_4D4::CheckDoorAccess_49D3C0` | 0x44D1E0 | ✓ `sub_40FEB0`, `sub_44C860`, ✓ `sub_40FEA0`, ✓ `sub_433A20` | inlines added | Door_38::sub_44C860 used (decl in Door_38.hpp, def in Door_4D4.hpp); get_ped_433A20 commented (broke match) |
| 0x49D4A0 | `Door_4D4::ctor_49D4A0` | 0x44D2E0 | `Door_10_Pool::ctor_44C800` | checked | Door_10_Pool::ctor_44C800 is the pool template ctor via new |
| 0x4A7FC0 | `Firefighter_28::sub_4A7FC0` | 0x450CD0 | ✓ `sub_4211C0`, ✓ `sub_4215B0`, ✓ `sub_4214B0`, ✓ `sub_403B70`, `sub_450CC0`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450` | matched | 9.6f compares get_car_velocity_4211C0() (GetLength_41E260, not 453590) and has no (u8) cast |
| 0x4ABBD0 | `Debug::Init_4ABBD0` | 0x451930 | `Registry::Get_Debug_Setting_4B53B0` (10.5 0x586E90) | checked | Get_Debug_Setting_4B53B0 = Registry::Get_Debug_Setting_586E90 (called) |
| 0x4ACFA0 | `Frontend::create_4ACFA0` | 0x457830 | `sub_410550`, `unknown_libname_18` (10.5 0x40EF10), `sub_481D10` | checked | sub_410550/unknown_libname_18 are root_sound thunks, sub_481D10 is Bink::Reset_513210 (called in 10.5) |
| 0x4AEC00 | `Frontend::sub_4AEC00` | 0x45A250 | `sub_453480` | checked | sub_453480 (268 bytes, 9.6f input/credits helper) has no inlined body in 10.5 here; 10.5 calls separate functions |
| 0x4B2F60 | `Frontend::sub_4B2F60` | 0x459E30 | ✓ `sub_4539D0` | done | (not in todo, same helper) GetCharWidth_4539D0; still matches |
| 0x4B4D00 | `Frontend::LoadMapFilenames_4B4D00` | 0x4556A0 | ✓ `sub_453A40` | done | EncodeStage_453A40; still matches |
| 0x4B4EC0 | `Frontend::sub_4B4EC0` | 0x455850 | ✓ `sub_453A60` | done | DecodeStage_453A60; still matches |
| 0x4B5270 | `Frontend::sub_4B5270` | 0x455B20 | ✓ `sub_453A60` | done | DecodeStage_453A60; still matches |
| 0x4B55F0 | `Frontend::sub_4B55F0` | 0x455C90 | ✓ `sub_453AA0` | done | GetTime_453AA0; still matches (sub_4D7787 is CRT _itow) |
| 0x4B5FF0 | `Frontend::intro_bik_exists_4B5FF0` | 0x452A10 | `sub_452990` | checked | sub_452990 is intro_bik_4B5E50, called out of line in 10.5 |
| 0x4B6780 | `Frontend::sub_4B6780` | 0x455F90 | ✓ `sub_453A60` | done | DecodeStage_453A60; still matches |
| 0x4B7360 | `Frontend::sub_4B7360` | 0x4562F0 | ✓ `sub_453A60`, ✓ `sub_453A40` | matched | DecodeStage_453A60/EncodeStage_453A40; unchanged (53) |
| 0x4B7610 | `Frontend::UpdateBonusStageArrows_4B7610` | 0x456490 | ✓ `sub_453A60` | done | DecodeStage_453A60; still matches |
| 0x4B8280 | `Frontend::sub_4B8280` | 0x45A010 | ✓ `sub_4539D0` | done | new GetCharWidth_4539D0 (11->6), plus field_C9B3 branch order fix -> 0 CANDIDATE MATCH |
| 0x4B8C40 | `Game_0x40::LoadGameFiles_4B8C40` | 0x45B470 | `sub_410550`, ✓ `lucid_hamilton::sub_45B420`, `sub_4B5420` | checked | clear_secret_tokens_collected = 9.6f 0x45B420 (already used, comment added); sub_410550 is a thunk on an unknown 9.6f global (0x5DCBC8), sub_4B5420 a large callee |
| 0x4B8EB0 | `Game_0x40::BootGame_4B8EB0` | 0x45B5F0 | `Game_0x40::sub_45ACE0`, `FatalError_450530` (10.5 0x4A38C0), `sub_4CAC30`, `Map_0x370::sub_46A4D0`, `Map_0x370::sub_4692B0` | checked | 9.6f has `if (!bStartNetworkGame && !Game_0x40::sub_45ACE0()) FatalError(0x48, ..., 0x283)` with sub_45ACE0 returning true; 10.5 drops it as dead code, so nothing to add; other callees are 10.5 calls |
| 0x4B8FF0 | `Game_0x40::ShowCounters_4B8FF0` | 0x45B750 | ✓ `Car_BC_Pool::get_cars_count_45AD30`, ✓ `eager_benz::get_accuracy_count_45B0A0`, ✓ `eager_benz::get_reverse_count_45B0B0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | Car_BC_Pool::get_cars_count_45AD30 (new), get_cam_x/y; code unchanged |
| 0x4B9270 | `Game_0x40::DebugShowCarStatsAndFrameSkip_4B9270` | 0x45BA10 | ✓ `Garox_C4::sub_45AFD0` | done | Garox_C4::SetDrawKind8_45AFD0; code unchanged |
| 0x4B92D0 | `Game_0x40::Draw_4B92D0` | 0x45A5A0 | `Montana::sub_44B890`, `PurpleDoom::sub_447390`, `Nanobotz::Draw_472110` | checked | Montana::sub_44B890/PurpleDoom::sub_447390/Nanobotz::Draw_472110 correspond to the 10.5 calls the source already makes (ResetAll_4954F0, DrawSpritesClipped_477A40, ...) |
| 0x4B9410 | `Game_0x40::UpdateGame_4B9410` | 0x45C1F0 | ✓ `Light_1D4CC::sub_45C1E0`, `Object_5C::sub_487F50`, `frosty_pasteur_0xC1EA8::sub_481900`, `Particle_8::sub_491CE0`, `sub_4C3590`, `CokeZero_100::sub_4B9260` | checked | callees are global thunks (Light sub_45C1E0 already used, others map to the 10.5 service calls in the source) |
| 0x4B9750 | `Game_0x40::GetFirstPlayerWithoutPed_4B9750` | 0x45BAB0 | ✓ `angry_lewin_0x85C::sub_45B0C0` | done | Player::has_player_ped_45B0C0; code unchanged |
| 0x4B9790 | `Game_0x40::sub_4B9790` | 0x45BB00 | ✓ `DrawUnk_0xBC::sub_40CF60`, `DrawUnk_0xBC::sub_41EAD0` | done | Camera_0xBC::check_camera (9.6f 0x40CF60); sub_41EAD0 is sub_436120, still called; code unchanged |
| 0x4B98E0 | `Game_0x40::sub_4B98E0` | 0x45A730 | `DrawUnk_0xBC::sub_41F180` | checked | DrawUnk_0xBC::sub_41F180 is a 368-byte callee, source calls the 10.5 function |
| 0x4B9950 | `Game_0x40::IsSpriteOnScreen_4B9950` | 0x45BBA0 | ✓ `DrawUnk_0xBC::sub_45AEA0` | done | Camera_0xBC::IsSpriteInBounds_45AEA0 (new) + Player*& slot ref; 3->0 CANDIDATE MATCH |
| 0x4B9A80 | `Game_0x40::is_point_on_screen_4B9A80` | 0x45BC10 | ✓ `DrawUnk_0xBC::sub_40CF60` | matched | DrawUnk_0xBC::sub_40CF60 is check_camera, already used (comment updated to 9.6f 0x40CF60); remaining 26 lines are register allocation |
| 0x4B9B10 | `Game_0x40::IsRectVisibleToAnyPlayer_4B9B10` | 0x45BC90 | ✓ `DrawUnk_0xBC::sub_45AF40` | done | Camera_0xBC::IsRectInBounds_45AF40 (new) + s32 loop index; 85->16 (register swap left) |
| 0x4B9D60 | `Game_0x40::sub_4B9D60` | 0x45BD40 | ✓ `Game_0x40::get_player_4219E0` | done | Game_0x40::get_player_4219E0; code unchanged |
| 0x4B9DE0 | `Game_0x40::ctor_4B9DE0` | 0x45C4D0 | `angry_lewin_0x85C::sub_45B0D0`, `rng::ctor_45A960`, `Nanobotz::ctor_45B050` (10.5 0x4BE650), `Mike_A80::ctor_45C040`, `Frismo_C_Pool::ctor_45BFE0`, `jawwie_110::ctor_45C0D0`, `Kfc_1E0::ctor_45B1A0`, `Police_7B8::ctor_45C150`, `Light_1D4CC::ctor_45B3D0`, `Zones_CA8::ctor_45AE60`, `sub_489AC0`, `CokeZero_100::ctor_4B9490`, `Tango_54::ctor_45B440`, `LangIsJapanese_452E60` | checked | ctor list: member ctors, pairing noise |
| 0x4BAE30 | `Game_0x40::dtor_4BAE30` | 0x45D3D0 | `angry_lewin_0x85C::dtor_45A970`, `text_0x14::dtor_405A80`, `gtx_0x106C::gdtor_451F90`, `Map_0x370::gdtor_45A990`, `Montana::gdtor_45A9B0`, `PedPool::gdtor_43DB20`, `frosty_pasteur_0xC1EA8::gdtor_45A9F0`, `Frismo_C_Pool::gdtor_45D350`, `Phi_8CA8::gdtor_45BDC0`, `Object_5C::gdtor_45AA10`, `PedManager::gdtor_45AA30`, `sharp_bose_0x54::gdtor_45AA50`, `Sprite_8::gdtor_45AA70`, `Collide_C::gdtor_45AA90`, `Varrok_7F8::gdtor_45AAB0`, `Sero_181C::gdtor_45AAD0`, `Taxi_4::gdtor_45AAF0`, `TileAnim_2::gdtor_45AB10`, `Weapon_8::gdtor_45AB30`, `Door_4D4::gdtor_45AB50`, `jawwie_110::gdtor_45BDE0`, `Garox_2B00::gdtor_45D3B0`, `sharp_pare_0x15D8::gdtor_451F70`, `TrafficLights_194::gdtor_45AB70`, `Marz_1D7E::gdtor_45BE00`, `Orca_2FD4::gdtor_45BE20`, `Monster_808::gdtor_45AB90`, `Particle_8::gdtor_45ABB0`, `Wolfy_3D4::gdtor_45ABD0`, `Wolfy_7A8::gdtor_45ABF0`, `Zheal_D9C::gdtor_45BE40`, `Snooky_94::gdtor_45BE60`, `Kfc_1E0::gdtor_45BE80`, `Police_7B8::gdtor_45BEA0`, `Light_1D4CC::gdtor_45BEC0`, `Zones_CA8::gdtor_45BEE0`, `ChickenLegend_48::dtor_45D370`, `Hamburger_500::dtor_45AC10`, `CokeZero_100::dtor_45AC30`, `Shooey_CC::gdtor_45AC50`, `Tango_54::gdtor_45BF00`, `Rozza_C88::gdtor_45AC70`, `magical_germain_0x8EC::gdtor_45AC90` | checked | dtor list: global dtors, pairing noise |
| 0x4BE650 | `Hud_Pager_C::dtor_4BE650` | 0x45B050 | `Nanobotz::ResetCount_45B040`, `Nanobotz::set_shading_lev_46B620` (10.5 0x4E9DB0) | checked | pairing error (9.6f Nanobotz helpers) |
| 0x4BEBC0 | `Light_1D4CC::dtor_4BEBC0` | 0x45B380 | `Light_1D4CC::sub_45AD00` | checked | Light_1D4CC::sub_45AD00 is a 9.6f-only reset; 10.5 dtor calls FreeGrid_4D6E30 |
| 0x4BECA0 | `GangPool_CA8::sub_4BECA0` | 0x45DD60 | ✓ `Zone_144::sub_45DD50` | done | Gang_144::HasKillRespectChange_45DD50; code unchanged |
| 0x4BECE0 | `GangPool_CA8::sub_4BECE0` | 0x45DDB0 | ✓ `Zone_144::sub_45DD50` | done | Gang_144::HasKillRespectChange_45DD50; code unchanged |
| 0x4C1A70 | `Generator_2C::sub_4C1A70` | 0x45E3A0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C1C70 | `Generator_2C::sub_4C1C70` | 0x45E270 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C1D70 | `GeneratorPool_14AC::GeneratorsService_4C1D70` | 0x45E2E0 | ✓ `rng::get_cur_rng_41CFE0`, `sub_45E240` | checked | 9.6f sub_45E240 corresponds to Generator_2C::Service_4C1C50 (still called); get_cur_rng_41CFE0 already used |
| 0x4C1E20 | `GeneratorPool_14AC::ctor_4C1E20` | 0x45E3E0 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler's array ctor helper, nothing to add |
| 0x4C5CD0 | `lucid_hamilton::sub_4C5CD0` | 0x45EC70 | ✓ `Game_0x40::get_player_4219E0`, `sub_4B7580` | done | Game_0x40::get_player_4219E0; sub_4B7580 = ChangeFragsByAmount thunk (still called); code unchanged |
| 0x4C8E90 | `PedGroup::ClearGroupData_4C8E90` | 0x403BE0 | ✓ `cool_nash_0x294::sub_403A30` | done | code unchanged |
| 0x4C8F90 | `PedGroup::sub_4C8F90` | 0x404C90 | `sub_404420` | checked | 404420 = add_ped_to_list_4C9B30 |
| 0x4C8FE0 | `PedGroup::replace_leader_4C8FE0` | 0x404CE0 | ✓ `cool_nash_0x294::sub_403940` | done | all 9.6f inlines used |
| 0x4C9150 | `PedGroup::sub_4C9150` | 0x403C40 | ✓ `cool_nash_0x294::sub_4039F0` | done | code unchanged |
| 0x4C91B0 | `PedGroup::ResetMembersToFollowLeader_4C91B0` | 0x403CB0 | `sub_403960`, ✓ `cool_nash_0x294::sub_403AE0` | checked | already uses unset_bitset_0x04 (0x403960) |
| 0x4C9240 | `PedGroup::KillEntireGroup_4C9240` | 0x403D50 | ✓ `cool_nash_0x294::sub_403A30` | done | diff 8->0 CANDIDATE MATCH |
| 0x4C92A0 | `PedGroup::DisbandGroup_4C92A0` | 0x403DA0 | ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x4C93A0 | `PedGroup::DestroyGroup_4C93A0` | 0x403E90 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x4C94E0 | `PedGroup::DisbandGroupDueToAttack_4C94E0` | 0x403FB0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `sub_403A20`, `sub_403A50`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::sub_403920` | inlines added | code unchanged; new Ped::ClearF228_403A20; 403A50 speed setter breaks match (commented) |
| 0x4C9680 | `PedGroup::PromoteMemberToLeader_4C9680` | 0x404120 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::get_objective_timer_403B30`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::sub_403B20`, ✓ `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::sub_403B40`, ✓ `cool_nash_0x294::sub_4039D0`, ✓ `cool_nash_0x294::sub_403B50`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403AF0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, `cool_nash_0x294::sub_403940`, ✓ `cool_nash_0x294::get_enter_car_as_passenger_4039C0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::set_occupation_403970` | inlines added | code unchanged; set_ped_group_id and get_occupation_403980 break the match |
| 0x4C9970 | `PedGroup::RemovePed_4C9970` | 0x404D40 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_occupation_403980`, `cool_nash_0x294::sub_403940`, `sub_4045D0` (10.5 0x4CAE80), ✓ `cool_nash_0x294::sub_403A30` | done | code unchanged |
| 0x4C9F00 | `PedGroup::CoordinateGroupCarEntry_4C9F00` | 0x405240 | ✓ `cool_nash_0x294::sub_403990`, `sub_403B70`, `sub_404490`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_403AE0`, `sub_404480`, `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `Car_BC::sub_403BA0`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, `sub_403960`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, `sub_403900`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `sub_403BC0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, ✓ `cool_nash_0x294::set_target_car_door_403A70` | matched | IsSwatVanOrBankVan_403BC0 inline added (Car_BC.hpp); Main diff: 10.5 places the SWATVAN/bank_van door search before the normal one; swapping the if/else in source gives identical code |
| 0x4CA5E0 | `PedGroup::UpdateMemberAIState_4CA5E0` | 0x405760 | ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_403B70`, `sub_404AD0`, ✓ `cool_nash_0x294::sub_403AF0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, ✓ `cool_nash_0x294::has_car_403B80` | done | follow-up: second bHasGameObject_403B70, code unchanged |
| 0x4CAA20 | `PedGroup::IsAllMembersInSomeCar_4CAA20` | 0x404840 | ✓ `cool_nash_0x294::sub_403990` | done | all 9.6f inlines used |
| 0x4CAD40 | `PedGroup::IsLeaderCloseToTargetCar_4CAD40` | 0x4049F0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | all 9.6f inlines used |
| 0x4CAE80 | `PedGroup::FindNearestOtherMember_4CAE80` | 0x4045D0 | `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_436920` (10.5 0x463830), `cool_nash_0x294::sub_403B60`, `sub_436160` (10.5 0x45C920), `cool_nash_0x294::sub_403AE0`, `cool_nash_0x294::sub_403A40` | matched | pairing error: the 9.6f callees (car state, SetObjective2, velocity) do not fit this function |
| 0x4CB0D0 | `PedGroup::sub_4CB0D0` | 0x404C40 | ✓ `sub_4038F0`, ✓ `?MarkRootRemoved@VirtualProcessorRoot@details@Concurrency@@QAEXXZ` | done | code unchanged; new PedGroup::set_in_use_4038E0 |
| 0x4CDD80 | `BurgerKing_67F8B0::should_ignore_input_4CDD80` | 0x45ED50 | `sub_4C6E20` | checked | sub_4C6E20 (Hud+0x2A1E thunk) vs 10.5 Hud_2B00::sub_5D6CB0: pairing difference |
| 0x4CDE20 | `BurgerKing_67F8B0::save_replay_record_4CDE20` | 0x45FA50 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_45F9E0` | done | new rng::get_rnd_45F9E0; still matches |
| 0x4CE380 | `BurgerKing_67F8B0::LoadReplayHeader_4CE380` | 0x45F270 | `FatalError_450530` (10.5 0x4A38C0) | checked | FatalError_450530 is FatalError_4A38C0 (10.5 has it out of line, inline via chunk.hpp elsewhere); no open-coded form here |
| 0x4CEAC0 | `BurgerKing_67F8B0::get_input_bits_4CEAC0` | 0x45FD10 | `sub_45ED00`, ✓ `rng::get_cur_rng_41CFE0`, `IsField238_45EDE0`, ✓ `sub_416BC0` | matched | get_cur_rng_41CFE0 (no u16 cast) fixes the rng compare; left: 10.5 has a redundant je-to-next before the jne on the 0x1FF000 tests (not in 9.6f), unexplained |
| 0x4D2090 | `magical_germain_0x8EC::Load_kanji_dat_4D2090` | 0x460F10 | `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version already used |
| 0x4D2B40 | `magical_germain_0x8EC::sub_4D2B40` | 0x460DE0 | `sub_4BF550` | checked | sub_4BF550 is gtx GetSpriteIdxFromFont_5AA710, called |
| 0x4D5FA0 | `keybrd_0x204::destroy_4D5FA0` | 0x461270 | `keybrd_0x204::gdtor_461250` | checked | keybrd gdtor_461250 is the compiler scalar deleting dtor |
| 0x4D9650 | `Write_Log_4D9650` | 0x461590 | `sub_461500` (10.5 0x4D9670) | checked | sub_461500 = log_on_line_written_4D9670 (called) |
| 0x4DA440 | `Init_keybrd_jolly_and_sound_4DA440` | 0x461880 | `unknown_libname_18` (10.5 0x40EF10) | checked | unknown_libname_18 is root_sound::static_dtor_40EF10 thunk |
| 0x4DA4D0 | `InitializeGame_4DA4D0` | 0x461DE0 | `unknown_libname_18` (10.5 0x40EF10), ✓ `sub_409C40`, `sub_409F90`, `Game_0x40::sub_45A8D0` (10.5 0x4B9CD0), `lucid_hamilton::sub_45E770` (10.5 0x4C5C30), `Game_0x40::sub_45A910` (10.5 0x4B9D10), ✓ `sub_461DC0` | matched | new NetPlay::GetPlayerIdx_409C40; 409F90=GetMaxPlayers_521350 called; 9.6f SetPlayerName loop absent in 10.5; 16->10 |
| 0x4DA700 | `CleanUpInputAndOthers_4DA700` | 0x462060 | `sub_461910`, `unknown_libname_18` (10.5 0x40EF10) | checked | sub_461910 is the keybrd scalar deleting dtor; unknown_libname_18 root_sound thunk |
| 0x4DA740 | `sub_4DA740` | 0x4620A0 | `sub_4A9270` | checked | sub_4A9270 is jolly_poitras_0x2BC0::destroy_56C340 (called) |
| 0x4DA9F0 | `Net_4DA9F0` | 0x462140 | `sub_461DD0` | checked | sub_461DD0 (SetQuit) not open-coded in this 10.5 version |
| 0x4DACB0 | `Net_4DACB0` | 0x462440 | ✓ `sub_461DA0` | done | New NetPlay::GetPlayerCount_461DA0; Still matches. |
| 0x4DAD50 | `Net_Set_Local_Player_Inputs_4DAD50` | 0x4624E0 | ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_461DB0` | done | all 9.6f inlines used |
| 0x4DAF30 | `do_network_and_local_inputs_4DAF30` | 0x4626D0 | ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_461DB0` | done | Game_0x40::get_player_4219E0 and Player::GetInUse_461DB0; Still matches. |
| 0x4DB070 | `sub_4DB070` | 0x4620E0 | ✓ `Game_0x40::get_player_4219E0` | done | Game_0x40::get_player_4219E0; Still matches. |
| 0x4DB2E0 | `sub_4DB2E0` | 0x461C60 | `lucid_hamilton::sub_45E4F0` (10.5 0x4C5950) | checked | sub_45E4F0 is GetStyleName_4C5950 (out of line, called) |
| 0x4DB440 | `CompareRemotePlayers_4DB440` | 0x462850 | `lucid_hamilton::sub_45E4F0` (10.5 0x4C5950) | checked | sub_45E4F0 is GetStyleName_4C5950 (out of line, called) |
| 0x4DEF00 | `get_zone_str_4DEF00` | 0x462BE0 | `sub_4C22D0` | checked | 9.6f sub_4C22D0 is a char->wchar copy into a global buffer; 10.5 builds a key and calls Find_5B5F90 instead |
| 0x4DEF40 | `gmp_map_zone::IsZoneVisibleToAnyPlayer_4DEF40` | 0x4690B0 | ✓ `sub_463710` | done | Fix16_Rect::SetFromPosSize_463710 replaces the 4-arg ctor (both emitted out of line, call kept); still matches |
| 0x4DEFD0 | `Map_0x370::zone_by_name_4DEFD0` | 0x464C70 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4DF050 | `Map_0x370::zone_idx_by_name_4DF050` | 0x464D00 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4DF0F0 | `Map_0x370::zone_by_type_bounded_4DF0F0` | 0x464DA0 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4DF1D0 | `Map_0x370::first_zone_by_type_4DF1D0` | 0x464E70 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::get_zone_4DFB30, called out of line in 10.5 |
| 0x4DF4D0 | `Map_0x370::zone_by_pos_and_type_4DF4D0` | 0x464FE0 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint_463020 (sub_462E40 = get_zone_4DFB30 called); still matches |
| 0x4DF6A0 | `Map_0x370::sub_4DF6A0` | 0x465130 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint_463020 (sub_462E40 = get_zone_4DFB30 called); still matches |
| 0x4DF770 | `Map_0x370::next_zone_4DF770` | 0x4651C0 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint_463020 (sub_462E40 = get_zone_4DFB30 called); still matches |
| 0x4DF890 | `Map_0x370::get_nav_zone_unknown_4DF890` | 0x465250 | `Map_0x370::sub_465090` | checked | sub_465090 is nav_zone_by_pos_4DF5C0, called |
| 0x4DFE10 | `Map_0x370::get_block_4DFE10` | 0x4653C0 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4DFE60 | `Map_0x370::GetEffectiveBlock_4DFE60` | 0x465410 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4DFEE0 | `Map_0x370::sub_4DFEE0` | 0x465490 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4DFF60 | `Map_0x370::sub_4DFF60` | 0x465510 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0 x2; still matches |
| 0x4E0000 | `Map_0x370::sub_4E0000` | 0x4655B0 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0 x2; still matches |
| 0x4E00A0 | `Map_0x370::GetBlockSpec_4E00A0` | 0x465650 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0; still matches |
| 0x4E4630 | `Map_0x370::sub_4E4630` | 0x466620 | ✓ `sub_42A630` | done | all 9.6f inlines used |
| 0x4E4820 | `Map_0x370::sub_4E4820` | 0x4667E0 | ✓ `sub_45ADB0`, ✓ `sub_410BF0`, ✓ `sub_45ADA0`, ✓ `sub_45ADC0`, ✓ `sub_45ADD0`, ✓ `sub_4637A0` | matched | Fix16_Rect getters return a copy like 9.6f (0x45ADA0-0x45ADD0), + get_low_z_4637A0; order left,right,top,bottom |
| 0x4E4AC0 | `Map_0x370::sub_4E4AC0` | 0x4693A0 | ✓ `sub_420420` | done | all 9.6f inlines used |
| 0x4E4BB0 | `Map_0x370::FindPavementBlockForCoord_4E4BB0` | 0x466910 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E4C30 | `Map_0x370::FindHighestBlockForCoord_4E4C30` | 0x466990 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E4CB0 | `Map_0x370::sub_4E4CB0` | 0x466A00 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E4D40 | `Map_0x370::sub_4E4D40` | 0x469570 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt_466CF0; 466B70=UpdateZFromSlopeAtCoord_4E5BF0 (called); still matches |
| 0x4E4E50 | `Map_0x370::sub_4E4E50` | 0x4696C0 | `sub_466B70` (10.5 0x466B70) | checked | sub_466B70 is UpdateZFromSlopeAtCoord_4E5BF0, already called |
| 0x4E4F40 | `Map_0x370::sub_4E4F40` | 0x4699A0 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt_466CF0; 466B70=UpdateZFromSlopeAtCoord_4E5BF0; still matches |
| 0x4E5170 | `Map_0x370::sub_4E5170` | 0x469B00 | ✓ `sub_42A630`, `sub_466B70` (10.5 0x466B70) | checked | sub_466B70 is UpdateZFromSlopeAtCoord_4E5BF0, already called |
| 0x4E52A0 | `Map_0x370::sub_4E52A0` | 0x466AF0 | ✓ `gtx_0x106C::sub_462FB0` | done | new gtx IsWaterTile_462FB0; still matches |
| 0x4E5300 | `Map_0x370::CheckZCollisionAtCoord_4E5300` | 0x469C20 | `sub_466B70` (10.5 0x466B70) | checked | sub_466B70 is UpdateZFromSlopeAtCoord_4E5BF0, already called |
| 0x4E5B60 | `Map_0x370::FindGroundZForCoord_4E5B60` | 0x46A420 | `sub_466B70` (10.5 0x466B70) | checked | sub_466B70 is UpdateZFromSlopeAtCoord_4E5BF0, already called |
| 0x4E62D0 | `Map_0x370::FindRailwayAtCoord_4E62D0` | 0x463570 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E6360 | `Map_0x370::FindRailwayBelowZAtCoord_4E6360` | 0x4635F0 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E6400 | `Map_0x370::sub_4E6400` | 0x466E20 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt_466CF0; still matches |
| 0x4E6510 | `Map_0x370::GetRailwayZCoordAtXY_4E6510` | 0x466F70 | `sub_466B70` (10.5 0x466B70) | checked | sub_466B70 is UpdateZFromSlopeAtCoord_4E5BF0, already called |
| 0x4E65A0 | `Map_0x370::sub_4E65A0` | 0x467020 | `sub_463530` | checked | sub_463530 is FloorPlusOne_4E62B0, called |
| 0x4E7FC0 | `Map_0x370::CheckColumnHasSolidAbove_4E7FC0` | 0x463850 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E80A0 | `gmp_compressed_map_32::sub_4E80A0` | 0x463940 | ✓ `gmp_compressed_map_32::sub_42A830` | done | get_base_42A830; still matches |
| 0x4E80E0 | `Map_sub::sub_4E80E0` | 0x463990 | ✓ `sub_463080` | done | all 9.6f inlines used |
| 0x4E8620 | `Map_0x370::ChangeBlock_4E8620` | 0x463F60 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E87C0 | `Map_0x370::AddNewBlock_4E87C0` | 0x464060 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E8940 | `Map_0x370::RemoveBlock_4E8940` | 0x464110 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E8A10 | `Map_0x370::sub_4E8A10` | 0x464160 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base_42A830; still matches |
| 0x4E92B0 | `Map_0x370::load_dmap_4E92B0` | 0x4646A0 | ✓ `sub_4630B0` | done | sub_4630B0 is the gmp_compressed_map_32 ctor already used by new; noted with comment |
| 0x4E9660 | `Map_0x370::ctor_4E9660` | 0x464A40 | `sub_4630A0`, ✓ `gmp_block_info::init_44C840` | done | new Map_sub ctor (4630A0), init_44C840; still matches |
| 0x4F0340 | `MapRenderer::DrawTriangularDiagonal_4F0340` | 0x46EE40 | `Nanobotz::sub_46E490`, `Nanobotz::sub_46E5C0`, `Nanobotz::sub_46E6E0`, `Nanobotz::sub_46E800`, `Nanobotz::sub_46E910`, `Nanobotz::sub_46EA30`, `Nanobotz::SpawnDummies_46EB60`, `Nanobotz::sub_46EC90` | checked | Nanobotz slope draw callees pair with out-of-line 10.5 functions (called) |
| 0x4F3FB0 | `draw_4F3FB0` | 0x46BEA0 | ✓ `sub_432860`, `sub_40F600` | checked | sub_40F600 is the Fix16_Point operator- (operator) |
| 0x4F6630 | `MapRenderer::DrawGradientSlope_4F6630` | 0x471CE0 | `Nanobotz::sub_46EF10`, `Nanobotz::sub_46F370`, `Nanobotz::sub_46F7C0`, `Nanobotz::sub_46FC10` | checked | Nanobotz gradient draw callees pair with out-of-line 10.5 functions (called) |
| 0x4F7AE0 | `rng::get_int_4F7AE0` | 0x472E00 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4F7B70 | `rng::get_uint8_4F7B70` | 0x472E90 | ✓ `rng::get_cur_rng_41CFE0` | done | rng::get_cur_rng_41CFE0; Still matches. |
| 0x4FA330 | `Ambulance_110::HandlePedDeath_4FA330` | 0x473010 | ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x4FA7D0 | `Ambulance_20::ClearTask_4FA7D0` | 0x473140 | ✓ `Ped_Unknown_4::ClearList_420E90` | done | all 9.6f inlines used |
| 0x4FA820 | `Ambulance_20::SpawnParamedicCrew_4FA820` | 0x473170 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433BB0`, ✓ `sub_433B90`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::sub_421560` | done | code unchanged |
| 0x4FA9D0 | `Ambulance_20::EvaluatePickupState_4FA9D0` | 0x473320 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `IsMaxDamage_40F890` | done | all 9.6f inlines used |
| 0x4FFA90 | `Mike_A80::sub_4FFA90` | 0x474C60 | ✓ `sub_474430`, ✓ `sub_474450` | done | TakeElapsed/AddSample are 474430/474450 (address comments added) |
| 0x502DC0 | `Miss2_25C::MissionCleanUp_502DC0` | 0x476BC0 | ✓ `Car_BC::sub_421560`, ✓ `sub_4214D0`, ✓ `sub_421470` | done | all 9.6f inlines used |
| 0x502FF0 | `Miss2_25C::push_type_2_502FF0` | 0x476D50 | ✓ `sub_40FEF0` | done | Object_2C::get_model_40FEF0 x4; code unchanged |
| 0x503130 | `miss2_8::dtor_503130` | 0x476DC0 | ✓ `Frismo_C_Pool::sub_476780` | done | Frismo_C_Pool::DeAllocate_476780 (new); code unchanged |
| 0x503200 | `miss2_0x11C::sub_503200` | 0x47F550 | ✓ `sub_476700`, ✓ `sub_476730`, `sub_4105B0` | done | Player::IsBustedNotObjective54_476700 and IsBustedObjective54_476730 (new); code unchanged |
| 0x5035D0 | `miss2_0x11C::Log_5035D0` | 0x476E10 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x503680 | `miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680` | 0x476EA0 | ✓ `sub_475AA0`, `sub_4CA910` | done | Object_2C::IsModel176To181_475AA0 (new); sub_4CA910 is place_gang_phone (called); code unchanged |
| 0x5038D0 | `miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0` | 0x476FF0 | ✓ `sub_475A70`, ✓ `sub_447E90`, ✓ `check_is_shop_421060`, ✓ `sub_45E0A0`, ✓ `sub_475A60`, `sub_475A50` | done | Object_2C::IsBehavior1_475A70 (new); check_is_busy_shop noted as 9.6f 0x475A60; set_field_26 and check_is_shop_421060 already used; code unchanged |
| 0x503A20 | `miss2_0x11C::SCRCMD_PLAYER_PED_503A20` | 0x477140 | ✓ `sub_475A20`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x503BC0 | `miss2_0x11C::SCRCMD_CAR_DECSET_503BC0` | 0x477290 | ✓ `EnqueueRadioLocationPhrase_426E10`, ✓ `sub_4764A0`, ✓ `sub_475C30`, ✓ `sub_476270`, ✓ `Car_BC::sub_421560`, ✓ `sub_475C40` | done | Car_BC::SetF98To2IfNot4_475C40 and set_f78_0x10_476270 (new), SetF98To4_475C30, sub_421560; SpawnCar_426E10/4764A0 already used; code unchanged |
| 0x503FB0 | `miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0` | 0x477560 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x5041C0 | `miss2_0x11C::SCRCMD_CRANE_5041C0` | 0x477660 | `sub_4495F0`, `sub_4496E0`, ✓ `sub_4768E0`, ✓ `sub_476900` | checked | sub_4495F0/sub_4496E0 are 236-byte crane functions (Cranes.cpp has 4496E0 noted), sub_4768E0 already used |
| 0x5043A0 | `miss2_0x11C::SCRCMD_CONVEYOR_DECSET1_2_5043A0` | 0x4777E0 | `sub_483C00` | checked | 9.6f sub_483C00 = Object_2C::sub_529030 (still called) |
| 0x5045D0 | `miss2_0x11C::SCRCMD_THREAD_DECLARE2_5045D0` | 0x4779A0 | ✓ `sub_420B60`, ✓ `sub_476360` | done | Car_BC::set_f8D_bit1_476360 (new); get_id (420B60) already used; code unchanged |
| 0x5047C0 | `miss2_0x11C::SCRCMD_THREAD_DECLARE4_5047C0` | 0x477B70 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x504830 | `miss2_0x11C::SCRCMD_SET_GANG_INFO1_504830` | 0x477BD0 | ✓ `sub_4758A0`, ✓ `sub_4758B0`, ✓ `sub_4758C0`, ✓ `sub_4758D0`, ✓ `sub_475940`, ✓ `sub_4758E0`, ✓ `sub_4758F0`, ✓ `sub_475910`, ✓ `sub_475950` | done | Gang_144::set_info_phone_pos_475910 (new); code unchanged |
| 0x504970 | `miss2_0x11C::SCRCMD_DOOR_DECLARE_D1_S1_504970` | 0x477D20 | ✓ `sub_476990`, `sub_476A10`, `sub_476A20` | inlines added | Door_38::set_field_24 (9.6f 0x476A10); set_field_1c_1e (0x476A20) changes the code, comment updated here and in SCRCMD_DOOR_DECLARE_D2_S2_504B80; code unchanged |
| 0x504B80 | `miss2_0x11C::SCRCMD_DOOR_DECLARE_D2_S2_504B80` | 0x477EE0 | `sub_4769E0`, `sub_476A00`, `sub_4769A0`, `sub_4769C0`, ✓ `sub_476990`, `sub_476A10`, `sub_476A20` | commented | door open/close helpers (4769A0/C0/E0, 476A00, 476990, 476A10) already used; set_field_1c_1e (476A20) breaks the match, comment updated |
| 0x504EE0 | `miss2_0x11C::CreateLight_504EE0` | 0x47F710 | ✓ `sub_469010` (10.5 0x52B2A0), ✓ `Light_1D4CC::sub_469070` | done | all 9.6f inlines used |
| 0x5051D0 | `miss2_0x11C::SCRCMD_RADIOSTATION_DEC_5051D0` | 0x4753B0 | `unknown_libname_9` | checked | unknown_libname_9 is a thunk on an unknown 9.6f global (0x5DCBC8), source calls the 10.5 function |
| 0x505790 | `miss2_0x11C::DisableThread_505790` | 0x478240 | ✓ `sub_475A80`, ✓ `sub_420B60`, ✓ `sub_4768C0`, `sub_4218E0` | done | Car_214::SetTriggerEnableState_4768C0; code unchanged |
| 0x505B10 | `miss2_0x11C::DeallocOrDeleteItem_505B10` | 0x47F760 | ✓ `sub_421470`, ✓ `sub_47F4F0`, `unknown_libname_10` | checked | Car_BC::sub_421470 and DeallocLight_47F4F0 noted/used already |
| 0x5061C0 | `miss2_0x11C::ExecOpCode_5061C0` | 0x481400 | `sub_477870`, `sub_478120`, `sub_478170`, `sub_477530`, `sub_4784A0` | checked | listed callees are SCRCMD handlers (sub_477870 etc.), called in 10.5 |
| 0x506B80 | `miss2_0x11C::SCRCMD_RETURN_506B80` | 0x478610 | `sub_476E00` | checked | 9.6f sub_476E00 is a thunk (frismo pool dealloc on a global); source calls field_114->sub_5031C0 |
| 0x508220 | `miss2_0x11C::SCRCMD_MAKE_CAR_DUMMY_508220` | 0x478BA0 | ✓ `sub_4118B0`, ✓ `Car_BC::sub_421560` | done | Car_BC::sub_421560 (9.6f 0x421560); code unchanged |
| 0x508550 | `miss2_0x11C::SCRCMD_POINT_ARROW_3D_508550` | 0x478E10 | ✓ `sub_476840` | done | Hud_Arrow_7C::SetArrowAim_476840; code unchanged |
| 0x5086F0 | `miss2_0x11C::sub_5086F0` | 0x478E90 | ✓ `sub_476860`, ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_476790`, ✓ `sub_476850`, ✓ `sub_476870`, ✓ `sub_476840` | done | Hud_Arrow_7C::SetArrowTargetPed_476850/Car_476860/Object_476870 and ArrowTrace_24::SetTargetPed_4767E0/SetTargetObject_476810 (new), Ped::is_player_41B0A0; code unchanged |
| 0x509A70 | `miss2_0x11C::SCRCMD_CAR_IN_AREA_509A70` | 0x4799D0 | ✓ `sub_463710` | done | Fix16_Rect::SetFromPosSize_463710 (new); code unchanged |
| 0x509BB0 | `miss2_0x11C::SCRCMD_HAS_CHAR_DIED_509BB0` | 0x479DA0 | `sub_433C20` | checked | 9.6f sub_433C20 (char velocity) has no open-coded counterpart; the dead check reads field_278 directly |
| 0x509C10 | `miss2_0x11C::SCRCMD_IS_CHAR_IN_CAR_509C10` | 0x479B70 | ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60` | done | Ped::has_car_403B80 and get_car_416B60; code unchanged |
| 0x509C90 | `miss2_0x11C::SCRCMD_IS_CHAR_IN_MODEL_509C90` | 0x479BF0 | ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_411940` | done | Ped::has_car_403B80, get_car_416B60 and Car_BC::GetCarInfoIdx_411940; code unchanged |
| 0x509D00 | `miss2_0x11C::SCRCMD_IS_CHAR_IN_ANY_CAR_509D00` | 0x479C60 | ✓ `cool_nash_0x294::has_car_403B80` | done | all 9.6f inlines used |
| 0x509D90 | `miss2_0x11C::SCRCMD_ADD_SCORE_509D90` | 0x479D40 | ✓ `sub_41B0A0`, ✓ `sub_41DC40` | done | Player::AddCash_421990 (new); code unchanged |
| 0x509E00 | `miss2_0x11C::SCRCMD_ADD_SCORE2_509E00` | 0x479CC0 | ✓ `sub_41B0A0`, ✓ `sub_41DC40`, `sub_421990` | done | Ped::is_player_41B0A0 and Player::Add_2D4; code unchanged |
| 0x50A200 | `miss2_0x11C::SCRCMD_SET_CHAR_OBJ2_50A200` | 0x47A1E0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | done | all 9.6f inlines used |
| 0x50A460 | `miss2_0x11C::SCRCMD_SET_CHAR_OBJ_FOLLOW_50A460` | 0x47A3B0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x50A760 | `miss2_0x11C::IsOnScreen_50A760` | 0x47A620 | ✓ `sub_411A00` | done | Crane_15C::GetRotorSprite_411A00; code unchanged |
| 0x50A9E0 | `miss2_0x11C::EnableThread_50A9E0` | 0x47A860 | ✓ `sub_4768C0` | done | Car_214::SetTriggerEnableState_4768C0 (new); code unchanged |
| 0x50AC20 | `miss2_0x11C::SCRCMD_SET_GANG_RESPECT_50AC20` | 0x47AAB0 | ✓ `sub_475900` | done | all 9.6f inlines used |
| 0x50ACF0 | `miss2_0x11C::SCRCMD_CHANGE_RESPECT_50ACF0` | 0x47AB80 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_475900` | done | all 9.6f inlines used |
| 0x50AEF0 | `miss2_0x11C::RespectOperator_50AEF0` | 0x47ACC0 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50B0E0 | `miss2_0x11C::SCRCMD_ADD_PATROL_POINT_50B0E0` | 0x47ADF0 | ✓ `cool_nash_0x294::get_objective_403A80` | done | all 9.6f inlines used |
| 0x50B180 | `miss2_0x11C::SCRCMD_ANSWER_PHONE_50B180` | 0x47AEA0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x50B3D0 | `miss2_0x11C::SCRCMD_IS_CHAR_FIRE_ONSCREEN_50B3D0` | 0x47B0C0 | ✓ `sub_433CA0`, ✓ `cool_nash_0x294::sub_4039F0` | done | Ped::GetBit11_433CA0 and Get_F20E_4039F0; code unchanged |
| 0x50B4F0 | `miss2_0x11C::SCRCMD_CHAR_TO_DRIVE_CAR_50B4F0` | 0x47B1D0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | done | all 9.6f inlines used |
| 0x50B600 | `miss2_0x11C::SCRCMD_GIVE_DRIVER_BRAKE_50B600` | 0x47B2D0 | ✓ `sub_4118B0`, ✓ `sub_421540`, ✓ `Car_BC::sub_421560` | done | Car_BC::SetA6Bit5_421540 (Car_BC.hpp also has an identical SetA6Bit20_421540 from another worker); code unchanged |
| 0x50B6F0 | `miss2_0x11C::SCRCMD_CHECK_SCORE_50B6F0` | 0x47B3C0 | ✓ `sub_41B0A0`, ✓ `sub_421980` | done | Ped::is_player_41B0A0 and Player::GetScore_421980; code unchanged |
| 0x50B760 | `miss2_0x11C::SCRCMD_GET_SCORE_50B760` | 0x47B430 | ✓ `sub_41B0A0`, ✓ `sub_421980` | done | Ped::is_player_41B0A0 and Player::GetScore_421980; code unchanged |
| 0x50B7D0 | `miss2_0x11C::SCRCMD_IS_CHAR_IN_GANG_50B7D0` | 0x47B4A0 | ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | Ped::is_player_41B0A0; code unchanged |
| 0x50BA30 | `miss2_0x11C::SCRCMD_CLEAR_WANTED_LEVEL_50BA30` | 0x47BA80 | `sub_475AF0`, `sub_475AE0` | done | Ped::ClearWantedPoints_420B80 (the 9.6f pair is a different command, group field_35 setter: pairing noise); code unchanged |
| 0x50BA70 | `miss2_0x11C::SCRCMD_ALT_WANTED_LEVEL_50BA70` | 0x47B7B0 | `cool_nash_0x294::sub_434D60` | checked | 9.6f sub_434D60 = WantedStartCountLessThan_46F100 (still called) |
| 0x50BC60 | `miss2_0x11C::SCRCMD_CHECK_NUM_ALIVE_50BC60` | 0x47C050 | `sub_433B60`, `sub_433B70` | checked | pairing noise: GetPedState2_433B60/get_health_433B70 not open-coded (function reads group field_34) |
| 0x50BD10 | `miss2_0x11C::sub_50BD10` | 0x47BAC0 | ✓ `sub_475AF0` | done | all 9.6f inlines used |
| 0x50BE00 | `miss2_0x11C::SCRCMD_HAS_CAR_WEAPON_50BE00` | 0x47BBE0 | `cool_nash_0x294::has_car_403B80`, `cool_nash_0x294::get_car_416B60`, `sub_411970` | checked | pairing noise: source just calls find_5E3D20 (no ped/car getters to replace) |
| 0x50BED0 | `miss2_0x11C::SCRCMD_CHECK_MAX_PASS_50BED0` | 0x47BC50 | `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60` | checked | get_car_416B60 already used |
| 0x50C350 | `miss2_0x11C::SCRCMD_GET_LAST_PUNCHED_50C350` | 0x47C000 | ✓ `sub_475B10`, ✓ `sub_475B40` | done | Ped::clear_last_char_punched_475B40 (new); code unchanged |
| 0x50C5A0 | `miss2_0x11C::SCRCMD_EXPLODE_50C5A0` | 0x47C1E0 | ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_4340F0`, ✓ `sub_4340E0`, ✓ `sub_4340D0` | commented | Object_2C get_x/y/z_4340x0 break the match; Ped cam getters already used |
| 0x50C6F0 | `miss2_0x11C::SCRCMD_PARK_50C6F0` | 0x47C3C0 | `sub_475B10` | checked | get_last_char_punched_475B10 not applicable here (pairing noise) |
| 0x50C7D0 | `miss2_0x11C::SCRCMD_UPDATE_DOOR_50C7D0` | 0x47C410 | ✓ `sub_476A90`, `sub_4769E0`, `sub_4769C0`, `sub_476A00` | checked | set_open_details_* and set_field_14_id (4769C0/E0, 476A00) already used |
| 0x50CAB0 | `miss2_0x11C::SCRCMD_ADD_NEW_BLOCK_50CAB0` | 0x47C6A0 | ✓ `gmp_block_info::init_44C840` | done | gmp_block_info::init_44C840; code unchanged |
| 0x50CE50 | `miss2_0x11C::Gosub_50CE50` | 0x47C8D0 | `sub_476DF0` | checked | 9.6f sub_476DF0 is a thunk on a global pool (frismo push); source calls the 10.5 function |
| 0x50CE90 | `miss2_0x11C::SCRCMD_PHONE_TEMPLATE_50CE90` | 0x47FAC0 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50D3C0 | `miss2_0x11C::sub_50D3C0` | 0x47FE80 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_421980`, `sub_4105B0`, ✓ `sub_421990`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | Player::GetScore_421980 and AddCash_421990; get_cur_rng_41CFE0 already used; code unchanged |
| 0x50D870 | `miss2_0x11C::SCRCMD_CHANGE_INTENSITY_50D870` | 0x47CCC0 | `sub_476610` | checked | pairing noise (9.6f sub_476610 copies a Fix16 pair, not used here; SetIntensity_476AE0 already used) |
| 0x50D900 | `miss2_0x11C::SCRCMD_CHANGE_COLOUR_50D900` | 0x47CD40 | `sub_433B60`, `cool_nash_0x294::sub_403990` | checked | pairing noise (ped state getters, function sets a light colour) |
| 0x50DA50 | `miss2_0x11C::SCRCMD_CREATE_LIGHT_50DA50` | 0x47CA20 | `sub_476AE0` | checked | SetIntensity_476AE0 noted; function calls CreateLight_504EE0 |
| 0x50DD00 | `miss2_0x11C::SCRCMD_GET_LIVES_MULT_50DD00` | 0x47CC50 | ✓ `sub_4766C0`, ✓ `angry_lewin_0x85C::sub_4766A0` | done | Player::get_lives_4766C0 and get_multiplier_4766A0; code unchanged |
| 0x50DE00 | `miss2_0x11C::SCRCMD_POINT_ONSCREEN_50DE00` | 0x47CDA0 | `frosty_pasteur_0xC1EA8::sub_476200` (10.5 0x512770), `sub_433B60`, `cool_nash_0x294::sub_403990` | checked | pairing noise (calls is_point_on_screen_4B9A80) |
| 0x50E360 | `miss2_0x11C::SCRCMD_CHECK_CAR_SPEED_50E360` | 0x47D170 | `sub_4211C0`, ✓ `sub_4754D0` | checked | CarPhysics_B0::get_car_velocity_4211C0 uses GetLength_41E260, the source needs the GetLength_no_sqrt_inline_abs_y_negate variant to match (helper body differs); get_value_4754D0 already used |
| 0x50E460 | `miss2_0x11C::SCRCMD_SET_CAR_GRAPHIC_50E460` | 0x47F890 | `sub_47ECB0`, `miss2_0x11C::sub_475B70` (10.5 0x511930) | checked | sub_47ECB0/InitThread copy (475B70) are not open-coded here |
| 0x50E730 | `miss2_0x11C::SCRCMD_CHAR_DRIVE_AGGR_50E730` | 0x47E040 | `cool_nash_0x294::has_car_403B80`, `cool_nash_0x294::get_car_416B60`, `sub_421540` | checked | pairing noise: source only sets Ped field_21C b3; listed has_car/get_car/SetA6Bit5 belong to another command |
| 0x50E780 | `miss2_0x11C::SCRCMD_SET_SPEED_50E780` | 0x47F920 | `sub_483C60` (10.5 0x5291E0), `sub_47F420` (10.5 0x512AA0) | checked | 483C60/47F420 have 10.5 copies (Object_2C::sub_5291E0, frosty sub_512AA0) not open-coded here (pairing noise) |
| 0x50E900 | `miss2_0x11C::SCRCMD_PUT_CAR_ON_TRAILER_50E900` | 0x47D9D0 | `sub_476950`, `sub_476930` | done | Car_BC::is_trailer_cab_41E460 (9.6f pairs these callees with 0x50E9E0; GetSprite_440840 = 41FC70 already used); code unchanged |
| 0x50E9E0 | `miss2_0x11C::SCRCMD_CHECK_HEADS_50E9E0` | 0x47D5D0 | `Car_BC::has_trailer_41E460`, `sub_41FC70`, `sub_4B9D50` (10.5 0x5A3100) | checked | its listed callees (has_trailer_41E460, 41FC70, 4B9D50) belong to PUT_CAR_ON_TRAILER_50E900 (pairing swap), done there |
| 0x50EB00 | `miss2_0x11C::SCRCMD_CHECK_WEAPONHIT_50EB00` | 0x47D7A0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x50EDC0 | `miss2_0x11C::SCRCMD_DO_EASY_PHONE_50EDC0` | 0x480080 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50F270 | `miss2_0x11C::SCRCMD_WARP_CHAR_50F270` | 0x47DAD0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_433C00`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410`, ✓ `DrawUnk_0xBC::sub_475B60` | done | Ped::SetRotation_433C00 and Camera_0xBC::inline_set_ped_id_to_1_475B60; get_car_416B60 and CommitCameraTarget_41E410 already used; code unchanged |
| 0x50F3D0 | `miss2_0x11C::SCRCMD_SET_GROUP_TYPE_50F3D0` | 0x47E210 | `sub_476660` | checked | pairing noise: 9.6f sub_476660 reads a byte from a 0x2C-stride table (frosty), not the ped group type set here |
| 0x50F450 | `miss2_0x11C::SCRCMD_EMERG_LIGHTS_50F450` | 0x4803C0 | ✓ `sub_414F20` | done | Car_BC::HasEmergencyLights_414F20; code unchanged |
| 0x510100 | `miss2_0x11C::SCRCMD_START_BASIC_KF_510100` | 0x480430 | ✓ `sub_475A30`, ✓ `sub_475A40`, `sub_4105B0`, ✓ `cool_nash_0x294::get_car_416B60` | checked | get/set_field_C1E2D_475A30/475A40 and get_car_416B60 already used; sub_4105B0 thunk on an unknown 9.6f global |
| 0x510280 | `miss2_0x11C::SCRCMD_DO_BASIC_KF_510280` | 0x47E7F0 | ✓ `sub_476660`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_476680`, `sub_4C93B0`, ✓ `sub_475A40`, ✓ `cool_nash_0x294::sub_420B80`, ✓ `sub_41DC40`, ✓ `sub_4766B0`, `sub_4A4D50`, `sub_4105B0` | done | sad_mirzakhani::get_bonus_count_476660 and DeactivateBonus_476680 (new), Ped::ClearWantedPoints_420B80, Player::ChangeMultipliers_4766B0; set_field_C1E2D_475A40/GetPedState_403990/Add_2D4 already used; sub_4A4D50 = ChangeLifeCountByAmount_5699F0 (still called); code unchanged |
| 0x510780 | `miss2_0x11C::SCRCMD_SAVE_RESTORE_RESPECT_510780` | 0x4802F0 | `frosty_pasteur_0xC1EA8::sub_476200` (10.5 0x512770), `sub_43DF60`, `cool_nash_0x294::sub_403920`, `sub_436070` (10.5 0x470F30), `sub_433B90`, `cool_nash_0x294::set_occupation_403970`, `sub_476D20`, `Car_BC::sub_421560`, `sub_421510`, `sub_42A9D0` (10.5 0x453BF0), `sub_425DD0` (10.5 0x43BCA0) | checked | pairing noise: 9.6f 0x4802F0 is a ped-spawning command; source only saves/restores gang respect |
| 0x5108D0 | `miss2_0x11C::PreExecOpCode_5108D0` | 0x4805B0 | `sub_479850`, `sub_479F10`, `sub_47B5E0`, `sub_47B810`, `sub_47BCD0`, `sub_47BE00`, `sub_47D070`, `sub_47DE70`, `sub_47E360` | checked | listed callees are SCRCMD handlers (still called) |
| 0x511E10 | `frosty_pasteur_0xC1EA8::SaveGame_511E10` | 0x47EF40 | `sub_483D90`, ✓ `lucid_hamilton::sub_453A80`, `sub_475CA0` | checked | get_secret_tokens_collected_453A80 already used; sub_483D90/sub_475CA0 are big callees the source calls as 10.5 functions |
| 0x511F80 | `frosty_pasteur_0xC1EA8::LoadSave_511F80` | 0x47F0B0 | `sub_47EF10`, ✓ `sub_476B10` | done | lucid_hamilton::set_secret_tokens_collected_476B10 (new); sub_47EF10 = LoadScriptCounters_511C30 (still called); code unchanged |
| 0x5120C0 | `sub_5120C0` | 0x47F200 | ✓ `miss2_0x11C_Pool::sub_4767A0` | done | all 9.6f inlines used |
| 0x512160 | `frosty_pasteur_0xC1EA8::Update_512160` | 0x481890 | ✓ `miss2_0x11C_Pool::sub_4767A0` | done | all 9.6f inlines used |
| 0x512AF0 | `frosty_pasteur_0xC1EA8::sub_512AF0` | 0x476400 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | HasCheckFlag4_4759C0/HasCheckFlag2_4759A0 (+HasCheckFlag1_475980 in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512BA0 | `frosty_pasteur_0xC1EA8::sub_512BA0` | 0x4764D0 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | HasCheckFlag4_4759C0/HasCheckFlag2_4759A0 (+HasCheckFlag1_475980 in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512C00 | `frosty_pasteur_0xC1EA8::sub_512C00` | 0x476530 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | HasCheckFlag4_4759C0/HasCheckFlag2_4759A0 (+HasCheckFlag1_475980 in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512C70 | `frosty_pasteur_0xC1EA8::sub_512C70` | 0x4765A0 | ✓ `sub_4759C0`, ✓ `sub_4759A0`, ✓ `sub_475980` | done | HasCheckFlag4_4759C0/HasCheckFlag2_4759A0 (+HasCheckFlag1_475980 in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512CE0 | `frosty_pasteur_0xC1EA8::ctor_512CE0` | 0x481960 | ✓ `miss2_0x11C_Pool::ctor_481310` | matched | all 9.6f inlines used |
| 0x512FD0 | `miss2_0x11C::dtor_512FD0` | 0x47F9C0 | `miss2_8::gdtor_47BF60` | checked | miss2_8::gdtor_47BF60 is a compiler-generated deleting dtor |
| 0x5130E0 | `frosty_pasteur_0xC1EA8::dtor_5130E0` | 0x481C30 | `miss2_0x11C_Pool::gdtor_481C10` | checked | miss2_0x11C_Pool::gdtor_481C10 is a scalar deleting dtor, compiler generated |
| 0x513240 | `Bink::sub_513240` | 0x481E00 | ✓ `sub_481DF0`, `sub_481D30` (10.5 0x513390) | matched | new IsDirectBufferMode_481DF0 (Bink.cpp static); 481D30=CloseSlot2 not in 10.5 version; unchanged (38) |
| 0x513560 | `Bink::sub_513560` | 0x481F20 | ✓ `sub_481DF0` | matched | IsDirectBufferMode_481DF0; unchanged (29) (also used in OpenSlot2_5133E0) |
| 0x516660 | `youthful_einstein::ExecuteGamemodeTick_516660` | 0x4821C0 | `unknown_libname_24`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41D020`, ✓ `cool_nash_0x294::get_car_416B60` | checked | unknown_libname_24 is a CRT/library thunk |
| 0x519FE0 | `Network_20324::OnWmCommand_519FE0` | 0x407DB0 | `text_0x14::dtor_405A80` | checked | text_0x14::dtor_405A80 is the scalar deleting dtor (compiler generated) |
| 0x51A9D0 | `Network_20324::OnTimer_51A9D0` | 0x406E40 | `text_0x14::dtor_405A80` | checked | text_0x14::dtor_405A80 is the scalar deleting dtor (compiler generated) |
| 0x51AA90 | `Network_20324::CreateMainUi_51AA90` | 0x4086A0 | `text_0x14::Find_4C23A0` (10.5 0x5B5F90), `sub_4C23B0` | checked | Find_4C23A0/sub_4C23B0 are inside GetString_519A00 (called) |
| 0x51AC60 | `Network_20324::OnInitDialog_51AC60` | 0x408840 | `text_0x14::ctor_4C2620` (10.5 0x5B5FB0), `text_0x14::Load_4C2540` (10.5 0x5B5E90), `sub_4061C0` | checked | 9.6f created/loaded the text object here (ctor/Load are out of line in 10.5, not called here); sub_4061C0 = SetPlayerNameText_51B7C0 (called) |
| 0x51AFA0 | `Network_20324::PopulateMainUI_51AFA0` | 0x407070 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51B810 | `Network_20324::sub_51B810` | 0x4073D0 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51B9C0 | `Network_20324::SetSetting_51B9C0` | 0x4075C0 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51CB30 | `Network_20324::sub_51CB30` | 0x4065F0 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51CD30 | `Network_20324::SetJoinedGamePoliceEnabledText_51CD30` | 0x406720 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51CDC0 | `Network_20324::SetFragsNumberAndLabel_51CDC0` | 0x406770 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51D0C0 | `Network_20324::SetJoinedGameTypeAndFragLimitText_51D0C0` | 0x406980 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51D2F0 | `Network_20324::SetJoinedGameTimeLimitText_51D2F0` | 0x406B10 | `sub_4C23B0` | checked | sub_4C23B0 is GetString_519A00, called out of line in 10.5 |
| 0x51D7B0 | `NetPlay::vdtor_51D7B0` | 0x406BC0 | `sub_405A40` | checked | sub_405A40 is the vtable store of the dtor |
| 0x51ED00 | `NetPlay::NetworkTick_51ED00` | 0x40C380 | `sub_40BF80` | checked | sub_40BF80 is a 9.6f dispatch thunk with no 10.5 form |
| 0x51F420 | `NetPlay::MakeSendData_51F420` | 0x40A7F0 | ✓ `sub_409C40`, `sub_409C50` | done | GetPlayerIdx_409C40 x3; SeqDiff=409C50; still matches |
| 0x521630 | `NetPlay::Send_521630` | 0x40ACC0 | ✓ `sub_409C40` | done | GetPlayerIdx_409C40; still matches |
| 0x521770 | `NetPlay::sub_521770` | 0x40A190 | `sub_409C50` | done | SeqDiff noted as 9.6f 0x409C50 (already used) |
| 0x521B20 | `NetPlay::Send_521B20` | 0x40AD70 | ✓ `sub_409C40` | done | GetPlayerIdx_409C40 x2; still matches |
| 0x521BE0 | `NetPlay::NoRefs_Send_521BE0` | 0x40AE30 | ✓ `sub_409C40` | done | GetPlayerIdx_409C40; still matches |
| 0x521C80 | `NetPlay::NoRefs_Send_521C80` | 0x40AED0 | ✓ `sub_409C40` | done | GetPlayerIdx_409C40; still matches |
| 0x522180 | `Object_2C::sub_522180` | 0x484910 | ✓ `sub_421080`, ✓ `Sprite_Pool::sub_421030` | done | is_not_type6_to_12_421080 used; code unchanged |
| 0x522250 | `Object_2C::CanCollideWithSpriteByVarrok_522250` | 0x482E80 | ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_40FEA0`, ✓ `sub_420B50` | done | is_not_type6_to_12_421080 used; code unchanged |
| 0x5223C0 | `Object_2C::ShouldCollideWith_5223C0` | 0x482FA0 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x522460 | `Object_2C::SelectCollisionSprite_522460` | 0x483060 | ✓ `sub_416B40`, ✓ `sub_40FEE0` | done | get_type_416B40 + Rozza SetSprite_40FEE0 used; code unchanged |
| 0x5226A0 | `Object_2C::sub_5226A0` | 0x484AA0 | `sub_4847D0` | checked | 4847D0 = NewObj3C_528130, still called |
| 0x522BE0 | `Object_2C::ResolveCollisionWithMapTile_522BE0` | 0x4869E0 | `sub_4BCD00`, ✓ `sub_432860`, `sub_4828C0`, `sub_40F640` (10.5 0x43D5D0) | done | SetXY_432860 + operator-= (4828C0) like 522D00, point decl order fixed; 4BCD00=GetNearestHorizontalEdge (called); diff 50->0 CANDIDATE MATCH |
| 0x522D00 | `Object_2C::ResolveCollisionWithMapTileHorizontal_522D00` | 0x486B20 | `sub_4BCFA0`, ✓ `sub_432860`, `sub_4828C0`, `sub_40F640` (10.5 0x43D5D0) | done | already used SetXY/-=; point decl order + t2.x restore fixed; 4BCFA0=GetNearestVerticalEdge (called); diff 63->0 CANDIDATE MATCH |
| 0x5233A0 | `Object_2C::sub_5233A0` | 0x483460 | ✓ `sub_482BF0`, ✓ `sub_40F7B0` | done | sub_482BF0 added+used; code unchanged |
| 0x523440 | `Object_2C::HandleCollisionOutcome_523440` | 0x486D70 | ✓ `sub_420660`, ✓ `sub_482A70`, ✓ `sub_482A80` | done | Rozza_28::SetType4_482A70/SetType5_482A80 added+used; code unchanged |
| 0x5235B0 | `Object_2C::HandleSpriteGroundAndCollision_5235B0` | 0x484090 | ✓ `sub_466CF0`, `sub_4699A0` (10.5 0x4E4F40), `sub_483100`, ✓ `sub_420420`, `sub_4BD670`, `sub_4207B0`, `sub_483500` | checked | 4699A0/483100/4BD670/4207B0/483500 are calls in 10.5 |
| 0x525190 | `Object_2C::sub_525190` | 0x4856E0 | ✓ `sub_482400`, ✓ `sub_420F10`, `sub_482790` | matched | IsDefinitionIdx39To42_482400 added, GetPedId_420F10 used; 482790=SetDamageOwner_529080 (called); diff 32->31 (PoolGiveAndSetDone_5291E0 arg looks s32 in orig) |
| 0x5257D0 | `Object_2C::UpdateAninmation_5257D0` | 0x485FD0 | ✓ `sub_4206C0`, ✓ `sub_482C10` | done | set_id_lazy_4206C0 + IsAnimFinished_482C10 (added) used; code unchanged |
| 0x525AE0 | `Object_2C::CheckCollisionForModel_139_And_141_525AE0` | 0x4826A0 | `sub_447BD0` (10.5 0x477B00), `PurpleDoom::sub_447C40` (10.5 0x477B60) | checked | 447BD0/447C40 pair with PurpleDoom Remove/477B60; 10.5 calls CheckAndHandleCollisionInStrips_477BD0 |
| 0x525B80 | `Object_2C::UpdatePhysicsAndMovement_525B80` | 0x487A30 | ✓ `sub_482730`, ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | all 9.6f inlines used |
| 0x525D90 | `Object_2C::UpdatePhysicsMovementAndAnimation_525D90` | 0x487BC0 | ✓ `sub_482730`, ✓ `sub_421080`, ✓ `sub_420FF0` | commented | get_field_26_420FF0 changes the matching code |
| 0x5263D0 | `Object_2C::Service_5263D0` | 0x487E80 | `Object_2C::sub_4826A0` (10.5 0x525AE0) | checked | 4826A0 pairs with CheckCollisionForModel_139_And_141_525AE0; not open-coded here |
| 0x526790 | `Object_2C::TriggerCarExplosionIfApplicable_526790` | 0x4837F0 | ✓ `sub_475A80`, ✓ `sub_482C10`, ✓ `sub_482400`, ✓ `sub_40FEB0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | IsAnimFinished_482C10 + IsDefinitionIdx39To42_482400 used; code unchanged |
| 0x526B40 | `Object_2C::sub_526B40` | 0x483880 | ✓ `sub_416B40`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_40F7B0`, `Car_BC::sub_421D90`, ✓ `sub_40FEB0` | matched | Sprite::get_type_416B40, Char_B4::get_velocity_41B080, Sprite::set_num_40F7B0 |
| 0x527630 | `Object_2C::InitializeObject_527630` | 0x483990 | `Phi_74::sub_488EF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_482A30` | done | SetObj2C_482A30 used; 488EF0 = CreateSpriteFromDefinition (called); code unchanged |
| 0x528240 | `Object_2C::HandleRotationStateTransition_528240` | 0x483A20 | `sub_4BED60` | checked | 4BED60 = struct_4::AddSprite_5A6CD0 (called) |
| 0x5288B0 | `Object_2C::OnObjectTouched_5288B0` | 0x483B50 | ✓ `sub_40FEA0`, ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x528900 | `Object_2C::HandleWaterDeath_528900` | 0x483BA0 | ✓ `rng::get_cur_rng_41CFE0`, `sub_4BDDD0`, ✓ `sub_420660` | done | set_z_lazy_420660 used; 4BDDD0 = ShrinkSprite (called); code unchanged |
| 0x528990 | `Object_2C::HandleObjectHit_528990` | 0x486390 | ✓ `sub_40FEA0`, `sub_4932D0` (10.5 0x545430), ✓ `sub_40FEB0`, `sub_4274C0`, ✓ `sub_40FEC0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | GetPedId_420F10 + get_field_26_420FF0 used x2; 4932D0/4274C0 = HandlePedObjectHit/HandleCarHitByObject (called); code unchanged |
| 0x528A20 | `Object_2C::ProcessObjectExplosionImpact_528A20` | 0x485B00 | `sub_483C80`, ✓ `sub_420F10`, `sub_482790`, ✓ `sub_420FF0` | checked | 483C80 = GetExplosionSideFromDiagonalWall_529210, 482790 = SetDamageOwner_529080 (called) |
| 0x528BA0 | `Object_2C::HandleImpactNoSprite_528BA0` | 0x485C90 | ✓ `sub_420F10`, `sub_482410`, `sub_482790`, ✓ `sub_41E210`, ✓ `sub_420FF0` | checked | 482410 = GetExplosionTypeForWallSide_528E00, 482790 = SetDamageOwner_529080 (called) |
| 0x5291B0 | `Object_2C::Dealloc_5291B0` | 0x483C40 | `sub_482F60` | checked | 482F60 = PoolGive_522340 (called) |
| 0x5291D0 | `Object_2C::sub_5291D0` | 0x483C50 | `sub_482F60` | checked | 482F60 = PoolGive_522340 (called) |
| 0x5291E0 | `Object_2C::sub_5291E0` | 0x483C60 | `sub_482F60` | checked | 482F60 = PoolGive_522340 (called) |
| 0x529240 | `Object_2C::sub_529240` | 0x483CC0 | `sub_421050`, ✓ `gtx_0x106C::sub_462FD0` | done | gtx::sub_462FD0 added+used x2; 421050 = get_field_26 (already); code unchanged |
| 0x5292D0 | `Object_2C::get_weapon_default_ammo_5292D0` | 0x483D50 | ✓ `sub_45E0A0` | checked | 45E0A0 = set_field_26 copy (already used, noted in header) |
| 0x529300 | `Object_5C::sub_529300` | 0x485E40 | ✓ `sub_40FEC0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | GetPedId_420F10 used; code unchanged |
| 0x529430 | `Object_5C::ctor_529430` | 0x484AF0 | `struct_4::ctor_424620`, `Object_2C_Pool::ctor_483F10`, `Object_8_Pool::ctor_483F60`, `Object_3C_Pool::ctor_4848C0`, ✓ `Sprite_Pool::sub_421000`, `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690` | checked | pool ctors / struct_4 ctor are implicit member ctors |
| 0x529750 | `Object_5C::dtor_529750` | 0x484CF0 | ✓ `Sprite_Pool::sub_421030`, `Object_2C_Pool::gdtor_484820`, `Object_8_Pool::gdtor_484840`, `Object_3C_Pool::gdtor_484860` | checked | pool dtors are implicit |
| 0x5297F0 | `Object_5C::sub_5297F0` | 0x485ED0 | ✓ `sub_447E90` | done | all 9.6f inlines used |
| 0x529950 | `Object_5C::NewTouchPoint_529950` | 0x485290 | ✓ `sub_482A40` | done | Sprite::SetDimensions_482A40 + Sprite_4C::SetDimensions_482980 added+used; code unchanged |
| 0x5299F0 | `Object_5C::sub_5299F0` | 0x485320 | ✓ `sub_482C00` | checked | 482C00 = set_field_26 copy (already used, noted in header) |
| 0x529A40 | `Object_5C::NewLight_529A40` | 0x485370 | ✓ `sub_482D60` | done | all 9.6f inlines used |
| 0x529AB0 | `Object_5C::NewLight_529AB0` | 0x4853C0 | ✓ `sub_482D60` | done | all 9.6f inlines used |
| 0x529C00 | `Object_5C::New_529C00` | 0x484E00 | `Object_2C_Pool::sub_4829A0`, `Object_2C_Pool::sub_4829C0`, `sub_482790`, `Object_2C_Pool::sub_484D60`, `Object_2C_Pool::sub_484DB0`, `sub_4BED60`, `Object_8_Pool::sub_483FA0`, `Object_3C_Pool::sub_483FE0`, ✓ `check_is_shop_421060`, ✓ `sub_447E90` | checked | pool Allocate/AllocateUntracked/DeAllocate, SetDamageOwner, AddSprite already used/called |
| 0x52A2C0 | `Object_5C::New_52A2C0` | 0x485180 | `Object_3C_Pool::sub_483FE0` | checked | 483FE0 = Object_3C pool Allocate (already used) |
| 0x52A3D0 | `Object_5C::CreateExplosion_52A3D0` | 0x485540 | ✓ `sub_482A90`, `Object_3C_Pool::sub_483FE0` | done | Wolfy_30::SetTypeOrState_482A90 added+used; 483FE0 = pool Allocate (already); code unchanged |
| 0x52A590 | `Object_5C::RestoreObjects_52A590` | 0x485640 | ✓ `sub_45E0A0` | checked | 45E0A0 = set_field_26 copy (already used, noted in header) |
| 0x52A6D0 | `Object_5C::ReactivateObjectAfterImpact_52A6D0` | 0x483E50 | `Object_2C::sub_4826A0` (10.5 0x525AE0), `sub_482F80`, ✓ `sub_40FEB0`, ✓ `sub_40F7B0` | checked | 4826A0 = RemoveFromCollisionBuckets/525AE0 pairing, 482F80 = PoolTake_522360 (called) |
| 0x52AD80 | `Object_3C::ctor_52AD80` | 0x484020 | `struct_4::ctor_424620` | checked | struct_4 ctor (424620) is the member ctor, already inline |
| 0x52AE90 | `Object_2C::GetSpeedVector_52AE90` | 0x482C50 | `sub_482BA0` | checked | 482BA0 = Object_3C::GetSpeedVector_52ADF0, called out of line by the matching 10.5 code |
| 0x52B2A0 | `Light_1D4CC::sub_52B2A0` | 0x469010 | `sub_464C40`, `sub_463F10`, `nostalgic_ellis_0x28::sub_45B2D0` | checked | Alloc_464C40, SetRadius_463F10, SetCurrentIntensity_45B2D0 already used via Init_469010 |
| 0x5331A0 | `Phi_74::ApplyDefinitionToSprite_5331A0` | 0x4883A0 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_40F7B0`, `sub_488200` | done | Sprite::SetType_4206F0 and set_id_lazy_4206C0; Still matches. |
| 0x534650 | `Garage_48::ValidateParkCommand_534650` | 0x489B10 | ✓ `sub_489640`, ✓ `sub_489630`, ✓ `sub_489620` | matched | all 9.6f inlines used |
| 0x538A40 | `Particle_4C::sub_538A40` | 0x48B4D0 | ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | SetFlags_4337D0 + Set_2C_0x4_Flag_4337F0; code unchanged |
| 0x53A180 | `Particle_4C::sub_53A180` | 0x48BE60 | ✓ `sub_4206C0` | done | all 9.6f inlines used |
| 0x53B580 | `Particle_4C::sub_53B580` | 0x48C790 | ✓ `sub_4206E0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | done | all 9.6f inlines used |
| 0x53B9F0 | `Particle_4C::sub_53B9F0` | 0x48C800 | ✓ `sub_4206E0`, ✓ `sub_4337D0`, ✓ `sub_4337F0`, `sub_4BDEF0` | done | SetFlags_4337D0 + Set_2C_0x4_Flag_4337F0; 4BDEF0=ApplyScaleToDimensions (already); code unchanged |
| 0x53E2E0 | `Particle_4C::sub_53E2E0` | 0x48C8F0 | ✓ `sub_433800`, ✓ `sub_4337D0`, ✓ `Sprite_Pool::sub_421030` | done | Clear_2C_0x4_Flag_433800 + SetFlags_4337D0(0,0); code unchanged |
| 0x53E3C0 | `Particle_8::sub_53E3C0` | 0x48C930 | ✓ `sub_48A8F0`, ✓ `sub_48A8D0`, `sub_48A900`, ✓ `Sprite_Pool::sub_421000` | done | has_pStart_48A8F0 + Sprite_Pool::has_free_48A8D0 (added); 48A900 = pool Allocate (already); code unchanged |
| 0x53E880 | `Particle_8::SpawnBlood_53E880` | 0x48CC50 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_40F7B0` | done | all 9.6f inlines used |
| 0x5405D0 | `sub_5405D0` | 0x48DFC0 | ✓ `sub_48A8F0`, `sub_48A900`, ✓ `Sprite_Pool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | done | has_pStart_48A8F0 used; 48A900 = pool Allocate (already); code unchanged |
| 0x5434A0 | `Wolfy_30::Update_5434A0` | 0x491A70 | `sub_48E480`, `sub_48E750`, `sub_48E5F0`, `sub_491550` | matched | 48E480/48E750/48E5F0/491550 are the state_* functions, still called in 10.5 |
| 0x5439D0 | `Particle_8::ctor_5439D0` | 0x491B90 | `Particle_4C_Pool::ctor_48F1E0` | checked | Particle_4C_Pool ctor is the implicit pool ctor |
| 0x543A60 | `Particle_8::dtor_543A60` | 0x491C20 | `Particle_4C_Pool::gdtor_48F1C0` | checked | Particle_4C_Pool dtor is implicit |
| 0x5453D0 | `Char_B4::sub_5453D0` | 0x493640 | ✓ `Sprite_Pool::sub_421030` | done | all 9.6f inlines used |
| 0x545430 | `Char_B4::sub_545430` | 0x4932D0 | `sub_420FF0`, `sub_420F10`, `sub_485540` (10.5 0x52A3D0), `sub_420B50`, `sub_441A30` | checked | pairing: 9.6f 0x4932D0 is a larger function (485540 = CreateExplosion, 441A30 = Ped hit handling); nothing open-coded here |
| 0x5456A0 | `Char_B4::InitSprite_5456A0` | 0x493850 | ✓ `Sprite_Pool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, `Char_B4::sub_492180` | done | SetType_4206F0 used; 492180 pairs with a ped state setter, not open-coded; code unchanged |
| 0x545720 | `Char_B4::Update_545720` | 0x49C460 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_40F7B0`, ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_41E210` | done | all 9.6f inlines used |
| 0x5459E0 | `Char_B4::DrownPed_5459E0` | 0x4938A0 | `sub_48D1F0`, ✓ `cool_nash_0x294::sub_433DD0` | done | Set_F250_IfBit_433DD0 used; 48D1F0 = EmitWaterSplash (called); code unchanged |
| 0x548590 | `Char_B4::ManageZCoordAndSlopes_548590` | 0x494180 | `sub_482510`, `sub_466B70` (10.5 0x466B70), ✓ `sub_42A630`, `sub_4824E0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40) | done | Fix16::operator--(int) added (482510) + operator++(int) (4824E0) used; 466B70 = UpdateZFromSlopeAtCoord (called); code unchanged |
| 0x548670 | `Char_B4::DispatchCollision_548670` | 0x499F00 | ✓ `sub_416B40`, ✓ `sub_420B70`, ✓ `sub_40FEA0`, `sub_494280`, ✓ `sub_40FEC0`, ✓ `sub_40FEB0` | checked | 494280 = HandlePedCollision_548BD0 (called) |
| 0x54C1A0 | `Char_B4::CanMoveOntoSlope_54C1A0` | 0x495220 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_420420`, ✓ `sub_42A630`, `sub_4824E0`, `sub_482510` | done | Fix16 operator++(int)/operator--(int) used; code unchanged |
| 0x54C500 | `Char_B4::CanMoveToTile_54C500` | 0x495540 | `sub_491EE0` | done | 491EE0 = Fix16::ToInt (already); load x and y first; diff 7->0 CANDIDATE MATCH |
| 0x54C900 | `Char_B4::TickMovementStateMachine_54C900` | 0x495700 | `sub_4955F0`, `sub_4920A0`, `sub_40EAB0` | checked | 4955F0 = ApplyRandomRotationJitter_54C6C0, 4920A0 a called helper, 40EAB0 = angle delta (called) |
| 0x54DD70 | `Char_B4::sub_54DD70` | 0x4958E0 | ✓ `sub_433CA0` | done | GetBit11_433CA0 used; code unchanged |
| 0x550090 | `Char_B4::CanReachTile_550090` | 0x492420 | `sub_491EE0`, ✓ `sub_491F00`, ✓ `sub_491EF0`, `sub_492190` | checked | 491EE0 = Fix16::ToInt (already), 492190 = CanStepForward_54FEC0 (called) |
| 0x551400 | `Char_B4::ChooseNextMovementTile_551400` | 0x492D00 | ✓ `sub_492CE0`, ✓ `sub_492CF0` | done | all 9.6f inlines used |
| 0x551A00 | `Char_B4::state_3_551A00` | 0x49BAD0 | ✓ `sub_433CA0`, ✓ `sub_416B40`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `is_on_trailer_421720`, ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x551B30 | `Char_B4::state_4_551B30` | 0x496800 | ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x551BB0 | `Char_B4::state_5_551BB0` | 0x49BC20 | ✓ `sub_433CA0`, ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x552E90 | `Char_B4::state_9_552E90` | 0x49C120 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_40F7B0` | done | all 9.6f inlines used |
| 0x5532C0 | `Char_B4::sub_5532C0` | 0x497410 | `sub_4937D0`, `sub_493810` | checked | 4937D0/493810 = GetTileFracX64_545640/GetTileFracY64_545670, called |
| 0x5535B0 | `Char_B4::PhoneTouched_5535B0` | 0x4930C0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x553640 | `Char_B4::OnObjectTouched_553640` | 0x4930F0 | ✓ `check_is_shop_421060`, `sub_421050`, ✓ `sub_433A20`, ✓ `sub_493090` | done | Object_2C::GetConveyorDirection_493090 added+used; 421050 = get_field_26 (already); code unchanged |
| 0x553E00 | `Char_B4::HandleGenericImpact_553E00` | 0x493390 | ✓ `cool_nash_0x294::sub_403990`, `sub_482790` | matched | 482790 = SetDamageOwner_529080, called in 10.5 |
| 0x5545E0 | `Orca_2FD4::init_5545E0` | 0x45C130 | `Ped_Unknown_4::ctor_425450`, `Orca_2FD4::init_49C700` | checked | 9.6f caller inlined init_49C700 (= this function); Ped_Unknown_4 ctor is the member ctor |
| 0x5597B0 | `CarPhysics_B0::ShowSpeedRevsDamage_5597B0` | 0x49F010 | `sub_4211A0`, `sub_49E820` | checked | 49E820 = get_revs_561940 (called), 4211A0 speed uses the special out-of-line length variants |
| 0x559A40 | `CarPhysics_B0::UpdateTrailerPhysicsFromTowingCar_559A40` | 0x49F1B0 | `sub_40F600`, `sub_40EAB0`, `sub_40F580` | matched | 40F600/40EAB0/40F580 are Point -, ComputeShortestAngleDelta_4056C0 and Ang16_to_Fix16, already used |
| 0x559B50 | `CarPhysics_B0::EnforceTrailerControlLimits_559B50` | 0x49F280 | ✓ `sub_49EFB0` | done | Trailer::get_field_0_49EFB0 added+used; code unchanged |
| 0x559BA0 | `CarPhysics_B0::SpinOutOnOil_559BA0` | 0x49F2D0 | ✓ `Car_BC::sub_403BA0`, ✓ `sub_49EF50` | done | AddDamage_49EF50 added (CarPhysics_B0.hpp decl, def in .cpp) + used; code unchanged |
| 0x559C30 | `CarPhysics_B0::ScarePedsOnDrivingFast_559C30` | 0x49F350 | ✓ `sub_466CF0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_4211A0`, ✓ `sub_411970` | matched | GetLinearSpeed_4211A0 makes it worse (41->56: GetLength_41E260 vs GetLength_2) |
| 0x559E20 | `CarPhysics_B0::ApplyObjectImpact_559E20` | 0x49F460 | ✓ `sub_493090`, ✓ `sub_49EF50` | done | (2nd pass) GetConveyorDirection_493090 used too; code unchanged |
| 0x55A100 | `CarPhysics_B0::GetTrailerAwareTurnRatio_55A100` | 0x49F720 | ✓ `Car_BC::has_trailer_41E460` | done | is_trailer_cab_41E460 used; code unchanged |
| 0x55A860 | `CarPhysics_B0::HandleUserInputs_55A860` | 0x49FB00 | `sub_420360`, ✓ `sub_40F840` | matched | IsNull (420360, 6FE20C variant) + IsVelocityAlignedWithHeading_40F840 used; diff 155->43 |
| 0x55AA00 | `CarPhysics_B0::HandleGravityOnSlope_55AA00` | 0x4A20E0 | ✓ `Car_BC::sub_403BA0`, `sub_4634E0` | matched | 4634E0 = Map get_grad_scale_from_size: 10.5 uses CarPhysics_B0 globals (6FDF7C...), not the map ones |
| 0x55B3F0 | `CarPhysics_B0::SyncZWithTrailer_55B3F0` | 0x4A2640 | `sub_4A2240` | checked | 4A2240 = UpdateZPhysics_55AD90, called in 10.5 |
| 0x55C150 | `CarPhysics_B0::TestCollision_55C150` | 0x4A0020 | `sub_4BD670`, ✓ `sub_49EF10` | done | Rozza_28::SetField24_49EF10 added+used; 4BD670 = CheckSpriteMovementRegion (called); code unchanged |
| 0x55C3B0 | `CarPhysics_B0::SweepTestMovementForCollision_55C3B0` | 0x4A2F90 | `sub_49F930` | matched | 49F930 = ComputeRequiredSweepSteps_55A6A0, called in 10.5 |
| 0x55CA70 | `CarPhysics_B0::DispatchCollision_55CA70` | 0x4A4170 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0` | done | all 9.6f inlines used |
| 0x55E260 | `CarPhysics_B0::DoSkidmarks_55E260` | 0x4A0560 | `sub_4A0290` | checked | 4A0290 = UpdateWheelSkidEffects_55DC00 (called) |
| 0x55E470 | `CarPhysics_B0::StepMovementAndCollisions_55E470` | 0x4A4310 | ✓ `sub_4637B0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_49E450`, ✓ `IsCharB4_49EF20`, `sub_446AA0` | matched | HasZeroComponent_49E450 added (fixes logic: x==0 |
| 0x55EC30 | `CarPhysics_B0::ApplyForwardEngineForce_55EC30` | 0x4A2A30 | `sub_49E330`, `sub_420390` | checked | 49E330 = ApplyAngularImpulse_55F970, 420390 = NormalizeSafe_442AD0 (called) |
| 0x55EF20 | `CarPhysics_B0::ApplyReverseEngineForce_55EF20` | 0x4A2B40 | `sub_49E330`, `sub_420390` | matched | 49E330 = ApplyAngularImpulse_55F970, 420390 = NormalizeSafe_442AD0: both called in 10.5 |
| 0x55F020 | `CarPhysics_B0::ApplyTurningForce_55F020` | 0x4A2C70 | ✓ `sub_40FEC0`, ✓ `sub_4118D0`, `sub_40F600`, ✓ `sub_40F6B0`, `sub_49E330`, `sub_420390` | matched | Force goes through the exported Multiply_438FE0, not the inline operator* (whose visible body dropped an EH state store) |
| 0x55F360 | `CarPhysics_B0::CheckPendingCollision_55F360` | 0x4A2DB0 | `sub_414F70`, `sub_49EFD0` | commented | 414F70 (IsFlagSet_411930(0x2000)) changes the matching code; 49EFD0 body not dumped |
| 0x55F740 | `CarPhysics_B0::ApplyForceWithTrailerRedirect_55F740` | 0x4A3140 | ✓ `is_on_trailer_421720`, `sub_4A2E00` | matched | is_on_trailer_421720 used; 4A2E00 = ApplyForceAndIntegrate_55F7A0 (called); diff 47->34 (by-value Fix16_Point copy differs) |
| 0x55F800 | `CarPhysics_B0::ApplyForceAtPoint_55F800` | 0x4A0850 | ✓ `sub_40F6B0`, `sub_40F680` | done | operator+= (40F680) x2; bRotate test as switch (dec/jne in 9.6f and 10.5); diff 135->0 CANDIDATE MATCH |
| 0x55F9A0 | `CarPhysics_B0::ApplyForceScaledByMass_55F9A0` | 0x4A0930 | `sub_40F680` | checked | 40F680 = operator+=, already used |
| 0x55FA10 | `CarPhysics_B0::ApplyImpulseWithTrailerRedirect_55FA10` | 0x4A0960 | ✓ `is_on_trailer_421720`, `sub_4A0900` | done | is_on_trailer_421720 used; 4A0900 = AccumulateImpulse_55F930 (called); diff 21->0 CANDIDATE MATCH |
| 0x5610B0 | `CarPhysics_B0::IntegrateAndClampVelocities_5610B0` | 0x4A0CC0 | `sub_40F680`, ✓ `ApplyDeadZone_49E3C0`, ✓ `sub_482730` | checked | 40F680 = operator+=, already used |
| 0x5626F0 | `CarPhysics_B0::ApplyArrowSteerAssist_5626F0` | 0x4A17C0 | `sub_40F580`, ✓ `sub_40F840`, `sub_420360`, ✓ `angry_lewin_0x85C::sub_41DC70` | checked | 40F580 = Ang16_to_Fix16, 420360 = IsNull: already used |
| 0x562FA0 | `CarPhysics_B0::UpdateLastMovementTimer_562FA0` | 0x4A1BA0 | `sub_49F170` | checked | 49F170 = IsNotMoving_5599D0 (called) |
| 0x562FE0 | `CarPhysics_B0::ProcessCarPhysicsStateMachine_562FE0` | 0x4A4570 | ✓ `unknown_libname_25`, ✓ `sub_49F4E0`, ✓ `sub_49F4F0`, ✓ `sub_49EA60`, ✓ `sub_4118D0`, ✓ `sub_4212B0`, ✓ `sub_4212A0` | done | all 9.6f inlines used |
| 0x563670 | `CarPhysics_B0::UpdateSpriteFromPhysics_563670` | 0x49EE10 | `CarPhysics_B0::sub_49ED60` (10.5 0x563350) | checked | 49ED60 = UpdateCenterOfMassPoint_563350 (called) |
| 0x5636C0 | `CarPhysics_B0::UpdateCarAndTrailerSpriteFromPhysics_5636C0` | 0x49EEB0 | `sub_49EE80` | checked | 49EE80 = sprite update helper (called) |
| 0x563900 | `CarPhysics_B0::ctor_563900` | 0x4A1D60 | `CarPhysics_B0::sub_4A1CF0` | checked | 4A1CF0 = CarPhysics_B0 ctor body helper (called/implicit) |
| 0x564710 | `Player::SetKFCarWeapon_564710` | 0x4A5220 | ✓ `sub_4A4FE0`, ✓ `sub_4A4F90` | done | Weapon_30::set_infinite_ammo_4A4F90 (new); get_ammo_4A4FE0 already used; code unchanged |
| 0x564790 | `Player::SetKFWeapon_564790` | 0x4A52B0 | ✓ `sub_4A4FE0`, ✓ `sub_4A4F90` | done | Weapon_30::set_infinite_ammo_4A4F90; code unchanged |
| 0x5647D0 | `Player::ClearKFWeapon_5647D0` | 0x4A5300 | ✓ `sub_433820`, ✓ `sub_4A4FF0`, ✓ `Weapon_30_Pool::sub_4A4F20` | done | all 9.6f inlines used |
| 0x564960 | `Player::AddWeaponWithAmmo_564960` | 0x4A5400 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60`, `sub_4A53D0` | done | GetPlayerPed_4A5130; sub_4A53D0 = HasAnyAmmo_564940 (already called); code unchanged |
| 0x5649D0 | `Player::SelectNextOrPrevWeapon_5649D0` | 0x4A5460 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `keen_bhaskara_0x30::sub_4A4F80` | done | all 9.6f inlines used |
| 0x564C00 | `Player::sub_564C00` | 0x4A5640 | `angry_lewin_0x85C::sub_4A5600` | checked | 4A5600 = CleanupEmptyAmmoWeapons_564B80, called out of line in 10.5 too; no inline missing |
| 0x564C50 | `Player::RemovePlayerWeapons_564C50` | 0x4A5690 | ✓ `keen_bhaskara_0x30::sub_4A4F80` | done | Weapon_30::HasAmmo_4A4F80; code unchanged |
| 0x564CF0 | `Player::sub_564CF0` | 0x4A5710 | `cool_nash_0x294::sub_435F00`, ✓ `sub_482080` | done | Ped::clear_bit_26_482080; sub_435F00 = Ped::sub_45C050 (still called); code unchanged |
| 0x564D60 | `Player::CollectPowerUp_564D60` | 0x4A5780 | ✓ `thirsty_lamarr::sub_41DC30`, ✓ `sub_4766B0`, `sub_4A4D50`, ✓ `sub_433B70`, ✓ `sub_4A5050`, ✓ `cool_nash_0x294::get_wanted_points_433DC0` (10.5 0x592370), ✓ `cool_nash_0x294::sub_420B80`, ✓ `sub_4A5060`, ✓ `sub_4A5020` | done | Player::ChangeMultipliers_4766B0, Ped::SetFullHealth_4A5050, set_bit_26_4A5060 (new), get_value x2, get_health_433B70, ClearWantedPoints_420B80; sub_4A4D50 = ChangeLifeCountByAmount_5699F0 (still called); code unchanged |
| 0x565070 | `Player::sub_565070` | 0x4A59A0 | `cool_nash_0x294::sub_435F00`, ✓ `sub_482080` | done | Ped::clear_bit_26_482080; sub_435F00 = Ped::sub_45C050 (still called); code unchanged |
| 0x5651F0 | `Player::RestorePowerUpsFromSave_5651F0` | 0x4A5A50 | ✓ `sub_4A5060` | matched | VC6 turns the two pointers into base+difference addressing; the target keeps both (same in 9.6f). Loop forms tried: while/for/indexed |
| 0x565310 | `Player::TeleportToDebugCam_565310` | 0x4A5AD0 | ✓ `DrawUnk_0xBC::sub_475B60` | done | all 9.6f inlines used |
| 0x565490 | `Player::InitPlayerPed_565490` | 0x4A5B40 | `cool_nash_0x294::sub_435C40` | checked | 9.6f sub_435C40 = Ped::sub_45B560, still called |
| 0x565890 | `Player::Hud_Controls_565890` | 0x4A7010 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_4A4770`, `sub_4C8A20`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `sub_4A5070`, ✓ `sub_4A4750`, `cool_nash_0x294::sub_435F40`, `angry_lewin_0x85C::sub_4A6FD0`, ✓ `sub_4A4760`, `sub_4A4940`, `angry_lewin_0x85C::sub_4A49B0`, ✓ `cool_nash_0x294::sub_420B80` | done | ToggleLimitFramerate_4A4750 + ToggleShowCoords_4A4760 (new), camera set_ped_id 1/2 (4A5070 noted), ClearWantedPoints_420B80, clear_zones = 4A4770 (noted); 4C8A20/4A6FD0/4A4940/4A49B0/435F40 correspond to Player/Ped methods still called |
| 0x5668D0 | `Player::HandleControls_5668D0` | 0x4A76D0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::sub_403990`, `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `Car_BC::sub_403BA0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, `sub_4A5030`, ✓ `cool_nash_0x294::sub_4A5010`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040` | done | Ped::SetBit11_4A5010 (new) x3, ClearBit11_403A40 x2; other listed getters already used (403A40 commented helper), sub_4039E0 = objective status getter (sub_450CB0 still called), sub_4A5030 unknown; code unchanged |
| 0x566C80 | `Player::DoPedControlInputs_566C80` | 0x4A5C50 | `sub_43E1E0`, ✓ `Char_B4::sub_433A80`, ✓ `sub_433C40`, ✓ `cool_nash_0x294::sub_433DD0` | done | Ped::DoJump_433C40 (new) + Set_F250_IfBit_433DD0; 162->0 CANDIDATE MATCH; sub_43E1E0 = Ped::sub_45C5C0 (still called) |
| 0x566EE0 | `Player::sub_566EE0` | 0x4A5E90 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60` | done | GetPlayerPed_4A5130 instead of the equivalent Get_Field_68_Ped; code unchanged |
| 0x5670B0 | `Player::RespawnPlayer_5670B0` | 0x4A6050 | ✓ `sub_434B10`, `sub_4A4D50`, ✓ `thirsty_lamarr::sub_41DC30`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | IsNetworkGame_434B10 x2, get_value, get_cam_x/y; sub_4A4D50 = ChangeLifeCountByAmount_5699F0 (still called); code unchanged |
| 0x567130 | `Player::Wasted_567130` | 0x4A7910 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `Game_0x40::get_player_4219E0`, `sub_4822A0`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `thirsty_lamarr::sub_41DC30`, `sub_4105B0`, ✓ `sub_434950`, ✓ `DrawUnk_0xBC::sub_475B60` | done | ClearBit11_403A40 x3, get_value x3, get_player_4219E0; code unchanged; sub_4822A0 = UpdateFugitive_516740 (still called) |
| 0x5679E0 | `Player::Busted_5679E0` | 0x4A7BA0 | ✓ `cool_nash_0x294::sub_403A40`, `sub_4105B0`, ✓ `sub_434950`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `thirsty_lamarr::sub_41DC30`, ✓ `sub_434920`, ✓ `sub_434940`, ✓ `sub_4A50B0`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | Player::HasPowerUp_434920 + DecPowerUp_434940 (new), SetValueClamped_4A50B0, get_value, ClearBit11_403A40 x3, ClearWantedPoints_420B80 x2; code unchanged; sub_4105B0 is a thunk on an unknown 9.6f global |
| 0x568670 | `Player::sub_568670` | 0x4A4CB0 | `sub_41E510`, `sub_41E4E0` | checked | 9.6f sub_41E510/sub_41E4E0 are Camera_0xBC::sub_435810/sub_4357F0, still called |
| 0x568730 | `Player::sub_568730` | 0x4A61C0 | `sub_4354C0`, ✓ `sub_4A5170` | checked | 9.6f sub_4354C0 = Ped::sub_470300, still called; set_bInUse_4A5170 already used |
| 0x5687F0 | `Player::Service_5687F0` | 0x4A7E80 | `sub_4A6100`, ✓ `sub_4A5070`, `sub_41E580`, ✓ `cool_nash_0x294::sub_403990` | checked | inline_set_ped_id_to_2 (9.6f 0x4A5070) already used; 4A6100 (zone update) and 41E580 correspond to calls in the source |
| 0x569410 | `Player::sub_569410` | 0x4A61F0 | ✓ `sub_4A5070` | checked | inline_set_ped_id_to_2 (9.6f 0x4A5070) already used |
| 0x569530 | `Player::sub_569530` | 0x4A62B0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_475C80` | done | Car_BC::SetF98To3IfNot4_475C80 (new); code unchanged |
| 0x5695A0 | `Player::sub_5695A0` | 0x4A6310 | ✓ `angry_lewin_0x85C::sub_4A5100`, ✓ `DrawUnk_0xBC::sub_475B60` | done | Player::sub_4A5100 (noted) + inline_set_ped_id_to_1_475B60; code unchanged |
| 0x569600 | `Player::sub_569600` | 0x4A6350 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, `cool_nash_0x294::sub_435C40`, `cool_nash_0x294::sub_436040`, ✓ `Car_BC::sub_475C10`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410` | done | Car_BC::SetF98To1IfNot4_475C10; sub_435C40 = Ped::sub_45B560 and sub_436040 = Ped::sub_45C4B0 (both still called); code unchanged |
| 0x5696D0 | `Player::sub_5696D0` | 0x4A6400 | ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410` | done | all 9.6f inlines used |
| 0x569920 | `Player::get_pos_569920` | 0x4A6610 | ✓ `sub_4A5150`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `angry_lewin_0x85C::get_camera_434900`, `sub_4A5090` | done | Player::GetActivePed_4A5150 (new), get_camera_434900, Car_BC get_x/y/z_41E4x0; 4A5090 (cam z + dword_6FE618) left open-coded (global is local to Player.cpp); code unchanged |
| 0x569CB0 | `Player::DoRestartZone_569CB0` | 0x4A81E0 | ✓ `sub_475A20`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x569E70 | `Player::sub_569E70` | 0x4A8340 | ✓ `angry_lewin_0x85C::sub_4A5100` | done | all 9.6f inlines used |
| 0x569F40 | `Player::DisableInputs_569F40` | 0x4A6900 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040`, ✓ `Car_BC::sub_403BA0` | done | Ped::ClearBit11_403A40; code unchanged |
| 0x56A0F0 | `Player::RestoreCarsFromSave_56A0F0` | 0x4A6A80 | ✓ `EnqueueRadioLocationPhrase_426E10`, `GetRaw_4A5190`, ✓ `sub_4A51B0` | matched | 426E10 = a SpawnCar wrapper using Player.cpp's dword_6FE614 copy (Car_6C::SpawnCar_426E10 uses dword_6F77C4, so not reusable); 4A51A0 is BitSet32::operator= but field_8 is CarFlags; set_damage_4A51B0 already used; 60 unchanged |
| 0x56A1A0 | `Player::CopyPlayerDataToSave_56A1A0` | 0x4A6B20 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_421980`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_433B70`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_4766C0`, ✓ `sub_4A4FB0` | done | Player::get_lives_4766C0 + GetScore_421980 (new), get_multiplier_4766A0, get_health_433B70, get_remap_433BA0; get_ammo_4A4FB0 already used; code unchanged |
| 0x56A310 | `Player::UpdateGameFromSave_56A310` | 0x4A6C80 | ✓ `sub_4A50B0`, ✓ `sub_4A50E0`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::sub_420B80` | done | SetValueClamped_4A50B0 x2, SetMoney_4A50E0, ClearWantedPoints_420B80; code unchanged |
| 0x56A490 | `Player::ApplyCheats_56A490` | 0x4A6DA0 | ✓ `sub_4A50E0`, ✓ `sub_4A4F90`, ✓ `sub_4A50B0` | done | also SetValueClamped_4A50B0(99) for lives; code unchanged |
| 0x56A740 | `Player::ctor_56A740` | 0x4A83C0 | `sub_4A6FC0`, ✓ `sub_4A5180`, ✓ `sub_434950`, `sub_409DA0` (10.5 0x521100) | checked | sub_4A5180 already a helper; sub_4A6FC0 is the chat sub-object ctor (fields set directly), sub_409DA0 a member ctor |
| 0x56A940 | `Player::dtor_56A940` | 0x4A6EF0 | ✓ `root_sound::DestroySoundObj_40FE60`, `sub_4A4F10` | checked | DestroySoundObj_40FE60 already used; sub_4A4F10 a member dtor |
| 0x56B6E0 | `jolly_poitras_0x2BC0::ctor_56B6E0` | 0x4A9290 | `sub_4A8910`, `sub_4A9050` | checked | sub_4A9050 = HiScoreHscExists_56BCA0, sub_4A8910 = 10.5 out-of-line call (pairing) |
| 0x56B990 | `jolly_poitras_0x2BC0::sub_56B990` | 0x4A8CB0 | `sub_4A8B60` (10.5 0x56BCF0) | checked | sub_4A8B60 is GetHiScoreHscFileName_56BCF0 (called) |
| 0x56BB10 | `jolly_poitras_0x2BC0::sub_56BB10` | 0x4A8F90 | ✓ `sub_453A60`, `j_thirsty_lamarr::sub_41DC30` | done | DecodeStage_453A60; thirsty_lamarr get_value via j_ thunk = GetScore_592370 (called); still matches |
| 0x56C010 | `jolly_poitras_0x2BC0::sub_56C010` | 0x4A90A0 | ✓ `sub_453A60` | done | DecodeStage_453A60; unchanged (14) |
| 0x56F5C0 | `Police_7B8::SpawnRoadblockGuard_56F5C0` | 0x4A9B40 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x56F6D0 | `Police_7B8::DespawnCrewInCar_56F6D0` | 0x4A9C50 | ✓ `sub_421470` | done | code unchanged |
| 0x56F940 | `Police_7B8::sub_56F940` | 0x4A9D60 | ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | code unchanged |
| 0x56FA40 | `Police_7B8::UpdatePlayerPursuitTimer_56FA40` | 0x4A9E80 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403B60` | done | code unchanged |
| 0x56FBD0 | `Police_7B8::UpdatePursuitTargets_56FBD0` | 0x4AA030 | ✓ `sub_420B70`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403B60`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0`, ✓ `cool_nash_0x294::sub_420B80` | done | code unchanged; new Ped::ClearWantedPoints_420B80 |
| 0x570270 | `Police_7B8::Service_570270` | 0x4AEF70 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40` | done | code unchanged |
| 0x570320 | `Police_7B8::SpawnWalkingGuard_570320` | 0x4AA710 | ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_433B90`, `cool_nash_0x294::get_remap_433BA0`, `sub_433C10`, ✓ `sub_433C00`, `cool_nash_0x294::sub_433DF0` | inlines added | code unchanged; SetRemap_433C10(get_remap_433BA0()) breaks the match |
| 0x5703E0 | `Police_7B8::SpawnCrewInCar_5703E0` | 0x4AA7B0 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433B90`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::sub_421560` | done | code unchanged |
| 0x5707B0 | `Police_7B8::TryAssignCarCrewToCriminal_5707B0` | 0x4AABB0 | ✓ `sub_41B0A0` | matched | all 9.6f inlines used |
| 0x5708C0 | `Police_7B8::UpdateLastSeenCoordsForCriminal_5708C0` | 0x4AACA0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | all 9.6f inlines used |
| 0x570940 | `Police_7B8::UpdateCriminalLatestPosition_570940` | 0x4AAD40 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50` | done | all 9.6f inlines used |
| 0x570BF0 | `PoliceCrew_38::SpawnPoliceInCar_570BF0` | 0x4AADD0 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::sub_421560` | done | code unchanged |
| 0x570E30 | `PoliceCrew_38::SpawnSWAT_570E30` | 0x4AB060 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::sub_421560` | done | code unchanged |
| 0x571150 | `PoliceCrew_38::SpawnFBI_nonused_571150` | 0x4AB220 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `Car_BC::sub_421560` | checked | 43DF60 = PedManager::AllocatePed_470F30 |
| 0x571350 | `PoliceCrew_38::sub_571350` | 0x4AB330 | ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_4039F0` | done | code unchanged |
| 0x571540 | `PoliceCrew_38::sub_571540` | 0x4AB400 | ✓ `sub_4A9AD0`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `sub_421470`, ✓ `cool_nash_0x294::sub_4039F0` | matched | Third car branch is `MarkForDespawn_421470()` + the shared 28/2C tail; Get_F76/Get_F20E/ClearGroupAndGroupIdx inlines |
| 0x572210 | `PoliceCrew_38::sub_572210` | 0x4AB930 | ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0` | matched | all 9.6f inlines used |
| 0x572340 | `PoliceCrew_38::State3_Arrest_572340` | 0x4AC080 | ✓ `sub_421470`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `sub_472FD0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `sub_450CB0` | done | code unchanged; new Ped::IsGroupLeader_472FD0 |
| 0x574F10 | `PoliceCrew_38::State1_574F10` | 0x4AD310 | ✓ `sub_421550`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, `sub_403960`, ✓ `sub_450CB0` | done | code unchanged |
| 0x575210 | `PoliceCrew_38::sub_575210` | 0x4AB9D0 | ✓ `sub_450CB0`, ✓ `sub_421540` | done | code unchanged |
| 0x575270 | `PoliceCrew_38::sub_575270` | 0x4ABA30 | ✓ `sub_450CB0` | done | all 9.6f inlines used |
| 0x5752C0 | `PoliceCrew_38::sub_5752C0` | 0x4ABA90 | ✓ `sub_450CB0` | done | all 9.6f inlines used |
| 0x575590 | `PoliceCrew_38::Service_575590` | 0x4AD600 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40`, `sub_404400` | done | code unchanged |
| 0x5757B0 | `PoliceRoadblock_A4::sub_5757B0` | 0x4AD6C0 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `MaxAbsDistance_42A6B0` | done | code unchanged |
| 0x575CA0 | `PoliceRoadblock_A4::sub_575CA0` | 0x4ABD70 | ✓ `sub_421470`, ✓ `Car_BC::sub_421560`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x578030 | `Train_58::ReassignTrainHead_578030` | 0x4AF8A0 | ✓ `Car_BC_Pool::sub_420F20`, ✓ `sub_420F30`, `Car_BC::sub_423A50`, `CarPhysics_B0::sub_49EE10` (10.5 0x563670), ✓ `sub_4118D0`, ✓ `sub_4212B0`, ✓ `sub_4212A0`, ✓ `sub_421510`, ✓ `sub_420B70`, ✓ `Car_BC::sub_421560` | matched | 74->54; UpdateNextPrev(420F20), is_driven_by_player(4118D0), SetField8C_to_1/2, SetUniNum_421560 used; 421510 open-coded (alloc AI, fixed DeAllocate bug) since Car_BC.hpp can't see gCarAI_78_Pool |
| 0x578330 | `Train_58::sub_578330` | 0x4AFAB0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x578360 | `Train_58::sub_578360` | 0x4AFAF0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x579A30 | `PublicTransport_181C::sub_579A30` | 0x4B0D70 | `Car_BC::sub_421D90` | checked | 421D90 = GetCarLinearSpeed_43A240 (called) |
| 0x579B90 | `PublicTransport_181C::sub_579B90` | 0x4B0DF0 | ✓ `Car_BC::sub_4118C0` | done | all 9.6f inlines used |
| 0x57B540 | `PublicTransport_181C::GetLeadTrainCar_57B540` | 0x4B1B40 | ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x57F090 | `sound_obj::IsPoliceOrServiceVehicle_57F090` | 0x4B2510 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x582310 | `Car_14::ctor_582310` | 0x4B3490 | ✓ `Car_14_18::ctor_41D070` | done | all 9.6f inlines used |
| 0x583670 | `Car_14::GenerateTraffic_583670` | 0x4B4E60 | ✓ `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `sub_433E90` | commented | get_cam_y breaks the match (get_cam_x already used) |
| 0x5885C0 | `Junction_10::sub_5885C0` | 0x40D0C0 | ✓ `sub_40CE90` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588810 | `RouteFinder::RoadOff_588810` | 0x40E1B0 | ✓ `sub_40CE90`, ✓ `Disable_40CEC0` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588950 | `RouteFinder::RoadOn_588950` | 0x40E2F0 | ✓ `sub_40CE90`, ✓ `Enable_40CEB0` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588B30 | `RouteFinder::Load_RGEN_588B30` | 0x40D250 | ✓ `sub_40CE90` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588E60 | `RouteFinder::sub_588E60` | 0x40D390 | ✓ `sub_40CF20` | done | all 9.6f inlines used |
| 0x588F30 | `RouteFinder::sub_588F30` | 0x40D450 | ✓ `sub_40CF20` | done | all 9.6f inlines used |
| 0x5895C0 | `RouteFinder::sub_5895C0` | 0x40D7D0 | ✓ `sub_40CE90`, ✓ `sub_40CEA0` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x5899C0 | `RouteFinder::sub_5899C0` | 0x40DB70 | ✓ `sub_40CE90`, ✓ `sub_40CED0`, ✓ `sub_40CEA0` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x589BB0 | `RouteFinder::sub_589BB0` | 0x40DD50 | ✓ `sub_40CE90`, ✓ `sub_40CED0`, ✓ `sub_40CEA0` | done | Link_2 helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x58A190 | `RouteFinder::StartRoute_58A190` | 0x40E030 | `sub_40D690` (10.5 0x589480), `sub_40DFA0` (10.5 0x589E20), `sub_40CC10` | checked | 9.6f body inlined DoStartRoute pieces (40D690=589480, 40DFA0=589E20 called in 10.5 via DoStartRoute_58A0D0) |
| 0x58A1C0 | `RouteFinder::ctor_58A1C0` | 0x40E100 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler's vector ctor helper |
| 0x591C70 | `eager_benz::sub_591C70` | 0x4B7770 | ✓ `sub_41D020`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::inline_check_0x20_info_4216C0`, `Char_8::sub_420EA0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::sub_41DC70`, `sub_4105B0`, ✓ `CarPhysics_B0::is_backward_gas_on_411810`, `Car_BC::sub_421EC0`, ✓ `angry_lewin_0x85C::sub_4219D0`, `Game_0x40::sub_4B7590`, `rng::get_cur_rng_41CFE0` | done | Ped::get_car_416B60, not_enter_car_as_passenger_4A5040, get_wanted_points_433DC0 and Car_BC::inline_check_0x20_info_4216C0; code unchanged |
| 0x592380 | `eager_benz::sub_592380` | 0x4B75B0 | ✓ `gtx_0x106C::sub_420200`, `sub_4BFFE0` | checked | does_car_exist (420200) already used; sub_4BFFE0 = IsCarModelInRecycleList_5AB380 (still called) |
| 0x592430 | `eager_benz::sub_592430` | 0x4B7D80 | ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::sub_41DC70`, `sub_4105B0` | inlines added | Player::Add_2D4 for the AddCash calls; code unchanged |
| 0x592620 | `eager_benz::AddCash_592620` | 0x4B7660 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x592DD0 | `eager_benz::sub_592DD0` | 0x4B85B0 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::get_car_416B60`, ✓ `sub_41C1F0`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_4118F0`, ✓ `sub_421790`, ✓ `sub_421780`, ✓ `sub_4217A0`, ✓ `sub_411920`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier_4766A0, Add_2D4, GetPlayerPed_4A5130 and Car_BC::get_x/y/z_41E4x0; code unchanged |
| 0x593030 | `eager_benz::sub_593030` | 0x4B8870 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier_4766A0, Add_2D4, GetPlayerPed_4A5130 and Car_BC::get_x/y/z_41E4x0; code unchanged |
| 0x593150 | `eager_benz::sub_593150` | 0x4B89B0 | ✓ `IsMaxDamage_40F890`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A B C (A=get_multiplier_4766A0, B=Add_2D4, C=GetPlayerPed_4A5130, D=Car_BC::get_x/y/z_41E4x0, E=IsUser_41DC70); code unchanged |
| 0x593240 | `eager_benz::sub_593240` | 0x4B8A70 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_41C1F0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A C D E (A=get_multiplier_4766A0, B=Add_2D4, C=GetPlayerPed_4A5130, D=Car_BC::get_x/y/z_41E4x0, E=IsUser_41DC70); code unchanged |
| 0x593370 | `eager_benz::sub_593370` | 0x4B8BD0 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::Add_2D4 and get_multiplier_4766A0 for the score; code unchanged |
| 0x593410 | `eager_benz::sub_593410` | 0x4B8C80 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A C D (A=get_multiplier_4766A0, B=Add_2D4, C=GetPlayerPed_4A5130, D=Car_BC::get_x/y/z_41E4x0, E=IsUser_41DC70); code unchanged |
| 0x5934F0 | `eager_benz::UpdateAccuracyCount_5934F0` | 0x4B76A0 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x5935D0 | `eager_benz::ChangeFragsByAmount_5935D0` | 0x4B8A60 | `sub_41DC40` | checked | pairing noise (9.6f sub_41DC40 is Add_2D4, not used by ChangeFrags) |
| 0x596890 | `ExplodingScore_100::PushScore_596890` | 0x4B91F0 | ✓ `CokeZero_FC::sub_4B8FD0` (10.5 0x5935C0), `CokeZero_FC::sub_4B9000`, `CokeZero_FC::sub_4B8FE0` | done | new Pool::GetFirstUsed_4B8FD0; 4B9000/4B8FE0 = FindAndDeAllocate/Allocate already used; still matches |
| 0x5969E0 | `ExplodingScore_100::DrawExploding_5969E0` | 0x4B98B0 | ✓ `CokeZero_FC::sub_4B8FD0` (10.5 0x5935C0) | done | Pool::GetFirstUsed_4B8FD0; still matches |
| 0x59DDF0 | `Fix16_Rect::IntersectsSpriteRenderingRect_59DDF0` | 0x4BA6C0 | ✓ `sub_4B9FD0` | done | all 9.6f inlines used |
| 0x59DE80 | `Fix16_Rect::CanRectEnterMovementRegion_59DE80` | 0x4BA720 | `sub_4BA5E0` | matched | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect_59DD60, called in 10.5 |
| 0x59E170 | `Sprite::IsControlledByActivePlayer_59E170` | 0x4BCA80 | `sub_4BAA70`, ✓ `sub_40FEB0`, `sub_423480`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | Sprite::AsCar_40FEB0; Still matches. |
| 0x59E1D0 | `Sprite::IsOnWater_59E1D0` | 0x4BAA90 | ✓ `sub_49E540` | done | gtx IsRemappedWaterTile_49E540; Still matches. |
| 0x59E250 | `Sprite::GetWaterCornerMask_59E250` | 0x4BDD40 | ✓ `sub_4B9F40` | done | all 9.6f inlines used |
| 0x59E320 | `Sprite::sub_59E320` | 0x4BAB10 | ✓ `sub_4BA230` | done | New Sprite::GetTrueSpriteIdx_4BA230 (defined in sprite.cpp); Still matches. |
| 0x59E590 | `Sprite::CollisionCheck_59E590` | 0x4BCAC0 | ✓ `sub_41E390` | done | all 9.6f inlines used |
| 0x59E7D0 | `Sprite::QuerySpriteCollision_59E7D0` | 0x4BDFE0 | `sub_4B9F30`, ✓ `sub_40FEE0` | done | Rozza_C88::SetSprite_40FEE0; Still matches. |
| 0x59E8C0 | `Sprite::HandleObjectCollision_59E8C0` | 0x4BAB70 | ✓ `sub_40FEC0`, ✓ `sub_484DD0` | commented | As2C_40FEC0 for both sprites broke the match |
| 0x59E9C0 | `Sprite::UpdateCollisionBoundsIfNeeded_59E9C0` | 0x4BCB40 | ✓ `sub_41E390` | done | all 9.6f inlines used |
| 0x59F950 | `Sprite::AllocInternal_59F950` | 0x4BCB90 | `Sprite_4C_Pool::sub_4BC9F0`, ✓ `Sprite_4C::sub_482980` | done | Sprite_4C::SetDimensions_482980; Still matches. |
| 0x59F990 | `Sprite::Update_4C_59F990` | 0x4BCBD0 | `Sprite_4C_Pool::sub_4BC9F0`, ✓ `sub_4BA070` | done | Sprite::GetTrueSpriteIdx_4BA230 (pool Allocate is 9.6f 0x4BC9F0); Still matches. |
| 0x59FA40 | `Sprite::UpdateDimensionsFromSpriteIndex_59FA40` | 0x420600 | `Car_3C::sub_4B99F0` | done | Sprite::GetTrueSpriteIdx_4BA230 (sub_4B99F0 is ResetZCollisionAndDebugBoxes_59E7B0); Still matches. |
| 0x59FAD0 | `Sprite::FreeSprite4CChildren_59FAD0` | 0x4BCCC0 | `Sprite_4C_Pool::sub_4BCA10` | checked | Sprite_4C_Pool::sub_4BCA10 is the pool Remove template, already used |
| 0x5A0970 | `Sprite::CheckBBoxScanlineIntersection_5A0970` | 0x4BB860 | `sub_472950`, ✓ `sub_4BA250` | commented | SetHorizontalSegment_4BA250 changes store order (broke match); 472950=ComputeScanlineIntersectionY called |
| 0x5A0EF0 | `Sprite::HitTestVerticalLine_5A0EF0` | 0x4BB910 | `sub_472AE0`, ✓ `sub_4BA280` | done | Rozza_C88::SetVerticalSegment_4BA280; Still matches. |
| 0x5A1A60 | `Sprite::sub_5A1A60` | 0x4BD290 | ✓ `sub_4B9F80` | done | New file-local ClampPurpleRectToMap_4B9F80; Still matches. |
| 0x5A1B30 | `Sprite::ResolveZOrder_5A1B30` | 0x4BE570 | ✓ `sub_4BA220` | done | all 9.6f inlines used |
| 0x5A1BD0 | `Sprite::ComputeZLayer_5A1BD0` | 0x4BD2E0 | ✓ `Car_BC::sub_403BA0`, `sub_491EE0` | checked | sub_491EE0 is Fix16::ToInt (already used) |
| 0x5A1CA0 | `Sprite::CheckCornerZCollisions_5A1CA0` | 0x4BD350 | ✓ `sub_492170` | commented | GetF8_492170 changes the code (broke match) |
| 0x5A2A30 | `Sprite::ResolveCollisionWithCarPedOrObject_5A2A30` | 0x4BDAD0 | `struct_4::ctor_424620`, ✓ `sub_416B40`, ✓ `sub_40FEB0`, ✓ `sub_4BA390`, ✓ `HasOtherCarOnTrailer_475E60`, ✓ `sub_40FEA0`, ✓ `sub_40FE80`, ✓ `sub_40FEC0` | checked | struct_4 ctor (424620) already used |
| 0x5A3100 | `Sprite::DispatchCollisionEvent_5A3100` | 0x4B9D50 | `sub_484880`, `sub_422260` | checked | sub_484880 = EnsureObject3C/ReactivateObjectAfterImpact pieces, sub_422260 = Car_BC::Deactivate_43AA60 (called) |
| 0x5A4D90 | `Sprite_4C::SetCurrentRect_5A4D90` | 0x4BC580 | `sub_4BA5E0` | checked | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect_59DD60, called in 10.5 |
| 0x5A57B0 | `Sprite_4C::ctor_5A57B0` | 0x4BC900 | `Sprite_4C::sub_4BC8F0` | checked | Sprite_4C::sub_4BC8F0 is PoolAllocate, already called |
| 0x5A5870 | `Sprite_8::sub_5A5870` | 0x4BDC60 | ✓ `Sprite_Pool::sub_421000` | done | all 9.6f inlines used |
| 0x5A58A0 | `Sprite_8::ctor_5A58A0` | 0x4BE5B0 | ✓ `Sprite_Pool::ctor_4BCA20`, ✓ `Sprite_4C_Pool::ctor_4BC9A0`, ✓ `Sprite_18_Pool::ctor_4BDCF0` | done | all 9.6f inlines used |
| 0x5A5B50 | `Sprite_8::dtor_5A5B50` | 0x4BE730 | `Sprite_Pool::gdtor_4BDC90`, `Sprite_4C_Pool::gdtor_4BDCB0`, `Sprite_3CC::gdtor_4B9F00`, `Sprite_18_Pool::gdtor_4BE710` | checked | pool gdtors are compiler scalar deleting dtors |
| 0x5A6910 | `Sprite_18::sub_5A6910` | 0x4BE870 | ✓ `sub_40FEC0`, `Object_2C_Pool::sub_484DB0`, ✓ `sub_40FEB0`, `Car_BC::sub_429FF0`, `sub_40F490`, ✓ `sub_40FEA0` | checked | 484DB0 = Object_2C pool DeAllocate (template, used), 429FF0 = UpdateAttachedToSprite_443360 (called) |
| 0x5A6A50 | `Object_3C::GetSpriteForModel_5A6A50` | 0x4BE980 | ✓ `sub_40FEC0` | done | As2C_40FEC0 used; code unchanged |
| 0x5A6AD0 | `struct_4::FindFirstActiveObject_5A6AD0` | 0x4BEA60 | `rng::get_cur_rng_41CFE0` | checked | pairing error: 9.6f 0x4BEA60 is a different function (rng store) |
| 0x5A6B10 | `Object_3C::RemoveSprite_5A6B10` | 0x4BEC60 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = Sprite_18 pool DeAllocate (already) |
| 0x5A6B60 | `Object_3C::RemoveSpriteSafe_5A6B60` | 0x4BECB0 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6BB0 | `Object_3C::sub_5A6BB0` | 0x4BEA20 | `sub_4BE920` | checked | 4BE920 = Sprite_18::sub_5A69E0, already its own helper |
| 0x5A6BD0 | `Object_3C::sub_5A6BD0` | 0x4BEA40 | `sub_4BE950` | checked | 4BE950 = Sprite_18::sub_5A6A20, already its own helper |
| 0x5A6BF0 | `Object_3C::DispatchCarImpactEvents_5A6BF0` | 0x4BE7A0 | `sub_4B9A80` | checked | 4B9A80 = Sprite::ProcessCarToCarImpactIfCar_59E910, already used |
| 0x5A6C40 | `struct_4::RemoveByRngValue_5A6C40` | 0x4BED00 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6D00 | `Object_3C::PushImpactEvent_5A6D00` | 0x4BED90 | `Sprite_18_Pool::sub_4BEC40` | checked | 4BEC40 = Sprite_18 pool Allocate (already) |
| 0x5A6D40 | `Object_3C::PushSprite_5A6D40` | 0x4BEDD0 | `Sprite_18_Pool::sub_4BEC40` | checked | 4BEC40 = Allocate (already) |
| 0x5A6DC0 | `struct_4::PopBackSprite_5A6DC0` | 0x4BEE30 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6E10 | `Object_3C::ClearList_5A6E10` | 0x4BEE80 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6E40 | `Object_3C::FindClosestSprite_5A6E40` | 0x4BEAC0 | ✓ `MaxAbsDistance_42A6B0` | commented | MaxAbsDistance_42A6B0 changes the matching code |
| 0x5A6EA0 | `Object_3C::TakeClosestSprite_5A6EA0` | 0x4BEEB0 | `MaxAbsDistance_42A6B0`, `Sprite_18_Pool::sub_4BEC50` | commented | MaxAbsDistance_42A6B0 worse (4->9); 4BEC50 = DeAllocate (already) |
| 0x5A6F70 | `struct_4::PoolUpdate_5A6F70` | 0x4BEF70 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A7010 | `struct_4::DestroyAllSprites_5A7010` | 0x4BF000 | ✓ `sub_416B40`, `sub_428F70`, `sub_485260` | checked | 428F70 = Car_6C::RemoveFromPoolAndCollision_446730, 485260 = RemoveAndFree_52A610 (called) |
| 0x5A7110 | `struct_4::ClearGangIconSprite_5A7110` | 0x4BF100 | ✓ `sub_40FEC0`, ✓ `sub_4BE850`, `sub_485260`, `Sprite_18_Pool::sub_4BEC50` | done | As2C_40FEC0 + IsGangIcon_4BE850 (added) used; 485260/4BEC50 called/already; code unchanged |
| 0x5A71F0 | `Object_3C::sub_5A71F0` | 0x4BEB70 | ✓ `sub_416B40`, ✓ `sub_4BE830` | done | Object_2C::sub_4BE830 used; code unchanged |
| 0x5A7240 | `Object_3C::PruneNonCollidingSprites_5A7240` | 0x4BF180 | `Sprite_18_Pool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A72B0 | `Object_3C::PropagateMaxZLayer_5A72B0` | 0x4BEBC0 | ✓ `sub_446950`, ✓ `sub_4BA220` | done | Sprite::IsTypeAbove1_446950 added+used; code unchanged |
| 0x5AB0F0 | `gtx_0x106C::load_font_base_5AB0F0` | 0x4BFDD0 | `sub_4BF790` | checked | sub_4BF790 is ConvertToVirtualOffsets_5AB1A0 (called) |
| 0x5AB2C0 | `gtx_0x106C::load_palete_base_5AB2C0` | 0x4BFF20 | `sub_4BF790` | checked | sub_4BF790 is ConvertToVirtualOffsets_5AB1A0 (called) |
| 0x5AB750 | `gtx_0x106C::LoadSty_5AB750` | 0x4C0820 | `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version already used |
| 0x5AB820 | `gtx_0x106C::ctor_5AB820` | 0x4C08D0 | ✓ `sub_4C03D0` | done | New gtx_0x106C::ResetSpecs_4C03D0; Still matches. |
| 0x5AE060 | `Taxi_4::ctor_5AE060` | 0x4C09C0 | ✓ `Taxi_4::sub_4C09B0`, `Taxi_4_Pool::ctor_4C0950` | checked | 4C09B0/4C0950 = Init_4C09B0 / Taxi_4_Pool ctor (already) |
| 0x5AE0D0 | `Taxi_4::dtor_5AE0D0` | 0x4C0A10 | `Taxi_4_Pool::gdtor_4C0990` | checked | 4C0990 = compiler deleting dtor |
| 0x5B1170 | `sub_5B1170` | 0x4C0AD0 | `sub_4BF550`, `gmp_block_info::init_44C840`, `sub_4C0A80`, `sub_475C30`, `sub_476270`, `sub_426E40`, `sub_475A50` | checked | debug-only ImGui reimplementation; 9.6f callees (4BF550 etc.) pair with called 10.5 functions |
| 0x5B2640 | `DoTest_5B2640` | 0x4C1F80 | ✓ `sub_4766B0`, ✓ `sub_476860` | done | Hud_Arrow_7C::SetArrowTargetCar_476860; code unchanged |
| 0x5B5E90 | `text_0x14::Load_5B5E90` | 0x4C2540 | `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version (460EE0/460EC0) already used |
| 0x5B5FB0 | `text_0x14::ctor_5B5FB0` | 0x4C2620 | `text_tkey::ctor_4C23F0`, `text_tdat::ctor_4C2420` | checked | text_tkey/text_tdat ctors (4C23F0/4C2420) already used as member ctors |
| 0x5B6050 | `text_0x14::dtor_5B6050` | 0x4C26C0 | `text_tdat::dtor_4C2430`, `text_tkey::dtor_4C2400` | checked | text_tkey/text_tdat dtors (4C2400/4C2430) already used as member dtors |
| 0x5B9180 | `sharp_pare_0x15D8::LoadTextures2_5B9180` | 0x4C2F90 | `sub_4C2EF0` | checked | sub_4C2EF0 is Sprite_3CC::get_s14 (already used) |
| 0x5B9350 | `sharp_pare_0x15D8::sub_5B9350` | 0x4C30A0 | `sub_4C2F30` | checked | sub_4C2F30 pairs with an out-of-line texture setup call in 10.5 |
| 0x5B9790 | `sharp_pare_0x15D8::ctor_5B9790` | 0x4C3190 | `array_constuctor_401CF0`, `festive_hopper::ctor_4C2F10` | checked | festive_hopper ctor (4C2F10) is already an inline ctor; array_constuctor_401CF0 is the compiler helper |
| 0x5BC260 | `TileAnim_2::sub_5BC260` | 0x4C3430 | `sub_4C3380`, ✓ `sub_4C33F0` | done | New TileAnim_18::Init_4C33F0 (sub_4C3380 is the pool Allocate); Still matches. |
| 0x5BC2C0 | `TileAnim_2::sub_5BC2C0` | 0x4C3470 | `sub_4C3380` | checked | sub_4C3380 is the TileAnim pool Allocate (template, used) |
| 0x5BC3A0 | `TileAnim_2::ctor_5BC3A0` | 0x4C35A0 | `TileAnimPool::ctor_4C34B0` | checked | TileAnimPool ctor is the pool template ctor |
| 0x5BC470 | `TileAnim_2::dtor_5BC470` | 0x4C3650 | `TileAnimPool::gdtor_4C3630` | checked | TileAnimPool gdtor is the compiler scalar deleting dtor |
| 0x5BEE90 | `get_rdtsc_5BEE90` | 0x4C3950 | `sub_4C3850` | checked | sub_4C3850 is the rdtsc body itself (10.5 has it in get_rdtsc_5BEE90 directly, inline asm) |
| 0x5C2950 | `TrafficLights_194::TrafficLightsService_5C2950` | 0x4C4A60 | ✓ `sub_4C39F0`, `sub_4C3A10` | checked | sub_4C39F0 already used; 4C3A10 = called debug/update function (pairing) |
| 0x5C2AC0 | `TrafficLights_194::sub_5C2AC0` | 0x4C4B00 | `TrafficLights_194::sub_4C4A30` | checked | 4C4A30 = AddTrafficLight_5C2910 (called) |
| 0x5C5DF0 | `Montana_4::Draw_5C5DF0` | 0x4C4D20 | ✓ `Montana_FA4::Push_4C4B80`, ✓ `Montana_FA4::IsEnd_4C4BC0`, ✓ `Montana_FA4::Pop_4C4BA0`, `Car_3C::sub_4BE060` | checked | Car_3C::sub_4BE060 is Sprite::Draw_59EFF0, called |
| 0x5C5E50 | `Montana_4::Reset_5C5E50` | 0x4C4D60 | ✓ `Montana_2EE4::sub_4C4B70` | done | new Montana_2EE4::Reset_4C4B70; still matches |
| 0x5C5E70 | `Montana_4::ctor_5C5E70` | 0x4C4DF0 | `Montana_FA4::ctor_4C4BD0` | matched | Montana_FA4 ctor (4C4BD0) already inline in class |
| 0x5C5F60 | `Montana_2EE4::ctor_5C5F60` | 0x4C4DC0 | ✓ `Montana_2EE4::sub_4C4B70` | done | Montana_2EE4::Reset_4C4B70; still matches |
| 0x5C86C0 | `Car_214::sub_5C86C0` | 0x4C4F30 | ✓ `sub_4C4F10` | done | new Object_2C::set_field_27_4C4F10, code unchanged |
| 0x5CBC90 | `Kfc_30::ReplaceLeaderIfNeeded_5CBC90` | 0x4C5510 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::set_occupation_403970` | done | all 9.6f inlines used |
| 0x5CC1C0 | `Kfc_30::CleanupExpiredEntities_5CC1C0` | 0x4C5A00 | ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, ✓ `sub_4A9AD0`, ✓ `sub_421470`, ✓ `cool_nash_0x294::sub_4039F0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403A30` | done | all 9.6f inlines used |
| 0x5CF910 | `Garox_110C_sub::Draw_5CF910` | 0x4C74A0 | ✓ `angry_lewin_0x85C::get_camera_434900` | done | Player::get_camera_434900; Still matches. |
| 0x5CF970 | `Garox_27B5_sub::sub_5CF970` | 0x4CA680 | ✓ `sub_4A5150`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `cool_nash_0x294::sub_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `Garox_C4::sub_45AFD0` | done | Garox_C4::SetDrawKind8_45AFD0; Still matches. |
| 0x5D00B0 | `Hud_CopHead_C_Array::UpdateWantedLevel_5D00B0` | 0x4C79D0 | ✓ `sub_41D020` | done | all 9.6f inlines used |
| 0x5D0260 | `Garox_1108_sub::DrawHealth_5D0260` | 0x4C7B70 | ✓ `sub_41D020`, ✓ `sub_433B70` | done | all 9.6f inlines used |
| 0x5D03C0 | `ArrowTrace_24::PointToInfoPhone_5D03C0` | 0x4C7CC0 | ✓ `sub_4767C0` | done | all 9.6f inlines used |
| 0x5D03F0 | `ArrowTrace_24::UpdateAimCoordinates_5D03F0` | 0x4C7CF0 | ✓ `sub_461DB0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, `cool_nash_0x294::sub_433B40`, ✓ `sub_4215B0`, `sub_476830`, `sub_4117B0`, ✓ `angry_lewin_0x85C::get_camera_434900` | done | Player::get_camera_434900; Still matches. |
| 0x5D0530 | `Hud_Arrow_7C::CheckVisibility_5D0530` | 0x4CA770 | ✓ `sub_4C70B0`, ✓ `sub_4C7350`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `sub_4C7340`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | New Hud_Arrow_7C_Array::ShowGangArrows_4C70B0 and frosty_pasteur_0xC1EA8::IsOnMission_4C7350; Still matches. |
| 0x5D0C60 | `Hud_Arrow_7C::Service_5D0C60` | 0x4CA860 | `sub_4C7FC0` | checked | sub_4C7FC0 (759 bytes) is the arrow service body; not split out (big open-coded body in a matching function) |
| 0x5D0C90 | `Hud_Arrow_7C::sub_5D0C90` | 0x4C82C0 | ✓ `sub_4C6F80`, ✓ `sub_4C7050`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `angry_lewin_0x85C::get_camera_434900` | done | IsType0_4C6F80 and IsVisible_4C7050; Still matches. |
| 0x5D0E40 | `Hud_Arrow_7C_Array::IsThereAnyOtherArrowsInSameGang_5D0E40` | 0x4C8470 | ✓ `sub_4C6F80`, ✓ `sub_4C7050` | done | all 9.6f inlines used |
| 0x5D0E90 | `Hud_Arrow_7C_Array::sub_5D0E90` | 0x4C84C0 | ✓ `sub_434B10`, ✓ `sub_4C6F30`, ✓ `sub_41D020` | done | New ArrowTrace_24::GetTargetPlayer_4C6F30; Still matches. |
| 0x5D0EF0 | `Hud_Arrow_7C_Array::sub_5D0EF0` | 0x4C8540 | ✓ `sub_4C6F80` | done | Hud_Arrow_7C::IsType0_4C6F80; Still matches. |
| 0x5D0F40 | `Hud_Arrow_7C_Array::IsThereAnyMissionPhoneArrowForGang_5D0F40` | 0x4C8590 | ✓ `sub_4C6F80` | done | Hud_Arrow_7C::IsType0_4C6F80; Still matches. |
| 0x5D0F80 | `Hud_Arrow_7C_Array::sub_5D0F80` | 0x4C85D0 | ✓ `sub_4C6F80`, `sub_476880` | done | Hud_Arrow_7C::IsType0_4C6F80; Still matches. |
| 0x5D0FD0 | `Hud_Arrow_7C_Array::UpdateArrows_5D0FD0` | 0x4CA890 | ✓ `sub_4C6F80` | done | Hud_Arrow_7C::IsType0_4C6F80; Still matches. |
| 0x5D1050 | `Hud_Arrow_7C_Array::AllocArrow_5D1050` | 0x4CA8E0 | ✓ `sub_4CA610`, ✓ `sub_4C6FF0` | done | New Hud_Arrow_7C::Reset_4CA610 and SetMinRadiusPos_4C6FF0 (defined in Hud.cpp); Still matches. |
| 0x5D13C0 | `Garox_12EC_sub::IsOnQuitMessage_5D13C0` | 0x4C8690 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_434B10`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_461DD0` | done | New youthful_einstein::SetQuit_461DD0; Still matches. |
| 0x5D16B0 | `Garox_2A25_sub::DrawChatMessages_5D16B0` | 0x4C8910 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700), ✓ `rng::get_cur_rng_41CFE0` | matched | GetLineSpacingFromFontType_5D7700_inlined (9.6f 0x4539B0), s32 positions so both go through Fix16(s32) |
| 0x5D1EB0 | `Garox_1700_L::sub_5D1EB0` | 0x4C8BE0 | ✓ `Garox_C4::sub_4C70F0` | done | New Garox_C4::Expire_4C70F0; Still matches. |
| 0x5D3040 | `Hud_Pager_C_Array::DrawPagers_5D3040` | 0x4C92A0 | ✓ `sub_4C7250`, ✓ `sub_4C7220` | done | all 9.6f inlines used |
| 0x5D31F0 | `Hud_Pager_C::CreateTimer_5D31F0` | 0x4C9310 | ✓ `sub_4C7170`, ✓ `sub_4C7160`, ✓ `sub_4C7120` | done | New Hud_Pager_C::IsTimerOff_4C7170 and SetTimer_4C7120, plus no_ptr_counter_4C7160; Still matches. |
| 0x5D3220 | `Hud_Pager_C_Array::AddOnScreenCounter_5D3220` | 0x4C9360 | ✓ `sub_4C7160`, ✓ `sub_4C7170`, ✓ `sub_4C7130` | done | Hud_Pager_C::SetCounter_4C7130 (defined in Hud.cpp), IsTimerOff_4C7170, no_ptr_counter_4C7160; Still matches. |
| 0x5D5770 | `Garox_1_v2::AnnounceKill_5D5770` | 0x4C9750 | ✓ `angry_lewin_0x85C::sub_41DC70`, `sub_4105B0` | checked | sub_4105B0 is a root_sound thunk (PlayVoice_40F090, called) |
| 0x5D5900 | `Hud_MapZone_98::DrawZoneName_5D5900` | 0x4C9890 | ✓ `sub_4C7220` | done | get_sprite_width_4C7220; still matches |
| 0x5D5B60 | `Hud_MapZone_98::sub_5D5B60` | 0x4C6B70 | `sub_4A6530` | checked | sub_4A6530 (171 bytes) open-coded; not split out |
| 0x5D5C80 | `Garox_1118_sub::DrawPlayerStats_5D5C80` | 0x4C9C20 | ✓ `sub_4A4FB0`, ✓ `sub_434B20`, ✓ `sub_4C7380`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | New youthful_einstein::GetPlayerTime_4C7380 (defined in Hud.cpp); Still matches. |
| 0x5D6060 | `sub_5D6060` | 0x4C9A40 | ✓ `sub_4C7220`, ✓ `sub_4C7250` | done | get_sprite_width_4C7220/get_sprite_height_4C7250; still matches |
| 0x5D6860 | `Hud_2B00::DrawGui_5D6860` | 0x4CA440 | `sub_4C78A0`, `sub_4C7A30` | checked | sub_4C78A0/sub_4C7A30 (304/218 bytes) open-coded in DrawGui; not split out |
| 0x5D69D0 | `Hud_2B00::UpdateHUD_5D69D0` | 0x4CAB50 | `sub_4C73A0`, `sub_4C62B0` | checked | sub_4C73A0 (246 bytes) and sub_4C62B0 (pager loop) open-coded in UpdateHUD; not split out |
| 0x5D6B00 | `Hud_2B00::sub_5D6B00` | 0x4CA520 | ✓ `LangIsJapanese_452E60` | done | all 9.6f inlines used |
| 0x5D6C20 | `Hud_2B00::IsBusy_5D6C20` | 0x4CA5D0 | `sub_4C8880` | checked | sub_4C8880 (142 bytes) open-coded in IsBusy; not split out |
| 0x5D6CB0 | `Hud_2B00::sub_5D6CB0` | 0x4CA650 | `sub_4C7CC0` (10.5 0x5D03C0) | checked | 9.6f sub_4C7CC0 (10.5 copy 0x5D03C0) vs 10.5 call to IsQuitMessageKey_5D15A0: pairing difference |
| 0x5D6CD0 | `Hud_2B00::ctor_5D6CD0` | 0x4CAC60 | `Garox_C_Array::ctor_4CA660`, `Garox_7C_Array::ctor_4C7080`, `Garox_Sub_C_Array::ctor_4C6EE0`, `Garox_27B5_sub::ctor_4C6E70`, `Garox_110C_sub::ctor_4C6E50`, `Garox_12E4_sub::ctor_4C71A0` | matched | member array ctors (4CA660, 4C7080, ...) are already inline ctors in Hud.hpp |
| 0x5D8940 | `CountLineSpacing_5D8940` | 0x4CC0C0 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700) | done | all 9.6f inlines used |
| 0x5D8E70 | `UpdateWinXY_5D8E70` | 0x4CC580 | `sub_4CB520` | checked | sub_4CB520 is IsFullScreen_5D9280 (called) |
| 0x5DCD50 | `Weapon_30::dtor_5DCD50` | 0x4CCB10 | ✓ `root_sound::DestroySoundObj_40FE60` | done | all 9.6f inlines used |
| 0x5DCE40 | `Weapon_30::add_ammo_capped_5DCE40` | 0x4CCB70 | ✓ `Weapon_30::sub_4A4FA0` | done | all 9.6f inlines used |
| 0x5DD0F0 | `Weapon_30::flamethrower_5DD0F0` | 0x4CDC20 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::sub_416B50`, ✓ `sub_41E210`, ✓ `Weapon_30::sub_4CCA80`, `sub_48D4E0`, ✓ `sub_4CCA60`, ✓ `sub_41B0A0` | done | diff 63->0 CANDIDATE MATCH |
| 0x5E3C10 | `Weapon_8::allocate_5E3C10` | 0x4CD770 | ✓ `sub_4CC9E0`, ✓ `sub_4CCA00`, ✓ `sub_4CCA10` | done | code unchanged; new Weapon_30_Pool::AllocateUnlinked_4CC9E0 |
| 0x5E3CE0 | `Weapon_8::allocate_5E3CE0` | 0x4CD7B0 | ✓ `sub_4CC9C0`, ✓ `sub_4CCA00`, ✓ `sub_4CCA20` | checked | already uses Weapon_30_Pool::Allocate (0x4CC9C0, address noted in Weapon_8.hpp) |
| 0x5E3D20 | `Weapon_8::find_5E3D20` | 0x4CD7F0 | ✓ `Weapon_30_Pool::sub_4CC9B0` | done | code unchanged |
| 0x5E3E90 | `Weapon_8::ctor_5E3E90` | 0x4D0740 | `struct_4::ctor_424620`, `Weapon_30_Pool::ctor_4CDA20`, ✓ `struct_4::sub_4207E0` | checked | struct_4 ctor and the pool ctor are the member init / new expression; matching as is |
| 0x5E3F60 | `Weapon_8::dtor_5E3F60` | 0x4D0800 | `Weapon_30_Pool::gdtor_4D07E0` | checked | Weapon_30_Pool dtor is the delete expression |
| 0x5E53F0 | `WinMain_5E53F0` | 0x4D1170 | `sub_45E8D0`, `unknown_libname_18` (10.5 0x40EF10), `Game_0x40::sub_45ACF0`, ✓ `Game_0x40::get_main_state_4D09C0` | checked | sub_45E8D0 (837 bytes) / sub_45ACF0 (returns 1) pair with 10.5 code not open-coded here |

<!-- table inlines -->
| 9.6f | 9.6f name | Size | 10.5 copy | Noted in Source | WIP/MATCH users | Status | Notes |
|---|---|---|---|---|---|---|---|
| 0x420600 | `Car_3C::set_xyz_lazy_420600` | 89 | 0x59FA40 MATCH | Source/sprite.hpp:507, Source/sprite.hpp:519 | 21/9 | todo |  |
| 0x403A00 | `cool_nash_0x294::get_cam_x_403A00` | 15 |  | Source/Ped.hpp:499 | 19/32 | todo |  |
| 0x40F6B0 | `sub_40F6B0` | 166 |  | Source/Fix16_Point.hpp:83 | 18/3 | todo |  |
| 0x41FC20 | `PolarToCartesian_41FC20` | 79 |  | Source/ang16.hpp:225 | 18/2 | todo |  |
| 0x42A6B0 | `MaxAbsDistance_42A6B0` | 103 |  | Source/Object_3C.cpp:427, Source/fix16.hpp:396 | 17/9 | todo |  |
| 0x4206C0 | `sub_4206C0` | 23 |  | Source/sprite.hpp:459 | 15/8 | todo |  |
| 0x403A10 | `cool_nash_0x294::get_cam_y_403A10` | 15 | 0x4086A0 MATCH | Source/Car_BC.cpp:7292, Source/Ped.hpp:505 | 14/32 | todo |  |
| 0x416B40 | `sub_416B40` | 4 |  | Source/sprite.hpp:305 | 14/12 | todo |  |
| 0x416B50 | `cool_nash_0x294::sub_416B50` | 15 |  | Source/Ped.hpp:511 | 14/22 | todo |  |
| 0x420690 | `Car_3C::set_ang_lazy_420690` | 36 |  | Source/sprite.hpp:497 | 14/6 | todo |  |
| 0x4215B0 | `sub_4215B0` | 11 |  | Source/Car_BC.hpp:1232 | 14/13 | todo |  |
| 0x466CF0 | `sub_466CF0` | 53 |  | Source/map_0x370.hpp:659 | 13/5 | todo |  |
| 0x40F600 | `sub_40F600` | 60 |  |  | 12/5 | todo |  |
| 0x4206F0 | `Car_3C::SetType_4206F0` | 15 |  | Source/sprite.hpp:443 | 12/5 | todo |  |
| 0x432860 | `sub_432860` | 20 |  | Source/Garage_48.cpp:97 | 12/4 | todo |  |
| 0x40E8D0 | `sub_40E8D0` | 406 |  |  | 11/4 | todo |  |
| 0x40F540 | `sub_40F540` | 59 |  | Source/ang16.hpp:247, Source/ang16.hpp:252 | 11/2 | todo |  |
| 0x40F890 | `IsMaxDamage_40F890` | 10 |  | Source/Car_BC.hpp:771 | 11/16 | todo |  |
| 0x41B0A0 | `sub_41B0A0` | 12 |  | Source/Ped.hpp:571 | 11/18 | todo |  |
| 0x4337F0 | `sub_4337F0` | 5 |  | Source/sprite.hpp:473 | 11/4 | todo |  |
| 0x420420 | `sub_420420` | 38 |  | Source/map_0x370.hpp:633 | 10/4 | todo |  |
| 0x4207B0 | `sub_4207B0` | 26 |  | Source/sprite.cpp:212 | 10/6 | todo |  |
| 0x403970 | `cool_nash_0x294::set_occupation_403970` | 13 |  | Source/Ped.hpp:626 | 9/21 | todo |  |
| 0x403920 | `cool_nash_0x294::sub_403920` | 13 |  | Source/Ped.hpp:636 | 8/25 | todo |  |
| 0x403BA0 | `Car_BC::sub_403BA0` | 32 |  | Source/Car_BC.hpp:752, Source/Car_BC.hpp:1251 | 8/17 | todo |  |
| 0x421560 | `Car_BC::sub_421560` | 16 |  | Source/Car_BC.hpp:758 | 8/18 | todo |  |
| 0x421EC0 | `Car_BC::sub_421EC0` | 38 |  |  | 8/7 | todo |  |
| 0x40F790 | `sub_40F790` | 22 |  | Source/CarPhysics_B0.hpp:105, Source/CarPhysics_B0.hpp:222 | 7/0 | todo |  |
| 0x40FEB0 | `sub_40FEB0` | 13 |  | Source/sprite.hpp:310 | 7/28 | todo |  |
| 0x421380 | `Car_10::set_obj_421380` | 10 |  | Source/Car_10.hpp:37 | 7/2 | todo |  |
| 0x433DD0 | `cool_nash_0x294::sub_433DD0` | 22 |  | Source/Ped.hpp:593, Source/Ped.hpp:683 | 7/5 | todo |  |
| 0x4BD670 | `sub_4BD670` | 545 |  |  | 7/3 | todo |  |
| 0x403980 | `cool_nash_0x294::get_occupation_403980` | 7 |  | Source/Car_BC.hpp:1181, Source/Ped.hpp:631 | 6/17 | todo |  |
| 0x403AA0 | `cool_nash_0x294::set_target_objective_car_403AA0` | 13 |  | Source/Ped.hpp:483 | 6/11 | todo |  |
| 0x40FEA0 | `sub_40FEA0` | 13 |  | Source/Rozza_C88.hpp:68, Source/sprite.hpp:323 | 6/17 | todo |  |
| 0x41E210 | `sub_41E210` | 76 |  | Source/Fix16_Point.hpp:76, Source/Fix16_Point.hpp:94 | 6/3 | todo |  |
| 0x420390 | `sub_420390` | 135 |  |  | 6/3 | todo |  |
| 0x421D90 | `Car_BC::sub_421D90` | 86 |  |  | 6/6 | todo |  |
| 0x42A630 | `sub_42A630` | 33 |  | Source/fix16.hpp:281 | 6/8 | todo |  |
| 0x42AB90 | `sub_42AB90` | 8 |  | Source/CarPhysics_B0.hpp:418 | 6/0 | todo |  |
| 0x4337D0 | `sub_4337D0` | 19 |  | Source/sprite.hpp:491 | 6/3 | todo |  |
| 0x433920 | `Char_B4::sub_433920` | 10 |  | Source/char.hpp:213, Source/char.hpp:219 | 6/14 | todo |  |
| 0x403990 | `cool_nash_0x294::sub_403990` | 7 |  | Source/Ped.hpp:751 | 5/20 | todo |  |
| 0x403A80 | `cool_nash_0x294::get_objective_403A80` | 7 |  | Source/Ped.hpp:786, Source/Player.hpp:97 | 5/8 | todo |  |
| 0x403A90 | `cool_nash_0x294::get_car_state_403A90` | 7 |  | Source/Ped.hpp:833 | 5/5 | todo |  |
| 0x403AE0 | `cool_nash_0x294::sub_403AE0` | 13 |  | Source/Ped.hpp:461 | 5/7 | todo |  |
| 0x40EAB0 | `sub_40EAB0` | 81 |  |  | 5/3 | todo |  |
| 0x41CFE0 | `rng::get_cur_rng_41CFE0` | 3 |  | Source/rng.hpp:14 | 5/19 | todo |  |
| 0x421470 | `sub_421470` | 32 |  | Source/Car_BC.hpp:1091 | 5/9 | todo |  |
| 0x421510 | `sub_421510` | 26 |  | Source/PublicTransport.cpp:109 | 5/4 | todo |  |
| 0x42ABA0 | `sub_42ABA0` | 8 |  | Source/CarPhysics_B0.hpp:412 | 5/0 | todo |  |
| 0x433A80 | `Char_B4::sub_433A80` | 4 |  | Source/char.hpp:255 | 5/11 | todo |  |
| 0x433B40 | `cool_nash_0x294::sub_433B40` | 10 |  | Source/Ped.hpp:746 | 5/16 | todo |  |
| 0x49E5A0 | `sub_49E5A0` | 161 |  |  | 5/0 | todo |  |
| 0x4CCA80 | `Weapon_30::sub_4CCA80` | 10 |  | Source/Weapon_30.hpp:128 | 5/1 | todo |  |
| 0x403A40 | `cool_nash_0x294::sub_403A40` | 16 |  | Source/Ped.hpp:802 | 4/9 | todo |  |
| 0x403B60 | `cool_nash_0x294::sub_403B60` | 11 |  | Source/Ped.hpp:818 | 4/14 | todo |  |
| 0x40F580 | `sub_40F580` | 36 |  | Source/ang16.hpp:237 | 4/9 | todo |  |
| 0x40FEC0 | `sub_40FEC0` | 21 |  | Source/sprite.hpp:355 | 4/17 | todo |  |
| 0x4118B0 | `sub_4118B0` | 4 |  | Source/Car_BC.hpp:701 | 4/3 | todo |  |
| 0x4118D0 | `sub_4118D0` | 23 |  | Source/Car_BC.hpp:684 | 4/5 | todo |  |
| 0x416B60 | `cool_nash_0x294::get_car_416B60` | 7 |  | Source/Ped.hpp:545 | 4/20 | todo |  |
| 0x41E450 | `Car_BC::sub_41E450` | 15 |  | Source/Car_BC.hpp:1211 | 4/9 | todo |  |
| 0x420EA0 | `Char_8::sub_420EA0` | 7 |  | Source/Char_Pool.hpp:33, Source/Ped_List_4.hpp:46 | 4/1 | todo |  |
| 0x421000 | `Sprite_Pool::sub_421000` | 19 |  | Source/sprite.hpp:754 | 4/7 | todo |  |
| 0x4221A0 | `sub_4221A0` | 8 |  |  | 4/1 | todo |  |
| 0x424620 | `struct_4::ctor_424620` | 12 |  |  | 4/6 | todo |  |
| 0x426E00 | `sub_426E00` | 15 |  | Source/Car_BC.hpp:1254 | 4/1 | todo |  |
| 0x426E40 | `sub_426E40` | 146 |  | Source/Car_BC.hpp:251 | 4/1 | todo |  |
| 0x426F00 | `sub_426F00` | 60 |  |  | 4/0 | todo |  |
| 0x42A720 | `sub_42A720` | 239 |  | Source/ang16.hpp:345 | 4/0 | todo |  |
| 0x433900 | `sub_433900` | 13 |  | Source/char.hpp:277 | 4/3 | todo |  |
| 0x433970 | `sub_433970` | 68 |  | Source/char.hpp:188, Source/char.hpp:201 | 4/5 | todo |  |
| 0x450CB0 | `sub_450CB0` | 7 |  | Source/Ped.hpp:411 | 4/5 | todo |  |
| 0x453A60 | `sub_453A60` | 26 |  | Source/lucid_hamilton.hpp:46 | 4/7 | todo |  |
| 0x482C80 | `sub_482C80` | 15 |  | Source/Object_5C.hpp:282 | 4/0 | todo |  |
| 0x482C90 | `sub_482C90` | 42 |  | Source/Object_5C.hpp:152 | 4/0 | todo |  |
| 0x4A65E0 | `sub_4A65E0` | 35 |  |  | 4/1 | todo |  |
| 0x4BDEF0 | `sub_4BDEF0` | 43 |  |  | 4/2 | todo |  |
| 0x4CD000 | `sub_4CD000` | 32 |  |  | 4/0 | todo |  |
| 0x403960 | `sub_403960` | 15 |  |  | 3/4 | todo |  |
| 0x403A70 | `cool_nash_0x294::set_target_car_door_403A70` | 13 |  | Source/Ped.hpp:71 | 3/6 | todo |  |
| 0x403AC0 | `cool_nash_0x294::set_objective_target_ped_403AC0` | 13 |  | Source/Ped.hpp:77 | 3/5 | todo |  |
| 0x409C40 | `sub_409C40` | 7 |  | Source/NetPlay.hpp:251 | 3/6 | todo |  |
| 0x40F680 | `sub_40F680` | 33 |  | Source/CarPhysics_B0.cpp:2876 | 3/6 | todo |  |
| 0x40F7B0 | `sub_40F7B0` | 10 |  | Source/Object_5C.hpp:290, Source/Object_5C.hpp:305 | 3/9 | todo |  |
| 0x411940 | `sub_411940` | 7 |  | Source/Car_BC.hpp:797 | 3/13 | todo |  |
| 0x41CFF0 | `angle::angle_not_equal_41CFF0` | 20 |  |  | 3/0 | todo |  |
| 0x41DC40 | `sub_41DC40` | 35 |  | Source/Player.hpp:131 | 3/13 | todo |  |
| 0x41DC70 | `angry_lewin_0x85C::sub_41DC70` | 3 |  | Source/Player.hpp:296 | 3/23 | todo |  |
| 0x41E430 | `Car_BC::sub_41E430` | 15 |  | Source/Car_BC.hpp:1201 | 3/9 | todo |  |
| 0x41E440 | `Car_BC::sub_41E440` | 15 |  | Source/Car_BC.hpp:1206 | 3/9 | todo |  |
| 0x420360 | `sub_420360` | 45 |  | Source/Fix16_Point.hpp:24 | 3/2 | todo |  |
| 0x421210 | `sub_421210` | 28 |  | Source/CarPhysics_B0.hpp:376 | 3/0 | todo |  |
| 0x4214D0 | `sub_4214D0` | 11 |  | Source/Car_BC.hpp:1099 | 3/2 | todo |  |
| 0x421D80 | `sub_421D80` | 8 |  |  | 3/2 | todo |  |
| 0x4292F0 | `sub_4292F0` | 118 |  |  | 3/1 | todo |  |
| 0x42ABB0 | `sub_42ABB0` | 16 |  | Source/CarPhysics_B0.hpp:424 | 3/0 | todo |  |
| 0x433470 | `sub_433470` | 46 |  | Source/map_0x370.hpp:622 | 3/0 | todo |  |
| 0x4334A0 | `sub_4334A0` | 46 |  | Source/map_0x370.hpp:622 | 3/0 | todo |  |
| 0x4334D0 | `sub_4334D0` | 46 |  | Source/map_0x370.hpp:622 | 3/0 | todo |  |
| 0x433500 | `sub_433500` | 46 |  | Source/map_0x370.hpp:622 | 3/0 | todo |  |
| 0x433910 | `sub_433910` | 10 |  | Source/char.hpp:155, Source/char.hpp:229 | 3/2 | todo |  |
| 0x433A30 | `sub_433A30` | 12 |  | Source/Ped.hpp:737, Source/char.hpp:165 | 3/0 | todo |  |
| 0x433A50 | `sub_433A50` | 10 |  | Source/char.hpp:234 | 3/2 | todo |  |
| 0x433B50 | `cool_nash_0x294::sub_433B50` | 11 |  | Source/Ped.hpp:561 | 3/6 | todo |  |
| 0x433BA0 | `cool_nash_0x294::get_remap_433BA0` | 7 |  | Source/Ped.hpp:566 | 3/6 | todo |  |
| 0x433BF0 | `sub_433BF0` | 13 |  | Source/Ped.hpp:843 | 3/0 | todo |  |
| 0x433C10 | `sub_433C10` | 11 |  | Source/Ped.hpp:669 | 3/7 | todo |  |
| 0x4340F0 | `sub_4340F0` | 15 |  | Source/Object_5C.hpp:250 | 3/2 | todo |  |
| 0x435610 | `sub_435610` | 76 |  |  | 3/4 | todo |  |
| 0x436140 | `sub_436140` | 23 |  | Source/Camera.hpp:63 | 3/0 | todo |  |
| 0x436200 | `ApplyCarVelocityCameraOffset_436200` | 23 |  | Source/Camera.hpp:65 | 3/0 | todo |  |
| 0x4637B0 | `sub_4637B0` | 11 |  | Source/Rozza_C88.hpp:16 | 3/1 | todo |  |
| 0x46BB50 | `sharp_pare_0x15D8::sub_46BB50` | 15 |  | Source/sharp_pare_0x15D8.hpp:83 | 3/0 | todo |  |
| 0x482790 | `sub_482790` | 22 |  |  | 3/6 | todo |  |
| 0x482A30 | `sub_482A30` | 10 |  | Source/sprite.hpp:449 | 3/1 | todo |  |
| 0x482C30 | `sub_482C30` | 20 |  |  | 3/0 | todo |  |
| 0x483100 | `sub_483100` | 254 |  |  | 3/1 | todo |  |
| 0x48A8F0 | `sub_48A8F0` | 7 |  | Source/Pool.hpp:59 | 3/2 | todo |  |
| 0x48A900 | `sub_48A900` | 28 |  |  | 3/2 | todo |  |
| 0x491F80 | `sub_491F80` | 20 |  | Source/gtx_0x106C.hpp:367 | 3/0 | todo |  |
| 0x49E360 | `sub_49E360` | 57 |  |  | 3/0 | todo |  |
| 0x4CCA30 | `Weapon_30::sub_4CCA30` | 33 |  | Source/Weapon_30.hpp:106 | 3/0 | todo |  |
| 0x4CCA90 | `sub_4CCA90` | 17 |  | Source/Ped.hpp:813 | 3/0 | todo |  |
| 0x403950 | `sub_403950` | 15 |  | Source/Ped.hpp:467 | 2/2 | todo |  |
| 0x4039A0 | `cool_nash_0x294::set_health_4039A0` | 15 |  | Source/Ped.hpp:614 | 2/20 | todo |  |
| 0x403A30 | `cool_nash_0x294::sub_403A30` | 15 |  | Source/Ped.hpp:367 | 2/11 | todo |  |
| 0x403AD0 | `cool_nash_0x294::get_objective_target_ped_403AD0` | 7 |  | Source/Ped.hpp:83 | 2/1 | todo |  |
| 0x403B10 | `cool_nash_0x294::get_target_to_enter_403B10` | 7 |  | Source/Ped.hpp:791 | 2/5 | todo |  |
| 0x404900 | `sub_404900` | 226 |  |  | 2/0 | todo |  |
| 0x40F640 | `sub_40F640` | 53 | 0x43D5D0 MATCH |  | 2/2 | todo |  |
| 0x40F820 | `sub_40F820` | 14 |  | Source/CarPhysics_B0.hpp:76 | 2/0 | todo |  |
| 0x40F840 | `sub_40F840` | 80 |  | Source/CarPhysics_B0.hpp:219 | 2/2 | todo |  |
| 0x410BF0 | `sub_410BF0` | 17 |  | Source/fix16.hpp:23 | 2/2 | todo |  |
| 0x411730 | `sub_411730` | 118 |  |  | 2/3 | todo |  |
| 0x414F20 | `sub_414F20` | 49 |  | Source/Car_BC.hpp:778, Source/Car_BC.hpp:803 | 2/3 | todo |  |
| 0x41B080 | `sub_41B080` | 12 | 0x41B480 unmarked | Source/char.hpp:30 | 2/2 | todo |  |
| 0x41CC70 | `sub_41CC70` | 4 |  | Source/Weapon_30.hpp:161 | 2/0 | todo |  |
| 0x41CC90 | `sub_41CC90` | 4 |  | Source/Weapon_30.hpp:151 | 2/0 | todo |  |
| 0x41FC90 | `RotateVector_41FC90` | 177 |  | Source/ang16.hpp:318 | 2/0 | todo |  |
| 0x41FE40 | `sub_41FE40` | 35 |  |  | 2/1 | todo |  |
| 0x420220 | `sub_420220` | 16 |  | Source/Hud.hpp:348, Source/gtx_0x106C.hpp:237 | 2/0 | todo |  |
| 0x420660 | `sub_420660` | 41 |  | Source/sprite.hpp:543 | 2/3 | todo |  |
| 0x420700 | `sub_420700` | 8 |  | Source/sprite.hpp:299 | 2/1 | todo |  |
| 0x420B50 | `sub_420B50` | 7 |  | Source/Ped.hpp:362 | 2/3 | todo |  |
| 0x420F10 | `sub_420F10` | 15 |  | Source/Varrok_7F8.hpp:31 | 2/9 | todo |  |
| 0x420F30 | `sub_420F30` | 69 |  | Source/Pool.hpp:293 | 2/2 | todo |  |
| 0x421050 | `sub_421050` | 4 |  |  | 2/3 | todo |  |
| 0x421060 | `check_is_shop_421060` | 32 |  | Source/Object_5C.hpp:219 | 2/4 | todo |  |
| 0x4211A0 | `sub_4211A0` | 20 |  | Source/CarPhysics_B0.hpp:310 | 2/3 | todo |  |
| 0x421490 | `sub_421490` | 20 |  | Source/Car_BC.hpp:914, Source/Car_BC.hpp:1082 | 2/1 | todo |  |
| 0x4214B0 | `sub_4214B0` | 27 |  | Source/Car_BC.hpp:1026 | 2/2 | todo |  |
| 0x4219D0 | `angry_lewin_0x85C::sub_4219D0` | 4 |  | Source/Player.hpp:313 | 2/17 | todo |  |
| 0x424220 | `sub_424220` | 88 |  | Source/Car_BC.cpp:5682 | 2/0 | todo |  |
| 0x42A620 | `sub_42A620` | 15 |  |  | 2/0 | todo |  |
| 0x42A660 | `sub_42A660` | 56 |  |  | 2/0 | todo |  |
| 0x42AC00 | `sub_42AC00` | 27 | 0x453F90 MATCH | Source/CarPhysics_B0.hpp:394 | 2/1 | todo |  |
| 0x433580 | `cool_nash_0x294::sub_433580` | 169 |  |  | 2/1 | todo |  |
| 0x433810 | `sub_433810` | 10 |  | Source/Weapon_30.hpp:156 | 2/5 | todo |  |
| 0x433A20 | `sub_433A20` | 4 |  | Source/char.hpp:224 | 2/2 | todo |  |
| 0x433B60 | `sub_433B60` | 7 |  | Source/Ped.hpp:756 | 2/3 | todo |  |
| 0x433B70 | `sub_433B70` | 8 |  | Source/Ped.hpp:620 | 2/5 | todo |  |
| 0x433B90 | `sub_433B90` | 13 |  | Source/Ped.hpp:608 | 2/15 | todo |  |
| 0x433C00 | `sub_433C00` | 11 |  | Source/Ped.hpp:734 | 2/4 | todo |  |
| 0x433C20 | `sub_433C20` | 23 |  | Source/Ped.hpp:56, Source/Ped.hpp:740 | 2/1 | todo |  |
| 0x4340D0 | `sub_4340D0` | 15 |  | Source/Object_5C.hpp:240 | 2/3 | todo |  |
| 0x4340E0 | `sub_4340E0` | 15 |  | Source/Object_5C.hpp:245 | 2/3 | todo |  |
| 0x434130 | `sub_434130` | 15 |  | Source/Object_5C.hpp:194 | 2/0 | todo |  |
| 0x434B20 | `sub_434B20` | 16 |  | Source/lucid_hamilton.hpp:196, Source/youthful_einstein.hpp:18 | 2/1 | todo |  |
| 0x435C20 | `sub_435C20` | 30 | 0x408680 MATCH |  | 2/0 | todo |  |
| 0x44B490 | `sub_44B490` | 97 |  |  | 2/0 | todo |  |
| 0x453A40 | `sub_453A40` | 21 |  | Source/lucid_hamilton.hpp:38 | 2/2 | todo |  |
| 0x453A80 | `lucid_hamilton::sub_453A80` | 7 |  | Source/lucid_hamilton.hpp:20 | 2/1 | todo |  |
| 0x45ADA0 | `sub_45ADA0` | 12 |  | Source/Camera.hpp:126, Source/Fix16_Rect.hpp:143 | 2/1 | todo |  |
| 0x45ADB0 | `sub_45ADB0` | 11 |  | Source/Camera.hpp:125, Source/Fix16_Rect.hpp:138 | 2/1 | todo |  |
| 0x45ADC0 | `sub_45ADC0` | 12 |  | Source/Camera.hpp:124, Source/Fix16_Rect.hpp:148 | 2/1 | todo |  |
| 0x45ADD0 | `sub_45ADD0` | 12 |  | Source/Camera.hpp:123, Source/Fix16_Rect.hpp:133 | 2/1 | todo |  |
| 0x462E40 | `Map_0x370::sub_462E40` | 22 |  |  | 2/7 | todo |  |
| 0x475C30 | `sub_475C30` | 11 |  | Source/Car_BC.hpp:397, Source/Car_BC.hpp:908 | 2/2 | todo |  |
| 0x4828C0 | `sub_4828C0` | 33 |  | Source/Fix16_Point.hpp:114 | 2/2 | todo |  |
| 0x482BE0 | `sub_482BE0` | 16 |  | Source/Object_5C.hpp:288 | 2/0 | todo |  |
| 0x482F80 | `sub_482F80` | 30 |  |  | 2/1 | todo |  |
| 0x483500 | `sub_483500` | 108 |  |  | 2/1 | todo |  |
| 0x483C20 | `sub_483C20` | 28 |  |  | 2/0 | todo |  |
| 0x483FC0 | `sub_483FC0` | 26 |  |  | 2/0 | todo |  |
| 0x484000 | `sub_484000` | 27 |  |  | 2/0 | todo |  |
| 0x4867E0 | `sub_4867E0` | 366 |  | Source/Object_5C.cpp:476 | 2/0 | todo |  |
| 0x492CC0 | `sub_492CC0` | 17 |  | Source/Ped.hpp:693 | 2/0 | todo |  |
| 0x493940 | `sub_493940` | 1706 |  |  | 2/1 | todo |  |
| 0x49A080 | `sub_49A080` | 1244 |  |  | 2/0 | todo |  |
| 0x49E3A0 | `sub_49E3A0` | 30 |  | Source/Fix16_Point.hpp:123 | 2/0 | todo |  |
| 0x4A09B0 | `sub_4A09B0` | 116 |  |  | 2/0 | todo |  |
| 0x4A0D40 | `sub_4A0D40` | 482 |  | Source/CarPhysics_B0.cpp:2960 | 2/0 | todo |  |
| 0x4A0F30 | `sub_4A0F30` | 509 |  | Source/CarPhysics_B0.cpp:3020 | 2/0 | todo |  |
| 0x4A9AD0 | `sub_4A9AD0` | 5 |  | Source/Car_BC.hpp:1227 | 2/2 | todo |  |
| 0x4BA0A0 | `sub_4BA0A0` | 80 |  | Source/sprite.hpp:144 | 2/0 | todo |  |
| 0x4BEE10 | `sub_4BEE10` | 31 |  |  | 2/0 | todo |  |
| 0x4CCA60 | `sub_4CCA60` | 17 |  | Source/Weapon_30.hpp:120 | 2/1 | todo |  |
| 0x403890 | `PedPool::sub_403890` | 34 |  | Source/Char_Pool.hpp:120 | 1/10 | todo |  |
| 0x4039B0 | `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | 13 |  | Source/Ped.hpp:38 | 1/7 | todo |  |
| 0x4039D0 | `cool_nash_0x294::sub_4039D0` | 7 |  | Source/Ped.hpp:50 | 1/1 | todo |  |
| 0x4039E0 | `cool_nash_0x294::sub_4039E0` | 7 |  |  | 1/2 | todo |  |
| 0x4039F0 | `cool_nash_0x294::sub_4039F0` | 8 |  | Source/Ped.hpp:823 | 1/6 | todo |  |
| 0x403A60 | `cool_nash_0x294::get_target_car_door_403A60` | 7 |  | Source/Ped.hpp:603 | 1/5 | todo |  |
| 0x403AF0 | `cool_nash_0x294::sub_403AF0` | 7 |  | Source/Ped.hpp:838 | 1/2 | todo |  |
| 0x403B00 | `cool_nash_0x294::set_target_to_enter_403B00` | 13 |  | Source/Ped.hpp:89 | 1/5 | todo |  |
| 0x403B30 | `cool_nash_0x294::get_objective_timer_403B30` | 8 |  | Source/Ped.hpp:101 | 1/1 | todo |  |
| 0x403B40 | `cool_nash_0x294::sub_403B40` | 13 |  | Source/Ped.hpp:107 | 1/1 | todo |  |
| 0x403B50 | `cool_nash_0x294::sub_403B50` | 13 |  | Source/Ped.hpp:113 | 1/1 | todo |  |
| 0x403B80 | `cool_nash_0x294::has_car_403B80` | 12 |  | Source/Ped.hpp:535 | 1/14 | todo |  |
| 0x403B90 | `sub_403B90` | 13 | 0x4CA3E0 MATCH |  | 1/0 | todo |  |
| 0x403D10 | `Mouze_44::sub_403D10` | 8 |  |  | 1/0 | todo |  |
| 0x403D20 | `Mouze_44::sub_403D20` | 36 |  |  | 1/0 | todo |  |
| 0x404450 | `sub_404450` | 46 |  |  | 1/0 | todo |  |
| 0x4048A0 | `sub_4048A0` | 83 |  |  | 1/0 | todo |  |
| 0x405A40 | `sub_405A40` | 7 |  |  | 1/1 | todo |  |
| 0x409C50 | `sub_409C50` | 37 |  | Source/NetPlay.cpp:14 | 1/2 | todo |  |
| 0x409F90 | `sub_409F90` | 73 |  |  | 1/1 | todo |  |
| 0x40C810 | `sub_40C810` | 74 |  |  | 1/1 | todo |  |
| 0x40CEE0 | `sub_40CEE0` | 59 |  | Source/CarAI_78.cpp:356, Source/RouteFinder.hpp:47 | 1/0 | todo |  |
| 0x40CF60 | `DrawUnk_0xBC::sub_40CF60` | 89 |  | Source/Camera.hpp:129 | 1/2 | todo |  |
| 0x40CFC0 | `DrawUnk_0xBC::sub_40CFC0` | 245 |  | Source/Camera.hpp:98, Source/RouteFinder.cpp:82 | 1/1 | todo |  |
| 0x40EC80 | `sub_40EC80` | 41 |  |  | 1/1 | todo |  |
| 0x40EE60 | `sub_40EE60` | 277 |  |  | 1/0 | todo |  |
| 0x40F0A0 | `sub_40F0A0` | 609 |  |  | 1/0 | todo |  |
| 0x40F7C0 | `sub_40F7C0` | 18 |  |  | 1/0 | todo |  |
| 0x40F7E0 | `sub_40F7E0` | 18 |  | Source/CarPhysics_B0.hpp:88 | 1/0 | todo |  |
| 0x40F800 | `sub_40F800` | 12 |  | Source/CarPhysics_B0.hpp:64 | 1/0 | todo |  |
| 0x40F810 | `sub_40F810` | 10 |  | Source/CarPhysics_B0.hpp:70 | 1/0 | todo |  |
| 0x40F830 | `sub_40F830` | 12 |  | Source/CarPhysics_B0.hpp:82 | 1/0 | todo |  |
| 0x40FD40 | `sub_40FD40` | 234 |  |  | 1/0 | todo |  |
| 0x40FE60 | `root_sound::DestroySoundObj_40FE60` | 27 |  | Source/root_sound.hpp:14 | 1/3 | todo |  |
| 0x40FEE0 | `sub_40FEE0` | 16 |  | Source/Rozza_C88.hpp:71 | 1/2 | todo |  |
| 0x4102A0 | `sub_4102A0` | 198 | 0x40BD10 MATCH |  | 1/0 | todo |  |
| 0x410460 | `sub_410460` | 19 |  |  | 1/0 | todo |  |
| 0x410480 | `sub_410480` | 34 |  |  | 1/0 | todo |  |
| 0x4105B0 | `sub_4105B0` | 10 |  |  | 1/9 | todo |  |
| 0x410670 | `unknown_libname_18` | 10 | 0x40EF10 unmarked |  | 1/5 | todo |  |
| 0x411810 | `CarPhysics_B0::is_backward_gas_on_411810` | 7 |  | Source/CarPhysics_B0.hpp:324 | 1/1 | todo |  |
| 0x411900 | `Car_BC::sub_411900` | 11 |  | Source/Car_BC.hpp:1120, Source/Car_BC.hpp:1287 | 1/7 | todo |  |
| 0x411930 | `Car_BC::sub_411930` | 15 |  | Source/Car_BC.hpp:678 | 1/3 | todo |  |
| 0x414F60 | `sub_414F60` | 11 |  |  | 1/0 | todo |  |
| 0x414FA0 | `sub_414FA0` | 235 |  |  | 1/0 | todo |  |
| 0x4168E0 | `sub_4168E0` | 92 |  |  | 1/0 | todo |  |
| 0x416940 | `sub_416940` | 18 |  |  | 1/1 | todo |  |
| 0x416BB0 | `sub_416BB0` | 16 |  | Source/Car_BC.hpp:1268 | 1/1 | todo |  |
| 0x416BC0 | `sub_416BC0` | 7 |  | Source/Game_0x40.hpp:45 | 1/1 | todo |  |
| 0x416E50 | `sub_416E50` | 193 |  |  | 1/0 | todo |  |
| 0x416F20 | `sub_416F20` | 262 |  |  | 1/0 | todo |  |
| 0x41B090 | `sub_41B090` | 4 |  | Source/char.hpp:48 | 1/0 | todo |  |
| 0x41B0B0 | `sub_41B0B0` | 17 |  | Source/Ped.hpp:585 | 1/0 | todo |  |
| 0x41CC80 | `sub_41CC80` | 8 |  | Source/Weapon_30.hpp:133 | 1/0 | todo |  |
| 0x41D020 | `sub_41D020` | 7 |  | Source/Player.hpp:180 | 1/7 | todo |  |
| 0x41E390 | `sub_41E390` | 40 |  | Source/sprite.hpp:54 | 1/3 | todo |  |
| 0x41E460 | `Car_BC::has_trailer_41E460` | 18 |  | Source/Car_BC.hpp:707 | 1/3 | todo |  |
| 0x41FAC0 | `sub_41FAC0` | 137 |  |  | 1/0 | todo |  |
| 0x41FE70 | `sub_41FE70` | 48 |  | Source/CarInfo_808.hpp:35 | 1/1 | todo |  |
| 0x4204D0 | `sub_4204D0` | 183 |  | Source/Fix16_Rect.hpp:34 | 1/1 | todo |  |
| 0x420590 | `sub_420590` | 11 |  | Source/sprite.hpp:95, Source/sprite.hpp:284 | 1/0 | todo |  |
| 0x4205A0 | `Car_3C::ctor_4205A0` | 85 |  |  | 1/0 | todo |  |
| 0x4207E0 | `struct_4::sub_4207E0` | 7 |  | Source/Object_3C.hpp:21, Source/Object_3C.hpp:24 | 1/2 | todo |  |
| 0x420B70 | `sub_420B70` | 7 |  | Source/Ped.hpp:373 | 1/6 | todo |  |
| 0x420E50 | `sub_420E50` | 4 |  | Source/Car_BC.hpp:1387 | 1/0 | todo |  |
| 0x420E90 | `Ped_Unknown_4::ClearList_420E90` | 7 |  | Source/Ped_List_4.hpp:15, Source/Ped_List_4.hpp:41 | 1/2 | todo |  |
| 0x420EB0 | `CarAI_78_Pool::ctor_420EB0` | 61 |  | Source/CarAI_78.hpp:135 | 1/0 | todo |  |
| 0x420F80 | `CarPhyisicsPool::ctor_420F80` | 74 |  |  | 1/0 | todo |  |
| 0x420FF0 | `sub_420FF0` | 4 |  | Source/Object_5C.hpp:188 | 1/9 | todo |  |
| 0x421080 | `sub_421080` | 47 |  | Source/Object_5C.hpp:162, Source/Object_5C.hpp:174 | 1/4 | todo |  |
| 0x421130 | `sub_421130` | 24 |  | Source/CarPhysics_B0.hpp:108 | 1/0 | todo |  |
| 0x4211C0 | `sub_4211C0` | 17 |  | Source/CarPhysics_B0.hpp:329 | 1/2 | todo |  |
| 0x421250 | `sub_421250` | 8 |  | Source/CarPhysics_B0.hpp:120 | 1/0 | todo |  |
| 0x421260 | `sub_421260` | 8 |  | Source/CarPhysics_B0.hpp:114, Source/CarPhysics_B0.hpp:318 | 1/2 | todo |  |
| 0x421360 | `sub_421360` | 18 |  | Source/Car_10.hpp:25 | 1/0 | todo |  |
| 0x421540 | `sub_421540` | 8 |  | Source/Car_BC.hpp:391 | 1/7 | todo |  |
| 0x421640 | `sub_421640` | 25 |  | Source/Car_BC.hpp:820, Source/Car_BC.hpp:1251 | 1/2 | todo |  |
| 0x4216E0 | `sub_4216E0` | 25 |  | Source/Car_BC.hpp:1274, Source/Car_BC.hpp:1287 | 1/2 | todo |  |
| 0x421720 | `is_on_trailer_421720` | 18 |  | Source/Car_BC.hpp:695 | 1/3 | todo |  |
| 0x421870 | `sub_421870` | 10 |  | Source/Car_BC.hpp:1031 | 1/3 | todo |  |
| 0x4218A0 | `sub_4218A0` | 8 |  | Source/Car_BC.hpp:923 | 1/1 | todo |  |
| 0x421C00 | `sub_421C00` | 59 |  |  | 1/0 | todo |  |
| 0x421C40 | `sub_421C40` | 104 |  | Source/Car_BC.cpp:1895 | 1/0 | todo |  |
| 0x421CB0 | `sub_421CB0` | 60 |  |  | 1/0 | todo |  |
| 0x421DF0 | `sub_421DF0` | 15 |  |  | 1/1 | todo |  |
| 0x4221B0 | `sub_4221B0` | 20 |  |  | 1/1 | todo |  |
| 0x423480 | `sub_423480` | 26 |  |  | 1/2 | todo |  |
| 0x423720 | `Car_BC::sub_423720` | 133 |  |  | 1/0 | todo |  |
| 0x423940 | `sub_423940` | 257 |  |  | 1/0 | todo |  |
| 0x423A70 | `sub_423A70` | 37 |  |  | 1/1 | todo |  |
| 0x425500 | `TrailerPool::ctor_425500` | 102 |  | Source/Car_BC.hpp:1405 | 1/0 | todo |  |
| 0x425570 | `sub_425570` | 8 |  |  | 1/0 | todo |  |
| 0x425770 | `sub_425770` | 11 |  |  | 1/0 | todo |  |
| 0x425780 | `Car_BC::sub_425780` | 11 |  |  | 1/0 | todo |  |
| 0x425D60 | `sub_425D60` | 42 |  |  | 1/0 | todo |  |
| 0x425DF0 | `Car_BC::sub_425DF0` | 52 |  |  | 1/1 | todo |  |
| 0x4260C0 | `Car_BC::sub_4260C0` | 87 |  |  | 1/0 | todo |  |
| 0x4262B0 | `sub_4262B0` | 48 |  |  | 1/0 | todo |  |
| 0x426DB0 | `Car_BC_Pool::ctor_426DB0` | 79 |  | Source/Car_BC.hpp:1365 | 1/0 | todo |  |
| 0x426E10 | `EnqueueRadioLocationPhrase_426E10` | 39 |  | Source/Car_BC.hpp:207, Source/Car_BC.hpp:214 | 1/3 | todo |  |
| 0x429250 | `Car_6C::dtor_429250` | 150 |  |  | 1/0 | todo |  |
| 0x42A5B0 | `sub_42A5B0` | 24 |  |  | 1/0 | todo |  |
| 0x42A5D0 | `sub_42A5D0` | 58 |  |  | 1/0 | todo |  |
| 0x42A810 | `sub_42A810` | 20 |  |  | 1/0 | todo |  |
| 0x42A8C0 | `sub_42A8C0` | 44 |  | Source/map_0x370.hpp:592, Source/map_0x370.hpp:612 | 1/0 | todo |  |
| 0x42ABC0 | `sub_42ABC0` | 28 | 0x453F50 MATCH | Source/CarPhysics_B0.hpp:385 | 1/0 | todo |  |
| 0x42AC20 | `sub_42AC20` | 14 |  | Source/Car_BC.hpp:427 | 1/1 | todo |  |
| 0x42AC30 | `sub_42AC30` | 14 |  | Source/Car_BC.hpp:433 | 1/1 | todo |  |
| 0x42ADA0 | `sub_42ADA0` | 1427 |  |  | 1/0 | todo |  |
| 0x42B8A0 | `sub_42B8A0` | 1844 |  |  | 1/0 | todo |  |
| 0x42C8B0 | `sub_42C8B0` | 2748 |  |  | 1/0 | todo |  |
| 0x42FDF0 | `sub_42FDF0` | 77 |  |  | 1/0 | todo |  |
| 0x42FE40 | `sub_42FE40` | 27 |  | Source/CarAI_78.cpp:1120 | 1/0 | todo |  |
| 0x430090 | `sub_430090` | 72 |  |  | 1/0 | todo |  |
| 0x431C10 | `sub_431C10` | 1855 |  |  | 1/0 | todo |  |
| 0x433370 | `Get_F3C_433370` | 4 |  | Source/PedGroup.hpp:69 | 1/1 | todo |  |
| 0x433430 | `sub_433430` | 56 |  | Source/map_0x370.hpp:644 | 1/0 | todo |  |
| 0x433530 | `sub_433530` | 44 |  | Source/map_0x370.hpp:602 | 1/4 | todo |  |
| 0x433560 | `sub_433560` | 28 |  | Source/Fix16_Rect.hpp:62 | 1/0 | todo |  |
| 0x433800 | `sub_433800` | 5 |  | Source/sprite.hpp:485 | 1/1 | todo |  |
| 0x4338F0 | `sub_4338F0` | 11 |  | Source/char.hpp:42 | 1/2 | todo |  |
| 0x433930 | `Char_B4::sub_433930` | 7 |  | Source/char.hpp:36 | 1/4 | todo |  |
| 0x433940 | `sub_433940` | 41 |  | Source/char.hpp:282 | 1/1 | todo |  |
| 0x4339C0 | `sub_4339C0` | 18 |  | Source/char.hpp:170 | 1/1 | todo |  |
| 0x4339E0 | `sub_4339E0` | 18 |  | Source/char.hpp:176 | 1/1 | todo |  |
| 0x433A00 | `sub_433A00` | 18 |  | Source/char.hpp:182 | 1/1 | todo |  |
| 0x433A60 | `sub_433A60` | 10 |  | Source/char.hpp:260 | 1/1 | todo |  |
| 0x433AA0 | `sub_433AA0` | 9 |  | Source/char.hpp:297 | 1/0 | todo |  |
| 0x433B80 | `sub_433B80` | 15 |  | Source/Ped.hpp:119 | 1/0 | todo |  |
| 0x433BC0 | `sub_433BC0` | 13 |  | Source/Ped.hpp:705 | 1/3 | todo |  |
| 0x433BD0 | `sub_433BD0` | 7 |  | Source/Ped.hpp:848 | 1/0 | todo |  |
| 0x433C40 | `sub_433C40` | 16 |  | Source/Ped.hpp:576 | 1/1 | todo |  |
| 0x433C80 | `sub_433C80` | 15 |  | Source/Ped.hpp:125 | 1/0 | todo |  |
| 0x433CA0 | `sub_433CA0` | 13 |  | Source/Ped.hpp:776 | 1/4 | todo |  |
| 0x433DA0 | `cool_nash_0x294::sub_433DA0` | 28 |  | Source/Ped.hpp:761 | 1/3 | todo |  |
| 0x433DF0 | `cool_nash_0x294::sub_433DF0` | 92 |  |  | 1/1 | todo |  |
| 0x433E50 | `sub_433E50` | 53 |  | Source/Ped.hpp:679 | 1/1 | todo |  |
| 0x434070 | `sub_434070` | 25 |  | Source/Varrok_7F8.hpp:25 | 1/0 | todo |  |
| 0x4340A0 | `sub_4340A0` | 4 |  |  | 1/0 | todo |  |
| 0x434140 | `sub_434140` | 23 |  | Source/Object_5C.hpp:143 | 1/0 | todo |  |
| 0x4341B0 | `sub_4341B0` | 4 |  | Source/Car_10.hpp:10 | 1/1 | todo |  |
| 0x4343B0 | `Car_BC::sub_4343B0` | 7 |  | Source/Car_BC.hpp:933 | 1/0 | todo |  |
| 0x434950 | `sub_434950` | 10 |  | Source/Player.hpp:196 | 1/4 | todo |  |
| 0x434A10 | `sub_434A10` | 7 |  | Source/lucid_hamilton.hpp:32 | 1/0 | todo |  |
| 0x434AF0 | `sub_434AF0` | 27 |  | Source/Garage_48.hpp:30 | 1/0 | todo |  |
| 0x434B10 | `sub_434B10` | 6 |  | Source/winmain.hpp:39 | 1/3 | todo |  |
| 0x434B60 | `sub_434B60` | 27 |  | Source/Player.hpp:459, Source/youthful_einstein.hpp:24 | 1/0 | todo |  |
| 0x435550 | `Char_B4_Pool::ctor_435550` | 71 |  |  | 1/0 | todo |  |
| 0x4355E0 | `Char_8_Pool::ctor_4355E0` | 41 |  |  | 1/0 | todo |  |
| 0x435F00 | `cool_nash_0x294::sub_435F00` | 25 |  |  | 1/2 | todo |  |
| 0x436BF0 | `sub_436BF0` | 982 |  |  | 1/0 | todo |  |
| 0x437EB0 | `sub_437EB0` | 12 |  |  | 1/0 | todo |  |
| 0x439D30 | `cool_nash_0x294::sub_439D30` | 294 |  |  | 1/0 | todo |  |
| 0x43BEB0 | `sub_43BEB0` | 12 |  |  | 1/0 | todo |  |
| 0x43BEC0 | `sub_43BEC0` | 12 |  |  | 1/0 | todo |  |
| 0x43BED0 | `sub_43BED0` | 12 |  |  | 1/0 | todo |  |
| 0x43D640 | `sub_43D640` | 485 |  |  | 1/0 | todo |  |
| 0x43DF60 | `sub_43DF60` | 27 |  |  | 1/6 | todo |  |
| 0x43E0E0 | `PedPool::ctor_43E0E0` | 85 |  |  | 1/0 | todo |  |
| 0x4402C0 | `sub_4402C0` | 142 |  |  | 1/0 | todo |  |
| 0x445960 | `sub_445960` | 188 |  |  | 1/0 | todo |  |
| 0x445CC0 | `sub_445CC0` | 375 |  |  | 1/0 | todo |  |
| 0x4460D0 | `Ped_Unknown_4::sub_4460D0` | 40 |  |  | 1/0 | todo |  |
| 0x4463C0 | `sub_4463C0` | 78 |  |  | 1/0 | todo |  |
| 0x446410 | `sub_446410` | 99 |  |  | 1/0 | todo |  |
| 0x446480 | `sub_446480` | 92 |  |  | 1/0 | todo |  |
| 0x4464E0 | `sub_4464E0` | 104 |  |  | 1/0 | todo |  |
| 0x446550 | `sub_446550` | 92 |  |  | 1/0 | todo |  |
| 0x4465B0 | `sub_4465B0` | 104 |  |  | 1/0 | todo |  |
| 0x446620 | `sub_446620` | 104 |  |  | 1/0 | todo |  |
| 0x446690 | `sub_446690` | 99 |  |  | 1/0 | todo |  |
| 0x447E10 | `sub_447E10` | 12 |  | Source/sprite.hpp:100, Source/sprite.hpp:289 | 1/0 | todo |  |
| 0x447E20 | `set_xy_lazy_447E20` | 65 |  | Source/sprite.hpp:532 | 1/1 | todo |  |
| 0x447EB0 | `sub_447EB0` | 11 |  | Source/Car_BC.hpp:1184 | 1/2 | todo |  |
| 0x447EC0 | `sub_447EC0` | 11 |  | Source/Car_BC.hpp:1173 | 1/0 | todo |  |
| 0x447ED0 | `sub_447ED0` | 33 |  | Source/Car_BC.hpp:1244, Source/Car_BC.hpp:1251 | 1/0 | todo |  |
| 0x447F00 | `sub_447F00` | 53 |  | Source/Car_BC.hpp:1249 | 1/0 | todo |  |
| 0x448150 | `Zheal_15C::sub_448150` | 160 |  | Source/Cranes.cpp:146 | 1/0 | todo |  |
| 0x448900 | `sub_448900` | 119 |  | Source/Cranes.cpp:507 | 1/1 | todo |  |
| 0x44A370 | `Shooey_CC::sub_44A370` | 38 |  |  | 1/1 | todo |  |
| 0x44A3E0 | `sub_44A3E0` | 8 |  | Source/Car_BC.hpp:863 | 1/1 | todo |  |
| 0x44C050 | `sub_44C050` | 19 |  | Source/BurgerKing_67F8B0.hpp:101 | 1/0 | todo |  |
| 0x44C840 | `gmp_block_info::init_44C840` | 28 |  | Source/map_0x370.hpp:23 | 1/3 | todo |  |
| 0x44CDD0 | `sub_44CDD0` | 525 |  |  | 1/1 | todo |  |
| 0x44D3A0 | `Door_10_Pool::gdtor_44D3A0` | 30 |  |  | 1/0 | todo |  |
| 0x4527A0 | `sub_4527A0` | 106 |  |  | 1/0 | todo |  |
| 0x452990 | `sub_452990` | 35 |  |  | 1/1 | todo |  |
| 0x4529C0 | `sub_4529C0` | 66 |  |  | 1/0 | todo |  |
| 0x4538E0 | `?do_always_noconv@codecvt_base@std@@MBE_NXZ` | 3 |  |  | 1/0 | todo |  |
| 0x4539B0 | `gtx_0x106C::sub_4539B0` | 24 | 0x5D7700 MATCH | Source/Draw.hpp:66 | 1/2 | todo |  |
| 0x4539D0 | `sub_4539D0` | 29 |  | Source/Draw.hpp:72 | 1/2 | todo |  |
| 0x453A30 | `laughing_blackwell_0x1EB54::sub_453A30` | 15 |  | Source/Frontend.hpp:315 | 1/0 | todo |  |
| 0x453A90 | `sub_453A90` | 11 |  | Source/youthful_einstein.hpp:27 | 1/0 | todo |  |
| 0x453AA0 | `sub_453AA0` | 11 |  | Source/youthful_einstein.hpp:42 | 1/1 | todo |  |
| 0x453AB0 | `sub_453AB0` | 169 |  | Source/Frontend.cpp:375, Source/youthful_einstein.hpp:48 | 1/0 | todo |  |
| 0x453D40 | `sub_453D40` | 49 |  |  | 1/0 | todo |  |
| 0x45AFD0 | `Garox_C4::sub_45AFD0` | 25 |  | Source/Hud.hpp:246 | 1/2 | todo |  |
| 0x45DD50 | `Zone_144::sub_45DD50` | 12 |  | Source/Gang.hpp:12 | 1/2 | todo |  |
| 0x460CC0 | `sub_460CC0` | 105 |  |  | 1/0 | todo |  |
| 0x460D30 | `sub_460D30` | 105 |  |  | 1/0 | todo |  |
| 0x460DA0 | `magical_germain_0x8EC::sub_460DA0` | 55 |  |  | 1/0 | todo |  |
| 0x461DC0 | `sub_461DC0` | 7 |  | Source/lucid_hamilton.hpp:98 | 1/1 | todo |  |
| 0x463150 | `sub_463150` | 162 |  |  | 1/0 | todo |  |
| 0x463210 | `sub_463210` | 189 |  |  | 1/0 | todo |  |
| 0x4632E0 | `ProcessObjective_4632E0` | 176 |  | Source/Ped.hpp:226 | 1/0 | todo |  |
| 0x463480 | `sub_463480` | 42 |  | Source/map_0x370.hpp:693 | 1/0 | todo |  |
| 0x4634B0 | `sub_4634B0` | 42 |  | Source/map_0x370.hpp:693 | 1/0 | todo |  |
| 0x4634E0 | `sub_4634E0` | 76 |  | Source/map_0x370.hpp:559 | 1/2 | todo |  |
| 0x463690 | `sub_463690` | 124 |  | Source/Fix16_Rect.hpp:74, Source/map_0x370.cpp:1202 | 1/0 | todo |  |
| 0x463760 | `sub_463760` | 56 |  | Source/Fix16_Rect.hpp:68 | 1/0 | todo |  |
| 0x466B70 | `sub_466B70` | 361 | 0x466B70 MATCH |  | 1/10 | todo |  |
| 0x467110 | `sub_467110` | 3660 |  |  | 1/1 | todo |  |
| 0x469010 | `sub_469010` | 84 | 0x52B2A0 MATCH |  | 1/1 | todo |  |
| 0x4699A0 | `sub_4699A0` | 346 | 0x4E4F40 MATCH |  | 1/1 | todo |  |
| 0x46B5E0 | `Nanobotz::sub_46B5E0` | 44 |  | Source/MapRenderer.hpp:116 | 1/0 | todo |  |
| 0x46BC70 | `sub_46BC70` | 207 |  |  | 1/0 | todo |  |
| 0x46BD40 | `Nanobotz::sub_46BD40` | 168 | 0x4EAE00 MATCH |  | 1/0 | todo |  |
| 0x46BEA0 | `sub_46BEA0` | 531 | 0x4F3FB0 MATCH |  | 1/0 | todo |  |
| 0x46C0C0 | `Nanobotz::Set_UV_46C0C0` | 115 | 0x4F4190 MATCH |  | 1/0 | matched |  |
| 0x46C2C0 | `Nanobotz::sub_46C2C0` | 1324 |  |  | 1/0 | todo |  |
| 0x46C7F0 | `Nanobotz::sub_46C7F0` | 1600 |  |  | 1/0 | todo |  |
| 0x46CE30 | `Nanobotz::sub_46CE30` | 1325 |  |  | 1/0 | todo |  |
| 0x46DFE0 | `Nanobotz::sub_46DFE0` | 723 |  |  | 1/0 | todo |  |
| 0x470060 | `Nanobotz::sub_470060` | 482 |  |  | 1/0 | todo |  |
| 0x470440 | `Nanobotz::sub_470440` | 478 |  |  | 1/0 | todo |  |
| 0x470620 | `Nanobotz::sub_470620` | 478 |  |  | 1/0 | todo |  |
| 0x472C60 | `sub_472C60` | 90 |  |  | 1/0 | todo |  |
| 0x472FD0 | `sub_472FD0` | 11 |  | Source/Ped.hpp:722 | 1/1 | todo |  |
| 0x4740F0 | `sub_4740F0` | 7 |  |  | 1/0 | todo |  |
| 0x4741F0 | `sub_4741F0` | 542 |  |  | 1/0 | todo |  |
| 0x474490 | `sub_474490` | 25 |  |  | 1/0 | todo |  |
| 0x4744C0 | `sub_4744C0` | 97 |  |  | 1/0 | todo |  |
| 0x475C10 | `Car_BC::sub_475C10` | 20 |  | Source/Car_BC.hpp:869 | 1/1 | todo |  |
| 0x476230 | `sub_476230` | 8 |  | Source/Garage_48.cpp:318 | 1/0 | todo |  |
| 0x4766A0 | `angry_lewin_0x85C::sub_4766A0` | 11 |  | Source/Player.hpp:125 | 1/8 | todo |  |
| 0x4766D0 | `angry_lewin_0x85C::sub_4766D0` | 4 |  | Source/Player.hpp:57 | 1/3 | todo |  |
| 0x476A30 | `sub_476A30` | 41 |  | Source/Garage_48.cpp:318 | 1/0 | todo |  |
| 0x47ED20 | `sub_47ED20` | 136 |  |  | 1/0 | todo |  |
| 0x47F4F0 | `sub_47F4F0` | 45 |  | Source/Light_1D4CC.hpp:109 | 1/1 | todo |  |
| 0x481D80 | `sub_481D80` | 29 |  |  | 1/0 | todo |  |
| 0x481DA0 | `sub_481DA0` | 21 |  |  | 1/0 | todo |  |
| 0x481DC0 | `sub_481DC0` | 11 |  |  | 1/0 | todo |  |
| 0x482080 | `sub_482080` | 11 |  | Source/Ped.hpp:396 | 1/2 | todo |  |
| 0x4820A0 | `sub_4820A0` | 17 |  | Source/Hud.hpp:455 | 1/0 | todo |  |
| 0x482400 | `sub_482400` | 16 |  | Source/Object_5C.hpp:276 | 1/2 | todo |  |
| 0x4824E0 | `sub_4824E0` | 39 |  | Source/fix16.hpp:446 | 1/2 | todo |  |
| 0x4826A0 | `Object_2C::sub_4826A0` | 79 | 0x525AE0 MATCH |  | 1/3 | todo |  |
| 0x4828F0 | `sub_4828F0` | 57 |  |  | 1/0 | todo |  |
| 0x482960 | `sub_482960` | 10 |  |  | 1/0 | todo |  |
| 0x482BA0 | `sub_482BA0` | 43 |  | Source/Object_3C.cpp:44, Source/Object_3C.hpp:100 | 1/1 | todo |  |
| 0x482BD0 | `sub_482BD0` | 5 |  | Source/Object_3C.hpp:92 | 1/0 | todo |  |
| 0x482CC0 | `sub_482CC0` | 50 |  | Source/CarPhysics_B0.hpp:280 | 1/0 | todo |  |
| 0x482D30 | `sub_482D30` | 40 |  | Source/nostalgic_ellis_0x28.hpp:94 | 1/0 | todo |  |
| 0x482F60 | `sub_482F60` | 30 |  |  | 1/3 | todo |  |
| 0x483570 | `sub_483570` | 105 |  |  | 1/0 | todo |  |
| 0x483FA0 | `Object_8_Pool::sub_483FA0` | 18 |  |  | 1/1 | todo |  |
| 0x483FE0 | `Object_3C_Pool::sub_483FE0` | 19 |  |  | 1/3 | todo |  |
| 0x484260 | `sub_484260` | 311 |  |  | 1/0 | todo |  |
| 0x4847D0 | `sub_4847D0` | 75 |  | Source/Object_5C.cpp:2077 | 1/1 | todo |  |
| 0x485260 | `sub_485260` | 45 |  |  | 1/2 | todo |  |
| 0x4852E0 | `Object_5C::sub_4852E0` | 52 | 0x5299B0 MATCH |  | 1/1 | todo |  |
| 0x485500 | `sub_485500` | 55 |  |  | 1/0 | todo |  |
| 0x4895D0 | `sub_4895D0` | 16 |  |  | 1/0 | todo |  |
| 0x4895E0 | `sub_4895E0` | 8 |  | Source/Garage_48.cpp:318 | 1/0 | todo |  |
| 0x4895F0 | `sub_4895F0` | 5 |  | Source/Door_38.hpp:79 | 1/0 | todo |  |
| 0x489600 | `sub_489600` | 18 |  | Source/Door_38.hpp:85 | 1/0 | todo |  |
| 0x489650 | `sub_489650` | 44 |  |  | 1/0 | todo |  |
| 0x48A390 | `sub_48A390` | 99 |  |  | 1/0 | todo |  |
| 0x48A420 | `sub_48A420` | 54 |  |  | 1/0 | todo |  |
| 0x48A4C0 | `sub_48A4C0` | 4 |  | Source/char.hpp:148 | 1/0 | todo |  |
| 0x48A550 | `Wolfy_30::sub_48A550` | 60 |  |  | 1/0 | todo |  |
| 0x48A930 | `sub_48A930` | 20 |  |  | 1/0 | todo |  |
| 0x48A950 | `sub_48A950` | 20 |  |  | 1/0 | todo |  |
| 0x48D1F0 | `sub_48D1F0` | 745 |  |  | 1/1 | todo |  |
| 0x48EA50 | `sub_48EA50` | 166 |  |  | 1/0 | todo |  |
| 0x491EA0 | `sub_491EA0` | 30 |  |  | 1/1 | todo |  |
| 0x491EE0 | `sub_491EE0` | 6 |  |  | 1/3 | todo |  |
| 0x491EF0 | `sub_491EF0` | 9 |  | Source/fix16.hpp:440 | 1/1 | todo |  |
| 0x491F00 | `sub_491F00` | 9 |  | Source/fix16.hpp:434 | 1/1 | todo |  |
| 0x491FA0 | `sub_491FA0` | 204 |  |  | 1/0 | todo |  |
| 0x492130 | `sub_492130` | 8 |  | Source/map_0x370.hpp:677 | 1/0 | todo |  |
| 0x492140 | `sub_492140` | 41 |  | Source/map_0x370.hpp:671 | 1/0 | todo |  |
| 0x492170 | `sub_492170` | 12 |  | Source/sprite.hpp:105 | 1/1 | todo |  |
| 0x492190 | `sub_492190` | 509 |  |  | 1/1 | todo |  |
| 0x4923A0 | `sub_4923A0` | 42 |  | Source/char.hpp:247 | 1/0 | todo |  |
| 0x4923D0 | `sub_4923D0` | 42 |  | Source/char.hpp:239 | 1/0 | todo |  |
| 0x492C20 | `sub_492C20` | 7 |  | Source/Ped.hpp:699 | 1/0 | todo |  |
| 0x492C30 | `sub_492C30` | 128 |  |  | 1/0 | todo |  |
| 0x492CB0 | `sub_492CB0` | 13 |  | Source/Ped.hpp:449 | 1/0 | todo |  |
| 0x492CE0 | `sub_492CE0` | 15 |  | Source/Ped.hpp:656 | 1/1 | todo |  |
| 0x492CF0 | `sub_492CF0` | 15 |  | Source/Ped.hpp:661 | 1/1 | todo |  |
| 0x492FD0 | `sub_492FD0` | 29 |  | Source/Ped.hpp:808 | 1/0 | todo |  |
| 0x493540 | `sub_493540` | 14 |  | Source/Garage_48.hpp:36 | 1/0 | todo |  |
| 0x4937D0 | `sub_4937D0` | 62 |  |  | 1/1 | todo |  |
| 0x493810 | `sub_493810` | 62 |  |  | 1/1 | todo |  |
| 0x495470 | `sub_495470` | 195 | 0x495470 MATCH |  | 1/0 | todo |  |
| 0x496500 | `sub_496500` | 751 |  |  | 1/0 | todo |  |
| 0x4994D0 | `sub_4994D0` | 190 |  |  | 1/0 | todo |  |
| 0x4995A0 | `sub_4995A0` | 2313 |  |  | 1/0 | todo |  |
| 0x49E0E0 | `sub_49E0E0` | 22 |  |  | 1/0 | todo |  |
| 0x49E240 | `sub_49E240` | 48 |  | Source/CarPhysics_B0.cpp:223 | 1/0 | todo |  |
| 0x49E420 | `sub_49E420` | 35 |  |  | 1/0 | todo |  |
| 0x49E480 | `sub_49E480` | 114 |  | Source/Fix16_Point.hpp:186 | 1/0 | todo |  |
| 0x49E500 | `sub_49E500` | 62 |  |  | 1/0 | todo |  |
| 0x49EBE0 | `sub_49EBE0` | 149 |  | Source/CarPhysics_B0.cpp:1270 | 1/0 | todo |  |
| 0x49ED00 | `sub_49ED00` | 81 |  |  | 1/0 | todo |  |
| 0x49EF40 | `sub_49EF40` | 7 |  | Source/Ped.hpp:455 | 1/0 | todo |  |
| 0x49EF50 | `sub_49EF50` | 34 |  | Source/CarPhysics_B0.cpp:180, Source/CarPhysics_B0.hpp:315 | 1/2 | todo |  |
| 0x49EFC0 | `sub_49EFC0` | 11 |  | Source/Car_BC.hpp:1295 | 1/0 | todo |  |
| 0x49EFE0 | `sub_49EFE0` | 42 |  | Source/Car_BC.hpp:1285 | 1/0 | todo |  |
| 0x49FF70 | `sub_49FF70` | 5 |  |  | 1/0 | todo |  |
| 0x4A5130 | `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | 20 |  | Source/Player.hpp:301 | 1/9 | todo |  |
| 0x4AF290 | `sub_4AF290` | 31 |  |  | 1/0 | todo |  |
| 0x4AF860 | `sub_4AF860` | 23 |  | Source/Ped.hpp:62 | 1/0 | todo |  |
| 0x4AF880 | `sub_4AF880` | 23 |  | Source/Ped.hpp:62 | 1/0 | todo |  |
| 0x4B25A0 | `sub_4B25A0` | 38 |  |  | 1/0 | todo |  |
| 0x4B25D0 | `sub_4B25D0` | 595 |  |  | 1/0 | todo |  |
| 0x4B30A0 | `sub_4B30A0` | 56 |  |  | 1/0 | todo |  |
| 0x4B3110 | `sub_4B3110` | 24 |  | Source/Camera.hpp:176 | 1/0 | todo |  |
| 0x4B3130 | `sub_4B3130` | 24 |  | Source/Camera.hpp:181 | 1/0 | todo |  |
| 0x4B3230 | `sub_4B3230` | 440 |  |  | 1/0 | todo |  |
| 0x4B33F0 | `sub_4B33F0` | 145 |  |  | 1/0 | todo |  |
| 0x4B6110 | `sub_4B6110` | 25 |  |  | 1/0 | todo |  |
| 0x4B6130 | `sub_4B6130` | 25 |  |  | 1/0 | todo |  |
| 0x4B6150 | `sub_4B6150` | 25 |  |  | 1/0 | todo |  |
| 0x4B6170 | `sub_4B6170` | 25 |  |  | 1/0 | todo |  |
| 0x4B6190 | `sub_4B6190` | 25 |  |  | 1/0 | todo |  |
| 0x4B61E0 | `sub_4B61E0` | 25 |  |  | 1/0 | todo |  |
| 0x4B6200 | `sub_4B6200` | 25 |  | Source/Frontend.hpp:124 | 1/0 | todo |  |
| 0x4B62B0 | `sub_4B62B0` | 28 |  |  | 1/0 | todo |  |
| 0x4B62D0 | `sub_4B62D0` | 28 |  |  | 1/0 | todo |  |
| 0x4B6320 | `sub_4B6320` | 28 |  |  | 1/0 | todo |  |
| 0x4B6340 | `sub_4B6340` | 28 |  |  | 1/0 | todo |  |
| 0x4B6360 | `sub_4B6360` | 28 |  |  | 1/0 | todo |  |
| 0x4B63B0 | `sub_4B63B0` | 28 |  |  | 1/0 | todo |  |
| 0x4B90E0 | `DrawUnk_0xBC::sub_4B90E0` | 243 |  | Source/Camera.hpp:76 | 1/0 | todo |  |
| 0x4B92B0 | `CokeZero_50::sub_4B92B0` | 387 |  | Source/ExplodingScore_100.cpp:327 | 1/0 | todo |  |
| 0x4B9E60 | `sub_4B9E60` | 147 |  |  | 1/0 | todo |  |
| 0x4BA5E0 | `sub_4BA5E0` | 54 |  |  | 1/3 | todo |  |
| 0x4BB3C0 | `sub_4BB3C0` | 266 |  |  | 1/0 | todo |  |
| 0x4BB4D0 | `sub_4BB4D0` | 142 |  |  | 1/0 | todo |  |
| 0x4BB9C0 | `sub_4BB9C0` | 531 |  |  | 1/0 | todo |  |
| 0x4BCD00 | `sub_4BCD00` | 668 |  |  | 1/1 | todo |  |
| 0x4BCFA0 | `sub_4BCFA0` | 683 |  |  | 1/1 | todo |  |
| 0x4BD490 | `sub_4BD490` | 381 |  |  | 1/0 | todo |  |
| 0x4BDDD0 | `sub_4BDDD0` | 276 |  |  | 1/1 | todo |  |
| 0x4BE830 | `sub_4BE830` | 22 |  | Source/Object_5C.hpp:261 | 1/1 | todo |  |
| 0x4BEA60 | `sub_4BEA60` | 48 | 0x5A6AD0 MATCH |  | 1/0 | todo |  |
| 0x4BEC50 | `Sprite_18_Pool::sub_4BEC50` | 14 |  |  | 1/9 | todo |  |
| 0x4BED60 | `sub_4BED60` | 43 |  |  | 1/5 | todo |  |
| 0x4BF550 | `sub_4BF550` | 25 |  |  | 1/2 | todo |  |
| 0x4C03F0 | `sub_4C03F0` | 25 |  |  | 1/0 | todo |  |
| 0x4C23B0 | `sub_4C23B0` | 19 |  |  | 1/9 | todo |  |
| 0x4C23D0 | `sub_4C23D0` | 24 |  | Source/gtx_0x106C.hpp:414 | 1/0 | todo |  |
| 0x4C2EB0 | `gtx_0x106C::get_tile_4C2EB0` | 33 |  | Source/gtx_0x106C.hpp:232 | 1/0 | todo |  |
| 0x4C2EE0 | `gtx_0x106C::has_tiles_4C2EE0` | 9 |  | Source/gtx_0x106C.hpp:226 | 1/0 | todo |  |
| 0x4C3970 | `unknown_libname_28` | 10 |  |  | 1/0 | todo |  |
| 0x4C4B40 | `sub_4C4B40` | 31 |  | Source/Montana.hpp:63 | 1/0 | todo |  |
| 0x4C4D80 | `Montana_2EE4::gdtor_4C4D80` | 26 |  |  | 1/0 | todo |  |
| 0x4C4DA0 | `Montana_FA4::gdtor_4C4DA0` | 26 |  |  | 1/0 | todo |  |
| 0x4C4F20 | `sub_4C4F20` | 7 |  | Source/Ped.hpp:550 | 1/0 | todo |  |
| 0x4C54F0 | `Kfc_30::sub_4C54F0` | 30 |  |  | 1/0 | todo |  |
| 0x4C6E30 | `sub_4C6E30` | 17 |  | Source/Phi_8CA8.hpp:336 | 1/0 | todo |  |
| 0x4C6F20 | `sub_4C6F20` | 9 |  | Source/Hud.hpp:443, Source/Hud.hpp:508 | 1/0 | todo |  |
| 0x4C6FB0 | `sub_4C6FB0` | 35 |  | Source/Hud.hpp:480 | 1/0 | todo |  |
| 0x4C7060 | `sub_4C7060` | 17 |  | Source/Hud.hpp:542 | 1/0 | todo |  |
| 0x4C70E0 | `Garox_C4::sub_4C70E0` | 15 |  | Source/Hud.hpp:239 | 1/0 | todo |  |
| 0x4C7220 | `sub_4C7220` | 38 |  | Source/Hud.hpp:345 | 1/3 | todo |  |
| 0x4C7250 | `sub_4C7250` | 38 |  | Source/Hud.hpp:351 | 1/2 | todo |  |
| 0x4C83D0 | `sub_4C83D0` | 121 |  |  | 1/0 | todo |  |
| 0x4C8620 | `sub_4C8620` | 35 |  |  | 1/0 | todo |  |
| 0x4C8CA0 | `sub_4C8CA0` | 400 |  |  | 1/0 | todo |  |
| 0x4C93B0 | `sub_4C93B0` | 18 |  |  | 1/1 | todo |  |
| 0x4CBA00 | `magical_germain_0x8EC::sub_4CBA00` | 4 |  |  | 1/0 | todo |  |
| 0x4CBA10 | `sub_4CBA10` | 4 |  |  | 1/0 | todo |  |
| 0x4CBA20 | `magical_germain_0x8EC::sub_4CBA20` | 4 |  |  | 1/0 | todo |  |
| 0x4CBA30 | `sub_4CBA30` | 4 |  |  | 1/0 | todo |  |
| 0x4CBA40 | `magical_germain_0x8EC::sub_4CBA40` | 9 |  |  | 1/0 | todo |  |
| 0x4CCBD0 | `sub_4CCBD0` | 1060 |  |  | 1/0 | todo |  |
| 0x4CD8C0 | `sub_4CD8C0` | 148 |  | Source/Weapon_30.cpp:1953 | 1/0 | todo |  |
| 0x4D09D0 | `sub_4D09D0` | 41 |  |  | 1/0 | todo |  |
| 0x401180 | `sub_401180` | 316 | 0x401180 unmarked |  | 0/0 | todo |  |
| 0x401CF0 | `array_constuctor_401CF0` | 42 |  |  | 0/6 | todo |  |
| 0x402B20 | `sub_402B20` | 253 | 0x4A6DB0 unmarked |  | 0/1 | todo |  |
| 0x4038E0 | `?MarkRootRemoved@VirtualProcessorRoot@details@Concurrency@@QAEXXZ` | 5 |  | Source/PedGroup.hpp:11 | 0/1 | todo |  |
| 0x4038F0 | `sub_4038F0` | 4 |  | Source/PedGroup.hpp:17 | 0/1 | todo |  |
| 0x403900 | `sub_403900` | 7 |  | Source/char.hpp:271 | 0/2 | todo |  |
| 0x403940 | `cool_nash_0x294::sub_403940` | 13 |  | Source/Ped.hpp:428 | 0/3 | todo |  |
| 0x4039C0 | `cool_nash_0x294::get_enter_car_as_passenger_4039C0` | 7 |  | Source/Ped.hpp:44 | 0/1 | todo |  |
| 0x403A20 | `sub_403A20` | 8 |  | Source/Ped.hpp:710 | 0/1 | todo |  |
| 0x403A50 | `sub_403A50` | 11 |  | Source/PedGroup.cpp:330 | 0/1 | todo |  |
| 0x403AB0 | `cool_nash_0x294::get_target_objective_car_403AB0` | 7 |  | Source/Ped.hpp:489 | 0/3 | todo |  |
| 0x403B20 | `cool_nash_0x294::sub_403B20` | 8 |  | Source/Ped.hpp:95 | 0/1 | todo |  |
| 0x403B70 | `sub_403B70` | 12 |  | Source/Ped.hpp:766 | 0/3 | todo |  |
| 0x403BC0 | `sub_403BC0` | 22 |  | Source/Car_BC.hpp:1130 | 0/1 | todo |  |
| 0x404400 | `sub_404400` | 30 |  |  | 0/7 | todo |  |
| 0x404420 | `sub_404420` | 42 |  |  | 0/6 | todo |  |
| 0x404480 | `sub_404480` | 8 |  |  | 0/1 | todo |  |
| 0x404490 | `sub_404490` | 310 |  |  | 0/1 | todo |  |
| 0x4045D0 | `sub_4045D0` | 257 | 0x4CAE80 MATCH |  | 0/1 | todo |  |
| 0x4046F0 | `sub_4046F0` | 334 |  |  | 0/1 | todo |  |
| 0x404AD0 | `sub_404AD0` | 366 |  |  | 0/1 | todo |  |
| 0x4059B0 | `Memory::malloc_4059B0` | 49 | 0x4FE4D0 MATCH |  | 0/0 | todo |  |
| 0x405A80 | `text_0x14::dtor_405A80` | 30 |  |  | 0/3 | todo |  |
| 0x4061C0 | `sub_4061C0` | 73 |  |  | 0/1 | todo |  |
| 0x409DA0 | `sub_409DA0` | 55 | 0x521100 MATCH |  | 0/1 | todo |  |
| 0x40BF80 | `sub_40BF80` | 26 |  |  | 0/1 | todo |  |
| 0x40CC10 | `sub_40CC10` | 176 |  |  | 0/1 | todo |  |
| 0x40CE90 | `sub_40CE90` | 9 |  | Source/RouteFinder.hpp:10 | 0/7 | todo |  |
| 0x40CEA0 | `sub_40CEA0` | 10 |  | Source/RouteFinder.hpp:22 | 0/3 | todo |  |
| 0x40CEB0 | `Enable_40CEB0` | 5 |  | Source/RouteFinder.hpp:33 | 0/1 | todo |  |
| 0x40CEC0 | `Disable_40CEC0` | 5 |  | Source/RouteFinder.hpp:28 | 0/1 | todo |  |
| 0x40CED0 | `sub_40CED0` | 9 |  | Source/RouteFinder.hpp:16 | 0/2 | todo |  |
| 0x40CF20 | `sub_40CF20` | 59 |  | Source/RouteFinder.hpp:66 | 0/2 | todo |  |
| 0x40D690 | `sub_40D690` | 320 | 0x589480 MATCH |  | 0/1 | todo |  |
| 0x40DFA0 | `sub_40DFA0` | 67 | 0x589E20 WIP |  | 0/1 | todo |  |
| 0x40E030 | `sub_40E030` | 83 | 0x58A190 MATCH |  | 0/1 | todo |  |
| 0x40E0F0 | `sub_40E0F0` | 5 |  |  | 0/1 | todo |  |
| 0x40E790 | `sub_40E790` | 126 |  | Source/Cranes.cpp:54 | 0/4 | todo |  |
| 0x40F490 | `sub_40F490` | 27 |  |  | 0/2 | todo |  |
| 0x40FB70 | `sub_40FB70` | 102 |  |  | 0/1 | todo |  |
| 0x40FE80 | `sub_40FE80` | 24 |  | Source/sprite.hpp:350, Source/sprite.hpp:357 | 0/1 | todo |  |
| 0x40FEF0 | `sub_40FEF0` | 4 |  | Source/Object_5C.hpp:200 | 0/6 | todo |  |
| 0x40FF00 | `sub_40FF00` | 7 |  | Source/Object_5C.hpp:206 | 0/1 | todo |  |
| 0x40FF10 | `sub_40FF10` | 10 |  | Source/Rozza_C88.hpp:97 | 0/3 | todo |  |
| 0x410550 | `sub_410550` | 10 |  |  | 0/2 | todo |  |
| 0x410580 | `unknown_libname_9` | 10 |  |  | 0/1 | todo |  |
| 0x410590 | `unknown_libname_10` | 10 |  |  | 0/1 | todo |  |
| 0x410730 | `root_sound::sub_410730` | 19 |  | Source/root_sound.hpp:23 | 0/1 | todo |  |
| 0x4116B0 | `sub_4116B0` | 64 | 0x419020 MATCH |  | 0/1 | todo |  |
| 0x4116F0 | `sub_4116F0` | 52 | 0x419070 MATCH |  | 0/1 | todo |  |
| 0x4117B0 | `sub_4117B0` | 30 |  | Source/sprite.hpp:292 | 0/3 | todo |  |
| 0x411870 | `sub_411870` | 24 |  |  | 0/1 | todo |  |
| 0x411890 | `sub_411890` | 17 |  |  | 0/1 | todo |  |
| 0x4118C0 | `Car_BC::sub_4118C0` | 11 |  | Source/Car_BC.hpp:451 | 0/3 | todo |  |
| 0x4118F0 | `sub_4118F0` | 11 |  | Source/Car_BC.hpp:1142 | 0/9 | todo |  |
| 0x411910 | `Car_BC::sub_411910` | 11 |  | Source/Car_BC.hpp:1115 | 0/6 | todo |  |
| 0x411920 | `sub_411920` | 11 |  | Source/Car_BC.hpp:805, Source/Car_BC.hpp:1158 | 0/7 | todo |  |
| 0x411950 | `sub_411950` | 17 |  | Source/Car_BC.hpp:789 | 0/1 | todo |  |
| 0x411970 | `sub_411970` | 20 |  | Source/Car_BC.hpp:1216 | 0/4 | todo |  |
| 0x411990 | `sub_411990` | 17 |  | Source/Car_BC.hpp:1221 | 0/1 | todo |  |
| 0x4119D0 | `sub_4119D0` | 17 |  | Source/Car_BC.hpp:781 | 0/1 | todo |  |
| 0x4119F0 | `?_GetResult@?$_Task_impl@_N@details@Concurrency@@QAE_NXZ` | 7 |  | Source/Car_BC.hpp:1136 | 0/1 | todo |  |
| 0x411A00 | `sub_411A00` | 7 |  | Source/Cranes.hpp:65 | 0/2 | todo |  |
| 0x411A10 | `sub_411A10` | 4 |  | Source/Crushers.hpp:25 | 0/1 | todo |  |
| 0x411A20 | `unknown_libname_20` | 12 |  | Source/Crushers.hpp:13 | 0/1 | todo |  |
| 0x411A30 | `sub_411A30` | 12 |  | Source/Crushers.hpp:19 | 0/1 | todo |  |
| 0x411A40 | `sub_411A40` | 3 |  | Source/Hud.hpp:305 | 0/1 | todo |  |
| 0x412680 | `sub_412680` | 428 |  |  | 0/1 | todo |  |
| 0x412830 | `sub_412830` | 416 |  |  | 0/1 | todo |  |
| 0x412A20 | `sub_412A20` | 426 |  |  | 0/1 | todo |  |
| 0x412BD0 | `sub_412BD0` | 428 |  |  | 0/1 | todo |  |
| 0x412D80 | `sub_412D80` | 299 |  |  | 0/1 | todo |  |
| 0x412EB0 | `sub_412EB0` | 289 |  |  | 0/1 | todo |  |
| 0x413160 | `sub_413160` | 909 |  |  | 0/2 | todo |  |
| 0x4139A0 | `sub_4139A0` | 300 |  |  | 0/1 | todo |  |
| 0x413DF0 | `sub_413DF0` | 84 | 0x417D70 MATCH |  | 0/1 | todo |  |
| 0x413EB0 | `sub_413EB0` | 370 |  |  | 0/1 | todo |  |
| 0x414030 | `sub_414030` | 363 |  |  | 0/1 | todo |  |
| 0x414200 | `sub_414200` | 426 |  |  | 0/1 | todo |  |
| 0x4143B0 | `sub_4143B0` | 968 | 0x413BE0 MATCH |  | 0/3 | todo |  |
| 0x414780 | `sub_414780` | 325 |  |  | 0/1 | todo |  |
| 0x4148D0 | `sub_4148D0` | 744 |  |  | 0/1 | todo |  |
| 0x414F70 | `sub_414F70` | 11 |  | Source/CarPhysics_B0.cpp:2205 | 0/2 | todo |  |
| 0x414F80 | `sub_414F80` | 29 |  | Source/Car_BC.hpp:776 | 0/1 | todo |  |
| 0x4156B0 | `sub_4156B0` | 5 |  |  | 0/1 | todo |  |
| 0x4156C0 | `sub_4156C0` | 443 |  |  | 0/1 | todo |  |
| 0x415A90 | `sub_415A90` | 336 |  |  | 0/1 | todo |  |
| 0x4166E0 | `sub_4166E0` | 103 | 0x41A650 MATCH |  | 0/1 | todo |  |
| 0x416B70 | `sub_416B70` | 4 |  | Source/Camera.hpp:39 | 0/1 | todo |  |
| 0x416B80 | `Car_BC::sub_416B80` | 39 |  | Source/Car_BC.hpp:1260 | 0/1 | todo |  |
| 0x416C00 | `ctor_416C00` | 3 |  | Source/sound_obj.hpp:26 | 0/1 | todo |  |
| 0x4171A0 | `sub_4171A0` | 178 | 0x41A850 MATCH |  | 0/1 | todo |  |
| 0x41B030 | `sub_41B030` | 45 |  |  | 0/1 | todo |  |
| 0x41C0F0 | `nullsub_4` | 1 | 0x426670 unmarked |  | 0/1 | todo |  |
| 0x41C1F0 | `sub_41C1F0` | 5 |  | Source/sprite.hpp:190 | 0/4 | todo |  |
| 0x41D070 | `Car_14_18::ctor_41D070` | 3 |  | Source/Fix16_Rect.hpp:29 | 0/1 | todo |  |
| 0x41D580 | `sub_41D580` | 160 |  |  | 0/1 | todo |  |
| 0x41DC30 | `thirsty_lamarr::sub_41DC30` | 3 |  | Source/thirsty_lamarr.hpp:35 | 0/4 | todo |  |
| 0x41E130 | `sub_41E130` | 44 |  | Source/Camera.hpp:219 | 0/2 | todo |  |
| 0x41E3D0 | `sub_41E3D0` | 51 |  | Source/Camera.hpp:168 | 0/3 | todo |  |
| 0x41E410 | `DrawUnk_0xBC::CommitCameraTarget_41E410` | 27 |  | Source/Camera.hpp:136, Source/Camera.hpp:148 | 0/5 | todo |  |
| 0x41E4E0 | `sub_41E4E0` | 34 |  |  | 0/1 | todo |  |
| 0x41E510 | `sub_41E510` | 34 |  |  | 0/1 | todo |  |
| 0x41E540 | `sub_41E540` | 53 |  |  | 0/1 | todo |  |
| 0x41E580 | `sub_41E580` | 148 |  |  | 0/1 | todo |  |
| 0x41EAD0 | `DrawUnk_0xBC::sub_41EAD0` | 34 |  |  | 0/1 | todo |  |
| 0x41EB00 | `sub_41EB00` | 101 |  |  | 0/1 | todo |  |
| 0x41F180 | `DrawUnk_0xBC::sub_41F180` | 368 |  |  | 0/1 | todo |  |
| 0x41F880 | `Car_BC::sub_41F880` | 21 | 0x43B360 MATCH |  | 0/1 | todo |  |
| 0x41FC70 | `sub_41FC70` | 18 |  |  | 0/1 | todo |  |
| 0x41FEA0 | `sub_41FEA0` | 11 |  | Source/Car_BC.hpp:1241, Source/gtx_0x106C.hpp:102 | 0/1 | todo |  |
| 0x41FF00 | `sub_41FF00` | 11 |  | Source/Car_BC.hpp:1277, Source/gtx_0x106C.hpp:107 | 0/1 | todo |  |
| 0x420200 | `gtx_0x106C::sub_420200` | 22 |  | Source/gtx_0x106C.hpp:361 | 0/3 | todo |  |
| 0x4206E0 | `sub_4206E0` | 12 |  | Source/sprite.hpp:468 | 0/2 | todo |  |
| 0x420710 | `sub_420710` | 10 |  | Source/sprite.hpp:454 | 0/1 | todo |  |
| 0x420760 | `sub_420760` | 46 |  |  | 0/1 | todo |  |
| 0x420B60 | `sub_420B60` | 7 |  | Source/Ped.hpp:550 | 0/7 | todo |  |
| 0x420B80 | `cool_nash_0x294::sub_420B80` | 10 |  | Source/Ped.hpp:716 | 0/6 | todo |  |
| 0x420C30 | `sub_420C30` | 11 |  | Source/Car_BC.cpp:2862 | 0/2 | todo |  |
| 0x420D80 | `maybe_flags::ctor_420D80` | 9 |  | Source/BitSet32.hpp:122 | 0/1 | todo |  |
| 0x420D90 | `sub_420D90` | 7 |  | Source/BitSet32.hpp:116 | 0/1 | todo |  |
| 0x420DC0 | `maybe_flags::set_420DC0` | 20 |  | Source/BitSet32.hpp:57, Source/Car_10.cpp:6 | 0/4 | todo |  |
| 0x420DE0 | `maybe_flags::clear_420DE0` | 26 |  | Source/BitSet32.hpp:51, Source/Car_10.cpp:6 | 0/6 | todo |  |
| 0x420E20 | `sub_420E20` | 27 |  | Source/BitSet32.hpp:69 | 0/2 | todo |  |
| 0x420F20 | `Car_BC_Pool::sub_420F20` | 16 |  | Source/Car_BC.hpp:1393 | 0/3 | todo |  |
| 0x421020 | `sub_421020` | 4 |  |  | 0/1 | todo |  |
| 0x421030 | `Sprite_Pool::sub_421030` | 27 |  | Source/sprite.hpp:760 | 0/5 | todo |  |
| 0x421150 | `sub_421150` | 18 |  |  | 0/1 | todo |  |
| 0x421170 | `sub_421170` | 18 |  | Source/CarPhysics_B0.hpp:95 | 0/1 | todo |  |
| 0x4211E0 | `sub_4211E0` | 20 |  | Source/CarPhysics_B0.hpp:102 | 0/1 | todo |  |
| 0x421270 | `sub_421270` | 7 |  | Source/CarPhysics_B0.hpp:126 | 0/1 | todo |  |
| 0x4212A0 | `sub_4212A0` | 11 |  | Source/CarPhysics_B0.hpp:334 | 0/4 | todo |  |
| 0x4212B0 | `sub_4212B0` | 11 |  | Source/CarPhysics_B0.hpp:340 | 0/3 | todo |  |
| 0x4212D0 | `sub_4212D0` | 14 |  | Source/Car_BC.hpp:340 | 0/3 | todo |  |
| 0x421340 | `sub_421340` | 8 |  | Source/Car_10.hpp:31 | 0/1 | todo |  |
| 0x4213D0 | `sub_4213D0` | 96 |  | Source/Car_BC.hpp:969 | 0/2 | todo |  |
| 0x421430 | `sub_421430` | 48 |  | Source/Car_BC.hpp:962 | 0/1 | todo |  |
| 0x421460 | `Car_BC::sub_421460` | 8 |  | Source/Car_BC.hpp:1037 | 0/3 | todo |  |
| 0x4214E0 | `sub_4214E0` | 11 |  | Source/Car_BC.hpp:1104 | 0/1 | todo |  |
| 0x4214F0 | `Car_BC::sub_4214F0` | 23 |  | Source/Car_BC.hpp:1280, Source/Car_BC.hpp:1287 | 0/1 | todo |  |
| 0x421530 | `sub_421530` | 8 |  | Source/Car_BC.hpp:385 | 0/1 | todo |  |
| 0x421550 | `sub_421550` | 8 |  | Source/Car_BC.hpp:403 | 0/8 | todo |  |
| 0x421570 | `sub_421570` | 27 |  | Source/Car_BC.hpp:1064 | 0/1 | todo |  |
| 0x421590 | `sub_421590` | 20 |  | Source/Car_BC.hpp:1073 | 0/1 | todo |  |
| 0x4215C0 | `sub_4215C0` | 33 |  | Source/Car_BC.hpp:1178 | 0/2 | todo |  |
| 0x4215F0 | `sub_4215F0` | 32 |  | Source/Car_BC.hpp:1043 | 0/2 | todo |  |
| 0x421610 | `sub_421610` | 11 |  | Source/Car_BC.hpp:457 | 0/1 | todo |  |
| 0x421680 | `inline_check_0x40_info_421680` | 25 |  | Source/Car_BC.hpp:831 | 0/1 | todo |  |
| 0x4216A0 | `sub_4216A0` | 25 |  | Source/Car_BC.hpp:837 | 0/2 | todo |  |
| 0x4216C0 | `Car_BC::inline_check_0x20_info_4216C0` | 25 |  | Source/Car_BC.hpp:825 | 0/2 | todo |  |
| 0x421700 | `inline_check_0x2_info_421700` | 25 |  | Source/Car_BC.hpp:808 | 0/1 | todo |  |
| 0x421760 | `sub_421760` | 22 |  |  | 0/1 | todo |  |
| 0x421780 | `sub_421780` | 11 |  | Source/Car_BC.hpp:1147 | 0/3 | todo |  |
| 0x421790 | `sub_421790` | 11 |  | Source/Car_BC.hpp:1153 | 0/4 | todo |  |
| 0x4217A0 | `sub_4217A0` | 11 |  | Source/Car_BC.hpp:1125 | 0/4 | todo |  |
| 0x4217D0 | `sub_4217D0` | 11 |  | Source/Car_BC.hpp:1163 | 0/1 | todo |  |
| 0x4217E0 | `sub_4217E0` | 11 |  | Source/Car_BC.hpp:1168 | 0/2 | todo |  |
| 0x421810 | `sub_421810` | 24 |  | Source/Car_BC.hpp:843 | 0/1 | todo |  |
| 0x421830 | `Car_BC::sub_421830` | 50 |  |  | 0/1 | todo |  |
| 0x421890 | `sub_421890` | 12 |  | Source/Car_BC.hpp:848, Source/Car_BC.hpp:855 | 0/1 | todo |  |
| 0x4218B0 | `sub_4218B0` | 8 |  | Source/Car_BC.hpp:928 | 0/1 | todo |  |
| 0x4218C0 | `sub_4218C0` | 11 |  | Source/Car_BC.hpp:853 | 0/1 | todo |  |
| 0x4218D0 | `sub_4218D0` | 11 |  | Source/Car_BC.hpp:858 | 0/1 | todo |  |
| 0x4218E0 | `sub_4218E0` | 8 |  | Source/Car_BC.hpp:1058 | 0/2 | todo |  |
| 0x4218F0 | `sub_4218F0` | 10 |  | Source/Car_BC.hpp:1052 | 0/1 | todo |  |
| 0x421910 | `sub_421910` | 55 |  | Source/Car_BC.hpp:1189 | 0/1 | todo |  |
| 0x421950 | `sub_421950` | 13 |  | Source/Car_BC.hpp:463 | 0/2 | todo |  |
| 0x421960 | `sub_421960` | 4 |  | Source/Car_BC.hpp:146 | 0/1 | todo |  |
| 0x421970 | `sub_421970` | 10 |  | Source/Car_BC.hpp:152 | 0/1 | todo |  |
| 0x421980 | `sub_421980` | 11 |  | Source/Player.hpp:119 | 0/6 | todo |  |
| 0x421990 | `sub_421990` | 11 |  | Source/Player.hpp:113 | 0/4 | todo |  |
| 0x4219A0 | `sub_4219A0` | 34 |  |  | 0/1 | todo |  |
| 0x4219E0 | `Game_0x40::get_player_4219E0` | 16 |  | Source/Game_0x40.hpp:108 | 0/7 | todo |  |
| 0x4219F0 | `Garox_2B00::sub_4219F0` | 4 |  |  | 0/1 | todo |  |
| 0x4221D0 | `Car_BC::sub_4221D0` | 84 | 0x43A9A0 MATCH |  | 0/1 | todo |  |
| 0x422260 | `sub_422260` | 53 |  |  | 0/1 | todo |  |
| 0x4226C0 | `sub_4226C0` | 511 |  | Source/Car_BC.cpp:2924 | 0/1 | todo |  |
| 0x422CB0 | `sub_422CB0` | 107 |  |  | 0/1 | todo |  |
| 0x422E00 | `sub_422E00` | 242 |  |  | 0/1 | todo |  |
| 0x422F00 | `sub_422F00` | 442 |  |  | 0/2 | todo |  |
| 0x4230D0 | `sub_4230D0` | 141 |  |  | 0/1 | todo |  |
| 0x423230 | `sub_423230` | 90 |  |  | 0/2 | todo |  |
| 0x4232E0 | `sub_4232E0` | 36 |  |  | 0/1 | todo |  |
| 0x423310 | `sub_423310` | 158 |  |  | 0/1 | todo |  |
| 0x4233B0 | `sub_4233B0` | 208 |  |  | 0/1 | todo |  |
| 0x423590 | `sub_423590` | 53 |  |  | 0/1 | todo |  |
| 0x423830 | `sub_423830` | 27 |  |  | 0/1 | todo |  |
| 0x423850 | `sub_423850` | 35 |  |  | 0/1 | todo |  |
| 0x423A50 | `Car_BC::sub_423A50` | 26 |  |  | 0/2 | todo |  |
| 0x423B50 | `sub_423B50` | 40 |  |  | 0/1 | todo |  |
| 0x423B80 | `sub_423B80` | 162 |  |  | 0/1 | todo |  |
| 0x423C30 | `sub_423C30` | 501 |  |  | 0/1 | todo |  |
| 0x424010 | `sub_424010` | 31 |  |  | 0/1 | todo |  |
| 0x424FF0 | `sub_424FF0` | 948 |  |  | 0/1 | todo |  |
| 0x425450 | `Ped_Unknown_4::ctor_425450` | 12 |  |  | 0/2 | todo |  |
| 0x425460 | `sub_425460` | 27 |  |  | 0/1 | todo |  |
| 0x425480 | `sub_425480` | 28 |  |  | 0/1 | todo |  |
| 0x4254A0 | `sub_4254A0` | 67 |  |  | 0/1 | todo |  |
| 0x425580 | `TrailerPool::sub_425580` | 14 |  | Source/Car_BC.hpp:1415 | 0/1 | todo |  |
| 0x425590 | `sub_425590` | 183 |  | Source/Car_BC.hpp:982 | 0/2 | todo |  |
| 0x425650 | `sub_425650` | 259 |  | Source/Car_BC.hpp:938 | 0/1 | todo |  |
| 0x4257B0 | `sub_4257B0` | 86 |  |  | 0/1 | todo |  |
| 0x425DD0 | `sub_425DD0` | 19 | 0x43BCA0 MATCH |  | 0/1 | todo |  |
| 0x425E30 | `sub_425E30` | 33 |  |  | 0/1 | todo |  |
| 0x425E60 | `sub_425E60` | 33 |  |  | 0/2 | todo |  |
| 0x425ED0 | `sub_425ED0` | 78 |  |  | 0/2 | todo |  |
| 0x425FD0 | `sub_425FD0` | 83 | 0x43D690 WIP |  | 0/1 | todo |  |
| 0x426120 | `sub_426120` | 31 |  |  | 0/3 | todo |  |
| 0x427450 | `sub_427450` | 98 |  |  | 0/2 | todo |  |
| 0x4274C0 | `sub_4274C0` | 1309 |  |  | 0/1 | todo |  |
| 0x428F70 | `sub_428F70` | 36 |  |  | 0/1 | todo |  |
| 0x4298B0 | `Car_BC::sub_4298B0` | 1422 |  |  | 0/1 | todo |  |
| 0x429FF0 | `Car_BC::sub_429FF0` | 279 |  |  | 0/1 | todo |  |
| 0x42A2D0 | `Car_BC_Pool::sub_42A2D0` | 141 |  |  | 0/1 | todo |  |
| 0x42A830 | `gmp_compressed_map_32::sub_42A830` | 19 |  | Source/map_0x370.hpp:130, Source/map_0x370.hpp:582 | 0/14 | todo |  |
| 0x42A9D0 | `sub_42A9D0` | 9 | 0x453BF0 MATCH |  | 0/1 | todo |  |
| 0x42AC40 | `sub_42AC40` | 14 |  | Source/Car_BC.hpp:439 | 0/1 | todo |  |
| 0x42AC50 | `sub_42AC50` | 14 |  | Source/Car_BC.hpp:445 | 0/1 | todo |  |
| 0x42AC60 | `sub_42AC60` | 14 |  | Source/Car_BC.hpp:421 | 0/1 | todo |  |
| 0x42AC70 | `sub_42AC70` | 8 |  | Source/Car_BC.hpp:367 | 0/1 | todo |  |
| 0x42AC80 | `sub_42AC80` | 8 |  | Source/Car_BC.hpp:373 | 0/1 | todo |  |
| 0x42AC90 | `sub_42AC90` | 8 |  | Source/Car_BC.hpp:379 | 0/1 | todo |  |
| 0x42ACA0 | `sub_42ACA0` | 8 |  | Source/Car_BC.hpp:409 | 0/1 | todo |  |
| 0x42ACB0 | `sub_42ACB0` | 8 |  | Source/Car_BC.hpp:415 | 0/1 | todo |  |
| 0x42FE60 | `sub_42FE60` | 18 |  |  | 0/1 | todo |  |
| 0x42FE80 | `sub_42FE80` | 145 |  |  | 0/1 | todo |  |
| 0x432810 | `Monster_808::sub_432810` | 23 |  |  | 0/1 | todo |  |
| 0x432850 | `gtx_0x106C::get_car_info_count_432850` | 10 |  | Source/gtx_0x106C.hpp:355 | 0/3 | todo |  |
| 0x432AA0 | `sub_432AA0` | 442 |  |  | 0/1 | todo |  |
| 0x432C60 | `sub_432C60` | 49 |  |  | 0/1 | todo |  |
| 0x432F70 | `Monster_2C::gdtor_432F70` | 77 |  |  | 0/1 | todo |  |
| 0x433120 | `sub_433120` | 8 |  |  | 0/1 | todo |  |
| 0x433160 | `sub_433160` | 40 |  |  | 0/1 | todo |  |
| 0x433360 | `sub_433360` | 13 |  | Source/PedGroup.hpp:62 | 0/6 | todo |  |
| 0x433820 | `sub_433820` | 11 |  | Source/Weapon_8.hpp:15 | 0/3 | todo |  |
| 0x433880 | `Char_B4::sub_433880` | 75 |  |  | 0/1 | todo |  |
| 0x4338D0 | `sub_4338D0` | 7 |  | Source/char.hpp:54 | 0/1 | todo |  |
| 0x4338E0 | `Char_B4::sub_4338E0` | 10 |  | Source/char.hpp:60 | 0/1 | todo |  |
| 0x433A40 | `sub_433A40` | 14 |  | Source/char.hpp:160 | 0/2 | todo |  |
| 0x433A90 | `Char_B4::sub_433A90` | 4 |  | Source/char.hpp:265 | 0/4 | todo |  |
| 0x433B30 | `Zone_144::sub_433B30` | 7 |  | Source/Gang.hpp:18 | 0/1 | todo |  |
| 0x433BB0 | `sub_433BB0` | 13 |  | Source/Ped.hpp:688 | 0/2 | todo |  |
| 0x433BE0 | `cool_nash_0x294::sub_433BE0` | 11 |  | Source/Ped.hpp:728 | 0/3 | todo |  |
| 0x433C50 | `sub_433C50` | 13 |  | Source/Ped.hpp:641 | 0/4 | todo |  |
| 0x433C60 | `sub_433C60` | 13 |  | Source/Ped.hpp:646 | 0/4 | todo |  |
| 0x433C70 | `sub_433C70` | 13 |  | Source/Ped.hpp:651 | 0/4 | todo |  |
| 0x433C90 | `sub_433C90` | 11 |  | Source/Ped.hpp:32 | 0/1 | todo |  |
| 0x433DC0 | `cool_nash_0x294::get_wanted_points_433DC0` | 8 | 0x592370 MATCH |  | 0/1 | todo |  |
| 0x433E90 | `sub_433E90` | 20 |  | Source/Camera.hpp:162 | 0/2 | todo |  |
| 0x433EB0 | `sub_433EB0` | 135 |  |  | 0/1 | todo |  |
| 0x433F40 | `sub_433F40` | 156 |  |  | 0/1 | todo |  |
| 0x433FE0 | `sub_433FE0` | 106 |  |  | 0/1 | todo |  |
| 0x434380 | `sub_434380` | 25 |  |  | 0/1 | todo |  |
| 0x434900 | `angry_lewin_0x85C::get_camera_434900` | 26 |  | Source/Player.hpp:324 | 0/4 | todo |  |
| 0x434920 | `sub_434920` | 19 |  | Source/Player.hpp:75 | 0/1 | todo |  |
| 0x434940 | `sub_434940` | 15 |  | Source/Player.hpp:81 | 0/1 | todo |  |
| 0x434960 | `sub_434960` | 11 |  | Source/TrafficLights_194.hpp:33 | 0/5 | todo |  |
| 0x434970 | `sub_434970` | 7 |  | Source/Taxi_4.hpp:55 | 0/1 | todo |  |
| 0x434D60 | `cool_nash_0x294::sub_434D60` | 15 |  |  | 0/1 | todo |  |
| 0x4354C0 | `sub_4354C0` | 46 |  |  | 0/1 | todo |  |
| 0x435530 | `PedPool::sub_435530` | 4 |  | Source/Char_Pool.hpp:126 | 0/1 | todo |  |
| 0x4355A0 | `Char_B4_Pool::sub_4355A0` | 19 |  |  | 0/1 | todo |  |
| 0x4355C0 | `Char_B4_Pool::DeAllocate` | 27 |  |  | 0/2 | todo |  |
| 0x435C40 | `cool_nash_0x294::sub_435C40` | 51 |  |  | 0/2 | todo |  |
| 0x435F40 | `cool_nash_0x294::sub_435F40` | 34 |  |  | 0/1 | todo |  |
| 0x436040 | `cool_nash_0x294::sub_436040` | 33 |  |  | 0/1 | todo |  |
| 0x436070 | `sub_436070` | 72 | 0x470F30 MATCH |  | 0/1 | todo |  |
| 0x436160 | `sub_436160` | 73 | 0x45C920 MATCH |  | 0/1 | todo |  |
| 0x436920 | `cool_nash_0x294::sub_436920` | 610 | 0x463830 WIP |  | 0/1 | todo |  |
| 0x4388E0 | `sub_4388E0` | 334 |  |  | 0/1 | todo |  |
| 0x439640 | `sub_439640` | 319 |  |  | 0/1 | todo |  |
| 0x43BEE0 | `sub_43BEE0` | 120 |  |  | 0/1 | todo |  |
| 0x43BF60 | `sub_43BF60` | 169 |  |  | 0/1 | todo |  |
| 0x43DB20 | `PedPool::gdtor_43DB20` | 30 |  |  | 0/1 | todo |  |
| 0x43E1E0 | `sub_43E1E0` | 131 |  |  | 0/1 | todo |  |
| 0x441A30 | `sub_441A30` | 1075 |  |  | 0/1 | todo |  |
| 0x4427E0 | `sub_4427E0` | 576 |  |  | 0/1 | todo |  |
| 0x445EC0 | `TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` | 34 | 0x445EC0 WIP |  | 0/1 | todo |  |
| 0x445EF0 | `sub_445EF0` | 8 |  | Source/Char_Pool.hpp:67 | 0/3 | todo |  |
| 0x445F00 | `Char_8_Pool::sub_445F00` | 14 |  | Source/Char_Pool.hpp:73 | 0/6 | todo |  |
| 0x446820 | `PurpleDoom::sub_446820` | 54 |  | Source/PurpleDoom.hpp:31 | 0/1 | todo |  |
| 0x4468C0 | `Collide_8_Pool::ctor_4468C0` | 41 |  |  | 0/1 | todo |  |
| 0x4468F0 | `PurpleDoom_C_Pool::ctor_4468F0` | 41 |  |  | 0/1 | todo |  |
| 0x446920 | `sub_446920` | 10 |  | Source/sprite.hpp:44 | 0/4 | todo |  |
| 0x446930 | `CanAllocateOfType_446930` | 15 |  | Source/Car_BC.hpp:202, Source/sprite.hpp:49 | 0/4 | todo |  |
| 0x446940 | `sub_446940` | 15 |  | Source/sprite.hpp:277 | 0/4 | todo |  |
| 0x446950 | `sub_446950` | 8 |  | Source/sprite.hpp:344 | 0/2 | todo |  |
| 0x446960 | `sub_446960` | 92 |  | Source/sprite.hpp:338 | 0/1 | todo |  |
| 0x446AA0 | `sub_446AA0` | 32 |  |  | 0/1 | todo |  |
| 0x4472F0 | `PurpleDoom::gdtor_4472F0` | 26 |  |  | 0/1 | todo |  |
| 0x447310 | `Collide_8_Pool::gdtor_447310` | 30 |  |  | 0/1 | todo |  |
| 0x447330 | `PurpleDoom_C_Pool::gdtor_447330` | 30 |  |  | 0/1 | todo |  |
| 0x447350 | `sub_447350` | 8 |  |  | 0/2 | todo |  |
| 0x447360 | `sub_447360` | 14 |  | Source/Car_BC.hpp:675 | 0/2 | todo |  |
| 0x447370 | `sub_447370` | 8 |  |  | 0/2 | todo |  |
| 0x447380 | `sub_447380` | 14 |  |  | 0/2 | todo |  |
| 0x447390 | `PurpleDoom::sub_447390` | 233 |  |  | 0/1 | todo |  |
| 0x447BD0 | `sub_447BD0` | 39 | 0x477B00 MATCH |  | 0/1 | todo |  |
| 0x447C40 | `PurpleDoom::sub_447C40` | 57 | 0x477B60 MATCH |  | 0/1 | todo |  |
| 0x447DF0 | `sub_447DF0` | 23 |  | Source/sprite.hpp:126 | 0/2 | todo |  |
| 0x447E90 | `sub_447E90` | 10 |  | Source/Object_5C.hpp:182 | 0/5 | todo |  |
| 0x447EA0 | `sub_447EA0` | 11 |  | Source/Car_BC.hpp:1109 | 0/1 | todo |  |
| 0x447F40 | `sub_447F40` | 19 |  | Source/Cranes.hpp:71 | 0/1 | todo |  |
| 0x447F60 | `Zheal_15C::sub_447F60` | 13 |  | Source/Cranes.hpp:15 | 0/1 | todo |  |
| 0x447F90 | `sub_447F90` | 72 |  |  | 0/2 | todo |  |
| 0x448030 | `Zheal_15C::sub_448030` | 83 |  | Source/Cranes.cpp:120 | 0/1 | todo |  |
| 0x4495F0 | `sub_4495F0` | 236 |  |  | 0/1 | todo |  |
| 0x4496E0 | `sub_4496E0` | 236 |  | Source/Cranes.cpp:952 | 0/1 | todo |  |
| 0x44A470 | `sub_44A470` | 669 |  |  | 0/1 | todo |  |
| 0x44A720 | `sub_44A720` | 583 |  |  | 0/1 | todo |  |
| 0x44AA60 | `sub_44AA60` | 19 |  | Source/BurgerKing_67F8B0.hpp:91 | 0/1 | todo |  |
| 0x44AA80 | `sub_44AA80` | 16 |  | Source/BurgerKing_67F8B0.hpp:107 | 0/1 | todo |  |
| 0x44AF70 | `sub_44AF70` | 7 |  | Source/sprite.hpp:609 | 0/1 | todo |  |
| 0x44AF90 | `sub_44AF90` | 19 |  |  | 0/1 | todo |  |
| 0x44B890 | `Montana::sub_44B890` | 23 |  |  | 0/1 | todo |  |
| 0x44B970 | `Montana_4::gdtor_44B970` | 30 |  |  | 0/1 | todo |  |
| 0x44C800 | `Door_10_Pool::ctor_44C800` | 41 |  |  | 0/1 | todo |  |
| 0x44C830 | `Door_10_Pool::sub_44C830` | 14 |  |  | 0/1 | todo |  |
| 0x44C860 | `sub_44C860` | 9 |  | Source/Door_38.hpp:76 | 0/1 | todo |  |
| 0x44C870 | `sub_44C870` | 35 |  | Source/Garage_48.hpp:21 | 0/1 | todo |  |
| 0x44CC30 | `sub_44CC30` | 389 | 0x49CC00 MATCH |  | 0/1 | todo |  |
| 0x44D840 | `sub_44D840` | 687 |  |  | 0/1 | todo |  |
| 0x44DBF0 | `sub_44DBF0` | 19 |  |  | 0/0 | todo |  |
| 0x450530 | `FatalError_450530` | 435 | 0x4A38C0 MATCH |  | 0/2 | todo |  |
| 0x450CC0 | `sub_450CC0` | 11 |  |  | 0/1 | todo |  |
| 0x451510 | `sub_451510` | 141 |  |  | 0/1 | todo |  |
| 0x451F70 | `sharp_pare_0x15D8::gdtor_451F70` | 30 |  |  | 0/1 | todo |  |
| 0x451F90 | `gtx_0x106C::gdtor_451F90` | 30 |  |  | 0/1 | todo |  |
| 0x452E60 | `LangIsJapanese_452E60` | 8 |  | Source/text_0x14.hpp:60 | 0/2 | todo |  |
| 0x453480 | `sub_453480` | 268 |  |  | 0/1 | todo |  |
| 0x45A8D0 | `Game_0x40::sub_45A8D0` | 64 | 0x4B9CD0 MATCH |  | 0/1 | todo |  |
| 0x45A910 | `Game_0x40::sub_45A910` | 67 | 0x4B9D10 MATCH |  | 0/1 | todo |  |
| 0x45A960 | `rng::ctor_45A960` | 16 |  |  | 0/1 | todo |  |
| 0x45A970 | `angry_lewin_0x85C::dtor_45A970` | 30 |  |  | 0/1 | todo |  |
| 0x45A990 | `Map_0x370::gdtor_45A990` | 30 |  |  | 0/1 | todo |  |
| 0x45A9B0 | `Montana::gdtor_45A9B0` | 30 |  |  | 0/1 | todo |  |
| 0x45A9F0 | `frosty_pasteur_0xC1EA8::gdtor_45A9F0` | 30 |  |  | 0/1 | todo |  |
| 0x45AA10 | `Object_5C::gdtor_45AA10` | 30 |  |  | 0/1 | todo |  |
| 0x45AA30 | `PedManager::gdtor_45AA30` | 30 |  |  | 0/1 | todo |  |
| 0x45AA50 | `sharp_bose_0x54::gdtor_45AA50` | 30 |  |  | 0/1 | todo |  |
| 0x45AA70 | `Sprite_8::gdtor_45AA70` | 30 |  |  | 0/1 | todo |  |
| 0x45AA90 | `Collide_C::gdtor_45AA90` | 30 |  |  | 0/1 | todo |  |
| 0x45AAB0 | `Varrok_7F8::gdtor_45AAB0` | 30 |  |  | 0/1 | todo |  |
| 0x45AAD0 | `Sero_181C::gdtor_45AAD0` | 30 |  |  | 0/1 | todo |  |
| 0x45AAF0 | `Taxi_4::gdtor_45AAF0` | 30 |  |  | 0/1 | todo |  |
| 0x45AB10 | `TileAnim_2::gdtor_45AB10` | 30 |  |  | 0/1 | todo |  |
| 0x45AB30 | `Weapon_8::gdtor_45AB30` | 30 |  |  | 0/1 | todo |  |
| 0x45AB50 | `Door_4D4::gdtor_45AB50` | 30 |  |  | 0/1 | todo |  |
| 0x45AB70 | `TrafficLights_194::gdtor_45AB70` | 30 |  |  | 0/1 | todo |  |
| 0x45AB90 | `Monster_808::gdtor_45AB90` | 30 |  |  | 0/1 | todo |  |
| 0x45ABB0 | `Particle_8::gdtor_45ABB0` | 30 |  |  | 0/1 | todo |  |
| 0x45ABD0 | `Wolfy_3D4::gdtor_45ABD0` | 30 |  |  | 0/1 | todo |  |
| 0x45ABF0 | `Wolfy_7A8::gdtor_45ABF0` | 30 |  |  | 0/1 | todo |  |
| 0x45AC10 | `Hamburger_500::dtor_45AC10` | 30 |  |  | 0/1 | todo |  |
| 0x45AC30 | `CokeZero_100::dtor_45AC30` | 30 |  |  | 0/1 | todo |  |
| 0x45AC50 | `Shooey_CC::gdtor_45AC50` | 30 |  |  | 0/1 | todo |  |
| 0x45AC70 | `Rozza_C88::gdtor_45AC70` | 30 |  |  | 0/1 | todo |  |
| 0x45AC90 | `magical_germain_0x8EC::gdtor_45AC90` | 30 |  |  | 0/1 | todo |  |
| 0x45ACE0 | `Game_0x40::sub_45ACE0` | 3 |  |  | 0/1 | todo |  |
| 0x45ACF0 | `Game_0x40::sub_45ACF0` | 3 |  |  | 0/1 | todo |  |
| 0x45AD00 | `Light_1D4CC::sub_45AD00` | 35 |  |  | 0/1 | todo |  |
| 0x45AD30 | `Car_BC_Pool::get_cars_count_45AD30` | 8 |  | Source/Car_BC.hpp:1381 | 0/1 | todo |  |
| 0x45AE60 | `Zones_CA8::ctor_45AE60` | 30 |  |  | 0/1 | todo |  |
| 0x45AEA0 | `DrawUnk_0xBC::sub_45AEA0` | 160 |  | Source/Camera.hpp:111 | 0/1 | todo |  |
| 0x45AF40 | `DrawUnk_0xBC::sub_45AF40` | 139 |  | Source/Camera.hpp:120 | 0/1 | todo |  |
| 0x45B040 | `Nanobotz::ResetCount_45B040` | 11 |  | Source/MapRenderer.hpp:157 | 0/1 | todo |  |
| 0x45B050 | `Nanobotz::ctor_45B050` | 69 | 0x4BE650 MATCH |  | 0/1 | todo |  |
| 0x45B0A0 | `eager_benz::get_accuracy_count_45B0A0` | 7 |  | Source/eager_benz.hpp:20 | 0/1 | todo |  |
| 0x45B0B0 | `eager_benz::get_reverse_count_45B0B0` | 7 |  | Source/eager_benz.hpp:26 | 0/1 | todo |  |
| 0x45B0C0 | `angry_lewin_0x85C::sub_45B0C0` | 12 |  | Source/Player.hpp:51 | 0/1 | todo |  |
| 0x45B0D0 | `angry_lewin_0x85C::sub_45B0D0` | 4 |  |  | 0/1 | todo |  |
| 0x45B1A0 | `Kfc_1E0::ctor_45B1A0` | 82 |  |  | 0/1 | todo |  |
| 0x45B2D0 | `nostalgic_ellis_0x28::sub_45B2D0` | 21 |  | Source/Light_1D4CC.hpp:58, Source/Light_1D4CC.hpp:71 | 0/1 | todo |  |
| 0x45B3D0 | `Light_1D4CC::ctor_45B3D0` | 65 |  |  | 0/1 | todo |  |
| 0x45B420 | `lucid_hamilton::sub_45B420` | 11 |  | Source/lucid_hamilton.hpp:92 | 0/1 | todo |  |
| 0x45B440 | `Tango_54::ctor_45B440` | 29 |  |  | 0/1 | todo |  |
| 0x45BDC0 | `Phi_8CA8::gdtor_45BDC0` | 30 |  |  | 0/1 | todo |  |
| 0x45BDE0 | `jawwie_110::gdtor_45BDE0` | 30 |  |  | 0/1 | todo |  |
| 0x45BE00 | `Marz_1D7E::gdtor_45BE00` | 30 |  |  | 0/1 | todo |  |
| 0x45BE20 | `Orca_2FD4::gdtor_45BE20` | 26 |  |  | 0/1 | todo |  |
| 0x45BE40 | `Zheal_D9C::gdtor_45BE40` | 30 |  |  | 0/1 | todo |  |
| 0x45BE60 | `Snooky_94::gdtor_45BE60` | 30 |  |  | 0/1 | todo |  |
| 0x45BE80 | `Kfc_1E0::gdtor_45BE80` | 30 |  |  | 0/1 | todo |  |
| 0x45BEA0 | `Police_7B8::gdtor_45BEA0` | 30 |  |  | 0/1 | todo |  |
| 0x45BEC0 | `Light_1D4CC::gdtor_45BEC0` | 30 |  | Source/Ped.hpp:166 | 0/1 | todo |  |
| 0x45BEE0 | `Zones_CA8::gdtor_45BEE0` | 30 |  |  | 0/1 | todo |  |
| 0x45BF00 | `Tango_54::gdtor_45BF00` | 26 |  |  | 0/1 | todo |  |
| 0x45BFE0 | `Frismo_C_Pool::ctor_45BFE0` | 63 |  |  | 0/1 | todo |  |
| 0x45C040 | `Mike_A80::ctor_45C040` | 103 |  |  | 0/1 | todo |  |
| 0x45C0D0 | `jawwie_110::ctor_45C0D0` | 96 |  |  | 0/1 | todo |  |
| 0x45C150 | `Police_7B8::ctor_45C150` | 138 |  |  | 0/1 | todo |  |
| 0x45C1E0 | `Light_1D4CC::sub_45C1E0` | 5 |  | Source/Light_1D4CC.hpp:21 | 0/1 | todo |  |
| 0x45D350 | `Frismo_C_Pool::gdtor_45D350` | 30 |  |  | 0/1 | todo |  |
| 0x45D370 | `ChickenLegend_48::dtor_45D370` | 26 |  |  | 0/1 | todo |  |
| 0x45D3B0 | `Garox_2B00::gdtor_45D3B0` | 30 |  |  | 0/1 | todo |  |
| 0x45D960 | `Zone_144::init_45D960` | 113 | 0x4BED70 MATCH |  | 0/1 | todo |  |
| 0x45E0A0 | `sub_45E0A0` | 10 |  | Source/Object_5C.hpp:182 | 0/3 | todo |  |
| 0x45E240 | `sub_45E240` | 36 |  |  | 0/1 | todo |  |
| 0x45E4F0 | `lucid_hamilton::sub_45E4F0` | 7 | 0x4C5950 MATCH |  | 0/2 | todo |  |
| 0x45E770 | `lucid_hamilton::sub_45E770` | 39 | 0x4C5C30 MATCH |  | 0/1 | todo |  |
| 0x45E8D0 | `sub_45E8D0` | 837 |  |  | 0/1 | todo |  |
| 0x45ED00 | `sub_45ED00` | 8 |  |  | 0/1 | todo |  |
| 0x45EDE0 | `IsField238_45EDE0` | 217 |  | Source/Car_BC.hpp:1282, Source/Ped.hpp:205 | 0/1 | todo |  |
| 0x45F9E0 | `sub_45F9E0` | 4 |  | Source/rng.hpp:20 | 0/1 | todo |  |
| 0x460EC0 | `chunk::verify_version_460EC0` | 31 |  |  | 0/3 | todo |  |
| 0x460EE0 | `chunk::verify_type_460EE0` | 40 |  |  | 0/3 | todo |  |
| 0x461250 | `keybrd_0x204::gdtor_461250` | 26 |  |  | 0/1 | todo |  |
| 0x461500 | `sub_461500` | 17 | 0x4D9670 MATCH |  | 0/1 | todo |  |
| 0x461910 | `sub_461910` | 30 |  |  | 0/1 | todo |  |
| 0x461DA0 | `sub_461DA0` | 7 |  | Source/NetPlay.hpp:245 | 0/1 | todo |  |
| 0x461DB0 | `sub_461DB0` | 7 |  | Source/Player.hpp:175 | 0/3 | todo |  |
| 0x461DD0 | `sub_461DD0` | 12 |  | Source/youthful_einstein.hpp:36 | 0/2 | todo |  |
| 0x462FB0 | `gtx_0x106C::sub_462FB0` | 20 |  | Source/gtx_0x106C.hpp:198 | 0/1 | todo |  |
| 0x462FD0 | `gtx_0x106C::sub_462FD0` | 26 |  | Source/gtx_0x106C.hpp:204 | 0/4 | todo |  |
| 0x463020 | `gmp_map_zone::sub_463020` | 88 |  | Source/map_0x370.hpp:179 | 0/3 | todo |  |
| 0x463080 | `sub_463080` | 23 |  | Source/map_0x370.hpp:138 | 0/1 | todo |  |
| 0x4630A0 | `sub_4630A0` | 13 |  | Source/map_0x370.hpp:153 | 0/1 | todo |  |
| 0x4630B0 | `sub_4630B0` | 29 |  | Source/map_0x370.hpp:110 | 0/1 | todo |  |
| 0x463530 | `sub_463530` | 53 |  |  | 0/1 | todo |  |
| 0x463710 | `sub_463710` | 70 |  | Source/Fix16_Rect.hpp:77 | 0/2 | todo |  |
| 0x4637A0 | `sub_4637A0` | 12 |  | Source/Fix16_Rect.hpp:153 | 0/1 | todo |  |
| 0x463F10 | `sub_463F10` | 55 |  | Source/Light_1D4CC.hpp:70, Source/nostalgic_ellis_0x28.hpp:14 | 0/1 | todo |  |
| 0x464C40 | `sub_464C40` | 26 |  | Source/Light_1D4CC.hpp:43, Source/Light_1D4CC.hpp:64 | 0/1 | todo |  |
| 0x465090 | `Map_0x370::sub_465090` | 151 |  |  | 0/1 | todo |  |
| 0x4653C0 | `sub_4653C0` | 80 | 0x4DFE10 MATCH |  | 0/1 | todo |  |
| 0x469070 | `Light_1D4CC::sub_469070` | 54 |  | Source/Light_1D4CC.hpp:52 | 0/1 | todo |  |
| 0x4692B0 | `Map_0x370::sub_4692B0` | 237 |  |  | 0/1 | todo |  |
| 0x46A4D0 | `Map_0x370::sub_46A4D0` | 155 |  |  | 0/1 | todo |  |
| 0x46B620 | `Nanobotz::set_shading_lev_46B620` | 233 | 0x4E9DB0 WIP |  | 0/1 | todo |  |
| 0x46E490 | `Nanobotz::sub_46E490` | 297 |  |  | 0/1 | todo |  |
| 0x46E5C0 | `Nanobotz::sub_46E5C0` | 281 |  |  | 0/1 | todo |  |
| 0x46E6E0 | `Nanobotz::sub_46E6E0` | 281 |  |  | 0/1 | todo |  |
| 0x46E800 | `Nanobotz::sub_46E800` | 265 |  |  | 0/1 | todo |  |
| 0x46E910 | `Nanobotz::sub_46E910` | 282 |  |  | 0/1 | todo |  |
| 0x46EA30 | `Nanobotz::sub_46EA30` | 298 |  |  | 0/1 | todo |  |
| 0x46EB60 | `Nanobotz::SpawnDummies_46EB60` | 298 |  | Source/Char_Pool.hpp:88 | 0/1 | todo |  |
| 0x46EC90 | `Nanobotz::sub_46EC90` | 314 |  |  | 0/1 | todo |  |
| 0x46EF10 | `Nanobotz::sub_46EF10` | 1112 |  |  | 0/1 | todo |  |
| 0x46F370 | `Nanobotz::sub_46F370` | 1096 |  |  | 0/1 | todo |  |
| 0x46F7C0 | `Nanobotz::sub_46F7C0` | 1099 |  |  | 0/1 | todo |  |
| 0x46FC10 | `Nanobotz::sub_46FC10` | 1102 |  |  | 0/1 | todo |  |
| 0x472110 | `Nanobotz::Draw_472110` | 1108 |  |  | 0/1 | todo |  |
| 0x4725B0 | `sub_4725B0` | 87 | 0x4F7940 MATCH |  | 0/1 | todo |  |
| 0x4727E0 | `sub_4727E0` | 362 |  |  | 0/1 | todo |  |
| 0x472950 | `sub_472950` | 389 |  |  | 0/1 | todo |  |
| 0x472AE0 | `sub_472AE0` | 281 |  |  | 0/1 | todo |  |
| 0x472C00 | `sub_472C00` | 96 | 0x4F78F0 MATCH |  | 0/1 | todo |  |
| 0x474430 | `sub_474430` | 17 |  | Source/Mike_A80.hpp:64 | 0/1 | todo |  |
| 0x474450 | `sub_474450` | 55 |  | Source/Mike_A80.hpp:36 | 0/1 | todo |  |
| 0x4754D0 | `sub_4754D0` | 9 |  | Source/fix16.hpp:363 | 0/1 | todo |  |
| 0x4758A0 | `sub_4758A0` | 13 |  | Source/Gang.hpp:24 | 0/1 | todo |  |
| 0x4758B0 | `sub_4758B0` | 13 |  | Source/Gang.hpp:30 | 0/1 | todo |  |
| 0x4758C0 | `sub_4758C0` | 13 |  | Source/Gang.hpp:36 | 0/1 | todo |  |
| 0x4758D0 | `sub_4758D0` | 13 |  | Source/Gang.hpp:42 | 0/1 | todo |  |
| 0x4758E0 | `sub_4758E0` | 13 |  | Source/Gang.hpp:48 | 0/1 | todo |  |
| 0x4758F0 | `sub_4758F0` | 13 |  | Source/Gang.hpp:54 | 0/1 | todo |  |
| 0x475900 | `sub_475900` | 13 |  | Source/Gang.hpp:60 | 0/2 | todo |  |
| 0x475910 | `sub_475910` | 33 |  | Source/Gang.hpp:66 | 0/1 | todo |  |
| 0x475940 | `sub_475940` | 13 |  | Source/Gang.hpp:74 | 0/1 | todo |  |
| 0x475950 | `sub_475950` | 13 |  | Source/Gang.hpp:80 | 0/1 | todo |  |
| 0x475980 | `sub_475980` | 19 |  | Source/frosty_pasteur_0xC1EA8.hpp:611 | 0/1 | todo |  |
| 0x4759A0 | `sub_4759A0` | 19 |  | Source/frosty_pasteur_0xC1EA8.hpp:617 | 0/4 | todo |  |
| 0x4759C0 | `sub_4759C0` | 19 |  | Source/frosty_pasteur_0xC1EA8.hpp:623 | 0/4 | todo |  |
| 0x475A20 | `sub_475A20` | 7 |  | Source/frosty_pasteur_0xC1EA8.hpp:629 | 0/2 | todo |  |
| 0x475A30 | `sub_475A30` | 7 |  | Source/frosty_pasteur_0xC1EA8.hpp:635 | 0/1 | todo |  |
| 0x475A40 | `sub_475A40` | 13 |  | Source/frosty_pasteur_0xC1EA8.hpp:641 | 0/2 | todo |  |
| 0x475A50 | `sub_475A50` | 10 |  |  | 0/2 | todo |  |
| 0x475A60 | `sub_475A60` | 11 |  | Source/Object_5C.hpp:130 | 0/1 | todo |  |
| 0x475A70 | `sub_475A70` | 11 |  | Source/Object_5C.hpp:124 | 0/1 | todo |  |
| 0x475A80 | `sub_475A80` | 32 |  | Source/Object_5C.hpp:137 | 0/2 | todo |  |
| 0x475AA0 | `sub_475AA0` | 51 |  | Source/Object_5C.hpp:212 | 0/1 | todo |  |
| 0x475AE0 | `sub_475AE0` | 10 |  |  | 0/1 | todo |  |
| 0x475AF0 | `sub_475AF0` | 7 |  | Source/Ped.hpp:828 | 0/2 | todo |  |
| 0x475B10 | `sub_475B10` | 7 |  | Source/Ped.hpp:137 | 0/2 | todo |  |
| 0x475B40 | `sub_475B40` | 11 |  | Source/Ped.hpp:131 | 0/1 | todo |  |
| 0x475B60 | `DrawUnk_0xBC::sub_475B60` | 8 |  | Source/Camera.hpp:87 | 0/7 | todo |  |
| 0x475B70 | `miss2_0x11C::sub_475B70` | 38 | 0x511930 MATCH |  | 0/1 | todo |  |
| 0x475C40 | `sub_475C40` | 20 |  | Source/Car_BC.hpp:884 | 0/1 | todo |  |
| 0x475C80 | `sub_475C80` | 20 |  | Source/Car_BC.hpp:899 | 0/1 | todo |  |
| 0x475CA0 | `sub_475CA0` | 138 |  |  | 0/1 | todo |  |
| 0x475E60 | `HasOtherCarOnTrailer_475E60` | 34 |  | Source/Car_BC.hpp:714 | 0/1 | todo |  |
| 0x476200 | `frosty_pasteur_0xC1EA8::sub_476200` | 42 | 0x512770 MATCH |  | 0/2 | todo |  |
| 0x476270 | `sub_476270` | 8 |  | Source/Car_BC.hpp:893 | 0/2 | todo |  |
| 0x476360 | `sub_476360` | 8 |  | Source/Car_BC.hpp:878 | 0/1 | todo |  |
| 0x4764A0 | `sub_4764A0` | 39 |  | Source/Car_BC.hpp:220 | 0/1 | todo |  |
| 0x476610 | `sub_476610` | 18 |  |  | 0/1 | todo |  |
| 0x476660 | `sub_476660` | 22 |  | Source/sad_mirzakhani.hpp:40 | 0/2 | todo |  |
| 0x476680 | `sub_476680` | 26 |  | Source/sad_mirzakhani.hpp:46 | 0/1 | todo |  |
| 0x4766B0 | `sub_4766B0` | 11 |  | Source/Player.hpp:87 | 0/3 | todo |  |
| 0x4766C0 | `sub_4766C0` | 11 |  | Source/Player.hpp:107 | 0/2 | todo |  |
| 0x476700 | `sub_476700` | 40 |  | Source/Player.hpp:93 | 0/1 | todo |  |
| 0x476730 | `sub_476730` | 40 |  | Source/Player.hpp:100 | 0/1 | todo |  |
| 0x476780 | `Frismo_C_Pool::sub_476780` | 14 |  | Source/Frismo_25C.hpp:35 | 0/1 | todo |  |
| 0x476790 | `sub_476790` | 4 |  | Source/Game_0x40.hpp:57 | 0/1 | todo |  |
| 0x4767A0 | `miss2_0x11C_Pool::sub_4767A0` | 24 |  | Source/miss2_0x11C.hpp:1176 | 0/2 | todo |  |
| 0x4767C0 | `sub_4767C0` | 31 |  | Source/Hud.hpp:436 | 0/1 | todo |  |
| 0x476830 | `sub_476830` | 8 |  |  | 0/1 | todo |  |
| 0x476840 | `sub_476840` | 8 |  | Source/Hud.hpp:572 | 0/2 | todo |  |
| 0x476850 | `sub_476850` | 8 |  | Source/Hud.hpp:554 | 0/1 | todo |  |
| 0x476860 | `sub_476860` | 8 |  | Source/Hud.hpp:560 | 0/2 | todo |  |
| 0x476870 | `sub_476870` | 8 |  | Source/Hud.hpp:566 | 0/1 | todo |  |
| 0x476880 | `sub_476880` | 20 |  |  | 0/1 | todo |  |
| 0x4768C0 | `sub_4768C0` | 23 |  | Source/Car_BC.hpp:102 | 0/2 | todo |  |
| 0x4768E0 | `sub_4768E0` | 32 |  | Source/Cranes.hpp:60 | 0/1 | todo |  |
| 0x476900 | `sub_476900` | 10 |  | Source/Cranes.hpp:21 | 0/1 | todo |  |
| 0x476930 | `sub_476930` | 21 |  |  | 0/1 | todo |  |
| 0x476950 | `sub_476950` | 9 |  |  | 0/1 | todo |  |
| 0x476990 | `sub_476990` | 10 |  | Source/Door_38.hpp:92 | 0/2 | todo |  |
| 0x4769A0 | `sub_4769A0` | 21 |  |  | 0/1 | todo |  |
| 0x4769C0 | `sub_4769C0` | 21 |  |  | 0/2 | todo |  |
| 0x4769E0 | `sub_4769E0` | 21 |  |  | 0/2 | todo |  |
| 0x476A00 | `sub_476A00` | 14 |  |  | 0/2 | todo |  |
| 0x476A10 | `sub_476A10` | 10 |  |  | 0/2 | todo |  |
| 0x476A20 | `sub_476A20` | 16 |  | Source/miss2_0x11C.cpp:993, Source/miss2_0x11C.cpp:1120 | 0/2 | todo |  |
| 0x476A90 | `sub_476A90` | 4 |  | Source/Door_38.hpp:54 | 0/1 | todo |  |
| 0x476AE0 | `sub_476AE0` | 22 |  | Source/nostalgic_ellis_0x28.hpp:88 | 0/1 | todo |  |
| 0x476B10 | `sub_476B10` | 13 |  | Source/lucid_hamilton.hpp:26 | 0/1 | todo |  |
| 0x476D20 | `sub_476D20` | 44 |  |  | 0/1 | todo |  |
| 0x476DF0 | `sub_476DF0` | 11 |  |  | 0/1 | todo |  |
| 0x476E00 | `sub_476E00` | 11 |  |  | 0/1 | todo |  |
| 0x477530 | `sub_477530` | 36 |  |  | 0/1 | todo |  |
| 0x477870 | `sub_477870` | 163 |  |  | 0/1 | todo |  |
| 0x478120 | `sub_478120` | 72 |  |  | 0/1 | todo |  |
| 0x478170 | `sub_478170` | 131 |  |  | 0/1 | todo |  |
| 0x4784A0 | `sub_4784A0` | 35 |  |  | 0/1 | todo |  |
| 0x479850 | `sub_479850` | 384 |  |  | 0/1 | todo |  |
| 0x479F10 | `sub_479F10` | 686 |  |  | 0/1 | todo |  |
| 0x47B5E0 | `sub_47B5E0` | 304 |  |  | 0/1 | todo |  |
| 0x47B810 | `sub_47B810` | 310 |  |  | 0/1 | todo |  |
| 0x47BCD0 | `sub_47BCD0` | 304 |  |  | 0/1 | todo |  |
| 0x47BE00 | `sub_47BE00` | 139 |  |  | 0/1 | todo |  |
| 0x47BF60 | `miss2_8::gdtor_47BF60` | 30 |  |  | 0/1 | todo |  |
| 0x47D070 | `sub_47D070` | 251 |  |  | 0/1 | todo |  |
| 0x47DE70 | `sub_47DE70` | 456 |  |  | 0/1 | todo |  |
| 0x47E360 | `sub_47E360` | 592 |  |  | 0/1 | todo |  |
| 0x47ECB0 | `sub_47ECB0` | 36 |  |  | 0/1 | todo |  |
| 0x47EF10 | `sub_47EF10` | 42 |  |  | 0/1 | todo |  |
| 0x47F420 | `sub_47F420` | 34 | 0x512AA0 MATCH |  | 0/1 | todo |  |
| 0x481310 | `miss2_0x11C_Pool::ctor_481310` | 111 |  | Source/miss2_0x11C.hpp:1186 | 0/1 | todo |  |
| 0x481900 | `frosty_pasteur_0xC1EA8::sub_481900` | 85 |  |  | 0/1 | todo |  |
| 0x481C10 | `miss2_0x11C_Pool::gdtor_481C10` | 30 |  |  | 0/1 | todo |  |
| 0x481D10 | `sub_481D10` | 18 |  |  | 0/1 | todo |  |
| 0x481D30 | `sub_481D30` | 77 | 0x513390 MATCH |  | 0/1 | todo |  |
| 0x481DF0 | `sub_481DF0` | 11 |  | Source/Bink.cpp:12 | 0/2 | todo |  |
| 0x482090 | `unknown_libname_24` | 10 |  |  | 0/1 | todo |  |
| 0x4822A0 | `sub_4822A0` | 341 |  |  | 0/1 | todo |  |
| 0x482410 | `sub_482410` | 42 |  |  | 0/1 | todo |  |
| 0x482510 | `sub_482510` | 39 |  | Source/fix16.hpp:453 | 0/2 | todo |  |
| 0x482730 | `sub_482730` | 56 |  | Source/fix16.hpp:266, Source/fix16.hpp:373 | 0/3 | todo |  |
| 0x482980 | `Sprite_4C::sub_482980` | 23 |  | Source/sprite.hpp:61 | 0/1 | todo |  |
| 0x4829A0 | `Object_2C_Pool::sub_4829A0` | 26 |  |  | 0/1 | todo |  |
| 0x4829C0 | `Object_2C_Pool::sub_4829C0` | 24 |  |  | 0/1 | todo |  |
| 0x482A40 | `sub_482A40` | 37 |  | Source/sprite.hpp:478 | 0/1 | todo |  |
| 0x482A70 | `sub_482A70` | 14 |  | Source/Rozza_C88.hpp:29 | 0/1 | todo |  |
| 0x482A80 | `sub_482A80` | 14 |  | Source/Rozza_C88.hpp:36 | 0/1 | todo |  |
| 0x482A90 | `sub_482A90` | 10 |  | Source/Wolfy_3D4.hpp:34 | 0/1 | todo |  |
| 0x482BF0 | `sub_482BF0` | 11 |  | Source/Object_5C.hpp:302 | 0/1 | todo |  |
| 0x482C00 | `sub_482C00` | 10 |  | Source/Object_5C.hpp:182 | 0/1 | todo |  |
| 0x482C10 | `sub_482C10` | 23 |  | Source/Object_5C.cpp:99, Source/Object_5C.hpp:293 | 0/2 | todo |  |
| 0x482D60 | `sub_482D60` | 43 |  | Source/nostalgic_ellis_0x28.hpp:80 | 0/2 | todo |  |
| 0x483A00 | `sub_483A00` | 8 |  |  | 0/4 | todo |  |
| 0x483C00 | `sub_483C00` | 27 |  |  | 0/1 | todo |  |
| 0x483C60 | `sub_483C60` | 19 | 0x5291E0 MATCH |  | 0/1 | todo |  |
| 0x483C80 | `sub_483C80` | 37 |  |  | 0/1 | todo |  |
| 0x483D90 | `sub_483D90` | 177 |  |  | 0/1 | todo |  |
| 0x483F10 | `Object_2C_Pool::ctor_483F10` | 72 |  |  | 0/1 | todo |  |
| 0x483F60 | `Object_8_Pool::ctor_483F60` | 64 |  |  | 0/1 | todo |  |
| 0x484820 | `Object_2C_Pool::gdtor_484820` | 30 |  |  | 0/1 | todo |  |
| 0x484840 | `Object_8_Pool::gdtor_484840` | 30 |  |  | 0/1 | todo |  |
| 0x484860 | `Object_3C_Pool::gdtor_484860` | 30 |  |  | 0/1 | todo |  |
| 0x484880 | `sub_484880` | 61 |  |  | 0/1 | todo |  |
| 0x4848C0 | `Object_3C_Pool::ctor_4848C0` | 66 |  |  | 0/1 | todo |  |
| 0x484D60 | `Object_2C_Pool::sub_484D60` | 80 |  |  | 0/1 | todo |  |
| 0x484DB0 | `Object_2C_Pool::sub_484DB0` | 26 |  |  | 0/2 | todo |  |
| 0x484DD0 | `sub_484DD0` | 46 |  |  | 0/1 | todo |  |
| 0x485540 | `sub_485540` | 231 | 0x52A3D0 MATCH |  | 0/1 | todo |  |
| 0x487F50 | `Object_5C::sub_487F50` | 16 |  |  | 0/1 | todo |  |
| 0x488200 | `sub_488200` | 10 |  |  | 0/1 | todo |  |
| 0x488EF0 | `Phi_74::sub_488EF0` | 30 |  |  | 0/1 | todo |  |
| 0x489620 | `sub_489620` | 6 |  | Source/Door_4D4.hpp:33 | 0/1 | todo |  |
| 0x489630 | `sub_489630` | 6 |  | Source/Door_4D4.hpp:28 | 0/1 | todo |  |
| 0x489640 | `sub_489640` | 6 |  | Source/Door_4D4.hpp:23 | 0/1 | todo |  |
| 0x489AC0 | `sub_489AC0` | 79 |  |  | 0/1 | todo |  |
| 0x48A8D0 | `sub_48A8D0` | 7 |  | Source/sprite.hpp:766 | 0/1 | todo |  |
| 0x48D4E0 | `sub_48D4E0` | 969 |  |  | 0/1 | todo |  |
| 0x48E480 | `sub_48E480` | 355 |  |  | 0/1 | todo |  |
| 0x48E5F0 | `sub_48E5F0` | 351 |  | Source/Wolfy_3D4.cpp:281 | 0/1 | todo |  |
| 0x48E750 | `sub_48E750` | 351 |  |  | 0/1 | todo |  |
| 0x48F1C0 | `Particle_4C_Pool::gdtor_48F1C0` | 30 |  |  | 0/1 | todo |  |
| 0x48F1E0 | `Particle_4C_Pool::ctor_48F1E0` | 69 |  |  | 0/1 | todo |  |
| 0x491550 | `sub_491550` | 1288 |  |  | 0/1 | todo |  |
| 0x491CE0 | `Particle_8::sub_491CE0` | 11 |  |  | 0/1 | todo |  |
| 0x491F10 | `sub_491F10` | 93 |  |  | 0/1 | todo |  |
| 0x4920A0 | `sub_4920A0` | 131 |  |  | 0/1 | todo |  |
| 0x492180 | `Char_B4::sub_492180` | 10 |  | Source/char.hpp:154 | 0/1 | todo |  |
| 0x492400 | `sub_492400` | 25 |  |  | 0/1 | todo |  |
| 0x493090 | `sub_493090` | 24 |  | Source/Object_5C.hpp:296 | 0/2 | todo |  |
| 0x4932D0 | `sub_4932D0` | 158 | 0x545430 MATCH |  | 0/1 | todo |  |
| 0x494280 | `sub_494280` | 1492 |  |  | 0/1 | todo |  |
| 0x495220 | `sub_495220` | 541 | 0x54C1A0 MATCH |  | 0/1 | todo |  |
| 0x4955F0 | `sub_4955F0` | 267 |  |  | 0/1 | todo |  |
| 0x49C700 | `Orca_2FD4::init_49C700` | 49 |  |  | 0/1 | todo |  |
| 0x49E330 | `sub_49E330` | 38 |  |  | 0/3 | todo |  |
| 0x49E3C0 | `ApplyDeadZone_49E3C0` | 82 |  | Source/Fix16_Point.hpp:41 | 0/1 | todo |  |
| 0x49E450 | `sub_49E450` | 42 |  | Source/Fix16_Point.hpp:35 | 0/1 | todo |  |
| 0x49E540 | `sub_49E540` | 43 |  | Source/gtx_0x106C.hpp:180, Source/map_0x370.hpp:556 | 0/1 | todo |  |
| 0x49E820 | `sub_49E820` | 35 |  |  | 0/1 | todo |  |
| 0x49EA60 | `sub_49EA60` | 14 |  | Source/CarPhysics_B0.hpp:352 | 0/1 | todo |  |
| 0x49ED60 | `CarPhysics_B0::sub_49ED60` | 87 | 0x563350 WIP |  | 0/1 | todo |  |
| 0x49EE10 | `CarPhysics_B0::sub_49EE10` | 61 | 0x563670 MATCH |  | 0/3 | todo |  |
| 0x49EE80 | `sub_49EE80` | 44 |  |  | 0/1 | todo |  |
| 0x49EF10 | `sub_49EF10` | 10 |  | Source/Rozza_C88.hpp:23 | 0/1 | todo |  |
| 0x49EF20 | `IsCharB4_49EF20` | 23 |  | Source/Rozza_C88.hpp:66 | 0/1 | todo |  |
| 0x49EFB0 | `sub_49EFB0` | 3 |  | Source/Car_BC.hpp:334 | 0/1 | todo |  |
| 0x49EFD0 | `sub_49EFD0` | 11 |  | Source/CarPhysics_B0.cpp:2205 | 0/1 | todo |  |
| 0x49F170 | `sub_49F170` | 55 |  |  | 0/1 | todo |  |
| 0x49F4E0 | `sub_49F4E0` | 14 |  | Source/CarPhysics_B0.hpp:358 | 0/1 | todo |  |
| 0x49F4F0 | `sub_49F4F0` | 14 |  | Source/CarPhysics_B0.hpp:364 | 0/1 | todo |  |
| 0x49F500 | `unknown_libname_25` | 10 |  | Source/CarPhysics_B0.hpp:370 | 0/1 | todo |  |
| 0x49F510 | `sub_49F510` | 94 |  |  | 0/1 | todo |  |
| 0x49F930 | `sub_49F930` | 392 |  |  | 0/1 | todo |  |
| 0x4A0290 | `sub_4A0290` | 713 |  |  | 0/1 | todo |  |
| 0x4A0900 | `sub_4A0900` | 45 |  |  | 0/1 | todo |  |
| 0x4A1CF0 | `CarPhysics_B0::sub_4A1CF0` | 51 |  |  | 0/1 | todo |  |
| 0x4A1D30 | `sub_4A1D30` | 39 |  |  | 0/1 | todo |  |
| 0x4A2240 | `sub_4A2240` | 1009 |  |  | 0/1 | todo |  |
| 0x4A2E00 | `sub_4A2E00` | 40 |  |  | 0/1 | todo |  |
| 0x4A4750 | `sub_4A4750` | 12 |  | Source/Game_0x40.hpp:39 | 0/1 | todo |  |
| 0x4A4760 | `sub_4A4760` | 9 |  | Source/Hud.hpp:177 | 0/1 | todo |  |
| 0x4A4770 | `sub_4A4770` | 15 |  | Source/Hud.hpp:683 | 0/1 | todo |  |
| 0x4A4940 | `sub_4A4940` | 105 |  |  | 0/1 | todo |  |
| 0x4A49B0 | `angry_lewin_0x85C::sub_4A49B0` | 11 |  |  | 0/1 | todo |  |
| 0x4A4D50 | `sub_4A4D50` | 35 |  |  | 0/3 | todo |  |
| 0x4A4F10 | `sub_4A4F10` | 11 |  |  | 0/1 | todo |  |
| 0x4A4F20 | `Weapon_30_Pool::sub_4A4F20` | 86 |  | Source/Weapon_8.hpp:63 | 0/1 | todo |  |
| 0x4A4F80 | `keen_bhaskara_0x30::sub_4A4F80` | 8 |  | Source/Weapon_30.hpp:141 | 0/2 | todo |  |
| 0x4A4F90 | `sub_4A4F90` | 6 |  | Source/Weapon_30.hpp:27 | 0/3 | todo |  |
| 0x4A4FA0 | `Weapon_30::sub_4A4FA0` | 9 |  | Source/Weapon_30.hpp:86 | 0/1 | todo |  |
| 0x4A4FB0 | `sub_4A4FB0` | 45 |  | Source/Weapon_30.hpp:92 | 0/2 | todo |  |
| 0x4A4FE0 | `sub_4A4FE0` | 4 |  | Source/Weapon_30.hpp:21 | 0/2 | todo |  |
| 0x4A4FF0 | `sub_4A4FF0` | 11 |  | Source/Weapon_30.hpp:146 | 0/1 | todo |  |
| 0x4A5010 | `cool_nash_0x294::sub_4A5010` | 16 |  | Source/Ped.hpp:796 | 0/1 | todo |  |
| 0x4A5020 | `sub_4A5020` | 7 |  | Source/Ped.hpp:143 | 0/1 | todo |  |
| 0x4A5030 | `sub_4A5030` | 11 |  |  | 0/1 | todo |  |
| 0x4A5040 | `cool_nash_0x294::not_enter_car_as_passenger_4A5040` | 11 |  | Source/Ped.hpp:540 | 0/3 | todo |  |
| 0x4A5050 | `sub_4A5050` | 8 |  | Source/Ped.hpp:390 | 0/1 | todo |  |
| 0x4A5060 | `sub_4A5060` | 11 |  | Source/Ped.hpp:384 | 0/2 | todo |  |
| 0x4A5070 | `sub_4A5070` | 8 |  | Source/Camera.hpp:92 | 0/3 | todo |  |
| 0x4A5090 | `sub_4A5090` | 28 |  |  | 0/1 | todo |  |
| 0x4A50B0 | `sub_4A50B0` | 38 |  | Source/thirsty_lamarr.hpp:18 | 0/3 | todo |  |
| 0x4A50E0 | `sub_4A50E0` | 5 |  | Source/eager_benz.hpp:14 | 0/2 | todo |  |
| 0x4A5100 | `angry_lewin_0x85C::sub_4A5100` | 36 |  | Source/Player.hpp:201 | 0/2 | todo |  |
| 0x4A5150 | `sub_4A5150` | 27 |  | Source/Player.hpp:318 | 0/2 | todo |  |
| 0x4A5170 | `sub_4A5170` | 13 |  | Source/Player.hpp:63 | 0/1 | todo |  |
| 0x4A5180 | `sub_4A5180` | 8 |  | Source/Player.hpp:191 | 0/1 | todo |  |
| 0x4A51A0 | `GetRaw_4A5190` | 11 |  | Source/BitSet32.hpp:75 | 0/1 | todo |  |
| 0x4A51B0 | `sub_4A51B0` | 12 |  | Source/Car_BC.hpp:469 | 0/1 | todo |  |
| 0x4A53D0 | `sub_4A53D0` | 38 |  |  | 0/1 | todo |  |
| 0x4A5600 | `angry_lewin_0x85C::sub_4A5600` | 59 |  |  | 0/1 | todo |  |
| 0x4A6100 | `sub_4A6100` | 182 |  |  | 0/1 | todo |  |
| 0x4A6530 | `sub_4A6530` | 171 |  |  | 0/1 | todo |  |
| 0x4A6FC0 | `sub_4A6FC0` | 15 |  |  | 0/1 | todo |  |
| 0x4A6FD0 | `angry_lewin_0x85C::sub_4A6FD0` | 62 |  |  | 0/1 | todo |  |
| 0x4A8910 | `sub_4A8910` | 199 |  |  | 0/1 | todo |  |
| 0x4A8B60 | `sub_4A8B60` | 23 | 0x56BCF0 MATCH |  | 0/1 | todo |  |
| 0x4A9050 | `sub_4A9050` | 69 |  |  | 0/1 | todo |  |
| 0x4A9270 | `sub_4A9270` | 28 |  |  | 0/1 | todo |  |
| 0x4B2F20 | `sound_obj::sub_4B2F20` | 193 |  |  | 0/1 | todo |  |
| 0x4B53B0 | `Registry::Get_Debug_Setting_4B53B0` | 108 | 0x586E90 MATCH |  | 0/1 | todo |  |
| 0x4B5420 | `sub_4B5420` | 102 |  |  | 0/1 | todo |  |
| 0x4B6020 | `sub_4B6020` | 26 | 0x58DBF0 MATCH |  | 0/1 | todo |  |
| 0x4B6700 | `sub_4B6700` | 168 |  |  | 0/1 | todo |  |
| 0x4B7580 | `sub_4B7580` | 8 |  |  | 0/1 | todo |  |
| 0x4B7590 | `Game_0x40::sub_4B7590` | 8 |  | Source/Game_0x40.hpp:51 | 0/1 | todo |  |
| 0x4B75A0 | `j_thirsty_lamarr::sub_41DC30` | 5 |  |  | 0/1 | todo |  |
| 0x4B8A60 | `sub_4B8A60` | 14 | 0x5935D0 MATCH |  | 0/2 | todo |  |
| 0x4B8FD0 | `CokeZero_FC::sub_4B8FD0` | 4 | 0x5935C0 MATCH | Source/Pool.hpp:65 | 0/2 | todo |  |
| 0x4B8FE0 | `CokeZero_FC::sub_4B8FE0` | 17 |  |  | 0/1 | todo |  |
| 0x4B9000 | `CokeZero_FC::sub_4B9000` | 69 |  |  | 0/1 | todo |  |
| 0x4B9260 | `CokeZero_100::sub_4B9260` | 8 |  |  | 0/1 | todo |  |
| 0x4B9490 | `CokeZero_100::ctor_4B9490` | 26 |  |  | 0/1 | todo |  |
| 0x4B99F0 | `Car_3C::sub_4B99F0` | 27 |  |  | 0/1 | todo |  |
| 0x4B9A30 | `sub_4B9A30` | 45 |  |  | 0/2 | todo |  |
| 0x4B9A80 | `sub_4B9A80` | 17 |  | Source/Game_0x40.hpp:97 | 0/2 | todo |  |
| 0x4B9D50 | `sub_4B9D50` | 145 | 0x5A3100 MATCH |  | 0/1 | todo |  |
| 0x4B9F00 | `Sprite_3CC::gdtor_4B9F00` | 30 |  |  | 0/1 | todo |  |
| 0x4B9F30 | `sub_4B9F30` | 12 |  |  | 0/1 | todo |  |
| 0x4B9F40 | `sub_4B9F40` | 54 |  | Source/map_0x370.hpp:553 | 0/1 | todo |  |
| 0x4B9F80 | `sub_4B9F80` | 65 |  | Source/sprite.cpp:1692 | 0/1 | todo |  |
| 0x4B9FD0 | `sub_4B9FD0` | 58 |  | Source/Fix16_Rect.hpp:115 | 0/1 | todo |  |
| 0x4BA070 | `sub_4BA070` | 38 |  | Source/sprite.hpp:69 | 0/1 | todo |  |
| 0x4BA220 | `sub_4BA220` | 10 |  | Source/sprite.hpp:196 | 0/2 | todo |  |
| 0x4BA230 | `sub_4BA230` | 21 |  | Source/sprite.cpp:341, Source/sprite.hpp:335 | 0/1 | todo |  |
| 0x4BA250 | `sub_4BA250` | 37 |  | Source/Rozza_C88.hpp:57 | 0/1 | todo |  |
| 0x4BA280 | `sub_4BA280` | 37 |  | Source/Rozza_C88.hpp:48 | 0/1 | todo |  |
| 0x4BA390 | `sub_4BA390` | 34 |  | Source/Car_BC.hpp:720 | 0/1 | todo |  |
| 0x4BA7E0 | `sub_4BA7E0` | 105 |  |  | 0/1 | todo |  |
| 0x4BAA70 | `sub_4BAA70` | 19 |  |  | 0/2 | todo |  |
| 0x4BBC80 | `sub_4BBC80` | 136 |  |  | 0/1 | todo |  |
| 0x4BC8F0 | `Sprite_4C::sub_4BC8F0` | 14 |  |  | 0/1 | todo |  |
| 0x4BC9A0 | `Sprite_4C_Pool::ctor_4BC9A0` | 66 |  | Source/sprite.hpp:722 | 0/1 | todo |  |
| 0x4BC9F0 | `Sprite_4C_Pool::sub_4BC9F0` | 19 |  |  | 0/2 | todo |  |
| 0x4BCA10 | `Sprite_4C_Pool::sub_4BCA10` | 14 |  |  | 0/1 | todo |  |
| 0x4BCA20 | `Sprite_Pool::ctor_4BCA20` | 66 |  | Source/sprite.hpp:747 | 0/1 | todo |  |
| 0x4BDC90 | `Sprite_Pool::gdtor_4BDC90` | 30 |  |  | 0/1 | todo |  |
| 0x4BDCB0 | `Sprite_4C_Pool::gdtor_4BDCB0` | 30 |  |  | 0/1 | todo |  |
| 0x4BDCF0 | `Sprite_18_Pool::ctor_4BDCF0` | 66 |  | Source/sprite.hpp:699 | 0/1 | todo |  |
| 0x4BE060 | `Car_3C::sub_4BE060` | 1293 |  |  | 0/1 | todo |  |
| 0x4BE710 | `Sprite_18_Pool::gdtor_4BE710` | 30 |  |  | 0/1 | todo |  |
| 0x4BE850 | `sub_4BE850` | 23 |  | Source/Object_5C.hpp:255 | 0/1 | todo |  |
| 0x4BE920 | `sub_4BE920` | 44 |  |  | 0/1 | todo |  |
| 0x4BE950 | `sub_4BE950` | 35 |  |  | 0/1 | todo |  |
| 0x4BE9C0 | `sub_4BE9C0` | 42 |  |  | 0/3 | todo |  |
| 0x4BEA90 | `sub_4BEA90` | 45 |  |  | 0/2 | todo |  |
| 0x4BEC40 | `Sprite_18_Pool::sub_4BEC40` | 8 |  |  | 0/2 | todo |  |
| 0x4BF210 | `sub_4BF210` | 27 | 0x5AA3D0 MATCH |  | 0/1 | todo |  |
| 0x4BF790 | `sub_4BF790` | 25 |  |  | 0/2 | todo |  |
| 0x4BFFE0 | `sub_4BFFE0` | 44 |  |  | 0/2 | todo |  |
| 0x4C03D0 | `sub_4C03D0` | 21 |  | Source/gtx_0x106C.hpp:189 | 0/1 | todo |  |
| 0x4C0950 | `Taxi_4_Pool::ctor_4C0950` | 41 |  |  | 0/1 | todo |  |
| 0x4C0990 | `Taxi_4_Pool::gdtor_4C0990` | 30 |  |  | 0/1 | todo |  |
| 0x4C09B0 | `Taxi_4::sub_4C09B0` | 12 |  | Source/Taxi_4.hpp:44 | 0/1 | todo |  |
| 0x4C0A80 | `sub_4C0A80` | 66 |  |  | 0/1 | todo |  |
| 0x4C22D0 | `sub_4C22D0` | 46 |  |  | 0/1 | todo |  |
| 0x4C23A0 | `text_0x14::Find_4C23A0` | 5 | 0x5B5F90 MATCH |  | 0/1 | todo |  |
| 0x4C23F0 | `text_tkey::ctor_4C23F0` | 16 |  |  | 0/1 | todo |  |
| 0x4C2400 | `text_tkey::dtor_4C2400` | 26 |  |  | 0/1 | todo |  |
| 0x4C2420 | `text_tdat::ctor_4C2420` | 16 |  |  | 0/1 | todo |  |
| 0x4C2430 | `text_tdat::dtor_4C2430` | 26 |  |  | 0/1 | todo |  |
| 0x4C2540 | `text_0x14::Load_4C2540` | 212 | 0x5B5E90 MATCH |  | 0/1 | todo |  |
| 0x4C2620 | `text_0x14::ctor_4C2620` | 152 | 0x5B5FB0 MATCH |  | 0/1 | todo |  |
| 0x4C2EF0 | `sub_4C2EF0` | 18 |  |  | 0/1 | todo |  |
| 0x4C2F10 | `festive_hopper::ctor_4C2F10` | 24 |  |  | 0/1 | todo |  |
| 0x4C2F30 | `sub_4C2F30` | 52 |  |  | 0/1 | todo |  |
| 0x4C3380 | `sub_4C3380` | 26 |  |  | 0/2 | todo |  |
| 0x4C33F0 | `sub_4C33F0` | 54 |  | Source/TileAnim_2.hpp:24 | 0/1 | todo |  |
| 0x4C34B0 | `TileAnimPool::ctor_4C34B0` | 71 |  |  | 0/1 | todo |  |
| 0x4C3590 | `sub_4C3590` | 11 |  |  | 0/1 | todo |  |
| 0x4C3630 | `TileAnimPool::gdtor_4C3630` | 30 |  |  | 0/1 | todo |  |
| 0x4C3850 | `sub_4C3850` | 35 |  |  | 0/1 | todo |  |
| 0x4C39F0 | `sub_4C39F0` | 32 |  | Source/Car_BC.hpp:158 | 0/1 | todo |  |
| 0x4C3A10 | `sub_4C3A10` | 390 |  |  | 0/1 | todo |  |
| 0x4C4A30 | `TrafficLights_194::sub_4C4A30` | 40 |  |  | 0/1 | todo |  |
| 0x4C4B70 | `Montana_2EE4::sub_4C4B70` | 11 |  | Source/Montana.hpp:77 | 0/2 | todo |  |
| 0x4C4B80 | `Montana_FA4::Push_4C4B80` | 30 |  | Source/Montana.hpp:34 | 0/1 | todo |  |
| 0x4C4BA0 | `Montana_FA4::Pop_4C4BA0` | 20 |  | Source/Montana.hpp:47 | 0/1 | todo |  |
| 0x4C4BC0 | `Montana_FA4::IsEnd_4C4BC0` | 10 |  | Source/Montana.hpp:41 | 0/1 | todo |  |
| 0x4C4BD0 | `Montana_FA4::ctor_4C4BD0` | 9 |  |  | 0/1 | todo |  |
| 0x4C4F10 | `sub_4C4F10` | 10 |  | Source/Object_5C.hpp:30 | 0/1 | todo |  |
| 0x4C62B0 | `sub_4C62B0` | 17 |  |  | 0/1 | todo |  |
| 0x4C6E20 | `sub_4C6E20` | 11 |  |  | 0/1 | todo |  |
| 0x4C6E50 | `Garox_110C_sub::ctor_4C6E50` | 19 |  |  | 0/1 | todo |  |
| 0x4C6E70 | `Garox_27B5_sub::ctor_4C6E70` | 6 |  |  | 0/1 | todo |  |
| 0x4C6EE0 | `Garox_Sub_C_Array::ctor_4C6EE0` | 29 |  |  | 0/1 | todo |  |
| 0x4C6F30 | `sub_4C6F30` | 4 |  | Source/Hud.hpp:449 | 0/1 | todo |  |
| 0x4C6F80 | `sub_4C6F80` | 35 |  | Source/Hud.hpp:522 | 0/6 | todo |  |
| 0x4C6FF0 | `sub_4C6FF0` | 50 |  | Source/Hud.cpp:2283, Source/Hud.hpp:505 | 0/1 | todo |  |
| 0x4C7050 | `sub_4C7050` | 4 |  | Source/Hud.hpp:532 | 0/2 | todo |  |
| 0x4C7080 | `Garox_7C_Array::ctor_4C7080` | 43 |  |  | 0/1 | todo |  |
| 0x4C70B0 | `sub_4C70B0` | 7 |  | Source/Hud.hpp:595 | 0/1 | todo |  |
| 0x4C70F0 | `Garox_C4::sub_4C70F0` | 11 |  | Source/Hud.hpp:233 | 0/1 | todo |  |
| 0x4C7120 | `sub_4C7120` | 9 |  | Source/Hud.hpp:296 | 0/1 | todo |  |
| 0x4C7130 | `sub_4C7130` | 46 |  | Source/Hud.cpp:1610, Source/Hud.hpp:302 | 0/1 | todo |  |
| 0x4C7160 | `sub_4C7160` | 9 |  | Source/Hud.hpp:284 | 0/2 | todo |  |
| 0x4C7170 | `sub_4C7170` | 7 |  | Source/Hud.hpp:290 | 0/2 | todo |  |
| 0x4C71A0 | `Garox_12E4_sub::ctor_4C71A0` | 10 |  |  | 0/1 | todo |  |
| 0x4C7340 | `sub_4C7340` | 4 |  | Source/Player.hpp:69 | 0/1 | todo |  |
| 0x4C7350 | `sub_4C7350` | 21 |  | Source/frosty_pasteur_0xC1EA8.hpp:605 | 0/1 | todo |  |
| 0x4C7380 | `sub_4C7380` | 25 |  | Source/Hud.cpp:671, Source/youthful_einstein.hpp:33 | 0/1 | todo |  |
| 0x4C73A0 | `sub_4C73A0` | 246 |  |  | 0/1 | todo |  |
| 0x4C78A0 | `sub_4C78A0` | 304 |  |  | 0/1 | todo |  |
| 0x4C7A30 | `sub_4C7A30` | 218 |  |  | 0/1 | todo |  |
| 0x4C7CC0 | `sub_4C7CC0` | 46 | 0x5D03C0 MATCH |  | 0/1 | todo |  |
| 0x4C7FC0 | `sub_4C7FC0` | 759 |  |  | 0/1 | todo |  |
| 0x4C8880 | `sub_4C8880` | 142 |  |  | 0/1 | todo |  |
| 0x4C8A20 | `sub_4C8A20` | 21 |  |  | 0/1 | todo |  |
| 0x4CA610 | `sub_4CA610` | 53 |  | Source/Hud.cpp:2271, Source/Hud.hpp:502 | 0/1 | todo |  |
| 0x4CA660 | `Garox_C_Array::ctor_4CA660` | 27 |  |  | 0/1 | todo |  |
| 0x4CA910 | `sub_4CA910` | 365 |  |  | 0/1 | todo |  |
| 0x4CAC30 | `sub_4CAC30` | 46 |  |  | 0/1 | todo |  |
| 0x4CB520 | `sub_4CB520` | 11 |  |  | 0/1 | todo |  |
| 0x4CC9B0 | `Weapon_30_Pool::sub_4CC9B0` | 4 |  | Source/Weapon_8.hpp:92 | 0/1 | todo |  |
| 0x4CC9C0 | `sub_4CC9C0` | 28 |  | Source/Weapon_8.hpp:42 | 0/1 | todo |  |
| 0x4CC9E0 | `sub_4CC9E0` | 26 |  | Source/Weapon_8.hpp:52 | 0/1 | todo |  |
| 0x4CCA00 | `sub_4CCA00` | 10 |  | Source/Weapon_30.hpp:33 | 0/2 | todo |  |
| 0x4CCA10 | `sub_4CCA10` | 10 |  | Source/Weapon_30.hpp:39 | 0/1 | todo |  |
| 0x4CCA20 | `sub_4CCA20` | 10 |  | Source/Weapon_30.hpp:45 | 0/1 | todo |  |
| 0x4CDA20 | `Weapon_30_Pool::ctor_4CDA20` | 74 |  |  | 0/1 | todo |  |
| 0x4CEF40 | `sub_4CEF40` | 1076 |  |  | 0/1 | todo |  |
| 0x4D06E0 | `Weapon_8::sub_4D06E0` | 27 |  |  | 0/2 | todo |  |
| 0x4D0700 | `Car_BC::sub_4D0700` | 59 |  |  | 0/1 | todo |  |
| 0x4D07E0 | `Weapon_30_Pool::gdtor_4D07E0` | 30 |  |  | 0/1 | todo |  |
| 0x4D09C0 | `Game_0x40::get_main_state_4D09C0` | 4 |  | Source/Game_0x40.hpp:63 | 0/1 | todo |  |

<!-- table unpaired -->
`0x407BD0`, `0x408370`, `0x414710`, `0x4157C0`, `0x416260`, `0x4174C0`, `0x4182E0`, `0x418720`, `0x418940`, `0x41A580`, `0x41E850`, `0x41F520`, `0x435630`, `0x439FB0`, `0x43AAE0`, `0x43AAF0`, `0x43B420`, `0x43B850`, `0x43B870`, `0x43CAC0`, `0x43DC00`, `0x43F130`, `0x440D90`, `0x442520`, `0x442AD0`, `0x442D70`, `0x443360`, `0x4436A0`, `0x447D40`, `0x44A1F0`, `0x44AF00`, `0x44D1D0`, `0x452060`, `0x454410`, `0x45D000`, `0x461290`, `0x4614E0`, `0x4645B0`, `0x465D00`, `0x4661F0`, `0x466BF0`, `0x46A350`, `0x4713C0`, `0x47E840`, `0x4CDF30`, `0x4D6000`, `0x4D94E0`, `0x4DF3E0`, `0x4E5E90`, `0x4E6660`, `0x4EA390`, `0x4EB940`, `0x4EBA60`, `0x4EC450`, `0x4EC7A0`, `0x4ECAF0`, `0x4ECE40`, `0x4EE130`, `0x4EEAF0`, `0x4EEE60`, `0x4EF1C0`, `0x4EF520`, `0x4EF880`, `0x4EFB20`, `0x4EFDB0`, `0x4F0030`, `0x4F0420`, `0x4F1660`, `0x4F22F0`, `0x4F33B0`, `0x4F3C00`, `0x4F4250`, `0x4F4600`, `0x4F49B0`, `0x4F6A20`, `0x4F76A0`, `0x4FFD90`, `0x509180`, `0x509FD0`, `0x50E190`, `0x5133E0`, `0x51E650`, `0x5229B0`, `0x525100`, `0x528130`, `0x538060`, `0x538AC0`, `0x53A280`, `0x53BAC0`, `0x53D260`, `0x53F060`, `0x540320`, `0x540D30`, `0x540F90`, `0x5411E0`, `0x542E30`, `0x545AF0`, `0x548BD0`, `0x54B8F0`, `0x54C3E0`, `0x54CAE0`, `0x54CC40`, `0x55AD90`, `0x55D200`, `0x55DC00`, `0x561130`, `0x561380`, `0x5615D0`, `0x561970`, `0x563280`, `0x57DF10`, `0x57E960`, `0x597100`, `0x59EB30`, `0x59EE40`, `0x59EFF0`, `0x5A0150`, `0x5A0A70`, `0x5A1030`, `0x5A1490`, `0x5A22B0`, `0x5A4DA0`, `0x5CF730`, `0x5CFE40`, `0x5D0110`, `0x5D0850`, `0x5D8470`, `0x5DD290`, `0x5DE4F0`, `0x5DE910`, `0x5DF270`, `0x5E0B10`, `0x5E0E70`, `0x5E10E0`, `0x5E13E0`, `0x5E2550`, `0x5E2940`
