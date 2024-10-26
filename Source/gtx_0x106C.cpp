#include "Function.hpp"
#include "gtx_0x106C.hpp"
#include "error.hpp"
#include "file.hpp"
#include "chunk.hpp"
#include "memory.hpp"
#include <string.h>
#include <stdio.h>
#include <windows.h>

gtx_0x106C *gGtx_0x106C_703DD4;

struct sty_header
{
    char field_0_type[4];
    short field_4_version;
};

MATCH_FUNC(0x5AA3B0)
car_info* gtx_0x106C::get_car_info_5AA3B0(unsigned __int8 idx)
{
    return field_5C_cari->field_0[idx];
}

MATCH_FUNC(0x5AA3D0)
BYTE* gtx_0x106C::get_car_remap_5AA3D0(unsigned __int8 idx)
{
    car_info* pCarInfo = field_5C_cari->field_0[idx];
    return &pCarInfo->remap[pCarInfo->num_remaps];
}

// nomatch
unsigned __int16 gtx_0x106C::convert_sprite_pal_5AA460(int type, __int16 sprite_pal)
{
    unsigned __int16 result; // ax

    switch (type)
    {
    case 2:
        result = sprite_pal + this->field_14_sprite_base2->field_0_car;
        break;
    case 3:
        result = sprite_pal + this->field_14_sprite_base2->field_2_ped;
        break;
    case 4:
    case 8:
        result = sprite_pal + this->field_14_sprite_base2->field_4_code_obj;
        break;
    case 5:
        result = sprite_pal + this->field_14_sprite_base2->field_6_map_obj;
        break;
    case 6:
        result = sprite_pal + this->field_14_sprite_base2->field_8_user;
        break;
    case 7:
        result = sprite_pal + this->field_14_sprite_base2->field_A_font;
        break;
    default:
        result = sprite_pal;
        break;
    }
    return result;
}

