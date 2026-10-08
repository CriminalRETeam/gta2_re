#pragma once

#include "Function.hpp"

// Maps a small u8 index (stored in objects / peds / explosions as the "owner" of damage etc.) to a ped id.
// Index 0 means "no ped", so slot 0 is never allocated. A slot is free when it has no ped id and no references.
class PedRef_8
{
  public:
    s32 field_0_ped_id;
    u8 field_4_ref_count;
};

class PedRefTable_7F8
{
  public:
    EXPORT u8 AllocForPed_59B060(s32 ped_id);
    EXPORT void IncrementRefCount_59B0B0(u8 idx);
    EXPORT void DecrementRefCount_59B0D0(u8 idx);
    EXPORT PedRefTable_7F8();
    EXPORT ~PedRefTable_7F8();

    inline void Clear_434070(u8 idx)
    {
        field_0_entries[idx].field_0_ped_id = 0;
        field_0_entries[idx].field_4_ref_count = 0;
    }

    s32 GetPedId_420F10(u8 idx)
    {
        return field_0_entries[idx].field_0_ped_id;
    }

    PedRef_8 field_0_entries[255];
};

EXTERN_GLOBAL(PedRefTable_7F8*, gPedRefTable_7F8_703398);
