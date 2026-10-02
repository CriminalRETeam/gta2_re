#include "NetPlay.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(NetPlay, gNetPlay_7071E8, 0x7071E8);
DEFINE_GLOBAL(GUID, kGta2_DP_Guid_5FE928, 0x5FE928);
DEFINE_GLOBAL_ARRAY(s32, dword_6F8A4C, 6, 0x6F8A4C);
DEFINE_GLOBAL_ARRAY(char_type, byte_6F8A64, 24, 0x6F8A64);

// Signed difference between two 8-bit sequence numbers, allowing for wrap around
static inline s32 SeqDiff(u8 a, u8 b)
{
    s32 diff = (u8)(a - b);
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

MATCH_FUNC(0x51d6b0)
NetPlay::NetPlay()
{
    field_CB8_count = 0;
    for (s32 i = 0; i < 48; i++)
    {
        field_8F8_packets[i].field_10_used = 0;
    }

    field_4_bModem = 0;
    field_5_modem_num = 0;
    field_8F0 = 0;
    field_8F4_time_diff = 0;
    field_5E4_pDPlay3 = 0;
    field_5E0_pDPlayLobby2 = 0;
    field_8E4_p0x1800_1 = (s32)operator new(0x1800);
    field_8E0_p0x1800_2 = (s32)operator new(0x1800);
    memset(&field_C4_sessions, 0, sizeof(field_C4_sessions));
    memset(field_4C_func_ptrs_and_params, 0, sizeof(field_4C_func_ptrs_and_params));
    memset(&field_30_enumed_connections, 0, sizeof(field_30_enumed_connections));
    memset(&field_758_n2, 0, sizeof(field_758_n2));
    memset(dword_6F8A4C, 0xFF, sizeof(dword_6F8A4C));
    memset(byte_6F8A64, 0, sizeof(byte_6F8A64));
    field_5DC_handle = CreateEventA(NULL, FALSE, FALSE, NULL);
    field_8EC = 0;
    field_48 = 5;

    for (s32 j = 0; j < 6; j++)
    {
        field_8C8[j] = (naughty_sinoussi_0x800*)operator new(0x800);
    }
}

// The scalar deleting destructor VC6 generates for the virtual ~NetPlay, written out
MATCH_FUNC(0x51d7b0)
void* NetPlay::vdtor_51D7B0(char_type flags)
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
    if (field_5E4_pDPlay3)
    {
        field_5E4_pDPlay3->DestroyPlayer(field_5D8_player_id);
        if (field_5CC)
        {
            field_5E4_pDPlay3->DestroyGroup(field_758_n2.field_0_group_id);
        }
        field_5E4_pDPlay3->Close();
    }

    if (field_30_enumed_connections.field_0_enumed_connections)
    {
        for (u32 i = 0; i < (u32)field_30_enumed_connections.field_8_connections_count; i++)
        {
            operator delete(field_30_enumed_connections.field_0_enumed_connections[i].field_10_pConnectionName);
            operator delete(field_30_enumed_connections.field_0_enumed_connections[i].field_14_pConnection);
        }
        operator delete(field_30_enumed_connections.field_0_enumed_connections);
        field_30_enumed_connections.field_0_enumed_connections = 0;
    }

    if (field_30_enumed_connections.field_4_d_array_8_entries)
    {
        for (u32 i = 0; i < field_30_enumed_connections.field_C_f4_d_array_count; i++)
        {
            operator delete(field_30_enumed_connections.field_4_d_array_8_entries[i].field_0_allocated_str);
        }
        operator delete(field_30_enumed_connections.field_4_d_array_8_entries);
        field_30_enumed_connections.field_4_d_array_8_entries = 0;
    }

    for (s32 j = 0; j < 6; j++)
    {
        operator delete(field_8C8[j]);
        operator delete(field_758_n2.field_10_players[j].field_1C_player_name);
        operator delete((void*)field_758_n2.field_10_players[j].field_24);
    }

    operator delete(field_758_n2.field_118_group_data);
    operator delete((void*)field_8E4_p0x1800_1);
    operator delete((void*)field_8E0_p0x1800_2);
    operator delete(field_758_n2.field_120_session_desc.lpszSessionName);
    field_30_enumed_connections.field_8_connections_count = 0;
    field_30_enumed_connections.field_C_f4_d_array_count = 0;
    field_30_enumed_connections.field_10 = 0;
    field_30_enumed_connections.field_14 = 0;
    CloseHandle(field_5DC_handle);
    DirectPlayDestroy_51DC90();
}

MATCH_FUNC(0x51d930)
void NetPlay::AddEnumeratedConnection_51D930(EnumeratedConnection* pConnectionInfo)
{
    const u32 connection_idx = this->field_30_enumed_connections.field_8_connections_count;
    if (connection_idx < 8)
    {
        EnumeratedConnection* pConnections = this->field_30_enumed_connections.field_0_enumed_connections;
        if (pConnections)
        {
            EnumeratedConnection* v5 = &pConnections[connection_idx];
            v5->field_0_sp_guid = pConnectionInfo->field_0_sp_guid;
            this->field_30_enumed_connections.field_0_enumed_connections[this->field_30_enumed_connections.field_8_connections_count]
                .field_10_pConnectionName = new wchar_t[wcslen(pConnectionInfo->field_10_pConnectionName) + 1];
            wcscpy(this->field_30_enumed_connections.field_0_enumed_connections[this->field_30_enumed_connections.field_8_connections_count]
                       .field_10_pConnectionName,
                   pConnectionInfo->field_10_pConnectionName);

            this->field_30_enumed_connections.field_0_enumed_connections[this->field_30_enumed_connections.field_8_connections_count]
                .field_14_pConnection = new u8[pConnectionInfo->field_18_connection_len];
            memcpy(this->field_30_enumed_connections.field_0_enumed_connections[this->field_30_enumed_connections.field_8_connections_count]
                       .field_14_pConnection,
                   pConnectionInfo->field_14_pConnection,
                   pConnectionInfo->field_18_connection_len);

            this->field_30_enumed_connections.field_0_enumed_connections[this->field_30_enumed_connections.field_8_connections_count]
                .field_18_connection_len = pConnectionInfo->field_18_connection_len;

            field_30_enumed_connections.field_8_connections_count++;
        }
    }
}

MATCH_FUNC(0x51da30)
s32 __stdcall NetPlay::EnumConnections_cb_51DA30(const GUID* lpguidSP,
                                                 void* lpConnection,
                                                 unsigned long dwConnectionSize,
                                                 const DPNAME* lpName,
                                                 unsigned long dwFlags,
                                                 void* lpContext)
{
    EnumeratedConnection info;
    info.field_0_sp_guid = *lpguidSP; // store guid
    info.field_14_pConnection = new u8[dwConnectionSize]; //alloc connection buffer
    memcpy(info.field_14_pConnection, lpConnection, dwConnectionSize); // copy into alloc
    info.field_18_connection_len = dwConnectionSize; // store size

    const size_t connectionNameLen = wcslen(lpName->lpszShortName); // get short name len
    info.field_10_pConnectionName = new wchar_t[connectionNameLen + 1]; // alloc buffer + null space
    wcscpy(info.field_10_pConnectionName, lpName->lpszShortName); // copy it
    ((NetPlay*)lpContext)->AddEnumeratedConnection_51D930(&info);
    delete[] info.field_10_pConnectionName; // free copy
    delete[] info.field_14_pConnection; // free copy
    return 1;
}

MATCH_FUNC(0x51dae0)
s32 NetPlay::SetProtoAndConnection_51DAE0(GUID* pProtocolGuid, Connection_Unknown* pUseThisConnection)
{
    s32 isIpx;

    if (pProtocolGuid)
    {
        if (memcmp(pProtocolGuid, &DPSPGUID_MODEM, sizeof(GUID)) == 0)
        {
            field_4_bModem = 1;
        }
        isIpx = 0;
        field_8_ip_or_ipx_guid = *pProtocolGuid;
    }
    else
    {
        isIpx = 1;
        field_8_ip_or_ipx_guid = DPSPGUID_IPX;
    }

    if (field_30_enumed_connections.field_0_enumed_connections)
    {
        delete[] field_30_enumed_connections.field_0_enumed_connections;
        field_30_enumed_connections.field_8_connections_count = 0;
        field_30_enumed_connections.field_10 = 0;
    }

    field_30_enumed_connections.field_0_enumed_connections = new EnumeratedConnection[8];
    if (NetPlay::DirectPlayCreate_51DCD0())
    {
        if (isIpx)
        {
            if (SUCCEEDED(field_5E4_pDPlay3->EnumConnections(&kGta2_DP_Guid_5FE928, NetPlay::EnumConnections_cb_51DA30, this, 1)))
            {
                field_30_enumed_connections.field_10 = 1;
                if (NetPlay::DirectPlayCreate_51DED0())
                {
                    field_30_enumed_connections.field_14 = 1;
                }
                return 1;
            }
            else
            {
                field_30_enumed_connections.field_10 = 0;
                NetPlay::DirectPlayDestroy_51DC90();
                return 0;
            }
        }
        else
        {
            EnumeratedConnection info;
            info.field_0_sp_guid = *pProtocolGuid;

            info.field_14_pConnection = new u8[pUseThisConnection->field_4_len];
            memcpy(info.field_14_pConnection, pUseThisConnection->field_0, pUseThisConnection->field_4_len);
            info.field_18_connection_len = pUseThisConnection->field_4_len;
            info.field_10_pConnectionName = new wchar_t[wcslen(L"dummy prot") + 1];
            wcscpy(info.field_10_pConnectionName, L"dummy prot");
            NetPlay::AddEnumeratedConnection_51D930(&info);

            delete[] info.field_10_pConnectionName;
            GTA2_DELETE_AND_NULL(info.field_14_pConnection);

            field_30_enumed_connections.field_10 = 1;
            return 1;
        }
    }
    else
    {
        NetPlay::DirectPlayDestroy_51DC90();
        return 0;
    }
}

