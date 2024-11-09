#include "Zones_CA8.hpp"
#include "Function.hpp"
#include <string.h>

EXPORT_VAR Zones_CA8 *gZones_CA8_67E274;

MATCH_FUNC(0x4BF100);
Zone_144 *Zones_CA8::zone_by_name_4BF100(const char *pZoneName)
{
    for (u8 i=0; i<10; i++)
    {
        if (_strnicmp(pZoneName, this->field_0[i].field_2_name, 4u) == 0)
        {
            return &this->field_0[i];
        }
    }
    return 0;
}
