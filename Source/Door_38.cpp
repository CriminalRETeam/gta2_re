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
DEFINE_GLOBAL_ARRAY(DoorAnimInfo_A, gDoorAnimInfo_67BB38, 5, 0x67BB38);
DEFINE_GLOBAL(Door_4D4*, gDoor_4D4_67BD2C, 0x67BD2C);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67BA20, Fix16(0x2000, 0), 0x67BA20);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67BBE4, Fix16(0x4000, 0), 0x67BBE4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_67BBE8, Fix16(0x8000, 0), 0x67BBE8);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67BA38, Ang16(0x168), 0x67BA38);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67BB2C, Ang16(0x438), 0x67BB2C);
DEFINE_GLOBAL(Ang16, DAT_0067BD18, 0x67BD18);

MATCH_FUNC(0x49c640)
Door_38::Door_38()
{
    field_0_primary_door_data = 0;
    field_4_secondary_door_data = 0;
    field_8_door_obj = 0;
    field_C_trigger_obj = 0;
    field_20_state = 0;
    field_24_close_type = 0;
    field_10_model_id = 0;
    field_18 = 0;
    field_28 = 1;
    field_2C = 1;
    field_1C_close_delay = 0;
    field_1E_close_timer = 0;
    field_14_target_id = 0;
    field_29_bAuto = 1;
    field_2A_bDoFlip = 0;
    field_2B_bReversed = 0;
    field_30_x = dword_67BBE0;
    field_34_y = dword_67BBE0;
    field_2D = 0;
}

MATCH_FUNC(0x49c690)
Door_38::~Door_38()
{
    field_0_primary_door_data = 0;
    field_4_secondary_door_data = 0;
    field_8_door_obj = 0;
    field_C_trigger_obj = 0;
}

MATCH_FUNC(0x49c6a0)
bool Door_38::IsSpriteClearOfDoorCollision_49C6A0(Sprite* a1)
{
    if (a1)
    {
        return field_8_door_obj->field_4->RotatedRectCollisionSAT_5A0380(a1) == false;
    }
    return true;
}

MATCH_FUNC(0x49c6d0)
bool Door_38::CanOpen_49C6D0(Car_BC* a2)
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
            if (this->field_10_car_bc && a2 == this->field_10_car_bc && a2->field_6C_maybe_id == this->field_14_target_id)
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
            if (a2->field_6C_maybe_id != this->field_14_target_id)
            {
                break;
            }

            if (IsSpriteClearOfDoorCollision_49C6A0(a2->field_50_car_sprite) && (!a2->is_driven_by_player() || gGarage_48_6FD26C->sub_44C870(a2)))
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

MATCH_FUNC(0x49c7f0)
bool Door_38::CanOpenForPed_49C7F0(Ped* a2)
{
    bool ret = false;
    switch (field_20_state)
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
                    if (a2->field_200_id == this->field_14_target_id)
                    {
                        ret = true;
                    }
                }
            }
            break;
    }
    return ret;
}

MATCH_FUNC(0x49c840)
void Door_38::Open_49C840()
{
    DoorData_10* this_00 = this->field_0_primary_door_data;
    if (this_00 != NULL)
    {
        if (this_00->field_0_state != 2)
        {
            this->field_2D = 1;
        }
        this_00->Open_49C4E0(0);
    }
    if (this->field_4_secondary_door_data != NULL)
    {
        this->field_4_secondary_door_data->Open_49C4E0(this->field_2A_bDoFlip);
    }
    this->field_28 = 0;
}

