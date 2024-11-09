#include "Zones_CA8.hpp"
#include "Function.hpp"
#include <string.h>

EXPORT_VAR Zones_CA8 *gZones_CA8_67E274;

MATCH_FUNC(0x4BF100);
Zone_144 *Zones_CA8::zone_by_name_4BF100(const char *pZoneName)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (_strnicmp(pZoneName, this->field_0[i].field_2_name, 4u) == 0)
        {
            return &this->field_0[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF170);
Zone_144 *Zones_CA8::next_free_zone_4BF170()
{
    for (u8 i = 0; i < 10; i++)
    {
        if (!field_0[i].field_0_used)
        {
            Zone_144 *result = &field_0[i];
            result->field_1_zone_idx = i;
            result->field_0_used = 1;
            return result;
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF1C0);
Zone_144 *Zones_CA8::ZoneByIdx_4BF1C0(u8 zone_idx)
{
    return &field_0[zone_idx];
}
