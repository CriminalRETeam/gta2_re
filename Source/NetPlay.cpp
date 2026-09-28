#include "NetPlay.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"

DEFINE_GLOBAL(NetPlay, gNetPlay_7071E8, 0x7071E8);
DEFINE_GLOBAL(GUID, kGta2_DP_Guid_5FE928, 0x5FE928);
DEFINE_GLOBAL_ARRAY(s32, dword_6F8A4C, 6, 0x6F8A4C);
DEFINE_GLOBAL_ARRAY(char_type, byte_6F8A64, 24, 0x6F8A64);

MATCH_FUNC(0x51d6b0)
NetPlay::NetPlay()
{
    field_CB8_count = 0;
    for (s32 i = 0; i < 48; i++)
    {
        field_8F8_packets[i].field_10_used = 0;
    }

    field_4 = 0;
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

STUB_FUNC(0x51d7b0)
void* NetPlay::vdtor_51D7B0(char_type flags)
{
    NOT_IMPLEMENTED;
    return 0;
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
        for (u32 i = 0; i < field_30_enumed_connections.field_8_connections_count; i++)
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
        operator delete(field_758_n2.field_10[j].field_1C);
        operator delete((void*)field_758_n2.field_10[j].field_24);
    }

    operator delete(field_758_n2.field_118);
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
            field_4 = 1;
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
                FAILED(field_5E0_pDPlayLobby2->EnumAddress(NetPlay::sub_51E030, pPlayerAddressBuf, playerAddressLen, this)))
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
BOOL NetPlay::sub_51E030(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext)
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
s32 NetPlay::NoRefs_51E140(wchar_t* pIpAddress, s32* ppAddress, size_t* pAddressLen)
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
s32 NetPlay::NoRefs_51E2B0(wchar_t* pPhoneNumber, wchar_t* pModemName, s32* ppAddress, size_t* pAddressLen)
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
s32 NetPlay::NoRefs_51E450(DPCOMPORTADDRESS* pComPort, u32* ppAddress, size_t* pAddressLen)
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
s32 NetPlay::sub_51E5C0()
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
            if (field_4 || sub_51E650() != -1)
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
s32 NetPlay::sub_51E650()
{
    WIP_IMPLEMENTED;

    MSG msg;
    DPSESSIONDESC2 desc;

    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.guidApplication = kGta2_DP_Guid_5FE928;

    HRESULT hr;
    if (field_4)
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

STUB_FUNC(0x51e7a0)
s32 NetPlay::sub_51E7A0(wchar_t* Source, wchar_t* a3, s32 a4, s32* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x51e9c0)
u32 NetPlay::sub_51E9C0(Network_8* pData, s32 player_id, DPNAME name, Network_Unknown* pStru)
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

            if (!pStru->field_10[i].field_0)
            {
                memset(&pStru->field_10[i], 0, sizeof(Nework_2C));
                pStru->field_10[i].field_10 = player_id;
                memset(&pStru->field_10[i].field_14, 0, sizeof(DPNAME));

                if (wcslen(name.lpszShortName) < 15)
                {
                    pStru->field_10[i].field_1C = (wchar_t*)operator new(2 * wcslen(name.lpszShortName) + 2);
                    wcscpy(pStru->field_10[i].field_1C, name.lpszShortName);
                }
                else
                {
                    pStru->field_10[i].field_1C = (wchar_t*)operator new(0x20);
                    wcsncpy(pStru->field_10[i].field_1C, name.lpszShortName, 15);
                    pStru->field_10[i].field_1C[15] = 0;
                }

                pStru->field_10[i].field_24 = (s32)operator new(pData->field_4_len);
                pStru->field_10[i].field_28 = pData->field_4_len;
                memcpy((void*)pStru->field_10[i].field_24, pData->field_0, pData->field_4_len);
                pStru->field_10[i].field_0 = 1;
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

STUB_FUNC(0x51ed00)
void NetPlay::NetworkTick_51ED00()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x51ef60)
s32 NetPlay::Send_51EF60()
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

WIP_FUNC(0x51f010)
char_type NetPlay::Receive_51F010(s32* pOutData, s32* pOutDataLen, unsigned long* recvId, unsigned long* senderId)
{
    WIP_IMPLEMENTED;

    unsigned long readLen = 0x1800;
    if (field_5E4_pDPlay3->Receive(senderId, recvId, 1, (void*)field_8E4_p0x1800_1, &readLen))
    {
        return 0;
    }

    while (!*senderId)
    {
        OnPacketReceived_51F870((char*)this->field_8E4_p0x1800_1, readLen, *recvId, 0);
        if (!this->field_8F0)
        {
            readLen = 0x1800;
            if (!field_5E4_pDPlay3->Receive(senderId, recvId, 1, (void*)field_8E4_p0x1800_1, &readLen))
            {
                continue;
            }
        }
        return 0;
    }

    s32 outDataLen = CalcPacketLen_51F210(this->field_8E4_p0x1800_1);
    *pOutData = this->field_8E4_p0x1800_1;
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

STUB_FUNC(0x51f110)
void NetPlay::ProcessPingOrHandshakeSend_51F110(void* pPacket, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x51f210)
s32 NetPlay::CalcPacketLen_51F210(u32 pPacket)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x51f420)
void NetPlay::MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x51f870)
void NetPlay::OnPacketReceived_51F870(void* pPacket, s32 packetLen, s32 recvId, s32 a5)
{
    NOT_IMPLEMENTED;
}

