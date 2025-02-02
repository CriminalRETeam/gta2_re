#include "Zones_CA8.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "text_0x14.hpp"
#include <string.h>

EXPORT_VAR Zones_CA8* gZones_CA8_67E274;
GLOBAL(gZones_CA8_67E274, 0x67E274)

EXPORT_VAR s32 gZoneIdx_6206B8; // = -1 // TODO
GLOBAL(gZoneIdx_6206B8, 0x6206B8)

EXPORT_VAR wchar_t gZoneNameWide_67E030[4];
GLOBAL(gZoneNameWide_67E030, 0x67E030)

MATCH_FUNC(0x4BE4E0);
Gang_144::Gang_144()
{
    init_4BED70();
}

MATCH_FUNC(0x4BE4F0);
Gang_144::~Gang_144()
{
}

MATCH_FUNC(0x4BED30)
wchar_t* Gang_144::get_name_wide_4BED30()
{
    gZoneNameWide_67E030[0] = field_2_name[0];
    gZoneNameWide_67E030[1] = field_2_name[1];
    gZoneNameWide_67E030[2] = field_2_name[2];
    gZoneNameWide_67E030[3] = 0;
    return gZoneNameWide_67E030;
}

STUB_FUNC(0x4BED70);
void Gang_144::init_4BED70()
{
}

MATCH_FUNC(0x4BEDF0)
char_type Gang_144::sub_4BEDF0(u8 a2)
{
    return field_112[a2];
}

MATCH_FUNC(0x4BEE30)
void Gang_144::sub_4BEE30(u8 idx, char_type value)
{
    field_11C[idx] = value;
}

STUB_FUNC(0x4BEE50)
char_type Gang_144::sub_4BEE50(u8 idx, char_type a3)
{
    return 0;
}

STUB_FUNC(0x4BEEA0)
void Gang_144::sub_4BEEA0(u8 a2, char_type a3)
{
}

MATCH_FUNC(0x4BEEF0)
char_type Gang_144::sub_4BEEF0(u8 a2)
{
    return field_11C[a2];
}

STUB_FUNC(0x4BEF10)
bool Gang_144::sub_4BEF10(u8 a2)
{
    return (field_11C[a2] < 237) ? true : false;
}

MATCH_FUNC(0x4BEF50)
void Gang_144::sub_4BEF50(u8 zone_idx, char_type a3)
{
    field_122[zone_idx] = a3;
}

STUB_FUNC(0x4BEF70)
char_type Gang_144::sub_4BEF70(u8 a2, u8 a3)
{
    return 0;
}

// https://decomp.me/scratch/pXGoj
STUB_FUNC(0x4BF000)
void Gang_144::sub_4BF000(u8 a2, char_type idx)
{
    if (idx <= 0)
    {
        sub_4BEEA0(a2, -idx);
    }
    else
    {
        sub_4BEE50(a2, idx);
    }

    for (u8 i = 0; i < 10; ++i)
    {
        Gang_144* pZoneFromIdx = gZones_CA8_67E274->ZoneByIdx_4BF1C0(i);
        if (field_1_zone_idx != pZoneFromIdx->field_1_zone_idx)
        {
            if (pZoneFromIdx->field_122[field_1_zone_idx])
            {
                if (idx <= 0)
                {
                    pZoneFromIdx->sub_4BEE50(a2, -idx);
                }
                else
                {
                    pZoneFromIdx->sub_4BEEA0(a2, idx);
                }
            }
        }
    }
}

MATCH_FUNC(0x4BF090);
void Gang_144::set_name_4BF090(const char_type* pName, u8 nameLen)
{
    strncpy(field_2_name, pName, nameLen);
    field_2_name[nameLen] = 0; // NULL terminate the string
}

MATCH_FUNC(0x4BF340)
wchar_t* Gang_144::sub_4BF340()
{
    sprintf(gTmpBuffer_67C598, "ganga%d", field_138_arrow_colour);
    return gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598);
}

// =====================

MATCH_FUNC(0x4be500)
Zones_CA8::~Zones_CA8()
{
}

MATCH_FUNC(0x4beca0)
Gang_144* Zones_CA8::sub_4BECA0()
{
    for (gZoneIdx_6206B8 = 0; gZoneIdx_6206B8 < GTA2_COUNTOF_S(field_0); gZoneIdx_6206B8++)
    {
        if (field_0[gZoneIdx_6206B8].field_0_used 
            && field_0[gZoneIdx_6206B8].field_139_kill_respect_change > 0)
        {
            return &field_0[gZoneIdx_6206B8];
        }
    }
    return 0;
}

MATCH_FUNC(0x4bece0)
Gang_144* Zones_CA8::sub_4BECE0()
{
    while (++gZoneIdx_6206B8 < GTA2_COUNTOF_S(field_0))
    {
        if (field_0[gZoneIdx_6206B8].field_0_used 
            && field_0[gZoneIdx_6206B8].field_139_kill_respect_change > 0)
        {
            return &field_0[gZoneIdx_6206B8];
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF100);
Gang_144* Zones_CA8::zone_by_name_4BF100(const char* pZoneName)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (_strnicmp(pZoneName, field_0[i].field_2_name, 4u) == 0)
        {
            return &field_0[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF170);
Gang_144* Zones_CA8::next_free_zone_4BF170()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if (!field_0[i].field_0_used)
        {
            Gang_144* result = &field_0[i];
            result->field_1_zone_idx = i;
            result->field_0_used = 1;
            return result;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF1C0);
Gang_144* Zones_CA8::ZoneByIdx_4BF1C0(u8 zone_idx)
{
    return &field_0[zone_idx];
}

MATCH_FUNC(0x4BF1E0);
void Zones_CA8::alloc_map_zone_4BF1E0(gmp_map_zone* pMapZone)
{
    if (!zone_by_name_4BF100(pMapZone->field_6_name))
    {
        Gang_144* pFreeZone = next_free_zone_4BF170();
        pFreeZone->set_name_4BF090(pMapZone->field_6_name, pMapZone->field_5_name_length);
    }
}

MATCH_FUNC(0x4BF210);
u8 Zones_CA8::get_zone_idx_4BF210(const char* zoneName)
{
    Gang_144* pZone = zone_by_name_4BF100(zoneName);
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
void Zones_CA8::sub_4BF230(Gang_144* pZone, u8 zone_idx)
{
    Gang_144 zone;
    zone.init_4BED70();
    if (pZone->field_1_zone_idx != zone_idx)
    {
        Gang_144* pZoneByIdx = ZoneByIdx_4BF1C0(zone_idx);
        if (pZoneByIdx->field_0_used)
        {
            if (pZoneByIdx->field_138_arrow_colour != pZone->field_138_arrow_colour)
            {
                memcpy(&zone, pZoneByIdx, sizeof(zone));
                memcpy(&gZones_CA8_67E274->field_0[pZoneByIdx->field_1_zone_idx],
                       pZone,
                       sizeof(gZones_CA8_67E274->field_0[pZoneByIdx->field_1_zone_idx]));
                memcpy(&gZones_CA8_67E274->field_0[pZone->field_1_zone_idx],
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
        if (ZoneByIdx_4BF1C0(i)->field_13C_gang_car_model == arg0)
        {
            return i;
        }
    }
    return -1;
}
