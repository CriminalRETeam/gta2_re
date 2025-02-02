#include "eager_benz.hpp"
#include "Car_BC.hpp"
#include "ExplodingScore_100.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "debug.hpp"
#include "lucid_hamilton.hpp"
#include <string.h>

// TODO: move
EXPORT_VAR extern s32 bStartNetworkGame_7081F0;
EXPORT_VAR extern Shooey_CC* gShooey_CC_67A4B8;

MATCH_FUNC(0x591bd0)
eager_benz::eager_benz()
{
    field_368_pObj = 0;

    field_74 = 1;
    field_75 = 1;

    field_18C = 0;
    field_190 = 0;
    field_70 = 0;
    field_78 = 0;
    field_7C = 0;
    field_86 = 0;
    field_88 = 0;
    field_8A = 0;
    field_80 = 0;
    field_84 = 0;
    field_194 = 0;
    field_198_accuracy_count = 0;
    field_19C_reverse_count = 0;
    field_1A0 = 0;
    field_1A4 = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_8C); i++)
    {
        field_8C[i] = 0;
    }
}

STUB_FUNC(0x591c70)
u32 eager_benz::sub_591C70()
{
    return 0;
}

MATCH_FUNC(0x5922f0)
void eager_benz::sub_5922F0(Player* a2, s16 a3, s32 a4, s16 a5, u16 a6)
{
    field_368_pObj = a2;
    field_0_unk.sub_492110(a3, a4, a5);
    field_38_unk.sub_492110(a3, a6, a5);
}

MATCH_FUNC(0x592330)
void eager_benz::sub_592330()
{
    field_0_unk.sub_492150();
    field_38_unk.sub_492150();
    field_1A8_unk.sub_431E10(this);
    sub_592380(3);
}

MATCH_FUNC(0x592360)
thirsty_lamarr* eager_benz::sub_592360()
{
    return &field_0_unk;
}

MATCH_FUNC(0x592370)
s32 eager_benz::sub_592370()
{
    return field_0_unk.field_0;
}

STUB_FUNC(0x592380)
void eager_benz::sub_592380(char_type a2)
{
}

STUB_FUNC(0x592430)
void eager_benz::sub_592430(char_type a2)
{
}

MATCH_FUNC(0x592570)
void eager_benz::sub_592570(char_type a2, s32 a3)
{
    field_8C[a3] |= a2;
    sub_592430(a2);
}

STUB_FUNC(0x5925b0)
s32 eager_benz::sub_5925B0(u8 a1, char_type a2)
{
    return 0;
}

MATCH_FUNC(0x592620)
void eager_benz::AddCash_592620(s32 cash)
{
    field_0_unk.sub_4921B0(cash);

    if (bStartNetworkGame_7081F0)
    {
        gLucid_hamilton_67E8E0.sub_4C5C80(field_368_pObj->field_2E_idx, cash);
    }
}

STUB_FUNC(0x592660)
void eager_benz::sub_592660(Ped* a2, Ped* a3)
{
}

STUB_FUNC(0x592dd0)
void eager_benz::sub_592DD0(Car_BC* a2, u32* a3)
{
}

STUB_FUNC(0x593030)
char_type eager_benz::sub_593030(Car_BC* a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x593150)
void eager_benz::sub_593150(Car_BC* a2, s16 a3)
{
}

MATCH_FUNC(0x593220)
void eager_benz::sub_593220()
{
    field_368_pObj->field_2D4_unk.AddCash_592620(field_368_pObj->field_6BC_multpliers.field_0 * 20);
}

STUB_FUNC(0x593240)
void eager_benz::sub_593240(Car_BC* a2)
{
}

STUB_FUNC(0x593370)
void eager_benz::sub_593370(Car_BC* a2)
{
    if (!bExplodingOff_67D4FB)
    {
        if (field_368_pObj->field_0)
        {
            gExplodingScore_100_702F34->sub_596890(a2->field_50_car_sprite->field_14_xpos,
                                             a2->field_50_car_sprite->field_18_ypos,
                                             a2->field_50_car_sprite->field_1C_zpos,
                                             10 * field_368_pObj->field_6BC_multpliers.field_0);
        }
    }
    field_368_pObj->field_2D4_unk.AddCash_592620(10 * field_368_pObj->field_6BC_multpliers.field_0);
    if (field_368_pObj->field_68 == 2)
    {
        gShooey_CC_67A4B8->sub_484FE0(4u, field_368_pObj->field_2C8_unkq);
    }
    else
    {
        gShooey_CC_67A4B8->sub_484FE0(4u, field_368_pObj->field_2C4_player_ped);
    }
}

STUB_FUNC(0x593410)
void eager_benz::sub_593410(Car_BC* pCar)
{
}

STUB_FUNC(0x5934f0)
void eager_benz::UpdateAccuracyCount_5934F0(u32 a2, s32 a3, Ped* a4)
{
}

MATCH_FUNC(0x5935b0)
thirsty_lamarr* eager_benz::sub_5935B0()
{
    return &field_38_unk;
}

MATCH_FUNC(0x5935c0)
s32 eager_benz::sub_5935C0()
{
    return field_38_unk.field_0;
}

MATCH_FUNC(0x5935d0)
void eager_benz::sub_5935D0(s32 a2)
{
    field_38_unk.sub_4921B0(a2);
}
