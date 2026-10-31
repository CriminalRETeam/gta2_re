#pragma once

#include "Function.hpp"

// Maps a small u8 index (stored in objects / peds / explosions as the "owner" of damage etc.) to a ped id.
// Index 0 means "no ped", so slot 0 is never allocated. A slot is free when it has no ped id and no references.
class PedRef
{
  public:
    s32 mPedId; // 0x0
    u8 mRefCount; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(PedRef, 0x8);

class PedRefTable
{
  public:
    EXPORT u8 AllocForPed(s32 ped_id);
    EXPORT void IncrementRefCount(u8 idx);
    EXPORT void DecrementRefCount(u8 idx);
    EXPORT PedRefTable();
    EXPORT ~PedRefTable();

    // 9.6f 0x434070
    inline void Clear_434070(u8 idx)
    {
        mEntries[idx].mPedId = 0;
        mEntries[idx].mRefCount = 0;
    }

    // 9.6f 0x420F10
    s32 GetPedId_420F10(u8 idx) const
    {
        return mEntries[idx].mPedId;
    }

    PedRef mEntries[255]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(PedRefTable, 0x7F8);

EXTERN_GLOBAL(PedRefTable*, gPedRefTable_7F8);
