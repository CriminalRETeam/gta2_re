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
    s32 field_9_unused; // not used
    s32 field_D_script_sum;
    s32 field_11_exe_a;
    s32 field_15_exe_b;
    s32 field_19_gci_sum;
    u8 field_1D_bFrench;
    u8 field_1E_player_idx;
};
#pragma pack(pop)

/*   TODO: unify all packets using unions
union ReceivedPacket
{
    struct InputData_0x8
    {
        u32 field_0_Inputs;
        u32 field_4_rng;
    };
    struct SPacket
    {
        u8 field_0_subtype;
        void* field_4_pData;
    };
};
*/


namespace pkt_data_type // first byte of data sent/received
{
enum
{
    PlayerInputsAndTick_1 = 1,
    Ping_2 = 2,
    SyncCheckData_3 = 3,
    Unknown_4 = 4,
    PlayerQuit_5 = 5,
    GiveUp_6 = 6,
    KickFromConnection_7 = 7,
    DisableJoining_8 = 8,
    PlayerTickOnly_9 = 9,
};
} // namespace pkt_data_type


namespace packet_type
{
enum
{
    DataPacket_1 = 1,
    Type2_2 = 2,
};
} // namespace packet_type


namespace packet_subtype
{
enum
{
    Ping_1 = 1,
    StandardPacket_2 = 2,
    PlayerInputs_3 = 3,
    DisableJoining_4 = 4,
};
} // namespace packet_subtype


namespace std_packet_subtype
{
enum
{
    GiveUp_1 = 1, // Player is not responding, so disconnect it
    PlayerQuit_2 = 2, // Player is exiting the game
    KickFromConnection_3 = 3,
    SyncCheckData_5 = 5, // Checksum data
};
} // namespace std_packet_subtype

