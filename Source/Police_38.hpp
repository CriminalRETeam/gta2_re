#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class EmergencyCrew;
class Ped;
class Car;
class Object;
class PolicePursuitTarget;
class Fix16_Rect;

namespace police_crew_state
{
enum
{
    none_0 = 0,
    patrol_1 = 1,
    despawning_2 = 2, // PoliceCrew::TryDespawnOffscreenCrew, back to shutdown_6 next update
    alerted_search_3 = 3,
    pursue_or_chase_5 = 5,
    shutdown_6 = 6,
};
} // namespace police_crew_state

// PolicePursuitTarget::mState, see Police::UpdatePursuitTargets
namespace pursuit_state
{
enum
{
    idle_0 = 0,       // criminal registered, no crew sent yet
    responding_1 = 1, // first crew sent, waiting until the criminal is spotted
    active_3 = 3,     // crews engaged, more are sent or sent away as the wanted level changes
    ended_4 = 4,      // wanted level gone or criminal dead: release the crews and clear the target
    searching_5 = 5,  // criminal not seen for a while: crews search, a new one is sent if none is
};
} // namespace pursuit_state

class PoliceCrew
{
  public:
    STATIC_MEMBER(Ped*, sCurrentCrewPed);
    EXPORT PoliceCrew();
    EXPORT ~PoliceCrew();
    EXPORT void Init();
    EXPORT void AddToPursuit();
    EXPORT void RemoveFromPursuit();
    EXPORT void SpawnPoliceInCar();
    EXPORT void SpawnSWAT();
    EXPORT void SpawnFBI_nonused();
    EXPORT void TryDespawnOffscreenCrew();
    EXPORT void TryDespawnFinishedCrew();
    EXPORT void TryDespawnInCarCrew();
    EXPORT void HandleShutdownAndDespawn();
    EXPORT bool CanTrackCriminal() const;
    EXPORT void State3_AlertedSearch();
    EXPORT void State5_PursueOrChase();
    EXPORT void State6_ShutDown();
    EXPORT void State1_Patrol();
    EXPORT void State2_SwitchToShutDown();
    EXPORT void HandleEnterCarObjective();
    EXPORT void HandleLeaveCarObjective();
    EXPORT void HandleGotoAreaInCarObjective();
    EXPORT void HandleChaseInCarObjective();
    EXPORT void Service();
    EXPORT void UnlinkFromPursuitTarget();

    u16 mId; // 0x0
    u8 mTargX; // 0x2
    u8 mTargY; // 0x3
    u8 mTargZ; // 0x4
    char_type mUnused5; // 0x5
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    Fix16 mCriminalDistance; // 0x8
    Fix16 mUnusedC; // 0xC
    EmergencyCrew* mSubObj; // 0x10
    PolicePursuitTarget* mpPursuitTarget; // 0x14: Pursuit of one wanted criminal by police crews
    s16 mUnused18; // 0x18
    char_type mUnused1A; // 0x1A
    char_type mUnused1B; // 0x1B
    s32 mUsed; // 0x1C
    s32 mCrewKind; // 0x20: crew_kind
    s32 mState; // 0x24
    bool mUseCar; // 0x28
    bool mbCountedInPoliceCount; // 0x29
    s16 mUnused2A; // 0x2A
    s16 mUnused2C; // 0x2C
    char_type mUnused2E; // 0x2E
    char_type mUnused2F; // 0x2F
    Ped* mPrevTargetPed; // 0x30
    bool mIsDismissed; // 0x34
    u8 mFastTargetCounter; // 0x35
    char_type mUnused36; // 0x36
    char_type mUnused37; // 0x37
};
GTA2_ASSERT_SIZEOF_ALWAYS(PoliceCrew, 0x38);

