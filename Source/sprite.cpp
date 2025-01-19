#include "sprite.hpp"
#include "Globals.hpp"

EXPORT_VAR Sprite_8* gSprite_8_703820;
GLOBAL(gSprite_8_703820, 0x703820);

EXPORT_VAR Sprite_5D598* gSprite_5D598_70381C;
GLOBAL(gSprite_5D598_70381C, 0x70381C);

EXPORT_VAR Sprite_49B28* gSprite_49B28_703818;
GLOBAL(gSprite_49B28_703818, 0x703818);

EXPORT_VAR Sprite_3CC* gSprite_3CC_67AF1C;
GLOBAL(gSprite_3CC_67AF1C, 0x67AF1C);

EXPORT_VAR Sprite_1C24* gSprite_1C24_703B80;
GLOBAL(gSprite_1C24_703B80, 0x703B80);

EXPORT_VAR Car_3C* dword_703814;
GLOBAL(dword_703814, 0x703814);

STUB_FUNC(0x48f600)
char_type* Sprite_3CC::sub_48F600(u16* a2, u32* a3, u32* a4, u16* a5)
{
    return 0;
}

STUB_FUNC(0x48f690)
char_type* Sprite_3CC::sub_48F690(u32* a2)
{
    return 0;
}

STUB_FUNC(0x48f6e0)
s16* Sprite_3CC::sub_48F6E0(u16* a2)
{
    return 0;
}

STUB_FUNC(0x48f710)
s32* Sprite_3CC::sub_48F710()
{
    return 0;
}

STUB_FUNC(0x48f730)
Sprite_3CC::Sprite_3CC()
{
}

STUB_FUNC(0x48F7F0)
Sprite_3CC::~Sprite_3CC()
{
}

MATCH_FUNC(0x5a5860)
void Sprite_8::sub_5A5860()
{
    field_0 = 0;
}

STUB_FUNC(0x5a5870)
void Sprite_8::sub_5A5870()
{
}

STUB_FUNC(0x5a58a0)
Sprite_8::Sprite_8()
{
}

MATCH_FUNC(0x5a5b50)
Sprite_8::~Sprite_8()
{
    GTA2_DELETE_AND_NULL(gSprite_49B28_703818);
    GTA2_DELETE_AND_NULL(gSprite_5D598_70381C);
    GTA2_DELETE_AND_NULL(gSprite_3CC_67AF1C);
    GTA2_DELETE_AND_NULL(gSprite_1C24_703B80);
    dword_703814 = 0;
}

MATCH_FUNC(0x5a5c40)
Sprite_18::~Sprite_18()
{
}

MATCH_FUNC(0x5a5c50)
Sprite_18::Sprite_18()
{
    field_10 = 0;
}

MATCH_FUNC(0x5a5c20)
Sprite_1C24::~Sprite_1C24()
{
    field_0 = 0;
}

MATCH_FUNC(0x5a57a0)
s32 Sprite_4C::sub_5A57A0()
{
    field_2C_pNext = NULL;
    field_48 = 0;
    return 0;
}

STUB_FUNC(0x5a57b0)
Sprite_4C::Sprite_4C()
{
}

MATCH_FUNC(0x5a5840)
Sprite_4C::~Sprite_4C()
{
}

MATCH_FUNC(0x5a5be0)
Sprite_5D598::~Sprite_5D598()
{
    field_0_pFree = 0;
}

MATCH_FUNC(0x5A5C00)
Sprite_49B28::~Sprite_49B28()
{
    field_0_first_free = 0;
}

Sprite_49B28::Sprite_49B28()
{
}
