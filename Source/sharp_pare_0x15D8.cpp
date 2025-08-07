#include "sharp_pare_0x15D8.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include "memory.hpp"

DEFINE_GLOBAL(sharp_pare_0x15D8*, gSharp_pare_0x15D8_705064, 0x705064);
DEFINE_GLOBAL(u32, dword_704ED0, 0x704ED0);
DEFINE_GLOBAL(u32, dword_704F28, 0x704F28);

// FUNCTION: 105 0x5B90F0
void sharp_pare_0x15D8::LoadPals_5B90F0()
{
    field_15D6_pal_count = gGtx_0x106C_703DD4->get_physical_palettes_len_5AA900();

    for (u16 palId = 0; palId < field_15D6_pal_count; palId++)
    {
        pgbh_RegisterPalette(palId, (DWORD*)gGtx_0x106C_703DD4->GetPalData_5AA6A0(palId));
    }
}

// FUNCTION: 105 0x5B9140
void sharp_pare_0x15D8::FreePals_5B9140()
{
    for (u16 i = 0; i < field_15D6_pal_count; ++i)
    {
        pgbh_FreePalette(i);
    }
    field_15D6_pal_count = 0;
}

// FUNCTION: 105 0x5B9180
void sharp_pare_0x15D8::LoadTextures2_5B9180()
{
    u16 width_height = 64;
    if (gGtx_0x106C_703DD4->sub_5AA4F0(2))
    {
        field_1000_bFreeTextures2 = 1;
        u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(2, 0);
        u16 v4 = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, v3);
        u16 pal_idx = (u16)gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(v4);

        for (u32 idx = 0; idx < 48; idx++)
        {
            if (idx == 32)
            {
                width_height = 128;
            }

            field_1004_textures2[idx] = pgbh_RegisterTexture(width_height, 
                width_height, 
                gSprite_3CC_67AF1C->get_s14(idx), 
                pal_idx, 
                0);
        }
    }
}

// FUNCTION: 105 0x5B9220
s16 sharp_pare_0x15D8::sub_5B9220(u16 a2, u16 a3)
{
    const u16 og_idx = field_15D4_idx;
    const s16 v4 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, a3);
    sprite_index* sprite_index_5AA440 = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v4);
    u8* field_0_pData = sprite_index_5AA440->field_0_pData;
    const u16 phys_pal_5AA6F0 = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, v4));

    field_15D4_idx += a2;

    for (s32 i = 0; i < a2; i++)
    {
        field_10C4[og_idx + i].field_4_pTexture =
            pgbh_RegisterTexture(sprite_index_5AA440->field_4_width, sprite_index_5AA440->field_5_height, field_0_pData, phys_pal_5AA6F0, 0);
        field_10C4[og_idx + i].field_0_pPixelData = field_0_pData;
    }
    return og_idx;
}

// FUNCTION: 105 0x5B92E0
void sharp_pare_0x15D8::ReadTextures_5B92E0()
{
    if (gGtx_0x106C_703DD4->has_tiles_4C2EE0())
    {
        field_1001_bFreeTextures1 = 1;
        STexture** pIter = field_0_textures1;
        u16 i = 0;
        while (i < GTA2_COUNTOF(field_0_textures1))
        {
            // 64 256x256 pages of 64x64 8 bit tiles
            *pIter = pgbh_RegisterTexture(64, 64, gGtx_0x106C_703DD4->get_tile_4C2EB0(i), gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(i), 0);
            i++;
            pIter++;
        }
    }
}

// FUNCTION: 105 0x5B9350
void sharp_pare_0x15D8::sub_5B9350()
{
    LoadPals_5B90F0();
    ReadTextures_5B92E0();

    field_1548_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA560(2), 1, 0, 2);
    field_1548_unk.LoadTextures_5B8F00();

    field_155C_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(2), gGtx_0x106C_703DD4->sub_5AA560(3), 2, 3);

    field_155C_unk.sub_5B8F70();
    field_1570_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(3), gGtx_0x106C_703DD4->sub_5AA560(4), 3, 4);
    field_1570_unk.sub_5B8F70();

    field_1584_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(4), gGtx_0x106C_703DD4->sub_5AA560(5), 4, 5);
    field_1584_unk.sub_5B8F70();

    field_1598_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(5), gGtx_0x106C_703DD4->sub_5AA560(6), 5, 6);
    field_1598_unk.sub_5B8F70();

    field_15AC_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(7), gGtx_0x106C_703DD4->sub_5AA560(8), 7, 8);
    field_15AC_unk.sub_5B8F70();

    field_15C0_unk.Alloc_5B8E90(gGtx_0x106C_703DD4->sub_5AA4F0(6), gGtx_0x106C_703DD4->sub_5AA560(7), 6, 7);
    field_15C0_unk.sub_5B8F70();

    LoadTextures2_5B9180();

    field_1544_pTexture = pgbh_RegisterTexture(128, 128, 0, 0, 0);
}