// Register allocation differs, see docs/match_attempts.md
WIP_FUNC(0x520040)
s32 NetPlay::sub_520040(s32 toFind, Network_Unknown* pStru, Network_Unknown* pDst, u32* pOutIdx)
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
        if (pStru->field_10[i].field_0 && pStru->field_10[i].field_10 == toFind)
        {
            u32 new_idx = sub_51E9C0((Network_8*)&pStru->field_10[i].field_24,
                                     pStru->field_10[i].field_10,
                                     *(DPNAME*)&pStru->field_10[i].field_14,
                                     pDst);
            if (new_idx != 0xEEEEEEEE)
            {
                *pOutIdx = new_idx;
                sub_5201A0(i, pStru);
                return 1;
            }
            break;
        }
        i++;
    }
    return 0;
}

MATCH_FUNC(0x5201a0)
void NetPlay::sub_5201A0(s32 idx, Network_Unknown* pStru)
{
    delete pStru->field_10[idx].field_1C;
    delete (void*)pStru->field_10[idx].field_24;
    memset(&pStru->field_10[idx], 0, sizeof(Nework_2C));
    pStru->field_10[idx].field_0 = 0;
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

STUB_FUNC(0x520570)
s32 NetPlay::sub_520570(int session_idx, wchar_t* a3, int* a4, s32* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x520c20)
s32 NetPlay::EnumGroups_cb_520C20(s32 a1, s32 a2, s32 a3, char_type a4, NetPlay* pContext)
{
    if ((a4 & 1) != 0)
    {
        return 0;
    }

    if (!a2)
    {
        pContext->sub_520D00(a1);
        return 1;
    }

    if (a2 == 1)
    {
        return pContext->sub_520CA0(a1, (DPNAME*)a3);
    }

    return 1;
}

MATCH_FUNC(0x520ca0)
s32 NetPlay::sub_520CA0(s32 player_id, DPNAME* pName)
{
    Network_8 data;
    data.field_0 = 0;
    data.field_4_len = 0;
    return sub_51E9C0(&data, player_id, *pName, &field_758_n2) != 0xEEEEEEEE;
}

MATCH_FUNC(0x520d00)
void NetPlay::sub_520D00(s32 a2)
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
            field_5E4_pDPlay3->DeletePlayerFromGroup(field_758_n2.field_0_group_id, field_758_n2.field_10[i].field_10);
        }
        field_5E4_pDPlay3->DestroyGroup(field_758_n2.field_0_group_id);
    }
    else if (field_758_n2.field_4_count > 1)
    {
        SendKeepAlive_521D20();
    }

    field_5E4_pDPlay3->DestroyPlayer(field_5D8_player_id);
    field_5E4_pDPlay3->Close();
    sub_520DE0(&field_758_n2);
    sub_520DE0(&field_5E8_n1);
    field_8E8_time = 0;
    field_5D4_player_idx = 0xEEEEEEEE;
    field_5D0 = 0xEEEEEEEE;
    field_48 = 1;
}

