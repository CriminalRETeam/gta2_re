#include "Taxi_4.hpp"
#include "Globals.hpp"
#include "error.hpp"

EXPORT_VAR Taxi_324* gTaxi_324_6783F8;
GLOBAL(gTaxi_324_6783F8, 0x6783F8);

STUB_FUNC(0x457ba0)
u32* Taxi_4::sub_457BA0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x457bc0)
s32 Taxi_4::sub_457BC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x457bf0)
s32 Taxi_4::sub_457BF0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

/*
 i = 99;
            pIter = pTaxi_324_mem->field_8_array;
            do
            {
                pIter->field_0_ptr = &pIter->field_4_data;
                ++pIter;
                --i;
            } while (i);
            pTaxi_324_mem->field_0_pFirst = &pTaxi_324_mem->field_4;
            pTaxi_324_mem->field_320_count = 0;
*/
MATCH_FUNC(0x5ae060)
Taxi_4::Taxi_4()
{
    sub_4C09B0();

    if (!gTaxi_324_6783F8)
    {
        gTaxi_324_6783F8 = new Taxi_324();
        if (!gTaxi_324_6783F8)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\taxi.cpp", 29);
        }
    }
}

MATCH_FUNC(0x5ae0d0)
Taxi_4::~Taxi_4()
{
    if (gTaxi_324_6783F8)
    {
        GTA2_DELETE_AND_NULL(gTaxi_324_6783F8);
    }
}
