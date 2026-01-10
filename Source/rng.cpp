#include "rng.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "Hud.hpp"
#include "BurgerKing_67F8B0.hpp"
#include <stdio.h>
#include <stdlib.h>

DEFINE_GLOBAL(rng*, rng_dword_67AB34, 0x67AB34);
DEFINE_GLOBAL(rng, stru_6F6784, 0x6F6784);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

MATCH_FUNC(0x48B900)
void rng::sub_48B900()
{
    ++field_0_rng;
    field_4_rnd = stru_6F6784.rand_4F7C00();
}

WIP_FUNC(0x48B920)
void rng::ShowCycle_48B920()
{
    WIP_IMPLEMENTED;

    if (gBurgerKing_67F8B0.inlined_check())
    {
        // TODO: Access to field_3C_rec_buff is wrong
        swprintf(tmpBuff_67BD9C,
                 L"%d / %d",
                 this->field_0_rng,
                 gBurgerKing_67F8B0.field_3C_rec_buff[gBurgerKing_67F8B0.field_7533C_used_recs_count].field_8_rng_rnd);

    }
    else
    {
        swprintf(tmpBuff_67BD9C, L"%d", this->field_0_rng);


    }
    gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 460, word_706600, 1);
}

MATCH_FUNC(0x4F7A40)
void rng::srand_4F7A40()
{
    srand(1);
}

MATCH_FUNC(0x4F7AE0)
s16 rng::get_int_4F7AE0(s16* max_rnd)
{
    if (bLog_random_67D5FC)
    {
        if (!*max_rnd)
        {
            this->field_0_rng = 0;
        }
        else
        {
            this->field_0_rng = rand() % *max_rnd;
        }
        sprintf(gTmpBuffer_67C598, "%d: random (get_int) %d", rng_dword_67AB34->field_0_rng, (s16)this->field_0_rng);
        gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
        return (s16)this->field_0_rng;
    }
    else
    {
        if (!*max_rnd)
        {
            return 0;
        }
        else
        {
            return rand() % *max_rnd;
        }
    }
}

MATCH_FUNC(0x4F7B70)
u8 rng::get_uint8_4F7B70(u8* max_rnd)
{
    if (bLog_random_67D5FC)
    {
        if (!*max_rnd)
        {
            this->field_0_rng = 0;
        }
        else
        {
            this->field_0_rng = rand() % *max_rnd;
        }
        sprintf(gTmpBuffer_67C598, "%d: random (get_uint8) %d", rng_dword_67AB34->field_0_rng, (u8)field_0_rng);
        gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
        return (u8)this->field_0_rng;
    }
    else
    {
        if (!*max_rnd)
        {
            return 0;
        }
        else
        {
            return rand() % *max_rnd;
        }
    }
}

MATCH_FUNC(0x4F7C00)
int rng::rand_4F7C00()
{
    return rand();
}