#include "CarAI_78.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Hamburger_500.hpp"
#include "Object_5C.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "rng.hpp"
#include "map_0x370.hpp"
#include "Game_0x40.hpp"
#include "Police_7B8.hpp"
#include "Orca_2FD4.hpp"
#include "PublicTransport.hpp"

DEFINE_GLOBAL(Ang16, word_677CE8, 0x677CE8);
DEFINE_GLOBAL(Fix16, dword_677B90, 0x677B90);
DEFINE_GLOBAL(Fix16, dword_677C30, 0x677C30);
DEFINE_GLOBAL(Fix16, dword_677C38, 0x677C38);
DEFINE_GLOBAL(Fix16, dword_677C48, 0x677C48);
DEFINE_GLOBAL(Fix16, dword_6779F8, 0x6779F8);

DEFINE_GLOBAL_INIT(Fix16, dword_677BA0, Fix16(0x10000, 0), 0x677BA0);
DEFINE_GLOBAL_INIT(Fix16, dword_677B5C, Fix16(0x147, 0), 0x677B5C);
DEFINE_GLOBAL_INIT(Fix16, dword_677AC4, dword_677B5C, 0x677AC4);

DEFINE_GLOBAL_INIT(Fix16, dword_677B94, Fix16(0x4000, 0), 0x677B94);
DEFINE_GLOBAL_INIT(Fix16, dword_6779B8, Fix16(0x666, 0), 0x6779B8);
DEFINE_GLOBAL_INIT(Fix16, dword_677B50, Fix16(163, 0), 0x677B50);
DEFINE_GLOBAL_INIT(Fix16, dword_677CB4, dword_6779B8, 0x677CB4);
DEFINE_GLOBAL_INIT(Fix16, dword_6779A4, dword_677B50, 0x6779A4);

DEFINE_GLOBAL_INIT(Fix16, dword_6779D4, Fix16(0x2CCC, 0), 0x6779D4);
DEFINE_GLOBAL(CarAI_78_Pool*, gCarAI_78_Pool_677CF8, 0x677CF8);

DEFINE_GLOBAL(Fix16, dword_677B00, 0x677B00);
DEFINE_GLOBAL_INIT(Fix16, dword_677B60, Fix16(0x333, 0), 0x677B60);

DEFINE_GLOBAL(u8, byte_677BBC, 0x677BBC);
DEFINE_GLOBAL(u8, byte_677B3C, 0x677B3C);
DEFINE_GLOBAL(u8, byte_677A78, 0x677A78);
DEFINE_GLOBAL(u8, byte_677C90, 0x677C90);
DEFINE_GLOBAL(u8, byte_677A5D, 0x677A5D);
DEFINE_GLOBAL(u8, byte_677A6C, 0x677A6C);
DEFINE_GLOBAL(u8, byte_677A94, 0x677A94);
DEFINE_GLOBAL(u8, byte_677A5C, 0x677A5C);
DEFINE_GLOBAL(u8, byte_677B8C, 0x677B8C);
DEFINE_GLOBAL(u8, byte_677CA8, 0x677CA8);
DEFINE_GLOBAL(u8, byte_677C06, 0x677C06);

DEFINE_GLOBAL(Fix16, dword_677A74, 0x677A74);
DEFINE_GLOBAL(Fix16, dword_677A80, 0x677A80);
DEFINE_GLOBAL(Fix16, dword_677C9C, 0x677C9C);
DEFINE_GLOBAL(Fix16, dword_677A8C, 0x677A8C);
DEFINE_GLOBAL(Fix16, dword_6779F0, 0x6779F0);
DEFINE_GLOBAL(Fix16, dword_6779F4, 0x6779F4);

DEFINE_GLOBAL(s32, dword_677C88, 0x677C88);
DEFINE_GLOBAL(Car_BC*, dword_6779B0, 0x6779B0);

DEFINE_GLOBAL_INIT(Fix16, dword_6779C8, Fix16(0x1999, 0), 0x6779C8);

DEFINE_GLOBAL(Fix16, dword_6779C0, 0x6779C0);
DEFINE_GLOBAL(Fix16, dword_677BB8, 0x677BB8);
DEFINE_GLOBAL(Fix16, dword_677B9C, 0x677B9C);
DEFINE_GLOBAL(Fix16, dword_677A84, 0x677A84);
DEFINE_GLOBAL(Fix16, dword_677A4C, 0x677A4C);
DEFINE_GLOBAL(Fix16, dword_677B58, 0x677B58);
DEFINE_GLOBAL(Fix16, dword_677B98, 0x677B98);
DEFINE_GLOBAL(Fix16, dword_6779BC, 0x6779BC);
DEFINE_GLOBAL(Fix16, dword_677CBC, 0x677CBC);
DEFINE_GLOBAL(Fix16, dword_677A38, 0x677A38);

DEFINE_GLOBAL(Ang16, word_677ADE, 0x677ADE);
DEFINE_GLOBAL(Ang16, dword_6779E4, 0x6779E4);
DEFINE_GLOBAL(Ang16, dword_677A2E, 0x677A2E);
DEFINE_GLOBAL(Ang16, word_677CE2, 0x677CE2);

DEFINE_GLOBAL(Fix16, dword_677C84, 0x677C84);
DEFINE_GLOBAL(Ang16, word_677B08, 0x677B08);
DEFINE_GLOBAL(Fix16, dword_677BA4, 0x677BA4);
DEFINE_GLOBAL(Fix16, dword_677CA0, 0x677CA0);

EXTERN_GLOBAL(u16, word_677CFC);

MATCH_FUNC(0x4476f0)
void CarAI_78::sub_4476F0()
{
    if (field_0->IsPoliceCar_439EC0())
    {
        field_0->sub_4417F0();
    }
}

