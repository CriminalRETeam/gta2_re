#include "rng.hpp"
#include "CarInfoTable.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "Hud.hpp"
#include "InputRecorder_67F8B0.hpp"
#include <stdio.h>
#include <stdlib.h>

DEFINE_GLOBAL(rng*, gpRng, 0x67AB34);
DEFINE_GLOBAL(rng, gRng, 0x6F6784);
DEFINE_GLOBAL(PixelsToFix16Table, gPixelsToFix16, 0x6F6850);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

MATCH_FUNC(0x48B900)
void rng::AdvanceCycle()
{
    ++mRng;
    mRnd = gRng.get_raw_rand();
}

MATCH_FUNC(0x48B920)
void rng::ShowCycle()
{
    if (gInputRecorder.inlined_check())
    {
        swprintf(tmpBuff,
                 L"%d / %d",
                 this->mRng,
                 gInputRecorder.GetLastRecRngIdx());

    }
    else
    {
        swprintf(tmpBuff, L"%d", this->mRng);


    }
    gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 460, gDebugFont, 1);
}

MATCH_FUNC(0x4F79F0)
PixelsToFix16Table::PixelsToFix16Table()
{
    for (s32 i = 0; i < 256; i++)
    {
        list[i] = Fix16(i) / 64;
    }
}

MATCH_FUNC(0x4F7A40)
void rng::reset_seed()
{
    srand(1);
}

MATCH_FUNC(0x4F7AE0)
s16 rng::get_int(const s16& max_rnd)
{
    if (bLog_random)
    {
        if (!max_rnd)
        {
            this->mRng = 0;
        }
        else
        {
            this->mRng = rand() % max_rnd;
        }
        sprintf(gTmpBuffer, "%d: random (get_int) %d", gpRng->get_cur_rng(), static_cast<s16>(this->get_cur_rng()));
        gWarningLog.Write(gTmpBuffer);
        return static_cast<s16>(this->get_cur_rng());
    }
    else
    {
        if (!max_rnd)
        {
            return 0;
        }
        else
        {
            return rand() % max_rnd;
        }
    }
}

MATCH_FUNC(0x4F7B70)
u8 rng::get_uint8(const u8& max_rnd)
{
    if (bLog_random)
    {
        if (!max_rnd)
        {
            this->mRng = 0;
        }
        else
        {
            this->mRng = rand() % max_rnd;
        }
        sprintf(gTmpBuffer, "%d: random (get_uint8) %d", gpRng->get_cur_rng(), static_cast<u8>(mRng));
        gWarningLog.Write(gTmpBuffer);
        return static_cast<u8>(this->mRng);
    }
    else
    {
        if (!max_rnd)
        {
            return 0;
        }
        else
        {
            return rand() % max_rnd;
        }
    }
}

MATCH_FUNC(0x4F7C00)
s32 rng::get_raw_rand()
{
    return rand();
}