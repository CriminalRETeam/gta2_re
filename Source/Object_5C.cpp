#include "Object_5C.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Light_1D4CC.hpp"
#include "Object_2C_Pool.hpp"
#include "Object_3C_Pool.hpp"
#include "Object_8_Pool.hpp"
#include "Particle_8.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_8.hpp"
#include "Wolfy_3D4.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Varrok_7F8*, gVarrok_7F8_703398);
EXTERN_GLOBAL(Ang16, kZeroAng_6F8F68);

DEFINE_GLOBAL(Object_5C*, gObject_5C_6F8F84, 0x6F8F84);
DEFINE_GLOBAL(s32, DAT_006f8f88, 0x6f8f88);
s32 dword_6F8F88; //DEFINE_GLOBAL(s32, dword_6F8F88, 0x6F8F88);
DEFINE_GLOBAL(Fix16_Point, stru_6F8EF0, 0x6F8EF0);
DEFINE_GLOBAL(Fix16, kFpZero_6F8E10, 0x6F8E10);

DEFINE_GLOBAL(u8, byte_6F8C68, 0x6F8C68);
DEFINE_GLOBAL(u8, byte_6F8C4C, 0x6F8C4C);
DEFINE_GLOBAL(u8, byte_6F8F40, 0x6F8F40);
DEFINE_GLOBAL(u32, dword_6F8E54, 0x6F8E54);
DEFINE_GLOBAL(u32, dword_6F8F18, 0x6F8F18);
DEFINE_GLOBAL(u32, dword_6F8DC0, 0x6F8DC0);
DEFINE_GLOBAL(u32, dword_6F8F0C, 0x6F8F0C);
DEFINE_GLOBAL(u8, byte_6F8EDC, 0x6F8EDC);

DEFINE_GLOBAL(Ang16, word_6F8D8C, 0x6F8D8C);
DEFINE_GLOBAL(Ang16, dword_6F8D80, 0x6F8D80);
DEFINE_GLOBAL(Ang16, word_6F8D54, 0x6F8D54);
DEFINE_GLOBAL(Ang16, dword_6F8CD0, 0x6F8CD0);

DEFINE_GLOBAL(Sprite*, dword_6F8F8C, 0x6F8F8C);
DEFINE_GLOBAL(u8, byte_6F8F94, 0x6F8F94);
DEFINE_GLOBAL(s32, dword_6F8F5C, 0x6F8F5C);
DEFINE_GLOBAL(Fix16, dword_6F8DA8, 0x6F8DA8);
DEFINE_GLOBAL(Fix16, dword_6F8E14, 0x6F8E14);
DEFINE_GLOBAL(Fix16, k_dword_6F8C58, 0x6F8C58);
DEFINE_GLOBAL(u8, byte_623EC4, 0x623EC4);

DEFINE_GLOBAL_INIT(Fix16, dword_6F8DC8, Fix16(256, 0), 0x6F8DC8);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8CE8, Fix16(12), 0x6F8CE8);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8CEC, Fix16(1), 0x6F8CEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8ECC, dword_6F8DC8, 0x6F8ECC);

MATCH_FUNC(0x522140)
Object_2C::Object_2C()
{
    mpNext = 0;
    field_4 = 0;
    field_18_model = 0;
    field_8 = 0;
    field_C_pAny.o8 = 0;
    field_10_obj_3c = 0;
    field_14_id = 99;
    field_24_bDoneThisFrame = 0;
    field_25 = 0;
    field_26_varrok_idx = 99;
    field_20 = 0;
    field_27 = 0;
    field_28 = -51;
    field_1C = 0;
}

MATCH_FUNC(0x522180)
void Object_2C::PoolDeallocate()
{
    this->field_18_model = 0;

    // TODO: Local required, inline?
    const s32 f5C = field_8->field_5C;
    if (f5C == 2)
    {
        --gObject_5C_6F8F84->field_10;
    }
    else if (f5C == 3)
    {
        --gObject_5C_6F8F84->field_14;
        gObject_5C_6F8F84->field_1C.sub_5A6B60(this->field_4);
    }

    --dword_6F8F88;

    const s32 phi_type = this->field_8->field_34;
    if (phi_type != 6 && phi_type != 7 && phi_type != 8 && phi_type != 9 && phi_type != 10 && phi_type != 1 && phi_type != 12)
    {
        if (field_26_varrok_idx > 0)
        {
            gVarrok_7F8_703398->sub_59B0D0(field_26_varrok_idx);
            this->field_26_varrok_idx = 0;
        }
    }

    Object_2C::sub_527F10();
    if (field_4)
    {
        gSprite_Pool_703818->remove(field_4);
        this->field_4 = 0;
    }
    this->field_14_id = 0;
}