MATCH_FUNC(0x51dc90)
void NetPlay::DirectPlayDestroy_51DC90()
{
    if (field_5E4_pDPlay3)
    {
        field_5E4_pDPlay3->Release();
        field_5E4_pDPlay3 = 0;
    }

    if (field_5E0_pDPlayLobby2)
    {
        field_5E0_pDPlayLobby2->Release();
        field_5E0_pDPlayLobby2 = 0;
    }
}

MATCH_FUNC(0x51dcd0)
s32 NetPlay::DirectPlayCreate_51DCD0()
{
    IDirectPlayLobby* pIDirectPlayLobby;

    DirectPlayDestroy_51DC90();

    if (CoCreateInstance(CLSID_DirectPlay, 0, 1u, IID_IDirectPlay3, (LPVOID*)&field_5E4_pDPlay3) < 0 ||
        DirectPlayLobbyCreateW(0, &pIDirectPlayLobby, 0, 0, 0) < 0)
    {
        return 0;
    }
    pIDirectPlayLobby->QueryInterface(IID_IDirectPlayLobby2, (void**)&this->field_5E0_pDPlayLobby2);
    pIDirectPlayLobby->Release();
    return 1;
}

MATCH_FUNC(0x51ded0)
s32 NetPlay::DirectPlayCreate_51DED0()
{
    GUID guid_DPSPGUID_MODEM = DPSPGUID_MODEM;
    this->field_30_enumed_connections.field_4_d_array_8_entries = new Network_4[8];
    this->field_30_enumed_connections.field_C_f4_d_array_count = 0;

    IDirectPlay* pDirectPlay;
    if (DirectPlayCreate(&guid_DPSPGUID_MODEM, &pDirectPlay, 0) < 0)
    {
        return 0;
    }

    IDirectPlay3* pDirectPlay3;
    if (FAILED(pDirectPlay->QueryInterface(IID_IDirectPlay3, (void**)&pDirectPlay3)))
    {
        return 0;
    }

    pDirectPlay->Release();

    if (!pDirectPlay3 || !field_5E0_pDPlayLobby2)
    {
        return 1;
    }
    {
        unsigned long playerAddressLen = 0;
        HRESULT result = pDirectPlay3->GetPlayerAddress(0, 0, &playerAddressLen);
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
                FAILED(field_5E0_pDPlayLobby2->EnumAddress(NetPlay::EnumAddress_cb_51E030, pPlayerAddressBuf, playerAddressLen, this)))
            {
                return 0;
            }
            delete[] pPlayerAddressBuf;
        }
        pDirectPlay3->Release();
    }

    return 1;
}

// Callee-saved register pushes differ, see docs/match_attempts.md
WIP_FUNC(0x51e030)
BOOL NetPlay::EnumAddress_cb_51E030(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext)
{
    WIP_IMPLEMENTED;

    LPCSTR pAddress = (LPCSTR)lpData;
    // The original keeps its "done" flag in lpData's stack slot and doesn't optimise it away
    volatile BOOL& bDone = *(volatile BOOL*)&lpData;
    bDone = FALSE;
    if (guidDataType == DPAID_INet && dwDataSize && lstrlenA(pAddress))
    {
        NetPlay* pThis = (NetPlay*)lpContext;
        if (!bDone)
        {
            do
            {
                wchar_t* pWide = new wchar_t[lstrlenA(pAddress) + 1];
                MultiByteToWideChar(0, 0, pAddress, -1, pWide, 2 * lstrlenA(pAddress) + 2);
                pThis->PushConnection_51E0E0(pWide);
                delete[] pWide;
                pAddress += lstrlenA(pAddress) + 1;
                if (!lstrlenA(pAddress))
                {
                    bDone = TRUE;
                }
            } while (lstrlenA(pAddress));
        }
    }
    return TRUE;
}

MATCH_FUNC(0x51e0e0)
s32 NetPlay::PushConnection_51E0E0(wchar_t* Source)
{
    if (field_30_enumed_connections.field_C_f4_d_array_count < 8)
    {
        size_t string_len_bytes = wcslen(Source);
        field_30_enumed_connections.field_4_d_array_8_entries[field_30_enumed_connections.field_C_f4_d_array_count].field_0_allocated_str =
            new wchar_t[string_len_bytes + 1];
        wcscpy(field_30_enumed_connections.field_4_d_array_8_entries[field_30_enumed_connections.field_C_f4_d_array_count]
                   .field_0_allocated_str,
               Source);
        ++field_30_enumed_connections.field_C_f4_d_array_count;
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x51e140)
s32 NetPlay::CreateTcpIpAddress_51E140(wchar_t* pIpAddress, s32* ppAddress, size_t* pAddressLen)
{
    DWORD addressLen = 0;
    if (!field_5E0_pDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    DPCOMPOUNDADDRESSELEMENT elements[2];
    memset(elements, 0, sizeof(elements));
    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = (LPVOID)&DPSPGUID_TCPIP;
    elements[1].guidDataType = DPAID_INetW;
    elements[1].dwDataSize = 2 * wcslen(pIpAddress) + 2;
    elements[1].lpData = pIpAddress;

    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 2, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    void* pAddress = operator new(addressLen);
    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 2, pAddress, &addressLen) < 0)
    {
        operator delete(pAddress);
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    *ppAddress = (s32)pAddress;
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e2b0)
s32 NetPlay::CreateModemAddress_51E2B0(wchar_t* pPhoneNumber, wchar_t* pModemName, s32* ppAddress, size_t* pAddressLen)
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

    if (!field_5E0_pDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = (LPVOID)&DPSPGUID_MODEM;

    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 1, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        // Shared cleanup block, likely a goto in the original too
        goto failed;
    }

    pAddress = operator new(addressLen);
    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 1, pAddress, &addressLen) != DP_OK)
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
    *ppAddress = (s32)pAddress;
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e450)
s32 NetPlay::CreateSerialAddress_51E450(DPCOMPORTADDRESS* pComPort, u32* ppAddress, size_t* pAddressLen)
{
    DWORD addressLen = 0;
    DPCOMPOUNDADDRESSELEMENT elements[2];
    memset(elements, 0, sizeof(elements));
    if (!field_5E0_pDPlayLobby2)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    elements[0].guidDataType = DPAID_ServiceProvider;
    elements[0].dwDataSize = sizeof(GUID);
    elements[0].lpData = (LPVOID)&DPSPGUID_SERIAL;
    elements[1].guidDataType = DPAID_ComPort;
    elements[1].dwDataSize = sizeof(DPCOMPORTADDRESS);
    elements[1].lpData = pComPort;

    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 2, NULL, &addressLen) != DPERR_BUFFERTOOSMALL)
    {
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    void* pAddress = operator new(addressLen);
    if (field_5E0_pDPlayLobby2->CreateCompoundAddress(elements, 2, pAddress, &addressLen) != DP_OK)
    {
        operator delete(pAddress);
        *ppAddress = 0;
        *pAddressLen = 0;
        return 0;
    }

    *ppAddress = (u32)pAddress;
    *pAddressLen = addressLen;
    return 1;
}

MATCH_FUNC(0x51e5c0)
s32 NetPlay::InitializeConnection_51E5C0()
{
    if (field_5E4_pDPlay3 && field_5E0_pDPlayLobby2)
    {
        u32 i;
        for (i = 0; i < 8; i++)
        {
            if (field_30_enumed_connections.field_0_enumed_connections[i].field_0_sp_guid == field_8_ip_or_ipx_guid)
            {
                break;
            }
        }

        if (field_5E4_pDPlay3->InitializeConnection(field_30_enumed_connections.field_0_enumed_connections[i].field_14_pConnection, 0) >= 0)
        {
            if (field_4_bModem || EnumSessions_51E650() != -1)
            {
                field_8E8_time = timeGetTime();
                field_48 = 1;
                return 1;
            }
        }
    }
    return 0;
}

// Return block layout differs, see docs/match_attempts.md
WIP_FUNC(0x51e650)
s32 NetPlay::EnumSessions_51E650()
{
    WIP_IMPLEMENTED;

    MSG msg;
    DPSESSIONDESC2 desc;

    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.guidApplication = kGta2_DP_Guid_5FE928;

    HRESULT hr;
    if (field_4_bModem)
    {
        hr = field_5E4_pDPlay3->EnumSessions(&desc, 0, (LPDPENUMSESSIONSCALLBACK2)NetPlay::EnumSessions_cb_51EAE0, this, 0);
        while (hr == DPERR_CONNECTING)
        {
            if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            Sleep(500);
            hr = field_5E4_pDPlay3->EnumSessions(&desc, 0, (LPDPENUMSESSIONSCALLBACK2)NetPlay::EnumSessions_cb_51EAE0, this, 0);
        }

        if (hr == DPERR_USERCANCEL)
        {
            return 0;
        }
        if (hr != DP_OK)
        {
            return -1;
        }

        hr = field_5E4_pDPlay3->EnumSessions(&desc, 0, (LPDPENUMSESSIONSCALLBACK2)NetPlay::EnumSessions_cb_51EAE0, this, DPENUMSESSIONS_STOPASYNC);
        if (hr == DPERR_USERCANCEL)
        {
            return 0;
        }
        if (hr != DP_OK)
        {
            return -1;
        }
    }
    else
    {
        hr = field_5E4_pDPlay3->EnumSessions(&desc,
                                             0,
                                             (LPDPENUMSESSIONSCALLBACK2)NetPlay::EnumSessions_cb_51EAE0,
                                             this,
                                             DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC);
        if (hr < 0)
        {
            return -1;
        }
        if (hr != DP_OK)
        {
            return -1;
        }
    }
    return field_C4_sessions.field_5C4_session_count;
}

