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
address (`MaxAbsDistance`). The basic `Fix16`/`ang16`/point operators are left out of
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

Paired: 3045/4459 10.5 functions. Marked functions with inlined callees: WIP 0, STUB 0, MATCH 1103. Distinct inlined 9.6f functions: 1351. WIPs without a 9.6f partner: 0.

<!-- table WIP -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|

<!-- table STUB -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|

<!-- table MATCH -->
| 10.5 | Function | 9.6f | Inlined 9.6f callees | Status | Notes |
|---|---|---|---|---|---|
| 0x405CE0 | `SmoothApproachAngle` | 0x40ECB0 | ✓ `WrapAngle_40E790` | matched | all 9.6f inlines used |
| 0x407CE0 | `Trailer::UpdateTrailerAlignment` | 0x40F900 | ✓ `sub_40F820`, ✓ `sub_40F580`, `sub_421CB0`, ✓ `sub_40F6B0`, `sub_40F7C0`, `sub_40F680`, ✓ `sub_40F540`, `sub_40F600`, ✓ `sub_40F790`, `sub_40F0A0`, ✓ `sub_40F840`, `sub_40EE60`, ✓ `sub_40F830` (10.5 0x407B80), ✓ `sub_40F7E0`, ✓ `sub_40F800`, ✓ `sub_40F810` | inlines added | 318->235; get/set_theta_40F820/40F830, set_cp1, +=, Fix16_To_Ang16, GetAngle used; 40F840 commented (different globals); 40F0A0/40EE60/421CB0 are pairing noise. vtall also showed DIFF WIP in CarAI 0x44E560 and ParticleManager 0x53F4C0 (not checked) |
| 0x408140 | `Trailer::ServicePhysics` | 0x40FB20 | `sub_427450` | checked | 427450 = a called Car method (ManageDrowning or physics state machine) |
| 0x4081B0 | `Trailer::DeAllocateCarPhysics` | 0x40F470 | `sub_426120` | checked | 426120 = DeAllocateOwnCarPhysics (called) |
| 0x408220 | `Trailer::Update` | 0x40FBE0 | ✓ `sub_40F490`, ✓ `sub_40F7B0`, `sub_40FB70` | checked | 40F490 = CarPool::Remove (already), 40FB70 = ExplodeBothIfOneDestroyed (called), SetDrawLayer already |
| 0x40B890 | `CollisionEvent::SetupForCar` | 0x40FF20 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40B980 | `CollisionEvent::SetupForPed` | 0x410010 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BA60 | `CollisionEvent::SetupForObject` | 0x4100F0 | ✓ `sub_40FF00`, ✓ `sub_40FEF0`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0` | done | Object::MakesCollisionSound (second use); Still matches. |
| 0x40BBA0 | `CollisionSoundQueue::AddCollision` | 0x410210 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `SetImpactStrength_40FF10` | done | all 9.6f inlines used |
| 0x40BC40 | `CollisionSoundQueue::AddFloorCollision` | 0x410370 | ✓ `SetImpactStrength_40FF10`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BD10 | `CollisionSoundQueue::AddBlockCollision` | 0x4102A0 | ✓ `SetImpactStrength_40FF10`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0`, ✓ `sub_40FEF0` | done | all 9.6f inlines used |
| 0x40BE00 | `CollisionSoundQueue::ctor_40BE00` | 0x4104B0 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler vector ctor helper |
| 0x40EF40 | `root_sound::CreateSoundObject` | 0x410750 | ✓ `root_sound::sub_410730` | done | root_sound::PopFree (new); its 9.6f callee SoundObject::sub_4106D0 isn't dumped, only field_8 = 0 assumed (TODO); code unchanged |
| 0x412490 | `sound_obj::ProcessType2_Frontend_412490` | 0x411A50 | `sub_4B6700` | checked | 9.6f sub_4B6700 = cSampleManager::OpenStream (still called) |
| 0x412740 | `sound_obj::ProcessType1_Sprite` | 0x4162C0 | ✓ `sub_4117B0`, `sub_411730`, ✓ `sub_40FEA0`, ✓ `sub_40FEB0`, ✓ `sub_40FEC0`, `sub_41B030` | checked | 9.6f sub_41B030 is a Char dispatcher (4181E0/41A3C0, not paired), 10.5 calls ProcessPed; GetPosition already used; 411730 still called |
| 0x412820 | `sound_obj::ProcessType8_Crane` | 0x411D20 | ✓ `sub_411A00`, ✓ `sub_4117B0`, `sub_411730` | matched | GetRotorSprite + GetPosition, 86->52; sub_411730 = ComputeEmitterDistanceSquared (still called in 10.5, pairing noise) |
| 0x412A60 | `sound_obj::ProcessType9_Crusher` | 0x412010 | ✓ `sub_411A10`, ✓ `unknown_libname_20`, ✓ `sub_411A30`, `sub_411730` | done | Crusher::get_state + get_ypos (new); code unchanged; 411730 still called |
| 0x412B80 | `sound_obj::ProcessCar` | 0x415DF0 | ✓ `sub_4118D0`, ✓ `sub_411940`, `sub_4156B0` | checked | 9.6f sub_4156B0 is a thunk to ProcessTrain (4143B0); GetCarInfoIdx already used |
| 0x413120 | `sound_obj::SelectObjectImpactSound` | 0x4150E0 | ✓ `IsBulletproof_414F60` | commented | Car IsBulletproof_414F60/IsFlagSet (f78 & 0x100) as bool inline makes 196->198 (value materialised in dl) |
| 0x413760 | `sound_obj::ProcessType6_CollisionSoundQueue_413760` | 0x415F90 | `sub_411730` | matched | sub_411730 = ComputeEmitterDistanceSquared, 10.5 still calls it (pairing noise) |
| 0x413A10 | `sound_obj::GetCollisionVolume_413A10` | 0x4121F0 | `Car_BC::sub_421D90`, ✓ `sub_410BF0` | checked | Car::sub_421D90 = GetCarLinearSpeed (still called in 10.5); fixed inverted field_10 null check, 98->92 |
| 0x413B90 | `sound_obj::ProcessTrainCab` | 0x415C70 | `sub_412680`, `sub_412830`, `sub_4143B0` (10.5 0x413BE0), `sub_412A20` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413BE0 | `sound_obj::ProcessTrain` | 0x4143B0 | `sub_411940`, `sub_4BF210` (10.5 0x5AA3D0), `Car_BC::sub_41F880` (10.5 0x43B360), `sub_411870`, `sub_4116B0` (10.5 0x419020), `sub_4116F0` (10.5 0x419070), `sub_413DF0` (10.5 0x417D70), `sub_4166E0` (10.5 0x41A650), `sub_4B6020` (10.5 0x58DBF0), `sub_4171A0` (10.5 0x41A850), `sub_411890` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413BF0 | `sound_obj::ProcessTank` | 0x415CC0 | `sub_412BD0`, `sub_413160`, `sub_4156C0`, `sub_412D80`, `sub_4143B0` (10.5 0x413BE0) | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413C50 | `sound_obj::ProcessOtherCarTypes` | 0x415D20 | `sub_413160`, `sub_414030`, `sub_414200`, `sub_4148D0`, `sub_4143B0` (10.5 0x413BE0), `sub_415A90`, `sub_4139A0`, `sub_414780`, `sub_413EB0`, `sub_412EB0` | checked | dispatcher calling Handle*/Tank_* helpers; missing 9.6f callees are those helpers, unpaired (pairing noise) |
| 0x413D10 | `sound_obj::HandleAICarHornBeep` | 0x4123C0 | ✓ `sub_411950`, ✓ `IsMaxDamage_40F890`, ✓ `sub_411940` | matched | Car::TakeSoundEvent added; 54->54 |
| 0x4145E0 | `sound_obj::GetCar` | 0x4129D0 | ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x415480 | `sound_obj::PlayCarShopEventSound` | 0x412FE0 | ✓ `sub_4119D0` | done | Car::TakeShopEvent (new); code unchanged |
| 0x415570 | `sound_obj::HandleCarHornSound` | 0x415880 | ✓ `sub_4118F0`, ✓ `HasEmergencyLights_414F20`, ✓ `AreEmergencyLightsOn_414F80`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `?_GetResult@?$_Task_impl@_N@details@Concurrency@@QAE_NXZ`, ✓ `sub_411970`, ✓ `sub_411940`, ✓ `sub_411990` | done | Car::IsTurretRotated (new); code unchanged |
| 0x4177D0 | `sound_obj::HandleCarDamageSound` | 0x413BD0 | ✓ `Car_BC::sub_4118C0` | matched | all 9.6f inlines used |
| 0x417A00 | `sound_obj::Type_4_InitSirenSound` | 0x413D40 | ✓ `sub_411940`, ✓ `sub_411970` | done | all 9.6f inlines used |
| 0x417D70 | `sound_obj::get_samp_idx_for_car` | 0x413DF0 | ✓ `sub_411940` | matched | Use GetCarInfoIdx; 'a2 ? 27 : 28' gives the neg/sbb/add |
| 0x418B60 | `sound_obj::ProcessType11_HudPager` | 0x414E50 | ✓ `sub_411A40` | done | Hud_Pager::get_timer (new); code unchanged |
| 0x419FA0 | `sound_obj::AddSoundObject` | 0x417DB0 | ✓ `sub_416B40`, ✓ `ctor_416C00` | done | SoundPosEntry empty ctor (9.6f 0x416C00); GetType already used; code unchanged (ImGuiDebug.cpp shows a DIFF in an unmarked function that also showed before, unrelated) |
| 0x41A090 | `sound_obj::FreeSoundEntry` | 0x416C10 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x41A3F0 | `sound_obj::CalcVolume` | 0x416DA0 | ✓ `sub_410BF0` | done | Round_To_Int + s32 clamp + reuse a3 like 9.6f; 31->0 CANDIDATE MATCH |
| 0x41A730 | `sound_obj::InterrogateAudioEntities` | 0x417030 | ✓ `sub_416B70`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::sub_416B80`, ✓ `sub_416BB0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | Car::GetXYZ (new) + GetCarRotation + get_car + get_cam_z; 4->0 CANDIDATE MATCH |
| 0x41AB80 | `sound_obj::ProcessActiveQueues` | 0x417410 | `sub_414FA0`, `sub_4B6320`, `sub_4B62B0`, `sub_4B6150`, `sub_4B6110`, `sub_4B6130`, `sub_416F20`, `sub_4168E0`, `sub_416940`, `sub_4B62D0`, `sub_416E50`, `sub_4B6200`, `sub_4B6360`, `sub_4B6340`, `sub_4B63B0`, `sub_4B6190`, `sub_4B6170`, `sub_4B61E0` | checked | all pairing noise: 10.5 still calls cSampleManager wrappers, sub_412260 (=414FA0), VecDiff (=4168E0), FromFix16 (=416940), AdjustPlaybackRate (=416F20), sub_41A4A0 (=416E50) |
| 0x41B540 | `sound_obj::AdjustSamplesVolume` | 0x417CE0 | `sub_416940` | checked | 9.6f sub_416940 = FromFix16, already called |
| 0x422B70 | `sound_obj::ProcessPed` | 0x41B7B0 | `sub_4A65E0`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_41B090`, ✓ `sub_41B0A0`, ✓ `sub_410BF0` | done | get_velocity, is_player, Round_To_Int x3 (get_ped_state_2 already used); 192->175; sub_4A65E0 = GetPlayerCar (still called) |
| 0x423080 | `sound_obj::HandlePedVoiceEvent` | 0x41B0D0 | ✓ `sub_41B0B0`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::get_occupation_403980` | matched | Ped::TakeVoiceEvent (new) + is_player; 6->6 (remaining diff is register choice for rate + RandomDisplacement) |
| 0x426790 | `sound_obj::PoliceRadioMessageGeneration_426790` | 0x41C600 | `nullsub_4` (10.5 0x426670), `sub_4A65E0` | checked | nullsub_4 is an empty method (10.5 nullsub_74 0x426670, no body to add); sub_4A65E0 = GetPlayerCar, already called |
| 0x426F20 | `sound_obj::GenerateRadioVehicleDescription_426F20` | 0x41C350 | ✓ `sub_411940`, ✓ `sub_41C1F0` | done | all 9.6f inlines used |
| 0x42A500 | `sound_obj::ProcessType7_Weapon` | 0x41CCA0 | ✓ `sub_41CC80`, ✓ `sub_41CC70`, ✓ `sub_41CC90`, `sub_4CD8C0`, `sub_411730` | done | Weapon::TakeShotFired (new), IsTargetHit, GetWeaponType, position fields like 9.6f; 92->67; 4CD8C0/411730 still called in 10.5 |
| 0x42D870 | `UnknownDebugClass::DoBrianTest` | 0x41D0B0 | `sub_40E030` (10.5 0x58A190), ✓ `sub_41D020`, `sub_451510` | checked | sub_40E030 is a StartRoute copy and sub_451510 a large callee; source already calls the 10.5 functions, nothing open-coded |
| 0x430C70 | `ParseTokenAndPush` | 0x41D620 | `sub_41D580` | checked | 41D580 (160 bytes) is a helper 10.5 keeps out of line / pairing |
| 0x431E30 | `BonusTracker::Service` | 0x41DCC0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x431FE0 | `BonusTracker::AllocRule` | 0x41DE40 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4320D0 | `BonusTracker::ProcessBonusEvent` | 0x41DEE0 | ✓ `sub_41DC40` | done | Player::AddScore = 9.6f 0x41DC40 (comment added); 51->51 |
| 0x4355D0 | `Camera::IsSpriteTheCameraSubject` | 0x41E480 | ✓ `sub_40FEB0`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_40FEA0` | done | AsCar, get_car, AsCharB4; still matches |
| 0x4358D0 | `Camera::ComputeTargetFacingAngle` | 0x41E620 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_416BB0`, ✓ `CarPhysics_B0::is_backward_gas_on_411810` | done | all 9.6f inlines used |
| 0x435A20 | `Camera::ReturnOwnerVelocity` | 0x41DFC0 | `Car_BC::sub_421D90` | checked | sub_421D90 is GetCarLinearSpeed (pairing error, 10.5 calls it) |
| 0x435A70 | `Camera::IsCoordsPosVisible` | 0x41E710 | ✓ `DrawUnk_0xBC::WorldToScreen_40CFC0` | done | all 9.6f inlines used |
| 0x435D20 | `Camera::MoveTarget` | 0x41EA10 | `sub_41E540` | checked | sub_41E540 is 10.5 sub_435840 (called out of line) |
| 0x435FF0 | `Camera::Update` | 0x41F2F0 | `sub_4727E0`, `sub_41EB00` | checked | sub_4727E0 = SmoothApproachClamped, sub_41EB00 = sub_436140, both called in 10.5 |
| 0x436200 | `Camera::ApplyCarVelocityCameraOffset` | 0x41EBF0 | ✓ `Car_BC::sub_403BA0`, ✓ `Car_BC::sub_411900`, ✓ `sub_40F790`, ✓ `Car_BC::has_trailer_41E460`, ✓ `SetFromPolar_41E210` | done | GetAngle, IsTrailerCab, FromPolar_41E210; 98->97 |
| 0x4364A0 | `Camera::UpdateFollowCarCamera` | 0x41EE30 | ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `Max_41E130`, ✓ `sub_41E3D0` | matched | get_x/y/z, sub_41E130 used; SetPendingTarget commented (26->38); stays 26 |
| 0x436540 | `Camera::UpdateFollowPedCamera` | 0x41F410 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `Max_41E130`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_41E3D0` | done | GetPedState, sub_41E130 in ped branch; still matches |
| 0x436860 | `Camera::ApplyPanOffset` | 0x41F0D0 | ✓ `cool_nash_0x294::get_cam_z_416B50` | done | all 9.6f inlines used |
| 0x4368E0 | `Camera::ctor_4368E0` | 0x41F580 | `sub_41E3D0`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410` | checked | sub_41E3D0 = out-of-line sub_4397D0 in 10.5; CommitPendingTarget called directly from the ctor |
| 0x439CD0 | `Car_Door::AnimateOpening` | 0x421A00 | `maybe_flags::clear_420DE0`, ✓ `maybe_flags::set_420DC0` | commented | BitSet32 clear_bit/set_bit, but the param is u32* (changing it would change the signature) |
| 0x439D40 | `Car_Door::AnimateClosing` | 0x421A80 | `maybe_flags::clear_420DE0`, `maybe_flags::set_420DC0` | commented | same as 0x439CD0 |
| 0x439DA0 | `Car_Door::Service` | 0x421AF0 | ✓ `cool_nash_0x294::sub_403B60` | done | all 9.6f inlines used |
| 0x439F80 | `Car::GetCarStr` | 0x41F700 | `text_0x14::Find_4C23A0` | todo |  |
| 0x43A120 | `Car::GetMass` | 0x421CF0 | ✓ `HasPlayerDriver_4215C0`, ✓ `ApplyScale_421910` | done | all 9.6f inlines used |
| 0x43A3E0 | `Car::GetOrientationAngle` | 0x421E00 | ✓ `sub_4211E0` | done | new CarPhysics::GetLinvelAngle, code unchanged |
| 0x43A450 | `Car::GetLinearVelocity` | 0x421E70 | ✓ `Car_BC::sub_403BA0`, `sub_421150` | checked | 421150 = get_linvel (out-of-line in 10.5) |
| 0x43A6F0 | `Car::IsNotCurrentRemap` | 0x421F40 | ✓ `sub_41C1F0` | done | get_remap, code unchanged |
| 0x43A7D0 | `Car::AssignRandomRemap` | 0x422020 | ✓ `sub_4212D0`, ✓ `sub_420700` | done | GetRngValue, SetPaletteSprites, code unchanged |
| 0x43A850 | `Car::GetCarModelForPhysics` | 0x4220A0 | ✓ `Car_BC::HasNonPlayerDriver_4214F0`, ✓ `Car_BC::sub_403BA0` | done | HasNonPlayerDriver used, code unchanged |
| 0x43A9A0 | `Car::SetDriver` | 0x4221D0 | ✓ `sub_421270` | done | all 9.6f inlines used |
| 0x43A9F0 | `Car::TriggerCarAlarm` | 0x426F00 | ✓ `Car_BC::IsFlagSet_411930`, ✓ `IsMaxDamage_40F890` | todo |  |
| 0x43ADC0 | `Car::ProcessCarToCarImpact` | 0x42A360 | ✓ `sub_40FEB0`, ✓ `IsMaxDamage_40F890`, `sub_4230D0`, `sub_422F00`, ✓ `Car_BC::sub_411900`, `sub_4292F0`, ✓ `sub_4118B0`, ✓ `sub_41B0A0`, ✓ `sub_421170`, `sub_4BED60` | done | AsCar, is_player, new CarPhysics::set_linvel; code unchanged; 4230D0/422F00/4292F0 = IsAreaDamaged/DamageArea/ApplyImpactDamage (called) |
| 0x43AF10 | `Car::CanExitCar` | 0x4222A0 | `HasSpriteZoom_421D80` | checked | 421D80 = HasSpriteZoom (called) |
| 0x43AF40 | `Car::ResumeAIDriving` | 0x4222D0 | ✓ `sub_421550` | done | new Car::ClearAIHalted, code unchanged |
| 0x43AF60 | `Car::HaltAIDriving` | 0x4222F0 | ✓ `sub_421540` | done | SetAIHalted, code unchanged |
| 0x43AFE0 | `Car::IsDoorAccessible` | 0x425A40 | ✓ `sub_4204D0`, `sub_4BA7E0` | checked | 4BA7E0 is a 10.5 out-of-line call (pairing) |
| 0x43B140 | `Car::IsStoppedWithPavementAtDoor` | 0x425B60 | `Car_BC::sub_421D90`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_420420` | matched | PolarToCartesian already used; 421D90 = GetCarLinearSpeed |
| 0x43B2B0 | `Car::IsDoorLockedForPed` | 0x422360 | ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_target_to_enter_403B10` | done | GetInternalObjective, get_target_to_enter used (6 unchanged) |
| 0x43B540 | `Car::IsDoorOffsetLarge` | 0x4224A0 | ✓ `SignedPixelsToFix16_41FE70` | done | all 9.6f inlines used |
| 0x43B5A0 | `Car::GetDoorWorldPosition` | 0x422500 | ✓ `SignedPixelsToFix16_41FE70`, ✓ `RotateVector_41FC90` | done | all 9.6f inlines used |
| 0x43B770 | `Car::AssignKillerToOccupants` | 0x422670 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x43B7B0 | `Car::AssignDriverBlameForExplosion` | 0x425CA0 | ✓ `sub_420C30` | done | CANDIDATE MATCH (92->0); file-static IsPlayerPedId (9.6f method of unknown global 0x5E5BC0) + rewrite without gotos |
| 0x43BC30 | `Car::SetupOwnCarPhysicsAndSpriteBinding` | 0x425D90 | `Car_BC::sub_423A50`, `sub_4A1D30`, `CarPhysics_B0::sub_49EE10` (10.5 0x563670), ✓ `Car_BC::sub_403BA0` | checked | 423A50/4A1D30/49EE10 = AllocCarPhysics/SetCar/SetSprite (called in 10.5) |
| 0x43BD00 | `Car::DeAllocateCarPhysicsWithTrailer` | 0x426F40 | `sub_426120` | checked | 426120 = DeAllocateOwnCarPhysics (called) |
| 0x43C1C0 | `Car::PrepareForExplosion` | 0x426F60 | `sub_425ED0`, ✓ `HasEmergencyLights_414F20`, `sub_425E60` | inlines added | HasEmergencyLights used, code unchanged; 425ED0/425E60 = StopLightFlashing/HeadlightsOff (called) |
| 0x43C310 | `Car::ResetTopRightRoofLight` | 0x422980 | ✓ `sub_4216A0`, ✓ `sub_420E20`, ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00` | checked | inline_check_0x80/mask_bit/clear_bit already used; 4BE9C0/483A00 = FindObject2CByModel/TurnOffLight (called) |
| 0x43C650 | `Car::ResetRoofLights` | 0x422B70 | ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00`, ✓ `sub_4217A0`, ✓ `sub_4118F0` | checked | same as 0x43C310 |
| 0x43C840 | `Car::ResetBottomLeftRoofLight` | 0x422A40 | `sub_4216A0`, `sub_420E20`, ✓ `maybe_flags::clear_420DE0`, `sub_4BE9C0`, `sub_483A00` | checked | pairing to a different 9.6f function; calls match 10.5 |
| 0x43C920 | `Car::ActivateEmergencyLights` | 0x422D20 | ✓ `sub_411920`, ✓ `maybe_flags::set_420DC0` | done | all 9.6f inlines used |
| 0x43C9D0 | `Car::DeactivateEmergencyLights` | 0x422D80 | `sub_422CB0`, ✓ `sub_411920`, ✓ `IsMaxDamage_40F890`, ✓ `maybe_flags::set_420DC0` | checked | is_FBI/IsMaxDamage/set_bit used; 422CB0 = a called reset function |
| 0x43D2C0 | `Car::TryDamageArea` | 0x423180 | ✓ `IsMaxDamage_40F890`, `Car_BC::IsFlagSet_411930`, `sub_422F00` | commented | IsFlagSet flag helper (bool) breaks the match; 422F00 = DamageArea (called) |
| 0x43D400 | `Car::RepairDamage` | 0x425F20 | ✓ `maybe_flags::clear_420DE0`, ✓ `SetHeadlightDeltas_425590`, ✓ `SetBrakeLightDeltas_4213D0`, ✓ `ResetEngineStatus_421570` | done | HasPopupHeadlights x2, HasRoofLight, new Car::ResetEngineStatus; code unchanged |
| 0x43D690 | `Car::EmitExplosion` | 0x425FD0 | `Object_5C::sub_4852E0` (10.5 0x5299B0), `sub_424220` | checked | 9.6f 0x425FD0 is a different function (NewPhysicsObj + radio tower angle), pairing miss |
| 0x43D7B0 | `Car::TriggerExplosion` | 0x429370 | ✓ `IsMaxDamage_40F890`, `sub_423230` | done | CANDIDATE MATCH (16->0); 9.6f branch order, pass kZeroPoint.x/.y as Fix16 (was .mValue); 423230 = EmitExplosion |
| 0x43D840 | `Car::HandleCarExplosion` | 0x426FA0 | ✓ `IsMaxDamage_40F890`, `sub_423830`, `sub_423230`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_4207B0`, `sub_4B8A60` (10.5 0x5935D0), ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41B0A0` | done | CANDIDATE MATCH (21->0); is_player, temp passed to ApplyExplosionImpulse, unused Fix16_Point local removed (EH state); 423830=StartWreckCountdown, 4B8A60 etc pairing |
| 0x43DA90 | `Car::AccumulateDamage` | 0x427180 | ✓ `IsMaxDamage_40F890`, `sub_4226C0` | matched | 4226C0 = SpawnDamageFireEffect (pairing); fixed clamp logic per 9.6f, still 4 (anti_s load order) |
| 0x43DB80 | `Car::KillContainedPeds` | 0x423290 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_4212A0` | done | is_player, SetField8C_to_1; code unchanged |
| 0x43DD60 | `Car::DestroyWhileSinking` | 0x427290 | `sub_4232E0`, `sub_4233B0`, ✓ `cool_nash_0x294::get_occupation_403980`, `sub_4B8A60` (10.5 0x5935D0), ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41B0A0`, ✓ `SetEngineDamaged_421590`, ✓ `sub_420660`, ✓ `MarkForDespawn_421470` | done | CANDIDATE MATCH (24->0); new Car::SetEngineDamaged, set_z_lazy, MarkForDespawn, is_player; s16 gang idx, branch orders; 4232E0/4233B0 = StopMovement/CarShrinkSprite |
| 0x43EA60 | `Car::OnObjectTouched` | 0x4293D0 | ✓ `check_is_shop_421060`, ✓ `sub_421050`, ✓ `DespawnSoon_421490`, `sub_423480` | done | DespawnSoon used (if !driver), code unchanged; 421050 = GetPedRefIdx, 423480 = GetEffectiveDriver |
| 0x440510 | `Car::GetMaxTurnRate` | 0x4234A0 | `Car_BC::sub_421D90` | checked | 421D90 = GetCarLinearSpeed (called) |
| 0x440590 | `Car::InitCarAIControl` | 0x4279E0 | ✓ `sub_421260`, ✓ `sub_420B70`, ✓ `AllocateAI_421510`, ✓ `StartEngine_426E00` | done | new CarPhysics::SetHandBrakeOff, StartEngine; 421510 open-coded (AI alloc); code unchanged |
| 0x4405F0 | `Car::SpawnDriverPed` | 0x423510 | ✓ `sub_421960`, ✓ `sub_421970` | done | new CarManager::GetSpareDriver/SetSpareDriver; code unchanged |
| 0x4406E0 | `Car::AssignDriver` | 0x427A20 | ✓ `sub_420B70`, ✓ `Car_BC::SetUniNum_421560`, ✓ `CancelDespawn_4215F0`, ✓ `sub_4218F0`, ✓ `sub_420B60`, ✓ `sub_4218E0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_4212B0` | done | SetDriverPedType, new CancelDespawn, IsCarThreadPending, ClearCarThreadPending, SetField8C_to_2; code unchanged |
| 0x4407F0 | `Car::ClearDriver` | 0x4235D0 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Garox_2B00::sub_4219F0`, ✓ `sub_4212A0`, ✓ `Car_BC::sub_421460` | inlines added | new ClearHorn, SetField8C_to_1; Garox_2B00::sub_4219F0 (hud display time = 0) left open-coded; code unchanged |
| 0x440C10 | `Car::RotateRoofObjectTowardTarget` | 0x423630 | ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, `sub_40EAB0`, `sub_40EC80` | done | 96->6; IsFireTruck/IsTank/IsGunJeep if-chain, 9.6f branch order; left: Ang16 -= normalize codegen in one branch; 40EAB0/40EC80 = 4056C0/IsAngleAhead |
| 0x440F90 | `Car::FireCarBomb` | 0x4295C0 | ✓ `sub_420B50`, ✓ `sub_420F10`, `sub_482790` | done | GetPedId used, code unchanged; 482790 = SetDamageOwner (called) |
| 0x441030 | `Car::GoToBlockTest` | 0x4237B0 | ✓ `AllocateAI_421510` | checked | 421510 AI alloc left open-coded (Car.hpp can't see gCarAI_78_Pool); no change |
| 0x441330 | `Car::GetZPos` | 0x420270 | `sub_49F510` | checked | 49F510 = ComputeZPosition (called) |
| 0x4413B0 | `Car::UpdateTrainCarriagesOnTrack` | 0x429680 | `sub_467110`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690` | matched | 467110 = Map_0x370::AdvanceAlongRailway (called in 10.5) |
| 0x441520 | `Car::UpdateEngineStatus` | 0x426030 | `sub_425E30`, `sub_425E60` | checked | 425E30/425E60 = HeadlightsOn/HeadlightsOff (called) |
| 0x4418B0 | `Car::FlashAndDetachTrailer` | 0x427B20 | `Car_BC::sub_425DF0` | done | add_f78_bits_421890(1), code unchanged; 425DF0 = FlashHeadlights (called) |
| 0x4418D0 | `Car::HandleUserInput` | 0x429810 | `Car_BC::sub_423720`, `Car_BC::sub_4260C0`, ✓ `Car_BC::IsAcceleratorJammed_425780` | commented | 4->2 (bool test of HandleRoofTurretRotation result); IsAcceleratorJammed_425780/411930 flag helper worsens (4->14) so commented; 423720/4260C0 = HandleRoofTurretRotation/HandleSpecialInput (called) |
| 0x441A70 | `Car::InitDoors` | 0x423AA0 | ✓ `Car_10::set_obj_421380`, ✓ `sub_421340` | done | new Car_Door::ResetState, code unchanged |
| 0x441E70 | `Car::UpdateLights` | 0x426140 | `sub_423B80`, `sub_425ED0`, ✓ `SetHeadlightDeltas_425590`, ✓ `SetBrakeLightDeltas_4213D0`, ✓ `ClearHeadlightDeltas_425650`, ✓ `ClearBrakeLightDeltas_421430`, ✓ `inline_check_0x2_info_421700`, `sub_423C30`, ✓ `Car_BC::inline_check_0x20_info_4216C0`, `sub_423B50`, ✓ `sub_411920` | checked | 423B80/425ED0/423C30/423B50 = called UpdateRoofLightFlasher/StopLightFlashing etc (pairing) |
| 0x4421B0 | `Car::IsMissionRelated` | 0x420450 | `TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` (10.5 0x445EC0) | checked | 445EC0 listed is HasScriptCreatedPed (pairing) |
| 0x442200 | `Car::CanDespawn` | 0x424030 | ✓ `IsMaxDamage_40F890`, ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x442310 | `Car::ManageDespawning` | 0x424090 | ✓ `Car_BC::sub_403BA0`, ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_421790`, ✓ `sub_4118F0`, ✓ `sub_4217A0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `sub_411920`, ✓ `IsMarkedForDespawn_4214B0`, ✓ `MarkForDespawn_421470` | done | all 9.6f inlines used |
| 0x4424C0 | `Car::UpdateCarDespawnStatus` | 0x4241C0 | `sub_424010` | checked | 424010 = IsCarInConditionsToDespawn (called) |
| 0x4425D0 | `Car::ManageTVAntenna` | 0x424280 | `sub_424220`, `angle::angle_not_equal_41CFF0`, ✓ `sub_40F580`, ✓ `sub_40F540` | done | 44->39; Ang16_to_Fix16 temp + zero copy order as in 9.6f; 424220 = GetRadioTowerAngle |
| 0x4426D0 | `Car::ServiceDoorsLightsAndHorn` | 0x426220 | ✓ `IsMaxDamage_40F890`, `sub_423850`, ✓ `sub_4217E0`, ✓ `Car_BC::sub_4118C0` | checked | 423850 = UpdateHorn (called) |
| 0x442760 | `Car::DetachTrailer` | 0x426270 | ✓ `Car_BC_Pool::sub_420F20`, ✓ `TrailerPool::sub_425580` | done | CarPool::UpdateNextPrev, new TrailerPool::Remove; code unchanged |
| 0x442810 | `Car::TryHitchTrailer` | 0x4262E0 | ✓ `sub_40F7B0`, `HasSpriteZoom_421D80`, ✓ `IsMaxDamage_40F890`, ✓ `IsMarkedForDespawn_4214B0`, ✓ `sub_4215B0`, ✓ `sub_40FEB0`, `sub_421C40`, `sub_40F600`, `sub_4262B0`, `Car_BC::sub_425DF0`, ✓ `sub_421130`, `sub_420390`, `sub_4207B0`, ✓ `sub_40F820`, ✓ `sub_40F580` | inlines added | 133->129; new CarPhysics::IsDamageWindowOpen; get_theta worsens (134) so not used; 421C40/4262B0/425DF0/420390/421D80 = GetHitchPoint/AttachTrailer/UpdateDamageFlags_43BD40/NormalizeSafe/HasSpriteZoom |
| 0x443170 | `Car::PoolUpdate` | 0x429E50 | ✓ `Car_BC::sub_403BA0`, `Car_BC::sub_4298B0`, ✓ `sub_420B70`, ✓ `sub_414F70`, `sub_427450`, `sub_421760`, `Car_BC::sub_421830` | checked | 4298B0/427450/421760/421830 are big or called helpers; 414F70 (flag 0x2000 via 411930) left open-coded (flag helper changes code elsewhere) |
| 0x4435B0 | `Car::GetAttachedDrawLayer` | 0x4243C0 | ✓ `Car_BC::IsFlagSet_411930`, `CanCollideOver_4216E0` | checked | CanCollideOver (via get_car_info_4BF1F0) and the 411930 flag test both change the code; left as is |
| 0x4435F0 | `Car::UpdateDrawLayer` | 0x424400 | ✓ `sub_421640`, ✓ `Car_BC::IsFlagSet_411930`, ✓ `sub_40F7B0`, `CanCollideOver_4216E0` | inlines added | IsTrailerInfo used; CanCollideOver breaks the match; code unchanged |
| 0x443710 | `Car::ApplyExplosionImpulse` | 0x426580 | ✓ `IsDeactivated_4214D0`, ✓ `sub_4207B0`, `sub_40F600`, `sub_420390` | checked | IsDeactivated, get_x_y, operator-, NormalizeSafe already used; 9.6f order same |
| 0x4438C0 | `Car::BuyCarWeapon` | 0x4244A0 | ✓ `sub_421870`, `sub_4219A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_421980`, ✓ `sub_421990`, ✓ `sub_421950` | inlines added | new GetDriverPlayer; 4219A0/421980/421990 Player helpers not added; code unchanged |
| 0x443AB0 | `Car::ShowCantAffordMessage` | 0x424470 | ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x443AE0 | `Car::ResprayOrChangePlates` | 0x427BD0 | ✓ `sub_421870`, ✓ `sub_421980`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_421950`, ✓ `sub_421990`, ✓ `cool_nash_0x294::sub_420B80` | inlines added | GetDriverPlayer; code unchanged |
| 0x443BD0 | `Car::ResprayOrCleanPlates` | 0x427CA0 | `sub_4257B0`, ✓ `sub_421870`, ✓ `angry_lewin_0x85C::sub_41DC70` | inlines added | GetDriverPlayer; 4257B0 not checked; code unchanged |
| 0x443C40 | `Car::HandleShops` | 0x427D10 | ✓ `HasPlayerDriver_4215C0`, `sub_421020`, `sub_420760` | done | HasPlayerDriver, GetDriverPlayer; code unchanged; 421020/420760 = GetPedRefIdx/ObjectTypeToWeaponType |
| 0x443D00 | `Car::SetPosition` | 0x4207F0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `CarPhysics_B0::sub_49EE10` (10.5 0x563670) | checked | 49EE10 = SetSprite (called), set_xyz_lazy already used |
| 0x443D70 | `Car::IncrementCarStats` | 0x424630 | ✓ `Car_BC::has_trailer_41E460` | done | IsTrailerCab, code unchanged |
| 0x443F30 | `Car::AttachLight` | 0x420950 | `sub_483A00` | checked | 483A00 = TurnOffLight (called) |
| 0x444090 | `Car::GetEffectiveDriverPedId` | 0x4246A0 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_420C30` | done | IsPlayerPedId used, code unchanged |
| 0x4441B0 | `Car::SetSirens` | 0x424700 | ✓ `sub_421780`, ✓ `sub_421790`, ✓ `sub_4217A0`, ✓ `sub_4118F0`, ✓ `sub_411920` | done | IsMediCar/IsCopCar/IsSwatVan/IsFireTruck/is_FBI_car tests, code unchanged |
| 0x444490 | `Car::PoolAllocate` | 0x424880 | ✓ `sub_420D90`, ✓ `Ped_Unknown_4::ClearList_420E90`, ✓ `Car_BC::sub_421460` | done | ClearAllBits/ClearList already, ClearHorn now used; code unchanged |
| 0x4446E0 | `Car::DeAllocateAI` | 0x4266F0 | ✓ `sub_425460` | checked | 425460 = pool DeAllocate (already) |
| 0x4447D0 | `Car::PoolDeallocate` | 0x426730 | `sub_426120`, ✓ `SpritePool::sub_421030`, `Car_BC::sub_4D0700` | checked | 426120 = DeAllocateOwnCarPhysics, 421030 = SpritePool::remove (already) |
| 0x444980 | `CarManager::DistributeCarsByRating` | 0x420A10 | ✓ `gtx_0x106C::sub_420200`, `sub_4BFFE0` | done | does_car_exist (420200) used; 4BFFE0 = IsCarModelInRecycleList (called); code unchanged |
| 0x444AB0 | `CarManager::SelectTrafficCarModel` | 0x424980 | ✓ `sub_4212D0` | done | GetRngValue used; code unchanged |
| 0x444E40 | `Car::SnapCarToGreenArrow` | 0x4268C0 | `sub_424FF0` | checked | 424FF0 (948 bytes) = TrySnap/other big call (pairing) |
| 0x444FC0 | `CarManager::DoGetNearestCarFromCoord` | 0x424BD0 | ✓ `sub_420E50`, ✓ `IsMaxDamage_40F890`, ✓ `sub_421640`, ✓ `HasSpriteZoom_421D80`, ✓ `IsDeactivated_4214D0`, ✓ `IsCarInAir_421DF0`, ✓ `sub_411940`, ✓ `Car_BC::sub_403BA0` | inlines added | 218->216; new CarPool::GetFirstCar_420E50; fixed TRAINFB test (!=); 421D80/421DF0 = HasSpriteZoom/IsCarInAir; MapRenderer 4EFDB0 +2 jitter |
| 0x4451E0 | `Car::CanBeEnteredByPed` | 0x424E30 | ✓ `sub_421780` | done | CANDIDATE MATCH (5->0); IsMediCar = 9.6f 0x421780 (comment), return via ternary |
| 0x445210 | `CarManager::GetNearestFrontVehicle` | 0x424E70 | ✓ `PolarToCartesian_41FC20`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_421610`, `Car_BC::sub_421D90` | checked | PolarToCartesian_41FC20 already used; 421D90 = GetCarLinearSpeed |
| 0x445360 | `Car::IsEnterable` | 0x424F80 | ✓ `IsMaxDamage_40F890`, ✓ `sub_421640`, `HasSpriteZoom_421D80`, ✓ `IsDeactivated_4214D0`, `IsCarInAir_421DF0` | done | CANDIDATE MATCH (6->0); IsTrailerInfo; model check is boxcar/TRAINFB/TRAINCAB only (not IsTrainModel) |
| 0x4453E0 | `CarManager::SpawnBusAtValidRoadPosition` | 0x427D60 | `sub_40C810`, ✓ `sub_41FE40`, ✓ `EnqueueRadioLocationPhrase_426E10` | checked | 40C810 (switch around HasBlockArrowInRoadDirection) open; SwapIf3or4 already used |
| 0x4458B0 | `CarManager::SpawnCarOnRoadNetwork` | 0x428540 | `sub_40C810`, ✓ `sub_41FE40`, ✓ `EnqueueRadioLocationPhrase_426E10`, ✓ `AllocateAI_421510` | done | 239->205; SpawnCar_426E10_v2 used; 40C810 = switch around HasBlockArrowInRoadDirection (already open), SwapIf3or4 already used, 421510 open-coded |
| 0x445EC0 | `Car::TrySnapCarToNearestDrivableRoadAndDriveForward` | 0x445EC0 | `sub_420B70` | matched | 181->148; pairing wrong (9.6f addr=10.5 addr), but used SwapIf3or4, SetBounds/SetHiLowZ, Accelerate |
| 0x446230 | `CarManager::SpawnCarAt` | 0x426AC0 | ✓ `sub_41FEA0`, `sub_4254A0`, ✓ `sub_41FF00`, `sub_425480`, ✓ `SpritePool::sub_421000`, `sub_466B70` (10.5 0x466B70), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_420710`, `sub_4BDEF0`, ✓ `inline_check_0x40_info_421680`, ✓ `sub_4217D0`, ✓ `sub_421810`, ✓ `sub_421790`, ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `AddFlags_421890`, ✓ `sub_4218C0`, ✓ `sub_4218D0`, ✓ `sub_4218A0`, ✓ `Car_BC::sub_411910`, ✓ `sub_4217E0`, `sub_425FD0` (10.5 0x43D690), ✓ `Car_BC::sub_403BA0`, ✓ `sub_4218B0`, ✓ `sub_4212D0`, `sub_422E00`, ✓ `sub_411920` | matched | all listed callees already used or called in 10.5 (pool alloc, ApplyScaleToDimensions, PutTV_Antenna, SetCarGraphic) |
| 0x446530 | `CarManager::SpawnCabAndTrailer` | 0x428EC0 | ✓ `sub_426E40`, `sub_40FD40`, ✓ `UnlinkFromActiveList_420F30`, `sub_425570` | checked | 426E40/420F30/425570 already used; 40FD40 = sub_408370 (called) |
| 0x446790 | `CarManager::CarsService` | 0x42A4C0 | `sub_433160`, `Car_BC_Pool::sub_42A2D0` | checked | 433160/42A2D0 = Taxi PopAll/pool UpdatePool (called/inline already) |
| 0x4469F0 | `CarManager::ctor_4469F0` | 0x428FA0 | ✓ `Car_BC_Pool::ctor_426DB0`, `CarPhyisicsPool::ctor_420F80`, ✓ `CarAI_78_Pool::ctor_420EB0`, ✓ `TrailerPool::ctor_425500`, ✓ `SpriteList_4::ResetHead_4207E0`, `Car_3C::ctor_4205A0` | checked | already 0 diff (candidate match?); CarPhyisicsPool ctor already inline, Car_3C ctor = Sprite ctor (new Sprite()) |
| 0x446DC0 | `CarManager::dtor_446DC0` | 0x45A9D0 | `CarManager_6C::dtor_429250` | checked | 9.6f 0x45A9D0 is the deleting-dtor wrapper of 0x429250 = this dtor; already 0 diff |
| 0x447710 | `CarAI::PlanTurnAtNextJunction` | 0x4300E0 | ✓ `sub_42ACA0`, ✓ `sub_42ACB0`, ✓ `sub_42AC80`, ✓ `sub_42AC20`, ✓ `sub_42AC40`, `sub_42FE60`, ✓ `sub_42AC90`, ✓ `sub_42AC70`, ✓ `sub_42AC30`, ✓ `sub_42AC50`, ✓ `sub_421530` | done | CANDIDATE MATCH (59->0); field_A6 bit helpers added to Car.hpp, also |
| 0x447970 | `CarAI::DoShortcutsUsingJunctions` | 0x430320 | `sub_42AB90`, ✓ `sub_40CEE0` | inlines added | TurnAntiClockwise used (286 unchanged); ContainsPoint (40CEE0) commented, worsens 286->298 |
| 0x447CA0 | `CarAI::GoToBlock` | 0x42ACF0 | `sub_40E0F0` | checked | 40E0F0 (5 bytes) is a Fix16 ctor (operator) |
| 0x4482C0 | `CarAI::BrakeForBlockedRoadAhead` | 0x42B350 | `sub_4221A0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, ✓ `sub_416B40`, `Car_BC::sub_421D90`, ✓ `Car_3C::set_ang_lazy_420690` | done | 393->315; PolarToCartesian_41FC20, GetType used; 4221A0/421D90 are DoBrake/GetCarLinearSpeed (pairing, called in 10.5) |
| 0x448770 | `CarAI::CheckRoadAhead` | 0x430650 | ✓ `PolarToCartesian_41FC20`, `sub_42A5D0`, ✓ `sub_42A810`, `sub_42FDF0`, ✓ `sub_42AC30`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, `sub_4221A0`, ✓ `sub_42AC20`, `sub_42ADA0` | inlines added | 447->48; PolarToCartesian_41FC20 x3, IsA6Bit0/1Set used; 42A810 (gtx spec==3 check) not added yet; 42A5D0/42FDF0/42ADA0 = IsClockwiseTurning/ChooseRandomTurn/BrakeForBlockedRoadAhead (called in 10.5) |
| 0x448CE0 | `CarAI::ManageTrafficCarDirection` | 0x430CC0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, `sub_42FE40`, ✓ `sub_42AB90`, ✓ `sub_42ABB0`, ✓ `sub_42ABA0`, `sub_42B8A0` | commented | 42FE40=ClearSteeringIfTurning (10.5 calls once, rest inlined); inline copy for calls 2-4 worsens 1295->1346; 42B8A0 big callee not identified |
| 0x44E0C0 | `CarAI::UpdateChase` | 0x42D390 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | done | Ped get_cam_x/y/z used (285 unchanged); MaxAbsDistance noted only: 10.5 calls Abs_436A50/Max_44E540 out of line |
| 0x44E560 | `CarAI::UpdateStateMachine` | 0x42D820 | `sub_4463C0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `PolarToCartesian_41FC20`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `sub_40E8D0`, `sub_42A5B0`, `sub_421C00`, `sub_446690`, `sub_446550`, `sub_446480`, `sub_4221A0`, `sub_40EC80`, ✓ `sub_42AB90`, ✓ `sub_42ABA0`, `sub_40EAB0`, ✓ `sub_421250`, ✓ `sub_421540`, ✓ `GetOppositeFace_42A660`, ✓ `sub_42ABB0`, `sub_42A620`, `sub_4BD670`, `Car_BC::sub_421D90`, ✓ `sub_421210`, `sub_446410`, `sub_446620`, `sub_4464E0`, `sub_4465B0` | inlines added | 5010->4843; TurnClockwise/AntiClockwise, SetGoStraight, new SetHandBrakeOn, SetAIHalted; Accelerate worsens (4863) so not used; PolarToCartesian sites (many += into globals) and 4463C0..4465B0 helpers not done |
| 0x451980 | `CarAI::ReactToNearbyCar` | 0x431770 | `sub_40EAB0`, `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `sub_42C8B0`, `sub_4221A0`, ✓ `sub_42AC00` (10.5 0x453F90), `sub_423940`, `Car_BC::sub_421D90`, ✓ `ForceNeutralInput_42ABC0` (10.5 0x453F50), ✓ `sub_421210`, ✓ `MarkForDespawn_421470`, `sub_40E8D0`, ✓ `sub_42ABA0`, ✓ `sub_42AB90`, `sub_430090`, `sub_4221B0` | checked | inlines already used; remaining (40EAB0/40E8D0/42C8B0/423940/421D90/4221A0/4221B0/430090) are 10.5 out-of-line calls (4056C0/405320/44D1D0/4416D0/43A240/43A950/43A970/453C00) |
| 0x452A20 | `CarAI::ManageCollisions` | 0x42FB80 | ✓ `sub_4215B0`, ✓ `SetFromPolar_41E210`, ✓ `sub_42A720`, ✓ `sub_42AC00` (10.5 0x453F90), ✓ `sub_42AB90`, ✓ `sub_42ABA0`, ✓ `sub_421210` | done | all 9.6f inlines used |
| 0x452DF0 | `CarAI::UpdateDrivingAI` | 0x432370 | ✓ `sub_42ABB0`, ✓ `sub_42AB90`, ✓ `sub_42ABA0`, `sub_431C10`, ✓ `sub_416B40`, ✓ `sub_40FEC0` | matched | only 431C10 (1855 bytes) missing, = a CarAI method 10.5 still calls |
| 0x453470 | `CarAI::UpdateDriving` | 0x4326E0 | ✓ `sub_421260`, ✓ `sub_4211A0`, ✓ `sub_421540`, ✓ `sub_421550`, ✓ `sub_42AC60`, `sub_4221A0`, `sub_4221B0`, ✓ `sub_42AC00` (10.5 0x453F90) | done | SetHandBrakeOff, SetA6Bit5/ClearA6Bit5, new IsAIHalted, NeutralGear; code unchanged; GetLength_453590 kept (out-of-line in 10.5); 4221A0/4221B0 = DoBreak/DoBrakeAndHandbrake (called) |
| 0x453A40 | `CarAI::UpdateTrainMovement` | 0x42FF20 | `sub_467110`, ✓ `sub_40F580`, `WrapAngle_40E790`, `sub_42FE80` | checked | 467110/40E790/42FE80 = AdvanceAlongRailway/... (called); Ang16_to_Fix16 already used |
| 0x4546D0 | `CarInfoTable::LoadModelPhysics` | 0x432D10 | ✓ `gtx_0x106C::get_car_info_count_432850`, `sub_402B20` (10.5 0x4A6DB0), `array_constuctor_401CF0` | checked | get_number_of_cars (432850) used; 402B20 = File::sub_4A6DB0 copy; array ctor is compiler-generated |
| 0x454850 | `CarInfoTable::CalculateAllCarInfo` | 0x432EA0 | ✓ `gtx_0x106C::get_car_info_count_432850`, ✓ `gtx_0x106C::sub_420200`, `sub_432AA0` | checked | get_number_of_cars, does_car_exist used; 432AA0 = CalculateCarInfo (called) |
| 0x4549C0 | `CarInfoTable::ConvertAllMass` | 0x432CD0 | ✓ `gtx_0x106C::get_car_info_count_432850`, `sub_432C60` | checked | get_number_of_cars used; 432C60 = ConvertMass |
| 0x454AA0 | `CarInfoTable::Free` | 0x433000 | `Monster_2C::gdtor_432F70` | checked | 432F70 is the compiler vector deleting dtor |
| 0x454B00 | `CarInfoTable::ctor_454B00` | 0x432830 | `Monster_808::sub_432810` | checked | 432810 = Clear |
| 0x457BA0 | `TaxiList::PushTaxi` | 0x433140 | `sub_433120` | checked | 433120 = pool Allocate (already inline) |
| 0x457BF0 | `TaxiList::GetTaxiNear` | 0x4330A0 | ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_4215B0` | done | all 9.6f inlines used |
| 0x45AE40 | `abs_sub_less_than_epislon` | 0x45AE40 | `Zone_144::init_45D960` (10.5 0x4BED70) | checked | Zone_144::init_45D960 is a pairing error |
| 0x45BC10 | `Ped::TeleportToCoord` | 0x435C80 | ✓ `cool_nash_0x294::get_car_416B60` | done | all 9.6f inlines used |
| 0x45BC70 | `Ped::ManageShocking` | 0x435CE0 | ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_4338D0`, `sub_4CEF40` | done | code unchanged |
| 0x45BD20 | `Ped::CloseDoorIfCarApproaching` | 0x435D90 | `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity |
| 0x45BE30 | `Ped::GetBulletSpriteOffset` | 0x435E40 | ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x45BEC0 | `Ped::ManageBurning` | 0x444A70 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `sub_434950`, ✓ `sub_41B0A0` | done | code unchanged |
| 0x45C350 | `Ped::RespawnPed` | 0x43E140 | ✓ `Char_B4_Pool::DeAllocate`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0` | commented | SetRemap breaks the match |
| 0x45C410 | `Ped::ResetForPlayerRespawn` | 0x435FA0 | ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::sub_403920` | done | all 9.6f inlines used |
| 0x45C650 | `Ped::SpawnDriverRunAway` | 0x43E270 | ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | todo |  |
| 0x45C830 | `Ped::AllocCharB4` | 0x4360C0 | `Char_B4_Pool::sub_4355A0`, `Char_B4::sub_433880`, ✓ `Char_B4::sub_4338E0` | checked | CharPool 4355A0 = mPool.Allocate; Char::sub_433880 (sprite xyz + alloc + region buckets) not added, it needs SpriteGrid and Ped.cpp globals in char.hpp |
| 0x45C920 | `Ped::GetPedVelocity` | 0x436160 | ✓ `sub_41B080` (10.5 0x41B480), `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity |
| 0x45C960 | `Ped::GetRotation` | 0x4361B0 | ✓ `sub_433A40` | done | code unchanged |
| 0x45C9D0 | `Ped::ComputeAimAngle` | 0x43E3A0 | `sub_445CC0`, `sub_43BEC0`, `sub_437EB0`, `sub_40E8D0` | checked | all missing are pairing noise (4713C0/466BB0/466F40 wrappers, atan2) |
| 0x45CAA0 | `Ped::HandleClosePedInteraction` | 0x444B50 | ✓ `cool_nash_0x294::get_cam_z_416B50`, `sub_435610`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_433E50`, ✓ `sub_433B70`, ✓ `sub_41DC40`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_433A60`, ✓ `cool_nash_0x294::sub_403AE0` | matched | diff 36->6; TriggerVoiceEventRateLimited fixed to unsigned compare; 435610 = abs_sub_less_than_epislon |
| 0x45DD30 | `Ped::AddWeaponWithAmmo` | 0x43E4B0 | ✓ `sub_433820` | done | code unchanged |
| 0x45DE80 | `Ped::HandlePickupCollision` | 0x43E550 | ✓ `sub_434B10`, ✓ `sub_434B20`, ✓ `sub_434B60`, ✓ `sub_434A10`, ✓ `sub_421050`, ✓ `sub_4340A0`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | diff 24 unchanged; new TagGame/GameSession helpers; 421050/4340A0 = get_field_26 already |
| 0x45E080 | `Ped::SpawnWeaponOnDeath` | 0x436250 | ✓ `sub_434130` | matched | diff 149->145; Object_5C.hpp includes Object_8.hpp now (CarAI WIP 44E560 5009->5010 from that include alone) |
| 0x45E4A0 | `Ped::StartCrossingRoad` | 0x43B7C0 | ✓ `sub_433470`, ✓ `sub_4334A0`, ✓ `sub_4334D0`, ✓ `sub_433500` | done | all 9.6f inlines used |
| 0x45EA00 | `Ped::DeallocateWithGroupCleanup` | 0x441F10 | `cool_nash_0x294::sub_403A30` | checked | Target keeps the 'all gone' loop index in cl and pFirst in edi; ours keeps the index in memory |
| 0x45EB60 | `Ped::Deallocate` | 0x43E650 | ✓ `sub_434070`, ✓ `Car_BC::sub_4343B0` | done | all 9.6f inlines used |
| 0x45EE70 | `Ped::EnterPublicTransport` | 0x43B9D0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x45F360 | `Ped::Mugger_AI` | 0x43E8B0 | ✓ `cool_nash_0x294::sub_4039F0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::has_car_403B80` | done | code unchanged |
| 0x45FF60 | `Ped::CarThief_AI` | 0x442050 | ✓ `sub_4215B0`, ✓ `Car_BC::sub_403BA0`, ✓ `Char_8::sub_420EA0`, ✓ `cool_nash_0x294::sub_433DD0` | done | diff 134 unchanged |
| 0x460820 | `Ped::TaxiCustomer_AI` | 0x442420 | ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `Char_8::sub_420EA0`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403920`, `Car_BC::sub_421EC0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | inlines added | diff 318 unchanged; MaxAbsDistance commented (worse); 421EC0 = GetVelocity (noise) |
| 0x4614E0 | `MaxAbsDistance_4614E0` | 0x42A6B0 | `Max_41E130` | todo |  |
| 0x461530 | `Ped::TrainCustomer_AI` | 0x43EAF0 | ✓ `cool_nash_0x294::sub_403920`, `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity |
| 0x461630 | `Ped::RobbedDriver_AI` | 0x442A40 | ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_403920`, ✓ `Car_BC::SetUniNum_421560` | done | diff 130 unchanged |
| 0x4619F0 | `Ped::RoadBlockTank_AI` | 0x43EBC0 | ✓ `sub_421540` | done | code unchanged |
| 0x461A60 | `Ped::UpdateFacingAngle` | 0x436460 | `sub_40E8D0`, `sub_40EAB0`, ✓ `Char_B4::sub_433920`, ✓ `sub_40F540` | checked | 40E8D0 = atan2_fixed, 40EAB0 = ComputeShortestAngleDelta (pairing noise) |
| 0x461F20 | `Ped::Occupation_AI` | 0x442DE0 | ✓ `sub_434970`, `sub_4427E0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_403960`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0` | done | code unchanged; new TaxiList::IsEmpty |
| 0x462280 | `Ped::UpdateAI` | 0x445330 | ✓ `Get_F3C_433370` | done | all 9.6f inlines used |
| 0x462510 | `Ped::RemovePedWeapons` | 0x436830 | ✓ `cool_nash_0x294::sub_403A40`, `Weapon_8::sub_4D06E0` | done | code unchanged; 4D06E0 = deallocate |
| 0x462550 | `Ped::RemoveSecondaryWeapon` | 0x436860 | ✓ `cool_nash_0x294::sub_403A40`, `Weapon_8::sub_4D06E0` | done | code unchanged; 4D06E0 = deallocate |
| 0x462620 | `Ped::UpdateOnScreenCounter` | 0x436890 | ✓ `Char_B4::sub_433A80`, ✓ `sub_4338F0`, `sub_491EA0` | done | code unchanged |
| 0x4626B0 | `Ped::StateMachineTick` | 0x43ECC0 | ✓ `sub_433810`, ✓ `Char_B4::sub_433A80`, ✓ `sub_4338F0`, `sub_491EA0`, `ApplyCarVelocityCameraOffset_436200`, `sub_436140`, `angle::angle_not_equal_41CFF0`, ✓ `sub_433C20`, ✓ `sub_4215B0` | matched | diff 187->177; 491EA0/436200/436140 pairing noise |
| 0x462B80 | `Ped::UpdateCharB4` | 0x43F180 | ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `sub_4339C0`, ✓ `sub_4339E0`, ✓ `sub_433A00`, ✓ `sub_433A40`, ✓ `sub_403900`, ✓ `Char_B4_Pool::DeAllocate`, `sub_423590`, ✓ `Car_10::set_obj_421380` | done | code unchanged; new Char::Get_F84 |
| 0x462E70 | `Ped::PoolUpdate` | 0x4454E0 | ✓ `sub_433C90`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_4215B0`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | code unchanged |
| 0x4633E0 | `Ped::SetStatesForObjective` | 0x433650 | `cool_nash_0x294::sub_433580` | checked | cool_nash_0x294::sub_433580 is ChangePedStatesByMode, which 10.5 calls (pairing noise) |
| 0x463570 | `Ped::SetObjective` | 0x43BBC0 | `cool_nash_0x294::sub_433580` | matched | cool_nash_0x294::sub_433580 is ChangePedStatesByMode, which 10.5 calls (pairing noise) |
| 0x463830 | `Ped::SetObjective2` | 0x436920 | ✓ `Car_10::set_obj_421380`, `cool_nash_0x294::sub_433580` | checked | cool_nash_0x294::sub_433580 is ChangePedStatesByMode, which 10.5 calls (pairing noise) |
| 0x463AA0 | `Ped::ProcessMainObjective` | 0x443170 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `sub_4215B0`, `sub_493940`, ✓ `sub_4340D0`, ✓ `sub_4340E0`, ✓ `sub_4340F0`, `sub_433EB0`, `sub_433F40`, `sub_433FE0`, `sub_43BEE0`, `sub_43BF60`, `sub_4388E0`, `sub_434380`, `sub_439640` | checked | the big callees (433EB0..439640, 493940) are case bodies already open-coded in the matching switch; Object getters already used |
| 0x463FB0 | `Ped::ProcessInternalObjective` | 0x444D00 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, `sub_436BF0`, `sub_493940`, ✓ `sub_4340D0`, ✓ `sub_4340E0`, ✓ `sub_4340F0`, `cool_nash_0x294::sub_439D30` | checked | already uses the cam/Object getters; 436BF0/493940/439D30 are CalcApproachPointNearTargetPed/CarDoorAlignmentSolver/StayCloseToTargetPed (pairing noise) |
| 0x465270 | `Ped::Threat_Reaction_AI` | 0x43F340 | `sub_43BED0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `sub_41CC90`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `cool_nash_0x294::sub_403AE0`, `sub_40E8D0`, ✓ `sub_433A30`, `sub_43BEB0`, `Car_BC::sub_421EC0`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, `Mouze_44::sub_403D20`, `Mouze_44::sub_403D10`, ✓ `cool_nash_0x294::get_car_416B60`, `sub_404900`, ✓ `sub_403950`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::IsInvisibleOnFoot_433DA0`, ✓ `sub_41B0A0`, ✓ `Char_B4::sub_433A80` | checked | 43BEB0/43BED0 = FindBestTargetPed_Mode1/Mode5, Mouze_44 403D20/403D10 = PedGroup::IsLeaderEnteringCarOrTrain/IsLeaderInCar, 404900 = MergeWithOtherGroup, 421EC0 = GetVelocity, 40E8D0 = atan2 (pairing noise) |
| 0x465B20 | `Ped::ReactToAttacker` | 0x437010 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::IsInvisibleOnFoot_433DA0`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::get_cam_z_416B50` | matched | diff 74->70 |
| 0x465D00 | `Ped::IsPedAThreat` | 0x4371D0 | ✓ `cool_nash_0x294::IsInvisibleOnFoot_433DA0`, ✓ `sub_433CA0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `Char_B4::sub_433A80`, `sub_4CC8C0`, ✓ `cool_nash_0x294::get_wanted_points_433DC0` (10.5 0x592370), `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_403AF0` | todo |  |
| 0x466B70 | `Ped::IsInvisibleOnFoot_466B70` | 0x466B70 | `sub_4653C0` (10.5 0x4DFE10), `sub_42A630`, `sub_4634E0` | checked | pairing miss: 10.5 0x466B70 is the small Ped::IsInvisibleOnFoot_466B70 flag test, 9.6f 0x466B70 is an unrelated map function |
| 0x466FB0 | `Ped::FindNearbyPed` | 0x433D00 | ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_40FEA0` | done | diff 55->23 (rest: abs order / Max_44E540 call form inside MaxAbsDistance) |
| 0x467090 | `Ped::FindUsableCarDoor` | 0x437EC0 | `Car_BC::sub_421EC0`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, ✓ `IsDeactivated_4214D0`, ✓ `Car_10::set_obj_421380` | checked | 421EC0 = GetVelocity (already called) |
| 0x4672E0 | `Ped::UpdateMovementTowardsTarget` | 0x4380D0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `sub_4340D0`, ✓ `sub_4340E0`, ✓ `sub_4340F0`, `sub_40E8D0` | done | diff 256->244; 40E8D0 = atan2 |
| 0x467E20 | `Ped::KillCharAnyMeans` | 0x43FD10 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::IsInvisibleOnFoot_433DA0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | all 9.6f inlines used |
| 0x467FD0 | `Ped::PunchChar` | 0x438A30 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x468040 | `Ped::WaitOnFootAtTarget` | 0x438AB0 | ✓ `Char_B4::sub_433A80`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | follow-up: branch order, diff 20->0 CANDIDATE MATCH |
| 0x468310 | `Ped::ProcessObjectiveGotoAreaInCar` | 0x438C40 | ✓ `sub_421550`, ✓ `sub_421540`, `Car_BC::sub_421EC0` | done | diff 12 unchanged; new Car::SetA6Bit20_421540/ClearA6Bit20_421550 |
| 0x4686C0 | `Ped::EnterTargetObjectiveCar` | 0x43C010 | ✓ `sub_433900`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890` | done | code unchanged |
| 0x468930 | `Ped::EnterTrain` | 0x438E80 | ✓ `sub_433900`, ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890` | done | code unchanged |
| 0x468A00 | `Ped::LeaveTrain` | 0x43C220 | ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x468BD0 | `Ped::LeaveCarThenStop` | 0x43C3E0 | ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `cool_nash_0x294::set_target_to_enter_403B00` | done | code unchanged |
| 0x468C70 | `Ped::PatrolOnFoot` | 0x438F60 | ✓ `sub_433970` | done | all 9.6f inlines used |
| 0x468DE0 | `Ped::ProcessObjectiveGotoAreaOnFoot` | 0x4390F0 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x468E80 | `Ped::UpdateFollowPedObjective` | 0x439190 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_B4::sub_433A80`, ✓ `cool_nash_0x294::get_cam_z_416B50`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | follow-up: branch order, diff 35->0 CANDIDATE MATCH |
| 0x469060 | `Ped::GotoAreaByAnyMeans` | 0x43C480 | ✓ `sub_4215B0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `Get_F3C_433370`, ✓ `Car_BC::SetUniNum_421560`, ✓ `AllocateAI_421510`, ✓ `StartEngine_426E00`, ✓ `sub_433900`, `Car_BC::sub_421EC0`, ✓ `cool_nash_0x294::sub_403AE0` | inlines added | diff 484 unchanged; only Get_F3C used, others comment (each made diff worse); 421510 commented; once matched, StartEngine, 433900 and 403AE0 used (same code) |
| 0x469BD0 | `Ped::UpdateGroupMemberFollow` | 0x4341D0 | `sub_4046F0` | checked | 4046F0 = PedGroup::UpdateMemberTightFollowState |
| 0x469BF0 | `Ped::GuardSpot` | 0x439360 | ✓ `Char_B4::sub_433A80`, `sub_435610`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970` | done | diff 26->0 CANDIDATE MATCH (RegulateVelocityByRef, u8 flag, inverted state test) |
| 0x469D60 | `Ped::GuardArea` | 0x4394C0 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x469E50 | `Ped::SetupCarFollowTargetPed` | 0x434230 | ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x469F30 | `Ped::ProcessObjectiveFollowPedInCar` | 0x434300 | ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x469FE0 | `Ped::CallPoliceCar` | 0x43CFC0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x46A1F0 | `Ped::PullDriverOutOfCar` | 0x439590 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x46A290 | `Ped::FollowCarInCurrCar` | 0x4343C0 | ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x46A530 | `Ped::FireAtObject` | 0x434470 | `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x46A5E0 | `Ped::FireAtPlayer` | 0x434530 | ✓ `cool_nash_0x294::IsInvisibleOnFoot_433DA0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `sub_40E8D0` | done | diff 84->0 CANDIDATE MATCH (also fixed bit 8 -> 0x800, restructured like 9.6f) |
| 0x46A6D0 | `Ped::AimVehicleTurretStateMachine` | 0x434640 | ✓ `sub_4215B0`, `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x46A850 | `Ped::DestroyTargetCar` | 0x439810 | ✓ `IsMaxDamage_40F890` | done | all 9.6f inlines used |
| 0x46A8F0 | `Ped::ProcessInternalObjectiveFleeOnFootTillSafe` | 0x434740 | ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433920` | done | code unchanged; new Char::Get_F44 |
| 0x46A9C0 | `Ped::FleeFromPedTillSafe` | 0x4347D0 | ✓ `cool_nash_0x294::sub_433BE0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433930` | done | code unchanged; new Ped::ClearAttacker |
| 0x46AAE0 | `Ped::FleeFromPedAlways` | 0x434870 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_433BE0`, ✓ `Char_B4::sub_433930` | done | code unchanged |
| 0x46AB50 | `Ped::CheckFollowTargetPedStillValid` | 0x4398B0 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_433BE0`, ✓ `Char_B4::sub_433930` | done | code unchanged |
| 0x46AC20 | `Ped::FollowTargetStateMachine` | 0x439970 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_B4::sub_433A80`, ✓ `sub_433940`, ✓ `sub_433970` | done | all 9.6f inlines used |
| 0x46B2F0 | `Ped::PullDriverOutOfCarStateMachine` | 0x439E60 | ✓ `Car_BC::sub_403BA0`, ✓ `sub_433900`, `sub_435610`, ✓ `Char_B4::sub_433920`, `Car_BC::sub_421EC0`, ✓ `sub_433A60`, ✓ `Car_10::set_obj_421380` | done | diff 177->164; listed missing were noise, used SetMaxSpeed/SetCharState |
| 0x46B670 | `Ped::MeleeAttackStateMachine` | 0x4436A0 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40`, `sub_435610`, ✓ `SetAttacker_433BF0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_416B40`, ✓ `Char_B4::sub_433920`, ✓ `Char_B4::sub_433A80`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_41DC40`, ✓ `sub_433B70`, ✓ `sub_433B60`, ✓ `sub_433E50`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `sub_433970`, ✓ `Car_10::set_obj_421380` | done | diff 363->292; 435610 noise |
| 0x46BD50 | `Ped::IsOtherPedEnteringAsDriver` | 0x4349A0 | ✓ `sub_4341B0` | done | all 9.6f inlines used |
| 0x46BDC0 | `Ped::EnterCarStateMachine` | 0x43D1C0 | ✓ `sub_434AF0`, ✓ `sub_433900`, `Car_BC::sub_421EC0`, ✓ `sub_433560`, ✓ `Char_B4::sub_433920`, ✓ `Car_10::set_obj_421380`, ✓ `Char_B4::sub_433930`, ✓ `sub_4118B0`, ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | diff 109->86; gotos removed; 426F00 = TriggerCarAlarm |
| 0x46C250 | `Ped::ExitCarStateMachine` | 0x443C30 | ✓ `sub_433C10`, ✓ `sub_433900`, ✓ `Char_B4::sub_433920`, ✓ `sub_433A30`, ✓ `sub_4341B0`, ✓ `Car_10::set_obj_421380`, ✓ `sub_4215B0`, ✓ `sub_433C40` | done | diff 374->358; new Ped::DoJump |
| 0x46C770 | `Ped::WalkToTargetPoint` | 0x43A210 | ✓ `Char_B4::sub_433A90`, ✓ `Char_B4::sub_433920` | done | code unchanged (SetVelocityByRef_433920) |
| 0x46C7E0 | `Ped::ProcessInternalObjectiveGotoAreaOnFoot` | 0x43A290 | ✓ `Char_B4::sub_433A80`, ✓ `Char_B4::sub_433920` | done | all 9.6f inlines used |
| 0x46C8A0 | `Ped::WalkToTargetThenStop` | 0x43A340 | ✓ `Char_B4::sub_433920` | done | code unchanged (SetVelocityByRef_433920) |
| 0x46C910 | `Ped::FollowPathPoints` | 0x43A3C0 | ✓ `Char_B4::sub_433920` | done | code unchanged (SetVelocityByRef_433920) |
| 0x46C9B0 | `Ped::CrossRoad` | 0x43A490 | ✓ `sub_434960`, ✓ `Char_B4::sub_433920`, ✓ `Char_B4::sub_433A90` | done | code unchanged (SetVelocityByRef_433920) |
| 0x46CA70 | `Ped::ProcessInternalObjectiveChasePedInCar` | 0x434A30 | ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_421550` | done | code unchanged |
| 0x46CB30 | `Ped::StartPedCrossingAtTrafficLight_Y_Backward` | 0x43A550 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CC70 | `Ped::StartPedCrossingAtTrafficLight_X_Forwards` | 0x43A660 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CDB0 | `Ped::StartPedCrossingAtTrafficLight_Y_Forwards` | 0x43A770 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46CEF0 | `Ped::StartPedCrossingAtTrafficLight_X_Backwards` | 0x43A880 | ✓ `sub_434960`, ✓ `sub_433530`, ✓ `sub_433C50`, ✓ `sub_433C60`, ✓ `sub_433C70` | done | all 9.6f inlines used |
| 0x46D030 | `Ped::WaitForTrain` | 0x43A990 | ✓ `sub_411940`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, ✓ `sub_433900`, ✓ `Char_B4::sub_433920` | done | code unchanged (SetVelocityByRef_433920) |
| 0x46D0D0 | `Ped::EnterTrainStateMachine` | 0x43FEE0 | ✓ `Char_B4::sub_433930` | done | diff 62 unchanged |
| 0x46D300 | `Ped::FollowCarOnFoot` | 0x43AA10 | ✓ `Char_B4::sub_433A80`, `Car_BC::sub_421EC0`, ✓ `Char_B4::sub_433920`, ✓ `sub_433970`, ✓ `sub_433940` | done | diff 111->0 CANDIDATE MATCH |
| 0x46D460 | `Ped::AttackTargetStateMachine` | 0x4441B0 | ✓ `sub_4340F0`, ✓ `IsDestroyedPowergen_434140`, ✓ `Car_BC::sub_41E450`, ✓ `IsMaxDamage_40F890`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_car_416B60`, `Car_BC::sub_421EC0`, ✓ `SetAttacker_433BF0`, ✓ `sub_433BD0`, ✓ `sub_433810`, `sub_435610`, ✓ `sub_41CC70`, ✓ `sub_433AA0`, ✓ `sub_433970`, ✓ `Char_B4::sub_433920`, ✓ `sub_420420`, ✓ `Char_B4::sub_433A80` | commented | 421EC0/435610 noise; SetMaxSpeed worse (23->30), commented |
| 0x46DD70 | `SpawnTestGroup` | 0x4400A0 | ✓ `sub_41D020`, `sub_404400`, ✓ `sub_433360`, ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::get_cam_z_416B50`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433BC0` | inlines added | SetCounts, SetPedClass x2, get_remap x2; get_cam_x/y/z and SetRemap break the match (commented) |
| 0x46E080 | `Ped::RecruitNearbyPeds` | 0x444930 | `SpriteList_4::ctor_424620`, `sub_4204D0`, ✓ `sub_40FEA0`, ✓ `sub_433A20`, `sub_4402C0` | commented | closer 116->56; sub_4204D0 inline left as a comment (made it worse); diff 93->50; z fix (cam.z not searchRadius); ComputeCollisionPrism commented (different k constant in 10.5) |
| 0x46E200 | `Ped::SpawnPedGroupFollowers` | 0x440350 | `sub_404400`, ✓ `sub_433360`, ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433BB0`, ✓ `sub_433BC0`, `sub_404420` | inlines added | code unchanged; new PedGroup::SetCounts; SetRemap breaks the match (left out) |
| 0x46E380 | `SpawnPedestrianAt` | 0x4404F0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_433B80`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433BC0`, ✓ `sub_433C80`, `sub_43D640`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_433C10`, ✓ `sub_433C00`, `cool_nash_0x294::sub_433DF0` | inlines added | diff 323 unchanged; SetRemap commented (+2); others noise |
| 0x46EB60 | `PedManager::SpawnDummies` | 0x440CC0 | `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_433430` | done | diff 148->147; new Map_0x370::IsSlopeB4ToD0At; 420600 = set_xyz_lazy_451950 (noise) |
| 0x46F110 | `Ped::GetWeaponFromPed` | 0x434D70 | ✓ `sub_433820`, ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910` | done | diff 15 unchanged |
| 0x46F490 | `Ped::ChooseAttackWeapon` | 0x434E60 | `Car_BC::sub_421EC0` | checked | 421EC0 = GetVelocity (noise) |
| 0x46F600 | `Ped::ForceWeapon` | 0x43D830 | ✓ `sub_433810` | done | code unchanged |
| 0x46F650 | `Ped::GiveWeapon` | 0x43AD10 | ✓ `sub_433810` | done | code unchanged |
| 0x46F680 | `Ped::ApplyGangRespectForKill` | 0x434FF0 | ✓ `Zone_144::sub_433B30`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x46F720 | `Ped::UpdateStatsForKiller` | 0x43D880 | `CrimeReportQueue_CC::sub_44A370`, ✓ `angry_lewin_0x85C::sub_4219D0` | matched | CrimeReportQueue::sub_44A370 = ShouldReportPedCrime, which 10.5 calls (noise) |
| 0x46F9D0 | `Ped::Kill` | 0x4411B0 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_4338F0`, ✓ `cool_nash_0x294::set_health_4039A0` | done | code unchanged |
| 0x46FC90 | `Ped::HandleShootingAtCar` | 0x441450 | `Car_BC::sub_421EC0`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_433810`, ✓ `cool_nash_0x294::get_car_416B60` | todo |  |
| 0x46FE20 | `Ped::ProcessWeaponHitResponse` | 0x4350C0 | ✓ `sub_4340E0`, ✓ `sub_4340D0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_433810` | matched | diff 41->26; MaxAbsDistance commented (10.5 out-of-line Abs/Max) |
| 0x46FF00 | `Ped::NotifyWeaponHit` | 0x435180 | ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_433810` | done | diff 58->30; MaxAbsDistance not used (10.5 out-of-line) |
| 0x46FFF0 | `Ped::HandleWeaponFireEnd` | 0x435220 | ✓ `sub_433810` | done | code unchanged |
| 0x470050 | `Ped::AimRoofGun` | 0x435280 | ✓ `sub_4118F0`, ✓ `Car_BC::sub_411900`, ✓ `Car_BC::sub_411910`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), `sub_40E8D0` | done | follow-up: atan2 deltas as temporaries, raw asm identical CANDIDATE MATCH |
| 0x4701D0 | `Ped::IsNearestSpriteACar` | 0x435430 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x470200 | `Ped::StartPedWalking` | 0x43AD50 | ✓ `sub_433C10`, ✓ `Char_B4::sub_433920` | done | inlines added; SetVelocityByRef_433920 used, SetRemap breaks match |
| 0x470330 | `PedManager::Dummies` | 0x4415E0 | ✓ `sub_433E90` | done | code unchanged |
| 0x4703F0 | `PedManager::PedsService` | 0x445A20 | ✓ `sub_445960`, `Ped_Unknown_4::sub_4460D0` | checked | 445960 = Pool::UpdatePool, 4460D0 = Ped_List::ClearPeds (both already used) |
| 0x470650 | `Char_C::ctor_470650` | 0x4416B0 | `PedPool::ctor_43E0E0`, `Char_B4_Pool::ctor_435550`, `Char_8_Pool::ctor_4355E0`, ✓ `SpritePool::sub_421000`, `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `Ped_Unknown_4::ClearList_420E90` | checked | pool ctors are the new expressions, set_xyz_lazy is ResetCharStatics / 451950 (noise); w.sh already 0 |
| 0x470A50 | `PedManager::SpawnPedAt` | 0x43DB40 | ✓ `PedPool::sub_403890`, ✓ `sub_433C00`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::set_health_4039A0` | done | follow-up: SetRemap too, code unchanged |
| 0x470B00 | `PedManager::SpawnDriver` | 0x43DBD0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470BA0 | `PedManager::SpawnGangDriver` | 0x43DC60 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433BC0` | done | code unchanged |
| 0x470CC0 | `PedManager::CreateDummyDriver` | 0x43DD80 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470D60 | `PedManager::SpawnRunAwayGuy` | 0x43DE10 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470E30 | `PedManager::SpawnTrainLeaver` | 0x43DEB0 | ✓ `PedPool::sub_403890`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | checked | already uses PedPool::Allocate |
| 0x470F30 | `PedManager::AllocatePed` | 0x436070 | `Car_BC::sub_4221D0` (10.5 0x43A9A0), `Car_BC::SetUniNum_421560`, `CancelDespawn_4215F0` | checked | pairing error (Car callees in a 2-line alloc function) |
| 0x470F90 | `PedManager::ClonePed` | 0x43DFB0 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `sub_433C00`, ✓ `cool_nash_0x294::sub_433B50` | inlines added | code unchanged; SetRemap breaks the match (commented) |
| 0x4710C0 | `PedManager::PedById` | 0x43AE10 | ✓ `PedPool::sub_435530` | done | code unchanged; new PedPool::GetFirstPed |
| 0x471140 | `Ped_List::AddPedToFront` | 0x445F10 | ✓ `sub_445EF0` | done | code unchanged |
| 0x471160 | `Ped_List::AddPedToBackIfMissing` | 0x445F30 | ✓ `sub_445EF0` | done | code unchanged |
| 0x4711B0 | `Ped_List::AddPedToFrontIfMissing` | 0x445F80 | ✓ `sub_445EF0` | done | code unchanged |
| 0x4711F0 | `Ped_List::RemovePed_4711F0` | 0x445FC0 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471240 | `Ped_List::RemovePed_471240` | 0x446010 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471290 | `Ped_List::RemoveNonCorpses` | 0x446060 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `Char_8_Pool::sub_445F00` | done | follow-up: IsActive, code unchanged |
| 0x471320 | `Ped_List::RemoveFirstPed` | 0x446100 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471340 | `Ped_List::FindClosestActivePed` | 0x445C30 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | matched | all 9.6f inlines used |
| 0x4715A0 | `Ped_List::KillAndClearAllPeds` | 0x446120 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x4715E0 | `Ped_List::KillAndClearAllPedsResetCarRef` | 0x446160 | ✓ `Char_8_Pool::sub_445F00` | done | code unchanged |
| 0x471630 | `Ped_List::ApplyPassengerBusStopBehavior` | 0x445E40 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x474850 | `CarChaseTaskTable::AreAllies` | 0x4462F0 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x477C90 | `SpriteGrid::FindNearestSprite_SpiralSearch` | 0x447540 | `sub_4BAA70` | checked | sub_4BAA70 = Sprite::GetPed (called) |
| 0x477F60 | `SpriteGrid::CheckRectForCollisions` | 0x4477B0 | `sub_4BA5E0` | checked | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect, called in 10.5 |
| 0x478060 | `SpriteGrid::CheckTileSpritesForClosestMatch` | 0x446B80 | ✓ `sub_446940`, `sub_4BBC80`, ✓ `sub_416B40`, ✓ `sub_446960` | done | New Sprite::ManhattanDistance; Still matches. |
| 0x478160 | `SpriteGrid::SearchTileColumnForClosestSprite` | 0x446CB0 | ✓ `SpriteGrid_400::GetCollideListAt_446820` | done | all 9.6f inlines used |
| 0x478240 | `SpriteGrid::AddToDrawList` | 0x446D60 | ✓ `sub_446950` | done | Sprite::IsTypeAbove1; Still matches. |
| 0x4782C0 | `SpriteGrid::DoRemove` | 0x447850 | `sub_447360`, `sub_447380` | checked | sub_447350/60/70/80 are the GridSpriteLink / GridCell pool Allocate/DeAllocate (template, already used) |
| 0x478370 | `SpriteGrid::RemoveFromRowBuckets` | 0x447900 | `sub_447360`, `sub_447380` | checked | sub_447350/60/70/80 are the GridSpriteLink / GridCell pool Allocate/DeAllocate (template, already used) |
| 0x478440 | `SpriteGrid::AddToSingleBucketAt` | 0x4479D0 | `sub_447350`, `sub_447370` | checked | sub_447350/60/70/80 are the GridSpriteLink / GridCell pool Allocate/DeAllocate (template, already used) |
| 0x4784D0 | `SpriteGrid::AddToRowBuckets` | 0x447A60 | `sub_447350`, `sub_447370` | checked | sub_447350/60/70/80 are the GridSpriteLink / GridCell pool Allocate/DeAllocate (template, already used) |
| 0x4785D0 | `SpriteGrid::CheckRowForRectCollisions` | 0x446DE0 | ✓ `sub_446940`, `CanAllocateOfType_446930`, `sub_4B9A30`, ✓ `sub_41E390`, `sub_4BED60`, ✓ `sub_446920` | checked | sub_4B9A30 = ShouldCollideWithSprite, sub_4BED60 = AddSprite (called) |
| 0x478750 | `SpriteGrid::CheckAndHandleCollisionsInStrip` | 0x446F30 | `CanAllocateOfType_446930`, ✓ `sub_446920` | done | all 9.6f inlines used |
| 0x4787E0 | `SpriteGrid::CheckAndHandleRowCollisionsForSprite` | 0x446FD0 | ✓ `sub_446940`, `CanAllocateOfType_446930`, `sub_4B9A80`, ✓ `sub_446920` | checked | sub_4B9A80 = ProcessCarToCarImpactIfCar (called) |
| 0x478880 | `SpriteGrid::FindCollidingSpriteInRow` | 0x4470B0 | ✓ `sub_446940`, `CanAllocateOfType_446930`, `sub_4B9A30`, ✓ `sub_446920` | checked | sub_4B9A30 = ShouldCollideWithSprite (called) |
| 0x478A30 | `CollisionCounters::ctor_478A30` | 0x4471B0 | `GridSpriteLink_Pool::ctor_4468C0`, `GridCell_Pool::ctor_4468F0` | checked | pool ctors are compiler-generated (new) |
| 0x478BF0 | `CollisionCounters::dtor_478BF0` | 0x447B20 | `SpriteGrid_400::gdtor_4472F0`, `GridSpriteLink_Pool::gdtor_447310`, `GridCell_Pool::gdtor_447330` | checked | pool dtors are compiler-generated (delete) |
| 0x47E5B0 | `Crane::dtor_47E5B0` | 0x447F70 | ✓ `root_sound::DestroySoundObj_40FE60` | done | all 9.6f inlines used |
| 0x47E610 | `Crane::ctor_47E610` | 0x449860 | `SpriteList_4::ctor_424620` | checked | SpriteList ctor is the member's implicit ctor |
| 0x47E730 | `Crane::ComputeHookPos_47E730` | 0x447FE0 | ✓ `sub_432860`, `sub_40F6B0`, ✓ `sub_4207B0`, `sub_40F680` | matched | all inlines already used; 4207B0 = Sprite::get_x_y_443580 (called in 10.5) |
| 0x47E920 | `Crane::IsDropPositionClear` | 0x448090 | ✓ `sub_40F540`, `sub_447F90`, `sub_40F680`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, `sub_4BD670` | checked | Fix16_To_Ang16 already used; 447F90/40F680/4BD670 = ComputeHookPos/+=/... already |
| 0x47ECC0 | `Crane::DropHookedCar` | 0x4481F0 | ✓ `sub_40FEB0`, ✓ `Car_BC_Pool::sub_420F20`, ✓ `sub_447EA0`, `sub_4BED60` | done | ResetDespawnStatus used; AsCar/UpdateNextPrev already; 4BED60 = AddSprite (called) |
| 0x47EDF0 | `Crane::HookTransporterCargo` | 0x448300 | ✓ `sub_40FEB0`, ✓ `sub_4207B0`, `sub_40F600`, ✓ `sub_447DF0`, ✓ `WrapAngle_40E790` | checked | AsCar, get_x_y, operator- already used |
| 0x47EF80 | `Crane::HookPickupCar` | 0x448450 | ✓ `sub_40FEB0`, ✓ `sub_41B0A0`, ✓ `UnlinkFromActiveList_420F30`, ✓ `sub_4214E0`, ✓ `sub_4207B0`, `sub_40F600`, ✓ `sub_447DF0`, ✓ `sub_447EB0` | done | is_player used; AsCar/UnlinkFromActiveList already |
| 0x47F220 | `Crane::SetTransporterCargoTarget` | 0x4485D0 | ✓ `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F290 | `Crane::SetDropTransporterTarget` | 0x448650 | ✓ `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F2F0 | `Crane::SetPickupCarTarget` | 0x4486C0 | ✓ `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F350 | `Crane::IsTransporterCargoTargetValid` | 0x448730 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, ✓ `sub_40F580` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F3D0 | `Crane::IsPickupCarTargetValid` | 0x4487D0 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, ✓ `sub_40F580`, `sub_423A70` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F450 | `Crane::IsDropTransporterTargetValid` | 0x448870 | ✓ `sub_40FEB0`, ✓ `sub_4215B0`, ✓ `sub_40F580`, `sub_4BEA90` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F7F0 | `Crane::TargetTransporter` | 0x448980 | `sub_4BEA90`, `sub_4207B0`, `sub_448900` | checked | inlines already used (Ang16_to_Fix16/AsCar/IsDespawning); 423A70/4BEA90/448900 = AreAllDoorsClosed/FirstSpriteOfType/ComputeHookPolar (called) |
| 0x47F930 | `Crane::PickUpCar` | 0x448A80 | ✓ `sub_4215B0`, `sub_4BEA60` (10.5 0x5A6AD0), ✓ `sub_447EC0`, ✓ `sub_447EB0`, `sub_423A70`, ✓ `CanBeLiftedByCrane_447F00`, ✓ `sub_4207B0`, `sub_448900`, `Zheal_15C::sub_448150`, `sub_4BED60` | checked | inlines already used or still called in 10.5 (TagSpriteWithRng, AreAllDoorsClosed, AddSprite, ComputeHookPolar, IsTarget1PositionClear) |
| 0x47FB40 | `Crane::TryNextDropPosition` | 0x448C00 | ✓ `WrapAngle_40E790` | done | all 9.6f inlines used |
| 0x47FE10 | `Crane::UpdateCraneSprites` | 0x448E30 | ✓ `sub_40F540`, ✓ `Car_3C::set_ang_lazy_420690`, ✓ `set_xy_lazy_447E20`, `Zheal_15C::sub_448030`, `sub_447F90`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_40F680` | checked | Fix16_To_Ang16, set_ang_lazy etc already used; Object::sub_4826A0/Zheal sub_448030/447F90 = called helpers |
| 0x480310 | `Crane::Service` | 0x449BA0 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_447F40`, ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x4803B0 | `Crane::InitCrane` | 0x449130 | ✓ `Zheal_15C::sub_447F60`, ✓ `sub_447E90`, ✓ `SpritePool::sub_421000`, ✓ `SpriteList_4::ResetHead_4207E0` | done | new Crane::SetDisabled, SpriteList::ResetHead; code unchanged |
| 0x483F20 | `CreditsText::LoadCredits` | 0x449D70 | `text_0x14::Find_4C23A0` | todo |  |
| 0x484CF0 | `CrimeReport::SetReport` | 0x44A060 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | all 9.6f inlines used |
| 0x484FE0 | `CrimeReportQueue::ReportCrimeForPed` | 0x44A200 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_41B0A0` | done | Ped::is_player; Still matches. |
| 0x485090 | `CrimeReportQueue::ShouldReportCarCrime` | 0x44A2C0 | `angry_lewin_0x85C::sub_4219D0` | checked | get_idx vs 10.5 Player::GetIdx (separate out-of-line copy, called) |
| 0x488310 | `Crusher::CrushCar` | 0x44A420 | ✓ `sub_4215B0`, `sub_423310`, ✓ `sub_447EB0`, ✓ `sub_44A3E0` | done | add_f78_bits_421890(2) (9.6f 0x44A3E0); IsDespawning/Is_F9_Eq7 already; code unchanged |
| 0x4887F0 | `CrusherPool::CrushersService` | 0x44A990 | `sub_44A470` | checked | 44A470 = Service (called) |
| 0x488820 | `CrusherPool::CreateCrusher` | 0x44A9C0 | `sub_44A720` | checked | 44A720 = InitCrusher (called) |
| 0x48B920 | `rng::ShowCycle` | 0x44AA90 | ✓ `sub_44AA60`, ✓ `sub_44AA80` | done | inlined_check (44AA60), new GetLastRecRngIdx; 2->0 CANDIDATE MATCH |
| 0x48F710 | `SpriteMaskCache::InvalidateAllMasks` | 0x44AFE0 | ✓ `sub_44AF70` | done | all 9.6f inlines used |
| 0x48F730 | `SpriteMaskCache::ctor_48F730` | 0x44B000 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler vector ctor helper |
| 0x48F8B0 | `DamageAreaFlags::GetOrBuildDamageMask` | 0x44B0C0 | `sub_44AF90` | checked | sub_44AF90 is SpriteMask::MarkUsed (called) |
| 0x492260 | `RollingDigitCounter::DrawDigitsRightAligned` | 0x44B500 | `sub_44B490` | checked | 9.6f sub_44B490 = sub_495470 (10.5 calls it directly; 9.6f had an extra wrapper level) |
| 0x492430 | `RollingDigitCounter::DrawDigitsLeftAligned` | 0x44B6E0 | `sub_44B490` | checked | 9.6f sub_44B490 = sub_495470 (10.5 calls it directly) |
| 0x495470 | `DrawTextureScaled` | 0x495470 | `sub_472C00` (10.5 0x4F78F0), `sub_495220` (10.5 0x54C1A0), `sub_491F10`, `sub_4725B0` (10.5 0x4F7940), `sub_492400` | checked | pairing error (9.6f entry is a different function at the same address) |
| 0x498D20 | `InputDevices::game_pad_read` | 0x44C070 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x498DA0 | `InputDevices::read_input_device` | 0x44C0F0 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_44C050` | done | get_cur_rng, new IsInputSet; 293->258 |
| 0x49C340 | `DoorData::Init` | 0x44C8A0 | ✓ `gmp_block_info::init_44C840` | done | new gmp_block_info::init; diff 0 before and after (CANDIDATE MATCH) |
| 0x49C6D0 | `Door::CanOpen` | 0x44CA70 | ✓ `sub_4118D0`, ✓ `sub_44C870` | done | all 9.6f inlines used |
| 0x49C7F0 | `Door::CanOpenForPed` | 0x44CB80 | ✓ `sub_41B0A0` | done | all 9.6f inlines used |
| 0x49CC00 | `Door::InitDouble` | 0x44CC30 | ✓ `sub_447E90`, `Object_5C::sub_4852E0` (10.5 0x5299B0) | checked | Object_5C::sub_4852E0 is NewPhysicsObj (out of line in 10.5); 10.5 calls NewTouchPoint; SetParam already used |
| 0x49CF10 | `DoorPool::AllocDoorData` | 0x44D140 | ✓ `Door_10_Pool::sub_44C830` | checked | Door_10_Pool::sub_44C830 is the pool Allocate() already used |
| 0x49CF50 | `DoorPool::RegisterSingleDoorNoCheck` | 0x44DB00 | `sub_44D840` | checked | sub_44D840 corresponds to Door::sub_49C8D0, called in 10.5 |
| 0x49CFA0 | `DoorPool::RegisterDoubleDoorNoCheck` | 0x44D430 | `sub_44CDD0` | checked | sub_44CDD0 is a 9.6f-only door setup variant; 10.5 calls Door::sub_49CC00 instead (pairing difference) |
| 0x49D170 | `DoorPool::RegisterSingleDoor` | 0x44D670 | `sub_44CC30` (10.5 0x49CC00) | checked | sub_44CC30 (=sub_49CC00) vs 10.5 call to sub_49CAC0: variant pairing difference |
| 0x49D1F0 | `DoorPool::RegisterDoubleDoor` | 0x44D6F0 | `sub_44CDD0` | checked | sub_44CDD0: same as 0x49CFA0, 10.5 calls sub_49CC00 |
| 0x49D3C0 | `DoorPool::CheckDoorAccess` | 0x44D1E0 | ✓ `sub_40FEB0`, `IsOpen_44C860`, ✓ `sub_40FEA0`, ✓ `sub_433A20` | inlines added | Door::sub_44C860 used (decl in Door.hpp, def in DoorPool.hpp); get_ped commented (broke match) |
| 0x49D4A0 | `DoorPool::ctor_49D4A0` | 0x44D2E0 | ✓ `Door_10_Pool::ctor_44C800` | checked | Door_10_Pool::ctor_44C800 is the pool template ctor via new |
| 0x49D570 | `DoorPool::dtor_49D570` | 0x44D7D0 | `Door_10_Pool::gdtor_44D3A0` | checked | gdtor_44D3A0 is the pool delete in GTA2_DELETE_AND_NULL |
| 0x4A3610 | `MissLog2_4A3610` | 0x4504D0 | `sub_44DBF0` | todo |  |
| 0x4A6DB0 | `File::sub_4A6DB0` | 0x402B20 | `Memory::malloc_4059B0` (10.5 0x4FE4D0) | todo |  |
| 0x4A7FC0 | `Firefighter::CheckAbortConditions` | 0x450CD0 | ✓ `sub_4211C0`, ✓ `sub_4215B0`, ✓ `IsMarkedForDespawn_4214B0`, ✓ `sub_403B70`, `sub_450CC0`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450` | matched | 9.6f compares get_car_velocity() (GetLength_41E260, not 453590) and has no (u8) cast |
| 0x4A81F0 | `Firefighter::Update` | 0x450F10 | ✓ `sub_4215B0`, ✓ `Car_BC::SetUniNum_421560`, ✓ `AllocateAI_421510`, `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `sub_4118B0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `GetObjectiveStatus_450CB0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `IsMarkedForDespawn_4214B0`, ✓ `cool_nash_0x294::sub_403A40` | inlines added | Case 1 now shares the state=6 block like 9.6f/10.5 (jump layout matches); left: ebx vs ebp for 0, and the MaxAbsDistance part of case 2 (10.5 partly out of line) |
| 0x4ABBD0 | `Debug::Init` | 0x451930 | `Registry::Get_Debug_Setting_4B53B0` (10.5 0x586E90) | checked | Get_Debug_Setting_4B53B0 = Registry::Get_Debug_Setting (called) |
| 0x4ACFA0 | `Frontend::create` | 0x457830 | `sub_410550`, `unknown_libname_18` (10.5 0x40EF10), `sub_481D10` | checked | sub_410550/unknown_libname_18 are root_sound thunks, sub_481D10 is Bink::Reset (called in 10.5) |
| 0x4AD140 | `Frontend::DrawMenu` | 0x457920 | ✓ `sub_453A60`, `text_0x14::Find_4C23A0` | matched | DecodeStage x2; unchanged (76) |
| 0x4AE2D0 | `Frontend::UpdatePageFromUserInput` | 0x4597C0 | ✓ `sub_453A60` | done | DecodeStage x2; unchanged (377) |
| 0x4AEC00 | `Frontend::Update` | 0x45A250 | `sub_453480` | checked | sub_453480 (268 bytes, 9.6f input/credits helper) has no inlined body in 10.5 here; 10.5 calls separate functions |
| 0x4AF2A0 | `Frontend::ctor_4AF2A0` | 0x456A60 | ✓ `laughing_blackwell_0x1EB54::sub_453A30`, `sub_453D40` | inlines added | new Frontend::SetField10D; sub_453D40 (sets field_11C by language) has no 10.5 form, sub_4AF0E0 used; diff 0 (CANDIDATE MATCH, was 0 already) |
| 0x4B0220 | `Frontend::SetupMenuStringsOptionsElements` | 0x453E20 | `text_0x14::Find_4C23A0`, `?do_always_noconv@codecvt_base@std@@MBE_NXZ` | checked | do_always_noconv (9.6f 0x4538E0, returns true) guards dead code that 10.5 dropped; nothing to use |
| 0x4B2F60 | `Frontend::HandlePlayerNameTyping` | 0x459E30 | ✓ `sub_4539D0` | done | (not in todo, same helper) GetCharWidthFromFontType; still matches |
| 0x4B3170 | `Frontend::ChangeMenuPage` | 0x4587B0 | ✓ `GameSession_578::sub_453A80`, ✓ `sub_453A60`, ✓ `sub_453AB0`, `text_0x14::Find_4C23A0`, ✓ `sub_434B20`, ✓ `sub_453A90`, ✓ `sub_453AA0`, `sub_4529C0`, `sub_452990` | inlines added | new IsTagGame, TagGame HasQuit/GetTime/GetLeaderIdx, DecodeStage once; 2 decodes commented (500->532); 4529C0/452990 pairing; 502->500 |
| 0x4B3AF0 | `Frontend::GetOptionText` | 0x452200 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4B4440 | `Frontend::GetMainAndBonusStagesFromSeqFile` | 0x455340 | `sub_4527A0` | checked | sub_4527A0 is Frontend::sub_4B4BC0, already called (10.5 merged the two calls) |
| 0x4B4D00 | `Frontend::LoadMapFilenames` | 0x4556A0 | ✓ `sub_453A40` | done | EncodeStage; still matches |
| 0x4B4EC0 | `Frontend::LoadMapFilenamesFromSvg` | 0x455850 | ✓ `sub_453A60` | done | DecodeStage; still matches |
| 0x4B5270 | `Frontend::DrawSavedStage` | 0x455B20 | ✓ `sub_453A60` | done | DecodeStage; still matches |
| 0x4B5430 | `Frontend::DrawScoreTable` | 0x456FB0 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4B55F0 | `Frontend::DrawMultiplayerScores` | 0x455C90 | ✓ `sub_453AA0` | done | GetTime; still matches (sub_4D7787 is CRT _itow) |
| 0x4B57B0 | `Frontend::DrawLastAndBestStats` | 0x457140 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4B5FF0 | `Frontend::intro_bik_exists` | 0x452A10 | `sub_452990` | checked | sub_452990 is intro_bik, called out of line in 10.5 |
| 0x4B6780 | `Frontend::UpdateMenuScreen` | 0x455F90 | ✓ `sub_453A60` | done | DecodeStage; still matches |
| 0x4B7120 | `Frontend::GetPreviousUnlockedBonusStage` | 0x456180 | ✓ `sub_453A60`, ✓ `sub_453A40` | done | new GameSession::DecodeStage/EncodeStage; code unchanged (83) |
| 0x4B7360 | `Frontend::GetNextUnlockedBonusStage` | 0x4562F0 | ✓ `sub_453A60`, ✓ `sub_453A40` | matched | DecodeStage/EncodeStage; unchanged (53) |
| 0x4B7610 | `Frontend::UpdateBonusStageArrows` | 0x456490 | ✓ `sub_453A60` | done | DecodeStage; still matches |
| 0x4B7800 | `Frontend::GetLastUnlockedBonusStageCode` | 0x4565E0 | ✓ `sub_453A40` | done | EncodeStage; 91->90 |
| 0x4B7D60 | `Frontend::DrawBonusRating` | 0x456820 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700), `text_0x14::Find_4C23A0` | todo |  |
| 0x4B7E10 | `Frontend::DrawKeyHint` | 0x4568C0 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4B8280 | `Frontend::HandlePasswordTyping` | 0x45A010 | ✓ `sub_4539D0`, `text_0x14::Find_4C23A0` | done | new GetCharWidthFromFontType (11->6), plus field_C9B3 branch order fix -> 0 CANDIDATE MATCH |
| 0x4B8560 | `Frontend::CheckPassword` | 0x459660 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4B8C40 | `Game::LoadGameFiles` | 0x45B470 | `sub_410550`, ✓ `GameSession_578::sub_45B420`, `sub_4B5420` | checked | clear_secret_tokens_collected = 9.6f 0x45B420 (already used, comment added); sub_410550 is a thunk on an unknown 9.6f global (0x5DCBC8), sub_4B5420 a large callee |
| 0x4B8EB0 | `Game::BootGame` | 0x45B5F0 | `Game_0x40::sub_45ACE0`, `FatalError_450530` (10.5 0x4A38C0), `sub_4CAC30`, `Map_0x370::sub_46A4D0`, `Map_0x370::sub_4692B0` | checked | 9.6f has `if (!bStartNetworkGame && !Game::sub_45ACE0()) FatalError(0x48, ..., 0x283)` with sub_45ACE0 returning true; 10.5 drops it as dead code, so nothing to add; other callees are 10.5 calls |
| 0x4B8FF0 | `Game::ShowCounters` | 0x45B750 | ✓ `Car_BC_Pool::get_cars_count_45AD30`, ✓ `PlayerScoreTracker_36C::get_accuracy_count_45B0A0`, ✓ `PlayerScoreTracker_36C::get_reverse_count_45B0B0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | CarPool::get_cars_count_45AD30 (new), get_cam_x/y; code unchanged |
| 0x4B9270 | `Game::ShowDebugOverlay` | 0x45BA10 | ✓ `Hud_TextEntry_C4::SetFontRemap_45AFD0` | done | Hud_TextEntry::SetFontRemap; code unchanged |
| 0x4B92D0 | `Game::Draw` | 0x45A5A0 | `SpriteRenderer_1C::sub_44B890`, `SpriteGrid_400::sub_447390`, `Nanobotz::Draw_472110` | checked | SpriteRenderer::sub_44B890/SpriteGrid::sub_447390/Nanobotz::Draw_472110 correspond to the 10.5 calls the source already makes (ResetAll, DrawSpritesClipped, ...) |
| 0x4B9410 | `Game::UpdateGame` | 0x45C1F0 | ✓ `Light_1D4CC::Service_45C1E0`, `Object_5C::sub_487F50`, `ScriptManager_C1EA8::sub_481900`, `Particle_8::sub_491CE0`, `sub_4C3590`, `CokeZero_100::sub_4B9260` | checked | callees are global thunks (Light sub_45C1E0 already used, others map to the 10.5 service calls in the source) |
| 0x4B9750 | `Game::GetFirstPlayerWithoutPed` | 0x45BAB0 | ✓ `angry_lewin_0x85C::sub_45B0C0` | done | Player::has_player_ped; code unchanged |
| 0x4B9790 | `Game::ShakeCamerasAtPos` | 0x45BB00 | ✓ `DrawUnk_0xBC::sub_40CF60`, `DrawUnk_0xBC::sub_41EAD0` | done | Camera::IsPointInViewBounds (9.6f 0x40CF60); sub_41EAD0 is sub_436120, still called; code unchanged |
| 0x4B98E0 | `Game::IsSpriteInPlayerView` | 0x45A730 | `DrawUnk_0xBC::sub_41F180` | checked | DrawUnk_0xBC::sub_41F180 is a 368-byte callee, source calls the 10.5 function |
| 0x4B9950 | `Game::IsSpriteInPlayerBounds` | 0x45BBA0 | ✓ `DrawUnk_0xBC::sub_45AEA0` | done | Camera::IsSpriteInBounds (new) + Player*& slot ref; 3->0 CANDIDATE MATCH |
| 0x4B9A80 | `Game::IsPointOnScreen` | 0x45BC10 | ✓ `DrawUnk_0xBC::sub_40CF60` | matched | DrawUnk_0xBC::sub_40CF60 is IsPointInViewBounds, already used (comment updated to 9.6f 0x40CF60); remaining 26 lines are register allocation |
| 0x4B9B10 | `Game::IsRectVisibleToAnyPlayer` | 0x45BC90 | ✓ `DrawUnk_0xBC::sub_45AF40` | done | Camera::IsRectInBounds (new) + s32 loop index; 85->16 (register swap left) |
| 0x4B9D60 | `Game::ReduceTrafficSpawnCounterOfOtherViewers` | 0x45BD40 | ✓ `Game_0x40::get_player_4219E0` | done | Game::GetPlayer; code unchanged |
| 0x4B9DE0 | `Game::ctor_4B9DE0` | 0x45C4D0 | `angry_lewin_0x85C::sub_45B0D0`, `rng::ctor_45A960`, `Nanobotz::ctor_45B050`, `Mike_A80::ctor_45C040`, `ScriptStackFramePool_25C::ctor_45BFE0`, `jawwie_110::ctor_45C0D0`, `EmergencyCrewPool_1E0::ctor_45B1A0`, `Police_7B8::ctor_45C150`, `Light_1D4CC::ctor_45B3D0`, `Zones_CA8::ctor_45AE60`, `sub_489AC0`, `CokeZero_100::ctor_4B9490`, `Tango_54::ctor_45B440`, ✓ `LangIsJapanese_452E60` | checked | ctor list: member ctors, pairing noise |
| 0x4BAE30 | `Game::dtor_4BAE30` | 0x45D3D0 | `angry_lewin_0x85C::dtor_45A970`, `text_0x14::dtor_405A80`, `gtx_0x106C::gdtor_451F90`, `Map_0x370::gdtor_45A990`, `SpriteRenderer_1C::gdtor_45A9B0`, `PedPool::gdtor_43DB20`, `ScriptManager_C1EA8::gdtor_45A9F0`, `ScriptStackFramePool_25C::gdtor_45D350`, `ObjectDefinitions_8CA8::gdtor_45BDC0`, `Object_5C::gdtor_45AA10`, `PedManager::gdtor_45AA30`, `FpsCounter_54::gdtor_45AA50`, `SpriteManager_8::gdtor_45AA70`, `CollisionCounters_C::gdtor_45AA90`, `PedRefTable_7F8::gdtor_45AAB0`, `Sero_181C::gdtor_45AAD0`, `Taxi_4::gdtor_45AAF0`, `TileAnim_2::gdtor_45AB10`, `Weapon_8::gdtor_45AB30`, `Door_4D4::gdtor_45AB50`, `jawwie_110::gdtor_45BDE0`, `Garox_2B00::gdtor_45D3B0`, `TextureCache_15D8::gdtor_451F70`, `TrafficLights_194::gdtor_45AB70`, `PatrolRoutePool_1D7E::gdtor_45BE00`, `PathFinder_2FD4::gdtor_45BE20`, `Monster_808::gdtor_45AB90`, `Particle_8::gdtor_45ABB0`, `ExplosionPool_3D4::gdtor_45ABD0`, `ExplosionPool_7A8::gdtor_45ABF0`, `Zheal_D9C::gdtor_45BE40`, `Snooky_94::gdtor_45BE60`, `EmergencyCrewPool_1E0::gdtor_45BE80`, `Police_7B8::gdtor_45BEA0`, `Light_1D4CC::gdtor_45BEC0`, `Zones_CA8::gdtor_45BEE0`, `ChickenLegend_48::dtor_45D370`, `CarChaseTaskTable_500::dtor_45AC10`, `CokeZero_100::dtor_45AC30`, `CrimeReportQueue_CC::gdtor_45AC50`, `Tango_54::gdtor_45BF00`, `CollisionSoundQueue_C88::gdtor_45AC70`, `KanjiFont_8EC::gdtor_45AC90` | checked | dtor list: global dtors, pairing noise |
| 0x4BECA0 | `GangPool::FirstGang` | 0x45DD60 | ✓ `Zone_144::sub_45DD50` | done | Gang::HasKillRespectChange; code unchanged |
| 0x4BECE0 | `GangPool::NextGang` | 0x45DDB0 | ✓ `Zone_144::sub_45DD50` | done | Gang::HasKillRespectChange; code unchanged |
| 0x4BF340 | `Gang::GetArrowColourText` | 0x45DD20 | `text_0x14::Find_4C23A0` | todo |  |
| 0x4C1A70 | `Generator::Activate` | 0x45E3A0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C1AB0 | `Generator::next_cycle` | 0x45E0B0 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C1C70 | `Generator::Init` | 0x45E270 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4C1D70 | `GeneratorPool::GeneratorsService` | 0x45E2E0 | ✓ `rng::get_cur_rng_41CFE0`, `sub_45E240` | checked | 9.6f sub_45E240 corresponds to Generator::Service (still called); get_cur_rng already used |
| 0x4C1E20 | `GeneratorPool::ctor_4C1E20` | 0x45E3E0 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler's array ctor helper, nothing to add |
| 0x4C5CD0 | `GameSession::UpdateFrags` | 0x45EC70 | ✓ `Game_0x40::get_player_4219E0`, `sub_4B7580` | done | Game::GetPlayer; sub_4B7580 = ChangeFragsByAmount thunk (still called); code unchanged |
| 0x4C8E90 | `PedGroup::ClearGroupData` | 0x403BE0 | ✓ `cool_nash_0x294::sub_403A30` | done | code unchanged |
| 0x4C8F90 | `PedGroup::add_ped_to_end_of_list` | 0x404C90 | `sub_404420` | checked | 404420 = add_ped_to_list |
| 0x4C8FE0 | `PedGroup::replace_leader` | 0x404CE0 | ✓ `cool_nash_0x294::sub_403940` | done | all 9.6f inlines used |
| 0x4C9150 | `PedGroup::AreAllMembersOffScreen` | 0x403C40 | ✓ `cool_nash_0x294::sub_4039F0` | done | code unchanged |
| 0x4C91B0 | `PedGroup::ResetMembersToFollowLeader` | 0x403CB0 | ✓ `sub_403960`, ✓ `cool_nash_0x294::sub_403AE0` | checked | already uses ClearPanicking (0x403960) |
| 0x4C9240 | `PedGroup::KillEntireGroup` | 0x403D50 | ✓ `cool_nash_0x294::sub_403A30` | done | diff 8->0 CANDIDATE MATCH |
| 0x4C92A0 | `PedGroup::DisbandGroup` | 0x403DA0 | ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x4C93A0 | `PedGroup::DestroyGroup` | 0x403E90 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x4C94E0 | `PedGroup::DisbandGroupDueToAttack` | 0x403FB0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `sub_403A20`, `sub_403A50`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::sub_403920` | inlines added | code unchanged; new Ped::ClearHitCount; 403A50 speed setter breaks match (commented) |
| 0x4C9680 | `PedGroup::PromoteMemberToLeader` | 0x404120 | ✓ `PedPool::sub_403890`, ✓ `cool_nash_0x294::get_objective_timer_403B30`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::sub_403B20`, ✓ `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::sub_403B40`, ✓ `cool_nash_0x294::sub_4039D0`, ✓ `cool_nash_0x294::sub_403B50`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::sub_403AF0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, `cool_nash_0x294::sub_403940`, ✓ `cool_nash_0x294::get_enter_car_as_passenger_4039C0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::set_occupation_403970` | inlines added | code unchanged; set_ped_group_id and get_occupation break the match |
| 0x4C9970 | `PedGroup::RemovePed` | 0x404D40 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_403940`, `sub_4045D0` (10.5 0x4CAE80), ✓ `cool_nash_0x294::sub_403A30` | done | code unchanged |
| 0x4C9B60 | `PedGroup::GroupAttackPed` | 0x404EF0 | `sub_403B90` (10.5 0x4CA3E0), `sub_404900`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `cool_nash_0x294::has_car_403B80`, `sub_404450`, ✓ `cool_nash_0x294::sub_403990` | done | diff 35 unchanged; 403B90 = GetPanicking, 404450 = FindLastNonPanickingMember |
| 0x4C9F00 | `PedGroup::CoordinateGroupCarEntry` | 0x405240 | ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_403B70`, `sub_404490`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::sub_403AE0`, `sub_404480`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `Car_BC::sub_403BA0`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, ✓ `sub_403960`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `sub_403900`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `sub_403BC0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, ✓ `cool_nash_0x294::set_target_car_door_403A70` | matched | IsSwatVanOrBankVan inline added (Car.hpp); Main diff: 10.5 places the SWATVAN/bank_van door search before the normal one; swapping the if/else in source gives identical code |
| 0x4CA5E0 | `PedGroup::UpdateMemberAIState` | 0x405760 | ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_403B70`, `sub_404AD0`, ✓ `cool_nash_0x294::sub_403AF0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `sub_403950`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::set_target_to_enter_403B00`, ✓ `cool_nash_0x294::has_car_403B80` | done | follow-up: second HasGameObject, code unchanged |
| 0x4CAA20 | `PedGroup::IsAllMembersInSomeCar` | 0x404840 | ✓ `cool_nash_0x294::sub_403990` | done | all 9.6f inlines used |
| 0x4CAD40 | `PedGroup::IsLeaderCloseToTargetCar` | 0x4049F0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | all 9.6f inlines used |
| 0x4CAE80 | `PedGroup::FindNearestOtherMember` | 0x4045D0 | `cool_nash_0x294::get_car_state_403A90`, `cool_nash_0x294::sub_436920` (10.5 0x463830), `cool_nash_0x294::sub_403B60`, `sub_436160` (10.5 0x45C920), `cool_nash_0x294::sub_403AE0`, `cool_nash_0x294::sub_403A40` | matched | pairing error: the 9.6f callees (car state, SetObjective2, velocity) do not fit this function |
| 0x4CB0D0 | `PedGroup::New` | 0x404C40 | ✓ `sub_4038F0`, ✓ `?MarkRootRemoved@VirtualProcessorRoot@details@Concurrency@@QAEXXZ` | done | code unchanged; new PedGroup::set_in_use |
| 0x4CDD80 | `InputRecorder_67F8B0::should_ignore_input` | 0x45ED50 | `sub_4C6E20` | checked | sub_4C6E20 (Hud+0x2A1E thunk) vs 10.5 Hud::sub_5D6CB0: pairing difference |
| 0x4CDE20 | `InputRecorder_67F8B0::save_replay_record` | 0x45FA50 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_45F9E0` | done | new rng::get_rnd; still matches |
| 0x4CE380 | `InputRecorder_67F8B0::LoadReplayHeader` | 0x45F270 | `FatalError_450530` (10.5 0x4A38C0) | checked | FatalError_450530 is FatalError (10.5 has it out of line, inline via chunk.hpp elsewhere); no open-coded form here |
| 0x4CEAC0 | `InputRecorder_67F8B0::get_input_bits` | 0x45FD10 | `sub_45ED00`, ✓ `rng::get_cur_rng_41CFE0`, `IsField238_45EDE0`, ✓ `sub_416BC0` | matched | get_cur_rng (no u16 cast) fixes the rng compare; left: 10.5 has a redundant je-to-next before the jne on the 0x1FF000 tests (not in 9.6f), unexplained |
| 0x4D2090 | `KanjiFont::Load_kanji_dat` | 0x460F10 | ✓ `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version already used |
| 0x4D2B40 | `KanjiFont::InitGlyphCaches` | 0x460DE0 | `sub_4BF550` | checked | sub_4BF550 is gtx GetSpriteIdxFromFont, called |
| 0x4D9650 | `ErrorLog::Write_Log` | 0x461590 | `sub_461500` (10.5 0x4D9670) | checked | sub_461500 = log_on_line_written (called) |
| 0x4DA440 | `Init_keybrd_jolly_and_sound` | 0x461880 | `unknown_libname_18` (10.5 0x40EF10) | checked | unknown_libname_18 is root_sound::static_dtor_40EF10 thunk |
| 0x4DA4D0 | `InitializeGame` | 0x461DE0 | `unknown_libname_18` (10.5 0x40EF10), ✓ `sub_409C40`, `sub_409F90`, `Game_0x40::sub_45A8D0` (10.5 0x4B9CD0), `GameSession_578::sub_45E770` (10.5 0x4C5C30), `Game_0x40::sub_45A910` (10.5 0x4B9D10), ✓ `sub_461DC0` | matched | new NetPlay::GetPlayerIdx; 409F90=CountActivePlayers called; 9.6f SetPlayerName loop absent in 10.5; 16->10 |
| 0x4DA700 | `CleanUpInputAndOthers` | 0x462060 | `sub_461910`, `unknown_libname_18` (10.5 0x40EF10) | checked | sub_461910 is the keybrd scalar deleting dtor; unknown_libname_18 root_sound thunk |
| 0x4DA740 | `ShutdownGame` | 0x4620A0 | `keybrd_0x204::destroy_461270`, `sub_4A9270` | checked | sub_4A9270 is PlyDat::Destroy (called) |
| 0x4DA9F0 | `Net_Receive_Players_Inputs` | 0x462140 | ✓ `sub_461DD0` | checked | sub_461DD0 (SetQuit) not open-coded in this 10.5 version |
| 0x4DACB0 | `Net_Send_Our_Inputs` | 0x462440 | ✓ `sub_461DA0` | done | New NetPlay::GetPlayerCount; Still matches. |
| 0x4DAD50 | `Net_Set_Local_Player_Inputs` | 0x4624E0 | ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_461DB0` | done | all 9.6f inlines used |
| 0x4DADA0 | `ManageNetworkTimeAndHudPager` | 0x462530 | `text_0x14::Find_4C23A0`, ✓ `sub_461DC0`, `sub_4C93B0` | done | GetTimeLimit x2 (was dword_67ED24); sub_4C93B0 = Hud ClearPager (out of line in 10.5); unchanged (18) |
| 0x4DAF30 | `do_network_and_local_inputs` | 0x4626D0 | ✓ `Game_0x40::get_player_4219E0`, ✓ `sub_461DB0` | done | Game::GetPlayer and Player::GetInUse; Still matches. |
| 0x4DB070 | `ShowPlayerLeftMessage` | 0x4620E0 | ✓ `Game_0x40::get_player_4219E0`, `text_0x14::Find_4C23A0` | done | Game::GetPlayer; Still matches. |
| 0x4DB2E0 | `MakeInitialSyncData` | 0x461C60 | `GameSession_578::sub_45E4F0` (10.5 0x4C5950) | checked | sub_45E4F0 is GetStyleName (out of line, called) |
| 0x4DB440 | `CompareRemotePlayers` | 0x462850 | `GameSession_578::sub_45E4F0` (10.5 0x4C5950) | checked | sub_45E4F0 is GetStyleName (out of line, called) |
| 0x4DEF00 | `gmp_map_zone::get_zone_str` | 0x462BE0 | `text_0x14::Find_4C23A0`, `sub_4C22D0` | checked | 9.6f sub_4C22D0 is a char->wchar copy into a global buffer; 10.5 builds a key and calls Find instead |
| 0x4DEF40 | `gmp_map_zone::IsZoneVisibleToAnyPlayer` | 0x4690B0 | ✓ `sub_463710` | done | Fix16_Rect::SetFromPosSize replaces the 4-arg ctor (both emitted out of line, call kept); still matches |
| 0x4DEFD0 | `Map_0x370::zone_by_name_4DEFD0` | 0x464C70 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4DF050 | `Map_0x370::zone_idx_by_name_4DF050` | 0x464D00 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4DF0F0 | `Map_0x370::zone_by_type_bounded_4DF0F0` | 0x464DA0 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4DF1D0 | `Map_0x370::first_zone_by_type_4DF1D0` | 0x464E70 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4DF240 | `Map_0x370::GetNearestZoneOfType` | 0x469110 | `Map_0x370::sub_462E40`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4DF4D0 | `Map_0x370::zone_by_pos_and_type_4DF4D0` | 0x464FE0 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint (sub_462E40 = GetZone called); still matches |
| 0x4DF6A0 | `Map_0x370::first_zone_by_pos_4DF6A0` | 0x465130 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint (sub_462E40 = GetZone called); still matches |
| 0x4DF770 | `Map_0x370::next_zone_4DF770` | 0x4651C0 | `Map_0x370::sub_462E40`, ✓ `gmp_map_zone::sub_463020` | done | Overlaps -> gmp_map_zone::ContainsPoint (sub_462E40 = GetZone called); still matches |
| 0x4DF890 | `Map_0x370::GetZoneInfoAt` | 0x465250 | `Map_0x370::sub_465090` | checked | sub_465090 is GetNavZoneAtPos, called |
| 0x4DFE10 | `Map_0x370::get_block_4DFE10` | 0x4653C0 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4DFE60 | `Map_0x370::GetEffectiveBlock` | 0x465410 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4DFEE0 | `Map_0x370::GetBlockClamped` | 0x465490 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4DFF60 | `Map_0x370::GetLeftEdgeSpec` | 0x465510 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0 x2; still matches |
| 0x4E0000 | `Map_0x370::GetTopEdgeSpec` | 0x4655B0 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0 x2; still matches |
| 0x4E00A0 | `Map_0x370::GetBlockSpec` | 0x465650 | ✓ `gtx_0x106C::sub_462FD0` | done | gtx sub_462FD0; still matches |
| 0x4E11E0 | `Map_0x370::RectHitsDiagonalWall` | 0x465FE0 | `sub_4BA5E0`, ✓ `sub_463760`, `sub_432860`, ✓ `sub_463690` | inlines added | new Fix16_Rect GetMidZ (const; fixes missing /2) + EdgesCrossSegment; x reset per row bug fixed; SetXY commented; 131->76 |
| 0x4E1520 | `Map_0x370::SpriteHitsDiagonalWall` | 0x466170 | `sub_432860`, `sub_4BB9C0`, `sub_4828F0`, `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `SetSprite_40FEE0` | done | SetXY; 4BB9C0=PointInsideRotatedBounds, 4828F0=GetDirectionalObject, set_xyz_lazy=451950 all called; unchanged (162) |
| 0x4E1A30 | `Map_0x370::sub_4E1A30` | 0x466430 | ✓ `sub_4634B0`, ✓ `sub_463480` | checked | sub_463480 is the existing IsWestOrEastGradSlope (address comment added), already used |
| 0x4E1E00 | `Map_0x370::IsTileBlockedForSprite` | 0x46A570 | ✓ `sub_466CF0` | done | IsGradientSlopeAt; 1042->993 |
| 0x4E4630 | `Map_0x370::sub_4E4630` | 0x466620 | ✓ `sub_42A630` | done | all 9.6f inlines used |
| 0x4E4820 | `Map_0x370::sub_4E4820` | 0x4667E0 | ✓ `sub_45ADB0`, ✓ `sub_410BF0`, ✓ `sub_45ADA0`, ✓ `sub_45ADC0`, ✓ `sub_45ADD0`, ✓ `sub_4637A0` | matched | Fix16_Rect getters return a copy like 9.6f (0x45ADA0-0x45ADD0), + get_low_z; order left,right,top,bottom |
| 0x4E4AC0 | `Map_0x370::IsSearchBlockOfType` | 0x4693A0 | ✓ `sub_420420` | done | all 9.6f inlines used |
| 0x4E4BB0 | `Map_0x370::FindPavementBlockForCoord` | 0x466910 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E4C30 | `Map_0x370::FindHighestBlockForCoord` | 0x466990 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E4CB0 | `Map_0x370::FindNonAirBlockAtOrBelowZ` | 0x466A00 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E4D40 | `Map_0x370::FindGroundZBelowCoord` | 0x469570 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt; 466B70=UpdateZFromSlopeAtCoord (called); still matches |
| 0x4E4E50 | `Map_0x370::sub_4E4E50` | 0x4696C0 | `sub_466B70` (10.5 0x466B70) | checked | IsInvisibleOnFoot_466B70 is UpdateZFromSlopeAtCoord, already called |
| 0x4E4F40 | `Map_0x370::GetGroundZBelowCoord` | 0x4699A0 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt; 466B70=UpdateZFromSlopeAtCoord; still matches |
| 0x4E5170 | `Map_0x370::IsZOnGround` | 0x469B00 | ✓ `sub_42A630`, `sub_466B70` (10.5 0x466B70) | checked | IsInvisibleOnFoot_466B70 is UpdateZFromSlopeAtCoord, already called |
| 0x4E52A0 | `Map_0x370::sub_4E52A0` | 0x466AF0 | ✓ `gtx_0x106C::sub_462FB0` | done | new gtx IsWaterTile; still matches |
| 0x4E5300 | `Map_0x370::CheckZCollisionAtCoord` | 0x469C20 | `sub_466B70` (10.5 0x466B70) | checked | IsInvisibleOnFoot_466B70 is UpdateZFromSlopeAtCoord, already called |
| 0x4E5640 | `Map_0x370::sub_4E5640` | 0x469F90 | ✓ `Reset_4637B0`, `sub_40E8D0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, `PolarToCartesian_41FC20`, ✓ `sub_420420`, ✓ `sub_466CF0`, `sub_4BD670` | checked | sub_40E8D0 = atan2_fixed, sub_4BD670 = 10.5 0x5A2500, both called |
| 0x4E5B60 | `Map_0x370::FindGroundZForCoord` | 0x46A420 | `sub_466B70` (10.5 0x466B70) | checked | IsInvisibleOnFoot_466B70 is UpdateZFromSlopeAtCoord, already called |
| 0x4E6190 | `Map_0x370::sub_4E6190` | 0x466D30 | ✓ `sub_466CF0` | done | all 9.6f inlines used |
| 0x4E62D0 | `Map_0x370::FindRailwayAtCoord` | 0x463570 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E6360 | `Map_0x370::FindRailwayBelowZAtCoord` | 0x4635F0 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E6400 | `Map_0x370::GetRailwayZBelowCoord` | 0x466E20 | ✓ `sub_42A630`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70) | done | IsGradientSlopeAt; still matches |
| 0x4E6510 | `Map_0x370::GetRailwayZCoordAtXY` | 0x466F70 | `sub_466B70` (10.5 0x466B70) | checked | IsInvisibleOnFoot_466B70 is UpdateZFromSlopeAtCoord, already called |
| 0x4E65A0 | `Map_0x370::sub_4E65A0` | 0x467020 | `sub_463530` | checked | sub_463530 is FloorPlusOne, called |
| 0x4E7190 | `Map_0x370::sub_4E7190` | 0x467F80 | `sub_463150`, `sub_463210`, `ProcessObjective_4632E0`, `GetOppositeFace_42A660` | commented | ReverseRoadDirection_42A660 helper 103->301, noted only; 463150/463210/4632E0 pair with called 4E5Dxx |
| 0x4E7FC0 | `Map_0x370::CheckColumnHasSolidAbove` | 0x463850 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E80A0 | `gmp_compressed_map_32::ApplyDmapInfos` | 0x463940 | ✓ `gmp_compressed_map_32::sub_42A830` | done | get_base; still matches |
| 0x4E80E0 | `DmapInfoList::AddOrUpdateDmapInfo` | 0x463990 | ✓ `sub_463080` | done | all 9.6f inlines used |
| 0x4E8620 | `Map_0x370::ChangeBlock` | 0x463F60 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E87C0 | `Map_0x370::AddNewBlock` | 0x464060 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E8940 | `Map_0x370::RemoveBlock` | 0x464110 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E8A10 | `Map_0x370::LowerColumn` | 0x464160 | ✓ `gmp_compressed_map_32::sub_42A830` | done | gmp_compressed_map_32::get_base; still matches |
| 0x4E8E30 | `Map_0x370::do_process_loaded_zone_data` | 0x464330 | `Map_0x370::sub_462E40` | checked | sub_462E40 is Map_0x370::GetZone, called out of line in 10.5 |
| 0x4E92B0 | `Map_0x370::load_dmap` | 0x4646A0 | ✓ `sub_4630B0` | done | sub_4630B0 is the gmp_compressed_map_32 ctor already used by new; noted with comment |
| 0x4E9660 | `Map_0x370::ctor_4E9660` | 0x464A40 | `sub_4630A0`, ✓ `gmp_block_info::init_44C840` | done | new DmapInfoList ctor (4630A0), init; still matches |
| 0x4EAF40 | `MapRenderer::DrawRightSide` | 0x470250 | `sub_46BEA0` (10.5 0x4F3FB0), `Nanobotz::Set_UV_46C0C0` (10.5 0x4F4190), ✓ `TextureCache_15D8::sub_46BB50` | inlines added | GetTexture (198 unchanged); sub_46BEA0 = SetUVCorners and Set_UV_46C0C0 = Set_UV (called) |
| 0x4EB940 | `ProjectVert` | 0x46BC70 | `sub_46BBF0` (10.5 0x4EAD90) | todo |  |
| 0x4ED290 | `MapRenderer::draw_bottom` | 0x46D9A0 | ✓ `TextureCache_15D8::sub_46BB50` | done | GetTexture; unchanged (192) |
| 0x4F0340 | `MapRenderer::DrawTriangularDiagonal` | 0x46EE40 | `Nanobotz::sub_46E490`, `Nanobotz::sub_46E5C0`, `Nanobotz::sub_46E6E0`, `Nanobotz::sub_46E800`, `Nanobotz::sub_46E910`, `Nanobotz::sub_46EA30`, `Nanobotz::SpawnDummies_46EB60`, `Nanobotz::sub_46EC90` | checked | Nanobotz slope draw callees pair with out-of-line 10.5 functions (called) |
| 0x4F3FB0 | `SetUVCorners` | 0x46BEA0 | ✓ `sub_432860`, `sub_40F600` | checked | sub_40F600 is the Fix16_Point operator- (operator) |
| 0x4F4D60 | `MapRenderer::draw_partial_lid` | 0x470800 | ✓ `Nanobotz::ProjectVertTop_46BD40` (10.5 0x4EAE00), ✓ `Nanobotz::GetColour_46B5E0`, ✓ `TextureCache_15D8::sub_46BB50` | commented | GetColour 43->105 |
| 0x4F6630 | `MapRenderer::DrawGradientSlope` | 0x471CE0 | `Nanobotz::DrawGradientSlopeNorthwards_46EF10`, `Nanobotz::sub_46F370`, `Nanobotz::sub_46F7C0`, `Nanobotz::sub_46FC10` | checked | Nanobotz gradient draw callees pair with out-of-line 10.5 functions (called) |
| 0x4F66C0 | `MapRenderer::RenderFlatBlock` | 0x471D60 | `Nanobotz::sub_470060`, `Nanobotz::sub_470440`, `Nanobotz::sub_470620`, `Nanobotz::sub_46C2C0`, `Nanobotz::sub_46C7F0`, `Nanobotz::sub_46CE30`, `Nanobotz::sub_46DFE0` | checked | Nanobotz draw callees (470060.., 46C2C0..) pair with the out-of-line 10.5 draw functions (called) |
| 0x4F7AE0 | `rng::get_int` | 0x472E00 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x4F7B70 | `rng::get_uint8` | 0x472E90 | ✓ `rng::get_cur_rng_41CFE0` | done | rng::get_cur_rng; Still matches. |
| 0x4FA330 | `AmbulanceService::HandlePedDeath` | 0x473010 | ✓ `cool_nash_0x294::sub_403990` | done | code unchanged |
| 0x4FA500 | `AmbulanceService::ProcessPatientQueue` | 0x473E00 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_z_416B50`, `EmergencyCrew_30::sub_4C54F0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | checked | get_cam_x/y/z (9.6f calls) give no change; diff is load/store scheduling of x/y/z |
| 0x4FA7D0 | `AmbulanceTask::ClearTask` | 0x473140 | ✓ `Ped_Unknown_4::ClearList_420E90` | done | all 9.6f inlines used |
| 0x4FA820 | `AmbulanceTask::SpawnParamedicCrew` | 0x473170 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433BB0`, ✓ `sub_433B90`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::SetUniNum_421560` | done | code unchanged |
| 0x4FA9D0 | `AmbulanceTask::EvaluatePickupState` | 0x473320 | ✓ `cool_nash_0x294::sub_403B60`, ✓ `IsMaxDamage_40F890` | done | all 9.6f inlines used |
| 0x4FAAC0 | `AmbulanceTask::HandleObjectiveState` | 0x473410 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_objective_403A80`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `cool_nash_0x294::get_objective_timer_403B30`, ✓ `sub_472FD0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `GetObjectiveStatus_450CB0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403B40`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_403960` | inlines added | get_cam_x/y/z for the objective target copy (score 64->48); MaxAbsDistance spot: 10.5 calls Abs_436A50/Max_44E540 out of line, load order still differs |
| 0x4FB330 | `AmbulanceTask::UpdateState` | 0x473CE0 | ✓ `Char_8::sub_420EA0`, ✓ `MarkForDespawn_421470` | inlines added | HasNoPeds added, no codegen change; case 3 else should share the default case's epilogue |
| 0x4FF250 | `FrameProfiler::DebugDrawProfiling` | 0x474530 | `sub_4744C0`, `sub_4740F0`, `sub_474490`, `unknown_libname_28`, `sub_4741F0` | matched | Fixed by `const s32& polys_drawn` (ref_local), not by the sum's term order |
| 0x4FFA90 | `FrameProfiler::UpdateProfiling` | 0x474C60 | ✓ `sub_474430`, ✓ `sub_474450` | done | TakeElapsed/AddSample are 474430/474450 (address comments added) |
| 0x502DC0 | `MissionCleanupList::MissionCleanUp` | 0x476BC0 | ✓ `Car_BC::SetUniNum_421560`, ✓ `IsDeactivated_4214D0`, ✓ `MarkForDespawn_421470` | done | all 9.6f inlines used |
| 0x502FF0 | `MissionCleanupList::push_type_2_non_pickup_object` | 0x476D50 | ✓ `sub_40FEF0` | done | Object::GetModel x4; code unchanged |
| 0x503200 | `ScriptThread::MissionFailOnDeathOrArrest` | 0x47F550 | ✓ `sub_476700`, ✓ `sub_476730`, `text_0x14::Find_4C23A0`, `sub_4105B0` | done | Player::IsWastedOrBustedNotObjective54 and IsBustedObjective54 (new); code unchanged |
| 0x5035D0 | `ScriptThread::Log` | 0x476E10 | ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x503680 | `ScriptThread::SCRCMD_OBJ_DECSET_2D_3D` | 0x476EA0 | ✓ `sub_475AA0`, `sub_4CA910` | done | Object::IsModel176To181 (new); sub_4CA910 is place_gang_phone (called); code unchanged |
| 0x5038D0 | `ScriptThread::SCRCMD_OBJ_DECSET` | 0x476FF0 | ✓ `sub_475A70`, ✓ `sub_447E90`, ✓ `check_is_shop_421060`, ✓ `sub_45E0A0`, ✓ `sub_475A60`, `sub_475A50` | done | Object::IsStaticCollisionCheck (new); IsShop noted as 9.6f 0x475A60; SetParam and IsPickup already used; code unchanged |
| 0x503A20 | `ScriptThread::Handle_PLAYER_PED` | 0x477140 | ✓ `sub_475A20`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x503BC0 | `ScriptThread::SCRCMD_CAR_DECSET` | 0x477290 | `EnqueueRadioLocationPhrase_426E10`, ✓ `sub_4764A0`, ✓ `sub_475C30`, ✓ `sub_476270`, ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_475C40` | done | Car::LockoutThiefUnlessPermanent and SetMiniCar (new), LockPermanently, sub_421560; SpawnFullSizeCar/4764A0 already used; code unchanged |
| 0x503FB0 | `ScriptThread::SCRCMD_CHAR_DECSET_2D_3D` | 0x477560 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x5043A0 | `ScriptThread::SCRCMD_CONVEYOR_DECSET1_2` | 0x4777E0 | `sub_483C00` | checked | 9.6f sub_483C00 = Object::sub_529030 (still called) |
| 0x5045D0 | `ScriptThread::Handle_THREAD_DECLARE2` | 0x4779A0 | ✓ `sub_420B60`, ✓ `sub_476360` | done | Car::SetCarThreadPending (new); get_id (420B60) already used; code unchanged |
| 0x5047C0 | `ScriptThread::Handle_THREAD_DECLARE4` | 0x477B70 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x504830 | `ScriptThread::ApplyGangInfo` | 0x477BD0 | ✓ `sub_4758A0`, ✓ `sub_4758B0`, ✓ `sub_4758C0`, ✓ `sub_4758D0`, ✓ `sub_475940`, ✓ `sub_4758E0`, ✓ `sub_4758F0`, ✓ `sub_475910`, ✓ `sub_475950` | done | Gang::SetInfoPhonePos (new); code unchanged |
| 0x504970 | `ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1` | 0x477D20 | ✓ `sub_476990`, ✓ `sub_476A10`, ✓ `sub_476A20` | inlines added | Door::set_field_24 (9.6f 0x476A10); set_field_1c_1e (0x476A20) changes the code, comment updated here and in SCRCMD_DOOR_DECLARE_D2_S2; code unchanged |
| 0x504B80 | `ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2` | 0x477EE0 | ✓ `sub_4769E0`, ✓ `sub_476A00`, ✓ `sub_4769A0`, ✓ `sub_4769C0`, ✓ `sub_476990`, ✓ `sub_476A10`, ✓ `sub_476A20` | commented | door open/close helpers (4769A0/C0/E0, 476A00, 476990, 476A10) already used; set_field_1c_1e (476A20) breaks the match, comment updated |
| 0x504EE0 | `ScriptThread::CreateLight` | 0x47F710 | ✓ `sub_469010` (10.5 0x52B2A0), ✓ `Light_1D4CC::SetFlashing_469070` | done | all 9.6f inlines used |
| 0x5051D0 | `ScriptThread::Handle_RADIOSTATION_DEC` | 0x4753B0 | `unknown_libname_9` | checked | unknown_libname_9 is a thunk on an unknown 9.6f global (0x5DCBC8), source calls the 10.5 function |
| 0x505790 | `ScriptThread::DisableThread` | 0x478240 | ✓ `sub_475A80`, ✓ `sub_420B60`, ✓ `sub_4768C0`, ✓ `sub_4218E0` | done | CarThreadTriggers::SetTriggerEnableState; code unchanged |
| 0x505B10 | `ScriptThread::DeallocOrDeleteItem` | 0x47F760 | ✓ `MarkForDespawn_421470`, ✓ `sub_47F4F0`, `unknown_libname_10` | checked | Car::sub_421470 and DeallocLight noted/used already |
| 0x505F50 | `ScriptThread::SCRCMD_DISPLAY_MESSAGES` | 0x4755F0 | `text_0x14::Find_4C23A0` | todo |  |
| 0x5061C0 | `ScriptThread::ExecOpCode` | 0x481400 | `miss2_0x11C::SCRCMD_CRANE_477660`, `sub_477870`, `sub_478120`, `sub_478170`, `sub_477530`, `sub_4784A0` | checked | listed callees are SCRCMD handlers (sub_477870 etc.), called in 10.5 |
| 0x506B80 | `ScriptThread::Handle_RETURN` | 0x478610 | `sub_476E00` | checked | 9.6f sub_476E00 is a thunk (frismo pool dealloc on a global); source calls field_114->sub_5031C0 |
| 0x508220 | `ScriptThread::Handle_MAKE_CAR_DUMMY` | 0x478BA0 | ✓ `sub_4118B0`, ✓ `Car_BC::SetUniNum_421560` | done | Car::sub_421560 (9.6f 0x421560); code unchanged |
| 0x508550 | `ScriptThread::Handle_POINT_ARROW_3D` | 0x478E10 | ✓ `sub_476840` | done | Hud_Arrow::SetArrowAim; code unchanged |
| 0x5086F0 | `ScriptThread::Handle_POINT_ARROW_AT` | 0x478E90 | ✓ `sub_476860`, ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_476790`, ✓ `sub_476850`, ✓ `sub_476870`, ✓ `sub_476840` | done | Hud_Arrow::SetArrowTargetPed/Car_476860/Object_476870 and ArrowTrace::SetTargetPed/SetTargetObject (new), Ped::is_player; code unchanged |
| 0x509A70 | `ScriptThread::Handle_CAR_IN_AREA` | 0x4799D0 | ✓ `sub_463710` | done | Fix16_Rect::SetFromPosSize (new); code unchanged |
| 0x509BB0 | `ScriptThread::Handle_HAS_CHAR_DIED` | 0x479DA0 | `sub_433C20` | checked | 9.6f sub_433C20 (char velocity) has no open-coded counterpart; the dead check reads field_278 directly |
| 0x509C10 | `ScriptThread::Handle_IS_CHAR_IN_CAR` | 0x479B70 | ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60` | done | Ped::has_car and get_car; code unchanged |
| 0x509C90 | `ScriptThread::Handle_IS_CHAR_IN_MODEL` | 0x479BF0 | ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_411940` | done | Ped::has_car, get_car and Car::GetCarInfoIdx; code unchanged |
| 0x509D00 | `ScriptThread::Handle_IS_CHAR_IN_ANY_CAR` | 0x479C60 | ✓ `cool_nash_0x294::has_car_403B80` | done | all 9.6f inlines used |
| 0x509D90 | `ScriptThread::SCRCMD_ADD_SCORE` | 0x479D40 | ✓ `sub_41B0A0`, ✓ `sub_41DC40` | done | Player::AddCash (new); code unchanged |
| 0x509E00 | `ScriptThread::Handle_ADD_SCORE2` | 0x479CC0 | ✓ `sub_41B0A0`, ✓ `sub_41DC40`, `sub_421990` | done | Ped::is_player and Player::AddScore; code unchanged |
| 0x50A200 | `ScriptThread::Handle_SET_CHAR_OBJ2` | 0x47A1E0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | done | all 9.6f inlines used |
| 0x50A460 | `ScriptThread::Handle_SET_CHAR_OBJ_FOLLOW` | 0x47A3B0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | all 9.6f inlines used |
| 0x50A760 | `ScriptThread::IsOnScreen` | 0x47A620 | ✓ `sub_411A00` | done | Crane::GetRotorSprite; code unchanged |
| 0x50A9E0 | `ScriptThread::EnableThread` | 0x47A860 | ✓ `sub_4768C0` | done | CarThreadTriggers::SetTriggerEnableState (new); code unchanged |
| 0x50AC20 | `ScriptThread::SCRCMD_SET_GANG_KILL_REACTION` | 0x47AAB0 | ✓ `sub_475900` | done | all 9.6f inlines used |
| 0x50ACF0 | `ScriptThread::Handle_CHANGE_RESPECT` | 0x47AB80 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_475900` | done | all 9.6f inlines used |
| 0x50AEF0 | `ScriptThread::RespectOperator` | 0x47ACC0 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50B0E0 | `ScriptThread::Handle_ADD_PATROL_POINT` | 0x47ADF0 | ✓ `cool_nash_0x294::get_objective_403A80` | done | all 9.6f inlines used |
| 0x50B180 | `ScriptThread::Handle_ANSWER_PHONE` | 0x47AEA0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x50B3D0 | `ScriptThread::Handle_IS_CHAR_FIRE_ONSCREEN` | 0x47B0C0 | ✓ `sub_433CA0`, ✓ `cool_nash_0x294::sub_4039F0` | done | Ped::GetAttacking and GetOffscreenCounter; code unchanged |
| 0x50B4F0 | `ScriptThread::Handle_CHAR_TO_DRIVE_CAR` | 0x47B1D0 | ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | done | all 9.6f inlines used |
| 0x50B600 | `ScriptThread::Handle_GIVE_DRIVER_BRAKE` | 0x47B2D0 | ✓ `sub_4118B0`, ✓ `sub_421540`, ✓ `Car_BC::SetUniNum_421560` | done | Car::SetAIHalted (Car.hpp also has an identical SetA6Bit20_421540 from another worker); code unchanged |
| 0x50B6F0 | `ScriptThread::Handle_CHECK_SCORE` | 0x47B3C0 | ✓ `sub_41B0A0`, ✓ `sub_421980` | done | Ped::is_player and Player::GetScore; code unchanged |
| 0x50B760 | `ScriptThread::Handle_GET_SCORE` | 0x47B430 | ✓ `sub_41B0A0`, ✓ `sub_421980` | done | Ped::is_player and Player::GetScore; code unchanged |
| 0x50B7D0 | `ScriptThread::Handle_IS_CHAR_IN_GANG` | 0x47B4A0 | ✓ `sub_41B0A0`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | Ped::is_player; code unchanged |
| 0x50BA30 | `ScriptThread::Handle_CLEAR_WANTED_LEVEL` | 0x47BA80 | `sub_475AF0`, `sub_475AE0` | done | Ped::ClearWantedPoints (the 9.6f pair is a different command, group field_35 setter: pairing noise); code unchanged |
| 0x50BA70 | `ScriptThread::Handle_ALT_WANTED_LEVEL` | 0x47B7B0 | `cool_nash_0x294::sub_434D60` | checked | 9.6f sub_434D60 = WantedStarCountGreaterThan (still called) |
| 0x50BC60 | `ScriptThread::Handle_CHECK_NUM_ALIVE` | 0x47C050 | `sub_433B60`, `sub_433B70` | checked | pairing noise: GetPedState2/get_health not open-coded (function reads group field_34) |
| 0x50BD10 | `ScriptThread::SCRCMD_CHAR_GROUP` | 0x47BAC0 | ✓ `sub_475AF0` | done | all 9.6f inlines used |
| 0x50BE00 | `ScriptThread::Handle_HAS_CAR_WEAPON` | 0x47BBE0 | `cool_nash_0x294::has_car_403B80`, `cool_nash_0x294::get_car_416B60`, `sub_411970` | checked | pairing noise: source just calls find (no ped/car getters to replace) |
| 0x50BED0 | `ScriptThread::Handle_CHECK_MAX_PASS` | 0x47BC50 | `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_car_416B60` | checked | get_car already used |
| 0x50C350 | `ScriptThread::Handle_GET_LAST_PUNCHED` | 0x47C000 | ✓ `sub_475B10`, ✓ `sub_475B40` | done | Ped::clear_last_char_punched (new); code unchanged |
| 0x50C5A0 | `ScriptThread::Handle_EXPLODE_ITEM` | 0x47C1E0 | ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_4340F0`, `sub_4340E0`, `sub_4340D0` | commented | Object get_x/y/z_4340x0 break the match; Ped cam getters already used |
| 0x50C6F0 | `ScriptThread::Handle_PARK` | 0x47C3C0 | `sub_475B10` | checked | get_last_char_punched not applicable here (pairing noise) |
| 0x50C7D0 | `ScriptThread::Handle_UPDATE_DOOR` | 0x47C410 | ✓ `sub_476A90`, ✓ `sub_4769E0`, ✓ `sub_4769C0`, ✓ `sub_476A00` | checked | set_open_details_* and set_field_14_id (4769C0/E0, 476A00) already used |
| 0x50CAB0 | `ScriptThread::Handle_ADD_NEW_BLOCK` | 0x47C6A0 | ✓ `gmp_block_info::init_44C840` | done | gmp_block_info::init; code unchanged |
| 0x50CE50 | `ScriptThread::Gosub` | 0x47C8D0 | `sub_476DF0` | checked | 9.6f sub_476DF0 is a thunk on a global pool (frismo push); source calls the 10.5 function |
| 0x50CE90 | `ScriptThread::Handle_PHONE_TEMPLATE` | 0x47FAC0 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50D3C0 | `ScriptThread::Handle_DO_SAVE_GAME` | 0x47FE80 | ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_421980`, `sub_4105B0`, ✓ `sub_421990`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0) | done | Player::GetScore and AddCash; get_cur_rng already used; code unchanged |
| 0x50D870 | `ScriptThread::Handle_CHANGE_INTENSITY` | 0x47CCC0 | `sub_476610` | checked | pairing noise (9.6f sub_476610 copies a Fix16 pair, not used here; SetIntensity already used) |
| 0x50D900 | `ScriptThread::Handle_CHANGE_COLOUR` | 0x47CD40 | `sub_433B60`, `cool_nash_0x294::sub_403990` | checked | pairing noise (ped state getters, function sets a light colour) |
| 0x50DA50 | `ScriptThread::SCRCMD_CREATE_LIGHT` | 0x47CA20 | `sub_476AE0` | checked | SetIntensity noted; function calls CreateLight |
| 0x50DD00 | `ScriptThread::SCRCMD_GET_LIVES_MULT` | 0x47CC50 | ✓ `sub_4766C0`, ✓ `angry_lewin_0x85C::sub_4766A0` | done | Player::get_lives and get_multiplier; code unchanged |
| 0x50DE00 | `ScriptThread::Handle_POINT_ONSCREEN` | 0x47CDA0 | `ScriptManager_C1EA8::sub_476200` (10.5 0x512770), `sub_433B60`, `cool_nash_0x294::sub_403990` | checked | pairing noise (calls IsPointOnScreen) |
| 0x50E360 | `ScriptThread::Handle_CHECK_CAR_SPEED` | 0x47D170 | ✓ `sub_4211C0`, ✓ `sub_4754D0` | checked | CarPhysics::get_car_velocity uses GetLength_41E260, the source needs the GetLength_no_sqrt_inline_abs_y_negate variant to match (helper body differs); get_value already used |
| 0x50E460 | `ScriptThread::Handle_SET_CAR_GRAPHIC` | 0x47F890 | `sub_47ECB0`, `miss2_0x11C::sub_475B70` (10.5 0x511930) | checked | sub_47ECB0/InitThread copy (475B70) are not open-coded here |
| 0x50E730 | `ScriptThread::Handle_CHAR_DRIVE_AGGR` | 0x47E040 | `cool_nash_0x294::has_car_403B80`, `cool_nash_0x294::get_car_416B60`, `sub_421540` | checked | pairing noise: source only sets Ped field_21C b3; listed has_car/get_car/SetA6Bit5 belong to another command |
| 0x50E780 | `ScriptThread::SCRCMD_SET_SPEED` | 0x47F920 | `sub_483C60` (10.5 0x5291E0), `sub_47F420` (10.5 0x512AA0) | checked | 483C60/47F420 have 10.5 copies (Object::sub_5291E0, frosty sub_512AA0) not open-coded here (pairing noise) |
| 0x50E900 | `ScriptThread::Handle_PUT_CAR_ON_TRAILER` | 0x47D9D0 | `sub_476950`, `sub_476930` | done | Car::IsTrailerCab (9.6f pairs these callees with 0x50E9E0; GetSprite = 41FC70 already used); code unchanged |
| 0x50E9E0 | `ScriptThread::Handle_CHECK_HEADS` | 0x47D5D0 | `Car_BC::has_trailer_41E460`, `sub_41FC70`, `sub_4B9D50` (10.5 0x5A3100) | checked | its listed callees (has_trailer_41E460, 41FC70, 4B9D50) belong to PUT_CAR_ON_TRAILER_50E900 (pairing swap), done there |
| 0x50EB00 | `ScriptThread::Handle_CHECK_WEAPONHIT` | 0x47D7A0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x50EDC0 | `ScriptThread::Handle_DO_EASY_PHONE` | 0x480080 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x50F270 | `ScriptThread::Handle_WARP_CHAR` | 0x47DAD0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_433C00`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410`, ✓ `DrawUnk_0xBC::sub_475B60` | done | Ped::SetRotation and Camera::SetSmoothFollow; get_car and CommitPendingTarget already used; code unchanged |
| 0x50F3D0 | `ScriptThread::Handle_SET_GROUP_TYPE` | 0x47E210 | `sub_476660` | checked | pairing noise: 9.6f sub_476660 reads a byte from a 0x2C-stride table (frosty), not the ped group type set here |
| 0x50F450 | `ScriptThread::Handle_EMERG_LIGHTS` | 0x4803C0 | ✓ `HasEmergencyLights_414F20` | done | Car::HasEmergencyLights; code unchanged |
| 0x510100 | `ScriptThread::Handle_START_BASIC_KF` | 0x480430 | ✓ `sub_475A30`, ✓ `sub_475A40`, `text_0x14::Find_4C23A0`, `sub_4105B0`, ✓ `cool_nash_0x294::get_car_416B60` | checked | get/set_field_C1E2D_475A30/475A40 and get_car already used; sub_4105B0 thunk on an unknown 9.6f global |
| 0x510280 | `ScriptThread::Handle_DO_BASIC_KF` | 0x47E7F0 | ✓ `sub_476660`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_476680`, `sub_4C93B0`, ✓ `sub_475A40`, `text_0x14::Find_4C23A0`, ✓ `cool_nash_0x294::sub_420B80`, ✓ `sub_41DC40`, ✓ `sub_4766B0`, `sub_4A4D50`, `sub_4105B0` | done | BonusTracker::GetProgressCount and DeactivateBonus (new), Ped::ClearWantedPoints, Player::ChangeMultipliers; SetKillFrenzyActive/GetPedState/AddScore already used; sub_4A4D50 = ChangeLifeCountByAmount (still called); code unchanged |
| 0x510780 | `ScriptThread::SCRCMD_SAVE_RESTORE_RESPECT` | 0x4802F0 | `ScriptManager_C1EA8::sub_476200` (10.5 0x512770), `sub_43DF60`, `cool_nash_0x294::sub_403920`, `sub_436070` (10.5 0x470F30), `sub_433B90`, `cool_nash_0x294::set_occupation_403970`, `sub_476D20`, `Car_BC::SetUniNum_421560`, `AllocateAI_421510`, `sub_42A9D0` (10.5 0x453BF0), `sub_425DD0` (10.5 0x43BCA0) | checked | pairing noise: 9.6f 0x4802F0 is a ped-spawning command; source only saves/restores gang respect |
| 0x5108D0 | `ScriptThread::PreExecOpCode` | 0x4805B0 | `sub_479850`, `sub_479F10`, `sub_47B5E0`, `sub_47B810`, `sub_47BCD0`, `sub_47BE00`, `sub_47D070`, `sub_47DE70`, `sub_47E360` | checked | listed callees are SCRCMD handlers (still called) |
| 0x511A70 | `ScriptManager::SetGeneratorTypeForCar` | 0x47ED20 | `sub_476950`, `sub_476930` | todo |  |
| 0x511E10 | `ScriptManager::SaveGame` | 0x47EF40 | `sub_483D90`, ✓ `GameSession_578::sub_453A80`, `sub_475CA0` | checked | get_secret_tokens_collected already used; sub_483D90/sub_475CA0 are big callees the source calls as 10.5 functions |
| 0x511F80 | `ScriptManager::LoadSave` | 0x47F0B0 | `sub_47EF10`, ✓ `sub_476B10` | done | GameSession::set_secret_tokens_collected (new); sub_47EF10 = LoadScriptCounters (still called); code unchanged |
| 0x5120C0 | `ScriptManager::SpawnThread` | 0x47F200 | ✓ `miss2_0x11C_Pool::Allocate_4767A0` | done | all 9.6f inlines used |
| 0x512160 | `ScriptManager::StartLevelThread` | 0x481890 | ✓ `miss2_0x11C_Pool::Allocate_4767A0` | done | all 9.6f inlines used |
| 0x512AF0 | `ScriptManager::TestAndRecordWeaponHit` | 0x476400 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | IsPedCheck/IsCarCheck (+IsCheckHit in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512BA0 | `ScriptManager::ClearWeaponHitFlag` | 0x4764D0 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | IsPedCheck/IsCarCheck (+IsCheckHit in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512C00 | `ScriptManager::RecordWeaponHit` | 0x476530 | ✓ `sub_4759C0`, ✓ `sub_4759A0` | done | IsPedCheck/IsCarCheck (+IsCheckHit in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512C70 | `ScriptManager::IsWeaponHitRecorded` | 0x4765A0 | ✓ `sub_4759C0`, ✓ `sub_4759A0`, ✓ `sub_475980` | done | IsPedCheck/IsCarCheck (+IsCheckHit in 512C70) (new); code unchanged; the 9.6f ternary form breaks the match |
| 0x512CE0 | `ScriptManager::ctor_512CE0` | 0x481960 | ✓ `miss2_0x11C_Pool::ctor_481310` | matched | all 9.6f inlines used |
| 0x5130E0 | `ScriptManager::dtor_5130E0` | 0x481C30 | `miss2_0x11C_Pool::gdtor_481C10` | checked | ScriptThreadPool::gdtor_481C10 is a scalar deleting dtor, compiler generated |
| 0x513240 | `Bink::TickFrame` | 0x481E00 | ✓ `sub_481DF0`, `sub_481D30` (10.5 0x513390) | matched | new IsDirectBufferMode (Bink.cpp static); 481D30=CloseSlot2 not in 10.5 version; unchanged (38) |
| 0x513560 | `Bink::OpenSlot1` | 0x481F20 | ✓ `sub_481DF0` | matched | IsDirectBufferMode; unchanged (29) (also used in OpenSlot2) |
| 0x516590 | `TagGame::SetNewFugitive` | 0x4820D0 | `sub_4C8620`, ✓ `sub_41D020`, `cool_nash_0x294::sub_435F00`, ✓ `sub_482080`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `SetTargetPlayer_4820A0`, `sub_4C83D0`, `text_0x14::Find_4C23A0` | checked | 4C8620=ReleaseAllArrows, 435F00=ClearInvulnerable, 4C83D0=SetPlayerArrowColour, all called in 10.5 |
| 0x516660 | `TagGame::ExecuteGamemodeTick` | 0x4821C0 | `unknown_libname_24`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_41D020`, ✓ `cool_nash_0x294::get_car_416B60` | checked | unknown_libname_24 is a CRT/library thunk |
| 0x519FE0 | `NetUi::OnWmCommand` | 0x407DB0 | `text_0x14::dtor_405A80` | checked | TextTable::dtor_405A80 is the scalar deleting dtor (compiler generated) |
| 0x51A9D0 | `NetUi::OnTimer` | 0x406E40 | `text_0x14::dtor_405A80` | checked | TextTable::dtor_405A80 is the scalar deleting dtor (compiler generated) |
| 0x51AA90 | `NetUi::CreateMainUi` | 0x4086A0 | `text_0x14::Find_4C23A0`, `sub_4C23B0` | checked | Find_4C23A0/sub_4C23B0 are inside GetNarrowText (called) |
| 0x51AC60 | `NetUi::OnInitDialog` | 0x408840 | `text_0x14::ctor_4C2620` (10.5 0x5B5FB0), `text_0x14::Load_4C2540` (10.5 0x5B5E90), `sub_4061C0` | checked | 9.6f created/loaded the text object here (ctor/Load are out of line in 10.5, not called here); sub_4061C0 = SetPlayerNameText (called) |
| 0x51AFA0 | `NetUi::PopulateMainUI` | 0x407070 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51B810 | `NetUi::ApplyHostGameSettings` | 0x4073D0 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51B9C0 | `NetUi::SetSetting` | 0x4075C0 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51CB30 | `NetUi::SetPlayerCountText` | 0x4065F0 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51CD30 | `NetUi::SetJoinedGamePoliceEnabledText` | 0x406720 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51CDC0 | `NetUi::SetFragsNumberAndLabel` | 0x406770 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51CFC0 | `NetUi::SetGameSpeedTextLabelAndSlider` | 0x4068C0 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51D0C0 | `NetUi::SetJoinedGameTypeAndFragLimitText` | 0x406980 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51D2F0 | `NetUi::SetJoinedGameTimeLimitText` | 0x406B10 | `sub_4C23B0` | checked | sub_4C23B0 is GetNarrowText, called out of line in 10.5 |
| 0x51D7B0 | `NetPlay::vdtor` | 0x406BC0 | `sub_405A40` | checked | sub_405A40 is the vtable store of the dtor |
| 0x51ED00 | `NetPlay::NetworkTick` | 0x40C380 | `sub_40BF80` | checked | sub_40BF80 is a 9.6f dispatch thunk with no 10.5 form |
| 0x51F420 | `NetPlay::MakeSendData` | 0x40A7F0 | ✓ `sub_409C40`, ✓ `sub_409C50` | done | GetPlayerIdx x3; SeqDiff=409C50; still matches |
| 0x5213E0 | `NetPlay::WaitForPlayersSync` | 0x40BFA0 | ✓ `sub_409C40` | done | GetPlayerIdx; unchanged (15) |
| 0x521630 | `NetPlay::SendToPlayer` | 0x40ACC0 | ✓ `sub_409C40` | done | GetPlayerIdx; still matches |
| 0x521770 | `NetPlay::GetOldestBufferedPacket` | 0x40A190 | ✓ `sub_409C50` | done | SeqDiff noted as 9.6f 0x409C50 (already used) |
| 0x521890 | `NetPlay::ReceiveGameMessage` | 0x40C120 | ✓ `sub_409C50`, ✓ `sub_409C40` | done | GetPlayerIdx x2; SeqDiff is 409C50 (comment added); unchanged (220) |
| 0x521B20 | `NetPlay::SendToAll` | 0x40AD70 | ✓ `sub_409C40` | done | GetPlayerIdx x2; still matches |
| 0x521BE0 | `NetPlay::NoRefs_SendToAllDelayed` | 0x40AE30 | ✓ `sub_409C40` | done | GetPlayerIdx; still matches |
| 0x521C80 | `NetPlay::NoRefs_SendTickedStandardPacket` | 0x40AED0 | ✓ `sub_409C40` | done | GetPlayerIdx; still matches |
| 0x521FD0 | `Object_3C::GetMovementSpeedAndAngle` | 0x482D90 | ✓ `sub_482BA0`, ✓ `sub_40F790`, ✓ `sub_482BD0` | matched | GetSpeedVector_482BA0 added; VC6 keeps it out of line (COMDAT), diff 82->82 |
| 0x522180 | `Object::PoolDeallocate` | 0x484910 | ✓ `sub_421080`, ✓ `SpritePool::sub_421030` | done | UsesParamAsPedRef used; code unchanged |
| 0x522250 | `Object::IsSpriteOwnerPed` | 0x482E80 | ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_40FEA0`, ✓ `sub_420B50` | done | UsesParamAsPedRef used; code unchanged |
| 0x5223C0 | `Object::ShouldCollideWith` | 0x482FA0 | ✓ `sub_416B40` | done | all 9.6f inlines used |
| 0x522460 | `Object::SelectCollisionSprite` | 0x483060 | ✓ `sub_416B40`, ✓ `SetSprite_40FEE0` | done | GetType + CollisionTarget SetSprite used; code unchanged |
| 0x5224E0 | `Object::SetMovementVector` | 0x4849B0 | ✓ `sub_40F790`, `sub_4847D0` | matched | GetSpeedVector already inlined in ours; 4847D0 = NewObj3C, which 10.5 still calls (pairing noise) |
| 0x5226A0 | `Object::SetConveyorPush` | 0x484AA0 | `sub_4847D0` | checked | 4847D0 = NewObj3C, still called |
| 0x522710 | `Object::ResolveCollisionWithObject` | 0x486580 | `sub_482C30`, `sub_40F600`, ✓ `sub_482C90`, ✓ `sub_482C80`, `sub_40F640` (10.5 0x43D5D0), `sub_40EAB0` | matched | GetMass added, CanBePushed used; 482C30/40EAB0/40F640 are real calls in 10.5; diff 235->177 |
| 0x522B20 | `Object::ResolveCollisionWithWorld` | 0x486950 | ✓ `sub_482C80`, `sub_482C30` | done | GetMass x2 (482C30 = GetXY, still called in 10.5); diff 53->48 |
| 0x522BE0 | `Object::ResolveCollisionWithHorizontalEdge` | 0x4869E0 | `sub_4BCD00`, ✓ `sub_432860`, `sub_4828C0`, `sub_40F640` (10.5 0x43D5D0) | done | SetXY + operator-= (4828C0) like 522D00, point decl order fixed; 4BCD00=GetNearestHorizontalEdge (called); diff 50->0 CANDIDATE MATCH |
| 0x522D00 | `Object::ResolveCollisionWithVerticalEdge` | 0x486B20 | `sub_4BCFA0`, ✓ `sub_432860`, `sub_4828C0`, `sub_40F640` (10.5 0x43D5D0) | done | already used SetXY/-=; point decl order + t2.x restore fixed; 4BCFA0=GetNearestVerticalEdge (called); diff 63->0 CANDIDATE MATCH |
| 0x522E10 | `Object::HandleCollision` | 0x486C60 | ✓ `sub_40FEB0`, ✓ `sub_482CC0`, ✓ `sub_40FEA0`, `sub_4867E0`, ✓ `sub_40FEC0` | done | HandleObjectCollisionAt added (CarPhysics.hpp), v13 hoisted; 4867E0=ResolveCollisionWithPed (called); diff 52->17 (left: 5229B0 a4 is s32 but orig passes unextended, tail merge) |
| 0x5233A0 | `Object::TryStartSlide` | 0x483460 | ✓ `sub_482BF0`, ✓ `sub_40F7B0` | done | SetTopDrawLayer added+used; code unchanged |
| 0x523440 | `Object::HandleCollisionOutcome` | 0x486D70 | ✓ `sub_420660`, ✓ `SetFloorBelow_482A70`, ✓ `SetBlockAtZ_482A80` | done | CollisionTarget::SetFloorBelow/SetBlockAtZ added+used; code unchanged |
| 0x5235B0 | `Object::HandleSpriteGroundAndCollision` | 0x484090 | ✓ `sub_466CF0`, `sub_4699A0` (10.5 0x4E4F40), `sub_483100`, ✓ `sub_420420`, `sub_4BD670`, ✓ `sub_4207B0`, `sub_483500` | checked | 4699A0/483100/4BD670/4207B0/483500 are calls in 10.5 |
| 0x5238B0 | `Object::HandleSpriteZCollision` | 0x4843A0 | ✓ `sub_466CF0`, ✓ `sub_420420`, `sub_4699A0` (10.5 0x4E4F40), ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_483100`, `sub_4BD670`, ✓ `sub_4207B0`, `sub_483500`, ✓ `sub_482BE0` | done | set_xyz_lazy x2; 4699A0/483100/4BD670/4207B0/483500 are called in 10.5 (pairing noise); diff 85->85 |
| 0x523BF0 | `Object::IntegrateMovementAndCollisions` | 0x486E90 | ✓ `sub_482BE0`, ✓ `Reset_4637B0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `sub_466CF0`, ✓ `PolarToCartesian_41FC20`, `sub_484260`, `sub_483100`, `sub_4BD670`, `sub_4207B0` | checked | 484260=HandleSpriteGroundAndCollisionSimple, 483100=522FA0, 4BD670=5A2500, 4207B0=get_x_y_443580: all still called in 10.5 (pairing noise) |
| 0x524630 | `Object::IntegrateHorizontalMovementAndCollisions` | 0x4874D0 | ✓ `Reset_4637B0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_466CF0`, `sub_483100`, ✓ `sub_420420`, `sub_4BD670`, `sub_4824E0`, ✓ `sub_4207B0`, `sub_483500` | done | 4637B0, 416B40, 4206F0, 482A30, 41FC20, 466CF0 x3, 420420 used; Fix16::operator++(int) added (4824E0); 483100/4BD670/4207B0/483500 called in 10.5; diff 518->349 |
| 0x525190 | `Object::OnExplosionNearby` | 0x4856E0 | ✓ `sub_482400`, ✓ `sub_420F10`, `sub_482790` | matched | IsExplosiveItemDefinition added, GetPedId used; 482790=SetDamageOwner (called); diff 32->31 (PoolGiveAndQueueObjectType arg looks s32 in orig) |
| 0x525370 | `Object::CanObjectCollideWithSprite` | 0x4835E0 | `sub_483570`, ✓ `sub_421050`, ✓ `sub_40FEB0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_482C90`, ✓ `sub_416B40`, ✓ `sub_40FEC0`, ✓ `check_is_shop_421060` | matched | CanBePushed used; 483570=525290 (called), 421050=get_field_26 (already); diff 49->43 |
| 0x5257D0 | `Object::UpdateAnimation` | 0x485FD0 | ✓ `sub_4206C0`, ✓ `sub_482C10` | done | set_id_lazy + IsAnimFinished (added) used; code unchanged |
| 0x525B80 | `Object::UpdatePhysicsAndMovement` | 0x487A30 | ✓ `ZeroIfNegligible_482730`, ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | all 9.6f inlines used |
| 0x525D90 | `Object::UpdatePhysicsMovementAndAnimation` | 0x487BC0 | ✓ `ZeroIfNegligible_482730`, ✓ `sub_421080`, ✓ `sub_420FF0` | commented | GetPedRefIdx changes the matching code |
| 0x526790 | `Object::TriggerCarExplosionIfApplicable` | 0x4837F0 | ✓ `sub_475A80`, ✓ `sub_482C10`, ✓ `sub_482400`, ✓ `sub_40FEB0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | IsAnimFinished + IsExplosiveItemDefinition used; code unchanged |
| 0x526B40 | `Object::InheritMotionFromSprite` | 0x483880 | ✓ `sub_416B40`, ✓ `sub_41B080` (10.5 0x41B480), ✓ `sub_40F7B0`, `Car_BC::sub_421D90`, ✓ `sub_40FEB0` | matched | Sprite::GetType, Char::get_velocity, Sprite::SetDrawLayer |
| 0x527070 | `Object::UpdateMovementAndEffects` | 0x486130 | `sub_482F80`, ✓ `RotateVector_41FC90`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `SetPosition_482D30`, ✓ `sub_416B40`, ✓ `sub_420660` | checked | 482F80 = PoolTake, still called in 10.5 |
| 0x527630 | `Object::InitializeObject` | 0x483990 | `ObjectDefinition_74::sub_488EF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_482A30` | done | SetObject2C used; 488EF0 = CreateSpriteFromDefinition (called); code unchanged |
| 0x527F10 | `Object::ReleaseSubObjects` | 0x484760 | ✓ `sub_47F4F0`, `sub_483FC0`, `sub_484000` | matched | 483FC0/484000 are the pool DeAllocate inlines, already used |
| 0x528240 | `Object::ChangeInstanceLimitKind` | 0x483A20 | `sub_4BED60` | checked | 4BED60 = SpriteList::AddSprite (called) |
| 0x5283C0 | `Object::TickObject` | 0x485760 | ✓ `sub_482400`, ✓ `sub_420F10`, `sub_482790`, ✓ `SetFromPolar_41E210`, `Object_3C_Pool::sub_483FE0`, `sub_483FC0`, `sub_484000`, `Object_8_Pool::sub_483FA0`, ✓ `sub_4206C0`, `sub_482F80` | done | 482400 (branches swapped), 420F10, 41E210 used; pool Allocate/DeAllocate already used; 482790/482F80/4826A0 are calls in 10.5; diff 321->301 |
| 0x5288B0 | `Object::OnObjectTouched` | 0x483B50 | ✓ `sub_40FEA0`, ✓ `sub_40FEB0` | done | all 9.6f inlines used |
| 0x528900 | `Object::HandleWaterDeath` | 0x483BA0 | ✓ `rng::get_cur_rng_41CFE0`, `sub_4BDDD0`, ✓ `sub_420660` | done | set_z_lazy used; 4BDDD0 = ShrinkSprite (called); code unchanged |
| 0x528990 | `Object::HandleObjectHit` | 0x486390 | ✓ `sub_40FEA0`, `sub_4932D0` (10.5 0x545430), ✓ `sub_40FEB0`, `sub_4274C0`, ✓ `sub_40FEC0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | GetPedId + GetPedRefIdx used x2; 4932D0/4274C0 = HandlePedObjectHit/HandleCarHitByObject (called); code unchanged |
| 0x528A20 | `Object::ProcessObjectExplosionImpact` | 0x485B00 | `sub_483C80`, ✓ `sub_420F10`, `sub_482790`, ✓ `sub_420FF0` | checked | 483C80 = GetExplosionTypeForDiagonalWall, 482790 = SetDamageOwner (called) |
| 0x528BA0 | `Object::HandleImpactNoSprite` | 0x485C90 | ✓ `sub_420F10`, `sub_482410`, `sub_482790`, ✓ `SetFromPolar_41E210`, ✓ `sub_420FF0` | checked | 482410 = GetExplosionTypeForWallSide, 482790 = SetDamageOwner (called) |
| 0x528E50 | `Object::HandleImpact` | 0x486410 | `sub_482F60`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `check_is_shop_421060`, `sub_410480`, `sub_410460` | checked | 482F60=PoolGive, 410480/410460=CollisionTarget AddSpriteCollision: all calls in 10.5 |
| 0x5291B0 | `Object::Dealloc` | 0x483C40 | `sub_482F60` | checked | 482F60 = PoolGive (called) |
| 0x5291D0 | `Object::PoolGiveAndMarkDone` | 0x483C50 | `sub_482F60` | checked | 482F60 = PoolGive (called) |
| 0x5291E0 | `Object::PoolGiveAndQueueObjectType` | 0x483C60 | `sub_482F60` | checked | 482F60 = PoolGive (called) |
| 0x529240 | `Object::GetMapBlockSoundSpec` | 0x483CC0 | ✓ `sub_421050`, ✓ `gtx_0x106C::sub_462FD0` | done | gtx::sub_462FD0 added+used x2; 421050 = get_field_26 (already); code unchanged |
| 0x5292D0 | `Object::SetDefaultWeaponAmmo` | 0x483D50 | ✓ `sub_45E0A0` | checked | 45E0A0 = SetParam copy (already used, noted in header) |
| 0x529300 | `Object_5C::TrimSpriteList` | 0x485E40 | ✓ `sub_40FEC0`, ✓ `sub_420FF0`, ✓ `sub_420F10` | done | GetPedId used; code unchanged |
| 0x529430 | `Object_5C::ctor_529430` | 0x484AF0 | `SpriteList_4::ctor_424620`, `Object_2C_Pool::ctor_483F10`, `Object_8_Pool::ctor_483F60`, `Object_3C_Pool::ctor_4848C0`, ✓ `SpritePool::sub_421000`, `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690` | checked | pool ctors / SpriteList ctor are implicit member ctors |
| 0x529750 | `Object_5C::dtor_529750` | 0x484CF0 | ✓ `SpritePool::sub_421030`, `Object_2C_Pool::gdtor_484820`, `Object_8_Pool::gdtor_484840`, `Object_3C_Pool::gdtor_484860` | checked | pool dtors are implicit |
| 0x5297F0 | `Object_5C::CreateDiagonalWallObjects` | 0x485ED0 | ✓ `sub_447E90` | done | all 9.6f inlines used |
| 0x529950 | `Object_5C::NewTouchPoint` | 0x485290 | ✓ `sub_482A40` | done | Sprite::SetDimensions + SpriteShape::SetDimensions added+used; code unchanged |
| 0x5299F0 | `Object_5C::NewWithPedRefIdx` | 0x485320 | ✓ `sub_482C00` | checked | 482C00 = SetParam copy (already used, noted in header) |
| 0x529A40 | `Object_5C::NewLight` | 0x485370 | ✓ `SetColourRadiusIntensity_482D60` | done | all 9.6f inlines used |
| 0x529AB0 | `Object_5C::NewLightOfType` | 0x4853C0 | ✓ `SetColourRadiusIntensity_482D60` | done | all 9.6f inlines used |
| 0x529B20 | `Object_5C::sub_529B20` | 0x485410 | ✓ `SetColourRadiusIntensity_482D60`, ✓ `Light_1D4CC::SetFlashing_469070` | todo |  |
| 0x529C00 | `Object_5C::CreateObject` | 0x484E00 | `Object_2C_Pool::sub_4829A0`, `Object_2C_Pool::sub_4829C0`, `sub_482790`, `Object_2C_Pool::sub_484D60`, `Object_2C_Pool::sub_484DB0`, `sub_4BED60`, `Object_8_Pool::sub_483FA0`, `Object_3C_Pool::sub_483FE0`, ✓ `check_is_shop_421060`, ✓ `sub_447E90` | checked | pool Allocate/AllocateUntracked/DeAllocate, SetDamageOwner, AddSprite already used/called |
| 0x52A2C0 | `Object_5C::CreateMovingObj` | 0x485180 | `Object_3C_Pool::sub_483FE0` | checked | 483FE0 = Object_3C pool Allocate (already used) |
| 0x52A3D0 | `Object_5C::CreateExplosion` | 0x485540 | ✓ `sub_482A90`, `Object_3C_Pool::sub_483FE0` | done | Explosion::SetType added+used; 483FE0 = pool Allocate (already); code unchanged |
| 0x52A590 | `Object_5C::RestoreObjects` | 0x485640 | ✓ `sub_45E0A0` | checked | 45E0A0 = SetParam copy (already used, noted in header) |
| 0x52A6D0 | `Object::OnAttachedToSprite` | 0x483E50 | `sub_482F80`, ✓ `sub_40FEB0`, ✓ `sub_40F7B0` | checked | 4826A0 = RemoveFromCollisionBuckets/525AE0 pairing, 482F80 = PoolTake (called) |
| 0x52AD80 | `Object_3C::ctor_52AD80` | 0x484020 | `SpriteList_4::ctor_424620` | checked | SpriteList ctor (424620) is the member ctor, already inline |
| 0x52AE90 | `Object::GetSpeedVector_52AE90` | 0x482C50 | `sub_482BA0` | checked | 482BA0 = Object_3C::GetSpeedVector_52ADF0, called out of line by the matching 10.5 code |
| 0x52B2A0 | `LightPool::CreateLight` | 0x469010 | `Alloc_464C40`, `sub_463F10`, `Light_28::SetCurrentIntensity_45B2D0` | checked | Alloc, SetRadius, SetCurrentIntensity already used via Init_469010 |
| 0x5331A0 | `ObjectDefinition::ApplyDefinitionToSprite` | 0x4883A0 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_40F7B0`, `sub_488200` | done | Sprite::SetType and set_id_lazy; Still matches. |
| 0x534650 | `Garage::ValidateParkCommand` | 0x489B10 | ✓ `sub_489640`, ✓ `sub_489630`, ✓ `sub_489620` | matched | all 9.6f inlines used |
| 0x534700 | `Garage::ParkCarAtDoor` | 0x489BC0 | ✓ `IsLongerThanOneBlock_447ED0`, ✓ `sub_489600`, ✓ `sub_432860` | inlines added | diff 191->187; 447ED0 commented (different constant), 432860 point setter commented |
| 0x5349D0 | `Garage::GaragesService` | 0x489680 | `sub_476230`, `sub_4895E0`, ✓ `sub_476A30`, ✓ `sub_4895F0`, ✓ `sub_475C30`, ✓ `sub_4118D0`, ✓ `sub_421870`, ✓ `sub_4118B0`, ✓ `Car_BC::sub_41E450`, ✓ `sub_433C00`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `DespawnSoon_421490`, ✓ `sub_489650`, `sub_4BB4D0`, ✓ `sub_4895D0`, ✓ `sub_44A3E0`, ✓ `sub_4218A0`, ✓ `Car_BC::sub_475C10`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_432860`, `sub_40F600`, `sub_420390` | inlines added CloseDoors, StopMoving, Reset; matched 2026-10-03 | diff 283->208; f78 clears, Door 476A30, 4895D0 physics stop, 489650 reset, point helpers left |
| 0x5384C0 | `Particle_4C::UpdateDirectedProjectile_state_3_12` | 0x48AE70 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | inlines added | Sprite::SetBlendAndAlpha added+used; RotateByAngle commented (192->198); diff 192->192 |
| 0x538A40 | `Particle_4C::UpdateBeamSegment_state_43` | 0x48B4D0 | ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | SetBlendAndAlpha + SetNoLighting; code unchanged |
| 0x539040 | `Particle_4C::UpdateDirectedBurstSweep_state_4` | 0x48B540 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0`, `sub_4BDEF0` | done | SetBlendAndAlpha used; 4BDEF0 = ApplyScaleToDimensions (called); diff 125->125 |
| 0x539480 | `Particle_4C::UpdateDirectedBurst_state_13_14_36` | 0x48B9F0 | ✓ `sub_4206C0`, ✓ `sub_40F6B0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | SetBlendAndAlpha used; diff 127->127 |
| 0x539890 | `Particle_4C::UpdateCircularBurst_state_5` | 0x48F650 | ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_433800`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_40F6B0`, ✓ `sub_4337F0` | done | SetBlendAndAlpha + ClearNoLighting (added) used; diff 188->184 |
| 0x53A180 | `Particle_4C::UpdateStaticAnim_state_39` | 0x48BE60 | ✓ `sub_4206C0` | done | all 9.6f inlines used |
| 0x53B580 | `Particle_4C::UpdateShortAnim_state_37` | 0x48C790 | ✓ `sub_4206E0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | done | all 9.6f inlines used |
| 0x53B670 | `Particle_4C::UpdateAttachedEmitter_state_9_10` | 0x48FE90 | ✓ `sub_416B40`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `sub_4206C0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_4337D0` | inlines added | SetBlendAndAlpha, IsActive, PolarToCartesian_41FC20 x3 used; Fix16_To_Ang16 left open-coded (7->9); diff 7->7 |
| 0x53B9F0 | `Particle_4C::UpdateBurstAnimation_state_29_30` | 0x48C800 | ✓ `sub_4206E0`, ✓ `sub_4337D0`, ✓ `sub_4337F0`, `sub_4BDEF0` | done | SetBlendAndAlpha + SetNoLighting; 4BDEF0=ApplyScaleToDimensions (already); code unchanged |
| 0x53E2E0 | `Particle_4C::PoolDeallocate` | 0x48C8F0 | ✓ `sub_433800`, ✓ `sub_4337D0`, ✓ `SpritePool::sub_421030` | done | ClearNoLighting + SetBlendAndAlpha(0,0); code unchanged |
| 0x53E3C0 | `ParticleManager::New` | 0x48C930 | ✓ `sub_48A8F0`, ✓ `sub_48A8D0`, `sub_48A900`, ✓ `SpritePool::sub_421000` | done | has_pStart + SpritePool::has_free (added); 48A900 = pool Allocate (already); code unchanged |
| 0x53E450 | `ParticleManager::EmitBloodBurst` | 0x48C9C0 | ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0` | done | all 9.6f inlines used |
| 0x53E880 | `ParticleManager::SpawnBlood` | 0x48CC50 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_40F7B0` | done | all 9.6f inlines used |
| 0x53E970 | `ParticleManager::GunMuzzelFlash` | 0x48CD10 | `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, `sub_48A930`, `sub_48A950`, ✓ `sub_432860`, ✓ `sub_40F6B0`, `sub_4207B0`, `sub_40F680`, ✓ `Car_3C::set_ang_lazy_420690`, `sub_4337F0`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_40FEA0` | matched | GetCarWidth/height (48A930/48A950), SetXY, operator+= (40F680), PolarToCartesian_41FC20 used; 4337F0 commented (both uses together 672->818); diff 819->658 |
| 0x53FE40 | `ParticleManager::EmitImpactParticles` | 0x48DB00 | ✓ `sub_40F790`, ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | checked | 40F790 is atan2 on (sinv,cosv), which 9.6f passes as one Fix16_Point by value; ours has two Fix16 params (signature not changed), atan2_fixed(cosv, sinv) is the same body |
| 0x5405D0 | `ParticleManager::SpawnParticleSprite` | 0x48DFC0 | ✓ `sub_48A8F0`, `sub_48A900`, ✓ `SpritePool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337F0` | done | has_pStart used; 48A900 = pool Allocate (already); code unchanged |
| 0x5406B0 | `ParticleManager::SpawnCigaretteSmokePuff` | 0x48E060 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, `PolarToCartesian_41FC20`, ✓ `sub_4337F0` | done | all 9.6f inlines used |
| 0x540A40 | `Explosion::sub_540A40` | 0x48E220 | ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0` | todo |  |
| 0x541430 | `Explosion::EmitFireTrail_5` | 0x48E8B0 | ✓ `sub_40F6B0`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4206C0`, ✓ `sub_4337D0`, ✓ `sub_4337F0` | done | RotateByAngle on a Fix16_Point, SetBlendAndAlpha; branch/store order fixed; diff 161->19 |
| 0x541850 | `Explosion::ApplyBlastDamage` | 0x490D60 | `SpriteList_4::ctor_424620`, `sub_48A420`, `sub_48A390`, `sub_48EA50`, `sub_4BEE10`, ✓ `sub_40FEA0`, ✓ `sub_48A4C0`, ✓ `sub_420F10`, `sub_40E8D0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_40FEB0`, ✓ `IsMaxDamage_40F890`, ✓ `Car_BC::sub_403BA0`, `sub_425D60`, `sub_4207B0`, ✓ `sub_40FEC0` | inlines added | get_ped_state_1 added; MaxAbsDistance x2 + GetPedId x1 used, 1 MaxAbs + 1 GetPedId commented (worse); 48A420/48A390/48EA50/4BEE10/425D60/426F00 are calls in 10.5; Fix16::Abs/Max now emitted as COMDAT calls; diff 246->240 |
| 0x541D60 | `Explosion::EmitExplosion_18_33` | 0x48EB00 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `SpritePool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0`, `sub_4BDEF0` | done | has_pStart, Fix16_To_Ang16 (was 482740) used; 48A900 = pool Allocate (already), 4BDEF0 = ApplyScale (called); diff 246->156 |
| 0x542060 | `Explosion::EmitExplosion_19_32` | 0x48ED40 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `SpritePool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0`, `sub_4BDEF0` | done | has_pStart, Fix16_To_Ang16, PolarToCartesian_41FC20; diff 270->264 |
| 0x542340 | `Explosion::EmitExplosion_20` | 0x48EF90 | ✓ `sub_48A8F0`, ✓ `sub_40F540`, ✓ `PolarToCartesian_41FC20`, `sub_48A900`, ✓ `SpritePool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_4337F0`, ✓ `sub_4206C0` | done | has_pStart, Fix16_To_Ang16, PolarToCartesian_41FC20; diff 260->248 |
| 0x5434A0 | `Explosion::Update` | 0x491A70 | `sub_48E480`, `sub_48E750`, `sub_48E5F0`, `sub_491550` | matched | 48E480/48E750/48E5F0/491550 are the state_* functions, still called in 10.5 |
| 0x543800 | `ExplosionPool::Allocate` | 0x48A6C0 | `Explosion_30::sub_48A550` | checked | 48A550 = Explosion::Init, still called in 10.5 |
| 0x543900 | `ExplosionPool::sub_543900` | 0x48A800 | `Explosion_30::sub_48A550` | todo |  |
| 0x5439D0 | `ParticleManager::ctor_5439D0` | 0x491B90 | `Particle_4C_Pool::ctor_48F1E0` | checked | Particle_4C_Pool ctor is the implicit pool ctor |
| 0x5453D0 | `Char::PoolDeallocate` | 0x493640 | ✓ `SpritePool::sub_421030` | done | all 9.6f inlines used |
| 0x545430 | `Char::DrawFlamesAndStartScreamTimer` | 0x4932D0 | `sub_420FF0`, `sub_420F10`, `sub_485540` (10.5 0x52A3D0), `sub_420B50`, `sub_441A30` | checked | pairing: 9.6f 0x4932D0 is a larger function (485540 = CreateExplosion, 441A30 = Ped hit handling); nothing open-coded here |
| 0x5456A0 | `Char::InitSprite` | 0x493850 | ✓ `SpritePool::sub_421000`, ✓ `Car_3C::SetType_4206F0`, ✓ `Char_B4::sub_492180` | done | SetType used; 492180 pairs with a ped state setter, not open-coded; code unchanged |
| 0x545720 | `Char::Update` | 0x49C460 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_40F7B0`, ✓ `Car_3C::set_ang_lazy_420690`, ✓ `SetFromPolar_41E210` | done | all 9.6f inlines used |
| 0x5459E0 | `Char::DrownPed` | 0x4938A0 | `sub_48D1F0`, ✓ `cool_nash_0x294::sub_433DD0` | done | SetVoiceEventIfNotOnFire used; 48D1F0 = EmitWaterSplash (called); code unchanged |
| 0x546360 | `Char::UpdateAnimState` | 0x497DF0 | ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_420700`, ✓ `sub_4206C0`, ✓ `cool_nash_0x294::get_target_car_door_403A60`, `sub_493940`, ✓ `sub_421360`, ✓ `sub_40F7B0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_492CB0`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::sub_4039D0`, ✓ `cool_nash_0x294::sub_403B50`, ✓ `sub_433A30`, ✓ `sub_433910`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `sub_433A50`, ✓ `sub_433C10`, ✓ `cool_nash_0x294::sub_433B50`, ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::sub_433DD0`, ✓ `Car_10::set_obj_421380`, ✓ `sub_466CF0`, `sub_466B70` (10.5 0x466B70), `sub_472C60`, `sub_4BDEF0` | done | IsStateActive, SetPaletteSprites, SetStolenCar added; SetInternalTargetPed, GetInternalObjectiveStatus, IsGradientSlopeAt x2 used; 493940/466B70/472C60/4BDEF0 are calls in 10.5; diff 2285->1931 |
| 0x548590 | `Char::ManageZCoordAndSlopes` | 0x494180 | `sub_482510`, `sub_466B70` (10.5 0x466B70), ✓ `sub_42A630`, `sub_4824E0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40) | done | Fix16::operator--(int) added (482510) + operator++(int) (4824E0) used; 466B70 = UpdateZFromSlopeAtCoord (called); code unchanged |
| 0x548670 | `Char::DispatchCollision` | 0x499F00 | ✓ `sub_416B40`, ✓ `sub_420B70`, ✓ `sub_40FEA0`, `sub_494280`, ✓ `sub_40FEC0`, ✓ `sub_40FEB0` | checked | 494280 = HandlePedCollision (called) |
| 0x548840 | `Char::HandleObjectCollision` | 0x4993B0 | ✓ `sub_421080`, ✓ `sub_420FF0`, ✓ `sub_482C90`, ✓ `sub_482C80`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_4867E0` | done | UsesParamAsPedRef, CanBePushed, GetMass used; 4867E0 = ResolveCollisionWithPed (called); diff 55->13 |
| 0x54A530 | `Char::HandleGenericCollision` | 0x4948C0 | ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_41FAC0`, ✓ `sub_493540`, `sub_420590`, `sub_447E10`, `sub_40E8D0`, ✓ `sub_492170`, ✓ `sub_492C20`, ✓ `PolarToCartesian_41FC20`, ✓ `sub_42A720`, ✓ `sub_492CE0`, ✓ `sub_492CF0`, ✓ `cool_nash_0x294::get_car_state_403A90`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `set_xy_lazy_447E20` | done | GetJumpOverMode added; 41FAC0=GetCornerAngle, 40E8D0=atan2 (called); code unchanged (716) |
| 0x54C1A0 | `Char::CanMoveOntoSlope` | 0x495220 | ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `sub_420420`, ✓ `sub_42A630`, `sub_4824E0`, `sub_482510` | done | Fix16 operator++(int)/operator--(int) used; code unchanged |
| 0x54C500 | `Char::CanMoveToTile` | 0x495540 | `sub_491EE0` | done | 491EE0 = Fix16::ToInt (already); load x and y first; diff 7->0 CANDIDATE MATCH |
| 0x54C900 | `Char::TickMovementStateMachine` | 0x495700 | `sub_4955F0`, `sub_4920A0`, `sub_40EAB0` | checked | 4955F0 = ApplyRandomRotationJitter, 4920A0 a called helper, 40EAB0 = angle delta (called) |
| 0x54DD70 | `Char::CheckCollisionsAndUpdateAnim` | 0x4958E0 | ✓ `sub_433CA0` | done | GetAttacking used; code unchanged |
| 0x54DDF0 | `Char::UpdateWalking` | 0x49A560 | ✓ `sub_42A630`, ✓ `sub_420420`, ✓ `sub_491F80`, `ApplyCarVelocityCameraOffset_436200`, ✓ `PolarToCartesian_41FC20`, `sub_436140`, ✓ `cool_nash_0x294::sub_403A40`, `sub_433970`, `sub_491FA0`, ✓ `cool_nash_0x294::get_objective_403A80`, `sub_4995A0`, `sub_495470` (10.5 0x495470), ✓ `sub_4923D0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_466CF0`, `sub_49A080`, ✓ `sub_4923A0`, `sub_4994D0` | inlines added | ClearAttacking_403A40 x2 used; RegulateVelocity skipped (uses kFpOne256th_678620, code uses kFpOne256th_6FD99C); rest are 10.5 calls; diff 556->556 |
| 0x54ECB0 | `Char::CanStepForwardWithRegionCheck` | 0x495980 | ✓ `sub_466CF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_4BD670`, ✓ `sub_492130`, ✓ `sub_420420`, ✓ `sub_42A630`, `sub_492190` | checked | 492190 = CanStepForward, 4BD670 = CheckSpriteMovementRegion: both called in 10.5 |
| 0x54EF60 | `Char::CanStepToTile` | 0x495BF0 | ✓ `PolarToCartesian_41FC20`, `sub_491EE0`, ✓ `sub_491F00`, ✓ `sub_491EF0` | checked | 491EE0 = Fix16::ToInt, already used |
| 0x550090 | `Char::CanReachTile` | 0x492420 | `sub_491EE0`, ✓ `sub_491F00`, ✓ `sub_491EF0`, `sub_492190` | checked | 491EE0 = Fix16::ToInt (already), 492190 = CanStepForward (called) |
| 0x5504F0 | `Char::UpdateFleeOrRunning` | 0x49B0D0 | ✓ `sub_42A630`, ✓ `sub_420420`, ✓ `sub_491F80`, ✓ `sub_492140`, ✓ `sub_492CC0`, ✓ `sub_492FD0`, `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `sub_40E8D0`, ✓ `PolarToCartesian_41FC20`, `sub_496500`, `sub_492C30`, `angle::angle_not_equal_41CFF0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_466CF0`, `sub_49A080` | done | GetTargetFacingAngle added x2; 41CFF0 = Ang16 operator!= (already); 492C30=CanStepInDirection, 496500/49A080 called; diff 387->387 |
| 0x550F60 | `Char::GetNextRotationToward` | 0x4928B0 | `sub_40EAB0` | checked | 40EAB0 = ComputeShortestAngleDelta wrapper, called in 10.5 (note: orig also calls Fix16::FromFix16_4516B0, the commented-out 'unused' line) |
| 0x551400 | `Char::ChooseNextMovementTile` | 0x492D00 | ✓ `sub_492CE0`, ✓ `sub_492CF0` | done | all 9.6f inlines used |
| 0x551A00 | `Char::UpdateEnteringCar` | 0x49BAD0 | ✓ `sub_433CA0`, ✓ `sub_416B40`, ✓ `cool_nash_0x294::get_target_to_enter_403B10`, ✓ `is_on_trailer_421720`, ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x551B30 | `Char::UpdateExitingCar` | 0x496800 | ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x551BB0 | `Char::UpdateEnteringTrain` | 0x49BC20 | ✓ `sub_433CA0`, ✓ `cool_nash_0x294::get_target_car_door_403A60` | done | all 9.6f inlines used |
| 0x551CB0 | `Char::UpdateStandingStill` | 0x49BD10 | `sub_436140`, ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `sub_42A630`, ✓ `sub_420420`, ✓ `sub_491F80`, ✓ `sub_492CC0`, ✓ `sub_433CA0` | done | GetTargetFacingAngle used; 436140 = GetTurnSpeed (called); diff 99->91 |
| 0x5520A0 | `Char::UpdateImmobilized` | 0x496880 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `sub_40F7B0`, `sub_4937D0`, `sub_493810`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), `sub_48D1F0`, `sub_4BDDD0`, ✓ `sub_420660`, ✓ `sub_433B70`, `sub_4211A0`, ✓ `cool_nash_0x294::sub_403990`, ✓ `sub_433B60`, ✓ `cool_nash_0x294::sub_433DD0`, `cool_nash_0x294::sub_433B50`, ✓ `sub_433910`, ✓ `sub_433A50`, ✓ `PolarToCartesian_41FC20`, ✓ `Car_3C::set_ang_lazy_420690`, `sub_4105B0`, ✓ `sub_434950` | matched | ClearAttacking_403A40, get_health used; SetStateInitPending commented (115->192); 4937D0/493810/48D1F0/4BDDD0/4211A0/4105B0 map to 10.5 calls; diff 115->115 |
| 0x552E90 | `Char::UpdateDead` | 0x49C120 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `sub_40F7B0` | done | all 9.6f inlines used |
| 0x5532C0 | `Char::IsNearTileCentre` | 0x497410 | `sub_4937D0`, `sub_493810` | checked | 4937D0/493810 = GetTileFracX64/GetTileFracY64, called |
| 0x5535B0 | `Char::PhoneTouched` | 0x4930C0 | ✓ `sub_420B60` | done | all 9.6f inlines used |
| 0x553640 | `Char::OnObjectTouched` | 0x4930F0 | ✓ `check_is_shop_421060`, ✓ `sub_421050`, ✓ `sub_433A20`, ✓ `sub_493090` | done | Object::GetConveyorDirection added+used; 421050 = get_field_26 (already); code unchanged |
| 0x5538A0 | `Char::HandleCarImpact` | 0x497570 | ✓ `sub_40F790`, `sub_482790` | checked | 482790 = SetDamageOwner, called in 10.5 |
| 0x553E00 | `Char::HandleGenericImpact` | 0x493390 | ✓ `cool_nash_0x294::sub_403990`, `sub_482790` | matched | 482790 = SetDamageOwner, called in 10.5 |
| 0x554110 | `PathFinder::CanMoveToAdjacentTile` | 0x49CB60 | ✓ `sub_466CF0` | matched | all 9.6f inlines used |
| 0x5545E0 | `PathFinder::Init` | 0x45C130 | `Ped_Unknown_4::ctor_425450`, `PathFinder_2FD4::init_49C700` | checked | 9.6f caller inlined init_49C700 (= this function); Ped_Unknown_4 ctor is the member ctor |
| 0x554AB0 | `PathFinder::ComputePath` | 0x49CF70 | ✓ `sub_466CF0` | done | all 9.6f inlines used |
| 0x5552B0 | `PathFinder::FindNearbyTileMatchingSlopeType` | 0x49D7A0 | ✓ `sub_466CF0`, ✓ `sub_420420` | done | all 9.6f inlines used |
| 0x559430 | `CarPhysics::ShowPhysicsDebug` | 0x4A1DA0 | ✓ `sub_49E240`, ✓ `Hud_TextEntry_C4::SetFontRemap_45AFD0` | inlines added | ThetaText + Hud_TextEntry::SetFontRemap (no change); left: the lea of the DisplayText this is scheduled early in the theta call and late in the rear-skid call |
| 0x5597B0 | `CarPhysics::ShowSpeedRevsDamage` | 0x49F010 | ✓ `sub_4211A0`, `sub_49E820` | checked | 49E820 = IsTurboActive (called), 4211A0 speed uses the special out-of-line length variants |
| 0x559A40 | `CarPhysics::UpdateTrailerPhysicsFromTowingCar` | 0x49F1B0 | `sub_40F600`, `sub_40EAB0`, ✓ `sub_40F580` | matched | 40F600/40EAB0/40F580 are Point -, ComputeShortestAngleDelta and Ang16_to_Fix16, already used |
| 0x559B50 | `CarPhysics::EnforceTrailerControlLimits` | 0x49F280 | ✓ `sub_49EFB0` | done | Trailer::IsHitchAngleClamped added+used; code unchanged |
| 0x559BA0 | `CarPhysics::SpinOutOnOil` | 0x49F2D0 | ✓ `Car_BC::sub_403BA0`, ✓ `sub_49EF50` | done | AddDamage added (CarPhysics.hpp decl, def in .cpp) + used; code unchanged |
| 0x559C30 | `CarPhysics::ScarePedsOnDrivingFast` | 0x49F350 | ✓ `sub_466CF0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_4211A0`, ✓ `sub_411970` | matched | GetLinearSpeed makes it worse (41->56: GetLength_41E260 vs GetLength_2) |
| 0x559E20 | `CarPhysics::ApplyConveyorPush` | 0x49F460 | ✓ `sub_493090`, ✓ `sub_49EF50` | done | (2nd pass) GetConveyorDirection used too; code unchanged |
| 0x55A100 | `CarPhysics::GetTrailerAwareTurnRatio` | 0x49F720 | ✓ `Car_BC::has_trailer_41E460` | done | IsTrailerCab used; code unchanged |
| 0x55A1D0 | `CarPhysics::SetVelocityTowardTarget` | 0x49F760 | ✓ `sub_40F6B0`, `sub_40F600`, ✓ `sub_40F580` | checked | 40F600 = Point - (40AC80 called), 40F580 = Ang16_to_Fix16 (already) |
| 0x55A860 | `CarPhysics::HandleUserInputs` | 0x49FB00 | `sub_420360`, `sub_40F840` | matched | IsNull (420360, 6FE20C variant) + IsVelocityAlignedWithHeading used; diff 155->43 |
| 0x55AA00 | `CarPhysics::HandleGravityOnSlope` | 0x4A20E0 | ✓ `Car_BC::sub_403BA0`, `sub_4634E0` | matched | 4634E0 = Map get_grad_scale_from_size: 10.5 uses CarPhysics globals (6FDF7C...), not the map ones |
| 0x55AB50 | `CarPhysics::ComputeSlopeCorrection` | 0x49FBE0 | ✓ `is_on_trailer_421720`, `sub_40F600`, `sub_4634E0`, ✓ `sub_42A630` | matched | IsTrailerCar used; 4634E0 skipped (map globals, 10.5 uses CarPhysics copies); 40F600 = Point - ; diff 96->96 |
| 0x55B3F0 | `CarPhysics::SyncZWithTrailer` | 0x4A2640 | `sub_4A2240` | checked | 4A2240 = UpdateZPhysics, called in 10.5 |
| 0x55B7E0 | `CarPhysics::EmitImpactParticles` | 0x49FF80 | ✓ `IsMaxDamage_40F890`, `sub_4102A0` (10.5 0x40BD10) | checked | 4102A0 pairs with CollisionTarget AddBlockCollision; 10.5 calls AddFloorCollision (pairing noise) |
| 0x55B970 | `CarPhysics::ProcessGroundCollisionAndSurfaceType` | 0x4A26C0 | `sub_4BD490`, `sub_40F600`, ✓ `sub_42A630`, ✓ `sub_49EBE0` | done | GetBlockSurfaceType (static inline in CarPhysics.cpp) + gtx::sub_49E570 added; 4BD490 = CheckCornerZCollisions? (called), 40F600 = Point -; diff 308->308 |
| 0x55C150 | `CarPhysics::TestCollision` | 0x4A0020 | `sub_4BD670`, ✓ `sub_49EF10` | done | CollisionTarget::SetSourceSprite added+used; 4BD670 = CheckSpriteMovementRegion (called); code unchanged |
| 0x55C3B0 | `CarPhysics::SweepTestMovementForCollision` | 0x4A2F90 | `sub_49F930` | matched | 49F930 = ComputeRequiredSweepSteps, called in 10.5 |
| 0x55C5C0 | `CarPhysics::HandleMapBoundaryCollisionY` | 0x4A3DF0 | `sub_4BCD00`, ✓ `sub_432860`, `sub_49E5A0`, `sub_4828C0`, ✓ `sub_40F6B0` | done | SetXY + operator-= (4828C0); 4BCD00/49E5A0 = edge / ComputeRelativePointVelocity (called); diff 88->87 |
| 0x55C820 | `CarPhysics::HandleMapBoundaryCollisionX` | 0x4A3FB0 | `sub_4BCFA0`, ✓ `sub_432860`, `sub_49E5A0`, `sub_4828C0`, ✓ `sub_40F6B0` | checked | SetXY/-= already used; 4BCFA0/49E5A0 called in 10.5 |
| 0x55CA70 | `CarPhysics::DispatchCollision` | 0x4A4170 | ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_40FEC0` | done | all 9.6f inlines used |
| 0x55CBB0 | `CarPhysics::ReplayAndDispatchCollision` | 0x4A4270 | ✓ `sub_40FEB0` | done | AsCar used; diff 51->51 |
| 0x55E260 | `CarPhysics::DoSkidmarks` | 0x4A0560 | `sub_4A0290` | checked | 4A0290 = UpdateWheelSkidEffects (called) |
| 0x55E470 | `CarPhysics::StepMovementAndCollisions` | 0x4A4310 | ✓ `Reset_4637B0`, ✓ `Car_BC::sub_403BA0`, ✓ `sub_49E450`, ✓ `IsCharB4_49EF20`, `sub_446AA0` | matched | HasZeroComponent added (fixes logic: x==0 |
| 0x55EC30 | `CarPhysics::ApplyVerticalEdgeNudge` | 0x4A2A30 | `sub_49E330`, `sub_420390` | checked | 49E330 = ApplyAngularImpulse, 420390 = NormalizeSafe (called) |
| 0x55EF20 | `CarPhysics::ApplyHorizontalEdgeNudge` | 0x4A2B40 | `sub_49E330`, `sub_420390` | matched | 49E330 = ApplyAngularImpulse, 420390 = NormalizeSafe: both called in 10.5 |
| 0x55F020 | `CarPhysics::ApplyTurningForce` | 0x4A2C70 | ✓ `sub_40FEC0`, ✓ `sub_4118D0`, `sub_40F600`, ✓ `sub_40F6B0`, `sub_49E330`, `sub_420390` | matched | Force goes through the exported Multiply, not the inline operator* (whose visible body dropped an EH state store) |
| 0x55F360 | `CarPhysics::CheckPendingCollision` | 0x4A2DB0 | ✓ `sub_414F70`, ✓ `sub_49EFD0` | commented | 414F70 (IsFlagSet(0x2000)) changes the matching code; 49EFD0 body not dumped |
| 0x55F3B0 | `ComputeCollisionImpulse` | 0x4A05C0 | `sub_420360`, `sub_40F600`, `sub_49E420`, `sub_420390`, ✓ `sub_49E500`, ✓ `sub_49E0E0` | checked | 49E420 = Rotate90CCW, 420390 = NormalizeSafe (called), 49E0E0/40F600 = Fix16/Point operators |
| 0x55F740 | `CarPhysics::ApplyForceWithTrailerRedirect` | 0x4A3140 | ✓ `is_on_trailer_421720`, `sub_4A2E00` | matched | IsTrailerCar used; 4A2E00 = ApplyForceAndIntegrate (called); diff 47->34 (by-value Fix16_Point copy differs) |
| 0x55F800 | `CarPhysics::ApplyForceAtPoint` | 0x4A0850 | ✓ `sub_40F6B0`, `sub_40F680` | done | operator+= (40F680) x2; bRotate test as switch (dec/jne in 9.6f and 10.5); diff 135->0 CANDIDATE MATCH |
| 0x55F9A0 | `CarPhysics::ApplyForceScaledByMass` | 0x4A0930 | `sub_40F680` | checked | 40F680 = operator+=, already used |
| 0x55FA10 | `CarPhysics::ApplyImpulseWithTrailerRedirect` | 0x4A0960 | ✓ `is_on_trailer_421720`, `sub_4A0900` | done | IsTrailerCar used; 4A0900 = AccumulateImpulse (called); diff 21->0 CANDIDATE MATCH |
| 0x55FA60 | `CarPhysics::ApplyImpactForcesAndDamage` | 0x4A31B0 | ✓ `Car_BC::IsFlagSet_411930`, ✓ `sub_4118D0`, ✓ `sub_4211C0`, ✓ `sub_49EF50`, ✓ `sub_421260` | done | IsFlagSet added, IsDrivenByPlayer (4118D0), AddDamage, ClearHandBrake_421260 used; 4211C0 = GetLength_453590 & 426F00 = ApplyVisualDamage (called); diff 180->140 |
| 0x55FD00 | `CarPhysics::HandleWorldCollision` | 0x4A32D0 | `sub_420360`, `sub_4292F0`, ✓ `IsMaxDamage_40F890` | matched | IsNull (420360) used; 4292F0 = ApplyImpactDamage (called); diff 63->63 |
| 0x55FF20 | `CarPhysics::HandleCarCollision` | 0x4A34A0 | `sub_49E5A0`, `sub_40F600`, `sub_420390`, `sub_49E360`, `CanCrushCars_49EFE0`, `CanCollideOver_4216E0`, ✓ `SetCrushFlag_49EFC0`, `sub_4A09B0`, ✓ `sub_4211A0`, `sub_423480`, ✓ `sub_41B0A0`, `sub_4292F0`, ✓ `Car_BC::sub_403BA0`, ✓ `IsMaxDamage_40F890` | done | GetLinearSpeed, IsMaxDamage used; 423480 = GetEffectiveDriver, 49E360/4A09B0/49E5A0 = called CarPhysics funcs/ops; diff 447->445 |
| 0x5606C0 | `CarPhysics::HandleObjectCollision` | 0x4A3A40 | `sub_482C30`, `sub_40F600`, ✓ `sub_482C90`, ✓ `sub_482C80`, `sub_49E5A0`, `sub_420390`, `sub_49E360`, `sub_4A09B0`, `sub_4292F0`, ✓ `IsMaxDamage_40F890` | done | GetMass used; 482C30 = GetXY (called), rest operators/called funcs; diff 260->260 |
| 0x560B40 | `CarPhysics::ProcessPedImpact` | 0x4A0A30 | ✓ `sub_4339E0`, ✓ `sub_4339C0`, `sub_49E5A0`, `sub_40F600`, ✓ `sub_433A00`, `sub_40F640` (10.5 0x43D5D0), ✓ `sub_49EF40` | done | get_sprite_xpos/ypos/zpos (4339C0/4339E0/433A00, noted) + GetStolenCar (added) used; 49E5A0 called, 40F600/40F640 operators; diff 207->189 |
| 0x560F20 | `CarPhysics::ApplyMovementStep` | 0x4A2E30 | ✓ `sub_40F540`, `sub_40F680`, `sub_49ED00`, ✓ `Car_BC::sub_403BA0`, `sub_49FF70` | matched | operator+= (40F680) worse 103->109; 49ED00/49FF70 = UpdateCp1FromCm1/UpdateTrailerAlignment (called) |
| 0x5610B0 | `CarPhysics::IntegrateAndClampVelocities` | 0x4A0CC0 | `sub_40F680`, ✓ `ApplyDeadZone_49E3C0`, ✓ `ZeroIfNegligible_482730` | checked | 40F680 = operator+=, already used |
| 0x561E50 | `CarPhysics::CalculateFrontWheelForce` | 0x4A1130 | `sub_4A0F30`, `sub_432860`, ✓ `sub_40F540`, `sub_4A0D40` | commented | SetXY worse (214->216); 4A0F30/4A0D40 = ComputeEngineTorque/ApplyDriveForce (called) |
| 0x5620D0 | `CarPhysics::CalculateRearWheelForce` | 0x4A1360 | `sub_4A0F30`, ✓ `sub_432860`, `sub_49E3A0`, ✓ `sub_40F540`, `sub_4A0D40` | inlines added | SetXY used; MultiplyByFix16 commented (196->222); 4A0F30/4A0D40 called; diff 196->196 |
| 0x5626F0 | `CarPhysics::ApplyArrowSteerAssist` | 0x4A17C0 | ✓ `sub_40F580`, ✓ `sub_40F840`, `sub_420360`, ✓ `angry_lewin_0x85C::sub_41DC70` | checked | 40F580 = Ang16_to_Fix16, 420360 = IsNull: already used |
| 0x562910 | `CarPhysics::StabilizeVelocityAtSpeed` | 0x49EA70 | ✓ `sub_49E3A0`, ✓ `sub_40F6B0` | matched | all 9.6f inlines used; matched with an assigning `Multiply` wrapper and a product-temp `x *=` |
| 0x562D00 | `CarPhysics::EnforceGearSensitiveMaxSpeed` | 0x4A1B20 | ✓ `sub_40F840`, ✓ `sub_40F790`, ✓ `SetFromPolar_41E210`, ✓ `sub_49E480` | inlines added | IsVelocityAlignedWithHeading now uses GetAngle (0.699->0.944); all 9.6f inlines used |
| 0x562FA0 | `CarPhysics::UpdateLastMovementTimer` | 0x4A1BA0 | `sub_49F170` | checked | 49F170 = IsNotMoving (called) |
| 0x562FE0 | `CarPhysics::ProcessCarPhysicsStateMachine` | 0x4A4570 | ✓ `unknown_libname_25`, ✓ `sub_49F4E0`, ✓ `sub_49F4F0`, ✓ `sub_49EA60`, ✓ `sub_4118D0`, ✓ `sub_4212B0`, ✓ `sub_4212A0` | done | all 9.6f inlines used |
| 0x563350 | `CarPhysics::UpdateCenterOfMassPoint` | 0x49ED60 | `sub_40F6B0` | matched | all 9.6f inlines used |
| 0x563460 | `CarPhysics::UpdateReferencePoint` | 0x49EDC0 | `sub_40F6B0` | matched | all 9.6f inlines used |
| 0x563670 | `CarPhysics::UpdateSpriteFromPhysics` | 0x49EE10 | `CarPhysics_B0::sub_49ED60` (10.5 0x563350) | checked | 49ED60 = UpdateCenterOfMassPoint (called) |
| 0x5636C0 | `CarPhysics::UpdateCarAndTrailerSpriteFromPhysics` | 0x49EEB0 | `sub_49EE80` | checked | 49EE80 = sprite update helper (called) |
| 0x564710 | `Player::SetKFCarWeapon` | 0x4A5220 | ✓ `sub_4A4FE0`, ✓ `sub_4A4F90` | done | Weapon::set_infinite_ammo (new); get_ammo already used; code unchanged |
| 0x564790 | `Player::SetKFWeapon` | 0x4A52B0 | ✓ `sub_4A4FE0`, ✓ `sub_4A4F90` | done | Weapon::set_infinite_ammo; code unchanged |
| 0x5647D0 | `Player::ClearKFWeapon` | 0x4A5300 | ✓ `sub_433820`, ✓ `sub_4A4FF0`, ✓ `Weapon_30_Pool::DeAllocate_4A4F20` | done | all 9.6f inlines used |
| 0x564960 | `Player::AddWeaponWithAmmo` | 0x4A5400 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60`, `sub_4A53D0` | done | GetPlayerPed_4A5130; sub_4A53D0 = HasAnyAmmo (already called); code unchanged |
| 0x5649D0 | `Player::SelectNextOrPrevWeapon` | 0x4A5460 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `keen_bhaskara_0x30::sub_4A4F80` | done | all 9.6f inlines used |
| 0x564C00 | `Player::UnloadCarWeapons` | 0x4A5640 | `angry_lewin_0x85C::sub_4A5600` | checked | 4A5600 = CleanupEmptyAmmoWeapons, called out of line in 10.5 too; no inline missing |
| 0x564C50 | `Player::RemovePlayerWeapons` | 0x4A5690 | ✓ `keen_bhaskara_0x30::sub_4A4F80` | done | Weapon::HasAmmo; code unchanged |
| 0x564CF0 | `Player::ClearPowerUpsExceptJailCard` | 0x4A5710 | `cool_nash_0x294::sub_435F00`, ✓ `sub_482080` | done | Ped::ClearElectrofingers; sub_435F00 = Ped::sub_45C050 (still called); code unchanged |
| 0x564D60 | `Player::CollectPowerUp` | 0x4A5780 | ✓ `RollingDigitCounter_38::sub_41DC30`, ✓ `sub_4766B0`, `sub_4A4D50`, ✓ `sub_433B70`, ✓ `sub_4A5050`, ✓ `cool_nash_0x294::get_wanted_points_433DC0` (10.5 0x592370), ✓ `cool_nash_0x294::sub_420B80`, ✓ `sub_4A5060`, ✓ `sub_4A5020` | done | Player::ChangeMultipliers, Ped::SetFullHealth, SetElectrofingers (new), get_value x2, get_health, ClearWantedPoints; sub_4A4D50 = ChangeLifeCountByAmount (still called); code unchanged |
| 0x565070 | `Player::tick_down_powerups` | 0x4A59A0 | `cool_nash_0x294::sub_435F00`, ✓ `sub_482080` | done | Ped::ClearElectrofingers; sub_435F00 = Ped::sub_45C050 (still called); code unchanged |
| 0x5651F0 | `Player::RestorePowerUpsFromSave` | 0x4A5A50 | ✓ `sub_4A5060` | matched | VC6 turns the two pointers into base+difference addressing; the target keeps both (same in 9.6f). Loop forms tried: while/for/indexed |
| 0x565310 | `Player::TeleportToDebugCam` | 0x4A5AD0 | ✓ `DrawUnk_0xBC::sub_475B60`, `text_0x14::Find_4C23A0` | done | all 9.6f inlines used |
| 0x565490 | `Player::InitPlayerPed` | 0x4A5B40 | `cool_nash_0x294::sub_435C40` | checked | 9.6f sub_435C40 = Ped::sub_45B560, still called |
| 0x565890 | `Player::Hud_Controls` | 0x4A7010 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_4A4770`, `sub_4C8A20`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `sub_4A5070`, ✓ `sub_4A4750`, `cool_nash_0x294::sub_435F40`, `angry_lewin_0x85C::sub_4A6FD0`, ✓ `sub_4A4760`, `sub_4A4940`, `angry_lewin_0x85C::sub_4A49B0`, ✓ `cool_nash_0x294::sub_420B80` | done | ToggleFastForward + ToggleShowCoords (new), camera set_ped_id 1/2 (4A5070 noted), ClearWantedPoints, clear_zones = 4A4770 (noted); 4C8A20/4A6FD0/4A4940/4A49B0/435F40 correspond to Player/Ped methods still called |
| 0x5668D0 | `Player::HandleControls` | 0x4A76D0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `Car_BC::sub_403BA0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_enter_car_as_passenger_4039B0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, `cool_nash_0x294::sub_4039E0`, ✓ `cool_nash_0x294::get_target_objective_car_403AB0`, `sub_4A5030`, ✓ `cool_nash_0x294::sub_4A5010`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040` | done | Ped::SetAttacking (new) x3, ClearAttacking_403A40 x2; other listed getters already used (403A40 commented helper), sub_4039E0 = objective status getter (sub_450CB0 still called), sub_4A5030 unknown; code unchanged |
| 0x566C80 | `Player::DoPedControlInputs` | 0x4A5C50 | `sub_43E1E0`, ✓ `Char_B4::sub_433A80`, ✓ `sub_433C40`, ✓ `cool_nash_0x294::sub_433DD0` | done | Ped::DoJump (new) + SetVoiceEventIfNotOnFire; 162->0 CANDIDATE MATCH; sub_43E1E0 = Ped::sub_45C5C0 (still called) |
| 0x566EE0 | `Player::ShowDebugInfo` | 0x4A5E90 | ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130`, ✓ `cool_nash_0x294::get_car_416B60` | done | GetPlayerPed_4A5130 instead of the equivalent GetCameraModePed; code unchanged |
| 0x5670B0 | `Player::RespawnPlayer` | 0x4A6050 | ✓ `sub_434B10`, `sub_4A4D50`, ✓ `RollingDigitCounter_38::sub_41DC30`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00` | done | IsNetworkGame x2, get_value, get_cam_x/y; sub_4A4D50 = ChangeLifeCountByAmount (still called); code unchanged |
| 0x567130 | `Player::Wasted` | 0x4A7910 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `Game_0x40::get_player_4219E0`, `sub_4822A0`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `RollingDigitCounter_38::sub_41DC30`, `text_0x14::Find_4C23A0`, `sub_4105B0`, ✓ `sub_434950`, ✓ `DrawUnk_0xBC::sub_475B60` | done | ClearAttacking_403A40 x3, get_value x3, GetPlayer; code unchanged; sub_4822A0 = UpdateFugitive (still called) |
| 0x5679E0 | `Player::Busted` | 0x4A7BA0 | ✓ `cool_nash_0x294::sub_403A40`, `sub_4105B0`, ✓ `sub_434950`, ✓ `angry_lewin_0x85C::sub_41DC70`, `text_0x14::Find_4C23A0`, ✓ `RollingDigitCounter_38::sub_41DC30`, ✓ `sub_434920`, ✓ `sub_434940`, ✓ `sub_4A50B0`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0` | done | Player::HasPowerUp + DecPowerUp (new), SetValueClamped, get_value, ClearAttacking_403A40 x3, ClearWantedPoints x2; code unchanged; sub_4105B0 is a thunk on an unknown 9.6f global |
| 0x568670 | `Player::HandleDebugZoom` | 0x4A4CB0 | `sub_41E510`, `sub_41E4E0` | checked | 9.6f sub_41E510/sub_41E4E0 are Camera::sub_435810/sub_4357F0, still called |
| 0x568730 | `Player::Disconnect` | 0x4A61C0 | `sub_4354C0`, ✓ `sub_4A5170` | checked | 9.6f sub_4354C0 = Ped::sub_470300, still called; set_bInUse_4A5170 already used |
| 0x5687F0 | `Player::Service` | 0x4A7E80 | `sub_4A6100`, ✓ `sub_4A5070`, `sub_41E580`, ✓ `cool_nash_0x294::sub_403990` | checked | SetSnapFollow (9.6f 0x4A5070) already used; 4A6100 (zone update) and 41E580 correspond to calls in the source |
| 0x569410 | `Player::UpdatePaused` | 0x4A61F0 | ✓ `sub_4A5070` | checked | SetSnapFollow (9.6f 0x4A5070) already used |
| 0x569530 | `Player::EndRemoteControl` | 0x4A62B0 | ✓ `cool_nash_0x294::get_car_416B60`, ✓ `sub_475C80` | done | Car::UnlockUnlessPermanent (new); code unchanged |
| 0x5695A0 | `Player::ResetAuxCamera` | 0x4A6310 | ✓ `angry_lewin_0x85C::IsRemoteControlActive_4A5100`, ✓ `DrawUnk_0xBC::sub_475B60` | done | Player::sub_4A5100 (noted) + SetSmoothFollow; code unchanged |
| 0x569600 | `Player::StartRemoteControl` | 0x4A6350 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, `cool_nash_0x294::sub_435C40`, `cool_nash_0x294::sub_436040`, ✓ `Car_BC::sub_475C10`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410` | done | Car::LockUnlessPermanent; sub_435C40 = Ped::sub_45B560 and sub_436040 = Ped::sub_45C4B0 (both still called); code unchanged |
| 0x5696D0 | `Player::WatchCar` | 0x4A6400 | ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410` | done | all 9.6f inlines used |
| 0x569920 | `Player::get_pos` | 0x4A6610 | ✓ `sub_4A5150`, ✓ `Car_BC::sub_41E430`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E450`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `angry_lewin_0x85C::get_camera_434900`, `sub_4A5090` | done | Player::GetActivePed (new), get_camera, Car get_x/y/z_41E4x0; 4A5090 (cam z + dword_6FE618) left open-coded (global is local to Player.cpp); code unchanged |
| 0x569CB0 | `Player::InitializePlayerState` | 0x4A81E0 | ✓ `sub_475A20`, ✓ `DrawUnk_0xBC::sub_475B60`, ✓ `DrawUnk_0xBC::CommitCameraTarget_41E410`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | all 9.6f inlines used |
| 0x569E70 | `Player::DebugToggleRemoteControl` | 0x4A8340 | ✓ `angry_lewin_0x85C::IsRemoteControlActive_4A5100` | done | all 9.6f inlines used |
| 0x569F40 | `Player::DisableInputs` | 0x4A6900 | ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040`, ✓ `Car_BC::sub_403BA0` | done | Ped::ClearAttacking_403A40; code unchanged |
| 0x56A0F0 | `Player::RestoreCarsFromSave` | 0x4A6A80 | `EnqueueRadioLocationPhrase_426E10`, `GetRaw_4A5190`, ✓ `sub_4A51B0` | matched | 426E10 = a SpawnCar wrapper using Player.cpp's dword_6FE614 copy (CarManager::SpawnFullSizeCar uses dword_6F77C4, so not reusable); 4A51A0 is BitSet32::operator= but field_8 is DamageAreaFlags; SetDamage already used; 60 unchanged |
| 0x56A1A0 | `Player::CopyPlayerDataToSave` | 0x4A6B20 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `sub_421980`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_433B70`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_4766C0`, ✓ `sub_4A4FB0` | done | Player::get_lives + GetScore (new), get_multiplier, get_health, get_remap; get_ammo_clips already used; code unchanged |
| 0x56A310 | `Player::UpdateGameFromSave` | 0x4A6C80 | ✓ `sub_4A50B0`, ✓ `sub_4A50E0`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `cool_nash_0x294::sub_420B80` | done | SetValueClamped x2, SetMoney, ClearWantedPoints; code unchanged |
| 0x56A490 | `Player::ApplyCheats` | 0x4A6DA0 | ✓ `sub_4A50E0`, ✓ `sub_4A4F90`, ✓ `sub_4A50B0` | done | also SetValueClamped(99) for lives; code unchanged |
| 0x56A740 | `Player::ctor_56A740` | 0x4A83C0 | `PlayerCrimeStats_3C::ctor_44A1D0`, `sub_4A6FC0`, ✓ `ResetThrowCharge_4A5180`, ✓ `sub_434950`, `sub_409DA0` (10.5 0x521100) | checked | sub_4A5180 already a helper; sub_4A6FC0 is the chat sub-object ctor (fields set directly), sub_409DA0 a member ctor |
| 0x56A940 | `Player::dtor_56A940` | 0x4A6EF0 | ✓ `root_sound::DestroySoundObj_40FE60`, `sub_4A4F10` | checked | DestroySoundObj already used; sub_4A4F10 a member dtor |
| 0x56B6E0 | `PlyDat::ctor_56B6E0` | 0x4A9290 | `sub_4A8910`, `sub_4A9050` | checked | sub_4A9050 = HiScoreHscExists, sub_4A8910 = 10.5 out-of-line call (pairing) |
| 0x56B990 | `PlyDat::LoadPlySlotDat` | 0x4A8CB0 | `sub_4A8B60` (10.5 0x56BCF0) | checked | sub_4A8B60 is GetHiScoreHscFileName (called) |
| 0x56BB10 | `PlyDat::UpdateStageScore` | 0x4A8F90 | ✓ `sub_453A60`, `j_RollingDigitCounter_38::sub_41DC30` | done | DecodeStage; RollingDigitCounter get_value via j_ thunk = GetScore (called); still matches |
| 0x56C010 | `PlyDat::UpdateHiScores` | 0x4A90A0 | ✓ `sub_453A60` | done | DecodeStage; unchanged (14) |
| 0x56F5C0 | `Police::SpawnRoadblockGuard` | 0x4A9B40 | ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0` | done | all 9.6f inlines used |
| 0x56F6D0 | `Police::sub_56F6D0` | 0x4A9C50 | ✓ `MarkForDespawn_421470` | done | code unchanged |
| 0x56F940 | `Police::RegisterCriminal` | 0x4A9D60 | ✓ `sub_41B0A0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | code unchanged |
| 0x56FA40 | `Police::sub_56FA40` | 0x4A9E80 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403B60` | done | code unchanged |
| 0x56FBD0 | `Police::sub_56FBD0` | 0x4AA030 | ✓ `sub_420B70`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `cool_nash_0x294::sub_420B80` | done | code unchanged; new Ped::ClearWantedPoints |
| 0x570270 | `Police::Service` | 0x4AEF70 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40` | done | code unchanged |
| 0x570320 | `Police::SpawnWalkingGuard` | 0x4AA710 | ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403920`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `sub_433C10`, ✓ `sub_433C00`, `cool_nash_0x294::sub_433DF0` | inlines added | code unchanged; SetRemap(get_remap()) breaks the match |
| 0x5703E0 | `Police::FBI_Army_5703E0` | 0x4AA7B0 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `sub_433B90`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::SetUniNum_421560` | done | code unchanged |
| 0x5707B0 | `Police::PromptCrewAtCarToPurseCriminal_5707B0` | 0x4AABB0 | ✓ `sub_41B0A0` | matched | all 9.6f inlines used |
| 0x5708C0 | `Police::UpdateLastSeenCoordsForCriminal` | 0x4AACA0 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | all 9.6f inlines used |
| 0x570940 | `Police::UpdateCriminalLatestPosition` | 0x4AAD40 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50` | done | all 9.6f inlines used |
| 0x570BF0 | `PoliceCrew::SpawnPoliceInCar` | 0x4AADD0 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::SetUniNum_421560` | done | code unchanged |
| 0x570E30 | `PoliceCrew::SpawnSWAT` | 0x4AB060 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, `sub_404400`, ✓ `sub_433360`, `sub_404420`, ✓ `Car_BC::SetUniNum_421560` | done | code unchanged |
| 0x571150 | `PoliceCrew::SpawnFBI_nonused` | 0x4AB220 | `sub_43DF60`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `sub_433B90`, ✓ `cool_nash_0x294::set_health_4039A0`, ✓ `Car_BC::SetUniNum_421560` | checked | 43DF60 = PedManager::AllocatePed |
| 0x571350 | `PoliceCrew::TryDespawnOffscreenCrew` | 0x4AB330 | ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_4039F0` | done | code unchanged |
| 0x571540 | `PoliceCrew::TryDespawnFinishedCrew` | 0x4AB400 | ✓ `sub_4A9AD0`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `MarkForDespawn_421470`, ✓ `cool_nash_0x294::sub_4039F0` | matched | Third car branch is `MarkForDespawn()` + the shared 28/2C tail; Get_F76/Get_F20E/ClearGroupAndGroupIdx inlines |
| 0x571A30 | `PoliceCrew::TryDespawnInCarCrew` | 0x4AB610 | ✓ `sub_4A9AD0`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `MarkForDespawn_421470`, ✓ `cool_nash_0x294::sub_4039F0` | inlines added | No codegen change; rest is block layout/tail merging of the 28/2C tails |
| 0x572210 | `PoliceCrew::CanTrackCriminal` | 0x4AB930 | ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | matched | all 9.6f inlines used |
| 0x572340 | `PoliceCrew::State3_AlertedSearch` | 0x4AC080 | ✓ `MarkForDespawn_421470`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `sub_472FD0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `GetObjectiveStatus_450CB0` | done | code unchanged; new Ped::IsGroupLeader |
| 0x572920 | `PoliceCrew::State5_PursueOrChase` | 0x4AC580 | ✓ `MarkForDespawn_421470`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, `sub_4048A0`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `sub_403960`, ✓ `cool_nash_0x294::sub_403AF0`, ✓ `cool_nash_0x294::sub_403AE0`, ✓ `cool_nash_0x294::get_objective_target_ped_403AD0`, ✓ `GetObjectiveStatus_450CB0`, `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `Car_BC::sub_421D90`, `cool_nash_0x294::sub_403990` | done | diff 287->281; 4048A0/421D90 noise; MaxAbsDistance not used (10.5 out-of-line Negate/Max) |
| 0x574720 | `PoliceCrew::State6_ShutDown` | 0x4ACE80 | ✓ `HasEmergencyLights_414F20`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `sub_403960`, ✓ `GetObjectiveStatus_450CB0` | done | diff 43 unchanged |
| 0x574F10 | `PoliceCrew::State1_Patrol` | 0x4AD310 | ✓ `sub_421550`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::sub_403A40`, ✓ `cool_nash_0x294::get_objective_403A80`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `sub_403960`, ✓ `GetObjectiveStatus_450CB0` | done | code unchanged |
| 0x575210 | `PoliceCrew::HandleEnterCarObjective` | 0x4AB9D0 | ✓ `GetObjectiveStatus_450CB0`, ✓ `sub_421540` | done | code unchanged |
| 0x575270 | `PoliceCrew::HandleLeaveCarObjective` | 0x4ABA30 | ✓ `GetObjectiveStatus_450CB0` | done | all 9.6f inlines used |
| 0x5752C0 | `PoliceCrew::HandleGotoAreaInCarObjective` | 0x4ABA90 | ✓ `GetObjectiveStatus_450CB0` | done | all 9.6f inlines used |
| 0x575310 | `PoliceCrew::HandleChaseInCarObjective` | 0x4ABAE0 | ✓ `cool_nash_0x294::set_objective_target_ped_403AC0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `Car_BC::sub_421EC0` | done | diff 159 unchanged; MaxAbsDistance not used (10.5 out-of-line calls); 421EC0 noise |
| 0x575590 | `PoliceCrew::Service` | 0x4AD600 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::sub_433B40`, `sub_404400` | done | code unchanged |
| 0x5757B0 | `PoliceRoadblock::Update` | 0x4AD6C0 | ✓ `cool_nash_0x294::sub_433B40`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | done | code unchanged |
| 0x575CA0 | `PoliceRoadblock::RemoveRoadblock` | 0x4ABD70 | ✓ `MarkForDespawn_421470`, ✓ `Car_BC::SetUniNum_421560`, ✓ `cool_nash_0x294::sub_403920` | done | code unchanged |
| 0x575FF0 | `PoliceRoadblock::CreateRoadblock` | 0x4ADB70 | ✓ `sub_420420`, ✓ `sub_40F540`, `sub_426E40`, ✓ `cool_nash_0x294::sub_403920`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `Car_BC::SetUniNum_421560`, ✓ `HasEmergencyLights_414F20` | done | diff 759 unchanged; 426E40 already used (Scaled variant) |
| 0x578030 | `Train::ReassignTrainHead` | 0x4AF8A0 | ✓ `Car_BC_Pool::sub_420F20`, ✓ `UnlinkFromActiveList_420F30`, `Car_BC::sub_423A50`, `CarPhysics_B0::sub_49EE10` (10.5 0x563670), ✓ `sub_4118D0`, ✓ `sub_4212B0`, ✓ `sub_4212A0`, ✓ `AllocateAI_421510`, ✓ `sub_420B70`, ✓ `Car_BC::SetUniNum_421560` | matched | 74->54; UpdateNextPrev(420F20), IsDrivenByPlayer(4118D0), SetField8C_to_1/2, SetDriverPedType used; 421510 open-coded (alloc AI, fixed DeAllocate bug) since Car.hpp can't see gCarAI_78_Pool |
| 0x578330 | `Train::CloseCarriageDoors` | 0x4AFAB0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x578360 | `Train::OpenCarriageDoors` | 0x4AFAF0 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x578390 | `Train::UpdatePassengerAI` | 0x4AFB30 | ✓ `sub_411940`, ✓ `cool_nash_0x294::set_target_objective_car_403AA0`, ✓ `cool_nash_0x294::set_target_car_door_403A70`, ✓ `Char_8::sub_420EA0`, ✓ `cool_nash_0x294::set_occupation_403970` | matched | 122->93; HasNoPeds with branches swapped to 9.6f order |
| 0x578860 | `PublicTransport::SpawnTrainsFromStations` | 0x4AFE20 | `sub_426E40`, ✓ `UnlinkFromActiveList_420F30`, ✓ `AllocateAI_421510`, ✓ `Car_BC::SetUniNum_421560`, ✓ `sub_475C30`, ✓ `StartEngine_426E00` | done | new Car::LockPermanently used (318 unchanged); 426E40/420F30/421560 already used, 421510 open-coded |
| 0x5794B0 | `PublicTransport::SetupTrainAndBusStops` | 0x4B08A0 | ✓ `sub_433470`, ✓ `sub_4334A0`, ✓ `sub_4334D0`, ✓ `sub_433500` | done | all 9.6f inlines used |
| 0x579A30 | `PublicTransport::OnBusStopMarkerHit` | 0x4B0D70 | `Car_BC::sub_421D90` | checked | 421D90 = GetCarLinearSpeed (called) |
| 0x579B90 | `PublicTransport::GetTrainSpeed` | 0x4B0DF0 | ✓ `Car_BC::sub_4118C0` | done | all 9.6f inlines used |
| 0x579CA0 | `PublicTransport::BusesService` | 0x4B0F20 | ✓ `sub_433470`, ✓ `sub_4334A0`, ✓ `sub_4334D0`, ✓ `sub_433500`, ✓ `AllocateAI_421510`, ✓ `Car_BC::SetUniNum_421560`, ✓ `StartEngine_426E00`, ✓ `sub_4118D0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403920`, `Car_BC::sub_421D90`, ✓ `sub_4A9AD0`, ✓ `MarkForDespawn_421470`, ✓ `sub_4215B0` | checked | only 421D90 listed = GetCarLinearSpeed (called in 10.5) |
| 0x57A7A0 | `PublicTransport::PublicTransportService` | 0x4B1560 | ✓ `sub_4118D0`, ✓ `Car_BC::SetUniNum_421560`, `ApplyCarVelocityCameraOffset_436200`, `sub_4AF290`, `sub_4AF860`, `sub_4AF880` | done | new Ped::SetTrainStation (4AF880 identical) used (95 unchanged); 436200=GetMoveDirection, 4AF290=StartIfHasDriver (called in 10.5) |
| 0x57B540 | `PublicTransport::GetLeadTrainCar` | 0x4B1B40 | ✓ `Car_BC::sub_403BA0` | done | all 9.6f inlines used |
| 0x57DD50 | `sound_obj::ProcessType3_CopRadioAndMusic` | 0x4B2D50 | `sub_4A65E0`, `sub_4B25A0`, ✓ `IsMaxDamage_40F890`, `sub_4B25D0` | checked | pairing noise: 10.5 still calls GetPlayerCar (4A65E0), IsTrainOrBoxcar (4B25A0), sub_57DF10 (4B25D0) |
| 0x57E220 | `sound_obj::UpdateCarRadioAudio` | 0x4B1E40 | `sub_4A65E0`, `Car_BC::sub_41E450`, `Car_BC::sub_41E440`, `Car_BC::sub_41E430` | matched | paused/static-volume block layout differs (else placed later in 10.5); more diffs further down |
| 0x57E6C0 | `sound_obj::ChooseRadioEmitterForVehicle` | 0x4B2830 | `sub_4A65E0`, ✓ `sub_411940` | checked | 9.6f sub_4A65E0 = Player::GetPlayerCar, still called in 10.5 |
| 0x57F090 | `sound_obj::IsPoliceOrServiceVehicle` | 0x4B2510 | ✓ `sub_411940` | done | all 9.6f inlines used |
| 0x582310 | `TrafficSpawner::ctor_582310` | 0x4B3490 | ✓ `Car_14_18::ctor_41D070` | done | all 9.6f inlines used |
| 0x582480 | `TrafficSpawner::SpawnTrafficCar` | 0x4B34E0 | ✓ `sub_41FE40`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `Zone_144::sub_45DD50`, ✓ `sub_4B3230`, `sub_4B33F0`, `sub_4B30A0`, `sub_42A620`, `sub_426E40`, ✓ `sub_420700`, ✓ `StartEngine_426E00`, ✓ `DespawnSoon_421490` | inlines added | 893 unchanged; new SetPaletteSprites, HasKillRespectChange, Car::DespawnSoon, + StartEngine used; SpawnCarAtCorrectZ (426E40) commented (893->907); 4B3230/4B33F0/4B30A0/42A620 not done |
| 0x5832C0 | `TrafficSpawner::MakeTrafficForCurrCamera` | 0x4B4A60 | ✓ `GetBoundariesWidth_4B3110`, ✓ `GetBoundariesHeight_4B3130` | done | all 9.6f inlines used |
| 0x583670 | `TrafficSpawner::GenerateTraffic` | 0x4B4E60 | ✓ `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `sub_433E90` | commented | get_cam_y breaks the match (get_cam_x already used) |
| 0x5885C0 | `Junction::GetDirectionToJunction` | 0x40D0C0 | ✓ `sub_40CE90` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588620 | `RouteFinder::ShowJunctionIds` | 0x40D120 | `DrawUnk_0xBC::sub_40CF60`, ✓ `DrawUnk_0xBC::WorldToScreen_40CFC0` | commented | ProjectToScreen = WorldToScreen but original uses RouteFinder's own constants 0x6FFC7C/9C, so noted; 40CF60 = IsPointInBoundaries called |
| 0x588810 | `RouteFinder::RoadOff` | 0x40E1B0 | ✓ `sub_40CE90`, ✓ `Disable_40CEC0` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588950 | `RouteFinder::RoadOn` | 0x40E2F0 | ✓ `sub_40CE90`, ✓ `Enable_40CEB0` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588B30 | `RouteFinder::Load_RGEN` | 0x40D250 | ✓ `sub_40CE90` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x588E60 | `RouteFinder::FindHorzSegmentJunction` | 0x40D390 | ✓ `ContainsPoint_40CF20` | done | all 9.6f inlines used |
| 0x588F30 | `RouteFinder::FindVertSegmentJunction` | 0x40D450 | ✓ `ContainsPoint_40CF20` | done | all 9.6f inlines used |
| 0x5895C0 | `RouteFinder::InitSearchWithDirection` | 0x40D7D0 | ✓ `sub_40CE90`, ✓ `sub_40CEA0` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x5899C0 | `RouteFinder::ExpandNode` | 0x40DB70 | ✓ `sub_40CE90`, ✓ `sub_40CED0`, ✓ `sub_40CEA0` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x589BB0 | `RouteFinder::ExpandNodeUnlessOffScreen` | 0x40DD50 | ✓ `sub_40CE90`, ✓ `sub_40CED0`, ✓ `sub_40CEA0` | done | Link helpers (40CE90/40CEA0/40CED0) already used; address comments added |
| 0x58A190 | `RouteFinder::StartRoute` | 0x40E030 | `sub_40D690` (10.5 0x589480), `sub_40DFA0` (10.5 0x589E20), `sub_40CC10` | checked | 9.6f body inlined DoStartRoute pieces (40D690=589480, 40DFA0=589E20 called in 10.5 via DoStartRoute) |
| 0x58A1C0 | `RouteFinder::ctor_58A1C0` | 0x40E100 | `array_constuctor_401CF0` | checked | array_constuctor_401CF0 is the compiler's vector ctor helper |
| 0x591C70 | `PlayerScoreTracker::Service` | 0x4B7770 | ✓ `sub_41D020`, ✓ `cool_nash_0x294::has_car_403B80`, ✓ `cool_nash_0x294::not_enter_car_as_passenger_4A5040`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `Car_BC::inline_check_0x20_info_4216C0`, ✓ `Char_8::sub_420EA0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::sub_41DC70`, `text_0x14::Find_4C23A0`, `sub_4105B0`, ✓ `CarPhysics_B0::is_backward_gas_on_411810`, `Car_BC::sub_421EC0`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `Game_0x40::sub_4B7590`, ✓ `rng::get_cur_rng_41CFE0` | done | Ped::get_car, not_enter_car_as_passenger, get_wanted_points and Car::HasTaxiLight; code unchanged |
| 0x592380 | `PlayerScoreTracker::ResetCarModelFlags` | 0x4B75B0 | ✓ `gtx_0x106C::sub_420200`, `sub_4BFFE0` | checked | does_car_exist (420200) already used; sub_4BFFE0 = IsCarModelInRecycleList (still called) |
| 0x592430 | `PlayerScoreTracker::CheckAllCarModelsFlagged` | 0x4B7D80 | ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::sub_41DC70`, `text_0x14::Find_4C23A0`, `sub_4105B0` | inlines added | Player::AddScore for the AddCash calls; code unchanged |
| 0x592620 | `PlayerScoreTracker::AddCash` | 0x4B7660 | ✓ `angry_lewin_0x85C::sub_4219D0` | done | all 9.6f inlines used |
| 0x592660 | `PlayerScoreTracker::AwardPedKilledScore` | 0x4B7EB0 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_car_416B60`, ✓ `cool_nash_0x294::get_remap_433BA0`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `sub_41DC40`, `CrimeReportQueue_CC::sub_44A370`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier (new), IsUser, GetPlayerPed_4A5130, AddScore, Ped get_remap/get_cam_x/y/z; 259->125; Shooey sub_44A370 = sub_485140 (still called); get_car_model already used |
| 0x592DD0 | `PlayerScoreTracker::AwardCarDestroyedScore` | 0x4B85B0 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, `cool_nash_0x294::get_car_416B60`, ✓ `sub_41C1F0`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_4118F0`, ✓ `sub_421790`, ✓ `sub_421780`, ✓ `sub_4217A0`, ✓ `sub_411920`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier, AddScore, GetPlayerPed_4A5130 and Car::get_x/y/z_41E4x0; code unchanged |
| 0x593030 | `PlayerScoreTracker::AwardCarDamageScore` | 0x4B8870 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::get_multiplier, AddScore, GetPlayerPed_4A5130 and Car::get_x/y/z_41E4x0; code unchanged |
| 0x593150 | `PlayerScoreTracker::sub_593150` | 0x4B89B0 | ✓ `IsMaxDamage_40F890`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A B C (A=get_multiplier, B=AddScore, C=GetPlayerPed_4A5130, D=Car::get_x/y/z_41E4x0, E=IsUser); code unchanged |
| 0x593240 | `PlayerScoreTracker::AwardCarHijackedScore` | 0x4B8A70 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_41C1F0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A C D E (A=get_multiplier, B=AddScore, C=GetPlayerPed_4A5130, D=Car::get_x/y/z_41E4x0, E=IsUser); code unchanged |
| 0x593370 | `PlayerScoreTracker::AwardBusStolenScore` | 0x4B8BD0 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | Player::AddScore and get_multiplier for the score; code unchanged |
| 0x593410 | `PlayerScoreTracker::AwardFullBusDestroyedScore` | 0x4B8C80 | ✓ `angry_lewin_0x85C::sub_4766A0`, ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `Car_BC::sub_41E450`, ✓ `Car_BC::sub_41E440`, ✓ `Car_BC::sub_41E430`, ✓ `sub_41DC40`, ✓ `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | done | transforms A C D (A=get_multiplier, B=AddScore, C=GetPlayerPed_4A5130, D=Car::get_x/y/z_41E4x0, E=IsUser); code unchanged |
| 0x5934F0 | `PlayerScoreTracker::UpdateAccuracyCount` | 0x4B76A0 | ✓ `cool_nash_0x294::get_occupation_403980` | done | all 9.6f inlines used |
| 0x5935D0 | `PlayerScoreTracker::ChangeFragsByAmount` | 0x4B8A60 | `sub_41DC40` | checked | pairing noise (9.6f sub_41DC40 is AddScore, not used by ChangeFrags) |
| 0x596890 | `ExplodingScorePool::PushScore` | 0x4B91F0 | ✓ `CokeZero_FC::sub_4B8FD0` (10.5 0x5935C0), `CokeZero_FC::sub_4B9000`, `CokeZero_FC::sub_4B8FE0` | done | new Pool::GetFirstUsed; 4B9000/4B8FE0 = FindAndDeAllocate/Allocate already used; still matches |
| 0x5969E0 | `ExplodingScorePool::DrawExplodingScores` | 0x4B98B0 | ✓ `CokeZero_FC::sub_4B8FD0` (10.5 0x5935C0) | done | Pool::GetFirstUsed; still matches |
| 0x596C90 | `ExplodingScore::DrawNumbers` | 0x4B94B0 | ✓ `DrawUnk_0xBC::sub_4B90E0`, `CokeZero_50::sub_4B92B0` | matched | private out-of-line copy of 9.6f inline 4B90E0 (Camera::ProjectWorldToScreen_OutOfLine_4B90E0); CokeZero_50::sub_4B92B0 is DrawSingleNumber, called |
| 0x59DDF0 | `Fix16_Rect::IntersectsSpriteRenderingRect` | 0x4BA6C0 | ✓ `sub_4B9FD0` | done | all 9.6f inlines used |
| 0x59DE80 | `Fix16_Rect::IsRectBlockedByMap` | 0x4BA720 | `sub_4BA5E0` | matched | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect, called in 10.5 |
| 0x59E170 | `Sprite::IsControlledByRemotePlayer` | 0x4BCA80 | `sub_4BAA70`, ✓ `sub_40FEB0`, `sub_423480`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | Sprite::AsCar; Still matches. |
| 0x59E1D0 | `Sprite::IsOnWater` | 0x4BAA90 | ✓ `IsRemappedWaterTile_49E540` | done | gtx IsRemappedWaterTile; Still matches. |
| 0x59E250 | `Sprite::GetWaterCornerMask` | 0x4BDD40 | ✓ `IsWaterBlockAt_4B9F40` | done | all 9.6f inlines used |
| 0x59E320 | `Sprite::IncreaseZoom` | 0x4BAB10 | ✓ `sub_4BA230` | done | New Sprite::GetTrueSpriteIdx (defined in sprite.cpp); Still matches. |
| 0x59E590 | `Sprite::CollidesWith` | 0x4BCAC0 | ✓ `sub_41E390` | done | all 9.6f inlines used |
| 0x59E7D0 | `Sprite::QuerySpriteCollision` | 0x4BDFE0 | `sub_4B9F30`, ✓ `SetSprite_40FEE0` | done | CollisionSoundQueue::SetSprite; Still matches. |
| 0x59E8C0 | `Sprite::HandleObjectCollision` | 0x4BAB70 | `sub_40FEC0`, ✓ `HandleCollisionWithObject_484DD0` | commented | AsObject2C for both sprites broke the match |
| 0x59E9C0 | `Sprite::UpdateCollisionBoundsIfNeeded` | 0x4BCB40 | ✓ `sub_41E390` | done | all 9.6f inlines used |
| 0x59F950 | `Sprite::AllocCollisionShape` | 0x4BCB90 | ✓ `SpriteShapePool::sub_4BC9F0`, ✓ `SpriteShape_4C::sub_482980` | done | SpriteShape::SetDimensions; Still matches. |
| 0x59F990 | `Sprite::EnsureVisualShape` | 0x4BCBD0 | ✓ `SpriteShapePool::sub_4BC9F0`, ✓ `sub_4BA070` | done | Sprite::GetTrueSpriteIdx (pool Allocate is 9.6f 0x4BC9F0); Still matches. |
| 0x59FA40 | `Sprite::UpdateDimensionsFromSpriteIndex` | 0x420600 | `Car_3C::sub_4B99F0` | done | Sprite::GetTrueSpriteIdx (sub_4B99F0 is InvalidateZColAndBBoxes); Still matches. |
| 0x59FAD0 | `Sprite::FreeShapes` | 0x4BCCC0 | ✓ `SpriteShapePool::sub_4BCA10` | checked | SpriteShapePool::sub_4BCA10 is the pool Remove template, already used |
| 0x59FB10 | `Sprite::IntersectsRectSAT` | 0x4BB020 | ✓ `sub_4BA0A0`, ✓ `sub_45ADD0`, ✓ `sub_45ADB0`, ✓ `sub_42A720`, ✓ `sub_45ADA0`, ✓ `sub_45ADC0` | done | all 9.6f inlines used |
| 0x5A0380 | `Sprite::RotatedRectCollisionSAT` | 0x4BB560 | ✓ `sub_41E390`, ✓ `sub_4BA0A0`, ✓ `sub_42A720` | matched | all 9.6f inlines used |
| 0x5A0970 | `Sprite::CheckBBoxScanlineIntersection` | 0x4BB860 | `sub_472950`, ✓ `SetHorizontalSegment_4BA250` | commented | SetHorizontalSegment changes store order (broke match); 472950=ComputeScanlineIntersectionY called |
| 0x5A0EF0 | `Sprite::HitTestVerticalLine` | 0x4BB910 | `sub_472AE0`, ✓ `SetVerticalSegment_4BA280` | done | CollisionSoundQueue::SetVerticalSegment; Still matches. |
| 0x5A1A60 | `Sprite::CheckSlopeTopInFootprint` | 0x4BD290 | ✓ `sub_4B9F80` | done | New file-local ClampPurpleRectToMap_4B9F80; Still matches. |
| 0x5A1B30 | `Sprite::ResolveZOrder` | 0x4BE570 | ✓ `sub_4BA220` | done | all 9.6f inlines used |
| 0x5A1BD0 | `Sprite::ComputeZLayer` | 0x4BD2E0 | ✓ `Car_BC::sub_403BA0`, `sub_491EE0` | checked | sub_491EE0 is Fix16::ToInt (already used) |
| 0x5A1CA0 | `Sprite::CheckCornerZCollisions` | 0x4BD350 | `sub_492170` | commented | GetDepth changes the code (broke match) |
| 0x5A2710 | `Sprite::FindCollisionIntersectionPoint` | 0x4BD8A0 | `sub_4207B0`, ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, `sub_4BB3C0`, `sub_49E360` | checked | 4207B0=get_x_y_443580 and 4BB3C0=FindOverlappingBoundingBoxCorners are called in 10.5; 49E360 is Fix16_Point operator/ (used) |
| 0x5A2A30 | `Sprite::ResolveCollisionWithCarPedOrObject` | 0x4BDAD0 | `SpriteList_4::ctor_424620`, ✓ `sub_416B40`, ✓ `sub_40FEB0`, ✓ `sub_4BA390`, ✓ `HasOtherCarOnTrailer_475E60`, ✓ `sub_40FEA0`, ✓ `sub_40FE80`, ✓ `sub_40FEC0` | checked | SpriteList ctor (424620) already used |
| 0x5A3100 | `Sprite::AttachSprite` | 0x4B9D50 | `sub_484880`, `sub_422260` | checked | sub_484880 = EnsureObject3C/ReactivateObjectAfterImpact pieces, sub_422260 = Car::Deactivate (called) |
| 0x5A3550 | `SpriteShape::UpdateRotatedBoundingBox` | 0x4BBD40 | ✓ `sub_432860`, ✓ `sub_40F6B0`, ✓ `sub_45ADB0`, ✓ `sub_45ADA0`, ✓ `sub_45ADD0`, ✓ `sub_45ADC0`, `sub_4B9E60` | inlines added | SetXY x21 (836->638); rect getters 45ADB0.. commented (->648); 4B9E60 = FindMinMax |
| 0x5A4D90 | `SpriteShape::SetCurrentRect` | 0x4BC580 | `sub_4BA5E0` | checked | sub_4BA5E0 is Fix16_Rect::DoSetCurrentRect, called in 10.5 |
| 0x5A5870 | `SpriteManager_8::AllocScratchSprite` | 0x4BDC60 | ✓ `SpritePool::sub_421000` | done | all 9.6f inlines used |
| 0x5A58A0 | `SpriteManager_8::ctor_5A58A0` | 0x4BE5B0 | ✓ `SpritePool::ctor_4BCA20`, ✓ `SpriteShapePool::ctor_4BC9A0`, ✓ `AttachedSpritePool::ctor_4BDCF0` | done | all 9.6f inlines used |
| 0x5A5B50 | `SpriteManager_8::dtor_5A5B50` | 0x4BE730 | `SpritePool::gdtor_4BDC90`, `SpriteShapePool::gdtor_4BDCB0`, `SpriteMaskCache_3CC::gdtor_4B9F00`, `AttachedSpritePool::gdtor_4BE710` | checked | pool gdtors are compiler scalar deleting dtors |
| 0x5A6910 | `AttachedSprite::UpdateAttached` | 0x4BE870 | ✓ `sub_40FEC0`, `Object_2C_Pool::sub_484DB0`, ✓ `sub_40FEB0`, `Car_BC::sub_429FF0`, ✓ `sub_40F490`, ✓ `sub_40FEA0` | checked | 484DB0 = Object pool DeAllocate (template, used), 429FF0 = UpdateAttachedToSprite (called) |
| 0x5A6A50 | `SpriteList::GetSpriteForModel` | 0x4BE980 | ✓ `sub_40FEC0` | done | AsObject2C used; code unchanged |
| 0x5A6AD0 | `SpriteList::FindFirstFireExplosion` | 0x4BEA60 | `rng::get_cur_rng_41CFE0` | checked | pairing error: 9.6f 0x4BEA60 is a different function (rng store) |
| 0x5A6B10 | `SpriteList::RemoveSprite` | 0x4BEC60 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = AttachedSprite pool DeAllocate (already) |
| 0x5A6B60 | `SpriteList::RemoveSpriteSafe` | 0x4BECB0 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6BB0 | `SpriteList::DestroyAllAttached` | 0x4BEA20 | `sub_4BE920` | checked | 4BE920 = AttachedSprite::DestroyAttached, already its own helper |
| 0x5A6BD0 | `SpriteList::SinkAllAttachedCars` | 0x4BEA40 | `sub_4BE950` | checked | 4BE950 = AttachedSprite::SinkAttachedCar, already its own helper |
| 0x5A6BF0 | `SpriteList::DispatchCarImpactEvents` | 0x4BE7A0 | `sub_4B9A80` | checked | 4B9A80 = Sprite::ProcessCarToCarImpactIfCar, already used |
| 0x5A6C40 | `SpriteList::RemoveAllExceptRngValue` | 0x4BED00 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6D00 | `SpriteList::PushAttachedSprite` | 0x4BED90 | `AttachedSpritePool::sub_4BEC40` | checked | 4BEC40 = AttachedSprite pool Allocate (already) |
| 0x5A6D40 | `SpriteList::PushSprite` | 0x4BEDD0 | `AttachedSpritePool::sub_4BEC40` | checked | 4BEC40 = Allocate (already) |
| 0x5A6DC0 | `SpriteList::PopBackSprite` | 0x4BEE30 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6E10 | `SpriteList::ClearList` | 0x4BEE80 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A6E40 | `SpriteList::FindClosestSprite` | 0x4BEAC0 | ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0) | commented | MaxAbsDistance changes the matching code |
| 0x5A6EA0 | `SpriteList::TakeClosestSprite` | 0x4BEEB0 | ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), `AttachedSpritePool::sub_4BEC50` | commented | MaxAbsDistance worse (4->9); 4BEC50 = DeAllocate (already) |
| 0x5A6F70 | `SpriteList::PoolUpdate` | 0x4BEF70 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A7010 | `SpriteList::DestroyAllSprites` | 0x4BF000 | ✓ `sub_416B40`, `sub_428F70`, `sub_485260` | checked | 428F70 = CarManager::RemoveFromPoolAndCollision, 485260 = RemoveAndFree (called) |
| 0x5A7080 | `SpriteList::CleanupSpriteList` | 0x4BF070 | ✓ `sub_416B40`, ✓ `sub_4BE830`, `sub_485260`, `AttachedSpritePool::sub_4BEC50` | checked | 485260 = Object_5C::RemoveAndFree (called), 4BEC50 = DeAllocate (already) |
| 0x5A7110 | `SpriteList::ClearGangIconSprite` | 0x4BF100 | ✓ `sub_40FEC0`, ✓ `sub_4BE850`, `sub_485260`, `AttachedSpritePool::sub_4BEC50` | done | AsObject2C + IsGangIcon (added) used; 485260/4BEC50 called/already; code unchanged |
| 0x5A71F0 | `SpriteList::ExpireAttachedFlames` | 0x4BEB70 | ✓ `sub_416B40`, ✓ `sub_4BE830` | done | Object::IsFlameObject used; code unchanged |
| 0x5A7240 | `SpriteList::PruneNonCollidingSprites` | 0x4BF180 | `AttachedSpritePool::sub_4BEC50` | checked | 4BEC50 = DeAllocate (already) |
| 0x5A72B0 | `SpriteList::PropagateMaxZLayer` | 0x4BEBC0 | ✓ `sub_446950`, ✓ `sub_4BA220` | done | Sprite::IsTypeAbove1 added+used; code unchanged |
| 0x5AA9A0 | `gtx_0x106C::BuildCarInfoContainer` | 0x4C0410 | `sub_4C03F0` | checked | Door count/offset arithmetic: 10.5 keeps num_remaps in edi then adds 0xE after the first read; not reproduced yet |
| 0x5AB0F0 | `gtx_0x106C::load_font_base` | 0x4BFDD0 | `sub_4BF790` | checked | sub_4BF790 is SumCounts (called) |
| 0x5AB2C0 | `gtx_0x106C::load_palete_base` | 0x4BFF20 | `sub_4BF790` | checked | sub_4BF790 is SumCounts (called) |
| 0x5AB750 | `gtx_0x106C::LoadSty` | 0x4C0820 | ✓ `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version already used |
| 0x5AB820 | `gtx_0x106C::ctor_5AB820` | 0x4C08D0 | ✓ `sub_4C03D0` | done | New gtx_0x106C::ResetSpecs; Still matches. |
| 0x5AE060 | `TaxiList::ctor_5AE060` | 0x4C09C0 | ✓ `Taxi_4::Init_4C09B0`, `Taxi_4_Pool::ctor_4C0950` | checked | 4C09B0/4C0950 = Init / TaxiListPool ctor (already) |
| 0x5B1170 | `NoRefs_sub` | 0x4C0AD0 | `sub_4BF550`, ✓ `gmp_block_info::init_44C840`, `sub_4C0A80`, ✓ `sub_475C30`, ✓ `sub_476270`, `sub_426E40`, `sub_475A50` | checked | debug-only ImGui reimplementation; 9.6f callees (4BF550 etc.) pair with called 10.5 functions |
| 0x5B2640 | `UnknownDebugClass::DoTest` | 0x4C1F80 | ✓ `sub_4766B0`, ✓ `sub_476860` | done | Hud_Arrow::SetArrowTargetCar; code unchanged |
| 0x5B5BC0 | `TextTable::InsertLineBreaksAndGetNumLines` | 0x4C2450 | ✓ `sub_4C23D0`, ✓ `sub_4539D0` | inlines added | new GetSpaceWidth (gtx_0x106C.hpp) 70->62; GetCharWidthFromFontType noted (Draw.hpp include changes static init/dtor code) |
| 0x5B5E90 | `TextTable::Load` | 0x4C2540 | ✓ `chunk::verify_type_460EE0`, `chunk::verify_version_460EC0` | checked | chunk verify_type/verify_version (460EE0/460EC0) already used |
| 0x5B5F90 | `TextTable::Find` | 0x4C6E30 | `ObjectDefinitions_8CA8::sub_488170` (10.5 0x534360) | todo |  |
| 0x5B5FB0 | `TextTable::ctor_5B5FB0` | 0x4C2620 | `text_tkey::ctor_4C23F0`, `text_tdat::ctor_4C2420` | checked | text_tkey/text_tdat ctors (4C23F0/4C2420) already used as member ctors |
| 0x5B6050 | `TextTable::dtor_5B6050` | 0x4C26C0 | `text_tdat::dtor_4C2430`, `text_tkey::dtor_4C2400` | checked | text_tkey/text_tdat dtors (4C2400/4C2430) already used as member dtors |
| 0x5B9180 | `TextureCache::LoadCarDamageTextures` | 0x4C2F90 | `sub_4C2EF0` | checked | sub_4C2EF0 is SpriteMaskCache::GetMaskPixels (already used) |
| 0x5B92E0 | `TextureCache::ReadTextures` | 0x4C3040 | ✓ `gtx_0x106C::has_tiles_4C2EE0`, ✓ `gtx_0x106C::get_tile_4C2EB0` | done | all 9.6f inlines used |
| 0x5B9350 | `TextureCache::LoadStyleTextures` | 0x4C30A0 | ✓ `sub_4C2F30` | checked | sub_4C2F30 pairs with an out-of-line texture setup call in 10.5 |
| 0x5B9790 | `TextureCache::ctor_5B9790` | 0x4C3190 | `array_constuctor_401CF0`, `RemapTextureSet_14::ctor_4C2F10` | checked | RemapTextureSet ctor (4C2F10) is already an inline ctor; array_constuctor_401CF0 is the compiler helper |
| 0x5BC260 | `TileAnimManager::AddAnim` | 0x4C3430 | `sub_4C3380`, ✓ `sub_4C33F0` | done | New TileAnim::Init (sub_4C3380 is the pool Allocate); Still matches. |
| 0x5BC2C0 | `TileAnimManager::AddGmpAnim` | 0x4C3470 | `sub_4C3380` | checked | sub_4C3380 is the TileAnim pool Allocate (template, used) |
| 0x5BC3A0 | `TileAnimManager::ctor_5BC3A0` | 0x4C35A0 | ✓ `TileAnimPool::ctor_4C34B0` | checked | TileAnimPool ctor is the pool template ctor |
| 0x5BEE90 | `get_rdtsc` | 0x4C3950 | `sub_4C3850` | checked | sub_4C3850 is the rdtsc body itself (10.5 has it in get_rdtsc directly, inline asm) |
| 0x5C27A0 | `TrafficLight::UpdateLightsFromPhase` | 0x4C3A10 | `sub_483C20`, `sub_476B00` | todo |  |
| 0x5C2950 | `TrafficLights::TrafficLightsService` | 0x4C4A60 | ✓ `sub_4C39F0` | checked | ToggleTrafficLightState already used; 4C3A10 = called debug/update function (pairing) |
| 0x5C2AC0 | `TrafficLights::CreateFromMapZones` | 0x4C4B00 | `TrafficLights_194::sub_4C4A30` | checked | 4C4A30 = AddTrafficLight (called) |
| 0x5C5CF0 | `SpriteTree::AddSprite` | 0x4C4BF0 | ✓ `Alloc_4C4B40` | done | all 9.6f inlines used |
| 0x5C5DF0 | `SpriteTree::Draw` | 0x4C4D20 | ✓ `SpriteTreeStack_FA4::Push_4C4B80`, ✓ `SpriteTreeStack_FA4::IsEnd_4C4BC0`, ✓ `SpriteTreeStack_FA4::Pop_4C4BA0`, `Car_3C::sub_4BE060` | checked | Car_3C::sub_4BE060 is Sprite::Draw, called |
| 0x5C5E50 | `SpriteTree::Reset` | 0x4C4D60 | ✓ `SpriteTreeNodePool_2EE4::sub_4C4B70` | done | new SpriteTreeNodePool::Reset; still matches |
| 0x5C5E70 | `SpriteTree::ctor_5C5E70` | 0x4C4DF0 | `SpriteTreeStack_FA4::ctor_4C4BD0` | matched | SpriteTreeStack ctor (4C4BD0) already inline in class |
| 0x5C5F60 | `SpriteTreeNodePool::ctor_5C5F60` | 0x4C4DC0 | ✓ `SpriteTreeNodePool_2EE4::sub_4C4B70` | done | SpriteTreeNodePool::Reset; still matches |
| 0x5C86C0 | `CarThreadTriggers::AddThreadTrigger` | 0x4C4F30 | ✓ `sub_4C4F10` | done | new Object::SetThreadTriggerIdx, code unchanged |
| 0x5C8780 | `CarThreadTriggers::CheckThreadTrigger` | 0x4C4FE0 | ✓ `sub_416B40`, ✓ `sub_40FEB0`, ✓ `sub_40FEA0`, ✓ `sub_433A20`, ✓ `sub_4C4F20`, ✓ `sub_4118B0`, ✓ `sub_433C20` | done | 150->145; AsCharB4, new Ped::GetGameObjectVelocity, get_id used; 47ED20 = SetGeneratorTypeForCar (called in 10.5) |
| 0x5CBC90 | `EmergencyCrew::ReplaceLeaderIfNeeded` | 0x4C5510 | ✓ `cool_nash_0x294::sub_403990`, ✓ `cool_nash_0x294::get_occupation_403980`, ✓ `cool_nash_0x294::set_occupation_403970` | done | all 9.6f inlines used |
| 0x5CBD50 | `EmergencyCrew::UpdateStateMachine` | 0x4C55D0 | ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, `HasSpriteZoom_421D80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403A30`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `Car_BC::SetUniNum_421560` | matched | 442->439; SetDriverPedType used; 421D80 = Car::HasSpriteZoom (called in 10.5) |
| 0x5CC1C0 | `EmergencyCrew::CleanupExpiredEntities` | 0x4C5A00 | ✓ `sub_4215B0`, ✓ `IsMaxDamage_40F890`, ✓ `sub_4A9AD0`, ✓ `MarkForDespawn_421470`, ✓ `cool_nash_0x294::sub_4039F0`, ✓ `cool_nash_0x294::sub_403B60`, ✓ `cool_nash_0x294::set_occupation_403970`, ✓ `cool_nash_0x294::sub_403A30` | done | all 9.6f inlines used |
| 0x5CF910 | `Hud_UnderRoofArrowMarker::Draw` | 0x4C74A0 | ✓ `angry_lewin_0x85C::get_camera_434900` | done | Player::get_camera; Still matches. |
| 0x5CF970 | `Hud_ShowCoords::ShowPlayerCoords` | 0x4CA680 | ✓ `sub_4A5150`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `Hud_TextEntry_C4::SetFontRemap_45AFD0` | done | Hud_TextEntry::SetFontRemap; Still matches. |
| 0x5CFA70 | `Hud_GangRespectBars::DrawGangRespectBars` | 0x4C74F0 | ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `rng::get_cur_rng_41CFE0` | done | all 9.6f inlines used |
| 0x5D00B0 | `Hud_CopHead_C_Array::UpdateWantedLevel` | 0x4C79D0 | ✓ `sub_41D020` | done | all 9.6f inlines used |
| 0x5D0260 | `Hud_Health::DrawHealth` | 0x4C7B70 | ✓ `sub_41D020`, ✓ `sub_433B70` | done | all 9.6f inlines used |
| 0x5D03C0 | `ArrowTrace::PointToInfoPhone` | 0x4C7CC0 | ✓ `sub_4767C0` | done | all 9.6f inlines used |
| 0x5D03F0 | `ArrowTrace::UpdateAimCoordinates` | 0x4C7CF0 | ✓ `sub_461DB0`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::sub_433B40`, ✓ `sub_4215B0`, ✓ `sub_476830`, ✓ `sub_4117B0`, ✓ `angry_lewin_0x85C::get_camera_434900` | done | Player::get_camera; Still matches. |
| 0x5D0530 | `Hud_Arrow::CheckVisibility` | 0x4CA770 | ✓ `sub_4C70B0`, ✓ `sub_4C7350`, ✓ `angry_lewin_0x85C::sub_4766D0`, ✓ `sub_4C7340`, ✓ `angry_lewin_0x85C::sub_4219D0` | done | New Hud_Arrow_7C_Array::ShowGangArrows and ScriptManager::IsOnMission; Still matches. |
| 0x5D0620 | `Hud_Arrow::UpdateTargets` | 0x4C7E60 | ✓ `HasNoTarget_4C6F20`, ✓ `sub_4C7060`, ✓ `HasBothTargets_4C6FB0`, ✓ `sub_432860` | matched | SetXY on one point x2; 67->53 |
| 0x5D0C60 | `Hud_Arrow::Service` | 0x4CA860 | `sub_4C7FC0` | checked | sub_4C7FC0 (759 bytes) is the arrow service body; not split out (big open-coded body in a matching function) |
| 0x5D0C90 | `Hud_Arrow::DrawArrow` | 0x4C82C0 | ✓ `HasNeitherTarget_4C6F80`, ✓ `sub_4C7050`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `angry_lewin_0x85C::get_camera_434900` | done | HasNeitherTarget and IsVisible; Still matches. |
| 0x5D0E40 | `Hud_Arrow_7C_Array::IsThereAnyOtherArrowsInSameGang` | 0x4C8470 | ✓ `HasNeitherTarget_4C6F80`, ✓ `sub_4C7050` | done | all 9.6f inlines used |
| 0x5D0E90 | `Hud_Arrow_7C_Array::DrawArrows` | 0x4C84C0 | ✓ `sub_434B10`, ✓ `sub_4C6F30`, ✓ `sub_41D020` | done | New ArrowTrace::GetTargetPlayer; Still matches. |
| 0x5D0EF0 | `Hud_Arrow_7C_Array::FindVisibleGangArrow` | 0x4C8540 | ✓ `HasNeitherTarget_4C6F80` | done | Hud_Arrow::HasNeitherTarget; Still matches. |
| 0x5D0F40 | `Hud_Arrow_7C_Array::IsThereAnyMissionPhoneArrowForGang` | 0x4C8590 | ✓ `HasNeitherTarget_4C6F80` | done | Hud_Arrow::HasNeitherTarget; Still matches. |
| 0x5D0F80 | `Hud_Arrow_7C_Array::ClearOrphanInfoPhoneArrows` | 0x4C85D0 | ✓ `HasNeitherTarget_4C6F80`, ✓ `sub_476880` | done | Hud_Arrow::HasNeitherTarget; Still matches. |
| 0x5D0FD0 | `Hud_Arrow_7C_Array::UpdateArrows` | 0x4CA890 | ✓ `HasNeitherTarget_4C6F80` | done | Hud_Arrow::HasNeitherTarget; Still matches. |
| 0x5D1050 | `Hud_Arrow_7C_Array::AllocArrow` | 0x4CA8E0 | ✓ `sub_4CA610`, ✓ `sub_4C6FF0` | done | New Hud_Arrow::Reset and SetMinRadiusPos (defined in Hud.cpp); Still matches. |
| 0x5D13C0 | `Hud_QuitMessage::HandleQuitMessageKey` | 0x4C8690 | ✓ `angry_lewin_0x85C::sub_41DC70`, ✓ `sub_434B10`, ✓ `angry_lewin_0x85C::sub_4219D0`, ✓ `sub_461DD0` | done | New TagGame::SetQuit; Still matches. |
| 0x5D1430 | `Hud_QuitMessage::DrawQuitMessage` | 0x4C8710 | `text_0x14::Find_4C23A0` | todo |  |
| 0x5D16B0 | `Hud_ChatInput::DrawChatMessages` | 0x4C8910 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700), ✓ `rng::get_cur_rng_41CFE0` | matched | GetLineSpacingFromFontType_5D7700_inlined (9.6f 0x4539B0), s32 positions so both go through Fix16(s32) |
| 0x5D1B10 | `Hud_TextEntry::FormatAndSetupText` | 0x4C8AA0 | ✓ `Hud_TextEntry_C4::sub_4C70E0` | done | new Hud_TextEntry::ClearAlpha (byte store matches now); 15 unchanged |
| 0x5D1EB0 | `Hud_TextList::ExpireDuplicates` | 0x4C8BE0 | ✓ `Hud_TextEntry_C4::sub_4C70F0` | done | New Hud_TextEntry::Expire; Still matches. |
| 0x5D2AB0 | `Hud_Pager::DrawPager` | 0x4C9040 | ✓ `sub_4C7250`, `sub_4C8CA0` | checked | sub_4C8CA0 pairs with 10.5 out-of-line 5D2380/5D2680 (called) |
| 0x5D3040 | `Hud_Pager_C_Array::DrawPagers` | 0x4C92A0 | ✓ `sub_4C7250`, ✓ `sub_4C7220` | done | all 9.6f inlines used |
| 0x5D31F0 | `Hud_Pager_C_Array::CreateTimer` | 0x4C9310 | ✓ `sub_4C7170`, ✓ `sub_4C7160`, ✓ `sub_4C7120` | done | New Hud_Pager::IsTimerOff and set_timer, plus no_ptr_counter; Still matches. |
| 0x5D3220 | `Hud_Pager_C_Array::AddOnScreenCounter` | 0x4C9360 | ✓ `sub_4C7160`, ✓ `sub_4C7170`, ✓ `sub_4C7130` | done | Hud_Pager::SetCounter (defined in Hud.cpp), IsTimerOff, no_ptr_counter; Still matches. |
| 0x5D3470 | `Hud_Brief::FormatCurrentBrief` | 0x4C6470 | `text_0x14::Find_4C23A0` | todo |  |
| 0x5D3B80 | `Hud_Brief::DrawBrief` | 0x4C9430 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700) | done | GetLineSpacingFromFontType_5D7700_inlined + s32 ypos; 20->0 CANDIDATE MATCH (w.sh only) |
| 0x5D4A10 | `Hud_CarName::DrawCarName` | 0x4C94F0 | ✓ `sub_4C7220` | done | get_user_sprite_width; stays 0 (candidate, w.sh only) |
| 0x5D5600 | `Hud_PickupText::ShowPickupText` | 0x4C69F0 | `text_0x14::Find_4C23A0` | todo |  |
| 0x5D5770 | `Hud_MpMessage::AnnounceKill` | 0x4C9750 | ✓ `angry_lewin_0x85C::sub_41DC70`, `text_0x14::Find_4C23A0`, `sub_4105B0` | checked | sub_4105B0 is a root_sound thunk (PlayVoice, called) |
| 0x5D5900 | `Hud_MapZone::DrawZoneName` | 0x4C9890 | ✓ `sub_4C7220` | done | get_user_sprite_width; still matches |
| 0x5D5B60 | `Hud_MapZone::UpdateZoneName` | 0x4C6B70 | `sub_4A6530` | checked | sub_4A6530 (171 bytes) open-coded; not split out |
| 0x5D5C80 | `Hud_PlayerStats::DrawPlayerStats` | 0x4C9C20 | ✓ `sub_4A4FB0`, ✓ `sub_434B20`, ✓ `sub_4C7380`, ✓ `angry_lewin_0x85C::sub_41DC70` | done | New TagGame::GetPlayerTime (defined in Hud.cpp); Still matches. |
| 0x5D6060 | `DrawAmmo` | 0x4C9A40 | ✓ `sub_4C7220`, ✓ `sub_4C7250` | done | get_user_sprite_width/get_user_sprite_height; still matches |
| 0x5D61A0 | `DrawPlayerStatsHelper` | 0x4C9B40 | ✓ `sub_420220` | done | all 9.6f inlines used |
| 0x5D63B0 | `Hud_PauseScreen::DrawPause` | 0x4C9FA0 | ✓ `sub_416BC0`, `text_0x14::Find_4C23A0`, ✓ `sub_420220`, ✓ `GameSession_578::sub_453A80` | done | new ObjectDefinitions::GetObjectBaseSpriteIdx, get_sprite_width; unchanged (37) |
| 0x5D6860 | `Hud::DrawGui` | 0x4CA440 | `sub_4C78A0`, `sub_4C7A30` | checked | sub_4C78A0/sub_4C7A30 (304/218 bytes) open-coded in DrawGui; not split out |
| 0x5D69D0 | `Hud::UpdateHUD` | 0x4CAB50 | `sub_4C73A0`, `sub_4C62B0` | checked | sub_4C73A0 (246 bytes) and sub_4C62B0 (pager loop) open-coded in UpdateHUD; not split out |
| 0x5D6B00 | `Hud::SetFontTypes` | 0x4CA520 | ✓ `LangIsJapanese_452E60` | done | all 9.6f inlines used |
| 0x5D6C20 | `Hud::HandleInputKey` | 0x4CA5D0 | `sub_4C8880` | checked | sub_4C8880 (142 bytes) open-coded in IsBusy; not split out |
| 0x5D6CB0 | `Hud::IsQuitMessageInputKey` | 0x4CA650 | `sub_4C7CC0` (10.5 0x5D03C0) | checked | 9.6f sub_4C7CC0 (10.5 copy 0x5D03C0) vs 10.5 call to IsQuitMessageKey: pairing difference |
| 0x5D6CD0 | `Hud::ctor_5D6CD0` | 0x4CAC60 | `Garox_C_Array::ctor_4CA660`, `Garox_7C_Array::ctor_4C7080`, `Garox_Sub_C_Array::ctor_4C6EE0`, `Hud_PickupText_88::ctor_4C6A50`, `Hud_UnderRoofArrowMarker_C::ctor_4C6E50`, `Hud_PauseScreen_2::ctor_4C71A0` | matched | member array ctors (4CA660, 4C7080, ...) are already inline ctors in Hud.hpp |
| 0x5D7EC0 | `DrawFigure` | 0x4CBA50 | ✓ `sub_432860`, ✓ `sub_40F6B0` | done | single point + SetXY x4; 412->345 |
| 0x5D8940 | `CountLineSpacing` | 0x4CC0C0 | ✓ `gtx_0x106C::sub_4539B0` (10.5 0x5D7700) | done | all 9.6f inlines used |
| 0x5D8A10 | `DrawString` | 0x4CC100 | `KanjiFont_8EC::sub_460DA0`, `KanjiFont_8EC::sub_4CBA40`, `KanjiFont_8EC::sub_4CBA00`, `sub_4CBA10`, `sub_460CC0`, `KanjiFont_8EC::sub_4CBA20`, `sub_4CBA30`, `sub_460D30`, `sub_4BF550` | checked | 9.6f magical_germain helpers use a different layout (glyph v1/v2 at 0x24/0x25); no clear 10.5 open-coded form, left |
| 0x5D8E70 | `UpdateWinXY` | 0x4CC580 | `sub_4CB520` | checked | sub_4CB520 is IsStartModeFullScreen (called) |
| 0x5DCE40 | `Weapon::add_ammo_capped` | 0x4CCB70 | ✓ `Weapon_30::sub_4A4FA0` | done | all 9.6f inlines used |
| 0x5DCF60 | `Weapon::spawn_bullet` | 0x4CDA90 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `Car_3C::set_xyz_lazy_420600` (10.5 0x59FA40), ✓ `Car_3C::set_ang_lazy_420690`, ✓ `sub_416B40`, ✓ `Car_3C::SetType_4206F0`, ✓ `sub_482A30`, ✓ `sub_420B50`, `sub_482790`, `sub_483C20`, `sub_4BD670` | checked | 482790/483C20/4BD670 = SetDamageOwner/SetSpriteIdxOffset/CheckSpriteMovementRegion, all called in 10.5 (noise) |
| 0x5DD0F0 | `Weapon::flamethrower` | 0x4CDC20 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `SetFromPolar_41E210`, ✓ `Weapon_30::sub_4CCA80`, `sub_48D4E0`, ✓ `sub_4CCA60`, ✓ `sub_41B0A0` | done | diff 63->0 CANDIDATE MATCH |
| 0x5DD860 | `Weapon::pistol` | 0x4CE070 | ✓ `Weapon_30::sub_4CCA80`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `SetFromPolar_41E210`, ✓ `Weapon_30::sub_4CCA30`, ✓ `sub_41B0A0`, `sub_4CD000`, ✓ `sub_4CCA90` | done | diff 58 unchanged; 4CD000 = ApplyFastReload (10.5 calls it twice, once per branch) |
| 0x5DDA70 | `Weapon::dual_pistol` | 0x4CE270 | ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `cool_nash_0x294::get_cam_y_403A10` (10.5 0x4086A0), ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `SetFromPolar_41E210`, ✓ `Weapon_30::sub_4CCA80`, ✓ `Weapon_30::sub_4CCA30`, ✓ `sub_41B0A0`, `sub_4CD000` | checked | 4CD000 = ApplyFastReload (noise) |
| 0x5DDD20 | `Weapon::smg` | 0x4CE4B0 | ✓ `Weapon_30::sub_4CCA80`, ✓ `sub_40F6B0`, ✓ `cool_nash_0x294::get_cam_z_416B50`, ✓ `cool_nash_0x294::get_cam_x_403A00`, ✓ `sub_4CCA60`, ✓ `sub_41B0A0`, `sub_4CD000`, ✓ `sub_4CCA90` | done | diff 117->87 |
| 0x5DE910 | `DrawElectricArc` | 0x4CCBD0 | ✓ `sub_40F6B0`, `sub_40F680`, `sub_40F600`, ✓ `sub_40F790`, ✓ `sub_40F540`, `SetFromPolar_41E210`, ✓ `sub_48A250`, `sub_48DDC0`, `sub_48A270` | todo |  |
| 0x5DFB60 | `Weapon::ChainShockTargets` | 0x4CF380 | `SpriteList_4::ctor_424620`, `sub_435C20` (10.5 0x408680), `sub_4BEE10`, ✓ `sub_416B40`, `sub_40E8D0`, ✓ `MaxAbsDistance_42A6B0` (10.5 0x4614E0), ✓ `sub_4207B0`, ✓ `SetAttacker_433BF0`, ✓ `sub_41B0A0`, ✓ `sub_425770`, ✓ `Weapon_30::sub_4CCA80`, ✓ `rng::get_cur_rng_41CFE0`, ✓ `sub_4CCA60` | done | diff 465 unchanged; most listed are noise |
| 0x5E2550 | `Weapon::car_mine` | 0x4CD5F0 | ✓ `sub_4118B0`, `sub_41FC70`, ✓ `sub_447E10`, ✓ `sub_40F6B0`, ✓ `sub_4207B0`, `sub_40F680`, `sub_492170`, `sub_469DC0`, `sub_420B50`, `sub_482790`, ✓ `Weapon_30::sub_4CCA30`, ✓ `Weapon_30::sub_4CCA80` | todo |  |
| 0x5E3C10 | `WeaponManager::allocate_ped_weapon` | 0x4CD770 | ✓ `sub_4CC9E0`, ✓ `sub_4CCA00`, ✓ `sub_4CCA10` | done | code unchanged; new WeaponPool::AllocateUnlinked |
| 0x5E3CE0 | `WeaponManager::allocate_car_weapon` | 0x4CD7B0 | ✓ `sub_4CC9C0`, ✓ `sub_4CCA00`, ✓ `sub_4CCA20` | checked | already uses WeaponPool::Allocate (0x4CC9C0, address noted in WeaponManager.hpp) |
| 0x5E3D20 | `WeaponManager::find` | 0x4CD7F0 | ✓ `Weapon_30_Pool::sub_4CC9B0` | done | code unchanged |
| 0x5E3E90 | `WeaponManager::ctor_5E3E90` | 0x4D0740 | `SpriteList_4::ctor_424620`, `Weapon_30_Pool::ctor_4CDA20`, ✓ `SpriteList_4::ResetHead_4207E0` | checked | SpriteList ctor and the pool ctor are the member init / new expression; matching as is |
| 0x5E4EE0 | `WindowProc` | 0x4D0A00 | `unknown_libname_18` (10.5 0x40EF10), `sub_4D09D0`, `sub_481DC0`, `sub_481D80`, `sub_481DA0` | checked | 4D09D0=Frontend::SetInputEnabled, 481DC0/481D80/481DA0 = Bink 513760/5136D0/513720, all called in 10.5 |
| 0x5E53F0 | `GameWinMain` | 0x4D1170 | `sub_45E8D0`, `unknown_libname_18` (10.5 0x40EF10), `Game_0x40::sub_45ACF0`, ✓ `Game_0x40::get_main_state_4D09C0` | checked | sub_45E8D0 (837 bytes) / sub_45ACF0 (returns 1) pair with 10.5 code not open-coded here |
| 0x5E5A30 | `Start_NetworkGame` | 0x4D0ED0 | `sub_409F90`, ✓ `sub_409C40`, `sub_405A40` | done | GetPlayerIdx (57->45); 409F90=CountActivePlayers; 405A40 is a vtable store (vdtor) |

<!-- table inlines -->
| 9.6f | 9.6f name | Size | 10.5 copy | Noted in Source | WIP/MATCH users | Status | Notes |
|---|---|---|---|---|---|---|---|
| 0x401180 | `sub_401180` | 316 | 0x401180 unmarked |  | 0/0 | todo |  |
| 0x401CF0 | `array_constuctor_401CF0` | 42 |  |  | 0/6 | todo |  |
| 0x402B20 | `sub_402B20` | 253 | 0x4A6DB0 MATCH |  | 0/1 | todo |  |
| 0x403890 | `PedPool::sub_403890` | 34 |  | Source/Char_Pool.hpp:127 | 0/11 | todo |  |
| 0x4038E0 | `?MarkRootRemoved@VirtualProcessorRoot@details@Concurrency@@QAEXXZ` | 5 |  | Source/PedGroup.hpp:11 | 0/1 | todo |  |
| 0x4038F0 | `sub_4038F0` | 4 |  | Source/PedGroup.hpp:17 | 0/1 | todo |  |
| 0x403900 | `sub_403900` | 7 |  | Source/char.hpp:293 | 0/2 | todo |  |
| 0x403920 | `cool_nash_0x294::sub_403920` | 13 |  | Source/Ped.hpp:525 | 0/33 | todo |  |
| 0x403940 | `cool_nash_0x294::sub_403940` | 13 |  | Source/Ped.hpp:454 | 0/3 | todo |  |
| 0x403950 | `sub_403950` | 15 |  | Source/Ped.hpp:496 | 0/4 | todo |  |
| 0x403960 | `sub_403960` | 15 |  | Source/Ped.hpp:507 | 0/7 | todo |  |
| 0x403970 | `cool_nash_0x294::set_occupation_403970` | 13 |  | Source/Ped.hpp:659 | 0/31 | todo |  |
| 0x403980 | `cool_nash_0x294::get_occupation_403980` | 7 |  | Source/Ped.hpp:665 | 0/23 | todo |  |
| 0x403990 | `cool_nash_0x294::sub_403990` | 7 |  | Source/Ped.hpp:472 | 0/25 | todo |  |
| 0x4039A0 | `cool_nash_0x294::set_health_4039A0` | 15 |  | Source/Ped.hpp:647 | 0/22 | todo |  |
| 0x4039B0 | `cool_nash_0x294::set_enter_car_as_passenger_4039B0` | 13 |  | Source/Ped.hpp:41 | 0/9 | todo |  |
| 0x4039C0 | `cool_nash_0x294::get_enter_car_as_passenger_4039C0` | 7 |  | Source/Ped.hpp:47 | 0/1 | todo |  |
| 0x4039D0 | `cool_nash_0x294::sub_4039D0` | 7 |  | Source/Ped.hpp:53 | 0/2 | todo |  |
| 0x4039E0 | `cool_nash_0x294::sub_4039E0` | 7 |  |  | 0/3 | todo |  |
| 0x4039F0 | `cool_nash_0x294::sub_4039F0` | 8 |  | Source/Ped.hpp:460 | 0/7 | todo |  |
| 0x403A00 | `cool_nash_0x294::get_cam_x_403A00` | 15 |  | Source/Ped.hpp:531 | 0/51 | todo |  |
| 0x403A10 | `cool_nash_0x294::get_cam_y_403A10` | 15 | 0x4086A0 MATCH | Source/Car.cpp:7020, Source/Ped.hpp:537 | 0/45 | todo |  |
| 0x403A20 | `sub_403A20` | 8 |  | Source/Ped.hpp:746 | 0/1 | todo |  |
| 0x403A30 | `cool_nash_0x294::sub_403A30` | 15 |  | Source/Ped.hpp:390 | 0/13 | todo |  |
| 0x403A40 | `cool_nash_0x294::sub_403A40` | 16 |  | Source/Ped.hpp:840 | 0/13 | todo |  |
| 0x403A50 | `sub_403A50` | 11 |  | Source/PedGroup.cpp:342 | 0/1 | todo |  |
| 0x403A60 | `cool_nash_0x294::get_target_car_door_403A60` | 7 |  | Source/Ped.hpp:635 | 0/6 | todo |  |
| 0x403A70 | `cool_nash_0x294::set_target_car_door_403A70` | 13 |  | Source/Ped.hpp:82 | 0/10 | todo |  |
| 0x403A80 | `cool_nash_0x294::get_objective_403A80` | 7 |  | Source/Ped.hpp:822 | 0/14 | todo |  |
| 0x403A90 | `cool_nash_0x294::get_car_state_403A90` | 7 |  | Source/Ped.hpp:870 | 0/11 | todo |  |
| 0x403AA0 | `cool_nash_0x294::set_target_objective_car_403AA0` | 13 |  | Source/Ped.hpp:513 | 0/18 | todo |  |
| 0x403AB0 | `cool_nash_0x294::get_target_objective_car_403AB0` | 7 |  | Source/Ped.hpp:519 | 0/3 | todo |  |
| 0x403AC0 | `cool_nash_0x294::set_objective_target_ped_403AC0` | 13 |  | Source/Ped.hpp:88 | 0/8 | todo |  |
| 0x403AD0 | `cool_nash_0x294::get_objective_target_ped_403AD0` | 7 |  | Source/Ped.hpp:94 | 0/3 | todo |  |
| 0x403AE0 | `cool_nash_0x294::sub_403AE0` | 13 |  | Source/Ped.hpp:490 | 0/12 | todo |  |
| 0x403AF0 | `cool_nash_0x294::sub_403AF0` | 7 |  | Source/Ped.hpp:876 | 0/4 | todo |  |
| 0x403B00 | `cool_nash_0x294::set_target_to_enter_403B00` | 13 |  | Source/Ped.hpp:100 | 0/6 | todo |  |
| 0x403B10 | `cool_nash_0x294::get_target_to_enter_403B10` | 7 |  | Source/Ped.hpp:828 | 0/7 | todo |  |
| 0x403B20 | `cool_nash_0x294::sub_403B20` | 8 |  | Source/Ped.hpp:106 | 0/1 | todo |  |
| 0x403B30 | `cool_nash_0x294::get_objective_timer_403B30` | 8 |  | Source/Ped.hpp:112 | 0/2 | todo |  |
| 0x403B40 | `cool_nash_0x294::sub_403B40` | 13 |  | Source/Ped.hpp:118 | 0/2 | todo |  |
| 0x403B50 | `cool_nash_0x294::sub_403B50` | 13 |  | Source/Ped.hpp:124 | 0/2 | todo |  |
| 0x403B60 | `cool_nash_0x294::sub_403B60` | 11 |  | Source/Ped.hpp:858 | 0/18 | todo |  |
| 0x403B70 | `sub_403B70` | 12 |  | Source/Ped.hpp:794 | 0/3 | todo |  |
| 0x403B80 | `cool_nash_0x294::has_car_403B80` | 12 |  | Source/Ped.hpp:466 | 0/15 | todo |  |
| 0x403B90 | `sub_403B90` | 13 | 0x4CA3E0 MATCH |  | 0/1 | todo |  |
| 0x403BA0 | `Car_BC::sub_403BA0` | 32 |  | Source/Car.hpp:810 | 0/25 | todo |  |
| 0x403BC0 | `sub_403BC0` | 22 |  | Source/Car.hpp:1202 | 0/1 | todo |  |
| 0x403D10 | `Mouze_44::sub_403D10` | 8 |  |  | 0/1 | todo |  |
| 0x403D20 | `Mouze_44::sub_403D20` | 36 |  |  | 0/1 | todo |  |
| 0x404400 | `sub_404400` | 30 |  |  | 0/7 | todo |  |
| 0x404420 | `sub_404420` | 42 |  |  | 0/6 | todo |  |
| 0x404450 | `sub_404450` | 46 |  |  | 0/1 | todo |  |
| 0x404480 | `sub_404480` | 8 |  |  | 0/1 | todo |  |
| 0x404490 | `sub_404490` | 310 |  |  | 0/1 | todo |  |
| 0x4045D0 | `sub_4045D0` | 257 | 0x4CAE80 MATCH |  | 0/1 | todo |  |
| 0x4046F0 | `sub_4046F0` | 334 |  |  | 0/1 | todo |  |
| 0x4048A0 | `sub_4048A0` | 83 |  |  | 0/1 | todo |  |
| 0x404900 | `sub_404900` | 226 |  |  | 0/2 | todo |  |
| 0x404AD0 | `sub_404AD0` | 366 |  |  | 0/1 | todo |  |
| 0x4059B0 | `Memory::malloc_4059B0` | 49 | 0x4FE4D0 MATCH |  | 0/1 | todo |  |
| 0x405A40 | `sub_405A40` | 7 |  |  | 0/2 | todo |  |
| 0x405A80 | `text_0x14::dtor_405A80` | 30 |  |  | 0/3 | todo |  |
| 0x4061C0 | `sub_4061C0` | 73 |  |  | 0/1 | todo |  |
| 0x409C40 | `sub_409C40` | 7 |  | Source/NetPlay.hpp:262 | 0/9 | todo |  |
| 0x409C50 | `sub_409C50` | 37 |  | Source/NetPlay.cpp:15 | 0/3 | todo |  |
| 0x409DA0 | `sub_409DA0` | 55 | 0x521100 MATCH |  | 0/1 | todo |  |
| 0x409F90 | `sub_409F90` | 73 |  |  | 0/2 | todo |  |
| 0x40BF80 | `sub_40BF80` | 26 |  |  | 0/1 | todo |  |
| 0x40C810 | `sub_40C810` | 74 |  |  | 0/2 | todo |  |
| 0x40CC10 | `sub_40CC10` | 176 |  |  | 0/1 | todo |  |
| 0x40CE90 | `sub_40CE90` | 9 |  | Source/RouteFinder.hpp:10 | 0/7 | todo |  |
| 0x40CEA0 | `sub_40CEA0` | 10 |  | Source/RouteFinder.hpp:22 | 0/3 | todo |  |
| 0x40CEB0 | `Enable_40CEB0` | 5 |  | Source/RouteFinder.hpp:34 | 0/1 | todo |  |
| 0x40CEC0 | `Disable_40CEC0` | 5 |  | Source/RouteFinder.hpp:28 | 0/1 | todo |  |
| 0x40CED0 | `sub_40CED0` | 9 |  | Source/RouteFinder.hpp:16 | 0/2 | todo |  |
| 0x40CEE0 | `sub_40CEE0` | 59 |  | Source/CarAI.cpp:375, Source/RouteFinder.hpp:50 | 0/1 | todo |  |
| 0x40CF20 | `ContainsPoint_40CF20` | 59 |  | Source/RouteFinder.hpp:70 | 0/2 | todo |  |
| 0x40CF60 | `DrawUnk_0xBC::sub_40CF60` | 89 |  | Source/Camera.hpp:148 | 0/3 | todo |  |
| 0x40CFC0 | `DrawUnk_0xBC::WorldToScreen_40CFC0` | 245 |  | Source/Camera.hpp:117, Source/Hud.cpp:87 | 0/2 | todo |  |
| 0x40D690 | `sub_40D690` | 320 | 0x589480 MATCH |  | 0/1 | todo |  |
| 0x40DFA0 | `sub_40DFA0` | 67 | 0x589E20 MATCH |  | 0/1 | todo |  |
| 0x40E030 | `sub_40E030` | 83 | 0x58A190 MATCH |  | 0/1 | todo |  |
| 0x40E0F0 | `sub_40E0F0` | 5 |  |  | 0/1 | todo |  |
| 0x40E790 | `WrapAngle_40E790` | 126 |  | Source/Car.cpp:5413, Source/Cranes.cpp:56 | 0/4 | todo |  |
| 0x40E8D0 | `sub_40E8D0` | 406 |  |  | 0/15 | todo |  |
| 0x40EAB0 | `sub_40EAB0` | 81 |  |  | 0/8 | todo |  |
| 0x40EC80 | `sub_40EC80` | 41 |  |  | 0/2 | todo |  |
| 0x40EE60 | `sub_40EE60` | 277 |  |  | 0/1 | todo |  |
| 0x40F0A0 | `sub_40F0A0` | 609 |  |  | 0/1 | todo |  |
| 0x40F490 | `sub_40F490` | 27 |  | Source/Car.hpp:1466 | 0/2 | todo |  |
| 0x40F540 | `sub_40F540` | 59 |  | Source/ang16.hpp:223, Source/ang16.hpp:269 | 0/14 | todo |  |
| 0x40F580 | `sub_40F580` | 36 |  | Source/ang16.hpp:254 | 0/13 | todo |  |
| 0x40F600 | `sub_40F600` | 60 |  |  | 0/18 | todo |  |
| 0x40F640 | `sub_40F640` | 53 | 0x43D5D0 MATCH | Source/Car.cpp:1518 | 0/4 | todo |  |
| 0x40F680 | `sub_40F680` | 33 |  | Source/CarPhysics.cpp:2886 | 0/11 | todo |  |
| 0x40F6B0 | `sub_40F6B0` | 166 |  | Source/Fix16_Point.hpp:63, Source/Fix16_Point.hpp:221 | 0/23 | todo |  |
| 0x40F790 | `sub_40F790` | 22 |  | Source/Car.cpp:1518, Source/Fix16_Point.hpp:126 | 0/8 | todo |  |
| 0x40F7B0 | `sub_40F7B0` | 10 |  | Source/sprite.hpp:624 | 0/12 | todo |  |
| 0x40F7C0 | `sub_40F7C0` | 18 |  |  | 0/1 | todo |  |
| 0x40F7E0 | `sub_40F7E0` | 18 |  | Source/CarPhysics.hpp:103 | 0/1 | todo |  |
| 0x40F800 | `sub_40F800` | 12 |  | Source/CarPhysics.hpp:79 | 0/1 | todo |  |
| 0x40F810 | `sub_40F810` | 10 |  | Source/CarPhysics.hpp:85 | 0/1 | todo |  |
| 0x40F820 | `sub_40F820` | 14 |  | Source/CarPhysics.hpp:91 | 0/2 | todo |  |
| 0x40F830 | `sub_40F830` | 12 | 0x407B80 MATCH | Source/CarPhysics.hpp:97 | 0/1 | todo |  |
| 0x40F840 | `sub_40F840` | 80 |  | Source/Car.cpp:6651, Source/CarPhysics.hpp:246 | 0/4 | todo |  |
| 0x40F890 | `IsMaxDamage_40F890` | 10 |  | Source/Car.hpp:831 | 0/28 | todo |  |
| 0x40FB70 | `sub_40FB70` | 102 |  |  | 0/1 | todo |  |
| 0x40FD40 | `sub_40FD40` | 234 |  |  | 0/1 | todo |  |
| 0x40FE60 | `root_sound::DestroySoundObj_40FE60` | 27 |  | Source/root_sound.hpp:14 | 0/2 | todo |  |
| 0x40FE80 | `sub_40FE80` | 24 |  | Source/sprite.hpp:393 | 0/1 | todo |  |
| 0x40FEA0 | `sub_40FEA0` | 13 |  | Source/sprite.hpp:365 | 0/23 | todo |  |
| 0x40FEB0 | `sub_40FEB0` | 13 |  | Source/sprite.hpp:352 | 0/35 | todo |  |
| 0x40FEC0 | `sub_40FEC0` | 21 |  | Source/sprite.hpp:399 | 0/21 | todo |  |
| 0x40FEE0 | `SetSprite_40FEE0` | 16 |  | Source/CollisionSoundQueue.hpp:78 | 0/3 | todo |  |
| 0x40FEF0 | `sub_40FEF0` | 4 |  | Source/Object_5C.hpp:252 | 0/6 | todo |  |
| 0x40FF00 | `sub_40FF00` | 7 |  | Source/Object_5C.hpp:258 | 0/1 | todo |  |
| 0x40FF10 | `SetImpactStrength_40FF10` | 10 |  | Source/CollisionSoundQueue.hpp:107 | 0/3 | todo |  |
| 0x4102A0 | `sub_4102A0` | 198 | 0x40BD10 MATCH |  | 0/1 | todo |  |
| 0x410460 | `sub_410460` | 19 |  |  | 0/1 | todo |  |
| 0x410480 | `sub_410480` | 34 |  |  | 0/1 | todo |  |
| 0x410550 | `sub_410550` | 10 |  |  | 0/2 | todo |  |
| 0x410580 | `unknown_libname_9` | 10 |  |  | 0/1 | todo |  |
| 0x410590 | `unknown_libname_10` | 10 |  |  | 0/1 | todo |  |
| 0x4105B0 | `sub_4105B0` | 10 |  |  | 0/10 | todo |  |
| 0x410670 | `unknown_libname_18` | 10 | 0x40EF10 unmarked |  | 0/6 | todo |  |
| 0x410730 | `root_sound::sub_410730` | 19 |  | Source/root_sound.hpp:24 | 0/1 | todo |  |
| 0x410BF0 | `sub_410BF0` | 17 |  | Source/fix16.hpp:24 | 0/4 | todo |  |
| 0x4116B0 | `sub_4116B0` | 64 | 0x419020 MATCH |  | 0/1 | todo |  |
| 0x4116F0 | `sub_4116F0` | 52 | 0x419070 MATCH |  | 0/1 | todo |  |
| 0x411730 | `sub_411730` | 118 |  |  | 0/5 | todo |  |
| 0x4117B0 | `sub_4117B0` | 30 |  | Source/sprite.hpp:332 | 0/3 | todo |  |
| 0x411810 | `CarPhysics_B0::is_backward_gas_on_411810` | 7 |  | Source/CarPhysics.hpp:384 | 0/2 | todo |  |
| 0x411870 | `sub_411870` | 24 |  |  | 0/1 | todo |  |
| 0x411890 | `sub_411890` | 17 |  |  | 0/1 | todo |  |
| 0x4118B0 | `sub_4118B0` | 4 |  | Source/Car.hpp:758 | 0/8 | todo |  |
| 0x4118C0 | `Car_BC::sub_4118C0` | 11 |  | Source/Car.hpp:482 | 0/3 | todo |  |
| 0x4118D0 | `sub_4118D0` | 23 |  | Source/Car.hpp:741 | 0/9 | todo |  |
| 0x4118F0 | `sub_4118F0` | 11 |  | Source/Car.hpp:1214 | 0/9 | todo |  |
| 0x411900 | `Car_BC::sub_411900` | 11 |  | Source/Car.hpp:1190 | 0/8 | todo |  |
| 0x411910 | `Car_BC::sub_411910` | 11 |  | Source/Car.hpp:1184 | 0/6 | todo |  |
| 0x411920 | `sub_411920` | 11 |  | Source/Car.hpp:1232 | 0/7 | todo |  |
| 0x411930 | `Car_BC::IsFlagSet_411930` | 15 |  | Source/Car.hpp:719 | 0/5 | todo |  |
| 0x411940 | `sub_411940` | 7 |  | Source/Car.hpp:859 | 0/16 | todo |  |
| 0x411950 | `sub_411950` | 17 |  | Source/Car.hpp:851 | 0/1 | todo |  |
| 0x411970 | `sub_411970` | 20 |  | Source/Car.hpp:1300 | 0/4 | todo |  |
| 0x411990 | `sub_411990` | 17 |  | Source/Car.hpp:1306 | 0/1 | todo |  |
| 0x4119D0 | `sub_4119D0` | 17 |  | Source/Car.hpp:843 | 0/1 | todo |  |
| 0x4119F0 | `?_GetResult@?$_Task_impl@_N@details@Concurrency@@QAE_NXZ` | 7 |  | Source/Car.hpp:1208 | 0/1 | todo |  |
| 0x411A00 | `sub_411A00` | 7 |  | Source/Cranes.hpp:68 | 0/2 | todo |  |
| 0x411A10 | `sub_411A10` | 4 |  | Source/Crushers.hpp:26 | 0/1 | todo |  |
| 0x411A20 | `unknown_libname_20` | 12 |  | Source/Crushers.hpp:14 | 0/1 | todo |  |
| 0x411A30 | `sub_411A30` | 12 |  | Source/Crushers.hpp:20 | 0/1 | todo |  |
| 0x411A40 | `sub_411A40` | 3 |  | Source/Hud.hpp:313 | 0/1 | todo |  |
| 0x412680 | `sub_412680` | 428 |  |  | 0/1 | todo |  |
| 0x412830 | `sub_412830` | 416 |  |  | 0/1 | todo |  |
| 0x412A20 | `sub_412A20` | 426 |  | Source/sound_obj.cpp:1633 | 0/1 | todo |  |
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
| 0x414F20 | `HasEmergencyLights_414F20` | 49 |  | Source/Car.hpp:865 | 0/5 | todo |  |
| 0x414F60 | `IsBulletproof_414F60` | 11 |  |  | 0/1 | todo |  |
| 0x414F70 | `sub_414F70` | 11 |  | Source/Car.hpp:735 | 0/2 | todo |  |
| 0x414F80 | `AreEmergencyLightsOn_414F80` | 29 |  | Source/Car.hpp:837 | 0/1 | todo |  |
| 0x414FA0 | `sub_414FA0` | 235 |  |  | 0/1 | todo |  |
| 0x4156B0 | `sub_4156B0` | 5 |  |  | 0/1 | todo |  |
| 0x4156C0 | `sub_4156C0` | 443 |  |  | 0/1 | todo |  |
| 0x415A90 | `sub_415A90` | 336 |  |  | 0/1 | todo |  |
| 0x4166E0 | `sub_4166E0` | 103 | 0x41A650 MATCH |  | 0/1 | todo |  |
| 0x4168E0 | `sub_4168E0` | 92 |  |  | 0/1 | todo |  |
| 0x416940 | `sub_416940` | 18 |  |  | 0/2 | todo |  |
| 0x416B40 | `sub_416B40` | 4 |  | Source/sprite.hpp:346 | 0/26 | todo |  |
| 0x416B50 | `cool_nash_0x294::get_cam_z_416B50` | 15 |  | Source/Ped.hpp:543 | 0/35 | todo |  |
| 0x416B60 | `cool_nash_0x294::get_car_416B60` | 7 |  | Source/Ped.hpp:574 | 0/25 | todo |  |
| 0x416B70 | `sub_416B70` | 4 |  | Source/Camera.hpp:49 | 0/1 | todo |  |
| 0x416B80 | `Car_BC::sub_416B80` | 39 |  | Source/Car.hpp:1349 | 0/1 | todo |  |
| 0x416BB0 | `sub_416BB0` | 16 |  | Source/Car.hpp:1357 | 0/2 | todo |  |
| 0x416BC0 | `sub_416BC0` | 7 |  | Source/Game.hpp:48 | 0/2 | todo |  |
| 0x416C00 | `ctor_416C00` | 3 |  | Source/sound_obj.hpp:27 | 0/1 | todo |  |
| 0x416E50 | `sub_416E50` | 193 |  |  | 0/1 | todo |  |
| 0x416F20 | `sub_416F20` | 262 |  |  | 0/1 | todo |  |
| 0x4171A0 | `sub_4171A0` | 178 | 0x41A850 MATCH |  | 0/1 | todo |  |
| 0x41B030 | `sub_41B030` | 45 |  |  | 0/1 | todo |  |
| 0x41B080 | `sub_41B080` | 12 | 0x41B480 unmarked | Source/char.hpp:43 | 0/4 | todo |  |
| 0x41B090 | `sub_41B090` | 4 |  | Source/char.hpp:61 | 0/1 | todo |  |
| 0x41B0A0 | `sub_41B0A0` | 12 |  | Source/Ped.hpp:603 | 0/28 | todo |  |
| 0x41B0B0 | `sub_41B0B0` | 17 |  | Source/Ped.hpp:618 | 0/1 | todo |  |
| 0x41C0F0 | `nullsub_4` | 1 | 0x426670 unmarked |  | 0/1 | todo |  |
| 0x41C1F0 | `sub_41C1F0` | 5 |  | Source/sprite.hpp:198 | 0/4 | todo |  |
| 0x41CC70 | `sub_41CC70` | 4 |  | Source/Weapon.hpp:169 | 0/2 | todo |  |
| 0x41CC80 | `sub_41CC80` | 8 |  | Source/Weapon.hpp:137 | 0/1 | todo |  |
| 0x41CC90 | `sub_41CC90` | 4 |  | Source/Weapon.hpp:157 | 0/2 | todo |  |
| 0x41CFE0 | `rng::get_cur_rng_41CFE0` | 3 |  | Source/rng.hpp:14 | 0/24 | todo |  |
| 0x41CFF0 | `angle::angle_not_equal_41CFF0` | 20 |  | Source/ang16.hpp:33 | 0/3 | todo |  |
| 0x41D020 | `sub_41D020` | 7 |  | Source/Player.hpp:181 | 0/8 | todo |  |
| 0x41D070 | `Car_14_18::ctor_41D070` | 3 |  | Source/Fix16_Rect.hpp:41 | 0/1 | todo |  |
| 0x41D580 | `sub_41D580` | 160 |  |  | 0/1 | todo |  |
| 0x41DC30 | `RollingDigitCounter_38::sub_41DC30` | 3 |  | Source/RollingDigitCounter.hpp:41 | 0/4 | todo |  |
| 0x41DC40 | `sub_41DC40` | 35 |  | Source/Player.hpp:130 | 0/16 | todo |  |
| 0x41DC70 | `angry_lewin_0x85C::sub_41DC70` | 3 |  | Source/Player.hpp:299 | 0/26 | todo |  |
| 0x41E130 | `Max_41E130` | 44 |  | Source/fix16.hpp:351 | 0/3 | todo |  |
| 0x41E210 | `SetFromPolar_41E210` | 76 |  | Source/Fix16_Point.hpp:55, Source/Fix16_Point.hpp:175 | 0/10 | todo |  |
| 0x41E390 | `sub_41E390` | 40 |  | Source/sprite.hpp:51 | 0/4 | todo |  |
| 0x41E3D0 | `sub_41E3D0` | 51 |  | Source/Camera.hpp:172 | 0/3 | todo |  |
| 0x41E410 | `DrawUnk_0xBC::CommitCameraTarget_41E410` | 27 |  | Source/Camera.hpp:155 | 0/5 | todo |  |
| 0x41E430 | `Car_BC::sub_41E430` | 15 |  | Source/Car.hpp:1282 | 0/12 | todo |  |
| 0x41E440 | `Car_BC::sub_41E440` | 15 |  | Source/Car.hpp:1288 | 0/12 | todo |  |
| 0x41E450 | `Car_BC::sub_41E450` | 15 |  | Source/Car.hpp:1294 | 0/13 | todo |  |
| 0x41E460 | `Car_BC::has_trailer_41E460` | 18 |  |  | 0/4 | todo |  |
| 0x41E4E0 | `sub_41E4E0` | 34 |  |  | 0/1 | todo |  |
| 0x41E510 | `sub_41E510` | 34 |  |  | 0/1 | todo |  |
| 0x41E540 | `sub_41E540` | 53 |  |  | 0/1 | todo |  |
| 0x41E580 | `sub_41E580` | 148 |  |  | 0/1 | todo |  |
| 0x41EAD0 | `DrawUnk_0xBC::sub_41EAD0` | 34 |  |  | 0/1 | todo |  |
| 0x41EB00 | `sub_41EB00` | 101 |  |  | 0/1 | todo |  |
| 0x41F180 | `DrawUnk_0xBC::sub_41F180` | 368 |  |  | 0/1 | todo |  |
| 0x41F880 | `Car_BC::sub_41F880` | 21 | 0x43B360 MATCH |  | 0/1 | todo |  |
| 0x41FAC0 | `sub_41FAC0` | 137 |  |  | 0/1 | todo |  |
| 0x41FC20 | `PolarToCartesian_41FC20` | 79 |  | Source/ang16.hpp:246 | 0/20 | todo |  |
| 0x41FC70 | `sub_41FC70` | 18 |  |  | 0/2 | todo |  |
| 0x41FC90 | `RotateVector_41FC90` | 177 |  | Source/ang16.hpp:362 | 0/2 | todo |  |
| 0x41FE40 | `sub_41FE40` | 35 |  | Source/Car.cpp:218 | 0/3 | todo |  |
| 0x41FE70 | `SignedPixelsToFix16_41FE70` | 48 |  | Source/CarInfoTable.hpp:22 | 0/2 | todo |  |
| 0x41FEA0 | `sub_41FEA0` | 11 |  | Source/gtx_0x106C.hpp:127 | 0/1 | todo |  |
| 0x41FF00 | `sub_41FF00` | 11 |  | Source/gtx_0x106C.hpp:140 | 0/1 | todo |  |
| 0x420200 | `gtx_0x106C::sub_420200` | 22 |  | Source/gtx_0x106C.hpp:405 | 0/3 | todo |  |
| 0x420220 | `sub_420220` | 16 |  | Source/gtx_0x106C.hpp:279 | 0/2 | todo |  |
| 0x420360 | `sub_420360` | 45 |  | Source/Fix16_Point.hpp:97 | 0/4 | todo |  |
| 0x420390 | `sub_420390` | 135 |  |  | 0/9 | todo |  |
| 0x420420 | `sub_420420` | 38 |  | Source/map_0x370.hpp:688 | 0/14 | todo |  |
| 0x4204D0 | `sub_4204D0` | 183 |  | Source/Fix16_Rect.hpp:46 | 0/2 | todo |  |
| 0x420590 | `sub_420590` | 11 |  | Source/sprite.hpp:95 | 0/1 | todo |  |
| 0x4205A0 | `Car_3C::ctor_4205A0` | 85 |  |  | 0/1 | todo |  |
| 0x420600 | `Car_3C::set_xyz_lazy_420600` | 89 | 0x59FA40 MATCH | Source/sprite.hpp:563, Source/sprite.hpp:575 | 0/30 | todo |  |
| 0x420660 | `sub_420660` | 41 |  | Source/sprite.hpp:599 | 0/5 | todo |  |
| 0x420690 | `Car_3C::set_ang_lazy_420690` | 36 |  | Source/sprite.hpp:553 | 0/20 | todo |  |
| 0x4206C0 | `sub_4206C0` | 23 |  | Source/sprite.hpp:512 | 0/24 | todo |  |
| 0x4206E0 | `sub_4206E0` | 12 |  | Source/sprite.hpp:522 | 0/2 | todo |  |
| 0x4206F0 | `Car_3C::SetType_4206F0` | 15 |  | Source/sprite.hpp:487 | 0/18 | todo |  |
| 0x420700 | `sub_420700` | 8 |  | Source/sprite.hpp:340 | 0/3 | todo |  |
| 0x420710 | `sub_420710` | 10 |  | Source/sprite.hpp:500 | 0/1 | todo |  |
| 0x420760 | `sub_420760` | 46 |  |  | 0/1 | todo |  |
| 0x4207B0 | `sub_4207B0` | 26 |  | Source/Car.cpp:4018, Source/sprite.cpp:156 | 0/17 | todo |  |
| 0x4207E0 | `SpriteList_4::ResetHead_4207E0` | 7 |  | Source/Object_3C.hpp:24 | 0/3 | todo |  |
| 0x420B50 | `sub_420B50` | 7 |  | Source/Ped.hpp:384 | 0/5 | todo |  |
| 0x420B60 | `sub_420B60` | 7 |  | Source/Ped.hpp:580 | 0/7 | todo |  |
| 0x420B70 | `sub_420B70` | 7 |  | Source/Ped.hpp:397 | 0/7 | todo |  |
| 0x420B80 | `cool_nash_0x294::sub_420B80` | 10 |  | Source/Ped.hpp:752 | 0/6 | todo |  |
| 0x420C30 | `sub_420C30` | 11 |  | Source/Car.cpp:2831 | 0/2 | todo |  |
| 0x420D90 | `sub_420D90` | 7 |  | Source/BitSet32.hpp:133 | 0/1 | todo |  |
| 0x420DC0 | `maybe_flags::set_420DC0` | 20 |  | Source/BitSet32.hpp:71, Source/Car_10.cpp:12 | 0/4 | todo |  |
| 0x420DE0 | `maybe_flags::clear_420DE0` | 26 |  | Source/BitSet32.hpp:65, Source/Car_10.cpp:12 | 0/6 | todo |  |
| 0x420E20 | `sub_420E20` | 27 |  | Source/BitSet32.hpp:84 | 0/2 | todo |  |
| 0x420E50 | `sub_420E50` | 4 |  | Source/Car.hpp:1478 | 0/1 | todo |  |
| 0x420E90 | `Ped_Unknown_4::ClearList_420E90` | 7 |  | Source/Ped_List.hpp:42 | 0/3 | todo |  |
| 0x420EA0 | `Char_8::sub_420EA0` | 7 |  | Source/Char_Pool.hpp:31, Source/Ped_List.hpp:48 | 0/5 | todo |  |
| 0x420EB0 | `CarAI_78_Pool::ctor_420EB0` | 61 |  | Source/CarAI.hpp:143 | 0/1 | todo |  |
| 0x420F10 | `sub_420F10` | 15 |  | Source/PedRefTable.hpp:31 | 0/11 | todo |  |
| 0x420F20 | `Car_BC_Pool::sub_420F20` | 16 |  | Source/Car.hpp:1484 | 0/3 | todo |  |
| 0x420F30 | `UnlinkFromActiveList_420F30` | 69 |  | Source/Pool.hpp:292 | 0/4 | todo |  |
| 0x420F80 | `CarPhyisicsPool::ctor_420F80` | 74 |  |  | 0/1 | todo |  |
| 0x420FF0 | `sub_420FF0` | 4 |  | Source/Object_5C.hpp:228 | 0/10 | todo |  |
| 0x421000 | `SpritePool::sub_421000` | 19 |  | Source/sprite.hpp:812 | 0/11 | todo |  |
| 0x421020 | `sub_421020` | 4 |  |  | 0/1 | todo |  |
| 0x421030 | `SpritePool::sub_421030` | 27 |  | Source/sprite.hpp:818 | 0/5 | todo |  |
| 0x421050 | `sub_421050` | 4 |  | Source/Object_5C.hpp:234 | 0/5 | todo |  |
| 0x421060 | `check_is_shop_421060` | 32 |  | Source/Object_5C.hpp:271 | 0/6 | todo |  |
| 0x421080 | `sub_421080` | 47 |  | Source/Object_5C.hpp:200 | 0/5 | todo |  |
| 0x421130 | `sub_421130` | 24 |  | Source/CarPhysics.hpp:130 | 0/1 | todo |  |
| 0x421150 | `sub_421150` | 18 |  |  | 0/1 | todo |  |
| 0x421170 | `sub_421170` | 18 |  | Source/CarPhysics.hpp:110 | 0/1 | todo |  |
| 0x4211A0 | `sub_4211A0` | 20 |  | Source/CarPhysics.hpp:369 | 0/5 | todo |  |
| 0x4211C0 | `sub_4211C0` | 17 |  | Source/CarPhysics.hpp:390 | 0/3 | todo |  |
| 0x4211E0 | `sub_4211E0` | 20 |  | Source/CarPhysics.hpp:124 | 0/1 | todo |  |
| 0x421210 | `sub_421210` | 28 |  | Source/CarPhysics.hpp:439 | 0/3 | todo |  |
| 0x421250 | `sub_421250` | 8 |  | Source/CarPhysics.hpp:142 | 0/1 | todo |  |
| 0x421260 | `sub_421260` | 8 |  | Source/CarPhysics.hpp:136, Source/CarPhysics.hpp:378 | 0/3 | todo |  |
| 0x421270 | `sub_421270` | 7 |  | Source/CarPhysics.hpp:148 | 0/1 | todo |  |
| 0x4212A0 | `sub_4212A0` | 11 |  | Source/CarPhysics.hpp:396 | 0/4 | todo |  |
| 0x4212B0 | `sub_4212B0` | 11 |  | Source/CarPhysics.hpp:402 | 0/3 | todo |  |
| 0x4212D0 | `sub_4212D0` | 14 |  | Source/Car.hpp:129, Source/Car.hpp:369 | 0/3 | todo |  |
| 0x421340 | `sub_421340` | 8 |  | Source/Car_10.hpp:31 | 0/1 | todo |  |
| 0x421360 | `sub_421360` | 18 |  | Source/Car_10.hpp:25 | 0/1 | todo |  |
| 0x421380 | `Car_10::set_obj_421380` | 10 |  | Source/Car_10.hpp:49 | 0/9 | todo |  |
| 0x4213D0 | `SetBrakeLightDeltas_4213D0` | 96 |  | Source/Car.hpp:1034 | 0/2 | todo |  |
| 0x421430 | `ClearBrakeLightDeltas_421430` | 48 |  | Source/Car.hpp:1027 | 0/1 | todo |  |
| 0x421460 | `Car_BC::sub_421460` | 8 |  | Source/Car.hpp:1103 | 0/2 | todo |  |
| 0x421470 | `MarkForDespawn_421470` | 32 |  | Source/Car.hpp:1157 | 0/14 | todo |  |
| 0x421490 | `DespawnSoon_421490` | 20 |  | Source/Car.hpp:976, Source/Car.hpp:1148 | 0/3 | todo |  |
| 0x4214B0 | `IsMarkedForDespawn_4214B0` | 27 |  | Source/Car.hpp:1091 | 0/4 | todo |  |
| 0x4214D0 | `IsDeactivated_4214D0` | 11 |  | Source/Car.hpp:1166 | 0/5 | todo |  |
| 0x4214E0 | `sub_4214E0` | 11 |  | Source/Car.hpp:1172 | 0/1 | todo |  |
| 0x4214F0 | `Car_BC::HasNonPlayerDriver_4214F0` | 23 |  | Source/Car.hpp:1370 | 0/1 | todo |  |
| 0x421510 | `AllocateAI_421510` | 26 |  | Source/Car.hpp:413, Source/PublicTransport.cpp:111 | 0/9 | todo |  |
| 0x421530 | `sub_421530` | 8 |  | Source/Car.hpp:422 | 0/1 | todo |  |
| 0x421540 | `sub_421540` | 8 |  | Source/Car.hpp:428 | 0/8 | todo |  |
| 0x421550 | `sub_421550` | 8 |  | Source/Car.hpp:434 | 0/8 | todo |  |
| 0x421560 | `Car_BC::SetUniNum_421560` | 16 |  | Source/Car.hpp:817 | 0/26 | todo |  |
| 0x421570 | `ResetEngineStatus_421570` | 27 |  | Source/Car.hpp:1130 | 0/1 | todo |  |
| 0x421590 | `SetEngineDamaged_421590` | 20 |  | Source/Car.hpp:1139 | 0/1 | todo |  |
| 0x4215B0 | `sub_4215B0` | 11 |  | Source/Car.hpp:1317 | 0/27 | todo |  |
| 0x4215C0 | `HasPlayerDriver_4215C0` | 33 |  | Source/Car.hpp:1256 | 0/2 | todo |  |
| 0x4215F0 | `CancelDespawn_4215F0` | 32 |  | Source/Car.hpp:1109 | 0/2 | todo |  |
| 0x421610 | `sub_421610` | 11 |  | Source/Car.hpp:488 | 0/1 | todo |  |
| 0x421640 | `sub_421640` | 25 |  | Source/Car.hpp:883 | 0/3 | todo |  |
| 0x421680 | `inline_check_0x40_info_421680` | 25 |  | Source/Car.hpp:895 | 0/1 | todo |  |
| 0x4216A0 | `sub_4216A0` | 25 |  | Source/Car.hpp:901 | 0/2 | todo |  |
| 0x4216C0 | `Car_BC::inline_check_0x20_info_4216C0` | 25 |  | Source/Car.hpp:889 | 0/2 | todo |  |
| 0x4216E0 | `CanCollideOver_4216E0` | 25 |  | Source/Car.hpp:1363 | 0/3 | todo |  |
| 0x421700 | `inline_check_0x2_info_421700` | 25 |  | Source/Car.hpp:871 | 0/1 | todo |  |
| 0x421720 | `is_on_trailer_421720` | 18 |  |  | 0/4 | todo |  |
| 0x421760 | `sub_421760` | 22 |  |  | 0/1 | todo |  |
| 0x421780 | `sub_421780` | 11 |  | Source/Car.hpp:1220 | 0/3 | todo |  |
| 0x421790 | `sub_421790` | 11 |  | Source/Car.hpp:1226 | 0/4 | todo |  |
| 0x4217A0 | `sub_4217A0` | 11 |  | Source/Car.hpp:1196 | 0/4 | todo |  |
| 0x4217D0 | `sub_4217D0` | 11 |  | Source/Car.hpp:1238 | 0/1 | todo |  |
| 0x4217E0 | `sub_4217E0` | 11 |  | Source/Car.hpp:1244 | 0/2 | todo |  |
| 0x421810 | `sub_421810` | 24 |  | Source/Car.hpp:907 | 0/1 | todo |  |
| 0x421830 | `Car_BC::sub_421830` | 50 |  |  | 0/1 | todo |  |
| 0x421870 | `sub_421870` | 10 |  | Source/Car.hpp:1097 | 0/4 | todo |  |
| 0x421890 | `AddFlags_421890` | 12 |  | Source/Car.hpp:1382 | 0/1 | todo |  |
| 0x4218A0 | `sub_4218A0` | 8 |  | Source/Car.hpp:985 | 0/2 | todo |  |
| 0x4218B0 | `sub_4218B0` | 8 |  | Source/Car.hpp:991 | 0/1 | todo |  |
| 0x4218C0 | `sub_4218C0` | 11 |  | Source/Car.hpp:913 | 0/1 | todo |  |
| 0x4218D0 | `sub_4218D0` | 11 |  | Source/Car.hpp:919 | 0/1 | todo |  |
| 0x4218E0 | `sub_4218E0` | 8 |  | Source/Car.hpp:1124 | 0/2 | todo |  |
| 0x4218F0 | `sub_4218F0` | 10 |  | Source/Car.hpp:1118 | 0/1 | todo |  |
| 0x421910 | `ApplyScale_421910` | 55 |  | Source/Car.hpp:1269 | 0/1 | todo |  |
| 0x421950 | `sub_421950` | 13 |  | Source/Car.hpp:494 | 0/2 | todo |  |
| 0x421960 | `sub_421960` | 4 |  | Source/Car.hpp:158 | 0/1 | todo |  |
| 0x421970 | `sub_421970` | 10 |  | Source/Car.hpp:164 | 0/1 | todo |  |
| 0x421980 | `sub_421980` | 11 |  | Source/Player.hpp:118 | 0/6 | todo |  |
| 0x421990 | `sub_421990` | 11 |  | Source/Player.hpp:112 | 0/4 | todo |  |
| 0x4219A0 | `sub_4219A0` | 34 |  |  | 0/1 | todo |  |
| 0x4219D0 | `angry_lewin_0x85C::sub_4219D0` | 4 |  | Source/Player.hpp:317 | 0/20 | todo |  |
| 0x4219E0 | `Game_0x40::get_player_4219E0` | 16 |  | Source/Game.hpp:111 | 0/7 | todo |  |
| 0x4219F0 | `Garox_2B00::sub_4219F0` | 4 |  | Source/Hud.hpp:731 | 0/1 | todo |  |
| 0x421C00 | `sub_421C00` | 59 |  |  | 0/1 | todo |  |
| 0x421C40 | `sub_421C40` | 104 |  | Source/Car.cpp:1923 | 0/1 | todo |  |
| 0x421CB0 | `sub_421CB0` | 60 |  |  | 0/1 | todo |  |
| 0x421D80 | `HasSpriteZoom_421D80` | 8 |  |  | 0/5 | todo |  |
| 0x421D90 | `Car_BC::sub_421D90` | 86 |  |  | 0/12 | todo |  |
| 0x421DF0 | `IsCarInAir_421DF0` | 15 |  |  | 0/2 | todo |  |
| 0x421EC0 | `Car_BC::sub_421EC0` | 38 |  |  | 0/16 | todo |  |
| 0x4221A0 | `sub_4221A0` | 8 |  |  | 0/5 | todo |  |
| 0x4221B0 | `sub_4221B0` | 20 |  |  | 0/2 | todo |  |
| 0x4221D0 | `Car_BC::sub_4221D0` | 84 | 0x43A9A0 MATCH |  | 0/1 | todo |  |
| 0x422260 | `sub_422260` | 53 |  |  | 0/1 | todo |  |
| 0x4226C0 | `sub_4226C0` | 511 |  | Source/Car.cpp:2907 | 0/1 | todo |  |
| 0x422CB0 | `sub_422CB0` | 107 |  |  | 0/1 | todo |  |
| 0x422E00 | `sub_422E00` | 242 |  |  | 0/1 | todo |  |
| 0x422F00 | `sub_422F00` | 442 |  |  | 0/2 | todo |  |
| 0x4230D0 | `sub_4230D0` | 141 |  |  | 0/1 | todo |  |
| 0x423230 | `sub_423230` | 90 |  |  | 0/2 | todo |  |
| 0x4232E0 | `sub_4232E0` | 36 |  |  | 0/1 | todo |  |
| 0x423310 | `sub_423310` | 158 |  |  | 0/1 | todo |  |
| 0x4233B0 | `sub_4233B0` | 208 |  |  | 0/1 | todo |  |
| 0x423480 | `sub_423480` | 26 |  |  | 0/3 | todo |  |
| 0x423590 | `sub_423590` | 53 |  |  | 0/1 | todo |  |
| 0x423720 | `Car_BC::sub_423720` | 133 |  |  | 0/1 | todo |  |
| 0x423830 | `sub_423830` | 27 |  |  | 0/1 | todo |  |
| 0x423850 | `sub_423850` | 35 |  |  | 0/1 | todo |  |
| 0x423940 | `sub_423940` | 257 |  |  | 0/1 | todo |  |
| 0x423A50 | `Car_BC::sub_423A50` | 26 |  |  | 0/2 | todo |  |
| 0x423A70 | `sub_423A70` | 37 |  |  | 0/2 | todo |  |
| 0x423B50 | `sub_423B50` | 40 |  |  | 0/1 | todo |  |
| 0x423B80 | `sub_423B80` | 162 |  |  | 0/1 | todo |  |
| 0x423C30 | `sub_423C30` | 501 |  |  | 0/1 | todo |  |
| 0x424010 | `sub_424010` | 31 |  |  | 0/1 | todo |  |
| 0x424220 | `sub_424220` | 88 |  | Source/Car.cpp:5399 | 0/2 | todo |  |
| 0x424620 | `SpriteList_4::ctor_424620` | 12 |  |  | 0/8 | todo |  |
| 0x424FF0 | `sub_424FF0` | 948 |  |  | 0/1 | todo |  |
| 0x425450 | `Ped_Unknown_4::ctor_425450` | 12 |  |  | 0/1 | todo |  |
| 0x425460 | `sub_425460` | 27 |  | Source/CarAI.hpp:158 | 0/1 | todo |  |
| 0x425480 | `sub_425480` | 28 |  |  | 0/1 | todo |  |
| 0x4254A0 | `sub_4254A0` | 67 |  |  | 0/1 | todo |  |
| 0x425500 | `TrailerPool::ctor_425500` | 102 |  | Source/Car.hpp:1496 | 0/1 | todo |  |
| 0x425570 | `sub_425570` | 8 |  |  | 0/1 | todo |  |
| 0x425580 | `TrailerPool::sub_425580` | 14 |  | Source/Car.hpp:1506 | 0/1 | todo |  |
| 0x425590 | `SetHeadlightDeltas_425590` | 183 |  | Source/Car.hpp:1047 | 0/2 | todo |  |
| 0x425650 | `ClearHeadlightDeltas_425650` | 259 |  | Source/Car.hpp:1003 | 0/1 | todo |  |
| 0x425770 | `sub_425770` | 11 |  | Source/Car.hpp:729 | 0/1 | todo |  |
| 0x425780 | `Car_BC::IsAcceleratorJammed_425780` | 11 |  |  | 0/1 | todo |  |
| 0x4257B0 | `sub_4257B0` | 86 |  |  | 0/1 | todo |  |
| 0x425D60 | `sub_425D60` | 42 |  |  | 0/1 | todo |  |
| 0x425DD0 | `sub_425DD0` | 19 | 0x43BCA0 MATCH |  | 0/1 | todo |  |
| 0x425DF0 | `Car_BC::sub_425DF0` | 52 |  |  | 0/2 | todo |  |
| 0x425E30 | `sub_425E30` | 33 |  |  | 0/1 | todo |  |
| 0x425E60 | `sub_425E60` | 33 |  |  | 0/2 | todo |  |
| 0x425ED0 | `sub_425ED0` | 78 |  |  | 0/2 | todo |  |
| 0x425FD0 | `sub_425FD0` | 83 | 0x43D690 MATCH |  | 0/1 | todo |  |
| 0x4260C0 | `Car_BC::sub_4260C0` | 87 |  |  | 0/1 | todo |  |
| 0x426120 | `sub_426120` | 31 |  |  | 0/3 | todo |  |
| 0x4262B0 | `sub_4262B0` | 48 |  |  | 0/1 | todo |  |
| 0x426DB0 | `Car_BC_Pool::ctor_426DB0` | 79 |  | Source/Car.hpp:1456 | 0/1 | todo |  |
| 0x426E00 | `StartEngine_426E00` | 15 |  | Source/Car.hpp:1342 | 0/5 | todo |  |
| 0x426E10 | `EnqueueRadioLocationPhrase_426E10` | 39 |  | Source/Car.hpp:240 | 0/4 | todo |  |
| 0x426E40 | `sub_426E40` | 146 |  | Source/Car.hpp:279 | 0/5 | todo |  |
| 0x427450 | `sub_427450` | 98 |  |  | 0/2 | todo |  |
| 0x4274C0 | `sub_4274C0` | 1309 |  | Source/Car.cpp:4016 | 0/1 | todo |  |
| 0x428F70 | `sub_428F70` | 36 |  |  | 0/1 | todo |  |
| 0x429250 | `CarManager_6C::dtor_429250` | 150 |  |  | 0/1 | todo |  |
| 0x4292F0 | `sub_4292F0` | 118 |  |  | 0/4 | todo |  |
| 0x4298B0 | `Car_BC::sub_4298B0` | 1422 |  |  | 0/1 | todo |  |
| 0x429FF0 | `Car_BC::sub_429FF0` | 279 |  |  | 0/1 | todo |  |
| 0x42A2D0 | `Car_BC_Pool::sub_42A2D0` | 141 |  |  | 0/1 | todo |  |
| 0x42A5B0 | `sub_42A5B0` | 24 |  |  | 0/1 | todo |  |
| 0x42A5D0 | `sub_42A5D0` | 58 |  |  | 0/1 | todo |  |
| 0x42A620 | `sub_42A620` | 15 |  |  | 0/2 | todo |  |
| 0x42A630 | `sub_42A630` | 33 |  | Source/CarPhysics.cpp:809, Source/fix16.hpp:293 | 0/14 | todo |  |
| 0x42A660 | `GetOppositeFace_42A660` | 56 |  | Source/CarAI.cpp:2959 | 0/2 | todo |  |
| 0x42A6B0 | `MaxAbsDistance_42A6B0` | 103 | 0x4614E0 MATCH | Source/fix16.hpp:435 | 0/28 | todo |  |
| 0x42A720 | `sub_42A720` | 239 |  | Source/ang16.hpp:401 | 0/4 | todo |  |
| 0x42A810 | `sub_42A810` | 20 |  | Source/gtx_0x106C.hpp:238 | 0/1 | todo |  |
| 0x42A830 | `gmp_compressed_map_32::sub_42A830` | 19 |  | Source/map_0x370.hpp:152 | 0/14 | todo |  |
| 0x42A9D0 | `sub_42A9D0` | 9 | 0x453BF0 MATCH |  | 0/1 | todo |  |
| 0x42AB90 | `sub_42AB90` | 8 |  | Source/CarPhysics.hpp:478 | 0/6 | todo |  |
| 0x42ABA0 | `sub_42ABA0` | 8 |  | Source/CarPhysics.hpp:475, Source/CarPhysics.hpp:550 | 0/5 | todo |  |
| 0x42ABB0 | `sub_42ABB0` | 16 |  | Source/CarPhysics.hpp:484 | 0/3 | todo |  |
| 0x42ABC0 | `ForceNeutralInput_42ABC0` | 28 | 0x453F50 MATCH | Source/CarPhysics.hpp:448 | 0/1 | todo |  |
| 0x42AC00 | `sub_42AC00` | 27 | 0x453F90 MATCH | Source/CarPhysics.hpp:457 | 0/3 | todo |  |
| 0x42AC20 | `sub_42AC20` | 14 |  | Source/Car.hpp:458 | 0/2 | todo |  |
| 0x42AC30 | `sub_42AC30` | 14 |  | Source/Car.hpp:464 | 0/2 | todo |  |
| 0x42AC40 | `sub_42AC40` | 14 |  | Source/Car.hpp:470 | 0/1 | todo |  |
| 0x42AC50 | `sub_42AC50` | 14 |  | Source/Car.hpp:476 | 0/1 | todo |  |
| 0x42AC60 | `sub_42AC60` | 14 |  | Source/Car.hpp:452 | 0/1 | todo |  |
| 0x42AC70 | `sub_42AC70` | 8 |  | Source/Car.hpp:395 | 0/1 | todo |  |
| 0x42AC80 | `sub_42AC80` | 8 |  | Source/Car.hpp:401 | 0/1 | todo |  |
| 0x42AC90 | `sub_42AC90` | 8 |  | Source/Car.hpp:407 | 0/1 | todo |  |
| 0x42ACA0 | `sub_42ACA0` | 8 |  | Source/Car.hpp:440 | 0/1 | todo |  |
| 0x42ACB0 | `sub_42ACB0` | 8 |  | Source/Car.hpp:446 | 0/1 | todo |  |
| 0x42ADA0 | `sub_42ADA0` | 1427 |  |  | 0/1 | todo |  |
| 0x42B8A0 | `sub_42B8A0` | 1844 |  |  | 0/1 | todo |  |
| 0x42C8B0 | `sub_42C8B0` | 2748 |  |  | 0/1 | todo |  |
| 0x42FDF0 | `sub_42FDF0` | 77 |  |  | 0/1 | todo |  |
| 0x42FE40 | `sub_42FE40` | 27 |  | Source/CarAI.cpp:1057 | 0/1 | todo |  |
| 0x42FE60 | `sub_42FE60` | 18 |  |  | 0/1 | todo |  |
| 0x42FE80 | `sub_42FE80` | 145 |  |  | 0/1 | todo |  |
| 0x430090 | `sub_430090` | 72 |  |  | 0/1 | todo |  |
| 0x431C10 | `sub_431C10` | 1855 |  |  | 0/1 | todo |  |
| 0x432810 | `Monster_808::sub_432810` | 23 |  |  | 0/1 | todo |  |
| 0x432850 | `gtx_0x106C::get_car_info_count_432850` | 10 |  | Source/gtx_0x106C.hpp:399 | 0/3 | todo |  |
| 0x432860 | `sub_432860` | 20 |  | Source/Fix16_Point.hpp:48 | 0/16 | todo |  |
| 0x432AA0 | `sub_432AA0` | 442 |  |  | 0/1 | todo |  |
| 0x432C60 | `sub_432C60` | 49 |  |  | 0/1 | todo |  |
| 0x432F70 | `Monster_2C::gdtor_432F70` | 77 |  |  | 0/1 | todo |  |
| 0x433120 | `sub_433120` | 8 |  |  | 0/1 | todo |  |
| 0x433160 | `sub_433160` | 40 |  |  | 0/1 | todo |  |
| 0x433360 | `sub_433360` | 13 |  | Source/PedGroup.hpp:84 | 0/6 | todo |  |
| 0x433370 | `Get_F3C_433370` | 4 |  | Source/PedGroup.hpp:91 | 0/2 | todo |  |
| 0x433430 | `sub_433430` | 56 |  | Source/map_0x370.hpp:699 | 0/1 | todo |  |
| 0x433470 | `sub_433470` | 46 |  | Source/Ped.cpp:2435, Source/map_0x370.hpp:677 | 0/3 | todo |  |
| 0x4334A0 | `sub_4334A0` | 46 |  | Source/Ped.cpp:2435, Source/map_0x370.hpp:677 | 0/3 | todo |  |
| 0x4334D0 | `sub_4334D0` | 46 |  | Source/Ped.cpp:2435, Source/map_0x370.hpp:677 | 0/3 | todo |  |
| 0x433500 | `sub_433500` | 46 |  | Source/Ped.cpp:2435, Source/map_0x370.hpp:677 | 0/3 | todo |  |
| 0x433530 | `sub_433530` | 44 |  | Source/map_0x370.hpp:645 | 0/4 | todo |  |
| 0x433560 | `sub_433560` | 28 |  | Source/Fix16_Rect.hpp:89 | 0/1 | todo |  |
| 0x433580 | `cool_nash_0x294::sub_433580` | 169 |  |  | 0/3 | todo |  |
| 0x4337D0 | `sub_4337D0` | 19 |  | Source/sprite.hpp:547 | 0/9 | todo |  |
| 0x4337F0 | `sub_4337F0` | 5 |  | Source/sprite.hpp:528 | 0/15 | todo |  |
| 0x433800 | `sub_433800` | 5 |  | Source/sprite.hpp:541 | 0/2 | todo |  |
| 0x433810 | `sub_433810` | 10 |  | Source/Weapon.hpp:163 | 0/8 | todo |  |
| 0x433820 | `sub_433820` | 11 |  | Source/WeaponManager.hpp:15 | 0/3 | todo |  |
| 0x433880 | `Char_B4::sub_433880` | 75 |  |  | 0/1 | todo |  |
| 0x4338D0 | `sub_4338D0` | 7 |  | Source/char.hpp:67 | 0/1 | todo |  |
| 0x4338E0 | `Char_B4::sub_4338E0` | 10 |  | Source/char.hpp:73 | 0/1 | todo |  |
| 0x4338F0 | `sub_4338F0` | 11 |  | Source/char.hpp:55 | 0/3 | todo |  |
| 0x433900 | `sub_433900` | 13 |  | Source/char.hpp:299 | 0/7 | todo |  |
| 0x433910 | `sub_433910` | 10 |  | Source/char.hpp:245 | 0/5 | todo |  |
| 0x433920 | `Char_B4::sub_433920` | 10 |  | Source/char.hpp:227 | 0/20 | todo |  |
| 0x433930 | `Char_B4::sub_433930` | 7 |  | Source/char.hpp:49 | 0/5 | todo |  |
| 0x433940 | `sub_433940` | 41 |  | Source/char.hpp:305 | 0/2 | todo |  |
| 0x433970 | `sub_433970` | 68 |  | Source/char.hpp:201 | 0/9 | todo |  |
| 0x4339C0 | `sub_4339C0` | 18 |  | Source/char.hpp:183 | 0/2 | todo |  |
| 0x4339E0 | `sub_4339E0` | 18 |  | Source/char.hpp:189 | 0/2 | todo |  |
| 0x433A00 | `sub_433A00` | 18 |  | Source/char.hpp:195 | 0/2 | todo |  |
| 0x433A20 | `sub_433A20` | 4 |  | Source/char.hpp:239 | 0/4 | todo |  |
| 0x433A30 | `sub_433A30` | 12 |  | Source/char.hpp:177 | 0/3 | todo |  |
| 0x433A40 | `sub_433A40` | 14 |  | Source/char.hpp:171 | 0/2 | todo |  |
| 0x433A50 | `sub_433A50` | 10 |  | Source/char.hpp:251 | 0/5 | todo |  |
| 0x433A60 | `sub_433A60` | 10 |  | Source/char.hpp:281 | 0/2 | todo |  |
| 0x433A80 | `Char_B4::sub_433A80` | 4 |  | Source/char.hpp:275 | 0/17 | todo |  |
| 0x433A90 | `Char_B4::sub_433A90` | 4 |  | Source/char.hpp:287 | 0/4 | todo |  |
| 0x433AA0 | `sub_433AA0` | 9 |  | Source/char.hpp:322 | 0/1 | todo |  |
| 0x433B30 | `Zone_144::sub_433B30` | 7 |  | Source/Gang.hpp:19 | 0/1 | todo |  |
| 0x433B40 | `cool_nash_0x294::sub_433B40` | 10 |  | Source/Ped.hpp:426 | 0/21 | todo |  |
| 0x433B50 | `cool_nash_0x294::sub_433B50` | 11 |  | Source/Ped.hpp:591 | 0/9 | todo |  |
| 0x433B60 | `sub_433B60` | 7 |  | Source/Ped.hpp:776 | 0/5 | todo |  |
| 0x433B70 | `sub_433B70` | 8 |  | Source/Ped.hpp:653 | 0/7 | todo |  |
| 0x433B80 | `sub_433B80` | 15 |  | Source/Ped.hpp:130 | 0/1 | todo |  |
| 0x433B90 | `sub_433B90` | 13 |  | Source/Ped.hpp:641 | 0/17 | todo |  |
| 0x433BA0 | `cool_nash_0x294::get_remap_433BA0` | 7 |  | Source/Ped.hpp:597 | 0/9 | todo |  |
| 0x433BB0 | `sub_433BB0` | 13 |  | Source/Ped.hpp:722 | 0/2 | todo |  |
| 0x433BC0 | `sub_433BC0` | 13 |  | Source/Ped.hpp:740 | 0/4 | todo |  |
| 0x433BD0 | `sub_433BD0` | 7 |  | Source/Ped.hpp:888 | 0/1 | todo |  |
| 0x433BE0 | `cool_nash_0x294::sub_433BE0` | 11 |  | Source/Ped.hpp:764 | 0/3 | todo |  |
| 0x433BF0 | `SetAttacker_433BF0` | 13 |  | Source/Ped.hpp:882 | 0/3 | todo |  |
| 0x433C00 | `sub_433C00` | 11 |  | Source/Ped.hpp:770 | 0/6 | todo |  |
| 0x433C10 | `sub_433C10` | 11 |  | Source/Ped.hpp:704 | 0/10 | todo |  |
| 0x433C20 | `sub_433C20` | 23 |  | Source/Ped.hpp:59 | 0/3 | todo |  |
| 0x433C40 | `sub_433C40` | 16 |  | Source/Ped.hpp:609 | 0/2 | todo |  |
| 0x433C50 | `sub_433C50` | 13 |  | Source/Ped.hpp:671 | 0/4 | todo |  |
| 0x433C60 | `sub_433C60` | 13 |  | Source/Ped.hpp:677 | 0/4 | todo |  |
| 0x433C70 | `sub_433C70` | 13 |  | Source/Ped.hpp:683 | 0/4 | todo |  |
| 0x433C80 | `sub_433C80` | 15 |  | Source/Ped.hpp:136 | 0/1 | todo |  |
| 0x433C90 | `sub_433C90` | 11 |  | Source/Ped.hpp:35 | 0/1 | todo |  |
| 0x433CA0 | `sub_433CA0` | 13 |  | Source/Ped.hpp:805 | 0/6 | todo |  |
| 0x433DA0 | `cool_nash_0x294::IsInvisibleOnFoot_433DA0` | 28 |  | Source/Ped.hpp:784 | 0/5 | todo |  |
| 0x433DC0 | `cool_nash_0x294::get_wanted_points_433DC0` | 8 | 0x592370 MATCH | Source/Ped.hpp:562 | 0/2 | todo |  |
| 0x433DD0 | `cool_nash_0x294::sub_433DD0` | 22 |  | Source/Ped.hpp:626 | 0/12 | todo |  |
| 0x433DF0 | `cool_nash_0x294::sub_433DF0` | 92 |  |  | 0/2 | todo |  |
| 0x433E50 | `sub_433E50` | 53 |  | Source/Ped.hpp:712 | 0/2 | todo |  |
| 0x433E90 | `sub_433E90` | 20 |  | Source/Camera.hpp:166 | 0/2 | todo |  |
| 0x433EB0 | `sub_433EB0` | 135 |  |  | 0/1 | todo |  |
| 0x433F40 | `sub_433F40` | 156 |  |  | 0/1 | todo |  |
| 0x433FE0 | `sub_433FE0` | 106 |  |  | 0/1 | todo |  |
| 0x434070 | `sub_434070` | 25 |  | Source/PedRefTable.hpp:24 | 0/1 | todo |  |
| 0x4340A0 | `sub_4340A0` | 4 |  | Source/Object_5C.hpp:240 | 0/1 | todo |  |
| 0x4340D0 | `sub_4340D0` | 15 |  | Source/Object_5C.hpp:293 | 0/5 | todo |  |
| 0x4340E0 | `sub_4340E0` | 15 |  | Source/Object_5C.hpp:299 | 0/5 | todo |  |
| 0x4340F0 | `sub_4340F0` | 15 |  | Source/Object_5C.hpp:305 | 0/5 | todo |  |
| 0x434130 | `sub_434130` | 15 |  | Source/Object_5C.hpp:246 | 0/1 | todo |  |
| 0x434140 | `IsDestroyedPowergen_434140` | 23 |  | Source/Object_5C.hpp:178 | 0/1 | todo |  |
| 0x4341B0 | `sub_4341B0` | 4 |  | Source/Car_10.hpp:10 | 0/2 | todo |  |
| 0x434380 | `sub_434380` | 25 |  |  | 0/1 | todo |  |
| 0x4343B0 | `Car_BC::sub_4343B0` | 7 |  | Source/Car.hpp:997 | 0/1 | todo |  |
| 0x434900 | `angry_lewin_0x85C::get_camera_434900` | 26 |  | Source/Player.hpp:329 | 0/4 | todo |  |
| 0x434920 | `sub_434920` | 19 |  | Source/Player.hpp:74 | 0/1 | todo |  |
| 0x434940 | `sub_434940` | 15 |  | Source/Player.hpp:80 | 0/1 | todo |  |
| 0x434950 | `sub_434950` | 10 |  | Source/Player.hpp:199 | 0/5 | todo |  |
| 0x434960 | `sub_434960` | 11 |  | Source/TrafficLights.hpp:55 | 0/5 | todo |  |
| 0x434970 | `sub_434970` | 7 |  | Source/TaxiList.hpp:60 | 0/1 | todo |  |
| 0x434A10 | `sub_434A10` | 7 |  | Source/GameSession.hpp:36 | 0/1 | todo |  |
| 0x434AF0 | `sub_434AF0` | 27 |  | Source/Garage.hpp:31 | 0/1 | todo |  |
| 0x434B10 | `sub_434B10` | 6 |  | Source/winmain.hpp:39 | 0/4 | todo |  |
| 0x434B20 | `sub_434B20` | 16 |  | Source/GameSession.hpp:199, Source/TagGame.hpp:18 | 0/3 | todo |  |
| 0x434B60 | `sub_434B60` | 27 |  | Source/Player.hpp:447, Source/TagGame.hpp:24 | 0/1 | todo |  |
| 0x434D60 | `cool_nash_0x294::sub_434D60` | 15 |  |  | 0/1 | todo |  |
| 0x4354C0 | `sub_4354C0` | 46 |  |  | 0/1 | todo |  |
| 0x435530 | `PedPool::sub_435530` | 4 |  | Source/Char_Pool.hpp:190 | 0/1 | todo |  |
| 0x435550 | `Char_B4_Pool::ctor_435550` | 71 |  |  | 0/1 | todo |  |
| 0x4355A0 | `Char_B4_Pool::sub_4355A0` | 19 |  |  | 0/1 | todo |  |
| 0x4355C0 | `Char_B4_Pool::DeAllocate` | 27 |  |  | 0/2 | todo |  |
| 0x4355E0 | `Char_8_Pool::ctor_4355E0` | 41 |  |  | 0/1 | todo |  |
| 0x435610 | `sub_435610` | 76 |  |  | 0/7 | todo |  |
| 0x435C20 | `sub_435C20` | 30 | 0x408680 MATCH |  | 0/1 | todo |  |
| 0x435C40 | `cool_nash_0x294::sub_435C40` | 51 |  |  | 0/2 | todo |  |
| 0x435F00 | `cool_nash_0x294::sub_435F00` | 25 |  |  | 0/3 | todo |  |
| 0x435F40 | `cool_nash_0x294::sub_435F40` | 34 |  |  | 0/1 | todo |  |
| 0x436040 | `cool_nash_0x294::sub_436040` | 33 |  |  | 0/1 | todo |  |
| 0x436070 | `sub_436070` | 72 | 0x470F30 MATCH |  | 0/1 | todo |  |
| 0x436140 | `sub_436140` | 23 |  |  | 0/3 | todo |  |
| 0x436160 | `sub_436160` | 73 | 0x45C920 MATCH |  | 0/1 | todo |  |
| 0x436200 | `ApplyCarVelocityCameraOffset_436200` | 23 |  |  | 0/3 | todo |  |
| 0x436920 | `cool_nash_0x294::sub_436920` | 610 | 0x463830 MATCH |  | 0/1 | todo |  |
| 0x436BF0 | `sub_436BF0` | 982 |  |  | 0/1 | todo |  |
| 0x437EB0 | `sub_437EB0` | 12 |  |  | 0/1 | todo |  |
| 0x4388E0 | `sub_4388E0` | 334 |  |  | 0/1 | todo |  |
| 0x439640 | `sub_439640` | 319 |  |  | 0/1 | todo |  |
| 0x439D30 | `cool_nash_0x294::sub_439D30` | 294 |  |  | 0/1 | todo |  |
| 0x43BEB0 | `sub_43BEB0` | 12 |  |  | 0/1 | todo |  |
| 0x43BEC0 | `sub_43BEC0` | 12 |  |  | 0/1 | todo |  |
| 0x43BED0 | `sub_43BED0` | 12 |  |  | 0/1 | todo |  |
| 0x43BEE0 | `sub_43BEE0` | 120 |  |  | 0/1 | todo |  |
| 0x43BF60 | `sub_43BF60` | 169 |  |  | 0/1 | todo |  |
| 0x43D640 | `sub_43D640` | 485 |  |  | 0/1 | todo |  |
| 0x43DB20 | `PedPool::gdtor_43DB20` | 30 |  |  | 0/1 | todo |  |
| 0x43DF60 | `sub_43DF60` | 27 |  |  | 0/7 | todo |  |
| 0x43E0E0 | `PedPool::ctor_43E0E0` | 85 |  |  | 0/1 | todo |  |
| 0x43E1E0 | `sub_43E1E0` | 131 |  |  | 0/1 | todo |  |
| 0x4402C0 | `sub_4402C0` | 142 |  |  | 0/1 | todo |  |
| 0x441A30 | `sub_441A30` | 1075 |  |  | 0/1 | todo |  |
| 0x4427E0 | `sub_4427E0` | 576 |  |  | 0/1 | todo |  |
| 0x445960 | `sub_445960` | 188 |  | Source/Char_Pool.hpp:133 | 0/1 | todo |  |
| 0x445CC0 | `sub_445CC0` | 375 |  |  | 0/1 | todo |  |
| 0x445EC0 | `TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0` | 34 | 0x445EC0 MATCH |  | 0/1 | todo |  |
| 0x445EF0 | `sub_445EF0` | 8 |  | Source/Char_Pool.hpp:60 | 0/3 | todo |  |
| 0x445F00 | `Char_8_Pool::sub_445F00` | 14 |  | Source/Char_Pool.hpp:66 | 0/6 | todo |  |
| 0x4460D0 | `Ped_Unknown_4::sub_4460D0` | 40 |  |  | 0/1 | todo |  |
| 0x4463C0 | `sub_4463C0` | 78 |  |  | 0/1 | todo |  |
| 0x446410 | `sub_446410` | 99 |  |  | 0/1 | todo |  |
| 0x446480 | `sub_446480` | 92 |  |  | 0/1 | todo |  |
| 0x4464E0 | `sub_4464E0` | 104 |  |  | 0/1 | todo |  |
| 0x446550 | `sub_446550` | 92 |  |  | 0/1 | todo |  |
| 0x4465B0 | `sub_4465B0` | 104 |  |  | 0/1 | todo |  |
| 0x446620 | `sub_446620` | 104 |  |  | 0/1 | todo |  |
| 0x446690 | `sub_446690` | 99 |  |  | 0/1 | todo |  |
| 0x446820 | `SpriteGrid_400::GetCollideListAt_446820` | 54 |  | Source/SpriteGrid.hpp:44 | 0/1 | todo |  |
| 0x4468C0 | `GridSpriteLink_Pool::ctor_4468C0` | 41 |  |  | 0/1 | todo |  |
| 0x4468F0 | `GridCell_Pool::ctor_4468F0` | 41 |  |  | 0/1 | todo |  |
| 0x446920 | `sub_446920` | 10 |  | Source/sprite.hpp:40 | 0/4 | todo |  |
| 0x446930 | `CanAllocateOfType_446930` | 15 |  |  | 0/4 | todo |  |
| 0x446940 | `sub_446940` | 15 |  | Source/sprite.hpp:314 | 0/4 | todo |  |
| 0x446950 | `sub_446950` | 8 |  | Source/sprite.hpp:387 | 0/2 | todo |  |
| 0x446960 | `sub_446960` | 92 |  | Source/sprite.hpp:381 | 0/1 | todo |  |
| 0x446AA0 | `sub_446AA0` | 32 |  |  | 0/1 | todo |  |
| 0x4472F0 | `SpriteGrid_400::gdtor_4472F0` | 26 |  |  | 0/1 | todo |  |
| 0x447310 | `GridSpriteLink_Pool::gdtor_447310` | 30 |  |  | 0/1 | todo |  |
| 0x447330 | `GridCell_Pool::gdtor_447330` | 30 |  |  | 0/1 | todo |  |
| 0x447350 | `sub_447350` | 8 |  |  | 0/2 | todo |  |
| 0x447360 | `sub_447360` | 14 |  | Source/Car.hpp:716 | 0/2 | todo |  |
| 0x447370 | `sub_447370` | 8 |  |  | 0/2 | todo |  |
| 0x447380 | `sub_447380` | 14 |  |  | 0/2 | todo |  |
| 0x447390 | `SpriteGrid_400::sub_447390` | 233 |  |  | 0/1 | todo |  |
| 0x447DF0 | `sub_447DF0` | 23 |  | Source/sprite.hpp:131 | 0/2 | todo |  |
| 0x447E10 | `sub_447E10` | 12 |  | Source/sprite.hpp:101 | 0/2 | todo |  |
| 0x447E20 | `set_xy_lazy_447E20` | 65 |  | Source/sprite.hpp:588 | 0/2 | todo |  |
| 0x447E90 | `sub_447E90` | 10 |  | Source/Object_5C.hpp:222 | 0/5 | todo |  |
| 0x447EA0 | `sub_447EA0` | 11 |  | Source/Car.hpp:1178 | 0/1 | todo |  |
| 0x447EB0 | `sub_447EB0` | 11 |  | Source/Car.hpp:1263 | 0/3 | todo |  |
| 0x447EC0 | `sub_447EC0` | 11 |  | Source/Car.hpp:1250 | 0/1 | todo |  |
| 0x447ED0 | `IsLongerThanOneBlock_447ED0` | 33 |  | Source/Car.hpp:1330 | 0/1 | todo |  |
| 0x447F00 | `CanBeLiftedByCrane_447F00` | 53 |  | Source/Car.hpp:1336 | 0/1 | todo |  |
| 0x447F40 | `sub_447F40` | 19 |  | Source/Cranes.hpp:74 | 0/1 | todo |  |
| 0x447F60 | `Zheal_15C::sub_447F60` | 13 |  | Source/Cranes.hpp:17 | 0/1 | todo |  |
| 0x447F90 | `sub_447F90` | 72 |  |  | 0/2 | todo |  |
| 0x448030 | `Zheal_15C::sub_448030` | 83 |  | Source/Cranes.cpp:119 | 0/1 | todo |  |
| 0x448150 | `Zheal_15C::sub_448150` | 160 |  | Source/Cranes.cpp:149 | 0/1 | todo |  |
| 0x448900 | `sub_448900` | 119 |  | Source/Cranes.cpp:510, Source/Cranes.cpp:517 | 0/2 | todo |  |
| 0x44A1D0 | `PlayerCrimeStats_3C::ctor_44A1D0` | 8 |  |  | 0/1 | todo |  |
| 0x44A370 | `CrimeReportQueue_CC::sub_44A370` | 38 |  |  | 0/2 | todo |  |
| 0x44A3E0 | `sub_44A3E0` | 8 |  | Source/Car.hpp:925 | 0/2 | todo |  |
| 0x44A470 | `sub_44A470` | 669 |  |  | 0/1 | todo |  |
| 0x44A720 | `sub_44A720` | 583 |  |  | 0/1 | todo |  |
| 0x44AA60 | `sub_44AA60` | 19 |  | Source/InputRecorder_67F8B0.hpp:115 | 0/1 | todo |  |
| 0x44AA80 | `sub_44AA80` | 16 |  | Source/InputRecorder_67F8B0.hpp:131 | 0/1 | todo |  |
| 0x44AF70 | `sub_44AF70` | 7 |  | Source/sprite.hpp:665 | 0/1 | todo |  |
| 0x44AF90 | `sub_44AF90` | 19 |  |  | 0/1 | todo |  |
| 0x44B490 | `sub_44B490` | 97 |  |  | 0/2 | todo |  |
| 0x44B890 | `SpriteRenderer_1C::sub_44B890` | 23 |  |  | 0/1 | todo |  |
| 0x44C050 | `sub_44C050` | 19 |  | Source/InputRecorder_67F8B0.hpp:125 | 0/1 | todo |  |
| 0x44C800 | `Door_10_Pool::ctor_44C800` | 41 |  |  | 0/1 | todo |  |
| 0x44C830 | `Door_10_Pool::sub_44C830` | 14 |  |  | 0/1 | todo |  |
| 0x44C840 | `gmp_block_info::init_44C840` | 28 |  | Source/map_0x370.hpp:45 | 0/4 | todo |  |
| 0x44C860 | `IsOpen_44C860` | 9 |  | Source/Door.hpp:75, Source/DoorPool.hpp:61 | 0/1 | todo |  |
| 0x44C870 | `sub_44C870` | 35 |  |  | 0/1 | todo |  |
| 0x44CC30 | `sub_44CC30` | 389 | 0x49CC00 MATCH |  | 0/1 | todo |  |
| 0x44CDD0 | `sub_44CDD0` | 525 |  |  | 0/2 | todo |  |
| 0x44D3A0 | `Door_10_Pool::gdtor_44D3A0` | 30 |  |  | 0/1 | todo |  |
| 0x44D840 | `sub_44D840` | 687 |  |  | 0/1 | todo |  |
| 0x44DBF0 | `sub_44DBF0` | 19 |  |  | 0/1 | todo |  |
| 0x450530 | `FatalError_450530` | 435 | 0x4A38C0 MATCH |  | 0/2 | todo |  |
| 0x450CB0 | `GetObjectiveStatus_450CB0` | 7 |  | Source/Ped.hpp:437 | 0/9 | todo |  |
| 0x450CC0 | `sub_450CC0` | 11 |  |  | 0/1 | todo |  |
| 0x451510 | `sub_451510` | 141 |  |  | 0/1 | todo |  |
| 0x451F70 | `TextureCache_15D8::gdtor_451F70` | 30 |  |  | 0/1 | todo |  |
| 0x451F90 | `gtx_0x106C::gdtor_451F90` | 30 |  |  | 0/1 | todo |  |
| 0x4527A0 | `sub_4527A0` | 106 |  |  | 0/1 | todo |  |
| 0x452990 | `sub_452990` | 35 |  |  | 0/2 | todo |  |
| 0x4529C0 | `sub_4529C0` | 66 |  |  | 0/1 | todo |  |
| 0x452E60 | `LangIsJapanese_452E60` | 8 |  | Source/TextTable.hpp:62 | 0/2 | todo |  |
| 0x453480 | `sub_453480` | 268 |  |  | 0/1 | todo |  |
| 0x4538E0 | `?do_always_noconv@codecvt_base@std@@MBE_NXZ` | 3 |  |  | 0/1 | todo |  |
| 0x4539B0 | `gtx_0x106C::sub_4539B0` | 24 | 0x5D7700 MATCH | Source/Draw.hpp:65 | 0/4 | todo |  |
| 0x4539D0 | `sub_4539D0` | 29 |  | Source/Draw.hpp:71, Source/TextTable.cpp:215 | 0/3 | todo |  |
| 0x453A30 | `laughing_blackwell_0x1EB54::sub_453A30` | 15 |  | Source/Frontend.hpp:314 | 0/1 | todo |  |
| 0x453A40 | `sub_453A40` | 21 |  | Source/GameSession.hpp:42 | 0/4 | todo |  |
| 0x453A60 | `sub_453A60` | 26 |  | Source/GameSession.hpp:50 | 0/11 | todo |  |
| 0x453A80 | `GameSession_578::sub_453A80` | 7 |  | Source/GameSession.hpp:24 | 0/3 | todo |  |
| 0x453A90 | `sub_453A90` | 11 |  | Source/TagGame.hpp:27 | 0/1 | todo |  |
| 0x453AA0 | `sub_453AA0` | 11 |  | Source/TagGame.hpp:42 | 0/2 | todo |  |
| 0x453AB0 | `sub_453AB0` | 169 |  | Source/Frontend.cpp:395, Source/TagGame.hpp:48 | 0/1 | todo |  |
| 0x453D40 | `sub_453D40` | 49 |  |  | 0/1 | todo |  |
| 0x45A8D0 | `Game_0x40::sub_45A8D0` | 64 | 0x4B9CD0 MATCH |  | 0/1 | todo |  |
| 0x45A910 | `Game_0x40::sub_45A910` | 67 | 0x4B9D10 MATCH |  | 0/1 | todo |  |
| 0x45A960 | `rng::ctor_45A960` | 16 |  |  | 0/1 | todo |  |
| 0x45A970 | `angry_lewin_0x85C::dtor_45A970` | 30 |  |  | 0/1 | todo |  |
| 0x45A990 | `Map_0x370::gdtor_45A990` | 30 |  |  | 0/1 | todo |  |
| 0x45A9B0 | `SpriteRenderer_1C::gdtor_45A9B0` | 30 |  |  | 0/1 | todo |  |
| 0x45A9F0 | `ScriptManager_C1EA8::gdtor_45A9F0` | 30 |  |  | 0/1 | todo |  |
| 0x45AA10 | `Object_5C::gdtor_45AA10` | 30 |  |  | 0/1 | todo |  |
| 0x45AA30 | `PedManager::gdtor_45AA30` | 30 |  |  | 0/1 | todo |  |
| 0x45AA50 | `FpsCounter_54::gdtor_45AA50` | 30 |  |  | 0/1 | todo |  |
| 0x45AA70 | `SpriteManager_8::gdtor_45AA70` | 30 |  |  | 0/1 | todo |  |
| 0x45AA90 | `CollisionCounters_C::gdtor_45AA90` | 30 |  |  | 0/1 | todo |  |
| 0x45AAB0 | `PedRefTable_7F8::gdtor_45AAB0` | 30 |  |  | 0/1 | todo |  |
| 0x45AAD0 | `Sero_181C::gdtor_45AAD0` | 30 |  |  | 0/1 | todo |  |
| 0x45AAF0 | `Taxi_4::gdtor_45AAF0` | 30 |  |  | 0/1 | todo |  |
| 0x45AB10 | `TileAnim_2::gdtor_45AB10` | 30 |  |  | 0/1 | todo |  |
| 0x45AB30 | `Weapon_8::gdtor_45AB30` | 30 |  |  | 0/1 | todo |  |
| 0x45AB50 | `Door_4D4::gdtor_45AB50` | 30 |  |  | 0/1 | todo |  |
| 0x45AB70 | `TrafficLights_194::gdtor_45AB70` | 30 |  |  | 0/1 | todo |  |
| 0x45AB90 | `Monster_808::gdtor_45AB90` | 30 |  |  | 0/1 | todo |  |
| 0x45ABB0 | `Particle_8::gdtor_45ABB0` | 30 |  |  | 0/1 | todo |  |
| 0x45ABD0 | `ExplosionPool_3D4::gdtor_45ABD0` | 30 |  |  | 0/1 | todo |  |
| 0x45ABF0 | `ExplosionPool_7A8::gdtor_45ABF0` | 30 |  |  | 0/1 | todo |  |
| 0x45AC10 | `CarChaseTaskTable_500::dtor_45AC10` | 30 |  |  | 0/1 | todo |  |
| 0x45AC30 | `CokeZero_100::dtor_45AC30` | 30 |  |  | 0/1 | todo |  |
| 0x45AC50 | `CrimeReportQueue_CC::gdtor_45AC50` | 30 |  |  | 0/1 | todo |  |
| 0x45AC70 | `CollisionSoundQueue_C88::gdtor_45AC70` | 30 |  |  | 0/1 | todo |  |
| 0x45AC90 | `KanjiFont_8EC::gdtor_45AC90` | 30 |  |  | 0/1 | todo |  |
| 0x45ACE0 | `Game_0x40::sub_45ACE0` | 3 |  |  | 0/1 | todo |  |
| 0x45ACF0 | `Game_0x40::sub_45ACF0` | 3 |  |  | 0/1 | todo |  |
| 0x45AD30 | `Car_BC_Pool::get_cars_count_45AD30` | 8 |  | Source/Car.hpp:1472 | 0/1 | todo |  |
| 0x45ADA0 | `sub_45ADA0` | 12 |  | Source/Fix16_Rect.hpp:174 | 0/3 | todo |  |
| 0x45ADB0 | `sub_45ADB0` | 11 |  | Source/Fix16_Rect.hpp:168 | 0/3 | todo |  |
| 0x45ADC0 | `sub_45ADC0` | 12 |  | Source/Fix16_Rect.hpp:180 | 0/3 | todo |  |
| 0x45ADD0 | `sub_45ADD0` | 12 |  | Source/Fix16_Rect.hpp:162 | 0/3 | todo |  |
| 0x45AE60 | `Zones_CA8::ctor_45AE60` | 30 |  |  | 0/1 | todo |  |
| 0x45AEA0 | `DrawUnk_0xBC::sub_45AEA0` | 160 |  | Source/Camera.hpp:130 | 0/1 | todo |  |
| 0x45AF40 | `DrawUnk_0xBC::sub_45AF40` | 139 |  | Source/Camera.hpp:139 | 0/1 | todo |  |
| 0x45AFD0 | `Hud_TextEntry_C4::SetFontRemap_45AFD0` | 25 |  | Source/Hud.hpp:254 | 0/3 | todo |  |
| 0x45B050 | `Nanobotz::ctor_45B050` | 69 |  |  | 0/1 | todo |  |
| 0x45B0A0 | `PlayerScoreTracker_36C::get_accuracy_count_45B0A0` | 7 |  | Source/PlayerScoreTracker.hpp:20 | 0/1 | todo |  |
| 0x45B0B0 | `PlayerScoreTracker_36C::get_reverse_count_45B0B0` | 7 |  | Source/PlayerScoreTracker.hpp:26 | 0/1 | todo |  |
| 0x45B0C0 | `angry_lewin_0x85C::sub_45B0C0` | 12 |  | Source/Player.hpp:50 | 0/1 | todo |  |
| 0x45B0D0 | `angry_lewin_0x85C::sub_45B0D0` | 4 |  |  | 0/1 | todo |  |
| 0x45B1A0 | `EmergencyCrewPool_1E0::ctor_45B1A0` | 82 |  |  | 0/1 | todo |  |
| 0x45B2D0 | `Light_28::SetCurrentIntensity_45B2D0` | 21 |  | Source/Light.hpp:82 | 0/1 | todo |  |
| 0x45B3D0 | `Light_1D4CC::ctor_45B3D0` | 65 |  |  | 0/1 | todo |  |
| 0x45B420 | `GameSession_578::sub_45B420` | 11 |  | Source/GameSession.hpp:91 | 0/1 | todo |  |
| 0x45B440 | `Tango_54::ctor_45B440` | 29 |  |  | 0/1 | todo |  |
| 0x45BDC0 | `ObjectDefinitions_8CA8::gdtor_45BDC0` | 30 |  |  | 0/1 | todo |  |
| 0x45BDE0 | `jawwie_110::gdtor_45BDE0` | 30 |  |  | 0/1 | todo |  |
| 0x45BE00 | `PatrolRoutePool_1D7E::gdtor_45BE00` | 30 |  |  | 0/1 | todo |  |
| 0x45BE20 | `PathFinder_2FD4::gdtor_45BE20` | 26 |  |  | 0/1 | todo |  |
| 0x45BE40 | `Zheal_D9C::gdtor_45BE40` | 30 |  |  | 0/1 | todo |  |
| 0x45BE60 | `Snooky_94::gdtor_45BE60` | 30 |  |  | 0/1 | todo |  |
| 0x45BE80 | `EmergencyCrewPool_1E0::gdtor_45BE80` | 30 |  |  | 0/1 | todo |  |
| 0x45BEA0 | `Police_7B8::gdtor_45BEA0` | 30 |  |  | 0/1 | todo |  |
| 0x45BEC0 | `Light_1D4CC::gdtor_45BEC0` | 30 |  |  | 0/1 | todo |  |
| 0x45BEE0 | `Zones_CA8::gdtor_45BEE0` | 30 |  |  | 0/1 | todo |  |
| 0x45BF00 | `Tango_54::gdtor_45BF00` | 26 |  |  | 0/1 | todo |  |
| 0x45BFE0 | `ScriptStackFramePool_25C::ctor_45BFE0` | 63 |  |  | 0/1 | todo |  |
| 0x45C040 | `Mike_A80::ctor_45C040` | 103 |  |  | 0/1 | todo |  |
| 0x45C0D0 | `jawwie_110::ctor_45C0D0` | 96 |  |  | 0/1 | todo |  |
| 0x45C150 | `Police_7B8::ctor_45C150` | 138 |  |  | 0/1 | todo |  |
| 0x45C1E0 | `Light_1D4CC::Service_45C1E0` | 5 |  | Source/LightPool.hpp:22 | 0/1 | todo |  |
| 0x45D350 | `ScriptStackFramePool_25C::gdtor_45D350` | 30 |  |  | 0/1 | todo |  |
| 0x45D370 | `ChickenLegend_48::dtor_45D370` | 26 |  |  | 0/1 | todo |  |
| 0x45D3B0 | `Garox_2B00::gdtor_45D3B0` | 30 |  |  | 0/1 | todo |  |
| 0x45D960 | `Zone_144::init_45D960` | 113 | 0x4BED70 MATCH |  | 0/1 | todo |  |
| 0x45DD50 | `Zone_144::sub_45DD50` | 12 |  | Source/Gang.hpp:13 | 0/3 | todo |  |
| 0x45E0A0 | `sub_45E0A0` | 10 |  | Source/Object_5C.hpp:222 | 0/3 | todo |  |
| 0x45E240 | `sub_45E240` | 36 |  |  | 0/1 | todo |  |
| 0x45E4F0 | `GameSession_578::sub_45E4F0` | 7 | 0x4C5950 MATCH |  | 0/2 | todo |  |
| 0x45E770 | `GameSession_578::sub_45E770` | 39 | 0x4C5C30 MATCH |  | 0/1 | todo |  |
| 0x45E8D0 | `sub_45E8D0` | 837 |  |  | 0/1 | todo |  |
| 0x45ED00 | `sub_45ED00` | 8 |  |  | 0/1 | todo |  |
| 0x45EDE0 | `IsField238_45EDE0` | 217 |  |  | 0/1 | todo |  |
| 0x45F9E0 | `sub_45F9E0` | 4 |  | Source/rng.hpp:20 | 0/1 | todo |  |
| 0x460CC0 | `sub_460CC0` | 105 |  |  | 0/1 | todo |  |
| 0x460D30 | `sub_460D30` | 105 |  |  | 0/1 | todo |  |
| 0x460DA0 | `KanjiFont_8EC::sub_460DA0` | 55 |  |  | 0/1 | todo |  |
| 0x460EC0 | `chunk::verify_version_460EC0` | 31 |  |  | 0/3 | todo |  |
| 0x460EE0 | `chunk::verify_type_460EE0` | 40 |  |  | 0/3 | todo |  |
| 0x461270 | `keybrd_0x204::destroy_461270` | 28 |  |  | 0/1 | todo |  |
| 0x461500 | `sub_461500` | 17 | 0x4D9670 MATCH |  | 0/1 | todo |  |
| 0x461910 | `sub_461910` | 30 |  |  | 0/1 | todo |  |
| 0x461DA0 | `sub_461DA0` | 7 |  | Source/NetPlay.hpp:256 | 0/1 | todo |  |
| 0x461DB0 | `sub_461DB0` | 7 |  | Source/Player.hpp:175 | 0/3 | todo |  |
| 0x461DC0 | `sub_461DC0` | 7 |  | Source/GameSession.hpp:97 | 0/2 | todo |  |
| 0x461DD0 | `sub_461DD0` | 12 |  | Source/TagGame.hpp:36 | 0/2 | todo |  |
| 0x462E40 | `Map_0x370::sub_462E40` | 22 |  |  | 0/9 | todo |  |
| 0x462FB0 | `gtx_0x106C::sub_462FB0` | 20 |  | Source/gtx_0x106C.hpp:232 | 0/1 | todo |  |
| 0x462FD0 | `gtx_0x106C::sub_462FD0` | 26 |  | Source/gtx_0x106C.hpp:244 | 0/4 | todo |  |
| 0x463020 | `gmp_map_zone::sub_463020` | 88 |  | Source/map_0x370.hpp:203 | 0/3 | todo |  |
| 0x463080 | `sub_463080` | 23 |  | Source/map_0x370.hpp:161 | 0/1 | todo |  |
| 0x4630A0 | `sub_4630A0` | 13 |  | Source/map_0x370.hpp:177 | 0/1 | todo |  |
| 0x4630B0 | `sub_4630B0` | 29 |  | Source/map_0x370.hpp:132 | 0/1 | todo |  |
| 0x463150 | `sub_463150` | 162 |  |  | 0/1 | todo |  |
| 0x463210 | `sub_463210` | 189 |  |  | 0/1 | todo |  |
| 0x4632E0 | `ProcessObjective_4632E0` | 176 |  |  | 0/1 | todo |  |
| 0x463480 | `sub_463480` | 42 |  | Source/map_0x370.hpp:751 | 0/1 | todo |  |
| 0x4634B0 | `sub_4634B0` | 42 |  | Source/map_0x370.hpp:739, Source/map_0x370.hpp:751 | 0/1 | todo |  |
| 0x4634E0 | `sub_4634E0` | 76 |  | Source/map_0x370.hpp:601 | 0/3 | todo |  |
| 0x463530 | `sub_463530` | 53 |  |  | 0/1 | todo |  |
| 0x463690 | `sub_463690` | 124 |  | Source/Fix16_Rect.hpp:101, Source/map_0x370.cpp:1608 | 0/1 | todo |  |
| 0x463710 | `sub_463710` | 70 |  | Source/Fix16_Rect.hpp:104 | 0/2 | todo |  |
| 0x463760 | `sub_463760` | 56 |  | Source/Fix16_Rect.hpp:95 | 0/1 | todo |  |
| 0x4637A0 | `sub_4637A0` | 12 |  | Source/Fix16_Rect.hpp:186 | 0/1 | todo |  |
| 0x4637B0 | `Reset_4637B0` | 11 |  | Source/CollisionSoundQueue.hpp:18 | 0/4 | todo |  |
| 0x463F10 | `sub_463F10` | 55 |  | Source/Light.hpp:15 | 0/1 | todo |  |
| 0x464C40 | `Alloc_464C40` | 26 |  | Source/LightPool.hpp:46 | 0/1 | todo |  |
| 0x465090 | `Map_0x370::sub_465090` | 151 |  |  | 0/1 | todo |  |
| 0x4653C0 | `sub_4653C0` | 80 | 0x4DFE10 MATCH |  | 0/1 | todo |  |
| 0x466B70 | `sub_466B70` | 361 | 0x466B70 MATCH |  | 0/11 | todo |  |
| 0x466CF0 | `sub_466CF0` | 53 |  | Source/map_0x370.hpp:714 | 0/18 | todo |  |
| 0x467110 | `sub_467110` | 3660 |  |  | 0/2 | todo |  |
| 0x469010 | `sub_469010` | 84 | 0x52B2A0 MATCH |  | 0/1 | todo |  |
| 0x469070 | `Light_1D4CC::SetFlashing_469070` | 54 |  | Source/LightPool.hpp:56 | 0/2 | todo |  |
| 0x4692B0 | `Map_0x370::sub_4692B0` | 237 |  |  | 0/1 | todo |  |
| 0x4699A0 | `sub_4699A0` | 346 | 0x4E4F40 MATCH |  | 0/2 | todo |  |
| 0x469DC0 | `sub_469DC0` | 455 |  |  | 0/1 | todo |  |
| 0x46A4D0 | `Map_0x370::sub_46A4D0` | 155 |  |  | 0/1 | todo |  |
| 0x46B5E0 | `Nanobotz::GetColour_46B5E0` | 44 |  | Source/MapRenderer.hpp:134 | 0/1 | todo |  |
| 0x46BB50 | `TextureCache_15D8::sub_46BB50` | 15 |  | Source/TextureCache.hpp:85 | 0/3 | todo |  |
| 0x46BBF0 | `sub_46BBF0` | 116 | 0x4EAD90 MATCH |  | 0/1 | todo |  |
| 0x46BD40 | `Nanobotz::ProjectVertTop_46BD40` | 168 | 0x4EAE00 MATCH | Source/MapRenderer.cpp:117 | 0/1 | todo |  |
| 0x46BEA0 | `sub_46BEA0` | 531 | 0x4F3FB0 MATCH |  | 0/1 | todo |  |
| 0x46C0C0 | `Nanobotz::Set_UV_46C0C0` | 115 | 0x4F4190 MATCH |  | 0/1 | matched |  |
| 0x46C2C0 | `Nanobotz::sub_46C2C0` | 1324 |  |  | 0/1 | todo |  |
| 0x46C7F0 | `Nanobotz::sub_46C7F0` | 1600 |  |  | 0/1 | todo |  |
| 0x46CE30 | `Nanobotz::sub_46CE30` | 1325 |  |  | 0/1 | todo |  |
| 0x46DFE0 | `Nanobotz::sub_46DFE0` | 723 |  |  | 0/1 | todo |  |
| 0x46E490 | `Nanobotz::sub_46E490` | 297 |  |  | 0/1 | todo |  |
| 0x46E5C0 | `Nanobotz::sub_46E5C0` | 281 |  |  | 0/1 | todo |  |
| 0x46E6E0 | `Nanobotz::sub_46E6E0` | 281 |  |  | 0/1 | todo |  |
| 0x46E800 | `Nanobotz::sub_46E800` | 265 |  |  | 0/1 | todo |  |
| 0x46E910 | `Nanobotz::sub_46E910` | 282 |  |  | 0/1 | todo |  |
| 0x46EA30 | `Nanobotz::sub_46EA30` | 298 |  |  | 0/1 | todo |  |
| 0x46EB60 | `Nanobotz::SpawnDummies_46EB60` | 298 |  |  | 0/1 | todo |  |
| 0x46EC90 | `Nanobotz::sub_46EC90` | 314 |  |  | 0/1 | todo |  |
| 0x46EF10 | `Nanobotz::DrawGradientSlopeNorthwards_46EF10` | 1112 |  | Source/MapRenderer.cpp:1840 | 0/1 | todo |  |
| 0x46F370 | `Nanobotz::sub_46F370` | 1096 |  |  | 0/1 | todo |  |
| 0x46F7C0 | `Nanobotz::sub_46F7C0` | 1099 |  |  | 0/1 | todo |  |
| 0x46FC10 | `Nanobotz::sub_46FC10` | 1102 |  |  | 0/1 | todo |  |
| 0x470060 | `Nanobotz::sub_470060` | 482 |  |  | 0/1 | todo |  |
| 0x470440 | `Nanobotz::sub_470440` | 478 |  |  | 0/1 | todo |  |
| 0x470620 | `Nanobotz::sub_470620` | 478 |  |  | 0/1 | todo |  |
| 0x472110 | `Nanobotz::Draw_472110` | 1108 |  |  | 0/1 | todo |  |
| 0x4725B0 | `sub_4725B0` | 87 | 0x4F7940 MATCH |  | 0/1 | todo |  |
| 0x4727E0 | `sub_4727E0` | 362 |  |  | 0/1 | todo |  |
| 0x472950 | `sub_472950` | 389 |  |  | 0/1 | todo |  |
| 0x472AE0 | `sub_472AE0` | 281 |  |  | 0/1 | todo |  |
| 0x472C00 | `sub_472C00` | 96 | 0x4F78F0 MATCH |  | 0/1 | todo |  |
| 0x472C60 | `sub_472C60` | 90 |  |  | 0/1 | todo |  |
| 0x472FD0 | `sub_472FD0` | 11 |  | Source/Ped.hpp:758 | 0/2 | todo |  |
| 0x4740F0 | `sub_4740F0` | 7 |  |  | 0/1 | todo |  |
| 0x4741F0 | `sub_4741F0` | 542 |  |  | 0/1 | todo |  |
| 0x474430 | `sub_474430` | 17 |  | Source/FrameProfiler.hpp:54 | 0/1 | todo |  |
| 0x474450 | `sub_474450` | 55 |  | Source/FrameProfiler.hpp:25 | 0/1 | todo |  |
| 0x474490 | `sub_474490` | 25 |  |  | 0/1 | todo |  |
| 0x4744C0 | `sub_4744C0` | 97 |  |  | 0/1 | todo |  |
| 0x4754D0 | `sub_4754D0` | 9 |  | Source/fix16.hpp:410 | 0/1 | todo |  |
| 0x4758A0 | `sub_4758A0` | 13 |  | Source/Gang.hpp:25 | 0/1 | todo |  |
| 0x4758B0 | `sub_4758B0` | 13 |  | Source/Gang.hpp:31 | 0/1 | todo |  |
| 0x4758C0 | `sub_4758C0` | 13 |  | Source/Gang.hpp:37 | 0/1 | todo |  |
| 0x4758D0 | `sub_4758D0` | 13 |  | Source/Gang.hpp:43 | 0/1 | todo |  |
| 0x4758E0 | `sub_4758E0` | 13 |  | Source/Gang.hpp:49 | 0/1 | todo |  |
| 0x4758F0 | `sub_4758F0` | 13 |  | Source/Gang.hpp:55 | 0/1 | todo |  |
| 0x475900 | `sub_475900` | 13 |  | Source/Gang.hpp:61 | 0/2 | todo |  |
| 0x475910 | `sub_475910` | 33 |  | Source/Gang.hpp:67 | 0/1 | todo |  |
| 0x475940 | `sub_475940` | 13 |  | Source/Gang.hpp:75 | 0/1 | todo |  |
| 0x475950 | `sub_475950` | 13 |  | Source/Gang.hpp:81 | 0/1 | todo |  |
| 0x475980 | `sub_475980` | 19 |  | Source/ScriptManager.hpp:630 | 0/1 | todo |  |
| 0x4759A0 | `sub_4759A0` | 19 |  | Source/ScriptManager.hpp:636 | 0/4 | todo |  |
| 0x4759C0 | `sub_4759C0` | 19 |  | Source/ScriptManager.hpp:642 | 0/4 | todo |  |
| 0x475A20 | `sub_475A20` | 7 |  | Source/ScriptManager.hpp:648 | 0/2 | todo |  |
| 0x475A30 | `sub_475A30` | 7 |  | Source/ScriptManager.hpp:654 | 0/1 | todo |  |
| 0x475A40 | `sub_475A40` | 13 |  | Source/ScriptManager.hpp:660 | 0/2 | todo |  |
| 0x475A50 | `sub_475A50` | 10 |  |  | 0/2 | todo |  |
| 0x475A60 | `sub_475A60` | 11 |  | Source/Object_5C.hpp:164 | 0/1 | todo |  |
| 0x475A70 | `sub_475A70` | 11 |  | Source/Object_5C.hpp:158 | 0/1 | todo |  |
| 0x475A80 | `sub_475A80` | 32 |  | Source/Object_5C.hpp:171 | 0/2 | todo |  |
| 0x475AA0 | `sub_475AA0` | 51 |  | Source/Object_5C.hpp:264 | 0/1 | todo |  |
| 0x475AE0 | `sub_475AE0` | 10 |  |  | 0/1 | todo |  |
| 0x475AF0 | `sub_475AF0` | 7 |  | Source/Ped.hpp:864 | 0/2 | todo |  |
| 0x475B10 | `sub_475B10` | 7 |  | Source/Ped.hpp:148 | 0/2 | todo |  |
| 0x475B40 | `sub_475B40` | 11 |  | Source/Ped.hpp:142 | 0/1 | todo |  |
| 0x475B60 | `DrawUnk_0xBC::sub_475B60` | 8 |  | Source/Camera.hpp:105 | 0/7 | todo |  |
| 0x475B70 | `miss2_0x11C::sub_475B70` | 38 | 0x511930 MATCH |  | 0/1 | todo |  |
| 0x475C10 | `Car_BC::sub_475C10` | 20 |  | Source/Car.hpp:931 | 0/2 | todo |  |
| 0x475C30 | `sub_475C30` | 11 |  | Source/Car.hpp:970 | 0/4 | todo |  |
| 0x475C40 | `sub_475C40` | 20 |  | Source/Car.hpp:946 | 0/1 | todo |  |
| 0x475C80 | `sub_475C80` | 20 |  | Source/Car.hpp:961 | 0/1 | todo |  |
| 0x475CA0 | `sub_475CA0` | 138 |  |  | 0/1 | todo |  |
| 0x475E60 | `HasOtherCarOnTrailer_475E60` | 34 |  |  | 0/1 | todo |  |
| 0x476200 | `ScriptManager_C1EA8::sub_476200` | 42 | 0x512770 MATCH |  | 0/2 | todo |  |
| 0x476230 | `sub_476230` | 8 |  | Source/Garage.cpp:327 | 0/1 | todo |  |
| 0x476270 | `sub_476270` | 8 |  | Source/Car.hpp:955 | 0/2 | todo |  |
| 0x476360 | `sub_476360` | 8 |  | Source/Car.hpp:940 | 0/1 | todo |  |
| 0x4764A0 | `sub_4764A0` | 39 |  | Source/Car.hpp:234 | 0/1 | todo |  |
| 0x476610 | `sub_476610` | 18 |  |  | 0/1 | todo |  |
| 0x476660 | `sub_476660` | 22 |  | Source/BonusTracker.hpp:46 | 0/2 | todo |  |
| 0x476680 | `sub_476680` | 26 |  | Source/BonusTracker.hpp:52 | 0/1 | todo |  |
| 0x4766A0 | `angry_lewin_0x85C::sub_4766A0` | 11 |  | Source/Player.hpp:124 | 0/9 | todo |  |
| 0x4766B0 | `sub_4766B0` | 11 |  | Source/Player.hpp:86 | 0/3 | todo |  |
| 0x4766C0 | `sub_4766C0` | 11 |  | Source/Player.hpp:106 | 0/2 | todo |  |
| 0x4766D0 | `angry_lewin_0x85C::sub_4766D0` | 4 |  | Source/Player.hpp:56 | 0/4 | todo |  |
| 0x476700 | `sub_476700` | 40 |  | Source/Player.hpp:92 | 0/1 | todo |  |
| 0x476730 | `sub_476730` | 40 |  | Source/Player.hpp:99 | 0/1 | todo |  |
| 0x476790 | `sub_476790` | 4 |  | Source/Game.hpp:60 | 0/1 | todo |  |
| 0x4767A0 | `miss2_0x11C_Pool::Allocate_4767A0` | 24 |  | Source/ScriptThread.hpp:1173 | 0/2 | todo |  |
| 0x4767C0 | `sub_4767C0` | 31 |  | Source/Hud.hpp:445 | 0/1 | todo |  |
| 0x476830 | `sub_476830` | 8 |  | Source/Hud.hpp:428 | 0/1 | todo |  |
| 0x476840 | `sub_476840` | 8 |  | Source/Hud.hpp:588 | 0/2 | todo |  |
| 0x476850 | `sub_476850` | 8 |  | Source/Hud.hpp:563 | 0/1 | todo |  |
| 0x476860 | `sub_476860` | 8 |  | Source/Hud.hpp:569 | 0/2 | todo |  |
| 0x476870 | `sub_476870` | 8 |  | Source/Hud.hpp:575 | 0/1 | todo |  |
| 0x476880 | `sub_476880` | 20 |  | Source/Hud.hpp:581 | 0/1 | todo |  |
| 0x4768C0 | `sub_4768C0` | 23 |  | Source/Car.hpp:107 | 0/2 | todo |  |
| 0x476930 | `sub_476930` | 21 |  |  | 0/2 | todo |  |
| 0x476950 | `sub_476950` | 9 |  |  | 0/2 | todo |  |
| 0x476990 | `sub_476990` | 10 |  | Source/Door.hpp:94 | 0/2 | todo |  |
| 0x4769A0 | `sub_4769A0` | 21 |  |  | 0/1 | todo |  |
| 0x4769C0 | `sub_4769C0` | 21 |  |  | 0/2 | todo |  |
| 0x4769E0 | `sub_4769E0` | 21 |  |  | 0/2 | todo |  |
| 0x476A00 | `sub_476A00` | 14 |  |  | 0/2 | todo |  |
| 0x476A10 | `sub_476A10` | 10 |  |  | 0/2 | todo |  |
| 0x476A20 | `sub_476A20` | 16 |  | Source/ScriptThread.cpp:1007, Source/ScriptThread.cpp:1130 | 0/2 | todo |  |
| 0x476A30 | `sub_476A30` | 41 |  | Source/Door.hpp:78, Source/DoorPool.hpp:67 | 0/1 | todo |  |
| 0x476A90 | `sub_476A90` | 4 |  | Source/Door.hpp:53 | 0/1 | todo |  |
| 0x476AE0 | `sub_476AE0` | 22 |  | Source/Light.hpp:97 | 0/1 | todo |  |
| 0x476B00 | `sub_476B00` | 10 |  |  | 0/1 | todo |  |
| 0x476B10 | `sub_476B10` | 13 |  | Source/GameSession.hpp:30 | 0/1 | todo |  |
| 0x476D20 | `sub_476D20` | 44 |  |  | 0/1 | todo |  |
| 0x476DF0 | `sub_476DF0` | 11 |  |  | 0/1 | todo |  |
| 0x476E00 | `sub_476E00` | 11 |  |  | 0/1 | todo |  |
| 0x477530 | `sub_477530` | 36 |  |  | 0/1 | todo |  |
| 0x477660 | `miss2_0x11C::SCRCMD_CRANE_477660` | 374 |  |  | 0/1 | todo |  |
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
| 0x47D070 | `sub_47D070` | 251 |  | Source/ScriptThread.cpp:5450 | 0/1 | todo |  |
| 0x47DE70 | `sub_47DE70` | 456 |  |  | 0/1 | todo |  |
| 0x47E360 | `sub_47E360` | 592 |  |  | 0/1 | todo |  |
| 0x47ECB0 | `sub_47ECB0` | 36 |  |  | 0/1 | todo |  |
| 0x47EF10 | `sub_47EF10` | 42 |  |  | 0/1 | todo |  |
| 0x47F420 | `sub_47F420` | 34 | 0x512AA0 MATCH |  | 0/1 | todo |  |
| 0x47F4F0 | `sub_47F4F0` | 45 |  | Source/LightPool.hpp:132 | 0/2 | todo |  |
| 0x481310 | `miss2_0x11C_Pool::ctor_481310` | 111 |  | Source/ScriptThread.hpp:1179 | 0/1 | todo |  |
| 0x481900 | `ScriptManager_C1EA8::sub_481900` | 85 |  |  | 0/1 | todo |  |
| 0x481C10 | `miss2_0x11C_Pool::gdtor_481C10` | 30 |  |  | 0/1 | todo |  |
| 0x481D10 | `sub_481D10` | 18 |  |  | 0/1 | todo |  |
| 0x481D30 | `sub_481D30` | 77 | 0x513390 MATCH |  | 0/1 | todo |  |
| 0x481D80 | `sub_481D80` | 29 |  |  | 0/1 | todo |  |
| 0x481DA0 | `sub_481DA0` | 21 |  |  | 0/1 | todo |  |
| 0x481DC0 | `sub_481DC0` | 11 |  |  | 0/1 | todo |  |
| 0x481DF0 | `sub_481DF0` | 11 |  | Source/Bink.cpp:20 | 0/2 | todo |  |
| 0x482080 | `sub_482080` | 11 |  | Source/Ped.hpp:420 | 0/3 | todo |  |
| 0x482090 | `unknown_libname_24` | 10 |  |  | 0/1 | todo |  |
| 0x4820A0 | `SetTargetPlayer_4820A0` | 17 |  | Source/Hud.hpp:465 | 0/1 | todo |  |
| 0x4822A0 | `sub_4822A0` | 341 |  |  | 0/1 | todo |  |
| 0x482400 | `sub_482400` | 16 |  | Source/Object_5C.hpp:333 | 0/3 | todo |  |
| 0x482410 | `sub_482410` | 42 |  |  | 0/1 | todo |  |
| 0x4824E0 | `sub_4824E0` | 39 |  | Source/fix16.hpp:492 | 0/3 | todo |  |
| 0x482510 | `sub_482510` | 39 |  | Source/fix16.hpp:499 | 0/2 | todo |  |
| 0x482730 | `ZeroIfNegligible_482730` | 56 |  | Source/fix16.hpp:277 | 0/3 | todo |  |
| 0x482790 | `sub_482790` | 22 |  |  | 0/10 | todo |  |
| 0x4828C0 | `sub_4828C0` | 33 |  | Source/Fix16_Point.hpp:271 | 0/4 | todo |  |
| 0x4828F0 | `sub_4828F0` | 57 |  |  | 0/1 | todo |  |
| 0x482980 | `SpriteShape_4C::sub_482980` | 23 |  | Source/sprite.hpp:58 | 0/1 | todo |  |
| 0x4829A0 | `Object_2C_Pool::sub_4829A0` | 26 |  |  | 0/1 | todo |  |
| 0x4829C0 | `Object_2C_Pool::sub_4829C0` | 24 |  |  | 0/1 | todo |  |
| 0x482A30 | `sub_482A30` | 10 |  | Source/sprite.hpp:494 | 0/4 | todo |  |
| 0x482A40 | `sub_482A40` | 37 |  | Source/sprite.hpp:534 | 0/1 | todo |  |
| 0x482A70 | `SetFloorBelow_482A70` | 14 |  | Source/CollisionSoundQueue.hpp:32 | 0/1 | todo |  |
| 0x482A80 | `SetBlockAtZ_482A80` | 14 |  | Source/CollisionSoundQueue.hpp:39 | 0/1 | todo |  |
| 0x482A90 | `sub_482A90` | 10 |  | Source/Explosion.hpp:37 | 0/1 | todo |  |
| 0x482BA0 | `sub_482BA0` | 43 |  | Source/Object_3C.cpp:72, Source/Object_3C.hpp:102 | 0/2 | todo |  |
| 0x482BD0 | `sub_482BD0` | 5 |  | Source/Object_3C.hpp:94 | 0/1 | todo |  |
| 0x482BE0 | `sub_482BE0` | 16 |  | Source/Object_5C.hpp:345 | 0/2 | todo |  |
| 0x482BF0 | `sub_482BF0` | 11 |  | Source/Object_5C.hpp:360 | 0/1 | todo |  |
| 0x482C00 | `sub_482C00` | 10 |  | Source/Object_5C.hpp:222 | 0/1 | todo |  |
| 0x482C10 | `sub_482C10` | 23 |  | Source/Object_5C.cpp:98, Source/Object_5C.hpp:351 | 0/2 | todo |  |
| 0x482C30 | `sub_482C30` | 20 |  |  | 0/3 | todo |  |
| 0x482C80 | `sub_482C80` | 15 |  | Source/Object_5C.hpp:339 | 0/4 | todo |  |
| 0x482C90 | `sub_482C90` | 42 |  | Source/Object_5C.hpp:188 | 0/4 | todo |  |
| 0x482CC0 | `sub_482CC0` | 50 |  | Source/CarPhysics.hpp:339 | 0/1 | todo |  |
| 0x482D30 | `SetPosition_482D30` | 40 |  | Source/Light.hpp:104 | 0/1 | todo |  |
| 0x482D60 | `SetColourRadiusIntensity_482D60` | 43 |  | Source/Light.hpp:88 | 0/3 | todo |  |
| 0x482F60 | `sub_482F60` | 30 |  |  | 0/4 | todo |  |
| 0x482F80 | `sub_482F80` | 30 |  |  | 0/3 | todo |  |
| 0x483100 | `sub_483100` | 254 |  |  | 0/4 | todo |  |
| 0x483500 | `sub_483500` | 108 |  |  | 0/3 | todo |  |
| 0x483570 | `sub_483570` | 105 |  |  | 0/1 | todo |  |
| 0x483A00 | `sub_483A00` | 8 |  |  | 0/4 | todo |  |
| 0x483C00 | `sub_483C00` | 27 |  |  | 0/1 | todo |  |
| 0x483C20 | `sub_483C20` | 28 |  |  | 0/2 | todo |  |
| 0x483C60 | `sub_483C60` | 19 | 0x5291E0 MATCH |  | 0/1 | todo |  |
| 0x483C80 | `sub_483C80` | 37 |  |  | 0/1 | todo |  |
| 0x483D90 | `sub_483D90` | 177 |  |  | 0/1 | todo |  |
| 0x483F10 | `Object_2C_Pool::ctor_483F10` | 72 |  |  | 0/1 | todo |  |
| 0x483F60 | `Object_8_Pool::ctor_483F60` | 64 |  |  | 0/1 | todo |  |
| 0x483FA0 | `Object_8_Pool::sub_483FA0` | 18 |  |  | 0/2 | todo |  |
| 0x483FC0 | `sub_483FC0` | 26 |  |  | 0/2 | todo |  |
| 0x483FE0 | `Object_3C_Pool::sub_483FE0` | 19 |  |  | 0/4 | todo |  |
| 0x484000 | `sub_484000` | 27 |  |  | 0/2 | todo |  |
| 0x484260 | `sub_484260` | 311 |  |  | 0/1 | todo |  |
| 0x4847D0 | `sub_4847D0` | 75 |  | Source/Object_5C.cpp:2264 | 0/2 | todo |  |
| 0x484820 | `Object_2C_Pool::gdtor_484820` | 30 |  |  | 0/1 | todo |  |
| 0x484840 | `Object_8_Pool::gdtor_484840` | 30 |  |  | 0/1 | todo |  |
| 0x484860 | `Object_3C_Pool::gdtor_484860` | 30 |  |  | 0/1 | todo |  |
| 0x484880 | `sub_484880` | 61 |  |  | 0/1 | todo |  |
| 0x4848C0 | `Object_3C_Pool::ctor_4848C0` | 66 |  |  | 0/1 | todo |  |
| 0x484D60 | `Object_2C_Pool::sub_484D60` | 80 |  |  | 0/1 | todo |  |
| 0x484DB0 | `Object_2C_Pool::sub_484DB0` | 26 |  |  | 0/2 | todo |  |
| 0x484DD0 | `HandleCollisionWithObject_484DD0` | 46 |  |  | 0/1 | todo |  |
| 0x485260 | `sub_485260` | 45 |  |  | 0/3 | todo |  |
| 0x4852E0 | `Object_5C::sub_4852E0` | 52 | 0x5299B0 MATCH |  | 0/2 | todo |  |
| 0x485540 | `sub_485540` | 231 | 0x52A3D0 MATCH |  | 0/1 | todo |  |
| 0x4867E0 | `sub_4867E0` | 366 |  | Source/Object_5C.cpp:451 | 0/2 | todo |  |
| 0x487F50 | `Object_5C::sub_487F50` | 16 |  |  | 0/1 | todo |  |
| 0x488170 | `ObjectDefinitions_8CA8::sub_488170` | 14 | 0x534360 MATCH |  | 0/1 | todo |  |
| 0x488200 | `sub_488200` | 10 |  |  | 0/1 | todo |  |
| 0x488EF0 | `ObjectDefinition_74::sub_488EF0` | 30 |  |  | 0/1 | todo |  |
| 0x4895D0 | `sub_4895D0` | 16 |  | Source/CarPhysics.hpp:117 | 0/1 | todo |  |
| 0x4895E0 | `sub_4895E0` | 8 |  | Source/Garage.cpp:327 | 0/1 | todo |  |
| 0x4895F0 | `sub_4895F0` | 5 |  | Source/Door.hpp:81 | 0/1 | todo |  |
| 0x489600 | `sub_489600` | 18 |  | Source/Door.hpp:87 | 0/1 | todo |  |
| 0x489620 | `sub_489620` | 6 |  | Source/DoorPool.hpp:45 | 0/1 | todo |  |
| 0x489630 | `sub_489630` | 6 |  | Source/DoorPool.hpp:39 | 0/1 | todo |  |
| 0x489640 | `sub_489640` | 6 |  | Source/DoorPool.hpp:33 | 0/1 | todo |  |
| 0x489650 | `sub_489650` | 44 |  | Source/Garage.hpp:37 | 0/1 | todo |  |
| 0x489AC0 | `sub_489AC0` | 79 |  |  | 0/1 | todo |  |
| 0x48A250 | `sub_48A250` | 30 |  | Source/Fix16_Point.hpp:323 | 0/1 | todo |  |
| 0x48A270 | `sub_48A270` | 78 |  | Source/Fix16_Point.hpp:311 | 0/1 | todo |  |
| 0x48A390 | `sub_48A390` | 99 |  |  | 0/1 | todo |  |
| 0x48A420 | `sub_48A420` | 54 |  |  | 0/1 | todo |  |
| 0x48A4C0 | `sub_48A4C0` | 4 |  | Source/char.hpp:160 | 0/1 | todo |  |
| 0x48A550 | `Explosion_30::sub_48A550` | 60 |  |  | 0/2 | todo |  |
| 0x48A8D0 | `sub_48A8D0` | 7 |  | Source/sprite.hpp:824 | 0/1 | todo |  |
| 0x48A8F0 | `sub_48A8F0` | 7 |  | Source/Pool.hpp:58 | 0/5 | todo |  |
| 0x48A900 | `sub_48A900` | 28 |  |  | 0/5 | todo |  |
| 0x48A930 | `sub_48A930` | 20 |  |  | 0/1 | todo |  |
| 0x48A950 | `sub_48A950` | 20 |  |  | 0/1 | todo |  |
| 0x48D1F0 | `sub_48D1F0` | 745 |  | Source/ParticleManager.cpp:274 | 0/2 | todo |  |
| 0x48D4E0 | `sub_48D4E0` | 969 |  | Source/ParticleManager.cpp:332 | 0/1 | todo |  |
| 0x48DDC0 | `sub_48DDC0` | 509 |  |  | 0/1 | todo |  |
| 0x48E480 | `sub_48E480` | 355 |  |  | 0/1 | todo |  |
| 0x48E5F0 | `sub_48E5F0` | 351 |  | Source/Explosion.cpp:206 | 0/1 | todo |  |
| 0x48E750 | `sub_48E750` | 351 |  |  | 0/1 | todo |  |
| 0x48EA50 | `sub_48EA50` | 166 |  |  | 0/1 | todo |  |
| 0x48F1E0 | `Particle_4C_Pool::ctor_48F1E0` | 69 |  |  | 0/1 | todo |  |
| 0x491550 | `sub_491550` | 1288 |  |  | 0/1 | todo |  |
| 0x491CE0 | `Particle_8::sub_491CE0` | 11 |  |  | 0/1 | todo |  |
| 0x491EA0 | `sub_491EA0` | 30 |  |  | 0/2 | todo |  |
| 0x491EE0 | `sub_491EE0` | 6 |  |  | 0/4 | todo |  |
| 0x491EF0 | `sub_491EF0` | 9 |  | Source/fix16.hpp:485 | 0/2 | todo |  |
| 0x491F00 | `sub_491F00` | 9 |  | Source/fix16.hpp:478 | 0/2 | todo |  |
| 0x491F10 | `sub_491F10` | 93 |  | Source/char.cpp:2603 | 0/1 | todo |  |
| 0x491F80 | `sub_491F80` | 20 |  | Source/gtx_0x106C.hpp:411 | 0/3 | todo |  |
| 0x491FA0 | `sub_491FA0` | 204 |  |  | 0/1 | todo |  |
| 0x4920A0 | `sub_4920A0` | 131 |  |  | 0/1 | todo |  |
| 0x492130 | `sub_492130` | 8 |  | Source/map_0x370.hpp:733 | 0/1 | todo |  |
| 0x492140 | `sub_492140` | 41 |  | Source/map_0x370.hpp:726 | 0/1 | todo |  |
| 0x492170 | `sub_492170` | 12 |  | Source/sprite.hpp:107 | 0/3 | todo |  |
| 0x492180 | `Char_B4::sub_492180` | 10 |  | Source/sprite.hpp:506 | 0/1 | todo |  |
| 0x492190 | `sub_492190` | 509 |  |  | 0/2 | todo |  |
| 0x4923A0 | `sub_4923A0` | 42 |  | Source/char.hpp:266 | 0/1 | todo |  |
| 0x4923D0 | `sub_4923D0` | 42 |  | Source/char.hpp:257 | 0/1 | todo |  |
| 0x492400 | `sub_492400` | 25 |  | Source/char.cpp:2621 | 0/1 | todo |  |
| 0x492C20 | `sub_492C20` | 7 |  | Source/Ped.hpp:734 | 0/1 | todo |  |
| 0x492C30 | `sub_492C30` | 128 |  |  | 0/1 | todo |  |
| 0x492CB0 | `sub_492CB0` | 13 |  | Source/Ped.hpp:478 | 0/1 | todo |  |
| 0x492CC0 | `sub_492CC0` | 17 |  | Source/Ped.hpp:728 | 0/2 | todo |  |
| 0x492CE0 | `sub_492CE0` | 15 |  | Source/Ped.hpp:689 | 0/2 | todo |  |
| 0x492CF0 | `sub_492CF0` | 15 |  | Source/Ped.hpp:695 | 0/2 | todo |  |
| 0x492FD0 | `sub_492FD0` | 29 |  | Source/Ped.hpp:846 | 0/1 | todo |  |
| 0x493090 | `sub_493090` | 24 |  | Source/Object_5C.hpp:354 | 0/2 | todo |  |
| 0x4932D0 | `sub_4932D0` | 158 | 0x545430 MATCH |  | 0/1 | todo |  |
| 0x493540 | `sub_493540` | 14 |  | Source/Garage.hpp:52 | 0/1 | todo |  |
| 0x4937D0 | `sub_4937D0` | 62 |  |  | 0/2 | todo |  |
| 0x493810 | `sub_493810` | 62 |  |  | 0/2 | todo |  |
| 0x493940 | `sub_493940` | 1706 |  |  | 0/3 | todo |  |
| 0x494280 | `sub_494280` | 1492 |  |  | 0/1 | todo |  |
| 0x495220 | `sub_495220` | 541 | 0x54C1A0 MATCH |  | 0/1 | todo |  |
| 0x495470 | `sub_495470` | 195 | 0x495470 MATCH | Source/char.cpp:2778 | 0/1 | todo |  |
| 0x4955F0 | `sub_4955F0` | 267 |  |  | 0/1 | todo |  |
| 0x496500 | `sub_496500` | 751 |  |  | 0/1 | todo |  |
| 0x4994D0 | `sub_4994D0` | 190 |  |  | 0/1 | todo |  |
| 0x4995A0 | `sub_4995A0` | 2313 |  |  | 0/1 | todo |  |
| 0x49A080 | `sub_49A080` | 1244 |  |  | 0/2 | todo |  |
| 0x49C700 | `PathFinder_2FD4::init_49C700` | 49 |  |  | 0/1 | todo |  |
| 0x49E0E0 | `sub_49E0E0` | 22 |  | Source/CarPhysics.cpp:2428 | 0/1 | todo |  |
| 0x49E240 | `sub_49E240` | 48 |  | Source/CarPhysics.cpp:224, Source/ang16.hpp:86 | 0/1 | todo |  |
| 0x49E330 | `sub_49E330` | 38 |  |  | 0/3 | todo |  |
| 0x49E360 | `sub_49E360` | 57 |  |  | 0/3 | todo |  |
| 0x49E3A0 | `sub_49E3A0` | 30 |  | Source/Fix16_Point.hpp:279 | 0/2 | todo |  |
| 0x49E3C0 | `ApplyDeadZone_49E3C0` | 82 |  | Source/Fix16_Point.hpp:114 | 0/1 | todo |  |
| 0x49E420 | `sub_49E420` | 35 |  |  | 0/1 | todo |  |
| 0x49E450 | `sub_49E450` | 42 |  | Source/Fix16_Point.hpp:108 | 0/1 | todo |  |
| 0x49E480 | `sub_49E480` | 114 |  | Source/Fix16_Point.hpp:356 | 0/1 | todo |  |
| 0x49E500 | `sub_49E500` | 62 |  |  | 0/1 | todo |  |
| 0x49E540 | `IsRemappedWaterTile_49E540` | 43 |  | Source/gtx_0x106C.hpp:213 | 0/1 | todo |  |
| 0x49E5A0 | `sub_49E5A0` | 161 |  |  | 0/5 | todo |  |
| 0x49E820 | `sub_49E820` | 35 |  |  | 0/1 | todo |  |
| 0x49EA60 | `sub_49EA60` | 14 |  | Source/CarPhysics.hpp:415 | 0/1 | todo |  |
| 0x49EBE0 | `sub_49EBE0` | 149 |  | Source/CarPhysics.cpp:1161, Source/map_0x370.hpp:449 | 0/1 | todo |  |
| 0x49ED00 | `sub_49ED00` | 81 |  | Source/CarPhysics.cpp:3768 | 0/1 | todo |  |
| 0x49ED60 | `CarPhysics_B0::sub_49ED60` | 87 | 0x563350 MATCH |  | 0/1 | todo |  |
| 0x49EE10 | `CarPhysics_B0::sub_49EE10` | 61 | 0x563670 MATCH |  | 0/3 | todo |  |
| 0x49EE80 | `sub_49EE80` | 44 |  |  | 0/1 | todo |  |
| 0x49EF10 | `sub_49EF10` | 10 |  | Source/CollisionSoundQueue.hpp:26 | 0/1 | todo |  |
| 0x49EF20 | `IsCharB4_49EF20` | 23 |  | Source/CollisionSoundQueue.hpp:72 | 0/1 | todo |  |
| 0x49EF40 | `sub_49EF40` | 7 |  | Source/Ped.hpp:484 | 0/1 | todo |  |
| 0x49EF50 | `sub_49EF50` | 34 |  | Source/CarPhysics.cpp:194, Source/CarPhysics.hpp:375 | 0/3 | todo |  |
| 0x49EFB0 | `sub_49EFB0` | 3 |  | Source/Car.hpp:363 | 0/1 | todo |  |
| 0x49EFC0 | `SetCrushFlag_49EFC0` | 11 |  | Source/Car.hpp:1388 | 0/1 | todo |  |
| 0x49EFD0 | `sub_49EFD0` | 11 |  | Source/Car.hpp:1394 | 0/1 | todo |  |
| 0x49EFE0 | `CanCrushCars_49EFE0` | 42 |  | Source/Car.hpp:1376 | 0/1 | todo |  |
| 0x49F170 | `sub_49F170` | 55 |  |  | 0/1 | todo |  |
| 0x49F4E0 | `sub_49F4E0` | 14 |  | Source/CarPhysics.hpp:421 | 0/1 | todo |  |
| 0x49F4F0 | `sub_49F4F0` | 14 |  | Source/CarPhysics.hpp:427 | 0/1 | todo |  |
| 0x49F500 | `unknown_libname_25` | 10 |  | Source/CarPhysics.hpp:433 | 0/1 | todo |  |
| 0x49F510 | `sub_49F510` | 94 |  |  | 0/1 | todo |  |
| 0x49F930 | `sub_49F930` | 392 |  |  | 0/1 | todo |  |
| 0x49FF70 | `sub_49FF70` | 5 |  |  | 0/1 | todo |  |
| 0x4A0290 | `sub_4A0290` | 713 |  |  | 0/1 | todo |  |
| 0x4A0900 | `sub_4A0900` | 45 |  |  | 0/1 | todo |  |
| 0x4A09B0 | `sub_4A09B0` | 116 |  |  | 0/2 | todo |  |
| 0x4A0D40 | `sub_4A0D40` | 482 |  | Source/CarPhysics.cpp:2967 | 0/2 | todo |  |
| 0x4A0F30 | `sub_4A0F30` | 509 |  | Source/CarPhysics.cpp:3025 | 0/2 | todo |  |
| 0x4A1D30 | `sub_4A1D30` | 39 |  |  | 0/1 | todo |  |
| 0x4A2240 | `sub_4A2240` | 1009 |  | Source/CarPhysics.cpp:1034, Source/CarPhysics.cpp:1040 | 0/1 | todo |  |
| 0x4A2E00 | `sub_4A2E00` | 40 |  |  | 0/1 | todo |  |
| 0x4A4750 | `sub_4A4750` | 12 |  | Source/Game.hpp:42 | 0/1 | todo |  |
| 0x4A4760 | `sub_4A4760` | 9 |  | Source/Hud.hpp:183 | 0/1 | todo |  |
| 0x4A4770 | `sub_4A4770` | 15 |  | Source/Hud.hpp:688 | 0/1 | todo |  |
| 0x4A4940 | `sub_4A4940` | 105 |  |  | 0/1 | todo |  |
| 0x4A49B0 | `angry_lewin_0x85C::sub_4A49B0` | 11 |  |  | 0/1 | todo |  |
| 0x4A4D50 | `sub_4A4D50` | 35 |  |  | 0/3 | todo |  |
| 0x4A4F10 | `sub_4A4F10` | 11 |  |  | 0/1 | todo |  |
| 0x4A4F20 | `Weapon_30_Pool::DeAllocate_4A4F20` | 86 |  | Source/WeaponManager.hpp:63 | 0/1 | todo |  |
| 0x4A4F80 | `keen_bhaskara_0x30::sub_4A4F80` | 8 |  | Source/Weapon.hpp:145 | 0/2 | todo |  |
| 0x4A4F90 | `sub_4A4F90` | 6 |  | Source/Weapon.hpp:28 | 0/3 | todo |  |
| 0x4A4FA0 | `Weapon_30::sub_4A4FA0` | 9 |  | Source/Weapon.hpp:87 | 0/1 | todo |  |
| 0x4A4FB0 | `sub_4A4FB0` | 45 |  | Source/Weapon.hpp:93 | 0/2 | todo |  |
| 0x4A4FE0 | `sub_4A4FE0` | 4 |  | Source/Weapon.hpp:22 | 0/2 | todo |  |
| 0x4A4FF0 | `sub_4A4FF0` | 11 |  | Source/Weapon.hpp:151 | 0/1 | todo |  |
| 0x4A5010 | `cool_nash_0x294::sub_4A5010` | 16 |  | Source/Ped.hpp:834 | 0/1 | todo |  |
| 0x4A5020 | `sub_4A5020` | 7 |  | Source/Ped.hpp:154 | 0/1 | todo |  |
| 0x4A5030 | `sub_4A5030` | 11 |  |  | 0/1 | todo |  |
| 0x4A5040 | `cool_nash_0x294::not_enter_car_as_passenger_4A5040` | 11 |  | Source/Ped.hpp:568 | 0/3 | todo |  |
| 0x4A5050 | `sub_4A5050` | 8 |  | Source/Ped.hpp:414 | 0/1 | todo |  |
| 0x4A5060 | `sub_4A5060` | 11 |  | Source/Ped.hpp:408 | 0/2 | todo |  |
| 0x4A5070 | `sub_4A5070` | 8 |  | Source/Camera.hpp:111 | 0/3 | todo |  |
| 0x4A5090 | `sub_4A5090` | 28 |  |  | 0/1 | todo |  |
| 0x4A50B0 | `sub_4A50B0` | 38 |  | Source/RollingDigitCounter.hpp:24 | 0/3 | todo |  |
| 0x4A50E0 | `sub_4A50E0` | 5 |  | Source/PlayerScoreTracker.hpp:14 | 0/2 | todo |  |
| 0x4A5100 | `angry_lewin_0x85C::IsRemoteControlActive_4A5100` | 36 |  | Source/Player.hpp:205 | 0/2 | todo |  |
| 0x4A5130 | `angry_lewin_0x85C::Get_Field_68_Ped_4A5130` | 20 |  | Source/Player.hpp:305 | 0/10 | todo |  |
| 0x4A5150 | `sub_4A5150` | 27 |  | Source/Player.hpp:323 | 0/2 | todo |  |
| 0x4A5170 | `sub_4A5170` | 13 |  | Source/Player.hpp:62 | 0/1 | todo |  |
| 0x4A5180 | `ResetThrowCharge_4A5180` | 8 |  | Source/Player.hpp:193 | 0/1 | todo |  |
| 0x4A51A0 | `GetRaw_4A5190` | 11 |  | Source/BitSet32.hpp:90 | 0/1 | todo |  |
| 0x4A51B0 | `sub_4A51B0` | 12 |  | Source/Car.hpp:500 | 0/1 | todo |  |
| 0x4A53D0 | `sub_4A53D0` | 38 |  |  | 0/1 | todo |  |
| 0x4A5600 | `angry_lewin_0x85C::sub_4A5600` | 59 |  |  | 0/1 | todo |  |
| 0x4A6100 | `sub_4A6100` | 182 |  |  | 0/1 | todo |  |
| 0x4A6530 | `sub_4A6530` | 171 |  |  | 0/1 | todo |  |
| 0x4A65E0 | `sub_4A65E0` | 35 |  |  | 0/5 | todo |  |
| 0x4A6FC0 | `sub_4A6FC0` | 15 |  |  | 0/1 | todo |  |
| 0x4A6FD0 | `angry_lewin_0x85C::sub_4A6FD0` | 62 |  |  | 0/1 | todo |  |
| 0x4A8910 | `sub_4A8910` | 199 |  |  | 0/1 | todo |  |
| 0x4A8B60 | `sub_4A8B60` | 23 | 0x56BCF0 MATCH |  | 0/1 | todo |  |
| 0x4A9050 | `sub_4A9050` | 69 |  |  | 0/1 | todo |  |
| 0x4A9270 | `sub_4A9270` | 28 |  |  | 0/1 | todo |  |
| 0x4A9AD0 | `sub_4A9AD0` | 5 |  | Source/Car.hpp:1311 | 0/4 | todo |  |
| 0x4AF290 | `sub_4AF290` | 31 |  |  | 0/1 | todo |  |
| 0x4AF860 | `sub_4AF860` | 23 |  | Source/Ped.hpp:65 | 0/1 | todo |  |
| 0x4AF880 | `sub_4AF880` | 23 |  | Source/Ped.hpp:65 | 0/1 | todo |  |
| 0x4B25A0 | `sub_4B25A0` | 38 |  |  | 0/1 | todo |  |
| 0x4B25D0 | `sub_4B25D0` | 595 |  | Source/sound_obj.cpp:6801 | 0/1 | todo |  |
| 0x4B30A0 | `sub_4B30A0` | 56 |  |  | 0/1 | todo |  |
| 0x4B3110 | `GetBoundariesWidth_4B3110` | 24 |  | Source/Camera.hpp:181 | 0/1 | todo |  |
| 0x4B3130 | `GetBoundariesHeight_4B3130` | 24 |  | Source/Camera.hpp:187 | 0/1 | todo |  |
| 0x4B3230 | `sub_4B3230` | 440 |  | Source/Car.hpp:1524 | 0/1 | todo |  |
| 0x4B33F0 | `sub_4B33F0` | 145 |  |  | 0/1 | todo |  |
| 0x4B53B0 | `Registry::Get_Debug_Setting_4B53B0` | 108 | 0x586E90 MATCH |  | 0/1 | todo |  |
| 0x4B5420 | `sub_4B5420` | 102 |  |  | 0/1 | todo |  |
| 0x4B6020 | `sub_4B6020` | 26 | 0x58DBF0 MATCH |  | 0/1 | todo |  |
| 0x4B6110 | `sub_4B6110` | 25 |  |  | 0/1 | todo |  |
| 0x4B6130 | `sub_4B6130` | 25 |  |  | 0/1 | todo |  |
| 0x4B6150 | `sub_4B6150` | 25 |  |  | 0/1 | todo |  |
| 0x4B6170 | `sub_4B6170` | 25 |  |  | 0/1 | todo |  |
| 0x4B6190 | `sub_4B6190` | 25 |  |  | 0/1 | todo |  |
| 0x4B61E0 | `sub_4B61E0` | 25 |  |  | 0/1 | todo |  |
| 0x4B6200 | `sub_4B6200` | 25 |  |  | 0/1 | todo |  |
| 0x4B62B0 | `sub_4B62B0` | 28 |  |  | 0/1 | todo |  |
| 0x4B62D0 | `sub_4B62D0` | 28 |  |  | 0/1 | todo |  |
| 0x4B6320 | `sub_4B6320` | 28 |  |  | 0/1 | todo |  |
| 0x4B6340 | `sub_4B6340` | 28 |  |  | 0/1 | todo |  |
| 0x4B6360 | `sub_4B6360` | 28 |  |  | 0/1 | todo |  |
| 0x4B63B0 | `sub_4B63B0` | 28 |  |  | 0/1 | todo |  |
| 0x4B6700 | `sub_4B6700` | 168 |  |  | 0/1 | todo |  |
| 0x4B7580 | `sub_4B7580` | 8 |  |  | 0/1 | todo |  |
| 0x4B7590 | `Game_0x40::sub_4B7590` | 8 |  | Source/Game.hpp:54 | 0/1 | todo |  |
| 0x4B75A0 | `j_RollingDigitCounter_38::sub_41DC30` | 5 |  |  | 0/1 | todo |  |
| 0x4B8A60 | `sub_4B8A60` | 14 | 0x5935D0 MATCH |  | 0/2 | todo |  |
| 0x4B8FD0 | `CokeZero_FC::sub_4B8FD0` | 4 | 0x5935C0 MATCH | Source/Pool.hpp:64 | 0/2 | todo |  |
| 0x4B8FE0 | `CokeZero_FC::sub_4B8FE0` | 17 |  |  | 0/1 | todo |  |
| 0x4B9000 | `CokeZero_FC::sub_4B9000` | 69 |  |  | 0/1 | todo |  |
| 0x4B90E0 | `DrawUnk_0xBC::sub_4B90E0` | 243 |  | Source/Hud.cpp:111 | 0/1 | todo |  |
| 0x4B9260 | `CokeZero_100::sub_4B9260` | 8 |  |  | 0/1 | todo |  |
| 0x4B92B0 | `CokeZero_50::sub_4B92B0` | 387 |  | Source/ExplodingScore_100.cpp:390 | 0/1 | todo |  |
| 0x4B9490 | `CokeZero_100::ctor_4B9490` | 26 |  |  | 0/1 | todo |  |
| 0x4B99F0 | `Car_3C::sub_4B99F0` | 27 |  |  | 0/1 | todo |  |
| 0x4B9A30 | `sub_4B9A30` | 45 |  |  | 0/2 | todo |  |
| 0x4B9A80 | `sub_4B9A80` | 17 |  |  | 0/2 | todo |  |
| 0x4B9D50 | `sub_4B9D50` | 145 | 0x5A3100 MATCH |  | 0/1 | todo |  |
| 0x4B9E60 | `sub_4B9E60` | 147 |  |  | 0/1 | todo |  |
| 0x4B9F00 | `SpriteMaskCache_3CC::gdtor_4B9F00` | 30 |  |  | 0/1 | todo |  |
| 0x4B9F30 | `sub_4B9F30` | 12 |  |  | 0/1 | todo |  |
| 0x4B9F40 | `IsWaterBlockAt_4B9F40` | 54 |  | Source/map_0x370.hpp:594 | 0/1 | todo |  |
| 0x4B9F80 | `sub_4B9F80` | 65 |  | Source/sprite.cpp:1445 | 0/1 | todo |  |
| 0x4B9FD0 | `sub_4B9FD0` | 58 |  | Source/Fix16_Rect.hpp:144 | 0/1 | todo |  |
| 0x4BA070 | `sub_4BA070` | 38 |  | Source/sprite.hpp:66 | 0/1 | todo |  |
| 0x4BA0A0 | `sub_4BA0A0` | 80 |  | Source/sprite.hpp:151 | 0/2 | todo |  |
| 0x4BA220 | `sub_4BA220` | 10 |  | Source/sprite.hpp:204 | 0/2 | todo |  |
| 0x4BA230 | `sub_4BA230` | 21 |  | Source/sprite.cpp:375, Source/sprite.hpp:378 | 0/1 | todo |  |
| 0x4BA250 | `SetHorizontalSegment_4BA250` | 37 |  | Source/CollisionSoundQueue.hpp:62 | 0/1 | todo |  |
| 0x4BA280 | `SetVerticalSegment_4BA280` | 37 |  | Source/CollisionSoundQueue.hpp:52 | 0/1 | todo |  |
| 0x4BA390 | `sub_4BA390` | 34 |  | Source/Car.hpp:777 | 0/1 | todo |  |
| 0x4BA5E0 | `sub_4BA5E0` | 54 |  |  | 0/4 | todo |  |
| 0x4BA7E0 | `sub_4BA7E0` | 105 |  |  | 0/1 | todo |  |
| 0x4BAA70 | `sub_4BAA70` | 19 |  |  | 0/2 | todo |  |
| 0x4BB3C0 | `sub_4BB3C0` | 266 |  |  | 0/1 | todo |  |
| 0x4BB4D0 | `sub_4BB4D0` | 142 |  |  | 0/1 | todo |  |
| 0x4BB9C0 | `sub_4BB9C0` | 531 |  |  | 0/1 | todo |  |
| 0x4BBC80 | `sub_4BBC80` | 136 |  |  | 0/1 | todo |  |
| 0x4BC9A0 | `SpriteShapePool::ctor_4BC9A0` | 66 |  | Source/sprite.hpp:780 | 0/1 | todo |  |
| 0x4BC9F0 | `SpriteShapePool::sub_4BC9F0` | 19 |  | Source/sprite.hpp:793 | 0/2 | todo |  |
| 0x4BCA10 | `SpriteShapePool::sub_4BCA10` | 14 |  | Source/sprite.hpp:787 | 0/1 | todo |  |
| 0x4BCA20 | `SpritePool::ctor_4BCA20` | 66 |  | Source/sprite.hpp:805 | 0/1 | todo |  |
| 0x4BCD00 | `sub_4BCD00` | 668 |  |  | 0/2 | todo |  |
| 0x4BCFA0 | `sub_4BCFA0` | 683 |  |  | 0/2 | todo |  |
| 0x4BD490 | `sub_4BD490` | 381 |  |  | 0/1 | todo |  |
| 0x4BD670 | `sub_4BD670` | 545 |  |  | 0/10 | todo |  |
| 0x4BDC90 | `SpritePool::gdtor_4BDC90` | 30 |  |  | 0/1 | todo |  |
| 0x4BDCB0 | `SpriteShapePool::gdtor_4BDCB0` | 30 |  |  | 0/1 | todo |  |
| 0x4BDCF0 | `AttachedSpritePool::ctor_4BDCF0` | 66 |  | Source/sprite.hpp:757 | 0/1 | todo |  |
| 0x4BDDD0 | `sub_4BDDD0` | 276 |  |  | 0/2 | todo |  |
| 0x4BDEF0 | `sub_4BDEF0` | 43 |  |  | 0/6 | todo |  |
| 0x4BE060 | `Car_3C::sub_4BE060` | 1293 |  |  | 0/1 | todo |  |
| 0x4BE710 | `AttachedSpritePool::gdtor_4BE710` | 30 |  |  | 0/1 | todo |  |
| 0x4BE830 | `sub_4BE830` | 22 |  | Source/Object_5C.hpp:317 | 0/2 | todo |  |
| 0x4BE850 | `sub_4BE850` | 23 |  | Source/Object_5C.hpp:311 | 0/1 | todo |  |
| 0x4BE920 | `sub_4BE920` | 44 |  |  | 0/1 | todo |  |
| 0x4BE950 | `sub_4BE950` | 35 |  |  | 0/1 | todo |  |
| 0x4BE9C0 | `sub_4BE9C0` | 42 |  |  | 0/3 | todo |  |
| 0x4BEA60 | `sub_4BEA60` | 48 | 0x5A6AD0 MATCH |  | 0/1 | todo |  |
| 0x4BEA90 | `sub_4BEA90` | 45 |  |  | 0/2 | todo |  |
| 0x4BEC40 | `AttachedSpritePool::sub_4BEC40` | 8 |  |  | 0/2 | todo |  |
| 0x4BEC50 | `AttachedSpritePool::sub_4BEC50` | 14 |  |  | 0/10 | todo |  |
| 0x4BED60 | `sub_4BED60` | 43 |  |  | 0/6 | todo |  |
| 0x4BEE10 | `sub_4BEE10` | 31 |  |  | 0/2 | todo |  |
| 0x4BF210 | `sub_4BF210` | 27 | 0x5AA3D0 MATCH |  | 0/1 | todo |  |
| 0x4BF550 | `sub_4BF550` | 25 |  |  | 0/3 | todo |  |
| 0x4BF790 | `sub_4BF790` | 25 |  |  | 0/2 | todo |  |
| 0x4BFFE0 | `sub_4BFFE0` | 44 |  |  | 0/2 | todo |  |
| 0x4C03D0 | `sub_4C03D0` | 21 |  | Source/gtx_0x106C.hpp:223 | 0/1 | todo |  |
| 0x4C03F0 | `sub_4C03F0` | 25 |  |  | 0/1 | todo |  |
| 0x4C0950 | `Taxi_4_Pool::ctor_4C0950` | 41 |  |  | 0/1 | todo |  |
| 0x4C09B0 | `Taxi_4::Init_4C09B0` | 12 |  | Source/TaxiList.hpp:45 | 0/1 | todo |  |
| 0x4C0A80 | `sub_4C0A80` | 66 |  |  | 0/1 | todo |  |
| 0x4C22D0 | `sub_4C22D0` | 46 |  |  | 0/1 | todo |  |
| 0x4C23A0 | `text_0x14::Find_4C23A0` | 5 |  |  | 0/32 | todo |  |
| 0x4C23B0 | `sub_4C23B0` | 19 |  |  | 0/10 | todo |  |
| 0x4C23D0 | `sub_4C23D0` | 24 |  | Source/gtx_0x106C.hpp:459 | 0/1 | todo |  |
| 0x4C23F0 | `text_tkey::ctor_4C23F0` | 16 |  |  | 0/1 | todo |  |
| 0x4C2400 | `text_tkey::dtor_4C2400` | 26 |  |  | 0/1 | todo |  |
| 0x4C2420 | `text_tdat::ctor_4C2420` | 16 |  |  | 0/1 | todo |  |
| 0x4C2430 | `text_tdat::dtor_4C2430` | 26 |  |  | 0/1 | todo |  |
| 0x4C2540 | `text_0x14::Load_4C2540` | 212 | 0x5B5E90 MATCH |  | 0/1 | todo |  |
| 0x4C2620 | `text_0x14::ctor_4C2620` | 152 | 0x5B5FB0 MATCH |  | 0/1 | todo |  |
| 0x4C2EB0 | `gtx_0x106C::get_tile_4C2EB0` | 33 |  | Source/gtx_0x106C.hpp:273 | 0/1 | todo |  |
| 0x4C2EE0 | `gtx_0x106C::has_tiles_4C2EE0` | 9 |  | Source/gtx_0x106C.hpp:266 | 0/1 | todo |  |
| 0x4C2EF0 | `sub_4C2EF0` | 18 |  |  | 0/1 | todo |  |
| 0x4C2F10 | `RemapTextureSet_14::ctor_4C2F10` | 24 |  |  | 0/1 | todo |  |
| 0x4C2F30 | `sub_4C2F30` | 52 |  | Source/TextureCache.cpp:193, Source/TextureCache.hpp:54 | 0/1 | todo |  |
| 0x4C3380 | `sub_4C3380` | 26 |  |  | 0/2 | todo |  |
| 0x4C33F0 | `sub_4C33F0` | 54 |  | Source/TileAnimManager.hpp:25 | 0/1 | todo |  |
| 0x4C34B0 | `TileAnimPool::ctor_4C34B0` | 71 |  |  | 0/1 | todo |  |
| 0x4C3590 | `sub_4C3590` | 11 |  |  | 0/1 | todo |  |
| 0x4C3850 | `sub_4C3850` | 35 |  |  | 0/1 | todo |  |
| 0x4C3970 | `unknown_libname_28` | 10 |  |  | 0/1 | todo |  |
| 0x4C39F0 | `sub_4C39F0` | 32 |  | Source/Car.hpp:170 | 0/1 | todo |  |
| 0x4C4A30 | `TrafficLights_194::sub_4C4A30` | 40 |  |  | 0/1 | todo |  |
| 0x4C4B40 | `Alloc_4C4B40` | 31 |  | Source/SpriteRenderer.hpp:72 | 0/1 | todo |  |
| 0x4C4B70 | `SpriteTreeNodePool_2EE4::sub_4C4B70` | 11 |  | Source/SpriteRenderer.hpp:87 | 0/2 | todo |  |
| 0x4C4B80 | `SpriteTreeStack_FA4::Push_4C4B80` | 30 |  | Source/SpriteRenderer.hpp:39 | 0/1 | todo |  |
| 0x4C4BA0 | `SpriteTreeStack_FA4::Pop_4C4BA0` | 20 |  | Source/SpriteRenderer.hpp:54 | 0/1 | todo |  |
| 0x4C4BC0 | `SpriteTreeStack_FA4::IsEnd_4C4BC0` | 10 |  | Source/SpriteRenderer.hpp:47 | 0/1 | todo |  |
| 0x4C4BD0 | `SpriteTreeStack_FA4::ctor_4C4BD0` | 9 |  |  | 0/1 | todo |  |
| 0x4C4F10 | `sub_4C4F10` | 10 |  | Source/Object_5C.hpp:54 | 0/1 | todo |  |
| 0x4C4F20 | `sub_4C4F20` | 7 |  | Source/Ped.hpp:580 | 0/1 | todo |  |
| 0x4C54F0 | `EmergencyCrew_30::sub_4C54F0` | 30 |  |  | 0/1 | todo |  |
| 0x4C62B0 | `sub_4C62B0` | 17 |  |  | 0/1 | todo |  |
| 0x4C6A50 | `Hud_PickupText_88::ctor_4C6A50` | 6 |  |  | 0/1 | todo |  |
| 0x4C6E20 | `sub_4C6E20` | 11 |  |  | 0/1 | todo |  |
| 0x4C6E50 | `Hud_UnderRoofArrowMarker_C::ctor_4C6E50` | 19 |  |  | 0/1 | todo |  |
| 0x4C6EE0 | `Garox_Sub_C_Array::ctor_4C6EE0` | 29 |  |  | 0/1 | todo |  |
| 0x4C6F20 | `HasNoTarget_4C6F20` | 9 |  | Source/Hud.hpp:453, Source/Hud.hpp:516 | 0/1 | todo |  |
| 0x4C6F30 | `sub_4C6F30` | 4 |  | Source/Hud.hpp:459 | 0/1 | todo |  |
| 0x4C6F80 | `HasNeitherTarget_4C6F80` | 35 |  | Source/Hud.hpp:530 | 0/6 | todo |  |
| 0x4C6FB0 | `HasBothTargets_4C6FB0` | 35 |  | Source/Hud.hpp:489 | 0/1 | todo |  |
| 0x4C6FF0 | `sub_4C6FF0` | 50 |  | Source/Hud.cpp:2362, Source/Hud.hpp:513 | 0/1 | todo |  |
| 0x4C7050 | `sub_4C7050` | 4 |  | Source/Hud.hpp:540 | 0/2 | todo |  |
| 0x4C7060 | `sub_4C7060` | 17 |  | Source/Hud.hpp:550 | 0/1 | todo |  |
| 0x4C7080 | `Garox_7C_Array::ctor_4C7080` | 43 |  |  | 0/1 | todo |  |
| 0x4C70B0 | `sub_4C70B0` | 7 |  | Source/Hud.hpp:612 | 0/1 | todo |  |
| 0x4C70E0 | `Hud_TextEntry_C4::sub_4C70E0` | 15 |  | Source/Hud.hpp:247 | 0/1 | todo |  |
| 0x4C70F0 | `Hud_TextEntry_C4::sub_4C70F0` | 11 |  | Source/Hud.hpp:241 | 0/1 | todo |  |
| 0x4C7120 | `sub_4C7120` | 9 |  | Source/Hud.hpp:304 | 0/1 | todo |  |
| 0x4C7130 | `sub_4C7130` | 46 |  | Source/Hud.cpp:1722, Source/Hud.hpp:310 | 0/1 | todo |  |
| 0x4C7160 | `sub_4C7160` | 9 |  | Source/Hud.hpp:292 | 0/2 | todo |  |
| 0x4C7170 | `sub_4C7170` | 7 |  | Source/Hud.hpp:298 | 0/2 | todo |  |
| 0x4C71A0 | `Hud_PauseScreen_2::ctor_4C71A0` | 10 |  |  | 0/1 | todo |  |
| 0x4C7220 | `sub_4C7220` | 38 |  | Source/Hud.hpp:353 | 0/4 | todo |  |
| 0x4C7250 | `sub_4C7250` | 38 |  | Source/Hud.hpp:360 | 0/3 | todo |  |
| 0x4C7340 | `sub_4C7340` | 4 |  | Source/Player.hpp:68 | 0/1 | todo |  |
| 0x4C7350 | `sub_4C7350` | 21 |  | Source/ScriptManager.hpp:624 | 0/1 | todo |  |
| 0x4C7380 | `sub_4C7380` | 25 |  | Source/Hud.cpp:976, Source/TagGame.hpp:33 | 0/1 | todo |  |
| 0x4C73A0 | `sub_4C73A0` | 246 |  |  | 0/1 | todo |  |
| 0x4C78A0 | `sub_4C78A0` | 304 |  |  | 0/1 | todo |  |
| 0x4C7A30 | `sub_4C7A30` | 218 |  |  | 0/1 | todo |  |
| 0x4C7CC0 | `sub_4C7CC0` | 46 | 0x5D03C0 MATCH |  | 0/1 | todo |  |
| 0x4C7FC0 | `sub_4C7FC0` | 759 |  |  | 0/1 | todo |  |
| 0x4C83D0 | `sub_4C83D0` | 121 |  |  | 0/1 | todo |  |
| 0x4C8620 | `sub_4C8620` | 35 |  |  | 0/1 | todo |  |
| 0x4C8880 | `sub_4C8880` | 142 |  |  | 0/1 | todo |  |
| 0x4C8A20 | `sub_4C8A20` | 21 |  |  | 0/1 | todo |  |
| 0x4C8CA0 | `sub_4C8CA0` | 400 |  |  | 0/1 | todo |  |
| 0x4C93B0 | `sub_4C93B0` | 18 |  |  | 0/2 | todo |  |
| 0x4CA610 | `sub_4CA610` | 53 |  | Source/Hud.cpp:2350, Source/Hud.hpp:510 | 0/1 | todo |  |
| 0x4CA660 | `Garox_C_Array::ctor_4CA660` | 27 |  |  | 0/1 | todo |  |
| 0x4CA910 | `sub_4CA910` | 365 |  |  | 0/1 | todo |  |
| 0x4CAC30 | `sub_4CAC30` | 46 |  |  | 0/1 | todo |  |
| 0x4CB520 | `sub_4CB520` | 11 |  |  | 0/1 | todo |  |
| 0x4CBA00 | `KanjiFont_8EC::sub_4CBA00` | 4 |  |  | 0/1 | todo |  |
| 0x4CBA10 | `sub_4CBA10` | 4 |  |  | 0/1 | todo |  |
| 0x4CBA20 | `KanjiFont_8EC::sub_4CBA20` | 4 |  |  | 0/1 | todo |  |
| 0x4CBA30 | `sub_4CBA30` | 4 |  |  | 0/1 | todo |  |
| 0x4CBA40 | `KanjiFont_8EC::sub_4CBA40` | 9 |  |  | 0/1 | todo |  |
| 0x4CC8C0 | `sub_4CC8C0` | 47 |  |  | 0/1 | todo |  |
| 0x4CC9B0 | `Weapon_30_Pool::sub_4CC9B0` | 4 |  | Source/WeaponManager.hpp:93 | 0/1 | todo |  |
| 0x4CC9C0 | `sub_4CC9C0` | 28 |  | Source/WeaponManager.hpp:44 | 0/1 | todo |  |
| 0x4CC9E0 | `sub_4CC9E0` | 26 |  | Source/WeaponManager.hpp:54 | 0/1 | todo |  |
| 0x4CCA00 | `sub_4CCA00` | 10 |  | Source/Weapon.hpp:34 | 0/2 | todo |  |
| 0x4CCA10 | `sub_4CCA10` | 10 |  | Source/Weapon.hpp:40 | 0/1 | todo |  |
| 0x4CCA20 | `sub_4CCA20` | 10 |  | Source/Weapon.hpp:46 | 0/1 | todo |  |
| 0x4CCA30 | `Weapon_30::sub_4CCA30` | 33 |  | Source/Weapon.hpp:107 | 0/3 | todo |  |
| 0x4CCA60 | `sub_4CCA60` | 17 |  | Source/Weapon.hpp:122 | 0/3 | todo |  |
| 0x4CCA80 | `Weapon_30::sub_4CCA80` | 10 |  | Source/Weapon.hpp:131 | 0/6 | todo |  |
| 0x4CCA90 | `sub_4CCA90` | 17 |  | Source/Ped.hpp:852 | 0/2 | todo |  |
| 0x4CD000 | `sub_4CD000` | 32 |  |  | 0/3 | todo |  |
| 0x4CD8C0 | `sub_4CD8C0` | 148 |  | Source/Weapon.cpp:2088 | 0/1 | todo |  |
| 0x4CDA20 | `Weapon_30_Pool::ctor_4CDA20` | 74 |  |  | 0/1 | todo |  |
| 0x4CEF40 | `sub_4CEF40` | 1076 |  | Source/Weapon.cpp:993 | 0/1 | todo |  |
| 0x4D06E0 | `Weapon_8::sub_4D06E0` | 27 |  |  | 0/2 | todo |  |
| 0x4D0700 | `Car_BC::sub_4D0700` | 59 |  |  | 0/1 | todo |  |
| 0x4D09C0 | `Game_0x40::get_main_state_4D09C0` | 4 |  | Source/Game.hpp:66 | 0/1 | todo |  |
| 0x4D09D0 | `sub_4D09D0` | 41 |  |  | 0/1 | todo |  |

<!-- table unpaired -->