MATCH_FUNC(0x522250)
bool Object_2C::sub_522250(Sprite* pSprite)
{
    const u32 phi_type = this->field_8->field_34;
    if (phi_type != 6 && phi_type != 7 && phi_type != 8 && phi_type != 9 && phi_type != 10 && phi_type != 1 && phi_type != 12)
    {
        u8 varrok_idx = this->field_26_varrok_idx;
        if (varrok_idx > 0)
        {
            if (pSprite)
            {
                Char_B4* cB4 = pSprite->AsCharB4_40FEA0();
                if (cB4)
                {
                    if (cB4->field_7C_pPed->field_267_varrok_idx == varrok_idx)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5222b0)
s32 Object_2C::sub_5222B0()
{
    return this->field_8->field_68 != 0 ? 2048 : 1024;
}

MATCH_FUNC(0x5222d0)
void Object_2C::sub_5222D0()
{
    if (field_10_obj_3c->field_2A == 1)
    {
        field_10_obj_3c->field_1C += dword_6F8DA8;
        field_10_obj_3c->field_10 += field_10_obj_3c->field_1C;
    }
    else
    {
        field_10_obj_3c->field_C += field_10_obj_3c->field_18;

        if (field_10_obj_3c->field_C < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_C = kFpZero_6F8E10;
            field_10_obj_3c->field_18 = kFpZero_6F8E10;
        }

        if (field_10_obj_3c->field_10 < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_10 = kFpZero_6F8E10;
        }
    }
}

MATCH_FUNC(0x522340)
void Object_2C::PoolGive_522340()
{
    if (field_20 == 2)
    {
        // TODO: Missing inline here
        TObject_2C_Pool* pRoot = gObject_2C_Pool_6F8F80;
        mpNext = pRoot->field_4_pPrev;
        pRoot->field_4_pPrev = this;
        field_20 = 1;
    }
}

MATCH_FUNC(0x522360)
void Object_2C::PoolTake_522360()
{
    Object_2C* pLast;
    Object_2C* pIter;

    if (field_20 == 1)
    {
        pLast = 0;
        for (pIter = gObject_2C_Pool_6F8F80->field_4_pPrev; pIter; pIter = pIter->mpNext)
        {
            if (pIter == this)
            {
                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                }
                else
                {
                    gObject_2C_Pool_6F8F80->field_4_pPrev = pIter->mpNext;
                }
                pIter->mpNext = 0;
                break;
            }
            else
            {
                pLast = pIter;
            }
        }
        field_20 = 2;
    }
}

MATCH_FUNC(0x5223c0)
char Object_2C::ShouldCollideWith_5223C0(Sprite* pSprite)
{
    s32 sprite_type;

    if (!pSprite)
    {
        return 0;
    }
    switch (field_8->field_54)
    {
        case 0:
            // Always
            return true;
        case 1:
            // Only cars
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car)
            {
                return false;
            }
            break;
        case 2:
            // Only peds
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::ped)
            {
                return false;
            }
            break;
        case 3:
            // Only objects?
            sprite_type = pSprite->field_30_sprite_type_enum;
            if (sprite_type != sprite_types_enum::code_obj1 && sprite_type != sprite_types_enum::map_obj &&
                sprite_type != sprite_types_enum::unknown_1)
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case 4:
            // Never
            return 0;
            break;
        default:
            return 1;
            break;
    }
    return true;
}

MATCH_FUNC(0x522430)
bool Object_2C::sub_522430(Sprite* pSprite)
{
    return (pSprite && ShouldCollideWith_5223C0(pSprite) && !sub_522250(pSprite)) ? true : false;
}

MATCH_FUNC(0x522460)
char_type Object_2C::sub_522460(Sprite* a2)
{
    byte_6F8F94 = 0;

    Sprite* pSprite = a2->sub_59E7D0(2);
    if (pSprite && dword_6F8F8C && pSprite->field_30_sprite_type_enum == 2 // IsCar
        || !sub_522430(pSprite) || pSprite == dword_6F8F8C)
    {
        return 0;
    }

    if (pSprite->field_8_object_2C_ptr->field_18_model == 166 || pSprite->field_8_object_2C_ptr->field_18_model == 169)
    {
        byte_6F8F94 = 1;
    }
    gRozza_679188.field_20_pSprite = pSprite;
    gRozza_679188.field_0_type = 3;
    return 1;
}

STUB_FUNC(0x5224e0)
s16* Object_2C::sub_5224E0(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x522640)
void Object_2C::sub_522640(Fix16_Point* a2)
{
    sub_5224E0(a2);

    if (field_8->field_4C == 3 && field_10_obj_3c->field_34 == 2)
    {
        s16 maxVal = 9;
        const s16 rng = stru_6F6784.get_int_4F7AE0(&maxVal);
        if (rng < 6)
        {
            if (rng < 3)
            {
                field_10_obj_3c->field_34 = 1;
            }
            else
            {
                field_10_obj_3c->field_34 = 0;
            }
        }
    }
}

WIP_FUNC(0x5226a0)
void Object_2C::sub_5226A0(char_type a2)
{
    // TODO: Missing SEH
    WIP_IMPLEMENTED;

    if (field_10_obj_3c)
    {
        field_10_obj_3c->field_38 = a2;
    }
    else
    {
        sub_528130(&stru_6F8EF0);
        field_10_obj_3c->field_38 = a2;
    }
}

STUB_FUNC(0x522710)
void Object_2C::sub_522710(Object_2C* a2, u32* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5229b0)
void Object_2C::sub_5229B0(s32 a2, u32* a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

// TODO: Probably move
STUB_FUNC(0x55F3B0)
EXPORT s32* __stdcall ComputeLineLineIntersection_55F3B0(s32* a1,
                                                         s32 a2,
                                                         s32 a3,
                                                         s32* a4,
                                                         s32 a5,
                                                         s32 a6,
                                                         Fix16_Point* a7,
                                                         Fix16_Point* a8,
                                                         s32 a9,
                                                         s32 a10,
                                                         s32 a11)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x522b20)
void Object_2C::sub_522B20(s32* f18, s32* a3, s32* a4)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x522be0)
void Object_2C::sub_522BE0(u32* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x522d00)
void Object_2C::sub_522D00(u32* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x522e10)
void Object_2C::sub_522E10(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5233a0)
char_type Object_2C::sub_5233A0(Fix16 a2)
{
    dword_6F8F8C = 0;

    if (sub_522460(this->field_4))
    {

        byte_623EC4 = 0;
        if (byte_6F8F94)
        {
            return 0;
        }

        dword_6F8F8C = gRozza_679188.field_20_pSprite;
        if (a2 > k_dword_6F8C58)
        {
            return 0;
        }

        if (this->field_10_obj_3c->field_34 == 1)
        {
            this->field_4->field_28_num = 29;
        }
        else
        {
            this->field_4->field_28_num = 6;
        }
        this->field_10_obj_3c->field_C = k_dword_6F8C58;
        this->field_10_obj_3c->field_18 = this->field_8->field_14_friction;
        return 1;
    }
    this->field_10_obj_3c->field_34 = 2;
    return 0;
}

