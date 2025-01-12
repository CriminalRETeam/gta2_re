#include "goofy_thompson.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"

EXPORT_VAR goofy_thompson gGoofy_thompson_7071E8;
GLOBAL(gGoofy_thompson_7071E8, 0x7071E8);

STUB_FUNC(0x51d6b0)
goofy_thompson* goofy_thompson::ctor_51D6B0()
{
    return 0;
}

STUB_FUNC(0x51d7b0)
void* goofy_thompson::vdtor_51D7B0(char_type flags)
{
    return 0;
}

STUB_FUNC(0x51d7d0)
goofy_thompson::~goofy_thompson()
{
}

STUB_FUNC(0x51d930)
void goofy_thompson::AddEnumeratedConnection_51D930(EnumeratedConnection* pConnectionInfo)
{
}

STUB_FUNC(0x51da30)
s32 goofy_thompson::EnumConnections_cb_51DA30(GUID* lpguidSP,
                                              const void* lpConnection,
                                              u32 dwConnectionSize,
                                              DPNAME* lpName,
                                              s32 dwFlags,
                                              goofy_thompson* lpContext)
{
    return 0;
}

STUB_FUNC(0x51dae0)
s32 goofy_thompson::SetProtoAndConnection_51DAE0(GUID* pProtocolGuid, s32 pUseThisConnection)
{
    return 0;
}

MATCH_FUNC(0x51dc90)
void goofy_thompson::DirectPlayDestroy_51DC90()
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
s32 goofy_thompson::DirectPlayCreate_51DCD0()
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
s32 goofy_thompson::DirectPlayCreate_51DED0()
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
                FAILED(field_5E0_pDPlayLobby2->EnumAddress(goofy_thompson::sub_51E030, pPlayerAddressBuf, playerAddressLen, this)))
            {
                return 0;
            }
            delete[] pPlayerAddressBuf;
        }
        pDirectPlay3->Release();
    }

    return 1;
}

STUB_FUNC(0x51e030)
BOOL goofy_thompson::sub_51E030(const GUID& guidDataType, DWORD dwDataSize, LPCVOID lpData, LPVOID lpContext)
{
    return 0;
}

STUB_FUNC(0x51e0e0)
s32 goofy_thompson::sub_51E0E0(wchar_t* Source)
{
    return 0;
}

STUB_FUNC(0x51e140)
s32 goofy_thompson::NoRefs_51E140(wchar_t* String, s32* a3, size_t* a4)
{
    return 0;
}

STUB_FUNC(0x51e2b0)
s32 goofy_thompson::NoRefs_51E2B0(wchar_t* Source, wchar_t* a3, s32* a4, size_t* a5)
{
    return 0;
}

STUB_FUNC(0x51e450)
s32 goofy_thompson::NoRefs_51E450(s32 a2, u32* a3, size_t* a4)
{
    return 0;
}

STUB_FUNC(0x51e5c0)
s32 goofy_thompson::sub_51E5C0()
{
    return 0;
}

STUB_FUNC(0x51e650)
s32 goofy_thompson::sub_51E650()
{
    return 0;
}

STUB_FUNC(0x51e7a0)
s32 goofy_thompson::sub_51E7A0(wchar_t* Source, wchar_t* a3, s32 a4, s32* a5)
{
    return 0;
}

STUB_FUNC(0x51e9c0)
u32 goofy_thompson::sub_51E9C0(s32 a1, s32 a2, s32 a3, s32 a4, wchar_t* Source, s32 a6, s32 a7)
{
    return 0;
}

STUB_FUNC(0x51eae0)
s32 goofy_thompson::EnumSessions_cb_51EAE0(DPSESSIONDESC2* lpThisSD, s32 lpDwTimeOut, char_type dwFlags, goofy_thompson* lpContext)
{
    return 0;
}

STUB_FUNC(0x51eb00)
s32 goofy_thompson::AddEnumeratedSession_51EB00(DPSESSIONDESC2* pSession)
{
    return 0;
}

STUB_FUNC(0x51ecd0)
void goofy_thompson::sub_51ECD0(s32 pFunc, Network_20324* pParam)
{
}

STUB_FUNC(0x51ed00)
void goofy_thompson::sub_51ED00()
{
}

STUB_FUNC(0x51ef60)
s32 goofy_thompson::Send_51EF60()
{
    return 0;
}

