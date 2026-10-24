// Hook on the game's DirectInput keyboard: drops NumLock events (wine workaround), plus test input tools.
#pragma once

void InstallDInputHook(const unsigned* pFrameCounter);
