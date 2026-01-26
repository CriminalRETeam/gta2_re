#include "Car_BC.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Char_Pool.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "Door_4D4.hpp"
#include "Firefighters.hpp"
#include "Fix16_Rect.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "PublicTransport.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Shooey_CC.hpp"
#include "Taxi_4.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_8.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(Car_214*, gCar_214_705F20, 0x705F20);
DEFINE_GLOBAL(Car_6C*, gCar_6C_677930, 0x677930);
DEFINE_GLOBAL(Car_BC_Pool*, gCar_BC_Pool_67792C, 0x67792C);
DEFINE_GLOBAL(Car_78_Pool*, gCar_78_Pool_677CF8, 0x677CF8);
DEFINE_GLOBAL(TrailerPool*, gTrailerPool_66AC80, 0x66AC80);
DEFINE_GLOBAL(Car_14*, gCar_14_677934, 0x677934);
DEFINE_GLOBAL(s32, dword_6772AC, 0x6772AC);

// This is not used outside this file.
// In fact, it's only allocated and deallocated, it's never used.
DEFINE_GLOBAL(Sprite*, gSprite_Unused_677938, 0x677938);
DEFINE_GLOBAL(Fix16, gFix16_6777CC, 0x6777CC);
DEFINE_GLOBAL(Fix16, dword_6778A0, 0x6778A0);
DEFINE_GLOBAL(Fix16, k_dword_6777FC, 0x6777FC);
DEFINE_GLOBAL(CarInfo_2C*, gCarInfo_2C_66AB78, 0x66AB78);
DEFINE_GLOBAL(ModelPhysics_48*, gCarInfo_48_66AB70, 0x66AB70);
DEFINE_GLOBAL(s16, DAT_677CFC, 0x677CFC);
DEFINE_GLOBAL(struct_4, stru_67737C, 0x67737c);

DEFINE_GLOBAL(Fix16, dword_6771F0, 0x6771F0);
DEFINE_GLOBAL(Ang16, word_677326, 0x677326);
DEFINE_GLOBAL(Fix16, unk_6772A4, 0x6772A4);

// Indicates if Car_2 is initialised
// It can probably turned into a static variable inside Car_2
DEFINE_GLOBAL(char_type, gbRngRemapTableDone_679C0A, 0x679C0A);

// Array of values used by Car_2.
// It can probably turned into a static variable inside Car_2
DEFINE_GLOBAL_ARRAY(u16, gRngRemapTable_679320, 1000, 0x679320);
DEFINE_GLOBAL(Fix16, dword_6777D0, 0x6777D0);
DEFINE_GLOBAL(s32, dword_677888, 0x677888);
DEFINE_GLOBAL(Fix16, dword_6778D0, 0x6778D0);
DEFINE_GLOBAL(Fix16, DAT_006FF744, 0x006FF744); //, 0x147, 0xUNKNOWN);
DEFINE_GLOBAL(Fix16, DAT_006FF774, 0x006FF774); //, 0x0, 0xUNKNOWN);
DEFINE_GLOBAL(Fix16, DAT_006FF570, 0x6FF570);
DEFINE_GLOBAL(Fix16, DAT_006FF7E8, 0x6FF7E8);
DEFINE_GLOBAL(s8, DAT_006FF8C4, 0x6FF8C4);
DEFINE_GLOBAL(s8, DAT_006FF8C5, 0x6FF8C5);
DEFINE_GLOBAL(Fix16, DAT_006FF778, 0x6ff778);
DEFINE_GLOBAL(Fix16, dword_6F7690, 0x6F7690);
DEFINE_GLOBAL(Fix16, dword_6F77D4, 0x6F77D4);
DEFINE_GLOBAL(Ang16, dword_6F804C, 0x6F804C);
DEFINE_GLOBAL(Ang16, word_6F771E, 0x6F771E);
DEFINE_GLOBAL(Ang16, word_67791C, 0x67791C);
DEFINE_GLOBAL_INIT(Fix16, dword_6777A0, Fix16(0x333, 0), 0x6777A0);

DEFINE_GLOBAL(Fix16_Point, stru_6778A8, 0x6778A8);
DEFINE_GLOBAL(Fix16, dword_677C38, 0x677C38);
DEFINE_GLOBAL(Fix16, dword_677C30, 0x677C30);
DEFINE_GLOBAL(Fix16, dword_677C48, 0x677C48);
DEFINE_GLOBAL(Fix16, dword_677B94, 0x677B94);
DEFINE_GLOBAL(Fix16, dword_6779F8, 0x6779F8);
DEFINE_GLOBAL_INIT(Fix16, dword_677908, Fix16(1), 0x677908);

DEFINE_GLOBAL(Fix16, dword_705DDC, 0x705DDC);
DEFINE_GLOBAL(Ang16, word_705F10, 0x705F10);

DEFINE_GLOBAL(Fix16, dword_677218, 0x677218);
DEFINE_GLOBAL(Fix16, k_dword_676984, 0x676984);
DEFINE_GLOBAL(Fix16, k_dword_6778B4, 0x6778B4);
DEFINE_GLOBAL(Fix16, k_dword_6778E0, 0x6778E0);
DEFINE_GLOBAL(Fix16, k_dword_6777D4, 0x6777D4);

DEFINE_GLOBAL(Fix16, k_dword_6772CC, 0x6772CC);
EXTERN_GLOBAL(u8, byte_6F8EDC);

DEFINE_GLOBAL(Fix16, k_dword_66AB38, 0x66AB38);

MATCH_FUNC(0x5639c0)
void sub_5639C0()
{
    gCarInfo_2C_66AB78 = NULL;
    gCarInfo_48_66AB70 = NULL;
}

MATCH_FUNC(0x447640)
void sub_447640()
{
    DAT_677CFC = 0;
}

STUB_FUNC(0x4476f0)
void Car_78::sub_4476F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x447710)
char_type Car_78::sub_447710()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x447970)
void Car_78::sub_447970()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x447ca0)
bool Car_78::GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 maybe_direction)
{
    field_28_junc_idx = gRouteFinder_6FFDC8->StartRoute_58A190(dword_677C38.ToInt(),
                                                               dword_677C30.ToInt(),
                                                               (dword_677C48 - dword_677B94).ToInt(),
                                                               x,
                                                               y,
                                                               z,
                                                               maybe_direction);
    if (field_28_junc_idx > 0)
    {
        field_0->field_60->field_14_target_x = Fix16(x);
        field_0->field_60->field_18_target_y = Fix16(y);
        field_0->field_60->field_1C_target_z = dword_6779F8;
        field_56 = 0;
        return true;
    }
    return false;
}

STUB_FUNC(0x447d40)
char_type Car_78::sub_447D40(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448270)
char_type Car_78::sub_448270()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4482c0)
char_type Car_78::sub_4482C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448770)
char_type Car_78::sub_448770()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448ce0)
char_type Car_78::sub_448CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44a1f0)
char_type Car_78::sub_44A1F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44af00)
s16 Car_78::sub_44AF00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44d1d0)
char_type Car_78::sub_44D1D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44e0c0)
void Car_78::sub_44E0C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x44e560)
void Car_78::sub_44E560()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451980)
void Car_78::sub_451980()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451fa0)
void Car_78::sub_451FA0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451ff0)
s32 Car_78::sub_451FF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452060)
char_type Car_78::sub_452060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452a20)
s16 Car_78::sub_452A20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452df0)
s16 Car_78::sub_452DF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453470)
s16 Car_78::sub_453470()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453590)
s32* Car_78::sub_453590(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x4537d0)
void Car_78::sub_4537D0()
{
    WIP_IMPLEMENTED;

    if ((this->field_0->field_A6 & 2) != 2 && (this->field_0->field_A6 & 1) != 1)
    {
        s16 k2 = 2;
        if (stru_6F6784.get_int_4F7AE0(&k2) <= 0)
        {
            field_0->field_A6 |= 1;
        }
        else
        {
            field_0->field_A6 |= 2;
        }
    }
}

MATCH_FUNC(0x4538b0)
void Car_78::sub_4538B0()
{
    if (this->field_24_flags & 0x80)
    {
        this->field_0->field_A6 &= ~2u;
        this->field_0->field_A6 &= ~1u;
    }
}

MATCH_FUNC(0x453990)
void Car_78::sub_453990(s32 a2)
{
    if (this->field_14 < a2)
    {
        this->field_14 = a2;
    }
}

STUB_FUNC(0x4539b0)
Car_BC* Car_78::sub_4539B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4539d0)
u8* Car_78::sub_4539D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453a40)
s32 Car_78::sub_453A40()
{
    NOT_IMPLEMENTED;
    return 0;
}

// TODO: Move
STUB_FUNC(0x447650)
void __stdcall sub_447650()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x453bb0)
void Car_78::sub_453BB0()
{
    WIP_IMPLEMENTED;

    sub_447650();
    this->field_10 = this->field_0->field_50_car_sprite->field_0;
    this->field_4C = Ang16::GetAngleFace_4F78F0(field_10);
    if (field_2B != -1)
    {
        field_2B++;
    }
    sub_453470();
}

MATCH_FUNC(0x453bf0)
void Car_78::SetCar_453BF0(Car_BC* a2)
{
    field_0 = a2;
}

STUB_FUNC(0x453c00)
u8* Car_78::sub_453C00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453d80)
s32 Car_78::PoolAllocate()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453cb0)
Car_78::Car_78()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5c8680)
void Car_214::sub_5C8680(u8 idx)
{
    field_0[idx].field_8_type = 0;
    field_0[idx].field_C = 0;
    field_0[idx].field_0 = 0;
    field_0[idx].field_14 = 2;
    field_0[idx].field_4_O2C->Dealloc_5291B0();
    field_0[idx].field_4_O2C = 0;
}

MATCH_FUNC(0x5c86c0)
char_type Car_214::sub_5C86C0(s32* pType, s32* f_C, s32 f_0, Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 a8, Fix16 a9)
{
    Car_18* pIter = field_0;
    for (u8 idx = 0; idx < GTA2_COUNTOF(field_0); idx++)
    {
        if (pIter->field_8_type == 0)
        {
            pIter->field_8_type = *pType;
            pIter->field_C = *f_C;
            pIter->field_0 = f_0;
            pIter->field_14 = 1;
            pIter->field_4_O2C = gObject_5C_6F8F84->NewTouchPoint_529950(161, // save point?
                                                                         xpos,
                                                                         ypos,
                                                                         zpos,
                                                                         word_705F10,
                                                                         a8,
                                                                         a9,
                                                                         dword_705DDC);
            pIter->field_4_O2C->field_27 = idx;
            field_210_count++;
            return idx;
        }
        pIter++;
    }
    return -1;
}

MATCH_FUNC(0x5c8750)
void Car_214::sub_5C8750()
{
    field_210_count = 0;
    Car_18* pOff = &field_0[0];
    for (u8 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        pOff->field_10_remap_rng = i;
        pOff->field_8_type = 0;
        pOff->field_C = 0;
        pOff->field_0 = 0;
        pOff->field_14 = 1;
        pOff++;
    }
}

STUB_FUNC(0x5c8780)
u16* Car_214::sub_5C8780(u8 a2, Sprite* pCarSprite)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x47bd00)
Car_2::Car_2()
{
    if (gbRngRemapTableDone_679C0A == false)
    {
        gbRngRemapTableDone_679C0A = true;
        for (u16 i = 0; i < 1000; i++)
        {
            gRngRemapTable_679320[i] = i;
        }

        for (u16 j = 0; j < 1000; j++)
        {
            s16 tmp = 1000;
            u16 idx = stru_6F6784.get_int_4F7AE0(&tmp);
            s16 next = gRngRemapTable_679320[j];
            gRngRemapTable_679320[j] = gRngRemapTable_679320[idx];
            gRngRemapTable_679320[idx] = next;
        }
    }
    field_0 = 0;
}

MATCH_FUNC(0x47bd90)
void Car_2::IncNextRngRemapIdx_47BD90()
{
    field_0++;
    if (field_0 == 1000)
    {
        field_0 = 0;
    }
}

STUB_FUNC(0x444980)
void Car_6C::sub_444980()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x444ab0)
u32 Car_6C::sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x444cf0)
Car_BC* Car_6C::sub_444CF0(s32 car_model_type, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x444E40)
char Car_BC::sub_444E40(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    const u8 x_int = xpos.ToInt();
    const u8 y_int = ypos.ToInt();
    const u8 z_int = zpos.ToInt();
    gmp_block_info* pBlockInfo = gMap_0x370_6F6268->get_block_4DFE10(x_int, y_int, z_int - 1);

    if (pBlockInfo)
    {

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(4, pBlockInfo))
        {
            return Car_BC::sub_445EC0(Fix16(x_int), (dword_677218 + (y_int)), 4);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(2, pBlockInfo))
        {
            return Car_BC::sub_445EC0((dword_677218 + (x_int)), (dword_6777D0 + (y_int)), 2);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(3, pBlockInfo))
        {
            return Car_BC::sub_445EC0(Fix16(x_int), (dword_677218 + (y_int)), 3);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(1, pBlockInfo))
        {
            return Car_BC::sub_445EC0((dword_677218 + (x_int)), Fix16(y_int), 1);
        }
    }
    return 0;
}

