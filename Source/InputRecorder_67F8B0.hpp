#pragma once

#include "Function.hpp"
#include <WINDOWS.H>

struct DIDEVICEOBJECTDATA;

struct AttractFile
{
    char field_0_path[256];
};

// InputRecorder_67F8B0::field_38_replay_state
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
class InputDevices_1
{
  public:
    // TODO: Probably this call
    EXPORT void __stdcall free_input_devices_4987A0();

    EXPORT void read_keyboard_and_gamepad_498CC0();

    EXPORT void get_registry_controls_498C00();
    EXPORT bool game_pads_init_498BA0();
    EXPORT BOOL __stdcall make_input_devices_498800(HINSTANCE hInstance);
    EXPORT void set_game_pad_device_properties_4989C0();
    EXPORT void __stdcall input_devices_init_498C40(HINSTANCE hInstance);
    EXPORT void SetAltKeyState_498CB0(u32 alt_down);
    EXPORT bool acquire_input_device_498730(struct IDirectInputDeviceA* pGamePadDevice);
    EXPORT bool game_pad_read_498D20();
    EXPORT void AddKeyToInputBits_498C80(s32* pInputBits, DIDEVICEOBJECTDATA* device_data_keys);
    EXPORT void read_input_device_498DA0(s32* input_bits, u8 bUnknown);

    u8 field_0;
};
#pragma pack(pop)

// One recorded input change: the rng index it happened at, the input bits and the random number
struct InputRecord_C
{
    s32 field_0_rng_idx;
    s32 field_4_inputs;
    s32 field_8_rng_rnd;
};

// The text header at the start of a replay file
struct ReplayHeader_10C
{
    char_type field_0_version[8];
    char_type field_8_date[30];
    char_type field_26_computer_name[30];
    char_type field_44_map_name[40];
    char_type field_6C_style_name[40];
    char_type field_94_script_name[40];
    char_type field_BC_debug_str[40];
    char_type field_E4_flags[40];
};

// Reads the inputs (get_input_bits_4CEAC0) and records / plays back replays
class InputRecorder_67F8B0
{
  public:
    enum
    {
        k_max_records = 40000
    };

    EXPORT void StaticShutdown_4CDCD0(); // static dtor
    EXPORT void clear_inputs_4CDCE0();
    EXPORT void set_input_4CDCF0(s32 mask_idx);
    EXPORT void clear_input_4CDD10(s32 mask_idx);
    EXPORT bool should_ignore_input_4CDD80(s32 dinput_key);
    EXPORT bool should_ignore_input_4CDDF0(s32 dinput_key);
    EXPORT void save_replay_record_4CDE20(u32 inputs);
    EXPORT void SaveReplay_4CDED0();
    EXPORT void modify_inputs_4CDF30(s32 match_mask);
    EXPORT void AppendReplayHeader_4CDF70();
    EXPORT void LoadReplayHeader_4CE380(char_type bLoadDebug);
    EXPORT void VerifyAttractFilesExist_4CE650();
    EXPORT void GetNextAttrReplay_4CE6E0(char_type* pAttrPathOut);
    EXPORT void input_init_replay_4CE740(HINSTANCE hInstance);
    EXPORT void input_init_live_4CE880(HINSTANCE hInstance);
    EXPORT void Shutdown_4CEA00();
    EXPORT void replay_save_4CEA40(u32* input_bits);
    EXPORT u32 get_input_bits_4CEAC0();
    EXPORT void save_replay_inputs_4CED00(s32 input_old, s32 input_new);
    EXPORT void DisplayInputBits_4CED90();
    EXPORT bool RecOrPlayBackState_4CEDF0();
    EXPORT void ShowInput_4CEE10();

    // 9.6f 0x44AA60
    bool inlined_check()
    {
        if (field_38_replay_state == Playback_1 || field_38_replay_state == Attract_3)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x44C050
    inline bool IsInputSet_44C050(s32 mask_idx)
    {
        return (u32)(field_4_input_bits & field_8_input_masks[mask_idx]) > 0;
    }

    // 9.6f 0x44AA80
    inline s32 GetLastRecRngIdx_44AA80()
    {
        return field_3C_rec_buff[field_7533C_used_recs_count - 1].field_0_rng_idx;
    }

    char_type field_0_bShutDown;
    s32 field_4_input_bits;
    s32 field_8_input_masks[12];
    s32 field_38_replay_state;
    InputRecord_C field_3C_rec_buff[k_max_records];
    s32 field_7533C_used_recs_count;
    u32 field_75340_rec_buf_idx;
    // front end input on/off ??
    char_type field_75344_bInputEnabled;
    u8 field_75345_attract_idx;
};

EXTERN_GLOBAL(InputRecorder_67F8B0, gInputRecorder_67F8B0);

EXTERN_GLOBAL(InputDevices_1*, gInputDevices_67B990);