MATCH_FUNC(0x51e7a0)
s32 NetPlay::HostSession_51E7A0(wchar_t* pSessionName, wchar_t* pPlayerName, Network_8* pGroupData, Network_8* pPlayerData)
{
    DPNAME name;

    if (field_5E4_pDPlay3 && field_5E0_pDPlayLobby2)
    {
        DPSESSIONDESC2* pDesc = &field_758_n2.field_120_session_desc;
        memset(pDesc, 0, sizeof(DPSESSIONDESC2));
        pDesc->dwSize = sizeof(DPSESSIONDESC2);
        field_758_n2.field_120_session_desc.dwFlags = 0x8000; // DPSESSION_DIRECTPLAYPROTOCOL, not in this DPLAY.H
        field_758_n2.field_120_session_desc.guidApplication = kGta2_DP_Guid_5FE928;
        field_758_n2.field_120_session_desc.dwMaxPlayers = 6;
        field_758_n2.field_120_session_desc.lpszSessionName = (LPWSTR)operator new(2 * wcslen(pSessionName) + 2);
        wcscpy(field_758_n2.field_120_session_desc.lpszSessionName, pSessionName);

        if (field_5E4_pDPlay3->Open(pDesc, DPOPEN_CREATE) >= 0)
        {
            memset(&name, 0, sizeof(name));
            name.lpszShortName = pPlayerName;
            name.lpszLongName = 0;
            name.dwSize = sizeof(DPNAME);
            field_5E4_pDPlay3->CreatePlayer((LPDPID)&field_5D8_player_id,
                                            &name,
                                            field_5DC_handle,
                                            pPlayerData->field_0,
                                            pPlayerData->field_4_len,
                                            0);

            field_5D0 = field_5D4_player_idx = AddPlayer_51E9C0(pPlayerData, field_5D8_player_id, name, &field_5E8_n1);

            field_758_n2.field_11C_group_data_len = pGroupData->field_4_len;
            field_758_n2.field_118_group_data = (u8*)operator new(pGroupData->field_4_len);
            if (field_758_n2.field_118_group_data)
            {
                memcpy(field_758_n2.field_118_group_data, pGroupData->field_0, pGroupData->field_4_len);

                memset(&name, 0, sizeof(name));
                name.dwSize = sizeof(DPNAME);
                // TODO: the group name string is a guess, only code is compared
                name.lpszShortName = L"GTA2";
                name.lpszLongName = 0;
                if (field_5E4_pDPlay3->CreateGroup((LPDPID)&field_758_n2.field_0_group_id,
                                                   &name,
                                                   field_758_n2.field_118_group_data,
                                                   field_758_n2.field_11C_group_data_len,
                                                   0) >= 0 &&
                    field_5E4_pDPlay3->AddPlayerToGroup(field_758_n2.field_0_group_id, field_5D8_player_id) >= 0)
                {
                    field_48 = 3;
                    field_5CC = 1;
                    field_5C8 = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x51e9c0)
u32 NetPlay::AddPlayer_51E9C0(Network_8* pData, s32 player_id, DPNAME name, Network_Unknown* pStru)
{
    u32 i = 0;
    if (pStru->field_4_count != 6)
    {
        while (1)
        {
            if (i >= 6)
            {
                break;
            }

            if (!pStru->field_10_players[i].field_0_in_use)
            {
                memset(&pStru->field_10_players[i], 0, sizeof(Nework_2C));
                pStru->field_10_players[i].field_10_player_id = player_id;
                memset(&pStru->field_10_players[i].field_14, 0, sizeof(DPNAME));

                if (wcslen(name.lpszShortName) < 15)
                {
                    pStru->field_10_players[i].field_1C_player_name = (wchar_t*)operator new(2 * wcslen(name.lpszShortName) + 2);
                    wcscpy(pStru->field_10_players[i].field_1C_player_name, name.lpszShortName);
                }
                else
                {
                    pStru->field_10_players[i].field_1C_player_name = (wchar_t*)operator new(0x20);
                    wcsncpy(pStru->field_10_players[i].field_1C_player_name, name.lpszShortName, 15);
                    pStru->field_10_players[i].field_1C_player_name[15] = 0;
                }

                pStru->field_10_players[i].field_24 = (s32)operator new(pData->field_4_len);
                pStru->field_10_players[i].field_28 = pData->field_4_len;
                memcpy((void*)pStru->field_10_players[i].field_24, pData->field_0, pData->field_4_len);
                pStru->field_10_players[i].field_0_in_use = 1;
                pStru->field_4_count++;
                return i;
            }
            i++;
        }
    }
    return 0xEEEEEEEE;
}

MATCH_FUNC(0x51eae0)
s32 __stdcall NetPlay::EnumSessions_cb_51EAE0(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, NetPlay* lpContext)
{
    if ((dwFlags & 1) != 0)
    {
        return 0;
    }
    else
    {
        return ((NetPlay*)lpContext)->AddEnumeratedSession_51EB00(lpThisSD);
    }
}

MATCH_FUNC(0x51eb00)
s32 NetPlay::AddEnumeratedSession_51EB00(DPSESSIONDESC2* pSession)
{
    //NOT_IMPLEMENTED;

    if (field_C4_sessions.field_5C4_session_count < 16)
    {
        memset(&field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count], 0, sizeof(DPSESSIONDESC2));
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwSize = sizeof(DPSESSIONDESC2);
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwFlags = pSession->dwFlags;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].guidInstance = pSession->guidInstance;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].guidApplication = pSession->guidApplication;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwMaxPlayers = pSession->dwMaxPlayers;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwCurrentPlayers = pSession->dwCurrentPlayers;

        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].lpszSessionName =
            new wchar_t[wcslen(pSession->lpszSessionName) + 1];
        wcscpy(field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].lpszSessionName, pSession->lpszSessionName);

        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwReserved1 = pSession->dwReserved1;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwReserved2 = pSession->dwReserved2;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwUser1 = pSession->dwUser1;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwUser2 = pSession->dwUser2;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwUser3 = pSession->dwUser3;
        field_C4_sessions.field_C4_sessions[field_C4_sessions.field_5C4_session_count].dwUser4 = pSession->dwUser4;

        field_C4_sessions.field_5C4_session_count++;

        return 1;
    }
    return 0;
}

MATCH_FUNC(0x51ecd0)
void NetPlay::Set15_51ECD0(s32 pFunc, Network_20324* pParam)
{
    this->field_4C_func_ptrs_and_params[5].field_0_param_fn_callback = (void*)pFunc;
    this->field_4C_func_ptrs_and_params[5].field_4_param_context = pParam;
    this->field_4C_func_ptrs_and_params[5].field_8_fn_type = 5;
}

MATCH_FUNC(0x51ed00)
void NetPlay::NetworkTick_51ED00()
{
    s32 pData;
    s32 dataLen;
    unsigned long recvId;
    unsigned long senderId;

    switch (field_48)
    {
        case 4:
        {
            if (timeGetTime() - field_8E8_time >= 1000)
            {
                SendPing_51EF60();
                field_8E8_time = timeGetTime();
            }

            if (Receive_51F010(&pData, &dataLen, &recvId, &senderId))
            {
                SendOrReceivePacket_51F0D0((void*)pData, dataLen, recvId, senderId);
            }
            break;
        }

        case 3:
        {
            if (timeGetTime() - field_8E8_time >= 1000)
            {
                SendPing_51EF60();
                field_8E8_time = timeGetTime();
            }

            if (Receive_51F010(&pData, &dataLen, &recvId, &senderId))
            {
                SendOrReceivePacket_51F0D0((void*)pData, dataLen, recvId, senderId);
            }
            break;
        }

        case 1:
            // Browsing for sessions: re-enumerate them once a second
            if (!field_4_bModem && timeGetTime() - field_8E8_time >= 1000)
            {
                u32 i;
                for (i = 0; i < field_C4_sessions.field_5C4_session_count; i++)
                {
                    operator delete(field_C4_sessions.field_C4_sessions[i].lpszSessionName);
                }
                memset(&field_C4_sessions, 0, sizeof(field_C4_sessions));

                DPSESSIONDESC2 desc;
                memset(&desc, 0, sizeof(desc));
                desc.dwSize = sizeof(desc);
                desc.guidApplication = kGta2_DP_Guid_5FE928;
                if (field_5E4_pDPlay3->EnumSessions(&desc,
                                                    1000,
                                                    (LPDPENUMSESSIONSCALLBACK2)NetPlay::EnumSessions_cb_51EAE0,
                                                    this,
                                                    DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC) == DP_OK)
                {
                    Network_NameList names;
                    names.field_40_count = field_C4_sessions.field_5C4_session_count;
                    for (i = 0; i < field_C4_sessions.field_5C4_session_count; i++)
                    {
                        names.field_0_names[i] = (wchar_t*)operator new(2 * wcslen(field_C4_sessions.field_C4_sessions[i].lpszSessionName) + 2);
                        wcscpy(names.field_0_names[i], field_C4_sessions.field_C4_sessions[i].lpszSessionName);
                    }

                    ProcessIncomingPacket_520230(5, (u32)&names);

                    for (i = 0; i < names.field_40_count; i++)
                    {
                        operator delete(names.field_0_names[i]);
                    }
                    names.field_40_count = 0;
                    field_8E8_time = timeGetTime();
                }
            }
            break;
    }
}

