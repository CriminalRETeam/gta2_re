#include "BurgerKing_67F8B0.hpp"
#include "debug.hpp"
#include "file.hpp"
#include <io.h>

#define ATTRACT_COUNT 3u

STUB_FUNC(0x4cdcd0)
void BurgerKing_67F8B0::sub_4CDCD0()
{

}

MATCH_FUNC(0x4cdce0)
void BurgerKing_67F8B0::sub_4CDCE0()
{
    field_4_input_bits &= ~0xFFFFF000;
}

MATCH_FUNC(0x4cdcf0)
void BurgerKing_67F8B0::sub_4CDCF0(s32 a2)
{
    field_4_input_bits |= field_8[a2];
}

MATCH_FUNC(0x4cdd10)
void BurgerKing_67F8B0::sub_4CDD10(s32 a2)
{
    field_4_input_bits &= ~field_8[a2];
}

STUB_FUNC(0x4cdd80)
s32 BurgerKing_67F8B0::sub_4CDD80(s32 a1)
{
    // needs Garox_2B00 stub
    return 0;
}

STUB_FUNC(0x4cddf0)
s32 BurgerKing_67F8B0::sub_4CDDF0(s32 a1)
{
    return !sub_4CDD80(a1) && a1 != 78;
}

STUB_FUNC(0x4cde20)
void BurgerKing_67F8B0::sub_4CDE20(size_t a3)
{

}

MATCH_FUNC(0x4cded0)
void BurgerKing_67F8B0::SaveReplay_4CDED0()
{
    if (bConstant_replay_save_67D5C4 != 1 &&
        bDo_release_replay_67D4EB &&
        field_38_replay_state != 2 &&
        field_75340_rec_len > 0 &&
        !RecOrPlayBackState_4CEDF0())
    {
        size_t len = 12 * field_75340_rec_len;
        File::AppendBufferToFile_4A6F50(
            "test\\replay.rep",
            field_3C_rec_buff,
            &len);
    }
}

STUB_FUNC(0x4cdf30)
void BurgerKing_67F8B0::sub_4CDF30(s32 a2)
{

}

STUB_FUNC(0x4cdf70)
void BurgerKing_67F8B0::AppendReplayHeader_4CDF70()
{

}

STUB_FUNC(0x4ce380)
char_type BurgerKing_67F8B0::LoadReplayHeader_4CE380(char_type bLoadDebug)
{
    return 0;
}

STUB_FUNC(0x4ce650)
void BurgerKing_67F8B0::sub_4CE650()
{

}
AttractFile attractFiles_62083C[ATTRACT_COUNT] =
{
    "data\\attract\\attr1.rep",
    "data\\attract\\attr2.rep",
    "data\\attract\\attr3.rep"
};

STUB_FUNC(0x4ce6e0)
void BurgerKing_67F8B0::GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut)
{
    strcpy(pAttrPathOut, attractFiles_62083C[field_75345_attract_idx].field_0_path);

    if (++field_75345_attract_idx >= ATTRACT_COUNT)
    {
        field_75345_attract_idx = 0;
    }
}

STUB_FUNC(0x4ce740)
s32 BurgerKing_67F8B0::sub_4CE740(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4ce880)
void BurgerKing_67F8B0::sub_4CE880(s32 a2)
{

}

STUB_FUNC(0x4cea00)
BurgerKing_67F8B0::~BurgerKing_67F8B0() // 4CEA00
{

}

STUB_FUNC(0x4cea40)
void BurgerKing_67F8B0::sub_4CEA40(u32 * a2)
{

}

STUB_FUNC(0x4ceac0)
u32 BurgerKing_67F8B0::sub_4CEAC0()
{
    return 0;
}

STUB_FUNC(0x4ced00)
void BurgerKing_67F8B0::sub_4CED00(s32 a2, s32 a3)
{

}

STUB_FUNC(0x4ced90)
void BurgerKing_67F8B0::sub_4CED90()
{

}

STUB_FUNC(0x4cedf0)
bool BurgerKing_67F8B0::RecOrPlayBackState_4CEDF0()
{
    return field_38_replay_state == 1 || field_38_replay_state == 3;
}

STUB_FUNC(0x4cee10)
void BurgerKing_67F8B0::ShowInput_4CEE10()
{

}

