#pragma once

#include "Function.hpp"
#include <WINDOWS.H>

struct AttractFile
{
    char field_0_path[256];
};

#pragma pack(push)
#pragma pack(1)
class BurgerKing_1
{
  public:
    EXPORT void __stdcall sub_4987A0();

    u8 field_0;
};
#pragma pack(pop)

struct BurgerKingBurger_0xC
{
    s32 field_0_rng_idx;
    s32 field_4_inputs;
    s32 field_8_rng_rnd;
};

class BurgerKing_67F8B0
{
  public:
    EXPORT void sub_4CDCD0(); // static dtor
    EXPORT void clear_inputs_4CDCE0();
    EXPORT void set_input_4CDCF0(s32 mask_idx);
    EXPORT void clear_input_4CDD10(s32 mask_idx);
    EXPORT bool should_ignore_input_4CDD80(s32 dinput_key);
    EXPORT bool should_ignore_input_4CDDF0(s32 dinput_key);
    EXPORT void save_replay_record_4CDE20(u32 inputs);
    EXPORT void SaveReplay_4CDED0();
    EXPORT void sub_4CDF30(s32 a2);
    EXPORT void AppendReplayHeader_4CDF70();
    EXPORT char_type LoadReplayHeader_4CE380(char_type a1);
    EXPORT void sub_4CE650();
    EXPORT void GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut);
    EXPORT s32 sub_4CE740(HINSTANCE a2);
    EXPORT void sub_4CE880(HINSTANCE a2);
    EXPORT void Shutdown_4CEA00();
    EXPORT void sub_4CEA40(u32* a2);
    EXPORT u32 sub_4CEAC0();
    EXPORT void sub_4CED00(s32 a2, s32 a3);
    EXPORT void sub_4CED90();
    EXPORT bool RecOrPlayBackState_4CEDF0();
    EXPORT void ShowInput_4CEE10();

    char_type field_0_bShutDown;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4_input_bits;
    s32 field_8_input_masks[12];
    s32 field_38_replay_state;
    BurgerKingBurger_0xC field_3C_rec_buff[40000];
    s32 field_7533C_used_recs_count;
    u32 field_75340_rec_buf_idx;
    // front end input on/off ??
    char_type field_75344_bSomething;
    u8 field_75345_attract_idx;
    s16 field_75346;
};

EXTERN_GLOBAL(BurgerKing_67F8B0, gBurgerKing_67F8B0);

EXTERN_GLOBAL(BurgerKing_1*, gBurgerKing_1_67B990);
