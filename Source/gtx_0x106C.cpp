#include "gtx_0x106C.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "chunk.hpp"
#include "crt_stubs.hpp"
#include "error.hpp"
#include "file.hpp"
#include "laughing_blackwell_0x1EB54.hpp" // TODO: for some globals, prob should be put some where else
#include "memory.hpp"
#include <stdio.h>
#include <string.h>
#include <windows.h>

EXPORT_VAR gtx_0x106C* gGtx_0x106C_703DD4;
GLOBAL(gGtx_0x106C_703DD4, 0x703DD4);

EXPORT_VAR s16 word_703D9C;
GLOBAL(word_703D9C, 0x703D9C);

EXPORT_VAR s16 word_703D9A;
GLOBAL(word_703D9A, 0x703D9A);

EXPORT_VAR s16 word_703C3E;
GLOBAL(word_703C3E, 0x703C3E);

EXPORT_VAR s16 word_703DA4;
GLOBAL(word_703DA4, 0x703DA4);

EXPORT_VAR s16 word_703BAA;
GLOBAL(word_703BAA, 0x703BAA);

EXPORT_VAR s16 word_703D98;
GLOBAL(word_703D98, 0x703D98);

EXPORT_VAR s16 word_703C9C;
GLOBAL(word_703C9C, 0x703C9C);

STUB_FUNC(0x5abaa0)
u8 sprite_index::sub_5ABAA0(char_type a2)
{
    return 0;
}

STUB_FUNC(0x5abb00)
void sprite_index::sub_5ABB00(s32 a2)
{
}

MATCH_FUNC(0x5AA3B0)
car_info* gtx_0x106C::get_car_info_5AA3B0(u8 idx)
{
    return field_5C_cari->field_0[idx];
}

MATCH_FUNC(0x5AA3D0)
BYTE* gtx_0x106C::get_car_remap_5AA3D0(u8 idx)
{
    car_info* pCarInfo = field_5C_cari->field_0[idx];
    return &pCarInfo->remap[pCarInfo->num_remaps];
}

STUB_FUNC(0x5AA3F0)
s32 sub_5AA3F0(u16 a2, u8 a3)
{
    return 0;
}

STUB_FUNC(0x5AA460)
u16 gtx_0x106C::convert_sprite_pal_5AA460(s32 type, s16 sprite_pal)
{
    u16 result;
    switch (type)
    {
        case 2:
            result = sprite_pal + field_14_sprite_base2->field_0_car;
            break;
        case 3:
            result = sprite_pal + field_14_sprite_base2->field_2_ped;
            break;
        case 4:
        case 8:
            result = sprite_pal + field_14_sprite_base2->field_4_code_obj;
            break;
        case 5:
            result = sprite_pal + field_14_sprite_base2->field_6_map_obj;
            break;
        case 6:
            result = sprite_pal + field_14_sprite_base2->field_8_user;
            break;
        case 7:
            result = sprite_pal + field_14_sprite_base2->field_A_font;
            break;
        default:
            return sprite_pal;
    }
    return result;
}

