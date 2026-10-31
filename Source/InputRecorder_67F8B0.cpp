#include "InputRecorder_67F8B0.hpp"
#include "Game.hpp"
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
#include "GameSession.hpp"
#include "input_bits.hpp"

#define ATTRACT_COUNT 3

extern DIDATAFORMAT gInputDeviceFormat_601A6C;

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

// DirectInput's pre-8 DIDEVICEOBJECTDATA (0x10 bytes, no uAppData): the 0x14 one of our SDK would
// make sGamePadDeviceData overlap gKeyboardDevice
struct mini_device_obj_data
{
    s32 dwOfs;
    s32 dwData;
    s32 dwTimeStamp;
    s32 dwSequence;
};

DEFINE_GLOBAL(InputRecorder_67F8B0, gInputRecorder, 0x67F8B0);
DEFINE_GLOBAL(InputDevices*, gInputDevices, 0x67B990);
DEFINE_STATIC_MEMBER(DWORD, InputDevices, sDeviceDataCount, 0x67B624);
DEFINE_STATIC_MEMBER(u8, InputDevices, sAltKeyDown, 0x67B80C);
DEFINE_STATIC_MEMBER(bool, InputDevices, sNeedKbAcquire, 0x67B66C);
DEFINE_STATIC_GLOBAL(mini_device_obj_data, sGamePadDeviceData, 0x67B5B0);

DEFINE_STATIC_MEMBER_ARRAY(s32, InputDevices, sControlDeviceType, 12, 0x67B91C);
DEFINE_STATIC_MEMBER_ARRAY(s32, InputDevices, sPlayerControlsBinding, 12, 0x67B6E8);

EXTERN_GLOBAL(DIDATAFORMAT, gKeyboardDataFormat);
EXTERN_GLOBAL(HINSTANCE, gHInstance);
EXTERN_GLOBAL(s32, gGTA2VersionMajor);
EXTERN_GLOBAL(s32, gGTA2VersionMinor);

DEFINE_GUID(GUID_SysKeyboard, 0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

const AttractFile attractFiles_62083C[ATTRACT_COUNT] = {"data\\attract\\attr1.rep", "data\\attract\\attr2.rep", "data\\attract\\attr3.rep"};

MATCH_FUNC(0x498730)
bool InputDevices::acquire_input_device(LPDIRECTINPUTDEVICEA pGamePadDevice)
{
    if (!pGamePadDevice)
    {
        return false;
    }

    if (sNeedKbAcquire && FAILED(gKeyboardDevice->Acquire()))
    {
        return false;
    }

    sDeviceDataCount = -1;

    const HRESULT hr = pGamePadDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &sDeviceDataCount, DIGDD_PEEK);
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
    {
        return SUCCEEDED(pGamePadDevice->Acquire()) ? true : false;
    }
    return true;
}

MATCH_FUNC(0x4987A0)
void InputDevices::free_input_devices()
{
    if (gpDInput)
    {
        if (gKeyboardDevice)
        {
            gKeyboardDevice->Unacquire();
            gKeyboardDevice->Release();
            gKeyboardDevice = NULL;
        }

        if (gGamePadDevice)
        {
            gGamePadDevice->Unacquire();
            gGamePadDevice->Release();
            gGamePadDevice = NULL;
        }
    }
}