// FUNCTION: 105 0x5B94F0
STexture* sharp_pare_0x15D8::sub_5B94F0(s32 type, s32 pal, s32 kind, s32 a5)
{
    STexture* result;

    switch (kind)
    {
        case 2:
            result = field_1548_unk.get_texture_5B90A0(type, pal);
            break;
        case 3:
            result = field_155C_unk.sub_5B90D0(pal, a5);
            break;
        case 4:
            result = field_1570_unk.sub_5B90D0(pal, a5);
            break;
        case 5:
            result = field_1584_unk.sub_5B90D0(pal, a5);
            break;
        case 6:
            result = field_1598_unk.sub_5B90D0(pal, a5);
            break;
        case 8:
            result = field_15AC_unk.sub_5B90D0(pal, a5);
            break;
        case 7:
            result = field_15C0_unk.sub_5B90D0(pal, a5);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// FUNCTION: 105 0x5B95D0
STexture* sharp_pare_0x15D8::GetTexture2_5B95D0(u16 textureIdx)
{
    return field_1004_textures2[textureIdx];
}

// FUNCTION: 105 0x5B95F0
STexture* sharp_pare_0x15D8::sub_5B95F0(u16 idx, u16 width, u16 height)
{
    optimistic_moser* pMoser = &field_10C4[idx];
    STexture* pTexture = pMoser->field_4_pTexture;

    if (height != pMoser->field_8_w || width != pMoser->field_A_h)
    {
        STexture* pTextureInternal = pMoser->field_4_pTexture;
        pMoser->field_8_w = height;
        pMoser->field_A_h = width;
        pgbh_LockTexture(pTextureInternal);
        pTexture->field_14_original_pixel_data_ptr = &pMoser->field_0_pPixelData[256 * width];
        pTexture->field_10_height = height;
        pgbh_UnlockTexture(pTexture);
    }
    return pTexture;
}

// FUNCTION: 105 0x5B9660
void sharp_pare_0x15D8::sub_5B9660(u16 texture_idx, u16 pal_idx)
{
    STexture* pTexture = field_10C4[texture_idx].field_4_pTexture;
    u16 pal = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(pal_idx);
    pgbh_LockTexture(pTexture);
    pgbh_AssignPalette(pTexture, pal);
    pgbh_UnlockTexture(pTexture);
}

// FUNCTION: 105 0x5B96B0
void sharp_pare_0x15D8::sub_5B96B0(u16 a4, u16 new_width, u16 new_height, u16 pal)
{
    u16 pal_idx = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(pal);
    STexture* pTexture = field_1004_textures2[a4];
    pgbh_LockTexture(pTexture);
    pTexture->field_E_width = new_width;
    pTexture->field_10_height = new_height;
    pgbh_AssignPalette(pTexture, pal_idx);
    pgbh_UnlockTexture(pTexture);
}

// FUNCTION: 105 0x5B9710
STexture* sharp_pare_0x15D8::sub_5B9710(s16 a2, s16 a4, u8* a3, u16 a5)
{
    u16 phys_pal_5AA6F0; // di

    phys_pal_5AA6F0 = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(a5);
    pgbh_LockTexture(field_1544_pTexture);
    field_1544_pTexture->field_14_original_pixel_data_ptr = a3;
    field_1544_pTexture->field_E_width = a2;
    field_1544_pTexture->field_10_height = a4;
    pgbh_AssignPalette(field_1544_pTexture, phys_pal_5AA6F0);
    pgbh_UnlockTexture(field_1544_pTexture);
    return field_1544_pTexture;
}

// FUNCTION: 105 0x5B9050
festive_hopper::~festive_hopper()
{
    if (field_10_bDoFree && field_0_pAlloc)
    {
        for (u16 i = 0; i < field_4_item_alloc_count; ++i)
        {
            pgbh_FreeTexture(field_0_pAlloc[i]);
        }

        crt::free(field_0_pAlloc);
        field_0_pAlloc = 0;
    }
}

// FUNCTION: 105 0x5B8E90
void festive_hopper::Alloc_5B8E90(s16 size, s16 count, s32 pal_type1, s32 pal_type2)
{
    if (!field_0_pAlloc)
    {
        if (size)
        {
            field_4_item_alloc_count = count * size;
            field_6_count = count;
            field_10_bDoFree = 1;
            field_8_pal_type = pal_type1;
            field_C_pal_type = pal_type2;
            field_0_pAlloc = (STexture**)Memory::malloc_4FE4D0(sizeof(STexture*) * field_4_item_alloc_count);

            for (u32 i = 0; i < field_4_item_alloc_count; i++)
            {
                field_0_pAlloc[i] = 0;
            }
        }
    }
}

// FUNCTION: 105 0x5B8F70
void festive_hopper::sub_5B8F70()
{
    if (field_10_bDoFree)
    {
        const u32 palTotal = field_4_item_alloc_count / field_6_count;
        for (u32 pal_idx = 0; pal_idx < palTotal; pal_idx++)
        {
            const u16 converted_pal_sprite_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_8_pal_type, pal_idx);
            sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(converted_pal_sprite_pal);

            dword_704ED0++;

            for (u32 texture_idx = 0; texture_idx < field_6_count; texture_idx++)
            {
                const s16 converted_pal_idx = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(field_C_pal_type, texture_idx);
                const u16 physPal = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(converted_pal_idx);
                field_0_pAlloc[texture_idx + (pal_idx * field_6_count)] =
                    pgbh_RegisterTexture(pSpriteIndex->field_4_width, pSpriteIndex->field_5_height, pSpriteIndex->field_0_pData, physPal, 1);

                dword_704F28++;
            }
        }
    }
}

// FUNCTION: 105 0x5B8F00
void festive_hopper::LoadTextures_5B8F00()
{
    if (field_10_bDoFree)
    {
        u32 i = 0;
        sprite_index* pSpriteIndex;
        u16 tmp;
        u16 t2;
        while (i < field_4_item_alloc_count)
        {
            pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(i);
            tmp = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, i);
            t2 = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(tmp);

            field_0_pAlloc[i++] = pgbh_RegisterTexture(
                pSpriteIndex
                    ->field_4_width, // note: missing xor of register due to passing BYTE -> BYTE param instead of BYTE -> s32 param, xor clears up 24 bits
                pSpriteIndex->field_5_height,
                pSpriteIndex->field_0_pData,
                t2, // pal idx
                0);
        }
    }
}