MATCH_FUNC(0x5AA4F0)
s16 gtx_0x106C::sub_5AA4F0(s32 a2)
{
    switch (a2)
    {
        case 2:
            return field_18_sprite_base1->field_0_car;
        case 3:
            return field_18_sprite_base1->field_2_ped;
        case 4:
        case 8:
            return field_18_sprite_base1->field_4_code_obj;
        case 5:
            return field_18_sprite_base1->field_6_map_obj;
        case 7:
            return field_18_sprite_base1->field_A_font;
        case 6:
            return field_18_sprite_base1->field_8_user;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5AA560)
s16 gtx_0x106C::sub_5AA560(s32 a2)
{
    switch (a2)
    {
        case 1:
            return field_10_palette_base1->field_0_tile;
        case 2:
            return field_10_palette_base1->field_2_sprite;
        case 3:
            return field_10_palette_base1->field_4_car_remap;
        case 4:
            return field_10_palette_base1->field_6_red_remap;
        case 5:
            return field_10_palette_base1->field_8_code_obj_remap;
        case 6:
            return field_10_palette_base1->field_A_map_obj_remap;
        case 7:
            return field_10_palette_base1->field_C_user_remap;
        case 8:
            return field_10_palette_base1->field_E_font_remap;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5AA5F0)
s16 gtx_0x106C::convert_pal_type_5AA5F0(s32 type, s16 pal)
{
    s16 result; // ax

    switch (type)
    {
        case 1:
            result = pal + field_C_palette_base2->field_0_tile;
            break;
        case 2:
            result = pal + field_C_palette_base2->field_2_sprite;
            break;
        case 3:
            result = pal + field_C_palette_base2->field_4_car_remap;
            break;
        case 4:
            result = pal + field_C_palette_base2->field_6_red_remap;
            break;
        case 5:
            result = pal + field_C_palette_base2->field_8_code_obj_remap;
            break;
        case 6:
            result = pal + field_C_palette_base2->field_A_map_obj_remap;
            break;
        case 7:
            result = pal + field_C_palette_base2->field_C_user_remap;
            break;
        case 8:
            result = pal + field_C_palette_base2->field_E_font_remap;
            break;
        default:
            result = pal;
            break;
    }
    return result;
}

MATCH_FUNC(0x5AA6A0)
BYTE* gtx_0x106C::GetPalData_5AA6A0(u16 a2)
{
    return (BYTE*)field_2C_physical_palettes + 4 * ((a2 & 63) + ((a2 & 0xFFFFC0) << 8));
}

MATCH_FUNC(0x5AA6F0)
u16 gtx_0x106C::get_phys_pal_5AA6F0(u16 palId)
{
    return field_28_palette_index->field_0_phys_palette[palId];
}

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define UNIQUE_FUNC printf(__FILE__ LINE_STRING)

MATCH_FUNC(0x5AA710)
u16 gtx_0x106C::sub_5AA710(u16 a2, s16 a3)
{
    u16 result = field_1C_font_base->field_2_base[a2] + a3;
    if (a2 == (u16)field_1C_font_base->field_0_font_count - 1)
    {
        if (result >= field_2_font_base_total)
        {
            return field_1C_font_base->field_2_base[a2];
        }
        else
        {
            return result;
        }
    }
    else if (result >= (u32)field_1C_font_base->field_2_base[a2 + 1])
    {
        return field_1C_font_base->field_2_base[a2];
    }
    return result;
}

MATCH_FUNC(0x5AA760)
u16 gtx_0x106C::sub_5AA760(u16* a2, wchar_t* a3)
{
    if (*a2 >= 0x65u)
    {
        return (*a2 < 0xC9u) ? 0x10 : 0x20;
    }
    else
    {
        return this->field_20_sprite_index[(u16)this->field_14_sprite_base2->field_A_font 
                                    + gtx_0x106C::sub_5AA710(*a2, *a3 - 33)].field_4_width;
    }
}

MATCH_FUNC(0x5AA7B0)
u16 gtx_0x106C::space_width_5AA7B0(u16* a2)
{
    if (*a2 >= 101)
    {
        return (*a2 < 0xc9) ? 0x10 : 0x20;
    }

    return field_20_sprite_index[field_14_sprite_base2->field_A_font + 77 + field_1C_font_base->field_2_base[*a2]].field_4_width;
}

MATCH_FUNC(0x5AA800)
s16 gtx_0x106C::sub_5AA800(u16* a2)
{
    if (*a2 >= 0x65u)
    {
        return (*a2 < 0xC9u) ? 0x11 : 0x22;
    }
    else
    {
        return this->field_20_sprite_index[this->field_14_sprite_base2->field_A_font + 32 
                        + this->field_1C_font_base->field_2_base[*a2]].field_5_height;
    }
}

MATCH_FUNC(0x5AA850)
bool gtx_0x106C::sub_5AA850(u16 tile_idx)
{
    return field_40_tile->field_0[tile_idx] != tile_idx;
}

MATCH_FUNC(0x5AA870)
s16 gtx_0x106C::sub_5AA870(u16 tile_idx)
{
    return field_40_tile->field_0[tile_idx];
}

STUB_FUNC(0x5AA890)
s16 gtx_0x106C::sub_5AA890()
{
    s16 internal_idx; // ax
    u16* i; // ecx

    internal_idx = 1023;
    for (i = &field_40_tile->field_0[1023]; *i; --i)
    {
        if ((u16)--internal_idx < 992u)
            return 0;
    }
    return internal_idx;
}

MATCH_FUNC(0x5AA8C0)
void* gtx_0x106C::GetTiles_5AA8C0()
{
    return field_3C_tiles;
}

MATCH_FUNC(0x5AA900)
s16 gtx_0x106C::get_physical_palettes_len_5AA900()
{
    return field_8_physical_palettes_len;
}

MATCH_FUNC(0x5AA910)
object_info* gtx_0x106C::get_map_object_info_5AA910(u16 idx)
{
    if (idx >= field_6_map_object_info_len)
    {
        return 0;
    }
    return &field_24_map_object_info[idx];
}

MATCH_FUNC(0x5AA930)
void gtx_0x106C::sub_5AA930(u16 tile_idx, s16 tile_val)
{
    field_40_tile->field_0[tile_idx] = tile_val;
}

MATCH_FUNC(0x5AA950)
void gtx_0x106C::create_tile_num_array_5AA950()
{
    field_40_tile = new tile_array();

    for (u16 tile_num = 0; tile_num < 992; tile_num++)
    {
        field_40_tile->field_0[tile_num] = tile_num;
    }

    u32 tile_num_2 = 992;
    for (u32 i = 32; i > 0; i--)
    {
        field_40_tile->field_0[tile_num_2++] = 0;
    }
}

/*
 //gtx_0x106C *this_; // edi
    u32 idx; // ebx
    car_info *pCarInfoIter; // esi
    u32 total_len; // ebp

   // BYTE new_total_sprite; // dl
    s32 num_remaps; // eax
    s32 next_item_len; // eax
    BYTE total_sprite; // [esp+12h] [ebp-6h]
   // BYTE car_sprite; // [esp+13h] [ebp-5h]

    //this_ = this;
    idx = 0;
    total_len = 0;
    total_sprite = 0;
    BYTE last_car_sprite = 0;
    pCarInfoIter = (car_info *)field_58_car_info;

    car_info_container* pInfo = new car_info_container();
    field_5C_cari = pInfo;                  // 257 "dynamic" array ??
    if (!field_5C_cari)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 821);
    }

    if (chunk_size > 0)
    {
        for (; total_len < chunk_size; idx++)
            //while (1)
        {
            if (idx >= 256)
            {
                FatalError_4A38C0(34, "C:\\Splitting\\Gta2\\Source\\style.cpp", 825);
            }

            if (pCarInfoIter->w > 0x80u || pCarInfoIter->h > 0x80u || pCarInfoIter->num_remaps > 0x40u)
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 826, pCarInfoIter->model);
            }

            BYTE sprite = pCarInfoIter->sprite;
            if (sprite && sprite != 1)
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 827, pCarInfoIter->model);
            }

            pInfo->field_0[pCarInfoIter->model] = pCarInfoIter;

            if (pCarInfoIter->sprite)
            {
                total_sprite = last_car_sprite + total_sprite;
                last_car_sprite = pCarInfoIter->sprite;
            }

            num_remaps = pCarInfoIter->num_remaps;
            pCarInfoIter->sprite = total_sprite;

            BYTE* pRemaps = pCarInfoIter->remap;
            BYTE* t = pRemaps + num_remaps;
            if (*t > 5u)// num_doors
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 842, pCarInfoIter->model);
            }

            // 0xE = remap
            next_item_len = pCarInfoIter->remap[num_remaps] + sizeof(door_info) * pCarInfoIter->remap[num_remaps] + 1;

            total_len += next_item_len;

            pCarInfoIter = (car_info *)((char_type *)pCarInfoIter + next_item_len);

            //++idx;

            //if (total_len >= chunk_size)
            //{
            //    break;
            //}
            //this_ = this;
        } // loop end

        pInfo->field_400_count = idx;

    }
    else
    {

        field_5C_cari->field_400_count = 0;
    }
*/

STUB_FUNC(0x5AA9A0)
void gtx_0x106C::sub_5AA9A0(s32 chunk_size)
{
    s32 total_len = 0;
    BYTE total_sprite = 0;
    BYTE last_car_sprite = 0;
    car_info* pCarInfoIter = (car_info*)field_58_car_info;

    //car_info_container* pInfo =
    field_5C_cari = new car_info_container(); // 257 "dynamic" array ??
    if (!field_5C_cari)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 821);
    }

    if (chunk_size) // jbe
    {

        s32 idx = 0;
        while (total_len < chunk_size)
        //while (1)
        {
            if (idx >= 256)
            {
                FatalError_4A38C0(34, "C:\\Splitting\\Gta2\\Source\\style.cpp", 825);
            }

            if (pCarInfoIter->w > 0x80u || pCarInfoIter->h > 0x80u || pCarInfoIter->num_remaps > 0x40u)
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 826, pCarInfoIter->model);
            }

            BYTE sprite = pCarInfoIter->sprite;
            if (sprite != 0 && sprite != 1)
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 827, pCarInfoIter->model);
            }

            field_5C_cari->field_0[pCarInfoIter->model] = pCarInfoIter;

            if (pCarInfoIter->sprite)
            {
                total_sprite += last_car_sprite;
                last_car_sprite = pCarInfoIter->sprite;
            }

            pCarInfoIter->sprite = total_sprite;

            BYTE* pRemaps = (BYTE*)pCarInfoIter;
            pRemaps += 0xE;
            s32 doorCount = *(pRemaps + pCarInfoIter->num_remaps);
            if (doorCount > 5u) // num_doors
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 842, pCarInfoIter->model);
            }

            // 0xE = remap
            s32 next_item_len = doorCount + 0xE + pCarInfoIter->num_remaps;

            total_len += next_item_len;

            pCarInfoIter = (car_info*)((char_type*)pCarInfoIter + next_item_len);

            idx++;

            //++idx;

            //if (total_len >= chunk_size)
            //{
            //    break;
            //}
            //this_ = this;
        } // loop end

        field_5C_cari->field_400_count = idx; // moved from if/else
        return;
    }

    field_5C_cari->field_400_count = 0; // moved from if/else
    //return;
}

