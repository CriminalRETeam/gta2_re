#pragma once

#include "Function.hpp"
// Included this way as a hack so > msvc6 can use these headers too as they've been removed in later versions.
#include <VC98/Include/DPLAY.H>
#include <VC98/Include/DPLOBBY.H>
#include <windows.h>

struct naughty_sinoussi_0x800;
class Game_0x40;
class Network_20324;

#pragma pack(push,1)
class PacketHeader
{
  public:
    s32 field_0_type;
    s32 field_4_sub_type;
};

class Packet_Ping_C
{
  public:
    s32 field_0_player_id;
    s32 field_4;
    s32 field_8_time;
};

class Packet_Byte_S32
{
  public:
    char_type field_0;
    s32 field_1;
};

class Packet_SubType_3
{
  public:
    PacketHeader header;
    char_type field_8;
    s32 field_9;
    s32 field_D;
    s32 field_11_len;
};
#pragma pack(pop)

class Nework_2C
{
  public:
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    wchar_t* field_1C; // player name?
    s32 field_20;
    s32 field_24;
    s32 field_28;
};

class Network_Unknown
{
  public:
    s32 field_0_group_id;
    u32 field_4_count;
    char_type field_8[4];
    s32 field_C;
    Nework_2C field_10[6];
    u8* field_118;
    s32 field_11C;
    DPSESSIONDESC2 field_120_session_desc;
};

class EnumeratedConnection
{
  public:
    GUID field_0_sp_guid;
    wchar_t* field_10_pConnectionName;
    void* field_14_pConnection;
    s32 field_18_connection_len;
};

struct Network_NameList
{
    wchar_t* field_0_names[16];
    u32 field_40_count;
};

struct Network_PlayerPing
{
    wchar_t field_0_name[260];
    s32 field_208_avg_ping;
};

struct Network_4
{
    wchar_t* field_0_allocated_str;
};

struct Network_8
{
    void* field_0;
    s32 field_4_len;
};

struct Network_18
{
    EnumeratedConnection* field_0_enumed_connections;
    Network_4* field_4_d_array_8_entries;
    s32 field_8_connections_count;
    u32 field_C_f4_d_array_count;
    s32 field_10;
    s32 field_14;
};

struct Network_504
{
    DPSESSIONDESC2 field_C4_sessions[16];
    u32 field_5C4_session_count;
};

struct Connection_Unknown
{
    void* field_0;
    s32 field_4_len;
};

struct Network_InputData_0x8
{
    u32 field_0_Inputs;
    u32 field_4_rng;
};

struct Network_14
{
    Network_InputData_0x8 field_0_inputs;
    s32 field_8_id;
    s32 field_C;
    u8 field_10_used;
    char_type field_11_type;
    char field_12;
    char field_13;
};

struct Network_Unknown_0x30
{
    Network_InputData_0x8 field_0_inputs[6];
};

struct PacketHandlerSlot
{
    void* field_0_param_fn_callback; // fn ptr
    void* field_4_param_context; // this ptr
    s32 field_8_fn_type;
};

