#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Police_38.hpp"

class Ped;
class Car;
class EmergencyCrew;
class Object;
class PolicePursuitTarget;
class Ang16;
class Police;

EXTERN_GLOBAL(Fix16, kFpOne64th_6FECA0);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FEB88);
EXTERN_GLOBAL(Fix16, kFpFour_6FECF8);
EXTERN_GLOBAL(Fix16, kFpOneSixteenth_6FEB0C);
EXTERN_GLOBAL(Fix16, kFpPoint8_6FEB68);
EXTERN_GLOBAL(Police*, gPolice_7B8_6FEE40);
EXTERN_GLOBAL(s32, gCrewKind_6FEDB8);

// Number of criminals the police can pursue at once (not the player count: MAX_PLAYERS is 6)
#define MAX_PURSUIT_TARGETS 4

// Size of the pool of police crews
#define MAX_POLICE_CREWS 20

class Police
{
  public:
    STATIC_MEMBER(u16, sIdCounter);
    STATIC_MEMBER(u32, sDispatchCrewType);
    STATIC_MEMBER(s32, sDispatchCrewState);
    STATIC_MEMBER(s16, sDispatchCrewSpawnDelay);
    STATIC_MEMBER(u8, sbHaveCriminals);
    Police()
    {
        Init_56F400();
    }

    EXPORT ~Police();

  private:
    EXPORT void Init_56F400();

  public:
    EXPORT bool TryReplaceCrewLeaderOnDeath_56F4D0(Ped* pPed);

  private:
    EXPORT PoliceCrew* NewCrew_56F560();

  public:
    EXPORT Ped* SpawnRoadblockGuard_56F5C0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT void DespawnCrewInCar_56F6D0(Car* pCar);
    EXPORT bool IsBeingPursued_56F800(Ped* pCriminal) const;
    EXPORT bool IsActivelyChased_56F880(Ped* pCriminal) const;
    EXPORT void SetArrestedPed_56F8E0(Ped* pCriminal, Ped* pAuxPed);
    EXPORT void RegisterCriminal_56F940(Ped* pCriminal);

  private:
    EXPORT void UpdateFirstPursuitTimer_56FA40();
    EXPORT bool DispatchNewCrewToPursuit_56FAA0(PolicePursuitTarget* pPursuitTarget);
    EXPORT void UpdatePursuitTargets_56FBD0();

  public:
    EXPORT void Service_570270();
    EXPORT void SpawnWalkingGuard_570320(Ped* pPed, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT bool SpawnCrewInCar_5703E0(Car* pCar);
    EXPORT bool AssignCrewToPursuit_570790(PoliceCrew* pCrew, PolicePursuitTarget* pPursuitTarget);
    EXPORT bool TryAssignCarCrewToCriminal_5707B0(Car* pCar, Ped* pCriminal);
    EXPORT void UpdateLastSeenCoordsForCriminal_5708C0(Ped* pPed);
    EXPORT void UpdateCriminalLatestPosition_570940(Ped* pPed);
    EXPORT bool TryBeginRoadblock_577320();
    EXPORT void TryCreateRoadblockAt_577370(u8 tileX, u8 tileY, s32 roadblock_type);

  private:
    u8 mUnused0; // 0x0: set to 1 in Init, never read

  public:
    PoliceCrew mCopCrew[MAX_POLICE_CREWS]; // 0x4
    PolicePursuitTarget mPursuitTargets[MAX_PURSUIT_TARGETS]; // 0x464: one per wanted criminal
    s32 mMaxWantedLevel; // 0x654
    u8 mPoliceCarCount; // 0x658
    char_type mMaxPoliceCars; // 0x659
    s32 mHighestCrewTypeInPursuit; // 0x65C
    u8 mMaxWantedStars; // 0x660

  private:
    PoliceRoadblock mRoadblock1; // 0x664
    PoliceRoadblock mRoadblock2; // 0x708
    u8 mRoadblockCooldown; // 0x7AC

  public:
    char_type mPolicePedsInRangeScreen; // 0x7AD
    Ped* mLastFiringEmergencyPed; // 0x7B0
    bool mCrewPedOnscreen; // 0x7B4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Police, 0x7B8);
