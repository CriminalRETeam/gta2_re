#pragma once

#include "Function.hpp"

class Ped;
class Car;
class Object;

// MissionCleanupEntry::field_4_type
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
        Car* field_0_car; // 0x0
        Object* field_0_obj; // 0x0
        Ped* field_0_ped; // 0x0
    };
    u8 field_4_type; // 0x4: determines what the union types are
    s8 field_5_unused; // 0x5
    s8 field_6_unused; // 0x6
    s8 field_7_unused; // 0x7
    s32 field_8_entity_id; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(MissionCleanupEntry, 0xC);

class MissionCleanupList
{
  public:
    // throw(): ScriptManager's ctor has no EH state around new MissionCleanupList()
    MissionCleanupList() throw();
    EXPORT void MissionCleanUp_502DC0();
    EXPORT MissionCleanupEntry* allocate_next_502F60();
    EXPORT void push_type_1_car_502F80(Car* pCar);
    EXPORT void push_type_3_ped_502FB0(Ped* pPed);
    EXPORT void push_type_2_non_pickup_object_502FF0(Object *pObj, bool bForce);
    EXPORT void push_type_2_object_503050(Object* pObj);

    MissionCleanupEntry field_0_entries[50]; // 0x0

    u8 field_258_count; // 0x258
    char_type field_259_unused; // 0x259
    char_type field_25A_unused; // 0x25A
    char_type field_25B_unused; // 0x25B
};
GTA2_ASSERT_SIZEOF_ALWAYS(MissionCleanupList, 0x25C);

EXTERN_GLOBAL(MissionCleanupList*, gMiss2_25C_6F805C);
