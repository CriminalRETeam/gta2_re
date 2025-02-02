#include "Garox_2B00.hpp"
#include "Car_BC.hpp"
#include "Frontend.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "Zones_CA8.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "lucid_hamilton.hpp"
#include "registry.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"

EXPORT_VAR Garox_2B00* gGarox_2B00_706620;
GLOBAL(gGarox_2B00_706620, 0x706620);

EXPORT_VAR s16 word_706600; // TODO
GLOBAL(word_706600, 0x706600);

EXPORT_VAR s16 word_7064B8; // TODO
GLOBAL(word_7064B8, 0x7064B8);

EXPORT_VAR s16 word_706618; // TODO
GLOBAL(word_706618, 0x706618);

EXPORT_VAR s16 word_706508; // TODO
GLOBAL(word_706508, 0x706508);

EXPORT_VAR char byte_67CE50[264]; // TODO
GLOBAL(byte_67CE50, 0x67CE50);

EXPORT_VAR s16 word_7064D8;
GLOBAL(word_7064D8, 0x7064D8);

// TODO
EXPORT_VAR extern wchar_t tmpBuff_67BD9C[640];

// TODO
EXPORT_VAR extern char_type gLighting_626A09;

// TODO: move
EXPORT_VAR extern s32 bStartNetworkGame_7081F0;
EXPORT_VAR extern wchar_t word_67DC8C[50];

STUB_FUNC(0x5cfe40)
void Garox_13C0_sub::sub_5CFE40()
{
}

// ----------------------------------------------------

MATCH_FUNC(0x5d56b0)
void Garox_1_v2::sub_5D56B0()
{
    if (this->field_0_timer)
    {
        this->field_CC = Frontend::sub_5D8990(field_2_str, word_7064D8);
    }
}

STUB_FUNC(0x5d56d0)
void Garox_1_v2::sub_5D56D0()
{
}

MATCH_FUNC(0x5d5730)
void Garox_1_v2::sub_5D5730(const wchar_t* pStr)
{
    this->field_0_timer = 120;
    wcscpy(this->field_2_str, pStr);
    sub_5D56B0();
}

MATCH_FUNC(0x5d5760)
void Garox_1_v2::sub_5D5760()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

STUB_FUNC(0x5d5770)
void Garox_1_v2::sub_5D5770(u8* a2, u8* a3)
{
}

MATCH_FUNC(0x5d58f0)
Garox_1_v2::Garox_1_v2()
{
    field_0_timer = 0;
}

// ----------------------------------------------------

STUB_FUNC(0x5d15e0)
char_type Garox_2A25_sub::sub_5D15E0(s32 a1, char_type* a2)
{
    return 0;
}

STUB_FUNC(0x5d16b0)
void Garox_2A25_sub::sub_5D16B0()
{
}

STUB_FUNC(0x5d17d0)
s32 Garox_2A25_sub::sub_5D17D0(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x5d1830)
void Garox_2A25_sub::sub_5D1830(Player* pPlayer)
{
    pPlayer->field_794 = 1;
    pPlayer->field_838_f796_idx = 0;
    pPlayer->field_796[0] = 0;
}

// ----------------------------------------------------

STUB_FUNC(0x5d13c0)
char_type Garox_12EC_sub::sub_5D13C0(s32 a2, char_type* a3)
{
    return 0;
}

STUB_FUNC(0x5d1430)
void Garox_12EC_sub::sub_5D1430()
{
}

MATCH_FUNC(0x5d15a0)
s32 Garox_12EC_sub::sub_5D15A0(s32 a1)
{
    return gGame_0x40_67E008->field_38_orf1->field_78A && (a1 == 28 || a1 == 1);
}

MATCH_FUNC(0x5d15d0)
void Garox_12EC_sub::sub_5D15D0(Player* pPlayer)
{
    pPlayer->field_78A = 1;
}

// ----------------------------------------------------

