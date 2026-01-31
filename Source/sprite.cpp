#include "sprite.hpp"
#include "CarInfo_808.hpp" // TODO: only because of dword_6F6850
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_2C_Pool.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "char.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "memory.hpp"
#include "root_sound.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "winmain.hpp" // TODO: only because of gLighting_626A09

DEFINE_GLOBAL(Sprite_8*, gSprite_8_703820, 0x703820);
DEFINE_GLOBAL(Sprite_4C_Pool*, gSprite_4C_Pool_70381C, 0x70381C);
DEFINE_GLOBAL(Sprite_Pool*, gSprite_Pool_703818, 0x703818);
DEFINE_GLOBAL(Sprite_3CC*, gSprite_3CC_67AF1C, 0x67AF1C);
DEFINE_GLOBAL(Sprite_18_Pool*, gSprite_18_Pool_703B80, 0x703B80);
DEFINE_GLOBAL(Sprite*, gSprite_703814, 0x703814);
DEFINE_GLOBAL(Sprite*, gSprite_6F61E8, 0x6F61E8);
DEFINE_GLOBAL_INIT(Fix16, dword_7035C4, Fix16(1), 0x7035C4);
DEFINE_GLOBAL_INIT(Fix16, gFix16_7035C0, Fix16(0), 0x7035C0);
DEFINE_GLOBAL_INIT(Ang16, gAng16_703804, Ang16(0), 0x703804);
DEFINE_GLOBAL(UnknownList, dword_6F6850, 0x6F6850);
DEFINE_GLOBAL_INIT(Fix16, dword_703424, Fix16(0xCCC, 0), 0x703424);
DEFINE_GLOBAL_INIT(Fix16, dword_7035E4, Fix16(8), 0x7035E4);
DEFINE_GLOBAL_INIT(Fix16, dword_703578, Fix16(256, 0), 0x703578);
DEFINE_GLOBAL_INIT(Fix16, dword_703678, dword_703578, 0x703678);
DEFINE_GLOBAL_INIT(Fix16, k_dword_7033C0, dword_7035E4 - dword_703678, 0x7033C0);
DEFINE_GLOBAL_INIT(Ang16, word_70344C, Ang16(360), 0x70344C);
DEFINE_GLOBAL_INIT(Ang16, word_70351E, Ang16(720), 0x70351E);
DEFINE_GLOBAL_INIT(Ang16, word_703544, Ang16(1080), 0x703544);
DEFINE_GLOBAL_ARRAY(Vert, gTileVerts_7036D0, 8, 0x7036D0);
DEFINE_GLOBAL_INIT(u32, kGlobalMask0_61A9AC, 0x0C00060, 0x61A9AC); // BitSet32 flag
DEFINE_GLOBAL_INIT(u32, kGlobalMask1_61A9A8, 0x0C70060, 0x61A9A8); // BitSet32 flag
DEFINE_GLOBAL_INIT(u32, kGlobalMask2_61A9A4, 0x0C78060, 0x61A9A4); // BitSet32 flag
DEFINE_GLOBAL(u32, gFlags_67ACF8, 0x67ACF8); // BitSet32 flag
DEFINE_GLOBAL(Fix16, dword_703A38, 0x703A38);

EXTERN_GLOBAL(s32, window_width_706630);
EXTERN_GLOBAL(s32, window_height_706B50);

// matched
static inline Fix16 __stdcall RoundZToLayer_4B9C70(Fix16& in)
{
    return Fix16((in.mValue + 0x80) & 0xFFFFFF00, 0);
}

// matched
static inline void __stdcall SaveUnprojectedVertex_4B9990(f32 xCoord, f32 yCoord, f32 z_val, Vert* pVerts)
{
    s32 next_idx = (pVerts - gTileVerts_7036D0) + 4;

    gTileVerts_7036D0[next_idx].x = xCoord;
    gTileVerts_7036D0[next_idx].y = yCoord;
    gTileVerts_7036D0[next_idx].z = z_val;
}

// partially matched: https://decomp.me/scratch/qtmIe
static inline void ProjectWorldPointToScreen_4BA4D0(Fix16_Point& point, Vert* pVert, Fix16& zpos)
{
    SaveUnprojectedVertex_4B9990(point.x.ToFloat(), point.y.ToFloat(), zpos.ToFloat(), pVert);
    pVert->z = 1.0 / (gViewCamera_676978->field_98_cam_pos2.field_8_z.ToFloat() + (8.0 - zpos.ToFloat()));
    pVert->x = gViewCamera_676978->field_60.x.ToFloat() * (point.x.ToFloat() - gViewCamera_676978->field_98_cam_pos2.field_0_x.ToFloat()) *
            pVert->z +
        (u32)gViewCamera_676978->field_70_screen_px_center_x;
    pVert->y = gViewCamera_676978->field_60.x.ToFloat() * (point.y.ToFloat() - gViewCamera_676978->field_98_cam_pos2.field_4_y.ToFloat()) *
            pVert->z +
        (u32)gViewCamera_676978->field_74_screen_px_center_y;
}

// not exactly __fastcall but something similar... https://decomp.me/scratch/N5g2D
static inline void __fastcall SetUV_4B9BC0(f32& u, f32& v)
{
    gTileVerts_7036D0[0].u = 0.30000001f;
    gTileVerts_7036D0[0].v = 0.30000001f;

    gTileVerts_7036D0[1].u = u;
    gTileVerts_7036D0[1].v = 0.30000001f;

    gTileVerts_7036D0[2].u = u;
    gTileVerts_7036D0[2].v = v;

    gTileVerts_7036D0[3].u = 0.30000001f;
    gTileVerts_7036D0[3].v = v;
}

MATCH_FUNC(0x562450)
Fix16_Point Sprite::GetBoundingBoxCorner_562450(s32 idx)
{
    return Fix16_Point(field_C_sprite_4c_ptr->field_C_renderingRect[idx].x, field_C_sprite_4c_ptr->field_C_renderingRect[idx].y);
}

// 9.6f 0x4207B0
MATCH_FUNC(0x443580)
Fix16_Point Sprite::get_x_y_443580()
{
    return Fix16_Point(field_14_xy.x, field_14_xy.y);
}

MATCH_FUNC(0x451950)
void Sprite::set_xyz_lazy_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (field_14_xy.x != xpos || field_14_xy.y != ypos || field_1C_zpos != zpos)
    {
        field_14_xy.x = xpos;
        field_14_xy.y = ypos;
        field_1C_zpos = zpos;
        ResetZCollisionAndDebugBoxes_59E7B0();
    }
}

MATCH_FUNC(0x54EC80)
void Sprite::setxy_lazy_54EC80(Fix16 xpos, Fix16 ypos)
{
    if (this->field_14_xy.x != xpos || this->field_14_xy.y != ypos)
    {
        this->field_14_xy.x = xpos;
        this->field_14_xy.y = ypos;
        ResetZCollisionAndDebugBoxes_59E7B0();
    }
}

MATCH_FUNC(0x59E170)
bool Sprite::IsControlledByActivePlayer_59E170()
{
    Ped* pPed = GetPed_59E1B0();
    if (!pPed)
    {
        if (field_30_sprite_type_enum == sprite_types_enum::car && field_8_car_bc_ptr)
        {
            pPed = field_8_car_bc_ptr->GetEffectiveDriver_43E990();
        }
    }

    if (pPed)
    {
        if (pPed->field_15C_player)
        {
            if (!pPed->field_15C_player->field_0_bIsUser)
            {
                return true;
            }
        }
    }

    return false;
}

