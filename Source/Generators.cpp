#include "Generators.hpp"
#include "Globals.hpp"
#include "rng.hpp"
#include "Object_5C.hpp"

DEFINE_GLOBAL(GeneratorPool*, gGeneratorPool_14AC, 0x67E5D0);
EXTERN_GLOBAL(s32, bStartNetworkGame);

MATCH_FUNC(0x4C1A70)
void Generator::Activate()
{
    mSpawnsLeft = -1;

    if (!bStartNetworkGame)
    {
        mCycle = next_cycle();
        gGeneratorPool_14AC->mKillCount++;
    }
    else
    {
        mCycle = gpRng->get_cur_rng_41CFE0() + 1;
        gGeneratorPool_14AC->mKillCount++;
    }
}

MATCH_FUNC(0x4C1AB0)
s32 Generator::next_cycle() const
{
    s32 result;
    if (mMinDelay == mMaxDelay)
    {
        result = mMinDelay;
    }
    else
    {
        result = gRng.get_int(4 * (mMaxDelay - mMinDelay)) + 4 * mMinDelay;
    }
    return result + gpRng->get_cur_rng_41CFE0();
}

MATCH_FUNC(0x4C1B10)
void Generator::SpawnObject()
{
    if (mSpawnMode == generator_spawn_mode::one_at_a_time_2)
    {
        if (mObj)
        {
            if (mObj->mId == mId && mObj->mModel)
            {
                mCycle += 4;
            }
            else
            {
                mObj = NULL;
                mId = 0;
                mCycle = next_cycle();
            }
        }
        else
        {
            Object* pObj = gObject_5C->NewPhysicsObjUnlessBlocked(mGenType, mX, mY, mZ, mRot);
            if (pObj)
            {
                mObj = pObj;
                mId = pObj->mId;
                if (mSpawnsLeft < 0xFFFF)
                {
                    mSpawnsLeft--;
                }

                if (pObj->IsPickup_421060())
                {
                    if (mAmmo > 0)
                    {
                        pObj->mPedRefIdx = static_cast<u8>(mAmmo);
                    }
                }
                mObj = pObj;
                mId = pObj->mId;
            }
            mCycle += 4;
        }
    }
    else
    {
        Object* pObj = gObject_5C->NewPhysicsObjUnlessBlocked(mGenType, mX, mY, mZ, mRot);
        if (pObj)
        {
            if (mSpawnsLeft < 0xFFFF)
            {
                mSpawnsLeft--;
            }

            if (pObj->IsPickup_421060())
            {
                if (mAmmo > 0)
                {
                    pObj->mPedRefIdx = static_cast<u8>(mAmmo);
                }
            }
            mObj = pObj;
            mId = pObj->mId;
        }
        mCycle = next_cycle();
    }

    if (mSpawnsLeft == 0)
    {
        gGeneratorPool_14AC->mKillCount--;
    }
}

MATCH_FUNC(0x4c1c50)
void Generator::Service()
{
    if (static_cast<u32>(gpRng->get_cur_rng_41CFE0()) >= mCycle)
    {
        if (mSpawnsLeft > 0)
        {
            SpawnObject();
        }
    }
}

MATCH_FUNC(0x4c1c70)
void Generator::Init(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay)
{
    mX = x.mValue;
    mY = y.mValue;
    mZ = z.mValue;
    mRot = rot.rValue;
    mGenType = generator_type;
    mMinDelay = min_delay;
    mMaxDelay = max_delay;
    mCycle = gpRng->get_cur_rng_41CFE0() + 1;
    mUnused20 = 1;
    mSpawnsLeft = 0;
    mAmmo = 0;
    mObj = NULL;
    mId = 0;
    mSpawnMode = generator_spawn_mode::every_cycle_1;
}

MATCH_FUNC(0x4c1cd0)
void GeneratorPool::FinalizeAfterScriptLoad()
{
    s32 count = mCount;
    mCountD3 = count / 3;

    if (!static_cast<u8>(count / 3))
    {
        mCountD3 = mCount;
    }

    if (count > 4)
    {
        Generator* pIter = &mPool[0];
        for (u8 i = 0; i < mCount; i++)
        {
            pIter->mMinDelay = pIter->mMinDelay >> 2;
            pIter->mMaxDelay = pIter->mMaxDelay >> 2;
            pIter++;
        }
    }
    if (bStartNetworkGame)
    {
        Generator* pIter = &mPool[0];
        for (u8 i = 0; i < mCount; i++)
        {
            pIter->mCycle = 1;
            pIter++;
        }
    }
}

MATCH_FUNC(0x4c1d70)
void GeneratorPool::GeneratorsService()
{
    s32 start_idx = mCountD3 * (gpRng->get_cur_rng_41CFE0() & 3);
    s32 end_idx = start_idx + mCountD3;
    if (end_idx > mCount)
    {
        end_idx = mCount;
    }

    Generator* pIter = &mPool[start_idx];
    for (s32 i = start_idx; i < end_idx; i++)
    {
        pIter->Service();
        pIter++;
    }
}

MATCH_FUNC(0x4c1dc0)
Generator* GeneratorPool::CreateGenerator(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay)
{
    Generator* pGen = &mPool[mCount];
    pGen->Init(x, y, z, rot, type, min_delay, max_delay);
    mCount++;
    return pGen;
}

MATCH_FUNC(0x4c1e20)
GeneratorPool::GeneratorPool()
{
    mCount = 0;
    mKillCount = 0;
    mCountD3 = 0;
}