STUB_FUNC(0x445EC0)
char Car_BC::sub_445EC0(Fix16 xpos, Fix16 ypos, s32 maybe_direction)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x444f80)
Car_BC* Car_6C::GetNearestCarFromCoord_444F80(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord_444FC0(x, y, z, 1, pPed, 1);
}

MATCH_FUNC(0x444fa0)
Car_BC* Car_6C::GetNearestEnterableCarFromCoord_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord_444FC0(x, y, z, 0, pPed, 0);
}

STUB_FUNC(0x444FC0)
Car_BC* Car_6C::DoGetNearestCarFromCoord_444FC0(Fix16 xpos,
                                                Fix16 ypos,
                                                Fix16 zpos,
                                                s32 bMatchDriverless,
                                                Ped* pPed,
                                                char_type bIgnorePedRestrictions)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x445210)
Car_BC* Car_6C::sub_445210(Sprite* a1, u8 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4458b0)
Car_BC* Car_6C::sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x446230)
Car_BC* Car_6C::SpawnCarAt_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 maybe_w_scale)
{
    NOT_IMPLEMENTED;
    // TODO: Standalone implement me
    return 0;
}

STUB_FUNC(0x446530)
Trailer* Car_6C::sub_446530(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_idx, s32 trailer_idx)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x446730)
void Car_6C::sub_446730(Car_BC* pCar)
{
    gPurpleDoom_3_679210->Remove_477B00(pCar->field_50_car_sprite);
    gCar_BC_Pool_67792C->Remove(pCar);
}

MATCH_FUNC(0x4466c0)
void Car_6C::sub_4466C0(s32 car_kind)
{
    switch (car_kind)
    {
        case 1:
            --this->field_28_recycled_cars;
            break;

        case 2:
            --this->field_40_proto_recycled_cars;
            break;

        case 10:
            --this->field_48;
            break;

        case 4:
            --this->field_2C;
            break;

        case 5:
            --this->field_30;
            break;

        case 6:
            --this->field_34_unit_cars;
            break;

        case 7:
            --this->field_38;
            break;

        case 8:
            --this->field_3C_mission_cars;
            break;
        case 9:
            --this->field_44;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x446760)
void Car_6C::sub_446760()
{
    // Enable free shopping once all KF's are passed
    s32* pSecretsPassed = gfrosty_pasteur_6F8060->field_338_secrets_passed;
    if (pSecretsPassed)
    {
        const u32 total_secrets = gfrosty_pasteur_6F8060->field_318_total_secrets;
        if (total_secrets > 0)
        {
            if (!this->field_69_do_free_shopping && *pSecretsPassed >= total_secrets)
            {
                this->field_69_do_free_shopping = 1;
            }
        }
    }
}

MATCH_FUNC(0x446790)
void Car_6C::CarsService_446790()
{
    sub_446760();

    gTaxi_4_704130->PopAll_457BC0();

    field_55 = 0;

    gCar_BC_Pool_67792C->field_0_pool.UpdatePool();

    gCar_14_677934->GenerateTraffic_583670();

    if (field_54 > 0)
    {
        field_54--;
    }
}

WIP_FUNC(0x446870)
bool Car_6C::CanAlloc_446870(s32 type)
{
    WIP_IMPLEMENTED;

    bool result;
    switch (type)
    {
        case 1:
            result = gCar_6C_677930->field_28_recycled_cars + gCar_6C_677930->field_40_proto_recycled_cars < 15;
            break;
        case 4:
            result = this->field_2C < 1;
            break;
        case 5:
            result = this->field_30 < 1;
            break;
        case 6:
            result = this->field_34_unit_cars < 5;
            break;
        case 7:
            result = this->field_38 < 11;
            break;
        case 8:
            result = this->field_3C_mission_cars < 23;
            break;
        case 9:
            result = this->field_44 < 200;
            break;
        case 10:
            result = this->field_48 < 11;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

WIP_FUNC(0x446930)
u32 Car_6C::CanAllocateOfType_446930(s32 type)
{
    WIP_IMPLEMENTED;

    BOOL bCanAlloc; // eax

    switch (type)
    {
        case 1:
            bCanAlloc = gCar_6C_677930->field_28_recycled_cars + gCar_6C_677930->field_40_proto_recycled_cars < 16;
            break;
        case 4: // kfc ?
            bCanAlloc = this->field_2C < 2;
            break;
        case 5: // fire engines?
            bCanAlloc = this->field_30 < 2;
            break;
        case 6: // police cars
            bCanAlloc = this->field_34_unit_cars < 6;
            break;
        case 7:
            bCanAlloc = this->field_38 < 12;
            break;
        case 8:
            bCanAlloc = this->field_3C_mission_cars < 24;
            break;
        case 9:
            bCanAlloc = this->field_44 < 200;
            break;
        case 10:
            bCanAlloc = this->field_48 < 12;
            break;
        default:
            bCanAlloc = 0; // LOBYTE =
            break;
    }
    return bCanAlloc;
}

STUB_FUNC(0x4469f0)
// There are still something missing here.
// But the structure it seems to be complete
Car_6C::Car_6C()
{
    NOT_IMPLEMENTED;
    if (!gCar_BC_Pool_67792C)
    {
        gCar_BC_Pool_67792C = new Car_BC_Pool();
        if (!gCar_BC_Pool_67792C)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8318);
        }
    }

    if (!gCarPhysicsPool_6FE3E0)
    {
        gCarPhysicsPool_6FE3E0 = new CarPhyisicsPool();
        if (!gCarPhysicsPool_6FE3E0)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8323);
        }
    }

    if (!gCar_14_677934)
    {
        gCar_14_677934 = new Car_14();
        if (!gCar_14_677934)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8329);
        }
    }

    if (!gCar_78_Pool_677CF8)
    {
        gCar_78_Pool_677CF8 = new Car_78_Pool();
        if (!gCar_78_Pool_677CF8)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8335);
        }
    }

    if (!gTrailerPool_66AC80)
    {
        gTrailerPool_66AC80 = new TrailerPool();
        if (!gTrailerPool_66AC80)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8341);
        }
    }

    field_64 = gFix16_6777CC;
    field_60 = 0;
    field_4 = 0;
    field_14 = 1;
    field_9 = 0;
    field_8 = 0;
    field_A = 0;
    field_B = 0;
    field_1A = 0;
    field_1C = 0;
    field_28_recycled_cars = 0;
    field_40_proto_recycled_cars = 0;
    field_30 = 0;
    field_2C = 0;
    field_34_unit_cars = 0;
    field_38 = 0;
    field_3C_mission_cars = 0;
    field_44 = 0;

    stru_67727C.sub_4207E0();
    stru_67737C.sub_4207E0();

    sub_5639C0();
    sub_447640();

    field_20 = 4;
    field_24 = 1;

    if (!gSprite_Unused_677938)
    {
        gSprite_Unused_677938 = new Sprite();
        if (!gSprite_Unused_677938)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8375);
        }
    }

    // On version 9.6f this is call to a class method
    // that initialises the next two fields
    //field_4C = 0;
    //field_50_tv_van_dir = 0;
    field_4C_tv_van_dir = Fix16_Point(Fix16(0), Fix16(0));
    field_54 = 0;
    field_55 = 0;
    field_58_model_to_check_destroy = car_model_enum::none;
    field_5C = 0;
    field_C_model_unk = car_model_enum::none;
    field_69_do_free_shopping = bDo_free_shopping_67D6CD;
}

// Some SEH stuff, prob needs some dtors moving to or from another file
STUB_FUNC(0x446dc0)
Car_6C::~Car_6C()
{
    NOT_IMPLEMENTED;

    if (gCar_BC_Pool_67792C)
    {
        GTA2_DELETE_AND_NULL(gCar_BC_Pool_67792C);
    }

    if (gCarPhysicsPool_6FE3E0)
    {
        GTA2_DELETE_AND_NULL(gCarPhysicsPool_6FE3E0);
    }

    if (gCar_14_677934)
    {
        GTA2_DELETE_AND_NULL(gCar_14_677934);
    }

    if (gCar_78_Pool_677CF8)
    {
        GTA2_DELETE_AND_NULL(gCar_78_Pool_677CF8);
    }

    if (gTrailerPool_66AC80)
    {
        GTA2_DELETE_AND_NULL(gTrailerPool_66AC80);
    }

    if (gSprite_Unused_677938)
    {
        GTA2_DELETE_AND_NULL(gSprite_Unused_677938);
    }

    field_4 = 0;
}

STUB_FUNC(0x4403a0)
Ang16 Car_BC::sub_4403A0()
{
    NOT_IMPLEMENTED;

    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    Fix16 w_fp = Fix16(pCarInfo->w) / 2;
    Fix16 h_fp = Fix16(pCarInfo->h) / 2;
    return Fix16::atan2_fixed_405320(h_fp, w_fp);
}

MATCH_FUNC(0x439ec0)
bool Car_BC::IsPoliceCar_439EC0()
{
    bool b = field_84_car_info_idx == car_model_enum::COPCAR || field_84_car_info_idx == car_model_enum::SWATVAN ||
        field_84_car_info_idx == car_model_enum::EDSELFBI;
    return b;
}

// https://decomp.me/scratch/I0oG6
STUB_FUNC(0x439ee0)
Fix16 Car_BC::GetDamageFactorOnSpeed_439EE0()
{
    NOT_IMPLEMENTED;

    if (field_74_damage < 16000)
    {
        return dword_6777D0;
    }
    else
    {
        return Fix16(32001 - field_74_damage) / Fix16(4000);
    }
}

MATCH_FUNC(0x439f30)
Fix16 Car_BC::GetMaxSpeed_439F30()
{
    ModelPhysics_48* pModelPhysics = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx);
    // Max speed is limited by how smashed up the car is
    return GetDamageFactorOnSpeed_439EE0() * pModelPhysics->field_28_max_speed;
}

MATCH_FUNC(0x439f80)
wchar_t* Car_BC::GetCarStr_439F80()
{
    sprintf(byte_67CE50, "car%d%c%c", field_84_car_info_idx, 0, 0);
    return gText_0x14_704DFC->Find_5B5F90(byte_67CE50);
}

STUB_FUNC(0x439fb0)
u32* Car_BC::sub_439FB0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a0e0)
Fix16_Point Car_BC::get_rear_wheel_offset_43A0E0()
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->sub_454840(field_84_car_info_idx);
    return Fix16_Point(Fix16(0), pInfo->field_8_rear_wheel_offset);
}

MATCH_FUNC(0x43a120)
Fix16 Car_BC::get_mass_43A120()
{
    ModelPhysics_48* pCarInfo = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx);

    if (sub_4215C0())
    {
        return sub_421910(pCarInfo->field_4_mass) * (dword_6777D0 + dword_6777A0);
    }
    else
    {
        return sub_421910(pCarInfo->field_4_mass);
    }
}

MATCH_FUNC(0x43a1d0)
Fix16 Car_BC::get_anti_strngth_43A1D0()
{
    return gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx)->field_2C_anti_strngth;
}

MATCH_FUNC(0x43a1f0)
bool Car_BC::is_bus_43A1F0()
{
    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(this))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x43a230)
bool Car_BC::sub_43A230()
{
    return field_50_car_sprite->field_38_zoom != 0;
}

STUB_FUNC(0x43a240)
Fix16 Car_BC::sub_43A240()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a3c0)
bool Car_BC::IsCarInAir_43A3C0()
{
    if (!field_58_physics)
    {
        return false;
    }
    return field_58_physics->field_98_surface_type == 6; // 6 = air surface (or no surface)
}

STUB_FUNC(0x43a3e0)
s16* Car_BC::GetOrientationAngle_43A3E0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a450)
Fix16_Point Car_BC::get_linvel_43A450()
{
    if (is_train_model())
    {
        Car_BC* carObj = gPublicTransport_181C_6FF1D4->GetLeadTrainCar_57B540(this);
        if (!carObj->field_58_physics)
        {
            return stru_6778A8;
        }
        else
        {
            return carObj->field_58_physics->get_linvel_447010();
        }
    }
    else
    {
        if (!this->field_58_physics)
        {
            return stru_6778A8;
        }
        else
        {
            return this->field_58_physics->get_linvel_447010();
        }
    }
}

MATCH_FUNC(0x43a4c0)
Fix16 Car_BC::GetVelocity_43A4C0()
{
    if (!field_58_physics)
    {
        return 0;
    }
    else
    {
        return field_58_physics->field_0_vel_read_only.GetLength();
    }
}

MATCH_FUNC(0x43a590)
Fix16 Car_BC::sub_43A590()
{
    return gCarInfo_808_678098->sub_454840(sub_43A850())->field_0;
}

MATCH_FUNC(0x43a5b0)
Fix16 Car_BC::sub_43A5B0()
{
    const car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (pInfo->w < pInfo->h)
    {
        Fix16 t = dword_6F6850.list[pInfo->w];
        return t;
    }
    else
    {
        Fix16 tt = dword_6F6850.list[pInfo->h];
        return tt;
    }
}

MATCH_FUNC(0x43a600)
void Car_BC::RemoveAllDamage()
{
    sub_43D400();
    if (field_64_pTrailer)
    {
        field_64_pTrailer->field_C_pCarOnTrailer->sub_43D400();
    }
}

