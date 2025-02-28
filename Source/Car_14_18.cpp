#include "Car_14_18.hpp"
#include "map_0x370.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

MATCH_FUNC(0x59dd60)
void Car_14_18::sub_59DD60()
{
    dword_6F5FD4 = field_0.ToInt();
    dword_6F5B80 = field_4.ToInt();
    dword_6F6108 = field_8.ToInt();
    dword_6F5F38 = field_C.ToInt();
}

MATCH_FUNC(0x59ddf0)
u32 Car_14_18::sub_59DDF0(Sprite* a2)
{
    Car_8* v3 = (Car_8*)&a2->sprite_4c_ptr->field_C;

    if (inline_sub_4B9FD0(&v3[0]) || inline_sub_4B9FD0(&v3[1]) 
        || inline_sub_4B9FD0(&v3[2]) || inline_sub_4B9FD0(&v3[3]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

STUB_FUNC(0x59de80)
char_type Car_14_18::sub_59DE80()
{
    return 0;
}

STUB_FUNC(0x59dee0)
void Car_14_18::sub_59DEE0(Fix16 a2, Fix16 a3)
{
}

STUB_FUNC(0x59df20)
bool Car_14_18::sub_59DF20(u8 a2)
{
    return 0;
}

STUB_FUNC(0x59dfb0)
bool Car_14_18::sub_59DFB0(s32 a2)
{
    return 0;
}
