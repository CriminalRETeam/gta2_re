#include "BurgerKing_67F8B0.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "input.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include <io.h>

#define ATTRACT_COUNT 3

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

DEFINE_GLOBAL(BurgerKing_67F8B0, gBurgerKing_67F8B0, 0x67F8B0);
DEFINE_GLOBAL(BurgerKing_1*, gBurgerKing_1_67B990, 0x67B990);
DEFINE_GLOBAL(DWORD, gKeyboardStatus_67B624, 0x67B624);
DEFINE_GLOBAL(u8, byte_67B80C, 0x67B80C);
DEFINE_GLOBAL(bool, gNeedKbAcquire_67B66C, 0x67B66C);
DEFINE_GLOBAL(DIDEVICEOBJECTDATA, stru_67B610, 0x67B610);
DEFINE_GLOBAL(s32, dword_67B5B0, 0x67B5B0);
DEFINE_GLOBAL(s32, dword_67B5B4, 0x67B5B4);
DEFINE_GLOBAL(s32, dword_67B5B8, 0x67B5B8);
DEFINE_GLOBAL(s32, dword_67B5BC, 0x67B5BC);

DEFINE_GLOBAL_ARRAY(s32, dword_61A9E4, 12, 0x61A9E4);
DEFINE_GLOBAL_ARRAY(s32, dword_67B91C, 12, 0x67B91C);
DEFINE_GLOBAL_ARRAY(s32, dword_67B6E8, 12, 0x67B6E8);

EXTERN_GLOBAL(DIDATAFORMAT, gKeyboardDataFormat_601A54);
EXTERN_GLOBAL(HINSTANCE, gHInstance_708220);

DEFINE_GUID(GUID_SysKeyboard, 0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

const AttractFile attractFiles_62083C[ATTRACT_COUNT] = {"data\\attract\\attr1.rep", "data\\attract\\attr2.rep", "data\\attract\\attr3.rep"};

// TODO: Move
EXPORT int __stdcall FatalDXError_4A3CF0(HRESULT hr, const char* pSourceFile, int lineNo)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x498910)
EXPORT BOOL CALLBACK DirectInputDeviceEnumCallBack_498910(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    NOT_IMPLEMENTED;
    return FALSE;
}

MATCH_FUNC(0x4987A0)
void BurgerKing_1::free_input_devices_4987A0()
{
    if (gpDInput_67B804)
    {
        if (gKeyboardDevice_67B5C0)
        {
            gKeyboardDevice_67B5C0->Unacquire();
            gKeyboardDevice_67B5C0->Release();
            gKeyboardDevice_67B5C0 = 0;
        }

        if (gGamePadDevice_67B6C0)
        {
            gGamePadDevice_67B6C0->Unacquire();
            gGamePadDevice_67B6C0->Release();
            gGamePadDevice_67B6C0 = 0;
        }
    }
}

MATCH_FUNC(0x498CC0)
void BurgerKing_1::read_keyboard_and_gamepad_498CC0()
{
    gKeyboardStatus_67B624 = -1;
    if (gKeyboardDevice_67B5C0)
    {
        gKeyboardDevice_67B5C0->GetDeviceData(16, 0, &gKeyboardStatus_67B624, 0);
    }

    gKeyboardStatus_67B624 = -1;
    if (gGamePadDevice_67B6C0)
    {
        gGamePadDevice_67B6C0->GetDeviceData(16, 0, &gKeyboardStatus_67B624, 0);
    }
}

MATCH_FUNC(0x498C00)
void BurgerKing_1::get_registry_controls_498C00()
{
    for (u32 i = 0; i < 12; ++i)
    {
        const u32 v1 = gRegistry_6FF968.Set_Control_Setting_587010(i, dword_61A9E4[i]);
        dword_67B91C[i] = v1 >> 15;
        dword_67B6E8[i] = (u8)v1;
    }
}

STUB_FUNC(0x4989C0)
void BurgerKing_1::set_game_pad_device_properties_4989C0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x498BA0)
bool BurgerKing_1::game_pads_init_498BA0()
{
    if (gpDInput_67B804)
    {
#if defined(__clang__) || (_MSC_VER <= 1200)
    #pragma comment(lib, "DInput.lib")
        // VC6-compatible path
        if (FAILED(DirectInputCreateA(gHInstance_708220, 0x700, &gpDInput_67B804, 0)))
        {
            return 1;
        }
#else
    #pragma comment(lib, "DInput8.lib")
        // Runtime dynamic loading path
        HMODULE hDx = LoadLibrary("DInput.dll");
        if (hDx)
        {
            FARPROC p = GetProcAddress(hDx, "DirectInputCreateA");
            if (p)
            {
                auto pDirectInputCreateA = reinterpret_cast<decltype(&DirectInputCreateA)>(p);
                if (FAILED(pDirectInputCreateA(gHInstance_708220, 0x700, &gpDInput_67B804, 0)))
                {
                    return 1;
                }
            }
        }
#endif
    }

    gpDInput_67B804->EnumDevices(DIDEVTYPE_JOYSTICK, DirectInputDeviceEnumCallBack_498910, this, DIEDFL_ATTACHEDONLY);

    if (!gGamePadDevice_67B6C0)
    {
        return 0;
    }
    set_game_pad_device_properties_4989C0();
    return 1;
}