MATCH_FUNC(0x5cf620)
void Garox_4::sub_5CF620()
{
    do
    {
        field_0_value++;
        if (field_0_value > 9999)
        {
            field_0_value = 0;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->sub_5B5FA0(gTmpBuffer_67C598));
    gGarox_2B00_706620->field_DC.sub_5D4400(3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gGarox_2B00_706620->field_111C.sub_5D1A00(tmpBuff_67BD9C, 3);
}

MATCH_FUNC(0x5cf6b0)
void Garox_4::sub_5CF6B0()
{
    do
    {
        field_0_value--;
        if (field_0_value < 0)
        {
            field_0_value = 9999;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->sub_5B5FA0(gTmpBuffer_67C598));
    gGarox_2B00_706620->field_DC.sub_5D4400(3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gGarox_2B00_706620->field_111C.sub_5D1A00(tmpBuff_67BD9C, 3);
}

// ----------------------------------------------------

STUB_FUNC(0x5d63b0)
void Garox_12E4_sub::sub_5D63B0()
{
}

// ----------------------------------------------------

STUB_FUNC(0x5d1850)
void Garox_1C8::sub_5D1850()
{
}

STUB_FUNC(0x5d1860)
void Garox_1C8::sub_5D1860()
{
}

STUB_FUNC(0x5d1940)
void Garox_1C8::sub_5D1940()
{
}

MATCH_FUNC(0x5d1a00)
void Garox_1C8::sub_5D1A00(wchar_t* pStr, s32 a3)
{
    if (field_0 <= 0 || a3 >= field_1C4)
    {
        field_1C4 = a3;
        wcscpy(field_2_str, pStr);
        gText_0x14_704DFC->sub_5B5B80(field_2_str);
        field_0 = 90;
        sub_5D1860();
    }
}

MATCH_FUNC(0x5d1ab0)
void Garox_1C8::sub_5D1AB0()
{
    if (field_0)
    {
        field_0--;
    }
}

MATCH_FUNC(0x5d1ae0)
Garox_1C8::Garox_1C8()
{
    field_0 = 0;
    field_1C4 = 1;
}

// ----------------------------------------------------

STUB_FUNC(0x5d5c80)
void Garox_1118_sub::sub_5D5C80()
{
}

MATCH_FUNC(0x5d6290)
void Garox_1118_sub::sub_5D6290()
{
    Player* pPlayerIter = gGame_0x40_67E008->sub_4B9CD0();
    while (pPlayerIter)
    {
        thirsty_lamarr* pLamarr1 = pPlayerIter->field_2D4_unk.sub_592360();
        pLamarr1->sub_4925E0();
        thirsty_lamarr* pLamarr2 = pPlayerIter->field_2D4_unk.sub_5935B0();
        pLamarr2->sub_4925E0();
        pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
    }
    Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
    pPlayer->field_684_lives.sub_4925E0();
    pPlayer->field_6BC_multpliers.sub_4925E0();
}

// ----------------------------------------------------

STUB_FUNC(0x5cf730)
void Garox_110C_sub::sub_5CF730()
{
}

STUB_FUNC(0x5cf910)
void Garox_110C_sub::sub_5CF910()
{
}

// ----------------------------------------------------

STUB_FUNC(0x5d0260)
void Garox_1108_sub::sub_5D0260()
{
}

// ----------------------------------------------------

MATCH_FUNC(0x5d53e0)
void Garox_1::sub_5D53E0()
{
    if (field_0_timer)
    {
        field_84 = Frontend::sub_5D8990(field_2_str, word_7064B8);
    }
}

STUB_FUNC(0x5d5420)
void Garox_1::sub_5D5420()
{
}

MATCH_FUNC(0x5d5600)
void Garox_1::sub_5D5600(u8 a2)
{
    sprintf(byte_67CE50, "c%02d", a2);
    field_0_timer = 90;
    wchar_t* pStr = gText_0x14_704DFC->Find_5B5F90(byte_67CE50);
    wcscpy(field_2_str, pStr);
    sub_5D53E0();
}

MATCH_FUNC(0x5d5690)
void Garox_1::sub_5D5690()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

MATCH_FUNC(0x5d56a0)
Garox_1::Garox_1()
{
    field_0_timer = 0;
}

// ----------------------------------------------------

// https://decomp.me/scratch/vYRH9
STUB_FUNC(0x5cf970)
void Garox_27B5_sub::sub_5CF970()
{
    if (field_27B5_sub)
    {
        Player* field_38_orf1 = gGame_0x40_67E008->field_38_orf1;

        Ped* pPed;
        if (field_38_orf1->field_68 == 2 || field_38_orf1->field_68 == 3)
        {
            pPed = field_38_orf1->field_2C8_unkq;
        }
        else
        {
            pPed = field_38_orf1->field_2C4_player_ped;
        }

        Zone_144* pZone = field_38_orf1->field_34_pObj;
        wchar_t* pZoneName;
        if (pZone)
        {
            pZoneName = pZone->get_name_wide_4BED30();
        }
        else
        {
            pZoneName = word_67DC8C;
        }

        Car_BC* pCar = pPed->field_16C_car;
        wchar_t* pCarOrPedStr;
        if (pCar)
        {
            pCarOrPedStr = pCar->GetCarStr_439F80();
        }
        else
        {
            pCarOrPedStr = L"ped";
        }
        swprintf(tmpBuff_67BD9C,
                 L"%s at (%3.1f, %3.1f, %3.1f) %s",
                 pCarOrPedStr,
                 pPed->field_1AC_cam.x.ToFloat() * 0.00006103515625,
                 pPed->field_1AC_cam.y.ToFloat() * 0.00006103515625,
                 pPed->field_1AC_cam.z.ToFloat() * 0.00006103515625,
                 pZoneName);

        Garox_C4* pC4 = gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, -1, 16, word_7064B8, 1);
        pC4->field_B0 = 8;
        pC4->field_B4 = 0;
    }
}

// ----------------------------------------------------

STUB_FUNC(0x5cfa70)
void Garox_107C_sub::sub_5CFA70()
{
}

MATCH_FUNC(0x5cfe20)
void Garox_107C_sub::sub_5CFE20()
{
}

MATCH_FUNC(0x5cfe30)
void Garox_107C_sub::Empty_5CFE30()
{
}

// ----------------------------------------------------

STUB_FUNC(0x5d0050)
void Garox_Sub_C::sub_5D0050(char_type a2)
{
}

MATCH_FUNC(0x5d7510)
Garox_Sub_C::Garox_Sub_C()
{
    field_0 = 0;
    field_2 = 0;
    field_1 = 0;
    field_4 = 0;
    field_8 = -1;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d00b0)
void Garox_Sub_C_Array::sub_5D00B0()
{
    Ped* pPed = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;
    field_48_count = pPed->get_wanted_star_count_46EF00();

    const bool a2 = gPolice_7B8_6FEE40->sub_56F800(pPed);
    s32 i = 0;
    Garox_Sub_C* pIter = &field_1028[0];
    while (i < field_48_count)
    {
        pIter->sub_5D0050(a2);
        i++;
        pIter++;
    }
}

STUB_FUNC(0x5d0110)
void Garox_Sub_C_Array::sub_5D0110()
{
}

MATCH_FUNC(0x5d0210)
void Garox_Sub_C_Array::sub_5D0210()
{
    u16 converted_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, 14);
    sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(converted_pal);

    field_4C_w_fp.FromU8(sprite_index->field_4_width);
    field_50_h_fp.FromU8(sprite_index->field_5_height);

    for (s32 i = 0; i < GTA2_COUNTOF(field_1028); i++)
    {
        field_1028[i].field_2 = 2;
        field_1028[i].field_1 = 2;
    }
}

// ----------------------------------------------------

STUB_FUNC(0x5d1b10)
void Garox_C4::sub_5D1B10(const wchar_t* pStr, s16 a3, s16 a4, s16 a5, s32 displayTime)
{
}

STUB_FUNC(0x5d1d00)
void Garox_C4::sub_5D1D00()
{
}

MATCH_FUNC(0x5d1db0)
bool Garox_C4::sub_5D1DB0()
{
    if (field_A4_display_time != -3)
    {
        field_A4_display_time--;
        if (field_A4_display_time < 0)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d1e10)
bool Garox_C4::operator_equals_5D1E10(Garox_C4* pOther)
{
    return field_A4_display_time > 0 && pOther != this && field_A8 == pOther->field_A8 && field_AA == pOther->field_AA &&
        field_AC == pOther->field_AC && !wcscmp(field_0_str_buf, pOther->field_0_str_buf);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1eb0)
void Garox_1700_L::sub_5D1EB0(Garox_C4* String2)
{
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->operator_equals_5D1E10(String2))
        {
            pIter->field_A4_display_time = 0;
            return;
        }
        pIter = pIter->field_C0_pNext;
    }
}

MATCH_FUNC(0x5d1f50)
Garox_C4* Garox_1700_L::sub_5D1F50(const wchar_t* pStr, s16 maybe_x, s16 maybe_y, s16 a5, s32 a6)
{
    Garox_C4* pOld_964 = field_964;
    Garox_C4* pOldFirst = field_960_pFirst;
    field_964 = pOld_964->field_C0_pNext;
    pOld_964->field_C0_pNext = pOldFirst;
    field_960_pFirst = pOld_964;
    pOld_964->sub_5D1B10(pStr, maybe_x, maybe_y, a5, a6);
    sub_5D1EB0(pOld_964);
    return pOld_964;
}

MATCH_FUNC(0x5d2010)
void Garox_1700_L::Service_5D2010()
{
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        pIter->sub_5D1D00();
        pIter = pIter->field_C0_pNext;
    }
}

