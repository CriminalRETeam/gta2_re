// Structures shared with the game through the gbh_* and Vid_* APIs.
// Layouts follow GTA2Hax (Common/types.hpp, d3ddll.hpp, DmaVideo.hpp); the game reads some fields directly.
#pragma once

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef float f32;

#define GTA2_STATIC_ASSERT(x) static_assert((x), #x)

#if defined(_WIN32)
    #define GTA2_CC __stdcall
#else
    #define GTA2_CC
#endif

struct STexture
{
    u16 field_0_id;
    u16 field_2;
    u16 field_4_pal_is_trans;
    u16 field_6_pal_size;
    void* field_8_locked_pixels_ptr;
    u8 field_C;
    u8 field_D;
    u16 field_E_width;
    u16 field_10_height;
    u8 field_12_bPalIsValid;
    u8 field_13_flags;
    u8* field_14_original_pixel_data_ptr;
    u16* field_18_pPaltData;
    void* field_1C_ptr; // backend texture handle (struct GLTex*)
};

struct Vert
{
    f32 x, y, z, w;
    u32 diff;
    u32 spec;
    f32 u, v;
};

struct SLight
{
    u32 field_0;
    f32 field_4_x;
    f32 field_8_y;
    f32 field_C_z;
    u32 field_10_colour;
};

#pragma pack(push, 1)
struct SImage
{
    u8 field_0_id_len;
    u8 field_1_cmap_type;
    u8 field_2_image_type;
    u8 field_3;
    u32 field_4;
    u32 field_8;
    u16 field_C_width;
    u16 field_E_height;
    u8 field_10_bpp;
    u8 field_11_descriptor;
    u32 field_12;
};
#pragma pack(pop)

struct SDevice
{
    u32 field_0_id;
    u32 field_4_flags; // bit 0: windowed mode capable
    char* field_8_driver_name;
    char* field_C_driver_desc;
    SDevice* field_10_next_ptr;
    void* field_14_pDeviceGuid;
    u8 field_18_guid[16];
    u32 field_28_dwVidMemTotal;
    u32 field_2C_end_buffer;
};

struct SDisplayMode
{
    u32 field_0_display_mode_idx;
    u32 field_4_deviceId;
    u32 field_8_width;
    u32 field_C_height;
    u32 field_10_pitch;
    u32 field_14_rgb_bit_count;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    SDisplayMode* field_38_pnext;
    u32 field_3C;
};

struct SVidVersion
{
    u32 mVersion;
    char mVersionString[255];
};

// The game reads field_40/48/4C/50/54, the colour bit layout at 0x5C-0x6C (Bink) and field_134 (Bink).
struct SVideo
{
    u32 field_0;
    u32 field_4_flags;
    u32 field_8_found_width;
    u32 field_C_found_height;
    u32 field_10_found_rgb_bit_count;
    u32 field_14_display_mode_count_2_q;
    u32 field_18_num_guids;
    u32 field_1C_num_display_modes;
    u32 field_20_num_enums;
    SDisplayMode* field_24_head_ptr;
    SDisplayMode* field_28_display_mode_array;
    SDevice* field_2C_device_info_head_ptr;
    SDevice* field_30_device_info_last;
    u32 field_34_active_device_id;
    u32 field_38;
    SDisplayMode* field_3C_current_enum_ptr;
    s32 field_40_full_screen;
    u32 field_44;
    u32 field_48_rect_right;
    u32 field_4C_rect_bottom;
    void* field_50_surface_pixels_ptr;
    u32 field_54_surface_pixels_pitch;
    u32 field_58;
    u32 field_5C;
    u32 field_60_g;
    u32 field_64_r;
    u32 field_68_b;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    void* field_7C_self_dll_handle;
    u32 field_80_active_mode_q;
    void* field_84_from_initDLL;
    u32 field_88_last_error;
    void* field_8C_DirectDraw7;
    char buffer[144];
    void* field_120_IDDraw4;
    u8 field_124[16];
    void* field_134_SurfacePrimary;
    void* field_138_Surface;
    u8 field_13C_DDSurfaceDesc7[0x7C];
    void* field_1B8_clipper;
    u32 field_1BC;
    u32 field_1C0;
    u32 field_1C4;
    u8 field_1C8_device_caps[0x17C];
    u8 field_344_hel_caps[0x17C];
    void* field_4C0_hwnd;
};

#if !defined(_WIN64) && (defined(_WIN32) || defined(__i386__))
GTA2_STATIC_ASSERT(sizeof(STexture) == 0x20);
GTA2_STATIC_ASSERT(sizeof(SVideo) == 0x4C4);
GTA2_STATIC_ASSERT(sizeof(SDevice) == 0x30);
GTA2_STATIC_ASSERT(sizeof(SDisplayMode) == 0x40);
GTA2_STATIC_ASSERT(offsetof(SVideo, field_134_SurfacePrimary) == 0x134);
GTA2_STATIC_ASSERT(offsetof(SVideo, field_4C0_hwnd) == 0x4C0);
#endif
GTA2_STATIC_ASSERT(sizeof(Vert) == 0x20);
GTA2_STATIC_ASSERT(sizeof(SLight) == 0x14);
GTA2_STATIC_ASSERT(sizeof(SImage) == 0x16);
