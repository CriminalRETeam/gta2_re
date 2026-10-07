// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kFP16Zero_6FE20C

#include "CarPhysics_B0.hpp"
#include "CarAI_78.hpp"
#include "CarInfo_808.hpp"
#include "Globals.hpp"
#include "Frontend.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"

// Forward declarations: the functions below are in address order
EXPORT s32 __stdcall get_skid_obj_type_55D490(s32 surface, Fix16 box_idx);
static inline Fix16 __stdcall DotProductOOL_49E500(Fix16_Point& Vector1, Fix16_Point& Vector2);
static inline Fix16 __stdcall Square_49E0E0(const Fix16& value);
EXPORT Fix16 __stdcall DotProduct_560680(const Fix16_Point& Vector1, const Fix16_Point& Vector2);

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0, 0x6FE3E0);
DEFINE_GLOBAL(CarInfo_2C*, gCarInfo_2C_6FE0E4, 0x6FE0E4);
DEFINE_GLOBAL(ModelPhysics_48*, gCarInfo_48_6FE258, 0x6FE258);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6FE3C0, Ang16(0), 0x6FE3C0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FE20C, Fix16(0), 0x6FE20C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE290, kFP16Zero_6FE20C, 0x6FE290);
DEFINE_GLOBAL_INIT(Fix16, kFP16Five_6FE220, Fix16(5), 0x6FE220);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDEFC, kFP16Five_6FE220, 0x6FDEFC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDF88, k_dword_6FDEFC, 0x6FDF88);
DEFINE_GLOBAL_INIT(Fix16, kBrakePressureStart_6FE200, kFP16Zero_6FE20C, 0x6FE200);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FE214, Fix16(2), 0x6FE214);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDFEC, Fix16(0x2000, 0), 0x6FDFEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1B0, kFP16Two_6FE214 + k_dword_6FDFEC, 0x6FE1B0);
DEFINE_GLOBAL(Fix16, gDamageSpeedFactor_6FE348, 0x6FE348);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FDFB0, kFP16Two_6FE214, 0x6FDFB0);
DEFINE_GLOBAL(Fix16, gBrakeForce_6FE0D8, 0x6FE0D8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FDFD4, Fix16(0x1000, 0), 0x6FDFD4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE364, kFP16Quarter_6FDFD4, 0x6FE364);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE3A0, kFP16Quarter_6FDFD4, 0x6FE3A0);
DEFINE_GLOBAL_INIT(Fix16, kBrakePressureStep_6FE2AC, kFP16Quarter_6FDFD4, 0x6FE2AC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE210, Fix16(1), 0x6FE210);
DEFINE_GLOBAL_INIT(Fix16, kBrakePressureMax_6FE1C0, k_dword_6FE210, 0x6FE1C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFE4, Fix16(0x1333, 0), 0x6FDFE4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0A8, dword_6FDFE4, 0x6FE0A8);

DEFINE_GLOBAL_INIT(Fix16, kFP16One64th_6FE1AC, Fix16(0x100, 0), 0x6FE1AC);
DEFINE_GLOBAL_INIT(Fix16, kFP16One64th_6FE2E0, kFP16One64th_6FE1AC, 0x6FE2E0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FE234, Fix16(8), 0x6FE234);
DEFINE_GLOBAL_INIT(Fix16, kMaxZ_6FDF34, kFP16Eight_6FE234 - kFP16One64th_6FE2E0, 0x6FDF34);

DEFINE_GLOBAL(Fix16_Point, g_cm1_6FDF10, 0x6FDF10);
DEFINE_GLOBAL(Fix16, g_cp3_6FDF08, 0x6FDF08);
DEFINE_GLOBAL_INIT(Ang16, g_theta_6FE344, Ang16(0), 0x6FE344);
DEFINE_GLOBAL(Fix16_Point, g_cp1_6FDF00, 0x6FDF00);
DEFINE_GLOBAL(Fix16, g_f70_6FDFE0, 0x6FDFE0);
DEFINE_GLOBAL(Fix16, g_ZPos_6FE0AC, 0x6FE0AC);

DEFINE_GLOBAL(Fix16_Point, g_trailer_cm1_6FE068, 0x6FE068);
DEFINE_GLOBAL(Fix16, gTrailer_cp3_6FE1B4, 0x6FE1B4);
DEFINE_GLOBAL_INIT(Ang16, gTrailer_theta_6FE018, Ang16(0), 0x6FE018);
DEFINE_GLOBAL(Fix16_Point, gTrailer_cp1_6FE3A8, 0x6FE3A8);
DEFINE_GLOBAL(Fix16, gTrailer_f70_6FE194, 0x6FE194);
DEFINE_GLOBAL(Fix16, gTrailer_ZPos_6FE354, 0x6FE354);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE1D8, Fix16(0x28F, 0), 0x6FE1D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDF3C, dword_6FE1D8, 0x6FDF3C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FDF7C, Fix16(1), 0x6FDF7C);
DEFINE_GLOBAL_INIT(Fix16, kFP16SixtyFour_6FE2EC, Fix16(0x100000, 0), 0x6FE2EC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Four_6FE21C, Fix16(0x10000, 0), 0x6FE21C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One256th_6FE07C, k_dword_6FE210 / (kFP16SixtyFour_6FE2EC * kFP16Four_6FE21C), 0x6FE07C);

DEFINE_GLOBAL_INIT(Fix16, FastCarMinVelocity_6FE1CC, Fix16(0x1EB, 0), 0x6FE1CC);
DEFINE_GLOBAL(Fix16, gRemainingTimeStep_6FE198, 0x6FE198);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FE370, Fix16(0x800, 0), 0x6FE370);

DEFINE_GLOBAL_INIT(Fix16, kFP16Three_6FE218, Fix16(3), 0x6FE218);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE1B8, kFP16Three_6FE218, 0x6FE1B8);

DEFINE_GLOBAL(Fix16_Point, stru_6FDF50, 0x6FDF50);
DEFINE_GLOBAL(Fix16, dword_6FE0B0, 0x6FE0B0);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDFF0, Fix16(0x2666, 0), 0x6FDFF0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD8, Fix16(0xCCC, 0), 0x6FDFD8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1D4, Fix16(0x333, 0), 0x6FE1D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFF8, Fix16(0x3333, 0), 0x6FDFF8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE228, dword_6FDFF0, 0x6FE228);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE374, dword_6FDFD8 + dword_6FE1D4, 0x6FE374);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE104, dword_6FDFF8, 0x6FE104);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6FE00C, Ang16(360), 0x6FE00C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FE154, Ang16(1080), 0x6FE154);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FE12A, Ang16(720), 0x6FE12A);
DEFINE_GLOBAL(Fix16_Point, stru_6FE1F0, 0x6FE1F0);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_6FE3C4, Fix16(0x11C, 0), 0x6FE3C4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDFA4, kAngFix16OneDegree_6FE3C4, 0x6FDFA4);
DEFINE_GLOBAL_INIT(Fix16, stru_6FDF80, kFP16One64th_6FE2E0, 0x6FDF80);

DEFINE_GLOBAL_INIT(Fix16, gCollisionDamage_6FE33C, Fix16(0), 0x6FE33C);
DEFINE_GLOBAL(u8, gCollisionArea_6FDFC4, 0x6FDFC4);
DEFINE_GLOBAL(u8, gOtherCollisionArea_6FDFCC, 0x6FDFCC);
DEFINE_GLOBAL(Fix16_Point, CollisionIntersectionPoint_6FE1A0, 0x6FE1A0);

DEFINE_GLOBAL(Fix16_Point, gSaved_cm1_6FE3C8, 0x6FE3C8);
DEFINE_GLOBAL(Fix16, gSaved_cp3_6FDF84, 0x6FDF84);
DEFINE_GLOBAL(Ang16, gSaved_theta_6FE158, 0x6FE158);
DEFINE_GLOBAL(Fix16_Point, gSaved_cp1_6FE090, 0x6FE090);
DEFINE_GLOBAL(Fix16, gSaved_f70_6FE268, 0x6FE268);
DEFINE_GLOBAL(Fix16, gSaved_zpos_6FE32C, 0x6FE32C);

DEFINE_GLOBAL(Fix16_Point, gSaved_trailer_cm1_6FE160, 0x6FE160);
DEFINE_GLOBAL(Fix16, gSaved_trailed_cp3_6FDF8C, 0x6FDF8C);
DEFINE_GLOBAL(Ang16, gSaved_trailer_theta_6FE310, 0x6FE310);
DEFINE_GLOBAL(Fix16_Point, gSaved_trailer_cp1_6FDF40, 0x6FDF40);
DEFINE_GLOBAL(Fix16, gSaved_trailer_f70_6FE0E0, 0x6FE0E0);
DEFINE_GLOBAL(Fix16, gSaved_trailer_zpos_6FE394, 0x6FE394);

DEFINE_GLOBAL_INIT(Fix16, kAngFix16FullCircle_6FE314, Fix16(0x18F60, 0), 0x6FE314);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE3D0, k_dword_6FE210, 0x6FE3D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE148, Fix16(0x8E5, 0), 0x6FE148);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3D4, dword_6FE148, 0x6FE3D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE320, dword_6FDFF8, 0x6FE320);
DEFINE_GLOBAL_INIT(Fix16, kFP16One128th_6FE0A0, Fix16(0x80, 0), 0x6FE0A0);
DEFINE_GLOBAL_INIT(Fix16, kFP16One128th_6FDFDC, kFP16One128th_6FE0A0, 0x6FDFDC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE334, k_dword_6FE210 - dword_6FE1D4, 0x6FE334);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2F0, Fix16(0x51, 0), 0x6FE2F0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1C4, Fix16(0xA3, 0), 0x6FE1C4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE330, k_dword_6FE210 - dword_6FE2F0 - dword_6FE1C4, 0x6FE330);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE240, k_dword_6FE210 - dword_6FE2F0 - dword_6FE1C4, 0x6FE240);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1E4, Fix16(0x3D7, 0), 0x6FE1E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDF18, k_dword_6FE210 - dword_6FE1E4, 0x6FDF18);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD0, Fix16(0x666, 0), 0x6FDFD0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE100, k_dword_6FE210 - dword_6FDFD0, 0x6FE100);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0FC, k_dword_6FE210 - FastCarMinVelocity_6FE1CC, 0x6FE0FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFBC, k_dword_6FE210 - FastCarMinVelocity_6FE1CC, 0x6FDFBC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1E0, Fix16(0x7AE, 0), 0x6FE1E0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE318, k_dword_6FE210 - dword_6FE1E0, 0x6FE318);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3B4, dword_6FE1D4, 0x6FE3B4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE15C, dword_6FE1C4, 0x6FE15C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2F4, dword_6FDFD0, 0x6FE2F4);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE0C0, Fix16(0x2000, 0), 0x6FE0C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE064, Fix16(0x1FE8, 0), 0x6FE064);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE350, Fix16(0x824, 0), 0x6FE350);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE10C, Fix16(0x63D8, 0), 0x6FE10C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE134, kAngFix16OneDegree_6FE3C4 * 25, 0x6FE134);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE278, Fix16(0x12B88, 0), 0x6FE278);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16HalfCircle_6FE260, Fix16(0xC7B0, 0), 0x6FE260);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE2F8, kFP16Half_6FE0C0, 0x6FE2F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE070, k_dword_6FE210, 0x6FE070);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE3DC, k_dword_6FE210, 0x6FE3DC);

DEFINE_GLOBAL_INIT(Ang16, word_6FE3B8, Ang16(4), 0x6FE3B8); // Only exists so that MultiplyByFix16_401CB0 can be called

DEFINE_GLOBAL_INIT(Ang16, word_6FE058, word_6FE3B8.MultiplyByFix16_401CB0(Fix16(45)), 0x6FE058);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE37C, dword_6FE1C4, 0x6FE37C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE004, Fix16(0x1C00, 0), 0x6FE004);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE2B0, k_dword_6FE210 - dword_6FE320, 0x6FE2B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE340, kFP16Three_6FE218 + dword_6FDFF8, 0x6FE340);

DEFINE_GLOBAL_INIT(Fix16, kFP16MinusOne_6FDF1C, Fix16(0xFFFFC000, 0), 0x6FDF1C);
DEFINE_GLOBAL(Fix16_Point, stru_6FE300, 0x6FE300);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDFFC, Fix16(0x3999, 0), 0x6FDFFC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE358, kFP16One64th_6FE2E0*(dword_6FDFD0 + k_dword_6FE210), 0x6FE358);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE078, (kAngFix16OneDegree_6FE3C4 * dword_6FDFFC), 0x6FE078);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE390, (kFP16One64th_6FE2E0 * dword_6FDFFC), 0x6FE390);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE080, kAngFix16OneDegree_6FE3C4*(dword_6FDFD0 + k_dword_6FE210), 0x6FE080);
DEFINE_GLOBAL_INIT(Fix16, kFP16One16th_6FE270, Fix16(0x400, 0), 0x6FE270);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE178, Fix16(0x1800, 0), 0x6FE178);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE0D0, kFP16Half_6FE0C0, 0x6FE0D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFF4, Fix16(0x2CCC, 0), 0x6FDFF4);
DEFINE_GLOBAL_INIT(Fix16, kFP16One32nd_6FE118, Fix16(0x200, 0), 0x6FE118);

DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FE1A8, kFP16Quarter_6FDFD4, 0x6FE1A8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE098, k_dword_6FE210, 0x6FE098);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE0F4, k_dword_6FE210, 0x6FE0F4);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE0D4, k_dword_6FE210, 0x6FE0D4);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FDFB8, kFP16Quarter_6FDFD4, 0x6FDFB8);

// 9.6f 0x49EF50
inline void CarPhysics_B0::AddDamage_49EF50(s32 damage)
{
    u32 new_damage = gpRng_67AB34->get_cur_rng_41CFE0() + damage;
    if (new_damage > field_8_total_damage_q)
    {
        field_8_total_damage_q = new_damage;
    }
}

MATCH_FUNC(0x40B560)
Fix16_Point CarPhysics_B0::get_cp1_40B560()
{
    return field_38_cp1;
}

MATCH_FUNC(0x446ee0)
CarPhysics_B0::~CarPhysics_B0()
{
}

MATCH_FUNC(0x447010)
EXPORT Fix16_Point CarPhysics_B0::get_linvel_447010()
{
    return field_40_linvel_1;
}

wchar_t gThetaText_66A8EC[32]; //DEFINE_GLOBAL_ARRAY(wchar_t, gThetaText_66A8EC, 32, 0x66A8EC); // global crashing standalone

// https://decomp.me/scratch/xqLh0
// 9.6f 0x49E240
inline wchar_t* Ang16::ThetaText_49E240()
{
    swprintf(gThetaText_66A8EC, L"%3.2f", (rValue * 0.25));
    return gThetaText_66A8EC;
}

inline f64 Fix16::to_float_410BA0() const
{
    return ((mValue / 16384.0));
}

MATCH_FUNC(0x453F50)
void CarPhysics_B0::ForceNeutralInput_453F50()
{
  this->field_95 = 1;
  this->field_91_is_foot_brake_on = 0;
  this->field_94_is_backward_gas_on = 0;
  this->field_93_is_forward_gas_on = 0;
}

MATCH_FUNC(0x453F70)
void CarPhysics_B0::ForceForwardAcceleration_453F70()
{
  this->field_93_is_forward_gas_on = 1;
  this->field_91_is_foot_brake_on = 0;
  this->field_94_is_backward_gas_on = 0;
  this->field_95 = 0;
}

MATCH_FUNC(0x453F90)
void CarPhysics_B0::ClearDriverInputs_453F90()
{
  this->field_91_is_foot_brake_on = 0;
  this->field_93_is_forward_gas_on = 0;
  this->field_94_is_backward_gas_on = 0;
  this->field_95 = 0;
}

MATCH_FUNC(0x559430)
void CarPhysics_B0::ShowPhysicsDebug_559430()
{
    if (bDo_show_physics_67D54F)
    {
        Garox_C4* pText;
        SetCurrentCarInfoAndModelPhysics_562EF0();

        swprintf(tmpBuff_67BD9C,
                 L"CM = (%3.3f,%3.3f) CP = (%3.3f,%3.3f,%3.3f)",
                 field_30_cm1.x.to_float_410BA0(),
                 field_30_cm1.y.to_float_410BA0(),
                 field_38_cp1.x.to_float_410BA0(),
                 field_38_cp1.y.to_float_410BA0(),
                 field_6C_cp3.to_float_410BA0());
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 64, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C,
                 L"linvel = (%3.3f,%3.3f) angvelrad = %3.3f",
                 field_40_linvel_1.x.to_float_410BA0(),
                 field_40_linvel_1.y.to_float_410BA0(),
                 field_74_ang_vel_rad.to_float_410BA0());
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 80, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C, L"theta = %s", field_58_theta.ThetaText_49E240());
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 96, gDebugFont_706600, 1);

        // TODO: the format string at 0x623FFC is a guess
        swprintf(tmpBuff_67BD9C, L"pointing ang = %3.3f", field_78_pointing_ang_rad.to_float_410BA0());
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 112, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C, L"mass = %3.3f", CalculateMass_559FF0().to_float_410BA0());
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 128, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C, L"front skid = %3.3f", field_84_front_skid.to_float_410BA0());
        pText = gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 144, gDebugFont_706600, 1);
        if (field_84_front_skid >= gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1 ||
            (field_AC_drive_wheels_locked_q > 0 && gCarInfo_48_6FE258->field_8_front_drive_bias > kFP16Zero_6FE20C))
        {
            pText->SetDrawKind8_45AFD0(5);
        }

        swprintf(tmpBuff_67BD9C, L"rear skid = %3.3f", field_88_rear_skid.to_float_410BA0());
        pText = gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 160, gDebugFont_706600, 1);
        if (field_88_rear_skid >= gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2 ||
            (field_AC_drive_wheels_locked_q > 0 && gCarInfo_2C_6FE0E4->field_20_front_drive_bias > kFP16Zero_6FE20C))
        {
            pText->SetDrawKind8_45AFD0(5);
        }

        swprintf(tmpBuff_67BD9C,
                 L"surface_mode = %d sbw = %d tpa = %d",
                 field_98_surface_type,
                 (u8)field_AA_sbw,
                 (u8)field_AB_tpa);
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 176, gDebugFont_706600, 1);
    }
}

