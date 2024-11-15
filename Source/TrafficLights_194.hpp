#pragma once

#include "Function.hpp"

class TrafficLight_20;

class TrafficLights_194
{
public:
    EXPORT void sub_5C2910(u8 x, u8 y, u8 w, u8 h);
    EXPORT void sub_5C2950();
    EXPORT void ShowTrafficLightsInfo_5C2A10();
    EXPORT void sub_5C2AC0();
    EXPORT TrafficLights_194* ctor_5C2B00();
    EXPORT void dtor_5C2B30();
    TrafficLight_20* field_0_traffic_lights[100];
    s16 field_190_array_used_count;
    u8 field_192_phase;
    u8 field_193_timer;
};