// nomatch
__int16 gtx_0x106C::sub_5AA4F0(int a2)
{
    __int16 result; // ax

    switch (a2)
    {
    case 2:
        result = this->field_18_sprite_base1->field_0_car;
        break;
    case 3:
        result = this->field_18_sprite_base1->field_2_ped;
        break;
    case 4:
    case 8:
        result = this->field_18_sprite_base1->field_4_code_obj;
        break;
    case 5:
        result = this->field_18_sprite_base1->field_6_map_obj;
        break;
    case 6:
        result = this->field_18_sprite_base1->field_8_user;
        break;
    case 7:
        result = this->field_18_sprite_base1->field_A_font;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

// nomatch
__int16 gtx_0x106C::sub_5AA560(int a2)
{
    __int16 result; // ax

    switch (a2)
    {
    case 1:
        result = this->field_10_palette_base1->field_0_tile;
        break;
    case 2:
        result = this->field_10_palette_base1->field_2_sprite;
        break;
    case 3:
        result = this->field_10_palette_base1->field_4_car_remap;
        break;
    case 4:
        result = this->field_10_palette_base1->field_6_red_remap;
        break;
    case 5:
        result = this->field_10_palette_base1->field_8_code_obj_remap;
        break;
    case 6:
        result = this->field_10_palette_base1->field_A_map_obj_remap;
        break;
    case 7:
        result = this->field_10_palette_base1->field_C_user_remap;
        break;
    case 8:
        result = this->field_10_palette_base1->field_E_font_remap;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

MATCH_FUNC(0x5AA5F0)
__int16 gtx_0x106C::convert_pal_type_5AA5F0(int type, __int16 pal)
{
    __int16 result; // ax

    switch (type)
    {
    case 1:
        result = pal + this->field_C_palette_base2->field_0_tile;
        break;
    case 2:
        result = pal + this->field_C_palette_base2->field_2_sprite;
        break;
    case 3:
        result = pal + this->field_C_palette_base2->field_4_car_remap;
        break;
    case 4:
        result = pal + this->field_C_palette_base2->field_6_red_remap;
        break;
    case 5:
        result = pal + this->field_C_palette_base2->field_8_code_obj_remap;
        break;
    case 6:
        result = pal + this->field_C_palette_base2->field_A_map_obj_remap;
        break;
    case 7:
        result = pal + this->field_C_palette_base2->field_C_user_remap;
        break;
    case 8:
        result = pal + this->field_C_palette_base2->field_E_font_remap;
        break;
    default:
        result = pal;
        break;
    }
    return result;
}

// nomatch
BYTE* gtx_0x106C::GetPalData_5AA6A0(__int16 a2)
{
    return (BYTE*)this->field_2C_physical_palettes + 4 * ((a2 & 63) + ((a2 & 0xFFC0) << 8));
}

MATCH_FUNC(0x5AA6F0)
unsigned __int16 gtx_0x106C::get_phys_pal_5AA6F0(unsigned __int16 palId)
{
    return this->field_28_palette_index->field_0_phys_palette[palId];
}

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define UNIQUE_FUNC printf(__FILE__ LINE_STRING)

// nomatch
unsigned __int16 gtx_0x106C::sub_5AA710(unsigned __int16 a2, __int16 a3)
{
    font_base* field_1C_font_base = this->field_1C_font_base;
    unsigned __int16 result = field_1C_font_base->field_2_base[a2] + a3;
    if (a2 == (unsigned __int16)field_1C_font_base->field_0_font_count - 1)
    {
        if (result >= (unsigned int)this->field_2_font_base_total)
        {
            return field_1C_font_base->field_2_base[a2];
        }
    }
    else if (result >= (unsigned int)field_1C_font_base->field_2_base[a2 + 1])
    {
        return field_1C_font_base->field_2_base[a2];
    }
    return result;
}

// nomatch
unsigned __int16 gtx_0x106C::sub_5AA760(WORD *a2, wchar_t *a3)
{
    unsigned __int16 v3; // ax
    int v4; // eax

    v3 = *a2;
    if (*a2 < 0x65u)
    {
        return this->field_20_sprite_index[field_14_sprite_base2->field_A_font + sub_5AA710(v3, *a3 - 33)].field_4_width;
    }
    v4 = -(v3 < 201u);
   // LOBYTE(v4) = v4 & 0xF0;
    v4 = v4 & 0xF0;
    return v4 + 32;
}



MATCH_FUNC(0x5AA7B0)
unsigned __int16 gtx_0x106C::space_width_5AA7B0(WORD *a2)
{
    if (*a2 >= 101)
    {
        return (*a2 < 0xc9) ? 0x10 : 0x20;
    }

    return field_20_sprite_index
        [
            field_14_sprite_base2->field_A_font +
            77 + 
            field_1C_font_base->field_2_base[*a2]
        ].field_4_width;
}

// nomatch
__int16 gtx_0x106C::sub_5AA800(WORD *a2)
{
    if (*a2 < 0x65u)
        return this->field_20_sprite_index[this->field_14_sprite_base2->field_A_font
        + 32
        + (unsigned __int16)this->field_1C_font_base->field_2_base[(unsigned __int16)*a2]].field_5_height;
    int v2 = -(*a2 < 0xC9u);
    v2 = v2 & 0xEF;
    return v2 + 0x22;
}

// nomatch
bool gtx_0x106C::sub_5AA850(unsigned __int16 tile_idx)
{
    return this->field_40_tile->field_0[tile_idx] != tile_idx;
}

// nomatch
__int16 gtx_0x106C::sub_5AA870(unsigned __int16 tile_idx)
{
    return this->field_40_tile->field_0[tile_idx];
}

// nomatch
__int16 gtx_0x106C::sub_5AA890()
{
    __int16 internal_idx; // ax
    __int16 *i; // ecx

    internal_idx = 1023;
    for (i = &this->field_40_tile->field_0[1023]; *i; --i)
    {
        if ((unsigned __int16)--internal_idx < 992u)
            return 0;
    }
    return internal_idx;
}

// nomatch
__int16 gtx_0x106C::get_physical_palettes_len_5AA900()
{
    return this->field_8_physical_palettes_len;
}

// nomatch
object_info* gtx_0x106C::get_map_object_info_5AA910(unsigned __int16 idx)
{
    if (idx < (unsigned int)this->field_6_map_object_info_len)
        return &this->field_24_map_object_info[idx];
    else
        return 0;
}

// nomatch
int gtx_0x106C::sub_5AA930(unsigned __int16 tile_idx, __int16 tile_val)
{
    int result; // eax

    result = tile_idx;
    this->field_40_tile->field_0[tile_idx] = tile_val;
    return result;
}

// nomatch
void gtx_0x106C::create_tile_num_array_5AA950()
{
    // TODO
    UNIQUE_FUNC;
}

/*
 //gtx_0x106C *this_; // edi
    unsigned int idx; // ebx
    car_info *pCarInfoIter; // esi
    unsigned int total_len; // ebp

   // BYTE new_total_sprite; // dl
    int num_remaps; // eax
    int next_item_len; // eax
    BYTE total_sprite; // [esp+12h] [ebp-6h]
   // BYTE car_sprite; // [esp+13h] [ebp-5h]

    //this_ = this;
    idx = 0;
    total_len = 0;
    total_sprite = 0;
    BYTE last_car_sprite = 0;
    pCarInfoIter = (car_info *)this->field_58_car_info;

    car_info_container* pInfo = new car_info_container();
    this->field_5C_cari = pInfo;                  // 257 "dynamic" array ??
    if (!this->field_5C_cari)
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

            pCarInfoIter = (car_info *)((char *)pCarInfoIter + next_item_len);

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

        this->field_5C_cari->field_400_count = 0;
    }
*/
// nomatch
void gtx_0x106C::sub_5AA9A0(int chunk_size)
{
    int total_len = 0;
    BYTE total_sprite = 0;
    BYTE last_car_sprite = 0;
    car_info *pCarInfoIter = (car_info *)this->field_58_car_info;

    //car_info_container* pInfo = 
    this->field_5C_cari = new car_info_container();                  // 257 "dynamic" array ??
    if (!this->field_5C_cari)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\style.cpp", 821);
    }

    if (chunk_size) // jbe
    {


        int idx = 0;
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
            int doorCount = *(pRemaps + pCarInfoIter->num_remaps);
            if (doorCount > 5u)// num_doors
            {
                FatalError_4A38C0(1107, "C:\\Splitting\\Gta2\\Source\\style.cpp", 842, pCarInfoIter->model);
            }

            // 0xE = remap
            int next_item_len = doorCount + 0xE + pCarInfoIter->num_remaps;

            total_len += next_item_len;

            pCarInfoIter = (car_info *)((char *)pCarInfoIter + next_item_len);

            idx++;

            //++idx;

            //if (total_len >= chunk_size)
            //{
            //    break;
            //}
            //this_ = this;
        } // loop end


        this->field_5C_cari->field_400_count = idx; // moved from if/else
        return;
    }

    this->field_5C_cari->field_400_count = 0; // moved from if/else
    //return;

}

// stub
void gtx_0x106C::sub_5AAB30(unsigned int delx_chunk_size)
{
    // TODO
    UNIQUE_FUNC;
}

// note: param type matters
MATCH_FUNC(0x5AA440)
sprite_index* gtx_0x106C::get_sprite_index_5AA440(unsigned __int16 idx)
{
    return &this->field_20_sprite_index[idx];
}

// stub
void gtx_0x106C::sub_5AABF0()
{
    // TODO
    UNIQUE_FUNC;
}

// nomatch
void gtx_0x106C::SetSpriteIndexDataPtrs_5AAC40()
{
    unsigned int sprite_idx; // eax
    sprite_index *field_20_sprite_index; // edx
    BYTE *field_0_pData; // edi
    sprite_index *pSpriteIdxIter; // edx

    for (sprite_idx = 0;
        sprite_idx < (unsigned __int16)this->field_4_sprite_index_count;
        )
    {
        field_20_sprite_index = this->field_20_sprite_index;
        field_0_pData = field_20_sprite_index[sprite_idx].field_0_pData;
        pSpriteIdxIter = &field_20_sprite_index[sprite_idx++];
        pSpriteIdxIter->field_0_pData = &this->field_34_sprite_graphics[(unsigned int)field_0_pData]; // converting offsets to ptrs ??
    }
}

// stub
void gtx_0x106C::sub_5AAC70()
{
    // TODO
    UNIQUE_FUNC;
}

// nomatch
void gtx_0x106C::load_car_info_5AAD50(unsigned int cari_chunk_size)
{
    unsigned int len = cari_chunk_size;
    void *v3; // eax

    v3 = malloc(cari_chunk_size);
    File::Global_Read_4A71C0(v3, &len);
    
    //this->field_58_car_info = v3;
    sub_5AA9A0(cari_chunk_size);

    /*
        chunk_header chunkHeader;
    for (len = sizeof(chunk_header); File::Global_Read_4A7210(&chunkHeader, &len); len = sizeof(chunk_header))
    */
}

// stub
void gtx_0x106C::load_delta_index_5AAD80(unsigned int delx_chunk_size)
{
    // TODO
    UNIQUE_FUNC;
}

MATCH_FUNC(0x5AADD0)
void gtx_0x106C::load_delta_store_5AADD0(unsigned int dels_chunk_size)
{
    field_48_delta_store = (delta_store_entry *)Memory::malloc_4FE4D0(dels_chunk_size); // todo: or new?
    File::Global_Read_4A71C0(field_48_delta_store, &dels_chunk_size);
}

MATCH_FUNC(0x5AADF0)
void gtx_0x106C::load_tiles_5AADF0(unsigned int tile_chunk_len)
{
    field_3C_tiles = Memory::Aligned_malloc_4FE510(tile_chunk_len, &field_44_aligned_tiles_size);
    File::Global_Read_4A71C0(field_3C_tiles, &tile_chunk_len);
    create_tile_num_array_5AA950();
}

// nomatch
void gtx_0x106C::skip_ovly_5AAE20(unsigned int a1)
{
    File::Global_Seek_4A7140(&a1);
}

// nomatch
void gtx_0x106C::skip_psxt_5AAE30(unsigned int a1)
{
    File::Global_Seek_4A7140(&a1);
}

MATCH_FUNC(0x5AAE40)
void gtx_0x106C::load_sprite_graphics_5AAE40(unsigned int sprg_chunk_len)
{
    field_34_sprite_graphics = reinterpret_cast<BYTE*>(Memory::Aligned_malloc_4FE510(sprg_chunk_len, &field_38));
    File::Global_Read_4A71C0(field_34_sprite_graphics, &sprg_chunk_len);
}

MATCH_FUNC(0x5AAE70)
void gtx_0x106C::load_physical_palettes_5AAE70(unsigned int ppal_chunk_size)
{
    field_2C_physical_palettes = Memory::Aligned_malloc_4FE510(ppal_chunk_size, &field_30_physical_palettes_size);
    File::Global_Read_4A71C0(field_2C_physical_palettes, &ppal_chunk_size);
    field_8_physical_palettes_len = ppal_chunk_size >> 10;
}

MATCH_FUNC(0x5AAEA0)
void gtx_0x106C::load_palette_index_5AAEA0(unsigned int palx_chunk_len)
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

// stub
void gtx_0x106C::load_map_object_info_5AAF00(unsigned int obji_chunk_len)
{
    // TODO
    UNIQUE_FUNC;
}

MATCH_FUNC(0x5AAF80)
void gtx_0x106C::load_sprite_index_5AAF80(unsigned int sprx_chunk_size)
{
    this->field_20_sprite_index = (sprite_index *)Memory::malloc_4FE4D0(sprx_chunk_size + 8);
    File::Global_Read_4A71C0(field_20_sprite_index, &sprx_chunk_size);
    if ((sprx_chunk_size & ~7u) > 524280)
    {
        FatalError_4A38C0(1005, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1198, (unsigned int)sprx_chunk_size >> 3);
    }
    this->field_4_sprite_index_count = ((unsigned int)sprx_chunk_size >> 3) + 1;
}

// stub
void gtx_0x106C::sub_5AAFE0(unsigned __int16 a1)
{
    // TODO
    UNIQUE_FUNC;
}

// nomatch
void gtx_0x106C::load_font_base_5AB0F0(unsigned int fonb_chunk_size)
{
    if (fonb_chunk_size < 2)
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1268, fonb_chunk_size);
    }

    this->field_1C_font_base = (font_base *)Memory::malloc_4FE4D0(fonb_chunk_size);
    File::Global_Read_4A71C0(field_1C_font_base, &fonb_chunk_size);
    
    unsigned __int16 fountCount = this->field_1C_font_base->field_0_font_count;
    if (fonb_chunk_size != (fountCount * 2) + 2)
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1271, fonb_chunk_size);
    }

    this->field_2_font_base_total = ConvertToVirtualOffsets_5AB1A0(
        this->field_1C_font_base->field_2_base,
        this->field_1C_font_base->field_0_font_count);

    ConvertToVirtualOffsets_5AB1C0(
        this->field_1C_font_base->field_2_base,
        this->field_1C_font_base->field_0_font_count);

    sub_5AAFE0(this->field_1C_font_base->field_0_font_count);
}