STUB_FUNC(0x5AAB30)
void gtx_0x106C::sub_5AAB30(u32 delx_chunk_size)
{
    // TODO
    UNIQUE_FUNC;
}

// note: param type matters
MATCH_FUNC(0x5AA440)
sprite_index* gtx_0x106C::get_sprite_index_5AA440(u16 idx)
{
    return &field_20_sprite_index[idx];
}

STUB_FUNC(0x5AABF0)
void gtx_0x106C::sub_5AABF0()
{
    // TODO
    UNIQUE_FUNC;
}

MATCH_FUNC(0x5AAC40)
void gtx_0x106C::SetSpriteIndexDataPtrs_5AAC40()
{
    u8* field_0_pData;
    sprite_index* v4;

    for (u32 i = 0; i < (u16)this->field_4_sprite_index_count; i++)
    {
        field_0_pData = this->field_20_sprite_index[i].field_0_pData;
        v4 = &this->field_20_sprite_index[i];
        v4->field_0_pData = &this->field_34_sprite_graphics[(u32)field_0_pData];
    }
}

STUB_FUNC(0x5AAC70)
void gtx_0x106C::sub_5AAC70()
{
    // TODO
    UNIQUE_FUNC;
}

MATCH_FUNC(0x5AAD50)
void gtx_0x106C::load_car_info_5AAD50(u32 cari_chunk_size)
{
    field_58_car_info = (car_info**)Memory::malloc_4FE4D0(cari_chunk_size);
    File::Global_Read_4A71C0(field_58_car_info, &cari_chunk_size);

    sub_5AA9A0(cari_chunk_size);
}

