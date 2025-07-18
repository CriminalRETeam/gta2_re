#pragma once

#include "Function.hpp"

class Ped;

class PedGroup
{
  public:
    EXPORT void sub_4C8E60();
    EXPORT static void sub_4C8E80();
    EXPORT void sub_4C8E90();
    EXPORT void sub_4C8EF0();
    EXPORT void sub_4C8F20();
    EXPORT void add_ped_to_end_of_list_4C8F90(Ped* pPed);
    EXPORT void replace_leader_4C8FE0(Ped* pPed);
    EXPORT bool sub_4C9040();
    EXPORT char_type sub_4C9150();
    EXPORT void sub_4C91B0();
    EXPORT bool sub_4C9210();
    EXPORT bool sub_4C9220();
    EXPORT void sub_4C9240();
    EXPORT void sub_4C92A0();
    EXPORT void sub_4C93A0();
    EXPORT void sub_4C94E0(Ped* a2);
    EXPORT void sub_4C9680(u8 a2);
    EXPORT void sub_4C9970(Ped* a2);
    EXPORT void add_ped_leader_4C9B10(Ped* a2);
    EXPORT void add_ped_to_list_4C9B30(Ped* ptr, u8 idx);
    EXPORT char_type sub_4C9B60(Ped* a2);
    EXPORT Ped* sub_4C9ED0();
    EXPORT void sub_4C9F00();
    EXPORT u32 sub_4CA3E0();
    EXPORT Ped* sub_4CA3F0(u32* a2);
    EXPORT void sub_4CA4B0();
    EXPORT void sub_4CA5E0(u8 idx);
    EXPORT void sub_4CA820(s32 a2);
    EXPORT bool sub_4CAA20();
    EXPORT char_type sub_4CAAE0();
    EXPORT char_type sub_4CAB80();
    EXPORT bool sub_4CAC20(s32 idx);
    EXPORT bool sub_4CAD40();
    EXPORT Ped* sub_4CAE80(u8 idx);
    EXPORT static void sub_4CB080();
    EXPORT static PedGroup* sub_4CB0D0();
    EXPORT PedGroup();
    EXPORT ~PedGroup();

    char_type field_0;
    char_type field_1;
    char_type field_2; // padding
    char_type field_3; // padding
    Ped* field_4_ped_list[9];
    s32 field_28;
    Ped* field_2C_ped_leader;
    s32 field_30;
    u8 field_34_count;
    char_type field_35;
    char_type field_36_count;
    char_type field_37;
    s32 field_38_group_type;
    s32 field_3C;
    char_type field_40_in_use;
    char_type field_41;
    char_type field_42;
    char_type field_43;
};