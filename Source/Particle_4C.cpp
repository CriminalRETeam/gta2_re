#include "Particle_4C.hpp"
#include "map_0x370.hpp"
#include "Object_5C.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "Wolfy_3D4.hpp"
#include "char.hpp"
#include "Ped.hpp"
#include "Particle_8.hpp"
#include "Car_BC.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "Object_3C.hpp"
#include "CarPhysics_B0.hpp"

EXTERN_GLOBAL(Fix16, dword_6FD49C);
EXTERN_GLOBAL(Fix16, dword_6FD2F0);
EXTERN_GLOBAL(Fix16, dword_6FD448);
EXTERN_GLOBAL(Fix16, dword_6FD4C0);
EXTERN_GLOBAL(Fix16, dword_6FD540);
EXTERN_GLOBAL(Fix16, dword_6FD464);
EXTERN_GLOBAL(Fix16, dword_6FD2F4);
EXTERN_GLOBAL(Ang16, word_6FD5CC);
EXTERN_GLOBAL(Ang16, word_6FD3EE);
EXTERN_GLOBAL(Fix16, dword_6FD2D4);
EXTERN_GLOBAL(Fix16, dword_6FD2D0);
EXTERN_GLOBAL(Fix16, dword_6FD48C);
EXTERN_GLOBAL(Fix16, dword_6FD4CC);
EXTERN_GLOBAL(Fix16, dword_6FD328);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD46C, Fix16(0x333, 0), 0x6FD46C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD554, dword_6FD448, 0x6FD554);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD28C, dword_6FD4C0 - dword_6FD554, 0x6FD28C);
//DEFINE_GLOBAL(Fix16_Point, stru_6FD388, 0x6FD388);

DEFINE_GLOBAL(Fix16, stru_6FD388, 0x6FD388);
DEFINE_GLOBAL(Fix16, stru_6FD38C, 0x6FD38C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD2E8, Fix16(0x666, 0), 0x6FD2E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD4A0, Fix16(1), 0x6FD4A0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD39C, Fix16(0.5f), 0x6FD39C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD4A4, Fix16(2), 0x6FD4A4);
DEFINE_GLOBAL(Fix16, dword_6FD4A8, 0x6FD4A8);
DEFINE_GLOBAL(Fix16, dword_6FD470, 0x6FD470);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD45C, Fix16(0xA3, 0), 0x6FD45C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD564, Fix16(0x51, 0), 0x6FD564);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD538, Fix16(0x31, 0), 0x6FD538);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD55C, Fix16(0x3000, 0), 0x6FD55C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD30C, Fix16(0x3999, 0), 0x6FD30C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD280, Fix16(255), 0x6FD280);
DEFINE_GLOBAL(Fix16, dword_6FD300, 0x6FD300);
DEFINE_GLOBAL(Fix16, dword_6FD304, 0x6FD304);
DEFINE_GLOBAL(Fix16, dword_6FD308, 0x6FD308);
DEFINE_GLOBAL(Fix16, dword_6FD364, 0x6FD364);
DEFINE_GLOBAL(Fix16, dword_6FD3C0, 0x6FD3C0);
DEFINE_GLOBAL(Fix16, dword_6FD5A8, 0x6FD5A8);
DEFINE_GLOBAL(Fix16, dword_6FD2F8, 0x6FD2F8);
DEFINE_GLOBAL(Fix16, dword_6FD4AC, 0x6FD4AC);

// https://decomp.me/scratch/nKSYL
WIP_FUNC(0x538060)
char_type Particle_4C::UpdateFloatingParticle_state_6_15_16_17_538060()
{
    WIP_IMPLEMENTED;
    Fix16 rng_1;
    Fix16 rng_2;
    Fix16_Point vector(Fix16(0), Fix16(0));
    Fix16 new_z = dword_6FD45C + field_30_pNext->field_1C_zpos;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
    if (new_z > dword_6FD28C)
    {
        return true;
    }

    if (new_z.GetFracValue() > dword_6FD55C &&
        gMap_0x370_6F6268->IsBlockNonAirType_48A350(field_30_pNext->field_14_xy.x.ToInt(),
                                                    field_30_pNext->field_14_xy.y.ToInt(),
                                                    new_z.ToInt()))
    {
        rng_1 = Fix16(stru_6F6784.get_int_4F7AE0(61) - 30) / 100;
        rng_2 = Fix16(stru_6F6784.get_int_4F7AE0(10) - 5) / 100;
        ++field_2C_counter;
    }
    else
    {
        rng_1 = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
        rng_2 = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
    }

    if (field_40_pUnknown)
    {
        if (field_2C_counter > 60)
        {
            field_40_pUnknown->field_14;
            if (field_40_pUnknown->field_14->field_4)
            {
                field_20 = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->sub_5290F0();
                field_24_angle = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pUnknown->field_1A == 1)
            {
                field_40_pUnknown = NULL;
            }
        }
    }
    else
    {
        field_20 = dword_6FD49C;
    }

    if (field_20 == dword_6FD49C)
    {
        stru_6FD388 = field_30_pNext->field_14_xy.x + rng_1;
        stru_6FD38C = field_30_pNext->field_14_xy.y + rng_2;
        if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
            stru_6FD38C < dword_6FD280 - dword_6FD4A0)
        {
            field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);
        }
        else
        {
            field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, new_z);
        }
    }
    else
    {
        field_20 = field_20 * dword_6FD30C;
        if (field_20 < dword_6FD49C)
        {
            field_20 = dword_6FD49C;
        }
        vector.x = field_20;
        vector.y = dword_6FD49C;
        vector.RotateByAngle_40F6B0(field_24_angle);

        field_14_additional_speed_x = vector.x;
        field_18_additional_speed_y = vector.y;

        field_8_speed_x = field_14_additional_speed_x + rng_1;
        field_C_speed_y = field_18_additional_speed_y + rng_2;

        stru_6FD388 = field_30_pNext->field_14_xy.x + rng_1;
        stru_6FD38C = field_30_pNext->field_14_xy.y + rng_2;
        if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
            stru_6FD38C < dword_6FD280 - dword_6FD4A0)
        {
            field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);
        }
        else
        {
            field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, new_z);
        }
    }

    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return false;
}

