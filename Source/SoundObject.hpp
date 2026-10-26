#pragma once

#include "Function.hpp"

namespace SoundObjectTypeEnum
{
enum
{
    Hud_Pager_C_11 = 11,
    Vocals_10 = 10,
    Crusher_30_9 = 9,
    Crane_15C_8 = 8,
    Weapon_30_7 = 7,
    CollisionSoundQueue_6 = 6,
    Camera_0xBC_5 = 5,
    Unknown_4 = 4,
    Radio_3 = 3,
    Unknown_2 = 2,
    Sprite_1 = 1,
};
} // namespace SoundObjectTypeEnum

// A sound emitting object registered with root_sound (a pool of 1000). Its owner is in field_C_pAny; sound_obj
// processes it each frame depending on field_0_object_type.
class SoundObject
{
  public:
    s32 field_0_object_type; // SoundObjectTypeEnum
    bool field_4_bStatus;
    s32 field_8_sound_entry;

    union SoundObjectType
    {
        class Hud_Pager* pHud_Pager_C;
        class Crusher* pCrusher_30;
        class Crane* pCrane_15C;
        class Weapon* pWeapon_30;
        class CollisionSoundQueue* pCollisionSoundQueue;
        class Camera* pCamera_0xBC;
        class SoundObject* pNextFree; // while the object is on the free list of root_sound
        class Sprite* pSprite;
        void* pAny;
    };

    // Type depends on what field_0_object_type is
    SoundObjectType field_C_pAny;

    EXPORT void Release_40EF20();
};
GTA2_ASSERT_SIZEOF_ALWAYS(SoundObject, 0x10);