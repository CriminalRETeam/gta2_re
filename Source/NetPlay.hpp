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
    s32 mType; // 0x0
    s32 mSubType; // 0x4
};

class Packet_Ping
{
  public:
    s32 mSenderPlayerId; // 0x0
    s32 mReceiverPlayerId; // 0x4
    s32 mTime; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(Packet_Ping, 0xC);

class Standard_Packet
{
  public:
    char_type mStdPacketSubtype; // 0x0
    s32 mUnused1; // 0x1
};

class Packet_SubType_3
{
  public:
    PacketHeader header;
    u8 mSeq; // 0x8: mod 256
    s32 mbLoadData; // 0x9
    void* mData; // 0xD
    s32 mLen; // 0x11
};
#pragma pack(pop)

class Network_Player
{
  public:
    s32 mInUse; // 0x0
    s32 mNumPingSamples; // 0x4
    s32 mAveragePing; // 0x8
    s32 mPingSum; // 0xC
    s32 mPlayerId; // 0x10
    u32 mDpname; // 0x14
    s32 mNotUsed18; // 0x18
    wchar_t* mPlayerName; // 0x1C
    s32 mNotUsed20; // 0x20: not used
    void* mpData; // 0x24
    s32 mDataLen; // 0x28
};

struct Network_Data
{
    u8 mPacketType; // 0x0
    u8 mTick; // 0x1
    u8 mMsgSize; // 0x2: mLength + 2
    u8 mType; // 0x3
    u8 mLength; // 0x4
    void* mData; // 0x5
};

class Network_Game_Session
{
  public:
    s32 mGroupId; // 0x0
    u32 mPlayersCount; // 0x4
    u8 mPacketGameTick[MAX_PLAYERS]; // 0x8
    u16 mNotUsedE; // 0xE
    Network_Player mPlayers[MAX_PLAYERS]; // 0x10
    u8* mGameSettings; // 0x118
    s32 mGroupDataLen; // 0x11C
    DPSESSIONDESC2 mSessionDesc; // 0x120
};

class EnumeratedConnection
{
  public:
    GUID mSpGuid; // 0x0
    wchar_t* mpConnectionName; // 0x10
    void* mpConnection; // 0x14
    s32 mConnectionLen; // 0x18
};

struct Network_NameList
{
    wchar_t* mNames[16]; // 0x0
    u32 mCount; // 0x40
};

struct Network_ChatMessage
{
    wchar_t mMessage[128]; // 0x0
    wchar_t mName[16]; // 0x100
};

struct Network_PlayerPing
{
    wchar_t mName[260]; // 0x0
    s32 mAvgPing; // 0x208
};

struct Network_IpAddress
{
    wchar_t* mAllocatedStr; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_IpAddress, 0x4);

struct Network_ConnectionList
{
    EnumeratedConnection* mEnumedConnections; // 0x0
    Network_IpAddress* mModemNames; // 0x4
    u32 mConnectionsCount; // 0x8
    u32 mModemCount; // 0xC
    s32 mEnumSuccess; // 0x10
    s32 mDplaySuccess; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_ConnectionList, 0x18);

struct Network_SessionList
{
    DPSESSIONDESC2 mSessions[16]; // 0xC4
    u32 mSessionCount; // 0x5C4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_SessionList, 0x504);

struct Network_Buffer
{
    void* mData; // 0x0
    s32 mLen; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_Buffer, 0x8);

struct Network_InputData
{
    u32 mInputs; // 0x0
    u32 mRng; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_InputData, 0x8);

struct Network_InputPacket
{
    Network_InputData mInputs; // 0x0
    s32 mId; // 0x8
    s32 mLen; // 0xC
    u8 mUsed; // 0x10
    char_type mGameTick; // 0x11: mod 256
    char_type mUnused12; // 0x12
    char_type mUnused13; // 0x13
};
GTA2_ASSERT_SIZEOF_ALWAYS(Network_InputPacket, 0x14);

struct PacketHandlerSlot
{
    void* mParamFnCallback; // 0x0: fn ptr
    void* mParamContext; // 0x4: this ptr
    s32 mFnType; // 0x8
};

struct NetPlay
{
    STATIC_MEMBER(GUID, kGta2_DP_Guid);
    STATIC_MEMBER_ARRAY(s32, sLastPlayerInputs, 6);
    STATIC_MEMBER_ARRAY(char_type, sLastPlayerInputSeq, 24);
    EXPORT NetPlay();
    EXPORT void* vdtor(char_type flags);
    EXPORT virtual ~NetPlay();
    EXPORT void AddEnumeratedConnection(EnumeratedConnection* pConnectionInfo);
    EXPORT static s32 __stdcall EnumConnections_cb(const GUID* lpguidSP,
                                         void* lpConnection,
                                         unsigned long dwConnectionSize,
                                         const DPNAME* lpName,
                                         unsigned long dwFlags,
                                         void* lpContext);
    EXPORT s32 SetProtoAndConnection(GUID* pProtocolGuid, Network_Buffer* pUseThisConnection);
    EXPORT void DirectPlayDestroy();
    EXPORT s32 DirectPlayCreate_51DCD0();
    EXPORT s32 DirectPlayCreate_51DED0();
    EXPORT static BOOL PASCAL EnumAddress_cb(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext);
    EXPORT s32 PushConnection(wchar_t* Source);
    EXPORT s32 CreateTcpIpAddress(wchar_t* pIpAddress, s32* ppAddress, size_t* pAddressLen);
    EXPORT s32 CreateModemAddress(wchar_t* pPhoneNumber, wchar_t* pModemName, s32* ppAddress, size_t* pAddressLen);
    EXPORT s32 CreateSerialAddress(DPCOMPORTADDRESS* pComPort, u32* ppAddress, size_t* pAddressLen);
    EXPORT s32 InitializeConnection();
    EXPORT s32 EnumSessions();
    EXPORT s32 HostSession(wchar_t* pSessionName, wchar_t* pPlayerName, Network_Buffer* pGroupData, Network_Buffer* pPlayerData);
    EXPORT u32 AddPlayer(Network_Buffer* pData, s32 player_id, DPNAME name, Network_Game_Session* pStru);
    EXPORT static s32 __stdcall EnumSessions_cb(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, NetPlay* lpContext);
    EXPORT s32 AddEnumeratedSession(DPSESSIONDESC2* pSession);
    EXPORT void Set15(s32 pFunc, NetUi* pParam);
    EXPORT void NetworkTick();
    EXPORT s32 SendPing();
    EXPORT bool Receive(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId);
    EXPORT void DispatchReceivedMessage(void* pPacket, s32 packetLen, s32 recvId, s32 bSendOrRecv);
    EXPORT void OnPlayerMessage(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag);
    EXPORT s32 DecodeReceivedPacket(s32 pPacket, u32 packetLen); // Also converts enums back
    EXPORT void MakeSendData(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen); // Format data before using IDirectPlay3::Send
    EXPORT void OnPacketReceived(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag);
    EXPORT s32 MovePlayerToGroup(s32 toFind, Network_Game_Session* pStru, Network_Game_Session* pDst, u32* pOutIdx);
    EXPORT void FreePlayerSlot(s32 idx, Network_Game_Session* pStru);
    EXPORT void InvokeCallback(s32 idx, u32 event_data);
    EXPORT void Set6(void* pFunc, void* pParam);
    EXPORT s32 JoinSession(u32 session_idx, wchar_t* pPlayerName, Network_Buffer* pPlayerData, Network_Buffer* pOutGroupData);
    EXPORT static s32 __stdcall EnumGroups_cb(s32 group_id, s32 callback_type, s32 pName, char_type flags, NetPlay* pContext);
    EXPORT s32 AddEnumeratedGroupPlayer(s32 player_id, DPNAME* pName);
    EXPORT void SetGroupId(s32 group_id);
    EXPORT void Disconnect();
    EXPORT void ClearPlayersAndSession(Network_Game_Session* pStru);
    EXPORT u32 IndexOf(s32 toFind, Network_Game_Session* pObj);
    EXPORT void Set9(s32 pFunc, s32 pParam);
    EXPORT void Set3_Disconnect(s32 pCallback, s32 pContext);
    EXPORT void NoRefs_null();
    EXPORT void UpdatePlayerPing(s32 player_id, s32 ping, Network_Game_Session* pStru);
    EXPORT void Set18(s32 pCallback, s32 pContext);
    EXPORT s32 RemovePlayerByName(wchar_t* String2);
    EXPORT s32 DeletePlayerFromGroup(u32 idx);
    EXPORT s32 SendChatMessage(wchar_t* pMsg, s32 idx_always_m1);
    EXPORT void Set21(s32 pCallback, s32 pContext);
    EXPORT void GetPlayerName(wchar_t* Destination, u32 idx);
    EXPORT void Set24(s32 pCallback, s32 pContext);
    EXPORT s32 SetGameSettings(Network_Buffer* pObj);
    EXPORT void Set27SavePlayerName(s32 pCallback, s32 pContext);
    EXPORT void DisableJoining();
    EXPORT s32 IsUserHost();
    EXPORT void SetExitGameCallBack(s32 pFunc, Game* pGame);
    EXPORT s32 CountActivePlayers();
    EXPORT void SendAcknowledgment();
    EXPORT bool WaitForPlayersSync();
    EXPORT s32 CopyConnection(u32 idx, u32* ppConnection, size_t* pLen);
    EXPORT s32 SendToPlayer(Network_Buffer* pSendData, s32 player_idx, char_type delayed_ticks);
    EXPORT void Add(Network_Buffer* pData, s32 id, char_type game_tick);
    EXPORT u32 GetOldestBufferedPacket(Network_Buffer* pOut, char_type* pSeq, u32* pPlayerId);
    EXPORT void ResolvePlayerInput(s32** ppData, s32 idx);
    EXPORT void Remove(s32 idx);
    EXPORT bool ReceiveGameMessage(Network_Buffer* pOut, s32* pPlayerIdx, u32* packet_subtype);
    EXPORT void SendToAll(Network_Buffer* pSendData);
    EXPORT s32 NoRefs_Send_521BE0(Network_Buffer* pSendData, s32 tick_delay);
    EXPORT s32 NoRefs_Send_521C80(s32 pSendData);
    EXPORT s32 SendPlayerIsQuitting();
    EXPORT s32 SendGiveUp(s32 value);
    EXPORT s32 SendSyncCheckData(s32 pSendData);
    EXPORT static void static_dtor();

