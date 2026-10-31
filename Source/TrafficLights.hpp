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
    EXPORT void Init(u8 x, u8 y, u8 w, u8 h);
    EXPORT void UpdateLightsFromPhase(u8 phase);

    Object* mNorthHeadlightObj; // 0x0
    Object* mSouthHeadlightObj; // 0x4
    Object* mWestHeadlightObj; // 0x8
    Object* mEastHeadlightObj; // 0xC
    Light* mNorthLight; // 0x10
    Light* mSouthLight; // 0x14
    Light* mWestLight; // 0x18
    Light* mEastLight; // 0x1C
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrafficLight, 0x20);

class TrafficLights
{
  private:
    EXPORT void AddTrafficLight(u8 x, u8 y, u8 w, u8 h);

  public:
    EXPORT void TrafficLightsService();

  private:
    EXPORT void ShowTrafficLightsInfo();

  public:
    EXPORT void CreateFromMapZones();
    EXPORT TrafficLights();
    EXPORT ~TrafficLights();
    // 9.6f 0x434960
    bool ArePedsCrossing_434960() const
    {
      return mPhase == traffic_light_phase::peds_crossing_7;
    }
    TrafficLight* mTrafficLights[100]; // 0x0
    u16 mArrayUsedCount; // 0x190
    u8 mPhase; // 0x192
    u8 mTimer; // 0x193
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrafficLights, 0x194);

EXTERN_GLOBAL(TrafficLights*, gTrafficLights_194);