MATCH_FUNC(0x523440)
void Object_2C::sub_523440(Fix16_Point point, char_type bUnknown1, char_type bUnknown2)
{
    if ((u8)field_4->IsOnWater_59E1D0())
    {
        this->field_10_obj_3c->field_C = kFpZero_6F8E10;
        this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        return;
    }

    switch (this->field_8->field_4C)
    {
        case 4:
            if (gRozza_679188.field_20_pSprite)
            {
                field_4->set_z_lazy(gRozza_679188.field_20_pSprite->field_1C_zpos);
            }

            if (!bUnknown1 || gRozza_679188.field_0_type == 3)
            {
                if (bUnknown2 && gRozza_679188.field_0_type != 3)
                {
                    gRozza_679188.field_0_type = 5;
                    gRozza_679188.field_20_pSprite = 0;
                }
            }
            else
            {
                gRozza_679188.field_0_type = 4;
                gRozza_679188.field_20_pSprite = 0;
            }

            HandleImpact_528E50(gRozza_679188.field_20_pSprite);
            return;

        case 0:
        case 1:
            if (bUnknown1)
            {
                gRozza_679188.field_0_type = 4;
                gRozza_679188.field_20_pSprite = 0;
            }
            else if (bUnknown2)
            {
                gRozza_679188.field_0_type = 5;
                gRozza_679188.field_20_pSprite = 0;
            }
            HandleImpact_528E50(gRozza_679188.field_20_pSprite);
            return;

        case 2:
        case 3:
            sub_522E10(&point);
            return;
        default:
            return;
    }
}

STUB_FUNC(0x5235b0)
char_type Object_2C::sub_5235B0(Sprite* a2, u32* a3, u8* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x524630)
void Object_2C::sub_524630(s32 a2, s16 a3)
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/jLuSq
WIP_FUNC(0x525190)
void Object_2C::sub_525190(u8 varrok_idx)
{
    WIP_IMPLEMENTED;

    if (field_8->field_3C < 39 || field_8->field_3C > 42)
    {
        if (field_8->field_48 == 13)
        {
            sub_5291D0();
            Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(this->field_4->field_14_xpos.x,
                                                                              this->field_4->field_14_xpos.y,
                                                                              this->field_4->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              19,
                                                                              gVarrok_7F8_703398->field_0[varrok_idx].field_0_ped_id);
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(varrok_idx);
            }
        }
    }
    else
    {
        sub_5291E0(field_8->field_3C);
    }
}

MATCH_FUNC(0x5257d0)
void Object_2C::UpdateAninmation_5257D0()
{
    ++this->field_C_pAny.o8->field_6_frame_counter;
    if (field_C_pAny.o8->field_6_frame_counter >= this->field_8->field_64_next_frame_max)
    {
        field_C_pAny.o8->field_6_frame_counter = 0;
        ++this->field_C_pAny.o8->field_7_anim_speed_counter;
        if (field_C_pAny.o8->field_7_anim_speed_counter >= this->field_8->field_6C_sprite_anim_speed)
        {
            field_C_pAny.o8->field_7_anim_speed_counter = 0;
            if (field_C_pAny.o8->field_4_timer > 0)
            {
                field_C_pAny.o8->field_4_timer--;
            }
        }
        const s16 target_id = this->field_8->field_1E + this->field_C_pAny.o8->field_7_anim_speed_counter;

        // TODO: Inline?
        Sprite* pSprite = this->field_4;
        if (pSprite->field_22_sprite_id != target_id)
        {
            pSprite->field_22_sprite_id = target_id;
            pSprite->sub_59FA40();
        }

        if (!field_C_pAny.o8->field_4_timer && !field_C_pAny.o8->field_7_anim_speed_counter)
        {
            Object_2C::sub_5283C0(this->field_8->field_3C);
        }
    }
}

MATCH_FUNC(0x525910)
bool Object_2C::sub_525910()
{
    if (field_24_bDoneThisFrame)
    {
        switch (field_8->field_44)
        {
            case 1:
            case 2:
            case 5:
            case 6:
            case 8:
            case 11:
                if (field_24_bDoneThisFrame == 1)
                {
                    sub_5283C0(field_8->field_38);
                }
                else
                {
                    sub_5283C0(field_24_bDoneThisFrame);
                }
                field_24_bDoneThisFrame = 0;
                return true;
            case 4:
                if (field_24_bDoneThisFrame != 1)
                {
                    sub_5283C0(field_24_bDoneThisFrame);
                }
                field_24_bDoneThisFrame = 0;
                return true;
            case 7:
            case 10:
                field_24_bDoneThisFrame = 0;
                sub_5290A0();
                return true;
            default:
                field_24_bDoneThisFrame = 0;
                return false;
        }
    }
    else if (field_8->field_44 == 3 || field_8->field_44 == 4)
    {
        sub_5283C0(field_8->field_38);
        return true;
    }

    return false;
}

STUB_FUNC(0x525370)
char Object_2C::sub_525370(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x525AE0)
EXPORT void Object_2C::sub_525AE0()
{
    switch (field_18_model)
    {
        case 139:
        case 141:
            gPurpleDoom_1_679208->CheckAndHandleCollisionInStrips_477BD0(field_4);
            gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_4);
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x525b40)
void Object_2C::sub_525B40()
{
    if (field_18_model == 128)
    {
        gParticle_8_6FD5E8->SpawnParticleSprite_5405D0(field_4);
    }
}

MATCH_FUNC(0x525B60)
char_type Object_2C::sub_525B60()
{
    // TODO: Forced eax -> al
    const u8 isWater = field_4->IsOnWater_59E1D0();
    if (isWater)
    {
        sub_528900();
        return 1;
    }
    return 0;
}

STUB_FUNC(0x525b80)
void Object_2C::sub_525B80()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x525d90)
void Object_2C::sub_525D90()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x525f30)
void Object_2C::Update_525F30()
{
    Wolfy_30* pWolfy;

    dword_6F8F8C = 0;
    while (2)
    {
        switch (this->field_8->field_34)
        {
            case 0:
                if (!sub_525910())
                {
                    sub_525B20();
                    return;
                }
                return;
            case 1:
                if (!sub_525910())
                {
                    sub_525B20();
                    sub_525AE0();
                }
                return;

            case 5:
                pWolfy = this->field_C_pAny.pExplosion;
                if (pWolfy)
                {
                    if (pWolfy->Update_5434A0(kFpZero_6F8E10, kZeroAng_6F8F68))
                    {
                        this->field_25 = 1;
                    }
                }
                return;

            case 3:
            case 7:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                sub_525B80();
                if (!byte_6F8F40)
                {
                    sub_527AE0();
                }
                return;

            case 2:
            case 8:
                if (!sub_525910())
                {
                    sub_525B20();
                    UpdateAninmation_5257D0();
                }
                return;

            case 4:
            case 9:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                sub_525D90();
                if (!byte_6F8F40)
                {
                    sub_527AE0();
                }
                return;

            case 6:
            case 10:
            case 11:
                sub_525910();
                sub_525B20();
                return;

            case 12:
                sub_525B20();
                return;

            default:
                continue;
        }
    }
}

