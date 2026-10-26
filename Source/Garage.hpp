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

    Car* field_0_parking_car;
    Car* field_4_parked_car;
    s32 field_8_parked_car_id;
    s32 field_C_state;
    Door* field_10_door;
    Ped* field_14_driver;
    Fix16 field_18_park_x_min;
    Fix16 field_1C_park_y_min;
    Fix16 field_20_park_x_max;
    Fix16 field_24_park_y_max;
    Fix16_Point field_28_push_dir;
    Fix16 field_30_target_x;
    Fix16 field_34_target_y;
    s32 field_38_door_face;
    char_type field_3C_settle_ticks;
    bool field_3D_in_position;
    u8 field_3E_park_ticket;
    bool field_3F_no_respawn;
    bool field_40_long_car;
    char_type field_41_unused;
    char_type field_42_unused;
    char_type field_43_unused;
    u32 field_44_wait_ticks;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Garage, 0x48);

EXTERN_GLOBAL(Garage*, gGarage_48_6FD26C);
