#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Door_38.hpp"
#include "Door_4D4.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "char.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(Ang16, word_6FD25C);
EXTERN_GLOBAL(Ang16, word_6FCFB0);
EXTERN_GLOBAL(Ang16, word_6FD07E);
EXTERN_GLOBAL(Ang16, word_6FD0A4);

DEFINE_GLOBAL(Garage_48*, gGarage_48_6FD26C, 0x6FD26C);

DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FD128, Fix16(0x8000, 0), 0x6FD128);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FCF88, Fix16(0x1000, 0), 0x6FCF88);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FD04C, kFpQuarter_6FCF88, 0x6FD04C);
DEFINE_GLOBAL_INIT(Fix16, kFpEighth_6FCF60, kFpQuarter_6FD04C / kFpTwo_6FD128, 0x6FCF60);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD0D8, Fix16(0x100, 0), 0x6FD0D8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD1D8, kFpOne64th_6FD0D8, 0x6FD1D8);

MATCH_FUNC(0x4bbc60)
Garage_48::~Garage_48()
{
}

// The heading for a ped leaving through a door facing `face`.
MATCH_FUNC(0x5345E0)
EXPORT Ang16 __stdcall sub_5345E0(s32 face)
{
    switch (face)
    {
        case 4:
            return word_6FD25C;
        case 2:
            return word_6FCFB0;
        case 3:
            return word_6FD07E;
        case 1:
            return word_6FD0A4;
    }
    return word_6FD25C;
}

DEFINE_GLOBAL_INIT(Fix16, dword_6FD124, Fix16(1), 0x6FD124);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCF98, Fix16(0.5), 0x6FCF98);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD218, Fix16(0x3333, 0), 0x6FD218);

// Car_BC::IsLongerThanOneBlock_447ED0 (9.6f), with this file's copy of the 1.0 constant
static inline bool IsLongerThanOneBlock_447ED0(Car_BC* pCar)
{
    return (pCar->field_50_car_sprite->GetH_447E70() > dword_6FD124) ? true : false;
}

// 9.6f 0x489B10
MATCH_FUNC(0x534650)
void Garage_48::ValidateParkCommand_534650()
{
    // The result is unused (the out-of-line Add_408660 on a const Fix16)
    {
        const Fix16 v4(this->field_10->field_0_primary_door_data->field_6_z);
        v4.Add_408660(kFpEighth_6FCF60);
    }

    if (gMap_0x370_6F6268->HasWallInArea_4E18A0(field_18_park_x_min.ToInt(),
                                      (field_20_park_x_max - kFpOne64th_6FD1D8).ToInt(),
                                      field_1C_park_y_min.ToInt(),
                                      (field_24_park_y_max - kFpOne64th_6FD1D8).ToInt(),
                                      field_10->field_0_primary_door_data->get_z_489640()))
    {
        FatalError_4A38C0(Gta2Error::ErrorInSetupOfParkCommand,
                          "C:\\Splitting\\Gta2\\Source\\park.cpp",
                          62,
                          this->field_10->field_0_primary_door_data->get_x_489620(),
                          this->field_10->field_0_primary_door_data->get_y_489630(),
                          this->field_10->field_0_primary_door_data->get_z_489640());
    }
}

DEFINE_GLOBAL(Fix16, dword_6FD120, 0x6FD120);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCF10, Fix16(0x666, 0), 0x6FCF10);

DEFINE_GLOBAL_INIT(Ang16, word_6FCFB0, Ang16(360), 0x6FCFB0);
DEFINE_GLOBAL_INIT(Ang16, word_6FD07E, Ang16(720), 0x6FD07E);
DEFINE_GLOBAL_INIT(Ang16, word_6FD0A4, Ang16(1080), 0x6FD0A4);
DEFINE_GLOBAL_INIT(Ang16, word_6FD25C, Ang16(0), 0x6FD25C);

