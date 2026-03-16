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

STUB_FUNC(0x534700)
void Garage_48::ParkCarAtDoor_534700(Car_BC* a2, void* a3)
{
    NOT_IMPLEMENTED;
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
    field_30 = 0;
    field_34 = 0;
    field_3C = 30;
    field_3D = 0;
    field_3E = 0;
    field_3F_no_respawn = 0;
    field_40 = 0;
    field_4 = 0;
    field_8 = 0;
}