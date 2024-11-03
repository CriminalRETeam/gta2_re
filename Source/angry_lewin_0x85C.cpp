#include "Function.hpp"
#include "angry_lewin_0x85C.hpp"
#include "debug.hpp"

MATCH_FUNC(0x4881E0)
u8 angry_lewin_0x85C::GetIdx_4881E0()
{
	return field_2E_idx;
}

STUB_FUNC(0x5645B0)
void angry_lewin_0x85C::sub_5645B0(Car_BC* a2)
{

}

STUB_FUNC(0x564610)
char_type angry_lewin_0x85C::sub_564610(Car_BC* a2, char_type a3)
{
	return 'a';
}

STUB_FUNC(0x564680)
u32* angry_lewin_0x85C::sub_564680(s32 a2)
{
	return 0;
}

STUB_FUNC(0x564710)
void angry_lewin_0x85C::sub_564710(Car_BC* a2, s32 a3)
{

}

STUB_FUNC(0x564790)
void angry_lewin_0x85C::sub_564790(s32 idx)
{

}

STUB_FUNC(0x5647D0)
void angry_lewin_0x85C::sub_5647D0()
{

}

MATCH_FUNC(0x5648F0)
keen_bhaskara_0x30* angry_lewin_0x85C::sub_5648F0()
{
	return field_788_idx == -1 ? NULL : field_718[field_788_idx];
}

STUB_FUNC(0x564910)
void angry_lewin_0x85C::sub_564910(keen_bhaskara_0x30* a2)
{

}

STUB_FUNC(0x564940)
char_type angry_lewin_0x85C::sub_564940()
{
	return 'a';
}

STUB_FUNC(0x564960)
char_type angry_lewin_0x85C::sub_564960(s32 a2, u8 a3)
{
	return 'a';
}

STUB_FUNC(0x5649D0)
u16 angry_lewin_0x85C::sub_5649D0(char_type a2, char_type a3)
{
	return 0;
}

STUB_FUNC(0x564AD0)
char_type angry_lewin_0x85C::sub_564AD0(Car_BC* a2)
{
	return 'a';
}

STUB_FUNC(0x564B60)
s32 angry_lewin_0x85C::sub_564B60()
{
	return 0;
}

STUB_FUNC(0x564B80)
s32 angry_lewin_0x85C::sub_564B80()
{
	return 0;
}

MATCH_FUNC(0x564C00)
u16 angry_lewin_0x85C::sub_564C00()
{
	sub_564B80();

	if (field_788_idx >= 15)
	{
		field_16 = field_788_idx;
		field_788_idx = field_14;
	}

	if (field_18 >= 15)
	{
		field_16 = field_18;
		field_18 = field_14;
	}

	return sub_5649D0(0, 0);
}

STUB_FUNC(0x564C50)
void angry_lewin_0x85C::sub_564C50(s32 a2)
{

}

u8 byte_67D57C;
u8 byte_67D539;

MATCH_FUNC(0x564CC0)
void angry_lewin_0x85C::sub_564CC0()
{
	for (s32 i = 0; i < GTA2_COUNTOF_S(field_6F4); i++)
	{
		if (i == 11 && byte_67D539)
		{
			continue;
		}

		if (i == 7 && byte_67D57C)
		{
			continue;
		}

		field_6F4[i] = 0;
	}
}

STUB_FUNC(0x564CF0)
void angry_lewin_0x85C::sub_564CF0()
{

}

STUB_FUNC(0x564D60)
char_type angry_lewin_0x85C::sub_564D60(s32 a2)
{
	return 'a';
}

STUB_FUNC(0x565070)
u16 angry_lewin_0x85C::sub_565070()
{
	return 0;
}

STUB_FUNC(0x5651F0)
s32 angry_lewin_0x85C::sub_5651F0(s32 a2)
{
	return 0;
}

STUB_FUNC(0x565310)
void angry_lewin_0x85C::sub_565310()
{

}

STUB_FUNC(0x5653E0)
void angry_lewin_0x85C::sub_5653E0()
{

}

STUB_FUNC(0x565490)
s32 angry_lewin_0x85C::sub_565490(cool_nash_0x294* pPed)
{
	return 0;
}

MATCH_FUNC(0x565740)
void angry_lewin_0x85C::SetInputs_565740(u32 input)
{
	field_4_inputs = input;
}

STUB_FUNC(0x565770)
void angry_lewin_0x85C::sub_565770(u8 count)
{

}

STUB_FUNC(0x565860)
void angry_lewin_0x85C::sub_565860()
{

}

STUB_FUNC(0x565890)
void angry_lewin_0x85C::sub_565890(u16 action)
{

}

STUB_FUNC(0x566380)
void angry_lewin_0x85C::sub_566380(u16 a2)
{

}

STUB_FUNC(0x566520)
char_type angry_lewin_0x85C::sub_566520()
{
	return 'a';
}

STUB_FUNC(0x566820)
void angry_lewin_0x85C::sub_566820()
{

}

STUB_FUNC(0x5668D0)
void angry_lewin_0x85C::sub_5668D0(cool_nash_0x294* a2)
{

}

STUB_FUNC(0x566C30)
char_type angry_lewin_0x85C::sub_566C30(Car_BC* a2)
{
	return 'a';
}

STUB_FUNC(0x566C80)
char_type angry_lewin_0x85C::sub_566C80(u32* a2)
{
	return 'a';
}

STUB_FUNC(0x566EE0)
void angry_lewin_0x85C::sub_566EE0(char_type a2)
{

}

STUB_FUNC(0x5670B0)
s32 angry_lewin_0x85C::sub_5670B0()
{
	return 0;
}

STUB_FUNC(0x567130)
void angry_lewin_0x85C::sub_567130()
{

}