WIP_FUNC(0x534700)
u8 Garage_48::ParkCarAtDoor_534700(Car_BC* pCar, Door_38* pDoor)
{
    field_0 = pCar;
    field_44 = 0;
    field_10 = pDoor;
    field_C = 1;
    field_14 = 0;
    field_3C = 30;
    field_3D = 0;
    if (field_3E == 255)
    {
        field_3E = 1;
    }
    else
    {
        field_3E++;
    }

    field_40 = IsLongerThanOneBlock_447ED0(pCar);

    u8 x;
    u8 y;
    u8 z;
    field_10->get_door_xyz_face_49CEE0(&x, &y, &z, (u32*)&field_38);

    Fix16 w1;
    Fix16 w2;
    if (field_40)
    {
        // Assigned separately: `w2 = w1` lets VC6 merge the two and lose the second register (ebp)
        w1 = kFpTwo_6FD128;
        w2 = kFpTwo_6FD128;
    }
    else
    {
        w1 = dword_6FD124;
        w2 = dword_6FD124;
    }

    // 9.6f: the park rectangle corners (field_18/1C, field_20/24) and the target (field_30/34) are
    // Fix16 pairs set with Fix16_Point_POD::SetXY_432860
    switch (field_38)
    {
        case 1:
            if (field_10->IsDoubleDoor_489600())
            {
                ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x), Fix16(y) - dword_6FD124 - w1);
            }
            else
            {
                ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x), Fix16(y) - w1);
            }
            ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + dword_6FD124 + w2, Fix16(y) + dword_6FD124 + w1);
            ((Fix16_Point_POD*)&field_30_target_x)->SetXY_432860(Fix16(x) - dword_6FD218, Fix16(y) + dword_6FCF98);
            break;
        case 2:
            ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x) - w2, Fix16(y) - w1);
            if (field_10->IsDoubleDoor_489600())
            {
                ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + dword_6FD124, Fix16(y) + kFpTwo_6FD128 + w1);
            }
            else
            {
                ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + dword_6FD124, Fix16(y) + dword_6FD124 + w1);
            }
            ((Fix16_Point_POD*)&field_30_target_x)->SetXY_432860(Fix16(x) + dword_6FD124 + dword_6FD218, Fix16(y) + dword_6FCF98);
            break;
        case 3:
            ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x) - w1, Fix16(y));
            if (field_10->IsDoubleDoor_489600())
            {
                ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpTwo_6FD128 + w1, Fix16(y) + dword_6FD124 + w2);
            }
            else
            {
                ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + dword_6FD124 + w1, Fix16(y) + dword_6FD124 + w2);
            }
            ((Fix16_Point_POD*)&field_30_target_x)->SetXY_432860(Fix16(x) + dword_6FCF98, Fix16(y) - dword_6FD218);
            break;
        case 4:
            if (field_10->IsDoubleDoor_489600())
            {
                ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x) - dword_6FD124 - w1, Fix16(y) - w2);
            }
            else
            {
                ((Fix16_Point_POD*)&field_18_park_x_min)->SetXY_432860(Fix16(x) - w1, Fix16(y) - w2);
            }
            ((Fix16_Point_POD*)&field_20_park_x_max)->SetXY_432860(Fix16(x) + dword_6FD124 + w1, Fix16(y) + dword_6FD124);
            ((Fix16_Point_POD*)&field_30_target_x)->SetXY_432860(Fix16(x) + dword_6FCF98, Fix16(y) + dword_6FD124 + dword_6FD218);
            break;
    }

    ValidateParkCommand_534650();
    return field_3E;
}