MATCH_FUNC(0x5d2050)
void Garox_1700_L::sub_5D2050()
{
    Garox_C4* pAltIter = 0;
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->sub_5D1DB0())
        {
            if (pAltIter)
            {
                pAltIter->field_C0_pNext = pIter->field_C0_pNext;
                pIter->field_C0_pNext = field_964;
                field_964 = pIter;
                pIter = pAltIter->field_C0_pNext;
            }
            else
            {
                Garox_C4* pOld_field_964 = field_964;
                field_960_pFirst = field_960_pFirst->field_C0_pNext;
                pIter->field_C0_pNext = pOld_field_964;
                field_964 = pIter;
                pIter = field_960_pFirst;
            }
        }
        else
        {
            pAltIter = pIter;
            pIter = pIter->field_C0_pNext;
        }
    }
}

STUB_FUNC(0x5d2280)
Garox_1700_L::Garox_1700_L()
{
}

// ----------------------------------------------------

MATCH_FUNC(0x4be650)
Garox_C::~Garox_C()
{
    field_0 = -1;
    field_4 = 0;

    if (field_8)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_8);
        field_8 = 0;
    }
}

MATCH_FUNC(0x5d2320)
void Garox_C::sub_5D2320()
{
    if (field_0 < 0)
    {
        return;
    }

    field_0--;
    if (field_0 == -1)
    {
        field_4 = 0;
    }
}