WIP_FUNC(0x447710)
void CarAI_78::sub_447710()
{
    WIP_IMPLEMENTED;

    u8 v12 = 0;
    if (field_28_junc_idx >= 0)
    {
        Junction_10* Junction_58A0B0 =
            gRouteFinder_6FFDC8->GetJunction_58A0B0(gRouteFinder_6FFDC8->field_2218[field_28_junc_idx].field_0[this->field_56]);
        u16 v4 = gRouteFinder_6FFDC8->field_2218[this->field_28_junc_idx].field_0[this->field_56 + 1];
        if (v4)
        {
            if ((this->field_24_flags & 1) == 0)
            {
                u16 v5 = Junction_58A0B0->sub_5885C0(v4);
                switch (Ang16::GetAngleFace_4F78F0(this->field_10))
                {
                    case 1:
                        this->field_0->field_A6 &= ~1u;
                        this->field_0->field_A6 &= ~2u;
                        switch (v5)
                        {
                            case 2:
                                this->field_38 = 2;
                                this->field_24_flags = 0x800;
                                break;
                            case 3:
                                //goto LABEL_14;
                                this->field_0->field_A6 |= 1u;
                                break;
                            case 4:
                                //goto LABEL_8;
                                this->field_0->field_A6 |= 2u;
                                break;
                        }
                        break;

                    case 2:
                        this->field_0->field_A6 &= ~1u;
                        this->field_0->field_A6 &= ~2u;
                        switch (v5)
                        {
                            case 1:
                                this->field_38 = 1;
                                this->field_24_flags |= 0x800u;
                                break;
                            case 3:
                                //goto LABEL_8;
                                this->field_0->field_A6 |= 2u;
                                break;
                            case 4:
                                //goto LABEL_14;
                                this->field_0->field_A6 |= 1u;
                                break;
                        }
                        break;

                    case 3:
                        this->field_0->field_A6 &= ~1u;
                        this->field_0->field_A6 &= ~2u;
                        switch (v5)
                        {
                            case 1:
                                //goto LABEL_14;
                                this->field_0->field_A6 |= 1u;
                                break;
                            case 2:
                                //goto LABEL_8;
                                this->field_0->field_A6 |= 2u;
                                break;
                            case 3:
                                this->field_38 = 4;
                                this->field_24_flags |= 0x800u;
                                break;
                        }
                        break;

                    case 4:
                        this->field_0->field_A6 &= ~1u;
                        this->field_0->field_A6 &= ~2u;
                        switch (v5)
                        {
                            case 1:
                                //LABEL_8:
                                this->field_0->field_A6 |= 2u;
                                break;
                            case 2:
                                //LABEL_14:
                                this->field_0->field_A6 |= 1u;
                                break;
                            case 4:
                                this->field_38 = 3;
                                this->field_24_flags |= 0x800u;
                                break;
                        }
                        break;

                    default:
                        break;
                }

                char_type v6 = this->field_0->CountConsecutiveArrowBlocks_4410D0(this->field_10,
                                                         &v12,
                                                         this->field_0->field_50_car_sprite->field_14_xy.x,
                                                         this->field_0->field_50_car_sprite->field_14_xy.y);
                if (v12 > -1)
                {
                    char_type v7 = this->field_0->field_A6;
                    if ((v7 & 1) == 1)
                    {
                        if (v6)
                        {
                            if ((v7 & 4) != 4)
                            {
                                sub_4539B0();
                                this->field_0->field_A6 |= 4u;
                            }
                        }
                    }
                    else if ((v7 & 2) == 2 && v6 != v12 - 1 && (v7 & 8) != 8)
                    {
                        sub_4539B0();
                        this->field_0->field_A6 |= 8u;
                    }
                }
            }
        }
    }
}

STUB_FUNC(0x447970)
void CarAI_78::sub_447970()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x447ca0)
bool CarAI_78::GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 maybe_direction)
{
    field_28_junc_idx = gRouteFinder_6FFDC8->StartRoute_58A190(dword_677C38.ToInt(),
                                                               dword_677C30.ToInt(),
                                                               (dword_677C48 - dword_677B94).ToInt(),
                                                               x,
                                                               y,
                                                               z,
                                                               maybe_direction);
    if (field_28_junc_idx > 0)
    {
        field_0->field_60->field_14_target_x = Fix16(x);
        field_0->field_60->field_18_target_y = Fix16(y);
        field_0->field_60->field_1C_target_z = dword_6779F8;
        field_56 = 0;
        return true;
    }
    return false;
}