MATCH_FUNC(0x5AAD80)
void gtx_0x106C::load_delta_index_5AAD80(u32 delx_chunk_size)
{
    this->field_4C_delta_index = (delta_entry*)Memory::malloc_4FE4D0(delx_chunk_size);
    File::Global_Read_4A71C0(field_4C_delta_index, &delx_chunk_size);
    
    sub_5AAB30(delx_chunk_size);
    sub_5AAC70();

    crt::free(this->field_4C_delta_index);
    this->field_4C_delta_index = 0;
}

MATCH_FUNC(0x5AADD0)
void gtx_0x106C::load_delta_store_5AADD0(u32 dels_chunk_size)
{
    field_48_delta_store = (delta_store_entry*)Memory::malloc_4FE4D0(dels_chunk_size); // todo: or new?
    File::Global_Read_4A71C0(field_48_delta_store, &dels_chunk_size);
}

MATCH_FUNC(0x5AADF0)
void gtx_0x106C::load_tiles_5AADF0(u32 tile_chunk_len)
{
    field_3C_tiles = (u8*)Memory::Aligned_malloc_4FE510(tile_chunk_len, &field_44_aligned_tiles_size);
    File::Global_Read_4A71C0(field_3C_tiles, &tile_chunk_len);
    create_tile_num_array_5AA950();
}

MATCH_FUNC(0x5AAE20)
void gtx_0x106C::skip_ovly_5AAE20(u32 a1)
{
    File::Global_Seek_4A7140(&a1);
}

MATCH_FUNC(0x5AAE30)
void gtx_0x106C::skip_psxt_5AAE30(u32 a1)
{
    File::Global_Seek_4A7140(&a1);
}

MATCH_FUNC(0x5AAE40)
void gtx_0x106C::load_sprite_graphics_5AAE40(u32 sprg_chunk_len)
{
    field_34_sprite_graphics = reinterpret_cast<BYTE*>(Memory::Aligned_malloc_4FE510(sprg_chunk_len, &field_38));
    File::Global_Read_4A71C0(field_34_sprite_graphics, &sprg_chunk_len);
}