MATCH_FUNC(0x49c870)
void Door_38::TryOpenForCar_49C870(Car_BC* a2)
{
    if (field_29_bAuto)
    {
        if (CanOpen_49C6D0(a2))
        {
            if (field_2C)
            {
                Open_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8a0)
void Door_38::TryOpenForPed_49C8A0(Ped* a2)
{
    if (field_29_bAuto)
    {
        if (CanOpenForPed_49C7F0(a2))
        {
            if (field_2C)
            {
                Open_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8d0)
void Door_38::InitSingleNoCheck_49C8D0(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, s32 a6)
{
    Fix16 iVar5;
    Fix16 iVar8;

    AddDoorData_49CA50(a2, a3, a4, a5, a6);

    Fix16 _param_4 = Fix16(a3) + kFpHalf_67BA20;
    Fix16 iVar6 = Fix16(a4) + kFpHalf_67BA20;

    switch (a6)
    {
        case 1:
            iVar5 = Fix16(a3) - kFpHalf_67BA20;
            iVar8 = Fix16(a4) + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                _param_4 -= kFpOne_67BBE4;
            }
            break;
        case 2:
            iVar5 = Fix16(a3) + kFpOne_67BBE4 + kFpHalf_67BA20;
            iVar8 = Fix16(a4) + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                _param_4 += kFpOne_67BBE4;
            }
            break;
        case 3:
            iVar5 = Fix16(a3) + kFpHalf_67BA20;
            iVar8 = Fix16(a4) - kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                iVar6 -= kFpOne_67BBE4;
            }
            break;
        case 4:
            iVar5 = Fix16(a3) + kFpHalf_67BA20;
            iVar8 = Fix16(a4) + kFpOne_67BBE4 + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                iVar6 += kFpOne_67BBE4;
            }
            break;
    }

    field_28 = 1;
    field_2C = 1;

    field_1E_close_timer = field_1C_close_delay = (gDoorAnimInfo_67BB38[a2].field_2_end_frame - gDoorAnimInfo_67BB38[a2].field_0_start_frame) * gDoorAnimInfo_67BB38[a2].field_8_speed;

    field_C_trigger_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(0xa7, iVar5, iVar8, a5, DAT_0067BD18);
    field_C_trigger_obj->set_field_26(a1);

    field_8_door_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(0xa9, _param_4, iVar6, a5, DAT_0067BD18);
    field_8_door_obj->set_field_26(a1);
    field_30_x = _param_4;
    field_34_y = iVar6;
}

MATCH_FUNC(0x49ca50)
void Door_38::AddDoorData_49CA50(u8 gr_id, char_type x, char_type y, char_type z, s32 face)
{
    if (!field_0_primary_door_data)
    {
        field_0_primary_door_data = gDoor_4D4_67BD2C->AllocDoorData_49CF10(gr_id, x, y, z, face, 0);
        return;
    }

    if (!field_4_secondary_door_data)
    {
        field_4_secondary_door_data = gDoor_4D4_67BD2C->AllocDoorData_49CF10(gr_id, x, y, z, face, field_2A_bDoFlip);
    }
}

MATCH_FUNC(0x49cac0)
void Door_38::InitSingle_49CAC0(DoorData_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Fix16 z(a1->field_6_z);
    Fix16 x = Fix16(a1->field_4_x) + kFpHalf_67BA20;
    Fix16 y = Fix16(a1->field_5_y) + kFpHalf_67BA20;
    switch (a1->field_8_face)
    {
        case 1:
            if (this->field_2B_bReversed)
            {
                x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            if (this->field_2B_bReversed)
            {
                x += kFpOne_67BBE4;
            }
            break;
        case 3:
            if (this->field_2B_bReversed)
            {
                y -= kFpOne_67BBE4;
            }
            break;
        case 4:
            if (this->field_2B_bReversed)
            {
                y += kFpOne_67BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_2_end_frame - gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_0_start_frame);
    field_1E_close_timer = field_1C_close_delay = sVar1 * gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_8_speed + 0x28;
    if (a2)
    {
        field_C_trigger_obj = gObject_5C_6F8F84->NewTouchPoint_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, kFpOne_67BBE4);
        field_C_trigger_obj->set_field_26(a3);
    }
    field_8_door_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(0xa9, x, y, z, DAT_0067BD18);
    field_8_door_obj->set_field_26(a3);
    field_30_x = x;
    field_34_y = y;
}

MATCH_FUNC(0x49cc00)
void Door_38::InitDouble_49CC00(DoorData_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Ang16 local_c;
    Fix16 z(a1->field_6_z);
    Fix16 x = Fix16(a1->field_4_x) + kFpHalf_67BA20;
    Fix16 y = Fix16(a1->field_5_y) + kFpHalf_67BA20;

    switch (a1->field_8_face)
    {
        case 3:
            x += kFpHalf_67BA20;
            local_c = kAng270_67BB2C;
            if (this->field_2B_bReversed)
            {
                y -= kFpOne_67BBE4;
            }
            break;
        case 1:
            y -= kFpHalf_67BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B_bReversed)
            {
                x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            y += kFpHalf_67BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B_bReversed)
            {
                x += kFpOne_67BBE4;
            }
            break;
        case 4:
            x -= kFpHalf_67BA20;
            local_c = kAng90_67BA38;
            if (this->field_2B_bReversed)
            {
                y += kFpOne_67BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_2_end_frame - gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_0_start_frame);
    field_1E_close_timer = field_1C_close_delay = sVar1 * gDoorAnimInfo_67BB38[a1->field_7_gr_id].field_8_speed + 0x28;
    if (a2)
    {
        field_C_trigger_obj = gObject_5C_6F8F84->NewTouchPoint_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, kFpOne_67BBE4);
        field_C_trigger_obj->set_field_26(a3);
    }
    field_8_door_obj = gObject_5C_6F8F84->NewTouchPoint_529950(0xa9, x, y, z, local_c, kFpOne_67BBE4, kFpTwo_67BBE8, kFpOne_67BBE4);
    field_8_door_obj->set_field_26(a3);
    field_30_x = x;
    field_34_y = y;
}

MATCH_FUNC(0x49cd90)
void Door_38::UpdateAutoClose_49CD90()
{
    if (field_29_bAuto != 0)
    {
        if ((field_0_primary_door_data != NULL && field_0_primary_door_data->field_0_state == 2) || (field_4_secondary_door_data != NULL && field_4_secondary_door_data->field_0_state == 2))
        {
            if (field_24_close_type != door_close_type::close_never && field_24_close_type != door_close_type::unknown1)
            {
                if (field_24_close_type == door_close_type::close_when_open_rule_fails && field_10_car_bc != NULL)
                {
                    if (CanOpen_49C6D0(field_10_car_bc) == false && CanOpenForPed_49C7F0(field_10_ped) == false &&
                        !IsSpriteClearOfDoorCollision_49C6A0(field_10_car_bc->field_50_car_sprite))
                    {
                        field_28 = 1;
                        field_1E_close_timer = 0;
                    }
                }
                if (field_28 != 0)
                {
                    if (field_1E_close_timer > 0)
                    {
                        field_1E_close_timer--;
                    }
                }
                if (field_28 != '\0' && field_1E_close_timer == 0)
                {
                    field_1E_close_timer = field_1C_close_delay;
                    if (field_0_primary_door_data != NULL)
                    {
                        gObject_5C_6F8F84->sub_5299F0(0x117, 0x33, field_30_x, field_34_y, field_0_primary_door_data->field_6_z);
                        field_0_primary_door_data->Close_49C590(false);
                    }
                    if (field_4_secondary_door_data != NULL)
                    {
                        field_4_secondary_door_data->Close_49C590(field_2A_bDoFlip);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x49ce90)
char_type Door_38::Service_49CE90()
{
    if (field_2D)
    {
        gObject_5C_6F8F84->sub_5299F0(0x117, 0x32, field_30_x, field_34_y, field_0_primary_door_data->field_6_z);
        field_2D = 0;
    }

    if (field_29_bAuto)
    {
        UpdateAutoClose_49CD90();
    }
    field_28 = 1;
    return 0;
}

MATCH_FUNC(0x49CEE0)
void Door_38::get_door_xyz_face_49CEE0(u8* pX, u8* pY, u8* pZ, u32* pFace)
{
    *pX = field_0_primary_door_data->field_4_x;
    *pY = field_0_primary_door_data->field_5_y;
    *pZ = field_0_primary_door_data->field_6_z;
    *pFace = field_0_primary_door_data->field_8_face;
}