MATCH_FUNC(0x43a680)
bool Car_BC::AllowResprayOrPlates()
{
    return (!is_train_model() && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 2) != 2 &&
            !is_FBI_car_411920() && !field_0_qq.sub_5A71A0() && !IsArmyModel(field_84_car_info_idx));
}

MATCH_FUNC(0x43a6f0)
bool Car_BC::IsNotCurrentRemap(u8 remap)
{
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->has_remaps() && field_50_car_sprite->field_24_remap != remap;
}

MATCH_FUNC(0x43a730)
bool Car_BC::IsNotCurrentRemapOfCarAndTrailerCar(u8 remap)
{
    if (field_64_pTrailer)
    {
        // Check trailer car
        return IsNotCurrentRemap(remap) || field_64_pTrailer->field_C_pCarOnTrailer->IsNotCurrentRemap(remap);
    }
    return IsNotCurrentRemap(remap);
}

MATCH_FUNC(0x43a780)
void Car_BC::SetCarRemap(u8 remap)
{
    if (IsNotCurrentRemap(remap))
    {
        field_50_car_sprite->SetRemap(remap);
    }

    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_C_pCarOnTrailer->IsNotCurrentRemap(remap))
        {
            field_64_pTrailer->field_C_pCarOnTrailer->field_50_car_sprite->SetRemap(remap);
        }
    }
}

MATCH_FUNC(0x43a7d0)
void Car_BC::AssignRandomRemap_43A7D0()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (pCarInfo->num_remaps)
    {
        const u32 remap_idx = gRngRemapTable_679320[gCar_6C_677930->field_10_remap_rng.field_0] % (pCarInfo->num_remaps + 1);
        if (remap_idx == pCarInfo->num_remaps)
        {
            this->field_50_car_sprite->field_34 = 2;
        }
        else
        {
            field_50_car_sprite->SetRemap(pCarInfo->remap[remap_idx]);
        }

        gCar_6C_677930->field_10_remap_rng.IncNextRngRemapIdx_47BD90();
    }
}

// https://decomp.me/scratch/zoRIL
STUB_FUNC(0x43a850)
char_type Car_BC::sub_43A850()
{
    if (field_54_driver && !field_54_driver->IsField238_45EDE0(2))
    {
        if (!is_train_model())
        {
            if (field_60 && field_60->field_22 != 0)
            {
                if (field_84_car_info_idx == car_model_enum::TANK)
                {
                    return car_model_enum::HOTDOG_D4;
                }
                return car_model_enum::HOTDOG_D1;
            }

            switch (field_84_car_info_idx)
            {
                case car_model_enum::SWATVAN:
                case car_model_enum::GUNJEEP:
                case car_model_enum::COPCAR:
                case car_model_enum::EDSELFBI:
                case car_model_enum::JEEP:
                    return car_model_enum::HOTDOG_D1;

                case car_model_enum::TANK:
                    return car_model_enum::HOTDOG_D4;
            }
        }
        return this->field_84_car_info_idx;
    }

    if (this->field_68 < dword_6777D0)
    {
        return car_model_enum::HOTDOG_D3;
    }

    if (field_84_car_info_idx == car_model_enum::TRUKCAB2 && !this->field_64_pTrailer)
    {
        return car_model_enum::TRUKCAB1;
    }

    if (field_84_car_info_idx == car_model_enum::TRUKCAB1 && this->field_64_pTrailer)
    {
        return car_model_enum::TRUKCAB2;
    }

    return this->field_84_car_info_idx;
}

MATCH_FUNC(0x43a950)
void Car_BC::sub_43A950()
{
    CarPhysics_B0* pCarPhysics = field_58_physics;
    pCarPhysics->field_91_is_foot_brake_on = 1;
    pCarPhysics->field_93_is_forward_gas_on = 0;
    pCarPhysics->field_94_is_backward_gas_on = 0;
    pCarPhysics->field_95 = 0;
}

MATCH_FUNC(0x43a970)
void Car_BC::sub_43A970()
{
    field_58_physics->field_92_is_hand_brake_on = 1;
    CarPhysics_B0* pCarPhysics = field_58_physics;
    pCarPhysics->field_91_is_foot_brake_on = 1;
    pCarPhysics->field_93_is_forward_gas_on = 0;
    pCarPhysics->field_94_is_backward_gas_on = 0;
    pCarPhysics->field_95 = 0;
}

MATCH_FUNC(0x43a9a0)
void Car_BC::SetDriver(Ped* pNewDriver)
{
    char hand_brake_on; // [esp+Ch] [ebp+4h]
    if (!pNewDriver)
    {
        CarPhysics_B0* pCarPhysics = this->field_58_physics;
        if (pCarPhysics)
        {
            Ped* pOldDriver = this->field_54_driver;
            if (pOldDriver && pOldDriver->field_15C_player)
            {
                hand_brake_on = pCarPhysics->field_92_is_hand_brake_on;
            }
            else
            {
                hand_brake_on = 1;
            }
            pCarPhysics->HandleUserInputs_55A860(0, 0, 0, 0, hand_brake_on);
        }
    }
    this->field_54_driver = pNewDriver;
}

STUB_FUNC(0x43a9f0)
void Car_BC::sub_43A9F0()
{
    if (!field_54_driver && (field_78_flags & 0x80) && field_7C_uni_num != 2 && (field_A4 & 8) == 0 && field_74_damage != 32001)
    {
        if ((field_A4 & 0x1C) == 0)
        {
            field_A5 = 12;
            field_A4 |= 8;

            if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
            {
                if (inline_check_0x2_info_421700())
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                }
                else
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
                }
            }

            if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
            {
                if (inline_check_0x2_info_421700())
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
                }
                else
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
                }
            }

            if (inline_check_0x4_info_421660())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
            }

            if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
            }

            if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
            }

            field_8E = 50;
        }
    }
}

MATCH_FUNC(0x43aa60)
void Car_BC::Deactivate_43AA60()
{
    gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_50_car_sprite);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_50_car_sprite);
    gCar_BC_Pool_67792C->field_0_pool.sub_420F30(this);
    SetF_88_4214E0();
}

STUB_FUNC(0x43AAE0)
char_type Car_BC::IsThreatToSearchingPed_43AAE0()
{
    NOT_IMPLEMENTED;

    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        return pDriver->IsThreatToSearchingPed_4661F0();
    }
    else
    {
        return 0;
    }
}

STUB_FUNC(0x43aaf0)
char_type Car_BC::sub_43AAF0(Sprite* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43adc0)
void Car_BC::ProcessCarToCarImpact_43ADC0(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x43af10)
bool Car_BC::CanExitCar_43AF10()
{
    if (sub_43A230())
    {
        return false;
    }

    if (field_58_physics)
    {
        return field_58_physics->IsNearlyStopped_5636E0();
    }

    return true;
}

MATCH_FUNC(0x43af40)
void Car_BC::sub_43AF40()
{
    if (field_5C)
    {
        field_5C->field_18 = k_dword_6778E0;
        field_A6 &= ~0x20u;
    }
}

MATCH_FUNC(0x43af60)
void Car_BC::sub_43AF60()
{
    if (field_5C)
    {
        field_A6 |= 0x20u;
    }
}

MATCH_FUNC(0x43afe0)
char_type Car_BC::sub_43AFE0(s32 target_door)
{
    Fix16 y;
    Fix16 x;
    Fix16_Rect fr;

    sub_43B5A0(target_door, &x, &y);
    if (gOrca_2FD4_6FDEF0->sub_5540E0(field_50_car_sprite->field_14_xy.x.ToInt(),
                                      field_50_car_sprite->field_14_xy.y.ToInt(),
                                      field_50_car_sprite->field_1C_zpos.ToInt(),
                                      x.ToInt(),
                                      y.ToInt()))
    {
        byte_6F8EDC = 1;
        fr.ComputeCollisionPrism_4204D0(x, y, k_dword_6772CC, field_50_car_sprite->field_1C_zpos);
        fr.ExpandToIncludePoint_59DEE0(field_50_car_sprite->field_14_xy.x, field_50_car_sprite->field_14_xy.y);
        gCollide_C_6791FC->field_8_bUnknown = 1;
        if (!fr.CanRectEnterMovementRegion_59DE80() &&
            !gPurpleDoom_1_679208->CheckRectForCollisions_477F60(&fr, 1, 3, field_50_car_sprite) && !gMap_0x370_6F6268->sub_4E11E0(&fr))
        {
            gCollide_C_6791FC->field_8_bUnknown = 0;
            byte_6F8EDC = 0;
            return 1;
        }
        byte_6F8EDC = 0;
        gCollide_C_6791FC->field_8_bUnknown = 0;
    }
    return 0;
}

STUB_FUNC(0x43b140)
bool Car_BC::sub_43B140(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x43b2b0)
bool Car_BC::sub_43B2B0(Ped* pPed)
{
    WIP_IMPLEMENTED;

    s32 useLabel12Branch = 0;
    if (pPed)
    {
        if (field_98 == 5)
        {
            return pPed->IsField238_45EDE0(2);
        }
        if (pPed->IsField238_45EDE0(2) ||
            (pPed->IsField238_45EDE0(5) && pPed->field_25C_car_state == 35 && pPed->field_154_target_to_enter == this))
        {
            useLabel12Branch = 1;
        }
    }
    else
    {
        useLabel12Branch = 1;
    }

    // TODO: Return value uses AL instead of EAX
    if (!useLabel12Branch)
    {
        if (field_98 != 1 && field_98 != 4 && field_98 != 2)
        {
            return 0;
        }
    }
    else
    {
        if (field_98 != 1 && field_98 != 4)
        {
            return 0;
        }
    }
    return 1;
}

MATCH_FUNC(0x43b340)
Car_Door_10* Car_BC::GetDoor(u8 door_idx)
{
    return &field_C_doors[door_idx];
}

MATCH_FUNC(0x43b360)
char_type Car_BC::GetRemap()
{
    return *gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
}

MATCH_FUNC(0x43b380)
void Car_BC::sub_43B380()
{
    u8* pRemap = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
    for (u8 i = 0; i < *pRemap; i++)
    {
        Car_Door_10* pDoor = GetDoor(i);
        pDoor->sub_439E60();
    }
}

MATCH_FUNC(0x43b3d0)
void Car_BC::sub_43B3D0()
{
    u8* pRemap = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
    for (u8 i = 0; i < *pRemap; i++)
    {
        Car_Door_10* pDoor = GetDoor(i);
        pDoor->sub_439EA0();
    }
}

STUB_FUNC(0x43b420)
s32 Car_BC::sub_43B420(s32 a2, u32* a3, u32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43B540)
bool Car_BC::sub_43B540(u8 targetDoor)
{
    s8* array = (s8*)gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
    if (Fix16::Abs(dword_6F6850.sub_41FE70(array[2 * targetDoor + 1])) >= dword_677908)
    {
        return true;
    }
    return false;
}

STUB_FUNC(0x43b5a0)
s32* Car_BC::sub_43B5A0(s32 a2, Fix16* a3, Fix16* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43b730)
char_type Car_BC::sub_43B730()
{
    return gGame_0x40_67E008->sub_4B97E0(field_50_car_sprite, dword_6778D0);
}

MATCH_FUNC(0x43b750)
char_type Car_BC::sub_43B750()
{
    return gGame_0x40_67E008->sub_4B9830(field_50_car_sprite, dword_6778D0);
}

MATCH_FUNC(0x43b770)
void Car_BC::sub_43B770()
{
    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->field_240_occupation != 4)
        {
            pDriver->field_204_killer_id = this->field_70_exploder_ped_id;
            this->field_54_driver->field_290 = this->field_90;
            this->field_54_driver->field_264 = 50;
        }
    }
    field_4_passengers_list.SyncPassengersWithCarState_4716D0(this);
}

STUB_FUNC(0x43b7b0)
void Car_BC::sub_43B7B0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x43b850)
bool Car_BC::sub_43B850(s32 wofly_type_or_state)
{
    WIP_IMPLEMENTED;
    // TODO: Flags is a bitfield ??
    return field_78_flags & 0x600 && wofly_type_or_state != 20 ? true : false;
}

STUB_FUNC(0x43b870)
void Car_BC::sub_43B870(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x43bb90)
s32 Car_BC::sub_43BB90(u8 a1)
{
    switch (a1)
    {
        case 1:
            return 12;

        case 2:
            return 13;

        case 3:
            return 14;

        default:
            return 12;
    }
}

WIP_FUNC(0x43bbc0)
void Car_BC::sub_43BBC0()
{
    WIP_IMPLEMENTED;

    if (gFirefighterPool_54_67D4C0->sub_4A8820(this))
    {
        Sprite* pCarSprite = this->field_50_car_sprite;
        s32 pedId = this->field_70_exploder_ped_id;

        Object_2C* pExplosion =
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCarSprite->field_14_xy.x, pCarSprite->field_14_xy.y, 4, word_67791C, 4, pedId);
        if (pExplosion)
        {
            field_50_car_sprite->DispatchCollisionEvent_5A3100(pExplosion->field_4, gFix16_6777CC, gFix16_6777CC, word_67791C);
        }
    }
}

MATCH_FUNC(0x43bc30)
void Car_BC::SetupCarPhysicsAndSpriteBinding_43BC30()
{
    if (!field_58_physics)
    {
        AllocCarPhysics_4419E0();
        field_58_physics->SetCar_5638C0(this);
        field_58_physics->SetSprite_563560(this->field_50_car_sprite);
    }
    else
    {
        if (is_train_model())
        {
            field_58_physics->SetSprite_563560(this->field_50_car_sprite);
        }
    }
}