MATCH_FUNC(0x5AAE70)
void gtx_0x106C::load_physical_palettes_5AAE70(u32 ppal_chunk_size)
{
    field_2C_physical_palettes = Memory::Aligned_malloc_4FE510(ppal_chunk_size, &field_30_physical_palettes_size);
    File::Global_Read_4A71C0(field_2C_physical_palettes, &ppal_chunk_size);
    field_8_physical_palettes_len = ppal_chunk_size >> 10;
}

MATCH_FUNC(0x5AAEA0)
void gtx_0x106C::load_palette_index_5AAEA0(u32 palx_chunk_len)
{
    if (palx_chunk_len != sizeof(palette_index))
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1157, palx_chunk_len);
    }

    field_28_palette_index = new palette_index();
    if (!field_28_palette_index)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1159);
    }

    File::Global_Read_4A71C0(field_28_palette_index, &palx_chunk_len);
}

STUB_FUNC(0x5AAF00)
void gtx_0x106C::load_map_object_info_5AAF00(u32 obji_chunk_len)
{
    // TODO
    UNIQUE_FUNC;
}

MATCH_FUNC(0x5AAF80)
void gtx_0x106C::load_sprite_index_5AAF80(u32 sprx_chunk_size)
{
    field_20_sprite_index = (sprite_index*)Memory::malloc_4FE4D0(sprx_chunk_size + 8);
    File::Global_Read_4A71C0(field_20_sprite_index, &sprx_chunk_size);
    if ((sprx_chunk_size & ~7u) > 524280)
    {
        FatalError_4A38C0(1005, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1198, (u32)sprx_chunk_size >> 3);
    }
    field_4_sprite_index_count = ((u32)sprx_chunk_size >> 3) + 1;
}

MATCH_FUNC(0x5AAFE0)
void gtx_0x106C::sub_5AAFE0(u16 fontCount)
{
    if (fontCount == 14)
    {
        word_703D9C = 0;
        word_703C3E = 1;
        word_703D9A = 2;
        word_703DA4 = 3;
        word_703D0C = 4;
        word_703C16 = 5;
        word_703C8C = 6;
        font_type_703C14 = 7;
        word_703C3C = 8;
        word_703C8A = 9;
        word_703BE2 = 10;
        word_703B88 = 11;
        word_703DAC = 12;
        word_703B9C = 13;
        word_703BAA = 99;
        word_703D98 = 99;
        word_703C9C = 99;
    }
    else if (fontCount == 7)
    {
        word_703D9C = 0;
        word_703BAA = 1;
        word_703D98 = 2;
        word_703C3E = 3;
        word_703D9A = 4;
        word_703C9C = 5;
        word_703DA4 = 6;
    }
    else
    {
        FatalError_4A38C0(0x468, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1251, fontCount);
    }
}

MATCH_FUNC(0x5AB0F0)
void gtx_0x106C::load_font_base_5AB0F0(u32 fonb_chunk_size)
{
    if (fonb_chunk_size < 2)
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1268, fonb_chunk_size);
    }

    field_1C_font_base = (font_base*)Memory::malloc_4FE4D0(fonb_chunk_size);
    File::Global_Read_4A71C0(field_1C_font_base, &fonb_chunk_size);

    u16 fountCount = field_1C_font_base->field_0_font_count;
    if (fonb_chunk_size != (fountCount * 2) + 2)
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1271, fonb_chunk_size);
    }

    field_2_font_base_total = ConvertToVirtualOffsets_5AB1A0(field_1C_font_base->field_2_base, field_1C_font_base->field_0_font_count);

    ConvertToVirtualOffsets_5AB1C0(field_1C_font_base->field_2_base, field_1C_font_base->field_0_font_count);

    sub_5AAFE0(field_1C_font_base->field_0_font_count);
}

MATCH_FUNC(0x5AB1A0)
u16 __stdcall gtx_0x106C::ConvertToVirtualOffsets_5AB1A0(u16* pOffsets, u32 offsetsCount)
{
    u16 total = 0;
    while (offsetsCount > 0) // jbe vs jz due to missing > 0 and signed vs unsigned
    {
        // note: mov si, [ecx] also did xor esi, esi because
        // total was 32bits not 16bits, then an additional bitmask of 0xFFFF was done
        // because the return value was 32bits rather than 16bits.
        total += *pOffsets;
        pOffsets++;
        offsetsCount--;
    }
    return total;
}

