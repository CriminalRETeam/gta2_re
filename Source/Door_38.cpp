#include "Door_38.hpp"
#include "Door_4D4.hpp"
#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnim_2.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(s32, dword_67BBE0, 0x67BBE0);
DEFINE_GLOBAL_ARRAY(Door_A, word_67BB38, 5, 0x67BB38);
DEFINE_GLOBAL(Door_4D4*, gDoor_4D4_67BD2C, 0x67BD2C);
DEFINE_GLOBAL(Fix16, DAT_0067BA20, 0x67BA20);
DEFINE_GLOBAL(Fix16, DAT_0067BBE4, 0x67BBE4);
DEFINE_GLOBAL(Fix16, DAT_0067BBE8, 0x67BBE8);
DEFINE_GLOBAL(Ang16, DAT_0067BA38, 0x67BA38);
DEFINE_GLOBAL(Ang16, DAT_0067BB2C, 0x67BB2C);
DEFINE_GLOBAL(Ang16, DAT_0067BD18, 0x67BD18);

// FUNCTION: 105 0x49c640
Door_38::Door_38()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_C = 0;
    field_20_state = 0;
    field_24 = 0;
    field_10_model_id = 0;
    field_18 = 0;
    field_28 = 1;
    field_2C = 1;
    field_1C = 0;
    field_1E = 0;
    field_14 = 0;
    field_29 = 1;
    field_2A = 0;
    field_2B = 0;
    field_30 = dword_67BBE0;
    field_34 = dword_67BBE0;
    field_2D = 0;
}

// FUNCTION: 105 0x49c690
Door_38::~Door_38()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_C = 0;
}

// FUNCTION: 105 0x49c6a0
bool Door_38::sub_49C6A0(Sprite* a1)
{
    if (a1)
    {
        return field_8->field_4->sub_5A0380(a1) == false;
    }
    return true;
}

// FUNCTION: 105 0x49c6d0
bool Door_38::sub_49C6D0(Car_BC* a2)
{
    bool ret = false;
    switch (field_20_state)
    {
        case door_open_type::any_player:
            if (!a2->is_driven_by_player())
            {
                if (ped_type_enum::ped_player != a2->field_7C_uni_num)
                {
                    break;
                }
            }
            ret = true;
            break;
        case door_open_type::one_car:
            if (this->field_10_car_bc && a2 == this->field_10_car_bc && a2->field_6C_maybe_id == this->field_14)
            {
                ret = true;
                break;
            }

            break;

        case door_open_type::one_model:
            if (a2->field_84_car_info_idx != this->field_10_model_id)
            {
                break;
            }
            ret = true;
            break;

        case door_open_type::any_player_one_car:
            if (this->field_10_car_bc == NULL)
            {
                break;
            }
            if (a2 != this->field_10_car_bc)
            {
                break;
            }
            if (a2->field_6C_maybe_id != this->field_14)
            {
                break;
            }

            if (sub_49C6A0(a2->field_50_car_sprite) && (!a2->is_driven_by_player() || gGarage_48_6FD26C->sub_44C870(a2)))
            {
                if (a2->is_driven_by_player())
                {
                    break;
                }
                if (!gGarage_48_6FD26C->sub_44C870(a2))
                {
                    break;
                }
            }
            ret = true;
            break;
        case door_open_type::any_car:
            ret = true;
            break;
    }
    return ret;
}

// FUNCTION: 105 0x49c7f0
bool Door_38::sub_49C7F0(Ped* a2)
{
    bool ret = false;
    switch(field_20_state)
    {
        case 1:
            if (a2->field_15C_player != 0)
            {
                ret = true;
            }
        break;

        case 5:
            if (field_10_ped)
            {
                if (a2 == field_10_ped)
                {
                    if ( a2->field_200_id == this->field_14)
                    {
                        ret = true;
                    }
                }
            }
        break;
    }
    return ret;
}

