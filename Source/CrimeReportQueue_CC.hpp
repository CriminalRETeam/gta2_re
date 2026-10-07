#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"

class Ped;
class Player;
class Car_BC;
class Char_B4;

// One reported crime: its type, the ped that committed it and where that ped was
class CrimeReport_14
{
  public:
    EXPORT CrimeReport_14();
    EXPORT ~CrimeReport_14();
    EXPORT void SetReport(s32 crime_type, s32 ped_id);
    EXPORT void GetReport(s32* pCrimeType, Fix16* pXPos, Fix16* yPos, Fix16* zPos);

    s32 field_0_crime_type; // crime_stats_type
    s32 field_4_ped_id;
    Fix16_Vec field_8_pos;
};

// Ring buffer of the last 10 reported crimes, drained by the police sound/AI code (TryPopOldestReport).
// field_2_read_idx is the oldest report still queued, field_0_write_idx the next free slot.
class CrimeReportQueue_CC
{
  public:
    EXPORT CrimeReportQueue_CC();
    EXPORT ~CrimeReportQueue_CC();
    EXPORT void QueueReport(s32 crime_type, s32 ped_id);
    EXPORT bool TryPopOldestReport(s32* pCrimeType, Fix16* pXPos, Fix16* pYPos, Fix16* pZPos);
    EXPORT bool IsCrimeQueued(s32 crime_type);
    EXPORT CrimeReportQueue_CC* ctor_484FC0();
    EXPORT void dtor_484FD0();
    EXPORT void ReportCrimeForPed(u32 crime_type, Ped* pPed);
    EXPORT bool ShouldReportCarCrime_485090(Car_BC* a2, Player* a3);
    EXPORT bool ShouldReportCharCrime_4850F0(Char_B4* a2, Player* a3);
    EXPORT bool ShouldReportPedCrime_485140(Ped* a2, Player* a3);

    u16 field_0_write_idx;
    u16 field_2_read_idx;
    CrimeReport_14 field_4_reports[10];
};

class CrimeReportQueue_CC_Sub : public CrimeReportQueue_CC {
  public:
    EXPORT CrimeReportQueue_CC_Sub();
    EXPORT ~CrimeReportQueue_CC_Sub();
};

EXTERN_GLOBAL(CrimeReportQueue_CC*, gCrimeReportQueue_67A4B8);