STUB_FUNC(0x51f010)
char_type goofy_thompson::Receive_51F010(s32* pOutData, s32* pOutDataLen, s32* recvId, u32* senderId)
{
    return 0;
}

STUB_FUNC(0x51f0d0)
void goofy_thompson::sub_51F0D0(char_type* a2, s32 a3, s32 a4, s32 a5)
{
}

STUB_FUNC(0x51f110)
void goofy_thompson::sub_51F110(s32 a2, s32 a3, s32 a4, s32 a5)
{
}

STUB_FUNC(0x51f210)
s32 goofy_thompson::CalcPacketLen_51F210(u32 pPacket)
{
    return 0;
}

STUB_FUNC(0x51f420)
void goofy_thompson::MakeSendData_51F420(Packet_SubType_3* pPacket, s32* pData, u32* pDataLen)
{
}

STUB_FUNC(0x51f870)
void goofy_thompson::sub_51F870(char_type* pPacket, s32 packetLen, s32 recvId, s32 a5)
{
}

STUB_FUNC(0x520040)
s32 goofy_thompson::sub_520040(s32 toFind, Network_Unknown* pStru, s32 a3, u32* pOutIdx)
{
    return 0;
}

STUB_FUNC(0x5201a0)
void goofy_thompson::sub_5201A0(s32 idx, Network_Unknown* pStru)
{
}

STUB_FUNC(0x520230)
s32 goofy_thompson::sub_520230(s32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x520530)
void goofy_thompson::sub_520530(s32 pFunc, s32 pParam)
{
}

STUB_FUNC(0x520570)
s32 goofy_thompson::sub_520570(int session_idx, wchar_t* a3, int* a4, s32* a5)
{
    return 0;
}

MATCH_FUNC(0x520c20)
s32 goofy_thompson::EnumGroups_cb_520C20(s32 a1, s32 a2, s32 a3, char_type a4, goofy_thompson* pContext)
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
        return pContext->sub_520CA0(a1, a3);
    }

    return 1;
}

STUB_FUNC(0x520ca0)
s32 goofy_thompson::sub_520CA0(s32 a2, s32 a3)
{
    return 0;
}

MATCH_FUNC(0x520d00)
void goofy_thompson::sub_520D00(s32 a2)
{
    field_758_n2.field_0_group_id = a2;
}

STUB_FUNC(0x520d10)
void goofy_thompson::sub_520D10()
{
}

MATCH_FUNC(0x520de0)
void goofy_thompson::sub_520DE0(Network_Unknown* pStru)
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
u32 goofy_thompson::IndexOf_520E30(s32 toFind, Network_Unknown* pObj)
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
void goofy_thompson::sub_520E60(s32 pFunc, s32 pParam)
{
    field_4C_func_ptrs_and_params[9] = pFunc;
    field_4C_func_ptrs_and_params[10] = pParam;
    field_4C_func_ptrs_and_params[11] = 3;
}

MATCH_FUNC(0x520e80)
void goofy_thompson::sub_520E80(s32 a2, s32 a3)
{
    field_4C_func_ptrs_and_params[3] = a2;
    field_4C_func_ptrs_and_params[4] = a3;
    field_4C_func_ptrs_and_params[5] = 1;
}

STUB_FUNC(0x520ea0)
void goofy_thompson::NoRefs_null_520EA0()
{
}

STUB_FUNC(0x520eb0)
s32 goofy_thompson::sub_520EB0(s32 a2, s32 a3, Network_Unknown* a4)
{
    return 0;
}

MATCH_FUNC(0x520f50)
void goofy_thompson::sub_520F50(s32 a2, s32 a3)
{
    field_4C_func_ptrs_and_params[18] = a2;
    field_4C_func_ptrs_and_params[19] = a3;
    field_4C_func_ptrs_and_params[20] = 6;
}

STUB_FUNC(0x520f80)
s32 goofy_thompson::sub_520F80(wchar_t* String2)
{
    return 0;
}

STUB_FUNC(0x521000)
s32 goofy_thompson::DeletePlayerFromGroup_521000(u32 idx)
{
    return 0;
}