MATCH_FUNC(0x43bca0)
void Car_BC::SetupCarPhysicsAndSpriteBinding_43BCA0()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->SetupCarPhysicsAndSpriteBinding_408190();
    }
    else
    {
        SetupCarPhysicsAndSpriteBinding_43BC30();
    }
}

MATCH_FUNC(0x43bd00)
void Car_BC::DeAllocateCarPhysics_43BD00()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->DeAllocateCarPhysics_4081B0();
    }
    else
    {
        DeAllocateCarPhysics_441A10();
    }
}

STUB_FUNC(0x43bd40)
char_type Car_BC::sub_43BD40()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43bf10)
void Car_BC::BrakeLightsOn_43BF10()
{
    if ((this->field_A4 & 8) == 0)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        }
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
        }
    }
    this->field_A4 |= 1u;
}

MATCH_FUNC(0x43bf70)
void Car_BC::BrakeLightsOff_43BF70()
{
    if ((this->field_A4 & 8) == 0)
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
    }
    this->field_A4 &= ~1u;
}

STUB_FUNC(0x43bfe0)
char_type Car_BC::sub_43BFE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43c0c0)
void Car_BC::sub_43C0C0()
{
    if ((field_A4 & 8) == 0) // bit 3
    {
        if (inline_check_0x2_info_421700()) // bit 1
        {
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }

        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);

        if (inline_check_0x4_info_421660()) // bit 2
        {
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }

    field_A4 &= ~2u; // bit 1
}

MATCH_FUNC(0x43c1c0)
void Car_BC::sub_43C1C0()
{
    if ((this->field_A4 & 8) != 0)
    {
        Car_BC::sub_43CBE0();
    }

    this->field_A4 = 0;

    if (inline_info_flags_bit2() || is_FBI_car_411920())
    {
        Car_BC::DeactivateEmergencyLights_43C9D0();
    }

    Car_BC::sub_43C0C0();
    Car_BC::BrakeLightsOff_43BF70();
}

MATCH_FUNC(0x43c260)
void Car_BC::sub_43C260()
{
    if (inline_check_0x80_info_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object_2C* p2C = field_0_qq.FindObject2CByModel_5A6A90(172);
        p2C->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c310)
void Car_BC::sub_43C310()
{
    if (inline_check_0x80_info_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object_2C* pLight = field_0_qq.FindObject2CByModel_5A6A90(173);
        if (pLight)
        {
            pLight->Light_527990();
        }
    }
}

MATCH_FUNC(0x43c3c0)
void Car_BC::sub_43C3C0()
{
    if (inline_check_0x80_info_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object_2C* pLight = field_0_qq.FindObject2CByModel_5A6A90(173);
        pLight->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c470)
void Car_BC::sub_43C470()
{
    if (inline_check_0x80_info_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object_2C* pLight = field_0_qq.FindObject2CByModel_5A6A90(173);
        if (pLight)
        {
            pLight->Light_527990();
        }
    }
}

STUB_FUNC(0x43c500)
s32 Car_BC::sub_43C500()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c650)
s32 Car_BC::sub_43C650()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c700)
s32 Car_BC::sub_43C700()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43c840)
void Car_BC::sub_43C840()
{
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);

    if (field_84_car_info_idx != car_model_enum::EDSELFBI)
    {
        Object_2C* p2C = field_0_qq.FindObject2CByModel_5A6A90(171);
        if (p2C)
        {
            p2C->Light_527990();
        }
    }

    if (field_84_car_info_idx == car_model_enum::SWATVAN || field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        Object_2C* p2C = field_0_qq.FindObject2CByModel_5A6A90(173);
        if (p2C)
        {
            p2C->Light_527990();
        }
    }
}

MATCH_FUNC(0x43c920)
void Car_BC::ActivateEmergencyLights_43C920()
{
    if ((field_A4 & 0x1C) == 0)
    {
        if (is_FBI_car_411920())
        {
            field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
        field_A4 |= 4u;
        if (is_FBI_car_411920())
        {
            field_A5 = 8;
        }
        else
        {
            field_A5 = 15;
        }
    }
}

MATCH_FUNC(0x43c9d0)
void Car_BC::DeactivateEmergencyLights_43C9D0()
{
    Car_BC::sub_43C650();
    Car_BC::sub_43C840();
    Car_BC::sub_43C310();
    Car_BC::sub_43C470();
    field_A4 &= ~4u;
    if (is_FBI_car_411920() && field_74_damage != 32001)
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
    }
}

MATCH_FUNC(0x43ca80)
void Car_BC::SyncEmergencyLightState_43CA80()
{
    if ((field_A4 & 0x1C) != 0)
    {
        Car_BC::DeactivateEmergencyLights_43C9D0();
    }
    else
    {
        Car_BC::ActivateEmergencyLights_43C920();
    }
}

STUB_FUNC(0x43cbe0)
s32 Car_BC::sub_43CBE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43cdf0)
Car_BC* Car_BC::sub_43CDF0(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43cf30)
void Car_BC::DamageArea_43CF30(s32 damage_area)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x43d1c0)
bool Car_BC::IsAreaDamaged_43D1C0(s32 damage_area)
{
    switch (damage_area)
    {
        case CarDeltaBitsEnum::BottomRightDamage_2:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2); // 4

        case CarDeltaBitsEnum::BottomLeftDamage_3:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3); // 8

        case CarDeltaBitsEnum::TopRightDamage_1:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1);

        case CarDeltaBitsEnum::TopLeftDamage_0:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0);

        case CarDeltaBitsEnum::WindshieldDamage_4:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::WindshieldDamage_4); // 10

        default:
            return false;
    }
    return false;
}

MATCH_FUNC(0x43d2c0)
void Car_BC::TryDamageArea_43D2C0(u8 damage_area, s32 damageAmount)
{
    if (!IsMaxDamage_40F890() && (field_78_flags & 8) == 0 && (get_anti_strngth_43A1D0() * Fix16(damageAmount, 0)) >= dword_6777D0)
    {
        switch (damage_area)
        {
            case 0:
                DamageArea_43CF30(3);
                break;
            case 1:
                DamageArea_43CF30(2);
                break;
            case 2:
                DamageArea_43CF30(0);
                break;
            case 3:
                DamageArea_43CF30(1);
                break;
            default:
                return;
        }
    }
}

STUB_FUNC(0x43d400)
s32 Car_BC::sub_43D400()
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x43d690)
void Car_BC::ExplodeCar_43D690(s32 a3, Fix16 x, Fix16 y)
{
    WIP_IMPLEMENTED;

    Object_2C* p2C = gObject_5C_6F8F84->CreateExplosion_52A3D0(gFix16_6777CC, gFix16_6777CC, 2, word_67791C, a3, field_70_exploder_ped_id);
    if (p2C)
    {
        field_50_car_sprite->DispatchCollisionEvent_5A3100(p2C->field_4, x, y, word_67791C);
    }
}

WIP_FUNC(0x43d7b0)
void Car_BC::sub_43D7B0(s32 k20Or19)
{
    WIP_IMPLEMENTED;

    if (get_anti_strngth_43A1D0() == gFix16_6777CC || this->field_74_damage == 32001)
    {
        ExplodeCar_43D690(k20Or19, stru_6778A8.x.mValue, stru_6778A8.y.mValue);
    }
    else
    {
        ExplodeCar_Unknown_43D840(k20Or19);
    }
}

WIP_FUNC(0x43d840)
void Car_BC::ExplodeCar_Unknown_43D840(s32 a2)
{
    WIP_IMPLEMENTED;

    char bOcc2; // bl
    s32 g6C_f_58; // eax
    Ped* pExploder; // eax
    Gang_144* pZone; // eax
    Fix16_Point v12; // [esp+Ch] [ebp-14h] BYREF

    bOcc2 = 0;
    if (field_60)
    {
        gHamburger_500_678E30->Cancel_474CC0(field_60);
        this->field_60 = 0;
    }

    if (this->field_74_damage != 32001)
    {
        sub_441380();
        ExplodeCar_43D690(a2, stru_6778A8.x, stru_6778A8.y);
        sub_43B770();
        field_0_qq.sub_5A71F0();

        if (field_54_driver)
        {
            if (field_54_driver->field_240_occupation == ped_ocupation_enum::unknown_2)
            {
                bOcc2 = 1;
            }
        }

        sub_43DB80();

        // TODO: Something wrong here
        Fix16_Point sprite_xy = field_50_car_sprite->get_x_y_443580();
        sub_443710(&sprite_xy);

        this->field_74_damage = 32001;

        field_0_qq.sub_5A6BB0();

        g6C_f_58 = gCar_6C_677930->field_58_model_to_check_destroy;
        if (g6C_f_58 != 87 && g6C_f_58 == this->field_84_car_info_idx)
        {
            gCar_6C_677930->field_5C = 1;
        }

        if (this->field_70_exploder_ped_id)
        {
            pExploder = gPedManager_6787BC->PedById(this->field_70_exploder_ped_id);
            if (pExploder)
            {
                if (pExploder->IsField238_45EDE0(2))
                {
                    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(this) &&
                        gPublicTransport_181C_6FF1D4->field_17C0_bus.field_56_passenger_count >= 10)
                    {
                        pExploder->field_15C_player->field_2D4_scores.sub_593410(this);
                    }
                    else
                    {
                        pExploder->field_15C_player->field_2D4_scores.sub_592DD0(this, pExploder);
                        if (bOcc2)
                        {
                            pExploder->field_15C_player->field_2D4_scores.sub_593220();
                        }
                    }
                    s16 zone_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(this->field_84_car_info_idx);
                    if (zone_idx != -1)
                    {
                        pZone = gGangPool_CA8_67E274->GangByIdx_4BF1C0(zone_idx);
                        pZone->sub_4BEF70(pExploder->field_15C_player->field_2E_idx, 1u);
                    }
                    if (pExploder->field_15C_player)
                    {
                        if (gShooey_CC_67A4B8->sub_485090(this, pExploder->field_15C_player))
                        {
                            if (pExploder->field_20A_wanted_points < 600)
                            {
                                pExploder->field_20A_wanted_points = 600;
                            }
                            else
                            {
                                pExploder->add_wanted_points_470160(200);
                            }
                        }
                    }
                }
            }
        }
    }
}

STUB_FUNC(0x43da90)
s16 Car_BC::sub_43DA90(s16 a2, Fix16_Point* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43db80)
void Car_BC::sub_43DB80()
{
    Ped* pDriver; // eax
    CarPhysics_B0* pPhysics; // eax

    pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->field_240_occupation != 4)
        {
            if (pDriver->field_15C_player)
            {
                pPhysics = this->field_58_physics;
                if (pPhysics)
                {
                    pPhysics->field_8C_state = 1;
                }
            }
            field_54_driver->Kill_46F9D0();
        }
        this->field_54_driver = 0;
    }
    field_4_passengers_list.KillAllPedsAndClearCarRef_4715E0();
}

MATCH_FUNC(0x43dbd0)
void Car_BC::sub_43DBD0()
{
    CarPhysics_B0* pPhysics = this->field_58_physics;
    if (pPhysics)
    {
        pPhysics->field_40_linvel_1 = stru_6778A8;
        this->field_58_physics->field_74_ang_vel_rad = gFix16_6777CC;
    }
}

WIP_FUNC(0x43dc00)
bool Car_BC::sub_43DC00()
{
    WIP_IMPLEMENTED;

    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (field_74_damage == 32001)
    {
        if (field_50_car_sprite->field_4_0x4C_len->field_0_width != field_50_car_sprite->field_C_sprite_4c_ptr->field_0_width)
        {
            return true;
        }
        return false;
    }

    u16 pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(2, pInfo->sprite);
    if (field_50_car_sprite->field_4_0x4C_len->field_0_width !=
        dword_6F6850.list[gGtx_0x106C_703DD4->get_sprite_index_5AA440(pal)->field_4_width])
    {
        return true;
    }
    return false;
}

STUB_FUNC(0x43dc80)
bool Car_BC::sub_43DC80(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43dd50)
bool Car_BC::IsBeingCrushed_43DD50()
{
    return gCrusherPool_94_67A830->IsCarBeingCrushed_4887A0(this);
}

STUB_FUNC(0x43dd60)
void Car_BC::sub_43DD60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43e560)
char_type Car_BC::ManageDrowning_43E560()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43e8d0)
Car_BC* Car_BC::GetCabOrSelf_43E8D0()
{
    if (field_64_pTrailer)
    {
        return field_64_pTrailer->field_8_truck_cab;
    }
    return this;
}

WIP_FUNC(0x43e990)
Ped* Car_BC::GetEffectiveDriver_43E990()
{
    WIP_IMPLEMENTED;

    if (is_on_trailer_421720())
    {
        // TODO: Function chunk here
        return field_64_pTrailer->field_8_truck_cab->field_54_driver;
    }
    else
    {
        return this->field_54_driver;
    }
}