MATCH_FUNC(0x5263d0)
bool Object_2C::PoolUpdate()
{
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    if (field_25 != 1)
    {
        Object_2C::Update_525F30();
    }

    if (field_25 <= 0 || (field_25 == 2 && gGame_0x40_67E008->sub_4B97E0(field_4, kFpZero_6F8E10)))
    {
        return false;
    }
    else
    {
        Object_2C::RemoveFromCollisionBuckets_527D00();
        return true;
    }
}

STUB_FUNC(0x526790)
void Object_2C::sub_526790(s32 a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x526830)
s32 Object_2C::sub_526830(s32 a1)
{
    WIP_IMPLEMENTED;

    int result;
    switch (a1)
    {
        case 39:
            result = 18;
            break;
        case 40:
            result = 33;
            break;
        case 41:
            result = 19;
            break;
        case 42:
            result = 20;
            break;
        default:
            result = a1;
            break;
    }
    return result;
}

STUB_FUNC(0x526b40)
s16 Object_2C::sub_526B40(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x527070)
char_type Object_2C::sub_527070(s16* a2, s32 a3, s16* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x527630)
void Object_2C::sub_527630(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Phi_74* phi74 = gPhi_8CA8_6FCF00->sub_534360(object_type);
    field_8 = phi74;
    field_18_model = object_type;
    field_24_bDoneThisFrame = 0;

    if (field_4)
    {
        phi74->sub_5331A0(field_4);
    }
    else
    {
        field_4 = phi74->sub_533170();
    }

    Sprite* pSprite = field_4;
    if (pSprite->field_14_xpos.x != xpos || pSprite->field_14_xpos.y != ypos || pSprite->field_1C_zpos != zpos)
    {
        pSprite->field_14_xpos.x = xpos;
        pSprite->field_14_xpos.y = ypos;
        pSprite->field_1C_zpos = zpos;
        pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
    }

    Sprite* pSprite_ = field_4;
    if (rotation.rValue != pSprite_->field_0.rValue)
    {
        pSprite_->field_0.rValue = rotation.rValue;
        pSprite_->ResetZCollisionAndDebugBoxes_59E7B0();
    }
    field_4->field_8_object_2C_ptr = this;
}

MATCH_FUNC(0x527990)
void Object_2C::Light_527990()
{
    field_C_pAny.pLight->field_0.flag &= ~0xFF;
}

MATCH_FUNC(0x527ae0)
void Object_2C::sub_527AE0()
{
    switch (field_8->field_40)
    {
        case 0:
        case 1:
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_4);
            return;
        case 3:
            DAT_006f8f88++;
            gPurpleDoom_2_67920C->AddToRegionBuckets_477B20(field_4);
            return;
        case 4:
            gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_4);
            return;
        case 2:
            return;
    }
}

MATCH_FUNC(0x527d00)
void Object_2C::RemoveFromCollisionBuckets_527D00()
{
    switch (field_8->field_40)
    {
        case 0:
        case 1:
            gPurpleDoom_3_679210->Remove_477B00(field_4);
            break;
        case 3:
            --DAT_006f8f88;
            gPurpleDoom_2_67920C->AddToSpriteRectBuckets_477B60(field_4);
            break;
        case 4:
            gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_4);
            break;
        default:
            return;
    }
}

STUB_FUNC(0x527f10)
void Object_2C::sub_527F10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x528130)
Ang16 Object_2C::sub_528130(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x528240)
char_type Object_2C::sub_528240(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5283c0)
void Object_2C::sub_5283C0(s32 a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5288B0)
bool Object_2C::OnObjectTouched_5288B0(Sprite* a2)
{
    if (!a2)
    {
        return false;
    }

    Char_B4* pCharB4 = a2->AsCharB4_40FEA0();
    if (pCharB4)
    {
        return pCharB4->OnObjectTouched_553640(this);
    }

    Car_BC* pCarBc = a2->AsCar_40FEB0();
    if (pCarBc)
    {
        return pCarBc->OnObjectTouched_43EA60(this);
    }

    return false;
}

MATCH_FUNC(0x528900)
void Object_2C::sub_528900()
{
    if (field_10_obj_3c)
    {
        field_10_obj_3c->field_30_bSkipAnim = 1;
    }

    if ((rng_dword_67AB34->field_0_rng & 3) == 0)
    {
        field_4->sub_59E320(1);
        if (field_4->sub_59E390(dword_6F8F5C, dword_6F8F5C, 0))
        {
            // inline - because has to be a local here?
            Sprite* pSprite = this->field_4;
            if (pSprite->field_1C_zpos != kFpZero_6F8E10)
            {
                pSprite->field_1C_zpos = kFpZero_6F8E10;
                pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
            }
            sub_5290A0();
        }
    }
}

MATCH_FUNC(0x528960)
char_type Object_2C::HandleObjectHitIfExplosive_528960(Object_2C* pOther)
{
    // Check is explosive range
    if (field_8->field_48 < 12 || field_8->field_48 > 13)
    {
        return 0;
    }

    pOther->ProcessObjectExplosionImpact_528A20(this);
    return 1;
}

MATCH_FUNC(0x528990)
char_type Object_2C::HandleObjectHit_528990(Sprite* pSprite)
{
    Char_B4* cB4 = pSprite->AsCharB4_40FEA0();
    if (cB4)
    {
        return cB4->HandlePedObjectHit_5537F0(this);
    }

    Car_BC* cBC = pSprite->AsCar_40FEB0();
    if (cBC)
    {
        return cBC->HandleCarHitByObject_43F130(this);
    }

    Object_2C* o2c = pSprite->As2C_40FEC0();

    if (gVarrok_7F8_703398->field_0[field_26_varrok_idx].field_0_ped_id)
    {
        Ped* pPed = gPedManager_6787BC->PedById(gVarrok_7F8_703398->field_0[field_26_varrok_idx].field_0_ped_id);
        if (pPed)
        {
            pPed->ProcessWeaponHitResponse_46FE20(o2c);
        }
    }
    return o2c->HandleObjectHitIfExplosive_528960(this);
}