MATCH_FUNC(0x5597b0)
void CarPhysics_B0::ShowSpeedRevsDamage_5597B0()
{
    if (bDo_show_instruments_67D64C)
    {
        SetCurrentCarInfoAndModelPhysics_562EF0();

        Fix16 speed = field_40_linvel_1.GetLength_41E260();
        s32 gear;
        if (speed > gCarInfo_48_6FE258->field_44_gear3_speed)
        {
            gear = 3;
        }
        else
        {
            gear = (speed > gCarInfo_48_6FE258->field_40_gear2_speed) + 1;
        }

        swprintf(tmpBuff_67BD9C, L"speed:%3.3f(%d)", GetLinearSpeed_4211A0().AsDouble(), gear);
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 16, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C, L"revs:%3.3f %c", field_60_gas_pedal.AsDouble(), get_revs_561940() ? 'T' : ' ');
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 32, gDebugFont_706600, 1);

        // TODO: the format string at 0x6240FC is a guess
        swprintf(tmpBuff_67BD9C, L"damage:%d", field_5C_pCar->field_74_damage * 100 / 32000);
        gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 48, gDebugFont_706600, 1);
    }
}

MATCH_FUNC(0x5599d0)
bool CarPhysics_B0::IsNotMoving_5599D0()
{
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_8_truck_cab->field_58_physics->IsStationary_49EF80() &&
            pTrailer->field_C_pCarOnTrailer->field_58_physics->IsStationary_49EF80();
    }
    else
    {
        return IsStationary_49EF80();
    }
}

MATCH_FUNC(0x559a40)
void CarPhysics_B0::UpdateTrailerPhysicsFromTowingCar_559A40()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        Fix16 v6 = (k_dword_6FE210) / gRemainingTimeStep_6FE198;
        CarPhysics_B0* pPhysics = field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pPhysics->field_40_linvel_1 = (pPhysics->field_38_cp1 - gTrailer_cp1_6FE3A8) * v6;
        pPhysics->field_74_ang_vel_rad = v6 * Ang16::Ang16_to_Fix16(ComputeShortestAngleDelta_4056C0(gTrailer_theta_6FE018, pPhysics->field_58_theta));
        pPhysics->field_70_z_vel = (v6 * (pPhysics->field_6C_cp3 - gTrailer_cp3_6FE1B4));
    }
}

MATCH_FUNC(0x559b40)
void CarPhysics_B0::UpdateTrailerAlignment_559B40()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->UpdateTrailerAlignment_407CE0();
    }
}

MATCH_FUNC(0x559b50)
void CarPhysics_B0::EnforceTrailerControlLimits_559B50()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        if (pTrailer->get_field_0_49EFB0())
        {
            if (this->field_94_is_backward_gas_on)
            {
                this->field_94_is_backward_gas_on = 0;
                this->field_91_is_foot_brake_on = 1;
            }
            if (!this->field_93_is_forward_gas_on)
            {
                this->field_AD_turn_direction = car_turn_direction::none_0;
            }
        }
    }
}

MATCH_FUNC(0x559b90)
void CarPhysics_B0::set_field_A0_559B90(const s32& a2)
{
    field_A0_oil_spin_dir = a2;
}

MATCH_FUNC(0x559ba0)
void CarPhysics_B0::SpinOutOnOil_559BA0()
{
    if (!field_5C_pCar->IsTrainModel_403BA0())
    {
        if (field_A0_oil_spin_dir != 1 && field_A0_oil_spin_dir != 2)
        {
            if (gRng_6F6784.get_int_4F7AE0(2))
            {
                set_field_A0_559B90(1);
            }
            else
            {
                set_field_A0_559B90(2);
            }
        }
        this->field_A4_oil_spin_timer = 30;
        AddDamage_49EF50(30);
    }
}

// https://decomp.me/scratch/yM7OA Fix16 annoying inlined stuff
MATCH_FUNC(0x559c30)
void CarPhysics_B0::ScarePedsOnDrivingFast_559C30()
{
    Fix16 cp3 = field_6C_cp3;

    if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_38_cp1.x.ToInt(), field_38_cp1.y.ToInt(), cp3.ToInt()))
    {
        cp3 = field_6C_cp3 - k_dword_6FE210;
    }

    gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_38_cp1.x.ToInt(), field_38_cp1.y.ToInt(), cp3.ToInt());
    if (pBlock)
    {
        u8 type = pBlock->field_B_slope_type & 3; //get_block_type(pBlock->field_B_slope_type);
        if (type == PAVEMENT || type == FIELD)
        {
            if (field_5C_pCar->field_54_driver)
            {
                if (!field_5C_pCar->IsTrainModel_403BA0())
                {
                    //Fix16 linvel_length = get_car_lin_vel_4754D0();
                    // 9.6f: CarPhysics_B0::GetLinearSpeed_4211A0 (inlined, using it makes the diff worse)

                    if (field_40_linvel_1.GetLength_out_of_line_x_squared() > FastCarMinVelocity_6FE1CC || field_5C_pCar->IsEmittingHorn_411970())
                    {
                        field_5C_pCar->field_54_driver->AddThreateningPedToList_46FC70();
                    }
                }
            }
        }
    }

    if (field_A0_oil_spin_dir > 0 && field_A0_oil_spin_dir <= 2)
    {
        field_A4_oil_spin_timer--;
        if (field_A4_oil_spin_timer == 0)
        {
            CarPhysics_B0::set_field_A0_559B90(0);
        }
    }
}

MATCH_FUNC(0x559dd0)
void CarPhysics_B0::ApplyForcedSteering_559DD0()
{
    if (this->field_5C_pCar->field_54_driver)
    {
        if (field_A0_oil_spin_dir == 1)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = car_turn_direction::clockwise_m1;
        }
        else if (field_A0_oil_spin_dir == 2)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = car_turn_direction::anticlockwise_1;
        }
    }
}

MATCH_FUNC(0x559e20)
void CarPhysics_B0::ApplyObjectImpact_559E20(Object_2C* pObj)
{
    s8 v1;
    s8 v2;
    pObj->GetConveyorDirection_493090(&v1, &v2);
    stru_6FDF50.x += kFP16One64th_6FE2E0 * v1;
    stru_6FDF50.y += kFP16One64th_6FE2E0 * v2;
    AddDamage_49EF50(15);
}

MATCH_FUNC(0x559E90)
Fix16 CarPhysics_B0::ComputeZPosition_559E90()
{
    if (field_70_z_vel > kFP16Zero_6FE20C)
    {
        Fix16 cp3 = field_6C_cp3;
        cp3 += k_dword_6FE210;
        if (cp3 > kMaxZ_6FDF34)
        {
            cp3 = kMaxZ_6FDF34;
        }
        return cp3;
    }
    else
    {
        return field_6C_cp3;
    }
}

MATCH_FUNC(0x559ec0)
Fix16_Point CarPhysics_B0::ComputeCombinedCenterOfMass_559EC0()
{

    if (field_5C_pCar->field_64_pTrailer)
    {
        Fix16 cab_mass;
        Fix16 trailer_mass;
        cab_mass = field_5C_pCar->field_64_pTrailer->field_8_truck_cab->get_mass_43A120();
        trailer_mass = field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->get_mass_43A120();

        Fix16 total_mass = trailer_mass + cab_mass;

        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->field_58_physics->field_30_cm1 * (cab_mass / total_mass) +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->field_58_physics->field_30_cm1 * (trailer_mass / total_mass);
    }
    else
    {
        return field_30_cm1;
    }
}

MATCH_FUNC(0x559ff0)
Fix16 CarPhysics_B0::CalculateMass_559FF0()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->get_mass_43A120() +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->get_mass_43A120();
    }
    else
    {
        return field_5C_pCar->get_mass_43A120();
    }
}

MATCH_FUNC(0x55a050)
Fix16 CarPhysics_B0::GetEffectiveMomentOfInertia_55A050()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->GetMomentOfInertia_43A590() +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->GetMomentOfInertia_43A590();
    }
    else
    {
        return field_5C_pCar->GetMomentOfInertia_43A590();
    }
}

MATCH_FUNC(0x55a0b0)
u8 CarPhysics_B0::IsInAir_55A0B0()
{
    // TODO: The surface checks are likely inlines
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_8_truck_cab->field_58_physics->field_98_surface_type == car_surface_type::air_surface_6 &&
            pTrailer->field_C_pCarOnTrailer->field_58_physics->field_98_surface_type == car_surface_type::air_surface_6;
    }
    return field_98_surface_type == car_surface_type::air_surface_6;
}

MATCH_FUNC(0x55a100)
Fix16 CarPhysics_B0::GetTrailerAwareTurnRatio_55A100()
{
    if (field_5C_pCar->is_trailer_cab_41E460())
    {
        return dword_6FE1B0 * gCarInfo_48_6FE258->field_18_turn_ratio;
    }
    else
    {
        return gCarInfo_48_6FE258->field_18_turn_ratio;
    }
}

MATCH_FUNC(0x55a150)
char_type CarPhysics_B0::IsFootBrakeOn_55A150()
{
    char_type bFootBrakeOn;

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pPhysics)
        {
            bFootBrakeOn = pPhysics->field_91_is_foot_brake_on;
        }
        else
        {
            bFootBrakeOn = 0;
        }
    }
    else
    {
        return this->field_91_is_foot_brake_on;
    }
    return bFootBrakeOn;
}

MATCH_FUNC(0x55a180)
char_type CarPhysics_B0::IsAccelerationOrReverseOn_55A180()
{
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pCarPhysics)
        {
            return (pCarPhysics->field_93_is_forward_gas_on || pCarPhysics->field_94_is_backward_gas_on);
        }
    }
    else
    {
        return (field_93_is_forward_gas_on || field_94_is_backward_gas_on);
    }
    return 0;
}

// 9.6f 0x49F760
MATCH_FUNC(0x55a1d0)
void CarPhysics_B0::SetVelocityTowardTarget_55A1D0(Fix16 targetX, Fix16 targetY, Fix16 targetAngle, s32* rotationMode)
{
    Fix16_Point local(targetX, targetY);
    Fix16_Point offset;
    CarInfo_2C* pCarInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->field_84_car_info_idx);
    offset = pCarInfo->field_C_center_of_mass_offset;
    Fix16_Point worldPoint;
    offset.RotateByAngle_40F6B0(field_58_theta);
    worldPoint = local.Add_40AC50(offset);

    field_40_linvel_1 = worldPoint - field_30_cm1;

    field_74_ang_vel_rad = targetAngle - Ang16::Ang16_to_Fix16(field_58_theta);

    if (*rotationMode == 1)
    {
        if (field_74_ang_vel_rad.mValue < kFP16Zero_6FE20C.mValue)
        {
            field_74_ang_vel_rad = field_74_ang_vel_rad + kAngFix16FullCircle_6FE314;
        }
    }
    else if (*rotationMode == 2)
    {
        if (field_74_ang_vel_rad.mValue > kFP16Zero_6FE20C.mValue)
        {
            field_74_ang_vel_rad = field_74_ang_vel_rad - kAngFix16FullCircle_6FE314;
        }
    }

    if (field_8C_state != 4)
    {
        field_8C_state = 0;
    }
}

// Like Fix16::Max, but by value and keeping the larger value in b. Fix16::Max picks the address of
// the larger operand and reads it through memory, which the original doesn't do here.
static inline Fix16 MaxByValue_55A6A0(Fix16 a, Fix16 b)
{
    if (a > b)
    {
        b = a;
    }
    return b;
}

MATCH_FUNC(0x55a400)
void CarPhysics_B0::restore_saved_physics_state_55A400()
{
    this->field_30_cm1 = g_cm1_6FDF10;
    this->field_6C_cp3 = g_cp3_6FDF08;
    this->field_58_theta = g_theta_6FE344;
    this->field_38_cp1 = g_cp1_6FDF00;
    this->field_70_z_vel = g_f70_6FDFE0;
    this->field_68_z_pos = g_ZPos_6FE0AC;
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pB0 = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pB0->field_30_cm1 = g_trailer_cm1_6FE068;
        pB0->field_6C_cp3 = gTrailer_cp3_6FE1B4;
        pB0->field_58_theta = gTrailer_theta_6FE018;
        pB0->field_38_cp1 = gTrailer_cp1_6FE3A8;
        pB0->field_70_z_vel = gTrailer_f70_6FE194;
        pB0->field_68_z_pos = gTrailer_ZPos_6FE354;
    }
}

MATCH_FUNC(0x55a4b0)
void CarPhysics_B0::save_physics_state_55A4B0()
{
    g_cm1_6FDF10 = this->field_30_cm1;
    g_cp3_6FDF08 = this->field_6C_cp3;
    g_theta_6FE344 = this->field_58_theta;
    g_cp1_6FDF00 = this->field_38_cp1;
    g_f70_6FDFE0 = this->field_70_z_vel;
    g_ZPos_6FE0AC = this->field_68_z_pos;
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        g_trailer_cm1_6FE068 = pPhysics->field_30_cm1;
        gTrailer_cp3_6FE1B4 = pPhysics->field_6C_cp3;
        gTrailer_theta_6FE018 = pPhysics->field_58_theta;
        gTrailer_cp1_6FE3A8 = pPhysics->field_38_cp1;
        gTrailer_f70_6FE194 = pPhysics->field_70_z_vel;
        gTrailer_ZPos_6FE354 = pPhysics->field_68_z_pos;
    }
}

MATCH_FUNC(0x55a550)
void CarPhysics_B0::restore_state_55A550()
{
    this->field_30_cm1 = gSaved_cm1_6FE3C8;
    this->field_6C_cp3 = gSaved_cp3_6FDF84;
    this->field_58_theta = gSaved_theta_6FE158;
    this->field_38_cp1 = gSaved_cp1_6FE090;
    this->field_70_z_vel = gSaved_f70_6FE268;
    this->field_68_z_pos = gSaved_zpos_6FE32C;

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pPhysics->field_30_cm1 = gSaved_trailer_cm1_6FE160;
        pPhysics->field_6C_cp3 = gSaved_trailed_cp3_6FDF8C;
        pPhysics->field_58_theta = gSaved_trailer_theta_6FE310;
        pPhysics->field_38_cp1 = gSaved_trailer_cp1_6FDF40;
        pPhysics->field_70_z_vel = gSaved_trailer_f70_6FE0E0;
        pPhysics->field_68_z_pos = gSaved_trailer_zpos_6FE394;
    }
}

MATCH_FUNC(0x55a600)
void CarPhysics_B0::save_state_55A600()
{
    gSaved_cm1_6FE3C8 = this->field_30_cm1;
    gSaved_cp3_6FDF84 = this->field_6C_cp3;
    gSaved_theta_6FE158 = this->field_58_theta;
    gSaved_cp1_6FE090 = this->field_38_cp1;
    gSaved_f70_6FE268 = this->field_70_z_vel;
    gSaved_zpos_6FE32C = this->field_68_z_pos;
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        gSaved_trailer_cm1_6FE160 = pPhysics->field_30_cm1;
        gSaved_trailed_cp3_6FDF8C = pPhysics->field_6C_cp3;
        gSaved_trailer_theta_6FE310 = pPhysics->field_58_theta;
        gSaved_trailer_cp1_6FDF40 = pPhysics->field_38_cp1;
        gSaved_trailer_f70_6FE0E0 = pPhysics->field_70_z_vel;
        gSaved_trailer_zpos_6FE394 = pPhysics->field_68_z_pos;
    }
}

MATCH_FUNC(0x55a6a0)
Fix16 CarPhysics_B0::ComputeRequiredSweepSteps_55A6A0()
{
    Fix16 v9 = MaxByValue_55A6A0((Fix16::ClampToRangeFlexible_55EEE0(Fix16::Abs(gSaved_cm1_6FE3C8.x - g_cm1_6FDF10.x),
                                                              Fix16::Abs(gSaved_cm1_6FE3C8.y - g_cm1_6FDF10.y),
                                                              Fix16::Abs(gSaved_cp3_6FDF84 - g_cp3_6FDF08))) /
                              field_5C_pCar->GetMinDimension_43A5B0(),
                          Ang16::NormalizeAngleDeltaScaled_405B60(g_theta_6FE344, gSaved_theta_6FE158, word_6FE058));

    if (field_5C_pCar->field_64_pTrailer)
    {

        v9 = Fix16::ClampToRangeFlexible_55EEE0(
            v9,
            (Fix16::ClampToRangeFlexible_55EEE0(Fix16::Abs(gSaved_trailer_cm1_6FE160.x - g_trailer_cm1_6FE068.x),
                                                Fix16::Abs(gSaved_trailer_cm1_6FE160.y - g_trailer_cm1_6FE068.y),
                                                Fix16::Abs(gSaved_trailed_cp3_6FDF8C - gTrailer_cp3_6FE1B4)) /
             field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->GetMinDimension_43A5B0()),
            Ang16::NormalizeAngleDeltaScaled_405B60(gTrailer_theta_6FE018, gSaved_trailer_theta_6FE310, word_6FE058));
    }

    return v9;
}

// 9.6f 0x42A630 called as a static with the value by reference (its 9.6f copy takes a pointer and a hidden
// return). Declared in fix16.hpp, defined here so no other TU changes.
inline Fix16 __stdcall Fix16::GetFracValue_42A630(const Fix16& v)
{
    return Fix16(v.mValue & 0x3FFF, 0);
}

MATCH_FUNC(0x55a840)
void CarPhysics_B0::ResetForceAccumulators_55A840()
{
    field_48_force_accum.x = 0;
    field_48_force_accum.y = 0;
    field_50_linear_accel.x = 0;
    field_50_linear_accel.y = 0;
    field_7C_torque_accum = 0;
    field_80_angular_accel = 0;
}