MATCH_FUNC(0x59E1B0)
Ped* Sprite::GetPed_59E1B0()
{
    if (this->field_30_sprite_type_enum == sprite_types_enum::ped && (this->field_8_char_b4_ptr) != 0)
    {
        return field_8_char_b4_ptr->field_7C_pPed;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x59E1D0)
s32 Sprite::IsOnWater_59E1D0()
{
    gmp_block_info* pBlock;

    const Fix16 zpos = field_1C_zpos;
    if (zpos > gFix16_7035C0)
    {
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_14_xy.x.ToInt(), field_14_xy.y.ToInt(), (zpos - dword_7035C4).ToInt());
    }
    else
    {
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_14_xy.x.ToInt(), field_14_xy.y.ToInt(), zpos.ToInt());
    }

    if (pBlock)
    {
        const u16 lid_idx = pBlock->field_8_lid & 1023;
        if (gGtx_0x106C_703DD4->field_6C_spec[lid_idx] == tile_spec::water && gGtx_0x106C_703DD4->IsTileRemapped_5AA850(lid_idx))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x59E250)
u8 Sprite::GetWaterCornerMask_59E250()
{
    u8 bits = 0;
    Fix16 zpos_delta = this->field_1C_zpos - dword_7035C4;
    UpdateCollisionBoundsIfNeeded_59E9C0();

    Fix16_Point_POD* pBBox = field_C_sprite_4c_ptr->field_C_renderingRect;
    for (s32 i = 0; i < 4; i++)
    {
        Fix16_Point_POD* pIter = &pBBox[i];
        if (gMap_0x370_6F6268->sub_4B9F40(pIter->x.ToInt(), pIter->y.ToInt(), zpos_delta.ToInt()))
        {
            bits |= 1 << i;
        }
    }

    return bits;
}

MATCH_FUNC(0x59e2e0)
void Sprite::sub_59E2E0(void)
{
    Update_4C_59F990();
    memcpy(field_4_0x4C_len, field_C_sprite_4c_ptr, sizeof(Sprite_4C));
}

MATCH_FUNC(0x59e300)
void Sprite::sub_59E300()
{
    memcpy(field_C_sprite_4c_ptr, field_4_0x4C_len, sizeof(Sprite_4C));
}

MATCH_FUNC(0x59e320)
void Sprite::sub_59E320(char_type a2)
{
    u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
    sprite_index* sprite_index_5AA440 = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3);
    u32 field_5_height = sprite_index_5AA440->field_5_height;
    u32 field_4_width = sprite_index_5AA440->field_4_width;

    s32 result;
    if ((s32)(field_4_width & 0xFFFFFFFE) < (s32)(field_5_height & 0xFFFFFFFE))
    {
        result = field_4_width >> 1;
    }
    else
    {
        result = field_5_height >> 1;
    }
    field_38_zoom = a2 + field_38_zoom;
    if ((field_38_zoom & 0xFF) > result)
    {
        field_38_zoom = result;
    }
}

STUB_FUNC(0x59e390) // https://decomp.me/scratch/dijmx
bool Sprite::sub_59E390(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x59e4c0)
s32 Sprite::sub_59E4C0(Fix16 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/GIpfM
MATCH_FUNC(0x59E590)
char_type Sprite::CollisionCheck_59E590(Sprite* pOther)
{
    // TODO: inlining issue inside of AABB_Intersects_41E2F0
    if (field_C_sprite_4c_ptr->field_30_boundingBox.AABB_Intersects_41E2F0(&pOther->field_C_sprite_4c_ptr->field_30_boundingBox))
    {
        if ((field_C_sprite_4c_ptr->IsZeroWidth_41E390() || field_0.jIsAxisAligned_41E3C0()) &&
            (pOther->field_C_sprite_4c_ptr->IsZeroWidth_41E390() || pOther->field_0.jIsAxisAligned_41E3C0()))
        {

            return 1;
        }

        if (RotatedRectCollisionSAT_5A0380(pOther) || pOther->RotatedRectCollisionSAT_5A0380(this))
        {
            return 1;
        }
    }

    return 0;
}

STUB_FUNC(0x59E680)
char_type Sprite::sub_59E680(Fix16 a2, Sprite* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x59e7b0)
void Sprite::ResetZCollisionAndDebugBoxes_59E7B0()
{
    field_39_z_col = -1;
    if (field_C_sprite_4c_ptr != NULL)
    {
        field_C_sprite_4c_ptr->field_48_bBoxUpToDate = false;
    }
    if (field_4_0x4C_len != NULL)
    {
        field_4_0x4C_len->field_48_bBoxUpToDate = false;
    }
}

MATCH_FUNC(0x59e7d0)
Sprite* Sprite::sub_59E7D0(s32 a2)
{
    Sprite* result;

    UpdateCollisionBoundsIfNeeded_59E9C0();
    field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    gSprite_6F61E8 = this;
    if (gMap_0x370_6F6268->sub_4E1520(field_1C_zpos.ToInt()))
    {
        return gRozza_679188.field_20_pSprite;
    }
    result = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this, a2);
    if (result)
    {
        gRozza_679188.field_0_type = 3;
        gRozza_679188.field_20_pSprite = result;
    }
    return result;
}

MATCH_FUNC(0x59E830)
char_type Sprite::IsThreatToSearchingPed_59E830()
{
    switch (this->field_30_sprite_type_enum)
    {
        case 2: // car
            return this->field_8_car_bc_ptr->IsThreatToSearchingPed_43AAE0();

        case 3: // char_b4
            return this->field_8_char_b4_ptr->IsThreatToSearchingPed_553330();

        default:
            return 1;
    }
}

MATCH_FUNC(0x59E850)
char_type Sprite::sub_59E850(Sprite* pSprite)
{
    switch (this->field_30_sprite_type_enum)
    {
        case 1:
        case 4:
        case 5:
            return field_8_object_2C_ptr->sub_525370(pSprite);
        case 2:
            return field_8_car_bc_ptr->CanCarCollideWithSprite_43AAF0(pSprite);
        case 3:
            return field_8_char_b4_ptr->sub_553340(pSprite);
        default:
            return 1;
    }
}

MATCH_FUNC(0x59E8C0)
void Sprite::HandleObjectCollision_59E8C0(Sprite* pSprite)
{
    s32 sprite_type = this->field_30_sprite_type_enum;
    if (sprite_type == sprite_types_enum::unknown_1 || sprite_type > sprite_types_enum::ped && sprite_type <= sprite_types_enum::map_obj)
    {
        field_8_object_2C_ptr->HandleImpact_528E50(pSprite);
        s32 type = pSprite->field_30_sprite_type_enum;
        if (type == sprite_types_enum::code_obj1 || type == sprite_types_enum::map_obj || type == sprite_types_enum::unknown_1)
        {
            if (pSprite->field_8_object_2C_ptr)
            {
                field_8_object_2C_ptr->HandleCollisionWithObject_529000(pSprite->field_8_object_2C_ptr);
            }
        }
    }
}

MATCH_FUNC(0x59E910)
void Sprite::ProcessCarToCarImpactIfCar_59E910(Sprite* pSprite)
{
    if (field_30_sprite_type_enum == sprite_types_enum::car)
    {
        field_8_car_bc_ptr->ProcessCarToCarImpact_43ADC0(pSprite);
    }
}