MATCH_FUNC(0x43ea60)
bool Car_BC::OnObjectTouched_43EA60(Object_2C* pObj)
{
    if (pObj->check_is_shop_421060() || pObj->field_18_model == objects::secret_token_266)
    {
        Ped* pPed = Car_BC::GetEffectiveDriver_43E990();
        if (pPed)
        {
            return (pPed->HandlePickupCollision_45DE80(pObj));
        }

        return 0;
    }

    switch (pObj->field_18_model)
    {

        case objects::moving_collect_33_129:
            gPublicTransport_181C_6FF1D4->sub_579A30(this);
            break;

        case objects::moving_collect_34_130:
            GetCabOrSelf_43E8D0()->HandleShops_443C40(pObj);
            return 1;

        case objects::moving_collect_41_137:
            gCranePool_D9C_679FD4->PickUpCar_480E00(this, pObj->field_26_varrok_idx);
            break;

        case objects::blood_spark_143:
            gCrusherPool_94_67A830->CrushCarWithCrusher_4887D0(this, pObj->field_26_varrok_idx);
            break;

        case objects::animating_oil_8:
        case objects::oil_9:
            field_58_physics->SpinOutOnOil_559BA0();
            break;
        case objects::moving_collect_43_139:
            field_58_physics->sub_559E20(pObj); // mine?
            break;
        case objects::bus_stop_marker_161:
            gCar_214_705F20->sub_5C8780(pObj->field_27, this->field_50_car_sprite);
            break;

        case objects::small_arrow_141:
            if (field_54_driver || field_88 == 5)
            {
                return 0;
            }
            field_88 = 3;
            break;

        case objects::molotov_moving_167: // try open door? for garage?
            gDoor_4D4_67BD2C->sub_49D340(this, pObj->field_26_varrok_idx);
            break;

        default:
            break;
    }
    return 0;
}

STUB_FUNC(0x43f130)
char_type Car_BC::HandleCarHitByObject_43F130(Object_2C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x440510)
Fix16 Car_BC::sub_440510()
{
    WIP_IMPLEMENTED;

    if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->h > 64u)
    {
        return k_dword_676984 * sub_43A240();
    }
    else
    {
        return k_dword_6778B4 * sub_43A240();
    }
}

MATCH_FUNC(0x440570)
char_type Car_BC::GetPassengersCount_440570()
{
    if ((field_78_flags & 4) != 0)
    {
        return 0;
    }
    else
    {
        return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->passengers;
    }
}

MATCH_FUNC(0x440590)
void Car_BC::InitCarAIControl_440590()
{
    SetupCarPhysicsAndSpriteBinding_43BCA0();

    field_58_physics->field_92_is_hand_brake_on = 0;

    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->field_238 != 2)
        {
            if (this->field_5C == 0)
            {
                this->field_5C = gCar_78_Pool_677CF8->Allocate();
            }
            this->field_5C->SetCar_453BF0(this);
            this->field_9C = 3;
            sub_43BFE0();
        }
    }
}

MATCH_FUNC(0x4405f0)
void Car_BC::SpawnDriverPed()
{
    Ped* pNextPed = gCar_6C_677930->field_4;
    if (!pNextPed)
    {
        pNextPed = gPedManager_6787BC->sub_470CC0(this);
        gCar_6C_677930->field_4 = pNextPed;
    }
    SetDriver(pNextPed);
    this->field_7C_uni_num = 3;
}

MATCH_FUNC(0x440630)
void Car_BC::AddGangDriver_440630(Gang_144* pGang)
{
    Ped* pNewPed = gPedManager_6787BC->SpawnGangDriver_470BA0(this, pGang);
    SetDriver(pNewPed);
    field_7C_uni_num = 3;
}

MATCH_FUNC(0x440660)
void Car_BC::AttachGangIcon_440660(u8 arrow_colour)
{
    Object_2C* pIcon =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(arrow_colour + 286, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->DispatchCollisionEvent_5A3100(pIcon->field_4, gFix16_6777CC, gFix16_6777CC, word_67791C);
}

MATCH_FUNC(0x4406b0)
void Car_BC::ShowCarName_4406B0(Ped* pPed)
{
    Player* pPlayer = pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_0_bIsUser)
        {
            gHud_2B00_706620->sub_5D5240(GetCarStr_439F80());
        }
    }
}

STUB_FUNC(0x4406e0)
void Car_BC::sub_4406E0(Ped* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4407f0)
void Car_BC::ClearDriver_4407F0()
{
    Player* pPlayer = field_54_driver->field_15C_player;
    if (pPlayer)
    {
        if (field_54_driver->field_240_occupation != 1)
        {
            if (pPlayer->field_0_bIsUser)
            {
                gHud_2B00_706620->field_0.field_0_display_time = 0;
            }
            pPlayer->sub_564C00();
        }

        if (field_58_physics)
        {
            field_58_physics->field_8C_state = 1;
        }
    }
    SetDriver(NULL);
    field_A7_horn = 0;
}

MATCH_FUNC(0x440840)
Sprite* Car_BC::GetSprite_440840()
{
    Trailer* pTrailer = this->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_C_pCarOnTrailer->field_50_car_sprite;
    }
    else
    {
        return this->field_50_car_sprite;
    }
}

MATCH_FUNC(0x440ac0)
void Car_BC::sub_440AC0()
{
    Object_2C* p2C =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::moving_collect_18_114, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->DispatchCollisionEvent_5A3100(p2C->field_4, gFix16_6777CC, dword_6771F0, word_677326);
}

MATCH_FUNC(0x440b10)
void Car_BC::sub_440B10()
{
    Object_2C* p2C =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::small_brown_skid_148, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->DispatchCollisionEvent_5A3100(p2C->field_4, gFix16_6777CC, unk_6772A4, word_67791C);
}

MATCH_FUNC(0x440b60)
void Car_BC::PutMachineGunOnRoof_440B60()
{
    Object_2C* pObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(248, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->DispatchCollisionEvent_5A3100(pObj->field_4, gFix16_6777CC, k_dword_6777FC, word_67791C);
}

MATCH_FUNC(0x440bb0)
void Car_BC::PutTV_Antenna_440BB0()
{
    Object_2C* pNewObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(149, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->DispatchCollisionEvent_5A3100(pNewObj->field_4, gFix16_6777CC, dword_6778A0, GetRadioTowerAngle_442520());
}

STUB_FUNC(0x440c10)
char_type Car_BC::RotateRoofObjectTowardTarget_440C10(Ang16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440d90)
char_type Car_BC::HandleRoofTurretRotation_440D90(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x440f90)
void Car_BC::sub_440F90(char_type instant_bomb)
{
    if (instant_bomb)
    {
        s32 ped_id = gVarrok_7F8_703398->field_0[field_54_driver->field_267_varrok_idx].field_0_ped_id;
        if (ped_id)
        {
            this->field_70_exploder_ped_id = ped_id;
            this->field_90 = 12;
            this->field_94 = 50;
        }
        Car_BC::sub_43D7B0(20);
    }
    else
    {
        Object_2C* pNew2C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::moving_collect_36_132,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    word_67791C);
        Ped* pDriver = this->field_54_driver;
        if (pDriver)
        {
            pNew2C->SetDamageOwner_529080(pDriver->field_267_varrok_idx);
        }
        field_50_car_sprite->DispatchCollisionEvent_5A3100(pNew2C->field_4, gFix16_6777CC, gFix16_6777CC, word_67791C);
    }
}

MATCH_FUNC(0x441030)
void Car_BC::GoToBlockTest_441030(u8 x, u8 y, u8 z, s32 maybe_direction)
{
    if (!field_5C)
    {
        field_5C = gCar_78_Pool_677CF8->Allocate();
    }
    field_5C->SetCar_453BF0(this);
    field_5C->GoToBlock_447CA0(x, y, z, maybe_direction);
}

MATCH_FUNC(0x441080)
void Car_BC::GotoBlock_441080(u8 x, u8 y, u8 z, s32 maybe_direction)
{
    if (!field_5C)
    {
        field_5C = gCar_78_Pool_677CF8->Allocate();
    }
    field_5C->SetCar_453BF0(this);
    field_5C->GoToBlock_447CA0(x, y, z, maybe_direction);
}

STUB_FUNC(0x4410d0)
char_type Car_BC::sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x441330)
Fix16 Car_BC::GetZPos_441330()
{
    if (field_58_physics)
    {
        return field_58_physics->ComputeZPosition_559E90();
    }
    return field_50_car_sprite->field_1C_zpos;
}

MATCH_FUNC(0x441360)
void Car_BC::sub_441360()
{
    if (field_A9)
    {
        field_A9--;
        if (field_A9 == 0)
        {
            sub_4436A0(); // jmp to function chunk
        }
    }
}

STUB_FUNC(0x4436A0)
void Car_BC::sub_4436A0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x441380)
void Car_BC::sub_441380()
{
    if (this->field_A9 == 0)
    {
        if (this->field_9C != 6)
        {
            this->field_9C = 5;
        }
        this->field_A9 = 50;
    }
}

STUB_FUNC(0x4F7940)
EXPORT Ang16* __stdcall sub_4F7940(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x4413b0)
void Car_BC::UpdateTrainCarriagesOnTrack_4413B0(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    WIP_IMPLEMENTED;

    u8 idx = 0;
    Car_BC** pTrainCars = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar_579B40(this);
    Train_58* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCar_57B5C0(this);
    if (pTrain)
    {
        pTrainCars = pTrain->field_10_carriages;
    }

    Fix16 newx;
    Fix16 newy;
    Fix16 newz;

    Fix16 car_angle;
    bool bUnknown = gPublicTransport_181C_6FF1D4->sub_579B90(this, &car_angle);

    for (Car_BC* pTrainCarIter = *pTrainCars; pTrainCarIter; pTrainCarIter = pTrainCars[idx])
    {

        newx = xpos;
        newy = ypos;
        newz = zpos;

        Ang16* v10;
        if (bUnknown)
        {
            s32 v21 = gMap_0x370_6F6268->sub_4E7190(&newx, &newy, &newz, k_dword_6777D4);
            v10 = sub_4F7940(&v21);
        }
        else
        {
            s32 v22 = gMap_0x370_6F6268->sub_4E6660(&newx, &newy, &newz, k_dword_6777D4);
            v10 = sub_4F7940(&v22);
        }

        pTrainCarIter->field_50_car_sprite->set_xyz_lazy_420600(newx, newy, newz);
        pTrainCarIter->field_50_car_sprite->set_ang_lazy_420690(*v10);

        if (pTrainCarIter->field_0_qq.field_0_p18)
        {
            pTrainCarIter->field_0_qq.PoolUpdate_5A6F70(pTrainCarIter->field_50_car_sprite);
            pTrainCarIter->field_0_qq.PropagateMaxZLayer_5A72B0(pTrainCarIter->field_50_car_sprite, 0);
        }

        pTrainCarIter->sub_4426D0();
        pTrainCarIter->sub_441360();

        idx++;
    }
}