// https://decomp.me/scratch/efo3b
MATCH_FUNC(0x55a860)
void CarPhysics_B0::HandleUserInputs_55A860(char_type bForwardGasOn,
                                            char_type bFootBrakeOn,
                                            char_type bLeftOn,
                                            char_type bRightOn,
                                            char_type bHandBrakeOn)
{

    if (this->field_40_linvel_1.IsNull())
    {
        this->field_93_is_forward_gas_on = bForwardGasOn;
        this->field_94_is_backward_gas_on = bFootBrakeOn;
        this->field_91_is_foot_brake_on = 0;
    }
    else
    {
        // IsVelocityAlignedWithHeading_40F840 inlined
        Ang16 v14 = (field_40_linvel_1.atan2_40F790() - field_58_theta);
        bool aligned = v14 <= kAng90_6FE00C || v14 >= kAng270_6FE154;
        if (aligned)
        {
            if (this->field_94_is_backward_gas_on)
            {
                this->field_93_is_forward_gas_on = 0;
                this->field_94_is_backward_gas_on = 0;
                this->field_91_is_foot_brake_on = 0;
            }
            else
            {
                this->field_93_is_forward_gas_on = bForwardGasOn;
                this->field_91_is_foot_brake_on = bFootBrakeOn;
                this->field_94_is_backward_gas_on = 0;
            }
        }
        else if (this->field_93_is_forward_gas_on)
        {
            this->field_94_is_backward_gas_on = 0;
            this->field_93_is_forward_gas_on = 0;
            this->field_91_is_foot_brake_on = 0;
        }
        else
        {
            this->field_91_is_foot_brake_on = bForwardGasOn;
            this->field_94_is_backward_gas_on = bFootBrakeOn;
            this->field_93_is_forward_gas_on = 0;
        }
    }
    this->field_95 = 0;
    this->field_92_is_hand_brake_on = bHandBrakeOn;
    if (bRightOn)
    {
        if (!bLeftOn)
        {
            this->field_AD_turn_direction = car_turn_direction::clockwise_m1;
            return;
        }
    }
    else if (bLeftOn)
    {
        this->field_AD_turn_direction = car_turn_direction::anticlockwise_1;
        return;
    }
    this->field_AD_turn_direction = car_turn_direction::none_0;
}

MATCH_FUNC(0x55aa00)
void CarPhysics_B0::HandleGravityOnSlope_55AA00()
{

    Fix16_Point force;

    // On a slope and no brake inputs
    if (field_A5_current_slope_length != 1 || field_92_is_hand_brake_on || field_91_is_foot_brake_on)
    {
        return;
    }

    // Trains ignore slope gravity
    if (field_5C_pCar->IsTrainModel_403BA0())
    {
        return;
    }

    switch (field_98_surface_type)
    {
        case car_surface_type::slope_northwards_1:
            force.x = kFP16Zero_6FE20C;
            force.y = (dword_6FDF3C * kFP16One_6FDF7C);
            break;

        case car_surface_type::slope_southwards_2:
            force.x = kFP16Zero_6FE20C;
            force.y = (kFP16One_6FDF7C * -dword_6FDF3C);
            break;

        case car_surface_type::slope_westwards_3:
            force.x = (dword_6FDF3C * kFP16One_6FDF7C);
            force.y = kFP16Zero_6FE20C;
            break;

        case car_surface_type::slope_eastwards_4:
            force.x = (kFP16One_6FDF7C * -dword_6FDF3C);
            force.y = kFP16Zero_6FE20C;
            break;

        default:
            return;
    }

    ApplyForceScaledByMass_55F9A0(force);
}

MATCH_FUNC(0x55ab50)
Fix16* CarPhysics_B0::ComputeSlopeCorrection_55AB50(Fix16* pOutX, Fix16* pOutY)
{

    Fix16_Point point_to_sub;
    Fix16_Point sub_point;
    if (field_5C_pCar->is_on_trailer_421720())
    {
        point_to_sub = gTrailer_cp1_6FE3A8;
    }
    else
    {
        point_to_sub = g_cp1_6FDF00;
    }
    sub_point = field_38_cp1 - point_to_sub;
    Fix16 x_val = sub_point.x;
    Fix16 y_val = sub_point.y;

    Fix16 slope_val;
    Fix16 lower;
    Fix16 upper;
    Fix16* result;

    switch (field_98_surface_type)
    {
        case car_surface_type::slope_northwards_1:
            y_val = -y_val;
            // fall through
        case car_surface_type::slope_southwards_2:
            x_val = y_val;
            break;

        case car_surface_type::slope_westwards_3:
            x_val = -x_val;
            break;

        case car_surface_type::slope_eastwards_4:
            break;

        default:
            *pOutY = kFP16Zero_6FE20C;
            *pOutX = kFP16Zero_6FE20C;
            return pOutX;
    }

    switch (this->field_A5_current_slope_length)
    {
        case 1:
            slope_val = kFP16One_6FDF7C;
            break;
        case 2:
            slope_val = dword_6FE064;
            break;
        case 8:
            slope_val = dword_6FE350;
            break;
        default:
            slope_val = kFP16Zero_6FE20C;
            break;
    }
    lower = (x_val * slope_val);
    if (!this->field_A6_current_slope_left_tiles && lower > kFP16Zero_6FE20C &&
            (u8)(this->field_6C_cp3.ToInt()) == this->field_A7_current_tile_z ||
        this->field_AA_sbw && this->field_AB_tpa)
    {
        upper = k_dword_6FE210 - (this->field_6C_cp3.GetFracValue());
        if (lower < upper)
        {
            *pOutY = upper;
        }
        else
        {
            *pOutY = lower;
        }
        result = pOutX;
        *pOutX = lower;
    }
    else
    {
        if (lower > kFP16Zero_6FE20C)
        {
            if (field_5C_pCar->field_64_pTrailer)
            {
                if (field_5C_pCar->field_64_pTrailer->GetCabOrLoadedCar_407B90(field_5C_pCar)
                        ->field_58_physics->field_98_surface_type != car_surface_type::air_surface_6)
                {
                    lower = kFP16Zero_6FE20C;
                }
            }
        }
        result = pOutX;
        *pOutY = lower;
        *pOutX = lower;
    }
    return result;
}

// 9.6f 0x4A2240: field_6C_cp3 read directly everywhere, g_ZPos * a2, IsFlagSet_411930(0x2000),
// the slope test nested as (slope && frac != 0 && zpos <= cp3 + k), ComputeSlopeCorrection's two outputs.
MATCH_FUNC(0x55ad90)
void CarPhysics_B0::UpdateZPhysics_55AD90(Fix16 a2)
{
    // a2 is copied into a register at entry; its stack slot is then reused as ComputeSlopeCorrection's second
    // output (9.6f 0x4A2240 passes a local there and reads the parameter afterwards)
    Fix16 a2_ = a2;
    Fix16 zpos;

    if (field_98_surface_type == car_surface_type::air_surface_6)
    {
        if (field_6C_cp3 > kMaxZ_6FDF34)
        {
            field_6C_cp3 = kMaxZ_6FDF34;
        }
        Fix16 map_z;
        map_z = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(field_38_cp1.x, field_38_cp1.y, field_6C_cp3);
        zpos = field_6C_cp3 + (g_ZPos_6FE0AC * a2_);
        if (zpos <= map_z)
        {
            zpos = map_z;
        }
        else if (zpos > kMaxZ_6FDF34)
        {
            zpos = kMaxZ_6FDF34;
        }
    }
    else
    {
        if (field_98_surface_type == car_surface_type::unknown_surface_7 ||
            field_98_surface_type == car_surface_type::water_surface_8)
        {
            goto reset_z;
        }

        zpos = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_38_cp1.x, field_38_cp1.y);
        if (zpos == kFP16Zero_6FE20C)
        {
            zpos = k_dword_6FE210;
        }

        if (zpos >= field_6C_cp3 + kFP16Half_6FE0C0)
        {
            if (!((field_98_surface_type == car_surface_type::slope_northwards_1 ||
                   field_98_surface_type == car_surface_type::slope_southwards_2 ||
                   field_98_surface_type == car_surface_type::slope_westwards_3 ||
                   field_98_surface_type == car_surface_type::slope_eastwards_4) &&
                  Fix16::GetFracValue_42A630(zpos) != kFP16Zero_6FE20C && zpos <= field_6C_cp3 + k_dword_6FE210))
            {
                zpos = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(field_38_cp1.x, field_38_cp1.y, zpos - kFP16One64th_6FE2E0);
                if (zpos > field_6C_cp3)
                {
                    Fix16 tmp;
                    Fix16 below = *gMap_0x370_6F6268->GetGroundZBelowCoord_4E4F40(&tmp,
                                                                                    field_38_cp1.x,
                                                                                    field_38_cp1.y,
                                                                                    zpos - kFP16One64th_6FE2E0);
                    if (below > kFP16Zero_6FE20C)
                    {
                        zpos = below;
                    }
                }

                if (zpos >= field_6C_cp3 + k_dword_6FE210)
                {
                    zpos = field_6C_cp3;
                }
            }
        }

        if (zpos <= field_6C_cp3 - kFP16Half_6FE0C0 || zpos < field_6C_cp3 && field_AA_sbw && field_AB_tpa)
        {
            Fix16 corr_x;
            field_68_z_pos = *ComputeSlopeCorrection_55AB50(&corr_x, &a2);
            zpos = field_6C_cp3 + a2;
            if (a2_ != kFP16Zero_6FE20C)
            {
                field_68_z_pos /= a2_;
            }

            if (zpos > kMaxZ_6FDF34)
            {
                zpos = kMaxZ_6FDF34;
            }
        }

        if (zpos < field_6C_cp3)
        {
            UpdateSpriteFromPhysics_563670();

            field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                    field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                    zpos);

            if (field_5C_pCar->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
            {
            // shared with the unknown/water surface case
            reset_z:
                zpos = field_6C_cp3;
            }
        }
    }

    if (field_5C_pCar->IsFlagSet_411930(0x2000))
    {
        UpdateSpriteFromPhysics_563670();

        field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                zpos);

        gCar_6C_677930->field_60 = 2;
        gCar_6C_677930->field_64_zpos = kFP16Zero_6FE20C;

        gPurpleDoom_1_679208->CheckAndHandleAllCollisionsForSprite_477C30(field_5C_pCar->field_50_car_sprite, sprite_types_enum::car_2);

        gCar_6C_677930->field_64_zpos += dword_6FDFD8;
        if (gCar_6C_677930->field_64_zpos > zpos)
        {
            zpos = gCar_6C_677930->field_64_zpos;
        }
    }

    field_70_z_vel = zpos - field_6C_cp3;
    field_6C_cp3 += field_70_z_vel;
    if (a2_ != kFP16Zero_6FE20C)
    {
        field_70_z_vel /= a2_;
    }
}

// 9.6f 0x49EBE0 (a Map_0x370 method: 9.6f calls it with gMap in ecx)
inline u8 Map_0x370::GetBlockSurfaceType_49EBE0(s32 x, s32 y, s32 z, u8* pGradientSize, u8* pGradientLevel)
{
    gmp_block_info* pBlock = get_block_4DFE10(x, y, z);
    if (pBlock)
    {
        if (gGtx_0x106C_703DD4->IsRemappedWaterTile_49E540(pBlock->field_8_lid & 0x3FF))
        {
            return 7;
        }

        if ((pBlock->field_B_slope_type & 3) != 0)
        {
            if (gGtx_0x106C_703DD4->sub_49E570(pBlock->field_8_lid & 0x3FF))
            {
                return 9;
            }
            gmp_map_slope* pSlope = &gGmpSlopes_6F5BA8[pBlock->field_B_slope_type >> 2];
            *pGradientSize = pSlope->field_1_gradient_size;
            *pGradientLevel = pSlope->field_2_gradient_level;
            return pSlope->field_0_gradient_direction;
        }
    }
    return 5;
}

MATCH_FUNC(0x55b3f0)
void CarPhysics_B0::SyncZWithTrailer_55B3F0(Fix16 a2)
{
    UpdateZPhysics_55AD90(a2);

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarOnTrailerPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pCarOnTrailerPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pCarOnTrailerPhysics->UpdateZPhysics_55AD90(a2);
        SetCurrentCarInfoAndModelPhysics_562EF0();
        Fix16 ourCp3 = this->field_6C_cp3;
        Fix16 carOnTrailer_cp3 = pCarOnTrailerPhysics->field_6C_cp3;
        if (ourCp3 > carOnTrailer_cp3)
        {
            pCarOnTrailerPhysics->field_6C_cp3 = ourCp3;
            pCarOnTrailerPhysics->field_70_z_vel = this->field_70_z_vel;
        }
        else if (ourCp3 < carOnTrailer_cp3)
        {
            this->field_6C_cp3 = carOnTrailer_cp3;
            this->field_70_z_vel = pCarOnTrailerPhysics->field_70_z_vel;
        }
    }
}

MATCH_FUNC(0x55b4f0)
void CarPhysics_B0::UpdateZPosition_55B4F0(Fix16 a2)
{

    Fix16 zCoord;
    Fix16* pZCoord = gMap_0x370_6F6268->GetRailwayZCoordAtXY_4E6510(&zCoord, this->field_38_cp1.x, this->field_38_cp1.y);
    Fix16 zCoordTmp = *pZCoord;
    if (*pZCoord == kFP16Zero_6FE20C)
    {
        zCoordTmp = k_dword_6FE210;
    }

    if (zCoordTmp >= field_6C_cp3 + kFP16Half_6FE0C0)
    {
        zCoordTmp = *gMap_0x370_6F6268->GetRailwayZBelowCoord_4E6400(&zCoord, field_38_cp1.x, field_38_cp1.y, zCoordTmp - k_dword_6FE210);
        if (zCoordTmp >= field_6C_cp3 + k_dword_6FE210)
        {
            zCoordTmp = this->field_6C_cp3;
        }
    }

    Fix16 a2_;
    if (zCoordTmp <= field_6C_cp3 - kFP16Half_6FE0C0)
    {
        a2_ = a2;
        Fix16 v14;
        this->field_68_z_pos = *ComputeSlopeCorrection_55AB50(&a2, &v14);
        zCoordTmp = field_6C_cp3 + field_68_z_pos;
        if (a2_ != kFP16Zero_6FE20C)
        {
            field_68_z_pos /= a2_;
        }
    }
    else
    {
        a2_ = a2;
        if (zCoordTmp < field_6C_cp3)
        {
            UpdateSpriteFromPhysics_563670();

            field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                    field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                    zCoordTmp);

            if (field_5C_pCar->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
            {
                zCoordTmp = this->field_6C_cp3;
            }
        }
    }

    this->field_70_z_vel = zCoordTmp - this->field_6C_cp3;
    this->field_6C_cp3 += field_70_z_vel;
    if (a2_ != kFP16Zero_6FE20C)
    {
        field_70_z_vel = field_70_z_vel / a2_;
    }
}

MATCH_FUNC(0x55B7B0)
void CarPhysics_B0::UpdateZPosition_55B7B0(Fix16 a2)
{
    UpdateZPosition_55B4F0(a2);
}

MATCH_FUNC(0x55B7E0)
void CarPhysics_B0::EmitImpactParticles_55B7E0(u8 apply_to_corners_mask)
{
    Sprite* pCarSprite = field_5C_pCar->field_50_car_sprite;
    Fix16_Point box_xy;
    u32 cornerCount;

    // The original tests only al: IsOnWater_59E1D0 returns a bool there.
    if (!(u8)pCarSprite->IsOnWater_59E1D0() && !field_5C_pCar->IsMaxDamage_40F890())
    {
        if (apply_to_corners_mask == 0)
        {
            apply_to_corners_mask = pCarSprite->CheckCornerZCollisions_5A1CA0(&cornerCount);
        }
        s32 box_idx = 0;
        u8 box_corner_mask = 1;
        do
        {
            if ((apply_to_corners_mask & box_corner_mask) == box_corner_mask)
            {
                box_xy = pCarSprite->GetBoundingBoxCorner_562450(box_idx);
                gParticle_8_6FD5E8->EmitImpactParticles_53FE40(box_xy.x, box_xy.y, field_6C_cp3, field_40_linvel_1);
            }
            box_idx++;
            box_corner_mask *= 2;
        } while (box_idx < 4);
    }

    gRozza_C88_66AFE0->Type4_40BC40(pCarSprite);
}

MATCH_FUNC(0x55b970)
char_type CarPhysics_B0::ProcessGroundCollisionAndSurfaceType_55B970(char_type* check_mask)
{
    Sprite* pSprite = this->field_5C_pCar->field_50_car_sprite;
    // The original has an EH state from entry for an object with a destructor that has no storage
    // (declared here: declared first it moves the `xor edi` zero above the sprite loads)
    Fix16_Point unused_point;
    s32 corner_idx_ = 0;
    u8 mask_;
    u32 v29;
    this->field_AB_tpa = 0;

    if (IsInAir_55A0B0())
    {
        if (this->field_68_z_pos > -kFP16Eighth_6FE370)
        {
            this->field_68_z_pos -= kFP16One64th_6FE1AC;
        }
    }

    if (!gMap_0x370_6F6268->IsZOnGround_4E5170(this->field_38_cp1.x, this->field_38_cp1.y, this->field_6C_cp3))
    {
        this->field_9C_block_spec = 0;
        *check_mask = pSprite->CheckCornerZCollisions_5A1CA0(&v29);
        if (v29 == 1 || v29 == 2)
        {
            this->field_AB_tpa = 1;
        }

        if (!*check_mask)
        {
            if (field_98_surface_type != car_surface_type::slope_northwards_1 && field_98_surface_type != car_surface_type::slope_southwards_2 && field_98_surface_type != car_surface_type::slope_westwards_3 && field_98_surface_type != car_surface_type::slope_eastwards_4)
            {
                this->field_98_surface_type = car_surface_type::air_surface_6;
                return 0;
            }

            char_type result = pSprite->IsTouchingSlopeBlock_5A1EB0();
            if (!result)
            {
                this->field_98_surface_type = car_surface_type::air_surface_6;
                return result;
            }
        }
        else
        {
            Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
            if (!pTrailer || pTrailer->GetCabOrLoadedCar_407B90(field_5C_pCar)->field_58_physics->field_98_surface_type == car_surface_type::air_surface_6)
            {
                mask_ = 1;
                do
                {
                    if (((u8)mask_ & (u8)*check_mask) != mask_)
                    {
                        // Temporaries chained: corner, corner - cm1, / 50, each with its own EH state
                        ApplyImpulseWithTrailerRedirect_55FA10(&pSprite->GetBoundingBoxCorner_562450(corner_idx_).Sub_40AC80(field_30_cm1).Divide_442CB0(Fix16(50)));
                    }
                    ++corner_idx_;
                    mask_ *= 2;
                } while (corner_idx_ < 4);
            }
        }
    }
    else
    {
        *check_mask = 0;
        this->field_9C_block_spec =
            gMap_0x370_6F6268->GetBlockSpec_4E00A0(this->field_38_cp1.x, this->field_38_cp1.y, this->field_6C_cp3 - k_dword_6FE210);
    }

    Fix16 cp3 = this->field_6C_cp3;
    bool is_air_surface = this->field_98_surface_type == car_surface_type::air_surface_6;
    if (cp3.GetFracValue() == kFP16Zero_6FE20C)
    {
        cp3 -= k_dword_6FE210;
    }

    s32 cp3_int = cp3.ToInt();
    u8 gradient_level;
    u8 graident_size;
    s32 v28 = gMap_0x370_6F6268->GetBlockSurfaceType_49EBE0(field_38_cp1.x.ToInt(), this->field_38_cp1.y.ToInt(), cp3_int, &graident_size, &gradient_level);

    //LABEL_37:
    if (v28 != 5)
    {
        this->field_98_surface_type = v28;
        this->field_A5_current_slope_length = (char)graident_size;
        this->field_A6_current_slope_left_tiles = gradient_level;
        this->field_A7_current_tile_z = cp3_int;
        if (v28 == 7)
        {
            u8 water_mask = pSprite->GetWaterCornerMask_59E250();
            if (water_mask == 15)
            {
                this->field_98_surface_type = car_surface_type::water_surface_8;
            }
            else
            {
                s32 corner_idx = 0;
                mask_ = 1;
                do
                {
                    if ((water_mask & mask_) != mask_)
                    {
                        ApplyImpulseWithTrailerRedirect_55FA10(&field_30_cm1.Sub_40AC80(pSprite->GetBoundingBoxCorner_562450(corner_idx)).Divide_442CB0(Fix16(50)));
                    }
                    ++corner_idx;
                    mask_ *= 2;
                } while (corner_idx < 4);
            }
        }
    }
    return is_air_surface;
}