MATCH_FUNC(0x59e960)
void Sprite::sub_59E960()
{
    switch (this->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car:
            this->field_28_num = 15;
            break;
        case sprite_types_enum::ped:
            this->field_28_num = 23;
            break;
        case sprite_types_enum::code_obj1:
            this->field_28_num = 2;
            break;
        case sprite_types_enum::map_obj:
            this->field_28_num = 2;
            break;
        case sprite_types_enum::code_obj2:
            this->field_28_num = 33;
            break;
        case sprite_types_enum::unknown_0:
            this->field_28_num = 0;
            break;
        case sprite_types_enum::unknown_1:
            this->field_28_num = 0;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x59e9c0)
void Sprite::UpdateCollisionBoundsIfNeeded_59E9C0()
{
    if (!field_C_sprite_4c_ptr->field_48_bBoxUpToDate)
    {
        if (field_C_sprite_4c_ptr->IsZeroWidth_41E390())
        {
            field_C_sprite_4c_ptr->UpdateRotatedBoundingBox_5A3550(field_14_xy.x, field_14_xy.y, field_1C_zpos, gAng16_703804);
        }
        else
        {
            field_C_sprite_4c_ptr->UpdateRotatedBoundingBox_5A3550(field_14_xy.x, field_14_xy.y, field_1C_zpos, field_0);
        }
    }
}

MATCH_FUNC(0x59ea00)
void Sprite::SetRemap(s16 remap)
{
    switch (this->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car:
            this->field_34 = 3;
            break;
        case sprite_types_enum::ped:
            this->field_34 = 4;
            break;
        case sprite_types_enum::code_obj1:
        case sprite_types_enum::code_obj2:
            this->field_34 = 5;
            break;
        case sprite_types_enum::map_obj:
            this->field_34 = 6;
            break;
        case sprite_types_enum::user:
            this->field_34 = 7;
            break;
        case sprite_types_enum::font:
            this->field_34 = 8;
        default:
            break;
    }
    this->field_24_remap = remap;
}

MATCH_FUNC(0x59eaa0)
s16 Sprite::sub_59EAA0()
{
    if (field_34 == 2)
    {
        s16 v2 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        return gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, v2);
    }
    return gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(field_34, field_24_remap);
}

MATCH_FUNC(0x59eae0)
char_type Sprite::has_shadows_59EAE0()
{
    switch (field_30_sprite_type_enum)
    {
        case 3:
            return field_8_char_b4_ptr->sub_5451C0();
        case 2:
            return 1;
        case 1:
        case 4:
        case 5:
            return field_8_object_2C_ptr->field_8->field_62_has_shadows;
        case 6:
        case 7:
        case 8:
            return 0;
        default:
            break;
    }
    return 0;
}

STUB_FUNC(0x59eb30)
void Sprite::ShowId_59EB30(f32& a2, f32& a3)
{
    NOT_IMPLEMENTED;
}

// 9.6f inline
static inline void __stdcall sub_4BA2C0(const wchar_t* pStr, Fix16 x, Fix16 y, s32 font)
{
    s32 draw_kind = 2;
    Fix16 scale_y = y * gViewCamera_676978->field_A8_ui_scale;
    Fix16 scale_x = x * gViewCamera_676978->field_A8_ui_scale;
    DrawText_5D8A10(pStr, scale_x, scale_y, font, gViewCamera_676978->field_A8_ui_scale, draw_kind, 0, 0, 0);
}

WIP_FUNC(0x59ee40)
void Sprite::ShowHorn_59EE40(f32& x, f32& y)
{
    WIP_IMPLEMENTED;

    // 0x4BAEF0 9.6f
    if (bDo_show_horn_67D4F2)
    {
        Car_BC* pCar = AsCar_40FEB0();
        if (pCar)
        {
            // TODO: Code is actually too "good" here so doesn't match
            f32 screen_x = (x / (f32)window_width_706630) * 640.0f;
            f32 screen_y = (y / (f32)window_height_706B50) * 480.0f;

            Fix16 xpos(screen_x);
            Fix16 ypos(screen_y);

            if (pCar->IsEmittingHorn_411970())
            {
                sub_4BA2C0(L"H", xpos, ypos, word_703BAA);
            }

            Ped* pDriver = pCar->field_54_driver;
            if (pDriver)
            {
                if (gPolice_7B8_6FEE40->sub_56F880(pDriver))
                {
                    sub_4BA2C0(L"P", xpos, ypos, word_703BAA);
                }
            }
        }
    }
}

