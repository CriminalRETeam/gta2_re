#include "PedRefTable_7F8.hpp"

DEFINE_GLOBAL(PedRefTable_7F8*, gPedRefTable_7F8_703398, 0x703398);

MATCH_FUNC(0x59b060)
u8 PedRefTable_7F8::AllocForPed_59B060(s32 ped_id)
{
    PedRef_8* pIter = &field_0_entries[1];
    for (u8 i = 1; i < GTA2_COUNTOF(field_0_entries); pIter++, i++)
    {
        if (!pIter->field_0_ped_id && !pIter->field_4_ref_count)
        {
            field_0_entries[i].field_0_ped_id = ped_id;
            return i;
        }
    }
    return 0;
}

MATCH_FUNC(0x59b0b0)
void PedRefTable_7F8::IncrementRefCount_59B0B0(u8 idx)
{
    field_0_entries[idx].field_4_ref_count++;
}

MATCH_FUNC(0x59b0d0)
void PedRefTable_7F8::DecrementRefCount_59B0D0(u8 idx)
{
    if (field_0_entries[idx].field_4_ref_count > 0)
    {
        field_0_entries[idx].field_4_ref_count--;
    }
}

MATCH_FUNC(0x59b0f0)
PedRefTable_7F8::PedRefTable_7F8()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_entries); i++)
    {
        field_0_entries[i].field_0_ped_id = 0;
        field_0_entries[i].field_4_ref_count = 0;
    }
}

MATCH_FUNC(0x59b110)
PedRefTable_7F8::~PedRefTable_7F8()
{
}