// https://decomp.me/scratch/dSpd7
STUB_FUNC(0x521060)
s32 goofy_thompson::SendChatMessage_521060(wchar_t* pMsg, s32 idx_always_m1)
{
    s32 id_to; // edx
    DPCHAT chatMsg; // [esp+0h] [ebp-Ch] BYREF

    chatMsg.dwSize = sizeof(DPCHAT);
    chatMsg.dwFlags = 0;
    chatMsg.lpszMessage = pMsg;
    if (idx_always_m1 >= 0)
    {
        id_to = this->field_758_n2.field_10[idx_always_m1].field_10;
    }
    else
    {
        id_to = this->field_758_n2.field_0_group_id;
    }
    return field_5E4_pDPlay3->SendChatMessage(field_5D8_player_id, id_to, 0, &chatMsg);
}

MATCH_FUNC(0x5210d0)
void goofy_thompson::sub_5210D0(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[21] = a2;
    this->field_4C_func_ptrs_and_params[22] = a3;
    this->field_4C_func_ptrs_and_params[23] = 7;
}

MATCH_FUNC(0x521100)
void goofy_thompson::GetPlayerName_521100(wchar_t* Destination, u32 idx)
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
void goofy_thompson::sub_521140(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[24] = a2;
    this->field_4C_func_ptrs_and_params[25] = a3;
    this->field_4C_func_ptrs_and_params[26] = 8;
}

MATCH_FUNC(0x521170)
s32 goofy_thompson::sub_521170(Network_8* pObj)
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
void goofy_thompson::sub_5211F0(s32 a2, s32 a3)
{
    this->field_4C_func_ptrs_and_params[27] = a2;
    this->field_4C_func_ptrs_and_params[28] = a3;
    this->field_4C_func_ptrs_and_params[29] = 9;
}

STUB_FUNC(0x521220)
void goofy_thompson::sub_521220()
{
}

MATCH_FUNC(0x521330)
void goofy_thompson::SetExitGameCallBack_521330(s32 pFunc, Game_0x40* pGame)
{
    // TODO: fix meme types
    this->field_4C_func_ptrs_and_params[12] = pFunc;
    this->field_4C_func_ptrs_and_params[13] = (int)pGame;
    this->field_4C_func_ptrs_and_params[14] = 4;
}

MATCH_FUNC(0x521350)
s32 goofy_thompson::GetMaxPlayers_521350()
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

// https://decomp.me/scratch/tpDuw
STUB_FUNC(0x521370)
s32 goofy_thompson::Send_521370()
{
    return 0;
}

STUB_FUNC(0x5213e0)
bool goofy_thompson::sub_5213E0()
{
    return 0;
}

STUB_FUNC(0x5215b0)
s32 goofy_thompson::NoRefs_5215B0(u32 a2, u32* a3, size_t* a4)
{
    return 0;
}

STUB_FUNC(0x521630)
s32 goofy_thompson::Send_521630(s32 pData, s32 idx, char_type a4)
{
    return 0;
}

STUB_FUNC(0x5216e0)
void goofy_thompson::Add_5216E0(s32 a2, s32** a3, s32 a4, char_type a5)
{
}

STUB_FUNC(0x521770)
u32 goofy_thompson::sub_521770(u32* a2, char_type* a3, u32* a4)
{
    return 0;
}

STUB_FUNC(0x521820)
s32 goofy_thompson::sub_521820(s32** a2, s32 idx)
{
    return 0;
}

MATCH_FUNC(0x521870)
void goofy_thompson::Remove_521870(s32 idx)
{
    field_CB8_count--;
    field_900_208_start[idx].field_8 = 0;
}

STUB_FUNC(0x521890)
char_type goofy_thompson::sub_521890(s32** a3, s32* arg4, u32* a4)
{
    return 0;
}

STUB_FUNC(0x521b20)
void goofy_thompson::Send_521B20(s32* a2)
{
}

STUB_FUNC(0x521be0)
s32 goofy_thompson::NoRefs_Send_521BE0(s32* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x521c80)
s32 goofy_thompson::NoRefs_Send_521C80(s32 a2)
{
    return 0;
}

STUB_FUNC(0x521d20)
s32 goofy_thompson::Send_521D20()
{
    return 0;
}

STUB_FUNC(0x521db0)
s32 goofy_thompson::Send_521DB0(s32 dataLen)
{
    return 0;
}

STUB_FUNC(0x521e40)
s32 goofy_thompson::Send_521E40(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5e4dd0)
void goofy_thompson::static_dtor_5E4DD0()
{
}