// https://decomp.me/scratch/omhyc
STUB_FUNC(0x59eff0)
void Sprite::Draw_59EFF0()
{
    NOT_IMPLEMENTED;
    sprite_index* pSpriteIndex;
    u16 converted_pal;
    sprite_index* pSpriteIndex2;
    u16 pal;
    STexture* pTexture;

    u16 sprite_idx;

    Sprite::Update_4C_59F990();
    if (field_38_zoom != 0)
    {
        sprite_idx = gGtx_0x106C_703DD4->field_4_sprite_index_count - 1;
        pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);
        converted_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        pSpriteIndex2 = gGtx_0x106C_703DD4->get_sprite_index_5AA440(converted_pal);
        pSpriteIndex->field_4_width = pSpriteIndex2->field_4_width - 2 * field_38_zoom;
        pSpriteIndex->field_5_height = pSpriteIndex2->field_5_height - 2 * field_38_zoom;
        if (!pSpriteIndex->field_4_width || !pSpriteIndex->field_5_height)
        {
            return;
        }
        pSpriteIndex->field_0_pData = &pSpriteIndex2->field_0_pData[257 * (u8)field_38_zoom];
        pal = Sprite::sub_59EAA0();
        pTexture = gSharp_pare_0x15D8_705064->sub_5B9710(pSpriteIndex->field_4_width,
                                                         pSpriteIndex->field_5_height,
                                                         pSpriteIndex->field_0_pData,
                                                         pal);
    }
    else
    {
        sprite_idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);
        pTexture = gSharp_pare_0x15D8_705064->sub_5B94F0(field_30_sprite_type_enum, field_22_sprite_id, field_34, field_24_remap);
    }

    if (!field_4_0x4C_len->field_48_bBoxUpToDate)
    {
        field_4_0x4C_len->UpdateRotatedBoundingBox_5A3550(field_14_xy.x, field_14_xy.y, field_1C_zpos, field_0);
    }

    //Sprite_4C* p4C = field_4_0x4C_len;

    f32 u = pSpriteIndex->field_4_width - 0.30000001f;
    f32 v = pSpriteIndex->field_5_height - 0.30000001f;

    Fix16 new_zpos = RoundZToLayer_4B9C70(field_1C_zpos);

    ProjectWorldPointToScreen_4BA4D0(field_4_0x4C_len->field_C_renderingRect[0], &gTileVerts_7036D0[0], new_zpos);
    ProjectWorldPointToScreen_4BA4D0(field_4_0x4C_len->field_C_renderingRect[1], &gTileVerts_7036D0[1], new_zpos);
    ProjectWorldPointToScreen_4BA4D0(field_4_0x4C_len->field_C_renderingRect[2], &gTileVerts_7036D0[2], new_zpos);
    ProjectWorldPointToScreen_4BA4D0(field_4_0x4C_len->field_C_renderingRect[3], &gTileVerts_7036D0[3], new_zpos);
    SetUV_4B9BC0(u, v);

    Car_BC* pCar = AsCar_40FEB0();
    if (pCar)
    {
        if (field_38_zoom)
        {
            gSprite_3CC_67AF1C->InvalidateMasksByType_48F6E0(&sprite_idx);
        }
        u32 car_flags = pCar->field_8_damaged_areas.GetRaw_4A5190();
        if (gLighting_626A09)
        {
            if (pCar->inline_check_0x40_info_421680())
            {
                pCar->field_8_damaged_areas.SetGlobal0_4BA360(); // sets a global to another
            }
            else
            {
                if (pCar->is_FBI_car_411920()) // line 6a8
                {
                    pCar->field_8_damaged_areas.SetGlobal1_4BA370();
                }
                else
                {
                    pCar->field_8_damaged_areas.SetGlobal2_4BA350();
                }
            }
            pCar->field_8_damaged_areas.sub_4BA340();
        }
        // TODO: stuff here
        u16 v25 = Sprite::sub_59EAA0();
        // ....
        pCar->field_8_damaged_areas = car_flags;
    }
    else
    {
        Char_B4* pChar = AsCharB4_40FEA0();
        if (pChar)
        {
            Ped* pPed = pChar->get_ped_433A20();
            if (pPed && pPed->field_15C_player)
            {
                if (pPed->field_21C_bf.b25 && !pPed->field_15C_player->IsUser_41DC70())
                {
                    return;
                }
            }
        }
    }

    if (Sprite::has_shadows_59EAE0())
    {
        Vert pShadowVerts[4]; // length 4? strange

        memcpy(&pShadowVerts[0], &gTileVerts_7036D0[0], sizeof(Vert));

        memcpy(&pShadowVerts[1], &gTileVerts_7036D0[1], sizeof(Vert));
        memcpy(&pShadowVerts[2], &gTileVerts_7036D0[2], sizeof(Vert));
        memcpy(&pShadowVerts[3], &gTileVerts_7036D0[3], sizeof(Vert));

        f32 shadow_offset = (gViewCamera_676978->field_A8_ui_scale * 5).ToFloat();
        pShadowVerts[0].diff = 0xA0000000;
        pShadowVerts[1].diff = 0xA0000000;
        pShadowVerts[2].diff = 0xA0000000;
        pShadowVerts[3].diff = 0xA0000000;

        pShadowVerts[0].x = gTileVerts_7036D0[0].x + shadow_offset;
        pShadowVerts[1].x = gTileVerts_7036D0[1].x + shadow_offset;
        pShadowVerts[2].x = gTileVerts_7036D0[2].x + shadow_offset;
        pShadowVerts[3].x = gTileVerts_7036D0[3].x + shadow_offset;

        pShadowVerts[0].y += shadow_offset;
        pShadowVerts[1].y += shadow_offset;
        pShadowVerts[2].y += shadow_offset;
        pShadowVerts[3].y += shadow_offset;

        pgbh_DrawQuad(8576, pTexture, pShadowVerts, 255);
    }

    //u32 render_flags = Sprite::sub_4BAC60();    // another inline...
    pgbh_DrawQuad(Sprite::sub_4BAC60(), pTexture, gTileVerts_7036D0, 255);

    // this part seems to not be critical for peds

    if (pCar && gLighting_626A09)
    {
        pCar->field_8_damaged_areas.sub_4BA330();

        u16 unk3 = Sprite::sub_59EAA0();
        // TODO: missing code here
    }
    // TODO: missing code here

    ++gSprite_8_703820->field_0;
    if (bDo_show_collision_box_67D6E5)
    {
        if (field_C_sprite_4c_ptr)
        {
            //field_C_sprite_4c_ptr->DrawCollisionBox_5A4DA0(field_1C_zpos);
        }
    }

    Sprite::ShowId_59EB30(gTileVerts_7036D0[1].x, gTileVerts_7036D0[1].y);
    Sprite::ShowHorn_59EE40(gTileVerts_7036D0[3].x, gTileVerts_7036D0[3].y);
}

MATCH_FUNC(0x59f950)
void Sprite::AllocInternal_59F950(Fix16 a2, Fix16 a3, Fix16 a4)
{
    if (field_C_sprite_4c_ptr == NULL)
    {
        field_C_sprite_4c_ptr = gSprite_4C_Pool_70381C->Allocate();
    }

    Sprite_4C* pSprite4C = field_C_sprite_4c_ptr;
    pSprite4C->field_0_width = a2;
    pSprite4C->field_4_height = a3;
    pSprite4C->field_8 = a4;
}

MATCH_FUNC(0x59f990)
void Sprite::Update_4C_59F990()
{
    if (this->field_4_0x4C_len == NULL)
    {
        this->field_4_0x4C_len = gSprite_4C_Pool_70381C->Allocate();

        const u16 sprite_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        const sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_pal);

        Fix16 w;
        Fix16 h;

        if (this->field_30_sprite_type_enum == sprite_types_enum::code_obj2)
        {
            w = dword_6F6850.list[sprite_index->field_4_width] / 2;
            h = dword_6F6850.list[sprite_index->field_5_height] / 2;
        }
        else
        {
            w = dword_6F6850.list[sprite_index->field_4_width];
            h = dword_6F6850.list[sprite_index->field_5_height];
        }
        field_4_0x4C_len->SetWidthHeight_4BA070(w, h);
    }
}

MATCH_FUNC(0x59fa40)
void Sprite::sub_59FA40()
{
    if (field_4_0x4C_len)
    {
        const u16 idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        sprite_index* pSprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(idx);

        const Fix16 height = dword_6F6850.list[pSprite_index->field_5_height];
        const Fix16 width = dword_6F6850.list[pSprite_index->field_4_width];

        field_4_0x4C_len->set_wh_4BA030(width, height);
    }
}

MATCH_FUNC(0x59fad0)
void Sprite::FreeSprite4CChildren_59FAD0()
{
    if (field_C_sprite_4c_ptr)
    {
        gSprite_4C_Pool_70381C->Remove(field_C_sprite_4c_ptr);
        field_C_sprite_4c_ptr = 0;
    }

    if (field_4_0x4C_len)
    {
        gSprite_4C_Pool_70381C->Remove(field_4_0x4C_len);
        field_4_0x4C_len = 0;
    }
}

STUB_FUNC(0x59FB10)
bool Sprite::sub_59FB10(Fix16_Rect* a2)
{
    NOT_IMPLEMENTED;
    return false;
}

STUB_FUNC(0x5a0150)
char_type Sprite::FindOverlappingBoundingBoxCorners_5A0150(s32 a2, u8* a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a0320)
char_type Sprite::CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2)
{
    u8 overlapCount = 0;
    const Fix16_Point* pBoundingBox = &field_C_sprite_4c_ptr->field_C_renderingRect[0];

    for (u8 i = 0; i < 4; i++)
    {
        if ((pBoundingBox[i].x > pXY1[0] && pBoundingBox[i].y > pXY1[1]) && (pBoundingBox[i].x < pXY2[0] && pBoundingBox[i].y < pXY2[1]))
        {
            // If we find the first valid match, store index in pCollisionIdx1
            overlapCount++;

            if (overlapCount == 1)
            {
                *pCollisionIdx1 = i;
            }
            // If we find the second valid match, store index in pCollisionIdx2
            else if (overlapCount == 2)
            {
                *pCollisionIdx2 = i;
            }
        }
    }

    // Return the result count (0, 1, or 2)
    return overlapCount;
}

