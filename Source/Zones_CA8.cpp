#include "Zones_CA8.hpp"
#include "Function.hpp"
#include "map_0x370.hpp"
#include <string.h>

EXPORT_VAR Zones_CA8 *gZones_CA8_67E274;

MATCH_FUNC(0x4BF090);
void Zone_144::set_name_4BF090(const char *pName, u8 nameLen)
{
    strncpy(this->field_2_name, pName, nameLen);
    this->field_2_name[nameLen] = 0; // NULL terminate the string
}

// =====================

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

MATCH_FUNC(0x4BF1E0);
void Zones_CA8::alloc_map_zone_4BF1E0(gmp_map_zone *pMapZone)
{
    if (!zone_by_name_4BF100(pMapZone->field_6_name))
    {
        Zone_144 *pFreeZone = next_free_zone_4BF170();
        pFreeZone->set_name_4BF090(pMapZone->field_6_name, pMapZone->field_5_name_length);
    }
}

MATCH_FUNC(0x4BF210);
u8 Zones_CA8::get_zone_idx_4BF210(const char *zoneName)
{
    Zone_144* pZone = zone_by_name_4BF100(zoneName);
    if (pZone)
    {
        return pZone->field_1_zone_idx;
    }
    else
    {
        return -1;
    }
}
