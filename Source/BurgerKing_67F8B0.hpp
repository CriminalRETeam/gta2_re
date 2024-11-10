#pragma once

#include "types.hpp"

struct AttractFile
{
     char field_0_path[256];
};


class BurgerKing_67F8B0
{
public:
    EXPORT void sub_4CDCD0();
    EXPORT void sub_4CDCE0();
    EXPORT void sub_4CDCF0(s32 a2);
    EXPORT void sub_4CDD10(s32 a2);
    EXPORT s32 sub_4CDD80(s32 a1);
    EXPORT s32 sub_4CDDF0(s32 a1);
    EXPORT void sub_4CDE20(size_t a3);
    EXPORT void SaveReplay_4CDED0();
    EXPORT void sub_4CDF30(s32 a2);
    EXPORT void AppendReplayHeader_4CDF70();
    EXPORT char_type LoadReplayHeader_4CE380(char_type a1);
    EXPORT void sub_4CE650();
    EXPORT void GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut);
    EXPORT s32 sub_4CE740(s32 a2);
    EXPORT void sub_4CE880(s32 a2);
    EXPORT ~BurgerKing_67F8B0(); // 4CEA00
    EXPORT void sub_4CEA40(u32 * a2);
    EXPORT u32 sub_4CEAC0();
    EXPORT void sub_4CED00(s32 a2, s32 a3);
    EXPORT void sub_4CED90();
    EXPORT bool RecOrPlayBackState_4CEDF0();
    EXPORT void ShowInput_4CEE10();

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4_input_bits;
    s32 field_8[12];
    s32 field_38_replay_state;
    char_type field_3C_rec_buff[480000];
    s32 field_7533C;
    u32 field_75340_rec_len;
    char_type field_75344;
    u8 field_75345_attract_idx;
    s16 field_75346;
};

// TODO: size does not match
//GTA2_ASSERT_SIZEOF_ALWAYS(BurgerKing_67F8B0, 0x67F8B0)
