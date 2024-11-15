#include "Zones_CA8.hpp"
#include "Function.hpp"
#include "map_0x370.hpp"
#include <string.h>

EXPORT_VAR Zones_CA8 *gZones_CA8_67E274;

MATCH_FUNC(0x4BE4E0);
Zone_144::Zone_144()
{
    init_4BED70();
}

MATCH_FUNC(0x4BE4F0);
Zone_144::~Zone_144()
{
}

MATCH_FUNC(0x4BF090);
void Zone_144::set_name_4BF090(const char *pName, u8 nameLen)
{
    strncpy(this->field_2_name, pName, nameLen);
    this->field_2_name[nameLen] = 0; // NULL terminate the string
}

STUB_FUNC(0x4BED70);
void Zone_144::init_4BED70()
{
}

// =====================

STUB_FUNC(0x4be500)
void Zones_CA8::dtor_4BE500()
{

}

STUB_FUNC(0x4beca0)
Zone_144* Zones_CA8::sub_4BECA0()
{
    return 0;
}

STUB_FUNC(0x4bece0)
Zone_144* Zones_CA8::sub_4BECE0()
{
    return 0;
}

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
    Zone_144 *pZone = zone_by_name_4BF100(zoneName);
    if (pZone)
    {
        return pZone->field_1_zone_idx;
    }
    else
    {
        return -1;
    }
}

STUB_FUNC(0x4BF230);
void Zones_CA8::sub_4BF230(Zone_144 *pZone, u8 zone_idx)
{
    Zone_144 zone;
    zone.init_4BED70();
    if (pZone->field_1_zone_idx != zone_idx)
    {
        Zone_144 *pZoneByIdx = ZoneByIdx_4BF1C0(zone_idx);
        if (pZoneByIdx->field_0_used)
        {
            if (pZoneByIdx->field_138 != pZone->field_138)
            {
                memcpy(&zone, pZoneByIdx, sizeof(zone));
                memcpy(
                    &gZones_CA8_67E274->field_0[pZoneByIdx->field_1_zone_idx],
                    pZone,
                    sizeof(gZones_CA8_67E274->field_0[pZoneByIdx->field_1_zone_idx]));
                memcpy(
                    &gZones_CA8_67E274->field_0[pZone->field_1_zone_idx],
                    &zone,
                    sizeof(gZones_CA8_67E274->field_0[pZone->field_1_zone_idx]));
                pZone->field_1_zone_idx = pZoneByIdx->field_1_zone_idx;
                gZones_CA8_67E274->field_0[zone_idx].field_1_zone_idx = zone_idx;
            }
        }
    }
}

MATCH_FUNC(0x4BF2F0);
u8 Zones_CA8::sub_4BF2F0(int arg0)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (ZoneByIdx_4BF1C0(i)->field_13C == arg0)
        {
            return i;
        }
    }
    return -1;
}
