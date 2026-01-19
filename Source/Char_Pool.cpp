#include "Char_Pool.hpp"
#include "char.hpp"
#include "Car_BC.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Hud.hpp"
#include "Ped.hpp"
#include "Ped_List_4.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "rng.hpp"
#include <DINPUT.H>

DEFINE_GLOBAL(PedManager*, gPedManager_6787BC, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool_6787B8, 0x6787B8);
DEFINE_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44, 0x6FDB44);
DEFINE_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50, 0x678b50);

DEFINE_GLOBAL(u16, word_6787F0, 0x6787F0);
DEFINE_GLOBAL(u8, byte_61A8A1, 0x61A8A1);
DEFINE_GLOBAL(u8, byte_61A8A2, 0x61A8A2);
DEFINE_GLOBAL(u8, unk_6787EE, 0x6787EE);
DEFINE_GLOBAL(u8, unk_6787EF, 0x6787EF);
DEFINE_GLOBAL(u8, byte_6787DA, 0x6787DA);

EXTERN_GLOBAL(u16, word_6787E0);
EXTERN_GLOBAL(u8, byte_6787E2);
EXTERN_GLOBAL(u8, byte_6787E4);
EXTERN_GLOBAL(u8, byte_6787E3);
EXTERN_GLOBAL(u8, byte_6787D8);
EXTERN_GLOBAL(u8, byte_6787D9);
EXTERN_GLOBAL(u8, byte_6787D2);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

STUB_FUNC(0x46eb60)
void PedManager::sub_46EB60(u32* a2)
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/dQf8H
WIP_FUNC(0x4703f0)
void PedManager::PedsService_4703F0()
{
    WIP_IMPLEMENTED;

    ++word_6787F0;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E4 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    byte_61A8A1 = 1;
    byte_61A8A2 = 1;
    byte_6787D2 = 0;
    unk_6787EE = 0;

    gPedPool_6787B8->field_0_pool.UpdatePool();

    if (unk_6787EF) // 11d: je 128
    {
        byte_6787DA = 1;
    }
    else
    {
        byte_6787DA = 0;
        gThreateningPedsList_678468.ClearPeds_4712F0();
    }

    byte_6787D8 = byte_61A8A1 == 1;
    byte_6787D9 = byte_61A8A2 == 1;

    if (!bSkip_dummies_67D4EF)
    {
        Dummies_470330();
    }
    field_3 = word_6787E0;
    field_2 = byte_6787E2;
    field_4 = byte_6787E3;
    field_5_fbi_army_count = byte_6787E4;
    field_6_num_peds_on_screen = gNumPedsOnScreen_6787EC;
    if (gPolice_7B8_6FEE40)
    {
        gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen = unk_6787EE;
    }
    unk_6787EF = 0;
    if (bDo_iain_test_67D4E9)
    {
        u16 num_peds = *(u32*)&gNumPedsOnScreen_6787EC; //  TODO: fix me
        swprintf(tmpBuff_67BD9C, L"num peds on screen : %d",
                 num_peds); // num peds on screen : %d
        gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 64, word_706600, 1);

        if (gPolice_7B8_6FEE40)
        {
            swprintf(tmpBuff_67BD9C,
                     L"num police peds in range screen : %d", // num police peds in range screen : %d
                     (u8)gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen);
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 80, word_706600, 1);
        }
    }
}

// https://decomp.me/scratch/P1OvR
WIP_FUNC(0x470650)
PedManager::PedManager()
{
    field_8 = 0;
    if (!gPedPool_6787B8)
    {
        gPedPool_6787B8 = new PedPool();
        if (!gPedPool_6787B8)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15827); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_B4_Pool_6FDB44)
    {
        gChar_B4_Pool_6FDB44 = new Char_B4_Pool();
        if (!gChar_B4_Pool_6FDB44)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15834); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_8_Pool_678b50)
    {
        gChar_8_Pool_678b50 = new Char_8_Pool();
        if (!gChar_8_Pool_678b50)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15841); // OutOfMemoryNewOperator
        }
    }

    field_8 = gSprite_Pool_703818->get_new_sprite();

    field_2 = 0;
    field_3 = 0;
    field_4 = 0;
    field_6_num_peds_on_screen = 0;
    field_0 = 50;
    field_7_make_all_muggers = false;
    /*
    gPedId_61A89C = 7;
    dword_6787C0 = 0;
    word_6787C6 = 0;
    byte_6787C8 = 0;
    byte_6787C9 = 0;
    gNumberMuggersSpawned_6787CA = 0;
    gNumberCarThiefsSpawned_6787CB = 0;
    gNumberElvisLeadersSpawned_6787CC = 0;
    gNumberWalkingCopsSpawned_6787CD = 0;
    byte_6787CE = 0;
    word_6787D0 = 0;
    this->field_5_fbi_army_count = 0;
    HIWORD(dword_678654) = word_61A898;
    word_6787F0 = 0;
    byte_6787D2 = 0;
    byte_6787D3 = 0;
    HIWORD(dword_6784EE) = word_6787A8;
    byte_6787D4 = 0;
    byte_6787D5 = 0;
    byte_6787D6 = 0;
    byte_6787D7 = 0;
    byte_61A8A0 = 1;
    byte_61A8A1 = 1;
    byte_6787D8 = 0;
    byte_61A8A2 = 1;
    byte_6787D9 = 0;
    byte_6787DA = 0;
    gDistanceToTarget_678750 = dword_678660;
    dword_6787DC = 0;
    word_678760 = word_6787A8;
    byte_61A8A3 = 1;
    byte_61A8A4 = 1;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    unk_6787EF = 0;
    */
    sub_553F90();
    gThreateningPedsList_678468.ClearList_420E90();
}

