#include "NetPlay.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "network_data.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(NetPlay, gNetPlay, 0x7071E8);
DEFINE_STATIC_MEMBER_INIT(GUID, NetPlay, kGta2_DP_Guid, {0x561E8920 COMMA 0x5C70 COMMA 0x11D2 COMMA {0xAD COMMA 0x00 COMMA 0x00 COMMA 0xA0 COMMA 0xC9 COMMA 0xB6 COMMA 0x13 COMMA 0x98}}, 0x5FE928);
DEFINE_STATIC_MEMBER_ARRAY(s32, NetPlay, sLastPlayerInputs, 6, 0x6F8A4C);
DEFINE_STATIC_MEMBER_ARRAY(char_type, NetPlay, sLastPlayerInputSeq, 24, 0x6F8A64);

// 9.6f 0x409C50
// Signed difference between two 8-bit sequence numbers, allowing for wrap around
static inline s32 SeqDiff(u8 a, u8 b)
{
    s32 diff = static_cast<u8>(a - b);
    if (diff == 0)
    {
        return 0;
    }
    if (diff >= 0x80)
    {
        // Written as a return: "diff -= 0x100" gives sub instead of the original's add $-0x100
        return diff - 0x100;
    }
    return diff;
}

// Advances a player's 8-bit sequence number
static inline void NextSeq(Network_Game_Session* pN2, s32 idx)
{
    pN2->mPacketGameTick[idx] = (pN2->mPacketGameTick[idx] + 1) % 256;
}

MATCH_FUNC(0x51d6b0)
NetPlay::NetPlay()
{
    mCount = 0;
    for (s32 i = 0; i < 48; i++)
    {
        mPackets[i].mUsed = 0;
    }

    mbModem = 0;
    mModemNum = 0;
    mReceiveDisabled = 0;
    mTimeDiff = 0;
    mpDPlay3 = NULL;
    mpDPlayLobby2 = NULL;
    mpReceivedDataBuffer = reinterpret_cast<s32>(operator new(0x1800));
    mpSendDataBuffer = reinterpret_cast<s32>(operator new(0x1800));
    memset(&mSessions, 0, sizeof(mSessions));
    memset(mFuncPtrsAndParams, 0, sizeof(mFuncPtrsAndParams));
    memset(&mEnumedConnections, 0, sizeof(mEnumedConnections));
    memset(&mN2, 0, sizeof(mN2));
    memset(sLastPlayerInputs, 0xFF, sizeof(sLastPlayerInputs));
    memset(sLastPlayerInputSeq, 0, sizeof(sLastPlayerInputSeq));
    mHandle = CreateEventA(NULL, FALSE, FALSE, NULL);
    mUnused8EC = 0;
    mNetState = 5;

    for (s32 j = 0; j < MAX_PLAYERS; j++)
    {
        mNotUsed8C8[j] = static_cast<NetPlayerBuffer_0x800*>(operator new(0x800));
    }
}

// The scalar deleting destructor VC6 generates for the virtual ~NetPlay, written out
MATCH_FUNC(0x51d7b0)
void* NetPlay::vdtor(char_type flags)
{
    this->NetPlay::~NetPlay();
    if (flags & 1)
    {
        operator delete(this);
    }
    return this;
}

MATCH_FUNC(0x51d7d0)
NetPlay::~NetPlay()
{
    if (mpDPlay3)
    {
        mpDPlay3->DestroyPlayer(mPlayerId);
        if (mbIsHost)
        {
            mpDPlay3->DestroyGroup(mN2.mGroupId);
        }
        mpDPlay3->Close();
    }

    if (mEnumedConnections.mEnumedConnections)
    {
        for (u32 i = 0; i < mEnumedConnections.mConnectionsCount; i++)
        {
            operator delete(mEnumedConnections.mEnumedConnections[i].mpConnectionName);
            operator delete(mEnumedConnections.mEnumedConnections[i].mpConnection);
        }
        operator delete(mEnumedConnections.mEnumedConnections);
        mEnumedConnections.mEnumedConnections = NULL;
    }

    if (mEnumedConnections.mModemNames)
    {
        for (u32 i = 0; i < mEnumedConnections.mModemCount; i++)
        {
            operator delete(mEnumedConnections.mModemNames[i].mAllocatedStr);
        }
        operator delete(mEnumedConnections.mModemNames);
        mEnumedConnections.mModemNames = NULL;
    }

    for (s32 j = 0; j < MAX_PLAYERS; j++)
    {
        operator delete(mNotUsed8C8[j]);
        operator delete(mN2.mPlayers[j].mPlayerName);
        operator delete(mN2.mPlayers[j].mpData);
    }

    operator delete(mN2.mGameSettings);
    operator delete(reinterpret_cast<void*>(mpReceivedDataBuffer));
    operator delete(reinterpret_cast<void*>(mpSendDataBuffer));
    operator delete(mN2.mSessionDesc.lpszSessionName);
    mEnumedConnections.mConnectionsCount = 0;
    mEnumedConnections.mModemCount = 0;
    mEnumedConnections.mEnumSuccess = 0;
    mEnumedConnections.mDplaySuccess = 0;
    CloseHandle(mHandle);
    DirectPlayDestroy();
}

MATCH_FUNC(0x51d930)
void NetPlay::AddEnumeratedConnection(EnumeratedConnection* pConnectionInfo)
{
    const u32 connection_idx = this->mEnumedConnections.mConnectionsCount;
    if (connection_idx < 8)
    {
        EnumeratedConnection* pConnections = this->mEnumedConnections.mEnumedConnections;
        if (pConnections)
        {
            EnumeratedConnection* pConnection = &pConnections[connection_idx];
            pConnection->mSpGuid = pConnectionInfo->mSpGuid;
            this->mEnumedConnections.mEnumedConnections[this->mEnumedConnections.mConnectionsCount]
                .mpConnectionName = new wchar_t[wcslen(pConnectionInfo->mpConnectionName) + 1];
            wcscpy(this->mEnumedConnections.mEnumedConnections[this->mEnumedConnections.mConnectionsCount]
                       .mpConnectionName,
                   pConnectionInfo->mpConnectionName);

            this->mEnumedConnections.mEnumedConnections[this->mEnumedConnections.mConnectionsCount]
                .mpConnection = new u8[pConnectionInfo->mConnectionLen];
            memcpy(this->mEnumedConnections.mEnumedConnections[this->mEnumedConnections.mConnectionsCount]
                       .mpConnection,
                   pConnectionInfo->mpConnection,
                   pConnectionInfo->mConnectionLen);

            this->mEnumedConnections.mEnumedConnections[this->mEnumedConnections.mConnectionsCount]
                .mConnectionLen = pConnectionInfo->mConnectionLen;

            mEnumedConnections.mConnectionsCount++;
        }
    }
}

MATCH_FUNC(0x51da30)
s32 __stdcall NetPlay::EnumConnections_cb(const GUID* lpguidSP,
                                                 void* lpConnection,
                                                 unsigned long dwConnectionSize,
                                                 const DPNAME* lpName,
                                                 unsigned long dwFlags,
                                                 void* lpContext)
{
    EnumeratedConnection info;
    info.mSpGuid = *lpguidSP;
    info.mpConnection = new u8[dwConnectionSize];
    memcpy(info.mpConnection, lpConnection, dwConnectionSize);
    info.mConnectionLen = dwConnectionSize;

    const size_t connectionNameLen = wcslen(lpName->lpszShortName);
    info.mpConnectionName = new wchar_t[connectionNameLen + 1];
    wcscpy(info.mpConnectionName, lpName->lpszShortName);
    static_cast<NetPlay*>(lpContext)->AddEnumeratedConnection(&info);
    delete[] info.mpConnectionName;
    delete[] info.mpConnection;
    return 1;
}

MATCH_FUNC(0x51dae0)
s32 NetPlay::SetProtoAndConnection(GUID* pProtocolGuid, Network_Buffer* pUseThisConnection)
{
    s32 isIpx;

    if (pProtocolGuid)
    {
        if (memcmp(pProtocolGuid, &DPSPGUID_MODEM, sizeof(GUID)) == 0)
        {
            mbModem = 1;
        }
        isIpx = 0;
        mIpOrIpxGuid = *pProtocolGuid;
    }
    else
    {
        isIpx = 1;
        mIpOrIpxGuid = DPSPGUID_IPX;
    }

    if (mEnumedConnections.mEnumedConnections)
    {
        delete[] mEnumedConnections.mEnumedConnections;
        mEnumedConnections.mConnectionsCount = 0;
        mEnumedConnections.mEnumSuccess = 0;
    }

    mEnumedConnections.mEnumedConnections = new EnumeratedConnection[8];
    if (NetPlay::CreateDirectPlayAndLobby())
    {
        if (isIpx)
        {
            if (SUCCEEDED(mpDPlay3->EnumConnections(&kGta2_DP_Guid, NetPlay::EnumConnections_cb, this, 1)))
            {
                mEnumedConnections.mEnumSuccess = 1;
                if (NetPlay::EnumModems())
                {
                    mEnumedConnections.mDplaySuccess = 1;
                }
                return 1;
            }
            else
            {
                mEnumedConnections.mEnumSuccess = 0;
                NetPlay::DirectPlayDestroy();
                return 0;
            }
        }
        else
        {
            EnumeratedConnection info;
            info.mSpGuid = *pProtocolGuid;

            info.mpConnection = new u8[pUseThisConnection->mLen];
            memcpy(info.mpConnection, pUseThisConnection->mData, pUseThisConnection->mLen);
            info.mConnectionLen = pUseThisConnection->mLen;
            info.mpConnectionName = new wchar_t[wcslen(L"dummy prot") + 1];
            wcscpy(info.mpConnectionName, L"dummy prot");
            NetPlay::AddEnumeratedConnection(&info);

            delete[] info.mpConnectionName;
            GTA2_DELETE_AND_NULL(info.mpConnection);

            mEnumedConnections.mEnumSuccess = 1;
            return 1;
        }
    }
    else
    {
        NetPlay::DirectPlayDestroy();
        return 0;
    }
}