MATCH_FUNC(0x5AB1A0)
WORD __stdcall gtx_0x106C::ConvertToVirtualOffsets_5AB1A0(WORD *pOffsets, unsigned int offsetsCount)
{
    WORD total = 0;
    while(offsetsCount >0 ) // jbe vs jz due to missing > 0 and signed vs unsigned
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

// nomatch
void __stdcall gtx_0x106C::ConvertToVirtualOffsets_5AB1C0(WORD *pBuffer, unsigned int len)
{
    int idx1 = len - 1;
    if (len - 1 > 0)
    {
        WORD *pIter1 = &pBuffer[idx1];
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
        WORD * pIter2 = pBuffer + 1;
        int idx2 = len - 1;
        do
        {
            *pIter2 += *(pIter2 - 1);
            ++pIter2;
            --idx2;
        } while (idx2);
    }
}

MATCH_FUNC(0x5AB210)
void gtx_0x106C::load_sprite_base_5AB210(unsigned int sprite_base_chunk_size)
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

// nomatch
void gtx_0x106C::load_palete_base_5AB2C0(unsigned int palette_base_chunk_len)
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

    File::Global_Read_4A71C0(this->field_10_palette_base1, &palette_base_chunk_len);
    
    *field_C_palette_base2 = *field_10_palette_base1;

    field_0_totalPalBase = ConvertToVirtualOffsets_5AB1A0(&field_C_palette_base2->field_0_tile, 8);
    ConvertToVirtualOffsets_5AB1C0(&field_C_palette_base2->field_0_tile, 8);
}