STUB_FUNC(0x5a0380)
bool Sprite::RotatedRectCollisionSAT_5A0380(Sprite* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4F76A0)
EXPORT char_type __stdcall ComputeScanlineIntersectionX_4F76A0(Fix16* scanXMin,
                                                               Fix16* scanXMax,
                                                               Fix16* scanY,
                                                               Fix16_Point* a4,
                                                               Fix16_Point* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5A0970)
char_type Sprite::CheckBBoxScanlineIntersection_5A0970(Fix16 scanXMin, Fix16 scanXMax, Fix16 scanY)
{
    Fix16_Point* pBBox = field_C_sprite_4c_ptr->field_C_renderingRect;
    if (ComputeScanlineIntersectionX_4F76A0(&scanXMin, &scanXMax, &scanY, &pBBox[0], &pBBox[1]) ||
        ComputeScanlineIntersectionX_4F76A0(&scanXMin, &scanXMax, &scanY, &pBBox[1], &pBBox[2]) ||
        ComputeScanlineIntersectionX_4F76A0(&scanXMin, &scanXMax, &scanY, &pBBox[2], &pBBox[3]) ||
        ComputeScanlineIntersectionX_4F76A0(&scanXMin, &scanXMax, &scanY, &pBBox[3], &pBBox[0]))
    {
        gRozza_679188.field_0_type = 1;
        gRozza_679188.field_4_mapx_t1 = scanXMin;
        gRozza_679188.field_8 = scanXMax;
        gRozza_679188.field_18_mapy_t1 = scanY;
        gRozza_679188.field_20_pSprite = 0;
        return 1;
    }
    return 0;
}

STUB_FUNC(0x5A0A70)
char_type Sprite::sub_5A0A70(Sprite_4C* a2, Sprite** a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A0EF0)
char_type Sprite::sub_5A0EF0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a1030)
char_type Sprite::sub_5A1030(Sprite* a2, Sprite** a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a1490)
bool Sprite::sub_5A1490(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a19c0)
char_type Sprite::sub_5A19C0()
{
    Update_4C_59F990();

    if (!field_4_0x4C_len->field_48_bBoxUpToDate)
    {
        field_4_0x4C_len->UpdateRotatedBoundingBox_5A3550(field_14_xy.x, field_14_xy.y, field_1C_zpos, field_0);
    }
    field_4_0x4C_len->SetCurrentRect_5A4D90();
    return gMap_0x370_6F6268->sub_4E4770(field_1C_zpos);
}

MATCH_FUNC(0x5a1a60)
char Sprite::sub_5A1A60()
{
    Update_4C_59F990();

    Sprite_4C* p4C = this->field_4_0x4C_len;

    if (!p4C->field_48_bBoxUpToDate)
    {
        p4C->UpdateRotatedBoundingBox_5A3550(this->field_14_xy.x, this->field_14_xy.y, this->field_1C_zpos, this->field_0);
    }

    field_4_0x4C_len->SetCurrentRect_5A4D90();

    if (gPurple_left_6F5FD4 < 0)
    {
        gPurple_left_6F5FD4 = 0;
    }

    if (gPurple_right_6F5B80 > 255)
    {
        gPurple_right_6F5B80 = 255;
    }

    if (gPurple_top_6F6108 < 0)
    {
        gPurple_top_6F6108 = 0;
    }

    if (gPurple_bottom_6F5F38 > 255)
    {
        gPurple_bottom_6F5F38 = 255;
    }

    return gMap_0x370_6F6268->sub_4E4630(field_1C_zpos);
}

MATCH_FUNC(0x5a1b30)
void Sprite::ResolveZOrder_5A1B30(Sprite* pOther)
{
    const char_type their_z = pOther->ComputeZLayer_5A1BD0();
    ComputeZLayer_5A1BD0();
    if (their_z < field_39_z_col)
    {
        pOther->field_39_z_col = field_39_z_col;
    }
    else
    {
        this->field_39_z_col = their_z;
    }
}

MATCH_FUNC(0x5a1bd0)
char_type Sprite::ComputeZLayer_5A1BD0()
{
    if (this->field_39_z_col == -1)
    {
        if (this->field_30_sprite_type_enum == sprite_types_enum::car && field_8_car_bc_ptr->IsTrainModel_403BA0())
        {
            this->field_39_z_col = (this->field_1C_zpos - dword_7035C4).ToInt();
        }
        else
        {
            this->field_39_z_col = (this->field_1C_zpos - dword_7035C4).ToInt() + sub_5A1A60();
        }
    }
    return field_39_z_col;
}

MATCH_FUNC(0x5a1ca0)
char_type Sprite::CheckCornerZCollisions_5A1CA0(u32* pCount)
{
    char_type flags;

    UpdateCollisionBoundsIfNeeded_59E9C0();

    Sprite_4C* p4C = field_C_sprite_4c_ptr;
    Fix16 v6 = field_1C_zpos - (p4C->field_8 / 2);
    Fix16 v7 = field_1C_zpos + (p4C->field_8 / 2);

    if (v7 > k_dword_7033C0)
    {
        v7 = k_dword_7033C0;
    }

    if (gMap_0x370_6F6268->CheckZCollisionAtCoord_4E5300(p4C->field_C_renderingRect[0].x, p4C->field_C_renderingRect[0].y, v6, v7))
    {
        flags = 1;
        *pCount = 1;
    }
    else
    {
        flags = 0;
        *pCount = 0;
    }

    if (gMap_0x370_6F6268->CheckZCollisionAtCoord_4E5300(p4C->field_C_renderingRect[1].x, p4C->field_C_renderingRect[1].y, v6, v7))
    {
        ++*pCount;
        flags |= 2u;
    }

    if (gMap_0x370_6F6268->CheckZCollisionAtCoord_4E5300(p4C->field_C_renderingRect[2].x, p4C->field_C_renderingRect[2].y, v6, v7))
    {
        ++*pCount;
        flags |= 4u;
    }

    if (gMap_0x370_6F6268->CheckZCollisionAtCoord_4E5300(p4C->field_C_renderingRect[3].x, p4C->field_C_renderingRect[3].y, v6, v7))
    {
        ++*pCount;
        flags |= 8;
    }

    return flags;
}

STUB_FUNC(0x5A1EB0)
char_type Sprite::IsTouchingSlopeBlock_5A1EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A21F0)
char_type Sprite::sub_5A21F0()
{
    NOT_IMPLEMENTED;

    Fix16 z_4c = this->field_C_sprite_4c_ptr->field_8; // which union type ??
    Fix16 zToUse = this->field_1C_zpos + z_4c / 2;
    if (zToUse > k_dword_7033C0)
    {
        zToUse = k_dword_7033C0;
    }
    return gMap_0x370_6F6268->CheckZCollisionAtCoord_4E5300(field_14_xy.x, field_14_xy.y, field_1C_zpos - z_4c / 2, zToUse);
}

