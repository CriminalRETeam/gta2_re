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
        Car* field_0_car;
        Object* field_0_obj;
        Ped* field_0_ped;
    };
    u8 field_4_type; // determines what the union types are
    s8 field_5_unused;
    s8 field_6_unused;
    s8 field_7_unused;
    s32 field_8_entity_id;
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
    EXPORT void push_type_2_502FF0(Object *pObj, bool bForce);
    EXPORT void push_type_2_503050(Object* pObj);

    MissionCleanupEntry field_0_entries[50];

    u8 field_258_count;
    char_type field_259_unused;
    char_type field_25A_unused;
    char_type field_25B_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(MissionCleanupList, 0x25C);

EXTERN_GLOBAL(MissionCleanupList*, gMiss2_25C_6F805C);
