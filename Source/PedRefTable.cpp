#include "PedRefTable.hpp"

DEFINE_GLOBAL(PedRefTable*, gPedRefTable_7F8_703398, 0x703398);

MATCH_FUNC(0x59b060)
u8 PedRefTable::AllocForPed_59B060(s32 ped_id)
{
    PedRef* pIter = &mEntries[1];
    for (u8 i = 1; i < GTA2_COUNTOF(mEntries); pIter++, i++)
    {
        if (!pIter->mPedId && !pIter->mRefCount)
        {
            mEntries[i].mPedId = ped_id;
            return i;
        }
    }
    return 0;
}

MATCH_FUNC(0x59b0b0)
void PedRefTable::IncrementRefCount_59B0B0(u8 idx)
{
    mEntries[idx].mRefCount++;
}

MATCH_FUNC(0x59b0d0)
void PedRefTable::DecrementRefCount_59B0D0(u8 idx)
{
    if (mEntries[idx].mRefCount > 0)
    {
        mEntries[idx].mRefCount--;
    }
}

MATCH_FUNC(0x59b0f0)
PedRefTable::PedRefTable()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mEntries); i++)
    {
        mEntries[i].mPedId = 0;
        mEntries[i].mRefCount = 0;
    }
}

MATCH_FUNC(0x59b110)
PedRefTable::~PedRefTable()
{
}
