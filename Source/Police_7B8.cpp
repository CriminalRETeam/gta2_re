#include "Police_7B8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Kfc_1E0.hpp"
#include "Object_5C.hpp"
#include "Game_0x40.hpp"
#include "Player.hpp"
#include "Ped.hpp"

EXPORT_VAR s32 dword_6FECE8;
GLOBAL(dword_6FECE8, 0x6FECE8);

EXPORT_VAR Fix16 dword_6FED54;
GLOBAL(dword_6FED54, 0x6FED54);

EXPORT_VAR Ped* pPed_6FEDDC;
GLOBAL(pPed_6FEDDC, 0x6FEDDC);

MATCH_FUNC(0x4beb30)
Police_38::Police_38()
{
    sub_5709C0();
}

MATCH_FUNC(0x4beb40)
Police_38::~Police_38()
{
}

STUB_FUNC(0x570790)
char_type Police_38::sub_570790(Police_38* a1, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5709c0)
void Police_38::sub_5709C0()
{
    field_2_targ_x = 0;
    field_3_targ_y = 0;
    field_4_targ_z = 0;
    field_18 = 0;
    field_1C = 0;
    field_1A = 0;
    field_10_subObj = 0;
    field_24_state = 0;
    field_28 = 0;
    field_8 = dword_6FECE8;
    field_C = dword_6FECE8;
    field_14_pObj = 0;
    field_1A = 0;
    field_29 = 0;
    field_2A = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
}

STUB_FUNC(0x570a10)
void Police_38::sub_570A10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570ab0)
void Police_38::sub_570AB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570bf0)
Kfc_30* Police_38::sub_570BF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x570e30)
s32 Police_38::sub_570E30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571150)
s32 Police_38::sub_571150()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571350)
u32* Police_38::sub_571350()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571540)
char_type Police_38::sub_571540()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571a30)
char_type Police_38::sub_571A30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5720c0)
char_type Police_38::sub_5720C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572210)
bool Police_38::sub_572210()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572340)
char_type Police_38::sub_572340()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572920)
void Police_38::sub_572920()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x574720)
void Police_38::sub_574720()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x574f10)
char_type Police_38::sub_574F10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x575200)
void Police_38::sub_575200()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575210)
void Police_38::sub_575210()
{
    NOT_IMPLEMENTED;
    Kfc_30* pKfc30 = this->field_10_subObj;
    if (!pKfc30->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(0, 9999);
    }
    else
    {
        pKfc30->field_0->field_A6 |= 0x20u;
    }
}

STUB_FUNC(0x575270)
void Police_38::sub_575270()
{
    NOT_IMPLEMENTED;
    if (!this->field_10_subObj->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(0, 9999);
    }
}

STUB_FUNC(0x5752c0)
void Police_38::sub_5752C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575310)
void Police_38::sub_575310()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575590)
void Police_38::sub_575590()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575650)
s32 Police_38::sub_575650()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x575710)
void Police_A4::sub_575710()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_9 = 0;
    field_A = 0;
    field_C = 0;
    field_10 = 0;
    field_14 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 0;
    field_50 = 0;
    field_54 = 0;
    field_58 = 0;
    field_5C = 0;
    field_60 = 0;
    field_64 = 0;
    field_68 = 0;
    field_6C = 0;
    field_70 = 0;
    field_74 = 0;
    field_78 = 0;
    field_7C = 0;
    field_80 = 0;
    field_84 = 0;
    field_88 = 0;
    field_8C = 0;
    field_90 = 0;
    field_94 = 0;
    field_98 = 0;
    field_9C = 0;
}

