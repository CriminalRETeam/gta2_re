// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kFP16Zero_6FE20C

#include "ped_death_cause.hpp"
#include "explosion_type.hpp"
#include "car_impact_mode.hpp"
#include "CarPhysics.hpp"
#include "CarAI.hpp"
#include "CarInfoTable.hpp"
#include "Globals.hpp"
#include "Frontend.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "ParticleManager.hpp"
#include "Player.hpp"
#include "SpriteGrid.hpp"
#include "CollisionSoundQueue.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "block_face.hpp"
#include "car_surface_type_more.hpp"

// Forward declarations: the functions below are in address order
EXPORT s32 __stdcall get_skid_obj_type(s32 surface, Fix16 skid_len);
static inline Fix16 __stdcall DotProductOOL_49E500(Fix16_Point& Vector1, Fix16_Point& Vector2);
static inline Fix16 __stdcall SquareFix16(const Fix16& value);
EXPORT Fix16 __stdcall DotProduct(const Fix16_Point& Vector1, const Fix16_Point& Vector2);

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool, 0x6FE3E0);
DEFINE_GLOBAL(CarInfo_2C*, gCarInfo_2C, 0x6FE0E4);
DEFINE_GLOBAL(ModelPhysics*, gCarInfo_48, 0x6FE258);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6FE3C0, Ang16(0), 0x6FE3C0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FE20C, Fix16(0), 0x6FE20C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpZero, kFP16Zero_6FE20C, 0x6FE290);
DEFINE_GLOBAL_INIT(Fix16, kFP16Five_6FE220, Fix16(5), 0x6FE220);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6FDEFC, kFP16Five_6FE220, 0x6FDEFC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFive, kFpFive_6FDEFC, 0x6FDF88);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kBrakePressureStart, kFP16Zero_6FE20C, 0x6FE200);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FE214, Fix16(2), 0x6FE214);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDFEC, Fix16(0x2000, 0), 0x6FDFEC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTwoAndHalf, kFP16Two_6FE214 + kFpHalf_6FDFEC, 0x6FE1B0);
DEFINE_GLOBAL(Fix16, gDamageSpeedFactor, 0x6FE348);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16Two, kFP16Two_6FE214, 0x6FDFB0);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sBrakeForce, 0x6FE0D8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FDFD4, Fix16(0x1000, 0), 0x6FDFD4);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FE364, kFP16Quarter_6FDFD4, 0x6FE364);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FE3A0, kFP16Quarter_6FDFD4, 0x6FE3A0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kBrakePressureStep, kFP16Quarter_6FDFD4, 0x6FE2AC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FE210, Fix16(1), 0x6FE210);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kBrakePressureMax, kFpOne_6FE210, 0x6FE1C0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FDFE4, Fix16(0x1333, 0), 0x6FDFE4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint3, kFpPoint3_6FDFE4, 0x6FE0A8);

DEFINE_GLOBAL_INIT(Fix16, kFP16One64th_6FE1AC, Fix16(0x100, 0), 0x6FE1AC);
DEFINE_GLOBAL_INIT(Fix16, kFP16One64th_6FE2E0, kFP16One64th_6FE1AC, 0x6FE2E0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FE234, Fix16(8), 0x6FE234);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxZ, kFP16Eight_6FE234 - kFP16One64th_6FE2E0, 0x6FDF34);

DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sCm1, 0x6FDF10);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sCp3, 0x6FDF08);
DEFINE_STATIC_GLOBAL_INIT(Ang16, sTheta, Ang16(0), 0x6FE344);
DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sCp1, 0x6FDF00);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sF70, 0x6FDFE0);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sAirZVel, 0x6FE0AC);

DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sTrailerCm1, 0x6FE068);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sTrailerCp3, 0x6FE1B4);
DEFINE_STATIC_GLOBAL_INIT(Ang16, sTrailerTheta, Ang16(0), 0x6FE018);
DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sTrailerCp1, 0x6FE3A8);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sTrailerF70, 0x6FE194);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sTrailerAirZVel, 0x6FE354);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint04_6FE1D8, Fix16(0x28F, 0), 0x6FE1D8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint04, kFpPoint04_6FE1D8, 0x6FDF3C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FDF7C, Fix16(1), 0x6FDF7C);
DEFINE_GLOBAL_INIT(Fix16, kFP16SixtyFour, Fix16(64), 0x6FE2EC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Four_6FE21C, Fix16(4), 0x6FE21C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One256th, kFpOne_6FE210 / (kFP16SixtyFour * kFP16Four_6FE21C), 0x6FE07C);

DEFINE_GLOBAL_INIT(Fix16, FastCarMinVelocity, Fix16(0x1EB, 0), 0x6FE1CC);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sRemainingTimeStep, 0x6FE198);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16Eighth, Fix16(0x800, 0), 0x6FE370);

DEFINE_GLOBAL_INIT(Fix16, kFP16Three_6FE218, Fix16(3), 0x6FE218);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThree, kFP16Three_6FE218, 0x6FE1B8);

DEFINE_GLOBAL(Fix16_Point, gConveyorVelocity, 0x6FDF50);
DEFINE_GLOBAL(Fix16, gArrowSteerAngVel, 0x6FE0B0);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_6FDFF0, Fix16(0x2666, 0), 0x6FDFF0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FDFD8, Fix16(0xCCC, 0), 0x6FDFD8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FE1D4, Fix16(0x333, 0), 0x6FE1D4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FDFF8, Fix16(0x3333, 0), 0x6FDFF8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint6, kFpPoint6_6FDFF0, 0x6FE228);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FE374, kFpPoint2_6FDFD8 + kFpPoint05_6FE1D4, 0x6FE374);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, kFpPoint8_6FDFF8, 0x6FE104);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6FE00C, Ang16(360), 0x6FE00C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FE154, Ang16(1080), 0x6FE154);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FE12A, Ang16(720), 0x6FE12A);
DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sCollisionNormal, 0x6FE1F0);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_6FE3C4, Fix16(0x11C, 0), 0x6FE3C4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16OneDegree, kAngFix16OneDegree_6FE3C4, 0x6FDFA4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne64th, kFP16One64th_6FE2E0, 0x6FDF80);

DEFINE_STATIC_GLOBAL_INIT(Fix16, sCollisionDamage, Fix16(0), 0x6FE33C);
DEFINE_STATIC_MEMBER(u8, CarPhysics, sCollisionArea, 0x6FDFC4);
DEFINE_STATIC_MEMBER(u8, CarPhysics, sOtherCollisionArea, 0x6FDFCC);
DEFINE_GLOBAL(Fix16_Point, CollisionIntersectionPoint, 0x6FE1A0);

DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sSavedCm1, 0x6FE3C8);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedCp3, 0x6FDF84);
DEFINE_STATIC_GLOBAL(Ang16, sSavedTheta, 0x6FE158);
DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sSavedCp1, 0x6FE090);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedF70, 0x6FE268);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedAirZVel, 0x6FE32C);

DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sSavedTrailerCm1, 0x6FE160);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedTrailedCp3, 0x6FDF8C);
DEFINE_STATIC_GLOBAL(Ang16, sSavedTrailerTheta, 0x6FE310);
DEFINE_STATIC_MEMBER(Fix16_Point, CarPhysics, sSavedTrailerCp1, 0x6FDF40);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedTrailerF70, 0x6FE0E0);
DEFINE_STATIC_MEMBER(Fix16, CarPhysics, sSavedTrailerAirZVel, 0x6FE394);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16FullCircle, Fix16(0x18F60, 0), 0x6FE314);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE3D0, kFpOne_6FE210, 0x6FE3D0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint139_6FE148, Fix16(0x8E5, 0), 0x6FE148);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint139, kFpPoint139_6FE148, 0x6FE3D4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FE320, kFpPoint8_6FDFF8, 0x6FE320);
DEFINE_GLOBAL_INIT(Fix16, kFP16One128th_6FE0A0, Fix16(0x80, 0), 0x6FE0A0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16One128th, kFP16One128th_6FE0A0, 0x6FDFDC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint95, kFpOne_6FE210 - kFpPoint05_6FE1D4, 0x6FE334);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint005_6FE2F0, Fix16(0x51, 0), 0x6FE2F0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FE1C4, Fix16(0xA3, 0), 0x6FE1C4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint985_6FE330, kFpOne_6FE210 - kFpPoint005_6FE2F0 - kFpPoint01_6FE1C4, 0x6FE330);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint985_6FE240, kFpOne_6FE210 - kFpPoint005_6FE2F0 - kFpPoint01_6FE1C4, 0x6FE240);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint06_6FE1E4, Fix16(0x3D7, 0), 0x6FE1E4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint94, kFpOne_6FE210 - kFpPoint06_6FE1E4, 0x6FDF18);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FDFD0, Fix16(0x666, 0), 0x6FDFD0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint9, kFpOne_6FE210 - kFpPoint1_6FDFD0, 0x6FE100);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint97_6FE0FC, kFpOne_6FE210 - FastCarMinVelocity, 0x6FE0FC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint97_6FDFBC, kFpOne_6FE210 - FastCarMinVelocity, 0x6FDFBC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint12, Fix16(0x7AE, 0), 0x6FE1E0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint88, kFpOne_6FE210 - kFpPoint12, 0x6FE318);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint05, kFpPoint05_6FE1D4, 0x6FE3B4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FE15C, kFpPoint01_6FE1C4, 0x6FE15C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint1, kFpPoint1_6FDFD0, 0x6FE2F4);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE0C0, Fix16(0x2000, 0), 0x6FE0C0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kSlopeGradient2, Fix16(0x1FE8, 0), 0x6FE064);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kSlopeGradient8, Fix16(0x824, 0), 0x6FE350);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16QuarterCircle, Fix16(0x63D8, 0), 0x6FE10C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16TwentyFiveDegrees, kAngFix16OneDegree_6FE3C4 * 25, 0x6FE134);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16ThreeQuarterCircle, Fix16(0x12B88, 0), 0x6FE278);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16HalfCircle, Fix16(0xC7B0, 0), 0x6FE260);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE2F8, kFP16Half_6FE0C0, 0x6FE2F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE070, kFpOne_6FE210, 0x6FE070);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE3DC, kFpOne_6FE210, 0x6FE3DC);

DEFINE_GLOBAL_INIT(Ang16, kAng1_6FE3B8, Ang16(4), 0x6FE3B8); // Only exists so that MultiplyByFix16 can be called

DEFINE_GLOBAL_INIT(Ang16, kAng45_6FE058, kAng1_6FE3B8.MultiplyByFix16(Fix16(45)), 0x6FE058);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FE37C, kFpPoint01_6FE1C4, 0x6FE37C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpSevenSixteenths, Fix16(0x1C00, 0), 0x6FE004);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint2, kFpOne_6FE210 - kFpPoint8_6FE320, 0x6FE2B0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreePoint8, kFP16Three_6FE218 + kFpPoint8_6FDFF8, 0x6FE340);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16MinusOne, Fix16(262143), 0x6FDF1C);
DEFINE_STATIC_GLOBAL(Fix16_Point, kZeroPoint, 0x6FE300);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint9_6FDFFC, Fix16(0x3999, 0), 0x6FDFFC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAiWallTurnForce, kFP16One64th_6FE2E0*(kFpPoint1_6FDFD0 + kFpOne_6FE210), 0x6FE358);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kPlayerWallTurnTorque, (kAngFix16OneDegree_6FE3C4 * kFpPoint9_6FDFFC), 0x6FE078);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kPlayerWallTurnForce, (kFP16One64th_6FE2E0 * kFpPoint9_6FDFFC), 0x6FE390);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAiWallTurnTorque, kAngFix16OneDegree_6FE3C4*(kFpPoint1_6FDFD0 + kFpOne_6FE210), 0x6FE080);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16One16th, Fix16(0x400, 0), 0x6FE270);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeEighths, Fix16(0x1800, 0), 0x6FE178);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FE0D0, kFP16Half_6FE0C0, 0x6FE0D0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint7, Fix16(0x2CCC, 0), 0x6FDFF4);
DEFINE_GLOBAL_INIT(Fix16, kFP16One32nd, Fix16(0x200, 0), 0x6FE118);

DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FE1A8, kFP16Quarter_6FDFD4, 0x6FE1A8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE098, kFpOne_6FE210, 0x6FE098);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE0F4, kFpOne_6FE210, 0x6FE0F4);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FE0D4, kFpOne_6FE210, 0x6FE0D4);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FDFB8, kFP16Quarter_6FDFD4, 0x6FDFB8);

// 9.6f 0x49EF50
inline void CarPhysics::AddDamage(s32 damage)
{
    u32 new_damage = gpRng->get_cur_rng() + damage;
    if (new_damage > mDamagedUntilCycle)
    {
        mDamagedUntilCycle = new_damage;
    }
}

MATCH_FUNC(0x40B560)
Fix16_Point CarPhysics::get_cp1()
{
    return mCp1;
}

MATCH_FUNC(0x446ee0)
CarPhysics::~CarPhysics()
{
}

MATCH_FUNC(0x447010)
Fix16_Point CarPhysics::get_linvel()
{
    return mLinvel1;
}

DEFINE_STATIC_GLOBAL_ARRAY(wchar_t, sThetaText, 16, 0x66A8EC);

// https://decomp.me/scratch/xqLh0
// 9.6f 0x49E240
inline wchar_t* Ang16::ThetaText()
{
    swprintf(sThetaText, L"%3.2f", (rValue * 0.25));
    return sThetaText;
}

// 9.6f 0x410BA0
inline f64 Fix16::to_float() const
{
    return ((mValue / 16384.0));
}

MATCH_FUNC(0x453F50)
void CarPhysics::ForceNeutralInput_453F50()
{
  this->mIsCoasting = true;
  this->mIsFootBrakeOn = 0;
  this->mIsBackwardGasOn = 0;
  this->mIsForwardGasOn = 0;
}

MATCH_FUNC(0x453F70)
void CarPhysics::ForceForwardAcceleration()
{
  this->mIsForwardGasOn = 1;
  this->mIsFootBrakeOn = 0;
  this->mIsBackwardGasOn = 0;
  this->mIsCoasting = false;
}

MATCH_FUNC(0x453F90)
void CarPhysics::ClearDriverInputs()
{
  this->mIsFootBrakeOn = 0;
  this->mIsForwardGasOn = 0;
  this->mIsBackwardGasOn = 0;
  this->mIsCoasting = false;
}

MATCH_FUNC(0x559430)
void CarPhysics::ShowPhysicsDebug()
{
    if (bDo_show_physics)
    {
        Hud_TextEntry* pText;
        SetCurrentCarInfoAndModelPhysics();

        swprintf(tmpBuff,
                 L"CM = (%3.3f,%3.3f) CP = (%3.3f,%3.3f,%3.3f)",
                 mCm1.x.to_float(),
                 mCm1.y.to_float(),
                 mCp1.x.to_float(),
                 mCp1.y.to_float(),
                 mCp3.to_float());
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 64, gDebugFont, 1);

        swprintf(tmpBuff,
                 L"linvel = (%3.3f,%3.3f) angvelrad = %3.3f",
                 mLinvel1.x.to_float(),
                 mLinvel1.y.to_float(),
                 mAngVelRad.to_float());
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 80, gDebugFont, 1);

        swprintf(tmpBuff, L"theta = %s", mTheta.ThetaText());
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 96, gDebugFont, 1);

        swprintf(tmpBuff, L"pointingangrad = %3.3f", mPointingAngRad.to_float());
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 112, gDebugFont, 1);

        swprintf(tmpBuff, L"mass = %3.3f", CalculateMass().to_float());
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 128, gDebugFont, 1);

        swprintf(tmpBuff, L"front skid = %3.3f", mFrontSkid.to_float());
        pText = gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 144, gDebugFont, 1);
        if (mFrontSkid >= gCarInfo_2C->mSkidThreshhold1 ||
            (mDriveWheelsLockedQ > 0 && gCarInfo_48->mFrontDriveBias > kFP16Zero_6FE20C))
        {
            pText->SetFontRemap(5);
        }

        swprintf(tmpBuff, L"rear skid = %3.3f", mRearSkid.to_float());
        pText = gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 160, gDebugFont, 1);
        if (mRearSkid >= gCarInfo_2C->mSkidThreshhold2 ||
            (mDriveWheelsLockedQ > 0 && gCarInfo_2C->mRearDriveBias > kFP16Zero_6FE20C))
        {
            pText->SetFontRemap(5);
        }

        swprintf(tmpBuff,
                 L"surface_mode = %d sbw = %d tpa = %d",
                 mSurfaceType,
                 static_cast<u8>(mSbw),
                 static_cast<u8>(mTpa));
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 176, gDebugFont, 1);
    }
}