MATCH_FUNC(0x441520)
void Car_BC::sub_441520()
{
    CarPhysics_B0* pCarPhysics;
    switch (this->field_9C)
    {
        case 1:
            pCarPhysics = this->field_58_physics;
            if (pCarPhysics)
            {
                if (pCarPhysics->IsAccelerationOrReverseOn_55A180())
                {
                    this->field_9C = 4;
                }
            }
            break;
        case 4:
            sub_43BFE0();
            this->field_9C = 3;
            break;
        case 2:
            sub_43C0C0();
            this->field_9C = 1;
            break;
        case 5:
            sub_43C0C0();
            this->field_9C = 6;
            break;
        case 7:
            field_50_car_sprite->sub_59E300();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4415c0)
void Car_BC::UpdateBrakeLights_4415C0()
{
    CarPhysics_B0* pCarPhysics = this->field_58_physics;
    if (pCarPhysics && pCarPhysics->IsFootBrakeOn_55A150())
    {
        BrakeLightsOn_43BF10();
    }
    else
    {
        BrakeLightsOff_43BF70();
    }
}

STUB_FUNC(0x441600)
u32* Car_BC::sub_441600(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4416d0)
void Car_BC::sub_4416D0(s32 a2)
{
    char bUnknown = 0;
    if (field_A8 > 0)
    {
        field_A8--;
    }

    if (a2 != 0)
    {
        if (!this->field_A8)
        {
            if (!is_train_model() && field_84_car_info_idx != car_model_enum::TANK && (this->field_6C_maybe_id & 7) == 0)
            {
                if (a2 == 2)
                {
                    this->field_AC = 1;
                }
                else if (a2 == 1)
                {
                    this->field_AC = 2;
                }
                bUnknown = 1;
                this->field_A8 = -1;
            }
        }
    }

    if (!this->field_A7_horn && !IsMaxDamage_40F890())
    {
        if (this->field_54_driver->field_15C_player)
        {
            this->field_A7_horn = 45;
        }
        else if (!this->field_A8 || bUnknown)
        {
            if (field_84_car_info_idx == car_model_enum::FIRETRUK || field_84_car_info_idx == car_model_enum::COPCAR ||
                field_84_car_info_idx == car_model_enum::MEDICAR || field_84_car_info_idx == car_model_enum::SWATVAN ||
                field_84_car_info_idx == car_model_enum::TANK || is_FBI_car_411920() || is_train_model() ||
                (this->field_6C_maybe_id & 3) == 0)
            {
                this->field_A7_horn = 45;
                this->field_A8 = -1;
            }
        }
    }
}

MATCH_FUNC(0x4417d0)
void Car_BC::sub_4417D0()
{
    if (field_A7_horn <= 0)
    {
        return;
    }

    if (field_A7_horn != 248)
    {
        field_A7_horn--;
    }
}

MATCH_FUNC(0x4417f0)
void Car_BC::sub_4417F0()
{
    field_A7_horn = 45;
}

MATCH_FUNC(0x441800)
void Car_BC::HandleSpecialInput_441800(char_type bNowSpecialPressed)
{
    if (sub_414F20() && !bNowSpecialPressed && field_A7_horn > 248u)
    {
        SyncEmergencyLightState_43CA80(); // on/off depending on field_A4 flags
    }

    if (!bNowSpecialPressed || IsMaxDamage_40F890())
    {
        field_A7_horn = 0;
        return;
    }

    if (sub_414F20())
    {
        field_A7_horn = -1;
    }
    else
    {
        field_A7_horn = -8;
    }
}

MATCH_FUNC(0x4418a0)
void Car_BC::DoDetachTrailer_4418A0()
{
    if (field_64_pTrailer)
    {
        DetachTrailerAndUpdateDamage_4418B0();
    }
}

MATCH_FUNC(0x4418b0)
void Car_BC::DetachTrailerAndUpdateDamage_4418B0()
{
    sub_43BD40();
    DetachTrailer_442760();
    field_78_flags |= 1;
}

// https://decomp.me/scratch/KU02C
STUB_FUNC(0x4418d0)
void Car_BC::HandleUserInput_4418D0(char_type bForwardGasOn,
                                    char_type bFootBrakeOn,
                                    char_type bLeftOn,
                                    char_type bRightOn,
                                    char_type bHandBrakeOn,
                                    char_type bNowSpecialPressed,
                                    char_type bWasSpecialPressed,
                                    char_type bAttack)
{
    this->field_B8 = 0;

    if (bNowSpecialPressed && (bLeftOn || bRightOn))
    {
        // NB: If left is off then right is on
        if (Car_BC::HandleRoofTurretRotation_440D90(bLeftOn) == 1)
        {
            bLeftOn = 0;
            bRightOn = 0;
            bNowSpecialPressed = 0;
        }
    }

    if (bWasSpecialPressed)
    {
        Car_BC::HandleSpecialInput_441800(bNowSpecialPressed);
    }

    if (bAttack)
    {
        Car_BC::DoDetachTrailer_4418A0();
    }

    if ((this->field_78_flags & 0x1000) != 0)
    {
        bForwardGasOn = 1;
    }

    field_58_physics->HandleUserInputs_55A860(bForwardGasOn, bFootBrakeOn, bLeftOn, bRightOn, bHandBrakeOn);
}

MATCH_FUNC(0x4419e0)
void Car_BC::AllocCarPhysics_4419E0()
{
    if (!field_58_physics)
    {
        field_58_physics = gCarPhysicsPool_6FE3E0->Allocate();
    }
}

MATCH_FUNC(0x441a10)
void Car_BC::DeAllocateCarPhysics_441A10()
{
    if (field_58_physics)
    {
        gCarPhysicsPool_6FE3E0->DeAllocate(field_58_physics);
    }
    field_58_physics = NULL;
}

MATCH_FUNC(0x441a40)
char_type Car_BC::sub_441A40()
{
    u32 i = 0;
    do
    {
        if (field_C_doors[i].field_4_state != 0 && field_C_doors[i].field_4_state != 6)
        {
            return 0;
        }
        i++;
    } while (i < GTA2_COUNTOF(field_C_doors));
    return 1;
}

MATCH_FUNC(0x441a70)
void Car_BC::sub_441A70()
{
    const u8* pRemapCount = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
    u8 i;
    for (i = 0; i < *pRemapCount; i++)
    {
        field_C_doors[i].sub_439E40(i);
        field_C_doors[i].field_8_pObj = 0;
    }

    for (i = *pRemapCount; i < 4; i++)
    {
        field_C_doors[i].field_4_state = 0;
    }
}

MATCH_FUNC(0x441b00)
void Car_BC::sub_441B00()
{
    Car_Door_10* p = field_C_doors;
    for (s32 i = 0; i < 4; i++)
    {
        p->sub_439DA0(&field_8_damaged_areas.m_var);
        p++;
    }
}

MATCH_FUNC(0x441b20)
void Car_BC::sub_441B20()
{
    if (this->field_9C != 3 || this->field_4_passengers_list.field_0_pFirstPed)
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
    else
    {
        this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
}

MATCH_FUNC(0x441b50)
void Car_BC::sub_441B50()
{
    char_type cmp1;
    char_type A5_if_zero;

    if (is_FBI_car_411920())
    {
        cmp1 = 0;
        A5_if_zero = 8;
    }
    else
    {
        cmp1 = 5;
        A5_if_zero = 15;
    }

    if (field_A5 > 0)
    {
        field_A5--;
        if (field_A5 == cmp1)
        {
            sub_43C840();
            sub_43C470();
        }

        if (!field_A5)
        {
            field_A5 = -A5_if_zero;
            sub_43C500();
            sub_43C260();
        }
    }
    else
    {
        field_A5++;
        if (field_A5 == -cmp1)
        {
            sub_43C650();
            sub_43C310();
        }

        if (!field_A5)
        {
            field_A5 = A5_if_zero;
            sub_43C700();
            sub_43C3C0();
        }
    }
}

STUB_FUNC(0x441c00)
void Car_BC::sub_441C00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x441d40)
s16 Car_BC::sub_441D40()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x441e70)
void Car_BC::sub_441E70()
{
    if ((field_A4 & 4) != 0)
    {
        Car_BC::sub_441B50();
    }
    else if ((field_A4 & 8) != 0)
    {
        field_A5--;
        if (field_A5 == 0)
        {
            field_8E--;
            if (field_8E == 0)
            {
                Car_BC::sub_43CBE0();
            }
            else
            {
                sub_425590();
                sub_4213D0();
                field_A5 = 12;
            }
        }
        else
        {
            if (field_A5 == 6)
            {
                sub_425650();
                sub_421430();
            }
        }
    }
    else if ((field_A4 & 0x10) != 0)
    {
        field_A5--;
        if (field_A5 == 0)
        {
            if ((field_A4 & 2) != 0)
            {
                sub_425590();
            }
            else
            {
                Car_BC::sub_447360();
            }
            field_A4 &= ~0x10u;
        }
    }

    if (inline_check_0x2_info_421700())
    {
        Car_BC::sub_441C00();
    }
    if (inline_check_0x20_info_4216C0())
    {
        Car_BC::sub_441B20();
    }
    if (field_84_car_info_idx == car_model_enum::EDSELFBI)
    {
        Car_BC::sub_441D40();
    }
}

MATCH_FUNC(0x442170)
bool Car_BC::sub_442170()
{
    Ped* pDriver = this->field_54_driver;
    if ((!pDriver || !pDriver->field_15C_player) && !sub_43B750())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x442190)
void Car_BC::sub_442190()
{
    if (field_58_physics->ProcessCarPhysicsStateMachine_562FE0())
    {
        if (!this->field_54_driver)
        {
            DeAllocateCarPhysics_43BD00();
        }
    }
}

MATCH_FUNC(0x4421b0)
char_type Car_BC::sub_4421B0()
{
    if (field_A0_car_kind != 8 && field_7C_uni_num != 5 && !field_4_passengers_list.HasPassengerWith_F238_Is_5_471710())
    {
        return 0;
    }

    return 1;
}

MATCH_FUNC(0x442200)
bool Car_BC::sub_442200()
{
    if (field_A0_car_kind == 9)
    {
        return field_74_damage == 32001 ? true : false;
    }

    if (!is_train_model() && !gGame_0x40_67E008->sub_4B9C10(this) && !Car_BC::sub_4421B0())
    {
        if (field_7C_uni_num != 4 && (field_7C_uni_num != 6 || !field_54_driver))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x442310)
void Car_BC::sub_442310()
{
    bool bIntersection = false;
    bool bCheckMax = true;

    if (bSkip_recycling_67D575 && field_84_car_info_idx != car_model_enum::MEDICAR && field_84_car_info_idx != car_model_enum::COPCAR &&
        field_84_car_info_idx != car_model_enum::EDSELFBI)
    {
        return;
    }

    if (is_train_model())
    {
        if (sub_43B730())
        {
            field_76 = 0;
            bCheckMax = false;
        }
    }
    else
    {
        for (u8 i = 0; i < 6; i++)
        {
            if (gGame_0x40_67E008->get_player_4219E0(i))
            {
                if (gGame_0x40_67E008->sub_4B9950(field_50_car_sprite, i, dword_6778D0))
                {
                    bIntersection = true;
                }
            }
        }

        if (bIntersection)
        {
            field_76 = 0;
            bCheckMax = false;
        }
    }

    if (bCheckMax)
    {
        field_76++;
        if (field_76 > 9999)
        {
            field_76 = 9999;
        }
    }

    if (((IsCopCar_421790() || IsFireTruck_4118F0() || IsSwatVan_4217A0() || IsTank_411900() || IsGunJeep_411910() ||
          is_FBI_car_411920()) &&
         field_76 == 300) ||
        field_76 >= 130)
    {
        if (sub_442200() && !sub_4214B0() && field_88 != 5)
        {
            sub_421470();
        }
    }
}

MATCH_FUNC(0x4424c0)
char_type Car_BC::sub_4424C0()
{
    switch (this->field_88)
    {
        case 3:
            this->field_88 = 5;
            return 0;

        case 4:
            this->field_88 = 2;
            return 0;

        case 2:
            if (!sub_442170())
            {
                return 0;
            }
            this->field_88 = 5;
            return 0;

        case 5:
            this->field_88 = 6;
            return 1;

        default:
            return 0;
    }
}

// https://decomp.me/scratch/vhWKK
STUB_FUNC(0x442520)
Ang16 Car_BC::GetRadioTowerAngle_442520()
{
    Fix16_Point xy = gCar_6C_677930->field_4C_tv_van_dir - field_50_car_sprite->get_x_y_443580();
    return Fix16::atan2_fixed_405320(xy.x, xy.y) - field_50_car_sprite->field_0;
}

STUB_FUNC(0x4425d0)
void Car_BC::sub_4425D0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4426d0)
void Car_BC::sub_4426D0()
{
    sub_441B00();
    sub_441520();
    if (field_74_damage != 32001)
    {
        UpdateBrakeLights_4415C0();
        sub_441E70();
    }
    sub_4417D0();
    if (field_84_car_info_idx == 67 && field_9C == 3)
    {
        sub_4425D0();
    }
}

STUB_FUNC(0x442760)
void Car_BC::DetachTrailer_442760()
{
    Trailer* p = field_64_pTrailer;
    gCar_BC_Pool_67792C->Remove(field_64_pTrailer->field_C_pCarOnTrailer);
    field_64_pTrailer->field_C_pCarOnTrailer->field_64_pTrailer = 0;
    field_64_pTrailer->field_8_truck_cab->field_64_pTrailer = 0;
    gTrailerPool_66AC80->field_0_pool.DeAllocate(p);
}

WIP_FUNC(0x4427a0)
void Car_BC::AttachTrailer_4427A0(Car_BC* pToFind)
{
    WIP_IMPLEMENTED;

    field_64_pTrailer = gTrailerPool_66AC80->field_0_pool.Allocate();
    field_64_pTrailer->SetTruckCabAndTrailerCar_407BB0(this, pToFind);

    Car_BC* pLast = 0;
    Car_BC* pIter = gCar_BC_Pool_67792C->field_0_pool.field_4_pPrev;
    if (pIter)
    {
        while (pIter != pToFind)
        {
            pLast = pIter;
            pIter = pIter->mpNext;
            if (!pIter)
            {
                return;
            }
        }
        if (pLast)
        {
            pLast->mpNext = pIter->mpNext;
        }
        else
        {
            gCar_BC_Pool_67792C->field_0_pool.field_4_pPrev = pIter->mpNext;
        }
        pIter->mpNext = 0;
    }
}

STUB_FUNC(0x442810)
s32 Car_BC::sub_442810()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x442d10)
void Car_BC::LightUpdate_442D10()
{
    Sprite_18* p18 = field_0_qq.GetSpriteForModel_5A6A50(165);
    if (p18)
    {
        Sprite* pSprite = p18->field_0;
        Object_2C* o2c = pSprite->As2C_40FEC0();
        if (gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos))
        {
            o2c->Light_527990();
        }
        else
        {
            o2c->UpdateLight_527A30();
        }
    }
}

STUB_FUNC(0x442d70)
char_type Car_BC::sub_442D70()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x443130)
char_type Car_BC::TrailerUpdate_443130()
{
    const s32 state = field_64_pTrailer->sub_408220();
    switch (state)
    {
        case 0:
            return 0;
        case 1:
            gTrailerPool_66AC80->field_0_pool.DeAllocate(field_64_pTrailer);
            return 1;
        case 2:
            Car_BC::DetachTrailer_442760();
            break;
    }
    return 0;
}

