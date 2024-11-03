#pragma once

// Pattern: 0x90, 0x90 0xB8 [addr bytes x4] 0xB8 [status bytes x4] 0x90 0x90
#define FUNC_MARKER_ASM(addr, status)\
        __asm nop\
        __asm nop\
        __asm mov eax, addr\
        __asm mov eax, status\
        __asm nop\
        __asm nop\

#define MATCH_FUNC(addr) __declspec(naked) void Marker_##addr() { FUNC_MARKER_ASM(addr, 1) }
#define STUB_FUNC(addr) __declspec(naked) void Marker_##addr() { FUNC_MARKER_ASM(addr, 0) }

#define GTA2_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define GTA2_COUNTOF_S(x) ((s32)GTA2_COUNTOF(x))
