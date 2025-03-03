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

MATCH_FUNC(0x5bc2c0)
void TileAnim_2::sub_5BC2C0(gmp_tile_animation* a1)
{
    TileAnim_18 *tmp = gTileAnim_4BC_7052C8->get_new_TileAnim_18();
    tmp->sub_5BC190(a1);
    tmp->sub_5BC1D0();
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

MATCH_FUNC(0x5bc470)
TileAnim_2::~TileAnim_2()
{
    if (gTileAnim_4BC_7052C8)
    {
        GTA2_DELETE_AND_NULL(gTileAnim_4BC_7052C8);
    }
}

MATCH_FUNC(0x5bc130)
void TileAnim_18::sub_5BC130()
{
    field_12_idx = gTileAnim_2_7052C4->field_0_count++;
}

STUB_FUNC(0x5bc150)
s32 TileAnim_18::sub_5BC150()
{
    return 0;
}

STUB_FUNC(0x5bc190)
void TileAnim_18::sub_5BC190(gmp_tile_animation* pStru)
{
}

MATCH_FUNC(0x5bc1d0)
void TileAnim_18::sub_5BC1D0()
{
    field_A = field_4;
    field_8 = field_0;
    sub_5BC150();
}

MATCH_FUNC(0x5bc1f0)
char_type TileAnim_18::sub_5BC1F0()
{
    u8 bVar3;

    field_A--;
    if (field_A == 0)
    {
        if (field_2 < field_0)
        {
            field_8--;
            bVar3 = field_8 < field_2;
        }
        else
        {
            field_8++;
            bVar3 = field_8 > field_2 ;
        }
        if (bVar3)
        {
            if (field_6 > 0 && --field_6 == 0)
            {
                return 1;
            }
            field_8 = field_0;
        }
        sub_5BC150();
        field_A = field_4;
    }
    return 0;
}

MATCH_FUNC(0x5beba0)
TileAnim_18::TileAnim_18()
{
    field_0 = 0;
    field_2 = 0;
    field_4 = 0;
    field_6 = 0;
    field_8 = 0;
    field_A = 0;
    field_C_ptr = 0;
    field_10 = 0;
    field_12_idx = 0;
    field_14_next_ptr = 0;
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
