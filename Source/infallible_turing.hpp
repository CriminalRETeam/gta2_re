#pragma once

#include "Function.hpp"

namespace SoundObjectTypeEnum
{
enum
{
    Hud_Pager_C_11 = 11,
    Unknown_10 = 10,
    Crusher_30_9 = 9,
    Crane_15C_8 = 8,
    Weapon_30_7 = 7,
    Rozza_C88_6 = 6,
    Camera_0xBC_5 = 5,
    Unknown_4 = 4,
    Unknown_3 = 3,
    infallible_turing_2 = 2,
    Sprite_1 = 1,
};
} // namespace SoundObjectTypeEnum

class infallible_turing
{
  public:
    s32 field_0_object_type; // SoundObjectTypeEnum
    char_type field_4_bStatus;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    s32 field_8;

    union SoundObjectType
    {
        class Hud_Pager_C* pHud_Pager_C;
        class Crusher_30* pCrusher_30;
        class Crane_15C* pCrane_15C;
        class Weapon_30* pWeapon_30;
        class Rozza_C88* pRozza_C88;
        class Camera_0xBC* pCamera_0xBC;
        class infallible_turing* pInfallible_turing;
        class Sprite* pSprite;
        void* pAny;
    };

    // Type depends on what field_0_object_type is
    SoundObjectType field_C_pAny;

    EXPORT void release_40EF20();
};