STUB_FUNC(0x5AB1C0)
void __stdcall gtx_0x106C::ConvertToVirtualOffsets_5AB1C0(u16* pBuffer, u32 len)
{
    s32 idx1 = len - 1;
    if (len - 1 > 0)
    {
        u16* pIter1 = &pBuffer[idx1];
        do
        {
            *pIter1 = *(pIter1 - 1);
            --pIter1;
            --idx1;
        } while (idx1);
    }

    if (len)
    {
        *pBuffer = 0;
    }

    if (len > 1)
    {
        u16* pIter2 = pBuffer + 1;
        s32 idx2 = len - 1;
        do
        {
            *pIter2 += *(pIter2 - 1);
            ++pIter2;
            --idx2;
        } while (idx2);
    }
}

MATCH_FUNC(0x5AB210)
void gtx_0x106C::load_sprite_base_5AB210(u32 sprite_base_chunk_size)
{
    if (sprite_base_chunk_size != sizeof(sprite_base))
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1289, sprite_base_chunk_size);
    }

    field_18_sprite_base1 = new sprite_base();
    if (!field_18_sprite_base1)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1291);
    }

    field_14_sprite_base2 = new sprite_base();
    if (!field_14_sprite_base2)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1293);
    }

    File::Global_Read_4A71C0(field_18_sprite_base1, &sprite_base_chunk_size);

    *field_14_sprite_base2 = *field_18_sprite_base1;

    ConvertToVirtualOffsets_5AB1C0(&field_14_sprite_base2->field_0_car, 6);
}

MATCH_FUNC(0x5AB2C0)
void gtx_0x106C::load_palete_base_5AB2C0(u32 palette_base_chunk_len)
{
    if (palette_base_chunk_len != sizeof(palette_base))
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1311, palette_base_chunk_len);
    }

    field_10_palette_base1 = new palette_base();
    if (!field_10_palette_base1)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1313);
    }

    field_C_palette_base2 = new palette_base();
    if (!field_C_palette_base2)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1315);
    }

    File::Global_Read_4A71C0(field_10_palette_base1, &palette_base_chunk_len);

    *field_C_palette_base2 = *field_10_palette_base1;

    field_0_totalPalBase = ConvertToVirtualOffsets_5AB1A0(&field_C_palette_base2->field_0_tile, 8);
    ConvertToVirtualOffsets_5AB1C0(&field_C_palette_base2->field_0_tile, 8);
}

STUB_FUNC(0x5AB380)
bool gtx_0x106C::sub_5AB380(u8 car_id)
{
    // TODO
    UNIQUE_FUNC;
    return false;
}

MATCH_FUNC(0x5AB3C0)
void gtx_0x106C::load_car_recycling_info_5AB3C0(u32 recy_chunk_size)
{
    field_64_car_recycling_info = (u8*)Memory::malloc_4FE4D0(recy_chunk_size);
    ;
    field_68_recy_chunk_size = recy_chunk_size;
    File::Global_Read_4A71C0(field_64_car_recycling_info, &recy_chunk_size);
}
/*
   // u16 readBuffer_; // ax
    s32 specEntry; // edi
    u16 readValue1; // [esp+4h] [ebp-8h] BYREF
    s32 read_size; // [esp+8h] [ebp-4h] BYREF

    read_size = 2;
    File::Global_Read_4A71C0(&readValue1, &read_size);
   // readBuffer_ = readValue1;
    if (readValue1)
    {
        specEntry = type;
        do
        {
            field_6C_spec[readValue1] = specEntry;
            type = 2;
            File::Global_Read_4A71C0(&readValue1, &type);
           // readBuffer_ = readValue1;
        } while (readValue1);
    }
*/

STUB_FUNC(0x5AB3F0)
void gtx_0x106C::read_spec_5AB3F0(u32 type)
{
    u16 read_value1;
    u32 read_value1_size = 2;
    File::Global_Read_4A71C0(&read_value1, &read_value1_size);
    if (!read_value1)
    {
        return;
    }
    //    if ((WORD)read_value1)
    //{
    do
    {
        read_value1_size = 2;
        File::Global_Read_4A71C0(&read_value1, &read_value1_size);
        field_6C_spec[read_value1] = type;
    } while (read_value1);
    // }
}

MATCH_FUNC(0x5AB450)
void gtx_0x106C::load_spec_5AB450()
{
    read_spec_5AB3F0(2);
    read_spec_5AB3F0(3);
    read_spec_5AB3F0(4);
    read_spec_5AB3F0(5);
    read_spec_5AB3F0(6);
    read_spec_5AB3F0(7);
    read_spec_5AB3F0(8);
    read_spec_5AB3F0(9);
    read_spec_5AB3F0(10);
}