MATCH_FUNC(0x55bfe0)
void CarPhysics_B0::ProcessGroundCollisionAndEmitImpactParticles_55BFE0()
{
    u8 corner_bits1;
    u8 corner_bits2;
    char b1 = ProcessGroundCollisionAndSurfaceType_55B970((char*)&corner_bits1);
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        char b2 = pPhysics->ProcessGroundCollisionAndSurfaceType_55B970((char*)&corner_bits2);
        if (b2)
        {
            if (b1 || field_98_surface_type == car_surface_type::air_surface_6)
            {
                pPhysics->EmitImpactParticles_55B7E0(corner_bits2);
            }
        }
        SetCurrentCarInfoAndModelPhysics_562EF0();
        if ((b2 || pPhysics->field_98_surface_type == car_surface_type::air_surface_6) && b1)
        {
            EmitImpactParticles_55B7E0(corner_bits1);
        }
    }
    else if (b1)
    {
        EmitImpactParticles_55B7E0(corner_bits1);
    }
}

MATCH_FUNC(0x55c150)
char_type CarPhysics_B0::TestCollision_55C150()
{
    Sprite* pCarSprite = this->field_5C_pCar->field_50_car_sprite;
    if (!pCarSprite->CheckSpriteMovementRegion_5A2500() && !pCarSprite->QuerySpriteCollision_59E7D0(0))
    {
        Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            pCarSprite = pTrailer->field_C_pCarOnTrailer->field_50_car_sprite;
            if (!pCarSprite->CheckSpriteMovementRegion_5A2500() && !pCarSprite->QuerySpriteCollision_59E7D0(0))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    gRozza_679188.SetField24_49EF10(pCarSprite);
    return 1;
}

// https://decomp.me/scratch/Mht60 stack size issue remaining
MATCH_FUNC(0x55c3b0)
char_type CarPhysics_B0::SweepTestMovementForCollision_55C3B0(Fix16* outHitStep, Fix16* outNoHitStep)
{
    save_state_55A600();

    Fix16 movement;
    movement = ComputeRequiredSweepSteps_55A6A0();

    *outNoHitStep = kFP16Zero_6FE20C;
    *outHitStep = kFP16Zero_6FE20C;

    if (movement > k_dword_6FE210)
    {
        Fix16 stepSize = k_dword_6FE210 / movement;
        Fix16 accumulated = kFP16Zero_6FE20C;
        s32 numSteps = movement.ToInt();

        for (s32 stepIndex = 0; stepIndex < numSteps; stepIndex++)
        {
            accumulated += stepSize;

            restore_saved_physics_state_55A400();
            ApplyMovementStep_560F20(accumulated);

            if (TestCollision_55C150())
            {
                *outHitStep = accumulated;
                return 1;
            }
            else
            {
                *outNoHitStep = accumulated;
            }
        }
    }

    restore_state_55A550();
    UpdateCarAndTrailerSpriteFromPhysics_5636C0();

    if (TestCollision_55C150())
    {
        *outHitStep = k_dword_6FE210;
        return 1;
    }
    else
    {
        *outNoHitStep = k_dword_6FE210;
        return 0;
    }
}

MATCH_FUNC(0x55c560)
void CarPhysics_B0::BinarySearchCollisionTime_55C560(Fix16& a2, Fix16& a3)
{
    for (s32 i = 0; i < 3; i++)
    {
        restore_saved_physics_state_55A400();
        Fix16 total = (a3 + a2) / 2;
        ApplyMovementStep_560F20(total);
        if (TestCollision_55C150())
        {
            a2 = total;
        }
        else
        {
            a3 = total;
        }
    }
}

// https://decomp.me/scratch/A5Yhg
MATCH_FUNC(0x55c5c0)
void CarPhysics_B0::HandleMapBoundaryCollisionY_55C5C0(Fix16_Point& pPoint, Ang16 angle)
{
    Fix16_Point RelativePointVelocity;
    if (field_5C_pCar->field_50_car_sprite->GetNearestHorizontalEdgeToCoordinate_5A0A70(gRozza_679188.field_18_mapy_t1,
                                                                                        CollisionIntersectionPoint_6FE1A0,
                                                                                        gCollisionArea_6FDFC4))
    {
        stru_6FE1F0.SetXY_432860(Fix16(0), field_38_cp1.y - gRozza_679188.field_18_mapy_t1);
        RelativePointVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
    }
    else
    {
        CollisionIntersectionPoint_6FE1A0.SetXY_432860(gRozza_679188.field_14_mapx_t2, gRozza_679188.field_18_mapy_t1);
        CollisionIntersectionPoint_6FE1A0 -= pPoint;
        {
            Ang16 rot_angle(field_58_theta.rValue - angle.rValue);
            rot_angle.Normalize_406C20();
            CollisionIntersectionPoint_6FE1A0.RotateByAngle_40F6B0(rot_angle);
        }
        CollisionIntersectionPoint_6FE1A0.x += field_38_cp1.x;
        CollisionIntersectionPoint_6FE1A0.y = gRozza_679188.field_18_mapy_t1;

        if (Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint_6FE1A0.x - gRozza_679188.field_4_mapx_t1) <
            Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint_6FE1A0.x - gRozza_679188.field_8_mapx_max_t1))
        {
            CollisionIntersectionPoint_6FE1A0.x = gRozza_679188.field_4_mapx_t1;
        }
        else
        {
            CollisionIntersectionPoint_6FE1A0.x = gRozza_679188.field_8_mapx_max_t1;
        }
        RelativePointVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
        if (field_38_cp1.y < CollisionIntersectionPoint_6FE1A0.y)
        {
            stru_6FE1F0.SetXY_432860(Fix16(0), -k_dword_6FE210);
        }
        else
        {
            stru_6FE1F0.SetXY_432860(Fix16(0), k_dword_6FE210);
        }
    }
    CarPhysics_B0::HandleWorldCollision_55FD00(RelativePointVelocity);
}

// https://decomp.me/scratch/N4ktT
MATCH_FUNC(0x55c820)
void CarPhysics_B0::HandleMapBoundaryCollisionX_55C820(Fix16_Point& pPoint, Ang16 angle)
{
    Fix16_Point RelativePointVelocity;
    if (field_5C_pCar->field_50_car_sprite->GetNearestVerticalEdgeToCoordinate_5A1030(gRozza_679188.field_14_mapx_t2,
                                                                                      CollisionIntersectionPoint_6FE1A0,
                                                                                      gCollisionArea_6FDFC4))
    {
        stru_6FE1F0.SetXY_432860(field_38_cp1.x - gRozza_679188.field_14_mapx_t2, Fix16(0));
        RelativePointVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
    }
    else
    {
        CollisionIntersectionPoint_6FE1A0.SetXY_432860(gRozza_679188.field_14_mapx_t2, gRozza_679188.field_18_mapy_t1);
        CollisionIntersectionPoint_6FE1A0 -= pPoint;
        {
            Ang16 rot_angle(field_58_theta.rValue - angle.rValue);
            rot_angle.Normalize_406C20();
            CollisionIntersectionPoint_6FE1A0.RotateByAngle_40F6B0(rot_angle);
        }
        CollisionIntersectionPoint_6FE1A0.y += field_38_cp1.y;
        CollisionIntersectionPoint_6FE1A0.x = gRozza_679188.field_14_mapx_t2;

        if (Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint_6FE1A0.y - gRozza_679188.field_C_mapy_t2) <
            Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint_6FE1A0.y - gRozza_679188.field_10_mapy_max_t2))
        {
            CollisionIntersectionPoint_6FE1A0.y = gRozza_679188.field_C_mapy_t2;
        }
        else
        {
            CollisionIntersectionPoint_6FE1A0.y = gRozza_679188.field_10_mapy_max_t2;
        }
        RelativePointVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
        if (field_38_cp1.x < CollisionIntersectionPoint_6FE1A0.x)
        {
            stru_6FE1F0.SetXY_432860(-k_dword_6FE210, Fix16(0));
        }
        else
        {
            stru_6FE1F0.SetXY_432860(k_dword_6FE210, Fix16(0));
        }
    }
    CarPhysics_B0::HandleWorldCollision_55FD00(RelativePointVelocity);
}

// 9.6f 0x4A4170
MATCH_FUNC(0x55ca70)
void CarPhysics_B0::DispatchCollision_55CA70(Fix16_Point& a2, Ang16 a3)
{
    Fix16_Point arg0;
    u8 hitType;
    //v7 = 0;
    switch (gRozza_679188.field_0_type)
    {
        case 1:
            HandleMapBoundaryCollisionY_55C5C0(a2, a3);
            break;
        case 2:
            HandleMapBoundaryCollisionX_55C820(a2, a3);
            break;
        case 3:
            // TODO: Likely wrong arguments here
            CollisionIntersectionPoint_6FE1A0 = field_5C_pCar->field_50_car_sprite->FindCollisionIntersectionPoint_5A2710(gRozza_679188.field_20_pSprite,
                                                                                                     a2,
                                                                                                     a3,
                                                                                                     gCollisionArea_6FDFC4,
                                                                                                     gOtherCollisionArea_6FDFCC,
                                                                                                     hitType);

            Car_BC* pCar = gRozza_679188.field_20_pSprite->AsCar_40FEB0();
            if (pCar)
            {
                HandleCarCollision_55FF20(pCar);
            }
            else
            {
                Char_B4* pB4 = gRozza_679188.field_20_pSprite->AsCharB4_40FEA0();
                if (pB4)
                {
                    ProcessPedImpact_560B40(pB4, hitType);
                }
                else
                {
                    Object_2C* p2C = gRozza_679188.field_20_pSprite->As2C_40FEC0();
                    HandleObjectCollision_5606C0(p2C, gCollisionArea_6FDFC4);
                }
            }
            break;
    }

    gRozza_C88_66AFE0->OtherType_40BBA0(field_5C_pCar->field_50_car_sprite, gCollisionDamage_6FE33C);
}

// https://decomp.me/scratch/0TpGe
MATCH_FUNC(0x55cbb0)
void CarPhysics_B0::ReplayAndDispatchCollision_55CBB0(Fix16 a2, Fix16 a3)
{
    Fix16_Point point;
    CarPhysics_B0* pPhysics = gRozza_679188.field_24->AsCar_40FEB0()->field_58_physics;

    restore_saved_physics_state_55A400();
    ApplyMovementStep_560F20(a2);

    point = pPhysics->field_38_cp1;
    Ang16 theta = pPhysics->field_58_theta;

    restore_saved_physics_state_55A400();
    ApplyMovementStep_560F20(a3);
    if (pPhysics == this)
    {
        DispatchCollision_55CA70(point, theta);
    }
    else
    {
        pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pPhysics->DispatchCollision_55CA70(point, theta);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
}

// 9.6f 0x4A0120
MATCH_FUNC(0x55d200)
void CarPhysics_B0::SpawnSkidSegment_55D200(s32 box_idx, Fix16_Point arg_4, s32 surface)
{
    Fix16_Point t;
    Fix16_Point v15;
    Fix16 len; // declared up here: its stack slot is the dead box_idx one, shared with the / 2 temporary

    arg_4.RotateByAngle_40F6B0_out_of_line(field_58_theta);

    arg_4 += this->field_38_cp1;

    char_type map_ret = gMap_0x370_6F6268->sub_4E52A0(arg_4.x, arg_4.y, field_6C_cp3);
    if (map_ret == 5 || surface == 3 && map_ret != 7)
    {
        Fix16_Point* pBoxCorner_ = &this->field_10_last_skid_pos[(u8)box_idx];
        pBoxCorner_->clear_41E1E0();
    }
    else
    {
        Fix16_Point* pBoxCorner = &this->field_10_last_skid_pos[(u8)box_idx];
        if (!pBoxCorner->IsNull())
        {
            t = arg_4 - *pBoxCorner;
            v15 = pBoxCorner->Add_40AC50(arg_4) / 2;
            Fix16 obj_x = v15.x;
            Fix16 obj_y = v15.y;

            Ang16 r = t.atan2_40F790();
            len = t.GetLength_all_out_of_line_abs();
            if (len > kFP16Zero_6FE20C)
            {
                Object_2C* pObj =
                    gObject_5C_6F8F84->NewPhysicsObj_5299B0(get_skid_obj_type_55D490(surface, len), obj_x, obj_y, field_6C_cp3, r);
                if (pObj)
                {
                    if (pObj->field_4->sub_5A19C0())
                    {
                        pObj->RequestRemoval_5290A0();
                    }
                }
            }
        }
        *pBoxCorner = arg_4;
    }
}

MATCH_FUNC(0x55D490)
EXPORT s32 __stdcall get_skid_obj_type_55D490(s32 surface, Fix16 box_idx)
{
    if (box_idx <= kFP16One16th_6FE270)
    {
        switch (surface)
        {
            case 0:
                return 117;
            case 2:
                return 116;
            case 1:
                return 118;
            case 3:
                return 126;
        }
    }
    else if (box_idx <= kFP16Quarter_6FDFD4)
    {
        switch (surface)
        {
            case 0:
                return 120;
            case 2:
                return 119;
            case 1:
                return 121;
            case 3:
                return 125;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\physics.cpp", 2331, surface);
        }
    }
    else if (box_idx <= dword_6FE178)
    {
        switch (surface)
        {
            case 0:
                return 250;
            case 2:
                return 249;
            case 1:
                return 253;
            case 3:
                return 124;
        }
    }
    else
    {
        switch (surface)
        {
            case 0:
                return 147;
            case 2:
                return 146;
            case 1:
                return 144;
            case 3:
                return 145;
        }
    }
    return 117;
}

// https://decomp.me/scratch/y9UHj
MATCH_FUNC(0x55dc00)
void CarPhysics_B0::UpdateWheelSkidEffects_55DC00()
{

    if (field_5C_pCar->IsOnScreenForAnyPlayer_43B730())
    {
        Fix16 rear_wheel_offset_;
        Fix16 front_wheel_offset_;

        s32 b_d9C = (field_9C_block_spec == 2 || field_9C_block_spec == 10) ? 2 : 0;

        Fix16 half_width = field_5C_pCar->get_car_width() * dword_6FE004;
        rear_wheel_offset_ = field_5C_pCar->ApplyScale_421910(gCarInfo_2C_6FE0E4->field_8_rear_wheel_offset);
        front_wheel_offset_ = field_5C_pCar->ApplyScale_421910(gCarInfo_2C_6FE0E4->field_4_front_wheel_offset);

        if (field_98_surface_type == car_surface_type::unknown_surface_7 || field_98_surface_type == car_surface_type::water_surface_8 || field_98_surface_type == car_surface_type::unknown_surface_9)
        {
            SpawnSkidSegment_55D200(0, Fix16_Point(-half_width, rear_wheel_offset_), 3); // spawns the skid obj?
            SpawnSkidSegment_55D200(1, Fix16_Point(half_width, rear_wheel_offset_), 3);
        }
        else if ((field_88_rear_skid >= gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2 ||
                 field_AC_drive_wheels_locked_q > 0 && gCarInfo_2C_6FE0E4->field_20_front_drive_bias > kFP16Zero_6FE20C) &&
                field_98_surface_type != car_surface_type::air_surface_6)
        {
            SpawnSkidSegment_55D200(0, Fix16_Point(-half_width, rear_wheel_offset_), b_d9C);
            SpawnSkidSegment_55D200(1, Fix16_Point(half_width, rear_wheel_offset_), b_d9C);
        }
        else
        {
            field_10_last_skid_pos[0].reset();
            field_10_last_skid_pos[1].reset();
        }

        if (field_98_surface_type == car_surface_type::unknown_surface_7 || field_98_surface_type == car_surface_type::water_surface_8 || field_98_surface_type == car_surface_type::unknown_surface_9)
        {
            SpawnSkidSegment_55D200(3, Fix16_Point(-half_width, front_wheel_offset_), 3);
            SpawnSkidSegment_55D200(2, Fix16_Point(half_width, front_wheel_offset_), 3);
        }
        else if ((field_84_front_skid >= gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1 ||
                  field_AC_drive_wheels_locked_q > 0 && gCarInfo_48_6FE258->field_8_front_drive_bias > kFP16Zero_6FE20C) &&
                 field_98_surface_type != car_surface_type::air_surface_6)
        {
            SpawnSkidSegment_55D200(3, Fix16_Point(-half_width, front_wheel_offset_), b_d9C);
            SpawnSkidSegment_55D200(2, Fix16_Point(half_width, front_wheel_offset_), b_d9C);
        }
        else
        {
            field_10_last_skid_pos[3].reset();
            field_10_last_skid_pos[2].reset();
        }
    }
    else
    {
        field_10_last_skid_pos[0].reset();
        field_10_last_skid_pos[1].reset();
        field_10_last_skid_pos[2].reset();
        field_10_last_skid_pos[3].reset();
    }

    if (field_AC_drive_wheels_locked_q > 0)
    {
        field_AC_drive_wheels_locked_q--;
    }
}

MATCH_FUNC(0x55e260)
void CarPhysics_B0::DoSkidmarks_55E260()
{
    if (!bSkip_skidmarks_67D585)
    {
        CarPhysics_B0::UpdateWheelSkidEffects_55DC00();
        Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
            pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
            pPhysics->field_84_front_skid = kFP16Zero_6FE20C;
            pPhysics->field_88_rear_skid = kFP16Zero_6FE20C;
            pPhysics->UpdateWheelSkidEffects_55DC00();
            CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
        }
    }
}