STUB_FUNC(0x528A20)
void Object_2C::ProcessObjectExplosionImpact_528A20(Object_2C* pObj)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x528ba0)
void Object_2C::HandleImpactNoSprite_528BA0()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x528e50)
void Object_2C::HandleImpact_528E50(Sprite* pSprite)
{
    WIP_IMPLEMENTED;

    // Mostly bad switch ordering I think

    if (!this->field_24_bDoneThisFrame && (!pSprite || ShouldCollideWith_5223C0(pSprite)))
    {
        this->field_24_bDoneThisFrame = 1;
        switch (field_8->field_44)
        {
            case 1:
            case 2:
                PoolGive_522340(); // destroy
                break;

            case 3:
                if (pSprite->AsCar_40FEB0())
                {
                    this->field_24_bDoneThisFrame = 1;
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 4:
            case 5:
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_bDoneThisFrame = 1; // is_charb4
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 6:
                this->field_24_bDoneThisFrame = OnObjectTouched_5288B0(pSprite);
                if (field_24_bDoneThisFrame)
                {
                    PoolGive_522340(); // destroy
                }
                break;

            case 7:
            case 8:
                if (pSprite)
                {
                    this->field_24_bDoneThisFrame = HandleObjectHit_528990(pSprite);
                }
                else
                {
                    HandleImpactNoSprite_528BA0();
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 9:
                this->field_24_bDoneThisFrame = OnObjectTouched_5288B0(pSprite);
                break;

            case 10:
                if (!pSprite)
                {
                    break;
                }
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_bDoneThisFrame = 1;
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }
                break;

            case 11:
                this->field_24_bDoneThisFrame = 0;
                break;

            default:
                break;
        }

        if (this->field_24_bDoneThisFrame)
        {
            if (pSprite)
            {
                if (!check_is_shop_421060() || !pSprite->IsControlledByActivePlayer_59E170())
                {
                    gRozza_C88_66AFE0->Type3_40BDD0(field_4, pSprite);
                }
            }
            else if (gRozza_679188.field_0_type == 4)
            {
                gRozza_C88_66AFE0->Type4_40BC40(field_4);
            }
            else if (gRozza_679188.field_0_type == 5)
            {
                gRozza_C88_66AFE0->Type5_40BD10(field_4);
            }
            else
            {
                gRozza_C88_66AFE0->OtherType_40BBA0(field_4, kFpZero_6F8E10);
            }
        }
    }
}

MATCH_FUNC(0x529000)
void Object_2C::HandleCollisionWithObject_529000(Object_2C* pObj)
{
    switch (pObj->field_18_model)
    {
        case 139:
            sub_529070(pObj);
            break;

        case 141:
            if (field_8->field_60)
            {
                Dealloc_5291B0();
            }
            break;
    }
}

MATCH_FUNC(0x529030)
void Object_2C::sub_529030(s8 speed_x, s8 speed_y)
{
    field_26_varrok_idx = (speed_y + 7) | (16 * (speed_x + 7)); //  two variables in the same byte?
}

MATCH_FUNC(0x529070)
void Object_2C::sub_529070(Object_2C* pObj)
{
    sub_5226A0(pObj->field_26_varrok_idx);
}

// https://decomp.me/scratch/lXvKN
MATCH_FUNC(0x529210)
s32 Object_2C::sub_529210()
{
    switch (field_26_varrok_idx)
    {
        case 46:
        case 48:
            return 22;
        case 45:
        case 47:
            return 23;
    }
    return 23;
}

MATCH_FUNC(0x529240)
s32 Object_2C::sub_529240()
{
    s32 result;
    gmp_block_info* pBlockInfo = gMap_0x370_6F6268->get_block_4DFE10(field_4->field_14_xpos.x.ToInt(),
                                                                     field_4->field_14_xpos.y.ToInt(),
                                                                     field_4->field_1C_zpos.ToInt());
    switch (field_26_varrok_idx)
    {
        case 45u:
        case 47u:
            result = gGtx_0x106C_703DD4->field_6C_spec[pBlockInfo->field_0_left & 0x3FF];
            if (result == 3)
            {
                return 1;
            }
            break;

        case 46u:
        case 48u:
            result = gGtx_0x106C_703DD4->field_6C_spec[pBlockInfo->field_2_right & 0x3FF];
            if (result == 3)
            {
                result = 1;
            }
            break;

        default:
            result = 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5292D0)
void Object_2C::sub_5292D0()
{
    s32 wepon_kind;
    if (field_18_model >= 200)
    {
        wepon_kind = field_18_model - 200;
    }
    else
    {
        wepon_kind = field_18_model - 64;
    }

    if (wepon_kind <= 27)
    {
        field_26_varrok_idx = gWeapon_8_707018->get_ammo_5E3E80(wepon_kind);
    }
}

MATCH_FUNC(0x529080)
void Object_2C::SetDamageOwner_529080(u8 idx)
{
    field_26_varrok_idx = idx;
    gVarrok_7F8_703398->IncrementRefCount_59B0B0(idx);
}

MATCH_FUNC(0x5290a0)
void Object_2C::sub_5290A0()
{
    field_25 = 1;
}

MATCH_FUNC(0x5290B0)
void Object_2C::sub_5290B0()
{
    field_25 = 2;
}

MATCH_FUNC(0x5291b0)
void Object_2C::Dealloc_5291B0()
{
    PoolGive_522340();
    sub_5290A0();
}

MATCH_FUNC(0x5291d0)
void Object_2C::sub_5291D0()
{
    PoolGive_522340();
    field_24_bDoneThisFrame = 1;
}

MATCH_FUNC(0x5291E0)
void Object_2C::sub_5291E0(u8 a2)
{
    PoolGive_522340();
    field_24_bDoneThisFrame = a2;
}

MATCH_FUNC(0x529200)
bool Object_2C::sub_529200()
{
    return (field_18_model != 174) ? true : false;
}

MATCH_FUNC(0x52ae60)
Object_2C::~Object_2C()
{
    mpNext = 0;
    field_4 = 0;
    field_8 = 0;
    field_10_obj_3c = 0;
}

MATCH_FUNC(0x52ae70)
Fix16_Point Object_2C::GetXY_52AE70()
{
    return field_4->get_x_y_443580();
}

MATCH_FUNC(0x52ae90)
Fix16_Point Object_2C::GetRot_52AE90()
{
    if (field_10_obj_3c)
    {
        return field_10_obj_3c->GetRot_52ADF0();
    }
    return stru_6F8EF0;
}

MATCH_FUNC(0x5290C0)
void Object_2C::sub_5290C0(u8 id_base)
{
    s16 new_id = id_base + field_8->field_1E;
    Sprite* pSprite = field_4;
    if (pSprite->field_22_sprite_id != new_id)
    {
        pSprite->field_22_sprite_id = new_id;
        pSprite->sub_59FA40();
    }
}

MATCH_FUNC(0x5290F0)
Fix16 Object_2C::sub_5290F0()
{
    if (field_10_obj_3c)
    {
        if (!field_10_obj_3c->field_2A)
        {
            if (field_10_obj_3c->field_C == kFpZero_6F8E10)
            {
                return field_10_obj_3c->field_10;
            }
            return field_10_obj_3c->field_C;
        }
        else
        {
            if (field_10_obj_3c->field_C == kFpZero_6F8E10)
            {
                if (field_10_obj_3c->field_10 == kFpZero_6F8E10)
                {
                    if (this->field_18_model == 113)
                    {
                        return kFpZero_6F8E10;
                    }
                    return dword_6F8E14;
                }
                return field_10_obj_3c->field_10;
            }
            return field_10_obj_3c->field_C;
        }
    }
    return kFpZero_6F8E10;
}

MATCH_FUNC(0x525AC0)
char Object_2C::sub_525AC0()
{
    if (field_18_model == 113)
    {
        return field_C_pAny.pExplosion->sub_5435D0();
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x525B20)
void Object_2C::sub_525B20()
{
    if (field_10_obj_3c)
    {
        if (field_10_obj_3c->field_0.field_0_p18)
        {
            field_10_obj_3c->field_0.PoolUpdate_5A6F70(field_4);
        }
    }
}

STUB_FUNC(0x527A30)
void Object_2C::UpdateLight_527A30()
{
    NOT_IMPLEMENTED;

    // TODO: Clears light radius? also probably an inline of nostalgic_ellis_0x28
    /*
 nostalgic_ellis_0x28 *pLight;
  int v2;

  pLight = this->field_C_pAny.pLight;
  v2 = pLight->field_0;
  LOBYTE(v2) = 0;
  pLight->field_0 = v2 | pLight->field_18_intensity;
*/
}

STUB_FUNC(0x525100)
void Object_2C::sub_525100()
{
    NOT_IMPLEMENTED;
    if (field_8->field_34 <= 1u)
    {

        if (get_model_40FEF0() == 148)
        {
            sub_5290C0(1u);
        }
        else
        {
            sub_5290A0();
        }
    }
}

MATCH_FUNC(0x529300)
void Object_5C::sub_529300()
{
    for (s32 i = field_14 - 88; i >= 0; i--)
    {
        Sprite* pSprite = field_1C.sub_5A6DC0();
        Object_2C* o2c = pSprite->get_o2c_or_null_40FEC0();
        if (o2c->field_18_model == 10)
        {
            if (gGame_0x40_67E008->sub_4B97E0(pSprite, kFpZero_6F8E10))
            {
                Object_5C::CreateExplosion_52A3D0(pSprite->field_14_xpos.x,
                                                  pSprite->field_14_xpos.y,
                                                  pSprite->field_1C_zpos,
                                                  kZeroAng_6F8F68,
                                                  18,
                                                  gVarrok_7F8_703398->field_0[o2c->field_26_varrok_idx].field_0_ped_id);
            }
        }
        o2c->Dealloc_5291B0();
    }
}

MATCH_FUNC(0x5293a0)
void Object_5C::ObjectsService_5293A0()
{
    sub_529300();

    gObject_2C_Pool_6F8F80->UpdatePool();
}

MATCH_FUNC(0x529430)
Object_5C::Object_5C()
{
    for (u8 i = 0; i < 50; i++)
    {
        field_20[i] = 1;
    }

    field_54_f20_idx = 0;
    byte_6F8C68 = 0;
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    field_10 = 0;
    field_14 = 0;
    dword_6F8E54 = 0;
    dword_6F8F18 = 0;
    dword_6F8DC0 = 0;
    dword_6F8F0C = 0;
    field_58 = 0;
    field_18 = 0;
    byte_6F8EDC = 0;

    if (!gObject_2C_Pool_6F8F80)
    {
        gObject_2C_Pool_6F8F80 = new TObject_2C_Pool();
        if (!gObject_2C_Pool_6F8F80)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4239);
        }
    }

    gObject_8_Pool_6F8F78 = new TObject_8_Pool();
    if (!gObject_8_Pool_6F8F78)
    {
        FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4243);
    }

    gObject_3C_Pool_6F8F7C = new TObject_3C_Pool();
    if (!gObject_3C_Pool_6F8F7C)
    {
        FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4245);
    }

    field_58 = gSprite_Pool_703818->get_new_sprite();
    field_58->sub_451950(0, 0, 0);

    field_58->sub_420690(kZeroAng_6F8F68);
    field_58->AllocInternal_59F950(0, 0, 0);

    field_0 = 0;
    field_C = 0;
    field_8 = 0;
    field_4 = 0;
}

