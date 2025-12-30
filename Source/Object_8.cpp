#include "Object_8.hpp"

MATCH_FUNC(0x52ad50)
Object_8::Object_8()
{
    this->mpNext = 0;
    this->field_7_anim_speed_counter = 99;
    this->field_6_frame_counter = 99;
    this->field_4_timer = 99;
}

MATCH_FUNC(0x52ad70)
Object_8::~Object_8()
{
    mpNext = 0;
}