MATCH_FUNC(0x5597b0)
void CarPhysics::ShowSpeedRevsDamage()
{
    if (bDo_show_instruments)
    {
        SetCurrentCarInfoAndModelPhysics();

        Fix16 speed = mLinvel1.GetLength_41E260();
        s32 gear;
        if (speed > gCarInfo_48->mGear3Speed)
        {
            gear = 3;
        }
        else
        {
            gear = (speed > gCarInfo_48->mGear2Speed) + 1;
        }

        swprintf(tmpBuff, L"speed:%3.3f(%d)", GetLinearSpeed().AsDouble(), gear);
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 16, gDebugFont, 1);

        swprintf(tmpBuff, L"revs:%3.3f %c", mGasPedal.AsDouble(), IsTurboActive() ? 'T' : ' ');
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 32, gDebugFont, 1);

        swprintf(tmpBuff, L"damage:%d%%", mpCar->mDamage * 100 / 32000);
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 48, gDebugFont, 1);
    }
}

MATCH_FUNC(0x5599d0)
bool CarPhysics::IsNotMoving()
{
    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        return pTrailer->mTruckCab->mPhysics->IsStationary() &&
            pTrailer->mTrailerCar->mPhysics->IsStationary();
    }
    else
    {
        return IsStationary();
    }
}

MATCH_FUNC(0x559a40)
void CarPhysics::UpdateTrailerPhysicsFromTowingCar()
{
    if (mpCar->mpTrailer)
    {
        Fix16 inv_step = (kFpOne_6FE210) / sRemainingTimeStep;
        CarPhysics* pPhysics = mpCar->mpTrailer->mTrailerCar->mPhysics;
        pPhysics->mLinvel1 = (pPhysics->mCp1 - sTrailerCp1) * inv_step;
        pPhysics->mAngVelRad = inv_step * Ang16::Ang16_to_Fix16(ComputeShortestAngleDelta(sTrailerTheta, pPhysics->mTheta));
        pPhysics->mZVel = (inv_step * (pPhysics->mCp3 - sTrailerCp3));
    }
}

MATCH_FUNC(0x559b40)
void CarPhysics::UpdateTrailerAlignment()
{
    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        pTrailer->UpdateTrailerAlignment();
    }
}

MATCH_FUNC(0x559b50)
void CarPhysics::EnforceTrailerControlLimits()
{
    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        if (pTrailer->IsHitchAngleClamped())
        {
            if (this->mIsBackwardGasOn)
            {
                this->mIsBackwardGasOn = 0;
                this->mIsFootBrakeOn = 1;
            }
            if (!this->mIsForwardGasOn)
            {
                this->mTurnDirection = car_turn_direction::none_0;
            }
        }
    }
}

MATCH_FUNC(0x559b90)
void CarPhysics::SetOilSpinDir(const s32& spin_dir)
{
    mOilSpinDir = spin_dir;
}

MATCH_FUNC(0x559ba0)
void CarPhysics::SpinOutOnOil()
{
    if (!mpCar->IsTrainModel())
    {
        if (mOilSpinDir != 1 && mOilSpinDir != 2)
        {
            if (gRng.get_int(2))
            {
                SetOilSpinDir(1);
            }
            else
            {
                SetOilSpinDir(2);
            }
        }
        this->mOilSpinTimer = 30;
        AddDamage(30);
    }
}

// https://decomp.me/scratch/yM7OA Fix16 annoying inlined stuff
MATCH_FUNC(0x559c30)
void CarPhysics::ScarePedsOnDrivingFast()
{
    Fix16 cp3 = mCp3;

    if (!gMap_0x370->IsGradientSlopeAt(mCp1.x.ToInt(), mCp1.y.ToInt(), cp3.ToInt()))
    {
        cp3 = mCp3 - kFpOne_6FE210;
    }

    gmp_block_info* pBlock = gMap_0x370->GetBlock(mCp1.x.ToInt(), mCp1.y.ToInt(), cp3.ToInt());
    if (pBlock)
    {
        u8 type = GET_BLOCK_TYPE(pBlock->mSlopeType);
        if (type == PAVEMENT || type == FIELD)
        {
            if (mpCar->mDriver)
            {
                if (!mpCar->IsTrainModel())
                {
                    // 9.6f: CarPhysics::GetLinearSpeed (inlined, using it makes the diff worse)

                    if (mLinvel1.GetLength_out_of_line_x_squared() > FastCarMinVelocity || mpCar->IsEmittingHorn())
                    {
                        mpCar->mDriver->AddThreateningPedToList();
                    }
                }
            }
        }
    }

    if (mOilSpinDir > 0 && mOilSpinDir <= 2)
    {
        mOilSpinTimer--;
        if (mOilSpinTimer == 0)
        {
            CarPhysics::SetOilSpinDir(0);
        }
    }
}

MATCH_FUNC(0x559dd0)
void CarPhysics::ApplyForcedSteering()
{
    if (this->mpCar->mDriver)
    {
        if (mOilSpinDir == 1)
        {
            this->mIsCoasting = false;
            this->mIsForwardGasOn = 1;
            this->mTurnDirection = car_turn_direction::clockwise_m1;
        }
        else if (mOilSpinDir == 2)
        {
            this->mIsCoasting = false;
            this->mIsForwardGasOn = 1;
            this->mTurnDirection = car_turn_direction::anticlockwise_1;
        }
    }
}

MATCH_FUNC(0x559e20)
void CarPhysics::ApplyConveyorPush(Object* pObj)
{
    s8 conveyor_x;
    s8 conveyor_y;
    pObj->GetConveyorDirection(&conveyor_x, &conveyor_y);
    gConveyorVelocity.x += kFP16One64th_6FE2E0 * conveyor_x;
    gConveyorVelocity.y += kFP16One64th_6FE2E0 * conveyor_y;
    AddDamage(15);
}

MATCH_FUNC(0x559E90)
Fix16 CarPhysics::ComputeZPosition()
{
    if (mZVel > kFP16Zero_6FE20C)
    {
        Fix16 cp3 = mCp3;
        cp3 += kFpOne_6FE210;
        if (cp3 > kMaxZ)
        {
            cp3 = kMaxZ;
        }
        return cp3;
    }
    else
    {
        return mCp3;
    }
}

MATCH_FUNC(0x559ec0)
Fix16_Point CarPhysics::ComputeCombinedCenterOfMass()
{

    if (mpCar->mpTrailer)
    {
        Fix16 cab_mass;
        Fix16 trailer_mass;
        cab_mass = mpCar->mpTrailer->mTruckCab->GetMass();
        trailer_mass = mpCar->mpTrailer->mTrailerCar->GetMass();

        Fix16 total_mass = trailer_mass + cab_mass;

        return mpCar->mpTrailer->mTruckCab->mPhysics->mCm1 * (cab_mass / total_mass) +
            mpCar->mpTrailer->mTrailerCar->mPhysics->mCm1 * (trailer_mass / total_mass);
    }
    else
    {
        return mCm1;
    }
}

MATCH_FUNC(0x559ff0)
Fix16 CarPhysics::CalculateMass()
{
    if (mpCar->mpTrailer)
    {
        return mpCar->mpTrailer->mTruckCab->GetMass() +
            mpCar->mpTrailer->mTrailerCar->GetMass();
    }
    else
    {
        return mpCar->GetMass();
    }
}

MATCH_FUNC(0x55a050)
Fix16 CarPhysics::GetEffectiveMomentOfInertia()
{
    if (mpCar->mpTrailer)
    {
        return mpCar->mpTrailer->mTruckCab->GetMomentOfInertia() +
            mpCar->mpTrailer->mTrailerCar->GetMomentOfInertia();
    }
    else
    {
        return mpCar->GetMomentOfInertia();
    }
}

MATCH_FUNC(0x55a0b0)
bool CarPhysics::IsInAir()
{
    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        return pTrailer->mTruckCab->mPhysics->mSurfaceType == car_surface_type::air_surface_6 &&
            pTrailer->mTrailerCar->mPhysics->mSurfaceType == car_surface_type::air_surface_6;
    }
    return mSurfaceType == car_surface_type::air_surface_6;
}

MATCH_FUNC(0x55a100)
Fix16 CarPhysics::GetTrailerAwareTurnRatio()
{
    if (mpCar->IsTrailerCab())
    {
        return kFpTwoAndHalf * gCarInfo_48->mTurnRatio;
    }
    else
    {
        return gCarInfo_48->mTurnRatio;
    }
}

MATCH_FUNC(0x55a150)
char_type CarPhysics::IsFootBrakeOn()
{
    char_type bFootBrakeOn;

    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTruckCab->mPhysics;
        if (pPhysics)
        {
            bFootBrakeOn = pPhysics->mIsFootBrakeOn;
        }
        else
        {
            bFootBrakeOn = 0;
        }
    }
    else
    {
        return this->mIsFootBrakeOn;
    }
    return bFootBrakeOn;
}

MATCH_FUNC(0x55a180)
bool CarPhysics::IsAccelerationOrReverseOn()
{
    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pCarPhysics = pTrailer->mTruckCab->mPhysics;
        if (pCarPhysics)
        {
            return (pCarPhysics->mIsForwardGasOn || pCarPhysics->mIsBackwardGasOn);
        }
    }
    else
    {
        return (mIsForwardGasOn || mIsBackwardGasOn);
    }
    return false;
}

// 9.6f 0x49F760
MATCH_FUNC(0x55a1d0)
void CarPhysics::SetVelocityTowardTarget(Fix16 targetX, Fix16 targetY, Fix16 targetAngle, s32* rotationMode)
{
    Fix16_Point local(targetX, targetY);
    Fix16_Point offset;
    CarInfo_2C* pCarInfo = gCarInfo_808->GetInfoAtIdx(mpCar->mCarInfoIdx);
    offset = pCarInfo->mCenterOfMassOffset;
    Fix16_Point worldPoint;
    offset.RotateByAngle(mTheta);
    worldPoint = local.Add(offset);

    mLinvel1 = worldPoint - mCm1;

    mAngVelRad = targetAngle - Ang16::Ang16_to_Fix16(mTheta);

    if (*rotationMode == 1)
    {
        if (mAngVelRad.mValue < kFP16Zero_6FE20C.mValue)
        {
            mAngVelRad = mAngVelRad + kAngFix16FullCircle;
        }
    }
    else if (*rotationMode == 2)
    {
        if (mAngVelRad.mValue > kFP16Zero_6FE20C.mValue)
        {
            mAngVelRad = mAngVelRad - kAngFix16FullCircle;
        }
    }

    if (mState != 4)
    {
        mState = 0;
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
void CarPhysics::restore_saved_physics_state()
{
    this->mCm1 = sCm1;
    this->mCp3 = sCp3;
    this->mTheta = sTheta;
    this->mCp1 = sCp1;
    this->mZVel = sF70;
    this->mAirZVel = sAirZVel;
    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
        pPhysics->mCm1 = sTrailerCm1;
        pPhysics->mCp3 = sTrailerCp3;
        pPhysics->mTheta = sTrailerTheta;
        pPhysics->mCp1 = sTrailerCp1;
        pPhysics->mZVel = sTrailerF70;
        pPhysics->mAirZVel = sTrailerAirZVel;
    }
}

MATCH_FUNC(0x55a4b0)
void CarPhysics::save_physics_state()
{
    sCm1 = this->mCm1;
    sCp3 = this->mCp3;
    sTheta = this->mTheta;
    sCp1 = this->mCp1;
    sF70 = this->mZVel;
    sAirZVel = this->mAirZVel;
    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
        sTrailerCm1 = pPhysics->mCm1;
        sTrailerCp3 = pPhysics->mCp3;
        sTrailerTheta = pPhysics->mTheta;
        sTrailerCp1 = pPhysics->mCp1;
        sTrailerF70 = pPhysics->mZVel;
        sTrailerAirZVel = pPhysics->mAirZVel;
    }
}

MATCH_FUNC(0x55a550)
void CarPhysics::restore_state()
{
    this->mCm1 = sSavedCm1;
    this->mCp3 = sSavedCp3;
    this->mTheta = sSavedTheta;
    this->mCp1 = sSavedCp1;
    this->mZVel = sSavedF70;
    this->mAirZVel = sSavedAirZVel;

    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
        pPhysics->mCm1 = sSavedTrailerCm1;
        pPhysics->mCp3 = sSavedTrailedCp3;
        pPhysics->mTheta = sSavedTrailerTheta;
        pPhysics->mCp1 = sSavedTrailerCp1;
        pPhysics->mZVel = sSavedTrailerF70;
        pPhysics->mAirZVel = sSavedTrailerAirZVel;
    }
}

MATCH_FUNC(0x55a600)
void CarPhysics::save_state()
{
    sSavedCm1 = this->mCm1;
    sSavedCp3 = this->mCp3;
    sSavedTheta = this->mTheta;
    sSavedCp1 = this->mCp1;
    sSavedF70 = this->mZVel;
    sSavedAirZVel = this->mAirZVel;
    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
        sSavedTrailerCm1 = pPhysics->mCm1;
        sSavedTrailedCp3 = pPhysics->mCp3;
        sSavedTrailerTheta = pPhysics->mTheta;
        sSavedTrailerCp1 = pPhysics->mCp1;
        sSavedTrailerF70 = pPhysics->mZVel;
        sSavedTrailerAirZVel = pPhysics->mAirZVel;
    }
}

MATCH_FUNC(0x55a6a0)
Fix16 CarPhysics::ComputeRequiredSweepSteps()
{
    Fix16 steps = MaxByValue_55A6A0((Fix16::Max3_55EEE0(Fix16::Abs(sSavedCm1.x - sCm1.x),
                                                              Fix16::Abs(sSavedCm1.y - sCm1.y),
                                                              Fix16::Abs(sSavedCp3 - sCp3))) /
                              mpCar->GetMinDimension(),
                          Ang16::NormalizeAngleDeltaScaled(sTheta, sSavedTheta, kAng45_6FE058));

    if (mpCar->mpTrailer)
    {

        steps = Fix16::Max3_55EEE0(
            steps,
            (Fix16::Max3_55EEE0(Fix16::Abs(sSavedTrailerCm1.x - sTrailerCm1.x),
                                                Fix16::Abs(sSavedTrailerCm1.y - sTrailerCm1.y),
                                                Fix16::Abs(sSavedTrailedCp3 - sTrailerCp3)) /
             mpCar->mpTrailer->mTrailerCar->GetMinDimension()),
            Ang16::NormalizeAngleDeltaScaled(sTrailerTheta, sSavedTrailerTheta, kAng45_6FE058));
    }

    return steps;
}

// 9.6f 0x42A630 called as a static with the value by reference (its 9.6f copy takes a pointer and a hidden
// return). Declared in fix16.hpp, defined here so no other TU changes.
inline Fix16 __stdcall Fix16::GetFracValue_42A630(const Fix16& v)
{
    return Fix16(v.mValue & 0x3FFF, 0);
}

MATCH_FUNC(0x55a840)
void CarPhysics::ResetForceAccumulators()
{
    mForceAccum.x = 0;
    mForceAccum.y = 0;
    mLinearAccel.x = 0;
    mLinearAccel.y = 0;
    mTorqueAccum = 0;
    mAngularAccel = 0;
}

// https://decomp.me/scratch/efo3b
MATCH_FUNC(0x55a860)
void CarPhysics::HandleUserInputs(char_type bForwardGasOn,
                                            char_type bFootBrakeOn,
                                            char_type bLeftOn,
                                            char_type bRightOn,
                                            char_type bHandBrakeOn)
{

    if (this->mLinvel1.IsNull())
    {
        this->mIsForwardGasOn = bForwardGasOn;
        this->mIsBackwardGasOn = bFootBrakeOn;
        this->mIsFootBrakeOn = 0;
    }
    else
    {
        // IsVelocityAlignedWithHeading inlined
        Ang16 heading_delta = (mLinvel1.GetAngle() - mTheta);
        bool aligned = heading_delta <= kAng90_6FE00C || heading_delta >= kAng270_6FE154;
        if (aligned)
        {
            if (this->mIsBackwardGasOn)
            {
                this->mIsForwardGasOn = 0;
                this->mIsBackwardGasOn = 0;
                this->mIsFootBrakeOn = 0;
            }
            else
            {
                this->mIsForwardGasOn = bForwardGasOn;
                this->mIsFootBrakeOn = bFootBrakeOn;
                this->mIsBackwardGasOn = 0;
            }
        }
        else if (this->mIsForwardGasOn)
        {
            this->mIsBackwardGasOn = 0;
            this->mIsForwardGasOn = 0;
            this->mIsFootBrakeOn = 0;
        }
        else
        {
            this->mIsFootBrakeOn = bForwardGasOn;
            this->mIsBackwardGasOn = bFootBrakeOn;
            this->mIsForwardGasOn = 0;
        }
    }
    this->mIsCoasting = false;
    this->mIsHandBrakeOn = bHandBrakeOn;
    if (bRightOn)
    {
        if (!bLeftOn)
        {
            this->mTurnDirection = car_turn_direction::clockwise_m1;
            return;
        }
    }
    else if (bLeftOn)
    {
        this->mTurnDirection = car_turn_direction::anticlockwise_1;
        return;
    }
    this->mTurnDirection = car_turn_direction::none_0;
}

