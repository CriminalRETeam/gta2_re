#include "KeyboardLayout.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include <stdlib.h>
#include <windows.h>
#include "enums.hpp"

DEFINE_GLOBAL(KeyboardLayout*, gKeybrd_0x204, 0x6F52F4);

char kTwoSpacesStr_620D2C[] = {' ', ' ', 0};

enum KeyboardLanguage
{
    KbLang_English = 0,
    KbLang_French = 1,
    KbLang_German = 2,
    KbLang_Italian = 3,
    KbLang_Spanish = 4,
    KbLang_Portuguese = 5,
    KbLang_Russian = 6,
};

MATCH_FUNC(0x4D5D70)
KeyboardLayout::KeyboardLayout()
{
    mKeyBoardLayout = 0;
    memset(mKeys, 0, sizeof(mKeys));
}

MATCH_FUNC(0x4D5D90)
KeyboardLayout::~KeyboardLayout()
{
}


static inline bool is_line_break_or_space(wchar_t letter)
{
    return letter == '\n' || letter == ' ';
}

MATCH_FUNC(0x4D5DA0)
void KeyboardLayout::ReadCfg(FILE* Stream, wchar_t* pOut)
{
    u8 i = 0;
    for (;;)
    {
        const wchar_t read_char = fgetc(Stream);
        if (is_line_break_or_space(read_char))
        {
            pOut[i] = 0;
            return;
        }
        pOut[i++] = read_char;
    }
}


MATCH_FUNC(0x4D5E00)
void KeyboardLayout::LoadKbCfg()
{
    mKeyBoardLayout = 0;
    mKeyBoardLayout = GetLayout();

    char_type FileName[128];
    const char_type* pKeyboardCfgFile;
    switch (mKeyBoardLayout)
    {
        case KbLang_English:
            pKeyboardCfgFile = "data\\keyboard\\eng_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile); // note: compiler de-dups this to the end for all cases
            break;
        case KbLang_French:
            pKeyboardCfgFile = "data\\keyboard\\fre_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
        case KbLang_German:
            pKeyboardCfgFile = "data\\keyboard\\ger_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
        case KbLang_Italian:
            pKeyboardCfgFile = "data\\keyboard\\ita_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
        case KbLang_Spanish:
            pKeyboardCfgFile = "data\\keyboard\\spa_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
        case KbLang_Portuguese:
            pKeyboardCfgFile = "data\\keyboard\\por_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
        case KbLang_Russian:
            pKeyboardCfgFile = "data\\keyboard\\rus_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;

        default:
            pKeyboardCfgFile = "data\\keyboard\\eng_kb.cfg";
            strcpy(FileName, pKeyboardCfgFile);
            break;
    }

    FILE* hConfigFile = fopen(FileName, "rt");
    if (!hConfigFile)
    {
        FatalError(Gta2Error::KeyboardTypeFileOpenError, "C:\\Splitting\\Gta2\\Source\\keybrd.cpp", 187);
    }

    for (s32 i = 0; i < 256; i++)
    {
        wchar_t token[16];
        ReadCfg(hConfigFile, token);
        if (token[0] == '~')
        {
            mKeys[i] = 0;
        }
        else if (token[0] != '`')
        {
            mKeys[i] = 0;
        }
        else
        {
            mKeys[i] = token[1];
        }
    }

    fclose(hConfigFile);
}

MATCH_FUNC(0x4D5F40)
wchar_t KeyboardLayout::GetKey(u16 key_idx)
{
    return mKeys[key_idx];
}


MATCH_FUNC(0x4D5F50)
void __stdcall KeyboardLayout::create()
{
    if (!gKeybrd_0x204)
    {
        gKeybrd_0x204 = new KeyboardLayout();
        if (!gKeybrd_0x204)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\keybrd.cpp", 244);
        }
    }
    gKeybrd_0x204->LoadKbCfg();
}

MATCH_FUNC(0x4D5FA0)
void KeyboardLayout::destroy()
{
    if (gKeybrd_0x204)
    {
        delete gKeybrd_0x204;
        gKeybrd_0x204 = NULL;
    }
}


MATCH_FUNC(0x4D5FD0)
void KeyboardLayout::RecreateIfLayoutChanged()
{
    if (gKeybrd_0x204)
    {
        s32 curLayout = gKeybrd_0x204->mKeyBoardLayout;
        if (curLayout == KeyboardLayout::GetLayout())
        {
            return;
        }
        KeyboardLayout::destroy();
        KeyboardLayout::create();
    }
    else
    {
        KeyboardLayout::create();
    }
}

// __stdcall, not __cdecl: with __cdecl VC6 gives the loads after GetKeyboardLayoutNameA no call-clobber
// latency, so the KLID byte loads are scheduled before `lea &Buffer` (see matching_quirks.md)
MATCH_FUNC(0x4D6000)
s32 __stdcall KeyboardLayout::GetLayout()
{
    s32 result;
    char_type Buffer[4];
    char_type szKLID[KL_NAMELENGTH];

    memcpy(Buffer, kTwoSpacesStr_620D2C, sizeof(Buffer));

    GetKeyboardLayoutNameA(szKLID);

    Buffer[0] = szKLID[6];
    Buffer[1] = szKLID[7];

    s32 langId;
    sscanf(Buffer, "%x", &langId);
    switch (langId)
    {
        case LANG_FRENCH:
            result = KbLang_French;
            break;

        case LANG_GERMAN:
            result = KbLang_German;
            break;

        case LANG_ITALIAN:
            result = KbLang_Italian;
            break;

        case LANG_SPANISH:
            result = KbLang_Spanish;
            break;

        case LANG_PORTUGUESE:
            result = KbLang_Portuguese;
            break;
        case LANG_RUSSIAN:
            result = KbLang_Russian;
            break;
        default:
            result = KbLang_English;
            break;
    }
    return result;
}