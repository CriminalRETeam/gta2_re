#include "Garage.hpp"
#include "Globals.hpp"
#include "Door.hpp"
#include "DoorPool.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "char.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "garage_state.hpp"

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(Ang16, kAng0_6FD25C);
EXTERN_GLOBAL(Ang16, kAng90_6FCFB0);
EXTERN_GLOBAL(Ang16, kAng180_6FD07E);
EXTERN_GLOBAL(Ang16, kAng270_6FD0A4);

DEFINE_GLOBAL(Garage*, gGarage_48_6FD26C, 0x6FD26C);

DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FD128, Fix16(2), 0x6FD128);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FCF88, Fix16(0x1000, 0), 0x6FCF88);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FD04C, kFpQuarter_6FCF88, 0x6FD04C);
DEFINE_GLOBAL_INIT(Fix16, kFpEighth_6FCF60, kFpQuarter_6FD04C / kFpTwo_6FD128, 0x6FCF60);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD0D8, Fix16(0x100, 0), 0x6FD0D8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD1D8, kFpOne64th_6FD0D8, 0x6FD1D8);

MATCH_FUNC(0x4bbc60)
Garage::~Garage()
{
}

// The heading for a ped leaving through a door facing `face`.
MATCH_FUNC(0x5345E0)
EXPORT Ang16 __stdcall GetHeadingForDoorFace_5345E0(s32 face)
{
    switch (face)
    {
        case 4:
            return kAng0_6FD25C;
        case 2:
            return kAng90_6FCFB0;
        case 3:
            return kAng180_6FD07E;
        case 1:
            return kAng270_6FD0A4;
    }
    return kAng0_6FD25C;
}

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FD124, Fix16(1), 0x6FD124);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FCF98, Fix16(0.5), 0x6FCF98);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FD218, Fix16(0x3333, 0), 0x6FD218);

// Car::IsLongerThanOneBlock_447ED0 (9.6f), with this file's copy of the 1.0 constant
static inline bool IsLongerThanOneBlock_447ED0(Car* pCar)
{
    if (pCar->field_50_car_sprite->GetHeight_447E70() > kFpOne_6FD124)
    {
        return true;
    }
    return false;
}

// 9.6f 0x489B10
MATCH_FUNC(0x534650)
void Garage::ValidateParkCommand_534650()
{
    // The result is unused (the out-of-line Add_408660 on a const Fix16)
    {
        const Fix16 door_z(this->field_10_door->field_0_primary_door_data->field_6_z);
        door_z.Add_408660(kFpEighth_6FCF60);
    }

    if (gMap_0x370_6F6268->HasWallInArea_4E18A0(field_18_park_x_min.ToInt(),
                                      (field_20_park_x_max - kFpOne64th_6FD1D8).ToInt(),
                                      field_1C_park_y_min.ToInt(),
                                      (field_24_park_y_max - kFpOne64th_6FD1D8).ToInt(),
                                      field_10_door->field_0_primary_door_data->get_z_489640()))
    {
        FatalError_4A38C0(Gta2Error::ErrorInSetupOfParkCommand,
                          "C:\\Splitting\\Gta2\\Source\\park.cpp",
                          62,
                          this->field_10_door->field_0_primary_door_data->get_x_489620(),
                          this->field_10_door->field_0_primary_door_data->get_y_489630(),
                          this->field_10_door->field_0_primary_door_data->get_z_489640());
    }
}

DEFINE_GLOBAL(Fix16, kFpZero_6FD120, 0x6FD120);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FCF10, Fix16(0x666, 0), 0x6FCF10);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6FCFB0, Ang16(360), 0x6FCFB0);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD07E, Ang16(720), 0x6FD07E);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD0A4, Ang16(1080), 0x6FD0A4);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FD25C, Ang16(0), 0x6FD25C);