MATCH_FUNC(0x51ef60)
s32 NetPlay::SendPing_51EF60()
{
    u32 dataLen;
    s32 pData;
    Packet_Ping_C ping;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_0_type = 1;
    pStru.header.field_4_sub_type = 1;
    pStru.field_9 = 1;
    pStru.field_8 = 0;
    pStru.field_D = (s32)&ping;
    pStru.field_11_len = sizeof(ping);
    memset(&ping, 0, sizeof(ping));
    ping.field_0_player_id = field_5D8_player_id;
    ping.field_8_time = timeGetTime();
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    return field_5E4_pDPlay3->Send(field_5D8_player_id, field_758_n2.field_0_group_id, 0, (void*)pData, dataLen);
}

MATCH_FUNC(0x51f010)
char_type NetPlay::Receive_51F010(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId)
{
    unsigned long readLen = 0x1800;
    // The shared return 0 has to sit at the end of the loop body to get the original layout;
    // early returns or a break out of the loop change the code
    if (field_5E4_pDPlay3->Receive(senderId, recvId, DPRECEIVE_ALL, (void*)field_8E4_p0x1800_1, &readLen))
    {
        goto failed;
    }

    while (1)
    {
        if (*senderId)
        {
            break;
        }

        // A system message: handle it and read the next message
        OnPacketReceived_51F870((char*)field_8E4_p0x1800_1, readLen, *recvId, 0);
        if (field_8F0)
        {
            goto failed;
        }

        readLen = 0x1800;
        if (!field_5E4_pDPlay3->Receive(senderId, recvId, DPRECEIVE_ALL, (void*)field_8E4_p0x1800_1, &readLen))
        {
            continue;
        }

    failed:
        return 0;
    }

    s32 outDataLen = CalcPacketLen_51F210(field_8E4_p0x1800_1, readLen);
    *pOutData = field_8E4_p0x1800_1;
    *pOutDataLen = outDataLen;
    return 1;
}

MATCH_FUNC(0x51f0d0)
void NetPlay::SendOrReceivePacket_51F0D0(void* pPacket, s32 a3, s32 a4, s32 bSendOrRecv)
{
    if (bSendOrRecv == 0)
    {
        NetPlay::OnPacketReceived_51F870(pPacket, a3, a4, 0);
    }
    else
    {
        NetPlay::ProcessPingOrHandshakeSend_51F110(pPacket, a3, a4, bSendOrRecv);
    }
}

MATCH_FUNC(0x51f110)
void NetPlay::ProcessPingOrHandshakeSend_51F110(void* pPacket, s32 a3, s32 a4, s32 a5)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    u8* pBytes = (u8*)pPacket;
    if (pBytes[0] == 1)
    {
        switch (pBytes[3])
        {
            case 1:
            {
                Packet_Ping_C* pPing = (Packet_Ping_C*)(pBytes + 5);
                if (field_5D8_player_id == pPing->field_0_player_id)
                {
                    UpdatePlayerPing_520EB0(pPing->field_4, timeGetTime() - pPing->field_8_time, &field_758_n2);
                }
                else
                {
                    pPing->field_4 = field_5D8_player_id;
                    memset(&pStru, 0, sizeof(pStru));
                    pStru.header.field_0_type = 1;
                    pStru.header.field_4_sub_type = 1;
                    pStru.field_9 = 1;
                    pStru.field_8 = 0;
                    pStru.field_D = (s32)pPing;
                    pStru.field_11_len = sizeof(Packet_Ping_C);
                    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
                    field_5E4_pDPlay3->Send(field_5D8_player_id, pPing->field_0_player_id, 0, (void*)pData, dataLen);
                }
                break;
            }
            case 4:
                ProcessIncomingPacket_520230(9, 0);
                field_48 = 2;
                break;
        }
    }
}

// Register allocation differs, see docs/match_attempts.md
WIP_FUNC(0x51f210)
s32 NetPlay::CalcPacketLen_51F210(s32 pPacket, u32 packetLen)
{
    WIP_IMPLEMENTED;

    u8 copy[64];
    u8* pBytes = (u8*)pPacket;
    u8* pPayload = pBytes + 5;
    s32 len = 0;

    memcpy(copy, pBytes, packetLen);
    switch (copy[0])
    {
        case 9:
            pBytes[3] = 3;
            if (bDo_sync_check_67D6C1)
            {
                pBytes[4] = 8;
            }
            else
            {
                pBytes[4] = 4;
            }
            pBytes[0] = 1;
            pBytes[1] = copy[1];
            pBytes[2] = pBytes[4] + 2;
            *(s32*)pPayload = -1;
            if (bDo_sync_check_67D6C1)
            {
                memcpy(pPayload + 4, &copy[2], packetLen - 2);
            }
            len = pBytes[4] + 5;
            break;

        case 1:
            pBytes[3] = 3;
            pBytes[4] = packetLen - 2;
            pBytes[0] = 1;
            pBytes[1] = copy[1];
            pBytes[2] = pBytes[4] + 2;
            memcpy(pPayload, &copy[2], packetLen - 2);
            len = packetLen + 3;
            break;

        case 2:
            pBytes[3] = 1;
            pBytes[4] = packetLen - 1;
            pBytes[0] = 1;
            pBytes[1] = copy[1];
            pBytes[2] = pBytes[4] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case 4:
            pBytes[0] = 2;
            pBytes[1] = 0;
            pBytes[2] = 0;
            len = 3;
            break;

        case 3:
        case 5:
        case 6:
        case 7:
            pBytes[3] = 2;
            pBytes[4] = packetLen - 1;
            pBytes[0] = 1;
            pBytes[1] = 0;
            pBytes[2] = pBytes[4] + 2;
            memcpy(pPayload, &copy[1], packetLen - 1);
            len = packetLen + 4;
            break;

        case 8:
            pBytes[3] = 4;
            pBytes[4] = 0;
            pBytes[0] = 1;
            pBytes[1] = 0;
            pBytes[2] = 2;
            len = 5;
            break;

        default:
            // TODO: the source file name is a guess
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\netplay.cpp", 2192, 0);
            break;
    }
    return len;
}

MATCH_FUNC(0x51f420)
void NetPlay::MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen)
{
    u8* pBuffer = (u8*)field_8E0_p0x1800_2;
    *pDataLen = 0;

    switch (pPacket->header.field_0_type)
    {
        case 1:
            if (pPacket->field_9)
            {
                switch (pPacket->header.field_4_sub_type)
                {
                    case 1:
                        pBuffer[0] = 2;
                        memcpy(pBuffer + 1, (u8*)pPacket->field_D, pPacket->field_11_len);
                        *pDataLen = pPacket->field_11_len + 1;
                        break;

                    case 2:
                        switch (*(u8*)pPacket->field_D)
                        {
                            case 1:
                                pBuffer[0] = 6;
                                memcpy(pBuffer + 1, (u8*)pPacket->field_D, 5);
                                *pDataLen = 6;
                                break;
                            case 2:
                                pBuffer[0] = 5;
                                pBuffer[1] = *(u8*)pPacket->field_D;
                                *pDataLen = 2;
                                break;
                            case 3:
                                pBuffer[0] = 7;
                                pBuffer[1] = *(u8*)pPacket->field_D;
                                *pDataLen = 2;
                                break;
                            case 5:
                                pBuffer[0] = 3;
                                memcpy(pBuffer + 1, (u8*)pPacket->field_D, 0x1F);
                                *pDataLen = 0x20;
                                break;
                            default:
                                // TODO: the source file name is a guess
                                FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\netplay.cpp", 2277, 0);
                                break;
                        }
                        break;

                    case 3:
                    {
                        s32 idx = field_5D4_player_idx;
                        if (SeqDiff(pPacket->field_8, byte_6F8A64[idx]) == 1 && dword_6F8A4C[idx] == *(s32*)pPacket->field_D)
                        {
                            byte_6F8A64[idx] = pPacket->field_8;
                            pBuffer[0] = 9;
                            pBuffer[1] = pPacket->field_8;
                            *pDataLen = 2;
                            if (bDo_sync_check_67D6C1)
                            {
                                *(s32*)(pBuffer + 2) = ((s32*)pPacket->field_D)[1];
                                *pDataLen += 4;
                            }
                        }
                        else
                        {
                            pBuffer[0] = 1;
                            pBuffer[1] = pPacket->field_8;
                            memcpy(pBuffer + 2, (u8*)pPacket->field_D, pPacket->field_11_len);
                            *pDataLen = pPacket->field_11_len + 2;
                            idx = field_5D4_player_idx;
                            if (SeqDiff(pPacket->field_8, byte_6F8A64[idx]) == 1)
                            {
                                dword_6F8A4C[idx] = *(s32*)pPacket->field_D;
                                byte_6F8A64[field_5D4_player_idx] = pPacket->field_8;
                            }
                        }
                        break;
                    }

                    case 4:
                        pBuffer[0] = 8;
                        *pDataLen = 1;
                        break;

                    default:
                        FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\netplay.cpp", 2338, 0);
                        break;
                }
            }
            break;

        case 2:
            pBuffer[0] = 4;
            *pDataLen = 1;
            break;

        default:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\netplay.cpp", 2356, 0);
            break;
    }
    *pData = (s32)pBuffer;
}

