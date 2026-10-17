#pragma once

#include "Function.hpp"

namespace traffic_light_phase
{
enum
{
    north_south_green_1 = 1,
    north_south_amber_2 = 2,
    north_south_red_3 = 3,
    east_west_green_4 = 4,
    east_west_amber_5 = 5,
    east_west_red_6 = 6,
    peds_crossing_7 = 7, // all red, pedestrians cross
    all_red_8 = 8,
};
} // namespace traffic_light_phase

class Light_28;
class Object_2C;

class TrafficLight_20
{
  public:
    EXPORT void Init_5C1D00(u8 x, u8 y, u8 w, u8 h);
    EXPORT void UpdateLightsFromPhase_5C27A0(u8 phase);

    Object_2C* field_0_north_headlight_obj;
    Object_2C* field_4_south_headlight_obj;
    Object_2C* field_8_west_headlight_obj;
    Object_2C* field_C_east_headlight_obj;
    Light_28* field_10_north_light;
    Light_28* field_14_south_light;
    Light_28* field_18_west_light;
    Light_28* field_1C_east_light;
};

class TrafficLights_194
{
  public:
    EXPORT void AddTrafficLight_5C2910(u8 x, u8 y, u8 w, u8 h);
    EXPORT void TrafficLightsService_5C2950();
    EXPORT void ShowTrafficLightsInfo_5C2A10();
    EXPORT void CreateFromMapZones_5C2AC0();
    EXPORT TrafficLights_194();
    EXPORT ~TrafficLights_194();
    bool ArePedsCrossing_434960() const
    {
      return field_192_phase == traffic_light_phase::peds_crossing_7;
    }
    TrafficLight_20* field_0_traffic_lights[100];
    u16 field_190_array_used_count;
    u8 field_192_phase;
    u8 field_193_timer;
};

EXTERN_GLOBAL(TrafficLights_194*, gTrafficLights_194_705958);