// FUNCTION: 105 0x5B90A0
STexture* festive_hopper::get_texture_5B90A0(s32 type, s16 sprite_pal)
{
    return field_0_pAlloc[gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(type, sprite_pal)];
}

// FUNCTION: 105 0x5B90D0
STexture* festive_hopper::sub_5B90D0(s16 a2, s16 a3)
{
    return field_0_pAlloc[(u16)(a3 + (a2 * field_6_count))];
}

// FUNCTION: 105 0x5B9790
sharp_pare_0x15D8::sharp_pare_0x15D8()
{

    field_1000_bFreeTextures2 = 0;
    field_1001_bFreeTextures1 = 0;
    field_15D4_idx = 0;
    field_15D6_pal_count = 0;
    s32 i;

    for (i = 0; i < GTA2_COUNTOF(field_0_textures1); i++)
    {
        field_0_textures1[i] = 0;
    }

    for (i = 0; i < GTA2_COUNTOF(field_1004_textures2); i++)
    {
        field_1004_textures2[i] = 0;
    }
    field_1544_pTexture = 0;
}

// FUNCTION: 105 0x5B9900
sharp_pare_0x15D8::~sharp_pare_0x15D8()
{
    if (field_1001_bFreeTextures1)
    {
        for (s32 i = 0; i < GTA2_COUNTOF(field_0_textures1); i++)
        {
            if (field_0_textures1[i])
            {
                pgbh_FreeTexture(field_0_textures1[i]);
                field_0_textures1[i] = 0;
            }
        }
    }

    if (field_1000_bFreeTextures2)
    {
        for (s32 i = 0; i < GTA2_COUNTOF(field_1004_textures2); i++)
        {
            if (field_1004_textures2[i])
            {
                pgbh_FreeTexture(field_1004_textures2[i]);
                field_1004_textures2[i] = 0;
            }
        }
    }

    for (s32 i = 0; i < GTA2_COUNTOF(field_10C4); i++)
    {
        if (field_10C4[i].field_4_pTexture)
        {
            pgbh_FreeTexture(field_10C4[i].field_4_pTexture);
            field_10C4[i].field_4_pTexture = 0;
        }
    }

    if (field_1544_pTexture)
    {
        pgbh_FreeTexture(field_1544_pTexture);
        field_1544_pTexture = 0;
    }

    FreePals_5B9140();
}