    // 9.6f 0x461DA0
    inline u32 GetPlayerCount()
    {
        return mN2.mPlayersCount;
    }

    // 9.6f 0x409C40
    inline s32 GetPlayerIdx()
    {
        return mPlayerIdx;
    }

    char_type mbModem; // 0x4
    char_type mModemNum; // 0x5
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    GUID mIpOrIpxGuid; // 0x8
    s32 mUnused18; // 0x18
    s32 mUnused1C; // 0x1C
    s32 mUnused20; // 0x20
    s32 mUnused24; // 0x24
    s32 mUnused28; // 0x28
    s32 mPtrs; // 0x2C
    Network_ConnectionList mEnumedConnections; // 0x30
    s32 mNetState; // 0x48
    PacketHandlerSlot mFuncPtrsAndParams[10]; // 0x4C
    Network_SessionList mSessions; // 0xC4
    s32 mNotUsed5C8; // 0x5C8
    s32 mbIsHost; // 0x5CC
    s32 mPlayerOgIdx; // 0x5D0: Only set for the host, but it's not used
    s32 mPlayerIdx; // 0x5D4: 1, 2, ..., MAX_PLAYERS
    s32 mPlayerId; // 0x5D8: From DPMSG_ADDPLAYERTOGROUP::dpIdPlayer
    HANDLE mHandle; // 0x5DC
    IDirectPlayLobby2* mpDPlayLobby2; // 0x5E0
    IDirectPlay3* mpDPlay3; // 0x5E4
    Network_Game_Session mN1; // 0x5E8
    Network_Game_Session mN2; // 0x758
    NetPlayerBuffer_0x800* mNotUsed8C8[MAX_PLAYERS]; // 0x8C8
    s32 mpSendDataBuffer; // 0x8E0: 0x1800 of alloc size
    s32 mpReceivedDataBuffer; // 0x8E4: 0x1800 of alloc size
    s32 mTime; // 0x8E8
    s32 mUnused8EC; // 0x8EC
    char_type mReceiveDisabled; // 0x8F0: Receive and ReceiveGameMessage always returns false if this var is true
    char_type mUnused8F1; // 0x8F1
    char_type mUnused8F2; // 0x8F2
    char_type mUnused8F3; // 0x8F3
    s32 mTimeDiff; // 0x8F4
    Network_InputPacket mPackets[48]; // 0x8F8
    s32 mCount; // 0xCB8
};

EXTERN_GLOBAL(NetPlay, gNetPlay);