MATCH_FUNC(0x534700)
u8 Garage::ParkCarAtDoor_534700(Car* pCar, Door* pDoor)
{
    field_0_parking_car = pCar;
    field_44_wait_ticks = 0;
    field_10_door = pDoor;
    field_C_state = garage_state::waiting_for_car_1;
    field_14_driver = NULL;
    field_3C_settle_ticks = 30;
    field_3D_in_position = 0;
    if (field_3E_park_ticket == 255)
    {
        field_3E_park_ticket = 1;
    }
    else
    {
        field_3E_park_ticket++;
    }

    field_40_long_car = IsLongerThanOneBlock_447ED0(pCar);

    u8 x;
    u8 y;
    u8 z;
    field_10_door->get_door_xyz_face_49CEE0(&x, &y, &z, reinterpret_cast<u32*>(&field_38_door_face));

    Fix16 w1;
    Fix16 w2;
    if (field_40_long_car)
    {
        // Assigned separately: `w2 = w1` lets VC6 merge the two and lose the second register (ebp)
        w1 = kFpTwo_6FD128;
        w2 = kFpTwo_6FD128;
    }
    else
    {
        w1 = kFpOne_6FD124;
        w2 = kFpOne_6FD124;
    }

    // The half size goes before the constant in the y sums (`Fix16(y) + w1 + 1`), the other order changes
    // VC6's operand order and register choices.
    // 9.6f: the park rectangle corners (field_18/1C, field_20/24) and the target (field_30/34) are
    // Fix16 pairs set with Fix16_Point_POD::SetXY_432860
    switch (field_38_door_face)
    {
        case 1:
            if (field_10_door->IsDoubleDoor_489600())
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x), Fix16(y) - w1 - kFpOne_6FD124);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x), Fix16(y) - w1);
            }
            reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpOne_6FD124 + w2, Fix16(y) + w1 + kFpOne_6FD124);
            reinterpret_cast<Fix16_Point_POD*>(&field_30_target_x)->SetXY_432860(Fix16(x) - kFpPoint8_6FD218, Fix16(y) + kFpHalf_6FCF98);
            break;
        case 2:
            reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x) - w2, Fix16(y) - w1);
            if (field_10_door->IsDoubleDoor_489600())
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpOne_6FD124, Fix16(y) + w1 + kFpTwo_6FD128);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpOne_6FD124, Fix16(y) + w1 + kFpOne_6FD124);
            }
            reinterpret_cast<Fix16_Point_POD*>(&field_30_target_x)->SetXY_432860(Fix16(x) + kFpOne_6FD124 + kFpPoint8_6FD218, Fix16(y) + kFpHalf_6FCF98);
            break;
        case 3:
            reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x) - w1, Fix16(y));
            if (field_10_door->IsDoubleDoor_489600())
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpTwo_6FD128 + w1, Fix16(y) + w2 + kFpOne_6FD124);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpOne_6FD124 + w1, Fix16(y) + w2 + kFpOne_6FD124);
            }
            reinterpret_cast<Fix16_Point_POD*>(&field_30_target_x)->SetXY_432860(Fix16(x) + kFpHalf_6FCF98, Fix16(y) - kFpPoint8_6FD218);
            break;
        case 4:
            if (field_10_door->IsDoubleDoor_489600())
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x) - kFpOne_6FD124 - w1, Fix16(y) - w2);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&field_18_park_x_min)->SetXY_432860(Fix16(x) - w1, Fix16(y) - w2);
            }
            reinterpret_cast<Fix16_Point_POD*>(&field_20_park_x_max)->SetXY_432860(Fix16(x) + kFpOne_6FD124 + w1, Fix16(y) + kFpOne_6FD124);
            reinterpret_cast<Fix16_Point_POD*>(&field_30_target_x)->SetXY_432860(Fix16(x) + kFpHalf_6FCF98, Fix16(y) + kFpOne_6FD124 + kFpPoint8_6FD218);
            break;
    }

    ValidateParkCommand_534650();
    return field_3E_park_ticket;
}

