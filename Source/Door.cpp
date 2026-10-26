#include "Door.hpp"
#include "DoorPool.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnimManager.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(s32, dword_67BBE0, 0x67BBE0);
DEFINE_GLOBAL_ARRAY(DoorAnimInfo, gDoorAnimInfo_67BB38, 5, 0x67BB38);
DEFINE_GLOBAL(DoorPool*, gDoor_4D4_67BD2C, 0x67BD2C);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67BA20, Fix16(0x2000, 0), 0x67BA20);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67BBE4, Fix16(1), 0x67BBE4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_67BBE8, Fix16(2), 0x67BBE8);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67BA38, Ang16(0x168), 0x67BA38);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67BB2C, Ang16(0x438), 0x67BB2C);
DEFINE_GLOBAL(Ang16, DAT_0067BD18, 0x67BD18);

MATCH_FUNC(0x49c640)
Door::Door()
{
    field_0_primary_door_data = 0;
    field_4_secondary_door_data = 0;
    field_8_door_obj = 0;
    field_C_trigger_obj = 0;
    field_20_open_type = 0;
    field_24_close_type = 0;
    field_10_model_id = 0;
    field_18_unused = 0;
    field_28_bClosing = 1;
    field_2C_bCanAutoOpen = 1;
    field_1C_close_delay = 0;
    field_1E_close_timer = 0;
    field_14_target_id = 0;
    field_29_bAuto = 1;
    field_2A_bDoFlip = 0;
    field_2B_bReversed = 0;
    field_30_x = dword_67BBE0;
    field_34_y = dword_67BBE0;
    field_2D_play_open_sound = 0;
}

MATCH_FUNC(0x49c690)
Door::~Door()
{
    field_0_primary_door_data = 0;
    field_4_secondary_door_data = 0;
    field_8_door_obj = 0;
    field_C_trigger_obj = 0;
}

MATCH_FUNC(0x49c6a0)
bool Door::IsSpriteClearOfDoorCollision_49C6A0(Sprite* pSprite)
{
    if (pSprite)
    {
        return field_8_door_obj->field_4_sprite->RotatedRectCollisionSAT_5A0380(pSprite) == false;
    }
    return true;
}

