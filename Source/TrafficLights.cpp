#include "TrafficLights.hpp"
#include "rgb.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "map_0x370.hpp"
#include "Light.hpp"
#include "LightPool.hpp"
#include "Car.hpp"
#include "debug.hpp"

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

DEFINE_GLOBAL(TrafficLights*, gTrafficLights_194, 0x705958);

DEFINE_GLOBAL_INIT(Fix16, kFpOne_70580C, Fix16(1), 0x70580C);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_705810, Fix16(2), 0x705810);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_705674, Fix16(0.5), 0x705674);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_705714, Fix16(0.5), 0x705714);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_705678, Fix16(0x2666, 0), 0x705678);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne, kFpOne_70580C, 0x7055E4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_7055E0, kFpTwo_705810, 0x7055E0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint6, kFpPoint6_705678, 0x705660);

DEFINE_GLOBAL_INIT(Fix16, kFpOneSixtyFourth_7057C0, Fix16(256, 0), 0x7057C0);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixtyFourth_7058C4, kFpOneSixtyFourth_7057C0, 0x7058C4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds, kFpOneSixtyFourth_7058C4 * 6, 0x7055FC);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, kFpOne_70580C + kFpHalf_705674, 0x705850);

DEFINE_GLOBAL_INIT(Ang16, kAng270_705788, Ang16(1080), 0x705788);
DEFINE_GLOBAL_INIT(Ang16, kAng90_705690, Ang16(360), 0x705690);
DEFINE_GLOBAL_INIT(Ang16, kAng180_705762, Ang16(720), 0x705762);
DEFINE_GLOBAL_INIT(Ang16, kAng0_705948, Ang16(0), 0x705948);

// Sprite index offset from the object's base sprite for the traffic_light_lighting_255 object
namespace traffic_light_sprite_offset
{
enum
{
    red_0 = 0,
    green_1 = 1,
    amber_2 = 2,
};
} // namespace traffic_light_sprite_offset

// Phase durations, indexed by traffic_light_phase
u8 traffic_light_phase_timers_626840[] = {0x00, 0x64, 0x18, 0x18, 0x64, 0x18, 0x18, 0x96, 0x50, 0x00, 0x00, 0x00};

