#include "Fix16_Rect.hpp"
#include "map_0x370.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL_INIT(Fix16, kSmallWidthEpslion_703450, Fix16(0xCCC, 0), 0x703450);

MATCH_FUNC(0x59dd60)
void Fix16_Rect::DoSetCurrentRect_59DD60()
{
    gPurple_left_6F5FD4 = field_0_left.ToInt();
    gPurple_right_6F5B80 = field_4_right.ToInt();
    gPurple_top_6F6108 = field_8_top.ToInt();
    gPurple_bottom_6F5F38 = field_C_bottom.ToInt();
}

// 0x4BA6C0 9.6f
MATCH_FUNC(0x59ddf0)
u32 Fix16_Rect::IntersectsSpriteRenderingRect_59DDF0(Sprite* a2)
{
    Fix16_Point* v3 = a2->field_C_sprite_4c_ptr->field_C_renderingRect;

    if (IsPointInRect_4B9FD0(&v3[0]) || IsPointInRect_4B9FD0(&v3[1]) 
        || IsPointInRect_4B9FD0(&v3[2]) || IsPointInRect_4B9FD0(&v3[3]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

WIP_FUNC(0x59de80)
char_type Fix16_Rect::CanRectEnterMovementRegion_59DE80()
{
    WIP_IMPLEMENTED;
    DoSetCurrentRect_59DD60();
    return gMap_0x370_6F6268->CanSpriteEnterMovementRegion_4E4460(
             ((this->field_0_left + this->field_4_right) / 2).ToInt(),
             ((this->field_8_top + this->field_C_bottom) / 2).ToInt(),
             ((this->field_10_low_z + this->field_14_high_z) / 2).ToInt(),
             0,
             1024);
}

MATCH_FUNC(0x59dee0)
void Fix16_Rect::ExpandToIncludePoint_59DEE0(Fix16 a2, Fix16 a3)
{
    if (a2 < field_0_left)
    {
        field_0_left = a2;
    }
    else
    {
        if (a2 > field_4_right)
        {
            field_4_right = a2;
        }
    }

    if (a3 < field_8_top)
    {
        field_8_top = a3;
    }
    else
    {
        if (a3 > field_C_bottom)
        {
            field_C_bottom = a3;
        }
    }
}

MATCH_FUNC(0x59df20)
bool Fix16_Rect::RectOverlapsZone_59DF20(u8 zone_type)
{
    u8 zone_y = field_8_top.ToUInt8();
    u8 zone_x;

    while (zone_y <= field_C_bottom.ToUInt8())
    {
        zone_x = field_0_left.ToUInt8();
        while (zone_x <= field_4_right.ToUInt8())
        {
            if (gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(zone_x, zone_y, zone_type))
            {
                return true;
            }
            ++zone_x;
        }
        ++zone_y;
    }
    return false;
}

MATCH_FUNC(0x59dfb0)
bool Fix16_Rect::EveryTileMatchesArrowType_59DFB0(s32 a2)
{
    u8 y_pos = field_8_top.ToUInt8();
    u8 x_pos;

    bool v3 = true;

    while (y_pos <= field_C_bottom.ToUInt8())
    {
        x_pos = field_0_left.ToUInt8();
        while (x_pos <= field_4_right.ToUInt8())
        {
            if (!v3)
            {
                return false;
            }

            gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x_pos, y_pos, field_10_low_z.ToInt());
            if (block_4DFE10)
            {
                
                switch (a2)
                {
                    case 1:
                        v3 = (block_4DFE10->field_A_arrows & 0xF) == 4;  //  green arrow up
                        break;
                    case 2:
                        v3 = (block_4DFE10->field_A_arrows & 0xF) == 8;  //  green arrow down
                        break;
                    case 3:
                        v3 = (block_4DFE10->field_A_arrows & 0xF) == 2;  //  green arrow right
                        break;
                    case 4:
                        v3 = (block_4DFE10->field_A_arrows & 0xF) == 1;  //  green arrow left
                        break;
                    default:
                        break;
                }
            }
            else
            {
                v3 = false;
            }
            ++x_pos;
        }
        ++y_pos;
    }
    return true;
}

MATCH_FUNC(0x5A5E30)
void Fix16_Rect::SetRect_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom)
{
    this->field_0_left = left;
    this->field_4_right = right;
    this->field_8_top = top;
    this->field_C_bottom = bottom;
}