MATCH_FUNC(0x5349d0)
void Garage_48::GaragesService_5349D0()
{
    u8 idx1;
    u8 idx2;
    Fix16_Point car_pos;

    switch (field_C)
    {
        case 1:
        {
            if (!field_0->field_50_car_sprite)
            {
                field_C = 3;
                return;
            }
            if (field_0->get_driver_4118B0() && field_0->field_54_driver != field_14)
            {
                field_14 = field_0->get_driver_4118B0();
            }

            u8 collision = field_0->field_50_car_sprite->CollisionCheck_5A0320(&field_18_park_x_min, &field_20_park_x_max, &idx1, &idx2);
            if (collision == 2)
            {
                if (idx1 == 0)
                {
                    if (idx2 == 1)
                    {
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    if (idx1 != 2)
                    {
                        return;
                    }
                    // goto: the original lays the parked block as the jump target of both
                    // corner checks, each with its own inline epilogue after it.
                    if (idx2 == 3)
                    {
                        goto park;
                    }
                    return;
                }
            }
            else if (collision > 2)
            {
                if (collision >= 4)
                {
                    field_C = 2;
                    return;
                }
            }
            else
            {
                return;
            }

        park:
            field_0->set_f78_0x2_44A3E0();
            field_0->set_f78_0x8_4218A0();
            if (field_0->is_driven_by_player())
            {
                field_0->GetDriverPlayer_421870()->DisableAllControls_569FF0();
            }
            field_0->SetF98To1IfNot4_475C10();
            field_0->field_58_physics->StopMoving_4895D0();

            car_pos.SetXY_432860(field_0->get_x_41E430(), field_0->get_y_41E440());
            Sprite_4C* pBox = field_0->field_50_car_sprite->field_C_sprite_4c_ptr;
            field_28_push_dir = (pBox->field_C_renderingRect[idx1] + pBox->field_C_renderingRect[idx2]).Divide_442CB0(kFpTwo_6FD128) - car_pos;
            if (field_28_push_dir.x == dword_6FD120 && field_28_push_dir.y == dword_6FD120)
            {
                field_3D = 1;
                field_C = 2;
            }
            else
            {
                field_28_push_dir = field_28_push_dir.NormalizeSafe_442AD0().Multiply_438FE0(dword_6FCF10);
            }
            field_0->field_58_physics->ApplyForceScaledByMass_55F9A0(field_28_push_dir);
            field_C = 2;
            break;
        }

        case 2:
        {
            if (!field_0->field_58_physics)
            {
                field_0->SetupCarPhysicsAndSpriteBinding_43BCA0();
            }
            if (++field_44 >= 300)
            {
                field_C = 3;
                return;
            }
            if (field_0->get_driver_4118B0() && !field_0->is_driven_by_player())
            {
                field_0->get_driver_4118B0()->SetObjective(objectives_enum::wait_in_car_27, 9999);
            }

            if (field_0->field_50_car_sprite->CollisionCheck_5A0320(&field_18_park_x_min, &field_20_park_x_max, &idx1, &idx2) != 4 && !field_3D)
            {
                field_0->field_58_physics->StopMoving_4895D0();
                field_0->field_58_physics->ApplyForceScaledByMass_55F9A0(field_28_push_dir);
                return;
            }

            field_3D = 1;
            --field_3C;
            field_0->field_58_physics->StopMoving_4895D0();
            if (field_3C == 0)
            {
                field_C = 3;
                field_0->field_58_physics->StopMoving_4895D0();
            }
            break;
        }

        case 3:
        {
            if (field_0->field_50_car_sprite)
            {
                // 9.6f: Car_BC 0x476230/0x4895E0 (clear f78 bits 2 and 8, inlined)
                field_0->field_78_flags &= ~2;
                field_0->field_78_flags &= ~8;
                field_10->CloseDoors_476A30();
                field_10->ClearF2C_4895F0();
                field_0->SetF98To4_475C30();
                field_0->PrepareForExplosion_43C1C0();
                field_0->field_4_passengers_list.KillAllPedsFromList_4715A0();
                if (field_0->is_driven_by_player())
                {
                    field_0->get_driver_4118B0()->field_15C_player->EnableAllControls_56A000();
                }
                if (!field_3F_no_respawn)
                {
                    Ped* pDriver = field_0->get_driver_4118B0();
                    if (pDriver)
                    {
                        pDriver->StartPedWalking_470200(field_30_target_x, field_34_target_y, field_0->get_z_41E450());
                        field_0->ClearDriver_4407F0();
                        pDriver->SetRotation_433C00(sub_5345E0(field_38));
                        field_0->field_54_driver = NULL;
                        pDriver->field_168_game_object->field_5C = 20;
                    }
                    else if (field_14 && field_14->get_cam_x() >= field_18_park_x_min && field_14->get_cam_x() <= field_20_park_x_max &&
                             field_14->get_cam_y() >= field_1C_park_y_min && field_14->get_cam_y() <= field_24_park_y_max)
                    {
                        field_14->StartPedWalking_470200(field_30_target_x, field_34_target_y, field_0->get_z_41E450());
                        field_14->SetRotation_433C00(sub_5345E0(field_38));
                    }
                }
                field_0->SetDespawn3IfNot5_421490();
            }
            field_4 = field_0;
            field_8 = field_4->field_6C_car_id;
            field_0 = NULL;
            Reset_489650();
            break;
        }
    }
}

MATCH_FUNC(0x534e80)
Garage_48::Garage_48()
{
    field_0 = 0;
    field_10 = 0;
    field_14 = 0;
    field_C = 0;
    field_28_push_dir.x = 0;
    field_28_push_dir.y = 0;
    field_18_park_x_min = 0;
    field_1C_park_y_min = 0;
    field_20_park_x_max = 0;
    field_24_park_y_max = 0;
    field_38 = 0;
    field_30_target_x = 0;
    field_34_target_y = 0;
    field_3C = 30;
    field_3D = 0;
    field_3E = 0;
    field_3F_no_respawn = 0;
    field_40 = 0;
    field_4 = 0;
    field_8 = 0;
}