// https://decomp.me/scratch/JogBj
MATCH_FUNC(0x5C1D00)
void TrafficLight::Init(u8 x, u8 y, u8 w, u8 h)
{

    Fix16 zpos = gMap_0x370->FindGroundZForCoord(x, y);
    u8 block_z = (zpos.ToInt()) - 1;

    // West side
    if (gMap_0x370->IsBlockRoadType_42A8C0(x - 1, h + y - 1, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::traffic_light_hull_170, Fix16(x) - kFpOne, Fix16(y + (h / 2)), kFpTwo_7055E0 + zpos, kAng270_705788);
        mWestHeadlightObj = gObject_5C->NewPhysicsObj(objects::traffic_light_lighting_255,
                                                          Fix16(x) - kFpThreeThirtySeconds - kFpOne,
                                                          Fix16(y + (h / 2)),
                                                          zpos + kFpTwo_7055E0,
                                                          kAng270_705788);
        mWestHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
        mWestLight = gLight_1D4CC->InitOutOfLine_469010(Fix16(x) - kFpOne, Fix16(y + (h / 2)), zpos, MAKE_RGB(255, 0, 0), kFpOneAndHalf, 200);
    }
    else
    {
        mWestHeadlightObj = NULL;
    }

    // East side
    if (gMap_0x370->IsBlockRoadType_42A8C0(x + w, y, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::traffic_light_hull_170, Fix16(x + w) + kFpOne, Fix16(y + (h / 2)), kFpTwo_7055E0 + zpos, kAng90_705690);
        mEastHeadlightObj = gObject_5C->NewPhysicsObj(objects::traffic_light_lighting_255,
                                                          Fix16(x + w) + kFpThreeThirtySeconds + kFpOne,
                                                          Fix16(y + (h / 2)),
                                                          zpos + kFpTwo_7055E0,
                                                          kAng90_705690);
        mEastHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
        mEastLight = gLight_1D4CC->InitOutOfLine_469010(Fix16(x + w) + kFpOne, Fix16(y + (h / 2)), zpos + kFpTwo_7055E0, MAKE_RGB(255, 0, 0), kFpOneAndHalf, 200);
    }
    else
    {
        mEastHeadlightObj = NULL;
    }

    // North side
    if (gMap_0x370->IsBlockRoadType_42A8C0(x, y - 1, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::traffic_light_hull_170, Fix16(x + (w / 2)), Fix16(y) - kFpOne, kFpTwo_7055E0 + zpos, kAng180_705762);
        mNorthHeadlightObj = gObject_5C->NewPhysicsObj(objects::traffic_light_lighting_255,
                                                          Fix16(x + (w / 2)),
                                                          Fix16(y) - kFpThreeThirtySeconds - kFpOne,
                                                          zpos + kFpTwo_7055E0,
                                                          kAng180_705762);
        mNorthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
        mNorthLight = gLight_1D4CC->InitOutOfLine_469010(Fix16(x + (w / 2)), Fix16(y) - kFpOne, zpos, MAKE_RGB(0, 255, 0), kFpOneAndHalf, 200);
    }
    else
    {
        mNorthHeadlightObj = NULL;
    }

    // South side
    if (gMap_0x370->IsBlockRoadType_42A8C0(x + w - 1, y + h, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::traffic_light_hull_170, Fix16(x + (w / 2)), Fix16(y + h) + kFpOne, kFpTwo_7055E0 + zpos, kAng0_705948);
        mSouthHeadlightObj = gObject_5C->NewPhysicsObj(objects::traffic_light_lighting_255,
                                                          Fix16(x + (w / 2)),
                                                          Fix16(y + h) + kFpOne + kFpThreeThirtySeconds,
                                                          zpos + kFpTwo_7055E0,
                                                          kAng0_705948);
        mSouthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
        mSouthLight = gLight_1D4CC->InitOutOfLine_469010(Fix16(x + (w / 2)), Fix16(y + h) + kFpOne, zpos, MAKE_RGB(0, 255, 0), kFpOneAndHalf, 200);
    }
    else
    {
        mSouthHeadlightObj = NULL;
    }

    if ((mWestHeadlightObj || mNorthHeadlightObj) && gMap_0x370->IsBlockPavementTypeAt_452980(x - 1, y - 1, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::ped_crossing_trigger_258, Fix16(x) - kFpHalf_705714, Fix16(y) - kFpHalf_705714, zpos, kAng0_705948);
    }

    if ((mEastHeadlightObj || mNorthHeadlightObj) && gMap_0x370->IsBlockPavementTypeAt_452980(x + w, y - 1, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::ped_crossing_trigger_258, Fix16(w + x) + kFpHalf_705714, Fix16(y) - kFpHalf_705714, zpos, kAng0_705948);
    }

    if ((mEastHeadlightObj || mSouthHeadlightObj) && gMap_0x370->IsBlockPavementTypeAt_452980(x + w, h + y, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::ped_crossing_trigger_258, Fix16(w + x) + kFpHalf_705714, Fix16(y + h) + kFpHalf_705714, zpos, kAng0_705948);
    }

    if ((mWestHeadlightObj || mSouthHeadlightObj) && gMap_0x370->IsBlockPavementTypeAt_452980(x - 1, y + h, block_z))
    {
        gObject_5C->NewPhysicsObj(objects::ped_crossing_trigger_258, Fix16(x) - kFpHalf_705714, Fix16(y + h) + kFpHalf_705714, zpos, kAng0_705948);
    }

    if (mNorthHeadlightObj)
    {
        for (s32 i = 0; i < w; i++)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock((Fix16(x + i) + kFpHalf_705714).ToInt(),
                                                                          (Fix16(y) - kFpPoint6).ToInt(),
                                                                          (zpos - kFpOne_70580C).ToInt());
            if (pBlock && (pBlock->mArrows & block_arrow::green_down_8) != 0)
            {
                gObject_5C->NewPhysicsObj(objects::pedestrian_crossing_marker_122, Fix16(x + i) + kFpHalf_705714, Fix16(y) - kFpPoint6, zpos, kAng0_705948);
            }
        }
    }

    if (mSouthHeadlightObj)
    {
        for (s32 i = 0; i < w; i++)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock((Fix16(x + i) + kFpHalf_705714).ToInt(),
                                                                          (Fix16(y + h) + kFpPoint6).ToInt(),
                                                                          (zpos - kFpOne_70580C).ToInt());
            if (pBlock && (pBlock->mArrows & block_arrow::green_up_4) != 0)
            {
                gObject_5C->NewPhysicsObj(objects::pedestrian_crossing_marker_122, Fix16(x + i) + kFpHalf_705714, Fix16(y + h) + kFpPoint6, zpos, kAng0_705948);
            }
        }
    }

    if (mEastHeadlightObj)
    {
        for (s32 i = 0; i < h; i++)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock((Fix16(x + w) + kFpPoint6).ToInt(),
                                                                          (Fix16(y + i) + kFpHalf_705714).ToInt(),
                                                                          (zpos - kFpOne_70580C).ToInt());
            if (pBlock && (pBlock->mArrows & block_arrow::green_left_1) != 0)
            {
                gObject_5C->NewPhysicsObj(objects::pedestrian_crossing_marker_122, Fix16(x + w) + kFpPoint6, Fix16(y + i) + kFpHalf_705714, zpos, kAng90_705690);
            }
        }
    }

    if (mWestHeadlightObj)
    {
        for (s32 i = 0; i < h; i++)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock((Fix16(x) - (kFpPoint6)).ToInt(),
                                                                          (Fix16(y + i) + kFpHalf_705714).ToInt(),
                                                                          (zpos - kFpOne_70580C).ToInt());
            if (pBlock && (pBlock->mArrows & block_arrow::green_right_2) != 0)
            {
                gObject_5C->NewPhysicsObj(objects::pedestrian_crossing_marker_122, Fix16(x) - kFpPoint6, Fix16(y + i) + kFpHalf_705714, zpos, kAng90_705690);
            }
        }
    }
}