MATCH_FUNC(0x5757b0)
void Police_A4::sub_5757B0()
{
    u8 v31 = 1;

    if (field_0)
    {
        if (field_C > 0)
        {
            field_C--;
        }
        if (!field_C)
        {
            if (field_10)
            {
                if (field_10->field_7C_uni_num != 4)
                {
                    field_10 = 0;
                }
                else
                {
                    if (!field_10->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_14)
            {
                if (field_14->field_7C_uni_num != 4)
                {
                    field_14 = 0;
                }
                else
                {
                    if (!field_14->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_18)
            {
                if (field_18->field_7C_uni_num != 4)
                {
                    field_18 = 0;
                }
                else
                {
                    if (!field_18->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_1C)
            {
                if (field_1C->field_7C_uni_num != 4)
                {
                    field_1C = 0;
                }
                else
                {
                    if (!field_1C->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_20)
            {
                if (field_20->field_7C_uni_num != 4)
                {
                    field_20 = 0;
                }
                else
                {
                    if (!field_20->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_24)
            {
                if (field_24->field_7C_uni_num != 4)
                {
                    field_24 = 0;
                }
                else
                {
                    if (!field_24->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_28)
            {
                if (field_28->field_14 == field_58)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_28->field_4->field_14_xpos, field_28->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_2C)
            {
                if (field_2C->field_14 == field_5C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_2C->field_4->field_14_xpos, field_2C->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_30)
            {
                if (field_30->field_14 == field_60)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_30->field_4->field_14_xpos, field_30->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_34)
            {
                if (field_34->field_14 == field_64)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_34->field_4->field_14_xpos, field_34->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_38)
            {
                if (field_38->field_14 == field_68)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_38->field_4->field_14_xpos, field_38->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_3C)
            {
                if (field_3C->field_14 == field_6C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_3C->field_4->field_14_xpos, field_3C->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_40)
            {
                if (field_40->field_14 == field_70)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_40->field_4->field_14_xpos, field_40->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_44)
            {
                if (field_44->field_14 == field_74)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_44->field_4->field_14_xpos, field_44->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_48)
            {
                if (field_48->field_14 == field_78)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_48->field_4->field_14_xpos, field_48->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_4C)
            {
                if (field_4C->field_14 == field_7C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_4C->field_4->field_14_xpos, field_4C->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_50)
            {
                if (field_50->field_14 == field_80)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_50->field_4->field_14_xpos, field_50->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_54)
            {
                if (field_54->field_14 == field_84)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_54->field_4->field_14_xpos, field_54->field_4->field_18_ypos))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_88)
            {
                if ((field_88->field_21C & 1) == 0)
                {
                    field_88 = 0;
                }
                else
                {
                    if (field_88->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_8C)
            {
                if ((field_8C->field_21C & 1) == 0)
                {
                    field_8C = 0;
                }
                else
                {
                    if (field_8C->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_90)
            {
                if ((field_90->field_21C & 1) == 0)
                {
                    field_90 = 0;
                }
                else
                {
                    if (field_90->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_94)
            {
                if ((field_94->field_21C & 1) == 0)
                {
                    field_94 = 0;
                }
                else
                {
                    if (field_94->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_98)
            {
                if ((field_98->field_21C & 1) == 0)
                {
                    field_98 = 0;
                }
                else
                {
                    if (field_98->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_9C)
            {
                if ((field_9C->field_21C & 1) == 0)
                {
                    field_9C = 0;
                }
                else
                {
                    if (field_9C->field_20e < 0x50u)
                    {
                        field_E = 0;
                        return;
                    }
                }
            }
            if (v31)
            {
                if (++field_E <= 0xC8u)
                {
                    return;
                }
                Ped* field_2C4_player_ped = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;

                Fix16 fix_y = field_2C4_player_ped->field_1AC_cam.y;
                Fix16 fix_x = field_2C4_player_ped->field_1AC_cam.x;

                Fix16 v29 = Fix16(field_8) - fix_x;
                Fix16 v30 = Fix16(field_9) - fix_y;

                v30.inline_abs_403840(v30);
                v29.inline_abs_403840(v29);

                if (v29 <= v30)
                {
                    v29 = v30;
                }
                if (v29 > dword_6FED54)
                {
                    Police_A4::sub_575CA0();
                    return;
                }
            }
            field_E = 0;
        }
    }
}

MATCH_FUNC(0x575ca0)
void Police_A4::sub_575CA0()
{
    if (field_10)
    {
        s32 v3 = field_10->field_88;
        if (v3 != 5 && v3 != 2 && v3 != 3)
        {
            field_10->field_88 = 4;
        }
        Car_BC* v4 = field_10;
        v4->field_7C_uni_num = 3;
        v4->field_76 = 0;
        field_10 = 0;
    }

    if (field_14)
    {
        s32 v6 = field_14->field_88;
        if (v6 != 5 && v6 != 2 && v6 != 3)
        {
            field_14->field_88 = 4;
        }
        Car_BC* v7 = field_14;
        v7->field_7C_uni_num = 3;
        v7->field_76 = 0;
        field_14 = 0;
    }
    if (field_18)
    {
        s32 v9 = field_18->field_88;
        if (v9 != 5 && v9 != 2 && v9 != 3)
        {
            field_18->field_88 = 4;
        }
        Car_BC* v10 = field_18;
        v10->field_7C_uni_num = 3;
        v10->field_76 = 0;
        field_18 = 0;
    }

    if (field_1C)
    {
        s32 v12 = field_1C->field_88;
        if (v12 != 5 && v12 != 2 && v12 != 3)
        {
            field_1C->field_88 = 4;
        }
        Car_BC* v13 = field_1C;
        v13->field_7C_uni_num = 3;
        v13->field_76 = 0;
        field_1C = 0;
    }

    if (field_20)
    {
        s32 v15 = field_20->field_88;
        if (v15 != 5 && v15 != 2 && v15 != 3)
        {
            field_20->field_88 = 4;
        }
        Car_BC* v16 = field_20;
        v16->field_7C_uni_num = 3;
        v16->field_76 = 0;
        field_20 = 0;
    }

    if (field_24)
    {
        s32 v18 = field_24->field_88;
        if (v18 != 5 && v18 != 2 && v18 != 3)
        {
            field_24->field_88 = 4;
        }
        Car_BC* v19 = field_24;
        v19->field_7C_uni_num = 3;
        v19->field_76 = 0;
        field_24 = 0;
    }

    if (field_28)
    {
        if (field_28->field_14 == field_58)
        {
            field_28->sub_5291B0();
        }
        field_28 = 0;
    }

    if (field_2C)
    {
        if (field_2C->field_14 == field_5C)
        {
            field_2C->sub_5291B0();
        }
        field_2C = 0;
    }

    if (field_30)
    {
        if (field_30->field_14 == field_60)
        {
            field_30->sub_5291B0();
        }
        field_30 = 0;
    }

    if (field_34)
    {
        if (field_34->field_14 == field_64)
        {
            field_34->sub_5291B0();
        }
        field_34 = 0;
    }

    if (field_38)
    {
        if (field_38->field_14 == field_68)
        {
            field_38->sub_5291B0();
        }
        field_38 = 0;
    }

    if (field_3C)
    {
        if (field_3C->field_14 == field_6C)
        {
            field_3C->sub_5291B0();
        }
        field_3C = 0;
    }

    if (field_40)
    {
        if (field_40->field_14 == field_70)
        {
            field_40->sub_5291B0();
        }
        field_40 = 0;
    }

    if (field_44)
    {
        if (field_44->field_14 == field_74)
        {
            field_44->sub_5291B0();
        }
        field_44 = 0;
    }

    if (field_48)
    {
        if (field_48->field_14 == field_78)
        {
            field_48->sub_5291B0();
        }
        field_48 = 0;
    }

    if (field_4C)
    {
        if (field_4C->field_14 == field_7C)
        {
            field_4C->sub_5291B0();
        }
        field_4C = 0;
    }

    if (field_50)
    {
        if (field_50->field_14 == field_80)
        {
            field_50->sub_5291B0();
        }
        field_50 = 0;
    }

    if (field_54)
    {
        if (field_54->field_14 == field_84)
        {
            field_54->sub_5291B0();
        }
        field_54 = 0;
    }

    if (field_88)
    {
        if (field_88->field_20e)
        {
            field_88->sub_45EB60();
        }
        else
        {
            field_88->field_238 = 3;
        }
        field_88 = 0;
    }

    if (field_8C)
    {
        if (field_8C->field_20e)
        {
            field_8C->sub_45EB60();
        }
        else
        {
            field_8C->field_238 = 3;
        }
        field_8C = 0;
    }

    if (field_90)
    {
        if (field_90->field_20e)
        {
            field_90->sub_45EB60();
        }
        else
        {
            field_90->field_238 = 3;
        }
        field_90 = 0;
    }

    if (field_94)
    {
        if (field_94->field_20e)
        {
            field_94->sub_45EB60();
        }
        else
        {
            field_94->field_238 = 3;
        }
        field_94 = 0;
    }

    if (field_98)
    {
        if (field_98->field_20e)
        {
            field_98->sub_45EB60();
        }
        else
        {
            field_98->field_238 = 3;
        }
        field_98 = 0;
    }

    if (field_9C)
    {
        if (field_9C->field_20e)
        {
            field_9C->sub_45EB60();
            field_9C = 0;
            field_0 = 0;
            return;
        }
        field_9C->field_238 = 3;
        field_9C = 0;
    }
    field_0 = 0;
}

STUB_FUNC(0x575ff0)
char_type Police_A4::sub_575FF0(u8 a2, s32 a3, u8 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x577480)
Police_A4::Police_A4()
{
    sub_575710();
    field_A0_pMem = new u8[0x18u]; // TODO: A class/struct?
}

MATCH_FUNC(0x5774a0)
Police_A4::~Police_A4()
{
    delete[] field_A0_pMem;
}

// https://decomp.me/scratch/McaDT
// TODO: Missing SEH for unknown reasons, SEH present on decomp.me
STUB_FUNC(0x4BEB50)
Police_7B8::~Police_7B8()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x56f400)
s32 Police_7B8::sub_56F400()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f4d0)
char_type Police_7B8::sub_56F4D0(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f560)
Police_38* Police_7B8::sub_56F560()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f5c0)
Ped* Police_7B8::sub_56F5C0(s32 a2, s32 a3, s16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f6d0)
void Police_7B8::sub_56F6D0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x56f800)
bool Police_7B8::sub_56F800(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f880)
bool Police_7B8::sub_56F880(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f8e0)
s32 Police_7B8::sub_56F8E0(Ped* a2, Ped* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56f940)
s32 Police_7B8::sub_56F940(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56fa40)
s16 Police_7B8::sub_56FA40()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56faa0)
char_type Police_7B8::sub_56FAA0(Police_7C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56fbd0)
void Police_7B8::sub_56FBD0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570270)
void Police_7B8::sub_570270()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570320)
s32 Police_7B8::sub_570320(Ped* a2, s32 a3, s32 a4, s32 a5, s16 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5703e0)
char_type Police_7B8::sub_5703E0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5707b0)
char_type Police_7B8::sub_5707B0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5708c0)
s32 Police_7B8::sub_5708C0(Police_7B8* a1, s32 a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x570940)
void Police_7B8::sub_570940(Ped* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x577320)
char_type Police_7B8::sub_577320()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x577370)
void Police_7B8::sub_577370(u8 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}
