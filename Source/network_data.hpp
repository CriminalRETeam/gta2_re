#pragma once

#include "Function.hpp"

#pragma pack(push, 1)
// What each player sends at the start of a network game, so everyone can check they run the same
// game, data files and settings
struct SyncCheckData_1F
{
    u8 field_0_type; // 5
    u32 field_1_flags; // 1: log random extra, 2: sync check, 4: log random
    s32 field_5_map_size;
    s32 field_9; // not used
    s32 field_D_script_sum;
    s32 field_11_exe_a;
    s32 field_15_exe_b;
    s32 field_19_gci_sum;
    u8 field_1D_bFrench;
    u8 field_1E_player_idx;
};
#pragma pack(pop)


namespace packet_subtype
{
enum
{
    StandardPacket_2 = 2,
    PlayerInputs_3 = 3,
};
} // namespace packet_subtype


namespace std_packet_subtype
{
enum
{
    GiveUp_1 = 1,
    KeepAlive_2 = 2,
    SyncCheckData_5 = 5,
};
} // namespace packet_subtype

