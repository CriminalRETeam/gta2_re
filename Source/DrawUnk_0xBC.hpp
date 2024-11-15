#pragma once

#include "Function.hpp"
#include "fix16.hpp"

// TODO: add later
class Car_3C;

class DrawUnk_0xBC
{
public:

	EXPORT char_type sub_4355D0(Car_3C* a2);
	EXPORT char_type sub_435630(s16* a2, s32 a3);
	EXPORT void sub_4357B0();
	EXPORT void sub_4357F0();
	EXPORT void sub_435810();
	EXPORT void sub_435830();
	EXPORT void sub_435840();
	EXPORT void sub_435860(DrawUnk_0xBC* a2);
	EXPORT s16* sub_4358D0(s16* a2);
	EXPORT s32* sub_435A20(s32* a2);
	EXPORT s32 sub_435A70(s32 a2, s32 a3, s32 a4);
	EXPORT s32 sub_435B90();
	EXPORT void sub_435D20( char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7);
	EXPORT void sub_435DD0();
	EXPORT u8 sub_435F90(s32 a2);
	EXPORT s32 sub_435FF0();
	EXPORT s32 sub_436110();
	EXPORT void sub_436120(s32 a2);
	EXPORT void sub_436140();
	EXPORT u32 sub_4361B0(u32 a2, u32 a3);
	EXPORT s32 sub_436200(s32 a2, s32* a3, s32* a4, s32* a5);
	EXPORT void sub_4364A0(s32 a2);
	EXPORT void sub_436540(s32* a2);
	EXPORT char_type sub_436710(char_type a2, char_type a3, char_type a4, char_type a5);
	EXPORT void sub_436830();
	EXPORT void sub_436860(s32 a2, s32* a3, s32* a4, s32 a5);
	EXPORT DrawUnk_0xBC(); // 4368E0
	EXPORT ~DrawUnk_0xBC(); // empty 4369E0
	EXPORT void sub_4397D0(Fix16 a2, Fix16 a3, Fix16 a4, Fix16 a5);
	EXPORT bool sub_58CF10(s32 a2, s32 a3);


	Fix16 field_0;
	Fix16 field_4_unk;
	Fix16 field_8;
	Fix16 field_C;
	Fix16 field_10;
	Fix16 field_14;
	Fix16 field_18;
	Fix16 field_1C;
	s32 field_20_right;
	s32 field_24_left;
	s32 field_28_bottom;
	s32 field_2C_top;
	s32 field_30;
	s32 field_34;
	s32 field_38;
	s32 field_3C;
	Fix16 field_40;
	char_type field_44;
	char_type field_45;
	char_type field_46;
	char_type field_47;
	s32 field_48;
	s32 field_4C;
	s32 field_50;
	s32 field_54;
	s32 field_58;
	s32 field_5C;
	s32 field_60;
	s32 field_64;
	s32 field_68;
	s32 field_6C;
	s32 field_70;
	s32 field_74;
	s32 field_78;
	s32 field_7C;
	s32 field_80;
	s32 field_84;
	Fix16 field_88;
	Fix16 field_8C;
	Fix16 field_90;
	Fix16 field_94;
	Fix16 field_98_x;
	Fix16 field_9C_y;
	Fix16 field_A0_z;
	Fix16 field_A4;
	s32 field_A8;
	Fix16 field_AC;
	Fix16 field_B0;
	Fix16 field_B4;
	Fix16 field_B8;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DrawUnk_0xBC, 0xBC)
