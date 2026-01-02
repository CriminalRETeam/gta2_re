#pragma once

#include "Function.hpp"
#include "Ped_List_4.hpp"

class Ped;
class Kfc_30;

class jawwie_8
{
  public:
    s32 field_0;
    s32 field_4;
};

class Ambulance_20
{
  public:
    EXPORT Ambulance_20();
    EXPORT ~Ambulance_20();
    EXPORT void sub_4FA7D0();
    EXPORT void AddPassenger_4FA800(Ped* pPed);
    EXPORT bool sub_4FA820();
    EXPORT char_type sub_4FA9D0();
    EXPORT u32* sub_4FAAC0();
    EXPORT void sub_4FB330();

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    Kfc_30* field_4;
    Ped* field_8;
    Ped* field_C;
    Ped_List_4 field_10;
    char_type field_14_count;
    char_type field_15;
    s16 field_16;
    s32 field_18;
    char_type field_1C;
    char_type field_1D;
    char_type field_1E;
    char_type field_1F;
};

class Ambulance_110
{
  public:
    Ambulance_110()
    {
        init_4FA310();
    }

    EXPORT ~Ambulance_110();
    EXPORT void init_4FA310();
    EXPORT char_type sub_4FA330(Ped* a2);
    EXPORT char_type TryAddPassenger_4FA470(Ped* a2);
    EXPORT Ambulance_20* sub_4FA4B0();
    EXPORT void sub_4FA500();
    EXPORT void AmbulancesService_4FA790();

    char_type field_0;
    u8 field_1_f8_idx;
    char_type field_2;
    char_type field_3;
    Ped_List_4 field_4;
    jawwie_8 field_8[25];
    Ambulance_20 field_D0[2];
};

EXTERN_GLOBAL(Ambulance_110*, gAmbulance_110_6F70A8);