WIP_FUNC(0x5384c0)
char_type Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0()
{
    Fix16 off_x = dword_6FD49C;
    Fix16 off_y = dword_6FD49C;
    char_type bJitter = 1;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }

    if (field_40_pUnknown)
    {
        if (field_40_pUnknown->field_6_id != field_44)
        {
            field_40_pUnknown = NULL;
            field_20 = dword_6FD49C;
        }
        else if (field_40_pUnknown->field_14)
        {
            if (field_40_pUnknown->field_14->field_4)
            {
                field_20 = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->sub_5290F0();
                field_24_angle = field_40_pUnknown->field_14->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pUnknown->field_1A == 1)
            {
                field_40_pUnknown = NULL;
            }
        }
        else
        {
            field_40_pUnknown = NULL;
            field_20 = dword_6FD49C;
        }
    }
    else
    {
        field_20 = dword_6FD49C;
    }

    Fix16 zpos = field_30_pNext->field_1C_zpos + dword_6FD540;
    if (zpos > dword_6FD28C)
    {
        return true;
    }

    Fix16 xpos = field_30_pNext->field_14_xy.x;
    Fix16 ypos = field_30_pNext->field_14_xy.y;

    if (field_20 == dword_6FD49C)
    {
        off_x = 0;
        off_y = -dword_6FD45C;
    }

    switch ((u8)(field_2C_counter >> 2))
    {
        case 2:
            if (stru_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 102);
            dir.x = 0;
            dir.y = field_20 * dword_6FD300;
            bJitter = 0;
            break;
        case 3:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 101);
            dir.x = 0;
            dir.y = field_20 * dword_6FD304;
            break;
        case 4:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 100);
            dir.x = 0;
            dir.y = field_20 * dword_6FD308;
            break;
        case 5:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 99);
            dir.x = 0;
            dir.y = field_20 * dword_6FD308;
            break;
        case 6:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 98);
            dir.x = 0;
            dir.y = field_20 * dword_6FD308;
            break;
        case 7:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 97);
            dir.x = 0;
            dir.y = field_20 * dword_6FD308;
            break;
        default:
            if (stru_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            break;
    }

    Fix16 sin = Ang16::sine_40F500(field_24_angle);
    Fix16 cos = Ang16::cosine_40F520(field_24_angle);
    Fix16 old_x = dir.x;
    dir.x = dir.x * cos + dir.y * sin;
    dir.y = -old_x * sin + dir.y * cos;
    field_14_additional_speed_x = dir.x;
    field_18_additional_speed_y = dir.y;

    Fix16 jitter_x = 0;
    Fix16 jitter_y = 0;
    if (bJitter)
    {
        jitter_x = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
        jitter_y = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
    }

    field_8_speed_x = field_14_additional_speed_x + off_x + jitter_x;
    field_C_speed_y = field_18_additional_speed_y + off_y + jitter_y;
    stru_6FD388 = xpos + field_8_speed_x;
    stru_6FD38C = ypos + field_C_speed_y;

    if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
        stru_6FD38C < dword_6FD280 - dword_6FD4A0)
    {
        field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    }
    else
    {
        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, zpos);
    }

    switch (field_46_sub_state)
    {
        case 0:
        case 1:
            field_30_pNext->field_2C_flags = 0x51;
            break;
        case 2:
            field_30_pNext->field_2C_flags = 0x51;
            break;
        case 3:
            field_30_pNext->field_2C_flags = 0x52;
            break;
        case 4:
            field_30_pNext->field_2C_flags = 0x7A;
            break;
        case 5:
            field_30_pNext->field_2C_flags = 0xA2;
            break;
        case 6:
            field_30_pNext->field_2C_flags = 0xCA;
            break;
        case 7:
            field_30_pNext->field_2C_flags = 0xF2;
            break;
    }

    field_30_pNext->Set_2C_0x4_Flag_4337F0();
    if (field_40_pUnknown)
    {
        field_30_pNext->ResolveZOrder_5A1B30(field_40_pUnknown->field_14->field_4);
    }
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);

    if (++field_46_sub_state > 8)
    {
        field_46_sub_state = 0;
    }
    return 0;
}

MATCH_FUNC(0x538a40)
char_type Particle_4C::UpdateBeamSegment_state_43_538A40()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    this->field_46_sub_state++;
    if (field_46_sub_state > 7u)
    {
        return 1;
    }

    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 103);

    this->field_30_pNext->field_2C_flags = 0xA2;
    this->field_30_pNext->field_2C_flags |= 4u;
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

