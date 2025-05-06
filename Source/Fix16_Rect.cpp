#include "Fix16_Rect.hpp"
#include "map_0x370.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

MATCH_FUNC(0x59dd60)
void Fix16_Rect::DoSetCurrentRect_59DD60()
{
    gPurple_left_6F5FD4 = field_0_left.ToInt();
    gPurple_right_6F5B80 = field_4_right.ToInt();
    gPurple_top_6F6108 = field_8_top.ToInt();
    gPurple_bottom_6F5F38 = field_C_bottom.ToInt();
}

MATCH_FUNC(0x59ddf0)
u32 Fix16_Rect::sub_59DDF0(Sprite* a2)
{
    Car_8* v3 = a2->field_C_sprite_4c_ptr->field_C_b_box;

    if (inline_sub_4B9FD0(&v3[0]) || inline_sub_4B9FD0(&v3[1]) 
        || inline_sub_4B9FD0(&v3[2]) || inline_sub_4B9FD0(&v3[3]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

STUB_FUNC(0x59de80)
char_type Fix16_Rect::sub_59DE80()
{
    DoSetCurrentRect_59DD60();
    return gMap_0x370_6F6268->sub_4E4460(
             ((this->field_0_left + this->field_4_right) / 2).ToInt(),
             ((this->field_8_top + this->field_C_bottom) / 2).ToInt(),
             ((this->field_10 + this->field_14) / 2).ToInt(),
             0,
             1024);
}

MATCH_FUNC(0x59dee0)
void Fix16_Rect::sub_59DEE0(Fix16 a2, Fix16 a3)
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
bool Fix16_Rect::sub_59DF20(u8 zone_type)
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
bool Fix16_Rect::sub_59DFB0(s32 a2)
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

            gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x_pos, y_pos, field_10.ToInt());
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
