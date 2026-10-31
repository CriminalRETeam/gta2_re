#pragma once

#include "Function.hpp"
#include "Game.hpp"
// Included this way as a hack so > msvc6 can use these headers too as they've been removed in later versions.
#include <VC98/Include/DPLAY.H>
#include <VC98/Include/DPLOBBY.H>
#include <windows.h>

struct NetPlayerBuffer_0x800;
class Game;
class NetUi;

#pragma pack(push,1)
class PacketHeader
{
  public:
    s32 field_0_type; // 0x0
    s32 field_4_sub_type; // 0x4
};

class Packet_Ping
{
  public:
    s32 field_0_sender_player_id; // 0x0
    s32 field_4_receiver_player_id; // 0x4
    s32 field_8_time; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(Packet_Ping, 0xC);

class Standard_Packet
{
  public:
    char_type field_0_std_packet_subtype; // 0x0
    s32 field_1_unused; // 0x1
};

class Packet_SubType_3
{
  public:
    PacketHeader header;
    u8 field_8_seq; // 0x8: mod 256
    s32 field_9_bLoadData; // 0x9
    void* field_D_data; // 0xD
    s32 field_11_len; // 0x11
};
#pragma pack(pop)

class Network_Player
{
  public:
    s32 field_0_in_use; // 0x0
    s32 field_4_num_ping_samples; // 0x4
    s32 field_8_average_ping; // 0x8
    s32 field_C_ping_sum; // 0xC
    s32 field_10_player_id; // 0x10
    u32 field_14_dpname; // 0x14
    s32 field_18_not_used; // 0x18
    wchar_t* field_1C_player_name; // 0x1C
    s32 field_20_not_used; // 0x20: not used
    void* field_24_pData; // 0x24
    s32 field_28_data_len; // 0x28
};

struct Network_Data
{
    u8 field_0_packet_type; // 0x0
    u8 field_1_tick; // 0x1
    u8 field_2_msg_size; // 0x2: field_4_length + 2
    u8 field_3_type; // 0x3
    u8 field_4_length; // 0x4
    void* field_5_data; // 0x5
};

class Network_Game_Session
{
  public:
    s32 field_0_group_id; // 0x0
    u32 field_4_players_count; // 0x4
    u8 field_8_packet_game_tick[MAX_PLAYERS]; // 0x8
    u16 field_E_not_used; // 0xE
    Network_Player field_10_players[MAX_PLAYERS]; // 0x10
    u8* field_118_game_settings; // 0x118
    s32 field_11C_group_data_len; // 0x11C
    DPSESSIONDESC2 field_120_session_desc; // 0x120
};

class EnumeratedConnection
{
  public:
    GUID field_0_sp_guid; // 0x0
    wchar_t* field_10_pConnectionName; // 0x10
    void* field_14_pConnection; // 0x14
    s32 field_18_connection_len; // 0x18
};

struct Network_NameList
{
    wchar_t* field_0_names[16]; // 0x0
    u32 field_40_count; // 0x40
};

struct Network_ChatMessage
{
    wchar_t field_0_message[128]; // 0x0
    wchar_t field_100_name[16]; // 0x100
};

struct Network_PlayerPing
{
    wchar_t field_0_name[260]; // 0x0
    s32 field_208_avg_ping; // 0x208
};

struct Network_IpAddress
{
    wchar_t* field_0_allocated_str; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_IpAddress, 0x4);

struct Network_ConnectionList
{
    EnumeratedConnection* field_0_enumed_connections; // 0x0
    Network_IpAddress* field_4_modem_names; // 0x4
    u32 field_8_connections_count; // 0x8
    u32 field_C_modem_count; // 0xC
    s32 field_10_enum_success; // 0x10
    s32 field_14_dplay_success; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_ConnectionList, 0x18);

struct Network_SessionList
{
    DPSESSIONDESC2 field_C4_sessions[16]; // 0xC4
    u32 field_5C4_session_count; // 0x5C4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_SessionList, 0x504);

struct Network_Buffer
{
    void* field_0_data; // 0x0
    s32 field_4_len; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_Buffer, 0x8);

struct Network_InputData
{
    u32 field_0_Inputs; // 0x0
    u32 field_4_rng; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_InputData, 0x8);

struct Network_InputPacket
{
    Network_InputData field_0_inputs; // 0x0
    s32 field_8_id; // 0x8
    s32 field_C_len; // 0xC
    u8 field_10_used; // 0x10
    char_type field_11_game_tick; // 0x11: mod 256
    char_type field_12_unused; // 0x12
    char_type field_13_unused; // 0x13
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_InputPacket, 0x14);

struct PacketHandlerSlot
{
    void* field_0_param_fn_callback; // 0x0: fn ptr
    void* field_4_param_context; // 0x4: this ptr
    s32 field_8_fn_type; // 0x8
};

struct NetPlay
{
    STATIC_MEMBER(GUID, kGta2_DP_Guid);
    STATIC_MEMBER_ARRAY(s32, sLastPlayerInputs, 6);
    STATIC_MEMBER_ARRAY(char_type, sLastPlayerInputSeq, 24);
    EXPORT NetPlay();
    EXPORT void* vdtor_51D7B0(char_type flags);
    EXPORT virtual ~NetPlay();
    EXPORT void AddEnumeratedConnection_51D930(EnumeratedConnection* pConnectionInfo);
    EXPORT static s32 __stdcall EnumConnections_cb_51DA30(const GUID* lpguidSP,
                                         void* lpConnection,
                                         unsigned long dwConnectionSize,
                                         const DPNAME* lpName,
                                         unsigned long dwFlags,
                                         void* lpContext);
    EXPORT s32 SetProtoAndConnection_51DAE0(GUID* pProtocolGuid, Network_Buffer* pUseThisConnection);
    EXPORT void DirectPlayDestroy_51DC90();
    EXPORT s32 DirectPlayCreate_51DCD0();
    EXPORT s32 DirectPlayCreate_51DED0();
    EXPORT static BOOL PASCAL EnumAddress_cb_51E030(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext);
    EXPORT s32 PushConnection_51E0E0(wchar_t* Source);
    EXPORT s32 CreateTcpIpAddress_51E140(wchar_t* pIpAddress, s32* ppAddress, size_t* pAddressLen);
    EXPORT s32 CreateModemAddress_51E2B0(wchar_t* pPhoneNumber, wchar_t* pModemName, s32* ppAddress, size_t* pAddressLen);
    EXPORT s32 CreateSerialAddress_51E450(DPCOMPORTADDRESS* pComPort, u32* ppAddress, size_t* pAddressLen);
    EXPORT s32 InitializeConnection_51E5C0();
    EXPORT s32 EnumSessions_51E650();
    EXPORT s32 HostSession_51E7A0(wchar_t* pSessionName, wchar_t* pPlayerName, Network_Buffer* pGroupData, Network_Buffer* pPlayerData);
    EXPORT u32 AddPlayer_51E9C0(Network_Buffer* pData, s32 player_id, DPNAME name, Network_Game_Session* pStru);
    EXPORT static s32 __stdcall EnumSessions_cb_51EAE0(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, NetPlay* lpContext);
    EXPORT s32 AddEnumeratedSession_51EB00(DPSESSIONDESC2* pSession);
    EXPORT void Set15_51ECD0(s32 pFunc, NetUi* pParam);
    EXPORT void NetworkTick_51ED00();
    EXPORT s32 SendPing_51EF60();
    EXPORT bool Receive_51F010(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId);
    EXPORT void DispatchReceivedMessage_51F0D0(void* pPacket, s32 packetLen, s32 recvId, s32 bSendOrRecv);
    EXPORT void OnPlayerMessage_51F110(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag);
    EXPORT s32 DecodeReceivedPacket_51F210(s32 pPacket, u32 packetLen); // Also converts enums back
    EXPORT void MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen); // Format data before using IDirectPlay3::Send
    EXPORT void OnPacketReceived_51F870(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag);
    EXPORT s32 MovePlayerToGroup_520040(s32 toFind, Network_Game_Session* pStru, Network_Game_Session* pDst, u32* pOutIdx);
    EXPORT void FreePlayerSlot_5201A0(s32 idx, Network_Game_Session* pStru);
    EXPORT void InvokeCallback_520230(s32 idx, u32 event_data);
    EXPORT void Set6_520530(void* pFunc, void* pParam);
    EXPORT s32 JoinSession_520570(u32 session_idx, wchar_t* pPlayerName, Network_Buffer* pPlayerData, Network_Buffer* pOutGroupData);
    EXPORT static s32 __stdcall EnumGroups_cb_520C20(s32 group_id, s32 callback_type, s32 pName, char_type flags, NetPlay* pContext);
    EXPORT s32 AddEnumeratedGroupPlayer_520CA0(s32 player_id, DPNAME* pName);
    EXPORT void SetGroupId_520D00(s32 group_id);
    EXPORT void Disconnect_520D10();
    EXPORT void ClearPlayersAndSession_520DE0(Network_Game_Session* pStru);
    EXPORT u32 IndexOf_520E30(s32 toFind, Network_Game_Session* pObj);
    EXPORT void Set9_520E60(s32 pFunc, s32 pParam);
    EXPORT void Set3_Disconnect_520E80(s32 pCallback, s32 pContext);
    EXPORT void NoRefs_null_520EA0();
    EXPORT void UpdatePlayerPing_520EB0(s32 player_id, s32 ping, Network_Game_Session* pStru);
    EXPORT void Set18_520F50(s32 pCallback, s32 pContext);
    EXPORT s32 RemovePlayerByName_520F80(wchar_t* String2);
    EXPORT s32 DeletePlayerFromGroup_521000(u32 idx);
    EXPORT s32 SendChatMessage_521060(wchar_t* pMsg, s32 idx_always_m1);
    EXPORT void Set21_5210D0(s32 pCallback, s32 pContext);
    EXPORT void GetPlayerName_521100(wchar_t* Destination, u32 idx);
    EXPORT void Set24_521140(s32 pCallback, s32 pContext);
    EXPORT s32 SetGameSettings_521170(Network_Buffer* pObj);
    EXPORT void Set27SavePlayerName_5211F0(s32 pCallback, s32 pContext);
    EXPORT void DisableJoining_521220();
    EXPORT s32 IsUserHost_5212E0();
    EXPORT void SetExitGameCallBack_521330(s32 pFunc, Game* pGame);
    EXPORT s32 CountActivePlayers_521350();
    EXPORT void SendAcknowledgment_521370();
    EXPORT bool WaitForPlayersSync_5213E0();
    EXPORT s32 CopyConnection_5215B0(u32 idx, u32* ppConnection, size_t* pLen);
    EXPORT s32 SendToPlayer_521630(Network_Buffer* pSendData, s32 player_idx, char_type delayed_ticks);
    EXPORT void Add_5216E0(Network_Buffer* pData, s32 id, char_type game_tick);
    EXPORT u32 GetOldestBufferedPacket_521770(Network_Buffer* pOut, char_type* pSeq, u32* pPlayerId);
    EXPORT void ResolvePlayerInput_521820(s32** ppData, s32 idx);
    EXPORT void Remove_521870(s32 idx);
    EXPORT bool ReceiveGameMessage_521890(Network_Buffer* pOut, s32* pPlayerIdx, u32* packet_subtype);
    EXPORT void SendToAll_521B20(Network_Buffer* pSendData);
    EXPORT s32 NoRefs_Send_521BE0(Network_Buffer* pSendData, s32 tick_delay);
    EXPORT s32 NoRefs_Send_521C80(s32 pSendData);
    EXPORT s32 SendPlayerIsQuitting_521D20();
    EXPORT s32 SendGiveUp_521DB0(s32 value);
    EXPORT s32 SendSyncCheckData_521E40(s32 pSendData);
    EXPORT static void static_dtor_5E4DD0();

