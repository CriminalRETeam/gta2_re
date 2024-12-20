#include "TileAnim_2.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR TileAnim_2* gTileAnim_2_7052C4;
GLOBAL(gTileAnim_2_7052C4, 0x7052C4);

EXPORT_VAR TileAnim_4BC* gTileAnim_4BC_7052C8;
GLOBAL(gTileAnim_4BC_7052C8, 0x7052C8);



STUB_FUNC(0x5bc260)
s32 TileAnim_2::sub_5BC260(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5)
{
    return 0;
}

STUB_FUNC(0x5bc2c0)
s32 TileAnim_2::sub_5BC2C0()
{
    return 0;
}

STUB_FUNC(0x5bc300)
void TileAnim_2::Empty_5BC300()
{
}

STUB_FUNC(0x5bc310)
void TileAnim_2::sub_5BC310()
{
}

STUB_FUNC(0x5bc3a0)
TileAnim_2::TileAnim_2()
{
}

STUB_FUNC(0x5bc470)
TileAnim_2::~TileAnim_2()
{
    // TODO: Jump target is diff ??
    GTA2_DELETE_AND_NULL(gTileAnim_4BC_7052C8);
}

STUB_FUNC(0x5bc130)
void TileAnim_18::sub_5BC130()
{
}

STUB_FUNC(0x5bc150)
s32 TileAnim_18::sub_5BC150()
{
    return 0;
}

STUB_FUNC(0x5bc190)
void TileAnim_18::sub_5BC190(s32 pStru)
{
}

STUB_FUNC(0x5bc1d0)
s32 TileAnim_18::sub_5BC1D0()
{
    return 0;
}

STUB_FUNC(0x5bc1f0)
char_type TileAnim_18::sub_5BC1F0()
{
    return 0;
}

MATCH_FUNC(0x5beba0)
TileAnim_18::TileAnim_18()
{
    this->field_0 = 0;
    this->field_2 = 0;
    this->field_4 = 0;
    this->field_6 = 0;
    this->field_8 = 0;
    this->field_A = 0;
    this->field_C_ptr = 0;
    this->field_10 = 0;
    this->field_12_idx = 0;
    this->field_14_next_ptr = 0;
}

MATCH_FUNC(0x5bebd0)
TileAnim_18::~TileAnim_18()
{
    field_C_ptr = NULL;
    field_14_next_ptr = NULL;
}

MATCH_FUNC(0x5bc4a0)
TileAnim_4BC::~TileAnim_4BC()
{
    field_0_pStart = NULL;
    field_4_pPrev = NULL;
}