STUB_FUNC(0x447d40)
char_type CarAI_78::sub_447D40(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x448270)
bool CarAI_78::sub_448270()
{
    switch (field_4C)
    {
        case 1:
            if (field_44 == 3)
            {
                return true;
            }
            break;
        case 3:
            if (field_44 == 2)
            {
                return true;
            }
            break;
        case 2:
            if (field_44 == 4)
            {
                return true;
            }
            break;
        case 4:
            if (field_44 == 1)
            {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

STUB_FUNC(0x4482c0)
char_type CarAI_78::sub_4482C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448770)
char_type CarAI_78::sub_448770()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448ce0)
char_type CarAI_78::sub_448CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44a1f0)
char_type CarAI_78::sub_44A1F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44af00)
s16 CarAI_78::sub_44AF00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44d1d0)
char_type CarAI_78::sub_44D1D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44e0c0)
void CarAI_78::sub_44E0C0()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x44e560)
void CarAI_78::UpdateStateMachine_44E560()
{
    NOT_IMPLEMENTED;

    Ang16 v248 = word_677CE8;
    Ang16 v263 = word_677CE8;
    Ang16 v256 = word_677CE8;
    Fix16 v241 = dword_677B90;
    this->field_24_flags &= ~0x4580u;
    //Hamburger_40* v4 = field_0->field_60;
    s32 v5 = 0;
    s32 v6 = 0;
    Hamburger_40* pHam40 = 0;
    s32 v270 = 0;
    s32 v242 = 0;
    s32 v243 = 0;

    u8 v246; // bool?
    u8 v71; // bool?

    Fix16 v12;
    Fix16 v245;

    if (field_0->field_60)
    {
        pHam40 = field_0->field_60;
        if (field_0->field_60->field_8_maybe_path_type - 1)
        {
            if ((field_0->field_60->field_8_maybe_path_type - 1) == 1)
            {
                field_0->field_60->field_C = 5;
            }
            else
            {
                if (field_0->field_60->field_30_ped_to_follow->field_168_game_object)
                {
                    field_0->field_60->field_C = 0;
                }
                else if (field_0->IsPoliceCar_439EC0())
                {
                    switch (field_0->field_60->field_30_ped_to_follow->get_wanted_star_count_46EF00())
                    {
                        case 0u:
                        case 1u:
                            v6 = 1; // Only checked for this value
                            break;
                        case 2u:
                        case 3u:
                            v6 = 2;
                            break;
                        case 4u:
                            v6 = 3;
                            break;
                        case 5u:
                            v6 = 4;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    this->field_24_flags &= 0xFFEFFFFF;
    if (!field_3C)
    {
        byte_677A5D = 0;
        u8 followerCount =
            (u8)gHamburger_500_678E30->CountFollowers_474920(pHam40->field_30_ped_to_follow, this->field_0->field_54_driver) - 1;
        // TODO: Switch case
        if (followerCount)
        {
            if (followerCount == 1)
            {
                // case 1
                v12 = dword_6779C0 + dword_677B60;
            }
            else
            {
                // default
                v12 = dword_677BB8;
            }
        }
        else
        {
            // case 0
            v12 = dword_6779C0;
        }

        switch (pHam40->field_C)
        {
            case 0:
            {
                if (dword_6779B0)
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v265 = dword_6779B0->field_50_car_sprite->field_0 - word_677ADE;

                    dword_6779F0 += (Ang16::sine_40F500(v265) * dword_677B9C);
                    dword_6779F4 += (Ang16::cosine_40F520(v265) * dword_677B9C);

                    Fix16 v219 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v17 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v17, v219);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                else
                {
                    Fix16 v220 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v18 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v18, v220);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                break;
            }

            case 1:
            {
                if (dword_6779B0)
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v254 = dword_6779B0->field_50_car_sprite->field_0 - word_677ADE;
                    dword_6779F0 += (dword_677B9C * Ang16::sine_40F500(v254));
                    dword_6779F4 += (dword_677B9C * Ang16::cosine_40F520(v254));

                    Fix16 v222 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v29 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v29, v222);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                break;
            }

            case 2:
            {
                if (dword_6779B0)
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v266 = dword_6779B0->field_50_car_sprite->field_0 - word_677ADE;
                    dword_6779F0 += (dword_677B9C * Ang16::sine_40F500(v266));
                    dword_6779F4 += (dword_677B9C * Ang16::cosine_40F520(v266));

                    Ang16 v264 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;
                    dword_6779F0 += (dword_677A84 * Ang16::sine_40F500(v264));
                    dword_6779F4 += (dword_677A84 * Ang16::cosine_40F520(v264));

                    Fix16 v221 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v24 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v24, v221);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                break;
            }

            case 3:
            {
                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v253 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;
                dword_6779F0 += (dword_677B94 * Ang16::sine_40F500(v253));
                dword_6779F4 += (dword_677B94 * Ang16::cosine_40F520(v253));

                dword_6779F0 += (dword_677A84 * Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0));
                dword_6779F4 += (dword_677A84 * Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0));

                Fix16 v223 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v36 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                v245 = Fix16::Max_44E540(v36, v223);

                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 4:
            {
                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v240 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;
                dword_6779F0 += (dword_677B94 * Ang16::sine_40F500(v240));
                dword_6779F4 += (dword_677B94 * Ang16::cosine_40F520(v240));

                dword_6779F0 += (dword_677A84 * Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0));
                dword_6779F4 += (dword_677A84 * Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0));

                Fix16 v225 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v50 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                v245 = Fix16::Max_44E540(v50, v225);
                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 5:
            {
                Fix16 v42;
                if (v6 == 1)
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v244 = dword_6779B0->field_50_car_sprite->field_0 - word_677ADE;
                    dword_6779F0 += (dword_677B94 * Ang16::sine_40F500(v244));
                    v42 = dword_6779F0; // TODO: Check ?
                    dword_6779F4 += (dword_677B94 * Ang16::cosine_40F520(v244));
                }
                else
                {
                    v42 = dword_6779F0;
                }
                Fix16 v224 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v43 = Fix16::Abs_436A50(v42 - dword_677C38);
                v245 = Fix16::Max_44E540(v43, v224);
                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 6:
            {
                sub_4476F0();
                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v260 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;
                dword_6779F0 += (Ang16::sine_40F500(v260) * dword_677A4C);
                dword_6779F4 += (Ang16::cosine_40F520(v260) * dword_677A4C);

                Fix16 v226 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v54 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                v245 = Fix16::Max_44E540(v54, v226);

                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 7:
            {
                sub_4476F0();
                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v258 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;
                dword_6779F0 += (Ang16::sine_40F500(v258) * dword_677A4C);
                dword_6779F4 += (Ang16::cosine_40F520(v258) * dword_677A4C);

                Fix16 v229 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v70 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                v245 = Fix16::Max_44E540(v70, v229);
                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 8:
            {
                v246 = 0;
                sub_4476F0();
                if (pHam40->field_8_maybe_path_type == 3)
                {
                    goto LABEL_66;
                }

                if (pHam40->field_8_maybe_path_type == 4)
                {
                    if (this->field_0->field_84_car_info_idx != car_model_enum::EDSELFBI)
                    {
                        v12 = dword_677BB8;
                    }
                    if (dword_6779B0->GetMaxSpeed_439F30() <= v12)
                    {
                        switch (gHamburger_500_678E30->CountFollowers_474920(pHam40->field_30_ped_to_follow, field_0->field_54_driver))
                        {
                            case 1:
                                v5 = gPolice_7B8_6FEE40->field_654_wanted_level != 1 ? 10 : 50;
                                break;

                            case 2:
                                if (!gHamburger_500_678E30->HasRelationshipCode_4_5_474C30(this->field_0->field_54_driver))
                                {
                                    goto LABEL_84;
                                }
                                v5 = 0;
                                break;

                            case 3:
                            case 4:
                                if (gHamburger_500_678E30->HasRelationshipCode_9_474AF0(this->field_0->field_54_driver))
                                {
                                    v5 = 0;
                                }
                                else
                                {
                                LABEL_84:
                                    v5 = 1;
                                    pHam40->field_2E = 0;
                                }
                                break;

                            default:
                                goto LABEL_78;
                        }
                    }
                    else if (pHam40->field_3C <= 3u)
                    {
                        v246 = 0;
                    }
                    else
                    {
                        v246 = 1;
                    LABEL_78:
                        v5 = 10;
                    }
                }
                else if (pHam40->field_8_maybe_path_type == 5 && dword_6779B0->GetMaxSpeed_439F30() > v12)
                {
                LABEL_66:
                    v246 = 1;
                }

                if ((u16)pHam40->field_2E >= v5)
                {
                    if (v246 != 1)
                    {
                        pHam40->field_C = 10;
                        return;
                    }
                    pHam40->field_10 = 1;

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);
                    dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);

                    Fix16 v228 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v66 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v66, v228);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                else
                {
                    ++pHam40->field_2E;

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v262 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;
                    dword_6779F4 += (Ang16::cosine_40F520(v262) * dword_677A4C);
                    dword_6779F0 += (Ang16::sine_40F500(v262) * dword_677A4C);
                    dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677A84);
                    dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677A84);

                    Fix16 v227 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v61 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                    v245 = Fix16::Max_44E540(v61, v227);

                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                break;
            }

            case 9:
            {
                v71 = 0;
                sub_4476F0();
                if (pHam40->field_8_maybe_path_type == 3)
                {
                    goto LABEL_103;
                }

                if (pHam40->field_8_maybe_path_type == 4)
                {
                    v71 = 0;
                    if (this->field_0->field_84_car_info_idx != car_model_enum::EDSELFBI)
                    {
                        v12 = dword_677BB8;
                    }

                    if (dword_6779B0->GetMaxSpeed_439F30() <= v12)
                    {
                        switch (gHamburger_500_678E30->CountFollowers_474920(pHam40->field_30_ped_to_follow, field_0->field_54_driver))
                        {
                            case 1:
                                v5 = gPolice_7B8_6FEE40->field_654_wanted_level != 1 ? 10 : 50;
                                break;

                            case 2:
                                if (!gHamburger_500_678E30->HasRelationshipCode_4_5_474C30(this->field_0->field_54_driver))
                                {
                                    goto LABEL_121;
                                }
                                v5 = 0;
                                break;
                            case 3:
                            case 4:
                                if (gHamburger_500_678E30->HasRelationshipCode_8_474A20(this->field_0->field_54_driver))
                                {
                                    v5 = 0;
                                }
                                else
                                {
                                LABEL_121:
                                    v5 = 1;
                                    pHam40->field_2E = 0;
                                }
                                break;
                            default:

                                goto LABEL_115;
                        }
                    }
                    else if (pHam40->field_3C <= 3u)
                    {
                        v71 = 0;
                    }
                    else
                    {
                        v71 = 1;
                    LABEL_115:
                        v5 = 10;
                    }
                }
                else if (pHam40->field_8_maybe_path_type == 5 && dword_6779B0->GetMaxSpeed_439F30() > v12)
                {
                LABEL_103:
                    v71 = 1;
                }
                if ((u16)pHam40->field_2E >= v5)
                {
                    if (v71 != 1)
                    {
                        pHam40->field_C = 11;
                        return;
                    }
                    pHam40->field_10 = 1;

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);
                    dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);
                    dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);

                    Fix16 v231 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v82 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);

                    v245 = Fix16::Max_44E540(v82, v231);
                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                else
                {
                    ++pHam40->field_2E;

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v259 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;
                    dword_6779F4 += (Ang16::cosine_40F520(v259) * dword_677A4C);
                    dword_6779F0 += (Ang16::sine_40F500(v259) * dword_677A4C);
                    dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);
                    dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94);

                    Fix16 v230 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                    Fix16 v77 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                    v245 = Fix16::Max_44E540(v77, v230);

                    v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                }
                break;
            }

            case 10:
            {
                sub_4476F0();

                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v257 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;

                dword_6779F0 += Ang16::sine_40F500(v257) * dword_677A4C;
                dword_6779F4 += Ang16::cosine_40F520(v257) * dword_677A4C;
                dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94 + dword_677A84);
                dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677B94 + dword_677A84);

                Fix16 v232 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v88 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                v245 = Fix16::Max_44E540(v88, v232);

                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 11:
            {
                sub_4476F0();

                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                Ang16 v261 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;

                dword_6779F0 += (Ang16::sine_40F500(v261) * dword_677A4C);
                dword_6779F4 += (Ang16::cosine_40F520(v261) * dword_677A4C);

                Fix16 v272 = dword_677B94 + dword_677A84;

                dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * v272);
                dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * v272);

                Fix16 v233 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v94 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                v245 = Fix16::Max_44E540(v94, v233);
                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                break;
            }

            case 12:
            case 13:
            {
                sub_4476F0();
                field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                dword_6779F0 += (Ang16::sine_40F500(dword_6779B0->field_50_car_sprite->field_0) * dword_677B9C);
                dword_6779F4 += (Ang16::cosine_40F520(dword_6779B0->field_50_car_sprite->field_0) * dword_677B9C);

                Fix16 v234 = Fix16::Abs_436A50(dword_6779F4 - dword_677C30);
                Fix16 v99 = Fix16::Abs_436A50(dword_6779F0 - dword_677C38);
                v245 = Fix16::Max_44E540(v99, v234);
                v248 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                if (pHam40->field_C != 13)
                {
                    break;
                }

                v256 = Fix16::atan2_fixed_405320(dword_6779F4 - dword_677C30, dword_6779F0 - dword_677C38);
                if (dword_677A8C >= dword_677B58)
                {
                    if (Ang16::IsAngleAhead_405C60(&this->field_10, &v256))
                    {
                        this->field_0->field_58_physics->field_AD_turn_direction = 1;
                    }
                    else
                    {
                        this->field_0->field_58_physics->field_AD_turn_direction = -1;
                    }

                    if (ComputeShortestAngleDelta_4056C0(this->field_10, dword_6779B0->field_50_car_sprite->field_0) <= dword_677A2E)
                    {
                        this->field_0->field_58_physics->field_92_is_hand_brake_on = 1;
                    }
                    else
                    {
                        pHam40->field_C = 15;
                    }
                }
                else
                {
                    field_0->sub_43A950();
                    pHam40->field_2C = 0;
                    pHam40->field_C = 15;
                }
                return;
            }

            case 15:
            {
                this->field_24_flags |= 0x100000u;
                field_0->field_A6 |= 0x20u;
                break;
            }

            default:
                break;
        }
    }

    Ang16 v267 = ComputeShortestAngleDelta_4056C0(this->field_10, v248);

    v263 = this->field_10;
    v256 = v248;
    s32 AngleFace_4F78F0 = Ang16::GetAngleFace_4F78F0(v256);
    s32 v104 = this->field_4C;
    if (v104 == AngleFace_4F78F0)
    {
        Ang16 v110;
        if (v104 == 2)
        {
            Ang16 v109 = dword_6779E4 + this->field_10;
            v263 = v109;

            Ang16 v240 = v248 + dword_6779E4;
            v110 = v240;
            v256 = v240;
        }
        else
        {
            v110 = v256;
        }

        Ang16 v240 = v110 - word_677CE2;
        Ang16 v244 = v240 - word_677CE2;
        if (v263 >= v244)
        {
            v240 = v256 + word_677CE2;
            //Ang16::Normalize_406C20(&v240);
            v244 = word_677CE2 + v240;
            //Ang16::Normalize_406C20(&v244);
            if (v263 <= v244)
            {
                CarPhysics_B0* t = this->field_0->field_58_physics;
                t->field_AD_turn_direction = 0;
                t->field_78_pointing_ang_rad = dword_677B90;
                byte_677A5C = 1;
                goto LABEL_186;
            }

            if (!stru_6F6784.get_int_4F7AE0(4))
            {
                if ((this->field_24_flags & 0x40) == 0)
                {
                    this->field_0->field_58_physics->field_AD_turn_direction = -1;
                    goto LABEL_186;
                }
                goto LABEL_183;
            }
        }
        else
        {
            if (!stru_6F6784.get_int_4F7AE0(4))
            {
                if ((this->field_24_flags & 0x40) != 0)
                {
                    this->field_0->field_58_physics->field_AD_turn_direction = -1;
                    goto LABEL_186;
                }
            LABEL_183:
                this->field_0->field_58_physics->field_AD_turn_direction = 1;
            }
        }
    LABEL_186:
        field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_677C38, dword_677C30, dword_677C48);
        goto LABEL_190;
    }

    if (v245 < dword_677BA0 && !this->field_3C)
    {
        s32 v105;
        switch (AngleFace_4F78F0)
        {
            case 1:
                v105 = 2;
                break;
            case 2:
                v105 = 1;
                break;
            case 3:
                v105 = 4;
                break;
            case 4:
                v105 = 3;
                break;
            default:
                v105 = 0;
                break;
        }

        if (v104 == v105 && v245 < dword_677B9C)
        {
            CarPhysics_B0* v106 = this->field_0->field_58_physics;
            v106->field_91_is_foot_brake_on = 0;
            v106->field_93_is_forward_gas_on = 0;
            v106->field_94_is_backward_gas_on = 0;
            v106->field_95 = 0;
        }
    }

    if (!Ang16::IsAngleAhead_405C60(&v263, &v256))
    {
        if ((this->field_24_flags & 0x40) == 0)
        {
            goto LABEL_169;
        }
        this->field_0->field_58_physics->field_AD_turn_direction = 1;
    }
    else
    {
        if ((this->field_24_flags & 0x40) != 0)
        {
        LABEL_169:
            this->field_0->field_58_physics->field_AD_turn_direction = -1;
            goto LABEL_190;
        }
        this->field_0->field_58_physics->field_AD_turn_direction = 1;
    }

