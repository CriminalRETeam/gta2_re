#include "CarChaseTaskTable.hpp"
#include "Globals.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(CarChaseTaskTable*, gCarChaseTaskTable_678E30, 0x678E30);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_678D0C, Fix16(0), 0x678D0C);

MATCH_FUNC(0x4747b0)
void CarChaseTask::ResetEntry()
{
    mInUse = 0;
    mRamTarget = false;
    mTaskType = 0;
    mTargetX = kFpZero_678D0C;
    mTargetY = kFpZero_678D0C;
    mTargetZ = kFpZero_678D0C;
    mCanSnapToTarget = false;
    mUnused21 = 0;
    mFollowingRoute = true;
    mUnused23 = 1;
    mUnused24 = 0;
    mUnused25 = 0;
    mpTargetPed = NULL;
    mRouteFinished = false;
    mSettleCounter = 0;
    mSideCounter = 0;
    mWaitCounter = 0;
    mChaseState = car_chase_state::follow_behind_0;
    mSnapTimer = 0;
    mpDriver = NULL;
    mUnused38 = 0;
    mBlockCounter = 0;
}

MATCH_FUNC(0x474810)
CarChaseTask* CarChaseTaskTable::AllocateEntry()
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (!mEntries[i].mInUse)
        {
            mEntries[i].mInUse = 1;
            return &mEntries[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x474850)
bool CarChaseTaskTable::AreAllies(Ped* pPed1, Ped* pPed2) const
{
    if (pPed1->get_occupation_403980() < ped_ocupation_enum::police || pPed1->get_occupation_403980() > ped_ocupation_enum::army_army) // ped 1 is not police
    {
        if (pPed2->mpGang == pPed1->mpGang) // they are from same gang (or both dont have any)
        {
            return true;
        }
    }
    else
    {
        if (pPed2->get_occupation_403980() >= ped_ocupation_enum::police && pPed2->mOccupation <= ped_ocupation_enum::army_army) // both ped 1 and ped 2 are police feds
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4748a0)
Ped* CarChaseTaskTable::FindDriverInState(s32 chase_state, Ped* pTargetPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1 && mEntries[i].mpTargetPed == pTargetPed && AreAllies(pTargetPed, mEntries[i].mpDriver) &&
            chase_state == mEntries[i].mChaseState)
        {
            return mEntries[i].mpDriver;
        }
    }
    return NULL;
}

MATCH_FUNC(0x474920)
u8 CarChaseTaskTable::CountPursuers(Ped* pTargetPed, Ped* pPed) const
{
    u8 total = 0;
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1 && mEntries[i].mpTargetPed == pTargetPed)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                total++;
            }
        }
    }
    return total;
}

MATCH_FUNC(0x474970)
bool CarChaseTaskTable::HasAnyPursuer(Ped* pTargetPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1 && mEntries[i].mpTargetPed == pTargetPed)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4749b0)
bool CarChaseTaskTable::HasPursuerInFront(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                switch (mEntries[i].mChaseState)
                {
                    case car_chase_state::in_front_stopping_13:
                    case car_chase_state::target_stopped_15:
                        return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474a20)
bool CarChaseTaskTable::HasPursuerAlongsideLeft(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver) && mEntries[i].mChaseState == car_chase_state::alongside_left_8)
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474a80)
bool CarChaseTaskTable::HasPursuerOnLeft(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                switch (mEntries[i].mChaseState)
                {
                    case car_chase_state::approach_left_6:
                    case car_chase_state::alongside_left_8:
                    case car_chase_state::pull_ahead_left_10:
                        return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474af0)
bool CarChaseTaskTable::HasPursuerAlongsideRight(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver) && mEntries[i].mChaseState == car_chase_state::alongside_right_9)
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474b50)
bool CarChaseTaskTable::HasPursuerOnRight(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                switch (mEntries[i].mChaseState)
                {
                    case car_chase_state::approach_right_7:
                        return true;
                    case car_chase_state::alongside_right_9:
                    case car_chase_state::pull_ahead_right_11:
                        return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474bc0)
bool CarChaseTaskTable::HasPursuerAlongside(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                if (mEntries[i].mChaseState >= car_chase_state::approach_left_6 && (mEntries[i].mChaseState <= car_chase_state::alongside_right_9 || mEntries[i].mChaseState == car_chase_state::in_front_stopping_13))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474c30)
bool CarChaseTaskTable::HasPursuerChoosingSide(Ped* pPed) const
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (mEntries[i].mInUse == 1)
        {
            if (AreAllies(pPed, mEntries[i].mpDriver))
            {
                if (mEntries[i].mChaseState < car_chase_state::move_right_4)
                {
                    continue;
                }

                if (mEntries[i].mChaseState <= car_chase_state::choose_side_5)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x474ca0)
CarChaseTask::CarChaseTask()
{
    ResetEntry();
}

MATCH_FUNC(0x474cb0)
CarChaseTask::~CarChaseTask()
{
}

MATCH_FUNC(0x474cc0)
void CarChaseTaskTable::FreeEntry(CarChaseTask* pEntry)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (&mEntries[i] == pEntry)
        {
            mEntries[i].mInUse = 0;
            mEntries[i].ResetEntry();
            return;
        }
    }
}

MATCH_FUNC(0x474d10)
CarChaseTaskTable::CarChaseTaskTable()
{
}

MATCH_FUNC(0x474d30)
CarChaseTaskTable::~CarChaseTaskTable()
{
}