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
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lucid_hamilton.hpp"

#define ATTRACT_COUNT 3

// TODO: From frontend.cpp
extern DIDATAFORMAT gInputDeviceFormat_601A6C;

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

// TODO: for some reason it does not have uAppData.
// Otherwise using DIDEVICEOBJECTDATA (size 0x14) makes gGamePadDeviceData_67B5B0 overlaps gKeyboardDevice_67B5C0
struct mini_device_obj_data
{
    s32 dwOfs;
    s32 dwData;
    s32 dwTimeStamp;
    s32 dwSequence;
};

DEFINE_GLOBAL(BurgerKing_67F8B0, gBurgerKing_67F8B0, 0x67F8B0);
DEFINE_GLOBAL(BurgerKing_1*, gBurgerKing_1_67B990, 0x67B990);
DEFINE_GLOBAL(DWORD, gKeyboardStatus_67B624, 0x67B624);
DEFINE_GLOBAL(u8, gAltKeyDown_67B80C, 0x67B80C);
DEFINE_GLOBAL(bool, gNeedKbAcquire_67B66C, 0x67B66C);
DEFINE_GLOBAL(DIDEVICEOBJECTDATA, gKeyboardDeviceData_67B610, 0x67B610);
DEFINE_GLOBAL(mini_device_obj_data, gGamePadDeviceData_67B5B0, 0x67B5B0);

DEFINE_GLOBAL_ARRAY(s32, gDefaultControls_61A9E4, 12, 0x61A9E4);
DEFINE_GLOBAL_ARRAY(s32, gMaybeDeviceType_67B91C, 12, 0x67B91C);
DEFINE_GLOBAL_ARRAY(s32, gPlayerControlsBinding_67B6E8, 12, 0x67B6E8);

EXTERN_GLOBAL(DIDATAFORMAT, gKeyboardDataFormat_601A54);
EXTERN_GLOBAL(HINSTANCE, gHInstance_708220);
EXTERN_GLOBAL(s32, gGTA2VersionMajor_708280);
EXTERN_GLOBAL(s32, gGTA2VersionMajor_708284);