MATCH_FUNC(0x498730)
EXPORT bool __stdcall acquire_input_device_498730(LPDIRECTINPUTDEVICEA pGamePadDevice)
{
    if (!pGamePadDevice)
    {
        return 0;
    }

    if (gNeedKbAcquire_67B66C && FAILED(gKeyboardDevice_67B5C0->Acquire()))
    {
        return 0;
    }

    gKeyboardStatus_67B624 = -1;

    const HRESULT hr = pGamePadDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 0, &gKeyboardStatus_67B624, DIGDD_PEEK);
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
    {
        return SUCCEEDED(pGamePadDevice->Acquire()) ? true : false;
    }
    return true;
}

MATCH_FUNC(0x498800)
BOOL __stdcall BurgerKing_1::make_input_devices_498800(HINSTANCE hInstance)
{
    DIPROPDWORD prop;

    gNeedKbAcquire_67B66C = 0;

    if (FAILED(gpDInput_67B804->CreateDevice(GUID_SysKeyboard, &gKeyboardDevice_67B5C0, 0)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputCreateFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 268);
    }

    if (FAILED(gKeyboardDevice_67B5C0->SetDataFormat(&gKeyboardDataFormat_601A54)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetDataFormatFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 279);
    }

    if (FAILED(gKeyboardDevice_67B5C0->SetCooperativeLevel(gHwnd_707F04, 6)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetCooperativeLevelFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 287);
    }

    prop.dwData = 10000; // buffer size

    prop.diph.dwSize = sizeof(DIPROPDWORD);
    prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    prop.diph.dwHow = DIPH_DEVICE;
    prop.diph.dwObj = 0;

    HRESULT hr = gKeyboardDevice_67B5C0->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
    if (FAILED(hr))
    {
        FatalDXError_4A3CF0(hr, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 300);
    }

    prop.dwData = 0;
    gKeyboardDevice_67B5C0->GetProperty(DIPROP_BUFFERSIZE, &prop.diph);
    return !FAILED(gKeyboardDevice_67B5C0->Acquire());
}

MATCH_FUNC(0x498C40)
void __stdcall BurgerKing_1::input_devices_init_498C40(HINSTANCE hInstance)
{
    get_registry_controls_498C00();

    byte_67B80C = 0;

    if (!make_input_devices_498800(hInstance))
    {
        gNeedKbAcquire_67B66C = 1;
    }
    game_pads_init_498BA0();
}

// https://decomp.me/scratch/LbfoG ridiculous function to match
STUB_FUNC(0x498CB0)
void BurgerKing_1::sub_498CB0(u32 a1)
{
    byte_67B80C = a1 >> 7;
}

MATCH_FUNC(0x498D20)
bool BurgerKing_1::game_pad_read_498D20()
{
    DWORD num_items = -1;
    if (acquire_input_device_498730(gGamePadDevice_67B6C0))
    {
        HRESULT jres = gGamePadDevice_67B6C0->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &num_items, DIGDD_PEEK);
        if (jres >= DI_OK)
        {
            sprintf(gTmpBuffer_67C598, "%d: num_items = %d jres = %d", rng_dword_67AB34->get_cur_rng_41CFE0(), num_items, jres);
        }
    }
    else
    {
        num_items = 0;
    }
    return num_items > 0;
}

MATCH_FUNC(0x498C80)
void BurgerKing_1::sub_498C80(s32* a1, DIDEVICEOBJECTDATA* device_data_keys)
{
    *a1 = (device_data_keys->dwOfs << 12) | *a1;
    if ((device_data_keys->dwData & 0x80) != 0)
    {
        *a1 |= 0x200000;
    }
}

