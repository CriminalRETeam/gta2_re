#pragma once

#include "Function.hpp"

class Ped;
class Car;
class Object;

// MissionCleanupEntry::mType
namespace cleanup_entry_type
{
enum
{
    empty_0 = 0,
    car_1 = 1,
    object_2 = 2,
    ped_3 = 3,
};
} // namespace cleanup_entry_type

class MissionCleanupEntry
{
  public:
    union
    {
        Car* mCar; // 0x0
        Object* mObj; // 0x0
        Ped* mPed; // 0x0
    };
    u8 mType; // 0x4: determines what the union types are
    s8 mUnused5; // 0x5
    s8 mUnused6; // 0x6
    s8 mUnused7; // 0x7
    s32 mEntityId; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(MissionCleanupEntry, 0xC);

class MissionCleanupList
{
  public:
    // throw(): ScriptManager's ctor has no EH state around new MissionCleanupList()
    MissionCleanupList() throw();
    EXPORT void MissionCleanUp();
    EXPORT MissionCleanupEntry* allocate_next();
    EXPORT void push_type_1_car(Car* pCar);
    EXPORT void push_type_3_ped(Ped* pPed);
    EXPORT void push_type_2_non_pickup_object(Object *pObj, bool bForce);
    EXPORT void push_type_2_object(Object* pObj);

    MissionCleanupEntry mEntries[50]; // 0x0

    u8 mCount; // 0x258
    char_type mUnused259; // 0x259
    char_type mUnused25A; // 0x25A
    char_type mUnused25B; // 0x25B
};
GTA2_ASSERT_SIZEOF_ALWAYS(MissionCleanupList, 0x25C);

EXTERN_GLOBAL(MissionCleanupList*, gMiss2_25C);
