#include "DrawUnk_0xBC.hpp"
#include "Function.hpp"

STUB_FUNC(0x4355D0)
char_type DrawUnk_0xBC::sub_4355D0(Car_3C* a2)
{
	return 0;
}

STUB_FUNC(0x435630)
char_type DrawUnk_0xBC::sub_435630(s16* a2, s32 a3)
{
	return 0;
}

MATCH_FUNC(0x4357B0)
void DrawUnk_0xBC::sub_4357B0()
{
	field_88 = field_98_x;
	field_8C = field_9C_y;
	field_90 = field_A0_z;
	field_94 = field_A4;
}

s32 dword_676894;
s32 dword_676678;

MATCH_FUNC(0x4357F0)
void DrawUnk_0xBC::sub_4357F0()
{
	if (field_40 < dword_676894)
	{
		field_40 += dword_676678;
	}
}

s32 dword_676818;

STUB_FUNC(0x435810)
void DrawUnk_0xBC::sub_435810()
{
	if (field_40 > dword_676818)
	{
		field_40 -= dword_676678;
	}
}

MATCH_FUNC(0x435830)
void DrawUnk_0xBC::sub_435830()
{
	field_40 = dword_676818;
}

s32 dword_676898;

MATCH_FUNC(0x435840)
void DrawUnk_0xBC::sub_435840()
{
	if (field_18 < dword_676818)
	{
		field_18 = dword_676818;
	}

	if (field_18 > dword_676898)
	{
		field_18 = dword_676898;
	}
}

MATCH_FUNC(0x435860)
void DrawUnk_0xBC::sub_435860(DrawUnk_0xBC* a2)
{
	field_10 += a2->field_98_x - a2->field_88;
	field_14 += a2->field_9C_y - a2->field_8C;
	field_18 += a2->field_A0_z - a2->field_90;
	field_1C += a2->field_A4 - a2->field_94;
	sub_435840();
}

STUB_FUNC(0x4358D0)
s16* DrawUnk_0xBC::sub_4358D0(s16* a2)
{
	return 0;
}

STUB_FUNC(0x435A20)
s32* DrawUnk_0xBC::sub_435A20(s32* a2)
{
	return 0;
}

STUB_FUNC(0x435A70)
s32 DrawUnk_0xBC::sub_435A70(s32 a2, s32 a3, s32 a4)
{
	return 0;
}

STUB_FUNC(0x435B90)
s32 DrawUnk_0xBC::sub_435B90()
{
	return 0;
}

s32 dword_67671C;
s32 dword_67681C;

STUB_FUNC(0x435D20)
void DrawUnk_0xBC::sub_435D20(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7)
{
	field_10 = field_0;
	field_14 = field_4_unk;
	field_18 = field_8;
	field_1C = field_C;

	if (a2)
	{
		field_14 -= dword_67671C;
	}

	if (a3)
	{
		field_14 += dword_67671C;
	}

	if (a4)
	{
		field_10 -= dword_67671C;
	}

	if (a5)
	{
		field_10 += dword_67671C;
	}

	if (a6)
	{
		field_18 += dword_67681C;
	}

	if (a7)
	{
		field_18 -= dword_67681C;
	}

	sub_435840();
}

MATCH_FUNC(0x435DD0)
void DrawUnk_0xBC::sub_435DD0()
{
	field_98_x = field_0;
	field_9C_y = field_4_unk;
	field_A0_z = field_8;
	field_A4 = field_C;

	field_AC = dword_676818;
	field_B0 = dword_676818;
	field_B4 = dword_676818;
	field_B8 = dword_676818;
}

STUB_FUNC(0x435F90)
u8 DrawUnk_0xBC::sub_435F90(s32 a2)
{
	return 0;
}

STUB_FUNC(0x435FF0)
s32 DrawUnk_0xBC::sub_435FF0()
{
	return 0;
}

STUB_FUNC(0x436110)
s32 DrawUnk_0xBC::sub_436110()
{
	return 0;
}

STUB_FUNC(0x436120)
void DrawUnk_0xBC::sub_436120(s32 a2)
{

}

STUB_FUNC(0x436140)
void DrawUnk_0xBC::sub_436140()
{
}

STUB_FUNC(0x4361B0)
u32 DrawUnk_0xBC::sub_4361B0(u32 a2, u32 a3)
{
	return 0;
}

STUB_FUNC(0x436200)
s32 DrawUnk_0xBC::sub_436200(s32 a2, s32* a3, s32* a4, s32* a5)
{
	return 0;
}

STUB_FUNC(0x4364A0)
void DrawUnk_0xBC::sub_4364A0(s32 a2)
{

}

STUB_FUNC(0x436540)
void DrawUnk_0xBC::sub_436540(s32* a2)
{

}

STUB_FUNC(0x436710)
char_type DrawUnk_0xBC::sub_436710(char_type a2, char_type a3, char_type a4, char_type a5)
{
	return 0;
}

STUB_FUNC(0x436830)
s32 DrawUnk_0xBC::sub_436830()
{
	return 0;
}

STUB_FUNC(0x436860)
void DrawUnk_0xBC::sub_436860(s32 a2, s32* a3, s32* a4, s32 a5)
{
}

STUB_FUNC(0x4368E0)
DrawUnk_0xBC::DrawUnk_0xBC() // 4368E0
{

}

STUB_FUNC(0x4369E0)
DrawUnk_0xBC::~DrawUnk_0xBC() // empty 4369E0
{

}

STUB_FUNC(0x4397D0)
s32 DrawUnk_0xBC::sub_4397D0(s32 a2, s32 a3, s32 a4, s32 a5)
{
	return 0;
}

STUB_FUNC(0x58CF10)
s32 DrawUnk_0xBC::sub_58CF10(s32 a2, s32 a3)
{
	return 0;
}