LABEL_190:
    //dword_6779F0 = dword_6779F0;
    //dword_6779F4 = dword_6779F4;
    if (!this->field_3C)
    {
        field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_677C38, dword_677C30, dword_677C48);

        Fix16 v247 = ((dword_677A38)*Fix16(this->field_0->field_58_physics->field_AD_turn_direction));
        Ang16 v244;
        v244.sub_4516B0(&v247, 0); // ctor ?

        Ang16 v240 = this->field_10 + v244;

        field_0->field_50_car_sprite->set_xyz_lazy_420600(
            (Ang16::sine_40F500(v240) * dword_677B94) + field_0->field_50_car_sprite->field_14_xy.x,
            (Ang16::cosine_40F520(v240) * dword_677B94) + field_0->field_50_car_sprite->field_14_xy.y,
            field_0->field_50_car_sprite->field_1C_zpos);

        if (field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
        {
            CarPhysics_B0* v126 = this->field_0->field_58_physics;
            v126->field_AD_turn_direction = 0;
            v126->field_78_pointing_ang_rad = dword_677B90;
        }
    }

    if (dword_677A8C < dword_677B58)
    {
        field_0->sub_43A950();
        pHam40->field_2C = 0;
        pHam40->field_C = 15;
        return;
    }

    if (v245 >= dword_677B98 || this->field_3C)
    {
        if (this->field_0->field_60)
        {
            if (pHam40->field_2A)
            {
                pHam40->field_2A--;
            }
            else if (pHam40->field_8_maybe_path_type != 1)
            {
                pHam40->field_C = 13;
                if (gHamburger_500_678E30->FindOwnerForFollowCode_4748A0(0, pHam40->field_30_ped_to_follow))
                {
                    pHam40->field_C = 2;
                }
                else
                {
                    /*
                    v217 = -(gHamburger_500_678E30->FindOwnerForFollowCode_4748A0(5, pHam40->field_30_ped_to_follow) != 0);
                    LOBYTE(v217) = v217 & 0xFE;
                    pHam40->field_C = v217 + 2;
                    */
                    if (gHamburger_500_678E30->FindOwnerForFollowCode_4748A0(5, pHam40->field_30_ped_to_follow))
                    {
                        pHam40->field_C = 0;
                    }
                    else
                    {
                        pHam40->field_C = 2;
                    }
                }
                pHam40->field_2E = 0;
            }
        }
    }
    else
    {
        if (!dword_6779B0)
        {
            return;
        }

        dword_677A8C = dword_6779B0->sub_43A240();

        switch (pHam40->field_C)
        {
            case 0:
            case 2:
                if (pHam40->field_2A >= 10u)
                {
                    //dword_6779F0 = dword_6779F0;
                    //dword_6779F4 = dword_6779F4;

                    Ang16 v240 = dword_6779B0->field_50_car_sprite->field_0 - word_677ADE;

                    dword_6779F0 = (Ang16::sine_40F500(v240) * dword_677B98) + dword_6779F0;
                    dword_6779F4 = (Ang16::cosine_40F520(v240) * dword_677B98) + dword_6779F4;

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0,
                                                                      dword_6779F4,
                                                                      field_0->field_50_car_sprite->field_1C_zpos);

                    if (!field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                    {
                        Ped* v137 = gHamburger_500_678E30->FindOwnerForFollowCode_4748A0(5, pHam40->field_30_ped_to_follow);
                        if (!v137)
                        {
                            pHam40->field_2A = 100;
                            pHam40->field_2C = 0;
                            pHam40->field_C = 5;
                            return;
                        }

                        Hamburger_40* v138 = v137->field_16C_car->field_60;
                        if (v138->field_2C > 0xAu)
                        {
                            v138->field_C = 3;
                            v137->field_16C_car->field_60->field_2C = 0;
                            pHam40->field_2A = 100;
                            pHam40->field_2C = 0;
                            pHam40->field_C = 5;
                            return;
                        }
                    }
                }

                dword_677A8C = dword_6779B0->sub_43A240();
                if (v245 >= dword_677B94)
                {
                    if (dword_677B00 >= dword_677A8C + dword_677B5C)
                    {
                        CarPhysics_B0* v144 = this->field_0->field_58_physics;
                        v144->field_95 = 1;
                        v144->field_91_is_foot_brake_on = 0;
                        v144->field_94_is_backward_gas_on = 0;
                        v144->field_93_is_forward_gas_on = 0;
                    }
                    else
                    {
                        CarPhysics_B0* v143 = this->field_0->field_58_physics;
                        v143->field_93_is_forward_gas_on = 1;
                        v143->field_91_is_foot_brake_on = 0;
                        v143->field_94_is_backward_gas_on = 0;
                        v143->field_95 = 0;
                    }
                }
                else
                {
                    CarPhysics_B0* v142 = this->field_0->field_58_physics;
                    v142->field_91_is_foot_brake_on = 0;
                    v142->field_93_is_forward_gas_on = 0;
                    v142->field_94_is_backward_gas_on = 0;
                    v142->field_95 = 0;
                }

                if (pHam40->field_2A < 0xAu)
                {
                    pHam40->field_2A++;
                }
                return;

            case 3:
                if (pHam40->field_2C == 10)
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                    Ang16 v244 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;

                    Fix16 v237 = (Ang16::sine_40F500(v244) * dword_677B94);
                    Fix16 v241 = (Ang16::cosine_40F520(v244) * dword_677B94);

                    field_0->field_50_car_sprite->set_xyz_lazy_420600(v237 + field_0->field_50_car_sprite->field_14_xy.x,
                                                                      v241 + field_0->field_50_car_sprite->field_14_xy.y,
                                                                      field_0->field_50_car_sprite->field_1C_zpos);

                    if (!field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                    {
                        Sprite* NearestSpriteOfType_477E60 =
                            gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite, 0);
                        this->field_70 = NearestSpriteOfType_477E60;
                        if (!NearestSpriteOfType_477E60)
                        {
                            pHam40->field_C = 6;
                        }
                    }
                }
                else
                {
                    dword_677A8C = dword_6779B0->sub_43A240();
                    if (v245 >= dword_677B94)
                    {
                        if (dword_677B00 >= dword_677A8C)
                        {
                            CarPhysics_B0* v161 = this->field_0->field_58_physics;
                            v161->field_95 = 1;
                            v161->field_91_is_foot_brake_on = 0;
                            v161->field_94_is_backward_gas_on = 0;
                            v161->field_93_is_forward_gas_on = 0;
                        }
                        else
                        {
                            CarPhysics_B0* v160 = this->field_0->field_58_physics;
                            v160->field_93_is_forward_gas_on = 1;
                            v160->field_91_is_foot_brake_on = 0;
                            v160->field_94_is_backward_gas_on = 0;
                            v160->field_95 = 0;
                        }
                    }
                    else
                    {
                        CarPhysics_B0* v159 = this->field_0->field_58_physics;
                        v159->field_91_is_foot_brake_on = 0;
                        v159->field_93_is_forward_gas_on = 0;
                        v159->field_94_is_backward_gas_on = 0;
                        v159->field_95 = 0;
                    }

                    if (pHam40->field_2C < 0xAu)
                    {
                        pHam40->field_2C++;
                    }
                }
                return;

            case 4:
                if (pHam40->field_2C < 0xAu)
                {
                    dword_677A8C = dword_6779B0->sub_43A240();
                    if (v245 < dword_677B94)
                    {
                        field_0->field_58_physics->ClearDriverInputs_453F90();
                    }
                    else if (dword_677B00 < dword_677A8C)
                    {
                        field_0->field_58_physics->ForceForwardAcceleration_453F70();
                    }
                    else
                    {
                        field_0->field_58_physics->ForceNeutralInput_453F50();
                    }

                    if (pHam40->field_2C < 0xAu)
                    {
                        pHam40->field_2C++;
                    }
                }
                else
                {
                    field_0->field_50_car_sprite->set_xyz_lazy_451950(dword_6779F0, dword_6779F4, dword_677C48);

                    // Ang16::sub_4516F0 - inlined operator+
                    Ang16 v206 = dword_6779B0->field_50_car_sprite->field_0 + dword_6779E4;

                    //  Ang16::sub_451730 - inlined PolarToCartesian_41FC20 ?
                    Fix16 v237;
                    Ang16::PolarToCartesian_41FC20(v206, dword_677B94, v237, v241);

                    Fix16 v235 = field_0->field_50_car_sprite->field_1C_zpos;
                    Fix16 v218 = (field_0->field_50_car_sprite->field_14_xy.y + v241);
                    Fix16 v208 = (field_0->field_50_car_sprite->field_14_xy.x + v237);
                    field_0->field_50_car_sprite->set_xyz_lazy_451950(v208, v218, v235);
                    if (!field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                    {
                        this->field_70 = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite, 0);
                        if (!this->field_70)
                        {
                            pHam40->field_C = 7;
                        }
                    }
                }
                return;

            case 5:
            {
                s32 v162;
                Fix16 v237;
                Ang16 v253;

                switch (gHamburger_500_678E30->CountFollowers_474920(pHam40->field_30_ped_to_follow, this->field_0->field_54_driver))
                {
                    case 1:
                        v243 = 1;
                        v242 = 1;
                        v162 = gPolice_7B8_6FEE40->field_654_wanted_level != 1 ? 80 : 400;
                        goto LABEL_250;

                    case 2:
                        if (gHamburger_500_678E30->HasRelationshipCode_13_15_4749B0(this->field_0->field_54_driver))
                        {
                            v162 = 10;
                            pHam40->field_2C = 0;
                            v243 = 0;
                            v242 = 0;
                        }
                        else
                        {
                            v162 = 80;
                            v243 = 1;
                            v242 = 1;
                        }
                        goto LABEL_250;

                    case 3:
                    case 4:
                        v243 = 1;
                        v242 = 1;
                        if (gHamburger_500_678E30->HasRelationshipCode_6_7_8_9_13_474BC0(field_0->field_54_driver))
                        {
                            if (gHamburger_500_678E30->HasRelationshipCode_6_8_10_474A80(this->field_0->field_54_driver))
                            {
                                v242 = 0;
                                if (gHamburger_500_678E30->HasRelationshipCode_7_9_11_474B50(field_0->field_54_driver))
                                {
                                    pHam40->field_2C = 0;
                                    v243 = 0;
                                    v242 = 0;
                                }
                                goto LABEL_249;
                            }

                            if (gHamburger_500_678E30->HasRelationshipCode_7_9_11_474B50(this->field_0->field_54_driver))
                            {
                                v243 = 0;
                            LABEL_249:
                                v162 = v270;
                                goto LABEL_250;
                            }
                            v162 = 10;
                            pHam40->field_2C = 0;
                        }
                        else
                        {
                            v162 = 80;
                        }

                    LABEL_250:
                        if (pHam40->field_8_maybe_path_type == 2)
                        {
                            v162 = 100;
                            pHam40->field_2C = 0;
                        }

                        sub_4476F0();

                        if (pHam40->field_2C < v162)
                        {
                            dword_677A8C = dword_6779B0->sub_43A240();
                            if (this->field_74 < dword_677A8C)
                            {
                                dword_677A8C = this->field_74;
                            }

                            if (v245 >= dword_677B94)
                            {
                                if (dword_677B00 >= dword_677A8C)
                                {
                                    if (dword_677B00 <= dword_677A8C + dword_677B5C)
                                    {
                                        field_0->field_58_physics->ForceNeutralInput_453F50();
                                    }
                                    else
                                    {
                                        field_0->sub_43A950();
                                    }
                                }
                                else
                                {
                                    CarPhysics_B0* v205 = this->field_0->field_58_physics;
                                    v205->field_93_is_forward_gas_on = 1;
                                    v205->field_91_is_foot_brake_on = 0;
                                    v205->field_94_is_backward_gas_on = 0;
                                    v205->field_95 = 0;
                                }
                                if (pHam40->field_2C < v162)
                                {
                                    ++pHam40->field_2C;
                                }
                                pHam40->field_2A = 100;
                            }
                            else
                            {
                                if (dword_677B00 > dword_677A8C + dword_677B5C)
                                {
                                    //goto LABEL_292;
                                    field_0->sub_43A950();
                                    return;
                                }
                                CarPhysics_B0* v204 = this->field_0->field_58_physics;
                                v204->field_91_is_foot_brake_on = 0;
                                v204->field_93_is_forward_gas_on = 0;
                                v204->field_94_is_backward_gas_on = 0;
                                v204->field_95 = 0;
                            }
                        }
                        else
                        {
                            if (!v242)
                            {
                                goto LABEL_273;
                            }

                            field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                            v253 = dword_6779B0->field_50_car_sprite->field_0 - dword_6779E4;
                            v237 = (Ang16::sine_40F500(v253) * dword_677B94);
                            v241 = (Ang16::cosine_40F520(v253) * dword_677B94);

                            field_0->field_50_car_sprite->set_xyz_lazy_420600(v237 + field_0->field_50_car_sprite->field_14_xy.x,
                                                                              v241 + field_0->field_50_car_sprite->field_14_xy.y,
                                                                              field_0->field_50_car_sprite->field_1C_zpos);

                            if (field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                            {
                                goto LABEL_273;
                            }

                            field_70 = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite,
                                                                                            sprite_types_enum::unknown_0);
                            if (!field_70)
                            {
                                v237 = (Ang16::sine_40F500(field_0->field_50_car_sprite->field_0) * dword_677B94);
                                v241 = (Ang16::cosine_40F520(field_0->field_50_car_sprite->field_0) * dword_677B94);

                                field_0->field_50_car_sprite->set_xyz_lazy_420600(v237 + field_0->field_50_car_sprite->field_14_xy.x,
                                                                                  v241 + field_0->field_50_car_sprite->field_14_xy.y,
                                                                                  field_0->field_50_car_sprite->field_1C_zpos);

                                field_70 = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite, 0);
                            }

                            if (field_70)
                            {
                            LABEL_273:
                                if (v243)
                                {
                                    field_0->field_50_car_sprite->set_xyz_lazy_420600(dword_6779F0, dword_6779F4, dword_677C48);

                                    Ang16 v254 = dword_6779E4 + dword_6779B0->field_50_car_sprite->field_0;
                                    Fix16 v237 = (Ang16::sine_40F500(v254) * dword_677B94);
                                    Fix16 v241 = (Ang16::cosine_40F520(v254) * dword_677B94);
                                    field_0->field_50_car_sprite->set_xyz_lazy_420600(v237 + field_0->field_50_car_sprite->field_14_xy.x,
                                                                                      v241 + field_0->field_50_car_sprite->field_14_xy.y,
                                                                                      field_0->field_50_car_sprite->field_1C_zpos);

                                    if (!field_0->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                                    {
                                        this->field_70 =
                                            gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite, 0);
                                        if (!this->field_70)
                                        {
                                            Fix16 v237 = (Ang16::sine_40F500(field_0->field_50_car_sprite->field_0) * dword_677B94);
                                            Fix16 v196 = (Ang16::cosine_40F520(field_0->field_50_car_sprite->field_0) * dword_677B94);

                                            field_0->field_50_car_sprite->set_xyz_lazy_420600(
                                                v237 + field_0->field_50_car_sprite->field_14_xy.x,
                                                v196 + field_0->field_50_car_sprite->field_14_xy.y,
                                                field_0->field_50_car_sprite->field_1C_zpos);

                                            this->field_70 =
                                                gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_0->field_50_car_sprite, 0);
                                            if (!this->field_70)
                                            {
                                                pHam40->field_C = 7;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                pHam40->field_C = 6;
                            }
                        }
                        break;

                    default:
                        v162 = 10;
                        goto LABEL_250;
                }
                return;
            }

            case 6:
                if (!(v245 < (dword_677A84 + dword_677CBC)))
                {
                    //goto LABEL_324;
                    field_0->field_58_physics->ForceForwardAcceleration_453F70();
                }
                else
                {
                    pHam40->field_C = 8;
                }
                return;

            case 7:
                if (!(v245 < (dword_677A84 + dword_677CBC)))
                {
                    //goto LABEL_324;
                    field_0->field_58_physics->ForceForwardAcceleration_453F70();
                }
                else
                {
                    pHam40->field_C = 9;
                }
                return;

            case 8:
            case 9:
                dword_677A8C = dword_6779B0->sub_43A240();
                if (v245 < dword_6779BC)
                {
                    if (dword_677B00 > dword_677A8C)
                    {
                        //LABEL_292:
                        field_0->sub_43A950();
                        return;
                    }
                    //goto LABEL_320;
                    field_0->field_58_physics->ClearDriverInputs_453F90();
                    return;
                }

                if (v245 > dword_677A4C || dword_677B00 < dword_677A8C)
                {
                    //LABEL_324:
                    field_0->field_58_physics->ForceForwardAcceleration_453F70();
                }
                else
                {
                    if (dword_677B00 > (dword_677A8C + dword_677B5C))
                    {
                        //LABEL_320:
                        field_0->field_58_physics->ClearDriverInputs_453F90();
                        return;
                    }
                    field_0->field_58_physics->ForceNeutralInput_453F50();
                }
                break;

            case 10:
            case 11:
                if (v245 < dword_6779BC)
                {
                    pHam40->field_C = 12;
                }
                return;

            case 12:
            case 13:
                if (v245 < (dword_677B94 + dword_677A84))
                {
                    ++pHam40->field_3C;
                    field_0->field_58_physics->ClearDriverInputs_453F90();
                    pHam40->field_C = 13;
                }
                return;
            default:
                return;
        }
    }
}

