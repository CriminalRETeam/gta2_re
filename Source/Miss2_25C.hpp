#pragma once

#include "Function.hpp"

class Ped;
class Car_BC;
class Object_2C;

class Miss2_C
{
  public:
    union
    {
        Car_BC* field_0_car;
        Object_2C* field_0_obj;
        Ped* field_0_ped;
    };
    u8 field_4_type; // determines what the union types are
    s8 field_5;
    s8 field_6;
    s8 field_7;
    s32 field_8_uni2;
};

class Miss2_25C
{
  public:
    Miss2_25C();
    EXPORT void sub_502DC0();
    EXPORT Miss2_C* allocate_next_502F60();
    EXPORT void push_type_1_car_502F80(Car_BC* pCar);
    EXPORT void push_type_3_ped_502FB0(Ped* pPed);
    EXPORT void push_type_2_502FF0(Object_2C *pObj, char_type a3);
    EXPORT void push_type_2_503050(Object_2C *pObj);

    Miss2_C field_0[50];

    char_type field_258_count;
    char_type field_259;
    char_type field_25A;
    char_type field_25B;
};

EXTERN_GLOBAL(Miss2_25C*, gMiss2_25C_6F805C);