struct NetPlay
{
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
    EXPORT s32 SetProtoAndConnection_51DAE0(GUID* pProtocolGuid, Connection_Unknown* pUseThisConnection);
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
    EXPORT s32 sub_51E7A0(wchar_t* Source, wchar_t* a3, s32 a4, s32* a5);
    EXPORT u32 AddPlayer_51E9C0(Network_8* pData, s32 player_id, DPNAME name, Network_Unknown* pStru);
    EXPORT static s32 __stdcall EnumSessions_cb_51EAE0(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, NetPlay* lpContext);
    EXPORT s32 AddEnumeratedSession_51EB00(DPSESSIONDESC2* pSession);
    EXPORT void Set15_51ECD0(s32 pFunc, Network_20324* pParam);
    EXPORT void NetworkTick_51ED00();
    EXPORT s32 SendPing_51EF60();
    EXPORT char_type Receive_51F010(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId);
    EXPORT void SendOrReceivePacket_51F0D0(void* pPacket, s32 a3, s32 a4, s32 a5);
    EXPORT void ProcessPingOrHandshakeSend_51F110(void* pPacket, s32 a3, s32 a4, s32 a5);
    EXPORT s32 CalcPacketLen_51F210(u32 pPacket);
    EXPORT void MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen);
    EXPORT void OnPacketReceived_51F870(void* pPacket, s32 packetLen, s32 recvId, s32 a5);
    EXPORT s32 MovePlayerToGroup_520040(s32 toFind, Network_Unknown* pStru, Network_Unknown* pDst, u32* pOutIdx);
    EXPORT void FreePlayerSlot_5201A0(s32 idx, Network_Unknown* pStru);
    EXPORT void ProcessIncomingPacket_520230(s32 idx, u32 pUnknown);
    EXPORT void Set6_520530(void* pFunc, void* pParam);
    EXPORT s32 sub_520570(int session_idx, wchar_t* a3, s32* a4, s32* a5);
    EXPORT s32 EnumGroups_cb_520C20(s32 a1, s32 a2, s32 a3, char_type a4, NetPlay* pContext);
    EXPORT s32 AddEnumeratedGroupPlayer_520CA0(s32 player_id, DPNAME* pName);
    EXPORT void sub_520D00(s32 a2);
    EXPORT void Disconnect_520D10();
    EXPORT void sub_520DE0(Network_Unknown* pStru);
    EXPORT u32 IndexOf_520E30(s32 toFind, Network_Unknown* pObj);
    EXPORT void Set9_520E60(s32 pFunc, s32 pParam);
    EXPORT void Set3_Disconnect_520E80(s32 a2, s32 a3);
    EXPORT void NoRefs_null_520EA0();
    EXPORT void UpdatePlayerPing_520EB0(s32 player_id, s32 ping, Network_Unknown* pStru);
    EXPORT void Set18_520F50(s32 a2, s32 a3);
    EXPORT s32 RemovePlayerByName_520F80(wchar_t* String2);
    EXPORT s32 DeletePlayerFromGroup_521000(u32 idx);
    EXPORT s32 SendChatMessage_521060(wchar_t* pMsg, s32 idx_always_m1);
    EXPORT void Set21_5210D0(s32 a2, s32 a3);
    EXPORT void GetPlayerName_521100(wchar_t* Destination, u32 idx);
    EXPORT void Set24_521140(s32 a2, s32 a3);
    EXPORT s32 sub_521170(Network_8* a2);
    EXPORT void Set27SavePlayerName_5211F0(s32 a2, s32 a3);
    EXPORT void DisableJoining_521220();
    EXPORT void SetExitGameCallBack_521330(s32 pFunc, Game_0x40* pGame);
    EXPORT s32 GetMaxPlayers_521350();
    EXPORT void Send_521370();
    EXPORT bool sub_5213E0();
    EXPORT s32 CopyConnection_5215B0(u32 a2, u32* a3, size_t* a4);
    EXPORT s32 SendToPlayer_521630(Network_8* pSendData, s32 idx, char_type a4);
    EXPORT void Add_5216E0(Network_8* pData, s32 id, char_type type);
    EXPORT u32 sub_521770(Network_8* pOut, char_type* pSeq, u32* pPlayerId);
    EXPORT void sub_521820(s32** a2, s32 idx);
    EXPORT void Remove_521870(s32 idx);
    EXPORT char_type sub_521890(s32** a3, s32* arg4, u32* a4);
    EXPORT void SendToAll_521B20(Network_8* pSendData);
    EXPORT s32 NoRefs_Send_521BE0(Network_8* pSendData, s32 a3);
    EXPORT s32 NoRefs_Send_521C80(s32 pSendData);
    EXPORT s32 SendKeepAlive_521D20();
    EXPORT s32 Send_521DB0(s32 value);
    EXPORT s32 Send_521E40(s32 pSendData);
    EXPORT void static_dtor_5E4DD0();

    //s32 field_0_vtbl;
    char field_4;
    char field_5_modem_num;
    char field_6;
    char field_7;
    GUID field_8_ip_or_ipx_guid;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C_ptrs;
    Network_18 field_30_enumed_connections;
    s32 field_48;
    PacketHandlerSlot field_4C_func_ptrs_and_params[10];
    Network_504 field_C4_sessions;
    s32 field_5C8;
    s32 field_5CC;
    s32 field_5D0;
    s32 field_5D4_player_idx;
    s32 field_5D8_player_id;
    HANDLE field_5DC_handle;
    IDirectPlayLobby2* field_5E0_pDPlayLobby2;
    IDirectPlay3* field_5E4_pDPlay3;
    Network_Unknown field_5E8_n1;
    Network_Unknown field_758_n2;
    naughty_sinoussi_0x800* field_8C8[6];
    s32 field_8E0_p0x1800_2;
    s32 field_8E4_p0x1800_1;
    s32 field_8E8_time;
    s32 field_8EC;
    char field_8F0;
    char field_8F1;
    char field_8F2;
    char field_8F3;
    s32 field_8F4_time_diff;
    Network_14 field_8F8_packets[48];
    s32 field_CB8_count;
};

EXTERN_GLOBAL(NetPlay, gNetPlay_7071E8);