STUB_FUNC(0x5A22B0)
Fix16 Sprite::MinDistanceToAnySpriteBBoxCorner_5A22B0(Sprite* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x5A2440)
char_type Sprite::sub_5A2440()
{
    WIP_IMPLEMENTED;

    UpdateCollisionBoundsIfNeeded_59E9C0();
    field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    char_type result = gMap_0x370_6F6268->CanSpriteEnterMovementRegion_4E4460(field_14_xy.x.ToInt(),
                                                                              field_14_xy.y.ToInt(),
                                                                              field_1C_zpos.ToInt(),
                                                                              this,
                                                                              2048);
    if (result)
    {
        gRozza_679188.field_1C_mapz = field_1C_zpos;
    }
    return result;
}

STUB_FUNC(0x5a2500)
char_type Sprite::sub_5A2500()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A26E0)
s16* Sprite::sub_5A26E0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a2710)
s32* Sprite::sub_5A2710(s32* a2, Sprite* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a29d0)
void Sprite::CreateSoundObj_5A29D0()
{
    if (!field_10_sound)
    {
        if (!bSkip_audio_67D6BE)
        {
            field_10_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Sprite_1);
        }
    }
}

MATCH_FUNC(0x59E930)
bool Sprite::IsObjectModelEqual_59E930(s32 model)
{
    return (Is2C_40FE80() && field_8_object_2C_ptr->field_18_model == model) ? true : false;
}

MATCH_FUNC(0x5a2a00)
void Sprite::FreeSound_5A2A00()
{
    if (field_10_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_10_sound);
        field_10_sound = 0;
    }
}

MATCH_FUNC(0x5a2a30)
void Sprite::ResolveCollisionWithCarPedOrObject_5A2A30()
{
    struct_4 collisions;
    collisions.field_0_p18 = 0;
    gPurpleDoom_1_679208->CollectRectCollisions_477F30(&field_C_sprite_4c_ptr->field_30_boundingBox, 0, 0, this, &collisions);
    for (Sprite_18* pCollisionIter = collisions.field_0_p18; pCollisionIter; pCollisionIter = pCollisionIter->mpNext)
    {
        Sprite* pCurrent = pCollisionIter->field_0;
        if (pCurrent->get_type_416B40() == sprite_types_enum::car)
        {
            Car_BC* pIterCar = pCurrent->AsCar_40FEB0();
            if (field_30_sprite_type_enum != sprite_types_enum::car ||
                (!pIterCar->IsCargoCarOf_4BA390(field_8_car_bc_ptr) && !pIterCar->HasOtherCarOnTrailer_475E60(field_8_car_bc_ptr)))
            {
                u8 x = field_14_xy.x.ToInt();
                u8 y = field_14_xy.y.ToInt();
                u8 z = field_1C_zpos.ToInt();

                // car shoving / overlap resolution ?
                if (gOrca_2FD4_6FDEF0->sub_5552B0(1, &x, &y, &z, 1))
                {
                    gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(pCurrent);
                    pIterCar->sub_444E40(x, y, z);
                    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(pCurrent);
                }
            }
        }
        else if (pCurrent->get_type_416B40() == sprite_types_enum::ped)
        {
            Char_B4* pB4 = pCurrent->AsCharB4_40FEA0();
            pB4->field_7C_pPed->Deallocate_45EB60();
        }
        else if (pCurrent->Is2C_40FE80())
        {
            Object_2C* pObj = pCurrent->As2C_40FEC0();
            pObj->Dealloc_5291B0();
        }
    }
}

MATCH_FUNC(0x5a2cf0)
void Sprite::PoolAllocate()
{
    this->field_2C = 0;
    this->field_28_num = 0;
    this->field_8_car_bc_ptr = 0;
    this->field_14_xy.x = gFix16_7035C0;
    this->field_14_xy.y = gFix16_7035C0;
    this->field_1C_zpos = gFix16_7035C0;
    this->field_39_z_col = -1;
    this->field_0 = gAng16_703804;
    this->field_22_sprite_id = 0;
    this->field_24_remap = 0;
    this->field_34 = 2;
    this->field_C_sprite_4c_ptr = 0;
    this->field_4_0x4C_len = 0;
    this->field_38_zoom = 0;
    this->field_10_sound = 0;

    // TODO: Probably an inline
    this->field_20_id = gSprite_8_703820->field_4_id_base++;
    if (!gSprite_8_703820->field_4_id_base)
    {
        gSprite_8_703820->field_4_id_base = 1;
    }
}

MATCH_FUNC(0x5a3030)
void Sprite::PoolDeallocate()
{
    FreeSprite4CChildren_59FAD0();
    this->field_20_id = 0;
    FreeSound_5A2A00();
}