MATCH_FUNC(0x529750)
Object_5C::~Object_5C()
{
    if (field_58)
    {
        gSprite_Pool_703818->remove(field_58);
        field_58 = 0;
    }

    if (gObject_2C_Pool_6F8F80)
    {
        GTA2_DELETE_AND_NULL(gObject_2C_Pool_6F8F80);
    }

    if (gObject_8_Pool_6F8F78)
    {
        GTA2_DELETE_AND_NULL(gObject_8_Pool_6F8F78);
    }

    if (gObject_3C_Pool_6F8F7C)
    {
        GTA2_DELETE_AND_NULL(gObject_3C_Pool_6F8F7C);
    }

    field_0 = 0;
    field_C = 0;
    field_8 = 0;
    field_4 = 0;
}

MATCH_FUNC(0x5297f0)
void Object_5C::sub_5297F0()
{
    field_0 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, word_6F8D8C);
    field_0->field_26_varrok_idx = 45;

    field_4 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, dword_6F8D80);
    field_4->field_26_varrok_idx = 48;

    field_8 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, word_6F8D54);
    field_8->field_26_varrok_idx = 46;

    field_C = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, dword_6F8CD0);
    field_C->field_26_varrok_idx = 47;
}

STUB_FUNC(0x5298e0)
s32 Object_5C::sub_5298E0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x529950)
Object_2C* Object_5C::NewTouchPoint_529950(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 a9)
{
    Object_2C* pNewObj = sub_529C00(object_type, x, y, z, rot, 0);
    if (pNewObj)
    {
        Sprite* pSprite = pNewObj->field_4;
        Sprite_4C* sprite_4c_ptr = pSprite->field_C_sprite_4c_ptr;
        sprite_4c_ptr->field_0_width = w;
        sprite_4c_ptr->field_4_height = h;
        sprite_4c_ptr->field_8 = a9;
        pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
        pNewObj->sub_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299b0)
Object_2C* Object_5C::NewPhysicsObj_5299B0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 maybe_rotation)
{
    Object_2C* pNewObj = sub_529C00(object_type, xpos, ypos, zpos, maybe_rotation, 0);
    if (pNewObj)
    {
        pNewObj->sub_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299f0)
Object_2C* Object_5C::sub_5299F0(s32 object_type, u32 varrok_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Object_2C* pNewObj = sub_529C00(object_type, xpos, ypos, zpos, kZeroAng_6F8F68, 0);
    if (pNewObj)
    {
        pNewObj->field_26_varrok_idx = varrok_idx;
        if (object_type == 279)
        {
            pNewObj->sub_5290A0();
        }
    }
    return pNewObj;
}

STUB_FUNC(0x529a40)
Object_2C* Object_5C::sub_529A40(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u8 a7)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/vf1YG need to inline sub_482D60
STUB_FUNC(0x529ab0)
Object_2C* Object_5C::sub_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, s32 radius_flags, u8 intensity)
{
    Object_2C* pNewObj = Object_5C::sub_529C00(light_type, xpos, ypos, zpos, kZeroAng_6F8F68, 0);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->sub_482D60(argb, radius_flags, intensity);
    }
    return pNewObj;
}