MATCH_FUNC(0x51f870)
void NetPlay::OnPacketReceived_51F870(void* pPacket, s32 packetLen, s32 recvId, s32 a5)
{
    switch (field_48)
    {
        case 1:
        case 3:
        case 4:
        {
            DPMSG_GENERIC* pMsg = (DPMSG_GENERIC*)pPacket;
            switch (pMsg->dwType)
            {
                case DPSYS_ADDPLAYERTOGROUP:
                {
                    DPMSG_ADDPLAYERTOGROUP* pAdd = (DPMSG_ADDPLAYERTOGROUP*)pMsg;
                    s32 value;
                    if (field_758_n2.field_0_group_id == pAdd->dpIdGroup &&
                        MovePlayerToGroup_520040(pAdd->dpIdPlayer, &field_5E8_n1, &field_758_n2, (u32*)&value))
                    {
                        if (field_5D8_player_id == pAdd->dpIdPlayer)
                        {
                            field_5D4_player_idx = value;
                        }
                        ProcessIncomingPacket_520230(2, (u32)&field_758_n2.field_10_players[value]);
                    }
                    break;
                }

                case DPSYS_CREATEPLAYERORGROUP:
                {
                    DPMSG_CREATEPLAYERORGROUP* pCreate = (DPMSG_CREATEPLAYERORGROUP*)pMsg;
                    if (pCreate->dwPlayerType != DPPLAYERTYPE_GROUP)
                    {
                        Network_8 data;
                        data.field_0 = pCreate->lpData;
                        data.field_4_len = pCreate->dwDataSize;
                        AddPlayer_51E9C0(&data, pCreate->dpId, pCreate->dpnName, &field_5E8_n1);
                    }
                    break;
                }

                case DPSYS_DELETEPLAYERFROMGROUP:
                {
                    DPMSG_DELETEPLAYERFROMGROUP* pDelete = (DPMSG_DELETEPLAYERFROMGROUP*)pMsg;
                    if (pDelete->dpIdGroup == field_758_n2.field_0_group_id)
                    {
                        if (field_5D8_player_id == pDelete->dpIdPlayer)
                        {
                            ProcessIncomingPacket_520230(1, 0);
                        }
                        else
                        {
                            u32 idx = IndexOf_520E30(pDelete->dpIdPlayer, &field_758_n2);
                            if (idx != 0xEEEEEEEE)
                            {
                                ProcessIncomingPacket_520230(3, (u32)field_758_n2.field_10_players[idx].field_1C_player_name);
                                FreePlayerSlot_5201A0(idx, &field_758_n2);
                            }
                        }
                    }
                    break;
                }

                case DPSYS_CHAT:
                {
                    DPMSG_CHAT* pChat = (DPMSG_CHAT*)pMsg;
                    Network_ChatMessage chat;
                    LPDPCHAT pChatData = pChat->lpChat;
                    u32 idx = IndexOf_520E30(pChat->idFromPlayer, &field_758_n2);
                    if (idx != 0xEEEEEEEE)
                    {
                        wcsncpy(chat.field_0_message, pChatData->lpszMessage, 128);
                        wcsncpy(chat.field_100_name, field_758_n2.field_10_players[idx].field_1C_player_name, 16);
                        ProcessIncomingPacket_520230(7, (u32)&chat);
                    }
                    break;
                }

                case DPSYS_SETPLAYERORGROUPDATA:
                {
                    DPMSG_SETPLAYERORGROUPDATA* pSetData = (DPMSG_SETPLAYERORGROUPDATA*)pMsg;
                    if (pSetData->dwPlayerType == DPPLAYERTYPE_GROUP && pSetData->dpId == field_758_n2.field_0_group_id)
                    {
                        if (field_758_n2.field_118_group_data)
                        {
                            operator delete(field_758_n2.field_118_group_data);
                            field_758_n2.field_11C_group_data_len = 0;
                        }
                        field_758_n2.field_118_group_data = (u8*)operator new(pSetData->dwDataSize);
                        field_758_n2.field_11C_group_data_len = pSetData->dwDataSize;
                        memcpy(field_758_n2.field_118_group_data, pSetData->lpData, pSetData->dwDataSize);
                        ProcessIncomingPacket_520230(8, (u32)&field_758_n2.field_118_group_data);
                    }
                    break;
                }
            }
            break;
        }

        case 2:
        {
            DPMSG_GENERIC* pMsg = (DPMSG_GENERIC*)pPacket;
            switch (pMsg->dwType)
            {
                case DPSYS_SESSIONLOST:
                {
                    s32 value = 0;
                    ProcessIncomingPacket_520230(4, (u32)&value);
                    field_8F0 = 1;
                    break;
                }

                case DPSYS_DELETEPLAYERFROMGROUP:
                {
                    DPMSG_DELETEPLAYERFROMGROUP* pDelete = (DPMSG_DELETEPLAYERFROMGROUP*)pMsg;
                    if (pDelete->dpIdGroup == field_758_n2.field_0_group_id)
                    {
                        if (field_5D8_player_id == pDelete->dpIdPlayer)
                        {
                            s32 value = 2;
                            ProcessIncomingPacket_520230(4, (u32)&value);
                            field_8F0 = 1;
                        }
                        else
                        {
                            u32 idx = IndexOf_520E30(pDelete->dpIdPlayer, &field_758_n2);
                            if (idx != 0xEEEEEEEE)
                            {
                                ProcessIncomingPacket_520230(3, (u32)field_758_n2.field_10_players[idx].field_1C_player_name);
                                FreePlayerSlot_5201A0(idx, &field_758_n2);
                            }
                        }
                    }
                    break;
                }

                case DPSYS_DESTROYPLAYERORGROUP:
                {
                    DPMSG_DESTROYPLAYERORGROUP* pDestroy = (DPMSG_DESTROYPLAYERORGROUP*)pMsg;
                    switch (pDestroy->dwPlayerType)
                    {
                        case DPPLAYERTYPE_PLAYER:
                            if (pDestroy->dpId == field_5D8_player_id)
                            {
                                s32 value = 2;
                                ProcessIncomingPacket_520230(4, (u32)&value);
                                field_8F0 = 1;
                            }
                            break;

                        case DPPLAYERTYPE_GROUP:
                            if (pDestroy->dpId == field_758_n2.field_0_group_id)
                            {
                                s32 value = 1;
                                ProcessIncomingPacket_520230(4, (u32)&value);
                                field_8F0 = 1;
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

// Register allocation differs, see docs/match_attempts.md
WIP_FUNC(0x520040)
s32 NetPlay::MovePlayerToGroup_520040(s32 toFind, Network_Unknown* pStru, Network_Unknown* pDst, u32* pOutIdx)
{
    WIP_IMPLEMENTED;

    *pOutIdx = 0xEEEEEEEE;

    u32 i = 0;
    while (1)
    {
        if (i >= 6)
        {
            break;
        }
        if (pStru->field_10_players[i].field_0_in_use && pStru->field_10_players[i].field_10_player_id == toFind)
        {
            u32 new_idx = AddPlayer_51E9C0((Network_8*)&pStru->field_10_players[i].field_24,
                                     pStru->field_10_players[i].field_10_player_id,
                                     *(DPNAME*)&pStru->field_10_players[i].field_14,
                                     pDst);
            if (new_idx != 0xEEEEEEEE)
            {
                *pOutIdx = new_idx;
                FreePlayerSlot_5201A0(i, pStru);
                return 1;
            }
            break;
        }
        i++;
    }
    return 0;
}

MATCH_FUNC(0x5201a0)
void NetPlay::FreePlayerSlot_5201A0(s32 idx, Network_Unknown* pStru)
{
    delete pStru->field_10_players[idx].field_1C_player_name;
    delete (void*)pStru->field_10_players[idx].field_24;
    memset(&pStru->field_10_players[idx], 0, sizeof(Nework_2C));
    pStru->field_10_players[idx].field_0_in_use = 0;
    pStru->field_4_count--;
}

// TODO: Temp - should be integrated into the type array or something
typedef void(__stdcall* T1)(void*);
typedef void(__stdcall* T2)(void*, s32);
typedef void(__stdcall* T3)(void*, s32, s32);

MATCH_FUNC(0x520230)
void NetPlay::ProcessIncomingPacket_520230(s32 idx, u32 pUnknown)
{
    // TODO: Figure out what pUnknown is
    if (idx >= 1 && idx <= 10)
    {
        switch (idx)
        {
            case 1: // disconnect ?
                if (field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)
                {
                    ((T1)field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)(
                        field_4C_func_ptrs_and_params[idx].field_4_param_context);
                }
                break;
            case 2: // adding to listbox, player? session??
                if (field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)
                {
                    ((T3)field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)(
                        field_4C_func_ptrs_and_params[idx].field_4_param_context,
                        *(s32*)(pUnknown + 0x1C),
                        pUnknown + 0x24);
                }
                break;
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
                if (field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)
                {
                    ((T2)field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)(
                        field_4C_func_ptrs_and_params[idx].field_4_param_context,
                        pUnknown); // a wchar_t* string?
                }
                break;
            case 4:
                // Exit game call back?
                if (field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)
                {
                    ((T2)field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)(
                        field_4C_func_ptrs_and_params[idx].field_4_param_context,
                        *(u32*)pUnknown); // s32* usually set to 2 ?
                }
                break;

            case 9:
                // set player name?
                if (field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)
                {
                    ((T1)field_4C_func_ptrs_and_params[idx].field_0_param_fn_callback)(
                        field_4C_func_ptrs_and_params[idx].field_4_param_context);
                }
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x520530)
void NetPlay::Set6_520530(void* pFunc, void* pParam)
{
    field_4C_func_ptrs_and_params[2].field_0_param_fn_callback = pFunc;
    field_4C_func_ptrs_and_params[2].field_4_param_context = pParam;
    field_4C_func_ptrs_and_params[2].field_8_fn_type = 2;
}

MATCH_FUNC(0x520570)
s32 NetPlay::JoinSession_520570(u32 session_idx, wchar_t* pPlayerName, Network_8* pPlayerData, Network_8* pOutGroupData)
{
    DPNAME name;

    if (!field_5E4_pDPlay3 || !pOutGroupData || !pPlayerName)
    {
        return 0;
    }

    memset(&field_5E8_n1.field_120_session_desc, 0, sizeof(DPSESSIONDESC2));
    DPSESSIONDESC2* pDesc = &field_758_n2.field_120_session_desc;
    memset(pDesc, 0, sizeof(DPSESSIONDESC2));
    pDesc->dwSize = sizeof(DPSESSIONDESC2);

    field_758_n2.field_120_session_desc.dwFlags = field_C4_sessions.field_C4_sessions[session_idx].dwFlags;
    field_758_n2.field_120_session_desc.guidInstance = field_C4_sessions.field_C4_sessions[session_idx].guidInstance;
    field_758_n2.field_120_session_desc.guidApplication = field_C4_sessions.field_C4_sessions[session_idx].guidApplication;
    field_758_n2.field_120_session_desc.dwMaxPlayers = field_C4_sessions.field_C4_sessions[session_idx].dwMaxPlayers;
    field_758_n2.field_120_session_desc.dwCurrentPlayers = field_C4_sessions.field_C4_sessions[session_idx].dwCurrentPlayers;
    field_758_n2.field_120_session_desc.lpszSessionName = (LPWSTR)operator new(2 * wcslen(field_C4_sessions.field_C4_sessions[session_idx].lpszSessionName) + 2);
    wcscpy(field_758_n2.field_120_session_desc.lpszSessionName, field_C4_sessions.field_C4_sessions[session_idx].lpszSessionName);

    if (field_5E4_pDPlay3->Open(pDesc, DPOPEN_JOIN | DPOPEN_RETURNSTATUS))
    {
        return 0;
    }

    memset(&name, 0, sizeof(name));
    name.lpszShortName = pPlayerName;
    name.dwSize = sizeof(DPNAME);
    name.lpszLongName = 0;
    if (field_5E4_pDPlay3->CreatePlayer((LPDPID)&field_5D8_player_id,
                                        &name,
                                        field_5DC_handle,
                                        pPlayerData->field_0,
                                        pPlayerData->field_4_len,
                                        0) < 0)
    {
        return 0;
    }

    if (field_5E4_pDPlay3->EnumGroups(&field_758_n2.field_120_session_desc.guidInstance,
                                      (LPDPENUMPLAYERSCALLBACK2)NetPlay::EnumGroups_cb_520C20,
                                      this,
                                      0) < 0)
    {
        return 0;
    }

    if (field_5E4_pDPlay3->EnumGroupPlayers(field_758_n2.field_0_group_id,
                                            &field_758_n2.field_120_session_desc.guidInstance,
                                            (LPDPENUMPLAYERSCALLBACK2)NetPlay::EnumGroups_cb_520C20,
                                            this,
                                            0) < 0)
    {
        return 0;
    }

    u32 i;
    for (i = 0; i < field_758_n2.field_4_count; i++)
    {
        if (field_5E4_pDPlay3->GetPlayerData(field_758_n2.field_10_players[i].field_10_player_id,
                                             (void*)field_758_n2.field_10_players[i].field_24,
                                             (LPDWORD)&field_758_n2.field_10_players[i].field_28,
                                             0) == DPERR_BUFFERTOOSMALL)
        {
            field_758_n2.field_10_players[i].field_24 = (s32)operator new(field_758_n2.field_10_players[i].field_28);
            field_5E4_pDPlay3->GetPlayerData(field_758_n2.field_10_players[i].field_10_player_id,
                                             (void*)field_758_n2.field_10_players[i].field_24,
                                             (LPDWORD)&field_758_n2.field_10_players[i].field_28,
                                             0);
        }
    }

    for (i = 0; i < field_758_n2.field_4_count; i++)
    {
        ProcessIncomingPacket_520230(2, (u32)&field_758_n2.field_10_players[i]);
    }

    if (field_5E4_pDPlay3->AddPlayerToGroup(field_758_n2.field_0_group_id, field_5D8_player_id) < 0)
    {
        return 0;
    }

    AddPlayer_51E9C0(pPlayerData, field_5D8_player_id, name, &field_5E8_n1);

    HRESULT hr = field_5E4_pDPlay3->GetGroupData(field_758_n2.field_0_group_id,
                                                  field_758_n2.field_118_group_data,
                                                  (LPDWORD)&field_758_n2.field_11C_group_data_len,
                                                  0);
    if (hr == DPERR_BUFFERTOOSMALL)
    {
        field_758_n2.field_118_group_data = (u8*)operator new(field_758_n2.field_11C_group_data_len);
        if (field_5E4_pDPlay3->GetGroupData(field_758_n2.field_0_group_id,
                                            field_758_n2.field_118_group_data,
                                            (LPDWORD)&field_758_n2.field_11C_group_data_len,
                                            0) < 0)
        {
            return 0;
        }
    }
    else if (hr < 0)
    {
        return 0;
    }

    field_5CC = 0;
    field_5C8 = 1;
    field_48 = 4;
    void* pGroupData = operator new(field_758_n2.field_11C_group_data_len);
    pOutGroupData->field_0 = pGroupData;
    pOutGroupData->field_4_len = field_758_n2.field_11C_group_data_len;
    memcpy(pGroupData, field_758_n2.field_118_group_data, field_758_n2.field_11C_group_data_len);
    ProcessIncomingPacket_520230(8, (u32)&field_758_n2.field_118_group_data);
    return 1;
}

MATCH_FUNC(0x520c20)
s32 __stdcall NetPlay::EnumGroups_cb_520C20(s32 a1, s32 a2, s32 a3, char_type a4, NetPlay* pContext)
{
    if ((a4 & 1) != 0)
    {
        return 0;
    }

    if (!a2)
    {
        pContext->SetGroupId_520D00(a1);
        return 1;
    }

    if (a2 == 1)
    {
        return pContext->AddEnumeratedGroupPlayer_520CA0(a1, (DPNAME*)a3);
    }

    return 1;
}

MATCH_FUNC(0x520ca0)
s32 NetPlay::AddEnumeratedGroupPlayer_520CA0(s32 player_id, DPNAME* pName)
{
    Network_8 data;
    data.field_0 = 0;
    data.field_4_len = 0;
    return AddPlayer_51E9C0(&data, player_id, *pName, &field_758_n2) != 0xEEEEEEEE;
}

MATCH_FUNC(0x520d00)
void NetPlay::SetGroupId_520D00(s32 a2)
{
    field_758_n2.field_0_group_id = a2;
}

MATCH_FUNC(0x520d10)
void NetPlay::Disconnect_520D10()
{
    if (field_5CC)
    {
        for (u32 i = field_758_n2.field_4_count; i > 0; i--)
        {
            field_5E4_pDPlay3->DeletePlayerFromGroup(field_758_n2.field_0_group_id, field_758_n2.field_10_players[i].field_10_player_id);
        }
        field_5E4_pDPlay3->DestroyGroup(field_758_n2.field_0_group_id);
    }
    else if (field_758_n2.field_4_count > 1)
    {
        SendKeepAlive_521D20();
    }

    field_5E4_pDPlay3->DestroyPlayer(field_5D8_player_id);
    field_5E4_pDPlay3->Close();
    ClearPlayersAndSession_520DE0(&field_758_n2);
    ClearPlayersAndSession_520DE0(&field_5E8_n1);
    field_8E8_time = 0;
    field_5D4_player_idx = 0xEEEEEEEE;
    field_5D0 = 0xEEEEEEEE;
    field_48 = 1;
}

MATCH_FUNC(0x520de0)
void NetPlay::ClearPlayersAndSession_520DE0(Network_Unknown* pStru)
{
    u32 count = pStru->field_4_count;
    for (u32 i = 0; i < count; i++)
    {
        FreePlayerSlot_5201A0(i, pStru);
    }

    if (pStru->field_120_session_desc.lpszSessionName)
    {
        crt::free(pStru->field_120_session_desc.lpszSessionName);
    }

    memset(&pStru->field_120_session_desc, 0, sizeof(pStru->field_120_session_desc));
}

MATCH_FUNC(0x520e30)
u32 NetPlay::IndexOf_520E30(s32 toFind, Network_Unknown* pObj)
{
    u32 i = 0;
    while (1)
    {
        if (i >= 6)
        {
            break;
        }
        if (pObj->field_10_players[i].field_10_player_id == toFind)
        {
            return i;
        }
        i++;
    }
    return 0xEEEEEEEE;
}

MATCH_FUNC(0x520e60)
void NetPlay::Set9_520E60(s32 pFunc, s32 pParam)
{
    this->field_4C_func_ptrs_and_params[3].field_0_param_fn_callback = (void*)pFunc;
    this->field_4C_func_ptrs_and_params[3].field_4_param_context = (void*)pParam;
    this->field_4C_func_ptrs_and_params[3].field_8_fn_type = 3;
}

MATCH_FUNC(0x520e80)
void NetPlay::Set3_Disconnect_520E80(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[1].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[1].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[1].field_8_fn_type = 1;
}

MATCH_FUNC(0x520ea0)
void NetPlay::NoRefs_null_520EA0()
{
}

MATCH_FUNC(0x520eb0)
void NetPlay::UpdatePlayerPing_520EB0(s32 player_id, s32 ping, Network_Unknown* pStru)
{
    Network_PlayerPing info;

    u32 idx = IndexOf_520E30(player_id, pStru);
    if (idx != 0xEEEEEEEE)
    {
        pStru->field_10_players[idx].field_C += ping;
        pStru->field_10_players[idx].field_4++;
        pStru->field_10_players[idx].field_8 = (u32)pStru->field_10_players[idx].field_C / (u32)pStru->field_10_players[idx].field_4;
        wcscpy(info.field_0_name, pStru->field_10_players[idx].field_1C_player_name);
        info.field_208_avg_ping = pStru->field_10_players[idx].field_8;
        ProcessIncomingPacket_520230(6, (u32)&info);
    }
}

MATCH_FUNC(0x520f50)
void NetPlay::Set18_520F50(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[6].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[6].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[6].field_8_fn_type = 6;
}

// https://decomp.me/scratch/oYBrX
// Result local isn't kept on the stack as in the original, see docs/match_attempts.md
WIP_FUNC(0x520f80)
s32 NetPlay::RemovePlayerByName_520F80(wchar_t* pToRemove)
{
    WIP_IMPLEMENTED;

    s32 bRemoved = 0;
    u32 i = 0;
    while (1)
    {
        if (i >= this->field_758_n2.field_4_count)
        {
            break;
        }
        if (wcscmp(field_758_n2.field_10_players[i].field_1C_player_name, pToRemove) == 0)
        {
            bRemoved = 1;
            field_5E4_pDPlay3->DeletePlayerFromGroup(field_758_n2.field_0_group_id, field_758_n2.field_10_players[i].field_10_player_id);
            break;
        }
        i++;
    }
    return bRemoved;
}

MATCH_FUNC(0x521000)
s32 NetPlay::DeletePlayerFromGroup_521000(u32 idx)
{
    if (idx < 6 && field_758_n2.field_10_players[idx].field_0_in_use)
    {
        s32 idPlayer = field_758_n2.field_10_players[idx].field_10_player_id;
        FreePlayerSlot_5201A0(idx, &field_758_n2);
        field_5E4_pDPlay3->DeletePlayerFromGroup(field_758_n2.field_0_group_id, idPlayer);
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x521060)
s32 NetPlay::SendChatMessage_521060(wchar_t* pMsg, s32 idx_always_m1)
{
    s32 id_to; // edx
    DPCHAT chatMsg; // [esp+0h] [ebp-Ch] BYREF
    memset(&chatMsg, 0, sizeof(chatMsg));

    chatMsg.dwSize = sizeof(DPCHAT);
    chatMsg.lpszMessage = pMsg;
    if (idx_always_m1 < 0)
    {
        id_to = this->field_758_n2.field_0_group_id;
    }
    else
    {
        id_to = this->field_758_n2.field_10_players[idx_always_m1].field_10_player_id;
    }
    return field_5E4_pDPlay3->SendChatMessage(field_5D8_player_id, id_to, 0, &chatMsg);
}

MATCH_FUNC(0x5210d0)
void NetPlay::Set21_5210D0(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[7].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[7].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[7].field_8_fn_type = 7;
}

MATCH_FUNC(0x521100)
void NetPlay::GetPlayerName_521100(wchar_t* Destination, u32 idx)
{
    u32 playerIdx = idx;
    if (idx == 7)
    {
        playerIdx = field_5D4_player_idx;
    }
    else if (field_758_n2.field_4_count <= playerIdx)
    {
        return;
    }
    wcscpy(Destination, field_758_n2.field_10_players[playerIdx].field_1C_player_name);
}

MATCH_FUNC(0x521140)
void NetPlay::Set24_521140(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[8].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[8].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[8].field_8_fn_type = 8;
}

MATCH_FUNC(0x521170)
s32 NetPlay::SetGroupData_521170(Network_8* pObj)
{

    if (field_758_n2.field_118_group_data)
    {
        delete[] field_758_n2.field_118_group_data;
        field_758_n2.field_11C_group_data_len = 0;
    }

    field_758_n2.field_118_group_data = new u8[pObj->field_4_len];
    field_758_n2.field_11C_group_data_len = pObj->field_4_len;

    memcpy(field_758_n2.field_118_group_data, pObj->field_0, pObj->field_4_len);

    return field_5E4_pDPlay3->SetGroupData(field_758_n2.field_0_group_id, field_758_n2.field_118_group_data, field_758_n2.field_11C_group_data_len, 2);
}

MATCH_FUNC(0x5211f0)
void NetPlay::Set27SavePlayerName_5211F0(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[9].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[9].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[9].field_8_fn_type = 9;
}

MATCH_FUNC(0x521220)
void NetPlay::DisableJoining_521220()
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.field_8 = 0;
    pStru.field_D = 0;
    pStru.field_11_len = 0;
    pStru.header.field_0_type = 1;
    pStru.header.field_4_sub_type = 4;
    pStru.field_9 = 1;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    field_5E4_pDPlay3->Send(field_5D8_player_id, field_758_n2.field_0_group_id, DPSEND_GUARANTEED, (void*)pData, dataLen);

    field_758_n2.field_120_session_desc.dwFlags |= DPSESSION_JOINDISABLED;
    if (field_5E4_pDPlay3->SetSessionDesc(&field_758_n2.field_120_session_desc, 0) >= 0)
    {
        field_48 = 2;
    }
}

MATCH_FUNC(0x521330)
void NetPlay::SetExitGameCallBack_521330(s32 pFunc, Game_0x40* pGame)
{
    this->field_4C_func_ptrs_and_params[4].field_0_param_fn_callback = (void*)pFunc;
    this->field_4C_func_ptrs_and_params[4].field_4_param_context = pGame;
    this->field_4C_func_ptrs_and_params[4].field_8_fn_type = 4;
}

MATCH_FUNC(0x521350)
s32 NetPlay::GetMaxPlayers_521350()
{
    s32 maxPlayers = 0;
    for (s32 idx = 0; idx < 6; idx++)
    {
        if (field_758_n2.field_10_players[idx].field_0_in_use)
        {
            maxPlayers++;
        }
    }
    return maxPlayers;
}

MATCH_FUNC(0x521370)
void NetPlay::Send_521370()
{
    u32 pDataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.field_9 = 0;
    pStru.header.field_0_type = 2;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &pDataLen);
    field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, pDataLen);
}

// Return block order differs, see docs/match_attempts.md
WIP_FUNC(0x5213e0)
bool NetPlay::WaitForPlayersSync_5213E0()
{
    WIP_IMPLEMENTED;

    unsigned long senderId;
    s32 pData;
    DWORD startTime;
    unsigned long recvId;
    s32 dataLen;

    startTime = timeGetTime();
    bool bTimedOut = false;
    sub_4DB2E0(gSyncCheckData_6F58E0);
    Send_521E40((s32)gSyncCheckData_6F58E0);
    Send_521370();

    // One bit per other player: waiting for their ack and for their sync data
    u32 waitingForAck = 0;
    u32 waitingForSync = 0;
    for (u32 i = 0; i < 6; i++)
    {
        if (field_758_n2.field_10_players[i].field_0_in_use && i != field_5D4_player_idx)
        {
            waitingForAck |= 1 << i;
            waitingForSync |= 1 << i;
        }
    }

    while (1)
    {
        if (!waitingForSync && !waitingForAck)
        {
            if (!bTimedOut)
            {
                return true;
            }
            return false;
        }

        if (bTimedOut)
        {
            return false;
        }

        if (Receive_51F010(&pData, &dataLen, &recvId, &senderId))
        {
            u8* pPacket = (u8*)pData;
            if (pPacket[0] == 2)
            {
                u32 idx = IndexOf_520E30(senderId, &field_758_n2);
                if (idx == 0xEEEEEEEE)
                {
                    return false;
                }
                waitingForAck &= ~(1 << idx);
            }
            else if (pPacket[0] == 1 && pPacket[3] == 2 && pPacket[5] == 5)
            {
                CompareRemotePlayers_4DB440(gSyncCheckData_6F58E0, pPacket + 5);
                u32 idx = IndexOf_520E30(senderId, &field_758_n2);
                if (idx == 0xEEEEEEEE)
                {
                    return false;
                }
                waitingForSync &= ~(1 << idx);
            }
        }
        bTimedOut = timeGetTime() - startTime > 20000;
    }
}

MATCH_FUNC(0x5215b0)
s32 NetPlay::CopyConnection_5215B0(u32 idx, u32* ppConnection, size_t* pLen)
{
    *ppConnection = 0;
    *pLen = 0;
    if (idx >= (u32)field_30_enumed_connections.field_8_connections_count)
    {
        return 0;
    }

    size_t len = field_30_enumed_connections.field_0_enumed_connections[idx].field_18_connection_len;
    void* pConnection = operator new(len);
    memcpy(pConnection, field_30_enumed_connections.field_0_enumed_connections[idx].field_14_pConnection, len);
    *ppConnection = (u32)pConnection;
    *pLen = len;
    return 1;
}

MATCH_FUNC(0x521630)
s32 NetPlay::SendToPlayer_521630(Network_8* pSendData, s32 idx, char_type a4)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_4_sub_type = 3;
    pStru.header.field_0_type = 1;
    pStru.field_9 = 1;
    pStru.field_D = (s32)pSendData->field_0;
    pStru.field_11_len = pSendData->field_4_len;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    ((char_type*)pData)[1] = field_758_n2.field_8[field_5D4_player_idx] - a4;
    return field_5E4_pDPlay3->Send(field_5D8_player_id, field_758_n2.field_10_players[idx].field_10_player_id, 0, (void*)pData, dataLen);
}

MATCH_FUNC(0x5216e0)
void NetPlay::Add_5216E0(Network_8* pData, s32 id, char_type type)
{
    for (u32 i = 0; i < 48; i++)
    {
        if (field_8F8_packets[i].field_10_used == 1 && field_8F8_packets[i].field_8_id == id &&
            field_8F8_packets[i].field_11_type == type)
        {
            return;
        }
    }

    for (u32 j = 0; j < 48; j++)
    {
        if (!field_8F8_packets[j].field_10_used)
        {
            field_8F8_packets[j].field_0_inputs = *(Network_InputData_0x8*)pData->field_0;
            field_8F8_packets[j].field_C = pData->field_4_len;
            field_8F8_packets[j].field_8_id = id;
            field_8F8_packets[j].field_11_type = type;
            field_8F8_packets[j].field_10_used = 1;
            field_CB8_count++;
            return;
        }
    }
}

MATCH_FUNC(0x521770)
u32 NetPlay::sub_521770(Network_8* pOut, char_type* pSeq, u32* pPlayerId)
{
#pragma warning(push)
#pragma warning(disable : 4700) // best_seq/best_idx are read uninitialised in the original too
    u8 best_seq;
    u32 best_idx;
    char_type bFound = 0;
    for (u32 i = 0; i < 48; i++)
    {
        if (field_8F8_packets[i].field_10_used == 1)
        {
            if (!bFound)
            {
                best_seq = field_8F8_packets[i].field_11_type;
                best_idx = i;
                bFound = 1;
            }
            else if (SeqDiff(field_8F8_packets[i].field_11_type, best_seq) < 0)
            {
                best_seq = field_8F8_packets[i].field_11_type;
                best_idx = i;
            }
        }
    }

    if (bFound)
    {
        pOut->field_4_len = field_8F8_packets[best_idx].field_C;
        pOut->field_0 = &field_8F8_packets[best_idx];
        *pPlayerId = field_8F8_packets[best_idx].field_8_id;
        *pSeq = best_seq;
        return best_idx;
    }
    return -1;
#pragma warning(pop)
}

MATCH_FUNC(0x521820)
void NetPlay::sub_521820(s32** a2, s32 idx)
{
    s32* p = *a2;
    if (*p == -1)
    {
        *p = dword_6F8A4C[idx];
    }
    else
    {
        dword_6F8A4C[idx] = *p;
    }
    byte_6F8A64[idx] = field_758_n2.field_8[idx];
}

MATCH_FUNC(0x521870)
void NetPlay::Remove_521870(s32 idx)
{
    field_CB8_count--;
    field_8F8_packets[idx].field_10_used = 0;
}

// Stack frame and registers differ, see docs/match_attempts.md
WIP_FUNC(0x521890)
char_type NetPlay::ReceiveGameMessage_521890(Network_8* pOut, s32* pPlayerIdx, u32* pType)
{
    WIP_IMPLEMENTED;

    char_type bGotMessage;
    char_type bCheckBuffered;
    char_type seq;
    s32 pData;
    unsigned long senderId;
    unsigned long recvId;
    s32 dataLen;

    bCheckBuffered = 1;
    bGotMessage = 0;
    bool bTimedOut = false;
    DWORD startTime = timeGetTime();

    while (1)
    {
        if (bTimedOut || field_8F0)
        {
            break;
        }

        if (bCheckBuffered)
        {
            // Take the oldest buffered (out of order) packet
            u32 slot = sub_521770(pOut, &seq, (u32*)pPlayerIdx);
            if (slot != -1)
            {
                *pType = 3;
                bGotMessage = 1;
                s32 diff = SeqDiff(seq, field_758_n2.field_8[*pPlayerIdx]);
                s32 diffLocal = SeqDiff(seq, field_758_n2.field_8[field_5D4_player_idx]);
                if (diff < 0)
                {
                    bGotMessage = 0;
                    Remove_521870(slot);
                }
                else if (diff == 0 && diffLocal < 0)
                {
                    Remove_521870(slot);
                    sub_521820((s32**)pOut, *pPlayerIdx);
                    field_758_n2.field_8[*pPlayerIdx] = ((u8)field_758_n2.field_8[*pPlayerIdx] + 1) % 256;
                }
                else
                {
                    bGotMessage = 0;
                    bCheckBuffered = 0;
                }
            }
            else
            {
                bGotMessage = 0;
                bCheckBuffered = 0;
            }
        }
        else if (Receive_51F010(&pData, &dataLen, &recvId, &senderId))
        {
            u8* pPacket = (u8*)pData;
            *pType = pPacket[3];
            *pPlayerIdx = IndexOf_520E30(senderId, &field_758_n2);
            if (*pPlayerIdx != 0xEEEEEEEE)
            {
                bGotMessage = 1;
                pOut->field_4_len = pPacket[4];
                pOut->field_0 = pPacket + 5;
                if (*pType == 3)
                {
                    seq = pPacket[1];
                    s32 diff = SeqDiff(seq, field_758_n2.field_8[*pPlayerIdx]);
                    s32 diffLocal = SeqDiff(seq, field_758_n2.field_8[field_5D4_player_idx]);
                    if (diff < 0)
                    {
                        bGotMessage = 0;
                    }
                    else if (diff == 0 && diffLocal < 0)
                    {
                        sub_521820((s32**)pOut, *pPlayerIdx);
                        field_758_n2.field_8[*pPlayerIdx] = ((u8)field_758_n2.field_8[*pPlayerIdx] + 1) % 256;
                    }
                    else
                    {
                        // Not the next one in sequence yet: buffer it
                        Add_5216E0(pOut, *pPlayerIdx, seq);
                        bGotMessage = 0;
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

    field_8F4_time_diff = timeGetTime() - startTime;
    return bGotMessage;
}

MATCH_FUNC(0x521b20)
void NetPlay::SendToAll_521B20(Network_8* pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_4_sub_type = 3;
    pStru.header.field_0_type = 1;
    pStru.field_8 = field_758_n2.field_8[field_5D4_player_idx];
    pStru.field_9 = 1;
    pStru.field_D = (s32)pSendData->field_0;
    pStru.field_11_len = pSendData->field_4_len;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
    const s32 player_idx = field_5D4_player_idx;
    field_758_n2.field_8[player_idx] = ((u8)field_758_n2.field_8[player_idx] + 1) % 256;
}

MATCH_FUNC(0x521be0)
s32 NetPlay::NoRefs_Send_521BE0(Network_8* pSendData, s32 a3)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_4_sub_type = 3;
    pStru.header.field_0_type = 1;
    pStru.field_8 = field_758_n2.field_8[field_5D4_player_idx] - (char_type)a3;
    pStru.field_9 = 1;
    pStru.field_D = (s32)pSendData->field_0;
    pStru.field_11_len = pSendData->field_4_len;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    return field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
}

MATCH_FUNC(0x521c80)
s32 NetPlay::NoRefs_Send_521C80(s32 pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_0_type = 1;
    pStru.header.field_4_sub_type = 2;
    pStru.field_9 = 1;
    pStru.field_D = pSendData;
    pStru.field_11_len = 0xE;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    ((char_type*)pData)[1] = field_758_n2.field_8[field_5D4_player_idx];
    return field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
}

// Store scheduling differs, see docs/match_attempts.md
WIP_FUNC(0x521d20)
s32 NetPlay::SendKeepAlive_521D20()
{
    WIP_IMPLEMENTED;

    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;
    char_type keep_alive;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_4_sub_type = 2;
    keep_alive = 2;
    pStru.header.field_0_type = 1;
    pStru.field_9 = 1;
    pStru.field_11_len = 1;
    pStru.field_D = (s32)&keep_alive;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    return field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
}

MATCH_FUNC(0x521db0)
s32 NetPlay::Send_521DB0(s32 value)
{
    Packet_Byte_S32 payload;
    payload.field_0 = 1;
    payload.field_1 = value;

    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_0_type = 1;
    pStru.field_9 = 1;
    pStru.field_D = (s32)&payload;
    pStru.header.field_4_sub_type = 2;
    pStru.field_11_len = 5;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    return field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
}

MATCH_FUNC(0x521e40)
s32 NetPlay::Send_521E40(s32 pSendData)
{
    u32 dataLen;
    s32 pData;
    Packet_SubType_3 pStru;

    memset(&pStru, 0, sizeof(pStru));
    pStru.header.field_0_type = 1;
    pStru.header.field_4_sub_type = 2;
    pStru.field_9 = 1;
    pStru.field_D = pSendData;
    pStru.field_11_len = 0x1F;
    NetPlay::MakeSendData_51F420(&pStru, &pData, &dataLen);
    return field_5E4_pDPlay3->Send(field_5D8_player_id, 0, 0, (void*)pData, dataLen);
}

// The atexit destructor VC6 generates for gNetPlay_7071E8, written out
MATCH_FUNC(0x5e4dd0)
void NetPlay::static_dtor_5E4DD0()
{
    gNetPlay_7071E8.NetPlay::~NetPlay();
}