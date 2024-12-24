#include "angry_lewin_0x85C.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "cool_nash_0x294.hpp"
#include "debug.hpp"
#include "infallible_turing.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "zealous_borg.hpp"

MATCH_FUNC(0x4881E0)
u8 angry_lewin_0x85C::GetIdx_4881E0()
{
    return field_2E_idx;
}

STUB_FUNC(0x5645B0)
void angry_lewin_0x85C::sub_5645B0(Car_BC* a2)
{
}

STUB_FUNC(0x564610)
char_type angry_lewin_0x85C::sub_564610(Car_BC* a2, char_type a3)
{
    return 'a';
}

STUB_FUNC(0x564680)
u32* angry_lewin_0x85C::sub_564680(s32 a2)
{
    return 0;
}

STUB_FUNC(0x564710)
void angry_lewin_0x85C::sub_564710(Car_BC* a2, s32 a3)
{
}

STUB_FUNC(0x564790)
void angry_lewin_0x85C::sub_564790(s32 idx)
{
}

STUB_FUNC(0x5647D0)
void angry_lewin_0x85C::sub_5647D0()
{
}

MATCH_FUNC(0x5648F0)
keen_bhaskara_0x30* angry_lewin_0x85C::sub_5648F0()
{
    return field_788_idx == -1 ? NULL : field_718[field_788_idx];
}

STUB_FUNC(0x564910)
void angry_lewin_0x85C::sub_564910(keen_bhaskara_0x30* a2)
{
}

STUB_FUNC(0x564940)
char_type angry_lewin_0x85C::sub_564940()
{
    return 'a';
}

STUB_FUNC(0x564960)
char_type angry_lewin_0x85C::sub_564960(s32 a2, u8 a3)
{
    return 'a';
}

STUB_FUNC(0x5649D0)
u16 angry_lewin_0x85C::sub_5649D0(char_type a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x564AD0)
void angry_lewin_0x85C::sub_564AD0(Car_BC* a2)
{
}

STUB_FUNC(0x564B60)
s32 angry_lewin_0x85C::sub_564B60()
{
    return 0;
}

STUB_FUNC(0x564B80)
s32 angry_lewin_0x85C::sub_564B80()
{
    return 0;
}

MATCH_FUNC(0x564C00)
u16 angry_lewin_0x85C::sub_564C00()
{
    sub_564B80();

    if (field_788_idx >= 15)
    {
        field_16 = field_788_idx;
        field_788_idx = field_14;
    }

    if (field_18 >= 15)
    {
        field_16 = field_18;
        field_18 = field_14;
    }

    return sub_5649D0(0, 0);
}

STUB_FUNC(0x564C50)
void angry_lewin_0x85C::sub_564C50(s32 a2)
{
}

EXPORT_VAR u8 byte_67D57C;
GLOBAL(byte_67D57C, 0x67D57C);

EXPORT_VAR u8 byte_67D539;
GLOBAL(byte_67D539, 0x67D539);

MATCH_FUNC(0x564CC0)
void angry_lewin_0x85C::sub_564CC0()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_6F4); i++)
    {
        if (i == 11 && byte_67D539)
        {
            continue;
        }

        if (i == 7 && byte_67D57C)
        {
            continue;
        }

        field_6F4[i] = 0;
    }
}

STUB_FUNC(0x564CF0)
void angry_lewin_0x85C::sub_564CF0()
{
}

STUB_FUNC(0x564D60)
char_type angry_lewin_0x85C::sub_564D60(s32 a2)
{
    return 'a';
}

STUB_FUNC(0x565070)
u16 angry_lewin_0x85C::sub_565070()
{
    return 0;
}

STUB_FUNC(0x5651F0)
s32 angry_lewin_0x85C::sub_5651F0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x565310)
void angry_lewin_0x85C::sub_565310()
{
}

STUB_FUNC(0x5653E0)
void angry_lewin_0x85C::sub_5653E0()
{
}

MATCH_FUNC(0x565460)
void angry_lewin_0x85C::sub_565460()
{
    // Empty
}

STUB_FUNC(0x565490)
s32 angry_lewin_0x85C::sub_565490(cool_nash_0x294* pPed)
{
    return 0;
}

MATCH_FUNC(0x565740)
void angry_lewin_0x85C::SetInputs_565740(u32 input)
{
    field_4_inputs = input;
}

STUB_FUNC(0x565770)
void angry_lewin_0x85C::sub_565770(u8 count)
{
}

STUB_FUNC(0x565860)
void angry_lewin_0x85C::sub_565860()
{
}

STUB_FUNC(0x565890)
void angry_lewin_0x85C::sub_565890(u16 action)
{
}

STUB_FUNC(0x566380)
void angry_lewin_0x85C::sub_566380(u16 a2)
{
}

