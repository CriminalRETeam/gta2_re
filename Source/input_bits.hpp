#pragma once

// Input word of Player::field_4_inputs and InputRecorder_67F8B0::field_4_input_bits: one bit per game control
// (InputRecorder_67F8B0::field_8_input_masks), then the DirectInput scan code of a key event and its key-down flag.
namespace input_bit
{
enum
{
    Forward = 0x1,
    Backward = 0x2,
    Left = 0x4,
    Right = 0x8,
    Attack = 0x10,
    EnterExit = 0x20,
    HandbrakeJump = 0x40,
    PrevWeapon = 0x80,
    NextWeapon = 0x100,
    Special1 = 0x200,
    Special2 = 0x400,
    Special3 = 0x800,
    KeyShift = 12,
    KeyMask = 0x1FF, // scan code, after >> KeyShift
    KeyBits = 0x1FF000,
    KeyDown = 0x200000,
};
} // namespace input_bit