STUB_FUNC(0x529b20)
Object_2C* Object_5C::sub_529B20(s32 obj_type,
                                 s32 a3,
                                 s32 a4,
                                 s32 argb,
                                 s32 a6,
                                 u8 intensity,
                                 char_type on_time,
                                 char_type off_time,
                                 u8 shape)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x529bc0)
Object_2C* Object_5C::sub_529BC0(s32 a2, Fix16 a3, Fix16 a4, Fix16 a5, Ang16 a6)
{
    Object_2C* tmp = sub_529C00(a2, a3, a4, a5, a6, 1);
    if (tmp)
    {
        tmp->sub_527AE0();
    }
    return tmp;
}

// https://decomp.me/scratch/dZQWS
STUB_FUNC(0x529c00)
Object_2C* Object_5C::sub_529C00(int object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char bUnknown)
{
    Phi_74* pPhi; // edi
    Object_2C* pNew2C; // esi
    Wolfy_30* pNew30; // eax
    Object_3C* pNew3C; // eax
    Object_8* pNew8; // eax

    if (object_type == 266)
    {
        if (!field_20[field_54_f20_idx]) // 20
        {
            field_54_f20_idx++;
            return 0;
        }
    }

    pPhi = gPhi_8CA8_6FCF00->sub_534360(object_type);
    if (pPhi->field_5C == 2)
    {
        if (field_10 == 360)
        {
            return 0;
        }
        field_10++;
    }

    if (pPhi->field_61) // 6c
    {
        pNew2C = gObject_2C_Pool_6F8F80->Allocate();
        pNew2C->field_20 = 1;
    }
    else
    {
        pNew2C = gObject_2C_Pool_6F8F80->AllocateUntracked();
        pNew2C->field_20 = 2;
    }

    pNew2C->sub_527630(object_type, xpos, ypos, zpos, rotation);
    if (field_18)
    {
        pNew2C->SetDamageOwner_529080(field_18);
        field_18 = 0;
    }

    if (bUnknown &&
        (pNew2C->field_4->sub_59E7D0(0) ||
         (pPhi->field_40 == 3 && gPurpleDoom_2_67920C->FindNearestSpriteOfType_477E60(pNew2C->field_4, 0))))
    {
        if (pNew2C->field_20 == 1) // 154: ~> cmpl    $0x1,0x0(%ebp)
        {
            gObject_2C_Pool_6F8F80->unknown_func(pNew2C);
            return 0;
        }
        else
        {
            gObject_2C_Pool_6F8F80->DeAllocate(pNew2C);
            return 0;
        }
    }

    if (pPhi->field_5C == 3) // 1e0
    {
        ++field_14;
        field_1C.sub_5A6CD0(pNew2C->field_4);
    }

    switch (pPhi->field_34)
    {

        case 0:
        case 1:
        case 6:
        case 10:
        case 12:
            pNew2C->field_C_pAny.o8 = 0;
            pNew2C->field_10_obj_3c = 0;
            break;

        case 5:
            pNew30 = gWolfy_7A8_6FD5F0->sub_543800();
            pNew2C->field_C_pAny.pExplosion = pNew30;
            if (pNew30) // 225
            {
                pNew2C->field_1C = 1;
            }
            else
            {
                return 0;
            }

            break;

        case 2:
        case 8:
            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNew2C->field_C_pAny.o8 = pNew8;
            pNew8->field_7_anim_speed_counter = 0;
            pNew2C->field_C_pAny.o8->field_4_timer = pPhi->field_65;
            pNew2C->field_C_pAny.o8->field_6_frame_counter = 0;
            break;

        case 3:
        case 7:
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNew2C->field_10_obj_3c = pNew3C;
            pNew3C->field_20 = pNew2C->field_14_id;
            pNew2C->field_10_obj_3c->field_C = pNew2C->field_8->field_10;
            pNew2C->field_10_obj_3c->field_18 = pPhi->field_14_friction;
            pNew2C->field_10_obj_3c->field_4 = rotation;
            pNew2C->field_10_obj_3c->field_28 = pNew2C->field_8->field_65;
            pNew2C->field_10_obj_3c->field_10 = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_1C = kFpZero_6F8E10;
            break;

        case 4:
        case 9:
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNew2C->field_10_obj_3c = pNew3C;
            pNew3C->field_20 = pNew2C->field_14_id;
            pNew2C->field_10_obj_3c->field_C = pNew2C->field_8->field_10;
            pNew2C->field_10_obj_3c->field_18 = pPhi->field_14_friction;
            pNew2C->field_10_obj_3c->field_10 = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_1C = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_4 = rotation;
            pNew2C->field_10_obj_3c->field_28 = pNew2C->field_8->field_65;

            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNew2C->field_C_pAny.o8 = pNew8;
            pNew8->field_7_anim_speed_counter = 0; // ??
            pNew2C->field_C_pAny.o8->field_4_timer = pPhi->field_65;
            pNew2C->field_C_pAny.o8->field_6_frame_counter = 0;
            break;

        case 11:
            pNew2C->field_C_pAny.o8 = (Object_8*)gLight_1D4CC_6F5520->sub_52B2A0(xpos, ypos, zpos, 0, 0, 0);
            break;

        default:
            break;
    }
    ++dword_6F8F88;

    pNew2C->field_18_model = object_type;
    if (pNew2C->check_is_shop_421060())
    {
        pNew2C->sub_5292D0();
    }

    if (pNew2C->field_18_model == 281)
    {
        Object_2C* v34 = NewPhysicsObj_5299B0(284, kFpZero_6F8E10, kFpZero_6F8E10, kFpZero_6F8E10, kZeroAng_6F8F68);
        pNew2C->field_4->DispatchCollisionEvent_5A3100(v34->field_4,
                                                       (dword_6F8CE8 * dword_6F8ECC), // x?
                                                       (dword_6F8CEC * dword_6F8ECC), // y?
                                                       kZeroAng_6F8F68); // ang?
        return pNew2C;
    }
    else
    {
        if (pNew2C->field_18_model == 266)
        {
            pNew2C->set_field_26(field_54_f20_idx);
            field_54_f20_idx++;
        }
        return pNew2C;
    }
    // return pNew2C;
}