MATCH_FUNC(0x5C27A0)
void TrafficLight::UpdateLightsFromPhase(u8 phase)
{
    switch (phase)
    {
        case traffic_light_phase::north_south_green_1:
            if (mNorthHeadlightObj)
            {
                mNorthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
                mNorthLight->mArgb = MAKE_RGB(0, 255, 0);
            }

            if (mSouthHeadlightObj)
            {
                mSouthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
                mSouthLight->mArgb = MAKE_RGB(0, 255, 0);
            }
            break;

        case traffic_light_phase::north_south_amber_2:
            if (mNorthHeadlightObj)
            {
                mNorthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::amber_2);
                mNorthLight->mArgb = MAKE_RGB(255, 128, 0);
            }

            if (mSouthHeadlightObj)
            {
                mSouthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::amber_2);
                mSouthLight->mArgb = MAKE_RGB(255, 128, 0);
            }
            break;

        case traffic_light_phase::north_south_red_3:
            if (mNorthHeadlightObj)
            {
                mNorthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
                mNorthLight->mArgb = MAKE_RGB(255, 0, 0);
            }

            if (mSouthHeadlightObj)
            {
                mSouthHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
                mSouthLight->mArgb = MAKE_RGB(255, 0, 0);
            }
            break;

        case traffic_light_phase::east_west_green_4:
            if (mEastHeadlightObj)
            {
                mEastHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
                mEastLight->mArgb = MAKE_RGB(0, 255, 0);
            }

            if (mWestHeadlightObj)
            {
                mWestHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::green_1);
                mWestLight->mArgb = MAKE_RGB(0, 255, 0);
            }
            break;

        case traffic_light_phase::east_west_amber_5:
            if (mEastHeadlightObj)
            {
                mEastHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::amber_2);
                mEastLight->mArgb = MAKE_RGB(255, 128, 0);
            }

            if (mWestHeadlightObj)
            {
                mWestHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::amber_2);
                mWestLight->mArgb = MAKE_RGB(255, 128, 0);
            }
            break;

        case traffic_light_phase::east_west_red_6:
            if (mEastHeadlightObj)
            {
                mEastHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
                mEastLight->mArgb = MAKE_RGB(255, 0, 0);
            }

            if (mWestHeadlightObj)
            {
                mWestHeadlightObj->SetSpriteIdxOffset(traffic_light_sprite_offset::red_0);
                mWestLight->mArgb = MAKE_RGB(255, 0, 0);
            }
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5c2910)
void TrafficLights::AddTrafficLight(u8 x, u8 y, u8 w, u8 h)
{
    TrafficLight* pLight = new TrafficLight();
    mTrafficLights[mArrayUsedCount++] = pLight;
    pLight->Init(x, y, w, h);
}