WIP_FUNC(0x538ac0)
char_type Particle_4C::UpdateObjectBeamLink_state_38_538AC0()
{
    Fix16_Point src;
    Fix16_Point dst;
    Fix16_Point delta;
    Fix16_Point cur;
    Fix16_Point prev;

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state != 6 && field_28_pSprite->field_30_sprite_type_enum == sprite_types_enum::code_obj1_4)
    {
        src.x = field_30_pNext->field_14_xy.x;
        src.y = field_30_pNext->field_14_xy.y;
        dst.x = field_28_pSprite->field_14_xy.x;
        dst.y = field_28_pSprite->field_14_xy.y;
        delta = dst - src;
        Ang16 beam_angle = Fix16::atan2_fixed_405320(delta.y, delta.x);
        Fix16 abs_x = Fix16::Abs_436A50(delta.x);
        Fix16 abs_y = Fix16::Abs_436A50(delta.y);
        Fix16 segments = (abs_x > abs_y ? abs_x : abs_y) / dword_6FD364;

        if (segments != dword_6FD49C)
        {
            delta.x /= segments;
            delta.y /= segments;
            prev = src;
            cur = src;
            for (s32 i = 1; i <= segments.ToInt(); i++)
            {
                cur.x = prev.x + delta.x;
                cur.y = prev.y + delta.y;
                Fix16_Point mid = cur - prev;
                mid.x /= dword_6FD4A4;
                mid.y /= dword_6FD4A4;
                mid.x += prev.x;
                mid.y += prev.y;

                Particle_4C* pSegment = gParticle_8_6FD5E8->New_53E3C0(0, 0, 0, 0, 0, 0);
                if (pSegment)
                {
                    pSegment->field_34 = 0;
                    pSegment->field_38_state = 43;
                    pSegment->field_2C_counter = 50;
                    pSegment->field_46_sub_state = 0;
                    pSegment->field_2E = 50;
                    pSegment->field_30_pNext->SetType_4206F0(8);
                    pSegment->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 104);
                    pSegment->field_30_pNext->set_xyz_lazy_420600(mid.x, mid.y, field_30_pNext->field_1C_zpos);
                    pSegment->field_30_pNext->field_2C_flags = 0xA2;
                    pSegment->field_30_pNext->Set_2C_0x4_Flag_4337F0();
                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pSegment->field_30_pNext);
                }
                prev = cur;
            }
        }

        Fix16 target_x = field_28_pSprite->field_14_xy.x;
        Fix16 target_y = field_28_pSprite->field_14_xy.y;
        Ang16 jitter(&(Fix16(word_6FD5CC.rValue) * Fix16(stru_6F6784.get_int_4F7AE0(16) - 8)), 0);

        switch (field_46_sub_state)
        {
            case 1:
            case 2:
            case 3:
            {
                Ang16 ang(field_28_pSprite->field_0 + word_6FD3EE, 0);
                Fix16 radius = Fix16(field_46_sub_state) * dword_6FD46C;
                src.x = radius * Ang16::sine_40F500(ang);
                src.y = radius * Ang16::cosine_40F520(ang);
                break;
            }
            case 4:
            case 5:
            {
                Ang16 base(field_28_pSprite->field_0 + word_6FD3EE, 0);
                Ang16 ang(base + jitter, 0);
                Fix16 radius = Fix16(field_46_sub_state) * dword_6FD46C + dword_6FD45C;
                src.x = radius * Ang16::sine_40F500(ang);
                src.y = radius * Ang16::cosine_40F520(ang);
                break;
            }
        }

        dst.x = src.x + target_x;
        dst.y = src.y + target_y;

        if (field_28_pSprite->field_30_sprite_type_enum == sprite_types_enum::code_obj1_4)
        {
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 163);
            field_30_pNext->field_2C_flags = 0xA2;
            field_30_pNext->Set_2C_0x4_Flag_4337F0();
            field_30_pNext->set_xyz_lazy_420600(dst.x, dst.y, field_30_pNext->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
            return 0;
        }
    }
    return true;
}

WIP_FUNC(0x539040)
char_type Particle_4C::UpdateDirectedBurstSweep_state_4_539040()
{
    Fix16 off_x = dword_6FD49C;
    Fix16 off_y = dword_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state != 16 && field_2C_counter != 0)
    {
        if (field_40_pUnknown)
        {
            if (field_40_pUnknown->field_6_id != field_44)
            {
                field_40_pUnknown = NULL;
                field_20 = dword_6FD49C;
            }
            else if (field_40_pUnknown->field_14)
            {
                if (field_40_pUnknown->field_14->field_4)
                {
                    field_20 = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->sub_5290F0();
                    field_24_angle = field_40_pUnknown->field_14->field_10_obj_3c->field_4_angle;
                }
                if (field_40_pUnknown->field_1A == 1)
                {
                    field_40_pUnknown = NULL;
                }
            }
            else
            {
                field_40_pUnknown = NULL;
                field_20 = dword_6FD49C;
            }
        }
        else
        {
            field_20 = dword_6FD49C;
        }

        Fix16 zpos = field_30_pNext->field_1C_zpos + dword_6FD45C;
        if (zpos < dword_6FD28C)
        {

            Fix16 xpos = field_30_pNext->field_14_xy.x;
            Fix16 ypos = field_30_pNext->field_14_xy.y;

            if (field_20 < dword_6FD464)
            {
                off_x = 0;
                off_y = -dword_6FD46C;
            }

            if (field_46_sub_state > 0 && field_46_sub_state <= 4)
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (dword_6FD4A0 - Fix16(field_46_sub_state) * dword_6FD2E8) * field_20;
            }
            else
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (field_20 * dword_6FD2F4) * Fix16(field_46_sub_state / 10);
            }

            dir.RotateByAngle_40F6B0(field_24_angle);
            field_14_additional_speed_x = dir.x;
            field_18_additional_speed_y = dir.y;

            Fix16 jitter_x = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 30;
            Fix16 jitter_y = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 30;

            field_8_speed_x = field_14_additional_speed_x + off_x + jitter_x;
            field_C_speed_y = field_18_additional_speed_y + off_y + jitter_y;
            stru_6FD388 = xpos + field_8_speed_x;
            stru_6FD38C = ypos + field_C_speed_y;

            if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
                stru_6FD38C < dword_6FD280 - dword_6FD4A0)
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
            }
            else
            {
                field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, zpos);
            }

            field_30_pNext->field_2C_flags = 0x7A;
            field_30_pNext->Set_2C_0x4_Flag_4337F0();
            field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A0 + Fix16(field_46_sub_state) * dword_6FD2E8, 0);
            if (field_40_pUnknown)
            {
                field_30_pNext->ResolveZOrder_5A1B30(field_40_pUnknown->field_14->field_4);
            }
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
            return 0;
        }
    }
    return true;
}