DEFINE_GUID(GUID_SysKeyboard, 0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

const AttractFile attractFiles_62083C[ATTRACT_COUNT] = {"data\\attract\\attr1.rep", "data\\attract\\attr2.rep", "data\\attract\\attr3.rep"};

// TODO: Move
EXPORT int __stdcall FatalDXError_4A3CF0(HRESULT hr, const char* pSourceFile, int lineNo)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x498910)
EXPORT BOOL CALLBACK DirectInputDeviceEnumCallBack_498910(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    const GUID guidInstanceCopy = lpddi->guidInstance;
    LPDIRECTINPUTDEVICEA tempDevice;
    if (SUCCEEDED(gpDInput_67B804->CreateDevice(guidInstanceCopy, &tempDevice, 0)))
    {
        const HRESULT hr = tempDevice->QueryInterface(IID_IDirectInputDevice2A, (LPVOID*)&gGamePadDevice_67B6C0);
        tempDevice->Release();

        if (SUCCEEDED(hr))
        {
            if (FAILED(gGamePadDevice_67B6C0->SetCooperativeLevel(gHwnd_707F04, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
            {
                gGamePadDevice_67B6C0->Release();
                gGamePadDevice_67B6C0 = 0;
                return DIENUM_STOP;
            }

            if (FAILED(gGamePadDevice_67B6C0->SetDataFormat(&gInputDeviceFormat_601A6C)))
            {
                gGamePadDevice_67B6C0->Release();
                gGamePadDevice_67B6C0 = 0;
                return DIENUM_STOP;
            }
        }
    }
    return DIENUM_STOP;
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
        const u32 v1 = gRegistry_6FF968.Set_Control_Setting_587010(i, gDefaultControls_61A9E4[i]);
        gMaybeDeviceType_67B91C[i] = v1 >> 15;
        gPlayerControlsBinding_67B6E8[i] = (u8)v1;
    }
}

// TODO: the debug strings are guesses, only code is compared
MATCH_FUNC(0x4989C0)
void BurgerKing_1::set_game_pad_device_properties_4989C0()
{
    DIPROPDWORD prop;
    DIPROPRANGE range;
    DIDEVICEINSTANCEA instance;

    if (gGamePadDevice_67B6C0)
    {
        instance.dwSize = sizeof(DIDEVICEINSTANCEA);
        gGamePadDevice_67B6C0->GetDeviceInfo(&instance);
        gGamePadDevice_67B6C0->Unacquire();

        prop.dwData = 10000;
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_DEVICE;
        prop.diph.dwObj = 0;
        gGamePadDevice_67B6C0->SetProperty(DIPROP_SATURATION, &prop.diph);

        prop.dwData = 10000;
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_DEVICE;
        prop.diph.dwObj = 0;
        HRESULT hr = gGamePadDevice_67B6C0->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
        if (FAILED(hr))
        {
            FatalDXError_4A3CF0(hr, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 434);
        }

        prop.dwData = 0;
        gGamePadDevice_67B6C0->GetProperty(DIPROP_BUFFERSIZE, &prop.diph);

        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwHow = DIPH_BYOFFSET;
        range.lMin = -1000;
        range.lMax = 1000;
        range.diph.dwObj = DIJOFS_X;
        if (FAILED(gGamePadDevice_67B6C0->SetProperty(DIPROP_RANGE, &range.diph)))
        {
            OutputDebugStringA("Failed to set the x axis range\n");
            return;
        }

        range.diph.dwObj = DIJOFS_Y;
        if (FAILED(gGamePadDevice_67B6C0->SetProperty(DIPROP_RANGE, &range.diph)))
        {
            OutputDebugStringA("Failed to set the y axis range\n");
            return;
        }

        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_BYOFFSET;
        prop.dwData = 2500;
        prop.diph.dwObj = DIJOFS_X;
        if (FAILED(gGamePadDevice_67B6C0->SetProperty(DIPROP_DEADZONE, &prop.diph)))
        {
            OutputDebugStringA("Failed to set the x axis dead zone\n");
            return;
        }

        prop.diph.dwObj = DIJOFS_Y;
        if (FAILED(gGamePadDevice_67B6C0->SetProperty(DIPROP_DEADZONE, &prop.diph)))
        {
            OutputDebugStringA("Failed to set the y axis dead zone\n");
            return;
        }

        gGamePadDevice_67B6C0->Acquire();
    }
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
bool BurgerKing_1::acquire_input_device_498730(LPDIRECTINPUTDEVICEA pGamePadDevice)
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

    gAltKeyDown_67B80C = 0;

    if (!make_input_devices_498800(hInstance))
    {
        gNeedKbAcquire_67B66C = 1;
    }
    game_pads_init_498BA0();
}

// https://decomp.me/scratch/LbfoG ridiculous function to match
STUB_FUNC(0x498CB0)
void BurgerKing_1::SetAltKeyState_498CB0(u32 a1)
{
    gAltKeyDown_67B80C = a1 >> 7;
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
            sprintf(gTmpBuffer_67C598, "%d: num_items = %d jres = %d", gpRng_67AB34->get_cur_rng_41CFE0(), num_items, jres);
        }
    }
    else
    {
        num_items = 0;
    }
    return num_items > 0;
}