MATCH_FUNC(0x5AB4B0)
void gtx_0x106C::LoadChunk_5AB4B0(const char_type* Str1, u32 chunk_len)
{
    if (!strncmp(Str1, "PALB", 4u))
    {
        load_palete_base_5AB2C0(chunk_len);
    }
    else if (!strncmp(Str1, "SPRB", 4u))
    {
        load_sprite_base_5AB210(chunk_len);
    }
    else if (!strncmp(Str1, "FONB", 4u))
    {
        load_font_base_5AB0F0(chunk_len);
    }
    else if (!strncmp(Str1, "SPRX", 4u))
    {
        load_sprite_index_5AAF80(chunk_len);
    }
    else if (!strncmp(Str1, "OBJI", 4u))
    {
        load_map_object_info_5AAF00(chunk_len);
    }
    else if (!strncmp(Str1, "PALX", 4u))
    {
        load_palette_index_5AAEA0(chunk_len);
    }
    else if (!strncmp(Str1, "PPAL", 4u))
    {
        load_physical_palettes_5AAE70(chunk_len);
    }
    else if (!strncmp(Str1, "SPRG", 4u))
    {
        load_sprite_graphics_5AAE40(chunk_len);
    }
    else if (!strncmp(Str1, "TILE", 4u))
    {
        load_tiles_5AADF0(chunk_len);
    }
    else if (!strncmp(Str1, "DELS", 4u))
    {
        load_delta_store_5AADD0(chunk_len);
    }
    else if (!strncmp(Str1, "DELX", 4u))
    {
        load_delta_index_5AAD80(chunk_len);
    }
    else if (!strncmp(Str1, "CARI", 4u))
    {
        load_car_info_5AAD50(chunk_len);
    }
    else if (!strncmp(Str1, "PSXT", 4u))
    {
        skip_psxt_5AAE30(chunk_len);
    }
    else if (!strncmp(Str1, "OVLY", 4u))
    {
        skip_ovly_5AAE20(chunk_len);
    }
    else if (!strncmp(Str1, "SPEC", 4u))
    {
        load_spec_5AB450();
    }
    else if (!strncmp(Str1, "RECY", 4u))
    {
        load_car_recycling_info_5AB3C0(chunk_len);
    }
    else
    {
        File::Global_Seek_4A7140(&chunk_len);
    }
}

MATCH_FUNC(0x5AB720)
void gtx_0x106C::sub_5AB720()
{
    if (field_50_delta_buffer && field_48_delta_store)
    {
        sub_5AABF0();
    }

    if (field_20_sprite_index && field_34_sprite_graphics)
    {
        SetSpriteIndexDataPtrs_5AAC40();
    }
}

MATCH_FUNC(0x5AB750)
void gtx_0x106C::LoadSty_5AB750(const char_type* pStyFileName)
{
    File::Global_Open_4A7060(pStyFileName);

    file_header styHeader;
    u32 len = sizeof(file_header);
    File::Global_Read_4A71C0(styHeader.field_0_type, &len);

    styHeader.verify_type("GBST");
    styHeader.verify_version(700);

    chunk_header chunkHeader;
    for (len = sizeof(chunk_header); File::Global_Read_4A7210(&chunkHeader, &len); len = sizeof(chunk_header))
    {
        if (chunkHeader.field_4_size != 0)
        {
            LoadChunk_5AB4B0(chunkHeader.field_0_type, chunkHeader.field_4_size);
        }
    }

    File::Global_Close_4A70C0();

    sub_5AB720();
}

MATCH_FUNC(0x5AB820)
gtx_0x106C::gtx_0x106C()
{
    for (int i = 0; i < GTA2_COUNTOF(field_6C_spec); i++)
    {
        field_6C_spec[i] = 1;
    }

    field_64_car_recycling_info = 0;
    field_68_recy_chunk_size = 0;
    field_14_sprite_base2 = 0;
    field_C_palette_base2 = 0;
    field_1C_font_base = 0;
    field_20_sprite_index = 0;
    field_24_map_object_info = 0;
    field_6_map_object_info_len = 0;
    field_28_palette_index = 0;
    field_2C_physical_palettes = 0;
    field_30_physical_palettes_size = 0;
    field_3C_tiles = 0;
    field_44_aligned_tiles_size = 0;
    field_34_sprite_graphics = 0;
    field_38 = 0;
    field_48_delta_store = 0;
    field_50_delta_buffer = 0;
    field_58_car_info = 0;
    field_5C_cari = 0;
    field_4C_delta_index = 0;
    field_60_delta_len = 0;
    field_54_del = 0;
    field_4_sprite_index_count = 0;
    field_0_totalPalBase = 0;
    field_2_font_base_total = 0;
    field_40_tile = 0;
    field_8_physical_palettes_len = 0;
    field_6A = 0;
    field_10_palette_base1 = 0;
    field_18_sprite_base1 = 0;
}

