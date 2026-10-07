#pragma once

#include "BitSet32.hpp"
#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "Marz_1D7E.hpp"
#include "ang16.hpp"
#include "char.hpp"
#include "enums.hpp"
#include "fix16.hpp"
#include "miss2_xyz.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include <cstdio>

class Player;
class gmp_map_zone;
class PedGroup;
class Weapon_30;
class Gang_144;
class Sprite;
class Char_8;
class Char_B4;
class Marz_96;
class Object_2C;
class Car_BC;
class TrainStation_34;

class Ped
{
  public:
    // 9.6f 0x433C90
    inline bool Is_occupation_elvis_433C90()
    {
        return field_240_occupation == ped_ocupation_enum::elvis;
    }

    // 9.6f 0x4039B0
    inline void set_enter_car_as_passenger_4039B0(s32 v)
    {
        field_248_enter_car_as_passenger = v;
    }

    // 9.6f 0x4039C0
    inline s32 get_enter_car_as_passenger_4039C0()
    {
        return field_248_enter_car_as_passenger;
    }

    // 9.6f 0x4039D0
    inline char_type get_field_226_4039D0()
    {
        return field_226_internal_objective_status;
    }

    // 9.6f 0x433C20
    inline Fix16 GetGameObjectVelocity_433C20()
    {
        return field_168_game_object->get_velocity_41B080();
    }

    // 9.6f 0x4AF860 (0x4AF880 is an identical copy)
    inline void SetTrainStation_4AF860(TrainStation_34* pStation)
    {
        if (pStation != field_13C_pTrainStation)
        {
            field_13C_pTrainStation = pStation;
        }
    }

    // 9.6f 0x4117D0 (0x4117F0 is an identical copy)
    inline TrainStation_34* PopTrainStation_4117D0()
    {
        TrainStation_34* pStation = field_13C_pTrainStation;
        field_13C_pTrainStation = 0;
        return pStation;
    }

    // 9.6f 0x403A70
    inline void set_target_car_door_403A70(u8 v)
    {
        field_24C_target_car_door = v;
    }

    // 9.6f 0x403AC0
    inline void set_objective_target_ped_403AC0(Ped* v)
    {
        field_148_objective_target_ped = v;
    }

    // 9.6f 0x403AD0
    inline Ped* get_objective_target_ped_403AD0()
    {
        return field_148_objective_target_ped;
    }

    // 9.6f 0x403B00
    inline void set_target_to_enter_403B00(Car_BC* v)
    {
        field_154_target_to_enter = v;
    }

    // 9.6f 0x403B20
    inline u16 get_car_state_timer_403B20()
    {
        return field_21A_car_state_timer;
    }

    // 9.6f 0x403B30
    inline u16 get_objective_timer_403B30()
    {
        return field_218_objective_timer;
    }

    // 9.6f 0x403B40
    inline void set_objective_status_403B40(u8 v)
    {
        field_225_objective_status = v;
    }

    // 9.6f 0x403B50
    inline void set_field_226_403B50(char_type v)
    {
        field_226_internal_objective_status = v;
    }

    // 9.6f 0x433B80
    inline void SetOffscreenCounter_433B80(s16 v)
    {
        field_20E_offscreen_counter = v;
    }

    // 9.6f 0x433C80
    inline void set_objective_timer_433C80(u16 v)
    {
        field_218_objective_timer = v;
    }

    // 9.6f 0x475B40
    inline void clear_last_char_punched_475B40()
    {
        field_188_last_char_punched = 0;
    }

    // 9.6f 0x475B10
    inline Ped* get_last_char_punched_475B10()
    {
        return field_188_last_char_punched;
    }

    // 9.6f 0x4A5020
    inline Char_B4* get_game_object_4A5020()
    {
        return field_168_game_object;
    }