STUB_FUNC(0x451980)
void CarAI_78::sub_451980()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x451fa0)
void CarAI_78::sub_451FA0()
{
    if (!byte_677BBC)
    {
        Object_2C* pObj = field_70->As2C_40FEC0();
        if (pObj->field_18_model == 21 || pObj->field_18_model == 122)
        {
            field_0->sub_43A970();
            byte_677B3C = 0;
            this->field_24_bf.b4 = 1;
        }
    }
}

MATCH_FUNC(0x451ff0)
void CarAI_78::sub_451FF0()
{
    if (!field_24_bf.b21)
    {
        if (!byte_677BBC || (field_0->field_60->field_8_maybe_path_type != 5) ||
            field_0->field_60->field_30_ped_to_follow != field_70->field_8_char_b4_ptr->field_7C_pPed)
        {
            if (dword_677B00 > dword_6779C8)
            {
                field_0->sub_43A970();
                byte_677B3C = 0;
                field_0->sub_4416D0(2);
                return;
            }
            field_0->sub_43A950();
            byte_677B3C = 0;
        }
    }
    field_0->sub_4416D0(2);
}

STUB_FUNC(0x452060)
char_type CarAI_78::sub_452060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452a20)
s16 CarAI_78::sub_452A20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452df0)
s16 CarAI_78::sub_452DF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x453470)
void CarAI_78::sub_453470()
{
    this->field_0->field_58_physics->field_92_is_hand_brake_on = 0;
    Fix16 t = field_0->field_58_physics->field_40_linvel_1.GetLength_453590();
    dword_677B00 = t;

    if (this->field_0->field_80)
    {
        this->field_0->field_A6 |= 0x20u;
    }

    if (this->field_30 > 0)
    {
        this->field_30--;

        if (this->field_30)
        {
            this->field_0->field_A6 |= 0x20;
        }
        else
        {
            this->field_0->field_A6 &= ~0x20;
        }
    }

    if ((this->field_0->field_A6 & 0x20) == 0x20)
    {
        if (this->field_24_bf.b20)
        {
            if (dword_677B00 > dword_677B60)
            {
                field_0->sub_43A970();
            }
            else
            {
                field_0->sub_43A950();
            }
            this->field_68 = 0;
            this->field_24_bf.b12 = 0;
        }
        else
        {
            if (dword_677B00 > dword_677B60)
            {
                field_0->sub_43A970(); // gas on
            }
            else
            {
                CarPhysics_B0* pPhysics = field_0->field_58_physics;
                pPhysics->field_91_is_foot_brake_on = 0;
                pPhysics->field_93_is_forward_gas_on = 0;
                pPhysics->field_94_is_backward_gas_on = 0;
                pPhysics->field_95 = 0;
            }
            this->field_68 = 0;
            this->field_24_bf.b12 = 0;
        }
    }
    else
    {
        sub_452DF0();
    }
}

