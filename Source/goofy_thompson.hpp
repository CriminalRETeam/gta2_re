#pragma once

#include "Function.hpp"
// Included this way as a hack so > msvc6 can use these headers too as they've been removed in later versions. 
#include <VC98/Include/DPLAY.H>
#include <VC98/Include/DPLOBBY.H>
#include <windows.h>

struct naughty_sinoussi_0x800;
class Game_0x40;
class Network_20324;

class PacketHeader
{
  public:
    s32 field_0_type;
    s32 field_4_sub_type;
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
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
};

class Network_Unknown
{
  public:
    s32 field_0_group_id;
    s32 field_4_count;
    char_type field_8[4];
    s32 field_C;
    s32 field_10;
    s32 field_14;
    Nework_2C field_18_ary_start[6];
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

struct Network_14
{
    s32 field_0;
    s32 field_4;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    s32 field_C;
    s32 field_10;
};

struct Network_4
{
    wchar_t* field_0_allocated_str;
};

struct Network_18
{
    EnumeratedConnection* field_0_enumed_connections;
    Network_4* field_4_d_array_8_entries;
    s32 field_8_connections_count;
    s32 field_C_f4_d_array_count;
    s32 field_10;
    s32 field_14;
};

struct Network_504
{
    DPSESSIONDESC2 field_C4_sessions[16];
    s32 field_5C4_session_count;
};

struct goofy_thompson
{
    EXPORT goofy_thompson* ctor_51D6B0();
    EXPORT void* vdtor_51D7B0(char_type flags);
    EXPORT virtual ~goofy_thompson();
    EXPORT void AddEnumeratedConnection_51D930(EnumeratedConnection* pConnectionInfo);
    EXPORT s32 EnumConnections_cb_51DA30(GUID* lpguidSP,
                                         const void* lpConnection,
                                         u32 dwConnectionSize,
                                         DPNAME* lpName,
                                         s32 dwFlags,
                                         goofy_thompson* lpContext);
    EXPORT s32 SetProtoAndConnection_51DAE0(GUID* pProtocolGuid, s32 pUseThisConnection);
    EXPORT void DirectPlayDestroy_51DC90();
    EXPORT s32 DirectPlayCreate_51DCD0();
    EXPORT s32 DirectPlayCreate_51DED0();
    EXPORT static BOOL PASCAL sub_51E030(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext);
    EXPORT s32 sub_51E0E0(wchar_t* Source);
    EXPORT s32 NoRefs_51E140(wchar_t* String, s32* a3, size_t* a4);
    EXPORT s32 NoRefs_51E2B0(wchar_t* Source, wchar_t* a3, s32* a4, size_t* a5);
    EXPORT s32 NoRefs_51E450(s32 a2, u32* a3, size_t* a4);
    EXPORT s32 sub_51E5C0();
    EXPORT s32 sub_51E650();
    EXPORT s32 sub_51E7A0(wchar_t* Source, wchar_t* a3, s32 a4, s32* a5);
    EXPORT u32 sub_51E9C0(s32 a1, s32 a2, s32 a3, s32 a4, wchar_t* Source, s32 a6, s32 a7);
    EXPORT s32 EnumSessions_cb_51EAE0(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, goofy_thompson* lpContext);
    EXPORT s32 AddEnumeratedSession_51EB00(DPSESSIONDESC2* pSession);
    EXPORT void sub_51ECD0(s32 pFunc, Network_20324* pParam);
    EXPORT void sub_51ED00();
    EXPORT s32 Send_51EF60();
    EXPORT char_type Receive_51F010(s32* pOutData, s32* pOutDataLen, s32* recvId, u32* senderId);
    EXPORT void sub_51F0D0(char_type* a2, s32 a3, s32 a4, s32 a5);
    EXPORT void sub_51F110(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32 CalcPacketLen_51F210(u32 pPacket);
    EXPORT void MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen);
    EXPORT void sub_51F870(char_type* pPacket, s32 packetLen, s32 recvId, s32 a5);
    EXPORT s32 sub_520040(s32 toFind, Network_Unknown* pStru, s32 a3, u32* pOutIdx);
    EXPORT void sub_5201A0(s32 idx, Network_Unknown* pStru);
    EXPORT s32 sub_520230(s32 a2, u32* a3);
    EXPORT void sub_520530(s32 pFunc, s32 pParam);
    EXPORT s32 sub_520570(int session_idx, wchar_t* a3, s32* a4, s32* a5);
    EXPORT s32 EnumGroups_cb_520C20(s32 a1, s32 a2, s32 a3, char_type a4, u32* a5);
    EXPORT s32 sub_520CA0(s32 a2, s32 a3);
    EXPORT s32 sub_520D00(s32 a2);
    EXPORT void sub_520D10();
    EXPORT void sub_520DE0(Network_Unknown* pStru);
    EXPORT u32 IndexOf_520E30(s32 toFind, Network_Unknown* pObj);
    EXPORT void sub_520E60(s32 pFunc, s32 pParam);
    EXPORT void sub_520E80(s32 a2, s32 a3);
    EXPORT void NoRefs_null_520EA0();
    EXPORT s32 sub_520EB0(s32 a2, s32 a3, Network_Unknown* a4);
    EXPORT s32 sub_520F50(s32 a2, s32 a3);
    EXPORT s32 sub_520F80(wchar_t* String2);
    EXPORT s32 DeletePlayerFromGroup_521000(u32 idx);
    EXPORT s32 SendChatMessage_521060(wchar_t* pMsg, s32 idx_always_m1);
    EXPORT void sub_5210D0(s32 a2, s32 a3);
    EXPORT void GetPlayerName_521100(wchar_t* Destination, u32 idx);
    EXPORT s32 sub_521140(s32 a2, s32 a3);
    EXPORT s32 sub_521170(s32 a2);
    EXPORT s32 sub_5211F0(s32 a2, s32 a3);
    EXPORT void sub_521220();
    EXPORT s32 SetExitGameCallBack_521330(s32 pFunc, Game_0x40* pGame);
    EXPORT s32 GetMaxPlayers_521350();
    EXPORT s32 Send_521370();
    EXPORT bool sub_5213E0();
    EXPORT s32 NoRefs_5215B0(u32 a2, u32* a3, size_t* a4);
    EXPORT s32 Send_521630(s32 pData, s32 idx, char_type a4);
    EXPORT void Add_5216E0(s32 a2, s32** a3, s32 a4, char_type a5);
    EXPORT u32 sub_521770(u32* a2, char_type* a3, u32* a4);
    EXPORT s32 sub_521820(s32** a2, s32 idx);
    EXPORT void Remove_521870(s32 idx);
    EXPORT char_type sub_521890(s32** a3, s32* arg4, u32* a4);
    EXPORT void Send_521B20(s32* a2);
    EXPORT s32 NoRefs_Send_521BE0(s32* a2, s32 a3);
    EXPORT s32 NoRefs_Send_521C80(s32 a2);
    EXPORT s32 Send_521D20();
    EXPORT s32 Send_521DB0(s32 dataLen);
    EXPORT s32 Send_521E40(s32 a2);
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
    s32 field_4C_func_ptrs_and_params[30];
    Network_504 field_C4_sessions;
    s32 field_5C8;
    s32 field_5CC;
    s32 field_5D0;
    s32 field_5D4_player_idx;
    s32 field_5D8_player_id;
    HANDLE field_5DC_handle;
    IDirectPlayLobby* field_5E0_pDPlayLobby2;
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
    s32 field_8F8;
    s32 field_8FC;
    Network_14 field_900_208_start[47];
    s32 field_CAC;
    s32 field_CB0;
    s32 field_CB4;
    s32 field_CB8_count;
};

EXPORT_VAR extern goofy_thompson gGoofy_thompson_7071E8;