WIP_FUNC(0x539480)
char_type Particle_4C::UpdateDirectedBurst_state_13_14_36_539480()
{
    Fix16 off_x = dword_6FD49C;
    Fix16 off_y = dword_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state != 16 && field_2C_counter != 0)
    {
        if (field_40_pUnknown)
        {
            if (field_40_pUnknown->field_6_id != field_44)
            {
                field_40_pUnknown = NULL;
                field_20 = dword_6FD49C;
            }
            else if (field_40_pUnknown->field_14)
            {
                if (field_40_pUnknown->field_14->field_4)
                {
                    field_20 = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->sub_5290F0();
                    field_24_angle = field_40_pUnknown->field_14->field_10_obj_3c->field_4_angle;
                }
                if (field_40_pUnknown->field_1A == 1)
                {
                    field_40_pUnknown = NULL;
                }
            }
            else
            {
                field_40_pUnknown = NULL;
                field_20 = dword_6FD49C;
            }
        }
        else
        {
            field_20 = dword_6FD49C;
        }

        Fix16 zpos = field_30_pNext->field_1C_zpos + dword_6FD540;
        if (zpos < dword_6FD28C)
        {

            Fix16 xpos = field_30_pNext->field_14_xy.x;
            Fix16 ypos = field_30_pNext->field_14_xy.y;

            if (field_20 < dword_6FD464)
            {
                off_x = 0;
                off_y = -dword_6FD46C;
            }

            if (field_46_sub_state > 0 && field_46_sub_state <= 4)
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (dword_6FD4A0 - Fix16(field_46_sub_state) * dword_6FD2E8) * field_20;
            }
            else
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (field_20 * dword_6FD300) * Fix16(field_46_sub_state / 10);
            }

            dir.RotateByAngle_40F6B0(field_24_angle);
            field_14_additional_speed_x = dir.x;
            field_18_additional_speed_y = dir.y;

            Fix16 jitter_x = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 50;
            Fix16 jitter_y = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 50;

            field_8_speed_x = field_14_additional_speed_x + off_x + jitter_x;
            field_C_speed_y = field_18_additional_speed_y + off_y + jitter_y;
            stru_6FD388 = xpos + field_8_speed_x;
            stru_6FD38C = ypos + field_C_speed_y;

            if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
                stru_6FD38C < dword_6FD280 - dword_6FD4A0)
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
            }
            else
            {
                field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, zpos);
            }

            field_30_pNext->field_2C_flags = 0x7A;
            field_30_pNext->Set_2C_0x4_Flag_4337F0();
            if (field_40_pUnknown)
            {
                field_30_pNext->ResolveZOrder_5A1B30(field_40_pUnknown->field_14->field_4);
            }
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
            return 0;
        }
    }
    return true;
}

// https://decomp.me/scratch/2UJLM
WIP_FUNC(0x539890)
char_type Particle_4C::UpdateCircularBurst_state_5_539890()
{
    char_type bJitter = 1;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }

    if (field_40_pUnknown)
    {
        if (field_40_pUnknown->field_6_id != field_44)
        {
            field_40_pUnknown = NULL;
        }
        else if (field_40_pUnknown->field_14)
        {
            if (field_40_pUnknown->field_14->field_4)
            {
                field_20 = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->sub_5290F0();
                field_24_angle = field_40_pUnknown->field_14->field_4->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pUnknown->field_1A == 1)
            {
                field_40_pUnknown = NULL;
            }
        }
        else
        {
            field_40_pUnknown = NULL;
            field_20 = dword_6FD49C;
        }
    }
    else
    {
        field_20 = dword_6FD49C;
    }
    field_20 = dword_6FD49C;

    Fix16 zpos = field_30_pNext->field_1C_zpos + dword_6FD540;
    if (zpos >= dword_6FD28C)
    {
        return true;
    }

    Fix16 off_y = -dword_6FD45C;
    Fix16 xpos = field_30_pNext->field_14_xy.x;
    Fix16 ypos = field_30_pNext->field_14_xy.y;

    switch ((u8)(field_2C_counter >> 2))
    {
        case 2:
            if (stru_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 102);
            field_30_pNext->field_2C_flags = 0xC9;
            field_30_pNext->field_2C_flags &= ~4u;
            dir.x = 0;
            dir.y = dword_6FD300 * field_20;
            bJitter = 0;
            break;
        case 3:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 101);
            field_30_pNext->field_2C_flags = 0xC9;
            field_30_pNext->field_2C_flags &= ~4u;
            dir.x = 0;
            dir.y = dword_6FD304 * field_20;
            break;
        case 4:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 100);
            dir.x = 0;
            dir.y = dword_6FD308 * field_20;
            break;
        case 5:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 99);
            dir.x = 0;
            dir.y = dword_6FD308 * field_20;
            break;
        case 6:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 98);
            dir.x = 0;
            dir.y = 0;
            if (field_40_pUnknown && field_40_pUnknown->field_14->field_4)
            {
                Sprite* pTarget = field_40_pUnknown->field_14->field_4;
                xpos = pTarget->field_14_xy.x;
                ypos = pTarget->field_14_xy.y;
                zpos = pTarget->field_1C_zpos;
            }
            bJitter = 0;
            break;
        case 7:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 97);
            dir.x = 0;
            dir.y = 0;
            if (field_40_pUnknown && field_40_pUnknown->field_14->field_4)
            {
                Sprite* pTarget = field_40_pUnknown->field_14->field_4;
                xpos = pTarget->field_14_xy.x;
                ypos = pTarget->field_14_xy.y;
                zpos = pTarget->field_1C_zpos;
            }
            bJitter = 0;
            break;
        default:
            if (stru_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 103);
            dir.x = 0;
            dir.y = field_20 / dword_6FD4A4;
            bJitter = 0;
            if (stru_6F6784.get_int_4F7AE0(7) == 4 && field_30_pNext->field_14_xy.x > dword_6FD4A0 &&
                field_30_pNext->field_14_xy.x < dword_6FD280 - dword_6FD4A0 && field_30_pNext->field_14_xy.y > dword_6FD4A0 &&
                field_30_pNext->field_14_xy.y < dword_6FD280 - dword_6FD4A0)
            {
                Particle_4C* pSmoke = gParticle_8_6FD5E8->New_53E3C0(0, 0, 0, 0, 0, 0);
                if (pSmoke)
                {
                    pSmoke->field_34 = 1;
                    pSmoke->field_38_state = 6;
                    pSmoke->field_2C_counter = 100;
                    pSmoke->field_2E = 100;
                    pSmoke->field_30_pNext->SetType_4206F0(8);
                    pSmoke->field_38_state = 6;
                    pSmoke->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4);
                    pSmoke->field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x,
                                                                field_30_pNext->field_14_xy.y,
                                                                field_30_pNext->field_1C_zpos);
                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pSmoke->field_30_pNext);
                }
            }
            break;
    }

    dir.RotateByAngle_40F6B0(field_24_angle);
    field_14_additional_speed_x = dir.x;
    field_18_additional_speed_y = dir.y;

    Fix16 jitter_x = 0;
    Fix16 jitter_y = 0;
    if (bJitter)
    {
        jitter_x = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
        jitter_y = Fix16(stru_6F6784.get_int_4F7AE0(3) - 1) / 100;
    }

    field_8_speed_x = field_14_additional_speed_x + jitter_x;
    field_C_speed_y = field_18_additional_speed_y + off_y + jitter_y;
    stru_6FD388 = xpos + field_8_speed_x;
    stru_6FD38C = ypos + field_C_speed_y;

    if (stru_6FD388 > dword_6FD4A0 && stru_6FD388 < dword_6FD280 - dword_6FD4A0 && stru_6FD38C > dword_6FD4A0 &&
        stru_6FD38C < dword_6FD280 - dword_6FD4A0)
    {
        field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    }
    else
    {
        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, zpos);
    }

    field_30_pNext->field_2C_flags = 0xA2;
    field_30_pNext->Set_2C_0x4_Flag_4337F0();
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);

    if (++field_46_sub_state > 8)
    {
        field_46_sub_state = 0;
    }
    return 0;
}