MATCH_FUNC(0x55aa00)
void CarPhysics::HandleGravityOnSlope()
{

    Fix16_Point force;

    // On a slope and no brake inputs
    if (mCurrentSlopeLength != 1 || mIsHandBrakeOn || mIsFootBrakeOn)
    {
        return;
    }

    // Trains ignore slope gravity
    if (mpCar->IsTrainModel())
    {
        return;
    }

    switch (mSurfaceType)
    {
        case car_surface_type::slope_northwards_1:
            force.x = kFP16Zero_6FE20C;
            force.y = (kFpPoint04 * kFP16One_6FDF7C);
            break;

        case car_surface_type::slope_southwards_2:
            force.x = kFP16Zero_6FE20C;
            force.y = (kFP16One_6FDF7C * -kFpPoint04);
            break;

        case car_surface_type::slope_westwards_3:
            force.x = (kFpPoint04 * kFP16One_6FDF7C);
            force.y = kFP16Zero_6FE20C;
            break;

        case car_surface_type::slope_eastwards_4:
            force.x = (kFP16One_6FDF7C * -kFpPoint04);
            force.y = kFP16Zero_6FE20C;
            break;

        default:
            return;
    }

    ApplyForceScaledByMass(force);
}

MATCH_FUNC(0x55ab50)
Fix16 CarPhysics::ComputeSlopeCorrection(Fix16* pOutY)
{

    Fix16_Point point_to_sub;
    Fix16_Point sub_point;
    if (mpCar->IsTrailerCar())
    {
        point_to_sub = sTrailerCp1;
    }
    else
    {
        point_to_sub = sCp1;
    }
    sub_point = mCp1 - point_to_sub;
    Fix16 x_val = sub_point.x;
    Fix16 y_val = sub_point.y;

    Fix16 slope_val;
    Fix16 lower;
    Fix16 upper;
    Fix16 result;

    switch (mSurfaceType)
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
            return kFP16Zero_6FE20C;
    }

    switch (this->mCurrentSlopeLength)
    {
        case 1:
            slope_val = kFP16One_6FDF7C;
            break;
        case 2:
            slope_val = kSlopeGradient2;
            break;
        case 8:
            slope_val = kSlopeGradient8;
            break;
        default:
            slope_val = kFP16Zero_6FE20C;
            break;
    }
    lower = (x_val * slope_val);
    if (!this->mCurrentSlopeLeftTiles && lower > kFP16Zero_6FE20C &&
            static_cast<u8>(this->mCp3.ToInt()) == this->mCurrentTileZ ||
        this->mSbw && this->mTpa)
    {
        upper = kFpOne_6FE210 - (this->mCp3.GetFracValue());
        if (lower < upper)
        {
            *pOutY = upper;
        }
        else
        {
            *pOutY = lower;
        }
        result = lower;
    }
    else
    {
        if (lower > kFP16Zero_6FE20C)
        {
            if (mpCar->mpTrailer)
            {
                if (mpCar->mpTrailer->GetCabOrTrailerCar(mpCar)
                        ->mPhysics->mSurfaceType != car_surface_type::air_surface_6)
                {
                    lower = kFP16Zero_6FE20C;
                }
            }
        }
        *pOutY = lower;
        result = lower;
    }
    return result;
}

// 9.6f 0x4A2240: mCp3 read directly everywhere, g_ZPos * a2,
// the slope test nested as (slope && frac != 0 && zpos <= cp3 + k), ComputeSlopeCorrection's two outputs.
MATCH_FUNC(0x55ad90)
void CarPhysics::UpdateZPhysics(Fix16 dt)
{
    // dt is copied into a register at entry; its stack slot is then reused as ComputeSlopeCorrection's second
    // output (9.6f 0x4A2240 passes a local there and reads the parameter afterwards)
    Fix16 dt_copy = dt;
    Fix16 zpos;

    if (mSurfaceType == car_surface_type::air_surface_6)
    {
        if (mCp3 > kMaxZ)
        {
            mCp3 = kMaxZ;
        }
        Fix16 map_z;
        map_z = gMap_0x370->FindGroundZBelowCoord(mCp1.x, mCp1.y, mCp3);
        zpos = mCp3 + (sAirZVel * dt_copy);
        if (zpos <= map_z)
        {
            zpos = map_z;
        }
        else if (zpos > kMaxZ)
        {
            zpos = kMaxZ;
        }
    }
    else if (mSurfaceType == car_surface_type::remapped_water_tile_7 ||
             mSurfaceType == car_surface_type::water_surface_8)
    {
        zpos = mCp3;
    }
    else
    {
        zpos = gMap_0x370->FindGroundZForCoord(mCp1.x, mCp1.y);
        if (zpos == kFP16Zero_6FE20C)
        {
            zpos = kFpOne_6FE210;
        }

        if (zpos >= mCp3 + kFP16Half_6FE0C0)
        {
            if (!((mSurfaceType == car_surface_type::slope_northwards_1 ||
                   mSurfaceType == car_surface_type::slope_southwards_2 ||
                   mSurfaceType == car_surface_type::slope_westwards_3 ||
                   mSurfaceType == car_surface_type::slope_eastwards_4) &&
                  Fix16::GetFracValue_42A630(zpos) != kFP16Zero_6FE20C && zpos <= mCp3 + kFpOne_6FE210))
            {
                zpos = gMap_0x370->FindGroundZBelowCoord(mCp1.x, mCp1.y, zpos - kFP16One64th_6FE2E0);
                if (zpos > mCp3)
                {
                    Fix16 below;
                    below = gMap_0x370->GetGroundZBelowCoord(mCp1.x,
                                                             mCp1.y,
                                                             zpos - kFP16One64th_6FE2E0);
                    if (below > kFP16Zero_6FE20C)
                    {
                        zpos = below;
                    }
                }

                if (zpos >= mCp3 + kFpOne_6FE210)
                {
                    zpos = mCp3;
                }
            }
        }

        if (zpos <= mCp3 - kFP16Half_6FE0C0 || zpos < mCp3 && mSbw && mTpa)
        {
            mAirZVel = ComputeSlopeCorrection(&dt);
            zpos = mCp3 + dt;
            if (dt_copy != kFP16Zero_6FE20C)
            {
                mAirZVel /= dt_copy;
            }

            if (zpos > kMaxZ)
            {
                zpos = kMaxZ;
            }
        }

        if (zpos < mCp3)
        {
            UpdateSpriteFromPhysics();

            mpCar->mCarSprite->set_xyz_lazy(mpCar->mCarSprite->mXy.x,
                                            mpCar->mCarSprite->mXy.y,
                                            zpos);

            if (mpCar->mCarSprite->CheckSpriteMovementRegion())
            {
                zpos = mCp3;
            }
        }
    }

    if (mpCar->IsCrushing())
    {
        UpdateSpriteFromPhysics();

        mpCar->mCarSprite->set_xyz_lazy(mpCar->mCarSprite->mXy.x,
                                        mpCar->mCarSprite->mXy.y,
                                        zpos);

        gCarManager_6C->mImpactMode = car_impact_mode::crush_max_z_2;
        gCarManager_6C->mZpos = kFP16Zero_6FE20C;

        gSpriteGrid_1->CheckAndHandleAllCollisionsForSprite(mpCar->mCarSprite, sprite_types_enum::car_2);

        gCarManager_6C->mZpos += kFpPoint2_6FDFD8;
        if (gCarManager_6C->mZpos > zpos)
        {
            zpos = gCarManager_6C->mZpos;
        }
    }

    mZVel = zpos - mCp3;
    mCp3 += mZVel;
    if (dt_copy != kFP16Zero_6FE20C)
    {
        mZVel /= dt_copy;
    }
}

// 9.6f 0x49EBE0 (a Map_0x370 method: 9.6f calls it with gMap in ecx)
inline u8 Map_0x370::GetBlockSurfaceType(s32 x, s32 y, s32 z, u8* pGradientSize, u8* pGradientLevel)
{
    gmp_block_info* pBlock = GetBlock(x, y, z);
    if (pBlock)
    {
        if (gGtx_0x106C->IsRemappedWaterTile(pBlock->mLid & block_face::tile_idx_mask))
        {
            return car_surface_type::remapped_water_tile_7;
        }

        if (!IS_AIR_TYPE(pBlock->mSlopeType))
        {
            if (gGtx_0x106C->IsUnremappedWaterTile(pBlock->mLid & block_face::tile_idx_mask))
            {
                return car_surface_type::water_tile_9;
            }
            gmp_map_slope* pSlope = &gGmpSlopes[pBlock->mSlopeType >> 2];
            *pGradientSize = pSlope->mGradientSize;
            *pGradientLevel = pSlope->mGradientLevel;
            return pSlope->mGradientDirection;
        }
    }
    return car_surface_type::no_block_5;
}

MATCH_FUNC(0x55b3f0)
void CarPhysics::SyncZWithTrailer(Fix16 dt)
{
    UpdateZPhysics(dt);

    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pCarOnTrailerPhysics = pTrailer->mTrailerCar->mPhysics;
        pCarOnTrailerPhysics->SetCurrentCarInfoAndModelPhysics();
        pCarOnTrailerPhysics->UpdateZPhysics(dt);
        SetCurrentCarInfoAndModelPhysics();
        Fix16 ourCp3 = this->mCp3;
        Fix16 carOnTrailer_cp3 = pCarOnTrailerPhysics->mCp3;
        if (ourCp3 > carOnTrailer_cp3)
        {
            pCarOnTrailerPhysics->mCp3 = ourCp3;
            pCarOnTrailerPhysics->mZVel = this->mZVel;
        }
        else if (ourCp3 < carOnTrailer_cp3)
        {
            this->mCp3 = carOnTrailer_cp3;
            this->mZVel = pCarOnTrailerPhysics->mZVel;
        }
    }
}

MATCH_FUNC(0x55b4f0)
void CarPhysics::UpdateRailwayZPosition(Fix16 dt)
{
    Fix16 slope_y;
    Fix16 new_z;
    new_z = gMap_0x370->GetRailwayZCoordAtXY(this->mCp1.x, this->mCp1.y);
    if (new_z == kFP16Zero_6FE20C)
    {
        new_z = kFpOne_6FE210;
    }

    if (new_z >= mCp3 + kFP16Half_6FE0C0)
    {
        new_z = gMap_0x370->GetRailwayZBelowCoord(mCp1.x, mCp1.y, new_z - kFpOne_6FE210);
        if (new_z >= mCp3 + kFpOne_6FE210)
        {
            new_z = this->mCp3;
        }
    }

    if (new_z <= mCp3 - kFP16Half_6FE0C0)
    {
        this->mAirZVel = ComputeSlopeCorrection(&slope_y);
        new_z = mCp3 + mAirZVel;
        if (dt != kFP16Zero_6FE20C)
        {
            mAirZVel /= dt;
        }
    }
    else
    {
        if (new_z < mCp3)
        {
            UpdateSpriteFromPhysics();

            mpCar->mCarSprite->set_xyz_lazy(mpCar->mCarSprite->mXy.x,
                                            mpCar->mCarSprite->mXy.y,
                                            new_z);

            if (mpCar->mCarSprite->CheckSpriteMovementRegion())
            {
                new_z = this->mCp3;
            }
        }
    }

    this->mZVel = new_z - this->mCp3;
    this->mCp3 += mZVel;
    if (dt != kFP16Zero_6FE20C)
    {
        mZVel = mZVel / dt;
    }
}

MATCH_FUNC(0x55B7B0)
void CarPhysics::UpdateRailwayZPositionThunk(Fix16 dt)
{
    UpdateRailwayZPosition(dt);
}

MATCH_FUNC(0x55B7E0)
void CarPhysics::EmitImpactParticles(u8 apply_to_corners_mask)
{
    Sprite* pCarSprite = mpCar->mCarSprite;
    Fix16_Point box_xy;
    u32 cornerCount;

    // The original tests only al: IsOnWater returns a bool there.
    if (!static_cast<u8>(pCarSprite->IsOnWater()) && !mpCar->IsMaxDamage())
    {
        if (apply_to_corners_mask == 0)
        {
            apply_to_corners_mask = pCarSprite->CheckCornerZCollisions(&cornerCount);
        }
        s32 box_idx = 0;
        u8 box_corner_mask = 1;
        do
        {
            if ((apply_to_corners_mask & box_corner_mask) == box_corner_mask)
            {
                box_xy = pCarSprite->GetBoundingBoxCorner(box_idx);
                gParticle_8->EmitImpactParticles(box_xy.x, box_xy.y, mCp3, mLinvel1);
            }
            box_idx++;
            box_corner_mask *= 2;
        } while (box_idx < 4);
    }

    gCollisionSoundQueue->AddFloorCollision(pCarSprite);
}

