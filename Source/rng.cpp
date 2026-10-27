#include "rng.hpp"
#include "CarInfoTable.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "Hud.hpp"
#include "InputRecorder_67F8B0.hpp"
#include <stdio.h>
#include <stdlib.h>

DEFINE_GLOBAL(rng*, gpRng_67AB34, 0x67AB34);
DEFINE_GLOBAL(rng, gRng_6F6784, 0x6F6784);
DEFINE_GLOBAL(PixelsToFix16Table, gPixelsToFix16_6F6850, 0x6F6850);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

MATCH_FUNC(0x48B900)
void rng::AdvanceCycle_48B900()
{
    ++field_0_rng;
    field_4_rnd = gRng_6F6784.rand_4F7C00();
}

MATCH_FUNC(0x48B920)
void rng::ShowCycle_48B920()
{
    if (gInputRecorder_67F8B0.inlined_check())
    {
        swprintf(tmpBuff_67BD9C,
                 L"%d / %d",
                 this->field_0_rng,
                 gInputRecorder_67F8B0.GetLastRecRngIdx_44AA80());

    }
    else
    {
        swprintf(tmpBuff_67BD9C, L"%d", this->field_0_rng);


    }
    gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 460, gDebugFont_706600, 1);
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
void rng::srand_4F7A40()
{
    srand(1);
}

MATCH_FUNC(0x4F7AE0)
s16 rng::get_int_4F7AE0(const s16& max_rnd)
{
    if (bLog_random_67D5FC)
    {
        if (!max_rnd)
        {
            this->field_0_rng = 0;
        }
        else
        {
            this->field_0_rng = rand() % max_rnd;
        }
        sprintf(gTmpBuffer_67C598, "%d: random (get_int) %d", gpRng_67AB34->get_cur_rng_41CFE0(), static_cast<s16>(this->get_cur_rng_41CFE0()));
        gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);
        return static_cast<s16>(this->get_cur_rng_41CFE0());
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
u8 rng::get_uint8_4F7B70(const u8& max_rnd)
{
    if (bLog_random_67D5FC)
    {
        if (!max_rnd)
        {
            this->field_0_rng = 0;
        }
        else
        {
            this->field_0_rng = rand() % max_rnd;
        }
        sprintf(gTmpBuffer_67C598, "%d: random (get_uint8) %d", gpRng_67AB34->get_cur_rng_41CFE0(), static_cast<u8>(field_0_rng));
        gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);
        return static_cast<u8>(this->field_0_rng);
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
s32 rng::rand_4F7C00()
{
    return rand();
}