MATCH_FUNC(0x498C80)
void BurgerKing_1::AddKeyToInputBits_498C80(s32* a1, DIDEVICEOBJECTDATA* device_data_keys)
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
    bool bUnk_1;
    s32 v5_edi;
    s32 v6_edx;
    s32 input;
    bool bUnk_2_unk;
    bool bPressed;
    bool bReleased;
    s32 unk_input;
    s32 bUnk_3;
    s32 status;
    s32 keyb_dev_data;
    s32 device_data;

    bUnk_1 = true;
    bUnk_3 = 0;
    status = 1;
    if (acquire_input_device_498730(gKeyboardDevice_67B5C0) || acquire_input_device_498730(gGamePadDevice_67B6C0))
    {
        gKeyboardDeviceData_67B610.dwOfs = 0;
        gGamePadDeviceData_67B5B0.dwOfs = 0;
        gKeyboardDeviceData_67B610.dwData = 0;
        gGamePadDeviceData_67B5B0.dwData = 0;
        gKeyboardDeviceData_67B610.dwTimeStamp = 0;
        gGamePadDeviceData_67B5B0.dwTimeStamp = 0;
        gKeyboardDeviceData_67B610.dwSequence = 0;
        gGamePadDeviceData_67B5B0.dwSequence = 0;
        if (acquire_input_device_498730(gGamePadDevice_67B6C0))
        {
            ((LPDIRECTINPUTDEVICE2A)gGamePadDevice_67B6C0)->Poll();
        }
        while (BurgerKing_1::game_pad_read_498D20() || bUnk_1)
        {
            gKeyboardStatus_67B624 = 0;
            if (gKeyboardDevice_67B5C0)
            {
                gKeyboardStatus_67B624 = 1;
                keyb_dev_data = gKeyboardDevice_67B5C0->GetDeviceData(16, &gKeyboardDeviceData_67B610, (unsigned long*)&gKeyboardStatus_67B624, 0);
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
                                                                   (DIDEVICEOBJECTDATA*)&gGamePadDeviceData_67B5B0,
                                                                   (unsigned long*)&gKeyboardStatus_67B624,
                                                                   0);
                if (bLog_directinput_67D6C0)
                {
                    if (gKeyboardStatus_67B624 > 0)
                    {
                        sprintf(gTmpBuffer_67C598,
                                "%d: input num_items = %d dwOfs = %d; data = %d",
                                gpRng_67AB34->get_cur_rng_41CFE0(),
                                status,
                                gGamePadDeviceData_67B5B0.dwOfs,
                                gGamePadDeviceData_67B5B0.dwData);
                        gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
                    }
                }
            }

            // line 195
            if (keyb_dev_data < 0 && device_data < 0 || gKeyboardStatus_67B624 <= 0)
            {
                return;
            }

            if (gKeyboardDeviceData_67B610.dwOfs == DIK_LMENU)
            {
                BurgerKing_1::SetAltKeyState_498CB0(gKeyboardDeviceData_67B610.dwData);
            }
            else if ((gKeyboardDeviceData_67B610.dwOfs == DIK_SPACE || gKeyboardDeviceData_67B610.dwOfs == DIK_TAB || gKeyboardDeviceData_67B610.dwOfs == DIK_RETURN) &&
                     gAltKeyDown_67B80C == 1)
            {
                return;
            }

            // line 1ef
            if (!gHud_2B00_706620->IsInputKeyConsumed_5D6C70(gKeyboardDeviceData_67B610.dwOfs)) // OBS: bool return type
            {
                v5_edi = gGamePadDeviceData_67B5B0.dwOfs;
                v6_edx = gGamePadDeviceData_67B5B0.dwData;

                // Check for player controls (up, down, shoot etc)
                for (input = 0; input < 12; input++)
                {
                    bUnk_2_unk = false;
                    if (keyb_dev_data == 0 && gMaybeDeviceType_67B91C[input] == 0 &&
                        gPlayerControlsBinding_67B6E8[input] == gKeyboardDeviceData_67B610.dwOfs)
                    {
                        // PC binding
                        bUnk_2_unk = true;
                        if ((gKeyboardDeviceData_67B610.dwData & 0x80) != 0)
                        {
                            if (bUnknown)
                            {
                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                                v5_edi = gGamePadDeviceData_67B5B0.dwOfs;
                                v6_edx = gGamePadDeviceData_67B5B0.dwData;
                            }
                        }
                        else
                        {
                            if (bUnknown)
                            {
                                gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                v5_edi = gGamePadDeviceData_67B5B0.dwOfs;
                                v6_edx = gGamePadDeviceData_67B5B0.dwData;
                            }
                        }
                    }
                    else if (device_data == 0 && gMaybeDeviceType_67B91C[input] == 1)
                    {
                        // Gamepad binding
                        unk_input = gPlayerControlsBinding_67B6E8[input];
                        bUnk_3 = 0;
                        bPressed = false;
                        bReleased = false;
                        switch (unk_input)
                        {
                            case 224:
                                if (v5_edi == 0)
                                {
                                    if (v6_edx <= -750)
                                    {
                                        bPressed = true;
                                    }
                                    else
                                    {
                                        bReleased = true;
                                    }
                                }
                                break;
                            case 225:
                                if (v5_edi == 0)
                                {
                                    if (v6_edx >= 750)
                                    {
                                        bPressed = true;
                                    }
                                    else
                                    {
                                        bReleased = true;
                                    }
                                }
                                break;
                            case 226:
                                if (v5_edi == 4)
                                {
                                    if (v6_edx <= -750)
                                    {
                                        bPressed = true;
                                    }
                                    else if (gBurgerKing_67F8B0.IsInputSet_44C050(input))
                                    {
                                        bReleased = true;
                                    }
                                }
                                break;
                            case 227:
                                if (v5_edi == 4)
                                {
                                    if (v6_edx >= 750)
                                    {
                                        bPressed = true;
                                    }
                                    else if (gBurgerKing_67F8B0.IsInputSet_44C050(input))
                                    {
                                        bReleased = true;
                                    }
                                }
                                break;
                            default:
                                if (v5_edi == unk_input + 48)
                                {
                                    if ((v6_edx & 0x80) != 0)
                                    {
                                        bPressed = true;
                                    }
                                    else
                                    {
                                        bReleased = true;
                                    }
                                }
                                break;
                        }

                        if (bPressed)
                        {
                            bUnk_2_unk = true;
                            if (bUnknown)
                            {
                                gBurgerKing_67F8B0.set_input_4CDCF0(input);
                                v5_edi = gGamePadDeviceData_67B5B0.dwOfs;
                                v6_edx = gGamePadDeviceData_67B5B0.dwData;
                            }
                        }
                        else if (bReleased)
                        {
                            bUnk_3 = 1;
                            if (bUnknown && gBurgerKing_67F8B0.IsInputSet_44C050(input))
                            {
                                gBurgerKing_67F8B0.clear_input_4CDD10(input);
                                v5_edi = gGamePadDeviceData_67B5B0.dwOfs;
                                v6_edx = gGamePadDeviceData_67B5B0.dwData;
                            }
                            bUnk_2_unk = true;
                        }
                    }
                }

                if (bUnk_2_unk)
                {
                    bUnk_1 = false;
                    continue;
                }
            }

            if (bUnk_3)
            {
                bUnk_1 = false;
            }
            else
            {
                BurgerKing_1::AddKeyToInputBits_498C80(input_bits, &gKeyboardDeviceData_67B610);
                bUnk_1 = false;
                if (bLog_directinput_67D6C0)
                {
                    if ((gKeyboardDeviceData_67B610.dwData & 0x80) != 0)
                    {
                        sprintf(gTmpBuffer_67C598, "%d: KEY OFF: %d", gpRng_67AB34->get_cur_rng_41CFE0(), gKeyboardDeviceData_67B610.dwOfs);
                    }
                    else
                    {
                        sprintf(gTmpBuffer_67C598, "%d: KEY ON : %d", gpRng_67AB34->get_cur_rng_41CFE0(), gKeyboardDeviceData_67B610.dwOfs);
                    }
                }
            }
        }
    }
}

