#include "BurgerKing_67F8B0.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "file.hpp"
#include "input.hpp"
#include <io.h>

#define ATTRACT_COUNT 3

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

// GLOBAL: 105 0x67F8B0
DEFINE_GLOBAL(BurgerKing_67F8B0, gBurgerKing_67F8B0);
// GLOBAL: 105 0x67B990
DEFINE_GLOBAL(BurgerKing_1*, gBurgerKing_1_67B990);

const AttractFile attractFiles_62083C[ATTRACT_COUNT] = {"data\\attract\\attr1.rep", "data\\attract\\attr2.rep", "data\\attract\\attr3.rep"};

// FUNCTION: 105 0x4987A0
void BurgerKing_1::sub_4987A0()
{
    if (gpDInput_67B804)
    {
        if (gKeyboardDevice_67B5C0)
        {
            gKeyboardDevice_67B5C0->Unacquire();
            gKeyboardDevice_67B5C0->Release();
            gKeyboardDevice_67B5C0 = 0;
        }

        if (gGamePadDevice_67B6C0)
        {
            gGamePadDevice_67B6C0->Unacquire();
            gGamePadDevice_67B6C0->Release();
            gGamePadDevice_67B6C0 = 0;
        }
    }
}

// ================================================

// STUB: 105 0x4cdcd0
void BurgerKing_67F8B0::sub_4CDCD0()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4cdce0
void BurgerKing_67F8B0::sub_4CDCE0()
{
    field_4_input_bits &= ~0xFFFFF000;
}

// FUNCTION: 105 0x4cdcf0
void BurgerKing_67F8B0::sub_4CDCF0(s32 a2)
{
    field_4_input_bits |= field_8[a2];
}

// FUNCTION: 105 0x4cdd10
void BurgerKing_67F8B0::sub_4CDD10(s32 a2)
{
    field_4_input_bits &= ~field_8[a2];
}

// STUB: 105 0x4cdd80
s32 BurgerKing_67F8B0::sub_4CDD80(s32 a1)
{
    NOT_IMPLEMENTED;
    return a1 == 79 || a1 == 80 || a1 == 81 || a1 == 75 || a1 == 76 || a1 == 77 || a1 == 71 || a1 == 72 || a1 == 73 || a1 == 55 ||
        a1 == 74 || a1 == 1 || a1 == 64 || a1 == 78 || gGarox_2B00_706620->sub_5D6CB0(a1);
}

// STUB: 105 0x4cddf0
s32 BurgerKing_67F8B0::sub_4CDDF0(s32 a1)
{
    NOT_IMPLEMENTED;
    return !sub_4CDD80(a1) && a1 != 78;
}

// STUB: 105 0x4cde20
void BurgerKing_67F8B0::sub_4CDE20(size_t a3)
{
    NOT_IMPLEMENTED;
}

// STRING: 105 0x620b3c
#define REPLAY_STRING "data\\replay.rep"

// FUNCTION: 105 0x4cded0
void BurgerKing_67F8B0::SaveReplay_4CDED0()
{
    if (bConstant_replay_save_67D5C4 != 1 && bDo_release_replay_67D4EB && field_38_replay_state != 2 && field_75340_rec_len > 0 &&
        !RecOrPlayBackState_4CEDF0())
    {
        size_t len = 12 * field_75340_rec_len;
        File::AppendBufferToFile_4A6F50(REPLAY_STRING, field_3C_rec_buff, &len);
    }
}

// STUB: 105 0x4cdf30
void BurgerKing_67F8B0::sub_4CDF30(s32 a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x4cdf70
void BurgerKing_67F8B0::AppendReplayHeader_4CDF70()
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x4ce380
char_type BurgerKing_67F8B0::LoadReplayHeader_4CE380(char_type bLoadDebug)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x4ce650
void BurgerKing_67F8B0::sub_4CE650()
{
    const AttractFile* attr1FilePath = &attractFiles_62083C[0];
    for (s32 i = 0; i < 3; i++)
    {
        struct _finddata_t fileInfo;
        long hFind = _findfirst(attr1FilePath->field_0_path, &fileInfo);
        if (hFind == -1)
        {
            strcpy(gErrStr_67C29C, attr1FilePath->field_0_path);
            FatalError_4A38C0(0x1BE5, "C:\\Splitting\\Gta2\\Source\\input.cpp", 524);
        }
        _findclose(hFind);
        ++attr1FilePath;
    }
}

// match but the post processor is bugged
// STUB: 105 0x4ce6e0
void BurgerKing_67F8B0::GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut)
{
    NOT_IMPLEMENTED;
    strcpy(pAttrPathOut, attractFiles_62083C[field_75345_attract_idx].field_0_path);

    if (++field_75345_attract_idx >= ATTRACT_COUNT)
    {
        field_75345_attract_idx = 0;
    }
}

// STUB: 105 0x4ce740
s32 BurgerKing_67F8B0::sub_4CE740(HINSTANCE a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x4ce880
void BurgerKing_67F8B0::sub_4CE880(HINSTANCE a2)
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4cea00
void BurgerKing_67F8B0::Shutdown_4CEA00() // 4CEA00
{
    if (!field_0)
    {
        field_0 = 1;
        gBurgerKing_1_67B990->sub_4987A0();
        SaveReplay_4CDED0();
        GTA2_DELETE_AND_NULL(gBurgerKing_1_67B990);
    }
}

// STUB: 105 0x4cea40
void BurgerKing_67F8B0::sub_4CEA40(u32* a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x4ceac0
u32 BurgerKing_67F8B0::sub_4CEAC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x4ced00
void BurgerKing_67F8B0::sub_4CED00(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4ced90
void BurgerKing_67F8B0::sub_4CED90()
{
    s8 i = 0;
    s32 bit_idx = 0;
    do
    {
        if (((1 << bit_idx) & field_4_input_bits) != 0)
        {
            swprintf(tmpBuff_67BD9C, L"Control %d", bit_idx);
            gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 10, 16 * (i + 1), word_706600, 1);
        }
        ++i;
        ++bit_idx;
    } while (i < 12);
}

// FUNCTION: 105 0x4cedf0
bool BurgerKing_67F8B0::RecOrPlayBackState_4CEDF0()
{
    if (field_38_replay_state == 1 || field_38_replay_state == 3)
    {
        return true;
    }
    return false;
}

// FUNCTION: 105 0x4cee10
void BurgerKing_67F8B0::ShowInput_4CEE10()
{
    if (RecOrPlayBackState_4CEDF0())
    {
        gGarox_2B00_706620->field_650.sub_5D1F50(L"PLAYBACK", -1, 0, word_706600, 1);
    }
    else
    {
        gGarox_2B00_706620->field_650.sub_5D1F50(L"RECORDING", -1, 0, word_706600, 1);
    }
    sub_4CED90();
}