STUB_FUNC(0x566520)
char_type angry_lewin_0x85C::sub_566520()
{
    return 'a';
}

STUB_FUNC(0x566820)
void angry_lewin_0x85C::sub_566820()
{
}

STUB_FUNC(0x5668D0)
void angry_lewin_0x85C::sub_5668D0(cool_nash_0x294* a2)
{
}

STUB_FUNC(0x566C30)
char_type angry_lewin_0x85C::sub_566C30(Car_BC* a2)
{
    return 'a';
}

STUB_FUNC(0x566C80)
char_type angry_lewin_0x85C::sub_566C80(u32* a2)
{
    return 'a';
}

STUB_FUNC(0x566EE0)
void angry_lewin_0x85C::sub_566EE0(char_type a2)
{
}

STUB_FUNC(0x5670B0)
s32 angry_lewin_0x85C::sub_5670B0()
{
    return 0;
}

STUB_FUNC(0x567130)
void angry_lewin_0x85C::sub_567130()
{
}

MATCH_FUNC(0x567850)
void angry_lewin_0x85C::sub_567850()
{
    if (field_29 == 0)
    {
        field_29 = 1;
        field_2C = 70;
        return;
    }

    if (--field_2C == 0)
    {
        field_29 = 0;
        sub_5695A0();
    }
}

STUB_FUNC(0x5679E0)
void angry_lewin_0x85C::sub_5679E0()
{
}

MATCH_FUNC(0x568520)
void angry_lewin_0x85C::sub_568520()
{
    const cool_nash_0x294* pPed = this->field_68 == 2 ? field_2C8_unkq : field_2C4_player_ped;
    const Fix16 cam_x_fp = pPed->field_1AC_cam.x;
    const Fix16 cam_y_fp = pPed->field_1AC_cam.y;
    field_38 = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 0xFu);
    field_3C = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 1u);
    field_40 = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 5u);
    field_34_pObj = gMap_0x370_6F6268->sub_4DFB50(cam_x_fp, cam_y_fp);
}

MATCH_FUNC(0x568630)
void angry_lewin_0x85C::sub_568630()
{
    infallible_turing* pCam = this->field_4C_pUnk;
    if (pCam)
    {
        pCam->field_C_pAny = &this->field_14C_view_camera;
    }
}

STUB_FUNC(0x568670)
char_type angry_lewin_0x85C::sub_568670()
{
    return 'a';
}

STUB_FUNC(0x5686D0)
char_type angry_lewin_0x85C::sub_5686D0(DrawUnk_0xBC* a2)
{
    return 'a';
}

MATCH_FUNC(0x568730)
void angry_lewin_0x85C::sub_568730()
{
    cool_nash_0x294* pPlayerPed = this->field_2C4_player_ped;
    if (pPlayerPed)
    {
        pPlayerPed->sub_470300();
    }
    cool_nash_0x294* pPed = this->field_2C8_unkq;
    if (pPed)
    {
        pPed->sub_470300();
    }
    this->field_8E_bInUse = 0;
}

STUB_FUNC(0x5687F0)
void angry_lewin_0x85C::sub_5687F0()
{
}

STUB_FUNC(0x569410)
s32 angry_lewin_0x85C::sub_569410()
{
    return 0;
}

MATCH_FUNC(0x569530)
void angry_lewin_0x85C::sub_569530()
{
    Car_BC* pCar = field_2C8_unkq->field_16C_car;
    if (pCar->field_54_driver)
    {
        pCar->sub_4407F0();
    }

    if (pCar->field_98 != 4)
    {
        pCar->field_98 = 3;
    }

    field_2C8_unkq->sub_46F9D0();
    field_2C8_unkq = 0;

    sub_564C00();

    if (field_2C4_player_ped->field_16C_car)
    {
        sub_564AD0(field_2C4_player_ped->field_16C_car);
    }
}

MATCH_FUNC(0x5695A0)
void angry_lewin_0x85C::sub_5695A0()
{
    if (!this->field_28)
    {
        this->field_68 = 0;
        if (field_2D0)
        {
            cool_nash_0x294* pPed = this->field_2C8_unkq;
            if (pPed)
            {
                if (pPed->field_240_occupation == 1)
                {
                    sub_569530();
                }
            }
        }
        this->field_2C8_unkq = 0;
        this->field_2CC = 0;
        this->field_2D0 = 0;
        this->field_90_game_camera.field_3C = 1;
    }
}

STUB_FUNC(0x569600)
char_type angry_lewin_0x85C::sub_569600(Car_BC* a2)
{
    return 'a';
}

STUB_FUNC(0x5696D0)
void angry_lewin_0x85C::sub_5696D0(s32 a2)
{
}

