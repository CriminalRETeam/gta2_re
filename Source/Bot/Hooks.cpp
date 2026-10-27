#include "Hooks.hpp"
#include "Bot.hpp"
#include "BotLog.hpp"
#include "HeapTracker.hpp"
#include <windows.h>
#include <string.h>
#include "3rdParty/Detours/include/detours.h"
#include "InputRecorder_67F8B0.hpp"
#include "BotCompat.hpp"

// Defined in winmain.cpp without a header declaration
void __stdcall ParseCommandLine_4DA320(char_type* pCommandLine);
bool ExecuteGame_4DA780();

namespace
{
void(__stdcall* Real_ParseCommandLine)(char_type*) = ParseCommandLine_4DA320;
bool (*Real_ExecuteGame)() = ExecuteGame_4DA780;
void(__fastcall* Real_ReadInputDevice)(InputDevices*, void*, s32*, u8) = NULL;

void __stdcall Hook_ParseCommandLine(char_type* pCommandLine)
{
    Real_ParseCommandLine(pCommandLine);
    Bot_Get()->ApplyBootOverrides();
}

bool Hook_ExecuteGame()
{
    if (Bot_Get()->Paused())
    {
        return false; // the overlay holds the game: no logic frame, so the world, the bot and its clock stand still
    }
    Bot_Get()->OnFrame();
    return Real_ExecuteGame();
}

// __thiscall seen as __fastcall: this in ecx, the unused edx, then the stack arguments
void __fastcall Hook_ReadInputDevice(InputDevices* /*pThis*/, void* /*edx*/, s32* input_bits, u8 /*bApplyKeyboardControls*/)
{
    *input_bits = static_cast<s32>(Bot_Get()->InputBits() & input_bit::ControlBits);
}

template<class T>
void* MemberToPtr(T fn)
{
    void* p = NULL;
    memcpy(&p, &fn, sizeof(p));
    return p;
}
} // namespace

bool Hooks::Attach()
{
    Real_ReadInputDevice = reinterpret_cast<void(__fastcall*)(InputDevices*, void*, s32*, u8)>(MemberToPtr(&InputDevices::read_input_device_498DA0));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<PVOID*>(&Real_ParseCommandLine), reinterpret_cast<PVOID>(Hook_ParseCommandLine));
    DetourAttach(reinterpret_cast<PVOID*>(&Real_ExecuteGame), reinterpret_cast<PVOID>(Hook_ExecuteGame));
    DetourAttach(reinterpret_cast<PVOID*>(&Real_ReadInputDevice), reinterpret_cast<PVOID>(Hook_ReadInputDevice));
    const LONG err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        BotLog("hooks: DetourTransactionCommit failed: %ld", err);
        return false;
    }
    BotLog("hooks: attached ParseCommandLine_4DA320, ExecuteGame_4DA780, read_input_device_498DA0");
    return HeapTracker::AttachDetours();
}