// stub
bool gtx_0x106C::sub_5AB380(unsigned __int8 car_id)
{
    // TODO
    UNIQUE_FUNC;
    return false;
}

MATCH_FUNC(0x5AB3C0)
void gtx_0x106C::load_car_recycling_info_5AB3C0(unsigned int recy_chunk_size)
{
    field_64_car_recycling_info = (unsigned __int8 *)Memory::malloc_4FE4D0(recy_chunk_size);;
    field_68_recy_chunk_size = recy_chunk_size;
    File::Global_Read_4A71C0(field_64_car_recycling_info, &recy_chunk_size);
}
/*
   // unsigned __int16 readBuffer_; // ax
    int specEntry; // edi
    unsigned __int16 readValue1; // [esp+4h] [ebp-8h] BYREF
    int read_size; // [esp+8h] [ebp-4h] BYREF

    read_size = 2;
    File::Global_Read_4A71C0(&readValue1, &read_size);
   // readBuffer_ = readValue1;
    if (readValue1)
    {
        specEntry = type;
        do
        {
            this->field_6C_spec[readValue1] = specEntry;
            type = 2;
            File::Global_Read_4A71C0(&readValue1, &type);
           // readBuffer_ = readValue1;
        } while (readValue1);
    }
*/
// nomatch
void gtx_0x106C::read_spec_5AB3F0(unsigned int type)
{
    WORD read_value1;
    unsigned int read_value1_size = 2;
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
void gtx_0x106C::LoadChunk_5AB4B0(const char *Str1, unsigned int chunk_len)
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
        load_car_info_5AAD50( chunk_len);
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
void gtx_0x106C::LoadSty_5AB750(const char *pStyFileName)
{
    File::Global_Open_4A7060(pStyFileName);

    sty_header styHeader;
    unsigned int len = sizeof(sty_header);
    File::Global_Read_4A71C0(styHeader.field_0_type, &len);

    if (strncmp("GBST", styHeader.field_0_type, sizeof(styHeader.field_0_type)))
    {
        FatalError_4A38C0(93, "C:\\Splitting\\Gta2\\Source\\chunk.h", 37);
    }

    if (styHeader.field_4_version != 700)
    {
        FatalError_4A38C0(94, "C:\\Splitting\\Gta2\\Source\\chunk.h", 33);
    }

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

// nomatch 0x5AB820
gtx_0x106C::gtx_0x106C()
{
    memset(this->field_6C_spec, 1, sizeof(field_6C_spec));
    this->field_64_car_recycling_info = 0;
    this->field_68_recy_chunk_size = 0;
    this->field_14_sprite_base2 = 0;
    this->field_C_palette_base2 = 0;
    this->field_1C_font_base = 0;
    this->field_20_sprite_index = 0;
    this->field_24_map_object_info = 0;
    this->field_6_map_object_info_len = 0;
    this->field_28_palette_index = 0;
    this->field_2C_physical_palettes = 0;
    this->field_30_physical_palettes_size = 0;
    this->field_3C_tiles = 0;
    this->field_44_aligned_tiles_size = 0;
    this->field_34_sprite_graphics = 0;
    this->field_38 = 0;
    this->field_48_delta_store = 0;
    this->field_50_delta_buffer = 0;
    this->field_58_car_info = 0;
    this->field_5C_cari = 0;
    this->field_4C_delta_index = 0;
    this->field_60_delta_len = 0;
    this->field_54_del = 0;
    this->field_4_sprite_index_count = 0;
    this->field_0_totalPalBase = 0;
    this->field_2_font_base_total = 0;
    this->field_40_tile = 0;
    this->field_8_physical_palettes_len = 0;
    this->field_6A = 0;
    this->field_10_palette_base1 = 0;
    this->field_18_sprite_base1 = 0;
}

// nomatch 0x5AB8A0
gtx_0x106C::~gtx_0x106C()
{
    sprite_base *field_14_sprite_base2; // eax
    sprite_base *field_18_sprite_base1; // ecx
    palette_base *field_C_palette_base2; // edx
    palette_base *field_10_palette_base1; // eax
    font_base *field_1C_font_base; // eax
    sprite_index *field_20_sprite_index; // eax
    object_info *field_24_map_object_info; // eax
    palette_index *field_28_palette_index; // ecx
    void *field_30_physical_palettes_size; // eax
    void *field_44_aligned_tiles_size; // eax
    void *v12; // eax
    delta_store_entry *field_48_delta_store; // eax
    void *field_50_delta_buffer; // eax
    void *field_58_car_info; // eax
    car_info_container*  field_5C_cari; // edx
    delta_entry *field_4C_delta_index; // eax
    void *field_54_del; // eax
    tile_array *field_40_tile; // eax

    if (this->field_64_car_recycling_info)
    {
        free(this->field_64_car_recycling_info);
    }

    field_14_sprite_base2 = this->field_14_sprite_base2;
    this->field_64_car_recycling_info = 0;
    delete field_14_sprite_base2;

    field_18_sprite_base1 = this->field_18_sprite_base1;
    this->field_14_sprite_base2 = 0;
    delete (field_18_sprite_base1);

    field_C_palette_base2 = this->field_C_palette_base2;
    this->field_18_sprite_base1 = 0;
    delete (field_C_palette_base2);
    field_10_palette_base1 = this->field_10_palette_base1;
    this->field_C_palette_base2 = 0;
    delete (field_10_palette_base1);
    field_1C_font_base = this->field_1C_font_base;
    this->field_10_palette_base1 = 0;
    if (field_1C_font_base)
    {
        free(field_1C_font_base);
    }

    field_20_sprite_index = this->field_20_sprite_index;
    this->field_1C_font_base = 0;
    if (field_20_sprite_index)
    {
        free(field_20_sprite_index);
    }
    field_24_map_object_info = this->field_24_map_object_info;
    this->field_20_sprite_index = 0;
    if (field_24_map_object_info)
    {
        free(field_24_map_object_info);
    }
    field_28_palette_index = this->field_28_palette_index;
    this->field_24_map_object_info = 0;
    delete(field_28_palette_index);
    field_30_physical_palettes_size = (void *)this->field_30_physical_palettes_size;
    this->field_28_palette_index = 0;
    if (field_30_physical_palettes_size)
    {
        free(field_30_physical_palettes_size);
    }
    field_44_aligned_tiles_size = (void *)this->field_44_aligned_tiles_size;
    this->field_2C_physical_palettes = 0;
    this->field_30_physical_palettes_size = 0;
    if (field_44_aligned_tiles_size)
    {
        free(field_44_aligned_tiles_size);
    }
    v12 = (void *)this->field_38;
    this->field_3C_tiles = 0;
    this->field_44_aligned_tiles_size = 0;
    if (v12)
    {
        free(v12);
    }
    field_48_delta_store = this->field_48_delta_store;
    this->field_34_sprite_graphics = 0;
    this->field_38 = 0;
    if (field_48_delta_store)
    {
        free(field_48_delta_store);
    }
    field_50_delta_buffer = this->field_50_delta_buffer;
    this->field_48_delta_store = 0;
    if (field_50_delta_buffer)
    {
        free(field_50_delta_buffer);
    }
    field_58_car_info = this->field_58_car_info;
    this->field_50_delta_buffer = 0;
    if (field_58_car_info)
    {
        free(field_58_car_info);
    }
    field_5C_cari = this->field_5C_cari;
    this->field_58_car_info = 0;
    delete(field_5C_cari);
    field_4C_delta_index = this->field_4C_delta_index;
    this->field_5C_cari = 0;
    if (field_4C_delta_index)
    {
        free(field_4C_delta_index);
    }
    field_54_del = (void *)this->field_54_del;
    this->field_4C_delta_index = 0;
    if (field_54_del)
    {
        free(field_54_del);
    }
    field_40_tile = this->field_40_tile;
    this->field_54_del = 0;
    delete(field_40_tile);
    this->field_40_tile = 0;
}