MATCH_FUNC(0x443170)
char_type Car_BC::PoolUpdate()
{
    if (!this->field_76)
    {
        gCar_6C_677930->field_55++;
    }

    sub_444020();

    if (this->field_64_pTrailer)
    {
        return TrailerUpdate_443130();
    }

    if (is_train_model())
    {
        return sub_442D70();
    }

    if (this->field_88 != 5)
    {
        gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(this->field_50_car_sprite);
    }

    if (sub_4424C0())
    {
        return 1;
    }

    if (this->field_58_physics)
    {
        Car_78* pAi = this->field_5C;
        if (pAi)
        {
            Ped* pDriver = this->field_54_driver;
            if (pDriver)
            {
                if (pDriver->sub_420B70() == 2 && this->field_84_car_info_idx == car_model_enum::TRAINCAB || pDriver->sub_420B70() != 2)
                {
                    pAi->sub_453BB0();
                }
            }
            field_5C->field_68 = 0;
            field_5C->field_24_flags &= ~0x1000u;
        }
        sub_442190();
    }

    if ((this->field_78_flags & 0x2000) != 0)
    {
        stru_67737C.PropagateMaxZLayer_5A72B0(this->field_50_car_sprite, 1);
        stru_67737C.ClearList_5A6E10();
    }

    if (this->field_0_qq.field_0_p18)
    {
        this->field_0_qq.PoolUpdate_5A6F70(this->field_50_car_sprite);
        this->field_0_qq.PropagateMaxZLayer_5A72B0(this->field_50_car_sprite, 0);
    }

    if (this->field_58_physics)
    {
        ManageDrowning_43E560();
    }

    if (!this->field_64_pTrailer && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & 8) == 8)
    {
        sub_442810();
    }

    sub_4426D0();
    sub_442310();
    sub_441360();

    if (this->field_88 != 5)
    {
        if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & 0x20) == 0x20)
        {
            if (this->field_54_driver)
            {
                if (this->field_9C == 3 && !this->field_4_passengers_list.field_0_pFirstPed && this->field_8C < 3u)
                {
                    gTaxi_4_704130->PushTaxi_457BA0(this);
                }
            }
        }
        gPurpleDoom_1_679208->AddToRegionBuckets_477B20(this->field_50_car_sprite);
    }
    return 0;
}

MATCH_FUNC(0x443330)
void Car_BC::sub_443330()
{
    Car_BC::sub_4426D0();
    if (field_0_qq.field_0_p18)
    {
        field_0_qq.PoolUpdate_5A6F70(field_50_car_sprite);
        field_0_qq.PropagateMaxZLayer_5A72B0(field_50_car_sprite, 0);
    }
    Car_BC::sub_441360();
}

STUB_FUNC(0x443360)
bool Car_BC::sub_443360(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4435a0)
void Car_BC::sub_4435A0()
{
    sub_441A70();
}

MATCH_FUNC(0x4435b0)
s32 Car_BC::GetCrashSoundCategory_4435B0()
{
    if ((field_78_flags & 0x10) != 0)
    {
        return 14;
    }

    if ((field_78_flags & 0x40) != 0)
    {
        return 20;
    }

    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 1) != 1)
    {
        return 17;
    }
    return 22;
}

MATCH_FUNC(0x4435f0)
void Car_BC::sub_4435F0()
{
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x10) == 0x10)
    {
        this->field_50_car_sprite->field_28_num = 16;
    }
    else
    {
        if ((field_78_flags & 0x10) != 0)
        {
            this->field_50_car_sprite->field_28_num = 13;
        }
        else if ((field_78_flags & 0x40) != 0)
        {
            this->field_50_car_sprite->field_28_num = 19;
        }
        else if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 1) == 1)
        {
            this->field_50_car_sprite->field_28_num = 21;
        }
        else
        {
            this->field_50_car_sprite->field_28_num = 15;
        }
    }
}

STUB_FUNC(0x443710)
void Car_BC::sub_443710(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x443A50)
s32 __stdcall Car_BC::get_car_weapon_cost_443A50(s32 weapon_kind)
{
    if (gCar_6C_677930->field_69_do_free_shopping)
    {
        return 0;
    }

    switch (weapon_kind)
    {
        case weapon_type::car_bomb:
            return 5000;
        case weapon_type::oil_stain:
            return 10000;
        case weapon_type::car_smg:
            return 25000;
        case weapon_type::car_mines:
            return 50000;
        default:
            return 0;
    }
}

MATCH_FUNC(0x4438C0)
void Car_BC::BuyCarWeapon_4438C0(s32 weapon_kind)
{
    u8 ammo_capacity;
    Ped* pDriver = this->field_54_driver;
    Player* pPlayer = pDriver->field_15C_player;
    Weapon_30* pWeapon = pPlayer->field_718_weapons[weapon_kind];
    if (pWeapon && pWeapon->is_max_capacity_5DCEA0())
    {
        // Ammo full
        if (pPlayer->field_0_bIsUser)
        {
            gHud_2B00_706620->field_DC.sub_5D4400(1, "arig");
        }
    }
    else
    {
        s32 car_weapon_cost = Car_BC::get_car_weapon_cost_443A50(weapon_kind);
        if (bStartNetworkGame_7081F0)
        {
            // Cheaper in multiplayer
            car_weapon_cost /= 10;
        }

        if (car_weapon_cost <= pPlayer->field_2D4_scores.GetScore_592370())
        {
            if (pPlayer->field_0_bIsUser)
            {
                gHud_2B00_706620->field_DC.sub_5D3F10(1, "bdone", car_weapon_cost);
            }

            pPlayer->field_2D4_scores.AddCash_592620(-car_weapon_cost);
            if (gWeapon_8_707018->get_max_ammo_capacity_5E3E70(weapon_kind) < 10u)
            {
                ammo_capacity = gWeapon_8_707018->get_max_ammo_capacity_5E3E70(weapon_kind);
            }
            else
            {
                ammo_capacity = 10;
            }
            gWeapon_8_707018->allocate_5E3D50(weapon_kind, ammo_capacity, this);
            switch (weapon_kind)
            {
                case weapon_type::car_bomb:
                    this->field_B4_weapon_kind = 4;
                    break;
                case weapon_type::car_mines:
                    this->field_B4_weapon_kind = 5;
                    break;
                case weapon_type::oil_stain:
                    this->field_B4_weapon_kind = 6;
                    break;
                case weapon_type::car_smg:
                    this->field_B4_weapon_kind = 7;
                    break;
                default:
                    FatalError_4A38C0(0x431, "C:\\Splitting\\Gta2\\Source\\car.cpp", 6107, 0);
            }
        }
        else
        {
            // Can't afford weapon
            Car_BC::sub_443AB0(pPlayer, car_weapon_cost);
            this->field_B4_weapon_kind = 8;
        }
    }
}

MATCH_FUNC(0x443AB0)
void __stdcall Car_BC::sub_443AB0(Player* pPlayer, s32 weapon_cost)
{
    if (pPlayer->field_0_bIsUser)
    {
        gHud_2B00_706620->field_DC.sub_5D3F10(1, "nspraya", weapon_cost);
    }
}

MATCH_FUNC(0x443ae0)
void Car_BC::ResprayOrChangePlates(u8 remap)
{
    Player* pPlayer = this->field_54_driver->field_15C_player;
    const s32 cost = gCar_6C_677930->field_69_do_free_shopping != 0 ? 0 : 5000;
    if (cost <= pPlayer->field_2D4_scores.GetScore_592370())
    {
        if (pPlayer->field_0_bIsUser)
        {
            if (remap == 0xFD) // clean plates only
            {
                gHud_2B00_706620->field_DC.sub_5D3F10(1, "cdone", cost);
            }
            else
            {
                gHud_2B00_706620->field_DC.sub_5D3F10(1, "sdone", cost);
            }
        }

        if (remap != 0xFD) // respray
        {
            SetCarRemap(remap);
            field_0_qq.sub_5A7110();
            this->field_B4_weapon_kind = 1;
        }
        else
        {
            this->field_B4_weapon_kind = 2;
        }

        pPlayer->field_2D4_scores.AddCash_592620(-cost);
        this->field_54_driver->field_20A_wanted_points = 0;
        RemoveAllDamage();
    }
    else
    {
        sub_443AB0(pPlayer, cost);
        this->field_B4_weapon_kind = 8;
    }
}

MATCH_FUNC(0x443bd0)
void Car_BC::ResprayOrCleanPlates(u8 remap)
{
    if (AllowResprayOrPlates())
    {
        if (IsNotCurrentRemapOfCarAndTrailerCar(remap))
        {
            ResprayOrChangePlates(remap);
        }
        else
        {
            ResprayOrChangePlates(0xFD); // change plates
        }
    }
    else if (field_54_driver->field_15C_player->field_0_bIsUser)
    {
        // I ain't touching that get outta here!
        gHud_2B00_706620->field_DC.sub_5D4400(1, "nespray");
    }
}

MATCH_FUNC(0x443c40)
void Car_BC::HandleShops_443C40(Object_2C* pObj)
{
    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        Player* pPlayer = pDriver->field_15C_player;
        if (pPlayer)
        {
            if (pDriver->field_240_occupation != 1 && pObj->field_18_model == objects::moving_collect_34_130)
            {
                const u8 idx = pObj->field_26_varrok_idx;
                if (idx >= objects::shop_car_smg_250 && (idx <= objects::shop_car_mines_252 || idx == objects::object_254))
                {
                    Car_BC::BuyCarWeapon_4438C0(pPlayer->ObjectTypeToWeaponType_443CB0(idx));
                }
                else
                {
                    Car_BC::ResprayOrCleanPlates(idx);
                }
            }
        }
    }
}

MATCH_FUNC(0x443d00)
void Car_BC::sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_50_car_sprite);
    Sprite* pCarSprite = field_50_car_sprite;
    if (pCarSprite->field_14_xy.x != xpos || pCarSprite->field_14_xy.y != ypos || pCarSprite->field_1C_zpos != zpos)
    {
        pCarSprite->field_14_xy.x = xpos;
        pCarSprite->field_14_xy.y = ypos;
        pCarSprite->field_1C_zpos = zpos;
        pCarSprite->ResetZCollisionAndDebugBoxes_59E7B0();
    }
    CarPhysics_B0* field_58_uni = field_58_physics;
    if (field_58_uni)
    {
        field_58_uni->SetSprite_563560(field_50_car_sprite);
    }
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_50_car_sprite);
}

MATCH_FUNC(0x443d70)
void Car_BC::IncrementCarStats_443D70(s32 a2)
{
    IncrementAllocatedCarType_443DA0(a2);

    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_8_truck_cab == this)
        {
            field_64_pTrailer->field_C_pCarOnTrailer->IncrementAllocatedCarType_443DA0(a2);
        }
    }
}

MATCH_FUNC(0x443da0)
void Car_BC::IncrementAllocatedCarType_443DA0(s32 a2)
{
    this->field_A0_car_kind = a2;
    switch (a2)
    {
        case 1:
            ++gCar_6C_677930->field_28_recycled_cars;
            break;
        case 2:
            ++gCar_6C_677930->field_40_proto_recycled_cars;
            break;
        case 5:
            ++gCar_6C_677930->field_30;
            break;
        case 10:
            ++gCar_6C_677930->field_48;
            break;
        case 4:
            ++gCar_6C_677930->field_2C;
            break;
        case 6:
            ++gCar_6C_677930->field_34_unit_cars;
            break;
        case 7:
            ++gCar_6C_677930->field_38;
            break;
        case 8:
            ++gCar_6C_677930->field_3C_mission_cars;
            break;
        case 9:
            ++gCar_6C_677930->field_44;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x443e50)
void Car_BC::sub_443E50()
{
    if (field_A0_car_kind == 1)
    {
        gCar_6C_677930->field_28_recycled_cars--;
        gCar_6C_677930->field_40_proto_recycled_cars++;
        field_A0_car_kind = 2;
    }
}

MATCH_FUNC(0x443e80)
void Car_BC::sub_443E80()
{
    if (field_A0_car_kind == 2)
    {
        gCar_6C_677930->field_28_recycled_cars++;
        gCar_6C_677930->field_40_proto_recycled_cars--;
        field_A0_car_kind = 1;
    }
}

MATCH_FUNC(0x443eb0)
void Car_BC::sub_443EB0(s32 a2)
{
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);
    IncrementCarStats_443D70(a2);
}

MATCH_FUNC(0x443ee0)
void Car_BC::sub_443EE0(s32 a2)
{
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);
    IncrementAllocatedCarType_443DA0(a2);
}

MATCH_FUNC(0x443f30)
void Car_BC::sub_443F30(s32 object_type, s32 argb, s32 a4, s32 a5)
{
    Object_2C* pObj = gObject_5C_6F8F84->NewLight_529AB0(object_type, 0, 0, 0, argb, dword_6772AC, 200);
    pObj->Light_527990();
    field_50_car_sprite->DispatchCollisionEvent_5A3100(pObj->field_4,
                                                       Fix16(a4 * dword_677888, 0),
                                                       Fix16(a5 * dword_677888, 0),
                                                       word_67791C);
}

MATCH_FUNC(0x444020)
void Car_BC::sub_444020()
{
    if (field_94 > 0)
    {
        field_94--;
        if (field_94 == 0)
        {
            field_70_exploder_ped_id = 0;
            field_90 = 0;
        }
    }
}

