#include "TileAnim_2.hpp"

#include "Globals.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include <stdio.h>
#include "enums.hpp"

DEFINE_GLOBAL(TileAnim_2*, gTileAnim_2_7052C4, 0x7052C4);
DEFINE_GLOBAL(TileAnimPool*, gTileAnimPool_7052C8, 0x7052C8);

MATCH_FUNC(0x5bc260)
void TileAnim_2::AddAnim_5BC260(s16 base, s16 f0, s16 length, s16 frame_rate, s16 repeat)
{
    TileAnim_18* p18 = gTileAnimPool_7052C8->Allocate();
    p18->Init_4C33F0(base, f0, length, frame_rate, repeat);
    p18->Start_5BC1D0();
}

MATCH_FUNC(0x5bc2c0)
void TileAnim_2::AddGmpAnim_5BC2C0(const gmp_tile_animation* pTileAnimation)
{
    TileAnim_18* tmp = gTileAnimPool_7052C8->Allocate();
    tmp->InitFromGmpAnim_5BC190(pTileAnimation);
    tmp->Start_5BC1D0();
}

MATCH_FUNC(0x5bc300)
void TileAnim_2::Empty_5BC300()
{
}

MATCH_FUNC(0x5bc310)
void TileAnim_2::UpdateTileAnimations_5BC310()
{
    gTileAnimPool_7052C8->field_0_pool.UpdatePool();
}

MATCH_FUNC(0x5bc3a0)
TileAnim_2::TileAnim_2()
{
    if (gTileAnimPool_7052C8 == NULL)
    {
        gTileAnimPool_7052C8 = new TileAnimPool();
        if (gTileAnimPool_7052C8 == NULL)
        {
            FatalError_4A38C0(Gta2Error::InvalidMapObjectData, "C:\\Splitting\\Gta2\\Source\\tileanim.cpp", 220);
        }
    }
    field_0_count = 1;
}

MATCH_FUNC(0x5bc470)
TileAnim_2::~TileAnim_2()
{
    if (gTileAnimPool_7052C8)
    {
        GTA2_DELETE_AND_NULL(gTileAnimPool_7052C8);
    }
}

MATCH_FUNC(0x5bc130)
void TileAnim_18::PoolAllocate()
{
    field_12_idx = gTileAnim_2_7052C4->field_0_count++;
}

MATCH_FUNC(0x5bc150)
void TileAnim_18::ApplyCurrentFrame_5BC150()
{
    if (field_C_ptr != NULL)
    {
        gGtx_0x106C_703DD4->SetTileRemap_5AA930(field_10_base, field_C_ptr->field_6_tiles[field_8_current_frame]);
        return;
    }
    gGtx_0x106C_703DD4->SetTileRemap_5AA930(field_10_base, field_8_current_frame);
}

MATCH_FUNC(0x5bc190)
void TileAnim_18::InitFromGmpAnim_5BC190(const gmp_tile_animation* pStru)
{
    field_C_ptr = pStru;
    field_4_frame_rate = pStru->field_2_frame_rate;
    field_6_repeat = pStru->field_3_repeat;
    field_10_base = pStru->field_0_base;
    field_2_anim_length = pStru->field_4_anim_length - 1;
    field_0_start_frame = 0;
}

MATCH_FUNC(0x5bc1d0)
void TileAnim_18::Start_5BC1D0()
{
    field_A_frame_timer = field_4_frame_rate;
    field_8_current_frame = field_0_start_frame;
    ApplyCurrentFrame_5BC150();
}

MATCH_FUNC(0x5bc1f0)
char_type TileAnim_18::PoolUpdate()
{
    u8 bVar3;

    field_A_frame_timer--;
    if (field_A_frame_timer == 0)
    {
        if (field_2_anim_length < field_0_start_frame)
        {
            field_8_current_frame--;
            bVar3 = field_8_current_frame < field_2_anim_length;
        }
        else
        {
            field_8_current_frame++;
            bVar3 = field_8_current_frame > field_2_anim_length;
        }
        if (bVar3)
        {
            if (field_6_repeat > 0 && --field_6_repeat == 0)
            {
                return 1;
            }
            field_8_current_frame = field_0_start_frame;
        }
        ApplyCurrentFrame_5BC150();
        field_A_frame_timer = field_4_frame_rate;
    }
    return 0;
}

MATCH_FUNC(0x5beba0)
TileAnim_18::TileAnim_18()
{
    field_0_start_frame = 0;
    field_2_anim_length = 0;
    field_4_frame_rate = 0;
    field_6_repeat = 0;
    field_8_current_frame = 0;
    field_A_frame_timer = 0;
    field_C_ptr = 0;
    field_10_base = 0;
    field_12_idx = 0;
    mpNext = 0;
}

MATCH_FUNC(0x5bebd0)
TileAnim_18::~TileAnim_18()
{
    field_C_ptr = NULL;
    mpNext = NULL;
}

MATCH_FUNC(0x5bc4a0)
TileAnimPool::~TileAnimPool()
{

}