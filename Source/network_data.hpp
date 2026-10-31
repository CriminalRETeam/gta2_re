#pragma once

#include "Function.hpp"
#include "Game.hpp"
#include "NetPlay.hpp"

#pragma pack(push, 1)
// What each player sends at the start of a network game, so everyone can check they run the same
// game, data files and settings
struct SyncCheckData
{
    u8 mType; // 0x0: 5
    u32 mFlags; // 0x1: 1: log random extra, 2: sync check, 4: log random
    s32 mMapSize; // 0x5
    s32 mUnused9; // 0x9: not used
    s32 mScriptSum; // 0xD
    s32 mExeA; // 0x11
    s32 mExeB; // 0x15
    s32 mGciSum; // 0x19
    u8 mbFrench; // 0x1D
    u8 mPlayerIdx; // 0x1E
};
GTA2_ASSERT_SIZEOF_ALWAYS(SyncCheckData, 0x1F);


struct SPacket
{
    u8 mSubtype; // 0x0
    void* mpData; // 0x1
};

struct Network_PlayerInputs
{
    Network_InputData mInputs[MAX_PLAYERS]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_PlayerInputs, 0x30);

union ReceivedPacket
{
    Network_InputData input_data;
    SPacket s_packet;
    Network_Buffer net_buffer;
};
#pragma pack(pop)


namespace pkt_data_type // first byte of data sent/received
{
enum
{
    PlayerInputsAndTick_1 = 1,
    Ping_2 = 2,
    SyncCheckData_3 = 3,
    Acknowledgment_4 = 4, // carries a packet_type::Acknowledgment_2 packet
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
    Acknowledgment_2 = 2,
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
    GiveUp_1 = 1, // Player is not responding/answering, so disconnect it
    PlayerQuit_2 = 2, // Player is exiting the game
    KickFromConnection_3 = 3, // Finish the connection of the receiver
    SyncCheckData_5 = 5, // Checksum data
};
} // namespace std_packet_subtype