// ================================================

MATCH_FUNC(0x4cdcd0)
void BurgerKing_67F8B0::StaticShutdown_4CDCD0()
{
    gBurgerKing_67F8B0.Shutdown_4CEA00();
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
        dinput_key == DIK_F6 || dinput_key == DIK_ADD || gHud_2B00_706620->IsQuitMessageInputKey_5D6CB0(dinput_key);
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
            field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx = gpRng_67AB34->get_cur_rng_41CFE0();
            field_3C_rec_buff[field_75340_rec_buf_idx].field_8_rng_rnd = gpRng_67AB34->get_rnd_45F9E0();

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
// Register allocation differs, see docs/match_attempts.md
WIP_FUNC(0x4cdf30)
void BurgerKing_67F8B0::modify_inputs_4CDF30(s32 match_mask)
{
    WIP_IMPLEMENTED;

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

    s32 high_bits = match_mask & 0xFFFFF000;
    if (high_bits != 0)
    {
        this->field_4_input_bits |= high_bits;
    }
}

MATCH_FUNC(0x4cdf70)
void BurgerKing_67F8B0::AppendReplayHeader_4CDF70()
{
    ReplayHeader_10C header;
    DWORD computer_name_size;
    size_t header_size;

    memset(&header, 0, sizeof(header));
    computer_name_size = 29;
    sprintf(header.field_0_version, "v%d.%d", gGTA2VersionMajor_708280, gGTA2VersionMajor_708284);

    time_t now = time(NULL);
    char_type* pDate = ctime(&now);
    pDate[strlen(pDate) - 1] = 0;
    sprintf(header.field_8_date, pDate);

    GetComputerNameA(header.field_26_computer_name, &computer_name_size);
    strcpy(header.field_44_map_name, gLucid_hamilton_67E8E0.GetMapName_4C5940());
    strcpy(header.field_6C_style_name, gLucid_hamilton_67E8E0.GetStyleName_4C5950());
    strcpy(header.field_94_script_name, gLucid_hamilton_67E8E0.GetScriptName_4C5960());
    strcpy(header.field_BC_debug_str, gLucid_hamilton_67E8E0.GetDebugStr_4C5970());

    header.field_0_version[7] = '\n';
    header.field_8_date[29] = '\n';
    header.field_26_computer_name[29] = '\n';
    header.field_44_map_name[39] = '\n';
    header.field_6C_style_name[39] = '\n';
    header.field_94_script_name[39] = '\n';
    header.field_BC_debug_str[39] = '\n';
    header.field_E4_flags[39] = '\n';

    header.field_E4_flags[0] = bSkip_dummies_67D4EF ? '1' : '0';
    header.field_E4_flags[1] = bDo_test_67D4F8 ? '1' : '0';
    header.field_E4_flags[2] = bSkip_mission_67D4E5 ? '1' : '0';
    header.field_E4_flags[3] = bDo_brian_test_67D544 ? '1' : '0';
    header.field_E4_flags[4] = bDo_iain_test_67D4E9 ? '1' : '0';
    header.field_E4_flags[5] = bSkip_traffic_lights_67D4EC ? '1' : '0';
    header.field_E4_flags[6] = bSkip_recycling_67D575 ? '1' : '0';
    header.field_E4_flags[7] = bLimit_recycling_67D4CA ? '1' : '0';
    header.field_E4_flags[8] = bNo_annoying_chars_67D586 ? '1' : '0';
    header.field_E4_flags[9] = bDo_mike_67D5CC ? '1' : '0';
    header.field_E4_flags[10] = bDo_kill_phones_on_answer_67D6E8 ? '1' : '0';
    header.field_E4_flags[11] = bGet_all_weapons_67D684 ? '1' : '0';
    header.field_E4_flags[12] = bDont_get_car_back_67D4F5 ? '1' : '0';
    header.field_E4_flags[13] = bSkip_ambulance_67D6C9 ? '1' : '0';
    header.field_E4_flags[14] = bSkip_police_67D4F9 ? '1' : '0';
    header.field_E4_flags[15] = bDo_invulnerable_67D4CB ? '1' : '0';
    header.field_E4_flags[16] = bDo_free_shopping_67D6CD ? '1' : '0';
    header.field_E4_flags[17] = bKeep_weapons_after_death_67D54D ? '1' : '0';
    header.field_E4_flags[18] = bSkip_skidmarks_67D585 ? '1' : '0';
    header.field_E4_flags[19] = bExplodingScoresOff_67D4FB ? '1' : '0';
    header.field_E4_flags[20] = gDo_infinite_lives_67D4C9 ? '1' : '0';
    header.field_E4_flags[21] = bDo_blood_67D5C5 ? '1' : '0';
    header.field_E4_flags[22] = bDo_load_savegame_67D4F0 ? '1' : '0';
    header.field_E4_flags[23] = bSkip_audio_67D6BE ? '1' : '0';
    header.field_E4_flags[24] = bDo_debug_keys_67D6CF ? '1' : '0';
    header.field_E4_flags[25] = bSkip_trains_67D550 ? '1' : '0';
    header.field_E4_flags[26] = bSkip_buses_67D558 ? '1' : '0';
    header.field_E4_flags[27] = bSkip_fire_engines_67D53A ? '1' : '0';
    header.field_E4_flags[28] = bDo_police_1_67D568 ? '1' : '0';
    header.field_E4_flags[29] = bDo_police_2_67D569 ? '1' : '0';
    header.field_E4_flags[30] = bDo_police_3_67D56A ? '1' : '0';

    header_size = sizeof(header);
    File::AppendBufferToFile_4A6F50("test\\replay.rep", &header, &header_size);
}

MATCH_FUNC(0x4ce380)
void BurgerKing_67F8B0::LoadReplayHeader_4CE380(char_type bLoadDebug)
{
    u32 header_size = sizeof(ReplayHeader_10C);
    ReplayHeader_10C header;
    File::Global_Read_4A71C0(&header, header_size);

    if (!bIgnore_replay_header_67D4F3)
    {
        s32 major;
        s32 minor;
        sscanf(header.field_0_version, "v%d.%d", &major, &minor);
        gLucid_hamilton_67E8E0.SetMapName_4C5870(header.field_44_map_name);
        gLucid_hamilton_67E8E0.SetStyleName_4C5890(header.field_6C_style_name);
        gLucid_hamilton_67E8E0.SetScriptName_4C58B0(header.field_94_script_name);
        gLucid_hamilton_67E8E0.DebugStr_4C58D0(header.field_BC_debug_str);

        if (bLoadDebug)
        {
        bSkip_dummies_67D4EF = header.field_E4_flags[0] == '1';
        bDo_test_67D4F8 = header.field_E4_flags[1] == '1';
        bSkip_mission_67D4E5 = header.field_E4_flags[2] == '1';
        bDo_brian_test_67D544 = header.field_E4_flags[3] == '1';
        bDo_iain_test_67D4E9 = header.field_E4_flags[4] == '1';
        bSkip_traffic_lights_67D4EC = header.field_E4_flags[5] == '1';
        bSkip_recycling_67D575 = header.field_E4_flags[6] == '1';
        bLimit_recycling_67D4CA = header.field_E4_flags[7] == '1';
        bNo_annoying_chars_67D586 = header.field_E4_flags[8] == '1';
        bDo_mike_67D5CC = header.field_E4_flags[9] == '1';
        bDo_kill_phones_on_answer_67D6E8 = header.field_E4_flags[10] == '1';
        bGet_all_weapons_67D684 = header.field_E4_flags[11] == '1';
        bDont_get_car_back_67D4F5 = header.field_E4_flags[12] == '1';
        bSkip_ambulance_67D6C9 = header.field_E4_flags[13] == '1';
        bSkip_police_67D4F9 = header.field_E4_flags[14] == '1';
        bDo_invulnerable_67D4CB = header.field_E4_flags[15] == '1';
        bDo_free_shopping_67D6CD = header.field_E4_flags[16] == '1';
        bKeep_weapons_after_death_67D54D = header.field_E4_flags[17] == '1';
        bSkip_skidmarks_67D585 = header.field_E4_flags[18] == '1';
        bExplodingScoresOff_67D4FB = header.field_E4_flags[19] == '1';
        gDo_infinite_lives_67D4C9 = header.field_E4_flags[20] == '1';
        bDo_blood_67D5C5 = header.field_E4_flags[21] == '1';
        bDo_load_savegame_67D4F0 = header.field_E4_flags[22] == '1';
        bSkip_audio_67D6BE = header.field_E4_flags[23] == '1';
        bDo_debug_keys_67D6CF = header.field_E4_flags[24] == '1';
        bSkip_trains_67D550 = header.field_E4_flags[25] == '1';
        bSkip_buses_67D558 = header.field_E4_flags[26] == '1';
        bSkip_fire_engines_67D53A = header.field_E4_flags[27] == '1';
        bDo_police_1_67D568 = header.field_E4_flags[28] == '1';
        bDo_police_2_67D569 = header.field_E4_flags[29] == '1';
        bDo_police_3_67D56A = header.field_E4_flags[30] == '1';
        }
    }
}

MATCH_FUNC(0x4ce650)
void BurgerKing_67F8B0::VerifyAttractFilesExist_4CE650()
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
MATCH_FUNC(0x4ceac0)
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
            if (gpRng_67AB34->get_cur_rng_41CFE0() >= (u32)field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx)
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
            if (field_75344_bInputEnabled)
            {
                saved_input = *control_status;
                // saved_input equals *control_status here; the original tests both (je/jne pair in the asm)
                if ((saved_input & 0x1FF000) == 0 || (*control_status & 0x1FF000) == 0)
                {
                    *control_status = 0;
                    gBurgerKing_1_67B990->read_input_device_498DA0((s32*)control_status, 0);
                    *control_status |= saved_input;
                    BurgerKing_67F8B0::replay_save_4CEA40(control_status);
                }
            }
            break;

        case Replay_3:
            if (gpRng_67AB34->get_cur_rng_41CFE0() >= (u32)field_3C_rec_buff[field_75340_rec_buf_idx].field_0_rng_idx)
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
            if (field_75344_bInputEnabled)
            {
                saved_input = *control_status;
                // Same double test as above
                if ((saved_input & 0x1FF000) == 0 || (*control_status & 0x1FF000) == 0)
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
            if (field_75344_bInputEnabled)
            {
                if (!gGame_0x40_67E008->Is_game_state_Paused_2_416BC0())
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
            sprintf(gTmpBuffer_67C598, "%d: control_status = %d", gpRng_67AB34->field_0_rng, *control_status);
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
void BurgerKing_67F8B0::DisplayInputBits_4CED90()
{
    s8 i = 0;
    s32 bit_idx = 0;
    do
    {
        if (((1 << bit_idx) & field_4_input_bits) != 0)
        {
            swprintf(tmpBuff_67BD9C, L"Control %d", bit_idx);
            gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 10, 16 * (i + 1), gDebugFont_706600, 1);
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
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(L"PLAYBACK", -1, 0, gDebugFont_706600, 1);
    }
    else
    {
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(L"RECORDING", -1, 0, gDebugFont_706600, 1);
    }
    DisplayInputBits_4CED90();
}