MATCH_FUNC(0x55e470)
char_type CarPhysics_B0::StepMovementAndCollisions_55E470()
{
    s32 sprites_array_idx = 0;
    char_type ret_val = 0;
    s32 k2Counter = 2;

    Sprite* sprites_array[4];

    Fix16 a2;
    Fix16 a3;

    while (gRemainingTimeStep_6FE198 >= kFP16Eighth_6FE370)
    {
        gRozza_679188.Reset_4637B0();
        this->field_70_z_vel = 0; // fp 0
        save_physics_state_55A4B0();
        ApplyMovementStep_560F20(k_dword_6FE210);
        UpdateTrailerPhysicsFromTowingCar_559A40();
        if (SweepTestMovementForCollision_55C3B0(&a2, &a3))
        {
            ret_val = 1;
            if (gRozza_679188.field_20_pSprite)
            {
                for (s32 i = 0; i < sprites_array_idx; i++)
                {
                    if (sprites_array[i] == gRozza_679188.field_20_pSprite)
                    {
                        restore_saved_physics_state_55A400();
                        UpdateCarAndTrailerSpriteFromPhysics_5636C0();
                        ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
                        return 1;
                    }
                }
            }

            BinarySearchCollisionTime_55C560(a2, a3);
            if (field_5C_pCar->IsTrainModel_403BA0() && !field_40_linvel_1.HasZeroComponent_49E450())
            {
                a3 = kFP16Zero_6FE20C;
            }
            ReplayAndDispatchCollision_55CBB0(a2, a3);
        }

        gRemainingTimeStep_6FE198 = (gRemainingTimeStep_6FE198 * (k_dword_6FE210 - a3));
        sprites_array[sprites_array_idx++] = gRozza_679188.field_20_pSprite;

        if ((gRozza_679188.IsCharB4_49EF20() || gRozza_679188.IsObj2C_477A10()) && k2Counter < 4)
        {
            ++k2Counter;
        }

        ProcessGroundCollisionAndEmitImpactParticles_55BFE0();

        if (sprites_array_idx >= k2Counter)
        {
            break;
        }
    }
    return ret_val;
}

MATCH_FUNC(0x55eb80)
char_type CarPhysics_B0::CheckAndHandleCarAndTrailerCollisions_55EB80()
{
    gCar_6C_677930->field_68 = 0;

    char_type bCollision = gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_5C_pCar->field_50_car_sprite);
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        bCollision |= gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(pTrailer->field_C_pCarOnTrailer->field_50_car_sprite);
    }

    return bCollision;
}

MATCH_FUNC(0x55ec30)
void CarPhysics_B0::ApplyForwardEngineForce_55EC30()
{
    Ang16 theta;
    if (field_94_is_backward_gas_on)
    {
        theta = field_58_theta + kAng180_6FE12A;
    }
    else
    {
        theta = field_58_theta;
    }

    if (stru_6FE1F0.x > kFP16Zero_6FE20C)
    {
        if (theta > kAng270_6FE154)
        {
            ApplyAngularImpulse_55F970(k_dword_6FDFA4);
        }
        else if (theta > kAng180_6FE12A)
        {
            ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
        }
    }
    else
    {
        if (theta < kAng90_6FE00C)
        {
            ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
        }
        else if (theta < kAng180_6FE12A)
        {
            ApplyAngularImpulse_55F970(k_dword_6FDFA4);
        }
    }

    ApplyForceScaledByMass_55F9A0(stru_6FE1F0.NormalizeSafe_442AD0() * stru_6FDF80);
    field_AA_sbw = 1;
}

// https://decomp.me/scratch/foNCl
MATCH_FUNC(0x55ef20)
void CarPhysics_B0::ApplyReverseEngineForce_55EF20()
{

    Ang16 theta;
    if (field_94_is_backward_gas_on)
    {
        theta = field_58_theta + kAng180_6FE12A;
    }
    else
    {
        theta = field_58_theta;
    }

    if (stru_6FE1F0.y > kFP16Zero_6FE20C)
    {
        if (theta > kAng90_6FE00C && theta < kAng180_6FE12A)
        {
            ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
        }
        else if (theta > kAng180_6FE12A && theta < kAng270_6FE154)
        {
            ApplyAngularImpulse_55F970(k_dword_6FDFA4);
        }
    }
    else
    {
        // Moving backwards or stationary
        if (theta < kAng90_6FE00C)
        {
            ApplyAngularImpulse_55F970(k_dword_6FDFA4);
        }
        else if (theta > kAng270_6FE154)
        {
            ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
        }
    }

    ApplyForceScaledByMass_55F9A0(stru_6FE1F0.NormalizeSafe_442AD0() * stru_6FDF80);
    this->field_AA_sbw = 1;
}

// matches on decompme: https://decomp.me/scratch/Hyun8
MATCH_FUNC(0x55f020)
void CarPhysics_B0::ApplyTurningForce_55F020()
{
    Fix16_Point v6;
    Fix16 v17 = dword_6FE358;
    Object_2C* pObj = gRozza_679188.field_20_pSprite->As2C_40FEC0();
    Fix16 v4;

    if (pObj && pObj->field_18_model == objects::diagonal_wall_collision_obj_166)
    {
        v4 = k_dword_6FDFA4;
        v17 = stru_6FDF80;
    }
    else
    {
        if (field_5C_pCar->is_driven_by_player())
        {
            v4 = dword_6FE078;
            v17 = dword_6FE390;
        }
        else
        {
            v4 = dword_6FE080;
            v17 = dword_6FE358;
        }
    }

    v6 = (field_38_cp1 - CollisionIntersectionPoint_6FE1A0);

    v6.RotateByAngle_40F6B0(-field_58_theta);

    if (v6.x >= kFP16Zero_6FE20C)
    {
        v4 = -v4;
    }
    ApplyAngularImpulse_55F970(v4);
    ApplyForceScaledByMass_55F9A0(stru_6FE1F0.NormalizeSafe_442AD0().Multiply_438FE0(v17));
}

MATCH_FUNC(0x55f240)
char_type CarPhysics_B0::ApplyMovementCommand_55F240()
{
    switch (gRozza_679188.field_0_type)
    {
        case 1:
            ApplyReverseEngineForce_55EF20();
            return 1;
        case 2:
            ApplyForwardEngineForce_55EC30();
            return 1;
        case 3:
            ApplyTurningForce_55F020();
            field_AA_sbw = 0;
            break;
    }
    return 0;
}

MATCH_FUNC(0x55f280)
char_type CarPhysics_B0::ProcessCollisionAndClampVelocity_55F280()
{
    char_type movementApplied = 0;

    Fix16_Point_POD cm1 = this->field_30_cm1;

    char_type stepResult;
    while (1)
    {
        gRemainingTimeStep_6FE198 = k_dword_6FE210;
        stepResult = StepMovementAndCollisions_55E470();
        const char_type prevStepResult = stepResult;

        if (movementApplied)
        {
            if (gRemainingTimeStep_6FE198 == k_dword_6FE210)
            {
                this->field_40_linvel_1.reset();
                this->field_74_ang_vel_rad = kFP16Zero_6FE20C;
            }
            break;
        }

        this->field_AA_sbw = 0;

        if (gRemainingTimeStep_6FE198 != k_dword_6FE210 || !stepResult)
        {
            break;
        }

        if (IsAccelerationOrReverseOn_55A180())
        {
            movementApplied = ApplyMovementCommand_55F240();

            if (movementApplied)
            {
                UpdateLinearAndAngularAccel_560EB0();
                IntegrateAndClampVelocities_5610B0();
                continue;
            }
        }

        stepResult = prevStepResult;
        break;
    }

    this->field_0_vel_read_only.x = this->field_30_cm1.x - cm1.x;
    this->field_0_vel_read_only.y = this->field_30_cm1.y - cm1.y;

    return stepResult;
}

MATCH_FUNC(0x55f330)
void CarPhysics_B0::StepPhysics_55F330()
{
    gRemainingTimeStep_6FE198 = k_dword_6FE210;
    save_physics_state_55A4B0();
    ApplyMovementStep_560F20(k_dword_6FE210);
}

MATCH_FUNC(0x55f360)
char_type CarPhysics_B0::CheckPendingCollision_55F360()
{
    // 9.6f: Car_BC flag helpers 0x414F70 (IsFlagSet_411930(0x2000)) and 0x49EFD0 (clear 0x2000)
    // (inlined, using IsFlagSet_411930 here changes the code)
    if ((this->field_5C_pCar->field_78_flags & 0x2000) != 0)
    {
        gCar_6C_677930->field_60 = 1;
        if (gPurpleDoom_1_679208->CheckAndHandleAllCollisionsForSprite_477C30(field_5C_pCar->field_50_car_sprite, 2))
        {
            return 1;
        }
        this->field_5C_pCar->field_78_flags &= ~0x2000u;
    }
    return 0;
}

// TODO: Probably move & Rename to ComputeImpulse or something
// https://decomp.me/scratch/dN85v
MATCH_FUNC(0x55F3B0)
EXPORT Fix16_Point __stdcall ComputeLineLineIntersection_55F3B0(Fix16 OwnerMass,
                                                                Fix16 TargetMass,
                                                                Fix16_Point& RelativeVelocity,
                                                                Fix16_Point& DistToCollision_ByRef,
                                                                Fix16_Point& CollisionIntersectPoint,
                                                                Fix16_Point& CoM_related,
                                                                Fix16_Point& a8,
                                                                Fix16 OwnerMomOfInertia,
                                                                Fix16 TargetMomOfInertia,
                                                                Fix16 offset)
{
    // The original enters EH state 2: three Fix16_Point locals are constructed up front
    Fix16_Point DistOrthogonalToCollision;
    Fix16_Point DirectionFromCoM_to_Collision;
    Fix16_Point Impulse;

    if (RelativeVelocity.IsNull() || DistToCollision_ByRef.IsNull())
    {
        return stru_6FE300;
    }

    // The function runs out of inline expansions: most Fix16 operators are the out-of-line copies
    Fix16 OwnerMassFactor = k_dword_6FE210 / OwnerMass;
    DistOrthogonalToCollision = (CollisionIntersectPoint - CoM_related).Rotate90CCW_5605E0();
    DirectionFromCoM_to_Collision = DistToCollision_ByRef.NormalizeSafe_442AD0(); // vector unit 1, supposedly

    Fix16 VelocityFactor = -(k_dword_6FE210 + offset) * DotProductOOL_49E500(RelativeVelocity, DirectionFromCoM_to_Collision);

    Fix16 MassFactor;
    if (TargetMass == kFP16MinusOne_6FDF1C) // infinite mass
    {
        MassFactor = DotProductOOL_49E500(DirectionFromCoM_to_Collision, DirectionFromCoM_to_Collision) * OwnerMassFactor
            + Square_49E0E0(DotProductOOL_49E500(DistOrthogonalToCollision, DirectionFromCoM_to_Collision)) / OwnerMomOfInertia;
    }
    else
    {
        Fix16 TargetMassFactor = k_dword_6FE210 / TargetMass;
        Fix16_Point TargetOrthogonal = (CollisionIntersectPoint - a8).Rotate90CCW_5605E0();

        // DotProduct_49E500 is out of line here (0x560680)
        MassFactor =
            (DotProduct_560680(DirectionFromCoM_to_Collision, DirectionFromCoM_to_Collision)
                 .Multiply_408680(OwnerMassFactor.Add_408660(TargetMassFactor))
                 .Add_408660(Square_49E0E0(DotProduct_560680(DistOrthogonalToCollision, DirectionFromCoM_to_Collision))
                                 .Divide_436A20(OwnerMomOfInertia)))
                .Add_408660(
                    Square_49E0E0(DotProduct_560680(TargetOrthogonal, DirectionFromCoM_to_Collision)).Divide_436A20(TargetMomOfInertia));
    }

    // scale vector norm by factors, so direction is kept
    Fix16 ImpulseScale;
    ImpulseScale = VelocityFactor.Divide_436A20(MassFactor);
    Impulse = DirectionFromCoM_to_Collision.Multiply_438FE0(ImpulseScale);
    return Impulse;
}

MATCH_FUNC(0x55f740)
void CarPhysics_B0::ApplyForceWithTrailerRedirect_55F740(Fix16_Point* a2, Fix16_Point* a3)
{

    if (field_5C_pCar->is_on_trailer_421720())
    {
        CarPhysics_B0* pB0 = field_5C_pCar->field_64_pTrailer->field_8_truck_cab->field_58_physics;
        pB0->SetCurrentCarInfoAndModelPhysics_562EF0();
        pB0->ApplyForceAndIntegrate_55F7A0(a2, *a3);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        ApplyForceAndIntegrate_55F7A0(a2, *a3);
    }
}

MATCH_FUNC(0x55f7a0)
void CarPhysics_B0::ApplyForceAndIntegrate_55F7A0(Fix16_Point* a2, Fix16_Point a3)
{
    ApplyForceAtPoint_55F800(a2, &a3, 0);
    UpdateLinearAndAngularAccel_560EB0();
    IntegrateAndClampVelocities_5610B0();
}

// GetLength_41E260 as inlined into ApplyImpactForcesAndDamage_55FA60: both Abs are called out of line
// (Abs_436A50) although the first multiply is inlined, which the plain inline can't give (Abs and operator*
// have the same front-end size, 57).
static inline Fix16 GetLength_AbsOutOfLine_41E260(Fix16_Point& p)
{
    if (p.x == FIX16_POINT_ZERO)
    {
        return Fix16::Abs_436A50(p.y);
    }
    else
    {
        if (p.y == FIX16_POINT_ZERO)
        {
            return Fix16::Abs_436A50(p.x);
        }
        else
        {
            return Fix16::SquareRoot(p.x * p.x + p.y * p.y);
        }
    }
}

// 9.6f 0x4A0850
MATCH_FUNC(0x55f800)
void CarPhysics_B0::ApplyForceAtPoint_55F800(Fix16_Point* a2, Fix16_Point* a3, s32 bRotate)
{
    Fix16_Point_POD point(a2->x, a2->y);

    switch (bRotate)
    {
        case 1:
            point.RotateByAngle_40F6B0(field_58_theta);
            point += field_38_cp1;
            break;
    }

    field_48_force_accum += *a3;

    Fix16 v11 = (point.y - field_30_cm1.y) * a3->x;
    Fix16 v9 = (point.x - field_30_cm1.x) * a3->y;
    Fix16 v10 = v9 - v11;
    field_7C_torque_accum = field_7C_torque_accum + v10;
}

MATCH_FUNC(0x55f930)
void CarPhysics_B0::AccumulateImpulse_55F930(Fix16_Point* a2)
{
    field_48_force_accum += (*a2 * gCarInfo_48_6FE258->field_4_mass);
}

// https://decomp.me/scratch/qCXRd
// Takes the direction by reference, so the negated point is read through the pointer the
// negation returns (a named local reads it from its stack slot instead)
static inline void EmitImpact_55FD00(Fix16& z, const Fix16_Point& dir)
{
    gParticle_8_6FD5E8->EmitImpactParticles_53FE40(CollisionIntersectionPoint_6FE1A0.x, CollisionIntersectionPoint_6FE1A0.y, z, dir);
}

MATCH_FUNC(0x55f970)
void CarPhysics_B0::ApplyAngularImpulse_55F970(Fix16 a2)
{
    this->field_7C_torque_accum -= (gCarInfo_2C_6FE0E4->field_0_moment_of_inertia * a2);
}

MATCH_FUNC(0x55f9a0)
void CarPhysics_B0::ApplyForceScaledByMass_55F9A0(Fix16_Point& pForce)
{
    field_48_force_accum += pForce.Multiply_438FE0(CarPhysics_B0::CalculateMass_559FF0());
}

MATCH_FUNC(0x55fa10)
void CarPhysics_B0::ApplyImpulseWithTrailerRedirect_55FA10(Fix16_Point* a2)
{
    if (field_5C_pCar->is_on_trailer_421720())
    {
        // We are on the trailer so apply impulse to the truck cab instead
        CarPhysics_B0* pPhysics = field_5C_pCar->field_64_pTrailer->field_8_truck_cab->field_58_physics;
        pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pPhysics->AccumulateImpulse_55F930(a2);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        AccumulateImpulse_55F930(a2);
    }
}

// https://decomp.me/scratch/TSKLx
MATCH_FUNC(0x55fa60)
Fix16 CarPhysics_B0::ApplyImpactForcesAndDamage_55FA60(Fix16_Point& PointOfForce, Fix16_Point& Impulse, s32 base_dmg)
{
    Fix16_Point NewImpulse;
    Fix16 ImpulseIntensity = GetLength_AbsOutOfLine_41E260(Impulse);

    if ((ImpulseIntensity / CalculateMass_559FF0()) > dword_6FE37C)
    {
        NewImpulse = Impulse;

        if (field_5C_pCar->IsFlagSet_411930(0x800))
        {
            if (!field_5C_pCar->is_driven_by_player())
            {
                Fix16 MaybeVelocity = get_car_velocity_4211C0();
                if (MaybeVelocity <= dword_6FE1D4 || field_92_is_hand_brake_on)
                {
                    NewImpulse = (Impulse / kFP16Three_6FE218);
                }
            }
        }

        field_5C_pCar->ApplyVisualDamage_43A9F0();

        if (!field_5C_pCar->IsFlagSet_411930(2))
        {
            ApplyForceWithTrailerRedirect_55F740(&PointOfForce, &NewImpulse);
            AddDamage_49EF50(base_dmg);

            if (!field_5C_pCar->is_driven_by_player())
            {
                ClearHandBrake_421260();
            }
        }
    }
    return ImpulseIntensity;
}

static inline Fix16 __stdcall DotProductInlined_49E500(Fix16_Point& Vector1, Fix16_Point& Vector2)
{
    return (Vector1.x * Vector2.x) + (Vector1.y * Vector2.y);
}

// DotProductInlined_49E500 with the out-of-line Fix16 operator copies
static inline Fix16 __stdcall DotProductOOL_49E500(Fix16_Point& Vector1, Fix16_Point& Vector2)
{
    return Vector1.x.Multiply_408680(Vector2.x).Add_408660(Vector1.y.Multiply_408680(Vector2.y));
}

// 9.6f 0x49E0E0, inlined in 10.5
static inline Fix16 __stdcall Square_49E0E0(const Fix16& value)
{
    return value.Multiply_408680(value);
}