    // 9.6f 0x461DA0
    inline u32 GetPlayerCount_461DA0()
    {
        return field_758_n2.field_4_players_count;
    }

    // 9.6f 0x409C40
    inline s32 GetPlayerIdx_409C40()
    {
        return field_5D4_player_idx;
    }

    char_type field_4_bModem; // 0x4
    char_type field_5_modem_num; // 0x5
    char_type field_6_unused; // 0x6
    char_type field_7_unused; // 0x7
    GUID field_8_ip_or_ipx_guid; // 0x8
    s32 field_18_unused; // 0x18
    s32 field_1C_unused; // 0x1C
    s32 field_20_unused; // 0x20
    s32 field_24_unused; // 0x24
    s32 field_28_unused; // 0x28
    s32 field_2C_ptrs; // 0x2C
    Network_ConnectionList field_30_enumed_connections; // 0x30
    s32 field_48_net_state; // 0x48
    PacketHandlerSlot field_4C_func_ptrs_and_params[10]; // 0x4C
    Network_SessionList field_C4_sessions; // 0xC4
    s32 field_5C8_not_used; // 0x5C8
    s32 field_5CC_bIsHost; // 0x5CC
    s32 field_5D0_player_og_idx; // 0x5D0: Only set for the host, but it's not used
    s32 field_5D4_player_idx; // 0x5D4: 1, 2, ..., MAX_PLAYERS
    s32 field_5D8_player_id; // 0x5D8: From DPMSG_ADDPLAYERTOGROUP::dpIdPlayer
    HANDLE field_5DC_handle; // 0x5DC
    IDirectPlayLobby2* field_5E0_pDPlayLobby2; // 0x5E0
    IDirectPlay3* field_5E4_pDPlay3; // 0x5E4
    Network_Game_Session field_5E8_n1; // 0x5E8
    Network_Game_Session field_758_n2; // 0x758
    NetPlayerBuffer_0x800* field_8C8_not_used[MAX_PLAYERS]; // 0x8C8
    s32 field_8E0_pSendDataBuffer; // 0x8E0: 0x1800 of alloc size
    s32 field_8E4_pReceivedDataBuffer; // 0x8E4: 0x1800 of alloc size
    s32 field_8E8_time; // 0x8E8
    s32 field_8EC_unused; // 0x8EC
    char_type field_8F0_receive_disabled; // 0x8F0: Receive_51F010 and ReceiveGameMessage_521890 always returns false if this var is true
    char_type field_8F1_unused; // 0x8F1
    char_type field_8F2_unused; // 0x8F2
    char_type field_8F3_unused; // 0x8F3
    s32 field_8F4_time_diff; // 0x8F4
    Network_InputPacket field_8F8_packets[48]; // 0x8F8
    s32 field_CB8_count; // 0xCB8
};

EXTERN_GLOBAL(NetPlay, gNetPlay_7071E8);