MATCH_FUNC(0x53a180)
char_type Particle_4C::UpdateStaticAnim_state_39_53A180()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    if (this->field_2C_counter == 0)
    {
        return 1;
    }

    if (field_48_timer > 0)
    {
        this->field_48_timer--;
    }
    else
    {
        this->field_48_timer = 3;
        this->field_46_sub_state++;
        if (field_46_sub_state > 5u)
        {
            this->field_46_sub_state = 5;
        }
    }

    field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + this->field_46_sub_state + 191);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

WIP_FUNC(0x53a280)
char_type Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280()
{
    Fix16_Point offset;

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    stru_6FD388 = field_30_pNext->field_14_xy.x;
    stru_6FD38C = field_30_pNext->field_14_xy.y;

    Sprite* pTarget = field_28_pSprite;
    if (pTarget->field_30_sprite_type_enum == sprite_types_enum::car_2)
    {
        Car_BC* pCar = pTarget->AsCar_40FEB0();
        if (!pCar || pCar->field_88_despawn_status == 5)
        {
            return true;
        }

        if (++field_46_sub_state == 5)
        {
            return true;
        }

        Ang16 angle = pTarget->field_0;
        Sprite_4C* pBox = pCar->field_50_car_sprite->field_C_sprite_4c_ptr;
        Fix16 half_w;
        Fix16 half_h;
        if (field_46_sub_state < 4)
        {
            half_w = pBox->field_0_width / 2 + dword_6FD3C0;
            half_h = pBox->field_4_height / 2 + dword_6FD5A8;
        }
        else
        {
            half_w = pBox->field_0_width / 2 + dword_6FD3C0;
            half_h = pBox->field_4_height / 2 + dword_6FD2E8 + dword_6FD554;
        }

        if (field_38_state == 40)
        {
            offset.x = half_w;
            offset.y = half_h;
            offset.RotateByAngle_40F6B0(angle);
        }
        else
        {
            offset.y = half_h;
            offset.x = -half_w;
            offset.RotateByAngle_40F6B0(angle);
        }

        Fix16_Point pos = pTarget->get_x_y_443580();
        offset.x += pos.x;
        offset.y += pos.y;

        field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 200);
        field_30_pNext->field_2C_flags = 0xA2;
        field_30_pNext->set_ang_lazy_420690(field_28_pSprite->field_0);
        field_30_pNext->set_xyz_lazy_420600(offset.x, offset.y, field_28_pSprite->field_1C_zpos);
        field_30_pNext->Set_2C_0x4_Flag_4337F0();
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
        return 0;
    }
    else if (pTarget->field_30_sprite_type_enum == sprite_types_enum::ped_3)
    {
        Char_B4* pB4 = pTarget->field_8_char_b4_ptr;
        if (!pB4 || !pB4->field_7C_pPed || !pB4->field_7C_pPed->field_21C_bf.b0)
        {
            return true;
        }

        if (++field_46_sub_state == 4)
        {
            return true;
        }

        Fix16 dist;
        if (!(pB4->field_58_flags & 8))
        {
            dist = dword_6FD2F4;
            offset.x = -dword_6FD46C;
        }
        else
        {
            dist = dword_6FD2F8;
            offset.x = -dword_6FD46C;
        }
        offset.y = dist;
        offset.RotateByAngle_40F6B0(pTarget->field_0);
        offset = offset + Fix16_Point(pB4->field_98.x, pB4->field_98.y);

        field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 197);
        field_30_pNext->field_2C_flags = 0xA2;
        field_30_pNext->set_xyz_lazy_420600(field_28_pSprite->field_14_xy.x + offset.x,
                                            field_28_pSprite->field_14_xy.y + offset.y,
                                            field_28_pSprite->field_1C_zpos);
        field_30_pNext->Set_2C_0x4_Flag_4337F0();
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
        return 0;
    }
    return true;
}

MATCH_FUNC(0x53ab70)
char_type Particle_4C::Empty_state_42_53AB70()
{
    return 0;
}

struct Fix16_Vec2
{
    Fix16 x, y, z;

    Fix16_Vec2(Fix16 x, Fix16 y, Fix16 z) : x(x), y(y), z(z)
    {
    }

    ~Fix16_Vec2()
    {
    }
};

// 9.6f 0x48C270
// 92%
WIP_FUNC(0x53aba0)
char_type Particle_4C::UpdateSimpleBallisticMotion_state_1_53ABA0()
{
    WIP_IMPLEMENTED;

    Fix16_Vec2 v(dword_6FD49C, dword_6FD49C, dword_6FD46C);
    Fix16 new_z;

    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    if (!field_2C_counter)
    {
        return 1;
    }

    if (field_2C_counter > this->field_2E >> 1)
    {
        this->field_8_speed_x = this->field_14_additional_speed_x + this->field_8_speed_x;
        this->field_C_speed_y = this->field_18_additional_speed_y + field_C_speed_y;
        new_z = field_30_pNext->field_1C_zpos + v.z;
    }
    else
    {
        this->field_8_speed_x = this->field_14_additional_speed_x + this->field_8_speed_x;
        this->field_C_speed_y = this->field_18_additional_speed_y + field_C_speed_y;
        new_z = field_30_pNext->field_1C_zpos - v.z;
    }

    stru_6FD388 = v.x + field_8_speed_x + field_30_pNext->field_14_xy.x;
    stru_6FD38C = v.y + field_C_speed_y + field_30_pNext->field_14_xy.y;
    if (new_z > dword_6FD28C)
    {
        new_z = dword_6FD28C;
    }

    this->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);

    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);

    return 0;
}

