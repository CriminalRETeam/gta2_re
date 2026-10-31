// Built with /GX- (see cmake/vc6.cmake) to match, the STL headers pulled in warn about that
#pragma warning(disable : 4530)

#include "FpsCounter.hpp"
#include "Function.hpp"
#include "Hud.hpp"
#include <windows.h>

DEFINE_GLOBAL(FpsCounter*, gFpsCounter, 0x7055D4);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);


MATCH_FUNC(0x5BEC30)
void FpsCounter::ShowFps()
{
    swprintf(tmpBuff, L"%d/%d fps", mUpdateFps, mDrawFps);
    gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 0, gDebugFont, 1);
}

MATCH_FUNC(0x5BEC70)
FpsCounter::FpsCounter()
{
    mUpdateFps = 0;
    mDrawFps = 0;
    mUpdateCount = 0;
    mDrawCount = 0;
    mUpdateStartTime = timeGetTime();
    mDrawStartTime = timeGetTime();
}

MATCH_FUNC(0x5BECC0)
FpsCounter::~FpsCounter()
{

}

MATCH_FUNC(0x5BECF0)
void FpsCounter::UpdateFpsCounters(char_type bDrewFrame, char_type bUpdatedLogic)
{
    if (bUpdatedLogic)
    {
        mUpdateCount++;

        if (mUpdateCount == 100)
        {
            u32 elapsed_ms = timeGetTime() - mUpdateStartTime;
            mUpdateFps = 100000 / elapsed_ms;
            mExecuteGameTimer.mTimePercent = mExecuteGameTimer.mAccumTime * 100 / elapsed_ms;
            mTimer1.mTimePercent = mTimer1.mAccumTime * 100 / elapsed_ms;
            mTimer2.mTimePercent = mTimer2.mAccumTime * 100 / elapsed_ms;
            mTimer3.mTimePercent = mTimer3.mAccumTime * 100 / elapsed_ms;
            mTimer4.mTimePercent = mTimer4.mAccumTime * 100 / elapsed_ms;
            mUpdateStartTime = timeGetTime();
            mUpdateCount = 0;
            mExecuteGameTimer.mAccumTime = 0;
            mTimer1.mAccumTime = 0;
            mTimer2.mAccumTime = 0;
            mTimer3.mAccumTime = 0;
            mTimer4.mAccumTime = 0;
        }
    }

    if (bDrewFrame)
    {
        mDrawCount++;
        if (mDrawCount == 100)
        {
            mDrawFps = 100000 / (timeGetTime() - mDrawStartTime);
            mDrawStartTime = timeGetTime();
            mDrawCount = 0;
        }
    }
}