MATCH_FUNC(0x520de0)
void NetPlay::sub_520DE0(Network_Unknown* pStru)
{
    u32 count = pStru->field_4_count;
    for (u32 i = 0; i < count; i++)
    {
        sub_5201A0(i, pStru);
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
        if (pObj->field_10[i].field_10 == toFind)
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

STUB_FUNC(0x520eb0)
s32 NetPlay::sub_520EB0(s32 a2, s32 a3, Network_Unknown* a4)
{
    NOT_IMPLEMENTED;
    return 0;
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
        if (wcscmp(field_758_n2.field_10[i].field_1C, pToRemove) == 0)
        {
            bRemoved = 1;
            field_5E4_pDPlay3->DeletePlayerFromGroup(field_758_n2.field_0_group_id, field_758_n2.field_10[i].field_10);
            break;
        }
        i++;
    }
    return bRemoved;
}

MATCH_FUNC(0x521000)
s32 NetPlay::DeletePlayerFromGroup_521000(u32 idx)
{
    if (idx < 6 && field_758_n2.field_10[idx].field_0)
    {
        s32 idPlayer = field_758_n2.field_10[idx].field_10;
        sub_5201A0(idx, &field_758_n2);
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
        id_to = this->field_758_n2.field_10[idx_always_m1].field_10;
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
    wcscpy(Destination, field_758_n2.field_10[playerIdx].field_1C);
}

MATCH_FUNC(0x521140)
void NetPlay::Set24_521140(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[8].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[8].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[8].field_8_fn_type = 8;
}

MATCH_FUNC(0x521170)
s32 NetPlay::sub_521170(Network_8* pObj)
{

    if (field_758_n2.field_118)
    {
        delete[] field_758_n2.field_118;
        field_758_n2.field_11C = 0;
    }

    field_758_n2.field_118 = new u8[pObj->field_4_len];
    field_758_n2.field_11C = pObj->field_4_len;

    memcpy(field_758_n2.field_118, pObj->field_0, pObj->field_4_len);

    return field_5E4_pDPlay3->SetGroupData(field_758_n2.field_0_group_id, field_758_n2.field_118, field_758_n2.field_11C, 2);
}

MATCH_FUNC(0x5211f0)
void NetPlay::Set27SavePlayerName_5211F0(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[9].field_0_param_fn_callback = (void*)a2;
    this->field_4C_func_ptrs_and_params[9].field_4_param_context = (void*)a3;
    this->field_4C_func_ptrs_and_params[9].field_8_fn_type = 9;
}

MATCH_FUNC(0x521220)
void NetPlay::sub_521220()
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
        if (field_758_n2.field_10[idx].field_0)
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

STUB_FUNC(0x5213e0)
bool NetPlay::sub_5213E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5215b0)
s32 NetPlay::NoRefs_5215B0(u32 idx, u32* ppConnection, size_t* pLen)
{
    *ppConnection = 0;
    *pLen = 0;
    if (idx >= field_30_enumed_connections.field_8_connections_count)
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
s32 NetPlay::Send_521630(Network_8* pSendData, s32 idx, char_type a4)
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
    return field_5E4_pDPlay3->Send(field_5D8_player_id, field_758_n2.field_10[idx].field_10, 0, (void*)pData, dataLen);
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

STUB_FUNC(0x521770)
u32 NetPlay::sub_521770(u32* a2, char_type* a3, u32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x521890)
char_type NetPlay::sub_521890(s32** a3, s32* arg4, u32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x521b20)
void NetPlay::Send_521B20(Network_8* pSendData)
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

STUB_FUNC(0x5e4dd0)
void NetPlay::static_dtor_5E4DD0()
{
    NOT_IMPLEMENTED;
}