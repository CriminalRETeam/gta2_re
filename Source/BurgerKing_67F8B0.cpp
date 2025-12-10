#include "BurgerKing_67F8B0.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "input.hpp"
#include "rng.hpp"
#include <io.h>

#define ATTRACT_COUNT 3

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

DEFINE_GLOBAL(BurgerKing_67F8B0, gBurgerKing_67F8B0, 0x67F8B0);
DEFINE_GLOBAL(BurgerKing_1*, gBurgerKing_1_67B990, 0x67B990);

DEFINE_GLOBAL(DWORD, dword_67B624, 0x67B624);

const AttractFile attractFiles_62083C[ATTRACT_COUNT] = {"data\\attract\\attr1.rep", "data\\attract\\attr2.rep", "data\\attract\\attr3.rep"};

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
    dword_67B624 = -1;
    if (gKeyboardDevice_67B5C0)
    {
        gKeyboardDevice_67B5C0->GetDeviceData(16, 0, &dword_67B624, 0);
    }

    dword_67B624 = -1;
    if (gGamePadDevice_67B6C0)
    {
        gGamePadDevice_67B6C0->GetDeviceData(16, 0, &dword_67B624, 0);
    }
}

STUB_FUNC(0x498C40)
void __stdcall BurgerKing_1::input_devices_init_498C40(HINSTANCE hInstance)
{
    NOT_IMPLEMENTED;
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
        if (field_75340_rec_buf_idx < 40000 && !this->field_38_replay_state)
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
    if (bConstant_replay_save_67D5C4 != 1 && bDo_release_replay_67D4EB && field_38_replay_state != 2 && field_75340_rec_buf_idx > 0 &&
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
    this->field_38_replay_state = 3;
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

STUB_FUNC(0x4ce880)
void BurgerKing_67F8B0::sub_4CE880(HINSTANCE a2)
{
    NOT_IMPLEMENTED;
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
        field_38_replay_state = 0;

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

STUB_FUNC(0x4ceac0)
u32 BurgerKing_67F8B0::sub_4CEAC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ced00)
void BurgerKing_67F8B0::sub_4CED00(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
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
    if (field_38_replay_state == 1 || field_38_replay_state == 3)
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