MATCH_FUNC(0x51dc90)
void NetPlay::DirectPlayDestroy()
{
    if (mpDPlay3)
    {
        mpDPlay3->Release();
        mpDPlay3 = NULL;
    }

    if (mpDPlayLobby2)
    {
        mpDPlayLobby2->Release();
        mpDPlayLobby2 = NULL;
    }
}

MATCH_FUNC(0x51dcd0)
s32 NetPlay::CreateDirectPlayAndLobby()
{
    IDirectPlayLobby* pIDirectPlayLobby;

    DirectPlayDestroy();

    if (CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER, IID_IDirectPlay3, reinterpret_cast<LPVOID*>(&mpDPlay3)) < 0 ||
        DirectPlayLobbyCreateW(NULL, &pIDirectPlayLobby, NULL, NULL, 0) < 0)
    {
        return 0;
    }
    pIDirectPlayLobby->QueryInterface(IID_IDirectPlayLobby2, reinterpret_cast<void**>(&this->mpDPlayLobby2));
    pIDirectPlayLobby->Release();
    return 1;
}

MATCH_FUNC(0x51ded0)
s32 NetPlay::EnumModems()
{
    GUID guid_DPSPGUID_MODEM = DPSPGUID_MODEM;
    this->mEnumedConnections.mModemNames = new Network_IpAddress[8];
    this->mEnumedConnections.mModemCount = 0;

    IDirectPlay* pDirectPlay;
    if (DirectPlayCreate(&guid_DPSPGUID_MODEM, &pDirectPlay, NULL) < 0)
    {
        return 0;
    }

    IDirectPlay3* pDirectPlay3;
    if (FAILED(pDirectPlay->QueryInterface(IID_IDirectPlay3, reinterpret_cast<void**>(&pDirectPlay3))))
    {
        return 0;
    }

    pDirectPlay->Release();

    if (!pDirectPlay3 || !mpDPlayLobby2)
    {
        return 1;
    }
    {
        unsigned long playerAddressLen = 0;
        HRESULT result = pDirectPlay3->GetPlayerAddress(0, NULL, &playerAddressLen);
        if (result == DPERR_UNAVAILABLE)
        {
            pDirectPlay3->Release();
            return 0;
        }

        if (result != DPERR_BUFFERTOOSMALL)
        {
            pDirectPlay3->Release();
            return 0;
        }

        if (playerAddressLen)
        {
            u8* pPlayerAddressBuf = new u8[playerAddressLen];
            if (pDirectPlay3->GetPlayerAddress(0, pPlayerAddressBuf, &playerAddressLen) >= 0 &&
                FAILED(mpDPlayLobby2->EnumAddress(NetPlay::EnumAddress_cb, pPlayerAddressBuf, playerAddressLen, this)))
            {
                return 0;
            }
            delete[] pPlayerAddressBuf;
        }
        pDirectPlay3->Release();
    }

    return 1;
}

// The guard's lstrlenA is the rotated entry test of the while loop. VC6 keeps bDone in lpData's
// stack slot, and the flag test stays at the loop head because it is inside the loop
MATCH_FUNC(0x51e030)
BOOL NetPlay::EnumAddress_cb(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext)
{
    LPCSTR pAddress = static_cast<LPCSTR>(lpData);
    BOOL bDone = FALSE;
    if (guidDataType == DPAID_Modem && dwDataSize)
    {
        NetPlay* pThis = static_cast<NetPlay*>(lpContext);
        while (lstrlenA(pAddress))
        {
            if (bDone)
            {
                break;
            }
            wchar_t* pWide = new wchar_t[lstrlenA(pAddress) + 1];
            MultiByteToWideChar(0, 0, pAddress, -1, pWide, 2 * lstrlenA(pAddress) + 2);
            pThis->PushConnection(pWide);
            delete[] pWide;
            pAddress += lstrlenA(pAddress) + 1;
            if (!lstrlenA(pAddress))
            {
                bDone = TRUE;
            }
        }
    }
    return TRUE;
}

