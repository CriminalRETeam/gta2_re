#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Door_38.hpp"
#include "Door_4D4.hpp"
#include "error.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(Garage_48*, gGarage_48_6FD26C, 0x6FD26C);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD128, Fix16(0x8000, 0), 0x6FD128);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCF88, Fix16(0x1000, 0), 0x6FCF88);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD04C, dword_6FCF88, 0x6FD04C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCF60, dword_6FD04C / dword_6FD128, 0x6FCF60);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD0D8, Fix16(0x100, 0), 0x6FD0D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD1D8, dword_6FD0D8, 0x6FD1D8);

MATCH_FUNC(0x4bbc60)
Garage_48::~Garage_48()
{
}

// 9.6f 0x489B10
WIP_FUNC(0x534650)
void Garage_48::ValidateParkCommand_534650()
{
    WIP_IMPLEMENTED;

    // TODO: Gets optimized out, also needs to call operator+
    // without that being inlined too, hmm
    Fix16 v4(this->field_10->field_0_primary_door_data->field_6_z);
    v4 = v4 + dword_6FCF60;

    if (gMap_0x370_6F6268->sub_4E18A0(field_18.ToInt(),
                                      (field_20 - dword_6FD1D8).ToInt(),
                                      field_1C.ToInt(),
                                      (field_24 - dword_6FD1D8).ToInt(),
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

DEFINE_GLOBAL(Fix16, dword_6FD124, 0x6FD124);
DEFINE_GLOBAL(Fix16, dword_6FCF98, 0x6FCF98);
DEFINE_GLOBAL(Fix16, dword_6FD218, 0x6FD218);

WIP_FUNC(0x534700)
u8 Garage_48::ParkCarAtDoor_534700(Car_BC* pCar, Door_38* pDoor)
{
    field_44 = 0;
    field_14 = 0;
    field_3D = 0;
    field_0 = pCar;
    field_10 = pDoor;
    field_C = 1;
    field_3C = 30;
    if (field_3E == 255)
    {
        field_3E = 1;
    }
    else
    {
        field_3E++;
    }

    field_40 = pCar->field_50_car_sprite->field_C_sprite_4c_ptr->field_4_height > dword_6FD124;

    u8 x;
    u8 y;
    u8 z;
    field_10->get_door_xyz_face_49CEE0(&x, &y, &z, (u32*)&field_38);

    Fix16 w1;
    Fix16 w2;
    if (field_40)
    {
        w1 = dword_6FD128;
        w2 = w1;
    }
    else
    {
        w1 = dword_6FD124;
        w2 = w1;
    }

    switch (field_38)
    {
        case 1:
            if (field_10->field_0_primary_door_data && field_10->field_4_secondary_door_data)
            {
                field_1C = Fix16(y) - dword_6FD124 - w1;
                field_18 = Fix16(x);
            }
            else
            {
                field_18 = Fix16(x);
                field_1C = Fix16(y) - w1;
            }
            field_24 = Fix16(y) + dword_6FD124 + w1;
            field_20 = Fix16(x) + dword_6FD124 + w2;
            field_34_target_y = Fix16(y) + dword_6FCF98;
            field_30_target_x = Fix16(x) - dword_6FD218;
            break;
        case 2:
            field_18 = Fix16(x) - w2;
            field_1C = Fix16(y) - w1;
            if (field_10->field_0_primary_door_data && field_10->field_4_secondary_door_data)
            {
                field_20 = dword_6FD124 + Fix16(x);
                field_24 = dword_6FD128 + Fix16(y) + w1;
            }
            else
            {
                field_20 = Fix16(x) + dword_6FD124;
                field_24 = Fix16(y) + dword_6FD124 + w1;
            }
            field_34_target_y = dword_6FCF98 + Fix16(y);
            field_30_target_x = Fix16(x) + dword_6FD218 + dword_6FD124;
            break;
        case 3:
            field_18 = Fix16(x) - w1;
            field_1C = Fix16(y);
            if (field_10->field_0_primary_door_data && field_10->field_4_secondary_door_data)
            {
                field_24 = dword_6FD124 + Fix16(y) + w2;
                field_20 = dword_6FD128 + Fix16(x) + w1;
            }
            else
            {
                field_24 = Fix16(y) + dword_6FD124 + w2;
                field_20 = dword_6FD124 + Fix16(x) + w1;
            }
            field_34_target_y = Fix16(y) - dword_6FD218;
            field_30_target_x = Fix16(x) + dword_6FCF98;
            break;
        case 4:
            if (field_10->field_0_primary_door_data && field_10->field_4_secondary_door_data)
            {
                field_18 = Fix16(x) - dword_6FD124 - w1;
            }
            else
            {
                field_18 = Fix16(x) - w1;
            }
            field_1C = Fix16(y) - w2;
            field_24 = Fix16(y) + dword_6FD124;
            field_20 = dword_6FD124 + Fix16(x) + w1;
            field_34_target_y = dword_6FD218 + Fix16(y) + dword_6FD124;
            field_30_target_x = Fix16(x) + dword_6FCF98;
            break;
    }

    ValidateParkCommand_534650();
    return field_3E;
}

STUB_FUNC(0x5349d0)
void Garage_48::GaragesService_5349D0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x534e80)
Garage_48::Garage_48()
{
    field_0 = 0;
    field_10 = 0;
    field_14 = 0;
    field_C = 0;
    field_28 = 0;
    field_2C = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 0;
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