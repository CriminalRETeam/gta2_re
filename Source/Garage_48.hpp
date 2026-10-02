#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include <cstdio>

class Car_BC;
class Ped;
class Door_38;

class Garage_48
{
  public:
    EXPORT ~Garage_48();
    EXPORT void ValidateParkCommand_534650();
    EXPORT u8 ParkCarAtDoor_534700(Car_BC* pCar, Door_38* pDoor);
    EXPORT void GaragesService_5349D0();
    EXPORT Garage_48();

    // inlined 0x44C870
    bool sub_44C870(Car_BC* param_1) const
    {
        if (param_1 == this->field_4 && param_1 != NULL && param_1->field_6C_maybe_id == this->field_8)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x434AF0
    inline bool IsParkingCarAndF3D_434AF0(Car_BC* pCar)
    {
        return pCar == field_0 && field_3D;
    }

    bool IsMaybeParkingCar_493540(Car_BC* pCar)
    {
        if (pCar == field_0)
        {
            return true;
        }
        return false;
    }

    Car_BC* field_0;
    Car_BC* field_4;
    s32 field_8;
    s32 field_C;
    Door_38* field_10;
    Ped* field_14;
    Fix16 field_18_park_x_min;
    Fix16 field_1C_park_y_min;
    Fix16 field_20_park_x_max;
    Fix16 field_24_park_y_max;
    Fix16_Point_POD field_28_push_dir;
    Fix16 field_30_target_x;
    Fix16 field_34_target_y;
    s32 field_38;
    char_type field_3C;
    char_type field_3D;
    u8 field_3E;
    char_type field_3F_no_respawn;
    char_type field_40;
    char_type field_41;
    char_type field_42;
    char_type field_43;
    s32 field_44;
};

EXTERN_GLOBAL(Garage_48*, gGarage_48_6FD26C);