MATCH_FUNC(0x5a3100)
void Sprite::DispatchCollisionEvent_5A3100(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 ang)
{
    Object_2C* o2c;

    switch (field_30_sprite_type_enum)
    {
        case sprite_types_enum::ped:
            field_8_char_b4_ptr->field_88_obj_2c.PushImpactEvent_5A6D00(pSprite, x, y, ang);
            break;
        case sprite_types_enum::car:
            field_8_car_bc_ptr->field_0_qq.PushImpactEvent_5A6D00(pSprite, x, y, ang);
            break;
        case 1: // sprite_type_1_Object_5C
        case 4: // sprite_type_4_Object_5C
        case 5: // sprite_type_5_Object_5C
            o2c = field_8_object_2C_ptr;
            if (!o2c->field_10_obj_3c)
            {
                o2c->EnsureObject3C_52A650();
            }
            field_8_object_2C_ptr->field_10_obj_3c->field_0.PushImpactEvent_5A6D00(pSprite, x, y, ang);
            break;
        default:
            break;
    }

    switch (pSprite->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car:
            pSprite->field_8_car_bc_ptr->Deactivate_43AA60();
            break;
        case 1: // sprite_type_1_Object_5C
        case 4: // sprite_type_4_Object_5C
        case 5: // sprite_type_5_Object_5C
            pSprite->field_8_object_2C_ptr->ReactivateObjectAfterImpact_52A6D0(this);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4833B0)
void Sprite::set_angle_4833B0(Ang16 ang)
{
    if (ang != field_0)
    {
        field_0 = ang;
        ResetZCollisionAndDebugBoxes_59E7B0();
    }
}

MATCH_FUNC(0x5a3540)
Sprite::~Sprite()
{
    FreeSound_5A2A00();
}

MATCH_FUNC(0x5a5e50)
Sprite::Sprite() : field_0(gAng16_703804)
{
    field_4_0x4C_len = NULL;
    field_14_xy.x = gFix16_7035C0;
    field_14_xy.y = gFix16_7035C0;
    field_1C_zpos = gFix16_7035C0;
    field_20_id = 0;
    field_22_sprite_id = 0;
    field_24_remap = 0;
    field_28_num = NULL;
    field_2C = 0;
    field_30_sprite_type_enum = 0;
    field_34 = 0;
    field_38_zoom = 0;
    field_39_z_col = -1;
    field_8_car_bc_ptr = NULL;
    mpNext = NULL;
    field_10_sound = NULL;
}

MATCH_FUNC(0x48F5A0)
void Sprite_14::sub_48F5A0()
{
    Sprite_3CC* pSprt = gSprite_3CC_67AF1C;
    s32 new_idx = ++pSprt->field_3C0;
    field_C = new_idx;
}

MATCH_FUNC(0x48F5C0)
EXPORT void Sprite_14::sub_48F5C0(u8 xCount, u8 yCount)
{
    u8* pData = this->field_0;
    for (s32 y = 0; y < yCount; y++)
    {
        memset(pData, 0, xCount);
        pData += 256;
    }
}

MATCH_FUNC(0x48f600)
Sprite_14* Sprite_3CC::sub_48F600(u16* a2, u32* a3, u32* a4, u16* a5)
{
    s32 final_idx;
    s32 start_idx = 0;
    Sprite_14* pSprt = NULL;
    if (!*a3)
    {
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (Sprite_14* pIter = &field_0[start_idx]; count < final_idx; count++, ++pIter)
    {
        s32 unk = *a2;
        if (pIter->field_4 == unk && pIter->field_12 == *a5)
        {
            if (pIter->field_8 == *a4)
            {
                return pIter;
            }
            if (pIter->field_8 < *a4 && (pIter->field_8 | (*a4 - pIter->field_8)) == *a4)
            {
                pSprt = pIter;
            }
        }
    }
    return pSprt;
}

MATCH_FUNC(0x48f690)
Sprite_14* Sprite_3CC::sub_48F690(u32* a2)
{
    u32 min_value = -1;
    s32 start_idx;
    s32 final_idx;
    Sprite_14* pSprt = NULL;
    if (!*a2)
    {
        start_idx = 0;
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (Sprite_14* pIter = &field_0[start_idx]; count < final_idx; count++, ++pIter)
    {
        if (pIter->field_C < min_value)
        {
            min_value = pIter->field_C;
            pSprt = pIter;
        }
    }
    return pSprt;
}

MATCH_FUNC(0x48f6e0)
void Sprite_3CC::InvalidateMasksByType_48F6E0(u16* a2)
{
    s32 count = 0;
    for (Sprite_14* pIter = &this->field_0[0]; count < 48; count++, ++pIter)
    {
        if (pIter->field_4 == *a2)
        {
            pIter->Invalidate_44AF70();
        }
    }
}

MATCH_FUNC(0x48f710)
void Sprite_3CC::InvalidateAllMasks_48F710()
{
    Sprite_14* psVar1 = field_0;
    s32 iVar2 = 0x30;
    do
    {
        psVar1->Invalidate_44AF70();
        psVar1++;
        iVar2--;
    } while (iVar2 != 0);
}

MATCH_FUNC(0x48f730)
Sprite_3CC::Sprite_3CC()
{
    u32 iVar4;
    field_3C0 = 0;
    field_3C4 = NULL;
    field_3C8 = NULL;

    // By the way this is later used, it seems to be an array of a structure of size 0x40.
    // For now, it's a s32* as it make the code to match.
    u8* pvVar2 = (u8*)Memory::Aligned_malloc_4FE510(0x40000, (void**)(&field_3C8));
    field_3C4 = pvVar2;

    Sprite_14* tmp = field_0;
    for (iVar4 = 8; iVar4 != 0; iVar4--, pvVar2 += 0x4000, tmp += 4)
    {
        tmp[0].field_0 = pvVar2;
        tmp[1].field_0 = (pvVar2 + 0x40);
        tmp[2].field_0 = (pvVar2 + 0x80);
        tmp[3].field_0 = (pvVar2 + 0xC0);
    }

    tmp = field_0 + 33;
    for (iVar4 = 4; iVar4 != 0; iVar4--, pvVar2 += 0x8000, tmp += 4)
    {
        // I don't know why this one starts at -1...
        // Maybe an artifact of the decompilation
        tmp[-1].field_0 = pvVar2;
        tmp[0].field_0 = (pvVar2 + 0x40);
        tmp[1].field_0 = (pvVar2 + 0x80);
        tmp[2].field_0 = (pvVar2 + 0xC0);
    }

    for (u16 uVar1 = 0; uVar1 < 48; uVar1++)
    {
        field_0[uVar1].field_10 = uVar1;
    }
}

MATCH_FUNC(0x48F7F0)
Sprite_3CC::~Sprite_3CC()
{
    if (this->field_3C8)
    {
        crt::free(this->field_3C8);
    }
    this->field_3C4 = 0;
    this->field_3C8 = 0;
}

MATCH_FUNC(0x5A4D90)
void Sprite_4C::SetCurrentRect_5A4D90()
{
    field_30_boundingBox.DoSetCurrentRect_59DD60();
}

// https://decomp.me/scratch/RAdGk
WIP_FUNC(0x5A3550)
void Sprite_4C::UpdateRotatedBoundingBox_5A3550(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    WIP_IMPLEMENTED;
    Fix16 width_over_2 = field_0_width / 2;
    Fix16 height_over_2 = field_4_height / 2;
    Fix16 unk_over_2 = field_8 / 2;

    Fix16_Point point = Fix16_Point(xpos, ypos);

    if (rotation == gAng16_703804) // = 0
    {
        // okay
        Fix16_Point northwest = Fix16_Point(-width_over_2, -height_over_2);
        Fix16_Point northeast = Fix16_Point(width_over_2, -height_over_2);
        Fix16_Point southeast = Fix16_Point(width_over_2, height_over_2);
        Fix16_Point southwest = Fix16_Point(-width_over_2, height_over_2);

        field_C_renderingRect[0] = point + northwest;
        field_C_renderingRect[1] = point + northeast;
        field_C_renderingRect[2] = point + southeast;
        field_C_renderingRect[3] = point + southwest;

        field_30_boundingBox = Fix16_Rect();
        field_30_boundingBox.SetRect_41E350(xpos - width_over_2, xpos + width_over_2, ypos - height_over_2, ypos + height_over_2);
    }
    else if (rotation == word_70344C) // = 360
    {
        // okay
        Fix16_Point southwest = Fix16_Point(-height_over_2, width_over_2);
        Fix16_Point northwest = Fix16_Point(-height_over_2, -width_over_2);
        Fix16_Point northeast = Fix16_Point(height_over_2, -width_over_2);
        Fix16_Point southeast = Fix16_Point(height_over_2, width_over_2);

        field_C_renderingRect[0] = point + southwest;
        field_C_renderingRect[1] = point + northwest;
        field_C_renderingRect[2] = point + northeast;
        field_C_renderingRect[3] = point + southeast;

        field_30_boundingBox = Fix16_Rect();
        field_30_boundingBox.SetRect_41E350(xpos - height_over_2, xpos + height_over_2, ypos - width_over_2, ypos + width_over_2);
    }
    else if (rotation == word_70351E) // = 720
    {
        // okay
        Fix16_Point southeast = Fix16_Point(width_over_2, height_over_2);
        Fix16_Point southwest = Fix16_Point(-width_over_2, height_over_2);
        Fix16_Point northwest = Fix16_Point(-width_over_2, -height_over_2);
        Fix16_Point northeast = Fix16_Point(width_over_2, -height_over_2);

        field_C_renderingRect[0] = point + southeast;
        field_C_renderingRect[1] = point + southwest;
        field_C_renderingRect[2] = point + northwest;
        field_C_renderingRect[3] = point + northeast;

        field_30_boundingBox = Fix16_Rect();
        field_30_boundingBox.SetRect_41E350(xpos - width_over_2, xpos + width_over_2, ypos - height_over_2, ypos + height_over_2);
    }
    else if (rotation == word_703544) // = 1080
    {
        // okay
        Fix16_Point northeast = Fix16_Point(height_over_2, -width_over_2);
        Fix16_Point southeast = Fix16_Point(height_over_2, width_over_2);
        Fix16_Point southwest = Fix16_Point(-height_over_2, width_over_2);
        Fix16_Point northwest = Fix16_Point(-height_over_2, -width_over_2);

        field_C_renderingRect[0] = point + northeast;
        field_C_renderingRect[1] = point + southeast;
        field_C_renderingRect[2] = point + southwest;
        field_C_renderingRect[3] = point + northwest;

        field_30_boundingBox = Fix16_Rect();
        field_30_boundingBox.SetRect_41E350(xpos - height_over_2, xpos + height_over_2, ypos - width_over_2, ypos + width_over_2);
    }
    else
    {
        //
        Fix16_Point northwest = Fix16_Point(-width_over_2, -height_over_2);
        Fix16_Point northeast = Fix16_Point(width_over_2, -height_over_2);
        Fix16_Point southeast = Fix16_Point(width_over_2, height_over_2);
        Fix16_Point southwest = Fix16_Point(-width_over_2, height_over_2);

        northwest.RotateByAngle_40F6B0(rotation);
        northeast.RotateByAngle_40F6B0(rotation);
        southeast.RotateByAngle_40F6B0(rotation);
        southwest.RotateByAngle_40F6B0(rotation);

        field_C_renderingRect[0] = point + northwest;
        field_C_renderingRect[1] = point + northeast;
        field_C_renderingRect[2] = point + southeast;
        field_C_renderingRect[3] = point + southwest;

        Fix16 left = field_30_boundingBox.field_0_left;
        Fix16 right = field_30_boundingBox.field_4_right;
        Fix16 top = field_30_boundingBox.field_8_top;
        Fix16 bottom = field_30_boundingBox.field_C_bottom;

        FindMinMax_5A57E0(left,
                          right,
                          field_C_renderingRect[0].x,
                          field_C_renderingRect[1].x,
                          field_C_renderingRect[2].x,
                          field_C_renderingRect[3].x);

        FindMinMax_5A57E0(top,
                          bottom,
                          field_C_renderingRect[0].y,
                          field_C_renderingRect[1].y,
                          field_C_renderingRect[2].y,
                          field_C_renderingRect[3].y);

        field_30_boundingBox.SetRect_5A5E30(left, right, top, bottom);
    }
    field_30_boundingBox.SetHiLowZ_41E370(zpos - unk_over_2, zpos + unk_over_2);
    field_48_bBoxUpToDate = true; // line 745

    return;
    /*
    // ????????????????????
    // not on 9.6f function:
    Fix16 y_negated = -ypos;
    Fix16 x_negated = -xpos;
    Fix16_Point_POD unk1 = Fix16_Point_POD(y_negated, x_negated); // ??
    Fix16_Point_POD unk2 = Fix16_Point_POD(y_negated, x_negated); // ??
    Fix16_Point_POD unk3 = Fix16_Point_POD(y_negated, x_negated); // ??
    Fix16_Point_POD unk4 = Fix16_Point_POD(y_negated, x_negated); // ??

    unk1.Rotate_562C20(rotation);
    unk2.Rotate_562C20(rotation);
    unk3.Rotate_562C20(rotation);
    unk4.Rotate_562C20(rotation);
    */
}

MATCH_FUNC(0x5a5860)
void Sprite_8::sub_5A5860()
{
    field_0 = 0;
}

MATCH_FUNC(0x5a5870)
void Sprite_8::sub_5A5870()
{
    gSprite_703814 = gSprite_Pool_703818->get_new_sprite();
    gSprite_703814->AllocInternal_59F950(gFix16_7035C0, gFix16_7035C0, gFix16_7035C0);
}

MATCH_FUNC(0x5a58a0)
Sprite_8::Sprite_8()
{
    gSprite_Pool_703818 = new Sprite_Pool();
    if (gSprite_Pool_703818 == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5039);
    }

    gSprite_4C_Pool_70381C = new Sprite_4C_Pool();
    if (gSprite_4C_Pool_70381C == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5041);
    }

    gSprite_3CC_67AF1C = new Sprite_3CC();
    if (gSprite_3CC_67AF1C == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5043);
    }

    gSprite_18_Pool_703B80 = new Sprite_18_Pool();
    if (gSprite_18_Pool_703B80 == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5046);
    }

    field_0 = 0;
    field_4_id_base = 1;
}

