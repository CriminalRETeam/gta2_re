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
EXTERN_GLOBAL(Police*, gPolice_7B8_6FEE40);

// Number of criminals the police can pursue at once (not the player count: MAX_PLAYERS is 6)
#define MAX_PURSUIT_TARGETS 4

// Size of the pool of police crews
#define MAX_POLICE_CREWS 20

class Police
{
  public:
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
    EXPORT bool IsBeingPursued_56F800(Ped* pCriminal);
    EXPORT bool IsActivelyChased_56F880(Ped* pCriminal);
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
    u8 field_0; // set to 1 in Init, never read

  public:
    PoliceCrew field_4_cop_crew[MAX_POLICE_CREWS];
    PolicePursuitTarget field_464_pursuit_targets[MAX_PURSUIT_TARGETS]; // one per wanted criminal
    s32 field_654_max_wanted_level;
    u8 field_658_police_car_count;
    char_type field_659_max_police_cars;
    s32 field_65C_highest_crew_type_in_pursuit;
    u8 field_660_max_wanted_stars;

  private:
    PoliceRoadblock field_664_roadblock_1;
    PoliceRoadblock field_708_roadblock_2;
    u8 field_7AC_roadblock_cooldown;

  public:
    char_type field_7AD_police_peds_in_range_screen;
    Ped* field_7B0_last_firing_emergency_ped;
    bool field_7B4_crew_ped_onscreen;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Police, 0x7B8);