MATCH_FUNC(0x55fc30)
void CarPhysics_B0::AccumulateImpulse_55FC30(Fix16_Point& arg0, s32 base_dmg)
{

    Fix16_Point a2;
    if (!field_5C_pCar->IsTrainModel_403BA0())
    {
        if (this->field_92_is_hand_brake_on)
        {
            a2 = (arg0 / kFP16Two_6FE214);
        }
        else
        {
            a2 = arg0;
        }

        ApplyImpulseWithTrailerRedirect_55FA10(&a2);

        u32 rng_damage = base_dmg + gpRng_67AB34->field_0_rng;
        if (rng_damage > this->field_8_total_damage_q)
        {
            this->field_8_total_damage_q = rng_damage;
        }

        if (!field_5C_pCar->is_driven_by_player())
        {
            this->field_92_is_hand_brake_on = 0;
        }
    }
}

MATCH_FUNC(0x55fd00)
void CarPhysics_B0::HandleWorldCollision_55FD00(Fix16_Point& pHitPoint)
{

    Fix16_Point Impulse = ComputeLineLineIntersection_55F3B0(CalculateMass_559FF0(),
                                                             kFP16MinusOne_6FDF1C,
                                                             pHitPoint,
                                                             stru_6FE1F0,
                                                             CollisionIntersectionPoint_6FE1A0,
                                                             ComputeCombinedCenterOfMass_559EC0(),
                                                             stru_6FE300,
                                                             GetEffectiveMomentOfInertia_55A050(),
                                                             kFP16Zero_6FE20C,
                                                             kFP16Quarter_6FE1A8);

    if (field_98_surface_type == car_surface_type::air_surface_6 && field_70_z_vel <= kFP16Zero_6FE20C)
    {
        field_68_z_pos = (-field_68_z_pos) * dword_6FDFF4;

        if (Fix16::Abs(field_68_z_pos) < kFP16One32nd_6FE118)
        {
            field_68_z_pos = kFP16Zero_6FE20C;
        }
    }

    Fix16 damage;
    gCollisionDamage_6FE33C = damage = ApplyImpactForcesAndDamage_55FA60(CollisionIntersectionPoint_6FE1A0, Impulse, 15);
    if (field_98_surface_type == car_surface_type::air_surface_6 && field_70_z_vel == kFP16Zero_6FE20C && field_68_z_pos == kFP16Zero_6FE20C &&
        field_40_linvel_1.IsNull() && damage < kFP16One_6FE098)
    {
        damage = kFP16One_6FE098;
        gCollisionDamage_6FE33C = damage;
    }
    field_5C_pCar->ApplyImpactDamage_43D5D0(damage);
    if (field_40_linvel_1.GetLength_41E260() > FastCarMinVelocity_6FE1CC)
    {
        if (!field_5C_pCar->IsMaxDamage_40F890())
        {
            EmitImpact_55FD00(field_6C_cp3, -pHitPoint);
        }
        field_5C_pCar->TryDamageArea_43D2C0(gCollisionArea_6FDFC4, gCollisionDamage_6FE33C.mValue);
    }
}

// https://decomp.me/scratch/IiClE
MATCH_FUNC(0x55ff20)
void CarPhysics_B0::HandleCarCollision_55FF20(Car_BC* pOtherCar)
{
    // Entry EH state 5: six Fix16_Point locals up front
    Fix16_Point RelativeVelocity_1;
    Fix16_Point RelativeVelocity;
    Fix16_Point DirectionBetweenCoMs_Scaled;
    Fix16_Point OtherCoM;
    Fix16_Point ThisCoM;
    Fix16_Point ImpulseForce;

    Fix16 ThisCarMass = CalculateMass_559FF0();
    pOtherCar->SetupCarPhysicsAndSpriteBinding_43BCA0();

    CarPhysics_B0* OtherCarPhysics = pOtherCar->field_58_physics;
    Car_BC* pThisCar;

    OtherCarPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
    RelativeVelocity_1 = OtherCarPhysics->ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
    SetCurrentCarInfoAndModelPhysics_562EF0();
    RelativeVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0) - RelativeVelocity_1;
    ThisCoM = ComputeCombinedCenterOfMass_559EC0();
    OtherCoM = OtherCarPhysics->ComputeCombinedCenterOfMass_559EC0();
    stru_6FE1F0 = ThisCoM - CollisionIntersectionPoint_6FE1A0;

    ImpulseForce = ComputeLineLineIntersection_55F3B0(ThisCarMass,
                                                      OtherCarPhysics->CalculateMass_559FF0(),
                                                      RelativeVelocity,
                                                      stru_6FE1F0,
                                                      CollisionIntersectionPoint_6FE1A0,
                                                      ThisCoM,
                                                      OtherCoM,
                                                      GetEffectiveMomentOfInertia_55A050(),
                                                      OtherCarPhysics->GetEffectiveMomentOfInertia_55A050(),
                                                      kFP16Half_6FE0D0);

    // If it's falling at another car
    if (field_98_surface_type == car_surface_type::air_surface_6 &&
        pOtherCar->field_50_car_sprite->field_1C_zpos != field_5C_pCar->field_50_car_sprite->field_1C_zpos)
    {
        field_68_z_pos = dword_6FDFF4 * (-field_68_z_pos);

        if (Fix16::Abs(field_68_z_pos) < kFP16One32nd_6FE118)
        {
            field_68_z_pos = kFP16Zero_6FE20C;
        }

        DirectionBetweenCoMs_Scaled = (ThisCoM - OtherCoM).NormalizeSafe_442AD0().DivideInl_55F9E0(10);

        // sub_49EFE0 and CanCollideOver_4216E0 with get_car_info_5AA3B0 and the driver check out of line
        // (IsDrivenByNonPlayer_564300)
        pThisCar = field_5C_pCar;
        if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(pThisCar->field_84_car_info_idx)->is_0x1_41FF00() &&
            (pThisCar->IsTank_411900() || !pThisCar->IsDrivenByNonPlayer_564300()) &&
            !gGtx_0x106C_703DD4->get_car_info_5AA3B0(pOtherCar->field_84_car_info_idx)->is_0x1_41FF00())
        {
            field_5C_pCar->sub_49EFC0();
        }
        else
        {
            AccumulateImpulse_55FC30(DirectionBetweenCoMs_Scaled, 50);
            OtherCarPhysics->AccumulateImpulse_55FC30(DirectionBetweenCoMs_Scaled.Negate_40ACB0(), 50);
        }

        gCollisionDamage_6FE33C = (ThisCarMass * Fix16::Abs(field_70_z_vel)) * 50;
    }
    else
    {
        gCollisionDamage_6FE33C = 0;
    }

    u8 bGreatCollision;

    // Implement developments of collision with CopCar
    // sub_49EFE0 with get_car_info_5AA3B0 called out of line
    pThisCar = field_5C_pCar;
    if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(pThisCar->field_84_car_info_idx)->is_0x1_41FF00() &&
        (pThisCar->IsTank_411900() || !pThisCar->sub_4214F0()) &&
        !gGtx_0x106C_703DD4->get_car_info_5AA3B0(pOtherCar->field_84_car_info_idx)->is_0x1_41FF00() &&
        ImpulseForce.GetLength_all_out_of_line_abs_y_negate_2() > dword_6FDFD8 && GetLinearSpeed_4211A0() > dword_6FE1C4)
    {
        bGreatCollision = true;
        field_5C_pCar->sub_49EFC0();
        if (pOtherCar->IsPoliceCar_439EC0())
        {
            Ped* pDriver = field_5C_pCar->GetEffectiveDriver_43E990();
            if (pDriver && pDriver->is_player_41B0A0() && pDriver->field_20A_wanted_points < 600)
            {
                pDriver->field_20A_wanted_points = 600;
            }
        }
    }
    else
    {
        bGreatCollision = false;
        gCollisionDamage_6FE33C += ApplyImpactForcesAndDamage_55FA60(CollisionIntersectionPoint_6FE1A0, ImpulseForce, 50);
    }
    field_5C_pCar->AssignDriverBlameForExplosion_43B7B0(pOtherCar);

    // The other car's driver scores for the damage done to this car
    s16 damage = field_5C_pCar->ApplyImpactDamage_43D5D0(gCollisionDamage_6FE33C);
    if (damage > 200)
    {
        Ped* pDriver = pOtherCar->GetEffectiveDriver_43E990();
        if (pDriver && pDriver->is_player_41B0A0())
        {
            pDriver->field_15C_player->field_2D4_scores.AwardCarDamageScore_593030(field_5C_pCar, damage);
        }
    }

    if (!bGreatCollision)
    {
        OtherCarPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        OtherCarPhysics->ApplyImpactForcesAndDamage_55FA60(CollisionIntersectionPoint_6FE1A0, ImpulseForce.Negate_40ACB0(), 50);

        // A train explodes the car it hits
        s16 damage_2;
        if (field_5C_pCar->IsTrainModel_403BA0())
        {
            pOtherCar->HandleCarExplosion_43D840(19);
            damage_2 = 32000;
        }
        else
        {
            damage_2 = pOtherCar->ApplyImpactDamage_43D5D0(gCollisionDamage_6FE33C);
        }

        // This car's driver scores for the damage done to the other car
        if (damage_2 > 200)
        {
            Ped* pDriver = field_5C_pCar->GetEffectiveDriver_43E990();
            if (pDriver && pDriver->is_player_41B0A0())
            {
                pDriver->field_15C_player->field_2D4_scores.AwardCarDamageScore_593030(pOtherCar, damage_2);
            }
        }
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }

    if (field_40_linvel_1.GetLength_all_out_of_line_abs_y_negate_2() > FastCarMinVelocity_6FE1CC && !field_5C_pCar->IsMaxDamage_40F890())
    {
        EmitImpact_55FD00(field_6C_cp3, RelativeVelocity.Negate_40ACB0());
    }

    if (gCollisionDamage_6FE33C > dword_6FDFE4)
    {
        field_5C_pCar->TryDamageArea_43D2C0(gCollisionArea_6FDFC4, gCollisionDamage_6FE33C.mValue);
        pOtherCar->TryDamageArea_43D2C0(gOtherCollisionArea_6FDFCC, gCollisionDamage_6FE33C.mValue);
        if (gCollisionDamage_6FE33C > dword_6FDFF0 && pOtherCar->IsPoliceCar_439EC0())
        {
            Ped* pDriver = field_5C_pCar->GetEffectiveDriver_43E990();
            if (pDriver && pDriver->is_player_41B0A0() && pDriver->field_20A_wanted_points < 600)
            {
                pDriver->field_20A_wanted_points = 600;
            }
        }
    }

    if (field_5C_pCar->field_5C_AI)
    {
        field_5C_pCar->field_5C_AI->field_24_flags |= 0x1000u;
        field_5C_pCar->field_5C_AI->field_68_car_in_collision = pOtherCar;
    }

    if (pOtherCar->field_5C_AI)
    {
        pOtherCar->field_5C_AI->field_24_flags |= 0x1000u;
        pOtherCar->field_5C_AI->field_68_car_in_collision = field_5C_pCar;
    }
}

MATCH_FUNC(0x560680)
EXPORT Fix16 __stdcall DotProduct_560680(const Fix16_Point& Vector1, const Fix16_Point& Vector2)
{
    return (Vector1.x * Vector2.x) + (Vector1.y * Vector2.y);
}

// https://decomp.me/scratch/tc7DX
MATCH_FUNC(0x5606c0)
void CarPhysics_B0::HandleObjectCollision_5606C0(Object_2C* p2C, char_type damage_area)
{
    Fix16_Point RelativeVelocity;
    Fix16_Point Impulse;
    Fix16_Point tmp;
    Fix16_Point ObjPos = p2C->GetXY_52AE70();
    Fix16_Point arg0a;
    Fix16_Point CoM = ComputeCombinedCenterOfMass_559EC0();
    Fix16 CarMass = CalculateMass_559FF0();
    Fix16 ObjMass;
    stru_6FE1F0 = CoM - CollisionIntersectionPoint_6FE1A0;
    if (p2C->sub_482C90())
    {
        ObjMass = p2C->GetMass_482C80();
        tmp = p2C->GetSpeedVector_52AE90();
        RelativeVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0) - tmp;

        Impulse = ComputeLineLineIntersection_55F3B0(CarMass,
                                                     ObjMass,
                                                     RelativeVelocity,
                                                     stru_6FE1F0,
                                                     CollisionIntersectionPoint_6FE1A0,
                                                     CoM,
                                                     ObjPos,
                                                     GetEffectiveMomentOfInertia_55A050(),
                                                     kFP16One_6FE0F4,
                                                     kFP16One_6FE0D4);

        // Obj Reaction impulse
        tmp = Impulse.Negate_40ACB0().DivideInl_442CB0(ObjMass);
        p2C->SetMovementVectorWithRandomState_522640(tmp);
    }
    else
    {
        // infinite mass
        RelativeVelocity = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);
        Impulse = ComputeLineLineIntersection_55F3B0(CarMass,
                                                     kFP16MinusOne_6FDF1C,
                                                     RelativeVelocity,
                                                     stru_6FE1F0,
                                                     CollisionIntersectionPoint_6FE1A0,
                                                     CoM,
                                                     ObjPos,
                                                     GetEffectiveMomentOfInertia_55A050(),
                                                     kFP16One_6FE0F4,
                                                     kFP16Quarter_6FDFB8);
    }

    if (field_98_surface_type == car_surface_type::air_surface_6 && p2C->field_4->field_1C_zpos != field_5C_pCar->field_50_car_sprite->field_1C_zpos)
    {
        field_68_z_pos = dword_6FDFF4 * (-field_68_z_pos);
        if (Fix16::Abs(field_68_z_pos) < kFP16One32nd_6FE118)
        {
            field_68_z_pos = kFP16Zero_6FE20C;
        }
        arg0a = (CoM - ObjPos).NormalizeSafe_442AD0().DivideInl_55F9E0(10);
        AccumulateImpulse_55FC30(arg0a, 50);
        if (p2C->sub_482C90())
        {
            p2C->SetMovementVectorWithRandomState_522640(arg0a.Negate_40ACB0());
        }
        gCollisionDamage_6FE33C = (CarMass * Fix16::Abs_negate_out_of_line(field_70_z_vel)) * 50;
    }
    else
    {
        gCollisionDamage_6FE33C = Fix16(0);
    }

    gCollisionDamage_6FE33C += ApplyImpactForcesAndDamage_55FA60(CollisionIntersectionPoint_6FE1A0, Impulse, 15);
    field_5C_pCar->ApplyImpactDamage_43D5D0(gCollisionDamage_6FE33C);
    p2C->HandleImpact_528E50(field_5C_pCar->field_50_car_sprite);

    if (gCollisionDamage_6FE33C > dword_6FDFE4)
    {
        if (!field_5C_pCar->IsMaxDamage_40F890())
        {
            EmitImpact_55FD00(field_6C_cp3, RelativeVelocity.Negate_40ACB0());
        }
        field_5C_pCar->TryDamageArea_43D2C0(damage_area, gCollisionDamage_6FE33C.mValue);
    }
}