MATCH_FUNC(0x53ae60)
char_type Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60()
{
    Fix16 v1 = dword_6FD2E8;
    Fix16 x = dword_6FD49C;
    Fix16 y = dword_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state == 15)
    {
        return true;
    }

    Fix16 zpos;
    if (field_2C_counter > (field_2E >> 1))
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos + v1;
    }
    else
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos - v1;
    }
    stru_6FD388 = field_30_pNext->field_14_xy.x + field_8_speed_x + x;
    stru_6FD38C = field_30_pNext->field_14_xy.y + field_C_speed_y + y;

    if (zpos > dword_6FD28C)
    {
        zpos = dword_6FD28C;
    }
    field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 131);
    field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    field_30_pNext->field_2C_flags = 0xA2;
    return 0;
}

MATCH_FUNC(0x53b170)
char_type Particle_4C::Empty_state_44_53B170()
{
    return 0;
}

// https://decomp.me/scratch/fW2BZ
WIP_FUNC(0x53b1a0)
bool Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
    WIP_IMPLEMENTED;
    Fix16 v1 = dword_6FD46C;
    Fix16_Point point(dword_6FD49C, dword_6FD49C);
    Fix16_Point point2(Fix16(0), Fix16(0));

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (!field_2C_counter)
    {
        return true;
    }

    Fix16 zpos;
    if (field_2C_counter > (field_2E >> 1))
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos + v1;
    }
    else
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos - v1;
    }
    stru_6FD388 = point.x + field_8_speed_x + field_30_pNext->field_14_xy.x;
    stru_6FD38C = point.y + field_C_speed_y + field_30_pNext->field_14_xy.y;

    switch (field_2C_counter)
    {
        case 5:
        case 6:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 127);
            break;
        case 4:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4 + 128);
            break;
        case 2:
        case 3:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4 + 129);
            break;
        case 0:
        case 1:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4 + 130);
            break;
        default:
            break;
    }
    if (zpos > dword_6FD28C)
    {
        zpos = dword_6FD28C;
    }
    field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return false;
}

// 9.6f 0x48C790
MATCH_FUNC(0x53b580)
char_type Particle_4C::UpdateShortAnim_state_37_53B580()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    field_46_sub_state++;

    if (field_46_sub_state == 5)
    {
        return 1;
    }

    switch (field_46_sub_state)
    {
        case 1:
            this->field_30_pNext->set_id_lazy_4206C0(field_30_pNext->field_22_sprite_id);
            break;

        default:
            this->field_30_pNext->set_id_4206E0(field_30_pNext->field_22_sprite_id + 4);
            break;
    }

    this->field_30_pNext->Set_2C_0x4_Flag_4337F0();
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

WIP_FUNC(0x53b670)
char_type Particle_4C::UpdateAttachedEmitter_state_9_10_53B670()
{
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }

    stru_6FD388 = field_30_pNext->field_14_xy.x;
    stru_6FD38C = field_30_pNext->field_14_xy.y;

    Sprite* pSprite = field_28_pSprite;
    Fix16 zpos = field_30_pNext->field_1C_zpos;
    Fix16_Point_POD offset;
    if (pSprite->get_type_416B40() != sprite_types_enum::ped_3)
    {
        return true;
    }

    Char_B4* pB4 = pSprite->field_8_char_b4_ptr;

    Ped* pPed = pB4->get_ped_433A20();
    if (!pPed || !pPed->check_bit_0())
    {
        return true;
    }

    if (field_38_state == 9)
    {
        if (pB4->field_6C_animation_state != 2)
        {
            return true;
        }
        field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 3);
        if (pB4->field_68_animation_frame != 5)
        {
            gParticle_8_6FD5E8->SpawnCigaretteSmokePuff_5406B0(field_28_pSprite, 0);
        }
    }
    else
    {
        if (field_2C_counter < 60)
        {
            if (field_2C_counter > 40)
            {
                Ang16 angle(Fix16((stru_6F6784.get_int_4F7AE0(8) - 4) / 2).GetRaw_40F4B0() / 71);
                angle.sub_406C20();
                {
                    Fix16 radius = dword_6FD540 * dword_6FD4A8;
                    angle.rValue = field_28_pSprite->field_0.rValue + angle.rValue;
                    angle.sub_406C20();
                    offset.FromPolar_41E210(radius, angle);
                }
                field_30_pNext->field_2C_flags = 0x51;
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 17);
                zpos += dword_6FD470;
            }
            else
            {
                Fix16 radius = dword_6FD4A0 * dword_6FD540;
                offset.FromPolar_41E210(radius, field_30_pNext->field_0);
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 17);
                field_30_pNext->field_2C_flags = 0x29;
                zpos += dword_6FD470;
            }
        }
        else
        {
            Fix16 radius = dword_6FD540 * dword_6FD4A8;
            offset.FromPolar_41E210(radius, field_28_pSprite->field_0);
            field_30_pNext->field_2C_flags = 0xA1;
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 17);
        }
        stru_6FD388 += offset.x;
        stru_6FD38C += offset.y;
    }

    if (zpos > dword_6FD28C)
    {
        zpos = dword_6FD28C;
    }
    field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return 0;
}

MATCH_FUNC(0x53b9f0)
char_type Particle_4C::UpdateBurstAnimation_state_29_30_53B9F0()
{
    Fix16 scale_related;
    u8 idx;

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state > 25)
    {
        return true;
    }

    if (field_46_sub_state < 20)
    {
        if (field_46_sub_state < 17)
        {
            if (field_46_sub_state < 12)
            {
                scale_related = dword_6FD45C;
                idx = 0;
            }
            else
            {
                scale_related = dword_6FD564;
                idx = 1;
            }
        }
        else
        {
            scale_related = dword_6FD564;
            idx = 2;
        }
    }
    else
    {
        scale_related = dword_6FD538;
        idx = 3;
    }

    field_30_pNext->set_id_4206E0(idx + gPhi_8CA8_6FCF00->field_8CA4 + 37);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    field_30_pNext->field_2C_flags = 0x52;
    field_30_pNext->field_2C_flags |= 4;
    field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A0 + scale_related * Fix16(field_46_sub_state), 0);
    return false;
}

