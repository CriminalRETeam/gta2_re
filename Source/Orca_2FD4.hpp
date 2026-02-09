#pragma once

#include "Function.hpp"
#include "Ped_List_4.hpp"

class Ped;

#pragma pack(push)
#pragma pack(1)
class Orca_8
{
  public:
    char_type field_0;
    char_type field_1;
    u8 field_2;
    char_type field_3;
    char_type field_4;
    char_type field_5;
    s16 field_6;
};
#pragma pack(pop)

class Orca_2FD4
{
  public:
    Orca_2FD4()
    {
        init_5545E0();
    }
    EXPORT bool CanMoveInDirection_554080(s32 a2);
    EXPORT char_type TestDiagonalMove_5540E0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6);
    EXPORT char_type Internel_CanMoveDiagonally_554110(char_type a2, char_type a3);
    EXPORT void Internel_ClearGrid_5545C0();
    EXPORT void init_5545E0();

    ~Orca_2FD4()
    {
        // TODO: Should this be empty?
    }
    EXPORT void remove_ped_554620(s32 a2);
    EXPORT char_type Internel_EvaluateBehaviorGridCell_554640();
    EXPORT char_type Internel_UpdateBehaviorGrid_554710();
    EXPORT char_type Internal_ProcessBehaviorGrid_5548C0();
    EXPORT char_type RestoreSavedPosition_554920();
    EXPORT s32 IsFirstPassenger_554A90(Ped* a2);
    EXPORT char_type ComputePath_554AB0(s32 a2, s32 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, s32 a10, u8* a11);
    EXPORT char_type FindNearbyTileMatchingSlopeType_5552B0(char_type a2, u8* xpos, u8* ypos, u8* zpos, char_type a6);

    s32 field_0;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    s16 field_C;
    char_type field_E;
    char_type field_F;
    char_type field_10;
    char_type field_11;
    char_type field_12;
    char_type field_13;
    char_type field_14;
    char_type field_15;
    char_type field_16;
    char_type field_17;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    s16 field_1C_f40_idx;
    s16 field_1E;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    char_type field_24;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    char_type field_28;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
    s16 field_2C;
    s16 field_2E;
    s32 field_30;
    s16 field_34;
    s16 field_36;
    char_type field_38;
    char_type field_39;
    s16 field_3A;
    Ped_List_4 field_3C;
    Orca_8 field_40[1122];
    s32 field_2350[796];
    s32 field_2FC0;
    s32 field_2FC4;
    s32 field_2FC8;
    s32 field_2FCC;
    char_type field_2FD0;
    char_type field_2FD1;
    char_type field_2FD2;
    char_type field_2FD3;
};

EXTERN_GLOBAL(Orca_2FD4*, gOrca_2FD4_6FDEF0);