MATCH_FUNC(0x5c2950)
void TrafficLights::TrafficLightsService()
{
    mTimer--;

    if (mTimer == 0)
    {
        if (mPhase == traffic_light_phase::east_west_amber_5)
        {
            gCarManager_6C->ToggleTrafficLightState_4C39F0();
        }

        mPhase++;

        if (mPhase > traffic_light_phase::all_red_8)
        {
            mPhase = traffic_light_phase::north_south_green_1;
        }

        mTimer = traffic_light_phase_timers_626840[mPhase];

        for (u32 i = 0; i < mArrayUsedCount; i++)
        {
            mTrafficLights[i]->UpdateLightsFromPhase(mPhase);
        }
    }

    if (bDo_show_traffic_lights_info)
    {
        ShowTrafficLightsInfo();
    }
}

MATCH_FUNC(0x5c2a10)
void TrafficLights::ShowTrafficLightsInfo()
{
    swprintf(tmpBuff, L"timer: %d", mTimer);
    gHud_2B00->mTexts.DisplayText(tmpBuff, -1, 0, gDebugFont, 1);

    swprintf(tmpBuff, L"phase: %d", mPhase);
    gHud_2B00->mTexts.DisplayText(tmpBuff, -1, 16, gDebugFont, 1);

    if (mPhase == traffic_light_phase::peds_crossing_7)
    {
        gHud_2B00->mTexts.DisplayText(L"peds crossing now!", -1, 32, gDebugFont, 1);
    }
}

MATCH_FUNC(0x5c2ac0)
void TrafficLights::CreateFromMapZones()
{
    gmp_map_zone* pZoneIter = gMap_0x370->GetFirstZoneOfType(Traffic_light_2);
    while (pZoneIter)
    {
        AddTrafficLight(pZoneIter->mX, pZoneIter->mY, pZoneIter->mW, pZoneIter->mH);
        pZoneIter = gMap_0x370->GetNextZone();
    }
}

MATCH_FUNC(0x5c2b00)
TrafficLights::TrafficLights()
{
    mArrayUsedCount = 0;
    for (s32 i = 0; i < GTA2_COUNTOF(mTrafficLights); i++)
    {
        mTrafficLights[i] = NULL;
    }
    mPhase = traffic_light_phase::north_south_green_1;
    mTimer = traffic_light_phase_timers_626840[traffic_light_phase::north_south_green_1];
}

MATCH_FUNC(0x5c2b30)
TrafficLights::~TrafficLights()
{
    for (u32 i = 0; i < mArrayUsedCount; i++)
    {
        if (mTrafficLights[i])
        {
            GTA2_DELETE_AND_NULL(mTrafficLights[i]);
        }
    }
}