WIP_FUNC(0x53bac0)
char_type Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0()
{
    Char_B4* pB4 = NULL;
    Car_BC* pCar = NULL;
    Fix16_Point vel(Fix16(0), Fix16(0));
    Ang16 angle = 0;

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }

    Fix16 xpos = stru_6FD388 = field_30_pNext->field_14_xy.x;
    Fix16 ypos = stru_6FD38C = field_30_pNext->field_14_xy.y;
    Fix16 zpos = field_30_pNext->field_1C_zpos;

    Sprite* pTarget = field_28_pSprite;
    switch (pTarget->field_30_sprite_type_enum)
    {
        case sprite_types_enum::ped_3:
            pB4 = pTarget->field_8_char_b4_ptr;
            if (!pB4 || !pB4->field_7C_pPed || !pB4->field_7C_pPed->field_21C_bf.b0)
            {
                return true;
            }
            if (!pB4->field_7C_pPed->field_21C_bf.b11)
            {
                field_4_flags &= ~1;
            }
            angle = pTarget->field_0;
            break;

        case sprite_types_enum::car_2:
            pCar = pTarget->field_8_car_bc_ptr;
            if (!pCar || !pCar->field_54_driver)
            {
                return true;
            }
            if (!pCar->field_54_driver->field_21C_bf.b11)
            {
                field_4_flags &= ~1;
            }
            if (!pCar->field_58_physics)
            {
                return true;
            }
            break;

        default:
            return true;
    }

    if (field_48_timer > 0)
    {
        field_48_timer--;
    }
    else
    {
        if (++field_46_sub_state > 15)
        {
            return true;
        }
        if (field_46_sub_state > 3)
        {
            field_48_timer = 1;
        }
    }

    Fix16 spread_val = (Fix16(stru_6F6784.get_int_4F7AE0(field_46_sub_state)) + Fix16(field_46_sub_state) - dword_6FD4AC) * dword_6FD328;
    Ang16 spread(spread_val.GetRaw_40F4B0() / 71);
    spread.sub_406C20();

    if (pB4)
    {
        vel = pB4->field_7C_pPed->sub_45B520();
    }
    else
    {
        Fix16_Point attach;
        Fix16_Point offset;
        Sprite_18* pGun = pCar->field_0_qq.GetSpriteForModel_5A6A50(114);
        if (pGun)
        {
            Ang16 gun_angle = pGun->field_0->field_0 + word_6FD3EE;
            gun_angle.sub_406C20();
            angle = gun_angle;
            attach.x = 0;
            attach.y = dword_6FD2D4;
            attach.RotateByAngle_40F6B0(angle);
            offset.x = 0;
            offset.y = dword_6FD2D0;
        }
        else
        {
            angle = pCar->field_0_qq.GetSpriteForModel_5A6A50(248)->field_0->field_0;
            attach.x = 0;
            attach.y = dword_6FD48C;
            attach.RotateByAngle_40F6B0(angle);
            offset.x = 0;
            offset.y = dword_6FD4CC;
        }
        offset.RotateByAngle_40F6B0(pCar->field_50_car_sprite->field_0);
        Fix16_Point world = offset + pCar->field_50_car_sprite->get_x_y_443580();
        attach.x += world.x;
        attach.y += world.y;
        vel = pCar->field_58_physics->GetPointVelocity_561350(&attach);
    }

    Fix16 speed;
    if (vel.x == dword_6FD49C)
    {
        speed = Fix16::Abs(vel.y);
    }
    else if (vel.y == dword_6FD49C)
    {
        speed = Fix16::Abs_436A50(vel.x);
    }
    else
    {
        speed = Fix16::SquareRoot_436A70(vel.x * vel.x + vel.y * vel.y);
    }

    u8 max_sub_state = 12;
    Fix16 dx;
    Fix16 dy;
    if ((field_4_flags & 1) && field_46_sub_state <= max_sub_state)
    {
        Ang16 dir(spread + angle);
        dir.sub_406C20();
        dx = Ang16::sine_40F500(dir) * dword_6FD2E8;
        dy = Ang16::cosine_40F520(dir) * dword_6FD2E8;
    }
    else
    {
        dx = Ang16::sine_40F500(field_30_pNext->field_0) * dword_6FD2E8;
        dy = Ang16::cosine_40F520(field_30_pNext->field_0) * dword_6FD2E8;
    }
    stru_6FD388 = stru_6FD388 + (vel.x + dx);
    stru_6FD38C = stru_6FD38C + (vel.y + dy);

    if (field_38_state == 31)
    {
        field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 73);
        max_sub_state = 14;
    }
    else
    {
        field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + field_46_sub_state + 111);
    }
    field_30_pNext->field_2C_flags = 0xA2;
    field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);

    if (field_46_sub_state > 1 && field_46_sub_state < max_sub_state)
    {
        Sprite* pHit = field_30_pNext->QuerySpriteCollision_59E7D0(2);
        if (pHit && pHit != field_28_pSprite)
        {
            if (field_38_state == 31)
            {
                field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos + dword_6FD45C);
                if (field_46_sub_state < 10)
                {
                    field_46_sub_state = 10;
                }
                gParticle_8_6FD5E8->field_0->field_4->set_xyz_lazy_420600(xpos, ypos, zpos);
                if (pB4)
                {
                    gParticle_8_6FD5E8->field_0->SetDamageOwner_529080(pB4->field_7C_pPed->field_267_varrok_idx);
                }
                else if (pCar)
                {
                    gParticle_8_6FD5E8->field_0->SetDamageOwner_529080(pCar->field_54_driver->field_267_varrok_idx);
                }
                if (pHit->field_30_sprite_type_enum != sprite_types_enum::code_obj1_4 &&
                    pHit->field_30_sprite_type_enum != sprite_types_enum::map_obj_5 &&
                    pHit->field_30_sprite_type_enum != sprite_types_enum::unknown_1)
                {
                    gParticle_8_6FD5E8->field_0->HandleObjectHit_528990(pHit);
                }
            }
            else
            {
                field_46_sub_state = max_sub_state;
                field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos + dword_6FD45C);
                if (!(rng_dword_67AB34->field_0_rng & 1))
                {
                    gParticle_8_6FD5E8->EmitWaterSplash_53F060(xpos, ypos, zpos, angle, 0);
                }
                gParticle_8_6FD5E8->field_4->field_4->set_xyz_lazy_420600(xpos, ypos, zpos);
                if (pB4)
                {
                    gParticle_8_6FD5E8->field_4->SetDamageOwner_529080(pB4->field_7C_pPed->field_267_varrok_idx);
                }
                else if (pCar)
                {
                    gParticle_8_6FD5E8->field_4->SetDamageOwner_529080(pCar->field_54_driver->field_267_varrok_idx);
                }
                if (pHit->field_30_sprite_type_enum != sprite_types_enum::code_obj1_4 &&
                    pHit->field_30_sprite_type_enum != sprite_types_enum::map_obj_5 &&
                    pHit->field_30_sprite_type_enum != sprite_types_enum::unknown_1)
                {
                    gParticle_8_6FD5E8->field_4->HandleObjectHit_528990(pHit);
                }
                if (pHit->field_30_sprite_type_enum == sprite_types_enum::ped_3)
                {
                    pHit->field_8_char_b4_ptr->HandleGenericImpact_553E00(angle, dword_6FD46C + dword_6FD2E8, dword_6FD49C, 0);
                    Ped* pHitPed = pHit->field_8_char_b4_ptr->field_7C_pPed;
                    if (pHitPed)
                    {
                        if (field_38_state == 31)
                        {
                            gfrosty_pasteur_6F8060->sub_512C00(pHitPed->field_200_id, 194, 1);
                        }
                        else
                        {
                            gfrosty_pasteur_6F8060->sub_512C00(pHitPed->field_200_id, 198, 1);
                        }
                    }
                }
            }
        }
    }

    if (field_30_pNext->CheckSpriteMovementRegion_5A2500())
    {
        field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos + dword_6FD45C);
        if (field_46_sub_state < 10)
        {
            field_46_sub_state = 10;
        }
    }
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return 0;
}