// FUNCTION: 105 0x49c840
void Door_38::sub_49C840()
{
    Door_10* this_00 = this->field_0;
    if (this_00 != NULL)
    {
        if (this_00->field_0 != 2)
        {
            this->field_2D = 1;
        }
        this_00->sub_49C4E0(0);
    }
    if (this->field_4 != NULL)
    {
        this->field_4->sub_49C4E0(this->field_2A);
    }
    this->field_28 = 0;
}

// FUNCTION: 105 0x49c870
void Door_38::sub_49C870(Car_BC* a2)
{
    if (field_29)
    {
        if (sub_49C6D0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

// FUNCTION: 105 0x49c8a0
void Door_38::sub_49C8A0(Ped* a2)
{
    if (field_29)
    {
        if (sub_49C7F0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

// FUNCTION: 105 0x49c8d0
void Door_38::sub_49C8D0(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, s32 a6)
{
    Fix16 iVar5;
    Fix16 iVar8;

    sub_49CA50(a2, a3, a4, a5, a6);

    Fix16 _param_4 = Fix16(a3) + DAT_0067BA20;
    Fix16 iVar6 = Fix16(a4) + DAT_0067BA20;

    switch (a6)
    {
        case 1:
            iVar5 = Fix16(a3) - DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BA20;
            if (this->field_2B)
            {
                _param_4 -= DAT_0067BBE4;
            }
            break;
        case 2:
            iVar5 = Fix16(a3) + DAT_0067BBE4 + DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BA20;
            if (this->field_2B)
            {
                _param_4 += DAT_0067BBE4;
            }
            break;
        case 3:
            iVar5 = Fix16(a3) + DAT_0067BA20;
            iVar8 = Fix16(a4) - DAT_0067BA20;
            if (this->field_2B)
            {
                iVar6 -= DAT_0067BBE4;
            }
            break;
        case 4:
            iVar5 = Fix16(a3) + DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BBE4 + DAT_0067BA20;
            if (this->field_2B)
            {
                iVar6 += DAT_0067BBE4;
            }
            break;
    }

    field_28 = 1;
    field_2C = 1;

    field_1E = field_1C = (word_67BB38[a2].field_2_end_frame - word_67BB38[a2].field_0_start_frame) * word_67BB38[a2].field_8_speed;

    field_C = gObject_5C_6F8F84->sub_5299B0(0xa7, iVar5, iVar8, a5, DAT_0067BD18);
    field_C->set_field_26(a1);

    field_8 = gObject_5C_6F8F84->sub_5299B0(0xa9, _param_4, iVar6, a5, DAT_0067BD18);
    field_8->set_field_26(a1);
    field_30 = _param_4;
    field_34 = iVar6;
}

// FUNCTION: 105 0x49ca50
void Door_38::sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5)
{
    if (!field_0)
    {
        field_0 = gDoor_4D4_67BD2C->sub_49CF10(a1, a2, a3, a4, a5, 0);
        return;
    }

    if (!field_4)
    {
        field_4 = gDoor_4D4_67BD2C->sub_49CF10(a1, a2, a3, a4, a5, field_2A);
    }
}

// FUNCTION: 105 0x49cac0
void Door_38::sub_49CAC0(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Fix16 z(a1->field_6_z);
    Fix16 x = Fix16(a1->field_4_x) + DAT_0067BA20;
    Fix16 y = Fix16(a1->field_5_y) + DAT_0067BA20;
    switch (a1->field_8_face)
    {
        case 1:
            if (this->field_2B)
            {
                x -= DAT_0067BBE4;
            }
            break;
        case 2:
            if (this->field_2B)
            {
                x += DAT_0067BBE4;
            }
            break;
        case 3:
            if (this->field_2B)
            {
                y -= DAT_0067BBE4;
            }
            break;
        case 4:
            if (this->field_2B)
            {
                y += DAT_0067BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (word_67BB38[a1->field_7_gr_id].field_2_end_frame - word_67BB38[a1->field_7_gr_id].field_0_start_frame);
    field_1E = field_1C = sVar1 * word_67BB38[a1->field_7_gr_id].field_8_speed + 0x28;
    if (a2)
    {
        field_C = gObject_5C_6F8F84->sub_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, DAT_0067BBE4);
        field_C->set_field_26(a3);
    }
    field_8 = gObject_5C_6F8F84->sub_5299B0(0xa9, x, y, z, DAT_0067BD18);
    field_8->set_field_26(a3);
    field_30 = x;
    field_34 = y;
}

// FUNCTION: 105 0x49cc00
void Door_38::sub_49CC00(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Ang16 local_c;
    Fix16 z(a1->field_6_z);
    Fix16 x = Fix16(a1->field_4_x) + DAT_0067BA20;
    Fix16 y = Fix16(a1->field_5_y) + DAT_0067BA20;

    switch (a1->field_8_face)
    {
        case 3:
            x += DAT_0067BA20;
            local_c = DAT_0067BB2C;
            if (this->field_2B)
            {
                y -= DAT_0067BBE4;
            }
            break;
        case 1:
            y -= DAT_0067BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B)
            {
                x -= DAT_0067BBE4;
            }
            break;
        case 2:
            y += DAT_0067BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B)
            {
                x += DAT_0067BBE4;
            }
            break;
        case 4:
            x -= DAT_0067BA20;
            local_c = DAT_0067BA38;
            if (this->field_2B)
            {
                y += DAT_0067BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (word_67BB38[a1->field_7_gr_id].field_2_end_frame - word_67BB38[a1->field_7_gr_id].field_0_start_frame);
    field_1E = field_1C = sVar1 * word_67BB38[a1->field_7_gr_id].field_8_speed + 0x28;
    if (a2)
    {
        field_C = gObject_5C_6F8F84->sub_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, DAT_0067BBE4);
        field_C->set_field_26(a3);
    }
    field_8 = gObject_5C_6F8F84->sub_529950(0xa9, x, y, z, local_c, DAT_0067BBE4, DAT_0067BBE8, DAT_0067BBE4);
    field_8->set_field_26(a3);
    field_30 = x;
    field_34 = y;
}

// FUNCTION: 105 0x49cd90
void Door_38::sub_49CD90()
{
    if (field_29 != 0)
    {
        if ((field_0 != NULL && field_0->field_0 == 2) || (field_4 != NULL && field_4->field_0 == 2))
        {
            if (field_24 != door_close_type::close_never && field_24 != door_close_type::unknown1)
            {
                if (field_24 == door_close_type::close_when_open_rule_fails && field_10_car_bc != NULL)
                {
                    if (sub_49C6D0(field_10_car_bc) == false && sub_49C7F0(field_10_ped) == false &&
                        !sub_49C6A0(field_10_car_bc->field_50_car_sprite))
                    {
                        field_28 = 1;
                        field_1E = 0;
                    }
                }
                if (field_28 != 0)
                {
                    if (field_1E > 0)
                    {
                        field_1E--;
                    }
                }
                if (field_28 != '\0' && field_1E == 0)
                {
                    field_1E = field_1C;
                    if (field_0 != NULL)
                    {
                        gObject_5C_6F8F84->sub_5299F0(0x117, 0x33, field_30, field_34, field_0->field_6_z);
                        field_0->sub_49C590(0);
                    }
                    if (field_4 != NULL)
                    {
                        field_4->sub_49C590(field_2A);
                    }
                }
            }
        }
    }
}

// FUNCTION: 105 0x49ce90
char_type Door_38::sub_49CE90()
{
    if (field_2D)
    {
        gObject_5C_6F8F84->sub_5299F0(0x117, 0x32, field_30, field_34, field_0->field_6_z);
        field_2D = 0;
    }

    if (field_29)
    {
        sub_49CD90();
    }
    field_28 = 1;
    return 0;
}