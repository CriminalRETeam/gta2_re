#pragma once

#include "Function.hpp"
#include "Object_3C.hpp"
#include "Phi_8CA8.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"

class Object_2C;
class Sprite;
class Sprite_4C;
class Car_BC;
class Phi_74;
class Object_3C;
class Object_2C_Pool;
class Wolfy_30;
class Object_8;
class nostalgic_ellis_0x28;

class Object_2C
{
  public:
    EXPORT Object_2C();
    EXPORT void PoolDeallocate();
    EXPORT bool sub_522250(Sprite* a2);
    EXPORT s32 sub_5222B0();
    EXPORT void sub_5222D0();
    EXPORT void PoolGive_522340();
    EXPORT void PoolTake_522360();
    EXPORT char_type ShouldCollideWith_5223C0(Sprite* a2);
    EXPORT bool sub_522430(Sprite* a2);
    EXPORT char_type sub_522460(Sprite* a2);
    EXPORT s16* sub_5224E0(Fix16_Point* a2);
    EXPORT void sub_522640(Fix16_Point* a2);
    EXPORT void sub_5226A0(char_type a2);
    EXPORT void sub_522710(Object_2C* a2, u32* a3);
    EXPORT void sub_5229B0(s32 a2, u32* a3, s32 a4);
    EXPORT void sub_522B20(s32 a2, s32 a3, s32* a4);
    EXPORT void sub_522BE0(u32* a2);
    EXPORT void sub_522D00(u32* a2);
    EXPORT void sub_522E10(s32* a2);
    EXPORT char_type sub_5233A0(s32 a2);
    EXPORT void sub_523440(s32 a2, s32 a3, char_type a4, char_type a5);
    EXPORT char_type sub_5235B0(Sprite* a2, u32* a3, u8* a4, s32 a5);
    EXPORT void sub_524630(s32 a2, s16 a3);
    EXPORT void sub_525190(u8 varrok_idx);
    EXPORT void UpdateAninmation_5257D0();
    EXPORT bool sub_525910();
    EXPORT char sub_525370(Sprite* pSprite);
    EXPORT void sub_525AE0();
    EXPORT void sub_525B40();
    EXPORT char_type sub_525B60();
    EXPORT void sub_525B80();
    EXPORT void sub_525D90();
    EXPORT void Update_525F30();
    EXPORT bool PoolUpdate();
    EXPORT void sub_526790(s32 a2);
    EXPORT s32 sub_526830(s32 a1);
    EXPORT s16 sub_526B40(s32 a2);
    EXPORT char_type sub_527070(s16* a2, s32 a3, s16* a4, s32 a5);
    EXPORT void sub_527630(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT void Light_527990();
    EXPORT void sub_527AE0();
    EXPORT void RemoveFromCollisionBuckets_527D00();
    EXPORT void sub_527F10();
    EXPORT s16* sub_528130(Fix16_Point* a2);
    EXPORT char_type sub_528240(s32 a2, s32 a3);
    EXPORT void sub_5283C0(s32 a2);
    EXPORT bool OnObjectTouched_5288B0(Sprite* a2);
    EXPORT void sub_528900();
    EXPORT char_type HandleObjectHitIfExplosive_528960(Object_2C* pOther);
    EXPORT char_type HandleObjectHit_528990(Sprite* a2);
    EXPORT void ProcessObjectExplosionImpact_528A20(Object_2C *pObj);
    EXPORT void HandleImpactNoSprite_528BA0();
    EXPORT void HandleImpact_528E50(Sprite* a3);
    EXPORT void HandleCollisionWithObject_529000(Object_2C* pObj);
    EXPORT void sub_529030(s8 speed_x, s8 speed_y);
    EXPORT void sub_529070(Object_2C* pObj);
    EXPORT s32 sub_529210();
    EXPORT s32 sub_529240();
    EXPORT void sub_5292D0();
    EXPORT void SetDamageOwner_529080(u8 a2);
    EXPORT void sub_5290A0();
    EXPORT void sub_5290B0();
    EXPORT void Dealloc_5291B0();
    EXPORT void sub_5291D0();
    EXPORT void sub_5291E0(u8 a2);
    EXPORT bool sub_529200();
    EXPORT ~Object_2C();
    EXPORT void EnsureObject3C_52A650();
    EXPORT void ReactivateObjectAfterImpact_52A6D0(Sprite* a2);
    EXPORT Fix16_Point GetXY_52AE70();
    EXPORT Fix16_Point GetRot_52AE90();

    // TODO: ordering
    EXPORT void sub_5290C0(u8 id_base);
    EXPORT Fix16 sub_5290F0();
    EXPORT char sub_525AC0();
    EXPORT void sub_525B20();
    EXPORT void UpdateLight_527A30();
    EXPORT void sub_525100();
    
    inline bool check_is_busy_shop()
    {
        s32 v1 = field_8->field_34;
        return v1 == 10;
    }

    // Inlined on version 9.6f 0x447e90
    inline void set_field_26(u8 v)
    {
        field_26_varrok_idx = v;
    }

    // Inlined on version 9.6f 0x420FF0
    inline u8 get_field_26_420FF0()
    {
        return field_26_varrok_idx;
    }

    s32 get_model_40FEF0()
    {
        return field_18_model;
    }

    bool check_is_shop_421060()
    {
        return field_8->field_34 == 6 || field_8->field_34 == 7 || field_8->field_34 == 8 || field_8->field_34 == 9;
    }

    inline void PoolAllocate()
    {
        field_26_varrok_idx = 99;
        field_10_obj_3c = 0;
        field_C_pAny.o8 = 0;
        field_1C = 0;
        field_4 = 0;
        field_18_model = 0;
    }

    Object_2C* mpNext;
    Sprite* field_4;
    Phi_74* field_8;
    union TAny
    {
        Wolfy_30* pExplosion;
        Object_8* o8;
        nostalgic_ellis_0x28* pLight;
    };
    TAny field_C_pAny;
    Object_3C* field_10_obj_3c;
    s32 field_14_id;
    s32 field_18_model;
    char_type field_1C;
    char_type field_1D;
    char_type field_1E;
    char_type field_1F;
    s32 field_20;
    u8 field_24;
    u8 field_25;
    u8 field_26_varrok_idx;
    char_type field_27;
    char_type field_28;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
};

struct TurkishDelight_164;

class Object_5C
{
  public:
    EXPORT void sub_529300();
    EXPORT void ObjectsService_5293A0();
    EXPORT Object_5C();
    EXPORT ~Object_5C();
    EXPORT void sub_5297F0();
    EXPORT s32 sub_5298E0(s32 a2);
    EXPORT Object_2C* NewTouchPoint_529950(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 a9);
    EXPORT Object_2C* NewPhysicsObj_5299B0(s32 object_type, Fix16 a3, Fix16 a4, Fix16 a5, Ang16 a6);
    EXPORT Object_2C* sub_5299F0(s32 a2, u32 a3, Fix16 a4, Fix16 a5, Fix16 a6);
    EXPORT Object_2C* sub_529A40(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u8 a7);
    EXPORT Object_2C* sub_529AB0(s32 a2, Fix16 a3, Fix16 a4, Fix16 a5, s32 a6, s32 a7, u8 a8);
    EXPORT Object_2C* sub_529B20(s32 obj_type,
                                 s32 a3,
                                 s32 a4,
                                 s32 argb,
                                 s32 a6,
                                 u8 intensity,
                                 char_type on_time,
                                 char_type off_time,
                                 u8 shape);
    EXPORT Object_2C* sub_529BC0(s32 a2, Fix16 a3, Fix16 a4, Fix16 a5, Ang16 a6);
    EXPORT Object_2C* sub_529C00(s32 object_type, Fix16 a3, Fix16 a4, Fix16 a5, Ang16 a6, char_type a7);
    EXPORT char_type sub_52A210(char_type a2);
    EXPORT s32* sub_52A240(s32 a2, s32 maybe_x, s32 maybe_y, s32 maybe_z, s16 pCarBC, s16 maybe_ang, s32 a8, s32 a9, s32 a10);
    EXPORT s32* sub_52A280(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10);
    EXPORT s32* sub_52A2C0(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10, char_type a11);
    EXPORT Object_2C* CreateExplosion_52A3D0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 a6, s32 pedId);

    EXPORT void SaveObjects_52A500(TurkishDelight_164* pUnknownObj);
    EXPORT void RestoreObjects_52A590(TurkishDelight_164* pUnknownObj);
    EXPORT void sub_52A610(Object_2C* p2C);

    Object_2C* field_0; // Object_2C* ?
    Object_2C* field_4;
    Object_2C* field_8;
    Object_2C* field_C;
    s32 field_10;
    s32 field_14;
    u8 field_18;
    u8 field_19;
    u16 field_1A;
    struct_4 field_1C;
    u8 field_20[50];
    u16 field_52;
    s32 field_54_f20_idx;
    Sprite* field_58;
};

EXTERN_GLOBAL(Object_5C*, gObject_5C_6F8F84);