// https://decomp.me/scratch/75Pau
WIP_FUNC(0x498DA0)
void BurgerKing_1::read_input_device_498DA0(s32* input_bits, u8 bUnknown)
{
    WIP_IMPLEMENTED;
    bool bUnk_1;
    s32 v5;
    s32 v6;
    s32 input;
    s32* field_8_input_masks;
    bool bUnk_2_unk;
    s32 unk_input;
    s32 v12;
    s32 field_4_input_bits;
    s32 field_0_rng;
    s32 bUnk_3;
    s32 status;
    s32 keyb_dev_data;
    s32 device_data;

    bUnk_1 = true;
    bUnk_3 = 0;
    status = 1;
    if (acquire_input_device_498730(gKeyboardDevice_67B5C0) || acquire_input_device_498730(gGamePadDevice_67B6C0))
    {
        stru_67B610.dwOfs = 0;
        dword_67B5B0 = 0;
        stru_67B610.dwData = 0;
        dword_67B5B4 = 0;
        stru_67B610.dwTimeStamp = 0;
        dword_67B5B8 = 0;
        stru_67B610.dwSequence = 0;
        dword_67B5BC = 0;
        if (acquire_input_device_498730(gGamePadDevice_67B6C0))
        {
            gGamePadDevice_67B6C0->Acquire();
        }
        while (BurgerKing_1::game_pad_read_498D20() || bUnk_1)
        {
            gKeyboardStatus_67B624 = 0;
            if (gKeyboardDevice_67B5C0)
            {
                gKeyboardStatus_67B624 = 1;
                keyb_dev_data = gKeyboardDevice_67B5C0->GetDeviceData(16, &stru_67B610, (unsigned long*)&gKeyboardStatus_67B624, 0);
            }
            else
            {
                keyb_dev_data = -1;
            }
            if (!gGamePadDevice_67B6C0 || gKeyboardStatus_67B624)
            {
                device_data = -1;
                status = 0;
            }
            else
            {
                gKeyboardStatus_67B624 = 1;
                gGamePadDevice_67B6C0->GetDeviceData(16, 0, (LPDWORD)&status, 1);

                device_data = gGamePadDevice_67B6C0->GetDeviceData(16,
                                                                   (LPDIDEVICEOBJECTDATA)&dword_67B5B0,
                                                                   (unsigned long*)&gKeyboardStatus_67B624,
                                                                   0);
                if (bLog_directinput_67D6C0)
                {
                    if (gKeyboardStatus_67B624 > 0)
                    {
                        sprintf(gTmpBuffer_67C598,
                                "%d: input num_items = %d dwOfs = %d; data = %d",
                                rng_dword_67AB34->field_0_rng,
                                status,
                                dword_67B5B0,
                                dword_67B5B4);
                        gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
                    }
                }
            }

            // line 195
            if (keyb_dev_data < 0 && device_data < 0 || gKeyboardStatus_67B624 <= 0)
            {
                return;
            }

            if (stru_67B610.dwOfs == 56)
            {
                BurgerKing_1::sub_498CB0(stru_67B610.dwData);
            }
            else if ((stru_67B610.dwOfs == DIK_SPACE || stru_67B610.dwOfs == DIK_TAB || stru_67B610.dwOfs == DIK_RETURN) &&
                     byte_67B80C == 1)
            {
                return;
            }

            // line 1ef
            if (!gHud_2B00_706620->sub_5D6C70(stru_67B610.dwOfs)) // OBS: bool return type
            {
                v5 = dword_67B5B0;
                v6 = dword_67B5B4;
                field_8_input_masks = gBurgerKing_67F8B0.field_8_input_masks;

                for (input = 0; input < 12; input++, ++field_8_input_masks)
                {
                    bUnk_2_unk = false;
                    if (keyb_dev_data == 0 && dword_67B91C[input] == 0 && dword_67B6E8[input] == stru_67B610.dwOfs)
                    {
                        bUnk_2_unk = true;
                        if ((stru_67B610.dwData & 0x80) != 0) // line 243
                        {
                            if (bUnknown)
                            {
                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            if (bUnknown)
                            {
                                gBurgerKing_67F8B0.clear_input_4CDD10(input);
                            }
                            else
                            {
                                continue;
                            }
                        }
                        v5 = dword_67B5B0;
                        v6 = dword_67B5B4;
                    }
                    else
                    {
                        if (device_data == 0 && dword_67B91C[input] == 1)
                        {
                            unk_input = dword_67B6E8[input];
                            bUnk_3 = false;
                            switch (unk_input)
                            {
                                case 224:
                                    if (!v5)
                                    {
                                        if (v6 > -750)
                                        {
                                            bUnk_3 = true;
                                            if (bUnknown)
                                            {
                                                if ((gBurgerKing_67F8B0.field_4_input_bits & *field_8_input_masks) != 0)
                                                {
                                                    gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                                    v5 = dword_67B5B0;
                                                    v6 = dword_67B5B4;
                                                }
                                            }
                                            bUnk_2_unk = true;
                                        }
                                        else
                                        {
                                            bUnk_2_unk = true;
                                            if (bUnknown)
                                            {
                                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                                                v5 = dword_67B5B0;
                                                v6 = dword_67B5B4;
                                            }
                                        }
                                    }
                                    break;
                                case 225:
                                    if (v5 == 0)
                                    {
                                        if (v6 < 750)
                                        {
                                            bUnk_3 = true;
                                            if (bUnknown)
                                            {
                                                if ((gBurgerKing_67F8B0.field_4_input_bits & *field_8_input_masks) != 0)
                                                {
                                                    gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                                    v5 = dword_67B5B0;
                                                    v6 = dword_67B5B4;
                                                }
                                            }
                                            bUnk_2_unk = true;
                                        }
                                        else
                                        {
                                            bUnk_2_unk = true;
                                            if (bUnknown)
                                            {
                                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                                                v5 = dword_67B5B0;
                                                v6 = dword_67B5B4;
                                            }
                                        }
                                    }
                                    break;
                                case 226:
                                    if (v5 == 4)
                                    {
                                        if (v6 <= -750)
                                        {
                                            goto LABEL_63;
                                        }
                                        v12 = *field_8_input_masks;
                                        field_4_input_bits = gBurgerKing_67F8B0.field_4_input_bits;
                                        goto LABEL_50;
                                    }
                                    break;
                                case 227:
                                    if (v5 == 4)
                                    {
                                        if (v6 >= 750)
                                        {
                                            goto LABEL_63;
                                        }
                                        else
                                        {
                                            field_4_input_bits = gBurgerKing_67F8B0.field_4_input_bits;
                                            v12 = *field_8_input_masks;
                                        LABEL_50:
                                            if ((field_4_input_bits & v12) != 0)
                                            {
                                                //goto LABEL_51;
                                                bUnk_3 = 1;
                                                if (bUnknown)
                                                {
                                                    if ((gBurgerKing_67F8B0.field_4_input_bits & *field_8_input_masks) != 0)
                                                    {
                                                        gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                                        v5 = dword_67B5B0;
                                                        v6 = dword_67B5B4;
                                                    }
                                                }
                                                bUnk_2_unk = true;
                                            }
                                        }
                                    }
                                    break;
                                default:
                                    if (v5 == unk_input + 48)
                                    {
                                        if ((v6 & 0x80u) != 0)
                                        {
                                        LABEL_63:
                                            bUnk_2_unk = true;
                                            if (bUnknown)
                                            {
                                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                                                v5 = dword_67B5B0;
                                                v6 = dword_67B5B4;
                                            }
                                        }
                                        else
                                        {
                                        LABEL_51:
                                            bUnk_3 = true;
                                            if (bUnknown)
                                            {
                                                if ((gBurgerKing_67F8B0.field_4_input_bits & *field_8_input_masks) != 0)
                                                {
                                                    gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                                    v5 = dword_67B5B0;
                                                    v6 = dword_67B5B4;
                                                }
                                            }
                                            bUnk_2_unk = true;
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }

                if (bUnk_2_unk)
                {
                    bUnk_1 = false;
                }
            }
            else
            {
                if (!bUnk_3)
                {
                    BurgerKing_1::sub_498C80(input_bits, &stru_67B610);
                    bUnk_1 = false;
                    if (bLog_directinput_67D6C0)
                    {
                        field_0_rng = rng_dword_67AB34->field_0_rng;
                        if ((stru_67B610.dwData & 0x80) != 0)
                        {
                            sprintf(gTmpBuffer_67C598, "%d: KEY OFF: %d", field_0_rng, stru_67B610.dwOfs);
                        }
                        else
                        {
                            sprintf(gTmpBuffer_67C598, "%d: KEY ON : %d", field_0_rng, stru_67B610.dwOfs);
                        }
                    }
                }
                else
                {
                    bUnk_1 = false;
                }
            }
        }
    }
}

// ================================================

STUB_FUNC(0x4cdcd0)
void BurgerKing_67F8B0::sub_4CDCD0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4cdce0)
void BurgerKing_67F8B0::clear_inputs_4CDCE0()
{
    field_4_input_bits &= ~0xFFFFF000;
}

MATCH_FUNC(0x4cdcf0)
void BurgerKing_67F8B0::set_input_4CDCF0(s32 mask_idx)
{
    field_4_input_bits |= field_8_input_masks[mask_idx];
}

MATCH_FUNC(0x4cdd10)
void BurgerKing_67F8B0::clear_input_4CDD10(s32 mask_idx)
{
    field_4_input_bits &= ~field_8_input_masks[mask_idx];
}

MATCH_FUNC(0x4cdd80)
bool BurgerKing_67F8B0::should_ignore_input_4CDD80(s32 dinput_key)
{
    return dinput_key == DIK_NUMPAD1 || dinput_key == DIK_NUMPAD2 || dinput_key == DIK_NUMPAD3 || dinput_key == DIK_NUMPAD4 ||
        dinput_key == DIK_NUMPAD5 || dinput_key == DIK_NUMPAD6 || dinput_key == DIK_NUMPAD7 || dinput_key == DIK_NUMPAD8 ||
        dinput_key == DIK_NUMPAD9 || dinput_key == DIK_MULTIPLY || dinput_key == DIK_SUBTRACT || dinput_key == DIK_ESCAPE ||
        dinput_key == DIK_F6 || dinput_key == DIK_ADD || gHud_2B00_706620->sub_5D6CB0(dinput_key);
}

MATCH_FUNC(0x4cddf0)
bool BurgerKing_67F8B0::should_ignore_input_4CDDF0(s32 dinput_key)
{
    return !should_ignore_input_4CDD80(dinput_key) && dinput_key != DIK_ADD;
}

MATCH_FUNC(0x4cde20)
void BurgerKing_67F8B0::save_replay_record_4CDE20(u32 inputs)
{
    if ((inputs & 0x1FF000) != 0)
    {
        if (should_ignore_input_4CDD80((inputs >> 12) & 0x1FF))
        {
            inputs &= 0xFFC00FFF;
        }
    }

    if (inputs)
    {
        if (field_75340_rec_buf_idx < 40000 && field_38_replay_state == Live_0)
        {
            field_3C_rec_buff[field_75340_rec_buf_idx].field_4_inputs = inputs;
            field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx = rng_dword_67AB34->field_0_rng;
            field_3C_rec_buff[field_75340_rec_buf_idx].field_8_rng_rnd = rng_dword_67AB34->field_4_rnd;

            if (bConstant_replay_save_67D5C4 == 1)
            {
                const s32 rec_idx = this->field_75340_rec_buf_idx;
                u32 rec_len = sizeof(BurgerKingBurger_0xC);
                File::AppendBufferToFile_4A6F50("test\\replay.rep", &field_3C_rec_buff[rec_idx], &rec_len);
            }

            field_75340_rec_buf_idx++;
        }
    }
}

MATCH_FUNC(0x4cded0)
void BurgerKing_67F8B0::SaveReplay_4CDED0()
{
    if (bConstant_replay_save_67D5C4 != 1 && bDo_release_replay_67D4EB && field_38_replay_state != Unkn_2 && field_75340_rec_buf_idx > 0 &&
        !RecOrPlayBackState_4CEDF0())
    {
        size_t len = sizeof(BurgerKingBurger_0xC) * field_75340_rec_buf_idx;
        File::AppendBufferToFile_4A6F50("test\\replay.rep", field_3C_rec_buff, &len);
    }
}

// https://decomp.me/scratch/c6Gy5
STUB_FUNC(0x4cdf30)
void BurgerKing_67F8B0::modify_inputs_4CDF30(s32 match_mask)
{
    NOT_IMPLEMENTED;

    for (s32 i = 0; i < 12; i++)
    {
        if ((field_8_input_masks[i] & match_mask) != 0)
        {
            if ((field_8_input_masks[i] & field_4_input_bits) != 0)
            {
                field_4_input_bits &= ~field_8_input_masks[i];
            }
            else
            {
                field_4_input_bits |= field_8_input_masks[i];
            }
        }
    }

    if ((match_mask & 0xFFFFF000) != 0)
    {
        this->field_4_input_bits |= match_mask & 0xFFFFF000;
    }
}

STUB_FUNC(0x4cdf70)
void BurgerKing_67F8B0::AppendReplayHeader_4CDF70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ce380)
char_type BurgerKing_67F8B0::LoadReplayHeader_4CE380(char_type bLoadDebug)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4ce650)
void BurgerKing_67F8B0::sub_4CE650()
{
    const AttractFile* attr1FilePath = &attractFiles_62083C[0];
    for (s32 i = 0; i < 3; i++)
    {
        struct _finddata_t fileInfo;
        long hFind = _findfirst(attr1FilePath->field_0_path, &fileInfo);
        if (hFind == -1)
        {
            strcpy(gErrStr_67C29C, attr1FilePath->field_0_path);
            FatalError_4A38C0(0x1BE5, "C:\\Splitting\\Gta2\\Source\\input.cpp", 524);
        }
        _findclose(hFind);
        ++attr1FilePath;
    }
}

MATCH_FUNC(0x4ce6e0)
void BurgerKing_67F8B0::GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut)
{
    strcpy(pAttrPathOut, attractFiles_62083C[field_75345_attract_idx].field_0_path);

    if (++field_75345_attract_idx >= ATTRACT_COUNT)
    {
        field_75345_attract_idx = 0;
    }
}

MATCH_FUNC(0x4ce740)
void BurgerKing_67F8B0::input_init_replay_4CE740(HINSTANCE hInstance)
{
    char FileName[256];

    this->field_0_bShutDown = 0;
    BurgerKing_67F8B0::GetNextAttrReplay_4CE6E0(FileName);
    this->field_8_input_masks[0] = 1;
    this->field_8_input_masks[1] = 2;
    this->field_8_input_masks[2] = 4;
    this->field_8_input_masks[3] = 8;
    this->field_8_input_masks[4] = 0x10;
    this->field_8_input_masks[5] = 0x20;
    this->field_8_input_masks[6] = 0x40;
    this->field_8_input_masks[7] = 0x80;
    this->field_8_input_masks[8] = 0x100;
    this->field_8_input_masks[9] = 0x200;
    this->field_8_input_masks[10] = 0x400;
    this->field_8_input_masks[11] = 0x800;
    this->field_4_input_bits = 0;
    this->field_38_replay_state = Replay_3;
    gBurgerKing_1_67B990->input_devices_init_498C40(hInstance);
    memset(this->field_3C_rec_buff, 0, sizeof(this->field_3C_rec_buff));
    this->field_75340_rec_buf_idx = 0;
    bConstant_replay_save_67D5C4 = 0;
    File::Global_Open_4A7060(FileName);
    LoadReplayHeader_4CE380(0);

    u32 bufLen = sizeof(field_3C_rec_buff);
    const s32 remainderSize = File::GetRemainderSize_4A7250(this->field_3C_rec_buff, &bufLen);
    File::Global_Close_4A70C0();
    this->field_7533C_used_recs_count = remainderSize / sizeof(BurgerKingBurger_0xC);
    if (sizeof(BurgerKingBurger_0xC) * (remainderSize / sizeof(BurgerKingBurger_0xC)) != remainderSize)
    {
        FatalError_4A38C0(Gta2Error::ReplayFileTooLarge, "C:\\Splitting\\Gta2\\Source\\input.cpp", 616, remainderSize);
    }

    if (this->field_7533C_used_recs_count == 0)
    {
        FatalError_4A38C0(Gta2Error::EmptyReplayFile, "C:\\Splitting\\Gta2\\Source\\input.cpp", 621);
    }
}

MATCH_FUNC(0x4ce880)
void BurgerKing_67F8B0::input_init_live_4CE880(HINSTANCE hInstance)
{
    field_0_bShutDown = 0;
    field_8_input_masks[0] = 1;
    field_8_input_masks[1] = 2;
    field_8_input_masks[2] = 4;
    field_8_input_masks[3] = 8;
    field_8_input_masks[4] = 0x10;
    field_8_input_masks[5] = 0x20;
    field_8_input_masks[6] = 0x40;
    field_8_input_masks[7] = 128;
    field_8_input_masks[8] = 0x100;
    field_8_input_masks[9] = 0x200;
    field_8_input_masks[10] = 1024;
    field_8_input_masks[11] = 0x800;
    field_4_input_bits = 0;
    field_38_replay_state = Live_0;

    gBurgerKing_1_67B990 = new BurgerKing_1();
    if (!gBurgerKing_1_67B990)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\input.cpp", 675);
    }

    gBurgerKing_1_67B990->input_devices_init_498C40(hInstance);
    memset(field_3C_rec_buff, 0, sizeof(field_3C_rec_buff));

    field_75340_rec_buf_idx = 0;

    if (bPlay_replay_67D4F4 == 1)
    {
        bConstant_replay_save_67D5C4 = 0;
        File::Global_Open_4A7060("test\\replay.rep");
        BurgerKing_67F8B0::LoadReplayHeader_4CE380(1);
        u32 size = 480000;
        size_t remainderSize = File::GetRemainderSize_4A7250(field_3C_rec_buff, &size);
        File::Global_Close_4A70C0();
        field_7533C_used_recs_count = remainderSize / 0xC;
        if (12 * (remainderSize / 0xC) != remainderSize)
        {
            FatalError_4A38C0(Gta2Error::ReplayFileTooLarge, "C:\\Splitting\\Gta2\\Source\\input.cpp", 702, remainderSize);
        }
        field_38_replay_state = field_7533C_used_recs_count != 0;
    }
    else
    {
        if (bDo_release_replay_67D4EB)
        {
            File::CreateFile_4A7000("test\\replay.rep");
            BurgerKing_67F8B0::AppendReplayHeader_4CDF70();
        }
        field_38_replay_state = Live_0;
    }
    if (bConstant_replay_save_67D5C4)
    {
        if (bPlay_replay_67D4F4 == 1)
        {
            if (bDo_release_replay_67D4EB)
            {
                File::CreateFile_4A7000("test\\replay.rep");
                BurgerKing_67F8B0::AppendReplayHeader_4CDF70();
            }
        }
    }
}

MATCH_FUNC(0x4cea00)
void BurgerKing_67F8B0::Shutdown_4CEA00() // 4CEA00
{
    if (!field_0_bShutDown)
    {
        field_0_bShutDown = 1;
        gBurgerKing_1_67B990->free_input_devices_4987A0();
        SaveReplay_4CDED0();
        GTA2_DELETE_AND_NULL(gBurgerKing_1_67B990);
    }
}

MATCH_FUNC(0x4cea40)
void BurgerKing_67F8B0::replay_save_4CEA40(u32* input_bits)
{
    if ((*input_bits & 0xFFFFF000) == 0x37000)
    {
        field_38_replay_state = Live_0;

        if (bDo_release_replay_67D4EB)
        {
            File::CreateFile_4A7000("test\\replay.rep");
            AppendReplayHeader_4CDF70();
        }

        // Clear out the unused records
        memset(&this->field_3C_rec_buff[this->field_75340_rec_buf_idx],
               0,
               sizeof(BurgerKingBurger_0xC) * ((GTA2_COUNTOF(field_3C_rec_buff) - field_75340_rec_buf_idx)));
        if (bConstant_replay_save_67D5C4)
        {
            SaveReplay_4CDED0();
        }
    }
}

// https://decomp.me/scratch/t5tNu
STUB_FUNC(0x4ceac0)
u32 BurgerKing_67F8B0::get_input_bits_4CEAC0()
{
    s32 inputs;
    s32 saved_input = field_4_input_bits;
    u32* control_status = (u32*)&field_4_input_bits;
    BurgerKing_67F8B0::clear_inputs_4CDCE0();

    s32 replay_state = field_38_replay_state;

    *control_status &= ~0x200000;
    u32 remove_bit = *control_status;

    switch (replay_state)
    {
        case Unkn_1:
            if (rng_dword_67AB34->field_0_rng >= (u32)field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx)
            {
                inputs = field_3C_rec_buff[field_75340_rec_buf_idx].field_4_inputs;
                field_75340_rec_buf_idx++;
                if (field_75340_rec_buf_idx >= field_7533C_used_recs_count)
                {
                    if (bDo_exit_after_replay_67D6E4)
                    {
                        gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::CloseGameImmediately_1);
                    }
                    else
                    {
                        if (bDo_release_replay_67D4EB)
                        {
                            File::CreateFile_4A7000("test\\replay.rep");
                            BurgerKing_67F8B0::AppendReplayHeader_4CDF70();
                        }
                        field_38_replay_state = Live_0;
                    }
                }

                if (field_75340_rec_buf_idx < 2)
                {
                    saved_input = 0;
                }
                else
                {
                    saved_input = field_8_input_masks[3 * field_75340_rec_buf_idx + 8];
                }
                BurgerKing_67F8B0::modify_inputs_4CDF30(inputs);
            }
            if (field_75344_bSomething)
            {
                saved_input = *control_status;
                // Problem here:
                if ((*control_status & 0x1FF000) == 0)
                {
                    *control_status = 0;
                    gBurgerKing_1_67B990->read_input_device_498DA0((s32*)control_status, 0);
                    *control_status |= saved_input;
                    BurgerKing_67F8B0::replay_save_4CEA40(control_status);
                }
            }
            break;

        case Replay_3:
            if (rng_dword_67AB34->field_0_rng >= (u32)field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx)
            {
                inputs = field_3C_rec_buff[field_75340_rec_buf_idx].field_4_inputs;
                field_75340_rec_buf_idx++;
                if (field_75340_rec_buf_idx >= field_7533C_used_recs_count)
                {
                    gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::ReplayExit_6);
                }

                if (field_75340_rec_buf_idx < 2)
                {
                    saved_input = 0;
                }
                else
                {
                    saved_input = field_8_input_masks[3 * field_75340_rec_buf_idx + 8];
                }
                BurgerKing_67F8B0::modify_inputs_4CDF30(inputs);
            }
            if (field_75344_bSomething)
            {
                saved_input = *control_status;
                // Problem also is here:
                if ((*control_status & 0x1FF000) == 0)
                {
                    *control_status = 0;
                    gBurgerKing_1_67B990->read_input_device_498DA0((s32*)control_status, 0);
                    if ((*control_status & 0xFFFFF000) != 0)
                    {
                        gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::ReplayExit_6);
                    }
                    *control_status |= saved_input;
                    BurgerKing_67F8B0::replay_save_4CEA40(control_status);
                }
            }
            break;

        case Live_0:
            if (field_75344_bSomething)
            {
                if (gGame_0x40_67E008->field_0_game_state != GameState::Paused_2)
                {
                    gBurgerKing_1_67B990->read_input_device_498DA0((s32*)control_status, 1);
                    BurgerKing_67F8B0::save_replay_inputs_4CED00(*control_status, saved_input);
                }
                else
                {
                    gBurgerKing_1_67B990->read_input_device_498DA0((s32*)control_status, 0);
                    if (BurgerKing_67F8B0::should_ignore_input_4CDDF0(((u32)*control_status >> 12) & 0x1FF))
                    {
                        *control_status = remove_bit;
                    }
                }
                BurgerKing_67F8B0::replay_save_4CEA40(control_status);
            }
            break;
    }

    if (bLog_input_67D4CC)
    {
        if (*control_status != saved_input)
        {
            sprintf(gTmpBuffer_67C598, "%d: control_status = %d", rng_dword_67AB34->field_0_rng, *control_status);
            gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
        }
    }
    return *control_status;
}

