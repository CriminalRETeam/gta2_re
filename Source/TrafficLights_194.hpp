#pragma once

#include "Function.hpp"

class nostalgic_ellis_0x28;

class TrafficLight_20
{
  public:
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    nostalgic_ellis_0x28* field_10;
    nostalgic_ellis_0x28* field_14;
    nostalgic_ellis_0x28* field_18;
    nostalgic_ellis_0x28* field_1C;
};

class TrafficLights_194
{
  public:
    EXPORT void sub_5C2910(u8 x, u8 y, u8 w, u8 h);
    EXPORT void sub_5C2950();
    EXPORT void ShowTrafficLightsInfo_5C2A10();
    EXPORT void sub_5C2AC0();
    EXPORT TrafficLights_194();
    EXPORT ~TrafficLights_194();
    TrafficLight_20* field_0_traffic_lights[100];
    u16 field_190_array_used_count;
    u8 field_192_phase;
    u8 field_193_timer;
};

EXPORT_VAR extern TrafficLights_194* gTrafficLights_194_705958;
