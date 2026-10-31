#pragma once

#include "BitSet32.hpp"
#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "PatrolRoutePool.hpp"
#include "ang16.hpp"
#include "char.hpp"
#include "enums.hpp"
#include "fix16.hpp"
#include "miss2_xyz.hpp"
#include "ped_flags.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include <cstdio>

class Player;
class gmp_map_zone;
class PedGroup;
class Weapon;
class Gang;
class Sprite;
class PedListNode;
class Char;
class PatrolRoute;
class Object;
class Car;
class TrainStation;

class Ped
{
  public:
    STATIC_MEMBER(u8, sPedUsedPathThisTick);
    STATIC_MEMBER(u8, sKeepPaceWithTarget);
    // 9.6f 0x433C90
    inline bool Is_occupation_elvis_433C90() const
    {
        return mOccupation == ped_ocupation_enum::elvis;
    }

    // 9.6f 0x4039B0
    inline void set_enter_car_as_passenger_4039B0(s32 v)
    {
        mEnterCarAsPassenger = v;
    }

    // 9.6f 0x4039C0
    inline s32 get_enter_car_as_passenger_4039C0() const
    {
        return mEnterCarAsPassenger;
    }

    // 9.6f 0x4039D0
    inline u8 GetInternalObjectiveStatus_4039D0() const
    {
        return mInternalObjectiveStatus;
    }

    // 9.6f 0x433C20
    inline Fix16 GetGameObjectVelocity_433C20() const
    {
        return mGameObject->get_velocity_41B080();
    }

    // 9.6f 0x4AF860 (0x4AF880 is an identical copy)
    inline void SetTrainStation_4AF860(TrainStation* pStation)
    {
        if (pStation != mpTrainStation)
        {
            mpTrainStation = pStation;
        }
    }

    // 9.6f 0x4117D0 (0x4117F0 is an identical copy)
    inline TrainStation* PopTrainStation_4117D0()
    {
        TrainStation* pStation = mpTrainStation;
        mpTrainStation = NULL;
        return pStation;
    }

    // 9.6f 0x403A70
    inline void set_target_car_door_403A70(u8 v)
    {
        mTargetCarDoor = v;
    }

    // 9.6f 0x403AC0
    inline void set_objective_target_ped_403AC0(Ped* v)
    {
        mObjectiveTargetPed = v;
    }

    // 9.6f 0x403AD0
    inline Ped* get_objective_target_ped_403AD0() const
    {
        return mObjectiveTargetPed;
    }

    // 9.6f 0x403B00
    inline void set_target_to_enter_403B00(Car* v)
    {
        mTargetToEnter = v;
    }

    // 9.6f 0x403B20
    inline u16 get_internal_objective_timer_403B20() const
    {
        return mInternalObjectiveTimer;
    }

    // 9.6f 0x403B30
    inline u16 get_objective_timer_403B30() const
    {
        return mObjectiveTimer;
    }

    // 9.6f 0x403B40
    inline void set_objective_status_403B40(u8 v)
    {
        mObjectiveStatus = v;
    }

    // 9.6f 0x403B50
    inline void SetInternalObjectiveStatus_403B50(u8 v)
    {
        mInternalObjectiveStatus = v;
    }

    // 9.6f 0x433B80
    inline void SetOffscreenCounter_433B80(s16 v)
    {
        mOffscreenCounter = v;
    }

    // 9.6f 0x433C80
    inline void set_objective_timer_433C80(u16 v)
    {
        mObjectiveTimer = v;
    }

    // 9.6f 0x475B40
    inline void clear_last_char_punched_475B40()
    {
        mLastCharPunched = NULL;
    }

    // 9.6f 0x475B10
    inline Ped* get_last_char_punched_475B10() const
    {
        return mLastCharPunched;
    }

    // 9.6f 0x4A5020
    inline Char* get_game_object_4A5020() const
    {
        return mGameObject;
    }