STUB_FUNC(0x5d2380)
void Garox_C::sub_5D2380(s32 a2, s32 a3)
{
}

STUB_FUNC(0x5d2680)
s32 Garox_C::sub_5D2680(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5d2ab0)
s32 Garox_C::sub_5D2AB0(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5d3040)
void Garox_C_Array::sub_5D3040()
{
}

MATCH_FUNC(0x5d31b0)
void Garox_C_Array::sub_5D31B0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_620); i++)
    {
        field_620[i].sub_5D2320();
    }
}

STUB_FUNC(0x5d31f0)
s32 Garox_C::sub_5D31F0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5d3220)
s32 Garox_C::sub_5D3220(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5d3280)
s32 Garox_C::sub_5D3280(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x5d32d0)
void Garox_C_Array::sub_5D32D0(s32 a2)
{
    field_620[a2].field_0 = -1;
}

STUB_FUNC(0x5d32f0)
Garox_C* Garox_C::sub_5D32F0(s32 a2, s32 a3)
{
    return 0;
}

MATCH_FUNC(0x5d3310)
void Garox_C_Array::sub_5D3310(s32 a2)
{
    field_620[a2].field_4 = 0;
}

MATCH_FUNC(0x5d7650)
Garox_C::Garox_C()
{
    field_0 = -1;
    field_4 = 0;
    field_8 = 0;
}

