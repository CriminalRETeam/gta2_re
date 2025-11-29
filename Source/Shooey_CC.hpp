#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"

class Ped;
class Player;
class Car_BC;
class Char_B4;

class Shooey_14
{
  public:
    EXPORT Shooey_14();
    EXPORT ~Shooey_14();
    EXPORT void ReportCrimeForPedAtLocation(s32 crime_type, s32 ped_id);
    EXPORT void GetCrimeTypeAndLocation(s32* pCrimeType, Fix16* pXPos, Fix16* yPos, Fix16* zPos);

    s32 field_0_crime_type;
    s32 field_4_ped_id;
    Fix16_Vec field_8_pos;
};

class Shooey_CC
{
  public:
    EXPORT Shooey_CC();
    EXPORT ~Shooey_CC();
    EXPORT void ReportCrime(s32 crime_type, s32 ped_id);
    EXPORT bool GetLatestReportedCrime(s32* pCrimeType, Fix16* pXPos, Fix16* pYPos, Fix16* pZPos);
    EXPORT char_type CanReportCrime(s32 crime_type);
    EXPORT Shooey_CC* ctor_484FC0();
    EXPORT void dtor_484FD0();
    EXPORT void ReportCrimeForPed(u32 crime_type, Ped* pPed);
    EXPORT bool sub_485090(Car_BC* a2, Player* a3);
    EXPORT char_type sub_4850F0(Char_B4* a2, Player* a3);
    EXPORT char_type sub_485140(Ped* a2, Player* a3);

    u16 field_0_idx;
    u16 field_2_report_count;
    Shooey_14 field_4_crimes[10];
};

EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);
