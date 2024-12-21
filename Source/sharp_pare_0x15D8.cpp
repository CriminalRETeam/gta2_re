#include "sharp_pare_0x15D8.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"

EXPORT_VAR sharp_pare_0x15D8* gSharp_pare_0x15D8_705064;
GLOBAL(gSharp_pare_0x15D8_705064, 0x705064);

EXPORT_VAR u32 dword_704ED0;
GLOBAL(dword_704ED0, 0x704ED0);

EXPORT_VAR u32 dword_704F28;
GLOBAL(dword_704F28, 0x704F28);

MATCH_FUNC(0x5B90F0)
void sharp_pare_0x15D8::LoadPals_5B90F0()
{
    field_15D6_pal_count = gGtx_0x106C_703DD4->get_physical_palettes_len_5AA900();

    for (u16 palId = 0; palId < field_15D6_pal_count; palId++)
    {
        gbh_RegisterPalette(palId, gGtx_0x106C_703DD4->GetPalData_5AA6A0(palId));
    }
}

MATCH_FUNC(0x5B9140)
void sharp_pare_0x15D8::FreePals_5B9140()
{
    for (u16 i = 0; i < field_15D6_pal_count; ++i)
    {
        gbh_FreePalette(i);
    }
    field_15D6_pal_count = 0;
}

STUB_FUNC(0x5B9180)
void sharp_pare_0x15D8::LoadTextures2_5B9180()
{
    // todo
    u16 width_height; // bp
    s16 v3; // ax
    s16 v4; // ax
    u32 idx; // esi
    s32 pal_idx; // edi

    width_height = 64;
    if (gGtx_0x106C_703DD4->sub_5AA4F0(2))
    {
        /*
        this->field_1000_bFreeTextures2 = 1;
        v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(2, 0);
        v4 = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, v3);
        idx = 0;
        pal_idx = (u16)gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(v4);
        do
        {
            if (idx == 32)
            {
                width_height = 128;
            }

            this->field_1004_textures2[idx] = gbh_RegisterTexture(
                width_height,
                width_height,
                gKind_khorana_0x3CC_67AF1C->field_0[(u16)idx].field_0_pTexturePixelData,
                pal_idx,
                0);
            ++idx;
        } while (idx < 48);
        */
    }
}

STUB_FUNC(0x5B9220)
s16 sub_5B9220(s16 a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x5B92E0)
void sharp_pare_0x15D8::ReadTextures_5B92E0()
{
    if (gGtx_0x106C_703DD4->has_tiles_4C2EE0())
    {
        field_1001_bFreeTextures1 = 1;
        for (u16 i = 0; i < GTA2_COUNTOF(field_0_textures1); i++) // TODO: = 0 xor instruction swapped
        {
            // 64 256x256 pages of 64x64 8 bit tiles
            field_0_textures1[i] =
                gbh_RegisterTexture(64, 64, gGtx_0x106C_703DD4->get_tile_4C2EB0(i), gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(i), 0);
        }
    }
}

STUB_FUNC(0x5B9350)
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

    field_1544_pTexture = gbh_RegisterTexture(128, 128, 0, 0, 0);
}

MATCH_FUNC(0x5B94F0)
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

STUB_FUNC(0x5B95D0)
STexture* sharp_pare_0x15D8::GetTexture2_5B95D0(u16 textureIdx)
{
    return 0;
}

STUB_FUNC(0x5B95F0)
STexture* sharp_pare_0x15D8::sub_5B95F0(u16 a2, u16 a3, s16 a4)
{
    return 0;
}

STUB_FUNC(0x5B9660)
s32 sharp_pare_0x15D8::sub_5B9660(sharp_pare_0x15D8* a1, u16 a2)
{
    return 0;
}

STUB_FUNC(0x5B96B0)
s32 sharp_pare_0x15D8::sub_5B96B0(u16 a2, s16 a3, s32 a4, u16 a5)
{
    return 0;
}

STUB_FUNC(0x5B9710)
STexture* sharp_pare_0x15D8::sub_5B9710(s16 a2, s32 a3, s32 a4, u16 a5)
{
    return 0;
}

MATCH_FUNC(0x5B9050)
void festive_hopper::Free_5B9050()
{
    if (field_10_bDoFree && field_0_pAlloc)
    {
        for (u16 i = 0; i < field_4_item_alloc_count; ++i)
        {
            gbh_FreeTexture(field_0_pAlloc[i]);
        }

        crt::free(field_0_pAlloc);
        field_0_pAlloc = 0;
    }
}

