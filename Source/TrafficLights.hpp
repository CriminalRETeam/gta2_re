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

class Light;
class Object;

class TrafficLight
{
  public:
    EXPORT void Init_5C1D00(u8 x, u8 y, u8 w, u8 h);
    EXPORT void UpdateLightsFromPhase_5C27A0(u8 phase);

    Object* field_0_north_headlight_obj; // 0x0
    Object* field_4_south_headlight_obj; // 0x4
    Object* field_8_west_headlight_obj; // 0x8
    Object* field_C_east_headlight_obj; // 0xC
    Light* field_10_north_light; // 0x10
    Light* field_14_south_light; // 0x14
    Light* field_18_west_light; // 0x18
    Light* field_1C_east_light; // 0x1C
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrafficLight, 0x20);

class TrafficLights
{
  private:
    EXPORT void AddTrafficLight_5C2910(u8 x, u8 y, u8 w, u8 h);

  public:
    EXPORT void TrafficLightsService_5C2950();

  private:
    EXPORT void ShowTrafficLightsInfo_5C2A10();

  public:
    EXPORT void CreateFromMapZones_5C2AC0();
    EXPORT TrafficLights();
    EXPORT ~TrafficLights();
    bool ArePedsCrossing_434960() const
    {
      return field_192_phase == traffic_light_phase::peds_crossing_7;
    }
    TrafficLight* field_0_traffic_lights[100]; // 0x0
    u16 field_190_array_used_count; // 0x190
    u8 field_192_phase; // 0x192
    u8 field_193_timer; // 0x193
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrafficLights, 0x194);

EXTERN_GLOBAL(TrafficLights*, gTrafficLights_194_705958);