MATCH_FUNC(0x4537d0)
void CarAI_78::sub_4537D0()
{
    if ((this->field_0->field_A6 & 2) != 2 && (this->field_0->field_A6 & 1) != 1)
    {
        if (stru_6F6784.get_int_4F7AE0(2) > 0)
        {
            field_0->field_A6 |= 2;
        }
        else
        {
            field_0->field_A6 |= 1;
        }
    }
}

MATCH_FUNC(0x4538b0)
void CarAI_78::sub_4538B0()
{
    if (this->field_24_flags & 0x80)
    {
        this->field_0->field_A6 &= ~2u;
        this->field_0->field_A6 &= ~1u;
    }
}

MATCH_FUNC(0x453990)
void CarAI_78::sub_453990(Fix16 a2)
{
    if (this->field_14 < a2)
    {
        this->field_14 = a2;
    }
}

MATCH_FUNC(0x4539b0)
void CarAI_78::sub_4539B0()
{
    this->field_0->field_A6 &= ~4u;
    this->field_0->field_A6 &= ~8u;
}

MATCH_FUNC(0x4539d0)
void CarAI_78::sub_4539D0()
{
    Fix16 f14 = this->field_14;
    Fix16 f18 = this->field_18;
    if (f14 < f18)
    {
        this->field_14 = field_1C + f14;
        if (field_1C < dword_677B90)
        {
            sub_453990(this->field_18);
        }
    }
    else
    {
        if (f14 > f18)
        {
            this->field_14 = f14 - ((dword_677BA0 * dword_677AC4));
            sub_453990(field_18);
        }
    }

    CarPhysics_B0* pPhysics = this->field_0->field_58_physics;
    pPhysics->field_91_is_foot_brake_on = 0;
    pPhysics->field_93_is_forward_gas_on = 0;
    pPhysics->field_94_is_backward_gas_on = 0;
    pPhysics->field_95 = 0;
}