// ----------------------------------------------------

STUB_FUNC(0x5d03c0)
void Garox_24::sub_5D03C0(Zone_144* pZone)
{
    // TODO: Try to match this soon to verify structs are correct
    field_18 = pZone->field_130_info_phone_y;
    field_14 = pZone->field_12C_info_phone_x;
    field_1C = pZone->field_134_info_phone_z;
    field_10 = 5;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0510)
void Garox_7C::sub_5D0510(s32 a2)
{
    field_18.field_28 = a2;
}

STUB_FUNC(0x5d0530)
char_type Garox_7C::sub_5D0530()
{
    return 0;
}

STUB_FUNC(0x5d0620)
char_type Garox_7C::sub_5D0620()
{
    return 0;
}

STUB_FUNC(0x5d0850)
s32 Garox_7C::sub_5D0850()
{
    return 0;
}

MATCH_FUNC(0x5d0c60)
void Garox_7C::sub_5D0C60()
{
    if (!sub_5D0620())
    {
        char_type v2 = sub_5D0530();
        field_18.field_10.field_5 = v2;
        if (v2)
        {
            sub_5D0850();
        }
    }
}

STUB_FUNC(0x5d0c90)
void Garox_7C::sub_5D0C90()
{
}

STUB_FUNC(0x5d0dc0)
void Garox_7C::sub_5D0DC0(Ped* a2)
{
    switch (a2->field_244_remap)
    {
        case 5:
        case 6:
            field_18.field_2C = 4;
            break;
        case 7:
            field_18.field_2C = 5;
            break;
        case 8:
            field_18.field_2C = 3;
            break;
        case 9:
            field_18.field_2C = 6;
            break;
        case 10:
            field_18.field_2C = 7;
            break;
        case 11:
            field_18.field_2C = 1;
            break;
        case 13:
            field_18.field_2C = 2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x5d1350)
void Garox_7C_Array::sub_5D1350()
{
    if ((u8)bStartNetworkGame_7081F0)
    {
        if (gLucid_hamilton_67E8E0.sub_4C5BC0() != 3)
        {
            sub_5D10B0();
            for (Player* pPlayerIter = gGame_0x40_67E008->sub_4B9CD0(); pPlayerIter;
                 pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                if (!pPlayerIter->field_0)
                {
                    Garox_7C* p7C = sub_5D1050();
                    p7C->field_18.field_18.field_C = pPlayerIter;
                    p7C->field_18.field_18.field_10 = 6;
                    Ped* pPlayerPed = pPlayerIter->field_2C4_player_ped;
                    if (pPlayerPed)
                    {
                        p7C->sub_5D0DC0(pPlayerPed);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5d7600)
Garox_7C::Garox_7C()
{
    field_18.field_10.field_30 = 0;
    field_18.field_10.field_34 = 0;
    field_18.field_10.field_5 = 0;

    field_18.field_18.init();
    field_18.field_3C.init();

    field_18.field_60 = &field_18.field_18;
    field_18.field_2E = 0;
    field_18.field_10.field_6 = 0;
}

// ----------------------------------------------------

STUB_FUNC(0x5d0e40)
char_type Garox_7C_Array::sub_5D0E40(s32* a2)
{
    return 0;
}

STUB_FUNC(0x5d0e90)
void Garox_7C_Array::sub_5D0E90()
{
}

STUB_FUNC(0x5d0ef0)
Garox_7C* Garox_7C_Array::sub_5D0EF0()
{
    return 0;
}

MATCH_FUNC(0x5d0f40)
char_type Garox_7C_Array::sub_5D0F40(Zone_144* a2)
{
    Garox_7C* pIter = &field_0_array[0];
    for (s32 i = 0; i < 17; i++, pIter++)
    {
        if ((pIter->field_18.field_18.field_10 || pIter->field_18.field_3C.field_10) &&
            (pIter->field_18.field_10.field_30 == a2 && pIter->field_18.field_60->field_10 != 5))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x5d0f80)
void Garox_7C_Array::sub_5D0F80()
{
    for (s32 i = 0; i < 17; i++)
    {
        if (field_0_array[i].field_18.field_18.field_10 || field_0_array[i].field_18.field_3C.field_10)
        {
            if (field_0_array[i].field_18.field_10.field_30)
            {
                if (field_0_array[i].field_18.field_60->field_10 == 5 && !sub_5D0F40(field_0_array[i].field_18.field_10.field_30))
                {
                    field_0_array[i].field_18.field_18.field_10 = 0;
                    field_0_array[i].field_18.field_3C.field_10 = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x5d0fd0)
void Garox_7C_Array::sub_5D0FD0()
{
    sub_5D0EF0();

    for (s32 i = 0; i < GTA2_COUNTOF(field_0_array); i++)
    {
        if (field_0_array[i].field_18.field_18.field_10 || field_0_array[i].field_18.field_3C.field_10)
        {
            field_0_array[i].sub_5D0C60();
        }
    }

    if (field_844)
    {
        sub_5D0F80();
        field_844 = 0;
    }
}

MATCH_FUNC(0x5d1020)
Garox_7C* Garox_7C_Array::sub_5D1020(s32* a2)
{
    Garox_7C* pIter = &field_0_array[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++, pIter++)
    {
        if (!pIter->field_18.field_10.field_6)
        {
            *a2 = i;
            pIter->field_18.field_10.field_6 = 1;
            return pIter;
        }
    }
    return 0;
}

STUB_FUNC(0x5d1050)
Garox_7C* Garox_7C_Array::sub_5D1050()
{
    return 0;
}

STUB_FUNC(0x5d10b0)
char_type* Garox_7C_Array::sub_5D10B0()
{
    return 0;
}

MATCH_FUNC(0x5d10d0)
Garox_7C* Garox_7C_Array::sub_5D10D0(Zone_144* pZone, s32 phone_type)
{
    s32 i = 0;
    Garox_7C* pIter = field_0_array;
    while (i < GTA2_COUNTOF_S(field_0_array))
    {
        if ((pIter->field_18.field_18.field_10 || pIter->field_18.field_3C.field_10) &&
            (pIter->field_18.field_10.field_30 == pZone && pIter->field_18.field_28 == phone_type))
        {
            return pIter;
        }
        i++;
        pIter++;
    }
    return 0;
}

STUB_FUNC(0x5d1110)
void Garox_7C_Array::place_gang_phone_5D1110(Object_2C* pPhoneInfo)
{
}

MATCH_FUNC(0x5d1310)
void Garox_7C_Array::sub_5D1310(Zone_144* pZone)
{
    Garox_7C* p7C = sub_5D1050();
    p7C->sub_5D0510(4);
    p7C->field_18.field_10.field_30 = pZone;
    p7C->field_18.field_2C = pZone->field_138_arrow_colour;
    p7C->field_18.field_10.field_34 = 0;
    p7C->field_18.field_18.sub_5D03C0(pZone);
}

// ----------------------------------------------------

STUB_FUNC(0x5d3330)
s32 Garox_1E34_L::sub_5D3330()
{
    return 0;
}

STUB_FUNC(0x5d3350)
char_type* Garox_1E34_L::sub_5D3350()
{
    return 0;
}

STUB_FUNC(0x5d3370)
s32 Garox_1E34_L::sub_5D3370()
{
    return 0;
}

STUB_FUNC(0x5d33a0)
s32 Garox_1E34_L::sub_5D33A0()
{
    return 0;
}

STUB_FUNC(0x5d33f0)
s32 Garox_1E34_L::sub_5D33F0()
{
    return 0;
}

STUB_FUNC(0x5d3470)
size_t Garox_1E34_L::sub_5D3470()
{
    return 0;
}

STUB_FUNC(0x5d3680)
char_type Garox_1E34_L::sub_5D3680(s16 a1)
{
    return 0;
}

STUB_FUNC(0x5d39d0)
s32 Garox_1E34_L::sub_5D39D0()
{
    return 0;
}

STUB_FUNC(0x5d3b80)
void Garox_1E34_L::sub_5D3B80()
{
}

STUB_FUNC(0x5d3f10)
s32 Garox_1E34_L::sub_5D3F10(s32 a2, const char_type* a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x5d4400)
s32 Garox_1E34_L::sub_5D4400(s32 a2, const char_type* a3)
{
    return 0;
}

STUB_FUNC(0x5d44d0)
void Garox_1E34_L::sub_5D44D0()
{
}

STUB_FUNC(0x5d4850)
s32 Garox_1E34_L::sub_5D4850()
{
    return 0;
}

STUB_FUNC(0x5d4890)
s32 Garox_1E34_L::sub_5D4890(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5d4930)
Garox_1E34_L::Garox_1E34_L()
{
}

// ----------------------------------------------------

STUB_FUNC(0x5d5900)
void Garox_90_L::sub_5D5900()
{
}

MATCH_FUNC(0x5d5ad0)
void Garox_90_L::sub_5D5AD0()
{
    if (field_0)
    {
        field_84 = Frontend::sub_5D8990(field_2_wstr, word_706618);
    }
}

STUB_FUNC(0x5d5af0)
void Garox_90_L::sub_5D5AF0(u8* a2, u8* a3)
{
}

STUB_FUNC(0x5d5b60)
void Garox_90_L::sub_5D5B60()
{
}

MATCH_FUNC(0x5d5c50)
void Garox_90_L::sub_5D5C50()
{
    field_90 = 0;
    field_94 = 0;
}

MATCH_FUNC(0x5d5c60)
Garox_90_L::Garox_90_L()
{
    field_0 = 0;
    field_88 = 0;
    field_8C = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d53b0)
Garox_4_L::Garox_4_L()
{
    field_0 = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x4bbbb0)
Garox_2B00::~Garox_2B00()
{
}

STUB_FUNC(0x5d4a10)
void Garox_2B00::sub_5D4A10()
{
}

MATCH_FUNC(0x5d5190)
void Garox_2B00::sub_5D5190()
{
    if (field_0.field_0)
    {
        // TODO: Structure seems wrong, probablty field_2 to field_4C of Garox_2B00 is a string buffer?
        field_44 = Frontend::sub_5D8990((wchar_t*)&field_0.field_2, word_706508);
    }
}

STUB_FUNC(0x5d5240)
void Garox_2B00::sub_5D5240(wchar_t* Source)
{
}

STUB_FUNC(0x5d5350)
void Garox_2B00::sub_5D5350()
{
    if (field_0.field_0)
    {
        field_0.field_0--;
        if (field_0.field_0 <= 80u)
        {
            if (field_0.field_0 < 40u)
            {
                --field_48;
            }
        }
        else
        {
            ++field_48;
        }
    }
}

MATCH_FUNC(0x5d6860)
void Garox_2B00::DrawGui_5D6860()
{
    if (!bSkip_user_67D506)
    {
        sub_5D6A70();
        field_1118_sub.sub_5D5C80();
        field_110C_sub.sub_5CF910();
        field_13C0_sub.sub_5CFE40();
        field_1028.sub_5D0110();
        field_107C_sub.sub_5CFA70();
        field_1108_sub.sub_5D0260();
        field_4C.sub_5D5900();
        sub_5D4A10();
        field_1080.sub_5D5420();
        field_DC.sub_5D3B80();
        field_620.sub_5D3040();
        field_650.Service_5D2010();
        field_1F18.sub_5D0E90();
        field_12F0.sub_5D56D0();
        field_111C.sub_5D1940();
        field_12E4_sub.sub_5D63B0();
        field_2A25_sub.sub_5D16B0();
        field_12EC_sub.sub_5D1430();
    }
}

STUB_FUNC(0x5d69c0)
void Garox_2B00::sub_5D69C0()
{
}

MATCH_FUNC(0x5d69d0)
void Garox_2B00::sub_5D69D0()
{
    field_1118_sub.sub_5D6290();
    field_110C_sub.sub_5CF730();
    field_27B5_sub.sub_5CF970();
    field_1028.sub_5D00B0();
    sub_5D5350();
    field_1080.sub_5D5690();
    field_4C.sub_5D5B60();
    field_DC.sub_5D44D0();
    field_620.sub_5D31B0();
    field_650.sub_5D2050();
    field_1F18.sub_5D0FD0();
    field_107C_sub.sub_5CFE20();
    field_111C.sub_5D1AB0();
    field_12F0.sub_5D5760();
}

MATCH_FUNC(0x5d6a70)
void Garox_2B00::sub_5D6A70()
{
    if (gLighting_626A09)
    {
        gbh_SetAmbient(1.0);
    }
}

MATCH_FUNC(0x5d6a90)
void Garox_2B00::sub_5D6A90()
{
    field_13C4_text_speed = gRegistry_6FF968.Set_Option_586F70("text_speed", 3);
}

MATCH_FUNC(0x5d6ab0)
void Garox_2B00::sub_5D6AB0()
{
    sub_5D6B00();
    field_DC.sub_5D3470();
    sub_5D5190();
    field_4C.sub_5D5AD0();
    field_111C.sub_5D1860();
    field_12F0.sub_5D56B0();
    field_1080.sub_5D53E0();
}

STUB_FUNC(0x5d6b00)
s16 Garox_2B00::sub_5D6B00()
{
    return 0;
}

MATCH_FUNC(0x5d6be0)
void Garox_2B00::sub_5D6BE0()
{
    sub_5D6A90();
    sub_5D6B00();
    field_4C.sub_5D5C50();
    field_107C_sub.Empty_5CFE30();
    field_1028.sub_5D0210();
    field_1F18.sub_5D1350();
}

STUB_FUNC(0x5d6c20)
s32 Garox_2B00::sub_5D6C20(s32 action, char_type* a2)
{
    return field_12EC_sub.sub_5D13C0(action, a2) || field_2A25_sub.sub_5D15E0(action, a2);
}

STUB_FUNC(0x5d6c70)
s32 Garox_2B00::sub_5D6C70(s32 a1)
{
    // TODO EAX vs AL reg
    return field_12EC_sub.sub_5D15A0(a1) || field_2A25_sub.sub_5D17D0(a1);
}

MATCH_FUNC(0x5d6cb0)
s32 Garox_2B00::sub_5D6CB0(s32 a1)
{
    return field_12EC_sub.sub_5D15A0(a1);
}

STUB_FUNC(0x5d6cd0)
Garox_2B00::Garox_2B00()
{
    field_13C4_text_speed = 0;
}