MATCH_FUNC(0x5AB8A0)
gtx_0x106C::~gtx_0x106C()
{
    sprite_base* local_field_14_sprite_base2; // eax
    sprite_base* local_field_18_sprite_base1; // ecx
    palette_base* local_field_C_palette_base2; // edx
    palette_base* local_field_10_palette_base1; // eax
    font_base* local_field_1C_font_base; // eax
    sprite_index* local_field_20_sprite_index; // eax
    object_info* local_field_24_map_object_info; // eax
    palette_index* local_field_28_palette_index; // ecx
    void* local_field_30_physical_palettes_size; // eax
    void* local_field_44_aligned_tiles_size; // eax
    void* local_v12; // eax
    delta_store_entry* local_field_48_delta_store; // eax
    void* local_field_50_delta_buffer; // eax
    void* local_field_58_car_info; // eax
    car_info_container* local_field_5C_cari; // edx
    delta_entry* local_field_4C_delta_index; // eax
    void* local_field_54_del; // eax
    tile_array* local_field_40_tile; // eax

    if (field_64_car_recycling_info)
    {
        crt::free(field_64_car_recycling_info);
    }

    local_field_14_sprite_base2 = field_14_sprite_base2;
    field_64_car_recycling_info = 0;
    delete local_field_14_sprite_base2;

    local_field_18_sprite_base1 = field_18_sprite_base1;
    field_14_sprite_base2 = 0;
    delete (local_field_18_sprite_base1);

    local_field_C_palette_base2 = field_C_palette_base2;
    field_18_sprite_base1 = 0;
    delete (local_field_C_palette_base2);
    local_field_10_palette_base1 = field_10_palette_base1;
    field_C_palette_base2 = 0;
    delete (local_field_10_palette_base1);
    local_field_1C_font_base = field_1C_font_base;
    field_10_palette_base1 = 0;
    if (local_field_1C_font_base)
    {
        crt::free(local_field_1C_font_base);
    }

    local_field_20_sprite_index = field_20_sprite_index;
    field_1C_font_base = 0;
    if (local_field_20_sprite_index)
    {
        crt::free(local_field_20_sprite_index);
    }
    local_field_24_map_object_info = field_24_map_object_info;
    field_20_sprite_index = 0;
    if (local_field_24_map_object_info)
    {
        crt::free(local_field_24_map_object_info);
    }
    local_field_28_palette_index = field_28_palette_index;
    field_24_map_object_info = 0;
    delete (local_field_28_palette_index);
    local_field_30_physical_palettes_size = (void*)field_30_physical_palettes_size;
    field_28_palette_index = 0;
    if (local_field_30_physical_palettes_size)
    {
        crt::free(local_field_30_physical_palettes_size);
    }
    local_field_44_aligned_tiles_size = (void*)field_44_aligned_tiles_size;
    field_2C_physical_palettes = 0;
    field_30_physical_palettes_size = 0;
    if (local_field_44_aligned_tiles_size)
    {
        crt::free(local_field_44_aligned_tiles_size);
    }
    local_v12 = (void*)field_38;
    field_3C_tiles = 0;
    field_44_aligned_tiles_size = 0;
    if (local_v12)
    {
        crt::free(local_v12);
    }
    local_field_48_delta_store = field_48_delta_store;
    field_34_sprite_graphics = 0;
    field_38 = 0;
    if (local_field_48_delta_store)
    {
        crt::free(local_field_48_delta_store);
    }
    local_field_50_delta_buffer = field_50_delta_buffer;
    field_48_delta_store = 0;
    if (local_field_50_delta_buffer)
    {
        crt::free(local_field_50_delta_buffer);
    }
    local_field_58_car_info = field_58_car_info;
    field_50_delta_buffer = 0;
    if (local_field_58_car_info)
    {
        crt::free(local_field_58_car_info);
    }
    local_field_5C_cari = field_5C_cari;
    field_58_car_info = 0;
    delete (local_field_5C_cari);
    local_field_4C_delta_index = field_4C_delta_index;
    field_5C_cari = 0;
    if (local_field_4C_delta_index)
    {
        crt::free(local_field_4C_delta_index);
    }
    local_field_54_del = (void*)field_54_del;
    field_4C_delta_index = 0;
    if (local_field_54_del)
    {
        crt::free(local_field_54_del);
    }
    local_field_40_tile = field_40_tile;
    field_54_del = 0;
    delete (local_field_40_tile);
    field_40_tile = 0;
}
