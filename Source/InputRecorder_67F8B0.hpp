#pragma once

#include "Function.hpp"
#include <WINDOWS.H>

struct DIDEVICEOBJECTDATA;

struct AttractFile
{
    char_type mPath[256]; // 0x0
};

// InputRecorder_67F8B0::mReplayState
enum ReplayState
{
    Live_0 = 0,     // reads the real input devices and records them (test\replay.rep)
    Playback_1 = 1, // plays back test\replay.rep, goes back to Live_0 when it ends
    Disabled_2 = 2, // nothing is recorded
    Attract_3 = 3,  // plays an attract replay, any input exits
};

#pragma pack(push)
#pragma pack(1)
// DirectInput keyboard and game pads (the object has no data of its own)
class InputDevices
{
  public:
    STATIC_MEMBER_ARRAY(s32, sPlayerControlsBinding, 12);
    STATIC_MEMBER(bool, sNeedKbAcquire);
    STATIC_MEMBER(DWORD, sDeviceDataCount);
    STATIC_MEMBER_ARRAY(s32, sControlDeviceType, 12);
    STATIC_MEMBER(u8, sAltKeyDown);
    EXPORT void __stdcall free_input_devices();

    EXPORT void flush_keyboard_and_gamepad();

    EXPORT void get_registry_controls();
    EXPORT bool game_pads_init();
    EXPORT BOOL __stdcall make_input_devices(HINSTANCE hInstance);
    EXPORT void set_game_pad_device_properties();
    EXPORT void __stdcall input_devices_init(HINSTANCE hInstance);
    EXPORT void SetAltKeyState(u32 alt_down);
    EXPORT bool acquire_input_device(struct IDirectInputDeviceA* pGamePadDevice);
    EXPORT bool game_pad_read();
    EXPORT void AddKeyToInputBits(s32* pInputBits, DIDEVICEOBJECTDATA* device_data_keys);
    EXPORT void read_input_device(s32* input_bits, bool bApplyKeyboardControls);

    u8 mUnused0; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(InputDevices, 0x1);
#pragma pack(pop)

// One recorded input change: the rng index it happened at, the input bits and the random number
struct InputRecord
{
    s32 mRngIdx; // 0x0
    s32 mInputs; // 0x4
    s32 mRngRnd; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(InputRecord, 0xC);

// The text header at the start of a replay file
struct ReplayHeader
{
    char_type mVersion[8]; // 0x0
    char_type mDate[30]; // 0x8
    char_type mComputerName[30]; // 0x26
    char_type mMapName[40]; // 0x44
    char_type mStyleName[40]; // 0x6C
    char_type mScriptName[40]; // 0x94
    char_type mDebugStr[40]; // 0xBC
    char_type mFlags[40]; // 0xE4
};
GTA2_ASSERT_SIZEOF_ALWAYS(ReplayHeader, 0x10C);

// Reads the inputs (get_input_bits) and records / plays back replays
class InputRecorder_67F8B0
{
  public:
    enum
    {
        k_max_records = 40000
    };

    EXPORT InputRecorder_67F8B0();

    ~InputRecorder_67F8B0()
    {
        Shutdown();
    }

    EXPORT void StaticShutdown(); // static dtor
    EXPORT void clear_inputs();
    EXPORT void set_input(s32 mask_idx);
    EXPORT void clear_input(s32 mask_idx);
    EXPORT bool should_ignore_input(s32 dinput_key);
    EXPORT bool should_record_input(s32 dinput_key);
    EXPORT void save_replay_record(u32 inputs);
    EXPORT void SaveReplay();
    EXPORT void modify_inputs(s32 match_mask);
    EXPORT void AppendReplayHeader();
    EXPORT void LoadReplayHeader(bool bLoadDebug);
    EXPORT void VerifyAttractFilesExist();
    EXPORT void GetNextAttrReplay(char_type* pAttrPathOut);
    EXPORT void input_init_replay(HINSTANCE hInstance);
    EXPORT void input_init_live(HINSTANCE hInstance);
    EXPORT void Shutdown();
    EXPORT void replay_save(u32* input_bits);
    EXPORT u32 get_input_bits();
    EXPORT void save_replay_inputs(s32 input_old, s32 input_new);
    EXPORT void DisplayInputBits();
    EXPORT bool IsPlayingBack() const;
    EXPORT void ShowInput();

    // 9.6f 0x44AA60
    bool inlined_check() const
    {
        if (mReplayState == Playback_1 || mReplayState == Attract_3)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x44C050
    inline bool IsInputSet(s32 mask_idx) const
    {
        return static_cast<u32>(mInputBits & mInputMasks[mask_idx]) > 0;
    }

    // 9.6f 0x44AA80
    inline s32 GetLastRecRngIdx() const
    {
        return mRecBuff[mUsedRecsCount - 1].mRngIdx;
    }

    bool mbShutDown; // 0x0
    s32 mInputBits; // 0x4
    s32 mInputMasks[12]; // 0x8
    s32 mReplayState; // 0x38
    InputRecord mRecBuff[k_max_records]; // 0x3C
    s32 mUsedRecsCount; // 0x7533C
    u32 mRecBufIdx; // 0x75340
    char_type mbInputEnabled; // 0x75344
    u8 mAttractIdx; // 0x75345
};

EXTERN_GLOBAL(InputRecorder_67F8B0, gInputRecorder);

EXTERN_GLOBAL(InputDevices*, gInputDevices);