MATCH_FUNC(0x5349d0)
void Garage::GaragesService_5349D0()
{
    u8 idx1;
    u8 idx2;
    Fix16_Point car_pos;

    switch (field_C_state)
    {
        case garage_state::waiting_for_car_1:
        {
            if (!field_0_parking_car->field_50_car_sprite)
            {
                field_C_state = garage_state::park_finished_3;
                return;
            }
            if (field_0_parking_car->GetDriver_4118B0() && field_0_parking_car->field_54_driver != field_14_driver)
            {
                field_14_driver = field_0_parking_car->GetDriver_4118B0();
            }

            u8 collision = field_0_parking_car->field_50_car_sprite->CollisionCheck_5A0320(&field_18_park_x_min, &field_20_park_x_max, &idx1, &idx2);
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
                    field_C_state = garage_state::pushing_2;
                    return;
                }
            }
            else
            {
                return;
            }

        park:
            field_0_parking_car->SetNoCollide_44A3E0();
            field_0_parking_car->SetFlag3_4218A0();
            if (field_0_parking_car->IsDrivenByPlayer_4118D0())
            {
                field_0_parking_car->GetDriverPlayer_421870()->DisableAllControls_569FF0();
            }
            field_0_parking_car->LockUnlessPermanent_475C10();
            field_0_parking_car->field_58_physics->StopMoving_4895D0();

            car_pos.SetXY_432860(field_0_parking_car->GetX_41E430(), field_0_parking_car->GetY_41E440());
            SpriteShape* pBox = field_0_parking_car->field_50_car_sprite->field_C_pCollisionShape;
            field_28_push_dir = (pBox->field_C_corners[idx1] + pBox->field_C_corners[idx2]).Divide_442CB0(kFpTwo_6FD128) - car_pos;
            if (field_28_push_dir.x == kFpZero_6FD120 && field_28_push_dir.y == kFpZero_6FD120)
            {
                field_3D_in_position = 1;
                field_C_state = garage_state::pushing_2;
            }
            else
            {
                field_28_push_dir = field_28_push_dir.NormalizeSafe_442AD0().Multiply_438FE0(kFpPoint1_6FCF10);
            }
            field_0_parking_car->field_58_physics->ApplyForceScaledByMass_55F9A0(field_28_push_dir);
            field_C_state = garage_state::pushing_2;
            break;
        }

        case garage_state::pushing_2:
        {
            if (!field_0_parking_car->field_58_physics)
            {
                field_0_parking_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
            }
            if (++field_44_wait_ticks >= 300)
            {
                field_C_state = garage_state::park_finished_3;
                return;
            }
            if (field_0_parking_car->GetDriver_4118B0() && !field_0_parking_car->IsDrivenByPlayer_4118D0())
            {
                field_0_parking_car->GetDriver_4118B0()->SetObjective(objectives_enum::wait_in_car_27);
            }

            if (field_0_parking_car->field_50_car_sprite->CollisionCheck_5A0320(&field_18_park_x_min, &field_20_park_x_max, &idx1, &idx2) != 4 && !field_3D_in_position)
            {
                field_0_parking_car->field_58_physics->StopMoving_4895D0();
                field_0_parking_car->field_58_physics->ApplyForceScaledByMass_55F9A0(field_28_push_dir);
                return;
            }

            field_3D_in_position = 1;
            --field_3C_settle_ticks;
            field_0_parking_car->field_58_physics->StopMoving_4895D0();
            if (field_3C_settle_ticks == 0)
            {
                field_C_state = garage_state::park_finished_3;
                field_0_parking_car->field_58_physics->StopMoving_4895D0();
            }
            break;
        }

        case garage_state::park_finished_3:
        {
            if (field_0_parking_car->field_50_car_sprite)
            {
                // 9.6f: Car 0x476230/0x4895E0 (clear f78 bits 2 and 8, inlined)
                field_0_parking_car->field_78_flags &= static_cast<u16>(~car_flag::NoCollide);
                field_0_parking_car->field_78_flags &= static_cast<u16>(~car_flag::Flag3);
                field_10_door->CloseDoors_476A30();
                field_10_door->ClearF2C_4895F0();
                field_0_parking_car->LockPermanently_475C30();
                field_0_parking_car->PrepareForExplosion_43C1C0();
                field_0_parking_car->field_4_passengers_list.KillAndClearAllPeds_4715A0();
                if (field_0_parking_car->IsDrivenByPlayer_4118D0())
                {
                    field_0_parking_car->GetDriver_4118B0()->field_15C_player->EnableAllControls_56A000();
                }
                if (!field_3F_no_respawn)
                {
                    Ped* pDriver = field_0_parking_car->GetDriver_4118B0();
                    if (pDriver)
                    {
                        pDriver->StartPedWalking_470200(field_30_target_x, field_34_target_y, field_0_parking_car->GetZ_41E450());
                        field_0_parking_car->ClearDriver_4407F0();
                        pDriver->SetRotation_433C00(GetHeadingForDoorFace_5345E0(field_38_door_face));
                        field_0_parking_car->field_54_driver = NULL;
                        pDriver->field_168_game_object->field_5C_jump_over_timer = 20;
                    }
                    else if (field_14_driver && field_14_driver->get_cam_x() >= field_18_park_x_min && field_14_driver->get_cam_x() <= field_20_park_x_max &&
                             field_14_driver->get_cam_y() >= field_1C_park_y_min && field_14_driver->get_cam_y() <= field_24_park_y_max)
                    {
                        field_14_driver->StartPedWalking_470200(field_30_target_x, field_34_target_y, field_0_parking_car->GetZ_41E450());
                        field_14_driver->SetRotation_433C00(GetHeadingForDoorFace_5345E0(field_38_door_face));
                    }
                }
                field_0_parking_car->SetDespawn3IfNot5_421490();
            }
            field_4_parked_car = field_0_parking_car;
            field_8_parked_car_id = field_4_parked_car->field_6C_car_id;
            field_0_parking_car = NULL;
            Reset_489650();
            break;
        }
    }
}

MATCH_FUNC(0x534e80)
Garage::Garage()
{
    field_0_parking_car = NULL;
    field_10_door = NULL;
    field_14_driver = NULL;
    field_C_state = garage_state::idle_0;
    field_28_push_dir.x = 0;
    field_28_push_dir.y = 0;
    field_18_park_x_min = 0;
    field_1C_park_y_min = 0;
    field_20_park_x_max = 0;
    field_24_park_y_max = 0;
    field_38_door_face = 0;
    field_30_target_x = 0;
    field_34_target_y = 0;
    field_3C_settle_ticks = 30;
    field_3D_in_position = 0;
    field_3E_park_ticket = 0;
    field_3F_no_respawn = 0;
    field_40_long_car = 0;
    field_4_parked_car = NULL;
    field_8_parked_car_id = 0;
}