#pragma once

#include "Function.hpp"

class nostalgic_ellis_0x28;
class Object_2C;

class TrafficLight_20
{
  public:
    EXPORT void sub_5C1D00(u8 a2, u8 a3, u8 a4, u8 a5);
    EXPORT void UpdateLightsFromPhase_5C27A0(u8 phase);

    Object_2C* field_0;
    Object_2C* field_4;
    Object_2C* field_8;
    Object_2C* field_C;
    nostalgic_ellis_0x28* field_10;
    nostalgic_ellis_0x28* field_14;
    nostalgic_ellis_0x28* field_18;
    nostalgic_ellis_0x28* field_1C;
};

class TrafficLights_194
{
  public:
    EXPORT void sub_5C2910(u8 x, u8 y, u8 w, u8 h);
    EXPORT void TrafficLightsService_5C2950();
    EXPORT void ShowTrafficLightsInfo_5C2A10();
    EXPORT void sub_5C2AC0();
    EXPORT TrafficLights_194();
    EXPORT ~TrafficLights_194();
    bool is_phase_7_434960() const
    {
      return field_192_phase == 7;
    }
    TrafficLight_20* field_0_traffic_lights[100];
    u16 field_190_array_used_count;
    u8 field_192_phase;
    u8 field_193_timer;
};

EXTERN_GLOBAL(TrafficLights_194*, gTrafficLights_194_705958);