STUB_FUNC(0x5B8E90)
void festive_hopper::Alloc_5B8E90(s16 a2, s16 a3, s32 a4, s32 a5)
{
    if (!this->field_0_pAlloc)
    {
        if (a2)
        {
            this->field_4_item_alloc_count = a3 * a2;
            this->field_6_count = a3;
            this->field_10_bDoFree = 1;
            this->field_8_pal_type = a4;
            this->field_C_pal_type = a5;
            this->field_0_pAlloc = (STexture**)malloc(sizeof(STexture*) * (u16)(a3 * a2));

            u32 i = 0;
            while (i < this->field_4_item_alloc_count)
            {
                this->field_0_pAlloc[i++] = 0;
            }
        }
    }
}

MATCH_FUNC(0x5B8F70)
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
                    gbh_RegisterTexture(pSpriteIndex->field_4_width, pSpriteIndex->field_5_height, pSpriteIndex->field_0_pData, physPal, 1);

                dword_704F28++;
            }
        }
    }
}

STUB_FUNC(0x5B8F00)
void festive_hopper::LoadTextures_5B8F00()
{
    if (this->field_10_bDoFree)
    {
        u32 i = 0;
        sprite_index* pSpriteIndex;
        u16 tmp;
        u16 t2;
        while (i < this->field_4_item_alloc_count)
        {
            pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(i);
            tmp = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, i);
            t2 = gGtx_0x106C_703DD4->get_phys_pal_5AA6F0(tmp);

            this->field_0_pAlloc[i++] = gbh_RegisterTexture(
                pSpriteIndex
                    ->field_4_width, // note: missing xor of register due to passing BYTE -> BYTE param instead of BYTE -> s32 param, xor clears up 24 bits
                pSpriteIndex->field_5_height,
                pSpriteIndex->field_0_pData,
                t2, // pal idx
                0);
        }
    }
}

MATCH_FUNC(0x5B90A0)
STexture* festive_hopper::get_texture_5B90A0(s32 type, s16 sprite_pal)
{
    return field_0_pAlloc[gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(type, sprite_pal)];
}

MATCH_FUNC(0x5B90D0)
STexture* festive_hopper::sub_5B90D0(s16 a2, s16 a3)
{
    return field_0_pAlloc[(u16)(a3 + (a2 * field_6_count))];
}

MATCH_FUNC(0x5B9790)
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

STUB_FUNC(0x5B9900)
sharp_pare_0x15D8::~sharp_pare_0x15D8()
{
    STexture** pIter1; // esi
    s32 k1024Counter; // ebp
    STexture** pIter2; // esi
    s32 k48Counter; // ebp
    STexture** pIter3; // esi
    s32 k96Counter; // ebp

    if (this->field_1001_bFreeTextures1)
    {
        pIter1 = this->field_0_textures1;
        k1024Counter = 1024;
        do
        {
            if (*pIter1)
            {
                gbh_FreeTexture(*pIter1);
                *pIter1 = 0;
            }
            ++pIter1;
            --k1024Counter;
        } while (k1024Counter);
    }

    if (this->field_1000_bFreeTextures2)
    {
        pIter2 = this->field_1004_textures2;
        k48Counter = 48;
        do
        {
            if (*pIter2)
            {
                gbh_FreeTexture(*pIter2);
                *pIter2 = 0;
            }
            ++pIter2;
            --k48Counter;
        } while (k48Counter);
    }

    pIter3 = &this->field_10C4[0].field_4_pTexture;
    k96Counter = 96;
    do
    {
        if (*pIter3)
        {
            gbh_FreeTexture(*pIter3);
            *pIter3 = 0;
        }
        pIter3 += 3;
        --k96Counter;
    } while (k96Counter);

    if (this->field_1544_pTexture)
    {
        gbh_FreeTexture(this->field_1544_pTexture);
        this->field_1544_pTexture = 0;
    }

    FreePals_5B9140();

    field_15C0_unk.Free_5B9050();
    field_15AC_unk.Free_5B9050();
    field_1598_unk.Free_5B9050();
    field_1584_unk.Free_5B9050();
    field_1570_unk.Free_5B9050();
    field_155C_unk.Free_5B9050();
    field_1548_unk.Free_5B9050();
}