STUB_FUNC(0x453a40)
s32 CarAI_78::sub_453A40()
{
    NOT_IMPLEMENTED;
    return 0;
}

// TODO: Move
MATCH_FUNC(0x447650)
void __stdcall sub_447650()
{
    byte_677A78 = 0;
    byte_677C90 = 0;
    byte_677A5D = 1;
    byte_677B3C = 1;
    byte_677BBC = 0;
    byte_677A6C = 1;
    byte_677A94 = 0;
    byte_677A5C = 0;
    byte_677B8C = 1;
    byte_677CA8 = 0;
    byte_677C06 = 0;
    dword_677C38 = dword_677B90;
    dword_677C30 = dword_677B90;
    dword_677C48 = dword_677B90;
    dword_677A74 = dword_677B90;
    dword_677A80 = dword_677B90;
    dword_677B00 = dword_677B90;
    dword_677C9C = dword_677B90;
    dword_677A8C = dword_677B90;
    dword_6779F0 = dword_677B90;
    dword_6779F4 = dword_677B90;
    dword_6779F8 = dword_677B90;
    dword_677C88 = 1;
    dword_6779B0 = 0;
}

MATCH_FUNC(0x453bb0)
void CarAI_78::sub_453BB0()
{
    sub_447650();
    this->field_10 = this->field_0->field_50_car_sprite->field_0;
    this->field_4C = Ang16::GetAngleFace_4F78F0(field_10);
    if (field_2B < 0xff)
    {
        field_2B++;
    }
    sub_453470();
}