WIP_FUNC(0x53d260)
char_type Particle_4C::PoolUpdate()
{
    WIP_IMPLEMENTED;

    s32 state = this->field_38_state;
    --this->field_2C_counter;
    switch (state - 1)
    {
        case 0:
            return UpdateSimpleBallisticMotion_state_1_53ABA0();
        case 2:
        case 11:
            return UpdateDirectedProjectile_state_3_12_5384C0();

        case 3:
            return UpdateDirectedBurstSweep_state_4_539040();

        case 4:
            return UpdateCircularBurst_state_5_539890();

        case 5:
        case 14:
        case 15:
        case 16:
            return UpdateFloatingParticle_state_6_15_16_17_538060();

        case 6:
            return UpdateDebrisArc_state_7_53B1A0();

        case 7:
            return true;

        case 8:
        case 9:
            return UpdateAttachedEmitter_state_9_10_53B670();

        case 12:
        case 13:
        case 35:
            return UpdateDirectedBurst_state_13_14_36_539480();

        case 17:
        case 32:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 15u)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 20);
                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = 0xA2; // sub_4337D0
                    }
                    this->field_30_pNext->field_2C_flags |= 4u;
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 18:
        case 31:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 15u)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 20);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = 0xA2;
                    }
                    field_30_pNext->field_2C_flags |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A0 + dword_6FD39C, 0);
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 19:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 0xAu)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 0xFu)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 56);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2E8 + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_451950(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = -94;
                    }

                    field_30_pNext->field_2C_flags |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A4, 0);
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 21:
        case 22:
        case 23:
        case 24:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }

            gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
            this->field_48_timer = 1;
            this->field_46_sub_state++;
            if (field_46_sub_state > 0xCu)
            {
                return true;
            }

            field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 147);

            stru_6FD388 = (gSin_table_667A80[this->field_24_angle.rValue] * this->field_20);
            stru_6FD38C = (gCos_table_669260[this->field_24_angle.rValue] * this->field_20);

            stru_6FD388 = this->field_30_pNext->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_30_pNext->field_14_xy.y + stru_6FD38C;


            if (this->field_46_sub_state >= 9u)
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_30_pNext->field_1C_zpos + dword_6FD46C);
            }
            else
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_30_pNext->field_1C_zpos + dword_6FD2E8);
            }

            if (field_30_pNext->field_1C_zpos > dword_6FD28C)
            {
                field_30_pNext->field_1C_zpos = dword_6FD28C;
                field_30_pNext->ResetZCollisionAndDebugBoxes_59E7B0();
            }

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
            field_30_pNext->field_2C_flags = 82;
            field_30_pNext->field_2C_flags |= 4u;
            return false;

        case 25:
            gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
            if (this->field_2C_counter)
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                return false;
            }
            else
            {
                return true;
            }
            break;

        case 28:
        case 29:
            return UpdateBurstAnimation_state_29_30_53B9F0();

        case 30:
        case 33:
            return UpdateCollisionBurst_state_31_34_53BAC0();

        case 34:
            return UpdateLargeBallisticDebris_state_35_53AE60();

        case 36:
            return UpdateShortAnim_state_37_53B580();

        case 37:
            return UpdateObjectBeamLink_state_38_538AC0();

        case 38:
            return UpdateStaticAnim_state_39_53A180();

        case 39:
        case 40:
            return UpdateSkidOrScrapeSpark_state_40_41_53A280();

        case 41:
            return Empty_state_42_53AB70();

        case 42:
            return UpdateBeamSegment_state_43_538A40();

        case 43:
            return Empty_state_44_53B170();

        default:
            return false;
    }
}

MATCH_FUNC(0x53e2c0)
void Particle_4C::PoolAllocate()
{
    field_8_speed_x = 0;
    field_C_speed_y = 0;
    field_10 = 0;
    field_40_pUnknown = 0;
    field_4_flags &= ~1;
}

MATCH_FUNC(0x53e2e0)
void Particle_4C::PoolDeallocate()
{
    if (field_30_pNext)
    {
        field_30_pNext->field_2C_flags &= ~4u;
        field_30_pNext->field_2C_flags = 0;
        gSprite_Pool_703818->remove(field_30_pNext);
        field_30_pNext = 0;
    }
}