MATCH_FUNC(0x560b40)
void CarPhysics_B0::ProcessPedImpact_560B40(Char_B4* pCharB4, u8 hitType)
{
    Fix16_Point v16; // a local with a destructor: the original constructs 4 points up front (EH state 3)
    Fix16_Point pIntersection;
    Fix16_Point relativePointVel;
    Fix16_Point sprite_xy(pCharB4->get_sprite_xpos(), pCharB4->get_sprite_ypos());

    Fix16_Point combinedCentreOfmass = ComputeCombinedCenterOfMass_559EC0();
    relativePointVel = ComputeRelativePointVelocity_561130(&CollisionIntersectionPoint_6FE1A0);

    stru_6FE1F0 = combinedCentreOfmass - CollisionIntersectionPoint_6FE1A0;

    pIntersection = ComputeLineLineIntersection_55F3B0(CalculateMass_559FF0(),
                                                       kFP16Half_6FE2F8,
                                                       relativePointVel,
                                                       stru_6FE1F0,
                                                       CollisionIntersectionPoint_6FE1A0,
                                                       combinedCentreOfmass,
                                                       sprite_xy,
                                                       GetEffectiveMomentOfInertia_55A050(),
                                                       kFP16One_6FE070,
                                                       kFP16One_6FE3DC);

    u8 bUnknown;
    if (field_98_surface_type == car_surface_type::air_surface_6 && pCharB4->get_sprite_zpos() != field_5C_pCar->field_50_car_sprite->field_1C_zpos ||
        hitType == 0)
    {
        bUnknown = 1;
    }
    else
    {
        bUnknown = 0;
    }

    gCollisionDamage_6FE33C = pIntersection.GetLength_41E260();

    Car_BC* pCar = this->field_5C_pCar;

    // Nothrow alias: no EH state around the Negate_40ACB0 temporary
    v16 = pIntersection.Negate_40ACB0().DivideInl_442CB0(kFP16Half_6FE2F8);

    Ped* pCarDriver = field_5C_pCar->field_54_driver;
    if (pCarDriver)
    {
        pCharB4->field_7C_pPed->field_204_killer_id = pCarDriver->field_200_id;
        pCharB4->field_7C_pPed->field_264_killer_id_timer = 50;

        Ped* pPed = pCharB4->field_7C_pPed;
        if (pPed->get_field_140_49EF40() == this->field_5C_pCar)
        {
            pPed->field_290 = 3;
        }
        else
        {
            pPed->field_290 = 1;
        }
    }
    else
    {
        Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            // is_on_trailer_421720 ?
            Car_BC* pCarOnTrailer = pTrailer->field_C_pCarOnTrailer;
            if (pCarOnTrailer)
            {
                if (pCarOnTrailer == field_5C_pCar)
                {
                    Car_BC* pTruckCab = pTrailer->field_8_truck_cab;
                    if (pTruckCab)
                    {
                        pCar = pTruckCab;
                        Ped* pTruckCabDriver = pTruckCab->field_54_driver;
                        if (pTruckCabDriver)
                        {
                            pCharB4->field_7C_pPed->field_204_killer_id = pTruckCabDriver->field_200_id;
                            pCharB4->field_7C_pPed->field_264_killer_id_timer = 50;

                            Ped* pPed = pCharB4->field_7C_pPed;
                            if (pPed->get_field_140_49EF40() == this->field_5C_pCar->field_64_pTrailer->field_8_truck_cab)
                            {
                                pPed->field_290 = 3;
                            }
                            else
                            {
                                pPed->field_290 = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    pCharB4->HandleCarImpact_5538A0(pCar, bUnknown, v16.x, v16.y);
}

MATCH_FUNC(0x560eb0)
void CarPhysics_B0::UpdateLinearAndAngularAccel_560EB0()
{
    field_50_linear_accel = field_48_force_accum.Divide_442CB0(CarPhysics_B0::CalculateMass_559FF0());
    field_80_angular_accel = -field_7C_torque_accum / CarPhysics_B0::GetEffectiveMomentOfInertia_55A050();
}

MATCH_FUNC(0x560f20)
void CarPhysics_B0::ApplyMovementStep_560F20(Fix16 a2)
{
    Fix16 v3 = (gRemainingTimeStep_6FE198 * a2);

    if (v3 != kFP16Zero_6FE20C)
    {
        // 9.6f: Ang16::Fix16_To_Ang16_40F540, written out so Normalize is inlined on a register
        Ang16 tmp((s16)((v3 * field_74_ang_vel_rad).GetRaw_40F4B0() / 71), (u8)0);
        this->field_58_theta = Ang16(field_58_theta.rValue + tmp.rValue).Normalized_406C20();

        // 9.6f: Fix16_Point operator+= (0x40F680, inlined)
        this->field_30_cm1 += (field_40_linvel_1 * v3);

        UpdateCp1FromCm1_563280();

        this->field_6C_cp3 += (g_f70_6FDFE0 * a2);

        UpdateTrailerAlignment_559B40();

        if (field_5C_pCar->IsTrainModel_403BA0())
        {
            UpdateZPosition_55B7B0(a2);
            UpdateCarAndTrailerSpriteFromPhysics_5636C0();
            return;
        }
        SyncZWithTrailer_55B3F0(a2);
    }

    UpdateCarAndTrailerSpriteFromPhysics_5636C0();
}

MATCH_FUNC(0x5610b0)
void CarPhysics_B0::IntegrateAndClampVelocities_5610B0()
{
    // Integrate linear and angular velocity
    this->field_40_linvel_1 += this->field_50_linear_accel;
    this->field_74_ang_vel_rad += this->field_80_angular_accel;

    ResetForceAccumulators_55A840();

    field_40_linvel_1.ApplyDeadZone_49E3C0();
    field_74_ang_vel_rad = field_74_ang_vel_rad.ApplyDeadZone_482730(field_74_ang_vel_rad);
}

MATCH_FUNC(0x561130)
Fix16_Point CarPhysics_B0::ComputeRelativePointVelocity_561130(Fix16_Point* a3)
{
    Fix16_Point v11 = (*a3 - field_38_cp1);
    v11.RotateByAngle_40F6B0(-field_58_theta);
    v11 -= gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;
    v11.RotateByAngle_40F6B0(Ang16::Fix16_To_Ang16_40F540(field_74_ang_vel_rad) + field_58_theta);
    v11 += (field_30_cm1 + field_40_linvel_1);
    return v11 - *a3;
}

MATCH_FUNC(0x561350)
Fix16_Point CarPhysics_B0::GetPointVelocity_561350(Fix16_Point* a3)
{
    SetCarInfoGlobal_562ED0();
    return ComputeRelativePointVelocity_561130(a3);
}

MATCH_FUNC(0x561380)
Fix16_Point CarPhysics_B0::ComputePointVelocity_561380(Fix16_Point& point)
{
    // Entry EH state 3: three points up front plus local_pos in the block below. The block ends
    // after the second rotation, so later temporaries reuse local_pos's slot.
    Fix16_Point old_pos;
    Fix16_Point new_pos;
    Fix16_Point unused;

    {
        Fix16_Point local_pos;
        local_pos = point - gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;

        old_pos = local_pos;
        old_pos.RotateByAngle_40F6B0(field_58_theta);
        old_pos += field_30_cm1;

        new_pos = local_pos;
        // The angle sum and the Fix16_To_Ang16 result are temporaries: they share slots with the
        // first rotation's sin/cos (cos sits in the dead `point` parameter slot). Past the inline
        // budget both normalizing Ang16 ctors are called out of line (0x409300).
        new_pos.RotateByAngle_40F6B0(field_58_theta + Ang16::Fix16_To_Ang16_40F540(field_74_ang_vel_rad));
    }

    new_pos += field_30_cm1 + field_40_linvel_1;
    return new_pos - old_pos;
}

// https://decomp.me/scratch/5Hj13
// 9.6f 0x4A0D40
MATCH_FUNC(0x5615d0)
Fix16 CarPhysics_B0::ApplyDriveForce_5615D0(Fix16_Point& a3, Ang16 angle, Fix16_Point& a5, Fix16 a6)
{
    Fix16_Point force;
    Fix16_Point v40;
    Fix16 y_abs;
    Fix16 x_abs;

    if (field_95)
    {
        force.SetXY_432860(kFP16Zero_6FE20C, kFP16Zero_6FE20C);
    }
    else
    {
        v40 = ComputePointVelocity_561380(a3);
        v40.RotateByAngle_40F6B0(-angle);

        force.x = (v40.x * -a5.x);
        force.y = (v40.y * -a5.y);
    }

    y_abs = Fix16::Abs(v40.y);
    x_abs = Fix16::Abs(force.x);

    if (field_8C_state == 2 && !field_5C_pCar->IsTank_411900())
    {
        if ((y_abs > dword_6FE15C) || (Fix16::Abs(a6) < dword_6FE3B4))
        {
            if (field_92_is_hand_brake_on)
            {
                if ((field_AC_drive_wheels_locked_q && y_abs > kFP16Zero_6FE20C || y_abs >= dword_6FE2F4) &&
                    field_AC_drive_wheels_locked_q < 2)
                {
                    field_AC_drive_wheels_locked_q = 2;
                }
            }
        }
        else
        {
            field_AC_drive_wheels_locked_q = (((Fix16::Abs(a6)) / dword_6FE3B4) * 8).ToInt();
        }
    }

    force.y += a6;
    force.RotateByAngle_40F6B0(angle);
    ApplyForceAtPoint_55F800(&a3, &force, 1);

    return x_abs;
}

MATCH_FUNC(0x561940)
bool CarPhysics_B0::get_revs_561940()
{
    return gCarInfo_48_6FE258->field_1_turbo && this->field_60_gas_pedal >= k_dword_6FE1B8;
}

// https://decomp.me/scratch/0MzjM
// 9.6f 0x4A0F30
MATCH_FUNC(0x561970)
Fix16 CarPhysics_B0::ComputeEngineTorque_561970()
{
    Fix16 torque;
    if (field_5C_pCar->Is_engine_status_on_3_4118C0() && field_98_surface_type != car_surface_type::unknown_surface_7 && field_98_surface_type != car_surface_type::water_surface_8)
    {
        if (this->field_8C_state == 2)
        {
            // Assigned, not initialised: the slot is then free for the temporaries of each branch
            Fix16 vel_len;
            vel_len = field_40_linvel_1.GetLength_ool_abs_mul();

            if (field_94_is_backward_gas_on)
            {
                if (vel_len == kFP16Zero_6FE20C && this->field_92_is_hand_brake_on)
                {
                    torque = kFP16Zero_6FE20C;
                }
                else
                {
                    torque = -ComputeTorqueUnknown_49E8E0_ool() * gCarInfo_48_6FE258->field_34_gear1_multiplier;
                }
            }
            else if (field_93_is_forward_gas_on)
            {
                if (vel_len == kFP16Zero_6FE20C && this->field_92_is_hand_brake_on)
                {
                    torque = kFP16Zero_6FE20C;
                }
                else if (vel_len > gCarInfo_48_6FE258->field_44_gear3_speed)
                {
                    // Gear 3
                    torque = inline_ComputeTorqueFromThrottle_561DD0_ool() * gCarInfo_48_6FE258->field_3C_gear3_multiplier;
                }
                else if (vel_len > gCarInfo_48_6FE258->field_40_gear2_speed)
                {
                    // Gear 2
                    torque = inline_ComputeTorqueFromThrottle_561DD0_ool() * gCarInfo_48_6FE258->field_38_gear2_multiplier;
                }
                else
                {
                    // Gear 1. The original inlines this multiply, ours goes out of line (inline budget), so it's written out
                    torque.mValue = (s32)((ComputeTorqueUnknown_49E8E0_ool().mValue * (__int64)gCarInfo_48_6FE258->field_34_gear1_multiplier.mValue) >> 14);
                }
            }
            else
            {
                torque = kFP16Zero_6FE20C;
            }
        }
        else if (this->field_93_is_forward_gas_on)
        {
            torque = ComputeTorqueFromThrottle_561DD0();
        }
        else if (this->field_94_is_backward_gas_on)
        {
            torque = -ComputeTorqueFromThrottle_561DD0();
        }
        else
        {
            torque = kFP16Zero_6FE20C;
        }
    }
    else
    {
        torque = kFP16Zero_6FE20C;
    }
    return torque;
}

MATCH_FUNC(0x561dd0)
Fix16 CarPhysics_B0::ComputeTorqueFromThrottle_561DD0()
{
    if (get_revs_561940() != 0)
    {
        return gCarInfo_2C_6FE0E4->field_14_half_thrust +
            ((field_60_gas_pedal * ((gDamageSpeedFactor_6FE348 * gCarInfo_2C_6FE0E4->field_18_fith_thrust)))) * 2;
    }
    else
    {
        return gCarInfo_2C_6FE0E4->field_14_half_thrust +
            ((field_60_gas_pedal * ((gDamageSpeedFactor_6FE348 * gCarInfo_2C_6FE0E4->field_18_fith_thrust))));
    }
}

// https://decomp.me/scratch/46zAM
MATCH_FUNC(0x561e50)
Fix16 CarPhysics_B0::CalculateFrontWheelForce_561E50()
{
    Fix16_Point point(Fix16(0), gCarInfo_2C_6FE0E4->field_4_front_wheel_offset);
    Fix16_Point point2;

    if (CarPhysics_B0::IsInAir_55A0B0())
    {
        return kFP16Zero_6FE20C;
    }
    else
    {
        Fix16 v6;
        Fix16 front_torque = CarPhysics_B0::ComputeEngineTorque_561970() * gCarInfo_48_6FE258->field_8_front_drive_bias;

        if (field_AD_turn_direction != car_turn_direction::none_0)
        {
            v6 = k_dword_6FE210 + gCarInfo_48_6FE258->field_14_turn_in;
        }
        else
        {
            v6 = k_dword_6FE210;
        }

        // The declaration order picks the registers of the default case's loads
        Fix16 v9;
        Fix16 v10;
        Fix16 lodword_v5;
        Fix16 pointing_ang_rad;

        switch (field_A0_oil_spin_dir)
        {
            case 0:
                v9 = kFP16One_6FE3D0 * v6;
                v10 = dword_6FE3D4;
                lodword_v5 = gBrakeForce_6FE0D8 * dword_6FE320;
                pointing_ang_rad = this->field_78_pointing_ang_rad;
                break;
            case 1:
            case 2:
                if (front_torque.mValue == kFP16Zero_6FE20C.mValue)
                {
                    // Copying the zero from lodword_v5 keeps kFP16Zero_6FE20C out of a register
                    lodword_v5 = kFP16Zero_6FE20C;
                    v9 = lodword_v5;
                    v10 = lodword_v5;
                    pointing_ang_rad = this->field_78_pointing_ang_rad;
                }
                else
                {
                    v9 = kFP16One_6FE3D0 * v6;
                    v10 = dword_6FE3D4;
                    if (field_A0_oil_spin_dir == 1)
                    {
                        pointing_ang_rad = this->field_78_pointing_ang_rad + kAngFix16OneDegree_6FE3C4 * 30;
                    }
                    else
                    {
                        pointing_ang_rad = this->field_78_pointing_ang_rad + kAngFix16OneDegree_6FE3C4 * -30;
                    }
                    lodword_v5 = kFP16Zero_6FE20C;
                }
                break;
            default:
                // The original leaves all four values uninitialised here
                break;
        }

        // 9.6f: Fix16_Point::SetXY_432860
        point2.x = v9;
        point2.y = v10 + lodword_v5;

        // 9.6f: theta + Fix16_To_Ang16_40F540(pointing_ang_rad). Written with the Ang16 ctor directly,
        // since Normalize is too deep to inline through those helpers
        Ang16 rotation(pointing_ang_rad.GetRaw_40F4B0() / 71, 0);

        return CarPhysics_B0::ApplyDriveForce_5615D0(point, Ang16(field_58_theta.rValue + rotation.rValue, 0), point2, front_torque);
    }
}

MATCH_FUNC(0x5620d0)
Fix16 CarPhysics_B0::CalculateRearWheelForce_5620D0()
{
    Fix16_Point wheel_point(Fix16(0), gCarInfo_2C_6FE0E4->field_8_rear_wheel_offset);
    Fix16_Point v25;
    Fix16 v;

    if (IsInAir_55A0B0())
    {
        return kFP16Zero_6FE20C;
    }

    Fix16& front_drive_bias = gCarInfo_2C_6FE0E4->field_20_front_drive_bias;
    Fix16 v5 = ComputeEngineTorque_561970() * front_drive_bias;

    Fix16 v7;
    if (field_AD_turn_direction != car_turn_direction::none_0)
    {
        v7 = k_dword_6FE210 - gCarInfo_48_6FE258->field_14_turn_in;
    }
    else
    {
        v7 = k_dword_6FE210;
    }

    Fix16 new_x;
    Fix16 brake_force1;
    Fix16 brake_force2;
    Fix16 brake_force3;
    Fix16 pointing_ang_rad;

    if (field_A0_oil_spin_dir)
    {
        if (field_A0_oil_spin_dir > 0 && field_A0_oil_spin_dir <= 2)
        {
            brake_force2 = kFP16Zero_6FE20C;
            if (v5.mValue == kFP16Zero_6FE20C.mValue)
            {
                new_x = kFP16Zero_6FE20C;
                brake_force1 = kFP16Zero_6FE20C;
                brake_force3 = kFP16Zero_6FE20C;
                pointing_ang_rad = this->field_78_pointing_ang_rad;
                this->field_A8_hand_brake_force = 0;
            }
            else
            {
                new_x = kFP16One_6FE3D0 * v7;
                brake_force1 = dword_6FE3D4;
                if (field_A0_oil_spin_dir == 1)
                {
                    pointing_ang_rad = this->field_78_pointing_ang_rad - kAngFix16OneDegree_6FE3C4 * 30;
                    brake_force3 = kFP16Zero_6FE20C;
                    this->field_A8_hand_brake_force = 0;
                }
                else
                {
                    pointing_ang_rad = this->field_78_pointing_ang_rad - kAngFix16OneDegree_6FE3C4 * (-30);
                    brake_force3 = kFP16Zero_6FE20C;
                    this->field_A8_hand_brake_force = 0;
                }
            }
        }
    }
    else
    {
        brake_force1 = dword_6FE3D4;
        brake_force2 = gBrakeForce_6FE0D8 * dword_6FE2B0;

        if (field_92_is_hand_brake_on)
        {
            if (field_A8_hand_brake_force != (char)0x80)
            {
                field_A8_hand_brake_force++;
            }
            new_x = kFP16One_6FE3D0 * v7 * gCarInfo_48_6FE258->field_20_handbrake_slide_value;
            brake_force3 = gCarInfo_48_6FE258->field_10_brake_friction * (s32)(u8)field_A8_hand_brake_force / 128;
        }
        else
        {
            this->field_A8_hand_brake_force = 0;
            new_x = kFP16One_6FE3D0 * v7;
            brake_force3 = kFP16Zero_6FE20C;
        }
        pointing_ang_rad = this->field_78_pointing_ang_rad;
    }

    v25.SetXY_432860(new_x, brake_force1 + brake_force2 + brake_force3);
    v25.MultiplyByFix16_inline_5620D0(gCarInfo_48_6FE258->field_1C_rear_end_stability);

    // The function is at VC6's inline budget: Fix16_To_Ang16_40F540 / Ang16 - Ang16 here push the Ang16
    // constructors and y * y out of line, so the angle is built and normalised (Normalize_406C20) by hand.
    v = dword_6FE228 - field_40_linvel_1.GetLength_inline_5620D0();
    v = v / (dword_6FE340 * dword_6FE228);
    {
        Ang16 ang((pointing_ang_rad * v).GetRaw_40F4B0() / 71);
        ang.Normalize_406C20();
        Ang16 steer(field_58_theta.rValue - ang.rValue);
        steer.Normalize_406C20();
        return ApplyDriveForce_5615D0(wheel_point, steer, v25, v5);
    }
}

// Defined here (its address range), not in sprite.cpp: with the body visible in sprite.cpp
// VC6 knows it cannot throw and drops the EH state updates around its calls in
// Sprite::FindCollisionIntersectionPoint_5A2710
MATCH_FUNC(0x562450)
Fix16_Point Sprite::GetBoundingBoxCorner_562450(s32 idx)
{
    return Fix16_Point(field_C_sprite_4c_ptr->field_C_renderingRect[idx].x, field_C_sprite_4c_ptr->field_C_renderingRect[idx].y);
}

MATCH_FUNC(0x562480)
void CarPhysics_B0::ApplyThrottleInput_562480()
{
    if (this->field_93_is_forward_gas_on)
    {
        this->field_60_gas_pedal += k_dword_6FE3A0;
        if (this->field_60_gas_pedal > k_dword_6FDEFC)
        {
            this->field_60_gas_pedal = k_dword_6FDEFC;
        }
    }
    else if (this->field_94_is_backward_gas_on)
    {
        this->field_60_gas_pedal += k_dword_6FE3A0;
        if (this->field_60_gas_pedal > k_dword_6FDF88)
        {
            this->field_60_gas_pedal = k_dword_6FDF88;
        }
    }
    else
    {
        this->field_60_gas_pedal -= k_dword_6FE364;
        if (this->field_60_gas_pedal < k_dword_6FE290)
        {
            this->field_60_gas_pedal = k_dword_6FE290;
        }
    }
}

MATCH_FUNC(0x5624f0)
void CarPhysics_B0::ApplyBrakePhysics_5624F0()
{
    if (!field_91_is_foot_brake_on || field_98_surface_type == car_surface_type::unknown_surface_7 || field_98_surface_type == car_surface_type::water_surface_8)
    {
        field_64_brake_pressure = kBrakePressureStart_6FE200;
        gBrakeForce_6FE0D8 = kFP16Zero_6FE20C; // final value used in skid calcs
    }
    else
    {
        field_64_brake_pressure += kBrakePressureStep_6FE2AC;
        if (field_64_brake_pressure > kBrakePressureMax_6FE1C0)
        {
            field_64_brake_pressure = kBrakePressureMax_6FE1C0; // 0x4000 fp16
        }
        gBrakeForce_6FE0D8 = field_64_brake_pressure * gCarInfo_48_6FE258->field_10_brake_friction;
    }
}

// https://decomp.me/scratch/vdIqi
// Fix16::operator*= with the product in a temporary (StabilizeVelocityAtSpeed_562910)
static inline void MultiplyAssign_ProductTemp(Fix16& value, const Fix16& factor)
{
    __int64 product = (__int64)value.mValue * factor.mValue;
    value.mValue = (s32)(product >> 14);
}

// `a = a * b` through an inline `*=`-style wrapper. The multiply is past the inline budget, so it
// calls the operator* copy (0x408680), like the rotations' multiplies. Written directly (9.6f has
// `a = a * b`), the multiply in the else branch's product-temp helper gets the operands swapped.
static inline void MultiplyAssign_inline_408680(Fix16& a, const Fix16& b)
{
    a = a * b;
}

// https://decomp.me/scratch/f2UpJ
MATCH_FUNC(0x562560)
void CarPhysics_B0::UpdateSteeringAngle_562560()
{
    if (field_5C_pCar->field_7C_uni_num != 2)
    {
        field_78_pointing_ang_rad = CarPhysics_B0::GetTrailerAwareTurnRatio_55A100() * field_AD_turn_direction;
    }
    else
    {
        Fix16 v6 = dword_6FE228 - field_40_linvel_1.GetLength_out_of_line_x_squared();
        if (v6.mValue < dword_6FE374.mValue)
        {
            v6 = dword_6FE374;
        }
        field_78_pointing_ang_rad = GetScaledTurnRatio(v6 / (dword_6FE228 * dword_6FE104), field_AD_turn_direction);
    }
}

MATCH_FUNC(0x5626a0)
bool CarPhysics_B0::IsGasPedalPressedEnough_5626A0()
{
    Fix16 t = MinGasPedalPressure_5626C0();
    return !!(field_60_gas_pedal >= t);
}

MATCH_FUNC(0x5626c0)
Fix16 CarPhysics_B0::MinGasPedalPressure_5626C0()
{
    if (field_5C_pCar->field_7C_uni_num == 2) // ??
    {
        return kFP16Two_6FDFB0;
    }
    else
    {
        return kFP16Zero_6FE20C;
    }
}

MATCH_FUNC(0x5626f0)
void CarPhysics_B0::ApplyArrowSteerAssist_5626F0()
{
    Fix16 theta_fp = Ang16::Ang16_to_Fix16(field_58_theta);
    dword_6FE0B0 = kFP16Zero_6FE20C;
    CarAI_78* pAi = this->field_5C_pCar->field_5C_AI;
    if ((!pAi || (pAi->field_24_flags & 0x2000) != 0) && this->field_78_pointing_ang_rad == kFP16Zero_6FE20C)
    {
        if (IsGasPedalPressedEnough_5626A0())
        {
            if (IsVelocityAlignedWithHeading_40F840() && !this->field_40_linvel_1.IsNull() && !this->field_A0_oil_spin_dir)
            {
                gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(this->field_38_cp1.x.ToInt(),
                                                                             this->field_38_cp1.y.ToInt(),
                                                                             (this->field_6C_cp3 - k_dword_6FE210).ToInt());
                if (pBlock)
                {
                    if ((pBlock->field_B_slope_type & 3) == 1)
                    {
                        if ((pBlock->field_A_arrows & 0x33) != 0)
                        {
                            if (theta_fp > dword_6FE10C - k_dword_6FE134 && theta_fp < k_dword_6FE134 + dword_6FE10C)
                            {
                                dword_6FE0B0 = dword_6FE10C - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > dword_6FE278 - k_dword_6FE134 && theta_fp < dword_6FE278 + k_dword_6FE134)
                                {
                                    dword_6FE0B0 = dword_6FE278 - theta_fp;
                                }
                            }
                        }
                        else if ((pBlock->field_A_arrows & 0xCC) != 0)
                        {
                            if (theta_fp > kAngFix16HalfCircle_6FE260 - k_dword_6FE134 && theta_fp < kAngFix16HalfCircle_6FE260 + k_dword_6FE134)
                            {
                                dword_6FE0B0 = kAngFix16HalfCircle_6FE260 - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > kAngFix16FullCircle_6FE314 - k_dword_6FE134)
                                {

                                    dword_6FE0B0 = kAngFix16FullCircle_6FE314 - theta_fp;
                                }
                                else
                                {
                                    if (theta_fp < k_dword_6FE134)
                                    {
                                        dword_6FE0B0 = -theta_fp;
                                    }
                                }
                            }
                        }

                        if (dword_6FE0B0 != kFP16Zero_6FE20C)
                        {
                            theta_fp = field_5C_pCar->sub_440510();
                            if (dword_6FE0B0 > kFP16Zero_6FE20C)
                            {
                                if (dword_6FE0B0 > theta_fp)
                                {
                                    dword_6FE0B0 = theta_fp;
                                }
                            }
                            else
                            {
                                if (dword_6FE0B0 < -theta_fp)
                                {
                                    dword_6FE0B0 = -theta_fp;
                                }
                            }

                            if (bDo_show_instruments_67D64C)
                            {
                                Ped* pDriver = this->field_5C_pCar->field_54_driver;
                                if (pDriver)
                                {
                                    Player* pPlayer = pDriver->field_15C_player;
                                    if (pPlayer)
                                    {
                                        if (pPlayer->IsUser_41DC70())
                                        {
                                            gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(L"snap", 0, 64, gDebugFont_706600, 1);
                                        }
                                    }
                                }
                            }

                            this->field_74_ang_vel_rad += dword_6FE0B0;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x562910)
void CarPhysics_B0::StabilizeVelocityAtSpeed_562910()
{
    if (CarPhysics_B0::IsInAir_55A0B0())
    {
        if (Fix16::Abs(field_40_linvel_1.x) <= kFP16One128th_6FDFDC &&
            Fix16::Abs(field_40_linvel_1.y) <= kFP16One128th_6FDFDC)
        {
            field_40_linvel_1.MultiplyByFix16_49E3A0(dword_6FE334);
        }
        else
        {
            field_40_linvel_1.RotateByAngle_40F6B0(-field_58_theta);
            field_40_linvel_1.x *= dword_6FE334;
            if (field_5C_pCar->field_64_pTrailer)
            {
                field_40_linvel_1.y *= dword_6FE240;
            }
            else
            {
                field_40_linvel_1.y *= dword_6FE330;
            }
            field_40_linvel_1.RotateByAngle_40F6B0(field_58_theta);
            field_74_ang_vel_rad = field_74_ang_vel_rad * dword_6FDF18;
        }
    }
    else
    {
        if (Fix16::Abs(field_40_linvel_1.x) <= kFP16One128th_6FDFDC &&
            Fix16::Abs(field_40_linvel_1.y) <= kFP16One128th_6FDFDC)
        {
            field_40_linvel_1.MultiplyByFix16_49E3A0(dword_6FE100);
        }
        else
        {
            field_40_linvel_1.RotateByAngle_40F6B0(-field_58_theta);
            // `x *=` gives the factor in eax (imull x); the product-temp form loads x into eax first
            MultiplyAssign_ProductTemp(field_40_linvel_1.x, dword_6FE100);
            // 9.6f has `*=` (0x41E0D0) in both branches. 10.5 inlines it here and calls the
            // out-of-line copy (0x562430) in the else branch, past the inline budget. Only this
            // helper gives the original operand order (y in eax) and budget use.
            if (field_5C_pCar->field_64_pTrailer)
            {
                MultiplyAssign_ProductTemp(field_40_linvel_1.y, dword_6FDFBC);
            }
            else
            {
                field_40_linvel_1.y *= dword_6FE0FC;
            }
            field_40_linvel_1.RotateByAngle_40F6B0(field_58_theta);
            MultiplyAssign_inline_408680(field_74_ang_vel_rad, dword_6FE318);
        }
    }
}

// Out-of-line copy of Fix16_Point::RotateByAngle_40F6B0
MATCH_FUNC(0x562c20)
void Fix16_Point::RotateVelocity_562C20(const Ang16& angle)
{
    const Fix16 sin = Ang16::sine_40F500(angle);
    const Fix16 cos = Ang16::cosine_40F520(angle);

    const Fix16 x_old = x;

    x = (sin * y) + (cos * x);
    y = (cos * y) + ((-x_old) * sin);
}

// https://decomp.me/scratch/0X4pK
// 9.6f 0x4A1B20
MATCH_FUNC(0x562d00)
void CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00()
{

    Fix16_Point polar;
    if (!IsInAir_55A0B0())
    {
        Fix16 radius;
        if (IsVelocityAlignedWithHeading_40F840())
        {
            radius = gCarInfo_48_6FE258->field_28_max_speed;
        }
        else
        {
            radius = gCarInfo_48_6FE258->field_40_gear2_speed;
        }

        if (field_40_linvel_1.GetLength_41E260() > radius)
        {
            polar.FromPolar_41E210(radius, field_40_linvel_1.atan2_40F790());
            field_40_linvel_1.ClampTowardsZero_49E480(polar);
        }
    }
}

MATCH_FUNC(0x562eb0)
void CarPhysics_B0::SetModelPhysicsGlobal_562EB0()
{
    gCarInfo_48_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_5C_pCar->GetCarModelForPhysics_43A850());
}

MATCH_FUNC(0x562ed0)
void CarPhysics_B0::SetCarInfoGlobal_562ED0()
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());
    gCarInfo_2C_6FE0E4 = pInfo;
}

// https://decomp.me/scratch/Uxers
// 9.6f 0x40F760: negates a point in place (inlined in 10.5)
static inline void NegateInPlace_40F760(Fix16_Point& p)
{
    p.x = -p.x;
    p.y = -p.y;
}

MATCH_FUNC(0x562ef0)
void CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0()
{
    u8 info_idx_remapped = field_5C_pCar->GetCarModelForPhysics_43A850();
    gCarInfo_2C_6FE0E4 = gCarInfo_808_678098->GetInfoAtIdx_454840(info_idx_remapped);
    gCarInfo_48_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(info_idx_remapped);
}

MATCH_FUNC(0x562f30)
void CarPhysics_B0::ApplyInputsAndIntegratePhysics_562F30()
{
    gDamageSpeedFactor_6FE348 = field_5C_pCar->GetDamageFactorOnSpeed_439EE0();
    ApplyThrottleInput_562480();
    ApplyBrakePhysics_5624F0();
    field_84_front_skid = CalculateFrontWheelForce_561E50();
    field_88_rear_skid = CalculateRearWheelForce_5620D0();
    HandleGravityOnSlope_55AA00();
    UpdateLinearAndAngularAccel_560EB0();
    IntegrateAndClampVelocities_5610B0();
}

MATCH_FUNC(0x562fa0)
char_type CarPhysics_B0::UpdateLastMovementTimer_562FA0()
{
    if (IsNotMoving_5599D0())
    {
        if (field_90_timer_since_last_move < 255)
        {
            field_90_timer_since_last_move++;
        }
        if (field_90_timer_since_last_move >= 20u)
        {
            return true;
        }
    }
    else
    {
        field_90_timer_since_last_move = 0;
    }
    return false;
}

MATCH_FUNC(0x562fe0)
bool CarPhysics_B0::ProcessCarPhysicsStateMachine_562FE0()
{
    char carModel; // al
    char flag; // bl

    SetCurrentCarInfoAndModelPhysics_562EF0();
    carModel = field_5C_pCar->GetCarModelForPhysics_43A850();
    if (carModel != this->field_A9_car_model)
    {
        this->field_A9_car_model = carModel;
        UpdateReferencePoint_563460();
    }

    this->field_84_front_skid = kFP16Zero_6FE20C;
    this->field_88_rear_skid = kFP16Zero_6FE20C;

    switch (field_8C_state)
    {
        case 1:
            ResetPoint6FDF50();
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyForcedSteering_559DD0();
            EnforceTrailerControlLimits_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            Field40Add();
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            Field40Subtract();
            Field74Subtract();
            break;
        case 2:
            ResetPoint6FDF50();
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            EnforceTrailerControlLimits_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            Field40Add();
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            Field40Subtract();
            Field74Subtract();
            break;
        case 3:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyInputsAndIntegratePhysics_562F30();
            ScarePedsOnDrivingFast_559C30();
            CheckPendingCollision_55F360();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 0:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 4:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            StepPhysics_55F330();
            ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        default:
            //bCol4 = v14;
            break;
    }

    return (UpdateLastMovementTimer_562FA0() && !flag && field_98_surface_type != car_surface_type::unknown_surface_7 && field_98_surface_type != car_surface_type::water_surface_8 &&
            field_98_surface_type != car_surface_type::air_surface_6);
}

// 9.6f 0x49ED00
MATCH_FUNC(0x563280)
void CarPhysics_B0::UpdateCp1FromCm1_563280()
{
    Fix16_Point point = gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;
    NegateInPlace_40F760(point);

    point.RotateByAngle_40F6B0(field_58_theta);

    field_38_cp1 = field_30_cm1 + point;
}

MATCH_FUNC(0x563350)
void CarPhysics_B0::UpdateCenterOfMassPoint_563350()
{
    const CarInfo_2C* info = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());

    Fix16_Point point(info->field_C_center_of_mass_offset.x, info->field_C_center_of_mass_offset.y);

    // RotateByAngle_40F6B0, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine_40F500(field_58_theta);
    Fix16 cos = Ang16::cosine_40F520(field_58_theta);
    Fix16 x_old = point.x;
    point.x = (point.x * cos) + (point.y * sin);
    point.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(point.y * cos);

    field_30_cm1 = field_38_cp1 + point;
}

// 0x49EDC0 9.6f
// https://decomp.me/scratch/xDPiP
MATCH_FUNC(0x563460)
void CarPhysics_B0::UpdateReferencePoint_563460()
{
    Fix16_Point point = gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;

    // RotateByAngle_40F6B0, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine_40F500(field_58_theta);
    Fix16 cos = Ang16::cosine_40F520(field_58_theta);
    Fix16 x_old = point.x;
    point.x = (point.x * cos) + (point.y * sin);
    point.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(point.y * cos);

    field_30_cm1 = field_38_cp1 + point;
}

MATCH_FUNC(0x563560)
void CarPhysics_B0::SetSprite_563560(Sprite* a2)
{
    field_38_cp1.x = a2->field_14_xy.x;
    field_38_cp1.y = a2->field_14_xy.y;
    field_6C_cp3 = a2->field_1C_zpos;
    field_58_theta = a2->field_0;
    field_78_pointing_ang_rad = 0;
    UpdateCenterOfMassPoint_563350();
}

MATCH_FUNC(0x563590)
void CarPhysics_B0::SnapVelocityToSpriteDirection_563590(Sprite* pSprt)
{
    field_40_linvel_1.SetFromPolar_41E210(field_40_linvel_1.GetLength_41E260(), pSprt->field_0);
    CarPhysics_B0::SetSprite_563560(pSprt);
}

MATCH_FUNC(0x563670)
void CarPhysics_B0::UpdateSpriteFromPhysics_563670()
{
    Sprite* car_sprite = field_5C_pCar->field_50_car_sprite;
    car_sprite->set_xyz_lazy_420600(field_38_cp1.x, field_38_cp1.y, field_6C_cp3);
    car_sprite->set_ang_lazy_420690(field_58_theta);
}

MATCH_FUNC(0x5636c0)
void CarPhysics_B0::UpdateCarAndTrailerSpriteFromPhysics_5636C0()
{
    UpdateSpriteFromPhysics_563670();

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->field_C_pCarOnTrailer->field_58_physics->UpdateSpriteFromPhysics_563670();
    }
}

MATCH_FUNC(0x5636e0)
bool CarPhysics_B0::IsNearlyStopped_5636E0()
{
    if (field_40_linvel_1.GetLength_41E260() < dword_6FE0A8)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x5637a0)
void CarPhysics_B0::Init_5637A0()
{
    field_40_linvel_1.reset();
    field_74_ang_vel_rad = Fix16(0);
    field_70_z_vel = kFP16Zero_6FE20C;
    ResetForceAccumulators_55A840();
    field_91_is_foot_brake_on = 0;
    field_92_is_hand_brake_on = 0;
    field_93_is_forward_gas_on = 0;
    field_94_is_backward_gas_on = 0;
    field_95 = 0;
    field_AD_turn_direction = car_turn_direction::none_0;
    field_A9_car_model = -1;
    field_A8_hand_brake_force = 0;
    field_90_timer_since_last_move = 0;
    set_field_A0_559B90(0);
    field_A4_oil_spin_timer = 0;
    field_98_surface_type = car_surface_type::flat_surface_0;
    field_9C_block_spec = 0;
    field_A5_current_slope_length = 0;
    field_A6_current_slope_left_tiles = 0;
    field_10_last_skid_pos[0].reset();
    field_10_last_skid_pos[1].reset();
    field_10_last_skid_pos[2].reset();
    field_10_last_skid_pos[3].reset();
    field_8C_state = 1;
    field_8_total_damage_q = 0;
    field_60_gas_pedal = k_dword_6FE290;
    field_AC_drive_wheels_locked_q = 0;
    field_64_brake_pressure = kBrakePressureStart_6FE200;
    field_68_z_pos = kFP16Zero_6FE20C;
    field_84_front_skid = kFP16Zero_6FE20C;
    field_88_rear_skid = kFP16Zero_6FE20C;
    field_AA_sbw = 0;
    field_AB_tpa = 0;
}

MATCH_FUNC(0x563890)
void CarPhysics_B0::PoolAllocate()
{
    field_30_cm1.reset();
    field_58_theta = kAngZero_6FE3C0;
    field_38_cp1.reset();
    field_5C_pCar = NULL;
    Init_5637A0();
    field_0_vel_read_only.reset();
}

MATCH_FUNC(0x5638c0)
void CarPhysics_B0::SetCar_5638C0(Car_BC* pCar)
{
    this->field_5C_pCar = pCar;
    Ped* pDriver = pCar->field_54_driver;
    if (pDriver && pDriver->field_15C_player)
    {
        this->field_8C_state = 2;
    }
    else
    {
        this->field_8C_state = 1;
    }
}

MATCH_FUNC(0x563900)
CarPhysics_B0::CarPhysics_B0()
{
    mpNext = NULL;
    PoolAllocate();
}