MATCH_FUNC(0x444090)
u32 Car_BC::GetEffectiveDriverPedId_444090()
{
    Car_BC* a1 = this;
    while (!a1->field_54_driver)
    {
        if (!a1->field_64_pTrailer || a1->field_64_pTrailer->field_C_pCarOnTrailer != a1)
        {
            return a1->field_70_exploder_ped_id;
        }
        a1 = a1->field_64_pTrailer->field_8_truck_cab;
    }

    Ped* pDriver = a1->field_54_driver;
    if (pDriver->field_240_occupation == 4)
    {
        return a1->field_70_exploder_ped_id;
    }

    if (pDriver->field_200_id > 12u)
    {
        if (a1->field_70_exploder_ped_id)
        {
            return a1->field_70_exploder_ped_id;
        }
    }
    return pDriver->field_200_id;
}

/*
OBS: it is not a switch case :)
*/
MATCH_FUNC(0x4441b0)
void Car_BC::sub_4441B0()
{
    // car sirens?
    s32 idx = this->field_84_car_info_idx;
    if (idx == car_model_enum::MEDICAR)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, 16);
        Car_BC::sub_443F30(171, 0xFF2010, -10, 16);
        Car_BC::sub_443F30(172, 0xFF2010, 10, -32);
        Car_BC::sub_443F30(173, 0xFF2010, -10, -32);
    }
    else if (idx == car_model_enum::COPCAR)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, -16);
        Car_BC::sub_443F30(171, 255, -10, -16);
    }
    else if (idx == car_model_enum::SWATVAN)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, 32);
        Car_BC::sub_443F30(171, 255, -10, 32);
        Car_BC::sub_443F30(172, 0xFF2010, 10, -32);
        Car_BC::sub_443F30(173, 255, -10, -32);
    }
    else if (idx == car_model_enum::FIRETRUK)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 16, 48);
        Car_BC::sub_443F30(171, 0xFF2010, -16, 48);
        Car_BC::sub_443F30(172, 0xFF2010, 16, -16);
        Car_BC::sub_443F30(173, 0xFF2010, -16, -26);
    }
    else if (idx == car_model_enum::EDSELFBI)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 0, 16);
    }
}

MATCH_FUNC(0x444490)
void Car_BC::sub_444490()
{
    this->field_6C_maybe_id = gCar_6C_677930->field_14++;
    this->field_74_damage = 0;
    this->field_8C = 0;
    this->field_8_damaged_areas.ClearAllBits_420D90();
    this->field_4_passengers_list.ClearList_420E90();
    this->field_54_driver = 0;
    this->field_98 = 3;
    this->field_58_physics = 0;
    this->field_A4 = 0;
    this->field_A5 = 0;
    this->field_76 = 0;
    this->field_7C_uni_num = 3;
    this->field_50_car_sprite = 0;
    this->field_9C = 1;
    this->field_A6 = 0;
    this->field_80 = 0;
    this->field_78_flags = 0;
    //clear();

    this->field_0_qq.sub_5A7010();

    this->field_A7_horn = 0;
    IncrementCarStats_443D70(0);
    this->field_8D = 0;
    this->field_60 = 0;
    this->field_70_exploder_ped_id = 0;
    this->field_90 = 0;
    this->field_94 = 0;
    this->field_95 = 0;
    //v3 = dword_6777D0;
    this->field_68 = dword_6777D0;

    this->field_8E = 0;
    this->field_A8 = 0;
    this->field_A9 = 0;
    this->field_B4_weapon_kind = 0;
    this->field_B8 = 0;
    this->field_B0 = 0;
}

MATCH_FUNC(0x4446e0)
void Car_BC::sub_4446E0()
{
    if (field_5C)
    {
        if (field_5C->field_28_junc_idx > 0)
        {
            gRouteFinder_6FFDC8->CancelRoute_589930(field_5C->field_28_junc_idx);
        }

        gCar_78_Pool_677CF8->DeAllocate(field_5C);
        field_5C = 0;
    }
}

MATCH_FUNC(0x4447d0)
void Car_BC::PoolDeallocate()
{
    this->field_0_qq.sub_5A7010();

    DeAllocateCarPhysics_441A10();

    if (field_50_car_sprite)
    {
        gSprite_Pool_703818->remove(field_50_car_sprite);
        field_50_car_sprite = 0;
    }

    sub_4446E0();

    if (field_60)
    {
        gHamburger_500_678E30->Cancel_474CC0(field_60);
        field_60 = 0;
    }

    sub_43DB80();

    gWeapon_8_707018->alloc_car_weapon_5E3DF0(this);
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);

    field_6C_maybe_id = 0;
}

MATCH_FUNC(0x444860)
Car_BC::Car_BC()
{
    field_54_driver = 0;
    field_74_damage = 0;
    field_8C = 0;
    field_98 = 0;
    field_9C = 0;
    field_7C_uni_num = 0;
    field_76 = 0;
    field_A4 = 0;
    field_A5 = 0;
    field_A6 = 0;
    mpNext = 0;
    field_84_car_info_idx = car_model_enum::none;
    field_50_car_sprite = 0;
    field_58_physics = 0;
    field_5C = 0;
    field_88 = 0;
    field_6C_maybe_id = 0xFFFF;
    field_64_pTrailer = 0;
    field_78_flags = 0;
    this->field_0_qq.sub_5A7010();
    field_A7_horn = 0;
    field_80 = 0;
    field_A0_car_kind = 0;
    field_8D = 0;
    field_70_exploder_ped_id = 0;
    field_90 = 0;
    field_94 = 0;
    field_95 = 0;
    field_68.mValue = 0x4000;
    field_8E = 0;
}

MATCH_FUNC(0x444960)
Car_BC::~Car_BC()
{
    mpNext = 0;
    field_50_car_sprite = 0;
    field_5C = 0;
}

MATCH_FUNC(0x447360)
void Car_BC::sub_447360()
{
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 2) == 2)
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
    }
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 4) == 4)
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
}

MATCH_FUNC(0x564300)
bool Car_BC::sub_564300()
{
    if (field_54_driver)
    {
        bool result = field_54_driver->IsField238_45EDE0(2) == 0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x407b90)
Car_BC* Trailer::sub_407B90(Car_BC* a2)
{
    if (a2 == field_8_truck_cab)
    {
        return field_C_pCarOnTrailer;
    }
    return field_8_truck_cab;
}

MATCH_FUNC(0x407bb0)
void Trailer::SetTruckCabAndTrailerCar_407BB0(Car_BC* pTruckCab, Car_BC* pTrailerCar)
{
    this->field_8_truck_cab = pTruckCab;
    this->field_C_pCarOnTrailer = pTrailerCar;

    pTruckCab->field_64_pTrailer = this;
    this->field_C_pCarOnTrailer->field_64_pTrailer = this;
    this->field_0 = 0;
}

STUB_FUNC(0x407bd0)
s32* Trailer::sub_407BD0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x407ce0)
s32* Trailer::sub_407CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x408140)
char_type Trailer::sub_408140()
{
    if (!field_8_truck_cab->field_58_physics && !field_C_pCarOnTrailer->field_58_physics)
    {
        return 0;
    }

    SetupCarPhysicsAndSpriteBinding_408190();

    if (!field_8_truck_cab->field_58_physics->ProcessCarPhysicsStateMachine_562FE0() || field_8_truck_cab->field_54_driver)
    {
        return field_8_truck_cab->ManageDrowning_43E560();
    }
    DeAllocateCarPhysics_4081B0();
    return 0;
}

MATCH_FUNC(0x408190)
void Trailer::SetupCarPhysicsAndSpriteBinding_408190()
{
    field_8_truck_cab->SetupCarPhysicsAndSpriteBinding_43BC30();
    field_C_pCarOnTrailer->SetupCarPhysicsAndSpriteBinding_43BC30();
}

MATCH_FUNC(0x4081b0)
void Trailer::DeAllocateCarPhysics_4081B0()
{
    field_8_truck_cab->DeAllocateCarPhysics_441A10();
    field_C_pCarOnTrailer->DeAllocateCarPhysics_441A10();
}

MATCH_FUNC(0x4081d0)
char_type Trailer::sub_4081D0()
{
    if (field_8_truck_cab->field_74_damage == 32001)
    {
        if (field_C_pCarOnTrailer->field_74_damage != 32001)
        {
            field_C_pCarOnTrailer->field_74_damage = 32000;
            field_C_pCarOnTrailer->ExplodeCar_Unknown_43D840(18);
        }
        return 1;
    }
    else if (field_C_pCarOnTrailer->field_74_damage == 32001)
    {
        field_8_truck_cab->field_74_damage = 32000;
        field_8_truck_cab->ExplodeCar_Unknown_43D840(18);
        return 1;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x408220)
s32 Trailer::sub_408220()
{
    if (field_8_truck_cab->field_88 != 5)
    {
        gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_8_truck_cab->field_50_car_sprite);
        gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_C_pCarOnTrailer->field_50_car_sprite);
    }

    const char_type v3 = field_8_truck_cab->sub_4424C0();
    field_C_pCarOnTrailer->field_88 = field_8_truck_cab->field_88;
    if (v3)
    {
        gCar_BC_Pool_67792C->Remove(field_C_pCarOnTrailer);
        return 1;
    }
    else
    {
        const char_type bUnknown = sub_408140();
        if (field_8_truck_cab->field_50_car_sprite->field_1C_zpos != k_dword_66AB38)
        {
            if (field_C_pCarOnTrailer->field_50_car_sprite->field_1C_zpos != k_dword_66AB38)
            {
                field_8_truck_cab->field_50_car_sprite->ResolveZOrder_5A1B30(field_C_pCarOnTrailer->field_50_car_sprite);
            }
        }

        field_8_truck_cab->field_50_car_sprite->field_28_num = 15;

        field_8_truck_cab->sub_4426D0();
        field_C_pCarOnTrailer->sub_4426D0();

        if (field_8_truck_cab->field_88 != 5)
        {
            gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_8_truck_cab->field_50_car_sprite);
            gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_C_pCarOnTrailer->field_50_car_sprite);
        }

        if (field_8_truck_cab->field_0_qq.field_0_p18)
        {
            field_8_truck_cab->field_0_qq.PoolUpdate_5A6F70(field_8_truck_cab->field_50_car_sprite);
            field_8_truck_cab->field_0_qq.PropagateMaxZLayer_5A72B0(field_8_truck_cab->field_50_car_sprite, 0);
        }

        if (field_C_pCarOnTrailer->field_0_qq.field_0_p18)
        {
            field_C_pCarOnTrailer->field_0_qq.PoolUpdate_5A6F70(field_C_pCarOnTrailer->field_50_car_sprite);
            field_C_pCarOnTrailer->field_0_qq.PropagateMaxZLayer_5A72B0(field_C_pCarOnTrailer->field_50_car_sprite, 0);
        }

        if (sub_4081D0() || bUnknown)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
}

MATCH_FUNC(0x5822E0)
void sub_5822E0()
{
    DAT_006FF7E8 = DAT_006FF774;
    DAT_006FF570 = DAT_006FF744;
    DAT_006FF8C4 = 0;
    DAT_006FF8C5 = 0;
}

MATCH_FUNC(0x582310)
Car_14::Car_14()
{
    field_9 = 0;
    field_A = 0;
    field_8 = 0;
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
    field_4_rect = new Fix16_Rect();
    sub_5822E0();
}

MATCH_FUNC(0x582340)
Car_14::~Car_14()
{
    delete field_4_rect;
    field_4_rect = NULL;
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
}

MATCH_FUNC(0x582360)
char Car_14::sub_582360(int param_1, Fix16 param_2, Fix16 param_3)
{
    switch (param_1)
    {
        case 1:
        case 2:
            if (field_8 == 0)
            {
                if (param_2 < (field_0_cam->field_78_boundaries_non_neg.field_4_right - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_2 > (field_0_cam->field_78_boundaries_non_neg.field_0_left + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
        case 3:
        case 4:
            if (field_8 == 0)
            {
                if (param_3 < (field_0_cam->field_78_boundaries_non_neg.field_C_bottom - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_3 > (field_0_cam->field_78_boundaries_non_neg.field_8_top + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
    }
    return 0;
}

STUB_FUNC(0x5832C0)
void Car_14::MakeTrafficForCurrCamera_5832C0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x583670)
void Car_14::GenerateTraffic_583670()
{
    if (!bSkip_recycling_67D575)
    {
        field_0_cam = gGame_0x40_67E008->IteratePlayerCamera_4B9BC0();
        field_C_player = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
        if (field_C_player)
        {
            field_10_zone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(field_C_player->field_2C4_player_ped->get_cam_x().ToInt(),
                                                                           field_C_player->field_2C4_player_ped->field_1AC_cam.y.ToInt());
        }

        for (; field_0_cam;)
        {
            if (field_0_cam->has_camera_car_or_ped_433E90())
            {
                Car_14::MakeTrafficForCurrCamera_5832C0();
            }
            field_0_cam = gGame_0x40_67E008->sub_4B9C50();

            Ped* pPed;
            if (field_0_cam == &field_C_player->field_208_aux_game_camera)
            {
                pPed = field_C_player->field_2C8_unkq;
            }
            else
            {
                field_C_player = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
                if (field_C_player)
                {
                    pPed = field_C_player->field_2C4_player_ped;
                }
                else
                {
                    continue;
                }
            }
            if (pPed)
            {
                field_10_zone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(pPed->get_cam_x().ToInt(), pPed->field_1AC_cam.y.ToInt());
            }
        }
    }
}