MATCH_FUNC(0x51e0e0)
s32 NetPlay::PushConnection(wchar_t* Source)
{
    if (mEnumedConnections.mModemCount < 8)
    {
        size_t string_len_bytes = wcslen(Source);
        mEnumedConnections.mModemNames[mEnumedConnections.mModemCount].mAllocatedStr =
            new wchar_t[string_len_bytes + 1];
        wcscpy(mEnumedConnections.mModemNames[mEnumedConnections.mModemCount]
                   .mAllocatedStr,
               Source);
        ++mEnumedConnections.mModemCount;
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x51e140)
s32 NetPlay::CreateTcpIpAddress(wchar_t* pIpAddress, s32* ppAddress, size_t* pAddressLen)
{
    DWORD addressLen = 0;
    if (!mpDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    DPCOMPOUNDADDRESSELEMENT elements[2];
    memset(elements, 0, sizeof(elements));
    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = const_cast<GUID*>(&DPSPGUID_TCPIP);
    elements[1].guidDataType = DPAID_INetW;
    elements[1].dwDataSize = 2 * wcslen(pIpAddress) + 2;
    elements[1].lpData = pIpAddress;

    if (mpDPlayLobby2->CreateCompoundAddress(elements, 2, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    void* pAddress = operator new(addressLen);
    if (mpDPlayLobby2->CreateCompoundAddress(elements, 2, pAddress, &addressLen) < 0)
    {
        operator delete(pAddress);
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    *ppAddress = reinterpret_cast<s32>(pAddress);
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e2b0)
s32 NetPlay::CreateModemAddress(wchar_t* pPhoneNumber, wchar_t* pModemName, s32* ppAddress, size_t* pAddressLen)
{
    char_type phoneNumber[128];
    char_type modemName[128];
    DWORD addressLen;
    DPCOMPOUNDADDRESSELEMENT elements[3];
    void* pAddress;

    wcstombs(phoneNumber, pPhoneNumber, sizeof(phoneNumber));
    wcstombs(modemName, pModemName, sizeof(modemName));
    memset(elements, 0, sizeof(elements));
    addressLen = 0;
    pAddress = NULL;

    if (!mpDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = const_cast<GUID*>(&DPSPGUID_MODEM);

    if (mpDPlayLobby2->CreateCompoundAddress(elements, 1, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        // Shared cleanup block, likely a goto in the original too
        goto failed;
    }

    pAddress = operator new(addressLen);
    if (mpDPlayLobby2->CreateCompoundAddress(elements, 1, pAddress, &addressLen) != DP_OK)
    {
    failed:
        *ppAddress = 0;
        *pAddressLen = 0;
        if (elements[1].lpData)
        {
            operator delete(elements[1].lpData);
        }
        if (elements[2].lpData)
        {
            operator delete(elements[2].lpData);
        }
        if (pAddress)
        {
            operator delete(pAddress);
        }
        return 0;
    }

    if (elements[1].lpData)
    {
        operator delete(elements[1].lpData);
    }
    if (elements[2].lpData)
    {
        operator delete(elements[2].lpData);
    }
    *ppAddress = reinterpret_cast<s32>(pAddress);
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e450)
s32 NetPlay::CreateSerialAddress(DPCOMPORTADDRESS* pComPort, u32* ppAddress, size_t* pAddressLen)
{
    DWORD addressLen = 0;
    DPCOMPOUNDADDRESSELEMENT elements[2];
    memset(elements, 0, sizeof(elements));
    if (!mpDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = const_cast<GUID*>(&DPSPGUID_SERIAL);
    elements[1].guidDataType = DPAID_ComPort;
    elements[1].dwDataSize = sizeof(DPCOMPORTADDRESS);
    elements[1].lpData = pComPort;

    if (mpDPlayLobby2->CreateCompoundAddress(elements, 2, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    void* pAddress = operator new(addressLen);
    if (mpDPlayLobby2->CreateCompoundAddress(elements, 2, pAddress, &addressLen) != DP_OK)
    {
        operator delete(pAddress);
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    *ppAddress = reinterpret_cast<u32>(pAddress);
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e5c0)
s32 NetPlay::InitializeConnection()
{
    if (mpDPlay3 && mpDPlayLobby2)
    {
        u32 i;
        for (i = 0; i < 8; i++)
        {
            if (mEnumedConnections.mEnumedConnections[i].mSpGuid == mIpOrIpxGuid)
            {
                break;
            }
        }

        if (mpDPlay3->InitializeConnection(mEnumedConnections.mEnumedConnections[i].mpConnection, 0) >= 0)
        {
            if (mbModem || EnumSessions() != -1)
            {
                mTime = timeGetTime();
                mNetState = 1;
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x51e650)
s32 NetPlay::EnumSessions()
{
    MSG msg;
    DPSESSIONDESC2 desc;

    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.guidApplication = kGta2_DP_Guid;

    HRESULT hr;
    if (mbModem)
    {
        hr = mpDPlay3->EnumSessions(&desc, 0, reinterpret_cast<LPDPENUMSESSIONSCALLBACK2>(NetPlay::EnumSessions_cb), this, 0);
        while (hr == DPERR_CONNECTING)
        {
            if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            Sleep(500);
            hr = mpDPlay3->EnumSessions(&desc, 0, reinterpret_cast<LPDPENUMSESSIONSCALLBACK2>(NetPlay::EnumSessions_cb), this, 0);
        }

        if (hr == DPERR_USERCANCEL)
        {
            return 0;
        }
        if (hr != DP_OK)
        {
            goto failed;
        }
        hr = mpDPlay3->EnumSessions(&desc, 0, reinterpret_cast<LPDPENUMSESSIONSCALLBACK2>(NetPlay::EnumSessions_cb), this, DPENUMSESSIONS_RETURNSTATUS);
        if (hr == DPERR_USERCANCEL)
        {
            return 0;
        }
        if (hr != DP_OK)
        {
            goto failed;
        }
        return mSessions.mSessionCount;
    }
    else
    {
        hr = mpDPlay3->EnumSessions(&desc,
                                             0,
                                             reinterpret_cast<LPDPENUMSESSIONSCALLBACK2>(NetPlay::EnumSessions_cb),
                                             this,
                                             DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC);
        if (hr < 0)
        {
        // Shared failure block, as in CreateModemAddress:
        // the modem path's failures jump forward into the non-modem path's `return -1`, and the two
        // `return session_count` blocks are merged. No goto-free form gave this layout.
        failed:
            return -1;
        }
        return mSessions.mSessionCount;
    }
}

MATCH_FUNC(0x51e7a0)
s32 NetPlay::HostSession(wchar_t* pSessionName, wchar_t* pPlayerName, Network_Buffer* pGroupData, Network_Buffer* pPlayerData)
{
    DPNAME name;

    if (mpDPlay3 && mpDPlayLobby2)
    {
        DPSESSIONDESC2* pDesc = &mN2.mSessionDesc;
        memset(pDesc, 0, sizeof(DPSESSIONDESC2));
        pDesc->dwSize = sizeof(DPSESSIONDESC2);
        mN2.mSessionDesc.dwFlags = 0x8000; // DPSESSION_DIRECTPLAYPROTOCOL, not in this DPLAY.H
        mN2.mSessionDesc.guidApplication = kGta2_DP_Guid;
        mN2.mSessionDesc.dwMaxPlayers = 6;
        mN2.mSessionDesc.lpszSessionName = static_cast<LPWSTR>(operator new(2 * wcslen(pSessionName) + 2));
        wcscpy(mN2.mSessionDesc.lpszSessionName, pSessionName);

        if (mpDPlay3->Open(pDesc, DPOPEN_CREATE) >= 0)
        {
            memset(&name, 0, sizeof(name));
            name.lpszShortName = pPlayerName;
            name.lpszLongName = NULL;
            name.dwSize = sizeof(DPNAME);
            mpDPlay3->CreatePlayer(reinterpret_cast<LPDPID>(&mPlayerId),
                                            &name,
                                            mHandle,
                                            pPlayerData->mData,
                                            pPlayerData->mLen,
                                            0);

            mPlayerOgIdx = mPlayerIdx = AddPlayer(pPlayerData, mPlayerId, name, &mN1);

            mN2.mGroupDataLen = pGroupData->mLen;
            mN2.mGameSettings = static_cast<u8*>(operator new(pGroupData->mLen));
            if (mN2.mGameSettings)
            {
                memcpy(mN2.mGameSettings, pGroupData->mData, pGroupData->mLen);

                memset(&name, 0, sizeof(name));
                name.dwSize = sizeof(DPNAME);
                name.lpszShortName = L"GBH Group";
                name.lpszLongName = NULL;
                if (mpDPlay3->CreateGroup(reinterpret_cast<LPDPID>(&mN2.mGroupId),
                                                   &name,
                                                   mN2.mGameSettings,
                                                   mN2.mGroupDataLen,
                                                   0) >= 0 &&
                    mpDPlay3->AddPlayerToGroup(mN2.mGroupId, mPlayerId) >= 0)
                {
                    mNetState = 3;
                    mbIsHost = 1;
                    mNotUsed5C8 = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x51e9c0)
u32 NetPlay::AddPlayer(Network_Buffer* pData, s32 player_id, DPNAME name, Network_Game_Session* pSession)
{
    u32 i = 0;
    if (pSession->mPlayersCount != MAX_PLAYERS)
    {
        while (1)
        {
            if (i >= MAX_PLAYERS)
            {
                break;
            }

            if (!pSession->mPlayers[i].mInUse)
            {
                memset(&pSession->mPlayers[i], 0, sizeof(Network_Player));
                pSession->mPlayers[i].mPlayerId = player_id;
                memset(&pSession->mPlayers[i].mDpname, 0, sizeof(DPNAME));

                if (wcslen(name.lpszShortName) < 15)
                {
                    pSession->mPlayers[i].mPlayerName = static_cast<wchar_t*>(operator new(2 * wcslen(name.lpszShortName) + 2));
                    wcscpy(pSession->mPlayers[i].mPlayerName, name.lpszShortName);
                }
                else
                {
                    pSession->mPlayers[i].mPlayerName = static_cast<wchar_t*>(operator new(0x20));
                    wcsncpy(pSession->mPlayers[i].mPlayerName, name.lpszShortName, 15);
                    pSession->mPlayers[i].mPlayerName[15] = 0;
                }

                pSession->mPlayers[i].mpData = operator new(pData->mLen);
                pSession->mPlayers[i].mDataLen = pData->mLen;
                memcpy(pSession->mPlayers[i].mpData, pData->mData, pData->mLen);
                pSession->mPlayers[i].mInUse = 1;
                pSession->mPlayersCount++;
                return i;
            }
            i++;
        }
    }
    return 0xEEEEEEEE;
}

MATCH_FUNC(0x51eae0)
s32 __stdcall NetPlay::EnumSessions_cb(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, NetPlay* lpContext)
{
    if ((dwFlags & 1) != 0)
    {
        return 0;
    }
    else
    {
        return static_cast<NetPlay*>(lpContext)->AddEnumeratedSession(lpThisSD);
    }
}

MATCH_FUNC(0x51eb00)
s32 NetPlay::AddEnumeratedSession(DPSESSIONDESC2* pSession)
{

    if (mSessions.mSessionCount < 16)
    {
        memset(&mSessions.mSessions[mSessions.mSessionCount], 0, sizeof(DPSESSIONDESC2));
        mSessions.mSessions[mSessions.mSessionCount].dwSize = sizeof(DPSESSIONDESC2);
        mSessions.mSessions[mSessions.mSessionCount].dwFlags = pSession->dwFlags;
        mSessions.mSessions[mSessions.mSessionCount].guidInstance = pSession->guidInstance;
        mSessions.mSessions[mSessions.mSessionCount].guidApplication = pSession->guidApplication;
        mSessions.mSessions[mSessions.mSessionCount].dwMaxPlayers = pSession->dwMaxPlayers;
        mSessions.mSessions[mSessions.mSessionCount].dwCurrentPlayers = pSession->dwCurrentPlayers;

        mSessions.mSessions[mSessions.mSessionCount].lpszSessionName =
            new wchar_t[wcslen(pSession->lpszSessionName) + 1];
        wcscpy(mSessions.mSessions[mSessions.mSessionCount].lpszSessionName, pSession->lpszSessionName);

        mSessions.mSessions[mSessions.mSessionCount].dwReserved1 = pSession->dwReserved1;
        mSessions.mSessions[mSessions.mSessionCount].dwReserved2 = pSession->dwReserved2;
        mSessions.mSessions[mSessions.mSessionCount].dwUser1 = pSession->dwUser1;
        mSessions.mSessions[mSessions.mSessionCount].dwUser2 = pSession->dwUser2;
        mSessions.mSessions[mSessions.mSessionCount].dwUser3 = pSession->dwUser3;
        mSessions.mSessions[mSessions.mSessionCount].dwUser4 = pSession->dwUser4;

        mSessions.mSessionCount++;

        return 1;
    }
    return 0;
}

MATCH_FUNC(0x51ecd0)
void NetPlay::Set15(s32 pFunc, NetUi* pParam)
{
    this->mFuncPtrsAndParams[5].mParamFnCallback = reinterpret_cast<void*>(pFunc);
    this->mFuncPtrsAndParams[5].mParamContext = pParam;
    this->mFuncPtrsAndParams[5].mFnType = 5;
}

MATCH_FUNC(0x51ed00)
void NetPlay::NetworkTick()
{
    s32 pData;
    s32 dataLen;
    unsigned long recvId;
    unsigned long senderId;

    switch (mNetState)
    {
        case 4:
        {
            if (timeGetTime() - mTime >= 1000)
            {
                SendPing();
                mTime = timeGetTime();
            }

            if (Receive(&pData, &dataLen, &recvId, &senderId))
            {
                DispatchReceivedMessage(reinterpret_cast<void*>(pData), dataLen, recvId, senderId);
            }
            break;
        }

        case 3:
        {
            if (timeGetTime() - mTime >= 1000)
            {
                SendPing();
                mTime = timeGetTime();
            }

            if (Receive(&pData, &dataLen, &recvId, &senderId))
            {
                DispatchReceivedMessage(reinterpret_cast<void*>(pData), dataLen, recvId, senderId);
            }
            break;
        }

        case 1:
            // Browsing for sessions: re-enumerate them once a second
            if (!mbModem && timeGetTime() - mTime >= 1000)
            {
                u32 i;
                for (i = 0; i < mSessions.mSessionCount; i++)
                {
                    operator delete(mSessions.mSessions[i].lpszSessionName);
                }
                memset(&mSessions, 0, sizeof(mSessions));

                DPSESSIONDESC2 desc;
                memset(&desc, 0, sizeof(desc));
                desc.dwSize = sizeof(desc);
                desc.guidApplication = kGta2_DP_Guid;
                if (mpDPlay3->EnumSessions(&desc,
                                                    1000,
                                                    reinterpret_cast<LPDPENUMSESSIONSCALLBACK2>(NetPlay::EnumSessions_cb),
                                                    this,
                                                    DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC) == DP_OK)
                {
                    Network_NameList names;
                    names.mCount = mSessions.mSessionCount;
                    for (i = 0; i < mSessions.mSessionCount; i++)
                    {
                        names.mNames[i] = static_cast<wchar_t*>(operator new(2 * wcslen(mSessions.mSessions[i].lpszSessionName) + 2));
                        wcscpy(names.mNames[i], mSessions.mSessions[i].lpszSessionName);
                    }

                    InvokeCallback(5, reinterpret_cast<u32>(&names));

                    for (i = 0; i < names.mCount; i++)
                    {
                        operator delete(names.mNames[i]);
                    }
                    names.mCount = 0;
                    mTime = timeGetTime();
                }
            }
            break;
    }
}

MATCH_FUNC(0x51ef60)
s32 NetPlay::SendPing()
{
    u32 dataLen;
    s32 pData;
    Packet_Ping ping;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mType = 1;
    pStru.header.mSubType = packet_subtype::Ping_1;
    pStru.mbLoadData = 1;
    pStru.mSeq = 0;
    pStru.mData = static_cast<void*>(&ping);
    pStru.mLen = sizeof(ping);
    memset(&ping, 0, sizeof(ping));
    ping.mSenderPlayerId = mPlayerId;
    ping.mTime = timeGetTime();
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    return mpDPlay3->Send(mPlayerId, mN2.mGroupId, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x51f010)
bool NetPlay::Receive(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId)
{
    unsigned long readLen;
    for (readLen = 0x1800;
         !mpDPlay3->Receive(senderId, recvId, DPRECEIVE_ALL, reinterpret_cast<void*>(mpReceivedDataBuffer), &readLen);
         readLen = 0x1800)
    {
        if (*senderId)
        {
            s32 outDataLen = DecodeReceivedPacket(mpReceivedDataBuffer, readLen);
            *pOutData = mpReceivedDataBuffer;
            *pOutDataLen = outDataLen;
            return true;
        }

        // A system message: handle it and read the next message
        OnPacketReceived(reinterpret_cast<char*>(mpReceivedDataBuffer), readLen, *recvId, 0);
        if (mReceiveDisabled)
        {
            break;
        }
    }
    return false;
}

MATCH_FUNC(0x51f0d0)
void NetPlay::DispatchReceivedMessage(void* pPacket, s32 packetLen, s32 recvId, s32 bSendOrRecv)
{
    if (bSendOrRecv == 0)
    {
        NetPlay::OnPacketReceived(pPacket, packetLen, recvId, 0);
    }
    else
    {
        NetPlay::OnPlayerMessage(pPacket, packetLen, recvId, bSendOrRecv);
    }
}

MATCH_FUNC(0x51f110)
void NetPlay::OnPlayerMessage(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    u8* pBytes = static_cast<u8*>(pPacket);
    if (pBytes[0] == 1)
    {
        switch (pBytes[3])
        {
            case packet_subtype::Ping_1:
            {
                Packet_Ping* pPing = reinterpret_cast<Packet_Ping*>(pBytes + 5);
                if (mPlayerId == pPing->mSenderPlayerId)
                {
                    UpdatePlayerPing(pPing->mReceiverPlayerId, timeGetTime() - pPing->mTime, &mN2);
                }
                else
                {
                    pPing->mReceiverPlayerId = mPlayerId;
                    memset(&pStru, 0, sizeof(pStru));
                    pStru.header.mType = 1;
                    pStru.header.mSubType = packet_subtype::Ping_1;
                    pStru.mbLoadData = 1;
                    pStru.mSeq = 0;
                    pStru.mData = static_cast<void*>(pPing);
                    pStru.mLen = sizeof(Packet_Ping);
                    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
                    mpDPlay3->Send(mPlayerId, pPing->mSenderPlayerId, 0, reinterpret_cast<void*>(pData), dataLen);
                }
                break;
            }
            case packet_subtype::DisableJoining_4:
                InvokeCallback(9, 0);
                mNetState = 2;
                break;
        }
    }
}

// The csv size of the original stops at the FatalError_4A38C0 call, the epilogue after it isn't compared
MATCH_FUNC(0x51f210)
s32 NetPlay::DecodeReceivedPacket(s32 pPacket, u32 packetLen)
{
    u8 copy[64];
    u8* pBytes = reinterpret_cast<u8*>(pPacket);
    // The message header (type, length) is written through its own pointer: VC6 then reloads
    // pMsg[1] after the pBytes[0]/pBytes[1] stores, as the original does
    u8* pMsg = pBytes + 3; // pPacket[3] = packet_subtype
    u8* pPayload = pBytes + 5; // pPacket[5] = payload/data
    s32 len = 0;

    memcpy(copy, pBytes, packetLen);
    switch (copy[0])
    {
        case pkt_data_type::PlayerTickOnly_9:
            pMsg[0] = packet_subtype::PlayerInputs_3;
            if (bDo_sync_check)
            {
                pMsg[1] = 8; // pPacket[4]
            }
            else
            {
                pMsg[1] = 4; // pPacket[4]
            }
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = copy[1];
            pBytes[2] = pMsg[1] + 2;
            *reinterpret_cast<s32*>(pPayload) = -1;
            if (bDo_sync_check)
            {
                memcpy(pPayload + 4, &copy[2], packetLen - 2);
            }
            len = pMsg[1] + 5;
            break;

        case pkt_data_type::PlayerInputsAndTick_1:
            pMsg[0] = packet_subtype::PlayerInputs_3;
            pMsg[1] = packetLen - 2;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = copy[1];
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[2], packetLen - 2);
            len = packetLen + 3;
            break;

        case pkt_data_type::Ping_2:
            pMsg[0] = packet_subtype::Ping_1;
            pMsg[1] = packetLen - 1;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = copy[1];
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case pkt_data_type::Acknowledgment_4:
            pBytes[0] = packet_type::Acknowledgment_2;
            pBytes[1] = 0;
            pBytes[2] = 0;
            len = 3;
            break;

        // Separate (identical) cases: with more than 8 paths into the final return, VC6 keeps len in a
        // register (ebp, zeroed up front) instead of copying the return into each case and folding the
        // constants. The compiler merges the four bodies again.
        case pkt_data_type::SyncCheckData_3:
            pMsg[0] = packet_subtype::StandardPacket_2;
            pMsg[1] = packetLen - 1;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = 0;
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case pkt_data_type::PlayerQuit_5:
            pMsg[0] = packet_subtype::StandardPacket_2;
            pMsg[1] = packetLen - 1;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = 0;
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case pkt_data_type::GiveUp_6:
            pMsg[0] = packet_subtype::StandardPacket_2;
            pMsg[1] = packetLen - 1;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = 0;
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case pkt_data_type::KickFromConnection_7:
            pMsg[0] = packet_subtype::StandardPacket_2;
            pMsg[1] = packetLen - 1;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = 0;
            pBytes[2] = pMsg[1] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case pkt_data_type::DisableJoining_8:
            pMsg[0] = packet_subtype::DisableJoining_4;
            pMsg[1] = 0;
            pBytes[0] = packet_type::DataPacket_1;
            pBytes[1] = 0;
            pBytes[2] = 2;
            len = 5;
            break;

        default:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\NetworkX.cpp", 2192, 0);
            break;
    }
    return len;
}

MATCH_FUNC(0x51f420)
void NetPlay::MakeSendData(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen)
{
    u8* pBuffer = reinterpret_cast<u8*>(mpSendDataBuffer);
    *pDataLen = 0;

    switch (pPacket->header.mType)
    {
        case packet_type::DataPacket_1:
            if (pPacket->mbLoadData)
            {
                switch (pPacket->header.mSubType)
                {
                    case packet_subtype::Ping_1:
                        pBuffer[0] = pkt_data_type::Ping_2;
                        memcpy(pBuffer + 1, pPacket->mData, pPacket->mLen);
                        *pDataLen = pPacket->mLen + 1;
                        break;

                    case packet_subtype::StandardPacket_2:
                        switch (*static_cast<u8*>(pPacket->mData)) // Standard_Packet::mStdPacketSubtype
                        {
                            case std_packet_subtype::GiveUp_1:
                                pBuffer[0] = pkt_data_type::GiveUp_6;
                                memcpy(pBuffer + 1, pPacket->mData, 5);
                                *pDataLen = 6;
                                break;
                            case std_packet_subtype::PlayerQuit_2:
                                pBuffer[0] = pkt_data_type::PlayerQuit_5;
                                pBuffer[1] = *static_cast<u8*>(pPacket->mData); // mStdPacketSubtype
                                *pDataLen = 2;
                                break;
                            case std_packet_subtype::KickFromConnection_3:
                                pBuffer[0] = pkt_data_type::KickFromConnection_7;
                                pBuffer[1] = *static_cast<u8*>(pPacket->mData); // mStdPacketSubtype
                                *pDataLen = 2;
                                break;
                            case std_packet_subtype::SyncCheckData_5:
                                pBuffer[0] = pkt_data_type::SyncCheckData_3;
                                memcpy(pBuffer + 1, pPacket->mData, sizeof(SyncCheckData));
                                *pDataLen = 1 + sizeof(SyncCheckData);
                                break;
                            default:
                                FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\NetworkX.cpp", 2277, 0);
                                break;
                        }
                        break;

                    case packet_subtype::PlayerInputs_3: // In-Game data?
                    {
                        s32 idx = GetPlayerIdx();
                        if (SeqDiff(pPacket->mSeq, sLastPlayerInputSeq[idx]) == 1 && sLastPlayerInputs[idx] == *static_cast<s32*>(pPacket->mData))
                        {
                            sLastPlayerInputSeq[idx] = pPacket->mSeq;
                            pBuffer[0] = pkt_data_type::PlayerTickOnly_9;
                            pBuffer[1] = pPacket->mSeq;
                            *pDataLen = 2;
                            if (bDo_sync_check)
                            {
                                *reinterpret_cast<s32*>(pBuffer + 2) = reinterpret_cast<s32*>(pPacket->mData)[1];
                                *pDataLen += 4;
                            }
                        }
                        else
                        {
                            pBuffer[0] = pkt_data_type::PlayerInputsAndTick_1;
                            pBuffer[1] = pPacket->mSeq;
                            memcpy(pBuffer + 2, pPacket->mData, pPacket->mLen);
                            *pDataLen = pPacket->mLen + 2;
                            idx = GetPlayerIdx();
                            if (SeqDiff(pPacket->mSeq, sLastPlayerInputSeq[idx]) == 1)
                            {
                                sLastPlayerInputs[idx] = *static_cast<s32*>(pPacket->mData);
                                sLastPlayerInputSeq[GetPlayerIdx()] = pPacket->mSeq;
                            }
                        }
                        break;
                    }

                    case packet_subtype::DisableJoining_4:
                        pBuffer[0] = pkt_data_type::DisableJoining_8;
                        *pDataLen = 1;
                        break;

                    default:
                        FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\NetworkX.cpp", 2338, 0);
                        break;
                }
            }
            break;

        case packet_type::Acknowledgment_2:
            pBuffer[0] = pkt_data_type::Acknowledgment_4;
            *pDataLen = 1;
            break;

        default:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\NetworkX.cpp", 2356, 0);
            break;
    }
    *pData = reinterpret_cast<s32>(pBuffer);
}

MATCH_FUNC(0x51f870)
void NetPlay::OnPacketReceived(void* pPacket, s32 packetLen, s32 recvId, s32 sendFlag)
{
    switch (mNetState)
    {
        case 1:
        case 3:
        case 4:
        {
            DPMSG_GENERIC* pMsg = static_cast<DPMSG_GENERIC*>(pPacket);
            switch (pMsg->dwType)
            {
                case DPSYS_ADDPLAYERTOGROUP:
                {
                    DPMSG_ADDPLAYERTOGROUP* pAdd = reinterpret_cast<DPMSG_ADDPLAYERTOGROUP*>(pMsg);
                    s32 value;
                    if (static_cast<u32>(mN2.mGroupId) == pAdd->dpIdGroup &&
                        MovePlayerToGroup(pAdd->dpIdPlayer, &mN1, &mN2, reinterpret_cast<u32*>(&value)))
                    {
                        if (static_cast<u32>(mPlayerId) == pAdd->dpIdPlayer)
                        {
                            mPlayerIdx = value;
                        }
                        InvokeCallback(2, reinterpret_cast<u32>(&mN2.mPlayers[value]));
                    }
                    break;
                }

                case DPSYS_CREATEPLAYERORGROUP:
                {
                    DPMSG_CREATEPLAYERORGROUP* pCreate = reinterpret_cast<DPMSG_CREATEPLAYERORGROUP*>(pMsg);
                    if (pCreate->dwPlayerType != DPPLAYERTYPE_GROUP)
                    {
                        Network_Buffer data;
                        data.mData = pCreate->lpData;
                        data.mLen = pCreate->dwDataSize;
                        AddPlayer(&data, pCreate->dpId, pCreate->dpnName, &mN1);
                    }
                    break;
                }

                case DPSYS_DELETEPLAYERFROMGROUP:
                {
                    DPMSG_DELETEPLAYERFROMGROUP* pDelete = reinterpret_cast<DPMSG_DELETEPLAYERFROMGROUP*>(pMsg);
                    if (pDelete->dpIdGroup == static_cast<u32>(mN2.mGroupId))
                    {
                        if (static_cast<u32>(mPlayerId) == pDelete->dpIdPlayer)
                        {
                            InvokeCallback(1, 0);
                        }
                        else
                        {
                            u32 idx = IndexOf(pDelete->dpIdPlayer, &mN2);
                            if (idx != 0xEEEEEEEE)
                            {
                                InvokeCallback(3, reinterpret_cast<u32>(mN2.mPlayers[idx].mPlayerName));
                                FreePlayerSlot(idx, &mN2);
                            }
                        }
                    }
                    break;
                }

                case DPSYS_CHAT:
                {
                    DPMSG_CHAT* pChat = reinterpret_cast<DPMSG_CHAT*>(pMsg);
                    Network_ChatMessage chat;
                    LPDPCHAT pChatData = pChat->lpChat;
                    u32 idx = IndexOf(pChat->idFromPlayer, &mN2);
                    if (idx != 0xEEEEEEEE)
                    {
                        wcsncpy(chat.mMessage, pChatData->lpszMessage, 128);
                        wcsncpy(chat.mName, mN2.mPlayers[idx].mPlayerName, 16);
                        InvokeCallback(7, reinterpret_cast<u32>(&chat));
                    }
                    break;
                }

                case DPSYS_SETPLAYERORGROUPDATA:
                {
                    DPMSG_SETPLAYERORGROUPDATA* pSetData = reinterpret_cast<DPMSG_SETPLAYERORGROUPDATA*>(pMsg);
                    if (pSetData->dwPlayerType == DPPLAYERTYPE_GROUP && pSetData->dpId == static_cast<u32>(mN2.mGroupId))
                    {
                        if (mN2.mGameSettings)
                        {
                            operator delete(mN2.mGameSettings);
                            mN2.mGroupDataLen = 0;
                        }
                        mN2.mGameSettings = static_cast<u8*>(operator new(pSetData->dwDataSize));
                        mN2.mGroupDataLen = pSetData->dwDataSize;
                        memcpy(mN2.mGameSettings, pSetData->lpData, pSetData->dwDataSize);
                        InvokeCallback(8, reinterpret_cast<u32>(&mN2.mGameSettings));
                    }
                    break;
                }
            }
            break;
        }

        case 2:
        {
            DPMSG_GENERIC* pMsg = static_cast<DPMSG_GENERIC*>(pPacket);
            switch (pMsg->dwType)
            {
                case DPSYS_SESSIONLOST:
                {
                    s32 value = 0;
                    InvokeCallback(4, reinterpret_cast<u32>(&value));
                    mReceiveDisabled = 1;
                    break;
                }

                case DPSYS_DELETEPLAYERFROMGROUP:
                {
                    DPMSG_DELETEPLAYERFROMGROUP* pDelete = reinterpret_cast<DPMSG_DELETEPLAYERFROMGROUP*>(pMsg);
                    if (pDelete->dpIdGroup == static_cast<u32>(mN2.mGroupId))
                    {
                        if (static_cast<u32>(mPlayerId) == pDelete->dpIdPlayer)
                        {
                            s32 value = 2;
                            InvokeCallback(4, reinterpret_cast<u32>(&value));
                            mReceiveDisabled = 1;
                        }
                        else
                        {
                            u32 idx = IndexOf(pDelete->dpIdPlayer, &mN2);
                            if (idx != 0xEEEEEEEE)
                            {
                                InvokeCallback(3, reinterpret_cast<u32>(mN2.mPlayers[idx].mPlayerName));
                                FreePlayerSlot(idx, &mN2);
                            }
                        }
                    }
                    break;
                }

                case DPSYS_DESTROYPLAYERORGROUP:
                {
                    DPMSG_DESTROYPLAYERORGROUP* pDestroy = reinterpret_cast<DPMSG_DESTROYPLAYERORGROUP*>(pMsg);
                    switch (pDestroy->dwPlayerType)
                    {
                        case DPPLAYERTYPE_PLAYER:
                            if (pDestroy->dpId == static_cast<u32>(mPlayerId))
                            {
                                s32 value = 2;
                                InvokeCallback(4, reinterpret_cast<u32>(&value));
                                mReceiveDisabled = 1;
                            }
                            break;

                        case DPPLAYERTYPE_GROUP:
                            if (pDestroy->dpId == static_cast<u32>(mN2.mGroupId))
                            {
                                s32 value = 1;
                                InvokeCallback(4, reinterpret_cast<u32>(&value));
                                mReceiveDisabled = 1;
                            }
                            break;
                    }
                    break;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x520040)
s32 NetPlay::MovePlayerToGroup(s32 toFind, Network_Game_Session* pStru, Network_Game_Session* pDst, u32* pOutIdx)
{

    *pOutIdx = 0xEEEEEEEE;

    s32 result = 0;
    u32 i = 0;
    while (1)
    {
        if (i >= 6)
        {
            break;
        }
        if (pStru->mPlayers[i].mInUse && pStru->mPlayers[i].mPlayerId == toFind)
        {
            u32 new_idx = AddPlayer(reinterpret_cast<Network_Buffer*>(&pStru->mPlayers[i].mpData),
                                     pStru->mPlayers[i].mPlayerId,
                                     *reinterpret_cast<DPNAME*>(&pStru->mPlayers[i].mDpname),
                                     pDst);
            if (new_idx != 0xEEEEEEEE)
            {
                *pOutIdx = new_idx;
                result = 1;
                FreePlayerSlot(i, pStru);
            }
            break;
        }
        i++;
    }
    return result;
}

MATCH_FUNC(0x5201a0)
void NetPlay::FreePlayerSlot(s32 idx, Network_Game_Session* pStru)
{
    delete pStru->mPlayers[idx].mPlayerName;
    delete pStru->mPlayers[idx].mpData;
    memset(&pStru->mPlayers[idx], 0, sizeof(Network_Player));
    pStru->mPlayers[idx].mInUse = 0;
    pStru->mPlayersCount--;
}

// PacketHandlerSlot callbacks get the slot's context first; the slot index decides the other arguments
typedef void(__stdcall* TPacketCallback)(void*);
typedef void(__stdcall* TPacketCallbackArg)(void*, s32);
typedef void(__stdcall* TPacketCallback2Args)(void*, s32, s32);

MATCH_FUNC(0x520230)
void NetPlay::InvokeCallback(s32 idx, u32 event_data)
{
    if (idx >= 1 && idx <= 10)
    {
        switch (idx)
        {
            case 1: // disconnect ?
                if (mFuncPtrsAndParams[idx].mParamFnCallback)
                {
                    static_cast<TPacketCallback>(mFuncPtrsAndParams[idx].mParamFnCallback)(
                        mFuncPtrsAndParams[idx].mParamContext);
                }
                break;
            case 2: // a player joined the group (NetUi::cb_AddPlayerToList)
                if (mFuncPtrsAndParams[idx].mParamFnCallback)
                {
                    static_cast<TPacketCallback2Args>(mFuncPtrsAndParams[idx].mParamFnCallback)(
                        mFuncPtrsAndParams[idx].mParamContext,
                        *reinterpret_cast<s32*>(event_data + 0x1C), // Network_Player::mPlayerName
                        event_data + 0x24); // Network_Player::mpData
                }
                break;
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
                if (mFuncPtrsAndParams[idx].mParamFnCallback)
                {
                    static_cast<TPacketCallbackArg>(mFuncPtrsAndParams[idx].mParamFnCallback)(
                        mFuncPtrsAndParams[idx].mParamContext,
                        event_data); // a wchar_t* string?
                }
                break;
            case 4:
                // Exit game call back?
                if (mFuncPtrsAndParams[idx].mParamFnCallback)
                {
                    static_cast<TPacketCallbackArg>(mFuncPtrsAndParams[idx].mParamFnCallback)(
                        mFuncPtrsAndParams[idx].mParamContext,
                        *reinterpret_cast<u32*>(event_data)); // s32* usually set to 2 ?
                }
                break;

            case 9:
                // set player name?
                if (mFuncPtrsAndParams[idx].mParamFnCallback)
                {
                    static_cast<TPacketCallback>(mFuncPtrsAndParams[idx].mParamFnCallback)(
                        mFuncPtrsAndParams[idx].mParamContext);
                }
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x520530)
void NetPlay::Set6(void* pFunc, void* pParam)
{
    mFuncPtrsAndParams[2].mParamFnCallback = pFunc;
    mFuncPtrsAndParams[2].mParamContext = pParam;
    mFuncPtrsAndParams[2].mFnType = 2;
}

MATCH_FUNC(0x520570)
s32 NetPlay::JoinSession(u32 session_idx, wchar_t* pPlayerName, Network_Buffer* pPlayerData, Network_Buffer* pOutGroupData)
{
    DPNAME name;

    if (!mpDPlay3 || !pOutGroupData || !pPlayerName)
    {
        return 0;
    }

    memset(&mN1.mSessionDesc, 0, sizeof(DPSESSIONDESC2));
    DPSESSIONDESC2* pDesc = &mN2.mSessionDesc;
    memset(pDesc, 0, sizeof(DPSESSIONDESC2));
    pDesc->dwSize = sizeof(DPSESSIONDESC2);

    mN2.mSessionDesc.dwFlags = mSessions.mSessions[session_idx].dwFlags;
    mN2.mSessionDesc.guidInstance = mSessions.mSessions[session_idx].guidInstance;
    mN2.mSessionDesc.guidApplication = mSessions.mSessions[session_idx].guidApplication;
    mN2.mSessionDesc.dwMaxPlayers = mSessions.mSessions[session_idx].dwMaxPlayers;
    mN2.mSessionDesc.dwCurrentPlayers = mSessions.mSessions[session_idx].dwCurrentPlayers;
    mN2.mSessionDesc.lpszSessionName = static_cast<LPWSTR>(operator new(2 * wcslen(mSessions.mSessions[session_idx].lpszSessionName) + 2));
    wcscpy(mN2.mSessionDesc.lpszSessionName, mSessions.mSessions[session_idx].lpszSessionName);

    if (mpDPlay3->Open(pDesc, DPOPEN_JOIN | DPOPEN_RETURNSTATUS))
    {
        return 0;
    }

    memset(&name, 0, sizeof(name));
    name.lpszShortName = pPlayerName;
    name.dwSize = sizeof(DPNAME);
    name.lpszLongName = NULL;
    if (mpDPlay3->CreatePlayer(reinterpret_cast<LPDPID>(&mPlayerId),
                                        &name,
                                        mHandle,
                                        pPlayerData->mData,
                                        pPlayerData->mLen,
                                        0) < 0)
    {
        return 0;
    }

    if (mpDPlay3->EnumGroups(&mN2.mSessionDesc.guidInstance,
                                      reinterpret_cast<LPDPENUMPLAYERSCALLBACK2>(NetPlay::EnumGroups_cb),
                                      this,
                                      0) < 0)
    {
        return 0;
    }

    if (mpDPlay3->EnumGroupPlayers(mN2.mGroupId,
                                            &mN2.mSessionDesc.guidInstance,
                                            reinterpret_cast<LPDPENUMPLAYERSCALLBACK2>(NetPlay::EnumGroups_cb),
                                            this,
                                            0) < 0)
    {
        return 0;
    }

    u32 i;
    for (i = 0; i < mN2.mPlayersCount; i++)
    {
        if (mpDPlay3->GetPlayerData(mN2.mPlayers[i].mPlayerId,
                                             mN2.mPlayers[i].mpData,
                                             reinterpret_cast<LPDWORD>(&mN2.mPlayers[i].mDataLen),
                                             0) == DPERR_BUFFERTOOSMALL)
        {
            mN2.mPlayers[i].mpData = operator new(mN2.mPlayers[i].mDataLen);
            mpDPlay3->GetPlayerData(mN2.mPlayers[i].mPlayerId,
                                             mN2.mPlayers[i].mpData,
                                             reinterpret_cast<LPDWORD>(&mN2.mPlayers[i].mDataLen),
                                             0);
        }
    }

    for (i = 0; i < mN2.mPlayersCount; i++)
    {
        InvokeCallback(2, reinterpret_cast<u32>(&mN2.mPlayers[i]));
    }

    if (mpDPlay3->AddPlayerToGroup(mN2.mGroupId, mPlayerId) < 0)
    {
        return 0;
    }

    AddPlayer(pPlayerData, mPlayerId, name, &mN1);

    HRESULT hr = mpDPlay3->GetGroupData(mN2.mGroupId,
                                                  mN2.mGameSettings,
                                                  reinterpret_cast<LPDWORD>(&mN2.mGroupDataLen),
                                                  0);
    if (hr == DPERR_BUFFERTOOSMALL)
    {
        mN2.mGameSettings = static_cast<u8*>(operator new(mN2.mGroupDataLen));
        if (mpDPlay3->GetGroupData(mN2.mGroupId,
                                            mN2.mGameSettings,
                                            reinterpret_cast<LPDWORD>(&mN2.mGroupDataLen),
                                            0) < 0)
        {
            return 0;
        }
    }
    else if (hr < 0)
    {
        return 0;
    }

    mbIsHost = 0;
    mNotUsed5C8 = 1;
    mNetState = 4;
    void* pGroupData = operator new(mN2.mGroupDataLen);
    pOutGroupData->mData = pGroupData;
    pOutGroupData->mLen = mN2.mGroupDataLen;
    memcpy(pGroupData, mN2.mGameSettings, mN2.mGroupDataLen);
    InvokeCallback(8, reinterpret_cast<u32>(&mN2.mGameSettings));
    return 1;
}

MATCH_FUNC(0x520c20)
s32 __stdcall NetPlay::EnumGroups_cb(s32 group_id, s32 callback_type, s32 pName, char_type flags, NetPlay* pContext)
{
    if ((flags & 1) != 0)
    {
        return 0;
    }

    if (!callback_type)
    {
        pContext->SetGroupId(group_id);
        return 1;
    }

    if (callback_type == 1)
    {
        return pContext->AddEnumeratedGroupPlayer(group_id, reinterpret_cast<DPNAME*>(pName));
    }

    return 1;
}

MATCH_FUNC(0x520ca0)
s32 NetPlay::AddEnumeratedGroupPlayer(s32 player_id, DPNAME* pName)
{
    Network_Buffer data;
    data.mData = NULL;
    data.mLen = 0;
    return AddPlayer(&data, player_id, *pName, &mN2) != 0xEEEEEEEE;
}

MATCH_FUNC(0x520d00)
void NetPlay::SetGroupId(s32 group_id)
{
    mN2.mGroupId = group_id;
}

MATCH_FUNC(0x520d10)
void NetPlay::Disconnect()
{
    if (mbIsHost)
    {
        for (u32 i = mN2.mPlayersCount; i > 0; i--)
        {
            mpDPlay3->DeletePlayerFromGroup(mN2.mGroupId, mN2.mPlayers[i].mPlayerId);
        }
        mpDPlay3->DestroyGroup(mN2.mGroupId);
    }
    else if (mN2.mPlayersCount > 1)
    {
        SendPlayerIsQuitting();
    }

    mpDPlay3->DestroyPlayer(mPlayerId);
    mpDPlay3->Close();
    ClearPlayersAndSession(&mN2);
    ClearPlayersAndSession(&mN1);
    mTime = 0;
    mPlayerIdx = 0xEEEEEEEE;
    mPlayerOgIdx = 0xEEEEEEEE;
    mNetState = 1;
}

MATCH_FUNC(0x520de0)
void NetPlay::ClearPlayersAndSession(Network_Game_Session* pStru)
{
    u32 count = pStru->mPlayersCount;
    for (u32 i = 0; i < count; i++)
    {
        FreePlayerSlot(i, pStru);
    }

    if (pStru->mSessionDesc.lpszSessionName)
    {
        crt::free(pStru->mSessionDesc.lpszSessionName);
    }

    memset(&pStru->mSessionDesc, 0, sizeof(pStru->mSessionDesc));
}

MATCH_FUNC(0x520e30)
u32 NetPlay::IndexOf(s32 toFind, Network_Game_Session* pObj)
{
    u32 i = 0;
    while (1)
    {
        if (i >= 6)
        {
            break;
        }
        if (pObj->mPlayers[i].mPlayerId == toFind)
        {
            return i;
        }
        i++;
    }
    return 0xEEEEEEEE;
}

MATCH_FUNC(0x520e60)
void NetPlay::Set9(s32 pFunc, s32 pParam)
{
    this->mFuncPtrsAndParams[3].mParamFnCallback = reinterpret_cast<void*>(pFunc);
    this->mFuncPtrsAndParams[3].mParamContext = reinterpret_cast<void*>(pParam);
    this->mFuncPtrsAndParams[3].mFnType = 3;
}

MATCH_FUNC(0x520e80)
void NetPlay::Set3_Disconnect(s32 pCallback, s32 pContext)
{
    this->mFuncPtrsAndParams[1].mParamFnCallback = reinterpret_cast<void*>(pCallback);
    this->mFuncPtrsAndParams[1].mParamContext = reinterpret_cast<void*>(pContext);
    this->mFuncPtrsAndParams[1].mFnType = 1;
}

MATCH_FUNC(0x520ea0)
void NetPlay::NoRefs_null()
{
}

MATCH_FUNC(0x520eb0)
void NetPlay::UpdatePlayerPing(s32 player_id, s32 ping, Network_Game_Session* pSession)
{
    Network_PlayerPing info;

    u32 idx = IndexOf(player_id, pSession);
    if (idx != 0xEEEEEEEE)
    {
        pSession->mPlayers[idx].mPingSum += ping;
        pSession->mPlayers[idx].mNumPingSamples++;
        pSession->mPlayers[idx].mAveragePing = static_cast<u32>(pSession->mPlayers[idx].mPingSum) / static_cast<u32>(pSession->mPlayers[idx].mNumPingSamples);
        wcscpy(info.mName, pSession->mPlayers[idx].mPlayerName);
        info.mAvgPing = pSession->mPlayers[idx].mAveragePing;
        InvokeCallback(6, reinterpret_cast<u32>(&info));
    }
}

MATCH_FUNC(0x520f50)
void NetPlay::Set18(s32 pCallback, s32 pContext)
{
    this->mFuncPtrsAndParams[6].mParamFnCallback = reinterpret_cast<void*>(pCallback);
    this->mFuncPtrsAndParams[6].mParamContext = reinterpret_cast<void*>(pContext);
    this->mFuncPtrsAndParams[6].mFnType = 6;
}

// https://decomp.me/scratch/oYBrX
MATCH_FUNC(0x520f80)
s32 NetPlay::RemovePlayerByName(wchar_t* pToRemove)
{
    u32 i = 0;
    s32 bRemoved = 0;
    while (1)
    {
        if (i >= this->mN2.mPlayersCount)
        {
            break;
        }
        if (wcscmp(mN2.mPlayers[i].mPlayerName, pToRemove) == 0)
        {
            bRemoved = 1;
            mpDPlay3->DeletePlayerFromGroup(mN2.mGroupId, mN2.mPlayers[i].mPlayerId);
            break;
        }
        i++;
    }
    return bRemoved;
}

MATCH_FUNC(0x521000)
s32 NetPlay::DeletePlayerFromGroup(u32 idx)
{
    if (idx < MAX_PLAYERS && mN2.mPlayers[idx].mInUse)
    {
        s32 idPlayer = mN2.mPlayers[idx].mPlayerId;
        FreePlayerSlot(idx, &mN2);
        mpDPlay3->DeletePlayerFromGroup(mN2.mGroupId, idPlayer);
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x521060)
s32 NetPlay::SendChatMessage(wchar_t* pMsg, s32 idx_always_m1)
{
    s32 id_to;
    DPCHAT chatMsg;
    memset(&chatMsg, 0, sizeof(chatMsg));

    chatMsg.dwSize = sizeof(DPCHAT);
    chatMsg.lpszMessage = pMsg;
    if (idx_always_m1 < 0)
    {
        id_to = this->mN2.mGroupId;
    }
    else
    {
        id_to = this->mN2.mPlayers[idx_always_m1].mPlayerId;
    }
    return mpDPlay3->SendChatMessage(mPlayerId, id_to, 0, &chatMsg);
}

MATCH_FUNC(0x5210d0)
void NetPlay::Set21(s32 pCallback, s32 pContext)
{
    this->mFuncPtrsAndParams[7].mParamFnCallback = reinterpret_cast<void*>(pCallback);
    this->mFuncPtrsAndParams[7].mParamContext = reinterpret_cast<void*>(pContext);
    this->mFuncPtrsAndParams[7].mFnType = 7;
}

MATCH_FUNC(0x521100)
void NetPlay::GetPlayerName(wchar_t* Destination, u32 idx)
{
    u32 playerIdx = idx;
    if (idx == 7)
    {
        playerIdx = mPlayerIdx;
    }
    else if (mN2.mPlayersCount <= playerIdx)
    {
        return;
    }
    wcscpy(Destination, mN2.mPlayers[playerIdx].mPlayerName);
}

MATCH_FUNC(0x521140)
void NetPlay::Set24(s32 pCallback, s32 pContext)
{
    this->mFuncPtrsAndParams[8].mParamFnCallback = reinterpret_cast<void*>(pCallback);
    this->mFuncPtrsAndParams[8].mParamContext = reinterpret_cast<void*>(pContext);
    this->mFuncPtrsAndParams[8].mFnType = 8;
}

MATCH_FUNC(0x521170)
s32 NetPlay::SetGameSettings(Network_Buffer* pObj)
{

    if (mN2.mGameSettings)
    {
        delete[] mN2.mGameSettings;
        mN2.mGroupDataLen = 0;
    }

    mN2.mGameSettings = new u8[pObj->mLen];
    mN2.mGroupDataLen = pObj->mLen;

    memcpy(mN2.mGameSettings, pObj->mData, pObj->mLen);

    return mpDPlay3->SetGroupData(mN2.mGroupId, mN2.mGameSettings, mN2.mGroupDataLen, 2);
}

MATCH_FUNC(0x5211f0)
void NetPlay::Set27SavePlayerName(s32 pCallback, s32 pContext)
{
    this->mFuncPtrsAndParams[9].mParamFnCallback = reinterpret_cast<void*>(pCallback);
    this->mFuncPtrsAndParams[9].mParamContext = reinterpret_cast<void*>(pContext);
    this->mFuncPtrsAndParams[9].mFnType = 9;
}

MATCH_FUNC(0x521220)
void NetPlay::DisableJoining()
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.mSeq = 0;
    pStru.mData = NULL;
    pStru.mLen = 0;
    pStru.header.mType = 1;
    pStru.header.mSubType = packet_subtype::DisableJoining_4;
    pStru.mbLoadData = 1;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    mpDPlay3->Send(mPlayerId, mN2.mGroupId, DPSEND_GUARANTEED, reinterpret_cast<void*>(pData), dataLen);

    mN2.mSessionDesc.dwFlags |= DPSESSION_JOINDISABLED;
    if (mpDPlay3->SetSessionDesc(&mN2.mSessionDesc, 0) >= 0)
    {
        mNetState = 2;
    }
}

MATCH_FUNC(0x5212E0)
s32 NetPlay::IsUserHost()
{
    return mbIsHost;
}

MATCH_FUNC(0x521330)
void NetPlay::SetExitGameCallBack(s32 pFunc, Game* pGame)
{
    this->mFuncPtrsAndParams[4].mParamFnCallback = reinterpret_cast<void*>(pFunc);
    this->mFuncPtrsAndParams[4].mParamContext = pGame;
    this->mFuncPtrsAndParams[4].mFnType = 4;
}

MATCH_FUNC(0x521350)
s32 NetPlay::CountActivePlayers()
{
    s32 maxPlayers = 0;
    for (s32 idx = 0; idx < MAX_PLAYERS; idx++)
    {
        if (mN2.mPlayers[idx].mInUse)
        {
            maxPlayers++;
        }
    }
    return maxPlayers;
}

MATCH_FUNC(0x521370)
void NetPlay::SendAcknowledgment()
{
    u32 pDataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.mbLoadData = 0;
    pStru.header.mType = 2;
    NetPlay::MakeSendData(&pStru, &pData, &pDataLen);
    mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), pDataLen);
}

// The timeout test in the loop condition gives the shared return false block
MATCH_FUNC(0x5213e0)
bool NetPlay::WaitForPlayersSync()
{
    unsigned long senderId;
    s32 pData;
    DWORD startTime;
    unsigned long recvId;
    s32 dataLen;

    startTime = timeGetTime();
    bool bTimedOut = false;
    MakeInitialSyncData(gSyncCheckData);
    SendSyncCheckData(reinterpret_cast<s32>(gSyncCheckData));
    SendAcknowledgment();

    // One bit per other player: waiting for their ack and for their sync data
    u32 waitingForAck = 0;
    u32 waitingForSync = 0;
    for (u32 i = 0; i < MAX_PLAYERS; i++)
    {
        if (mN2.mPlayers[i].mInUse && i != static_cast<u32>(GetPlayerIdx()))
        {
            waitingForAck |= 1 << i;
            waitingForSync |= 1 << i;
        }
    }

    while ((waitingForSync || waitingForAck) && !bTimedOut)
    {
        if (Receive(&pData, &dataLen, &recvId, &senderId))
        {
            // Receive the acks or sync-check data
            u8* pPacket = reinterpret_cast<u8*>(pData);
            if (pPacket[0] == packet_type::Acknowledgment_2)
            {
                u32 idx = IndexOf(senderId, &mN2);
                if (idx == 0xEEEEEEEE)
                {
                    return false;
                }
                waitingForAck &= ~(1 << idx);
            }
            else if (pPacket[0] == packet_type::DataPacket_1 
                && pPacket[3] == packet_subtype::StandardPacket_2 
                && pPacket[5] == std_packet_subtype::SyncCheckData_5)
            {
                CompareRemotePlayers(gSyncCheckData, pPacket + 5);
                u32 idx = IndexOf(senderId, &mN2);
                if (idx == 0xEEEEEEEE)
                {
                    return false;
                }
                waitingForSync &= ~(1 << idx);
            }
        }
        bTimedOut = timeGetTime() - startTime > 20000;
    }
    if (bTimedOut)
    {
        return false;
    }
    return true;
}

MATCH_FUNC(0x5215b0)
s32 NetPlay::CopyConnection(u32 idx, u32* ppConnection, size_t* pLen)
{
    *ppConnection = 0;
    *pLen = 0;
    if (idx >= mEnumedConnections.mConnectionsCount)
    {
        return 0;
    }

    size_t len = mEnumedConnections.mEnumedConnections[idx].mConnectionLen;
    void* pConnection = operator new(len);
    memcpy(pConnection, mEnumedConnections.mEnumedConnections[idx].mpConnection, len);
    *ppConnection = reinterpret_cast<u32>(pConnection);
    *pLen = len;
    return 1;
}

MATCH_FUNC(0x521630)
s32 NetPlay::SendToPlayer(Network_Buffer* pSendData, s32 player_idx, char_type delayed_ticks)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mSubType = packet_subtype::PlayerInputs_3;
    pStru.header.mType = 1;
    pStru.mbLoadData = 1;
    pStru.mData = pSendData->mData;
    pStru.mLen = pSendData->mLen;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    reinterpret_cast<char_type*>(pData)[1] = mN2.mPacketGameTick[GetPlayerIdx()] - delayed_ticks;
    return mpDPlay3->Send(mPlayerId, mN2.mPlayers[player_idx].mPlayerId, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x5216e0)
void NetPlay::Add(Network_Buffer* pData, s32 id, char_type game_tick)
{
    for (u32 i = 0; i < 48; i++)
    {
        if (mPackets[i].mUsed == 1 && mPackets[i].mId == id &&
            mPackets[i].mGameTick == game_tick)
        {
            return;
        }
    }

    for (u32 j = 0; j < 48; j++)
    {
        if (!mPackets[j].mUsed)
        {
            mPackets[j].mInputs = *static_cast<Network_InputData*>(pData->mData);
            mPackets[j].mLen = pData->mLen;
            mPackets[j].mId = id;
            mPackets[j].mGameTick = game_tick;
            mPackets[j].mUsed = 1;
            mCount++;
            return;
        }
    }
}

MATCH_FUNC(0x521770)
u32 NetPlay::GetOldestBufferedPacket(Network_Buffer* pOut, char_type* pSeq, u32* pPlayerId)
{
#pragma warning(push)
#pragma warning(disable : 4700) // best_seq/best_idx are read uninitialised in the original too
    u8 best_seq;
    u32 best_idx;
    char_type bFound = 0;
    for (u32 i = 0; i < 48; i++)
    {
        if (mPackets[i].mUsed == 1)
        {
            if (!bFound)
            {
                best_seq = mPackets[i].mGameTick;
                best_idx = i;
                bFound = 1;
            }
            else if (SeqDiff(mPackets[i].mGameTick, best_seq) < 0)
            {
                best_seq = mPackets[i].mGameTick;
                best_idx = i;
            }
        }
    }

    if (bFound)
    {
        pOut->mLen = mPackets[best_idx].mLen;
        pOut->mData = &mPackets[best_idx];
        *pPlayerId = mPackets[best_idx].mId;
        *pSeq = best_seq;
        return best_idx;
    }
    return -1;
#pragma warning(pop)
}

MATCH_FUNC(0x521820)
void NetPlay::ResolvePlayerInput(s32** ppData, s32 idx)
{
    s32* p = *ppData;
    if (*p == -1)
    {
        *p = sLastPlayerInputs[idx];
    }
    else
    {
        sLastPlayerInputs[idx] = *p;
    }
    sLastPlayerInputSeq[idx] = mN2.mPacketGameTick[idx];
}

MATCH_FUNC(0x521870)
void NetPlay::Remove(s32 idx)
{
    mCount--;
    mPackets[idx].mUsed = 0;
}

MATCH_FUNC(0x521890)
bool NetPlay::ReceiveGameMessage(Network_Buffer* pOut, s32* pPlayerIdx, u32* packet_subtype)
{
    bool bGotMessage;
    char_type bCheckBuffered;
    char_type receivedTick;
    s32 pData;
    unsigned long senderId;
    unsigned long recvId;
    s32 dataLen;

    bCheckBuffered = 1;
    bGotMessage = false;
    bool bTimedOut = false;
    DWORD startTime = timeGetTime();

    // ((u8*)pData)[1] = mTick
    // ((u8*)pData)[2] = mMsgSize
    // ((u8*)pData)[3] = mType
    // ((u8*)pData)[4] = mLength
    // ((u8*)pData)[5] = mData

    while (1)
    {
        if (bTimedOut || mReceiveDisabled)
        {
            break;
        }

        u32 slot;
        if (bCheckBuffered && (slot = GetOldestBufferedPacket(pOut, &receivedTick, reinterpret_cast<u32*>(pPlayerIdx))) != -1)
        {
            // The oldest buffered (out of order) packet
            *packet_subtype = packet_subtype::PlayerInputs_3;
            bGotMessage = true;

            // diff: received player tick - our (buffered?) current stored tick for the same player
            s32 diff = SeqDiff(receivedTick, mN2.mPacketGameTick[*pPlayerIdx]);
            s32 diffLocal = SeqDiff(receivedTick, mN2.mPacketGameTick[GetPlayerIdx()]);
            if (diff < 0)
            {
                // we received an old packet, so ignore it and clear its packet slot
                bGotMessage = false;
                Remove(slot);
            }
            else if (diff <= 0 && diffLocal < 0)  // diff == 0 && diffLocal < 0
            {
                // our buffer matches tick from the player packet, but this tick is older than our user player tick
                Remove(slot);
                ResolvePlayerInput(reinterpret_cast<s32**>(pOut), *pPlayerIdx);
                NextSeq(&mN2, *pPlayerIdx);
            }
            else
            {
                // Not the next one yet: read new packets until it is
                bGotMessage = false;
                bCheckBuffered = 0;
            }
        }
        else
        {
            // No buffered packet: try a new one in the same pass (bCheckBuffered stays set)
            if (Receive(&pData, &dataLen, &recvId, &senderId))
            {
                *packet_subtype = reinterpret_cast<u8*>(pData)[3];
                Network_Game_Session* pN2 = &mN2;
                *pPlayerIdx = IndexOf(senderId, pN2);
                if (*pPlayerIdx != 0xEEEEEEEE)
                {
                    bGotMessage = true;
                    pOut->mLen = reinterpret_cast<u8*>(pData)[4];
                    pOut->mData = reinterpret_cast<u8*>(pData) + 5;
                    if (*packet_subtype == packet_subtype::PlayerInputs_3)
                    {
                        s32 diff = SeqDiff(reinterpret_cast<u8*>(pData)[1], mN2.mPacketGameTick[*pPlayerIdx]);
                        s32 diffLocal = SeqDiff(reinterpret_cast<u8*>(pData)[1], mN2.mPacketGameTick[GetPlayerIdx()]);
                        if (diff < 0)
                        {
                            // we received an old packet, so ignore it
                            bGotMessage = false;
                        }
                        else if (diff <= 0 && diffLocal < 0) // diff == 0 && diffLocal < 0
                        {
                            ResolvePlayerInput(reinterpret_cast<s32**>(pOut), *pPlayerIdx);
                            NextSeq(pN2, *pPlayerIdx);
                        }
                        else
                        {
                            // Not the next one in sequence yet: buffer it
                            bGotMessage = false;
                            Add(pOut, *pPlayerIdx, reinterpret_cast<u8*>(pData)[1]);
                        }
                    }
                }
            }
        }

        bTimedOut = timeGetTime() - startTime > 500;
        if (bGotMessage)
        {
            break;
        }
    }

    mTimeDiff = timeGetTime() - startTime;
    return bGotMessage;
}

MATCH_FUNC(0x521b20)
void NetPlay::SendToAll(Network_Buffer* pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mSubType = packet_subtype::PlayerInputs_3;
    pStru.header.mType = 1;
    pStru.mSeq = mN2.mPacketGameTick[GetPlayerIdx()];
    pStru.mbLoadData = 1;
    pStru.mData = pSendData->mData;
    pStru.mLen = pSendData->mLen;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
    const s32 player_idx = GetPlayerIdx();
    mN2.mPacketGameTick[player_idx] = (mN2.mPacketGameTick[player_idx] + 1) % 256;
}

MATCH_FUNC(0x521be0)
s32 NetPlay::NoRefs_SendToAllDelayed(Network_Buffer* pSendData, s32 tick_delay)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mSubType = packet_subtype::PlayerInputs_3;
    pStru.header.mType = 1;
    pStru.mSeq = mN2.mPacketGameTick[GetPlayerIdx()] - static_cast<char_type>(tick_delay);
    pStru.mbLoadData = 1;
    pStru.mData = pSendData->mData;
    pStru.mLen = pSendData->mLen;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    return mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x521c80)
s32 NetPlay::NoRefs_SendTickedStandardPacket(s32 pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mType = 1;
    pStru.header.mSubType = packet_subtype::StandardPacket_2;
    pStru.mbLoadData = 1;
    pStru.mData = reinterpret_cast<void*>(pSendData);
    pStru.mLen = 0xE;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    reinterpret_cast<char_type*>(pData)[1] = mN2.mPacketGameTick[GetPlayerIdx()];
    return mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x521d20)
s32 NetPlay::SendPlayerIsQuitting()
{

    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;
    char_type std_pkt_subtype;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mType = 1;
    std_pkt_subtype = std_packet_subtype::PlayerQuit_2;
    pStru.header.mSubType = packet_subtype::StandardPacket_2;
    pStru.mbLoadData = 1;
    pStru.mData = static_cast<void*>(&std_pkt_subtype);
    pStru.mLen = 1;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    return mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x521db0)
s32 NetPlay::SendGiveUp(s32 player_idx)
{
    Standard_Packet payload;
    payload.mStdPacketSubtype = std_packet_subtype::GiveUp_1;
    payload.mUnused1 = player_idx;

    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mType = packet_type::DataPacket_1;
    pStru.mbLoadData = 1;
    pStru.mData = static_cast<void*>(&payload);
    pStru.header.mSubType = packet_subtype::StandardPacket_2;
    pStru.mLen = 5;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    return mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
}

MATCH_FUNC(0x521e40)
s32 NetPlay::SendSyncCheckData(s32 pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.mType = 1;
    pStru.header.mSubType = packet_subtype::StandardPacket_2;
    pStru.mbLoadData = 1;
    pStru.mData = reinterpret_cast<void*>(pSendData); // SyncCheckData*
    pStru.mLen = 0x1F;
    NetPlay::MakeSendData(&pStru, &pData, &dataLen);
    return mpDPlay3->Send(mPlayerId, 0, 0, reinterpret_cast<void*>(pData), dataLen);
}

// The atexit destructor VC6 generates for gNetPlay, written out
MATCH_FUNC(0x5e4dd0)
void NetPlay::static_dtor()
{
    gNetPlay.NetPlay::~NetPlay();
}