MATCH_FUNC(0x52a210)
char_type Object_5C::sub_52A210(char_type a2)
{
    // TODO: Why is it a byte here? enum ??
    *(u8*)&field_18 = a2;
    return a2;
}

MATCH_FUNC(0x52a240)
s32* Object_5C::sub_52A240(s32 a2, s32 maybe_x, s32 maybe_y, s32 maybe_z, s16 pCarBC, s16 maybe_ang, s32 a8, s32 a9, s32 a10)
{
    return Object_5C::sub_52A2C0(a2, maybe_x, maybe_y, maybe_z, pCarBC, maybe_ang, a8, a9, a10, 0);
}

MATCH_FUNC(0x52a280)
s32* Object_5C::sub_52A280(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10)
{
    return Object_5C::sub_52A2C0(a2, a3, a4, a5, a6, a7, a8, a9, a10, 1);
}

STUB_FUNC(0x52a2c0)
s32* Object_5C::sub_52A2C0(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10, char_type a11)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x52a3d0)
Object_2C* Object_5C::CreateExplosion_52A3D0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 a6, s32 pedId)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x52A500)
void Object_5C::SaveObjects_52A500(TurkishDelight_164* pUnknownObj)
{
    NOT_IMPLEMENTED;
}

// TODO: This is a mess, probably a fake match
// https://decomp.me/scratch/juEUq
MATCH_FUNC(0x52A590)
void Object_5C::RestoreObjects_52A590(TurkishDelight_164* pData)
{
    u8* pVarrok = &pData->field_118_varrok_idx[0];
    // TurkishDelight_4* pX = &pData->field_0_obj_x[0][0];
    u16* pModel = &pData->field_F0_model[0];

    s32 i = 19;
    while (i >= 0)
    {
        if (pData->field_0_obj_x[0][19 - i].field_0 != kFpZero_6F8E10 &&
            pData->field_0_obj_x[1][19 - i].field_0 != kFpZero_6F8E10) // x and y not zero?
        {
            Object_2C* pNewObj = NewPhysicsObj_5299B0(*pModel,
                                                      pData->field_0_obj_x[0][19 - i].field_0, // x?
                                                      pData->field_0_obj_x[1][19 - i].field_0,
                                                      pData->field_0_obj_x[2][19 - i].field_0, // z?
                                                      kZeroAng_6F8F68);
            pNewObj->set_field_26(*pVarrok);
        }
        // pX++;
        pModel++; // add $0x2,$esi
        i--;
        pVarrok++;
    }

    memset(pData, 0, 0x12Cu); // everything before field_12C_obj_5C_buffer
}

MATCH_FUNC(0x52A610)
void Object_5C::sub_52A610(Object_2C* p2C)
{
    if (p2C->field_8->field_34 != 11)
    {
        gPurpleDoom_3_679210->Remove_477B00(p2C->field_4);
    }
    gObject_2C_Pool_6F8F80->DeAllocate(p2C);
}

MATCH_FUNC(0x52A650)
void Object_2C::EnsureObject3C_52A650()
{
    if (!field_10_obj_3c)
    {
        Object_3C* p3C = gObject_3C_Pool_6F8F7C->Allocate();

        // TODO: some of this is probably part of PoolAllocate for Object_3C

        ++dword_6F8E54;
        p3C->field_C = 0;

        Ang16 v2 = kZeroAng_6F8F68;
        p3C->field_18 = 0;
        p3C->field_4 = v2;

        p3C->field_28 = 0;
        p3C->field_38 = 0;
        p3C->field_34 = 2;
        p3C->field_24 = 0;
        p3C->field_2F = 0;
        p3C->field_30_bSkipAnim = 0;
        field_10_obj_3c = p3C;
        p3C->field_20 = field_14_id;
        field_10_obj_3c->field_C = kFpZero_6F8E10;
        field_10_obj_3c->field_10 = kFpZero_6F8E10;
    }
    Object_2C::PoolGive_522340();
}

MATCH_FUNC(0x52a6d0)
void Object_2C::ReactivateObjectAfterImpact_52A6D0(Sprite* pSprite)
{
    RemoveFromCollisionBuckets_527D00();

    if (field_8->field_34 != 11)
    {
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_4);
    }

    PoolTake_522360();

    if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car)
    {
        Car_BC* pObj = pSprite->field_8_car_bc_ptr;
        if (pObj)
        {
            field_4->field_28_num = pObj->GetCrashSoundCategory_4435B0();
        }
    }
}