STUB_FUNC(0x4709b0)
PedManager::~PedManager()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x470a50)
Ped* PedManager::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    if (!pPed->AllocCharB4_45C830(xpos, ypos, zpos))
    {
        return 0;
    }
    pPed->field_168_game_object->field_40_rotation.rValue = rotation.rValue;
    pPed->field_244_remap = remap;

    Char_B4* pB4 = pPed->field_168_game_object;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->field_134_rotation = rotation;
    pPed->field_288_threat_search = 2;
    pPed->field_28C_threat_reaction = 3;
    pPed->field_216_health = 100;
    pPed->field_26C_graphic_type = 0;
    return pPed;
}

MATCH_FUNC(0x470b00)
Ped* PedManager::SpawnDriver_470B00(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->sub_45C500(10);
    pNewPed->sub_45C540(10);
    pNewPed->field_248_enter_car_as_passenger = 0;
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    pCar->SetDriver(pNewPed);
    return pNewPed;
}

// https://decomp.me/scratch/6AW5o 
MATCH_FUNC(0x470ba0)
Ped* PedManager::SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang)
{
 Ped *pNewPed = gPedPool_6787B8->field_0_pool.Allocate();
    
  pNewPed->field_238 = 6;
  pNewPed->field_240_occupation = ped_ocupation_enum::unknown_19;
  pNewPed->field_16C_car = pCar;
  pNewPed->field_168_game_object = 0;
  pNewPed->sub_45C500(10);
  pNewPed->sub_45C540(10);
  pNewPed->field_248_enter_car_as_passenger = 0;
  pNewPed->field_24C_target_car_door = 0;
  pNewPed->field_288_threat_search = threat_search_enum::area_2;
  pNewPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
  pNewPed->field_216_health = 100;
  pNewPed->field_26C_graphic_type = 1;
    
  pCar->SetDriver(pNewPed);
    
  pNewPed->field_17C_pZone = pGang;
  pNewPed->field_244_remap = pGang->field_101;
    
  if ( pNewPed->field_244_remap == 5 )
  {
    s16 constant = 2;
    if ( !stru_6F6784.get_int_4F7AE0(&constant) )
    {
      pNewPed->field_244_remap = 6;
    }
  }
  pNewPed->field_26C_graphic_type = 1;
  pNewPed->field_22C = 1;
    
  pNewPed->ForceWeapon_46F600(pNewPed->field_17C_pZone->GetGangCurrWeapon_4BF0C0());
  pNewPed->GiveWeapon_46F650(weapon_type::pistol);
    
  pNewPed->field_270 = 0;
  pNewPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
  pNewPed->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;
    
  return pNewPed;
}

MATCH_FUNC(0x470cc0)
Ped* PedManager::sub_470CC0(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_244_remap = -1;
    pNewPed->field_26C_graphic_type = 0;
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->sub_45C500(10);
    pNewPed->sub_45C540(10);
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    return pNewPed;
}

STUB_FUNC(0x470d60)
Ped* PedManager::sub_470D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470e30)
Ped* PedManager::sub_470E30()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x470f30)
Ped* PedManager::sub_470F30()
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_216_health = 100;
    return pNewPed;
}

MATCH_FUNC(0x470f90)
Ped* PedManager::sub_470F90(Ped* pSrc)
{
    Ped* pDst = gPedPool_6787B8->Allocate();
    Ped* pNext = pDst->mpNext;
    memcpy(pDst, pSrc, sizeof(Ped));
    pDst->mpNext = pNext;

    if (pSrc->field_168_game_object)
    {
        pDst->AllocCharB4_45C830(pSrc->field_1AC_cam.x, pSrc->field_1AC_cam.y, pSrc->field_1AC_cam.z);
        Char_B4* pCharObj = pDst->field_168_game_object;
        u8 remap = pSrc->field_244_remap;
        pCharObj->field_5_remap = remap;
        if (remap != 0xFF)
        {
            pCharObj->field_80_sprite_ptr->SetRemap(remap);
        }
        pDst->field_168_game_object->set_rotation_433A30(pSrc->GetRotation());
        pDst->field_168_game_object->field_16 = 1;
        pDst->field_168_game_object->field_84 = pSrc->field_168_game_object->field_84;
    }
    return pDst;
}

WIP_FUNC(0x471060)
void PedManager::DoIanTest_471060(s16 key)
{
    WIP_IMPLEMENTED;

    switch (key)
    {
        case DIK_9:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetOnFire();
            break;
        case DIK_8:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetVisible();
            break;

        case DIK_7:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetInvisible();
            break;
    }
}

MATCH_FUNC(0x4710c0)
Ped* PedManager::PedById(s32 pedId)
{
    for (Ped* pPedIter = gPedPool_6787B8->field_0_pool.field_4_pPrev; pPedIter; pPedIter = pPedIter->mpNext)
    {
        if (pPedIter->field_200_id == pedId)
        {
            return pPedIter;
        }
    }
    return NULL;
}

STUB_FUNC(0x470330)
void PedManager::Dummies_470330()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x471110)
PedPool::~PedPool()
{
}
