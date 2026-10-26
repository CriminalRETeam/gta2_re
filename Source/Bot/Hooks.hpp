#pragma once

#include "types.hpp"

// The bot's seams into the game, all Detours attached before WinMain_5E53F0 runs:
//
//  ParseCommandLine_4DA320            after it: the boot overrides (skip frontend/draw/audio,
//                                     the map, style and script names, replay recording on)
//  ExecuteGame_4DA780                 before it: Bot::OnFrame (observe, snapshot, restore, decide)
//  InputDevices::read_input_device    replaced: the bot's input bits instead of DirectInput, so
//                                     the game records them into its replay like key presses
//  malloc/calloc/realloc/free         HeapTracker
namespace Hooks
{
bool Attach();
}
