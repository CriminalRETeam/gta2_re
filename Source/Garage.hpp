#pragma once

#include "Car.hpp"
#include "Function.hpp"
#include "garage_state.hpp"
#include <cstdio>

class Car;
class Ped;
class Door;

class Garage
{
  public:
    EXPORT ~Garage();
    EXPORT void ValidateParkCommand_534650();
    EXPORT u8 ParkCarAtDoor_534700(Car* pCar, Door* pDoor);
    EXPORT void GaragesService_5349D0();
    EXPORT Garage();

    // inlined 0x44C870
    bool IsParkedCar_44C870(const Car* pCar) const
    {
        if (pCar == this->field_4_parked_car && pCar != NULL && pCar->field_6C_car_id == this->field_8_parked_car_id)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x434AF0
    inline bool IsParkingCarInPosition_434AF0(const Car* pCar) const
    {
        return pCar == field_0_parking_car && field_3D_in_position;
    }

    // 9.6f 0x489650
    inline void Reset_489650()
    {
        field_10_door = NULL;
        field_C_state = garage_state::idle_0;
        field_28_push_dir.x = 0;
        field_28_push_dir.y = 0;
        field_18_park_x_min = 0;
        field_1C_park_y_min = 0;
        field_20_park_x_max = 0;
        field_24_park_y_max = 0;
        field_30_target_x = 0;
        field_34_target_y = 0;
    }

    bool IsParkingCar_493540(const Car* pCar) const
    {
        if (pCar == field_0_parking_car)
        {
            return true;
        }
        return false;
    }

    Car* field_0_parking_car; // 0x0
    Car* field_4_parked_car; // 0x4
    s32 field_8_parked_car_id; // 0x8
    s32 field_C_state; // 0xC
    Door* field_10_door; // 0x10
    Ped* field_14_driver; // 0x14
    Fix16 field_18_park_x_min; // 0x18
    Fix16 field_1C_park_y_min; // 0x1C
    Fix16 field_20_park_x_max; // 0x20
    Fix16 field_24_park_y_max; // 0x24
    Fix16_Point field_28_push_dir; // 0x28
    Fix16 field_30_target_x; // 0x30
    Fix16 field_34_target_y; // 0x34
    s32 field_38_door_face; // 0x38
    char_type field_3C_settle_ticks; // 0x3C
    bool field_3D_in_position; // 0x3D
    u8 field_3E_park_ticket; // 0x3E
    bool field_3F_no_respawn; // 0x3F
    bool field_40_long_car; // 0x40
    char_type field_41_unused; // 0x41
    char_type field_42_unused; // 0x42
    char_type field_43_unused; // 0x43
    u32 field_44_wait_ticks; // 0x44
};
GTA2_ASSERT_SIZEOF_ALWAYS(Garage, 0x48);

EXTERN_GLOBAL(Garage*, gGarage_48_6FD26C);