MATCH_FUNC(0x498800)
BOOL __stdcall InputDevices::make_input_devices(HINSTANCE hInstance)
{
    DIPROPDWORD prop;

    sNeedKbAcquire = false;

    if (FAILED(gpDInput->CreateDevice(GUID_SysKeyboard, &gKeyboardDevice, NULL)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputCreateFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 268);
    }

    if (FAILED(gKeyboardDevice->SetDataFormat(&gKeyboardDataFormat)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetDataFormatFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 279);
    }

    if (FAILED(gKeyboardDevice->SetCooperativeLevel(gHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetCooperativeLevelFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 287);
    }

    prop.dwData = 10000; // buffer size

    prop.diph.dwSize = sizeof(DIPROPDWORD);
    prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    prop.diph.dwHow = DIPH_DEVICE;
    prop.diph.dwObj = 0;

    HRESULT hr = gKeyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
    if (FAILED(hr))
    {
        FatalDXError(hr, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 300);
    }

    prop.dwData = 0;
    gKeyboardDevice->GetProperty(DIPROP_BUFFERSIZE, &prop.diph);
    return !FAILED(gKeyboardDevice->Acquire());
}

MATCH_FUNC(0x498910)
EXPORT BOOL CALLBACK DirectInputDeviceEnumCallBack(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    const GUID guidInstanceCopy = lpddi->guidInstance;
    LPDIRECTINPUTDEVICEA tempDevice;
    if (SUCCEEDED(gpDInput->CreateDevice(guidInstanceCopy, &tempDevice, NULL)))
    {
        const HRESULT hr = tempDevice->QueryInterface(IID_IDirectInputDevice2A, reinterpret_cast<LPVOID*>(&gGamePadDevice));
        tempDevice->Release();

        if (SUCCEEDED(hr))
        {
            if (FAILED(gGamePadDevice->SetCooperativeLevel(gHwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
            {
                gGamePadDevice->Release();
                gGamePadDevice = NULL;
                return DIENUM_STOP;
            }

            if (FAILED(gGamePadDevice->SetDataFormat(&gInputDeviceFormat_601A6C)))
            {
                gGamePadDevice->Release();
                gGamePadDevice = NULL;
                return DIENUM_STOP;
            }
        }
    }
    return DIENUM_STOP;
}

MATCH_FUNC(0x4989C0)
void InputDevices::set_game_pad_device_properties()
{
    DIPROPDWORD prop;
    DIPROPRANGE range;
    DIDEVICEINSTANCEA instance;

    if (gGamePadDevice)
    {
        instance.dwSize = sizeof(DIDEVICEINSTANCEA);
        gGamePadDevice->GetDeviceInfo(&instance);
        gGamePadDevice->Unacquire();

        prop.dwData = 10000;
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_DEVICE;
        prop.diph.dwObj = 0;
        gGamePadDevice->SetProperty(DIPROP_SATURATION, &prop.diph);

        prop.dwData = 10000;
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_DEVICE;
        prop.diph.dwObj = 0;
        HRESULT hr = gGamePadDevice->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
        if (FAILED(hr))
        {
            FatalDXError(hr, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 434);
        }

        prop.dwData = 0;
        gGamePadDevice->GetProperty(DIPROP_BUFFERSIZE, &prop.diph);

        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwHow = DIPH_BYOFFSET;
        range.lMin = -1000;
        range.lMax = 1000;
        range.diph.dwObj = DIJOFS_X;
        if (FAILED(gGamePadDevice->SetProperty(DIPROP_RANGE, &range.diph)))
        {
            OutputDebugStringA("SetProperty(RANGE, X-Axis) failed.\n");
            return;
        }

        range.diph.dwObj = DIJOFS_Y;
        if (FAILED(gGamePadDevice->SetProperty(DIPROP_RANGE, &range.diph)))
        {
            OutputDebugStringA("SetProperty(RANGE, Y-Axis) failed.\n");
            return;
        }

        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwHow = DIPH_BYOFFSET;
        prop.dwData = 2500;
        prop.diph.dwObj = DIJOFS_X;
        if (FAILED(gGamePadDevice->SetProperty(DIPROP_DEADZONE, &prop.diph)))
        {
            OutputDebugStringA("SetProperty(DEADZONE, X-Axis) failed.\n");
            return;
        }

        prop.diph.dwObj = DIJOFS_Y;
        if (FAILED(gGamePadDevice->SetProperty(DIPROP_DEADZONE, &prop.diph)))
        {
            OutputDebugStringA("SetProperty(DEADZONE, Y-Axis) failed.\n");
            return;
        }

        gGamePadDevice->Acquire();
    }
}

MATCH_FUNC(0x498BA0)
bool InputDevices::game_pads_init()
{
    if (gpDInput)
    {
#if defined(__clang__) || (_MSC_VER <= 1200)
    #pragma comment(lib, "DInput.lib")
        // VC6-compatible path
        if (FAILED(DirectInputCreateA(gHInstance, 0x700, &gpDInput, NULL)))
        {
            return true;
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
                if (FAILED(pDirectInputCreateA(gHInstance, 0x700, &gpDInput, NULL)))
                {
                    return true;
                }
            }
        }
#endif
    }

    gpDInput->EnumDevices(DIDEVTYPE_JOYSTICK, DirectInputDeviceEnumCallBack, this, DIEDFL_ATTACHEDONLY);

    if (!gGamePadDevice)
    {
        return false;
    }
    set_game_pad_device_properties();
    return true;
}

MATCH_FUNC(0x498C00)
void InputDevices::get_registry_controls()
{
    LOCAL_STATIC_ARRAY_INIT(s32, sDefaultControls, 12, 0x61A9E4, DIK_UP COMMA DIK_DOWN COMMA DIK_LEFT COMMA DIK_RIGHT COMMA DIK_LCONTROL COMMA DIK_RETURN COMMA DIK_SPACE COMMA DIK_Z COMMA DIK_X COMMA DIK_TAB COMMA DIK_LMENU COMMA DIK_RSHIFT);
    for (u32 i = 0; i < 12; ++i)
    {
        const u32 setting = gRegistry.Get_Control_Setting(i, sDefaultControls[i]);
        sControlDeviceType[i] = setting >> 15;
        sPlayerControlsBinding[i] = static_cast<u8>(setting);
    }
}

MATCH_FUNC(0x498C40)
void __stdcall InputDevices::input_devices_init(HINSTANCE hInstance)
{
    get_registry_controls();

    sAltKeyDown = 0;

    if (!make_input_devices(hInstance))
    {
        sNeedKbAcquire = true;
    }
    game_pads_init();
}

MATCH_FUNC(0x498C80)
void InputDevices::AddKeyToInputBits(s32* pInputBits, DIDEVICEOBJECTDATA* device_data_keys)
{
    *pInputBits = (device_data_keys->dwOfs << input_bit::KeyShift) | *pInputBits;
    if ((device_data_keys->dwData & 0x80) != 0)
    {
        *pInputBits |= input_bit::KeyDown;
    }
}

// https://decomp.me/scratch/LbfoG ridiculous function to match
MATCH_FUNC(0x498CB0)
void InputDevices::SetAltKeyState(u32 alt_down)
{
    // An if/else on the DirectInput key state bit: VC6 turns it into a dword load + `shr $7,%al`,
    // the expression forms all give a byte load or `and $1`
    if (alt_down & 0x80)
    {
        sAltKeyDown = 1;
    }
    else
    {
        sAltKeyDown = 0;
    }
}

MATCH_FUNC(0x498CC0)
void InputDevices::flush_keyboard_and_gamepad()
{
    sDeviceDataCount = -1;
    if (gKeyboardDevice)
    {
        gKeyboardDevice->GetDeviceData(16, NULL, &sDeviceDataCount, 0);
    }

    sDeviceDataCount = -1;
    if (gGamePadDevice)
    {
        gGamePadDevice->GetDeviceData(16, NULL, &sDeviceDataCount, 0);
    }
}

MATCH_FUNC(0x498D20)
bool InputDevices::game_pad_read()
{
    DWORD num_items = -1;
    if (acquire_input_device(gGamePadDevice))
    {
        HRESULT jres = gGamePadDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &num_items, DIGDD_PEEK);
        if (jres >= DI_OK)
        {
            sprintf(gTmpBuffer, "%d: num_items = %d jres = %d", gpRng->get_cur_rng_41CFE0(), num_items, jres);
        }
    }
    else
    {
        num_items = 0;
    }
    return num_items > 0;
}

// https://decomp.me/scratch/75Pau
MATCH_FUNC(0x498DA0)
void InputDevices::read_input_device(s32* input_bits, bool bApplyKeyboardControls)
{
    LOCAL_STATIC(DIDEVICEOBJECTDATA, sKeyboardDeviceData, 0x67B610);
    bool bFirstPass = true;
    s32 bReleased = 0;
    DWORD padItems = 1;
    if (acquire_input_device(gKeyboardDevice) || acquire_input_device(gGamePadDevice))
    {
        memset(&sKeyboardDeviceData, 0, sizeof(mini_device_obj_data));
        memset(&sGamePadDeviceData, 0, sizeof(mini_device_obj_data));
        if (acquire_input_device(gGamePadDevice))
        {
            static_cast<LPDIRECTINPUTDEVICE2A>(gGamePadDevice)->Poll();
        }

        while (game_pad_read() || bFirstPass)
        {
            HRESULT kbResult;
            HRESULT padResult;
            bool bHandled = false;

            sDeviceDataCount = 0;
            if (gKeyboardDevice)
            {
                sDeviceDataCount = 1;
                kbResult = gKeyboardDevice->GetDeviceData(16, &sKeyboardDeviceData, &sDeviceDataCount, 0);
            }
            else
            {
                kbResult = -1;
            }

            if (gGamePadDevice && !sDeviceDataCount)
            {
                sDeviceDataCount = 1;
                gGamePadDevice->GetDeviceData(16, NULL, &padItems, DIGDD_PEEK);
                padResult = gGamePadDevice->GetDeviceData(16,
                                                                 reinterpret_cast<DIDEVICEOBJECTDATA*>(&sGamePadDeviceData),
                                                                 &sDeviceDataCount,
                                                                 0);
                if (bLog_directinput && sDeviceDataCount > 0)
                {
                    sprintf(gTmpBuffer,
                            "%d: input num_items = %d dwOfs = %d; data = %d",
                            gpRng->get_cur_rng_41CFE0(),
                            padItems,
                            sGamePadDeviceData.dwOfs,
                            sGamePadDeviceData.dwData);
                    gErrorLog_67C530.Write(gTmpBuffer);
                }
            }
            else
            {
                padResult = -1;
                padItems = 0;
            }

            if ((kbResult < 0 && padResult < 0) || sDeviceDataCount <= 0)
            {
                return;
            }

            if (sKeyboardDeviceData.dwOfs == DIK_LMENU)
            {
                SetAltKeyState(sKeyboardDeviceData.dwData);
            }
            else if ((sKeyboardDeviceData.dwOfs == DIK_SPACE || sKeyboardDeviceData.dwOfs == DIK_TAB ||
                      sKeyboardDeviceData.dwOfs == DIK_RETURN) &&
                     sAltKeyDown == 1)
            {
                return;
            }

            if (!gHud_2B00->IsInputKeyConsumed(sKeyboardDeviceData.dwOfs))
            {
                // The pad fields are read directly: VC6 keeps them in edi/edx across the loop and
                // reloads them after each set/clear call, as the original does
                // Check for player controls (up, down, shoot etc)
                for (s32 input = 0; input < 12; input++)
                {
                    // Reset for every input, so only the last binding (input 11) counts
                    bHandled = false;
                    if (kbResult == 0 && sControlDeviceType[input] == 0 &&
                        static_cast<u32>(sPlayerControlsBinding[input]) == sKeyboardDeviceData.dwOfs)
                    {
                        // Keyboard binding
                        bHandled = true;
                        if (sKeyboardDeviceData.dwData & 0x80)
                        {
                            if (bApplyKeyboardControls)
                            {
                                gInputRecorder.set_input(input);
                            }
                        }
                        else
                        {
                            if (bApplyKeyboardControls)
                            {
                                gInputRecorder.clear_input(input);
                            }
                        }
                    }
                    else if (padResult == 0 && sControlDeviceType[input] == 1)
                    {
                        // Gamepad binding
                        s32 binding = sPlayerControlsBinding[input];
                        bool bPressed = false;
                        bool bRelease = false;
                        bReleased = 0;
                        switch (binding)
                        {
                            case 224:
                                if (sGamePadDeviceData.dwOfs == 0)
                                {
                                    if (sGamePadDeviceData.dwData > -750)
                                    {
                                        bRelease = true;
                                    }
                                    else
                                    {
                                        bPressed = true;
                                    }
                                }
                                break;
                            case 225:
                                if (sGamePadDeviceData.dwOfs == 0)
                                {
                                    if (sGamePadDeviceData.dwData < 750)
                                    {
                                        bRelease = true;
                                    }
                                    else
                                    {
                                        bPressed = true;
                                    }
                                }
                                break;
                            case 226:
                                if (sGamePadDeviceData.dwOfs == 4)
                                {
                                    if (sGamePadDeviceData.dwData <= -750)
                                    {
                                        bPressed = true;
                                    }
                                    else if (gInputRecorder.IsInputSet_44C050(input))
                                    {
                                        bReleased = 1;
                                    }
                                }
                                break;
                            case 227:
                                if (sGamePadDeviceData.dwOfs == 4)
                                {
                                    if (sGamePadDeviceData.dwData >= 750)
                                    {
                                        bPressed = true;
                                    }
                                    else if (gInputRecorder.IsInputSet_44C050(input))
                                    {
                                        bReleased = 1;
                                    }
                                }
                                break;
                            default:
                                if (sGamePadDeviceData.dwOfs == binding + 48)
                                {
                                    if (sGamePadDeviceData.dwData & 0x80)
                                    {
                                        bPressed = true;
                                    }
                                    else
                                    {
                                        bRelease = true;
                                    }
                                }
                                break;
                        }

                        // The X axis and button cases set bRelease, the Y axis cases set bReleased directly.
                        // VC6 merges the stores of 1 later but counts them all, so bReleased gets the lowest slot
                        if (bRelease)
                        {
                            bReleased = 1;
                        }

                        if (bPressed)
                        {
                            bHandled = true;
                            if (bApplyKeyboardControls)
                            {
                                gInputRecorder.set_input(input);
                            }
                        }
                        else if (bReleased)
                        {
                            if (bApplyKeyboardControls && gInputRecorder.IsInputSet_44C050(input))
                            {
                                gInputRecorder.clear_input(input);
                            }
                            bHandled = true;
                        }
                    }
                }

            }

            if (bHandled || bReleased)
            {
                bFirstPass = false;
            }
            else
            {
                AddKeyToInputBits(input_bits, &sKeyboardDeviceData);
                bFirstPass = false;
                if (bLog_directinput)
                {
                    if (sKeyboardDeviceData.dwData & 0x80)
                    {
                        sprintf(gTmpBuffer, "%d: KEY ON : %d", gpRng->get_cur_rng_41CFE0(), sKeyboardDeviceData.dwOfs);
                    }
                    else
                    {
                        sprintf(gTmpBuffer, "%d: KEY OFF: %d", gpRng->get_cur_rng_41CFE0(), sKeyboardDeviceData.dwOfs);
                    }
                }
            }
        }
    }
}

// ================================================

MATCH_FUNC(0x4cdcd0)
void InputRecorder_67F8B0::StaticShutdown()
{
    gInputRecorder.Shutdown();
}

MATCH_FUNC(0x4cdce0)
void InputRecorder_67F8B0::clear_inputs()
{
    mInputBits &= ~0xFFFFF000;
}

MATCH_FUNC(0x4cdcf0)
void InputRecorder_67F8B0::set_input(s32 mask_idx)
{
    mInputBits |= mInputMasks[mask_idx];
}

MATCH_FUNC(0x4cdd10)
void InputRecorder_67F8B0::clear_input(s32 mask_idx)
{
    mInputBits &= ~mInputMasks[mask_idx];
}

MATCH_FUNC(0x4cdd30)
InputRecorder_67F8B0::InputRecorder_67F8B0()
{
    mbShutDown = false;
    mInputBits = 0;
    mbInputEnabled = true;
    mReplayState = Disabled_2;
    mAttractIdx = 0;
    memset(mInputMasks, 0, sizeof(mInputMasks));
    memset(mRecBuff, 0, sizeof(mRecBuff));
    mUsedRecsCount = 205;
    mRecBufIdx = 205;
    VerifyAttractFilesExist();
}

MATCH_FUNC(0x4cdd80)
bool InputRecorder_67F8B0::should_ignore_input(s32 dinput_key)
{
    return dinput_key == DIK_NUMPAD1 || dinput_key == DIK_NUMPAD2 || dinput_key == DIK_NUMPAD3 || dinput_key == DIK_NUMPAD4 ||
        dinput_key == DIK_NUMPAD5 || dinput_key == DIK_NUMPAD6 || dinput_key == DIK_NUMPAD7 || dinput_key == DIK_NUMPAD8 ||
        dinput_key == DIK_NUMPAD9 || dinput_key == DIK_MULTIPLY || dinput_key == DIK_SUBTRACT || dinput_key == DIK_ESCAPE ||
        dinput_key == DIK_F6 || dinput_key == DIK_ADD || gHud_2B00->IsQuitMessageInputKey(dinput_key);
}

MATCH_FUNC(0x4cddf0)
bool InputRecorder_67F8B0::should_record_input(s32 dinput_key)
{
    return !should_ignore_input(dinput_key) && dinput_key != DIK_ADD;
}

MATCH_FUNC(0x4cde20)
void InputRecorder_67F8B0::save_replay_record(u32 inputs)
{
    if ((inputs & input_bit::KeyBits) != 0)
    {
        if (should_ignore_input((inputs >> input_bit::KeyShift) & input_bit::KeyMask))
        {
            inputs &= 0xFFC00FFF;
        }
    }

    if (inputs)
    {
        if (mRecBufIdx < k_max_records && mReplayState == Live_0)
        {
            mRecBuff[mRecBufIdx].mInputs = inputs;
            mRecBuff[mRecBufIdx].mRngIdx = gpRng->get_cur_rng_41CFE0();
            mRecBuff[mRecBufIdx].mRngRnd = gpRng->get_rnd_45F9E0();

            if (bConstant_replay_save == 1)
            {
                const s32 rec_idx = this->mRecBufIdx;
                u32 rec_len = sizeof(InputRecord);
                File::AppendBufferToFile("test\\replay.rep", &mRecBuff[rec_idx], &rec_len);
            }

            mRecBufIdx++;
        }
    }
}

MATCH_FUNC(0x4cded0)
void InputRecorder_67F8B0::SaveReplay()
{
    if (bConstant_replay_save != 1 && bDo_release_replay && mReplayState != Disabled_2 && mRecBufIdx > 0 &&
        !IsPlayingBack())
    {
        size_t len = sizeof(InputRecord) * mRecBufIdx;
        File::AppendBufferToFile("test\\replay.rep", mRecBuff, &len);
    }
}

// https://decomp.me/scratch/c6Gy5
MATCH_FUNC(0x4cdf30)
void InputRecorder_67F8B0::modify_inputs(s32 match_mask)
{

    for (s32 i = 0; i < 12; i++)
    {
        if ((mInputMasks[i] & match_mask) != 0)
        {
            if ((mInputMasks[i] & mInputBits) != 0)
            {
                mInputBits &= ~mInputMasks[i];
            }
            else
            {
                mInputBits |= mInputMasks[i];
            }
        }
    }

    if (static_cast<s32>(match_mask & 0xFFFFF000) != 0)
    {
        this->mInputBits |= match_mask & 0xFFFFF000;
    }
}

MATCH_FUNC(0x4cdf70)
void InputRecorder_67F8B0::AppendReplayHeader()
{
    ReplayHeader header;

    memset(&header, 0, sizeof(header));
    DWORD computer_name_size = 29;
    sprintf(header.mVersion, "v%d.%d", gGTA2VersionMajor, gGTA2VersionMinor);

    time_t now = time(NULL);
    char_type* pDate = ctime(&now);
    pDate[strlen(pDate) - 1] = 0;
    sprintf(header.mDate, pDate);

    GetUserNameA(header.mComputerName, &computer_name_size);
    strcpy(header.mMapName, gGameSession.GetMapName());
    strcpy(header.mStyleName, gGameSession.GetStyleName());
    strcpy(header.mScriptName, gGameSession.GetScriptName());
    strcpy(header.mDebugStr, gGameSession.GetSaveFileName_4C5970());

    header.mVersion[7] = '\n';
    header.mDate[29] = '\n';
    header.mComputerName[29] = '\n';
    header.mMapName[39] = '\n';
    header.mStyleName[39] = '\n';
    header.mScriptName[39] = '\n';
    header.mDebugStr[39] = '\n';
    header.mFlags[39] = '\n';

    header.mFlags[0] = bSkip_dummies ? '1' : '0';
    header.mFlags[1] = bDo_test ? '1' : '0';
    header.mFlags[2] = bSkip_mission ? '1' : '0';
    header.mFlags[3] = bDo_brian_test ? '1' : '0';
    header.mFlags[4] = bDo_iain_test ? '1' : '0';
    header.mFlags[5] = bSkip_traffic_lights ? '1' : '0';
    header.mFlags[6] = bSkip_recycling ? '1' : '0';
    header.mFlags[7] = bLimit_recycling ? '1' : '0';
    header.mFlags[8] = bNo_annoying_chars ? '1' : '0';
    header.mFlags[9] = bDo_mike ? '1' : '0';
    header.mFlags[10] = bDo_kill_phones_on_answer ? '1' : '0';
    header.mFlags[11] = bGet_all_weapons ? '1' : '0';
    header.mFlags[12] = bDont_get_car_back ? '1' : '0';
    header.mFlags[13] = bSkip_ambulance ? '1' : '0';
    header.mFlags[14] = bSkip_police ? '1' : '0';
    header.mFlags[15] = bDo_invulnerable ? '1' : '0';
    header.mFlags[16] = bDo_free_shopping ? '1' : '0';
    header.mFlags[17] = bKeep_weapons_after_death ? '1' : '0';
    header.mFlags[18] = bSkip_skidmarks ? '1' : '0';
    header.mFlags[19] = bExplodingScoresOff ? '1' : '0';
    header.mFlags[20] = gDo_infinite_lives ? '1' : '0';
    header.mFlags[21] = bDo_blood ? '1' : '0';
    header.mFlags[22] = bDo_load_savegame ? '1' : '0';
    header.mFlags[23] = bSkip_audio ? '1' : '0';
    header.mFlags[24] = bDo_debug_keys ? '1' : '0';
    header.mFlags[25] = bSkip_trains ? '1' : '0';
    header.mFlags[26] = bSkip_buses ? '1' : '0';
    header.mFlags[27] = bSkip_fire_engines ? '1' : '0';
    header.mFlags[28] = bDo_police_1 ? '1' : '0';
    header.mFlags[29] = bDo_police_2 ? '1' : '0';
    header.mFlags[30] = bDo_police_3 ? '1' : '0';

    size_t header_size = sizeof(header);
    File::AppendBufferToFile("test\\replay.rep", &header, &header_size);
}

MATCH_FUNC(0x4ce380)
void InputRecorder_67F8B0::LoadReplayHeader(bool bLoadDebug)
{
    u32 header_size = sizeof(ReplayHeader);
    ReplayHeader header;
    File::Global_Read_4A71C0(&header, header_size);

    if (!bIgnore_replay_header)
    {
        s32 major;
        s32 minor;
        sscanf(header.mVersion, "v%d.%d", &major, &minor);
        gGameSession.SetMapName(header.mMapName);
        gGameSession.SetStyleName(header.mStyleName);
        gGameSession.SetScriptName(header.mScriptName);
        gGameSession.SetSaveFileName(header.mDebugStr);

        if (bLoadDebug)
        {
        bSkip_dummies = header.mFlags[0] == '1';
        bDo_test = header.mFlags[1] == '1';
        bSkip_mission = header.mFlags[2] == '1';
        bDo_brian_test = header.mFlags[3] == '1';
        bDo_iain_test = header.mFlags[4] == '1';
        bSkip_traffic_lights = header.mFlags[5] == '1';
        bSkip_recycling = header.mFlags[6] == '1';
        bLimit_recycling = header.mFlags[7] == '1';
        bNo_annoying_chars = header.mFlags[8] == '1';
        bDo_mike = header.mFlags[9] == '1';
        bDo_kill_phones_on_answer = header.mFlags[10] == '1';
        bGet_all_weapons = header.mFlags[11] == '1';
        bDont_get_car_back = header.mFlags[12] == '1';
        bSkip_ambulance = header.mFlags[13] == '1';
        bSkip_police = header.mFlags[14] == '1';
        bDo_invulnerable = header.mFlags[15] == '1';
        bDo_free_shopping = header.mFlags[16] == '1';
        bKeep_weapons_after_death = header.mFlags[17] == '1';
        bSkip_skidmarks = header.mFlags[18] == '1';
        bExplodingScoresOff = header.mFlags[19] == '1';
        gDo_infinite_lives = header.mFlags[20] == '1';
        bDo_blood = header.mFlags[21] == '1';
        bDo_load_savegame = header.mFlags[22] == '1';
        bSkip_audio = header.mFlags[23] == '1';
        bDo_debug_keys = header.mFlags[24] == '1';
        bSkip_trains = header.mFlags[25] == '1';
        bSkip_buses = header.mFlags[26] == '1';
        bSkip_fire_engines = header.mFlags[27] == '1';
        bDo_police_1 = header.mFlags[28] == '1';
        bDo_police_2 = header.mFlags[29] == '1';
        bDo_police_3 = header.mFlags[30] == '1';
        }
    }
}

MATCH_FUNC(0x4ce650)
void InputRecorder_67F8B0::VerifyAttractFilesExist()
{
    const AttractFile* pAttractFile = &attractFiles_62083C[0];
    for (s32 i = 0; i < ATTRACT_COUNT; i++)
    {
        struct _finddata_t fileInfo;
        long hFind = _findfirst(pAttractFile->mPath, &fileInfo);
        if (hFind == -1)
        {
            strcpy(gErrStr, pAttractFile->mPath);
            FatalError_4A38C0(Gta2Error::CouldntFindAttractModeFile, "C:\\Splitting\\Gta2\\Source\\input.cpp", 524);
        }
        _findclose(hFind);
        ++pAttractFile;
    }
}

MATCH_FUNC(0x4ce6e0)
void InputRecorder_67F8B0::GetNextAttrReplay(char_type* pAttrPathOut)
{
    strcpy(pAttrPathOut, attractFiles_62083C[mAttractIdx].mPath);

    if (++mAttractIdx >= ATTRACT_COUNT)
    {
        mAttractIdx = 0;
    }
}

MATCH_FUNC(0x4ce740)
void InputRecorder_67F8B0::input_init_replay(HINSTANCE hInstance)
{
    char_type FileName[256];

    this->mbShutDown = false;
    InputRecorder_67F8B0::GetNextAttrReplay(FileName);
    this->mInputMasks[0] = input_bit::Forward;
    this->mInputMasks[1] = input_bit::Backward;
    this->mInputMasks[2] = input_bit::Left;
    this->mInputMasks[3] = input_bit::Right;
    this->mInputMasks[4] = input_bit::Attack;
    this->mInputMasks[5] = input_bit::EnterExit;
    this->mInputMasks[6] = input_bit::HandbrakeJump;
    this->mInputMasks[7] = input_bit::PrevWeapon;
    this->mInputMasks[8] = input_bit::NextWeapon;
    this->mInputMasks[9] = input_bit::Special1;
    this->mInputMasks[10] = input_bit::Special2;
    this->mInputMasks[11] = input_bit::Special3;
    this->mInputBits = 0;
    this->mReplayState = Attract_3;
    gInputDevices->input_devices_init(hInstance);
    memset(this->mRecBuff, 0, sizeof(this->mRecBuff));
    this->mRecBufIdx = 0;
    bConstant_replay_save = false;
    File::Global_Open(FileName);
    LoadReplayHeader(false);

    u32 bufLen = sizeof(mRecBuff);
    const u32 remainderSize = File::Global_ReadRemainder(this->mRecBuff, &bufLen);
    File::Global_Close();
    this->mUsedRecsCount = remainderSize / sizeof(InputRecord);
    if (sizeof(InputRecord) * (remainderSize / sizeof(InputRecord)) != remainderSize)
    {
        FatalError_4A38C0(Gta2Error::ReplayFileTooLarge, "C:\\Splitting\\Gta2\\Source\\input.cpp", 616, remainderSize);
    }

    if (this->mUsedRecsCount == 0)
    {
        FatalError_4A38C0(Gta2Error::EmptyReplayFile, "C:\\Splitting\\Gta2\\Source\\input.cpp", 621);
    }
}

MATCH_FUNC(0x4ce880)
void InputRecorder_67F8B0::input_init_live(HINSTANCE hInstance)
{
    mbShutDown = false;
    mInputMasks[0] = input_bit::Forward;
    mInputMasks[1] = input_bit::Backward;
    mInputMasks[2] = input_bit::Left;
    mInputMasks[3] = input_bit::Right;
    mInputMasks[4] = input_bit::Attack;
    mInputMasks[5] = input_bit::EnterExit;
    mInputMasks[6] = input_bit::HandbrakeJump;
    mInputMasks[7] = input_bit::PrevWeapon;
    mInputMasks[8] = input_bit::NextWeapon;
    mInputMasks[9] = input_bit::Special1;
    mInputMasks[10] = input_bit::Special2;
    mInputMasks[11] = input_bit::Special3;
    mInputBits = 0;
    mReplayState = Live_0;

    gInputDevices = new InputDevices();
    if (!gInputDevices)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\input.cpp", 675);
    }

    gInputDevices->input_devices_init(hInstance);
    memset(mRecBuff, 0, sizeof(mRecBuff));

    mRecBufIdx = 0;

    if (bPlay_replay == 1)
    {
        bConstant_replay_save = false;
        File::Global_Open("test\\replay.rep");
        InputRecorder_67F8B0::LoadReplayHeader(true);
        u32 size = sizeof(mRecBuff);
        size_t remainderSize = File::Global_ReadRemainder(mRecBuff, &size);
        File::Global_Close();
        mUsedRecsCount = remainderSize / sizeof(InputRecord);
        if (sizeof(InputRecord) * (remainderSize / sizeof(InputRecord)) != remainderSize)
        {
            FatalError_4A38C0(Gta2Error::ReplayFileTooLarge, "C:\\Splitting\\Gta2\\Source\\input.cpp", 702, remainderSize);
        }
        mReplayState = mUsedRecsCount != 0;
    }
    else
    {
        if (bDo_release_replay)
        {
            File::CreateFile_4A7000("test\\replay.rep");
            InputRecorder_67F8B0::AppendReplayHeader();
        }
        mReplayState = Live_0;
    }
    if (bConstant_replay_save)
    {
        if (bPlay_replay == 1)
        {
            if (bDo_release_replay)
            {
                File::CreateFile_4A7000("test\\replay.rep");
                InputRecorder_67F8B0::AppendReplayHeader();
            }
        }
    }
}

MATCH_FUNC(0x4cea00)
void InputRecorder_67F8B0::Shutdown()
{
    if (!mbShutDown)
    {
        mbShutDown = true;
        gInputDevices->free_input_devices();
        SaveReplay();
        GTA2_DELETE_AND_NULL(gInputDevices);
    }
}

MATCH_FUNC(0x4cea40)
void InputRecorder_67F8B0::replay_save(u32* input_bits)
{
    if ((*input_bits & 0xFFFFF000) == 0x37000)
    {
        mReplayState = Live_0;

        if (bDo_release_replay)
        {
            File::CreateFile_4A7000("test\\replay.rep");
            AppendReplayHeader();
        }

        // Clear out the unused records
        memset(&this->mRecBuff[this->mRecBufIdx],
               0,
               sizeof(InputRecord) * ((GTA2_COUNTOF(mRecBuff) - mRecBufIdx)));
        if (bConstant_replay_save)
        {
            SaveReplay();
        }
    }
}

// https://decomp.me/scratch/t5tNu
MATCH_FUNC(0x4ceac0)
u32 InputRecorder_67F8B0::get_input_bits()
{
    s32 inputs;
    u32 saved_input = mInputBits;
    u32* control_status = reinterpret_cast<u32*>(&mInputBits);
    InputRecorder_67F8B0::clear_inputs();

    s32 replay_state = mReplayState;

    *control_status &= ~input_bit::KeyDown;
    u32 remove_bit = *control_status;

    switch (replay_state)
    {
        case Playback_1:
            if (static_cast<u32>(gpRng->get_cur_rng_41CFE0()) >= static_cast<u32>(mRecBuff[mRecBufIdx].mRngIdx))
            {
                inputs = mRecBuff[mRecBufIdx].mInputs;
                mRecBufIdx++;
                if (mRecBufIdx >= static_cast<u32>(mUsedRecsCount))
                {
                    if (bDo_exit_after_replay)
                    {
                        gGame_0x40->ExitGameNoBonus(0, GameExitType::CloseGameImmediately_1);
                    }
                    else
                    {
                        if (bDo_release_replay)
                        {
                            File::CreateFile_4A7000("test\\replay.rep");
                            InputRecorder_67F8B0::AppendReplayHeader();
                        }
                        mReplayState = Live_0;
                    }
                }

                if (mRecBufIdx < 2)
                {
                    saved_input = 0;
                }
                else
                {
                    saved_input = mInputMasks[3 * mRecBufIdx + 8];
                }
                InputRecorder_67F8B0::modify_inputs(inputs);
            }
            if (mbInputEnabled)
            {
                saved_input = *control_status;
                // saved_input equals *control_status here; the original tests both (je/jne pair in the asm)
                if ((saved_input & input_bit::KeyBits) == 0 || (*control_status & input_bit::KeyBits) == 0)
                {
                    *control_status = 0;
                    gInputDevices->read_input_device(reinterpret_cast<s32*>(control_status), false);
                    *control_status |= saved_input;
                    InputRecorder_67F8B0::replay_save(control_status);
                }
            }
            break;

        case Attract_3:
            if (static_cast<u32>(gpRng->get_cur_rng_41CFE0()) >= static_cast<u32>(mRecBuff[mRecBufIdx].mRngIdx))
            {
                inputs = mRecBuff[mRecBufIdx].mInputs;
                mRecBufIdx++;
                if (mRecBufIdx >= static_cast<u32>(mUsedRecsCount))
                {
                    gGame_0x40->ExitGameNoBonus(0, GameExitType::ReplayExit_6);
                }

                if (mRecBufIdx < 2)
                {
                    saved_input = 0;
                }
                else
                {
                    saved_input = mInputMasks[3 * mRecBufIdx + 8];
                }
                InputRecorder_67F8B0::modify_inputs(inputs);
            }
            if (mbInputEnabled)
            {
                saved_input = *control_status;
                // Same double test as above
                if ((saved_input & input_bit::KeyBits) == 0 || (*control_status & input_bit::KeyBits) == 0)
                {
                    *control_status = 0;
                    gInputDevices->read_input_device(reinterpret_cast<s32*>(control_status), false);
                    if ((*control_status & 0xFFFFF000) != 0)
                    {
                        gGame_0x40->ExitGameNoBonus(0, GameExitType::ReplayExit_6);
                    }
                    *control_status |= saved_input;
                    InputRecorder_67F8B0::replay_save(control_status);
                }
            }
            break;

        case Live_0:
            if (mbInputEnabled)
            {
                if (!gGame_0x40->IsPaused_416BC0())
                {
                    gInputDevices->read_input_device(reinterpret_cast<s32*>(control_status), true);
                    InputRecorder_67F8B0::save_replay_inputs(*control_status, saved_input);
                }
                else
                {
                    gInputDevices->read_input_device(reinterpret_cast<s32*>(control_status), false);
                    if (InputRecorder_67F8B0::should_record_input((static_cast<u32>(*control_status) >> input_bit::KeyShift) & input_bit::KeyMask))
                    {
                        *control_status = remove_bit;
                    }
                }
                InputRecorder_67F8B0::replay_save(control_status);
            }
            break;
    }

    if (bLog_input)
    {
        if (*control_status != saved_input)
        {
            sprintf(gTmpBuffer, "%d: control_status = %d", gpRng->get_cur_rng_41CFE0(), *control_status);
            gErrorLog_67C530.Write(gTmpBuffer);
        }
    }
    return *control_status;
}

MATCH_FUNC(0x4ced00)
void InputRecorder_67F8B0::save_replay_inputs(s32 input_old, s32 input_new)
{
    u32 calc_inputs = 0;
    if (input_old != input_new)
    {
        if ((input_new & 1) != (input_old & 1))
        {
            calc_inputs |= 1u;
        }
        if ((input_new & 2) != (input_old & 2))
        {
            calc_inputs |= 2u;
        }
        if ((input_new & 4) != (input_old & 4))
        {
            calc_inputs |= 4u;
        }
        if ((input_new & 8) != (input_old & 8))
        {
            calc_inputs |= 8u;
        }
        if ((input_new & 0x10) != (input_old & 0x10))
        {
            calc_inputs |= 0x10u;
        }
        if ((input_new & 0x20) != (input_old & 0x20))
        {
            calc_inputs |= 0x20u;
        }
        if ((input_new & 0x40) != (input_old & 0x40))
        {
            calc_inputs |= 0x40u;
        }
        if ((input_new & 0x80u) != (input_old & 0x80u))
        {
            calc_inputs |= 0x80;
        }
        if ((input_new & 0x100) != (input_old & 0x100))
        {
            calc_inputs |= 0x100;
        }
        if ((input_new & 0x200) != (input_old & 0x200))
        {
            calc_inputs |= 0x200;
        }
        if ((input_new & 0x400) != (input_old & 0x400))
        {
            calc_inputs |= 0x400;
        }
        if ((input_new & 0x800) != (input_old & 0x800))
        {
            calc_inputs |= 0x800;
        }
        if ((input_old & input_bit::KeyBits) != 0)
        {
            calc_inputs |= mInputBits & 0xFFFFF000;
        }
        if (calc_inputs)
        {
            InputRecorder_67F8B0::save_replay_record(calc_inputs);
        }
    }
}

MATCH_FUNC(0x4ced90)
void InputRecorder_67F8B0::DisplayInputBits()
{
    s8 i = 0;
    s32 bit_idx = 0;
    do
    {
        if (((1 << bit_idx) & mInputBits) != 0)
        {
            swprintf(tmpBuff, L"Control %d", bit_idx);
            gHud_2B00->mTexts.DisplayText(tmpBuff, 10, 16 * (i + 1), gDebugFont, 1);
        }
        ++i;
        ++bit_idx;
    } while (i < 12);
}

MATCH_FUNC(0x4cedf0)
bool InputRecorder_67F8B0::IsPlayingBack() const
{
    if (mReplayState == Playback_1 || mReplayState == Attract_3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4cee10)
void InputRecorder_67F8B0::ShowInput()
{
    if (IsPlayingBack())
    {
        gHud_2B00->mTexts.DisplayText(L"PLAYBACK", -1, 0, gDebugFont, 1);
    }
    else
    {
        gHud_2B00->mTexts.DisplayText(L"RECORDING", -1, 0, gDebugFont, 1);
    }
    DisplayInputBits();
}