MATCH_FUNC(0x5a5b50)
Sprite_8::~Sprite_8()
{
    GTA2_DELETE_AND_NULL(gSprite_Pool_703818);
    GTA2_DELETE_AND_NULL(gSprite_4C_Pool_70381C);
    GTA2_DELETE_AND_NULL(gSprite_3CC_67AF1C);
    GTA2_DELETE_AND_NULL(gSprite_18_Pool_703B80);
    gSprite_703814 = 0;
}

MATCH_FUNC(0x5a5c40)
Sprite_18::~Sprite_18()
{
}

MATCH_FUNC(0x5a5c50)
Sprite_18::Sprite_18()
{
    field_10 = 0;
}

MATCH_FUNC(0x5a5c20)
Sprite_18_Pool::~Sprite_18_Pool()
{
}

MATCH_FUNC(0x5a57a0)
s32 Sprite_4C::PoolAllocate()
{
    mpNext = NULL;
    field_48_bBoxUpToDate = false;
    return 0;
}

MATCH_FUNC(0x5a57b0)
Sprite_4C::Sprite_4C()
{
    PoolAllocate();
}

MATCH_FUNC(0x5a5840)
Sprite_4C::~Sprite_4C()
{
}

MATCH_FUNC(0x5a5be0)
Sprite_4C_Pool::~Sprite_4C_Pool()
{
}

MATCH_FUNC(0x5A5C00)
Sprite_Pool::~Sprite_Pool()
{
}

MATCH_FUNC(0x5A6A20)
void Sprite_18::sub_5A6A20()
{
    Car_BC* cBC = this->field_0->AsCar_40FEB0();
    if (cBC)
    {
        if (cBC->field_88 != 2 && cBC->field_88 != 4 && cBC->field_88 != 3)
        {
            cBC->sub_43DD60();
        }
    }
}

MATCH_FUNC(0x5A69E0)
void Sprite_18::sub_5A69E0()
{
    Object_2C* pO2c = field_0->As2C_40FEC0();
    if (pO2c)
    {
        pO2c->sub_525100();
    }
    else
    {
        Car_BC* pBC = field_0->AsCar_40FEB0();
        if (pBC)
        {
            pBC->sub_43D7B0(19);
        }
    }
}

MATCH_FUNC(0x5A6910)
bool Sprite_18::PoolUpdate_5A6910(Sprite* a2)
{
    bool bRet;
    Object_2C* o2c = field_0->As2C_40FEC0();
    if (o2c)
    {
        bRet = o2c->sub_527070(a2, field_6_x, field_8_y, field_10);
        if (bRet)
        {
            gObject_2C_Pool_6F8F80->DeAllocate(o2c);
            return bRet;
        }
    }
    else
    {
        Car_BC* cBC = field_0->AsCar_40FEB0();
        if (cBC)
        {
            bRet = cBC->sub_443360(a2, field_6_x, field_8_y, field_10);
            if (bRet)
            {
                gCar_BC_Pool_67792C->Remove(cBC);
            }
        }
        else
        {
            Char_B4* cB4 = field_0->AsCharB4_40FEA0();
            bRet = 0;
            cB4->sub_545720(dword_703A38);
        }
    }
    return bRet;
}