class PolicePursuitTarget  // Pursuit of one wanted criminal by police crews
{
  public:
    Ped* mCriminalPed; // 0x0
    s32 mWantedLevel; // 0x4
    s32 mState; // 0x8: pursuit_state
    u16 mTimer; // 0xC
    char_type mBlockCounter; // 0xE
    char_type mUnusedF; // 0xF
    Fix16 mX; // 0x10
    Fix16 mY; // 0x14
    Fix16 mZ; // 0x18
    bool mCrewGaveUp; // 0x1C
    char_type mUnused1D; // 0x1D
    char_type mUnused1E; // 0x1E
    char_type mUnused1F; // 0x1F
    PoliceCrew* mCrews[6]; // 0x20
    s32 mUnused38; // 0x38
    s32 mUnused3C; // 0x3C
    s32 mUnused40; // 0x40
    s32 mUnused44; // 0x44
    s32 mUnused48; // 0x48
    s32 mUnused4C; // 0x4C
    s32 mUnused50; // 0x50
    s32 mUnused54; // 0x54
    s32 mBarrier1Id; // 0x58
    s32 mBarrier2Id; // 0x5C
    s32 mBarrier3Id; // 0x60
    s32 mBarrier4Id; // 0x64
    s32 mBarrier5Id; // 0x68
    s32 mBarrier6Id; // 0x6C
    u8 mNumPoliceCrews; // 0x70
    u8 mTargetPoliceCrews; // 0x71
    u8 mNumSwatCrews; // 0x72
    u8 mNumFbiCrews; // 0x73
    u8 mNumArmyCrews; // 0x74
    u8 mNumCrews; // 0x75
    s16 mUnused76; // 0x76
    bool mIsActivelyChased; // 0x78
    char_type mUnused79; // 0x79
    s16 mWantedTimer; // 0x7A
};
GTA2_ASSERT_SIZEOF_ALWAYS(PolicePursuitTarget, 0x7C);

class PoliceRoadblock
{
  public:
    EXPORT void Reset();
    EXPORT void Update();
    EXPORT void RemoveRoadblock();
    EXPORT bool CreateRoadblock(u8 x, u8 y, u8 z, s32 orientation);

    EXPORT PoliceRoadblock();
    EXPORT ~PoliceRoadblock();
    bool mbActive; // 0x0
    char_type mUnused1; // 0x1
    char_type mUnused2; // 0x2
    char_type mUnused3; // 0x3
    s32 mUnused4; // 0x4
    u8 mX; // 0x8
    u8 mY; // 0x9
    char_type mUnusedA; // 0xA
    char_type mUnusedB; // 0xB
    u16 mStartupTimer; // 0xC
    s16 mUnseenTimer; // 0xE
    Car* mCar1; // 0x10
    Car* mCar2; // 0x14
    Car* mCar3; // 0x18
    Car* mCar4; // 0x1C
    Car* mCar5; // 0x20
    Car* mCar6; // 0x24
    Object* mBarrier1; // 0x28
    Object* mBarrier2; // 0x2C
    Object* mBarrier3; // 0x30
    Object* mBarrier4; // 0x34
    Object* mBarrier5; // 0x38
    Object* mBarrier6; // 0x3C
    Object* mBarrier7; // 0x40
    Object* mBarrier8; // 0x44
    Object* mBarrier9; // 0x48
    Object* mBarrier10; // 0x4C
    Object* mBarrier11; // 0x50
    Object* mBarrier12; // 0x54
    s32 mBarrier1Id; // 0x58
    s32 mBarrier2Id; // 0x5C
    s32 mBarrier3Id; // 0x60
    s32 mBarrier4Id; // 0x64
    s32 mBarrier5Id; // 0x68
    s32 mBarrier6Id; // 0x6C
    s32 mBarrier7Id; // 0x70
    s32 mBarrier8Id; // 0x74
    s32 mBarrier9Id; // 0x78
    s32 mBarrier10Id; // 0x7C
    s32 mBarrier11Id; // 0x80
    s32 mBarrier12Id; // 0x84
    Ped* mGuard1; // 0x88
    Ped* mGuard2; // 0x8C
    Ped* mGuard3; // 0x90
    Ped* mGuard4; // 0x94
    Ped* mGuard5; // 0x98
    Ped* mGuard6; // 0x9C
    Fix16_Rect* mRect; // 0xA0
};
GTA2_ASSERT_SIZEOF_ALWAYS(PoliceRoadblock, 0xA4);