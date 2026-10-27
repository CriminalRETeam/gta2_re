#include "GlobalsRegistry.hpp"

void GlobalsRegistry::Add(GlobalRef* pRef)
{
    mGlobals.push_back(pRef);
}

// CheckVars iterates over all stored globals and checks only their stored mOgAddr and mSize
// (i.e. the memory region [mOgAddr, mOgAddr + mSize)). It ignores mVar entirely.
void GlobalsRegistry::CheckVars()
{
    bool duplicateFound = false;
    bool overlapFound = false;

    for (size_t i = 0; i < mGlobals.size(); ++i)
    {
        GlobalRef* ref1 = mGlobals[i];
        u32 start1 = ref1->mOgAddr;
        u32 end1 = start1 + ref1->mSize;

        for (size_t j = i + 1; j < mGlobals.size(); ++j)
        {
            GlobalRef* ref2 = mGlobals[j];

            if (ref1->mOgAddr == ref2->mOgAddr)
            {
                printf("Duplicate mOgAddr: 0x%X appears more than once.\n", ref1->mOgAddr);
                duplicateFound = true;
            }

            u32 start2 = ref2->mOgAddr;
            u32 end2 = start2 + ref2->mSize;
            // They overlap unless one region ends before the other starts
            if (!(end1 <= start2 || end2 <= start1))
            {
                u32 minEnd = (end1 < end2) ? end1 : end2;
                u32 maxStart = (start1 > start2) ? start1 : start2;
                u32 overlap_size = minEnd - maxStart;

                printf("Overlap detected: Region [0x%X, 0x%X) overlaps with region [0x%X, 0x%X). Overlap size: %u byte(s).\n",
                       start1,
                       end1,
                       start2,
                       end2,
                       overlap_size);
                overlapFound = true;
            }
        }
    }

    if (!duplicateFound && !overlapFound)
    {
        printf("No duplicate or overlapping globals found.\n");
    }
    else
    {
        abort();
    }
}

extern "C"
{
    __declspec(dllexport) GlobalsRegistry* __cdecl GetGlobalsRegistry()
    {
        static GlobalsRegistry globalsReg;
        return &globalsReg;
    }
}

GlobalRef::GlobalRef(void* pVar, u32 addr, u32 size) : mVar(pVar), mOgAddr(addr), mSize(size)
{
    GetGlobalsRegistry()->Add(this);
}