    EXPORT Ped(); // 45AE70
    EXPORT ~Ped(); // 45AF00
    EXPORT void Reset_45AFC0();
    EXPORT void PoolAllocate();
    EXPORT bool IsLawEnforcement_45B4E0() const;
    EXPORT Fix16_Point GetVelocityVector_45B520() const;
    EXPORT void SetRecentCrimeTimer_45B550();
    EXPORT void SetPlayer_45B560(Player* pPlayer, bool bIsAuxPed);
    EXPORT bool IsEmergencyOccupation_45B590() const;
    EXPORT void CopyStatsFromPed_45B5B0(const Ped* pSrc);
    EXPORT Car* GetCarBeingEnteredOrExited_45BBF0() const;
    EXPORT void TeleportToCoord_45BC10(Fix16 xpos, Fix16 ypos);
    EXPORT void ManageShocking_45BC70();
    EXPORT bool CloseDoorIfCarApproaching_45BD20(Car* pCar);
    EXPORT s32 GetBulletSpriteOffset_45BE30() const;
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
    EXPORT void SpawnCharInZone_45C0C0(gmp_map_zone* pZone);
    EXPORT void PoolDeallocate();
    EXPORT void RespawnPed_45C350(gmp_map_zone* pZone);
    EXPORT void ResetForPlayerRespawn_45C410();
    EXPORT void UpdatePositionFromCar_45C4B0();
    EXPORT void ChangeNextPedState1_45C500(s32 new_state);
    EXPORT void ChangeNextPedState2_45C540(s32 new_state);
    EXPORT void RestorePreviousPedState_45C5A0();
    EXPORT void CancelEnterCarObjective_45C5C0();
    EXPORT void SpawnDriverRunAway_45C650(Car* pCar, Ped* pCarThief);
    EXPORT void SpawnPedInCar_45C730(Car* pCar);
    EXPORT void EnterCarByBackDoor_45C7A0(Car* pCar);
    EXPORT void EnterCarAsPassenger_45C7F0(Car* pCar);
    EXPORT bool AllocCharB4_45C830(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Ang16 GetTurnSpeed_45C900() const;
    EXPORT Fix16 GetPedVelocity_45C920() const;
    EXPORT Ang16 GetRotation() const;
    EXPORT Fix16 GetMoveDirection_45C9B0() const;
    EXPORT Ang16 ComputeAimAngle_45C9D0();
    EXPORT void HandleClosePedInteraction_45CAA0();
    EXPORT void TakeDamage(s16 damage);
    EXPORT void HandlePedCrossingTrigger_45CF20(const Object* pObj);
    EXPORT bool HandlePedHitByObject_45D000(Object* pObj);
    EXPORT bool AddWeaponWithAmmo_45DD30(s32 weapon_kind, char_type ammo);
    EXPORT bool HandlePickupCollision_45DE80(Object* pPickUp);
    EXPORT void SpawnWeaponOnDeath_45E080();
    EXPORT void StartCrossingRoad_45E4A0();
    EXPORT void DeallocateWithGroupCleanup_45EA00();
    EXPORT void Deallocate_45EB60();
    EXPORT bool IsActivePlayerPed_45EDC0() const;
    EXPORT bool PedTypeIs_45EDE0(s32 type) const;
  private:
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
  public:
    EXPORT void RemovePedWeapons_462510();
    EXPORT void RemoveSecondaryWeapon_462550();
    EXPORT void ForceDoNothing_462590();
    EXPORT void UpdateOnScreenCounter_462620();
    EXPORT bool StateMachineTick_4626B0();
    EXPORT void UpdateCharB4_462B80();
    EXPORT bool PoolUpdate();
    EXPORT void ProcessObjective_4632E0();
    EXPORT void ChangePedStatesByMode_463300(u8 mode);
    EXPORT void SetStatesForObjective_4633E0(bool bMainObj);
    EXPORT void SetObjective(s32 objective, s16 objective_timer = 9999); // 9999: never times out
    EXPORT void SetObjective2_463830(s32 internal_objective, s16 internal_objective_timer = 9999);
  private:
    EXPORT void ProcessMainObjective_463AA0();
    EXPORT void ProcessInternalObjective_463FB0();
    EXPORT void CalcApproachPointNearTargetPed_4645B0();
    EXPORT void Threat_Reaction_AI_465270();
    EXPORT void ReactToAttacker_465B20();
  public:
    EXPORT bool IsAttackingTargetPed_465CD0() const;
    EXPORT bool IsPedAThreat_465D00(Ped* pTargetPed);
    inline bool IsPedAThreat_Inline_465D00(Ped* pTargetPed);
    EXPORT bool IsInvisibleOnFoot_466B70() const;
    EXPORT bool IsThreatToSearchingPed_4661F0();
  private:
    EXPORT Ped* FindBestTargetPed_Mode1_466B90(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode4_466BB0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode5_466BD0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_466BF0(s32 max_x_check);
    EXPORT Ped* FindNearestPed_Mode4_466F40(u8 max_x_check);
    EXPORT Ped* FindNearestPed_466F60(u8 max_x_check);
    EXPORT Ped* FindNearbyPed_466FB0();
    EXPORT Ped* GetLastProcessedPedOnFoot_467070();
  public:
    EXPORT bool FindUsableCarDoor_467090();
    EXPORT Sprite* ResetAnimAndFindNearestSprite_467280();
  private:
    EXPORT void UpdateMovementTowardsTarget_4672E0(Fix16 distance, u8 type);
    EXPORT void FleeOnFootTillSafe_4678E0();
    EXPORT void FleeCharOnFootTillSafe_467960();
    EXPORT void FleeFromCharOnFootAlways_467A20();
    EXPORT void FleeCharAlwaysOnceCarStopped_467AD0();
    EXPORT void LeaveCarAndFleeFromIt_467BD0();
    EXPORT void KillCharOnFoot_467CA0();
    EXPORT void KillCharAnyMeans_467E20();
    EXPORT void KillFrenzy_467FB0();
    EXPORT void PunchChar_467FD0();
    EXPORT void WaitOnFootAtTarget_468040();
    EXPORT void TimeWaitedInCar_4682A0();
    EXPORT void GotoAreaInCar_468310();
    EXPORT void EnterTargetObjectiveCar_4686C0();
    EXPORT void LeaveTargetObjectiveCar_468820();
    EXPORT void EnterTrain_468930();
    EXPORT void LeaveTrain_468A00();
    EXPORT void LeaveCarThenStop_468BD0();
    EXPORT void PatrolOnFoot_468C70();
    EXPORT void GotoAreaOnFoot_468DE0();
    EXPORT void UpdateFollowPedObjective_468E80();
    EXPORT s32 GetValueByIdParity_469010() const;
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
    EXPORT void CallPoliceCar_469FE0();
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
    EXPORT void StayCloseToTargetPed_46B170();
    EXPORT void PullDriverOutOfCarStateMachine_46B2F0();
    EXPORT void MeleeAttackStateMachine_46B670();
    EXPORT void WaitOnFoot_46BD30();
    EXPORT bool IsOtherPedEnteringAsDriver_46BD50(Car* pCar);
    EXPORT void EnterCarStateMachine_46BDC0();
    EXPORT void ExitCarStateMachine_46C250();
    EXPORT void WalkToTargetPoint_46C770();
    EXPORT void GotoAreaOnFoot_46C7E0();
    EXPORT void WalkToTargetThenStop_46C8A0();
    EXPORT void FollowPathPoints_46C910();
    EXPORT void CrossRoad_46C9B0();
    EXPORT void GotoAreaInCar_46CA60();
    EXPORT void FollowPedInCar_46CA70();
    EXPORT void StartPedCrossingAtTrafficLight_Y_Backward_46CB30();
    EXPORT void StartPedCrossingAtTrafficLight_X_Forwards_46CC70();
    EXPORT void StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0();
    EXPORT void StartPedCrossingAtTrafficLight_X_Backwards_46CEF0();
    EXPORT void WaitForTrain_46D030();
    EXPORT void WaitForInternalObjectiveTimer_46D0B0();
    EXPORT void EnterTrainStateMachine_46D0D0();
    EXPORT void ExitTrainStateMachine_46D240();
    EXPORT void FollowCarOnFoot_46D300();
    EXPORT void AttackTargetStateMachine_46D460(u8 targetType);
    EXPORT void AttackPed_46DB60();
    EXPORT void AttackCar_46DB70();
    EXPORT void AttackObject_46DB80();
  public:
    EXPORT Sprite* GetSprite_46DF50() const;
    EXPORT void SetupFollower_46DF70(const Ped* pToFollow, s32 weaponIdx);
    EXPORT bool CanBeRecruitedToGroup_46E020(const PedGroup* pGroup) const;
    EXPORT void RecruitNearbyPeds_46E080(s32 desiredCount, Fix16 searchRadius);
    EXPORT void SpawnPedGroupFollowers_46E200(u8 total);
    EXPORT u8 get_wanted_star_count_46EF00() const;
    EXPORT void set_wanted_level_46EF40(u16 wanted);
    EXPORT void IncreaseWantedLevelFromDebugKeys_46EFD0();
    EXPORT void set_wanted_star_count_46F070(u8 star_count);
    EXPORT bool WantedStarCountGreaterThan_46F100(u8 star_count) const;
    EXPORT Weapon* GetWeaponFromPed_46F110();
    EXPORT void ApplyAimJitter_46F1E0(Weapon* pWeapon);
    EXPORT void ManageWeapon_46F390();
    EXPORT Weapon* ChooseAttackWeapon_46F490();
    EXPORT void ForceWeapon_46F600(s32 weapon_kind);
    EXPORT void GiveWeapon_46F650(s32 weapon_kind);
    EXPORT void ApplyGangRespectForKill_46F680(const Ped* pPed);
    EXPORT void UpdateStatsForKiller_46F720();
    EXPORT void Kill_46F9D0();
    EXPORT void AddThreateningPedToList_46FC70();
    EXPORT void HandleShootingAtCar_46FC90(Car* pCar, s32 model);
    EXPORT void ProcessWeaponHitResponse_46FE20(Object* pObj);
    EXPORT void NotifyWeaponHit_46FF00(Fix16 xpos, Fix16 ypos, s32 model);
    EXPORT void HandleWeaponFireEnd_46FFF0(s32 model);
    EXPORT void AimRoofGun_470050();
    EXPORT void add_wanted_points_470160(s16 wanted_amount);
    EXPORT bool IsNearestSpriteACar_4701D0() const;
    EXPORT void StartPedWalking_470200(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void BecomeLeaderOfGroup_4702D0(const Ped* pPed);
    EXPORT void BecomeDummyOnPlayerDisconnect_470300();
    EXPORT void PushPatrolPoint_4702A0(s8 x, s8 y, s8 z);
    EXPORT s32 IsInTrain_470F00() const;

  private:
    EXPORT void FleeCharAnyMeansTillSafe_Nullsub();
    EXPORT void FleeCharAnyMeansAlways_Nullsub();
    EXPORT void KillCar_Nullsub();
    EXPORT void Objective50_Nullsub();
    EXPORT void State30_Nullsub();
  public:

    inline u8 get_ped_ref_idx_420B50() const
    {
        return mPedRefIdx;
    }

    inline void ClearGroupAndGroupIdx_403A30()
    {
        this->mPedGroup = NULL;
        this->mGroupIdx = 0;
    }

    inline s32 GetPedType_420B70() const
    {
        return mPedType;
    }

    void ClearAttacking()
    {
        mBf21C.bAttacking = false;
    }

    // 9.6f 0x4A5060
    inline void SetElectrofingers_4A5060()
    {
        mBf21C.bElectroFingers = true;
    }

    // 9.6f 0x4A5050
    inline void SetFullHealth_4A5050()
    {
        set_health_4039A0(100);
    }

    inline void ClearElectrofingers_482080()
    {
        mBf21C.bElectroFingers = false;
    }

    bool IsActive_433B40() const
    {
        return mBf21C.bActive;
    }

    bool IsAttacking() const
    {
        return mBf21C.bAttacking;
    }

    // 9.6f inline 0x450CB0
    inline u8 GetObjectiveStatus_450CB0() const
    {
        return mObjectiveStatus;
    }

    void reset_ped_group()
    {
        mPedGroup = NULL;
        mGroupIdx = 0;
    }

    void set_ped_group(PedGroup* ptr)
    {
        mPedGroup = ptr;
    }

    // 9.6f 0x403940
    void set_ped_group_id(s8 group_idx)
    {
        mGroupIdx = group_idx;
    }

    u16 GetOffscreenCounter_4039F0() const
    {
        return mOffscreenCounter;
    }

    bool has_car_403B80() const
    {
        return mCar != NULL;
    }

    s32 GetPedState_403990() const
    {
        return mPedState1;
    }

    // 9.6f 0x492CB0
    void SetStolenCar_492CB0(Car* pCar)
    {
        mStolenCar = pCar;
    }

    // 9.6f 0x49EF40
    inline Car* GetStolenCar_49EF40() const
    {
        return mStolenCar;
    }

    // 9.6f inline 0x403AE0
    void SetInternalTargetPed_403AE0(Ped* pSrc)
    {
        mInternalTargetPed = pSrc;
    }

    // 9.6f inline 0x403950
    inline void SetPanicking_403950()
    {
        mBf21C.bPanicking = true;
    }

    bool IsPanicking() const
    {
        return mBf21C.bPanicking;
    }

    // 9.6f 0x403960
    void ClearPanicking_403960()
    {
        mBf21C.bPanicking = false;
    }

    // 9.6f 0x403AA0
    void SetTargetObjectiveCar(Car* ptr)
    {
        mTargetObjectiveCar = ptr;
    }

    Car* get_target_objective_car_403AB0() const
    {
        return mTargetObjectiveCar;
    }

    void SetPedType_403920(s32 type)
    {
        mPedType = type;
    }

    // 9.6f 0x403A00
    inline Fix16 get_cam_x() const
    {
        return mCam.x;
    }

    // 9.6f 0x403A10
    inline Fix16 get_cam_y() const
    {
        return mCam.y;
    }

    // 9.6f 0x416B50
    inline Fix16 get_cam_z() const
    {
        return mCam.z;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = mCam.x;
        Fix16 width = rect->mSize.mX;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (x_pos >= rect->mPos.x - width && x_pos <= rect->mPos.x + width &&
                (y_pos = mCam.y, height = rect->mSize.mY, y_pos >= rect->mPos.y - height) &&
                mCam.y <= rect->mPos.y + height &&
                (z_pos = mCam.z, z_target = rect->mPos.z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline s16 get_wanted_points_433DC0() const
    {
        return mWantedPoints;
    }

    inline bool not_enter_car_as_passenger_4A5040() const
    {
        return mEnterCarAsPassenger != 1;
    }

    inline Car* get_car_416B60() const
    {
        return mCar;
    }

    // 9.6f 0x420B60, 0x4C4F20
    u32 get_id() const
    {
        return mId;
    }

    inline Fix16 get_max_speed_1F0() const
    {
        return mMaxSpeed;
    }

    inline void SetStateInitPending_433B50()
    {
        mGameObject->mStateInitPending = true;
    }

    inline s8 get_remap_433BA0() const
    {
        return mRemap;
    }

    inline bool is_player_41B0A0() const
    {
        return mPlayer != NULL;
    }

    // 9.6f 0x433C40
    inline void DoJump_433C40()
    {
        if (mGameObject)
        {
            mGameObject->DoJump_5454D0();
        }
    }

    // 9.6f 0x41B0B0
    inline s32 TakeVoiceEvent_41B0B0()
    {
        s32 ret = mVoiceEvent; // sound id the ped says next
        mVoiceEvent = 0;
        return ret;
    }

    inline void SetVoiceEventIfNotOnFire_433DD0(s32 voice_event)
    {
        if (!mBf21C.bOnFire)
        {
            mVoiceEvent = voice_event;
        }
    }

    inline u8 get_target_car_door_403A60() const
    {
        return mTargetCarDoor;
    }

    // 9.6f inline 0x433B90
    void set_remap_433B90(u8 remap)
    {
        mRemap = remap;
    }

    // 9.6f inline 0x4039A0
    void set_health_4039A0(s16 health)
    {
        mHealth = health;
    }

    // 9.6f inline 0x433B70
    inline s32 get_health_433B70() const
    {
        return mHealth;
    }

    void set_occupation_403970(s32 occupation)
    {
        mOccupation = occupation;
    }

    s32 get_occupation_403980() const
    {
        return mOccupation;
    }

    void SetMoveTargetX_433C50(Fix16 x)
    {
        this->mMoveTargetX = x;
    }

    void SetMoveTargetY_433C60(Fix16 y)
    {
        this->mMoveTargetY = y;
    }

    void SetMoveTargetZ_433C70(Fix16 z)
    {
        this->mMoveTargetZ = z;
    }

    Fix16 GetMoveTargetX_492CE0() const
    {
        return mMoveTargetX;
    }

    Fix16 GetMoveTargetY_492CF0() const
    {
        return mMoveTargetY;
    }

    // In Ped.inl, which needs Car.hpp
    inline s32 get_car_model() const;

    // 9.6f 0x433C10: a thin wrapper around Char::SetRemap. The remap is taken by const reference:
    // by value, VC6 loads the argument before the Char and 9 callers stop matching.
    inline void SetRemap_433C10(const u8& remap)
    {
        Char* pChar = mGameObject; // local necessary to match Ped::SetupFollower_46DF70
        pChar->SetRemap_Inline(remap);
    }

    inline void TriggerVoiceEventRateLimited_433E50()
    {
        if (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0() - mLastVoiceRng) > 5)
        {
            SetVoiceEventIfNotOnFire_433DD0(25);
            mLastVoiceRng = gpRng_67AB34->get_cur_rng_41CFE0();
        }
    }

    inline void SetJumpOverMode_433BB0(s32 value)
    {
        mJumpOverMode = value;
    }

    // 9.6f 0x492CC0
    inline Ang16 GetTargetFacingAngle_492CC0() const
    {
        return mTargetFacingAngle;
    }

    // 9.6f 0x492C20
    inline s32 GetJumpOverMode_492C20() const
    {
        return mJumpOverMode;
    }

    inline void SetPedClass_433BC0(s32 value)
    {
        mPedClass = value;
    }

    // 9.6f 0x403A20
    inline void ClearHitCount_403A20()
    {
        mHitCount = 0;
    }

    // 9.6f 0x420B80
    inline void ClearWantedPoints_420B80()
    {
        mWantedPoints = cop_level_ped_enum::cop_0_stars;
    }

    // 9.6f 0x472FD0
    inline bool IsGroupLeader_472FD0() const
    {
        return mGroupIdx == 99;
    }

    // 9.6f 0x433BE0
    inline void ClearAttacker_433BE0()
    {
        mAttacker = NULL;
    }

    // 9.6f 0x433C00
    inline void SetRotation_433C00(Ang16 rotation)
    {
        mGameObject->set_rotation_433A30(rotation);
    }

    inline s32 GetPedState2_433B60() const
    {
        return mPedState2;
    }

    // Same body as the out-of-line copy IsInvisibleOnFoot_466B70: this form's front-end size (47) is what
    // makes the inlined IsPedAThreat in IsThreatToSearchingPed_4661F0 call the copy (nested budget 42)
    inline bool IsInvisibleOnFoot_433DA0() const
    {
        if (mBf21C.bInvisible && mGameObject != NULL)
        {
            return true;
        }

        return false;
    }

    bool bHasGameObject_403B70() const
    {
        return mGameObject != NULL;
    }

    inline bool GetPanicking() const
    {
        return mBf21C.bPanicking;
    }

    inline bool GetAttacking_433CA0() const
    {
        return mBf21C.bAttacking;
    }

    bool IsOnFire_475B50() const
    {
        return mBf21C.bOnFire;
    }

    bool HasSpottedPlayer() const
    {
        return mBf21C.bSpottedPlayer;
    }

    inline s32 get_objective_403A80() const
    {
        return mObjective;
    }

    inline Car* get_target_to_enter_403B10() const
    {
        return mTargetToEnter;
    }

    // 9.6f 0x4A5010
    inline void SetAttacking_4A5010()
    {
        mBf21C.bAttacking = true;
    }

    // 9.6f 0x403A40
    inline void ClearAttacking_403A40()
    {
        mBf21C.bAttacking = false;
    }

    inline bool IsPedGoingToEnterCar_492FD0() const
    {
        return mObjective == objectives_enum::enter_car_as_driver_35 || mInternalObjective == objectives_enum::enter_car_as_driver_35;
    }

    Ang16 GetAimAngle_4CCA90() const
    {
        return mAimAngle;
    }

    bool isDead_403B60() const
    {
        return this->mPedState1 == ped_state_1::dead_9;
    }

    PedGroup* GetGroup_475AF0() const
    {
        return this->mPedGroup;
    }

    inline s32 GetInternalObjective_403A90() const
    {
        return mInternalObjective;
    }

    inline Ped* GetInternalTargetPed_403AF0() const
    {
        return mInternalTargetPed;
    }

    inline void SetAttacker_433BF0(Ped* pPed)
    {
        mAttacker = pPed;
    }

    inline void IncrementAttackersCount_433BD0()
    {
        ++mAttackersCount;
    }

    PatrolPoint mPatrolPoints[100]; // 0x0
    Ang16 mFacingAngle; // 0x12C
    Ang16 mAimAngle; // 0x12E
    Ang16 mTargetFacingAngle; // 0x130
    Ang16 mFollowCarOffsetAngle; // 0x132
    Ang16 mRotation; // 0x134
    s16 mUnused136; // 0x136
    s32 mCollidedCharB4; // 0x138
    TrainStation* mpTrainStation; // 0x13C
    Car* mStolenCar; // 0x140
    Ped* mAttacker; // 0x144
    Ped* mObjectiveTargetPed; // 0x148
    Ped* mInternalTargetPed; // 0x14C
    Car* mTargetObjectiveCar; // 0x150
    Car* mTargetToEnter; // 0x154
    Car* mEscapeCar; // 0x158
    Player* mPlayer; // 0x15C
    Ped* mpNext;
    PedGroup* mPedGroup; // 0x164
    Char* mGameObject; // 0x168
    Car* mCar; // 0x16C
    Weapon* mSelectedWeapon; // 0x170
    Weapon* mpWeapon; // 0x174
    Weapon* mCarWeapon; // 0x178
    Gang* mpGang; // 0x17C
    Ped* mCarThief; // 0x180
    Object* mpObj2C; // 0x184
    Ped* mLastCharPunched; // 0x188
    PatrolPoint* mCurrentPathPoint; // 0x18C
    PatrolRoute* mPatrolRoute; // 0x190
    PatrolPoint* mCurrentPatrolPoint; // 0x194
    Ped* mHitTargetPed; // 0x198: ped last hit by this ped's weapon / electrobaton
    Gang* mDummyGang; // 0x19C: gang of a dummy ped
    Object* mObjectiveTargetObject; // 0x1A0
    Object* mInternalTargetObject; // 0x1A4
    Ped* mPedKiller; // 0x1A8
    Fix16_Vec mCam; // 0x1AC
    Fix16 mTargetX; // 0x1B8
    Fix16 mTargetY; // 0x1BC
    Fix16 mTargetZ; // 0x1C0
    Fix16 mMoveTargetX; // 0x1C4
    Fix16 mMoveTargetY; // 0x1C8
    Fix16 mMoveTargetZ; // 0x1CC
    Fix16 mInternalTargetX; // 0x1D0
    Fix16 mInternalTargetY; // 0x1D4
    Fix16 mInternalTargetZ; // 0x1D8
    Fix16 mObjectiveTargetX; // 0x1DC
    Fix16 mObjectiveTargetY; // 0x1E0
    Fix16 mObjectiveTargetZ; // 0x1E4
    Fix16 mTargetDistanceLimit; // 0x1E8
    s32 mUnused1EC; // 0x1EC
    Fix16 mMaxSpeed; // 0x1F0
    Fix16 mWalkSpeed; // 0x1F4
    Fix16 mRunSpeed; // 0x1F8
    Fix16 mFollowCarOffsetDistance; // 0x1FC
    u32 mId; // 0x200
    s32 mKillerId; // 0x204
    u16 mInvulnerability; // 0x208
    s16 mWantedPoints; // 0x20A
    u16 mTargetUnseenCounter; // 0x20C: frames the target was not seen; gives up after 15
    s16 mOffscreenCounter; // 0x20E
    u16 mShockCounter; // 0x210
    u16 mElectrocutionThreshold; // 0x212
    s16 mUnused214; // 0x214
    s16 mHealth; // 0x216
    u16 mObjectiveTimer; // 0x218
    u16 mInternalObjectiveTimer; // 0x21A

    PedFlags32 mBf21C; // 0x21C

    s32 mLastVoiceRng; // 0x220
    PedFlags8 mBf224; // 0x224
    u8 mObjectiveStatus; // 0x225: it uses objective_status enum
    u8 mInternalObjectiveStatus; // 0x226
    char_type mUnused227; // 0x227
    u8 mHitCount; // 0x228: times hit by the attacker
    u8 mMugCount; // 0x229: times mugged while mugging a player
    s32 mPedClass; // 0x22C: ped_class
    s32 mJumpOverMode; // 0x230: ped_jump_over_mode
    char_type mLifetimeTimer; // 0x234: ped_lifetime
    s32 mPedType; // 0x238
    u8 mGroupIdx; // 0x23C
    s32 mOccupation; // 0x240
    char_type mRemap; // 0x244
    s32 mEnterCarAsPassenger; // 0x248
    u8 mTargetCarDoor; // 0x24C
    s32 mVoiceEvent; // 0x250
    s32 mBlockSpec; // 0x254
    s32 mObjective; // 0x258
    s32 mInternalObjective; // 0x25C
    char_type mWaitCounter; // 0x260: frames waited on the target (mBf224.bWaitingOnTarget)
    char_type mUnused261; // 0x261
    u8 mAttackersCount; // 0x262
    u8 mPrevAttackersCount; // 0x263
    u8 mKillerIdTimer; // 0x264
    u8 mPatrolListIdx; // 0x265
    u8 mPathPointCount; // 0x266
    u8 mPedRefIdx; // 0x267
    char_type mElectrocutionTimer; // 0x268
    char_type mUnused269; // 0x269
    u8 mRecentCrimeTimer; // 0x26A
    s32 mGraphicType; // 0x26C
    s32 mFireMode; // 0x270: ped_fire_mode
    s32 mGangCarModel; // 0x274
    s32 mPedState1; // 0x278
    s32 mPedState2; // 0x27C
    s32 mStoredPedState1; // 0x280
    s32 mStoredPedState2; // 0x284
    s32 mThreatSearch; // 0x288
    s32 mThreatReaction; // 0x28C
    s32 mDeathCause; // 0x290: ped_death_cause
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped, 0x294)

EXPORT void __stdcall CarDoorAlignmentSolver_545AF0(u8 animPhase, Car* pCar, u8 doorId, Fix16& outX, Fix16& outY, Ang16& outAng);

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