MATCH_FUNC(0x55b970)
char_type CarPhysics::ProcessGroundCollisionAndSurfaceType(char_type* check_mask)
{
    Sprite* pSprite = this->mpCar->mCarSprite;
    // The original has an EH state from entry for an object with a destructor that has no storage
    // (declared here: declared first it moves the `xor edi` zero above the sprite loads)
    Fix16_Point unused_point;
    s32 corner_idx_ = 0;
    u8 mask_;
    u32 corner_result;
    this->mTpa = 0;

    if (IsInAir())
    {
        if (this->mAirZVel > -kFP16Eighth)
        {
            this->mAirZVel -= kFP16One64th_6FE1AC;
        }
    }

    if (!gMap_0x370->IsZOnGround(this->mCp1.x, this->mCp1.y, this->mCp3))
    {
        this->mBlockSpec = 0;
        *check_mask = pSprite->CheckCornerZCollisions(&corner_result);
        if (corner_result == 1 || corner_result == 2)
        {
            this->mTpa = 1;
        }

        if (!*check_mask)
        {
            if (mSurfaceType != car_surface_type::slope_northwards_1 && mSurfaceType != car_surface_type::slope_southwards_2 && mSurfaceType != car_surface_type::slope_westwards_3 && mSurfaceType != car_surface_type::slope_eastwards_4)
            {
                this->mSurfaceType = car_surface_type::air_surface_6;
                return 0;
            }

            char_type result = pSprite->IsTouchingSlopeBlock();
            if (!result)
            {
                this->mSurfaceType = car_surface_type::air_surface_6;
                return result;
            }
        }
        else
        {
            Trailer* pTrailer = this->mpCar->mpTrailer;
            if (!pTrailer || pTrailer->GetCabOrTrailerCar(mpCar)->mPhysics->mSurfaceType == car_surface_type::air_surface_6)
            {
                mask_ = 1;
                do
                {
                    if ((mask_ & static_cast<u8>(*check_mask)) != mask_)
                    {
                        // Temporaries chained: corner, corner - cm1, / 50, each with its own EH state
                        ApplyImpulseWithTrailerRedirect(&pSprite->GetBoundingBoxCorner(corner_idx_).Sub(mCm1).Divide(Fix16(50)));
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
        this->mBlockSpec =
            gMap_0x370->GetBlockSpec(this->mCp1.x, this->mCp1.y, this->mCp3 - kFpOne_6FE210);
    }

    Fix16 cp3 = this->mCp3;
    bool is_air_surface = this->mSurfaceType == car_surface_type::air_surface_6;
    if (cp3.GetFracValue() == kFP16Zero_6FE20C)
    {
        cp3 -= kFpOne_6FE210;
    }

    s32 cp3_int = cp3.ToInt();
    u8 gradient_level;
    u8 gradient_size;
    s32 surface_type = gMap_0x370->GetBlockSurfaceType(mCp1.x.ToInt(), this->mCp1.y.ToInt(), cp3_int, &gradient_size, &gradient_level);

    if (surface_type != car_surface_type::no_block_5)
    {
        this->mSurfaceType = surface_type;
        this->mCurrentSlopeLength = static_cast<char>(gradient_size);
        this->mCurrentSlopeLeftTiles = gradient_level;
        this->mCurrentTileZ = cp3_int;
        if (surface_type == car_surface_type::remapped_water_tile_7)
        {
            u8 water_mask = pSprite->GetWaterCornerMask();
            if (water_mask == 15)
            {
                this->mSurfaceType = car_surface_type::water_surface_8;
            }
            else
            {
                s32 corner_idx = 0;
                mask_ = 1;
                do
                {
                    if ((water_mask & mask_) != mask_)
                    {
                        ApplyImpulseWithTrailerRedirect(&mCm1.Sub(pSprite->GetBoundingBoxCorner(corner_idx)).Divide(Fix16(50)));
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
void CarPhysics::ProcessGroundCollisionAndEmitImpactParticles()
{
    u8 corner_bits1;
    u8 corner_bits2;
    char_type bWasInAir = ProcessGroundCollisionAndSurfaceType(reinterpret_cast<char_type*>(&corner_bits1));
    Trailer* pTrailer = this->mpCar->mpTrailer;
    if (pTrailer)
    {
        CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
        pPhysics->SetCurrentCarInfoAndModelPhysics();
        char_type bTrailerWasInAir = pPhysics->ProcessGroundCollisionAndSurfaceType(reinterpret_cast<char_type*>(&corner_bits2));
        if (bTrailerWasInAir)
        {
            if (bWasInAir || mSurfaceType == car_surface_type::air_surface_6)
            {
                pPhysics->EmitImpactParticles(corner_bits2);
            }
        }
        SetCurrentCarInfoAndModelPhysics();
        if ((bTrailerWasInAir || pPhysics->mSurfaceType == car_surface_type::air_surface_6) && bWasInAir)
        {
            EmitImpactParticles(corner_bits1);
        }
    }
    else if (bWasInAir)
    {
        EmitImpactParticles(corner_bits1);
    }
}

MATCH_FUNC(0x55c150)
bool CarPhysics::TestCollision()
{
    Sprite* pCarSprite = this->mpCar->mCarSprite;
    if (!pCarSprite->CheckSpriteMovementRegion() && !pCarSprite->QuerySpriteCollision(sprite_types_enum::none_0))
    {
        Trailer* pTrailer = this->mpCar->mpTrailer;
        if (pTrailer)
        {
            pCarSprite = pTrailer->mTrailerCar->mCarSprite;
            if (!pCarSprite->CheckSpriteMovementRegion() && !pCarSprite->QuerySpriteCollision(sprite_types_enum::none_0))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    gCollisionTarget.SetSourceSprite(pCarSprite);
    return true;
}

// https://decomp.me/scratch/Mht60 stack size issue remaining
MATCH_FUNC(0x55c3b0)
bool CarPhysics::SweepTestMovementForCollision(Fix16* outHitStep, Fix16* outNoHitStep)
{
    save_state();

    Fix16 movement;
    movement = ComputeRequiredSweepSteps();

    *outNoHitStep = kFP16Zero_6FE20C;
    *outHitStep = kFP16Zero_6FE20C;

    if (movement > kFpOne_6FE210)
    {
        Fix16 stepSize = kFpOne_6FE210 / movement;
        Fix16 accumulated = kFP16Zero_6FE20C;
        s32 numSteps = movement.ToInt();

        for (s32 stepIndex = 0; stepIndex < numSteps; stepIndex++)
        {
            accumulated += stepSize;

            restore_saved_physics_state();
            ApplyMovementStep(accumulated);

            if (TestCollision())
            {
                *outHitStep = accumulated;
                return true;
            }
            else
            {
                *outNoHitStep = accumulated;
            }
        }
    }

    restore_state();
    UpdateCarAndTrailerSpriteFromPhysics();

    if (TestCollision())
    {
        *outHitStep = kFpOne_6FE210;
        return true;
    }
    else
    {
        *outNoHitStep = kFpOne_6FE210;
        return false;
    }
}

MATCH_FUNC(0x55c560)
void CarPhysics::BinarySearchCollisionTime(Fix16& low, Fix16& high)
{
    for (s32 i = 0; i < 3; i++)
    {
        restore_saved_physics_state();
        Fix16 total = (high + low) / 2;
        ApplyMovementStep(total);
        if (TestCollision())
        {
            low = total;
        }
        else
        {
            high = total;
        }
    }
}

// https://decomp.me/scratch/A5Yhg
MATCH_FUNC(0x55c5c0)
void CarPhysics::HandleMapBoundaryCollisionY(Fix16_Point& point, Ang16 angle)
{
    Fix16_Point RelativePointVelocity;
    if (mpCar->mCarSprite->GetNearestHorizontalEdgeToCoordinate(gCollisionTarget.mHsegY,
                                                                CollisionIntersectionPoint,
                                                                sCollisionArea))
    {
        sCollisionNormal.SetXY(Fix16(0), mCp1.y - gCollisionTarget.mHsegY);
        RelativePointVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint);
    }
    else
    {
        CollisionIntersectionPoint.SetXY(gCollisionTarget.mVsegX, gCollisionTarget.mHsegY);
        CollisionIntersectionPoint -= point;
        {
            Ang16 rot_angle(mTheta.rValue - angle.rValue);
            rot_angle.Normalize_406C20();
            CollisionIntersectionPoint.RotateByAngle(rot_angle);
        }
        CollisionIntersectionPoint.x += mCp1.x;
        CollisionIntersectionPoint.y = gCollisionTarget.mHsegY;

        if (Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint.x - gCollisionTarget.mHsegXMin) <
            Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint.x - gCollisionTarget.mHsegXMax))
        {
            CollisionIntersectionPoint.x = gCollisionTarget.mHsegXMin;
        }
        else
        {
            CollisionIntersectionPoint.x = gCollisionTarget.mHsegXMax;
        }
        RelativePointVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint);
        if (mCp1.y < CollisionIntersectionPoint.y)
        {
            sCollisionNormal.SetXY(Fix16(0), -kFpOne_6FE210);
        }
        else
        {
            sCollisionNormal.SetXY(Fix16(0), kFpOne_6FE210);
        }
    }
    CarPhysics::HandleWorldCollision(RelativePointVelocity);
}

// https://decomp.me/scratch/N4ktT
MATCH_FUNC(0x55c820)
void CarPhysics::HandleMapBoundaryCollisionX(Fix16_Point& point, Ang16 angle)
{
    Fix16_Point RelativePointVelocity;
    if (mpCar->mCarSprite->GetNearestVerticalEdgeToCoordinate(gCollisionTarget.mVsegX,
                                                              CollisionIntersectionPoint,
                                                              sCollisionArea))
    {
        sCollisionNormal.SetXY(mCp1.x - gCollisionTarget.mVsegX, Fix16(0));
        RelativePointVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint);
    }
    else
    {
        CollisionIntersectionPoint.SetXY(gCollisionTarget.mVsegX, gCollisionTarget.mHsegY);
        CollisionIntersectionPoint -= point;
        {
            Ang16 rot_angle(mTheta.rValue - angle.rValue);
            rot_angle.Normalize_406C20();
            CollisionIntersectionPoint.RotateByAngle(rot_angle);
        }
        CollisionIntersectionPoint.y += mCp1.y;
        CollisionIntersectionPoint.x = gCollisionTarget.mVsegX;

        if (Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint.y - gCollisionTarget.mVsegYMin) <
            Fix16::Abs_negate_out_of_line(CollisionIntersectionPoint.y - gCollisionTarget.mVsegYMax))
        {
            CollisionIntersectionPoint.y = gCollisionTarget.mVsegYMin;
        }
        else
        {
            CollisionIntersectionPoint.y = gCollisionTarget.mVsegYMax;
        }
        RelativePointVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint);
        if (mCp1.x < CollisionIntersectionPoint.x)
        {
            sCollisionNormal.SetXY(-kFpOne_6FE210, Fix16(0));
        }
        else
        {
            sCollisionNormal.SetXY(kFpOne_6FE210, Fix16(0));
        }
    }
    CarPhysics::HandleWorldCollision(RelativePointVelocity);
}

// 9.6f 0x4A4170
MATCH_FUNC(0x55ca70)
void CarPhysics::DispatchCollision(Fix16_Point& point, Ang16 angle)
{
    Fix16_Point unusedPoint; // never read, but needed to match
    u8 overlap_count;
    switch (gCollisionTarget.mType)
    {
        case collision_target_type::horizontal_edge_1:
            HandleMapBoundaryCollisionY(point, angle);
            break;
        case collision_target_type::vertical_edge_2:
            HandleMapBoundaryCollisionX(point, angle);
            break;
        case collision_target_type::sprite_3:
            CollisionIntersectionPoint = mpCar->mCarSprite->FindCollisionIntersectionPoint(gCollisionTarget.mpHitSprite,
                                                                                                     point,
                                                                                                     angle,
                                                                                                     sCollisionArea,
                                                                                                     sOtherCollisionArea,
                                                                                                     overlap_count);

            Car* pCar = gCollisionTarget.mpHitSprite->AsCar();
            if (pCar)
            {
                HandleCarCollision(pCar);
            }
            else
            {
                Char* pChar = gCollisionTarget.mpHitSprite->AsCharB4();
                if (pChar)
                {
                    ProcessPedImpact(pChar, overlap_count);
                }
                else
                {
                    Object* pObject = gCollisionTarget.mpHitSprite->AsObject2C();
                    HandleObjectCollision(pObject, sCollisionArea);
                }
            }
            break;
    }

    gCollisionSoundQueue->AddCollision(mpCar->mCarSprite, sCollisionDamage);
}

// https://decomp.me/scratch/0TpGe
MATCH_FUNC(0x55cbb0)
void CarPhysics::ReplayAndDispatchCollision(Fix16 step_a, Fix16 step_b)
{
    Fix16_Point point;
    CarPhysics* pPhysics = gCollisionTarget.mpSourceSprite->AsCar()->mPhysics;

    restore_saved_physics_state();
    ApplyMovementStep(step_a);

    point = pPhysics->mCp1;
    Ang16 theta = pPhysics->mTheta;

    restore_saved_physics_state();
    ApplyMovementStep(step_b);
    if (pPhysics == this)
    {
        DispatchCollision(point, theta);
    }
    else
    {
        pPhysics->SetCurrentCarInfoAndModelPhysics();
        pPhysics->DispatchCollision(point, theta);
        SetCurrentCarInfoAndModelPhysics();
    }
}

// 9.6f 0x4A0120
MATCH_FUNC(0x55d200)
void CarPhysics::SpawnSkidSegment(s32 box_idx, Fix16_Point wheel_offset, s32 surface)
{
    Fix16_Point skid_vec;
    Fix16_Point skid_mid;
    Fix16 len; // declared up here: its stack slot is the dead box_idx one, shared with the / 2 temporary

    wheel_offset.RotateByAngle(mTheta);

    wheel_offset += this->mCp1;

    char_type map_ret = gMap_0x370->GetSurfaceTypeBelowCoord(wheel_offset.x, wheel_offset.y, mCp3);
    if (map_ret == car_surface_type::no_block_5 || surface == 3 && map_ret != car_surface_type::remapped_water_tile_7)
    {
        Fix16_Point* pBoxCorner_ = &this->mLastSkidPos[static_cast<u8>(box_idx)];
        pBoxCorner_->SetZero();
    }
    else
    {
        Fix16_Point* pBoxCorner = &this->mLastSkidPos[static_cast<u8>(box_idx)];
        if (!pBoxCorner->IsNull())
        {
            skid_vec = wheel_offset - *pBoxCorner;
            skid_mid = pBoxCorner->Add(wheel_offset) / 2;
            Fix16 obj_x = skid_mid.x;
            Fix16 obj_y = skid_mid.y;

            Ang16 r = skid_vec.GetAngle();
            len = skid_vec.GetLength_all_out_of_line_abs();
            if (len > kFP16Zero_6FE20C)
            {
                Object* pObj =
                    gObject_5C->NewPhysicsObj(get_skid_obj_type(surface, len), obj_x, obj_y, mCp3, r);
                if (pObj)
                {
                    if (pObj->mSprite->IsStraddlingSlopeAndFlat())
                    {
                        pObj->RequestRemoval();
                    }
                }
            }
        }
        *pBoxCorner = wheel_offset;
    }
}

MATCH_FUNC(0x55D490)
EXPORT s32 __stdcall get_skid_obj_type(s32 surface, Fix16 skid_len)
{
    if (skid_len <= kFP16One16th)
    {
        switch (surface)
        {
            case 0:
                return objects::small_grey_skid_117;
            case 2:
                return objects::small_brown_skid_116;
            case 1:
                return objects::small_red_skid_118;
            case 3:
                return objects::small_white_skid_126;
        }
    }
    else if (skid_len <= kFP16Quarter_6FDFD4)
    {
        switch (surface)
        {
            case 0:
                return objects::medium_grey_skid_120;
            case 2:
                return objects::medium_brown_skid_119;
            case 1:
                return objects::medium_red_skid_121;
            case 3:
                return objects::medium_white_skid_125;
            default:
                FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\physics.cpp", 2331, surface);
        }
    }
    else if (skid_len <= kFpThreeEighths)
    {
        switch (surface)
        {
            case 0:
                return objects::big_grey_skid_250;
            case 2:
                return 249;
            case 1:
                return 253;
            case 3:
                return objects::big_white_skid_124;
        }
    }
    else
    {
        switch (surface)
        {
            case 0:
                return objects::huge_grey_skid_147;
            case 2:
                return objects::huge_brown_skid_146;
            case 1:
                return objects::huge_red_skid_144;
            case 3:
                return objects::huge_white_skid_145;
        }
    }
    return objects::small_grey_skid_117;
}

// https://decomp.me/scratch/y9UHj
MATCH_FUNC(0x55dc00)
void CarPhysics::UpdateWheelSkidEffects()
{

    if (mpCar->IsOnScreenForAnyPlayer())
    {
        Fix16 rear_wheel_offset;
        Fix16 front_wheel_offset;

        s32 skid_surface = (mBlockSpec == 2 || mBlockSpec == 10) ? 2 : 0;

        Fix16 half_width = mpCar->GetCarWidth() * kFpSevenSixteenths;
        rear_wheel_offset = mpCar->ApplyScale(gCarInfo_2C->mRearWheelOffset);
        front_wheel_offset = mpCar->ApplyScale(gCarInfo_2C->mFrontWheelOffset);

        if (mSurfaceType == car_surface_type::remapped_water_tile_7 || mSurfaceType == car_surface_type::water_surface_8 || mSurfaceType == car_surface_type::water_tile_9)
        {
            SpawnSkidSegment(0, Fix16_Point(-half_width, rear_wheel_offset), 3);
            SpawnSkidSegment(1, Fix16_Point(half_width, rear_wheel_offset), 3);
        }
        else if ((mRearSkid >= gCarInfo_2C->mSkidThreshhold2 ||
                 mDriveWheelsLockedQ > 0 && gCarInfo_2C->mRearDriveBias > kFP16Zero_6FE20C) &&
                mSurfaceType != car_surface_type::air_surface_6)
        {
            SpawnSkidSegment(0, Fix16_Point(-half_width, rear_wheel_offset), skid_surface);
            SpawnSkidSegment(1, Fix16_Point(half_width, rear_wheel_offset), skid_surface);
        }
        else
        {
            mLastSkidPos[0].reset();
            mLastSkidPos[1].reset();
        }

        if (mSurfaceType == car_surface_type::remapped_water_tile_7 || mSurfaceType == car_surface_type::water_surface_8 || mSurfaceType == car_surface_type::water_tile_9)
        {
            SpawnSkidSegment(3, Fix16_Point(-half_width, front_wheel_offset), 3);
            SpawnSkidSegment(2, Fix16_Point(half_width, front_wheel_offset), 3);
        }
        else if ((mFrontSkid >= gCarInfo_2C->mSkidThreshhold1 ||
                  mDriveWheelsLockedQ > 0 && gCarInfo_48->mFrontDriveBias > kFP16Zero_6FE20C) &&
                 mSurfaceType != car_surface_type::air_surface_6)
        {
            SpawnSkidSegment(3, Fix16_Point(-half_width, front_wheel_offset), skid_surface);
            SpawnSkidSegment(2, Fix16_Point(half_width, front_wheel_offset), skid_surface);
        }
        else
        {
            mLastSkidPos[3].reset();
            mLastSkidPos[2].reset();
        }
    }
    else
    {
        mLastSkidPos[0].reset();
        mLastSkidPos[1].reset();
        mLastSkidPos[2].reset();
        mLastSkidPos[3].reset();
    }

    if (mDriveWheelsLockedQ > 0)
    {
        mDriveWheelsLockedQ--;
    }
}

MATCH_FUNC(0x55e260)
void CarPhysics::DoSkidmarks()
{
    if (!bSkip_skidmarks)
    {
        CarPhysics::UpdateWheelSkidEffects();
        Trailer* pTrailer = mpCar->mpTrailer;
        if (pTrailer)
        {
            CarPhysics* pPhysics = pTrailer->mTrailerCar->mPhysics;
            pPhysics->SetCurrentCarInfoAndModelPhysics();
            pPhysics->mFrontSkid = kFP16Zero_6FE20C;
            pPhysics->mRearSkid = kFP16Zero_6FE20C;
            pPhysics->UpdateWheelSkidEffects();
            CarPhysics::SetCurrentCarInfoAndModelPhysics();
        }
    }
}

MATCH_FUNC(0x55e470)
bool CarPhysics::StepMovementAndCollisions()
{
    s32 hit_count = 0;
    bool bCollided = false;
    s32 max_hits = 2;

    Sprite* hit_sprites[4];

    Fix16 hit_step;
    Fix16 no_hit_step;

    while (sRemainingTimeStep >= kFP16Eighth)
    {
        gCollisionTarget.Reset();
        this->mZVel = 0;
        save_physics_state();
        ApplyMovementStep(kFpOne_6FE210);
        UpdateTrailerPhysicsFromTowingCar();
        if (SweepTestMovementForCollision(&hit_step, &no_hit_step))
        {
            bCollided = true;
            if (gCollisionTarget.mpHitSprite)
            {
                for (s32 i = 0; i < hit_count; i++)
                {
                    if (hit_sprites[i] == gCollisionTarget.mpHitSprite)
                    {
                        restore_saved_physics_state();
                        UpdateCarAndTrailerSpriteFromPhysics();
                        ProcessGroundCollisionAndEmitImpactParticles();
                        return true;
                    }
                }
            }

            BinarySearchCollisionTime(hit_step, no_hit_step);
            if (mpCar->IsTrainModel() && !mLinvel1.HasZeroComponent())
            {
                no_hit_step = kFP16Zero_6FE20C;
            }
            ReplayAndDispatchCollision(hit_step, no_hit_step);
        }

        sRemainingTimeStep = (sRemainingTimeStep * (kFpOne_6FE210 - no_hit_step));
        hit_sprites[hit_count++] = gCollisionTarget.mpHitSprite;

        if ((gCollisionTarget.IsCharB4() || gCollisionTarget.IsObj2C()) && max_hits < 4)
        {
            ++max_hits;
        }

        ProcessGroundCollisionAndEmitImpactParticles();

        if (hit_count >= max_hits)
        {
            break;
        }
    }
    return bCollided;
}

MATCH_FUNC(0x55eb80)
bool CarPhysics::CheckAndHandleCarAndTrailerCollisions()
{
    gCarManager_6C->mObjectHitActive = false;

    bool bCollision = gSpriteGrid_2->CheckAndHandleCollisionInStrips(mpCar->mCarSprite);
    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        bCollision |= gSpriteGrid_2->CheckAndHandleCollisionInStrips(pTrailer->mTrailerCar->mCarSprite);
    }

    return bCollision;
}

MATCH_FUNC(0x55ec30)
void CarPhysics::ApplyVerticalEdgeNudge()
{
    Ang16 theta;
    if (mIsBackwardGasOn)
    {
        theta = mTheta + kAng180_6FE12A;
    }
    else
    {
        theta = mTheta;
    }

    if (sCollisionNormal.x > kFP16Zero_6FE20C)
    {
        if (theta > kAng270_6FE154)
        {
            ApplyAngularImpulse(kAngFix16OneDegree);
        }
        else if (theta > kAng180_6FE12A)
        {
            ApplyAngularImpulse(-kAngFix16OneDegree);
        }
    }
    else
    {
        if (theta < kAng90_6FE00C)
        {
            ApplyAngularImpulse(-kAngFix16OneDegree);
        }
        else if (theta < kAng180_6FE12A)
        {
            ApplyAngularImpulse(kAngFix16OneDegree);
        }
    }

    ApplyForceScaledByMass(sCollisionNormal.NormalizeSafe() * kFpOne64th);
    mSbw = 1;
}

// https://decomp.me/scratch/foNCl
// The largest of the three values
MATCH_FUNC(0x55EEE0)
Fix16 __stdcall Fix16::Max3_ool_55EEE0(Fix16& first, Fix16& second, Fix16& third)
{
    return Max3_55EEE0(first, second, third);
}

MATCH_FUNC(0x55ef20)
void CarPhysics::ApplyHorizontalEdgeNudge()
{

    Ang16 theta;
    if (mIsBackwardGasOn)
    {
        theta = mTheta + kAng180_6FE12A;
    }
    else
    {
        theta = mTheta;
    }

    if (sCollisionNormal.y > kFP16Zero_6FE20C)
    {
        if (theta > kAng90_6FE00C && theta < kAng180_6FE12A)
        {
            ApplyAngularImpulse(-kAngFix16OneDegree);
        }
        else if (theta > kAng180_6FE12A && theta < kAng270_6FE154)
        {
            ApplyAngularImpulse(kAngFix16OneDegree);
        }
    }
    else
    {
        // Moving backwards or stationary
        if (theta < kAng90_6FE00C)
        {
            ApplyAngularImpulse(kAngFix16OneDegree);
        }
        else if (theta > kAng270_6FE154)
        {
            ApplyAngularImpulse(-kAngFix16OneDegree);
        }
    }

    ApplyForceScaledByMass(sCollisionNormal.NormalizeSafe() * kFpOne64th);
    this->mSbw = 1;
}

// matches on decompme: https://decomp.me/scratch/Hyun8
MATCH_FUNC(0x55f020)
void CarPhysics::ApplyTurningForce()
{
    Fix16_Point local_offset;
    Fix16 turn_force = kAiWallTurnForce;
    Object* pObj = gCollisionTarget.mpHitSprite->AsObject2C();
    Fix16 turn_torque;

    if (pObj && pObj->mModel == objects::diagonal_wall_collision_obj_166)
    {
        turn_torque = kAngFix16OneDegree;
        turn_force = kFpOne64th;
    }
    else
    {
        if (mpCar->IsDrivenByPlayer())
        {
            turn_torque = kPlayerWallTurnTorque;
            turn_force = kPlayerWallTurnForce;
        }
        else
        {
            turn_torque = kAiWallTurnTorque;
            turn_force = kAiWallTurnForce;
        }
    }

    local_offset = (mCp1 - CollisionIntersectionPoint);

    local_offset.RotateByAngle(-mTheta);

    if (local_offset.x >= kFP16Zero_6FE20C)
    {
        turn_torque = -turn_torque;
    }
    ApplyAngularImpulse(turn_torque);
    ApplyForceScaledByMass(sCollisionNormal.NormalizeSafe().Multiply(turn_force));
}

MATCH_FUNC(0x55f240)
bool CarPhysics::ApplyMovementCommand()
{
    switch (gCollisionTarget.mType)
    {
        case collision_target_type::horizontal_edge_1:
            ApplyHorizontalEdgeNudge();
            return true;
        case collision_target_type::vertical_edge_2:
            ApplyVerticalEdgeNudge();
            return true;
        case collision_target_type::sprite_3:
            ApplyTurningForce();
            mSbw = 0;
            break;
    }
    return false;
}

MATCH_FUNC(0x55f280)
char_type CarPhysics::ProcessCollisionAndClampVelocity()
{
    bool movementApplied = false;

    Fix16_Point_POD cm1 = this->mCm1;

    char_type stepResult;
    while (1)
    {
        sRemainingTimeStep = kFpOne_6FE210;
        stepResult = StepMovementAndCollisions();
        const char_type prevStepResult = stepResult;

        if (movementApplied)
        {
            if (sRemainingTimeStep == kFpOne_6FE210)
            {
                this->mLinvel1.reset();
                this->mAngVelRad = kFP16Zero_6FE20C;
            }
            break;
        }

        this->mSbw = 0;

        if (sRemainingTimeStep != kFpOne_6FE210 || !stepResult)
        {
            break;
        }

        if (IsAccelerationOrReverseOn())
        {
            movementApplied = ApplyMovementCommand();

            if (movementApplied)
            {
                UpdateLinearAndAngularAccel();
                IntegrateAndClampVelocities();
                continue;
            }
        }

        stepResult = prevStepResult;
        break;
    }

    this->mVelReadOnly.x = this->mCm1.x - cm1.x;
    this->mVelReadOnly.y = this->mCm1.y - cm1.y;

    return stepResult;
}

MATCH_FUNC(0x55f330)
void CarPhysics::StepPhysics()
{
    sRemainingTimeStep = kFpOne_6FE210;
    save_physics_state();
    ApplyMovementStep(kFpOne_6FE210);
}

MATCH_FUNC(0x55f360)
char_type CarPhysics::CheckPendingCollision()
{
    if (this->mpCar->IsCrushing())
    {
        gCarManager_6C->mImpactMode = car_impact_mode::crush_damage_1;
        if (gSpriteGrid_1->CheckAndHandleAllCollisionsForSprite(mpCar->mCarSprite, sprite_types_enum::car_2))
        {
            return 1;
        }
        this->mpCar->ClearCrushFlag();
    }
    return 0;
}

// https://decomp.me/scratch/dN85v
MATCH_FUNC(0x55F3B0)
EXPORT Fix16_Point __stdcall ComputeCollisionImpulse(Fix16 OwnerMass,
                                                                Fix16 TargetMass,
                                                                Fix16_Point& RelativeVelocity,
                                                                Fix16_Point& DistToCollision_ByRef,
                                                                Fix16_Point& CollisionIntersectPoint,
                                                                Fix16_Point& CoM_related,
                                                                Fix16_Point& TargetCoM,
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
        return kZeroPoint;
    }

    // The function runs out of inline expansions: most Fix16 operators are the out-of-line copies
    Fix16 OwnerMassFactor = kFpOne_6FE210 / OwnerMass;
    DistOrthogonalToCollision = (CollisionIntersectPoint - CoM_related).Rotate90CCW();
    DirectionFromCoM_to_Collision = DistToCollision_ByRef.NormalizeSafe(); // vector unit 1, supposedly

    Fix16 VelocityFactor = -(kFpOne_6FE210 + offset) * DotProductOOL_49E500(RelativeVelocity, DirectionFromCoM_to_Collision);

    Fix16 MassFactor;
    if (TargetMass == kFP16MinusOne) // infinite mass
    {
        MassFactor = DotProductOOL_49E500(DirectionFromCoM_to_Collision, DirectionFromCoM_to_Collision) * OwnerMassFactor
            + SquareFix16(DotProductOOL_49E500(DistOrthogonalToCollision, DirectionFromCoM_to_Collision)) / OwnerMomOfInertia;
    }
    else
    {
        Fix16 TargetMassFactor = kFpOne_6FE210 / TargetMass;
        Fix16_Point TargetOrthogonal = (CollisionIntersectPoint - TargetCoM).Rotate90CCW();

        // DotProduct_49E500 is out of line here (0x560680)
        MassFactor =
            (DotProduct(DirectionFromCoM_to_Collision, DirectionFromCoM_to_Collision)
                 .Multiply(OwnerMassFactor.Add(TargetMassFactor))
                 .Add(SquareFix16(DotProduct(DistOrthogonalToCollision, DirectionFromCoM_to_Collision))
                                 .Divide(OwnerMomOfInertia)))
                .Add(
                    SquareFix16(DotProduct(TargetOrthogonal, DirectionFromCoM_to_Collision)).Divide(TargetMomOfInertia));
    }

    // scale vector norm by factors, so direction is kept
    Fix16 ImpulseScale;
    ImpulseScale = VelocityFactor.Divide(MassFactor);
    Impulse = DirectionFromCoM_to_Collision.Multiply(ImpulseScale);
    return Impulse;
}

MATCH_FUNC(0x55f740)
void CarPhysics::ApplyForceWithTrailerRedirect(Fix16_Point* pPoint, Fix16_Point* pForce)
{

    if (mpCar->IsTrailerCar())
    {
        CarPhysics* pPhysics = mpCar->mpTrailer->mTruckCab->mPhysics;
        pPhysics->SetCurrentCarInfoAndModelPhysics();
        pPhysics->ApplyForceAndIntegrate(pPoint, *pForce);
        SetCurrentCarInfoAndModelPhysics();
    }
    else
    {
        ApplyForceAndIntegrate(pPoint, *pForce);
    }
}

MATCH_FUNC(0x55f7a0)
void CarPhysics::ApplyForceAndIntegrate(Fix16_Point* pPoint, Fix16_Point force)
{
    ApplyForceAtPoint(pPoint, &force, 0);
    UpdateLinearAndAngularAccel();
    IntegrateAndClampVelocities();
}

// GetLength_41E260 as inlined into ApplyImpactForcesAndDamage: both Abs are called out of line
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
void CarPhysics::ApplyForceAtPoint(Fix16_Point* pPoint, Fix16_Point* pForce, s32 bRotate)
{
    Fix16_Point_POD point(pPoint->x, pPoint->y);

    switch (bRotate)
    {
        case 1:
            point.RotateByAngle(mTheta);
            point += mCp1;
            break;
    }

    mForceAccum += *pForce;

    Fix16 cross_a = (point.y - mCm1.y) * pForce->x;
    Fix16 cross_b = (point.x - mCm1.x) * pForce->y;
    Fix16 torque = cross_b - cross_a;
    mTorqueAccum = mTorqueAccum + torque;
}

MATCH_FUNC(0x55f930)
void CarPhysics::AccumulateImpulse(Fix16_Point* pPoint)
{
    mForceAccum += (*pPoint * gCarInfo_48->mMass);
}

// https://decomp.me/scratch/qCXRd
// Takes the direction by reference, so the negated point is read through the pointer the
// negation returns (a named local reads it from its stack slot instead)
static inline void EmitImpactAtCollisionPoint(Fix16& z, const Fix16_Point& dir)
{
    gParticle_8->EmitImpactParticles(CollisionIntersectionPoint.x, CollisionIntersectionPoint.y, z, dir);
}

MATCH_FUNC(0x55f970)
void CarPhysics::ApplyAngularImpulse(Fix16 impulse)
{
    this->mTorqueAccum -= (gCarInfo_2C->mMomentOfInertia * impulse);
}

MATCH_FUNC(0x55f9a0)
void CarPhysics::ApplyForceScaledByMass(Fix16_Point& force)
{
    mForceAccum += force.Multiply(CarPhysics::CalculateMass());
}

MATCH_FUNC(0x55fa10)
void CarPhysics::ApplyImpulseWithTrailerRedirect(Fix16_Point* pPoint)
{
    if (mpCar->IsTrailerCar())
    {
        // We are on the trailer so apply impulse to the truck cab instead
        CarPhysics* pPhysics = mpCar->mpTrailer->mTruckCab->mPhysics;
        pPhysics->SetCurrentCarInfoAndModelPhysics();
        pPhysics->AccumulateImpulse(pPoint);
        SetCurrentCarInfoAndModelPhysics();
    }
    else
    {
        AccumulateImpulse(pPoint);
    }
}

// https://decomp.me/scratch/TSKLx
MATCH_FUNC(0x55fa60)
Fix16 CarPhysics::ApplyImpactForcesAndDamage(Fix16_Point& PointOfForce, Fix16_Point& Impulse, s32 base_dmg)
{
    Fix16_Point NewImpulse;
    Fix16 ImpulseIntensity = GetLength_AbsOutOfLine_41E260(Impulse);

    if ((ImpulseIntensity / CalculateMass()) > kFpPoint01_6FE37C)
    {
        NewImpulse = Impulse;

        if (mpCar->IsFlagSet(car_flag::ResistsPushWhenStopped))
        {
            if (!mpCar->IsDrivenByPlayer())
            {
                Fix16 CarSpeed = get_car_velocity();
                if (CarSpeed <= kFpPoint05_6FE1D4 || mIsHandBrakeOn)
                {
                    NewImpulse = (Impulse / kFP16Three_6FE218);
                }
            }
        }

        mpCar->TriggerCarAlarm();

        if (!mpCar->IsFlagSet(car_flag::NoCollide))
        {
            ApplyForceWithTrailerRedirect(&PointOfForce, &NewImpulse);
            AddDamage(base_dmg);

            if (!mpCar->IsDrivenByPlayer())
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
    return Vector1.x.Multiply(Vector2.x).Add(Vector1.y.Multiply(Vector2.y));
}

// 9.6f 0x49E0E0, inlined in 10.5
static inline Fix16 __stdcall SquareFix16(const Fix16& value)
{
    return value.Multiply(value);
}

MATCH_FUNC(0x55fc30)
void CarPhysics::ApplyImpulseAndDamage(Fix16_Point& point, s32 base_dmg)
{

    Fix16_Point impulse;
    if (!mpCar->IsTrainModel())
    {
        if (this->mIsHandBrakeOn)
        {
            impulse = (point / kFP16Two_6FE214);
        }
        else
        {
            impulse = point;
        }

        ApplyImpulseWithTrailerRedirect(&impulse);

        u32 rng_damage = base_dmg + gpRng->get_cur_rng();
        if (rng_damage > this->mDamagedUntilCycle)
        {
            this->mDamagedUntilCycle = rng_damage;
        }

        if (!mpCar->IsDrivenByPlayer())
        {
            this->mIsHandBrakeOn = 0;
        }
    }
}

MATCH_FUNC(0x55fd00)
void CarPhysics::HandleWorldCollision(Fix16_Point& relative_velocity)
{

    Fix16_Point Impulse = ComputeCollisionImpulse(CalculateMass(),
                                                             kFP16MinusOne,
                                                             relative_velocity,
                                                             sCollisionNormal,
                                                             CollisionIntersectionPoint,
                                                             ComputeCombinedCenterOfMass(),
                                                             kZeroPoint,
                                                             GetEffectiveMomentOfInertia(),
                                                             kFP16Zero_6FE20C,
                                                             kFP16Quarter_6FE1A8);

    if (mSurfaceType == car_surface_type::air_surface_6 && mZVel <= kFP16Zero_6FE20C)
    {
        mAirZVel = (-mAirZVel) * kFpPoint7;

        if (Fix16::Abs(mAirZVel) < kFP16One32nd)
        {
            mAirZVel = kFP16Zero_6FE20C;
        }
    }

    Fix16 damage;
    sCollisionDamage = damage = ApplyImpactForcesAndDamage(CollisionIntersectionPoint, Impulse, 15);
    if (mSurfaceType == car_surface_type::air_surface_6 && mZVel == kFP16Zero_6FE20C && mAirZVel == kFP16Zero_6FE20C &&
        mLinvel1.IsNull() && damage < kFP16One_6FE098)
    {
        damage = kFP16One_6FE098;
        sCollisionDamage = damage;
    }
    mpCar->ApplyImpactDamage(damage);
    if (mLinvel1.GetLength_41E260() > FastCarMinVelocity)
    {
        if (!mpCar->IsMaxDamage())
        {
            EmitImpactAtCollisionPoint(mCp3, -relative_velocity);
        }
        mpCar->TryDamageArea(sCollisionArea, sCollisionDamage.mValue);
    }
}

// https://decomp.me/scratch/IiClE
MATCH_FUNC(0x55ff20)
void CarPhysics::HandleCarCollision(Car* pOtherCar)
{
    // Entry EH state 5: six Fix16_Point locals up front
    Fix16_Point RelativeVelocity_1;
    Fix16_Point RelativeVelocity;
    Fix16_Point DirectionBetweenCoMs_Scaled;
    Fix16_Point OtherCoM;
    Fix16_Point ThisCoM;
    Fix16_Point ImpulseForce;

    Fix16 ThisCarMass = CalculateMass();
    pOtherCar->SetupCarPhysicsAndSpriteBindingWithTrailer();

    CarPhysics* OtherCarPhysics = pOtherCar->mPhysics;
    Car* pThisCar;

    OtherCarPhysics->SetCurrentCarInfoAndModelPhysics();
    RelativeVelocity_1 = OtherCarPhysics->ComputeRelativePointVelocity(&CollisionIntersectionPoint);
    SetCurrentCarInfoAndModelPhysics();
    RelativeVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint) - RelativeVelocity_1;
    ThisCoM = ComputeCombinedCenterOfMass();
    OtherCoM = OtherCarPhysics->ComputeCombinedCenterOfMass();
    sCollisionNormal = ThisCoM - CollisionIntersectionPoint;

    ImpulseForce = ComputeCollisionImpulse(ThisCarMass,
                                                      OtherCarPhysics->CalculateMass(),
                                                      RelativeVelocity,
                                                      sCollisionNormal,
                                                      CollisionIntersectionPoint,
                                                      ThisCoM,
                                                      OtherCoM,
                                                      GetEffectiveMomentOfInertia(),
                                                      OtherCarPhysics->GetEffectiveMomentOfInertia(),
                                                      kFP16Half_6FE0D0);

    // If it's falling at another car
    if (mSurfaceType == car_surface_type::air_surface_6 &&
        pOtherCar->mCarSprite->mZpos != mpCar->mCarSprite->mZpos)
    {
        mAirZVel = kFpPoint7 * (-mAirZVel);

        if (Fix16::Abs(mAirZVel) < kFP16One32nd)
        {
            mAirZVel = kFP16Zero_6FE20C;
        }

        DirectionBetweenCoMs_Scaled = (ThisCoM - OtherCoM).NormalizeSafe().DivideInl_55F9E0(10);

        // CanCrushCars and CanCollideOver with get_car_info_5AA3B0 and the driver check out of line
        // (IsDrivenByNonPlayer)
        pThisCar = mpCar;
        if (gGtx_0x106C->get_car_info_5AA3B0(pThisCar->mCarInfoIdx)->IsHeavy() &&
            (pThisCar->IsTank() || !pThisCar->IsDrivenByNonPlayer()) &&
            !gGtx_0x106C->get_car_info_5AA3B0(pOtherCar->mCarInfoIdx)->IsHeavy())
        {
            mpCar->SetCrushFlag();
        }
        else
        {
            ApplyImpulseAndDamage(DirectionBetweenCoMs_Scaled, 50);
            OtherCarPhysics->ApplyImpulseAndDamage(DirectionBetweenCoMs_Scaled.Negated(), 50);
        }

        sCollisionDamage = (ThisCarMass * Fix16::Abs(mZVel)) * 50;
    }
    else
    {
        sCollisionDamage = 0;
    }

    bool bGreatCollision;

    // CanCrushCars with get_car_info_5AA3B0 called out of line
    pThisCar = mpCar;
    if (gGtx_0x106C->get_car_info_5AA3B0(pThisCar->mCarInfoIdx)->IsHeavy() &&
        (pThisCar->IsTank() || !pThisCar->HasNonPlayerDriver()) &&
        !gGtx_0x106C->get_car_info_5AA3B0(pOtherCar->mCarInfoIdx)->IsHeavy() &&
        ImpulseForce.GetLength_all_out_of_line_abs_y_negate_2() > kFpPoint2_6FDFD8 && GetLinearSpeed() > kFpPoint01_6FE1C4)
    {
        bGreatCollision = true;
        mpCar->SetCrushFlag();
        if (pOtherCar->IsPoliceCar())
        {
            Ped* pDriver = mpCar->GetEffectiveDriver();
            if (pDriver && pDriver->is_player() && pDriver->mWantedPoints < cop_level_ped_enum::cop_1_stars)
            {
                pDriver->mWantedPoints = cop_level_ped_enum::cop_1_stars;
            }
        }
    }
    else
    {
        bGreatCollision = false;
        sCollisionDamage += ApplyImpactForcesAndDamage(CollisionIntersectionPoint, ImpulseForce, 50);
    }
    mpCar->AssignDriverBlameForExplosion(pOtherCar);

    // The other car's driver scores for the damage done to this car
    s16 damage = mpCar->ApplyImpactDamage(sCollisionDamage);
    if (damage > 200)
    {
        Ped* pDriver = pOtherCar->GetEffectiveDriver();
        if (pDriver && pDriver->is_player())
        {
            pDriver->mPlayer->mScores.AwardCarDamageScore(mpCar, damage);
        }
    }

    if (!bGreatCollision)
    {
        OtherCarPhysics->SetCurrentCarInfoAndModelPhysics();
        OtherCarPhysics->ApplyImpactForcesAndDamage(CollisionIntersectionPoint, ImpulseForce.Negated(), 50);

        // A train explodes the car it hits
        s16 damage_2;
        if (mpCar->IsTrainModel())
        {
            pOtherCar->HandleCarExplosion(explosion_type::item_19);
            damage_2 = 32000;
        }
        else
        {
            damage_2 = pOtherCar->ApplyImpactDamage(sCollisionDamage);
        }

        // This car's driver scores for the damage done to the other car
        if (damage_2 > 200)
        {
            Ped* pDriver = mpCar->GetEffectiveDriver();
            if (pDriver && pDriver->is_player())
            {
                pDriver->mPlayer->mScores.AwardCarDamageScore(pOtherCar, damage_2);
            }
        }
        SetCurrentCarInfoAndModelPhysics();
    }

    if (mLinvel1.GetLength_all_out_of_line_abs_y_negate_2() > FastCarMinVelocity && !mpCar->IsMaxDamage())
    {
        EmitImpactAtCollisionPoint(mCp3, RelativeVelocity.Negated());
    }

    if (sCollisionDamage > kFpPoint3_6FDFE4)
    {
        mpCar->TryDamageArea(sCollisionArea, sCollisionDamage.mValue);
        pOtherCar->TryDamageArea(sOtherCollisionArea, sCollisionDamage.mValue);
        if (sCollisionDamage > kFpPoint6_6FDFF0 && pOtherCar->IsPoliceCar())
        {
            Ped* pDriver = mpCar->GetEffectiveDriver();
            if (pDriver && pDriver->is_player() && pDriver->mWantedPoints < cop_level_ped_enum::cop_1_stars)
            {
                pDriver->mWantedPoints = cop_level_ped_enum::cop_1_stars;
            }
        }
    }

    if (mpCar->mAI)
    {
        mpCar->mAI->mBf24.bCollidedWithCar = true;
        mpCar->mAI->mCarInCollision = pOtherCar;
    }

    if (pOtherCar->mAI)
    {
        pOtherCar->mAI->mBf24.bCollidedWithCar = true;
        pOtherCar->mAI->mCarInCollision = mpCar;
    }
}

MATCH_FUNC(0x560680)
EXPORT Fix16 __stdcall DotProduct(const Fix16_Point& Vector1, const Fix16_Point& Vector2)
{
    return (Vector1.x * Vector2.x) + (Vector1.y * Vector2.y);
}

// https://decomp.me/scratch/tc7DX
MATCH_FUNC(0x5606c0)
void CarPhysics::HandleObjectCollision(Object* pObject, char_type damage_area)
{
    Fix16_Point RelativeVelocity;
    Fix16_Point Impulse;
    Fix16_Point tmp;
    Fix16_Point ObjPos = pObject->GetXY();
    Fix16_Point push_impulse;
    Fix16_Point CoM = ComputeCombinedCenterOfMass();
    Fix16 CarMass = CalculateMass();
    Fix16 ObjMass;
    sCollisionNormal = CoM - CollisionIntersectionPoint;
    if (pObject->CanBePushed())
    {
        ObjMass = pObject->GetMass();
        tmp = pObject->GetSpeedVector_52AE90();
        RelativeVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint) - tmp;

        Impulse = ComputeCollisionImpulse(CarMass,
                                                     ObjMass,
                                                     RelativeVelocity,
                                                     sCollisionNormal,
                                                     CollisionIntersectionPoint,
                                                     CoM,
                                                     ObjPos,
                                                     GetEffectiveMomentOfInertia(),
                                                     kFP16One_6FE0F4,
                                                     kFP16One_6FE0D4);

        // Obj Reaction impulse
        tmp = Impulse.Negated().DivideInl_442CB0(ObjMass);
        pObject->SetMovementVectorWithRandomState(tmp);
    }
    else
    {
        // infinite mass
        RelativeVelocity = ComputeRelativePointVelocity(&CollisionIntersectionPoint);
        Impulse = ComputeCollisionImpulse(CarMass,
                                                     kFP16MinusOne,
                                                     RelativeVelocity,
                                                     sCollisionNormal,
                                                     CollisionIntersectionPoint,
                                                     CoM,
                                                     ObjPos,
                                                     GetEffectiveMomentOfInertia(),
                                                     kFP16One_6FE0F4,
                                                     kFP16Quarter_6FDFB8);
    }

    if (mSurfaceType == car_surface_type::air_surface_6 && pObject->mSprite->mZpos != mpCar->mCarSprite->mZpos)
    {
        mAirZVel = kFpPoint7 * (-mAirZVel);
        if (Fix16::Abs(mAirZVel) < kFP16One32nd)
        {
            mAirZVel = kFP16Zero_6FE20C;
        }
        push_impulse = (CoM - ObjPos).NormalizeSafe().DivideInl_55F9E0(10);
        ApplyImpulseAndDamage(push_impulse, 50);
        if (pObject->CanBePushed())
        {
            pObject->SetMovementVectorWithRandomState(push_impulse.Negated());
        }
        sCollisionDamage = (CarMass * Fix16::Abs_negate_out_of_line(mZVel)) * 50;
    }
    else
    {
        sCollisionDamage = Fix16(0);
    }

    sCollisionDamage += ApplyImpactForcesAndDamage(CollisionIntersectionPoint, Impulse, 15);
    mpCar->ApplyImpactDamage(sCollisionDamage);
    pObject->HandleImpact(mpCar->mCarSprite);

    if (sCollisionDamage > kFpPoint3_6FDFE4)
    {
        if (!mpCar->IsMaxDamage())
        {
            EmitImpactAtCollisionPoint(mCp3, RelativeVelocity.Negated());
        }
        mpCar->TryDamageArea(damage_area, sCollisionDamage.mValue);
    }
}

MATCH_FUNC(0x560b40)
void CarPhysics::ProcessPedImpact(Char* pChar, u8 overlap_count)
{
    Fix16_Point impact_dir; // a local with a destructor: the original constructs 4 points up front (EH state 3)
    Fix16_Point impulse;
    Fix16_Point relativePointVel;
    Fix16_Point sprite_xy(pChar->get_sprite_xpos(), pChar->get_sprite_ypos());

    Fix16_Point combinedCentreOfmass = ComputeCombinedCenterOfMass();
    relativePointVel = ComputeRelativePointVelocity(&CollisionIntersectionPoint);

    sCollisionNormal = combinedCentreOfmass - CollisionIntersectionPoint;

    impulse = ComputeCollisionImpulse(CalculateMass(),
                                                       kFP16Half_6FE2F8,
                                                       relativePointVel,
                                                       sCollisionNormal,
                                                       CollisionIntersectionPoint,
                                                       combinedCentreOfmass,
                                                       sprite_xy,
                                                       GetEffectiveMomentOfInertia(),
                                                       kFP16One_6FE070,
                                                       kFP16One_6FE3DC);

    u8 bAirborneOrNoOverlap;
    if (mSurfaceType == car_surface_type::air_surface_6 && pChar->get_sprite_zpos() != mpCar->mCarSprite->mZpos ||
        overlap_count == 0)
    {
        bAirborneOrNoOverlap = 1;
    }
    else
    {
        bAirborneOrNoOverlap = 0;
    }

    sCollisionDamage = impulse.GetLength_41E260();

    Car* pCar = this->mpCar;

    // Nothrow alias: no EH state around the Negated temporary
    impact_dir = impulse.Negated().DivideInl_442CB0(kFP16Half_6FE2F8);

    Ped* pCarDriver = mpCar->mDriver;
    if (pCarDriver)
    {
        pChar->mpPed->mKillerId = pCarDriver->mId;
        pChar->mpPed->mKillerIdTimer = 50;

        Ped* pPed = pChar->mpPed;
        if (pPed->GetStolenCar() == this->mpCar)
        {
            pPed->mDeathCause = ped_death_cause::run_over_by_stolen_car_3;
        }
        else
        {
            pPed->mDeathCause = ped_death_cause::run_over_1;
        }
    }
    else
    {
        Trailer* pTrailer = mpCar->mpTrailer;
        if (pTrailer)
        {
            // IsTrailerCar ?
            Car* pCarOnTrailer = pTrailer->mTrailerCar;
            if (pCarOnTrailer)
            {
                if (pCarOnTrailer == mpCar)
                {
                    Car* pTruckCab = pTrailer->mTruckCab;
                    if (pTruckCab)
                    {
                        pCar = pTruckCab;
                        Ped* pTruckCabDriver = pTruckCab->mDriver;
                        if (pTruckCabDriver)
                        {
                            pChar->mpPed->mKillerId = pTruckCabDriver->mId;
                            pChar->mpPed->mKillerIdTimer = 50;

                            Ped* pPed = pChar->mpPed;
                            if (pPed->GetStolenCar() == this->mpCar->mpTrailer->mTruckCab)
                            {
                                pPed->mDeathCause = ped_death_cause::run_over_by_stolen_car_3;
                            }
                            else
                            {
                                pPed->mDeathCause = ped_death_cause::run_over_1;
                            }
                        }
                    }
                }
            }
        }
    }

    pChar->HandleCarImpact(pCar, bAirborneOrNoOverlap, impact_dir.x, impact_dir.y);
}

MATCH_FUNC(0x560eb0)
void CarPhysics::UpdateLinearAndAngularAccel()
{
    mLinearAccel = mForceAccum.Divide(CarPhysics::CalculateMass());
    mAngularAccel = -mTorqueAccum / CarPhysics::GetEffectiveMomentOfInertia();
}

MATCH_FUNC(0x560f20)
void CarPhysics::ApplyMovementStep(Fix16 time_scale)
{
    Fix16 dt = (sRemainingTimeStep * time_scale);

    if (dt != kFP16Zero_6FE20C)
    {
        // 9.6f: Ang16::Fix16_To_Ang16, written out so Normalize is inlined on a register
        Ang16 delta_theta(static_cast<s16>((dt * mAngVelRad).GetRaw() / 71), static_cast<u8>(0));
        this->mTheta = Ang16(mTheta.rValue + delta_theta.rValue).Normalized();

        // 9.6f: Fix16_Point operator+= (0x40F680, inlined)
        this->mCm1 += (mLinvel1 * dt);

        UpdateCp1FromCm1();

        this->mCp3 += (sF70 * time_scale);

        UpdateTrailerAlignment();

        if (mpCar->IsTrainModel())
        {
            UpdateRailwayZPositionThunk(time_scale);
            UpdateCarAndTrailerSpriteFromPhysics();
            return;
        }
        SyncZWithTrailer(time_scale);
    }

    UpdateCarAndTrailerSpriteFromPhysics();
}

MATCH_FUNC(0x5610b0)
void CarPhysics::IntegrateAndClampVelocities()
{
    // Integrate linear and angular velocity
    this->mLinvel1 += this->mLinearAccel;
    this->mAngVelRad += this->mAngularAccel;

    ResetForceAccumulators();

    mLinvel1.ApplyDeadZone();
    mAngVelRad = mAngVelRad.ApplyDeadZone_482730(mAngVelRad);
}

MATCH_FUNC(0x561130)
Fix16_Point CarPhysics::ComputeRelativePointVelocity(Fix16_Point* pPoint)
{
    Fix16_Point rel_pos = (*pPoint - mCp1);
    rel_pos.RotateByAngle(-mTheta);
    rel_pos -= gCarInfo_2C->mCenterOfMassOffset;
    rel_pos.RotateByAngle(Ang16::Fix16_To_Ang16(mAngVelRad) + mTheta);
    rel_pos += (mCm1 + mLinvel1);
    return rel_pos - *pPoint;
}

MATCH_FUNC(0x561350)
Fix16_Point CarPhysics::GetPointVelocity(Fix16_Point* pPoint)
{
    SetCarInfoGlobal();
    return ComputeRelativePointVelocity(pPoint);
}

MATCH_FUNC(0x561380)
Fix16_Point CarPhysics::ComputePointVelocity(Fix16_Point& point)
{
    // Entry EH state 3: three points up front plus local_pos in the block below. The block ends
    // after the second rotation, so later temporaries reuse local_pos's slot.
    Fix16_Point old_pos;
    Fix16_Point new_pos;
    Fix16_Point unused;

    {
        Fix16_Point local_pos;
        local_pos = point - gCarInfo_2C->mCenterOfMassOffset;

        old_pos = local_pos;
        old_pos.RotateByAngle(mTheta);
        old_pos += mCm1;

        new_pos = local_pos;
        // The angle sum and the Fix16_To_Ang16 result are temporaries: they share slots with the
        // first rotation's sin/cos (cos sits in the dead `point` parameter slot). Past the inline
        // budget both normalizing Ang16 ctors are called out of line (0x409300).
        new_pos.RotateByAngle(mTheta + Ang16::Fix16_To_Ang16(mAngVelRad));
    }

    new_pos += mCm1 + mLinvel1;
    return new_pos - old_pos;
}

// https://decomp.me/scratch/5Hj13
// 9.6f 0x4A0D40
MATCH_FUNC(0x5615d0)
Fix16 CarPhysics::ApplyDriveForce(Fix16_Point& wheel_point, Ang16 angle, Fix16_Point& friction, Fix16 slip_speed)
{
    Fix16_Point force;
    Fix16_Point local_vel;
    Fix16 y_abs;
    Fix16 x_abs;

    if (mIsCoasting)
    {
        force.SetXY(kFP16Zero_6FE20C, kFP16Zero_6FE20C);
    }
    else
    {
        local_vel = ComputePointVelocity(wheel_point);
        local_vel.RotateByAngle(-angle);

        force.x = (local_vel.x * -friction.x);
        force.y = (local_vel.y * -friction.y);
    }

    y_abs = Fix16::Abs(local_vel.y);
    x_abs = Fix16::Abs(force.x);

    if (mState == 2 && !mpCar->IsTank())
    {
        if ((y_abs > kFpPoint01_6FE15C) || (Fix16::Abs(slip_speed) < kFpPoint05))
        {
            if (mIsHandBrakeOn)
            {
                if ((mDriveWheelsLockedQ && y_abs > kFP16Zero_6FE20C || y_abs >= kFpPoint1) &&
                    mDriveWheelsLockedQ < 2)
                {
                    mDriveWheelsLockedQ = 2;
                }
            }
        }
        else
        {
            mDriveWheelsLockedQ = (((Fix16::Abs(slip_speed)) / kFpPoint05) * 8).ToInt();
        }
    }

    force.y += slip_speed;
    force.RotateByAngle(angle);
    ApplyForceAtPoint(&wheel_point, &force, 1);

    return x_abs;
}

MATCH_FUNC(0x561940)
bool CarPhysics::IsTurboActive()
{
    return gCarInfo_48->mTurbo && this->mGasPedal >= kFpThree;
}

// https://decomp.me/scratch/0MzjM
// 9.6f 0x4A0F30
MATCH_FUNC(0x561970)
Fix16 CarPhysics::ComputeEngineTorque()
{
    Fix16 torque;
    if (mpCar->IsEngineOn() && mSurfaceType != car_surface_type::remapped_water_tile_7 && mSurfaceType != car_surface_type::water_surface_8)
    {
        if (this->mState == 2)
        {
            // Assigned, not initialised: the slot is then free for the temporaries of each branch
            Fix16 vel_len;
            vel_len = mLinvel1.GetLength_ool_abs_mul();

            if (mIsBackwardGasOn)
            {
                if (vel_len == kFP16Zero_6FE20C && this->mIsHandBrakeOn)
                {
                    torque = kFP16Zero_6FE20C;
                }
                else
                {
                    torque = -ComputeUndamagedThrottleTorque_49E8E0_ool() * gCarInfo_48->mGear1Multiplier;
                }
            }
            else if (mIsForwardGasOn)
            {
                if (vel_len == kFP16Zero_6FE20C && this->mIsHandBrakeOn)
                {
                    torque = kFP16Zero_6FE20C;
                }
                else if (vel_len > gCarInfo_48->mGear3Speed)
                {
                    // Gear 3
                    torque = inline_ComputeTorqueFromThrottle_561DD0_ool() * gCarInfo_48->mGear3Multiplier;
                }
                else if (vel_len > gCarInfo_48->mGear2Speed)
                {
                    // Gear 2
                    torque = inline_ComputeTorqueFromThrottle_561DD0_ool() * gCarInfo_48->mGear2Multiplier;
                }
                else
                {
                    // Gear 1. The original inlines this multiply, ours goes out of line (inline budget), so it's written out
                    torque.mValue = static_cast<s32>((ComputeUndamagedThrottleTorque_49E8E0_ool().mValue * static_cast<__int64>(gCarInfo_48->mGear1Multiplier.mValue)) >> 14);
                }
            }
            else
            {
                torque = kFP16Zero_6FE20C;
            }
        }
        else if (this->mIsForwardGasOn)
        {
            torque = ComputeTorqueFromThrottle();
        }
        else if (this->mIsBackwardGasOn)
        {
            torque = -ComputeTorqueFromThrottle();
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
Fix16 CarPhysics::ComputeTorqueFromThrottle()
{
    if (IsTurboActive())
    {
        return gCarInfo_2C->mHalfThrust +
            ((mGasPedal * ((gDamageSpeedFactor * gCarInfo_2C->mFithThrust)))) * 2;
    }
    else
    {
        return gCarInfo_2C->mHalfThrust +
            ((mGasPedal * ((gDamageSpeedFactor * gCarInfo_2C->mFithThrust))));
    }
}

// https://decomp.me/scratch/46zAM
MATCH_FUNC(0x561e50)
Fix16 CarPhysics::CalculateFrontWheelForce()
{
    Fix16_Point point(Fix16(0), gCarInfo_2C->mFrontWheelOffset);
    Fix16_Point point2;

    if (CarPhysics::IsInAir())
    {
        return kFP16Zero_6FE20C;
    }
    else
    {
        Fix16 grip_scale;
        Fix16 front_torque = CarPhysics::ComputeEngineTorque() * gCarInfo_48->mFrontDriveBias;

        if (mTurnDirection != car_turn_direction::none_0)
        {
            grip_scale = kFpOne_6FE210 + gCarInfo_48->mTurnIn;
        }
        else
        {
            grip_scale = kFpOne_6FE210;
        }

        // The declaration order picks the registers of the default case's loads
        Fix16 force_x;
        Fix16 force_y_base;
        Fix16 brake_force;
        Fix16 pointing_ang_rad;

        switch (mOilSpinDir)
        {
            case 0:
                force_x = kFP16One_6FE3D0 * grip_scale;
                force_y_base = kFpPoint139;
                brake_force = sBrakeForce * kFpPoint8_6FE320;
                pointing_ang_rad = this->mPointingAngRad;
                break;
            case 1:
            case 2:
                if (front_torque.mValue == kFP16Zero_6FE20C.mValue)
                {
                    // Copying the zero from brake_force keeps kFP16Zero_6FE20C out of a register
                    brake_force = kFP16Zero_6FE20C;
                    force_x = brake_force;
                    force_y_base = brake_force;
                    pointing_ang_rad = this->mPointingAngRad;
                }
                else
                {
                    force_x = kFP16One_6FE3D0 * grip_scale;
                    force_y_base = kFpPoint139;
                    if (mOilSpinDir == 1)
                    {
                        pointing_ang_rad = this->mPointingAngRad + kAngFix16OneDegree_6FE3C4 * 30;
                    }
                    else
                    {
                        pointing_ang_rad = this->mPointingAngRad + kAngFix16OneDegree_6FE3C4 * -30;
                    }
                    brake_force = kFP16Zero_6FE20C;
                }
                break;
            default:
                // The original leaves all four values uninitialised here
                break;
        }

        // 9.6f: Fix16_Point::SetXY
        point2.x = force_x;
        point2.y = force_y_base + brake_force;

        // 9.6f: theta + Fix16_To_Ang16(pointing_ang_rad). Written with the Ang16 ctor directly,
        // since Normalize is too deep to inline through those helpers
        Ang16 rotation(pointing_ang_rad.GetRaw() / 71, 0);

        return CarPhysics::ApplyDriveForce(point, Ang16(mTheta.rValue + rotation.rValue, 0), point2, front_torque);
    }
}

MATCH_FUNC(0x5620d0)
Fix16 CarPhysics::CalculateRearWheelForce()
{
    Fix16_Point wheel_point(Fix16(0), gCarInfo_2C->mRearWheelOffset);
    Fix16_Point rear_force;
    Fix16 v;

    if (IsInAir())
    {
        return kFP16Zero_6FE20C;
    }

    Fix16& rear_drive_bias = gCarInfo_2C->mRearDriveBias;
    Fix16 engine_torque = ComputeEngineTorque() * rear_drive_bias;

    Fix16 grip_scale;
    if (mTurnDirection != car_turn_direction::none_0)
    {
        grip_scale = kFpOne_6FE210 - gCarInfo_48->mTurnIn;
    }
    else
    {
        grip_scale = kFpOne_6FE210;
    }

    Fix16 new_x;
    Fix16 brake_force1;
    Fix16 brake_force2;
    Fix16 brake_force3;
    Fix16 pointing_ang_rad;

    if (mOilSpinDir)
    {
        if (mOilSpinDir > 0 && mOilSpinDir <= 2)
        {
            brake_force2 = kFP16Zero_6FE20C;
            if (engine_torque.mValue == kFP16Zero_6FE20C.mValue)
            {
                new_x = kFP16Zero_6FE20C;
                brake_force1 = kFP16Zero_6FE20C;
                brake_force3 = kFP16Zero_6FE20C;
                pointing_ang_rad = this->mPointingAngRad;
                this->mHandBrakeForce = 0;
            }
            else
            {
                new_x = kFP16One_6FE3D0 * grip_scale;
                brake_force1 = kFpPoint139;
                if (mOilSpinDir == 1)
                {
                    pointing_ang_rad = this->mPointingAngRad - kAngFix16OneDegree_6FE3C4 * 30;
                }
                else
                {
                    pointing_ang_rad = this->mPointingAngRad - kAngFix16OneDegree_6FE3C4 * (-30);
                }
                brake_force3 = kFP16Zero_6FE20C;
                this->mHandBrakeForce = 0;
            }
        }
    }
    else
    {
        brake_force1 = kFpPoint139;
        brake_force2 = sBrakeForce * kFpPoint2;

        if (mIsHandBrakeOn)
        {
            if (mHandBrakeForce != '\x80')
            {
                mHandBrakeForce++;
            }
            new_x = kFP16One_6FE3D0 * grip_scale * gCarInfo_48->mHandbrakeSlideValue;
            brake_force3 = gCarInfo_48->mBrakeFriction * static_cast<s32>(static_cast<u8>(mHandBrakeForce)) / 128;
        }
        else
        {
            this->mHandBrakeForce = 0;
            new_x = kFP16One_6FE3D0 * grip_scale;
            brake_force3 = kFP16Zero_6FE20C;
        }
        pointing_ang_rad = this->mPointingAngRad;
    }

    rear_force.SetXY(new_x, brake_force1 + brake_force2 + brake_force3);
    rear_force.MultiplyByFix16_inline_5620D0(gCarInfo_48->mRearEndStability);

    // The function is at VC6's inline budget: Fix16_To_Ang16 / Ang16 - Ang16 here push the Ang16
    // constructors and y * y out of line, so the angle is built and normalised (Normalize_406C20) by hand.
    v = kFpPoint6 - mLinvel1.GetLength_inline_5620D0();
    v = v / (kFpThreePoint8 * kFpPoint6);
    {
        Ang16 ang((pointing_ang_rad * v).GetRaw() / 71);
        ang.Normalize_406C20();
        Ang16 steer(mTheta.rValue - ang.rValue);
        steer.Normalize_406C20();
        return ApplyDriveForce(wheel_point, steer, rear_force, engine_torque);
    }
}

// Defined here (its address range), not in sprite.cpp: with the body visible in sprite.cpp
// VC6 knows it cannot throw and drops the EH state updates around its calls in
// Sprite::FindCollisionIntersectionPoint
MATCH_FUNC(0x562450)
Fix16_Point Sprite::GetBoundingBoxCorner(s32 idx)
{
    return Fix16_Point(mpCollisionShape->mCorners[idx].x, mpCollisionShape->mCorners[idx].y);
}

MATCH_FUNC(0x562480)
void CarPhysics::ApplyThrottleInput()
{
    if (this->mIsForwardGasOn)
    {
        this->mGasPedal += kFpQuarter_6FE3A0;
        if (this->mGasPedal > kFpFive_6FDEFC)
        {
            this->mGasPedal = kFpFive_6FDEFC;
        }
    }
    else if (this->mIsBackwardGasOn)
    {
        this->mGasPedal += kFpQuarter_6FE3A0;
        if (this->mGasPedal > kFpFive)
        {
            this->mGasPedal = kFpFive;
        }
    }
    else
    {
        this->mGasPedal -= kFpQuarter_6FE364;
        if (this->mGasPedal < kFpZero)
        {
            this->mGasPedal = kFpZero;
        }
    }
}

MATCH_FUNC(0x5624f0)
void CarPhysics::ApplyBrakePhysics()
{
    if (!mIsFootBrakeOn || mSurfaceType == car_surface_type::remapped_water_tile_7 || mSurfaceType == car_surface_type::water_surface_8)
    {
        mBrakePressure = kBrakePressureStart;
        sBrakeForce = kFP16Zero_6FE20C; // final value used in skid calcs
    }
    else
    {
        mBrakePressure += kBrakePressureStep;
        if (mBrakePressure > kBrakePressureMax)
        {
            mBrakePressure = kBrakePressureMax; // 0x4000 fp16
        }
        sBrakeForce = mBrakePressure * gCarInfo_48->mBrakeFriction;
    }
}

// https://decomp.me/scratch/vdIqi
// Fix16::operator*= with the product in a temporary (StabilizeVelocityAtSpeed)
static inline void MultiplyAssign_ProductTemp(Fix16& value, const Fix16& factor)
{
    __int64 product = static_cast<__int64>(value.mValue) * factor.mValue;
    value.mValue = static_cast<s32>(product >> 14);
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
void CarPhysics::UpdateSteeringAngle()
{
    if (mpCar->mPedType != ped_type::player_2)
    {
        mPointingAngRad = CarPhysics::GetTrailerAwareTurnRatio() * mTurnDirection;
    }
    else
    {
        Fix16 speed_factor = kFpPoint6 - mLinvel1.GetLength_out_of_line_x_squared();
        if (speed_factor.mValue < kFpQuarter_6FE374.mValue)
        {
            speed_factor = kFpQuarter_6FE374;
        }
        mPointingAngRad = GetScaledTurnRatio(speed_factor / (kFpPoint6 * kFpPoint8), mTurnDirection);
    }
}

MATCH_FUNC(0x5626a0)
bool CarPhysics::IsGasPedalPressedEnough()
{
    Fix16 min_pressure = MinGasPedalPressure();
    return !!(mGasPedal >= min_pressure);
}

MATCH_FUNC(0x5626c0)
Fix16 CarPhysics::MinGasPedalPressure()
{
    if (mpCar->mPedType == ped_type::player_2)
    {
        return kFP16Two;
    }
    else
    {
        return kFP16Zero_6FE20C;
    }
}

MATCH_FUNC(0x5626f0)
void CarPhysics::ApplyArrowSteerAssist()
{
    Fix16 theta_fp = Ang16::Ang16_to_Fix16(mTheta);
    gArrowSteerAngVel = kFP16Zero_6FE20C;
    CarAI* pAi = this->mpCar->mAI;
    if ((!pAi || pAi->mBf24.bArrowSteerAssist) && this->mPointingAngRad == kFP16Zero_6FE20C)
    {
        if (IsGasPedalPressedEnough())
        {
            if (IsVelocityAlignedWithHeading() && !this->mLinvel1.IsNull() && !this->mOilSpinDir)
            {
                gmp_block_info* pBlock = gMap_0x370->GetBlock(this->mCp1.x.ToInt(),
                                                                             this->mCp1.y.ToInt(),
                                                                             (this->mCp3 - kFpOne_6FE210).ToInt());
                if (pBlock)
                {
                    if (GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD)
                    {
                        if ((pBlock->mArrows & block_arrow::horizontal_mask) != 0)
                        {
                            if (theta_fp > kAngFix16QuarterCircle - kAngFix16TwentyFiveDegrees && theta_fp < kAngFix16TwentyFiveDegrees + kAngFix16QuarterCircle)
                            {
                                gArrowSteerAngVel = kAngFix16QuarterCircle - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > kAngFix16ThreeQuarterCircle - kAngFix16TwentyFiveDegrees && theta_fp < kAngFix16ThreeQuarterCircle + kAngFix16TwentyFiveDegrees)
                                {
                                    gArrowSteerAngVel = kAngFix16ThreeQuarterCircle - theta_fp;
                                }
                            }
                        }
                        else if ((pBlock->mArrows & block_arrow::vertical_mask) != 0)
                        {
                            if (theta_fp > kAngFix16HalfCircle - kAngFix16TwentyFiveDegrees && theta_fp < kAngFix16HalfCircle + kAngFix16TwentyFiveDegrees)
                            {
                                gArrowSteerAngVel = kAngFix16HalfCircle - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > kAngFix16FullCircle - kAngFix16TwentyFiveDegrees)
                                {

                                    gArrowSteerAngVel = kAngFix16FullCircle - theta_fp;
                                }
                                else
                                {
                                    if (theta_fp < kAngFix16TwentyFiveDegrees)
                                    {
                                        gArrowSteerAngVel = -theta_fp;
                                    }
                                }
                            }
                        }

                        if (gArrowSteerAngVel != kFP16Zero_6FE20C)
                        {
                            theta_fp = mpCar->GetMaxTurnRate();
                            if (gArrowSteerAngVel > kFP16Zero_6FE20C)
                            {
                                if (gArrowSteerAngVel > theta_fp)
                                {
                                    gArrowSteerAngVel = theta_fp;
                                }
                            }
                            else
                            {
                                if (gArrowSteerAngVel < -theta_fp)
                                {
                                    gArrowSteerAngVel = -theta_fp;
                                }
                            }

                            if (bDo_show_instruments)
                            {
                                Ped* pDriver = this->mpCar->mDriver;
                                if (pDriver)
                                {
                                    Player* pPlayer = pDriver->mPlayer;
                                    if (pPlayer)
                                    {
                                        if (pPlayer->IsUser())
                                        {
                                            gHud_2B00->mTexts.DisplayText(L"snap", 0, 64, gDebugFont, 1);
                                        }
                                    }
                                }
                            }

                            this->mAngVelRad += gArrowSteerAngVel;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x562910)
void CarPhysics::StabilizeVelocityAtSpeed()
{
    if (CarPhysics::IsInAir())
    {
        if (Fix16::Abs(mLinvel1.x) <= kFP16One128th &&
            Fix16::Abs(mLinvel1.y) <= kFP16One128th)
        {
            mLinvel1.MultiplyByFix16(kFpPoint95);
        }
        else
        {
            mLinvel1.RotateByAngle(-mTheta);
            mLinvel1.x *= kFpPoint95;
            if (mpCar->mpTrailer)
            {
                mLinvel1.y *= kFpPoint985_6FE240;
            }
            else
            {
                mLinvel1.y *= kFpPoint985_6FE330;
            }
            mLinvel1.RotateByAngle(mTheta);
            mAngVelRad = mAngVelRad * kFpPoint94;
        }
    }
    else
    {
        if (Fix16::Abs(mLinvel1.x) <= kFP16One128th &&
            Fix16::Abs(mLinvel1.y) <= kFP16One128th)
        {
            mLinvel1.MultiplyByFix16(kFpPoint9);
        }
        else
        {
            mLinvel1.RotateByAngle(-mTheta);
            // `x *=` gives the factor in eax (imull x); the product-temp form loads x into eax first
            MultiplyAssign_ProductTemp(mLinvel1.x, kFpPoint9);
            // 9.6f has `*=` (0x41E0D0) in both branches. 10.5 inlines it here and calls the
            // out-of-line copy (0x562430) in the else branch, past the inline budget. Only this
            // helper gives the original operand order (y in eax) and budget use.
            if (mpCar->mpTrailer)
            {
                MultiplyAssign_ProductTemp(mLinvel1.y, kFpPoint97_6FDFBC);
            }
            else
            {
                mLinvel1.y *= kFpPoint97_6FE0FC;
            }
            mLinvel1.RotateByAngle(mTheta);
            MultiplyAssign_inline_408680(mAngVelRad, kFpPoint88);
        }
    }
}

// Out-of-line copy of Fix16_Point::RotateByAngle
MATCH_FUNC(0x562c20)
void Fix16_Point::RotateVelocity(const Ang16& angle)
{
    const Fix16 sin = Ang16::sine(angle);
    const Fix16 cos = Ang16::cosine(angle);

    const Fix16 x_old = x;

    x = (sin * y) + (cos * x);
    y = (cos * y) + ((-x_old) * sin);
}

// https://decomp.me/scratch/0X4pK
// 9.6f 0x4A1B20
MATCH_FUNC(0x562d00)
void CarPhysics::EnforceGearSensitiveMaxSpeed()
{

    Fix16_Point polar;
    if (!IsInAir())
    {
        Fix16 radius;
        if (IsVelocityAlignedWithHeading())
        {
            radius = gCarInfo_48->mMaxSpeed;
        }
        else
        {
            radius = gCarInfo_48->mGear2Speed;
        }

        if (mLinvel1.GetLength_41E260() > radius)
        {
            polar.FromPolar_41E210(radius, mLinvel1.GetAngle());
            mLinvel1.ClampTowardsZero(polar);
        }
    }
}

MATCH_FUNC(0x562eb0)
void CarPhysics::SetModelPhysicsGlobal()
{
    gCarInfo_48 = gCarInfo_808->GetModelPhysicsFromIdx(mpCar->GetCarModelForPhysics());
}

MATCH_FUNC(0x562ed0)
void CarPhysics::SetCarInfoGlobal()
{
    CarInfo_2C* pInfo = gCarInfo_808->GetInfoAtIdx(mpCar->GetCarModelForPhysics());
    gCarInfo_2C = pInfo;
}

// https://decomp.me/scratch/Uxers
// 9.6f 0x40F760: negates a point in place (inlined in 10.5)
static inline void NegateInPlace(Fix16_Point& p)
{
    p.x = -p.x;
    p.y = -p.y;
}

MATCH_FUNC(0x562ef0)
void CarPhysics::SetCurrentCarInfoAndModelPhysics()
{
    u8 info_idx_remapped = mpCar->GetCarModelForPhysics();
    gCarInfo_2C = gCarInfo_808->GetInfoAtIdx(info_idx_remapped);
    gCarInfo_48 = gCarInfo_808->GetModelPhysicsFromIdx(info_idx_remapped);
}

MATCH_FUNC(0x562f30)
void CarPhysics::ApplyInputsAndIntegratePhysics()
{
    gDamageSpeedFactor = mpCar->GetDamageFactorOnSpeed();
    ApplyThrottleInput();
    ApplyBrakePhysics();
    mFrontSkid = CalculateFrontWheelForce();
    mRearSkid = CalculateRearWheelForce();
    HandleGravityOnSlope();
    UpdateLinearAndAngularAccel();
    IntegrateAndClampVelocities();
}

MATCH_FUNC(0x562fa0)
bool CarPhysics::UpdateLastMovementTimer()
{
    if (IsNotMoving())
    {
        if (mTimerSinceLastMove < 255)
        {
            mTimerSinceLastMove++;
        }
        if (mTimerSinceLastMove >= 20u)
        {
            return true;
        }
    }
    else
    {
        mTimerSinceLastMove = 0;
    }
    return false;
}

MATCH_FUNC(0x562fe0)
bool CarPhysics::ProcessCarPhysicsStateMachine()
{
    char_type bCollided;

    SetCurrentCarInfoAndModelPhysics();
    char_type car_model = mpCar->GetCarModelForPhysics();
    if (car_model != this->mCarModel)
    {
        this->mCarModel = car_model;
        UpdateReferencePoint();
    }

    this->mFrontSkid = kFP16Zero_6FE20C;
    this->mRearSkid = kFP16Zero_6FE20C;

    switch (mState)
    {
        case 1:
            ResetPoint6FDF50();
            bCollided = CheckAndHandleCarAndTrailerCollisions();
            ApplyForcedSteering();
            EnforceTrailerControlLimits();
            UpdateSteeringAngle();
            ApplyInputsAndIntegratePhysics();
            StabilizeVelocityAtSpeed();
            EnforceGearSensitiveMaxSpeed();
            Field40Add();
            ApplyArrowSteerAssist();
            ScarePedsOnDrivingFast();
            bCollided = ProcessCollisionAndClampVelocity() | bCollided;
            bCollided = CheckPendingCollision() | bCollided;
            DoSkidmarks();
            Field40Subtract();
            Field74Subtract();
            break;
        case 2:
            ResetPoint6FDF50();
            bCollided = CheckAndHandleCarAndTrailerCollisions();
            EnforceTrailerControlLimits();
            UpdateSteeringAngle();
            ApplyInputsAndIntegratePhysics();
            StabilizeVelocityAtSpeed();
            EnforceGearSensitiveMaxSpeed();
            Field40Add();
            ApplyArrowSteerAssist();
            ScarePedsOnDrivingFast();
            bCollided = ProcessCollisionAndClampVelocity() | bCollided;
            bCollided = CheckPendingCollision() | bCollided;
            DoSkidmarks();
            Field40Subtract();
            Field74Subtract();
            break;
        case 3:
            bCollided = CheckAndHandleCarAndTrailerCollisions();
            ApplyInputsAndIntegratePhysics();
            ScarePedsOnDrivingFast();
            CheckPendingCollision();
            bCollided = ProcessCollisionAndClampVelocity() | bCollided;
            bCollided = CheckPendingCollision() | bCollided;
            DoSkidmarks();
            if (this->mpCar->IsDrivenByPlayer())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 0:
            bCollided = CheckAndHandleCarAndTrailerCollisions();
            ScarePedsOnDrivingFast();
            bCollided = ProcessCollisionAndClampVelocity() | bCollided;
            bCollided = CheckPendingCollision() | bCollided;
            DoSkidmarks();
            if (this->mpCar->IsDrivenByPlayer())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 4:
            bCollided = CheckAndHandleCarAndTrailerCollisions();
            StepPhysics();
            ProcessGroundCollisionAndEmitImpactParticles();
            DoSkidmarks();
            if (this->mpCar->IsDrivenByPlayer())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        default:
            break;
    }

    return (UpdateLastMovementTimer() && !bCollided && mSurfaceType != car_surface_type::remapped_water_tile_7 && mSurfaceType != car_surface_type::water_surface_8 &&
            mSurfaceType != car_surface_type::air_surface_6);
}

// 9.6f 0x49ED00
MATCH_FUNC(0x563280)
void CarPhysics::UpdateCp1FromCm1()
{
    Fix16_Point point = gCarInfo_2C->mCenterOfMassOffset;
    NegateInPlace(point);

    point.RotateByAngle(mTheta);

    mCp1 = mCm1 + point;
}

MATCH_FUNC(0x563350)
void CarPhysics::UpdateCenterOfMassPoint()
{
    const CarInfo_2C* info = gCarInfo_808->GetInfoAtIdx(mpCar->GetCarModelForPhysics());

    Fix16_Point point(info->mCenterOfMassOffset.x, info->mCenterOfMassOffset.y);

    // RotateByAngle, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine(mTheta);
    Fix16 cos = Ang16::cosine(mTheta);
    Fix16 x_old = point.x;
    point.x = (point.x * cos) + (point.y * sin);
    point.y = x_old.Negated().Multiply(sin).Add(point.y * cos);

    mCm1 = mCp1 + point;
}

// 0x49EDC0 9.6f
// https://decomp.me/scratch/xDPiP
MATCH_FUNC(0x563460)
void CarPhysics::UpdateReferencePoint()
{
    Fix16_Point point = gCarInfo_2C->mCenterOfMassOffset;

    // RotateByAngle, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine(mTheta);
    Fix16 cos = Ang16::cosine(mTheta);
    Fix16 x_old = point.x;
    point.x = (point.x * cos) + (point.y * sin);
    point.y = x_old.Negated().Multiply(sin).Add(point.y * cos);

    mCm1 = mCp1 + point;
}

MATCH_FUNC(0x563560)
void CarPhysics::SetSprite(Sprite* pSprite)
{
    mCp1.x = pSprite->mXy.x;
    mCp1.y = pSprite->mXy.y;
    mCp3 = pSprite->mZpos;
    mTheta = pSprite->mRotation;
    mPointingAngRad = 0;
    UpdateCenterOfMassPoint();
}

MATCH_FUNC(0x563590)
void CarPhysics::SnapVelocityToSpriteDirection(Sprite* pSprt)
{
    mLinvel1.SetFromPolar(mLinvel1.GetLength_41E260(), pSprt->mRotation);
    CarPhysics::SetSprite(pSprt);
}

MATCH_FUNC(0x563670)
void CarPhysics::UpdateSpriteFromPhysics()
{
    Sprite* car_sprite = mpCar->mCarSprite;
    car_sprite->set_xyz_lazy(mCp1.x, mCp1.y, mCp3);
    car_sprite->set_ang_lazy(mTheta);
}

MATCH_FUNC(0x5636c0)
void CarPhysics::UpdateCarAndTrailerSpriteFromPhysics()
{
    UpdateSpriteFromPhysics();

    Trailer* pTrailer = mpCar->mpTrailer;
    if (pTrailer)
    {
        pTrailer->mTrailerCar->mPhysics->UpdateSpriteFromPhysics();
    }
}

MATCH_FUNC(0x5636e0)
bool CarPhysics::IsNearlyStopped()
{
    if (mLinvel1.GetLength_41E260() < kFpPoint3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x5637a0)
void CarPhysics::Init()
{
    mLinvel1.reset();
    mAngVelRad = Fix16(0);
    mZVel = kFP16Zero_6FE20C;
    ResetForceAccumulators();
    mIsFootBrakeOn = 0;
    mIsHandBrakeOn = 0;
    mIsForwardGasOn = 0;
    mIsBackwardGasOn = 0;
    mIsCoasting = false;
    mTurnDirection = car_turn_direction::none_0;
    mCarModel = -1;
    mHandBrakeForce = 0;
    mTimerSinceLastMove = 0;
    SetOilSpinDir(0);
    mOilSpinTimer = 0;
    mSurfaceType = car_surface_type::flat_surface_0;
    mBlockSpec = 0;
    mCurrentSlopeLength = 0;
    mCurrentSlopeLeftTiles = 0;
    mLastSkidPos[0].reset();
    mLastSkidPos[1].reset();
    mLastSkidPos[2].reset();
    mLastSkidPos[3].reset();
    mState = 1;
    mDamagedUntilCycle = 0;
    mGasPedal = kFpZero;
    mDriveWheelsLockedQ = 0;
    mBrakePressure = kBrakePressureStart;
    mAirZVel = kFP16Zero_6FE20C;
    mFrontSkid = kFP16Zero_6FE20C;
    mRearSkid = kFP16Zero_6FE20C;
    mSbw = 0;
    mTpa = 0;
}

MATCH_FUNC(0x563890)
void CarPhysics::PoolAllocate()
{
    mCm1.reset();
    mTheta = kAngZero_6FE3C0;
    mCp1.reset();
    mpCar = NULL;
    Init();
    mVelReadOnly.reset();
}

MATCH_FUNC(0x5638c0)
void CarPhysics::SetCar(Car* pCar)
{
    this->mpCar = pCar;
    Ped* pDriver = pCar->mDriver;
    if (pDriver && pDriver->mPlayer)
    {
        this->mState = 2;
    }
    else
    {
        this->mState = 1;
    }
}

MATCH_FUNC(0x563900)
CarPhysics::CarPhysics()
{
    mpNext = NULL;
    PoolAllocate();
}