MATCH_FUNC(0x4ced00)
void BurgerKing_67F8B0::save_replay_inputs_4CED00(s32 input_old, s32 input_new)
{
    u32 calc_inputs = 0;
    if (input_old != input_new)
    {
        u32 xor_inputs = input_new ^ input_old;

        if ((xor_inputs & 1) != 0)
        {
            calc_inputs |= 1u;
        }
        if ((xor_inputs & 2) != 0)
        {
            calc_inputs |= 2u;
        }
        if ((xor_inputs & 4) != 0)
        {
            calc_inputs |= 4u;
        }
        if ((xor_inputs & 8) != 0)
        {
            calc_inputs |= 8u;
        }
        if ((xor_inputs & 0x10) != 0)
        {
            calc_inputs |= 0x10u;
        }
        if ((xor_inputs & 0x20) != 0)
        {
            calc_inputs |= 0x20u;
        }
        if ((xor_inputs & 0x40) != 0)
        {
            calc_inputs |= 0x40u;
        }
        if ((xor_inputs & 0x80u) != 0)
        {
            calc_inputs |= 0x80;
        }
        if ((xor_inputs & 0x100) != 0)
        {
            calc_inputs |= 0x100;
        }
        if ((xor_inputs & 0x200) != 0)
        {
            calc_inputs |= 0x200;
        }
        if ((xor_inputs & 0x400) != 0)
        {
            calc_inputs |= 0x400;
        }
        if ((xor_inputs & 0x800) != 0)
        {
            calc_inputs |= 0x800;
        }
        if ((input_old & 0x1FF000) != 0)
        {
            calc_inputs |= field_4_input_bits & 0xFFFFF000;
        }
        if (calc_inputs)
        {
            BurgerKing_67F8B0::save_replay_record_4CDE20(calc_inputs);
        }
    }
}

MATCH_FUNC(0x4ced90)
void BurgerKing_67F8B0::sub_4CED90()
{
    s8 i = 0;
    s32 bit_idx = 0;
    do
    {
        if (((1 << bit_idx) & field_4_input_bits) != 0)
        {
            swprintf(tmpBuff_67BD9C, L"Control %d", bit_idx);
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 10, 16 * (i + 1), word_706600, 1);
        }
        ++i;
        ++bit_idx;
    } while (i < 12);
}

MATCH_FUNC(0x4cedf0)
bool BurgerKing_67F8B0::RecOrPlayBackState_4CEDF0()
{
    if (field_38_replay_state == Unkn_1 || field_38_replay_state == Replay_3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4cee10)
void BurgerKing_67F8B0::ShowInput_4CEE10()
{
    if (RecOrPlayBackState_4CEDF0())
    {
        gHud_2B00_706620->field_650.sub_5D1F50(L"PLAYBACK", -1, 0, word_706600, 1);
    }
    else
    {
        gHud_2B00_706620->field_650.sub_5D1F50(L"RECORDING", -1, 0, word_706600, 1);
    }
    sub_4CED90();
}