STUB_FUNC(0x569840)
s32 angry_lewin_0x85C::sub_569840(u8* a2, u8* a3, u8* a4)
{
    return 0;
}

STUB_FUNC(0x5698E0)
Car_BC* angry_lewin_0x85C::sub_5698E0()
{
    return 0;
}

STUB_FUNC(0x569920)
s32 angry_lewin_0x85C::sub_569920(u32* a2, s32* a3, s32* a4)
{
    return 0;
}

MATCH_FUNC(0x5699F0)
void angry_lewin_0x85C::sub_5699F0(s32 a2)
{
    if (!gDo_infinite_lives_67D4C9 || a2 > 0)
    {
        field_684_unk.sub_4921B0(a2);
    }
}

STUB_FUNC(0x569A10)
void angry_lewin_0x85C::sub_569A10()
{
}

STUB_FUNC(0x569C20)
void angry_lewin_0x85C::sub_569C20()
{
}

STUB_FUNC(0x569CB0)
void angry_lewin_0x85C::sub_569CB0()
{
}

STUB_FUNC(0x569E70)
void angry_lewin_0x85C::sub_569E70()
{
}

MATCH_FUNC(0x569F00)
char* angry_lewin_0x85C::GetDeathText_569F00()
{
    switch (field_44_death_type)
    {
        case 0:
        case 1:
            return "wasted";
            break;
        case 2:
            return "fried";
            break;
        case 3:
            return "nicked";
            break;
        case 4:
            return "shocked";
            break;
        default:
            return 0;
            break;
    }
}

STUB_FUNC(0x569F40)
s32 angry_lewin_0x85C::sub_569F40()
{
    return 0;
}

MATCH_FUNC(0x569FF0)
s32 angry_lewin_0x85C::sub_569FF0()
{
    field_2F = 1;
    return sub_569F40();
}

MATCH_FUNC(0x56A000)
void angry_lewin_0x85C::SetUnknown_56A000()
{
    field_2F = 0;
}

MATCH_FUNC(0x56A010)
void angry_lewin_0x85C::sub_56A010()
{
    field_31 = 1;
}

MATCH_FUNC(0x56A020)
void angry_lewin_0x85C::sub_56A020()
{
    field_31 = 0;
}

MATCH_FUNC(0x56A030)
void angry_lewin_0x85C::sub_56A030()
{
    field_30 = 1;
}

MATCH_FUNC(0x56A040)
void angry_lewin_0x85C::sub_56A040()
{
    field_30 = 0;
}

STUB_FUNC(0x56A0F0)
s32 angry_lewin_0x85C::sub_56A0F0()
{
    return 0;
}

STUB_FUNC(0x56A1A0)
zealous_borg* angry_lewin_0x85C::sub_56A1A0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x56A310)
void angry_lewin_0x85C::sub_56A310(s32 a2)
{
}

STUB_FUNC(0x56A490)
void angry_lewin_0x85C::sub_56A490()
{
}

MATCH_FUNC(0x56A6D0)
void angry_lewin_0x85C::sub_56A6D0()
{
    // sets some car info to 0
    field_78 = 0;
    field_79 = 0;
    field_7A = 0;
    field_7B = 0;
    field_7C = 0;
    field_7D = 0;
    field_7E = 0;
    field_7F = 0;
    field_80 = 0;
    field_81 = 0;
    field_82 = 0;
    field_83 = 0;
    field_84 = 0;
    field_88 = 0;
    field_87 = 0;
    field_89 = 0;
    field_8D = 0;
    field_8A = 0;
    field_8B = 0;
    field_8C = 0;
}

STUB_FUNC(0x56A740)
angry_lewin_0x85C::angry_lewin_0x85C(u8 a2)
{
}

STUB_FUNC(0x56A940)
angry_lewin_0x85C::~angry_lewin_0x85C()
{
    Car_BC** field_54_unk; // ecx

    field_54_unk = this->field_54_unk;
    this->field_2C4_player_ped = 0;
    this->field_2C8_unkq = 0;
    this->field_2CC = 0;
    this->field_34_pObj = 0;
    this->field_38 = 0;
    this->field_3C = 0;
    field_54_unk[0] = 0;
    field_54_unk[1] = 0;
    field_54_unk[2] = 0;
    
    if (field_4C_pUnk)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_4C_pUnk);
        field_4C_pUnk = 0;
    }

    /*
	zealous_borg::dtor_484EE0(&this->field_644_unk);
	sad_mirzakhani::dtor_431DF0(&this->field_2D4_unk.field_1A8_unk);
	DrawUnk_0xBC::dtor_4369E0(&this->field_208_aux_game_camera);
	DrawUnk_0xBC::dtor_4369E0(&this->field_14C_view_camera);
	DrawUnk_0xBC::dtor_4369E0(&this->field_90_game_camera);
	*/
}