MATCH_FUNC(0x453bf0)
void CarAI_78::SetCar_453BF0(Car_BC* a2)
{
    field_0 = a2;
}

STUB_FUNC(0x453c00)
u8* CarAI_78::sub_453C00()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x453d80)
void CarAI_78::PoolAllocate()
{
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_9 = 0;
    this->field_A = 0;
    this->field_10 = word_677CE8;
    this->field_14 = dword_677B90;
    this->field_18 = dword_677CB4;
    this->field_1C = dword_6779A4;
    this->field_20 = 0;
    this->field_56 = 0;
    this->field_28_junc_idx = -1;
    this->field_29 = 0;
    this->field_58 = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D = 0;
    this->field_2E = 0;
    this->field_2F = 0;
    this->field_5C = dword_677B90;
    this->field_60 = dword_677B90;
    this->field_64 = dword_677B90;
    this->field_68 = 0;
    this->field_6C = 0;
    this->field_34 = 0;
    this->field_38 = 0;
    this->field_3C = 0;
    this->field_40 = 0;
    this->field_44 = 0;
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_70 = 0;
    this->field_74 = dword_6779D4;
    this->field_54 = 0;
    this->field_2A = 0;
    this->field_2B = 0;
    this->field_2C = 0;
    this->field_24_flags &= ~0x3C0000u;
    this->field_50 = 0;
    this->field_5A = 0;
    this->field_30 = 0;
}

MATCH_FUNC(0x453cb0)
CarAI_78::CarAI_78()
{
    this->field_10 = 0;
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_9 = 0;
    this->field_A = 0;
    this->mpNext = 0;
    this->field_10 = word_677CE8;
    this->field_14 = dword_677B90;
    this->field_18 = dword_677CB4;
    this->field_1C = dword_6779A4;
    this->field_20 = 0;
    this->field_56 = 0;
    this->field_28_junc_idx = -1;
    this->field_29 = 0;
    this->field_58 = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D = 0;
    this->field_2E = 0;
    this->field_2F = 0;
    this->field_5C = dword_677B90;
    this->field_60 = dword_677B90;
    this->field_64 = dword_677B90;
    this->field_68 = 0;
    this->field_6C = 0;
    this->field_34 = 0;
    this->field_38 = 0;
    this->field_3C = 0;
    this->field_40 = 0;
    this->field_44 = 0;
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_70 = 0;
    this->field_74 = dword_677B90;
    this->field_54 = 0;
    this->field_2A = 0;
    this->field_2B = 0;
    this->field_2C = 0;
    this->field_5A = 0;
    this->field_50 = 0;
    this->field_24_flags &= ~0x3C0000u;
}