    EXPORT Ped(); // 45AE70
    EXPORT ~Ped(); // 45AF00
    EXPORT void Reset_45AFC0();
    EXPORT void PoolAllocate();
    EXPORT char_type IsLawEnforcement_45B4E0();
    EXPORT Fix16_Point GetVelocityVector_45B520();
    EXPORT void SetRecentCrimeTimer_45B550();
    EXPORT void SetPlayer_45B560(Player* a2, char_type a3);
    EXPORT bool IsEmergencyOccupation_45B590();
    EXPORT void CopyStatsFromPed_45B5B0(Ped* pSrc);
    EXPORT Car_BC* GetCarBeingEnteredOrExited_45BBF0();
    EXPORT void TeleportToCoord_45BC10(Fix16 xpos, Fix16 ypos);
    EXPORT void ManageShocking_45BC70();
    EXPORT bool CloseDoorIfCarApproaching_45BD20(Car_BC* pCar);
    EXPORT s32 GetBulletSpriteOffset_45BE30();
    EXPORT void SetOnFire();
    EXPORT void PutOutFire();
    EXPORT void ManageBurning_45BEC0();
    EXPORT void DrawFlamesAndStartScreamTimer();
    EXPORT void SetInvisible();
    EXPORT void SetVisible();
    EXPORT void SetSpriteSemiTransIfInvisible();
    EXPORT void SetInvulnerable();
    EXPORT void ClearInvulnerable_45C050();
    EXPORT void SetSpriteFlagIfInvulnerable_45C070();
    EXPORT void RestoreCarOrPedHealth();
    EXPORT void SpawnCharInZone_45C0C0(gmp_map_zone* a2);
    EXPORT void PoolDeallocate();
    EXPORT void RespawnPed_45C350(gmp_map_zone* pZone);
    EXPORT void ResetForPlayerRespawn_45C410();
    EXPORT void UpdatePositionFromCar_45C4B0();
    EXPORT void ChangeNextPedState1_45C500(s32 a2);
    EXPORT void ChangeNextPedState2_45C540(s32 a2);
    EXPORT void RestorePreviousPedState_45C5A0();
    EXPORT void CancelEnterCarObjective_45C5C0();
    EXPORT void SpawnDriverRunAway_45C650(Car_BC* pCar, Ped* pPed);
    EXPORT void SpawnPedInCar_45C730(Car_BC* pCar);
    EXPORT void EnterCarAsDriver(Car_BC* a2);
    EXPORT void EnterCarAsPassenger_45C7F0(Car_BC* pCar);
    EXPORT char_type AllocCharB4_45C830(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Ang16 GetTurnSpeed_45C900();
    EXPORT Fix16 GetPedVelocity_45C920();
    EXPORT Ang16 GetRotation();
    EXPORT Fix16 GetMoveDirection_45C9B0();
    EXPORT Ang16 ComputeAimAngle_45C9D0();
    EXPORT void HandleClosePedInteraction_45CAA0();
    EXPORT void TakeDamage(s16 damage);
    EXPORT void HandlePedCrossingTrigger_45CF20(Object_2C* a2);
    EXPORT char_type HandlePedHitByObject_45D000(Object_2C* a2);
    EXPORT char_type AddWeaponWithAmmo_45DD30(s32 weapon_kind, char_type ammo);
    EXPORT char_type HandlePickupCollision_45DE80(Object_2C* pPickUp);
    EXPORT void SpawnWeaponOnDeath_45E080();
    EXPORT void StartCrossingRoad_45E4A0();
    EXPORT void DeallocateWithGroupCleanup_45EA00();
    EXPORT void Deallocate_45EB60();
    EXPORT char_type IsActivePlayerPed_45EDC0();
    EXPORT bool PedTypeIs_45EDE0(s32 type);
    EXPORT void SetOccupation_45EE00(u32 occupation);
    EXPORT void EnterPublicTransport_45EE70();
    EXPORT void Mugger_AI_45F360();
    EXPORT void CarThief_AI_45FF60();
    EXPORT void TaxiCustomer_AI_460820();
    EXPORT void BusCustomer_AI_461290();
    EXPORT void TrainCustomer_AI_461530();
    EXPORT void RobbedDriver_AI_461630();
    EXPORT void RoadBlockTank_AI_4619F0();
    EXPORT void UpdateFacingAngle_461A60();
    EXPORT void Occupation_AI_461F20();
    EXPORT void UpdateAI_462280();
    EXPORT void ReleaseGroupSpritesAndWeapons_4624A0();
    EXPORT void RemovePedWeapons_462510();
    EXPORT void RemoveSecondaryWeapon_462550();
    EXPORT void ForceDoNothing_462590();
    EXPORT void UpdateOnScreenCounter_462620();
    EXPORT char_type StateMachineTick_4626B0();
    EXPORT void UpdateCharB4_462B80();
    EXPORT bool PoolUpdate();
    EXPORT void ProcessObjective_4632E0();
    EXPORT void ChangePedStatesByMode_463300(u8 a1);
    EXPORT void SetStatesForObjective_4633E0(char_type a2);
    EXPORT void SetObjective(s32 objective, s16 objective_timer);
    EXPORT void SetObjective2_463830(s32 a2, s16 a3);
    EXPORT void ProcessOnFootObjective_463AA0();
    EXPORT void ProcessInCarObjective_463FB0();
    EXPORT void CalcApproachPointNearTargetPed_4645B0();
    EXPORT void Threat_Reaction_AI_465270();
    EXPORT void ReactToAttacker_465B20();
    EXPORT bool IsAttackingTargetPed_465CD0();
    EXPORT bool IsPedAThreat_465D00(Ped* pTargetPed);
    inline bool IsPedAThreat_Inline_465D00(Ped* pTargetPed);
    EXPORT char_type HasBit25AndGameObject_466B70();
    EXPORT char_type IsThreatToSearchingPed_4661F0();
    EXPORT Ped* FindBestTargetPed_Mode1_466B90(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode4_466BB0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode5_466BD0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_466BF0(s32 a2);
    EXPORT Ped* FindNearestPed_Mode4_466F40(u8 a2);
    EXPORT Ped* FindNearestPed_466F60(u8 a2);
    EXPORT Ped* FindNearbyPed_466FB0();
    EXPORT Ped* GetLastProcessedPedOnFoot_467070();
    EXPORT char_type FindUsableCarDoor_467090();
    EXPORT Sprite* ResetAnimAndFindNearestSprite_467280();
    EXPORT void UpdateMovementTowardsTarget_4672E0(Fix16 distance, u8 type);
    EXPORT void FleeOnFootTillSafe_4678E0();
    EXPORT void FleeCharOnFootTillSafe_467960();
    EXPORT void FleeFromCharOnFootAlways_467A20();
    EXPORT void FleeCharAlwaysOnceCarStopped_467AD0();
    EXPORT void EnterCarOrFlee_467BD0();
    EXPORT void KillCharOnFoot_467CA0();
    EXPORT void KillCharAnyMeans_467E20();
    EXPORT void KillFrenzy_467FB0();
    EXPORT void PunchChar_467FD0();
    EXPORT void ProcessAirborneMovement_468040();
    EXPORT void TimeWaitedInCar_4682A0();
    EXPORT void GotoAreaInCar_468310();
    EXPORT void EnterTargetObjectiveCar_4686C0();
    EXPORT void LeaveTargetObjectiveCar_468820();
    EXPORT void EnterTrain_468930();
    EXPORT void LeaveTrain_468A00();
    EXPORT void AbortEnterCarObjective_468BD0();
    EXPORT void PatrolOnFoot_468C70();
    EXPORT void GotoAreaOnFoot_468DE0();
    EXPORT void UpdateFollowPedObjective_468E80();
    EXPORT s32 GetValueByIdParity_469010();
    EXPORT void UpdateKillerIdTimer_469030();
    EXPORT void GotoAreaByAnyMeans_469060();
    EXPORT void UpdateGroupMemberFollow_469BD0();
    EXPORT void GuardSpot_469BF0();
    EXPORT void GuardArea_469D60();
    EXPORT void FailWhenObjectiveTimerExpired_469E10();
    EXPORT void SetCarAiSpeedOne_469E30();
    EXPORT void SetupCarFollowTargetPed_469E50();
    EXPORT void FollowPedInCar_469F30();
    EXPORT void WaitInCurrentCar_469FC0();
    EXPORT void sub_469FE0();
    EXPORT void PullDriverOutOfCar_46A1F0();
    EXPORT void FollowCarInCurrCar_46A290();
    EXPORT void FollowCarOnFootWithOffset_46A350();
    EXPORT void FireAtObject_46A530();
    EXPORT void FireAtPlayer_46A5E0();
    EXPORT void AimVehicleTurretStateMachine_46A6D0();
    EXPORT void DestroyTargetObject_46A7C0();
    EXPORT void DestroyTargetCar_46A850();
    EXPORT void FleeOnFootTillSafe_46A8F0();
    EXPORT void FleeFromPedTillSafe_46A9C0();
    EXPORT void FleeFromPedAlways_46AAE0();
    EXPORT void CheckFollowTargetPedStillValid_46AB50();
    EXPORT void FollowTargetStateMachine_46AC20();
    EXPORT void ChaseTargetStateMachine_46B170();
    EXPORT void PullDriverOutOfCarStateMachine_46B2F0();
    EXPORT void MeleeAttackStateMachine_46B670();
    EXPORT void WaitOnFoot_46BD30();
    EXPORT char_type IsOtherPedEnteringAsDriver_46BD50(Car_BC* pCar);
    EXPORT void EnterCarStateMachine_46BDC0();
    EXPORT void ExitCarStateMachine_46C250();
    EXPORT void WalkToTargetPoint_46C770();
    EXPORT void GotoAreaOnFoot_46C7E0();
    EXPORT void WalkToTargetThenStop_46C8A0();
    EXPORT void FollowPathPoints_46C910();
    EXPORT void CrossRoad_46C9B0();
    EXPORT void EmptyState14_46CA60();
    EXPORT void FollowPedInCar_46CA70();
    EXPORT void StartPedCrossingAtTrafficLight_Y_Backward_46CB30();
    EXPORT void StartPedCrossingAtTrafficLight_X_Forwards_46CC70();
    EXPORT void StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0();
    EXPORT void StartPedCrossingAtTrafficLight_X_Backwards_46CEF0();
    EXPORT void WaitForTrain_46D030();
    EXPORT void WaitForCarStateTimer_46D0B0();
    EXPORT void EnterTrainStateMachine_46D0D0();
    EXPORT void ExitTrainStateMachine_46D240();
    EXPORT void FollowCarOnFoot_46D300();
    EXPORT void AttackTargetStateMachine_46D460(u8 targetType);
    EXPORT void AttackPed_46DB60();
    EXPORT void AttackCar_46DB70();
    EXPORT void AttackObject_46DB80();
    EXPORT Sprite* GetSprite_46DF50();
    EXPORT void SetupFollower_46DF70(Ped* arg0, s32 WeaponIdx);
    EXPORT bool CanBeRecruitedToGroup_46E020(PedGroup* a2);
    EXPORT void RecruitNearbyPeds_46E080(s32 desiredCount, Fix16 searchRadius);
    EXPORT void SpawnPedGroupFollowers_46E200(u8 total);
    EXPORT u8 get_wanted_star_count_46EF00();
    EXPORT void set_wanted_level_46EF40(u16 wanted);
    EXPORT void IncreaseWantedLevelFromDebugKeys_46EFD0();
    EXPORT void set_wanted_star_count_46F070(u8 star_count);
    EXPORT bool WantedStartCountLessThan_46F100(u8 a2);
    EXPORT Weapon_30* GetWeaponFromPed_46F110();
    EXPORT void ApplyAimJitter_46F1E0(Weapon_30* a2);
    EXPORT void ManageWeapon_46F390();
    EXPORT Weapon_30* ChooseAttackWeapon_46F490();
    EXPORT void ForceWeapon_46F600(s32 a2);
    EXPORT void GiveWeapon_46F650(s32 a2);
    EXPORT void ApplyGangRespectForKill_46F680(Ped* a2);
    EXPORT void UpdateStatsForKiller_46F720();
    EXPORT void Kill_46F9D0();
    EXPORT void AddThreateningPedToList_46FC70();
    EXPORT void HandleShootingAtCar_46FC90(Car_BC* pCar, s32 model);
    EXPORT void ProcessWeaponHitResponse_46FE20(Object_2C* a2);
    EXPORT void NotifyWeaponHit_46FF00(Fix16 xpos, Fix16 ypos, s32 model);
    EXPORT void HandleWeaponFireEnd_46FFF0(s32 a2);
    EXPORT void AimRoofGun_470050();
    EXPORT void add_wanted_points_470160(s16 wanted_amount);
    EXPORT bool IsNearestSpriteACar_4701D0();
    EXPORT void StartPedWalking_470200(Fix16 a2, Fix16 a3, Fix16 a4);
    EXPORT void BecomeLeaderOfGroup_4702D0(Ped* pPed);
    EXPORT void BecomeDummyOnPlayerDisconnect_470300();
    EXPORT void PushPatrolPoint_4702A0(s8 x, s8 y, s8 z);
    EXPORT s32 IsInTrain_470F00();

    EXPORT void FleeCharAnyMeansTillSafe_Nullsub();
    EXPORT void FleeCharAnyMeansAlways_Nullsub();
    EXPORT void KillCar_Nullsub();
    EXPORT void Objective50_Nullsub();
    EXPORT void State30_Nullsub();

    inline u8 get_varrok_idx_420B50()
    {
        return field_267_varrok_idx;
    }

    inline void ClearGroupAndGroupIdx_403A30()
    {
        this->field_164_ped_group = 0;
        this->field_23C_group_idx = 0;
    }

    inline s32 GetPedType_420B70()
    {
        return field_238_ped_type;
    }

    void inline_clear_bit()
    {
        // There was no way to match this without using a bit field
        field_21C_bf.b11 = 0;
    }

    // 9.6f 0x4A5060
    inline void set_bit_26_4A5060()
    {
        field_21C_bf.b26 = true;
    }

    // 9.6f 0x4A5050
    inline void SetFullHealth_4A5050()
    {
        set_health_4039A0(100);
    }

    inline void clear_bit_26_482080()
    {
        field_21C_bf.b26 = false;
    }

    bool check_bit_0()
    {
        return field_21C_bf.b0 != 0;
    }

    bool check_bit_11()
    {
        return field_21C_bf.b11 != 0;
    }

    // 9.6f inline 0x450CB0
    inline u8 GetObjectiveStatus_450CB0()
    {
        return field_225_objective_status;
    }

    void reset_ped_group()
    {
        field_164_ped_group = NULL;
        field_23C_group_idx = 0;
    }

    void set_ped_group(PedGroup* ptr)
    {
        field_164_ped_group = ptr;
    }

    // 9.6f 0x403940
    void set_ped_group_id(s8 param_1)
    {
        field_23C_group_idx = param_1;
    }

    u16 GetOffscreenCounter() const
    {
        return field_20E_offscreen_counter;
    }

    bool has_field_16C_car() const
    {
        return field_16C_car != NULL;
    }

    s32 get_ped_state1() const
    {
        return field_278_ped_state_1;
    }

    // 9.6f 0x492CB0
    void set_field_140_492CB0(Car_BC* pCar)
    {
        field_140_stolen_car = pCar;
    }

    // 9.6f 0x49EF40
    inline Car_BC* get_field_140_49EF40()
    {
        return field_140_stolen_car;
    }

    // 9.6f inline 0x403AE0
    void set_field_14C_403AE0(Ped* pSrc)
    {
        field_14C_internal_target_ped = pSrc;
    }

    // 9.6f inline 0x403950
    inline void SetBit2_403950()
    {
        field_21C_bf.b2 = true;
    }

    bool get_bitset_0x04()
    {
        return field_21C & ped_bit_status_enum::k_ped_0x00000004 ? true : false;
    }

    // 9.6f 0x403960
    void unset_bitset_0x04()
    {
        field_21C &= ~ped_bit_status_enum::k_ped_0x00000004;
    }

    // 9.6f 0x403AA0
    void set_field_150_target_objective_car(Car_BC* ptr)
    {
        field_150_target_objective_car = ptr;
    }

    Car_BC* get_target_objective_car_403AB0()
    {
        return field_150_target_objective_car;
    }

    void set_ped_type(s32 param_1)
    {
        field_238_ped_type = param_1;
    }

    // 9.6f 0x403A00
    inline Fix16 get_cam_x()
    {
        return field_1AC_cam.x;
    }

    // 9.6f 0x403A10
    inline Fix16 get_cam_y()
    {
        return field_1AC_cam.y;
    }

    // 9.6f 0x416B50
    inline Fix16 get_cam_z()
    {
        return field_1AC_cam.z;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = field_1AC_cam.x;
        Fix16 width = rect->field_C_size.field_0_x;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (x_pos >= rect->field_0_pos.field_0_x - width && x_pos <= rect->field_0_pos.field_0_x + width &&
                (y_pos = field_1AC_cam.y, height = rect->field_C_size.field_4_y, y_pos >= rect->field_0_pos.field_4_y - height) &&
                field_1AC_cam.y <= rect->field_0_pos.field_4_y + height &&
                (z_pos = field_1AC_cam.z, z_target = rect->field_0_pos.field_8_z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline s16 get_wanted_points_433DC0()
    {
        return field_20A_wanted_points;
    }

    inline bool has_car_403B80()
    {
        return field_16C_car != 0;
    }

    inline bool not_enter_car_as_passenger_4A5040()
    {
        return field_248_enter_car_as_passenger != 1;
    }

    inline Car_BC* get_car_416B60()
    {
        return field_16C_car;
    }

    // 9.6f 0x420B60, 0x4C4F20
    u32 get_id() const
    {
        return field_200_id;
    }

    inline Fix16 get_max_speed_1F0()
    {
        return field_1F0_max_speed;
    }

    inline void Set_B4_F16_To_1_433B50()
    {
        field_168_game_object->field_16_state_init_pending = 1;
    }

    inline s8 get_remap_433BA0()
    {
        return field_244_remap;
    }

    inline bool is_player_41B0A0()
    {
        return field_15C_player != 0;
    }

    // 9.6f 0x433C40
    inline void DoJump_433C40()
    {
        if (field_168_game_object)
        {
            field_168_game_object->DoJump_5454D0();
        }
    }

    // 9.6f 0x41B0B0
    inline s32 TakeVoiceEvent_41B0B0()
    {
        s32 ret = field_250_voice_event; // sound id the ped says next
        field_250_voice_event = 0;
        return ret;
    }

    inline void SetVoiceEvent_IfBit24Clear_433DD0(s32 a2)
    {
        // TODO: Check if (HIBYTE(this->field_21C) & 1)
        // is correct
        if (field_21C_bf.b24 == 0)
        {
            field_250_voice_event = a2;
        }
    }

    inline u8 get_target_car_door_403A60()
    {
        return field_24C_target_car_door;
    }

    // 9.6f inline 0x433B90
    void set_remap_433B90(u8 remap)
    {
        field_244_remap = remap;
    }

    // 9.6f inline 0x4039A0
    void set_health_4039A0(s16 health)
    {
        field_216_health = health;
    }

    // 9.6f inline 0x433B70
    inline s32 get_health_433B70()
    {
        return field_216_health;
    }

    void set_occupation_403970(s32 occupation)
    {
        field_240_occupation = occupation;
    }

    s32 get_occupation_403980()
    {
        return field_240_occupation;
    }

    void SetField238_403920(s32 unk)
    {
        field_238_ped_type = unk;
    }

    void SetMoveTargetX_433C50(Fix16 a2)
    {
        this->field_1C4_move_target_x = a2;
    }

    void SetMoveTargetY_433C60(Fix16 a2)
    {
        this->field_1C8_move_target_y = a2;
    }

    void SetMoveTargetZ_433C70(Fix16 a2)
    {
        this->field_1CC_move_target_z = a2;
    }

    Fix16 GetMoveTargetX_492CE0()
    {
        return field_1C4_move_target_x;
    }

    Fix16 GetMoveTargetY_492CF0()
    {
        return field_1C8_move_target_y;
    }

    // TODO: to use this inline we need to fix a circular dependency issue
    inline s32 get_car_model();

    // 9.6f 0x433C10: a thin wrapper around Char_B4::SetRemap. The remap is taken by const reference:
    // by value, VC6 loads the argument before the Char_B4 and 9 callers stop matching.
    inline void SetRemap_433C10(const u8& remap)
    {
        Char_B4* p_B4 = field_168_game_object; // local necessary to match Ped::SetupFollower_46DF70
        p_B4->SetRemap_Inline(remap);
    }

    inline void TriggerVoiceEventRateLimited_433E50()
    {
        if ((u32)(gpRng_67AB34->get_cur_rng_41CFE0() - field_220_last_voice_rng) > 5)
        {
            SetVoiceEvent_IfBit24Clear_433DD0(25);
            field_220_last_voice_rng = gpRng_67AB34->get_cur_rng_41CFE0();
        }
    }

    inline void SetJumpOverMode_433BB0(s32 value)
    {
        field_230_jump_over_mode = value;
    }

    // 9.6f 0x492CC0
    inline Ang16 GetTargetFacingAngle_492CC0()
    {
        return field_130_target_facing_angle;
    }

    // 9.6f 0x492C20
    inline s32 GetJumpOverMode_492C20()
    {
        return field_230_jump_over_mode; // 2 = may jump over obstacles (player, special peds)
    }

    inline void SetPedClass_433BC0(s32 value)
    {
        field_22C_ped_class = value;
    }

    // 9.6f 0x403A20
    inline void ClearHitCount_403A20()
    {
        field_228_hit_count = 0;
    }

    // 9.6f 0x420B80
    inline void ClearWantedPoints_420B80()
    {
        field_20A_wanted_points = 0;
    }

    // 9.6f 0x472FD0
    inline bool IsGroupLeader_472FD0()
    {
        return field_23C_group_idx == 99;
    }

    // 9.6f 0x433BE0
    inline void ClearF144_433BE0()
    {
        field_144_attacker = 0;
    }

    // 9.6f 0x433C00
    inline void SetRotation_433C00(Ang16 rotation)
    {
        field_168_game_object->set_rotation_433A30(rotation);
    }

    // 9.6f 0x433C20
    inline Fix16 GetCharVelocity_433C20()
    {
        return field_168_game_object->get_velocity_41B080();
    }

    inline bool CheckBit0_433B40()
    {
        return field_21C_bf.b0;
    }

    inline s32 GetPedState_403990()
    {
        return field_278_ped_state_1;
    }

    inline s32 GetPedState2_433B60()
    {
        return field_27C_ped_state_2;
    }

    inline bool sub_433DA0()
    {
        return field_21C_bf.b25 && field_168_game_object;
    }

    bool bHasGameObject_403B70()
    {
        return field_168_game_object != NULL;
    }

    inline u8 GetBit2()
    {
        return field_21C_bf.b2;
    }

    inline u8 GetBit11_433CA0()
    {
        return field_21C_bf.b11;
    }

    u8 GetBit24_475B50()
    {
        return field_21C_bf.b24;
    }

    inline s32 get_objective_403A80()
    {
        return field_258_objective;
    }

    inline Car_BC* get_target_to_enter_403B10()
    {
        return field_154_target_to_enter;
    }

    // 9.6f 0x4A5010
    inline void SetBit11_4A5010()
    {
        field_21C_bf.b11 = true;
    }

    // 9.6f 0x403A40
    inline void ClearBit11_403A40()
    {
        field_21C_bf.b11 = false;
    }

    inline bool IsPedGoingToEnterCar_492FD0()
    {
        return field_258_objective == objectives_enum::enter_car_as_driver_35 || field_25C_internal_objective == 35;
    }

    Ang16 Get_F12E_4CCA90()
    {
        return field_12E_aim_angle;
    }

    bool isDead_403B60()
    {
        return this->field_278_ped_state_1 == ped_state_1::dead_9;
    }

    u16 Ped::GetOffscreenCounter_4039F0()
    {
        return this->field_20E_offscreen_counter;
    }

    PedGroup* GetGroup_475AF0()
    {
        return this->field_164_ped_group;
    }

    inline s32 GetInternalObjective_403A90()
    {
        return field_25C_internal_objective;
    }

    inline Ped* Get_F14C_403AF0()
    {
        return field_14C_internal_target_ped;
    }

    inline void SetAttacker_433BF0(Ped* pPed)
    {
        field_144_attacker = pPed;
    }

    inline void Increment_F262_433BD0()
    {
        ++field_262_attackers_count;
    }

    Marz_3 field_0_patrol_points[100];
    Ang16 field_12C_facing_angle;
    Ang16 field_12E_aim_angle;
    Ang16 field_130_target_facing_angle;
    Ang16 field_132_follow_car_offset_angle;
    Ang16 field_134_rotation;
    s16 field_136_unused;
    s32 field_138_collided_char_b4;
    TrainStation_34* field_13C_pTrainStation;
    Car_BC* field_140_stolen_car;
    Ped* field_144_attacker;
    Ped* field_148_objective_target_ped;
    Ped* field_14C_internal_target_ped;
    Car_BC* field_150_target_objective_car;
    Car_BC* field_154_target_to_enter;
    Car_BC* field_158_escape_car;
    Player* field_15C_player;
    Ped* mpNext;
    PedGroup* field_164_ped_group;
    Char_B4* field_168_game_object;
    Car_BC* field_16C_car;
    Weapon_30* field_170_selected_weapon;
    Weapon_30* field_174_pWeapon;
    Weapon_30* field_178_car_weapon;
    Gang_144* field_17C_pGang;
    Ped* field_180_car_thief;
    Object_2C* field_184_pObj2C;
    Ped* field_188_last_char_punched;
    Marz_3* field_18C_current_path_point;
    Marz_96* field_190_patrol_route;
    Marz_3* field_194_current_patrol_point;
    Ped* field_198_hit_target_ped; // ped last hit by this ped's weapon / electrobaton
    Gang_144* field_19C_dummy_gang; // gang of a dummy ped
    Object_2C* field_1A0_objective_target_object;
    Object_2C* field_1A4_internal_target_object;
    Ped* field_1A8_ped_killer;
    Fix16_Vec field_1AC_cam;
    Fix16 field_1B8_target_x;
    Fix16 field_1BC_target_y;
    Fix16 field_1C0_target_z;
    Fix16 field_1C4_move_target_x;
    Fix16 field_1C8_move_target_y;
    Fix16 field_1CC_move_target_z;
    Fix16 field_1D0_internal_target_x;
    Fix16 field_1D4_internal_target_y;
    Fix16 field_1D8_internal_target_z;
    Fix16 field_1DC_objective_target_x;
    Fix16 field_1E0_objective_target_y;
    Fix16 field_1E4_objective_target_z;
    Fix16 field_1E8_target_distance_limit;
    s32 field_1EC_unused;
    Fix16 field_1F0_max_speed;
    Fix16 field_1F4_walk_speed;
    Fix16 field_1F8_run_speed;
    Fix16 field_1FC_follow_car_offset_distance;
    u32 field_200_id;
    s32 field_204_killer_id;
    u16 field_208_invulnerability;
    s16 field_20A_wanted_points;
    u16 field_20C_target_unseen_counter; // frames the target was not seen; gives up after 15
    s16 field_20E_offscreen_counter;
    u16 field_210_shock_counter;
    u16 field_212_electrocution_threshold;
    s16 field_214_unused;
    s16 field_216_health;
    u16 field_218_objective_timer;
    u16 field_21A_car_state_timer;

    union
    {
        CompilerBitField32 field_21C_bf;
        // TODO: Move everything to use the above field and remove union
        s32 field_21C;
    };

    s32 field_220_last_voice_rng;
    union
    {
        CompilerBitField8 field_224_bf;
        char_type field_224;
    };
    u8 field_225_objective_status; // it uses objective_status enum
    char_type field_226_internal_objective_status;
    char_type field_227_unused;
    char_type field_228_hit_count; // times hit by the attacker
    u8 field_229_mug_count; // times mugged while mugging a player
    char_type field_22A_pad;
    char_type field_22B_pad;
    s32 field_22C_ped_class; // 2 = mugger/car thief, 1 = gang member, 0 = gang dummy
    s32 field_230_jump_over_mode;
    char_type field_234_lifetime_timer; // 0 = remove ped, 99 = never expires
    char_type field_235_pad;
    char_type field_236_pad;
    char_type field_237_pad;
    s32 field_238_ped_type;
    u8 field_23C_group_idx;
    char_type field_23D_pad;
    char_type field_23E_pad;
    char_type field_23F_pad;
    s32 field_240_occupation;
    char_type field_244_remap;
    char_type field_245_pad;
    char_type field_246_pad;
    char_type field_247_pad;
    s32 field_248_enter_car_as_passenger;
    u8 field_24C_target_car_door;
    char_type field_24D_pad;
    char_type field_24E_pad;
    char_type field_24F_pad;
    s32 field_250_voice_event;
    s32 field_254_block_spec;
    //char_type field_255;
    //char_type field_256;
    //char_type field_257;
    s32 field_258_objective;
    s32 field_25C_internal_objective;
    char_type field_260_wait_counter; // frames waited on the target (flag 0x10 of field_224)
    char_type field_261_unused;
    u8 field_262_attackers_count;
    u8 field_263_prev_attackers_count;
    u8 field_264_killer_id_timer;
    u8 field_265_patrol_list_idx;
    u8 field_266_path_fail_count;
    u8 field_267_varrok_idx;
    char_type field_268_electrocution_timer;
    char_type field_269_unused;
    u8 field_26A_recent_crime_timer;
    char_type field_26B_pad;
    s32 field_26C_graphic_type;
    s32 field_270_fire_mode; // 0 = random shots, 2 = never fires
    s32 field_274_gang_car_model;
    s32 field_278_ped_state_1;
    s32 field_27C_ped_state_2;
    s32 field_280_stored_ped_state_1;
    s32 field_284_stored_ped_state_2;
    s32 field_288_threat_search;
    s32 field_28C_threat_reaction;
    s32 field_290_death_cause; // why the ped died (1/3 = car, 2/5 = killed by a ped, 10 = beaten)
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped, 0x294)

EXPORT void __stdcall CarDoorAlignmentSolver_545AF0(u8 a1, Car_BC* a2, u8 a3, Fix16& a4, Fix16& a5, Ang16& a6);

EXTERN_GLOBAL(s32, gPedId_61A89C);

EXTERN_GLOBAL(u8, gNumberMuggersSpawned_6787CA);

EXTERN_GLOBAL(u8, gNumberCarThiefsSpawned_6787CB);

EXTERN_GLOBAL(u8, gNumberElvisLeadersSpawned_6787CC);

EXTERN_GLOBAL(u8, gNumberWalkingCopsSpawned_6787CD);

EXTERN_GLOBAL(u8, bThreateningPedAdded_6787EF);

EXTERN_GLOBAL(Fix16, kFpOne256th_678620);
EXTERN_GLOBAL(Fix16, kFpFour_678670);
EXTERN_GLOBAL(Fix16, kFpOneSixteenth_678448);
EXTERN_GLOBAL(Fix16, kFpPoint1_6FD824);
