#pragma once

#include "Function.hpp"
#include "BitSet32.hpp"
#include "char.hpp"
#include "Marz_1D7E.hpp"
#include "ang16.hpp"
#include "enums.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"
#include "miss2_xyz.hpp"
#include "sprite.hpp"
#include <cstdio>

class Player;
class gmp_map_zone;
class PedGroup;
class Weapon_30;
class Gang_144;
class Sprite;
class Char_8;
class Char_B4;
class Marz_96;
class Object_2C;
class Car_BC;

class Ped
{
  public:
    EXPORT Ped(); // 45AE70
    EXPORT ~Ped(); // 45AF00
    EXPORT char_type sub_45AFC0();
    EXPORT void PoolAllocate();
    EXPORT char_type sub_45B4E0();
    EXPORT Fix16_Point sub_45B520();
    EXPORT void sub_45B550();
    EXPORT void sub_45B560(Player* a2, char_type a3);
    EXPORT bool sub_45B590();
    EXPORT s32 CopyStatsFromPed_45B5B0(s32 a2);
    EXPORT Car_BC* sub_45BBF0();
    EXPORT void TeleportToCoord_45BC10(Fix16 xpos, Fix16 ypos);
    EXPORT void sub_45BC70();
    EXPORT bool sub_45BD20(Car_BC* pCar);
    EXPORT s32 sub_45BE30();
    EXPORT void SetOnFire();
    EXPORT void PutOutFire();
    EXPORT void ManageBurning_45BEC0();
    EXPORT void DrawFlamesAndStartScreamTimer();
    EXPORT void SetInvisible();
    EXPORT void SetVisible();
    EXPORT void SetSpriteSemiTransIfInvisible();
    EXPORT void SetInvulnerable();
    EXPORT void sub_45C050();
    EXPORT void sub_45C070();
    EXPORT void RestoreCarOrPedHealth();
    EXPORT void sub_45C0C0(gmp_map_zone* a2);
    EXPORT void PoolDeallocate();
    EXPORT void RespawnPed_45C350(gmp_map_zone* pZone);
    EXPORT void sub_45C410();
    EXPORT void sub_45C4B0();
    EXPORT void sub_45C500(s32 a2);
    EXPORT void sub_45C540(s32 a2);
    EXPORT void sub_45C5A0();
    EXPORT void sub_45C5C0();
    EXPORT void SpawnDriverRunAway_45C650(Car_BC* pCar, Ped* pPed);
    EXPORT void SpawnPedInCar_45C730(Car_BC* pCar);
    EXPORT void EnterCarAsDriver(Car_BC* a2);
    EXPORT void sub_45C7F0(Car_BC* pCar);
    EXPORT char_type AllocCharB4_45C830(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Ang16& sub_45C900(Ang16& a2);
    EXPORT Fix16 sub_45C920();
    EXPORT Ang16 GetRotation();
    EXPORT Fix16& sub_45C9B0(Fix16& a2);
    EXPORT s16* sub_45C9D0(s16* a2);
    EXPORT void sub_45CAA0();
    EXPORT void TakeDamage(s16 damage);
    EXPORT void sub_45CF20(Object_2C* a2);
    EXPORT char_type HandlePedHitByObject_45D000(Object_2C* a2);
    EXPORT char_type AddWeaponWithAmmo_45DD30(s32 weapon_kind, char_type ammo);
    EXPORT char_type HandlePickupCollision_45DE80(s32 a2);
    EXPORT void sub_45E080();
    EXPORT void sub_45E4A0();
    EXPORT void sub_45EA00();
    EXPORT void Deallocate_45EB60();
    EXPORT char_type sub_45EDC0();
    EXPORT bool IsField238_45EDE0(s32 a2);
    EXPORT char_type sub_45EE00(s32 a2);
    EXPORT gmp_map_zone* sub_45EE70();
    EXPORT void Mugger_AI_45F360();
    EXPORT void CarThief_AI_45FF60();
    EXPORT void sub_460820();
    EXPORT void sub_461290();
    EXPORT char_type sub_461530();
    EXPORT void sub_461630();
    EXPORT char_type RoadBlockTank_AI_4619F0();
    EXPORT s16 sub_461A60();
    EXPORT void Occupation_AI_461F20();
    EXPORT void sub_462280();
    EXPORT void sub_4624A0();
    EXPORT void RemovePedWeapons_462510();
    EXPORT void sub_462550();
    EXPORT void ForceDoNothing_462590();
    EXPORT void sub_462620();
    EXPORT char_type sub_4626B0();
    EXPORT void sub_462B80();
    EXPORT bool PoolUpdate();
    EXPORT void ProcessObjective_4632E0();
    EXPORT void sub_463300(u8 a1);
    EXPORT s32 sub_4633E0(char_type a2);
    EXPORT char_type SetObjective(s32 a2, s16 a3);
    EXPORT void sub_463830(s32 a2, s16 a3);
    EXPORT void ProcessOnFootObjective_463AA0();
    EXPORT void ProcessInCarObjective_463FB0();
    EXPORT s16* sub_4645B0();
    EXPORT void Threat_Reaction_AI_465270();
    EXPORT char_type sub_465B20();
    EXPORT bool sub_465CD0();
    EXPORT char_type sub_465D00(Ped* a2);
    EXPORT char_type IsThreatToSearchingPed_4661F0();
    EXPORT Ped* FindBestTargetPed_Mode1_466B90(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode4_466BB0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_Mode5_466BD0(s32 max_x_check);
    EXPORT Ped* FindBestTargetPed_466BF0(s32 a2);
    EXPORT Ped* sub_466F40(u8 a2);
    EXPORT Ped* FindNearestPed_466F60(u8 a2);
    EXPORT s32 sub_466FB0();
    EXPORT Ped* sub_467070();
    EXPORT char_type sub_467090();
    EXPORT Sprite* sub_467280();
    EXPORT char_type sub_4672E0(Fix16 a2, s32 a3);
    EXPORT void sub_4678E0();
    EXPORT void sub_467960();
    EXPORT void sub_467A20();
    EXPORT void sub_467AD0();
    EXPORT void sub_467BD0();
    EXPORT void sub_467CA0();
    EXPORT void sub_467E20();
    EXPORT void sub_467FB0();
    EXPORT void sub_467FD0();
    EXPORT s16 sub_468040();
    EXPORT void sub_4682A0();
    EXPORT void sub_468310();
    EXPORT void sub_4686C0();
    EXPORT void sub_468820();
    EXPORT void sub_468930();
    EXPORT void sub_468A00();
    EXPORT void sub_468BD0();
    EXPORT void sub_468C70();
    EXPORT void sub_468DE0();
    EXPORT char_type sub_468E80();
    EXPORT s32 sub_469010();
    EXPORT void sub_469030();
    EXPORT void sub_469060();
    EXPORT void sub_469BD0();
    EXPORT char_type sub_469BF0();
    EXPORT s32 sub_469D60();
    EXPORT void sub_469E10();
    EXPORT s32 sub_469E30();
    EXPORT s32 sub_469E50();
    EXPORT s32 sub_469F30();
    EXPORT void sub_469FC0();
    EXPORT void sub_469FE0();
    EXPORT void sub_46A1F0();
    EXPORT s32 sub_46A290();
    EXPORT char_type sub_46A350();
    EXPORT s32 sub_46A530();
    EXPORT void sub_46A5E0();
    EXPORT s16 sub_46A6D0();
    EXPORT void sub_46A7C0();
    EXPORT void sub_46A850();
    EXPORT void sub_46A8F0();
    EXPORT s32 sub_46A9C0();
    EXPORT char_type sub_46AAE0();
    EXPORT char_type sub_46AB50();
    EXPORT char_type sub_46AC20();
    EXPORT s32 sub_46B170();
    EXPORT s32 sub_46B2F0();
    EXPORT void sub_46B670();
    EXPORT void sub_46BD30();
    EXPORT char_type sub_46BD50(Car_BC *pCar);
    EXPORT void sub_46BDC0();
    EXPORT void sub_46C250();
    EXPORT void sub_46C770();
    EXPORT s32 sub_46C7E0();
    EXPORT void sub_46C8A0();
    EXPORT void sub_46C910();
    EXPORT void sub_46C9B0();
    EXPORT void sub_46CA60();
    EXPORT void sub_46CA70();
    EXPORT char_type sub_46CB30();
    EXPORT char_type sub_46CC70();
    EXPORT char_type sub_46CDB0();
    EXPORT char_type sub_46CEF0();
    EXPORT void sub_46D030();
    EXPORT void sub_46D0B0();
    EXPORT void sub_46D0D0();
    EXPORT void sub_46D240();
    EXPORT void sub_46D300();
    EXPORT void sub_46D460(char_type a2);
    EXPORT void sub_46DB60();
    EXPORT void sub_46DB70();
    EXPORT void sub_46DB80();
    EXPORT Sprite* GetSprite_46DF50();
    EXPORT void SetupFollower_46DF70(Ped* arg0, s32 WeaponIdx);
    EXPORT bool sub_46E020(PedGroup* a2);
    EXPORT void RecruitNearbyPeds_46E080(s32 desiredCount, Fix16 searchRadius);
    EXPORT void SpawnPedGroupFollowers_46E200(u8 total);
    EXPORT u8 get_wanted_star_count_46EF00();
    EXPORT void set_wanted_level_46EF40(u16 wanted);
    EXPORT void set_wanted_star_count_46F070(u8 star_count);
    EXPORT s32 sub_46F100(u8 a2);
    EXPORT Weapon_30* GetWeaponFromPed_46F110();
    EXPORT void sub_46F1E0(Weapon_30* a2);
    EXPORT void ManageWeapon_46F390();
    EXPORT Weapon_30* sub_46F490();
    EXPORT void ForceWeapon_46F600(s32 a2);
    EXPORT void GiveWeapon_46F650(s32 a2);
    EXPORT void sub_46F680(Ped* a2);
    EXPORT void UpdateStatsForKiller_46F720();
    EXPORT void Kill_46F9D0();
    EXPORT void AddThreateningPedToList_46FC70();
    EXPORT void sub_46FC90(s32 a2, s32 a3);
    EXPORT char_type ProcessWeaponHitResponse_46FE20(Object_2C* a2);
    EXPORT void NotifyWeaponHit_46FF00(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_46FFF0(s32 a2);
    EXPORT s16 AimRoofGun_470050();
    EXPORT void add_wanted_points_470160(s16 wanted_amount);
    EXPORT bool sub_4701D0();
    EXPORT void sub_470200(Fix16 a2, Fix16 a3, Fix16 a4);
    EXPORT void sub_4702D0(Ped* pPed);
    EXPORT void sub_470300();
    EXPORT void PushPatrolPoint_4702A0(s8 x, s8 y, s8 z);
    EXPORT s32 sub_470F00();

    EXPORT void nullsub_9();
    EXPORT void nullsub_10();
    EXPORT void nullsub_11();
    EXPORT void nullsub_12();

    inline s32 sub_420B70()
    {
        return field_238;
    }
        
    void inline_clear_bit()
    {
        // There was no way to match this without using a bit field
        field_21C_bf.b11 = 0;
    }

    bool check_bit_0()
    {
        return field_21C_bf.b0 != 0;
    }

    // 9.6f inline 0x450CB0
    inline u8 sub_450CB0()
    {
        return field_225;
    }

    bool sub_433DA0()
    {
      return (field_21C & 0x2000000) != 0 && field_168_game_object;
    }

    void reset_ped_group()
    {
        field_164_ped_group = NULL;
        field_23C = 0;
    }

    void set_ped_group(PedGroup* ptr)
    {
        field_164_ped_group = ptr;
    }

    void set_ped_group_id(s8 param_1)
    {
        field_23C = param_1;
    }

    u16 get_field_20e() const
    {
        return field_20e;
    }

    bool has_field_16C_car() const
    {
        return field_16C_car != NULL;
    }

    s32 get_ped_state1() const
    {
        return field_278;
    }

    void set_field_14C(Ped* pSrc)
    {
        field_14C = pSrc;
    }

    bool get_bitset_0x04()
    {
        return field_21C & ped_bit_status_enum::k_ped_0x00000004 ? true : false;
    }    

    void unset_bitset_0x04()
    {
        field_21C &= ~ped_bit_status_enum::k_ped_0x00000004;
    }

    void set_field_150_target_objective_car(Car_BC* ptr)
    {
        field_150_target_objective_car = ptr;
    }

    void set_ped_type(s32 param_1)
    {
        field_238 = param_1;
    }

    inline Fix16 get_cam_x()
    {
        return field_1AC_cam.x;
    }

    inline Fix16 get_cam_y()
    {
        return field_1AC_cam.y;
    }

    inline Fix16 get_cam_z()
    {
        return field_1AC_cam.z;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = field_1AC_cam.x;
        Fix16 width = rect->field_C_size.field_0_x;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (x_pos >= rect->field_0_pos.field_0_x - width && x_pos <= rect->field_0_pos.field_0_x + width &&
                (y_pos = field_1AC_cam.y, height = rect->field_C_size.field_4_y, y_pos >= rect->field_0_pos.field_4_y - height) &&
                field_1AC_cam.y <= rect->field_0_pos.field_4_y + height &&
                (z_pos = field_1AC_cam.z, z_target = rect->field_0_pos.field_8_z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline s16 get_wanted_points_433DC0()
    {
        return field_20A_wanted_points;
    }

    inline bool has_car_403B80()
    {
        return field_16C_car != 0;
    }

    inline bool not_enter_car_as_passenger_4A5040()
    {
        return field_248_enter_car_as_passenger != 1;
    }

    inline Car_BC* get_car_416B60()
    {
        return field_16C_car;
    }

    u32 get_id() const
    {
        return field_200_id;
    }

    inline Fix16 get_field_1F0()
    {
        return field_1F0_maybe_max_speed;
    }
    
    inline u8 get_remap_433BA0()
    {
        return field_244_remap;
    }

    // 9.6f inline 0x433B90
    void set_remap_433B90(u8 remap)
    {
        field_244_remap = remap;
    }

    // 9.6f inline 0x4039A0
    void set_health_4039A0(s16 health)
    {
        field_216_health = health;
    }

    // 9.6f inline 0x433B70
    inline s32 get_health_433B70()
    {
        return field_216_health;
    }

    void set_occupation_403970(s32 occupation)
    {
        field_240_occupation = occupation;
    }

    s32 get_occupation_403980()
    {
        return field_240_occupation;
    }

    void SetField238_403920(s32 unk)
    {
        field_238 = unk;
    }

    // TODO: to use this inline we need to fix a circular dependency issue
    inline s32 get_car_model();

    inline void SetRemap_433C10(u8 remap)
    {
        Char_B4* p_B4 = field_168_game_object; // local necessary to match Ped::SetupFollower_46DF70
        p_B4->field_5_remap = remap;
        if (remap != 0xFF)
        {
            p_B4->field_80_sprite_ptr->SetRemap(remap);
        }
    }

    inline void sub_433BB0(s32 value)
    {
        field_230 = value;
    }

    inline void sub_433BC0(s32 value)
    {
        field_22C = value;
    }

    Marz_3 field_0[100];
    Ang16 field_12C;
    Ang16 field_12E;
    Ang16 field_130;
    Ang16 field_132;
    Ang16 field_134;
    s16 field_136;
    s32 field_138;
    s32 field_13C;
    Car_BC* field_140;
    Ped* field_144;
    Ped* field_148_objective_target_ped;
    Ped* field_14C;
    Car_BC* field_150_target_objective_car;
    Car_BC* field_154_target_to_enter;
    Car_BC* field_158_unk_car;
    Player* field_15C_player;
    Ped* mpNext;
    PedGroup* field_164_ped_group;
    Char_B4* field_168_game_object;
    Car_BC* field_16C_car;
    Weapon_30* field_170_selected_weapon;
    Weapon_30* field_174_pWeapon;
    Weapon_30* field_178;
    Gang_144* field_17C_pZone; //  Maybe it's the zone of the gang the ped belongs to;
    Ped* field_180;
    s32 field_184;
    Ped* field_188_last_char_punched;
    Ped* field_18C;
    Marz_96* field_190;
    Marz_3* field_194;
    s32 field_198;
    Gang_144* field_19C;
    Object_2C* field_1A0_objective_target_object;
    Object_2C* field_1A4;
    Ped* field_1A8_elvis_leader;
    Fix16_Vec field_1AC_cam;
    Fix16 field_1B8_target_x;
    Fix16 field_1BC_target_y;
    Fix16 field_1C0_target_z;
    s32 field_1C4_x;
    s32 field_1C8_y;
    s32 field_1CC_z;
    Fix16 field_1D0;
    Fix16 field_1D4;
    Fix16 field_1D8;
    Fix16 field_1DC_objective_target_x;
    Fix16 field_1E0_objective_target_y;
    Fix16 field_1E4_objective_target_z;
    s32 field_1E8;
    s32 field_1EC;
    Fix16 field_1F0_maybe_max_speed;
    Fix16 field_1F4;
    Fix16 field_1F8;
    s32 field_1FC;
    u32 field_200_id;
    s32 field_204_killer_id;
    u16 field_208_invulnerability;
    s16 field_20A_wanted_points;
    s16 field_20C;
    s16 field_20e;
    u16 field_210;
    u16 field_212;
    s16 field_214;
    s16 field_216_health;
    u16 field_218_objective_timer;
    u16 field_21A_car_state_timer;

    union 
    {
        CompilerBitField32 field_21C_bf;
        // TODO: Move everything to use the above field and remove union
        s32 field_21C;
    };

    s32 field_220;
    char_type field_224;
    u8 field_225;
    char_type field_226;
    char_type field_227;
    char_type field_228;
    char_type field_229;
    char_type field_22A;
    char_type field_22B;
    s32 field_22C;
    s32 field_230;
    char_type field_234_timer;
    char_type field_235;
    char_type field_236;
    char_type field_237;
    s32 field_238;
    char_type field_23C;
    char_type field_23D;
    char_type field_23E;
    char_type field_23F;
    s32 field_240_occupation;
    char_type field_244_remap;
    char_type field_245;
    char_type field_246;
    char_type field_247;
    s32 field_248_enter_car_as_passenger;
    u8 field_24C_target_car_door;
    char_type field_24D;
    char_type field_24E;
    char_type field_24F;
    s32 field_250;
    s32 field_254;
    //char_type field_255;
    //char_type field_256;
    //char_type field_257;
    s32 field_258_objective;
    s32 field_25C_car_state;
    char_type field_260;
    char_type field_261;
    char_type field_262;
    char_type field_263;
    u8 field_264;
    char_type field_265;
    char_type field_266;
    u8 field_267_varrok_idx;
    char_type field_268;
    char_type field_269;
    u8 field_26A;
    char_type field_26B;
    s32 field_26C_graphic_type;
    s32 field_270;
    s32 field_274_gang_car_model;
    s32 field_278;
    s32 field_27C;
    s32 field_280;
    s32 field_284;
    s32 field_288_threat_search;
    s32 field_28C_threat_reaction;
    s32 field_290;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped, 0x294)

EXTERN_GLOBAL(s32, gPedId_61A89C);

EXTERN_GLOBAL(u8, gNumberMuggersSpawned_6787CA);

EXTERN_GLOBAL(u8, byte_6787CB);

EXTERN_GLOBAL(u8, byte_6787CC);

EXTERN_GLOBAL(u8, byte_6787CD);

EXTERN_GLOBAL(u8, byte_6787EF);

EXTERN_GLOBAL(Fix16, dword_678620);