MATCH_FUNC(0x49c6d0)
bool Door::CanOpen_49C6D0(Car* pCar)
{
    bool ret = false;
    switch (field_20_open_type)
    {
        case door_open_type::any_player:
            if (!pCar->IsDrivenByPlayer_4118D0())
            {
                if (ped_type::player_2 != pCar->field_7C_ped_type)
                {
                    break;
                }
            }
            ret = true;
            break;
        case door_open_type::one_car:
            if (this->field_10_car_bc && pCar == this->field_10_car_bc && pCar->field_6C_car_id == this->field_14_target_id)
            {
                ret = true;
                break;
            }

            break;

        case door_open_type::one_model:
            if (pCar->field_84_car_info_idx != this->field_10_model_id)
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
            if (pCar != this->field_10_car_bc)
            {
                break;
            }
            if (pCar->field_6C_car_id != this->field_14_target_id)
            {
                break;
            }

            if (IsSpriteClearOfDoorCollision_49C6A0(pCar->field_50_car_sprite) && (!pCar->IsDrivenByPlayer_4118D0() || gGarage_48_6FD26C->IsParkedCar_44C870(pCar)))
            {
                if (pCar->IsDrivenByPlayer_4118D0())
                {
                    break;
                }
                if (!gGarage_48_6FD26C->IsParkedCar_44C870(pCar))
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
bool Door::CanOpenForPed_49C7F0(Ped* pPed)
{
    bool ret = false;
    switch (field_20_open_type)
    {
        case door_open_type::any_player:
            if (pPed->is_player_41B0A0())
            {
                ret = true;
            }
            break;

        case door_open_type::one_char_on_foot:
            if (field_10_ped)
            {
                if (pPed == field_10_ped)
                {
                    if (pPed->field_200_id == static_cast<u32>(this->field_14_target_id))
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
void Door::Open_49C840()
{
    DoorData* pDoorData = this->field_0_primary_door_data;
    if (pDoorData != NULL)
    {
        if (pDoorData->field_0_state != door_state::open_2)
        {
            this->field_2D_play_open_sound = 1;
        }
        pDoorData->Open_49C4E0(0);
    }
    if (this->field_4_secondary_door_data != NULL)
    {
        this->field_4_secondary_door_data->Open_49C4E0(this->field_2A_bDoFlip);
    }
    this->field_28_bClosing = 0;
}

MATCH_FUNC(0x49c870)
void Door::TryOpenForCar_49C870(Car* pCar)
{
    if (field_29_bAuto)
    {
        if (CanOpen_49C6D0(pCar))
        {
            if (field_2C_bCanAutoOpen)
            {
                Open_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8a0)
void Door::TryOpenForPed_49C8A0(Ped* pPed)
{
    if (field_29_bAuto)
    {
        if (CanOpenForPed_49C7F0(pPed))
        {
            if (field_2C_bCanAutoOpen)
            {
                Open_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8d0)
void Door::InitSingleNoCheck_49C8D0(u8 door_group, u8 gr_id, u8 x, u8 y, u8 z, s32 facing)
{
    Fix16 trigger_x;
    Fix16 trigger_y;

    AddDoorData_49CA50(gr_id, x, y, z, facing);

    Fix16 door_x = Fix16(x) + kFpHalf_67BA20;
    Fix16 door_y = Fix16(y) + kFpHalf_67BA20;

    switch (facing)
    {
        case 1:
            trigger_x = Fix16(x) - kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                door_x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            trigger_x = Fix16(x) + kFpOne_67BBE4 + kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                door_x += kFpOne_67BBE4;
            }
            break;
        case 3:
            trigger_x = Fix16(x) + kFpHalf_67BA20;
            trigger_y = Fix16(y) - kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                door_y -= kFpOne_67BBE4;
            }
            break;
        case 4:
            trigger_x = Fix16(x) + kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpOne_67BBE4 + kFpHalf_67BA20;
            if (this->field_2B_bReversed)
            {
                door_y += kFpOne_67BBE4;
            }
            break;
    }

    field_28_bClosing = 1;
    field_2C_bCanAutoOpen = 1;

    field_1E_close_timer = field_1C_close_delay = (gDoorAnimInfo_67BB38[gr_id].field_2_end_frame - gDoorAnimInfo_67BB38[gr_id].field_0_start_frame) * gDoorAnimInfo_67BB38[gr_id].field_8_speed;

    field_C_trigger_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::door_trigger_167, trigger_x, trigger_y, z, DAT_0067BD18);
    field_C_trigger_obj->set_field_26(door_group);

    field_8_door_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::door_collision_obj_169, door_x, door_y, z, DAT_0067BD18);
    field_8_door_obj->set_field_26(door_group);
    field_30_x = door_x;
    field_34_y = door_y;
}

MATCH_FUNC(0x49ca50)
void Door::AddDoorData_49CA50(u8 gr_id, char_type x, char_type y, char_type z, s32 face)
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
void Door::InitSingle_49CAC0(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h)
{
    Fix16 z(pDoorData->field_6_z);
    Fix16 x = Fix16(pDoorData->field_4_x) + kFpHalf_67BA20;
    Fix16 y = Fix16(pDoorData->field_5_y) + kFpHalf_67BA20;
    switch (pDoorData->field_8_face)
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
    field_28_bClosing = 1;
    field_2C_bCanAutoOpen = 1;
    u16 anim_frames = (gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_2_end_frame - gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_0_start_frame);
    field_1E_close_timer = field_1C_close_delay = anim_frames * gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_8_speed + 0x28;
    if (bFlip)
    {
        field_C_trigger_obj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::door_trigger_167,
                                                                      trigger_x,
                                                                      trigger_y,
                                                                      trigger_z,
                                                                      DAT_0067BD18,
                                                                      trigger_w,
                                                                      trigger_h,
                                                                      kFpOne_67BBE4);
        field_C_trigger_obj->set_field_26(obj_id);
    }
    field_8_door_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::door_collision_obj_169, x, y, z, DAT_0067BD18);
    field_8_door_obj->set_field_26(obj_id);
    field_30_x = x;
    field_34_y = y;
}

MATCH_FUNC(0x49cc00)
void Door::InitDouble_49CC00(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h)
{
    Ang16 door_angle;
    Fix16 z(pDoorData->field_6_z);
    Fix16 x = Fix16(pDoorData->field_4_x) + kFpHalf_67BA20;
    Fix16 y = Fix16(pDoorData->field_5_y) + kFpHalf_67BA20;

    switch (pDoorData->field_8_face)
    {
        case 3:
            x += kFpHalf_67BA20;
            door_angle = kAng270_67BB2C;
            if (this->field_2B_bReversed)
            {
                y -= kFpOne_67BBE4;
            }
            break;
        case 1:
            y -= kFpHalf_67BA20;
            door_angle = DAT_0067BD18;
            if (this->field_2B_bReversed)
            {
                x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            y += kFpHalf_67BA20;
            door_angle = DAT_0067BD18;
            if (this->field_2B_bReversed)
            {
                x += kFpOne_67BBE4;
            }
            break;
        case 4:
            x -= kFpHalf_67BA20;
            door_angle = kAng90_67BA38;
            if (this->field_2B_bReversed)
            {
                y += kFpOne_67BBE4;
            }
            break;
    }
    field_28_bClosing = 1;
    field_2C_bCanAutoOpen = 1;
    u16 anim_frames = (gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_2_end_frame - gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_0_start_frame);
    field_1E_close_timer = field_1C_close_delay = anim_frames * gDoorAnimInfo_67BB38[pDoorData->field_7_gr_id].field_8_speed + 0x28;
    if (bFlip)
    {
        field_C_trigger_obj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::door_trigger_167,
                                                                      trigger_x,
                                                                      trigger_y,
                                                                      trigger_z,
                                                                      DAT_0067BD18,
                                                                      trigger_w,
                                                                      trigger_h,
                                                                      kFpOne_67BBE4);
        field_C_trigger_obj->set_field_26(obj_id);
    }
    field_8_door_obj =
        gObject_5C_6F8F84
            ->NewTouchPoint_529950(objects::door_collision_obj_169, x, y, z, door_angle, kFpOne_67BBE4, kFpTwo_67BBE8, kFpOne_67BBE4);
    field_8_door_obj->set_field_26(obj_id);
    field_30_x = x;
    field_34_y = y;
}

MATCH_FUNC(0x49cd90)
void Door::UpdateAutoClose_49CD90()
{
    if (field_29_bAuto != 0)
    {
        if ((field_0_primary_door_data != NULL && field_0_primary_door_data->field_0_state == door_state::open_2) || (field_4_secondary_door_data != NULL && field_4_secondary_door_data->field_0_state == door_state::open_2))
        {
            if (field_24_close_type != door_close_type::close_never && field_24_close_type != door_close_type::unknown1)
            {
                if (field_24_close_type == door_close_type::close_when_open_rule_fails && field_10_car_bc != NULL)
                {
                    if (CanOpen_49C6D0(field_10_car_bc) == false && CanOpenForPed_49C7F0(field_10_ped) == false &&
                        !IsSpriteClearOfDoorCollision_49C6A0(field_10_car_bc->field_50_car_sprite))
                    {
                        field_28_bClosing = 1;
                        field_1E_close_timer = 0;
                    }
                }
                if (field_28_bClosing != 0)
                {
                    if (field_1E_close_timer > 0)
                    {
                        field_1E_close_timer--;
                    }
                }
                if (field_28_bClosing != '\0' && field_1E_close_timer == 0)
                {
                    field_1E_close_timer = field_1C_close_delay;
                    if (field_0_primary_door_data != NULL)
                    {
                        gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(0x117, 0x33, field_30_x, field_34_y, field_0_primary_door_data->field_6_z);
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
bool Door::Service_49CE90()
{
    if (field_2D_play_open_sound)
    {
        gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(0x117, 0x32, field_30_x, field_34_y, field_0_primary_door_data->field_6_z);
        field_2D_play_open_sound = 0;
    }

    if (field_29_bAuto)
    {
        UpdateAutoClose_49CD90();
    }
    field_28_bClosing = 1;
    return 0;
}

MATCH_FUNC(0x49CEE0)
void Door::get_door_xyz_face_49CEE0(u8* pX, u8* pY, u8* pZ, u32* pFace)
{
    *pX = field_0_primary_door_data->field_4_x;
    *pY = field_0_primary_door_data->field_5_y;
    *pZ = field_0_primary_door_data->field_6_z;
    *pFace = field_0_primary_door_data->field_8_face;
}