MATCH_FUNC(0x567850)
void angry_lewin_0x85C::sub_567850()
{
	if (field_29 == 0)
	{
		field_29 = 1;
		field_2C = 70;
		return;
	}

	if (--field_2C == 0)
	{
		field_29 = 0;
		sub_5695A0();
	}
}

STUB_FUNC(0x5679E0)
void angry_lewin_0x85C::sub_5679E0()
{

}

STUB_FUNC(0x568520)
const char* angry_lewin_0x85C::sub_568520()
{
	return "gobbins";
}

STUB_FUNC(0x568630)
void angry_lewin_0x85C::sub_568630()
{
	/*
	infallible_turing* field_4C_pUnk; // eax

	field_4C_pUnk = field_4C_pUnk;
	if (field_4C_pUnk)
	{
		field_4C_pUnk->field_C_pAny = &field_14C_view_camera;
	}
	*/
}

STUB_FUNC(0x568670)
char_type angry_lewin_0x85C::sub_568670()
{
	return 'a';
}

STUB_FUNC(0x5686D0)
char_type angry_lewin_0x85C::sub_5686D0(DrawUnk_0xBC* a2)
{
	return 'a';
}

STUB_FUNC(0x568730)
s32 angry_lewin_0x85C::sub_568730()
{
	return 0;
}

STUB_FUNC(0x5687F0)
void angry_lewin_0x85C::sub_5687F0()
{

}

STUB_FUNC(0x569410)
s32 angry_lewin_0x85C::sub_569410()
{
	return 0;
}

STUB_FUNC(0x569530)
s32 angry_lewin_0x85C::sub_569530()
{
	return 0;
}

STUB_FUNC(0x5695A0)
void angry_lewin_0x85C::sub_5695A0()
{

}

STUB_FUNC(0x569600)
char_type angry_lewin_0x85C::sub_569600(Car_BC* a2)
{
	return 'a';
}

STUB_FUNC(0x5696D0)
void angry_lewin_0x85C::sub_5696D0(s32 a2)
{

}

STUB_FUNC(0x569840)
s32 angry_lewin_0x85C::sub_569840(u8* a2, u8* a3, u8* a4)
{
	return 0;
}

STUB_FUNC(0x5698E0)
Car_BC* angry_lewin_0x85C::sub_5698E0()
{
	return 0;
}

STUB_FUNC(0x569920)
s32 angry_lewin_0x85C::sub_569920(u32* a2, s32* a3, s32* a4)
{
	return 0;
}

STUB_FUNC(0x5699F0)
s32 angry_lewin_0x85C::sub_5699F0(s32 a2)
{
	return 0;
}

STUB_FUNC(0x569A10)
void angry_lewin_0x85C::sub_569A10()
{

}

STUB_FUNC(0x569C20)
void angry_lewin_0x85C::sub_569C20()
{

}

STUB_FUNC(0x569CB0)
void angry_lewin_0x85C::sub_569CB0()
{

}

STUB_FUNC(0x569E70)
void angry_lewin_0x85C::sub_569E70()
{

}

MATCH_FUNC(0x569F00)
char* angry_lewin_0x85C::GetDeathText_569F00()
{
	switch (field_44_death_type)
	{
		case 0:
		case 1:
			return "wasted";
			break;
		case 2:
			return "fried";
			break;
		case 3:
			return "nicked";
			break;
		case 4:
			return "shocked";
			break;
		default:
			return 0;
			break;
	}
}

STUB_FUNC(0x569F40)
s32 angry_lewin_0x85C::sub_569F40()
{
	return 0;
}

MATCH_FUNC(0x569FF0)
s32 angry_lewin_0x85C::sub_569FF0()
{
	field_2F = 1;
	return sub_569F40();
}

MATCH_FUNC(0x56A000)
void angry_lewin_0x85C::SetUnknown_56A000()
{
	field_2F = 0;
}

MATCH_FUNC(0x56A010)
void angry_lewin_0x85C::sub_56A010()
{
	field_31 = 1;
}

MATCH_FUNC(0x56A020)
void angry_lewin_0x85C::sub_56A020()
{
	field_31 = 0;
}

MATCH_FUNC(0x56A030)
void angry_lewin_0x85C::sub_56A030()
{
	field_30 = 1;
}

MATCH_FUNC(0x56A040)
void angry_lewin_0x85C::sub_56A040()
{
	field_30 = 0;
}

STUB_FUNC(0x56A0F0)
s32 angry_lewin_0x85C::sub_56A0F0()
{
	return 0;
}

STUB_FUNC(0x56A1A0)
zealous_borg* angry_lewin_0x85C::sub_56A1A0(s32 a2)
{
	return 0;
}

STUB_FUNC(0x56A310)
void angry_lewin_0x85C::sub_56A310(s32 a2)
{

}

STUB_FUNC(0x56A490)
void angry_lewin_0x85C::sub_56A490()
{

}

MATCH_FUNC(0x56A6D0)
void angry_lewin_0x85C::sub_56A6D0()
{
	// sets some car info to 0
	field_78 = 0;
	field_79 = 0;
	field_7A = 0;
	field_7B = 0;
	field_7C = 0;
	field_7D = 0;
	field_7E = 0;
	field_7F = 0;
	field_80 = 0;
	field_81 = 0;
	field_82 = 0;
	field_83 = 0;
	field_84 = 0;
	field_88 = 0;
	field_87 = 0;
	field_89 = 0;
	field_8D = 0;
	field_8A = 0;
	field_8B = 0;
	field_8C = 0;
}

STUB_FUNC(0x56A740)
angry_lewin_0x85C::angry_lewin_0x85C(u8 a2)
{

}

STUB_FUNC(0x56A940)
angry_lewin_0x85C::~angry_lewin_0x85C()
{

}