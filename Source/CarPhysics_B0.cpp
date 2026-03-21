#include "CarPhysics_B0.hpp"
#include "CarAI_78.hpp"
#include "CarInfo_808.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0, 0x6FE3E0);
DEFINE_GLOBAL(CarInfo_2C*, gCarInfo_2C_6FE0E4, 0x6FE0E4);
DEFINE_GLOBAL(ModelPhysics_48*, gCarInfo_48_6FE258, 0x6FE258);
DEFINE_GLOBAL_INIT(Ang16, DAT_0066AC08, Ang16(0), 0x66AC08);
DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FE20C, Fix16(0), 0x6FE20C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE290, kFP16Zero_6FE20C, 0x6FE290);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE220, Fix16(5), 0x6FE220);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDEFC, dword_6FE220, 0x6FDEFC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDF88, k_dword_6FDEFC, 0x6FDF88);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE200, kFP16Zero_6FE20C, 0x6FE200);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE214, Fix16(2), 0x6FE214);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDFEC, Fix16(0x2000, 0), 0x6FDFEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1B0, dword_6FE214 + k_dword_6FDFEC, 0x6FE1B0);
DEFINE_GLOBAL(Fix16, dword_6FE348, 0x6FE348);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFB0, dword_6FE214, 0x6FDFB0);
DEFINE_GLOBAL(Fix16, dword_6FE0D8, 0x6FE0D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD4, Fix16(0x1000, 0), 0x6FDFD4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE364, dword_6FDFD4, 0x6FE364);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE3A0, dword_6FDFD4, 0x6FE3A0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2AC, dword_6FDFD4, 0x6FE2AC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE210, Fix16(1), 0x6FE210);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1C0, k_dword_6FE210, 0x6FE1C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFE4, Fix16(0x1333, 0), 0x6FDFE4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0A8, dword_6FDFE4, 0x6FE0A8);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE1AC, Fix16(0x100, 0), 0x6FE1AC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2E0, dword_6FE1AC, 0x6FE2E0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE234, Fix16(8), 0x6FE234);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDF34, dword_6FE234 - dword_6FE2E0, 0x6FDF34);

DEFINE_GLOBAL(Fix16_Point, g_cm1_6FDF10, 0x6FDF10);
DEFINE_GLOBAL(Fix16, g_cp3_6FDF08, 0x6FDF08);
DEFINE_GLOBAL_INIT(Ang16, g_theta_6FE344, Ang16(0), 0x6FE344);
DEFINE_GLOBAL(Fix16_Point, g_cp1_6FDF00, 0x6FDF00);
DEFINE_GLOBAL(Fix16, g_f70_6FDFE0, 0x6FDFE0);
DEFINE_GLOBAL(Fix16, g_ZPos_6FE0AC, 0x6FE0AC);

DEFINE_GLOBAL(Fix16_Point, g_trailer_cm1_6FE068, 0x6FE068);
DEFINE_GLOBAL(Fix16, gTrailer_cp3_6FE1B4, 0x6FE1B4);
DEFINE_GLOBAL_INIT(Ang16, gTrailer_theta_6FE018, Ang16(0), 0x6FE018);
DEFINE_GLOBAL(Fix16_Point, gTrailer_cp1_6FE3A8, 0x6FE3A8);
DEFINE_GLOBAL(Fix16, gTrailer_f70_6FE194, 0x6FE194);
DEFINE_GLOBAL(Fix16, gTrailer_ZPos_6FE354, 0x6FE354);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE1D8, Fix16(0x28F, 0), 0x6FE1D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDF3C, dword_6FE1D8, 0x6FDF3C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDF7C, Fix16(1), 0x6FDF7C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2EC, Fix16(0x100000, 0), 0x6FE2EC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE21C, Fix16(0x10000, 0), 0x6FE21C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE07C, k_dword_6FE210 / (dword_6FE2EC * dword_6FE21C), 0x6FE07C);

DEFINE_GLOBAL_INIT(Fix16, FastCarMinVelocity_6FE1CC, Fix16(0x1EB, 0), 0x6FE1CC);
DEFINE_GLOBAL(Fix16, dword_6FE198, 0x6FE198);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE370, Fix16(0x800, 0), 0x6FE370);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE218, Fix16(3), 0x6FE218);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE1B8, dword_6FE218, 0x6FE1B8);

DEFINE_GLOBAL(Fix16_Point, stru_6FDF50, 0x6FDF50);
DEFINE_GLOBAL(Fix16, dword_6FE0B0, 0x6FE0B0);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDFF0, Fix16(0x2666, 0), 0x6FDFF0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD8, Fix16(0xCCC, 0), 0x6FDFD8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1D4, Fix16(0x333, 0), 0x6FE1D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFF8, Fix16(0x3333, 0), 0x6FDFF8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE228, dword_6FDFF0, 0x6FE228);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE374, dword_6FDFD8 + dword_6FE1D4, 0x6FE374);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE104, dword_6FDFF8, 0x6FE104);

DEFINE_GLOBAL_INIT(Ang16, word_6FE00C, Ang16(360), 0x6FE00C);
DEFINE_GLOBAL_INIT(Ang16, word_6FE154, Ang16(1080), 0x6FE154);
DEFINE_GLOBAL_INIT(Ang16, k_word_6FE12A, Ang16(720), 0x6FE12A);
DEFINE_GLOBAL(Fix16_Point, stru_6FE1F0, 0x6FE1F0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3C4, Fix16(0x11C, 0), 0x6FE3C4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDFA4, dword_6FE3C4, 0x6FDFA4);
DEFINE_GLOBAL_INIT(Fix16, stru_6FDF80, dword_6FE2E0, 0x6FDF80);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE33C, Fix16(0), 0x6FE33C);
DEFINE_GLOBAL(u8, byte_6FDFC4, 0x6FDFC4);
DEFINE_GLOBAL(u8, byte_6FDFCC, 0x6FDFCC);
DEFINE_GLOBAL(Fix16_Point, stru_6FE1A0, 0x6FE1A0);

DEFINE_GLOBAL(Fix16_Point, gSaved_cm1_6FE3C8, 0x6FE3C8);
DEFINE_GLOBAL(Fix16, gSaved_cp3_6FDF84, 0x6FDF84);
DEFINE_GLOBAL(Ang16, gSaved_theta_6FE158, 0x6FE158);
DEFINE_GLOBAL(Fix16_Point, gSaved_cp1_6FE090, 0x6FE090);
DEFINE_GLOBAL(Fix16, gSaved_f70_6FE268, 0x6FE268);
DEFINE_GLOBAL(Fix16, gSaved_zpos_6FE32C, 0x6FE32C);

DEFINE_GLOBAL(Fix16_Point, gSaved_trailer_cm1_6FE160, 0x6FE160);
DEFINE_GLOBAL(Fix16, gSaved_trailed_cp3_6FDF8C, 0x6FDF8C);
DEFINE_GLOBAL(Ang16, gSaved_trailer_theta_6FE310, 0x6FE310);
DEFINE_GLOBAL(Fix16_Point, gSaved_trailer_cp1_6FDF40, 0x6FDF40);
DEFINE_GLOBAL(Fix16, gSaved_trailer_f70_6FE0E0, 0x6FE0E0);
DEFINE_GLOBAL(Fix16, gSaved_trailer_zpos_6FE394, 0x6FE394);

DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE314, Fix16(0x18F60, 0), 0x6FE314);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3D0, k_dword_6FE210, 0x6FE3D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE148, Fix16(0x8E5, 0), 0x6FE148);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3D4, dword_6FE148, 0x6FE3D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE320, dword_6FDFF8, 0x6FE320);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0A0, Fix16(0x80, 0), 0x6FE0A0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFDC, dword_6FE0A0, 0x6FDFDC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE334, k_dword_6FE210 - dword_6FE1D4, 0x6FE334);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2F0, Fix16(0x51, 0), 0x6FE2F0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1C4, Fix16(0xA3, 0), 0x6FE1C4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE330, k_dword_6FE210 - dword_6FE2F0 - dword_6FE1C4, 0x6FE330);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE240, k_dword_6FE210 - dword_6FE2F0 - dword_6FE1C4, 0x6FE240);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1E4, Fix16(0x3D7, 0), 0x6FE1E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDF18, k_dword_6FE210 - dword_6FE1E4, 0x6FDF18);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD0, Fix16(0x666, 0), 0x6FDFD0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE100, k_dword_6FE210 - dword_6FDFD0, 0x6FE100);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0FC, k_dword_6FE210 - FastCarMinVelocity_6FE1CC, 0x6FE0FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFBC, k_dword_6FE210 - FastCarMinVelocity_6FE1CC, 0x6FDFBC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1E0, Fix16(0x7AE, 0), 0x6FE1E0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE318, k_dword_6FE210 - dword_6FE1E0, 0x6FE318);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3B4, dword_6FE1D4, 0x6FE3B4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE15C, dword_6FE1C4, 0x6FE15C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2F4, dword_6FDFD0, 0x6FE2F4);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE0C0, Fix16(0x2000, 0), 0x6FE0C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE064, Fix16(0x1FE8, 0), 0x6FE064);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE350, Fix16(0x824, 0), 0x6FE350);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE10C, Fix16(0x63D8, 0), 0x6FE10C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE134, dword_6FE3C4 * 25, 0x6FE134);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE278, Fix16(0x12B88, 0), 0x6FE278);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE260, Fix16(0xC7B0, 0), 0x6FE260);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE2F8, dword_6FE0C0, 0x6FE2F8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE070, k_dword_6FE210, 0x6FE070);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE3DC, k_dword_6FE210, 0x6FE3DC);

DEFINE_GLOBAL_INIT(Ang16, word_6FE3B8, Ang16(4), 0x6FE3B8); // Only exists so that sub_401CB0 can be called

DEFINE_GLOBAL_INIT(Ang16, word_6FE058, word_6FE3B8.sub_401CB0(Fix16(45)), 0x6FE058);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE37C, dword_6FE1C4, 0x6FE37C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE004, Fix16(0x1C00, 0), 0x6FE004);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE2B0, k_dword_6FE210 - dword_6FE320, 0x6FE2B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE340, dword_6FE218 + dword_6FDFF8, 0x6FE340);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDF1C, Fix16(0xFFFFC000, 0), 0x6FDF1C);
DEFINE_GLOBAL(Fix16_Point, stru_6FE300, 0x6FE300);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDFFC, Fix16(0x3999, 0), 0x6FDFFC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE358, dword_6FE2E0*(dword_6FDFD0 + k_dword_6FE210), 0x6FE358);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE078, (dword_6FE3C4 * dword_6FDFFC), 0x6FE078);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE390, (dword_6FE2E0 * dword_6FDFFC), 0x6FE390);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE080, dword_6FE3C4*(dword_6FDFD0 + k_dword_6FE210), 0x6FE080);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE270, Fix16(0x400, 0), 0x6FE270);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE178, Fix16(0x1800, 0), 0x6FE178);

MATCH_FUNC(0x559E90)
Fix16 CarPhysics_B0::ComputeZPosition_559E90()
{
    if (field_70 > kFP16Zero_6FE20C)
    {
        Fix16 cp3 = field_6C_cp3;
        cp3 += k_dword_6FE210;
        if (cp3 > k_dword_6FDF34)
        {
            cp3 = k_dword_6FDF34;
        }
        return cp3;
    }
    else
    {
        return field_6C_cp3;
    }
}

MATCH_FUNC(0x40B560)
Fix16_Point CarPhysics_B0::get_cp1_40B560()
{
    return field_38_cp1;
}

MATCH_FUNC(0x446ee0)
CarPhysics_B0::~CarPhysics_B0()
{
}

// https://decomp.me/scratch/xqLh0
STUB_FUNC(0x559430)
void CarPhysics_B0::ShowPhysicsDebug_559430()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5597b0)
void CarPhysics_B0::ShowSpeedRevsDamage_5597B0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5599d0)
bool CarPhysics_B0::IsNotMoving_5599D0()
{
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_8_truck_cab->field_58_physics->sub_49EF80() &&
            pTrailer->field_C_pCarOnTrailer->field_58_physics->sub_49EF80();
    }
    else
    {
        return sub_49EF80();
    }
}

// 89%
WIP_FUNC(0x559a40)
void CarPhysics_B0::UpdateTrailerPhysicsFromTowingCar_559A40()
{
    WIP_IMPLEMENTED;

    if (field_5C_pCar->field_64_pTrailer)
    {
        Fix16 v6 = (k_dword_6FE210) / dword_6FE198;
        CarPhysics_B0* pPhysics = field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->field_58_physics;
        Fix16_Point tmp = pPhysics->field_38_cp1 - gTrailer_cp1_6FE3A8;
        pPhysics->field_40_linvel_1 = (tmp)*v6;
        Ang16 v4 = ComputeShortestAngleDelta_4056C0(gTrailer_theta_6FE018, pPhysics->field_58_theta);
        pPhysics->field_74_ang_vel_rad = v6 * Ang16::Ang16_to_Fix16(v4);
        pPhysics->field_70 = (v6 * (pPhysics->field_6C_cp3 - gTrailer_cp3_6FE1B4));
    }
}

MATCH_FUNC(0x559b40)
void CarPhysics_B0::UpdateTrailerAlignment_559B40()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->UpdateTrailerAlignment_407CE0();
    }
}

MATCH_FUNC(0x559b50)
void CarPhysics_B0::EnforceTrailerControlLimits_559B50()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        if (pTrailer->field_0)
        {
            if (this->field_94_is_backward_gas_on)
            {
                this->field_94_is_backward_gas_on = 0;
                this->field_91_is_foot_brake_on = 1;
            }
            if (!this->field_93_is_forward_gas_on)
            {
                this->field_AD_turn_direction = 0;
            }
        }
    }
}

MATCH_FUNC(0x559b90)
void CarPhysics_B0::set_field_A0_559B90(const s32& a2)
{
    field_A0 = a2;
}

MATCH_FUNC(0x559ba0)
void CarPhysics_B0::SpinOutOnOil_559BA0()
{
    if (!field_5C_pCar->IsTrainModel_403BA0())
    {
        if (field_A0 != 1 && field_A0 != 2)
        {
            if (stru_6F6784.get_int_4F7AE0(2))
            {
                set_field_A0_559B90(1);
            }
            else
            {
                set_field_A0_559B90(2);
            }
        }
        this->field_A4 = 30;
        const u32 new_val = rng_dword_67AB34->field_0_rng + 30;
        if (new_val > field_8_total_damage_q)
        {
            this->field_8_total_damage_q = new_val;
        }
    }
}

// https://decomp.me/scratch/yM7OA Fix16 annoying inlined stuff
WIP_FUNC(0x559c30)
void CarPhysics_B0::ScarePedsOnDrivingFast_559C30()
{
    WIP_IMPLEMENTED;
    Fix16 cp3 = field_6C_cp3;

    if (!gMap_0x370_6F6268->sub_466CF0(field_38_cp1.x.ToInt(), field_38_cp1.y.ToInt(), cp3.ToInt()))
    {
        cp3 = field_6C_cp3 - k_dword_6FE210;
    }

    gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_38_cp1.x.ToInt(), field_38_cp1.y.ToInt(), cp3.ToInt());
    if (pBlock)
    {
        u8 type = pBlock->field_B_slope_type & 3; //get_block_type(pBlock->field_B_slope_type);
        if (type == PAVEMENT || type == FIELD)
        {
            if (field_5C_pCar->field_54_driver)
            {
                if (!field_5C_pCar->IsTrainModel_403BA0())
                {
                    //Fix16 linvel_length = get_car_lin_vel_4754D0();

                    if (field_40_linvel_1.GetLength_2() > FastCarMinVelocity_6FE1CC || field_5C_pCar->IsEmittingHorn_411970())
                    {
                        field_5C_pCar->field_54_driver->AddThreateningPedToList_46FC70();
                    }
                }
            }
        }
    }

    if (field_A0 > 0 && field_A0 <= 2)
    {
        field_A4--;
        if (field_A4 == 0)
        {
            CarPhysics_B0::set_field_A0_559B90(0);
        }
    }
}

MATCH_FUNC(0x559dd0)
void CarPhysics_B0::ApplyForcedSteering_559DD0()
{
    if (this->field_5C_pCar->field_54_driver)
    {
        if (field_A0 == 1)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = -1;
        }
        else if (field_A0 == 2)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = 1;
        }
    }
}

MATCH_FUNC(0x559e20)
void CarPhysics_B0::ApplyObjectImpact_559E20(Object_2C* pObj)
{
    s8 v1;
    s8 v2;
    sub_529050(pObj->field_26_varrok_idx, &v1, &v2);
    stru_6FDF50.x += dword_6FE2E0 * v1;
    stru_6FDF50.y += dword_6FE2E0 * v2;
    u32 rng = rng_dword_67AB34->field_0_rng + 15;
    if (rng > this->field_8_total_damage_q)
    {
        this->field_8_total_damage_q = rng;
    }
}

WIP_FUNC(0x559ec0)
Fix16_Point CarPhysics_B0::ComputeCombinedCenterOfMass_559EC0()
{
    WIP_IMPLEMENTED;

    if (field_5C_pCar->field_64_pTrailer)
    {
        Fix16 cab_mass = field_5C_pCar->field_64_pTrailer->field_8_truck_cab->get_mass_43A120();
        Fix16 trailer_mass = field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->get_mass_43A120();

        Fix16 total_mass = trailer_mass + cab_mass;

        Fix16_Point v10 =
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->field_58_physics->field_30_cm1 * (trailer_mass / total_mass);
        Fix16_Point v9 = field_5C_pCar->field_64_pTrailer->field_8_truck_cab->field_58_physics->field_30_cm1 * (cab_mass / total_mass);

        return v9 + v10;
    }
    else
    {
        return field_30_cm1;
    }
}

MATCH_FUNC(0x559ff0)
Fix16 CarPhysics_B0::CalculateMass_559FF0()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->get_mass_43A120() +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->get_mass_43A120();
    }
    else
    {
        return field_5C_pCar->get_mass_43A120();
    }
}

MATCH_FUNC(0x55a050)
Fix16 CarPhysics_B0::GetEffectiveMomentOfInertia_55A050()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->GetMomentOfInertia_43A590() +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->GetMomentOfInertia_43A590();
    }
    else
    {
        return field_5C_pCar->GetMomentOfInertia_43A590();
    }
}

MATCH_FUNC(0x55a0b0)
u8 CarPhysics_B0::IsInAir_55A0B0()
{
    // TODO: The surface checks are likely inlines
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_8_truck_cab->field_58_physics->field_98_surface_type == 6 &&
            pTrailer->field_C_pCarOnTrailer->field_58_physics->field_98_surface_type == 6;
    }
    return field_98_surface_type == 6;
}

MATCH_FUNC(0x55a100)
Fix16 CarPhysics_B0::GetTrailerAwareTurnRatio_55A100()
{
    // TODO: This check is very likely an inline
    if (field_5C_pCar->field_64_pTrailer != NULL && field_5C_pCar->field_64_pTrailer->field_8_truck_cab == field_5C_pCar)
    {
        return dword_6FE1B0 * gCarInfo_48_6FE258->field_18_turn_ratio;
    }
    else
    {
        return gCarInfo_48_6FE258->field_18_turn_ratio;
    }
}

MATCH_FUNC(0x55a150)
char_type CarPhysics_B0::IsFootBrakeOn_55A150()
{
    char_type bFootBrakeOn;

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pPhysics)
        {
            bFootBrakeOn = pPhysics->field_91_is_foot_brake_on;
        }
        else
        {
            bFootBrakeOn = 0;
        }
    }
    else
    {
        return this->field_91_is_foot_brake_on;
    }
    return bFootBrakeOn;
}

MATCH_FUNC(0x55a180)
char_type CarPhysics_B0::IsAccelerationOrReverseOn_55A180()
{
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pCarPhysics)
        {
            return (pCarPhysics->field_93_is_forward_gas_on || pCarPhysics->field_94_is_backward_gas_on);
        }
    }
    else
    {
        return (field_93_is_forward_gas_on || field_94_is_backward_gas_on);
    }
    return 0;
}

// 9.6f 0x49F760
WIP_FUNC(0x55a1d0)
void CarPhysics_B0::SetVelocityTowardTarget_55A1D0(Fix16 targetX, Fix16 targetY, Fix16 targetAngle, s32* rotationMode)
{
    WIP_IMPLEMENTED;

    Fix16_Point local(targetX, targetY);
    CarInfo_2C* pCarInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->field_84_car_info_idx);

    Fix16_Point offset;
    offset.x = pCarInfo->field_C_center_of_mass_offset.x;
    offset.y = pCarInfo->field_C_center_of_mass_offset.y;
    offset.RotateByAngle_40F6B0(field_58_theta);

    Fix16_Point worldPoint = local + offset;
    offset.x = worldPoint.x;
    offset.y = worldPoint.y;

    field_40_linvel_1 = offset - field_30_cm1;

    field_74_ang_vel_rad = targetAngle - Ang16::Ang16_to_Fix16(field_58_theta);

    if (*rotationMode == 1)
    {
        if (field_74_ang_vel_rad < kFP16Zero_6FE20C)
        {
            field_74_ang_vel_rad += k_dword_6FE314;
        }
    }
    else if (*rotationMode == 2)
    {
        if (field_74_ang_vel_rad > kFP16Zero_6FE20C)
        {
            field_74_ang_vel_rad -= k_dword_6FE314;
        }
    }

    if (field_8C_state != 4)
    {
        field_8C_state = 0;
    }
}

MATCH_FUNC(0x55a400)
void CarPhysics_B0::restore_saved_physics_state_55A400()
{
    this->field_30_cm1 = g_cm1_6FDF10;
    this->field_6C_cp3 = g_cp3_6FDF08;
    this->field_58_theta = g_theta_6FE344;
    this->field_38_cp1 = g_cp1_6FDF00;
    this->field_70 = g_f70_6FDFE0;
    this->field_68_z_pos = g_ZPos_6FE0AC;
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pB0 = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pB0->field_30_cm1 = g_trailer_cm1_6FE068;
        pB0->field_6C_cp3 = gTrailer_cp3_6FE1B4;
        pB0->field_58_theta = gTrailer_theta_6FE018;
        pB0->field_38_cp1 = gTrailer_cp1_6FE3A8;
        pB0->field_70 = gTrailer_f70_6FE194;
        pB0->field_68_z_pos = gTrailer_ZPos_6FE354;
    }
}

MATCH_FUNC(0x55a4b0)
void CarPhysics_B0::save_physics_state_55A4B0()
{
    g_cm1_6FDF10 = this->field_30_cm1;
    g_cp3_6FDF08 = this->field_6C_cp3;
    g_theta_6FE344 = this->field_58_theta;
    g_cp1_6FDF00 = this->field_38_cp1;
    g_f70_6FDFE0 = this->field_70;
    g_ZPos_6FE0AC = this->field_68_z_pos;
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        g_trailer_cm1_6FE068 = pPhysics->field_30_cm1;
        gTrailer_cp3_6FE1B4 = pPhysics->field_6C_cp3;
        gTrailer_theta_6FE018 = pPhysics->field_58_theta;
        gTrailer_cp1_6FE3A8 = pPhysics->field_38_cp1;
        gTrailer_f70_6FE194 = pPhysics->field_70;
        gTrailer_ZPos_6FE354 = pPhysics->field_68_z_pos;
    }
}

MATCH_FUNC(0x55a550)
void CarPhysics_B0::restore_state_55A550()
{
    this->field_30_cm1 = gSaved_cm1_6FE3C8;
    this->field_6C_cp3 = gSaved_cp3_6FDF84;
    this->field_58_theta = gSaved_theta_6FE158;
    this->field_38_cp1 = gSaved_cp1_6FE090;
    this->field_70 = gSaved_f70_6FE268;
    this->field_68_z_pos = gSaved_zpos_6FE32C;

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pPhysics->field_30_cm1 = gSaved_trailer_cm1_6FE160;
        pPhysics->field_6C_cp3 = gSaved_trailed_cp3_6FDF8C;
        pPhysics->field_58_theta = gSaved_trailer_theta_6FE310;
        pPhysics->field_38_cp1 = gSaved_trailer_cp1_6FDF40;
        pPhysics->field_70 = gSaved_trailer_f70_6FE0E0;
        pPhysics->field_68_z_pos = gSaved_trailer_zpos_6FE394;
    }
}

MATCH_FUNC(0x55a600)
void CarPhysics_B0::save_state_55A600()
{
    gSaved_cm1_6FE3C8 = this->field_30_cm1;
    gSaved_cp3_6FDF84 = this->field_6C_cp3;
    gSaved_theta_6FE158 = this->field_58_theta;
    gSaved_cp1_6FE090 = this->field_38_cp1;
    gSaved_f70_6FE268 = this->field_70;
    gSaved_zpos_6FE32C = this->field_68_z_pos;
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        gSaved_trailer_cm1_6FE160 = pPhysics->field_30_cm1;
        gSaved_trailed_cp3_6FDF8C = pPhysics->field_6C_cp3;
        gSaved_trailer_theta_6FE310 = pPhysics->field_58_theta;
        gSaved_trailer_cp1_6FDF40 = pPhysics->field_38_cp1;
        gSaved_trailer_f70_6FE0E0 = pPhysics->field_70;
        gSaved_trailer_zpos_6FE394 = pPhysics->field_68_z_pos;
    }
}

WIP_FUNC(0x55a6a0)
Fix16 CarPhysics_B0::ComputeRequiredSweepSteps_55A6A0()
{
    WIP_IMPLEMENTED;

    Fix16 v9 = Fix16::Max((Fix16::ClampToRangeFlexible_55EEE0(Fix16::Abs(gSaved_cm1_6FE3C8.x - g_cm1_6FDF10.x),
                                                              Fix16::Abs(gSaved_cm1_6FE3C8.y - g_cm1_6FDF10.y),
                                                              Fix16::Abs(gSaved_cp3_6FDF84 - g_cp3_6FDF08))) /
                              field_5C_pCar->sub_43A5B0(),
                          Ang16::NormalizeAngleDeltaScaled_405B60(g_theta_6FE344, gSaved_theta_6FE158, word_6FE058));

    if (field_5C_pCar->field_64_pTrailer)
    {

        v9 = Fix16::ClampToRangeFlexible_55EEE0(
            v9,
            (Fix16::ClampToRangeFlexible_55EEE0(Fix16::Abs(gSaved_trailer_cm1_6FE160.x - g_trailer_cm1_6FE068.x),
                                                Fix16::Abs(gSaved_trailer_cm1_6FE160.y - g_trailer_cm1_6FE068.y),
                                                Fix16::Abs(gSaved_trailed_cp3_6FDF8C - gTrailer_cp3_6FE1B4)) /
             field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->sub_43A5B0()),
            Ang16::NormalizeAngleDeltaScaled_405B60(gTrailer_theta_6FE018, gSaved_trailer_theta_6FE310, word_6FE058));
    }

    return v9;
}

MATCH_FUNC(0x55a840)
void CarPhysics_B0::ResetForceAccumulators_55A840()
{
    field_48.x = 0;
    field_48.y = 0;
    field_50.x = 0;
    field_50.y = 0;
    field_7C = 0;
    field_80 = 0;
}

// https://decomp.me/scratch/efo3b
WIP_FUNC(0x55a860)
void CarPhysics_B0::HandleUserInputs_55A860(char_type bForwardGasOn,
                                            char_type bFootBrakeOn,
                                            char_type bLeftOn,
                                            char_type bRightOn,
                                            char_type bHandBrakeOn)
{
    WIP_IMPLEMENTED;

    Ang16 ang; // di
    Ang16 v8; // [esp+Ah] [ebp-2h] BYREF

    if (this->field_40_linvel_1.x == kFP16Zero_6FE20C && this->field_40_linvel_1.y == kFP16Zero_6FE20C)
    {
        this->field_93_is_forward_gas_on = bForwardGasOn;
        this->field_94_is_backward_gas_on = bFootBrakeOn;
        this->field_91_is_foot_brake_on = 0;
    }
    else
    {
        Ang16 v8 = Fix16::atan2_fixed_405320(
            //kFP16Zero_6FE20C,
            this->field_40_linvel_1.y,
            this->field_40_linvel_1.x);

        ang = v8 - this->field_58_theta;
        ang.Normalize();
        /*
    if ( ang < 0 )
    {
      ang += 1440 * ((1439 - ang) / 0x5A0u);
    }
    if ( ang >= 1440 )
    {
      ang %= 0x5A0u;
    }*/

        if (ang > word_6FE00C && ang < word_6FE154)
        {
            if (this->field_93_is_forward_gas_on)
            {
                this->field_94_is_backward_gas_on = 0;
                this->field_93_is_forward_gas_on = 0;
                this->field_91_is_foot_brake_on = 0;
            }
            else
            {
                this->field_91_is_foot_brake_on = bForwardGasOn;
                this->field_94_is_backward_gas_on = bFootBrakeOn;
                this->field_93_is_forward_gas_on = 0;
            }
        }
        else if (this->field_94_is_backward_gas_on)
        {
            this->field_93_is_forward_gas_on = 0;
            this->field_94_is_backward_gas_on = 0;
            this->field_91_is_foot_brake_on = 0;
        }
        else
        {
            this->field_93_is_forward_gas_on = bForwardGasOn;
            this->field_91_is_foot_brake_on = bFootBrakeOn;
            this->field_94_is_backward_gas_on = 0;
        }
    }
    this->field_95 = 0;
    this->field_92_is_hand_brake_on = bHandBrakeOn;
    if (bRightOn)
    {
        if (!bLeftOn)
        {
            this->field_AD_turn_direction = -1;
            return;
        }
    }
    else if (bLeftOn)
    {
        this->field_AD_turn_direction = 1;
        return;
    }
    this->field_AD_turn_direction = 0;
}

WIP_FUNC(0x55aa00)
void CarPhysics_B0::HandleGravityOnSlope_55AA00()
{
    WIP_IMPLEMENTED;

    Fix16_Point_POD force;

    // On a slope and no brake inputs
    if (field_A5_current_slope_length != 1 || field_92_is_hand_brake_on || field_91_is_foot_brake_on)
    {
        return;
    }

    // Trains ignore slope gravity
    if (field_5C_pCar->IsTrainModel_403BA0())
    {
        return;
    }

    switch (field_98_surface_type)
    {
        case 1:
            force.x = kFP16Zero_6FE20C;
            force.y = (dword_6FDF3C * dword_6FDF7C);
            ApplyForceScaledByMass_55F9A0(force);
            break;

        case 2:
            force.x = kFP16Zero_6FE20C;
            force.y = (dword_6FDF7C * -dword_6FDF3C);
            ApplyForceScaledByMass_55F9A0(force);
            break;

        case 3:
            force.x = (dword_6FDF3C * dword_6FDF7C);
            force.y = kFP16Zero_6FE20C;
            ApplyForceScaledByMass_55F9A0(force);
            break;

        case 4:
            force.x = (dword_6FDF7C * -dword_6FDF3C);
            force.y = kFP16Zero_6FE20C;
            ApplyForceScaledByMass_55F9A0(force);
            break;

        default:
            return;
    }
}

WIP_FUNC(0x55ab50)
Fix16* CarPhysics_B0::ComputeSlopeCorrection_55AB50(Fix16* pOutX, Fix16* pOutY)
{
    WIP_IMPLEMENTED;

    Fix16_Point point_to_sub;
    if (field_5C_pCar->field_64_pTrailer && field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer == field_5C_pCar)
    {
        point_to_sub = gTrailer_cp1_6FE3A8;
    }
    else
    {
        point_to_sub = g_cp1_6FDF00;
    }
    Fix16_Point sub_point = field_38_cp1 - point_to_sub;
    Fix16 x_val = sub_point.x;
    Fix16 y_val = sub_point.y;

    s32 surface_type_m1 = this->field_98_surface_type - 1;
    Fix16 x_val_ = x_val;
    Fix16 y_val_ = y_val;

    Fix16 slope_val;
    Fix16 lower;
    Fix16 upper;
    Fix16* result;

    switch (surface_type_m1)
    {
        case 0:
            y_val = -y_val;
            goto LABEL_7;

        case 1:
        LABEL_7:
            x_val = y_val;
            goto LABEL_9;

        case 2:
            x_val = -x_val;
            goto LABEL_9;

        case 3:
        LABEL_9:
            switch (this->field_A5_current_slope_length)
            {
                case 1:
                    slope_val = dword_6FDF7C;
                    break;
                case 2:
                    slope_val = dword_6FE064;
                    break;
                case 8:
                    slope_val = dword_6FE350;
                    break;
                default:
                    slope_val = kFP16Zero_6FE20C;
                    break;
            }
            lower = (x_val * slope_val);
            if (!this->field_A6_current_slope_left_tiles && lower > kFP16Zero_6FE20C &&
                    (u8)(this->field_6C_cp3.ToInt()) == this->field_A7_current_tile_z ||
                this->field_AA_sbw && this->field_AB_tpa)
            {
                upper = k_dword_6FE210 - (this->field_6C_cp3.GetFracValue());
                if (lower >= upper)
                {
                    *pOutY = lower;
                }
                else
                {
                    *pOutY = upper;
                }
                result = pOutX;
                *pOutX = lower;
            }
            else
            {
                if (lower > kFP16Zero_6FE20C)
                {
                    if (field_5C_pCar->field_64_pTrailer)
                    {
                        if (field_5C_pCar->field_64_pTrailer->GetCabOrLoadedCar_407B90(field_5C_pCar)
                                ->field_58_physics->field_98_surface_type != 6)
                        {
                            lower = kFP16Zero_6FE20C;
                        }
                    }
                }
                result = pOutX;
                *pOutY = lower;
                *pOutX = lower;
            }
            break;

        default:
            *pOutY = kFP16Zero_6FE20C;
            result = pOutX;
            *pOutX = kFP16Zero_6FE20C;
            break;
    }
    return result;
}

WIP_FUNC(0x55ad90)
void CarPhysics_B0::UpdateZPhysics_55AD90(Fix16 a2)
{
    WIP_IMPLEMENTED;

    Fix16 a2_ = a2;
    s32 surface_type = this->field_98_surface_type;

    Fix16 map_z;
    Fix16 zpos;
    Fix16 v20;
    Fix16* ground_z;
    Fix16 cp3;
    s32 surface_type_;
    Fix16* map_z_;
    Fix16 map_z__;
    Fix16 a2__;
    Fix16* new_z;

    switch (surface_type)
    {
        case 6:
            if (this->field_6C_cp3 > k_dword_6FDF34)
            {
                this->field_6C_cp3 = k_dword_6FDF34;
            }
            map_z = *gMap_0x370_6F6268->sub_4E4D40(&v20, this->field_38_cp1.x, this->field_38_cp1.y, this->field_6C_cp3);
            zpos = this->field_6C_cp3 + ((a2_ * g_ZPos_6FE0AC));
            if (zpos > map_z)
            {
                if (zpos > k_dword_6FDF34)
                {
                    zpos = k_dword_6FDF34;
                }
            }
            else
            {
                zpos = map_z;
            }
            break;

        case 7:
            goto LABEL_36;

        case 8:
            goto LABEL_36;

        default:
            ground_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&v20, field_38_cp1.x, field_38_cp1.y);
            zpos = *ground_z;
            if (*ground_z == kFP16Zero_6FE20C)
            {
                zpos = k_dword_6FE210;
            }
            cp3 = this->field_6C_cp3;
            if (zpos >= cp3 + dword_6FE0C0)
            {
                surface_type_ = this->field_98_surface_type;
                if (surface_type_ != 1 && surface_type_ != 2 && surface_type_ != 3 && surface_type_ != 4 ||
                    (zpos.GetFracValue()) == kFP16Zero_6FE20C || zpos > cp3 + k_dword_6FE210)
                {
                    map_z_ = gMap_0x370_6F6268->sub_4E4D40(&v20, this->field_38_cp1.x, this->field_38_cp1.y, zpos - dword_6FE2E0);
                    zpos = *map_z_;

                    if (*map_z_ > this->field_6C_cp3)
                    {
                        map_z__ = *gMap_0x370_6F6268->sub_4E4F40(&v20, this->field_38_cp1.x, this->field_38_cp1.y, zpos - dword_6FE2E0);
                        if (map_z__ > kFP16Zero_6FE20C)
                        {
                            zpos = map_z__;
                        }
                    }

                    cp3 = this->field_6C_cp3;
                    if (zpos >= cp3 + k_dword_6FE210)
                    {
                        zpos = this->field_6C_cp3;
                    }
                }
            }
            if (zpos <= cp3 - dword_6FE0C0 || zpos < cp3 && this->field_AA_sbw && this->field_AB_tpa)
            {
                new_z = ComputeSlopeCorrection_55AB50(&v20, &a2);
                a2__ = a2;
                this->field_68_z_pos = *new_z;

                zpos = this->field_6C_cp3 + a2__;
                if (a2_ != kFP16Zero_6FE20C)
                {
                    this->field_68_z_pos = field_68_z_pos / a2_;
                }

                if (zpos > k_dword_6FDF34)
                {
                    zpos = k_dword_6FDF34;
                }
            }

            if (zpos < this->field_6C_cp3)
            {
                UpdateSpriteFromPhysics_563670();

                field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                        field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                        zpos);

                if (field_5C_pCar->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
                {
                LABEL_36:
                    zpos = this->field_6C_cp3;
                }
            }
            break;
    }

    if ((this->field_5C_pCar->field_78_flags & 0x2000) != 0)
    {
        UpdateSpriteFromPhysics_563670();

        field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                zpos);

        gCar_6C_677930->field_60 = 2;
        gCar_6C_677930->field_64_zpos = kFP16Zero_6FE20C;

        gPurpleDoom_1_679208->CheckAndHandleAllCollisionsForSprite_477C30(field_5C_pCar->field_50_car_sprite, sprite_types_enum::car);

        gCar_6C_677930->field_64_zpos += dword_6FDFD8;
        if (gCar_6C_677930->field_64_zpos > zpos)
        {
            zpos = gCar_6C_677930->field_64_zpos;
        }
    }

    this->field_70 = zpos - this->field_6C_cp3;
    this->field_6C_cp3 += field_70;
    if (a2_ != kFP16Zero_6FE20C)
    {
        this->field_70 = field_70 / a2_;
    }
}

WIP_FUNC(0x55b3f0)
void CarPhysics_B0::SyncZWithTrailer_55B3F0(Fix16 a2)
{
    WIP_IMPLEMENTED;

    UpdateZPhysics_55AD90(a2);

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarOnTrailerPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pCarOnTrailerPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pCarOnTrailerPhysics->UpdateZPhysics_55AD90(a2);
        SetCurrentCarInfoAndModelPhysics_562EF0();
        Fix16 ourCp3 = this->field_6C_cp3;
        Fix16 carOnTrailer_cp3 = pCarOnTrailerPhysics->field_6C_cp3;
        if (ourCp3 <= carOnTrailer_cp3)
        {
            if (ourCp3 < carOnTrailer_cp3)
            {
                this->field_6C_cp3 = carOnTrailer_cp3;
                this->field_70 = pCarOnTrailerPhysics->field_70;
            }
        }
        else
        {
            pCarOnTrailerPhysics->field_6C_cp3 = ourCp3;
            pCarOnTrailerPhysics->field_70 = this->field_70;
        }
    }
}

WIP_FUNC(0x55b4f0)
void CarPhysics_B0::UpdateZPosition_55B4F0(Fix16 a2)
{
    WIP_IMPLEMENTED;

    Fix16 zCoord;
    Fix16* pZCoord = gMap_0x370_6F6268->GetRailwayZCoordAtXY_4E6510(&zCoord, this->field_38_cp1.x, this->field_38_cp1.y);
    Fix16 zCoordTmp = *pZCoord;
    if (*pZCoord == kFP16Zero_6FE20C)
    {
        zCoordTmp = k_dword_6FE210;
    }

    if (zCoordTmp >= field_6C_cp3 + dword_6FE0C0)
    {
        zCoordTmp = *gMap_0x370_6F6268->sub_4E6400(&zCoord, field_38_cp1.x, field_38_cp1.y, zCoordTmp - k_dword_6FE210);
        if (zCoordTmp >= field_6C_cp3 + k_dword_6FE210)
        {
            zCoordTmp = this->field_6C_cp3;
        }
    }

    Fix16 a2_ = a2;
    if (zCoordTmp <= field_6C_cp3 - dword_6FE0C0)
    {
        Fix16 v14;
        Fix16 new_z = *ComputeSlopeCorrection_55AB50(&a2, &v14);
        this->field_68_z_pos = new_z;
        zCoordTmp = new_z + field_6C_cp3;
        if (a2_ != kFP16Zero_6FE20C)
        {
            field_68_z_pos = field_68_z_pos / a2_;
        }
    }
    else
    {
        if (zCoordTmp < field_6C_cp3)
        {
            UpdateSpriteFromPhysics_563670();

            field_5C_pCar->field_50_car_sprite->set_xyz_lazy_420600(field_5C_pCar->field_50_car_sprite->field_14_xy.x,
                                                                    field_5C_pCar->field_50_car_sprite->field_14_xy.y,
                                                                    zCoordTmp);

            if (field_5C_pCar->field_50_car_sprite->CheckSpriteMovementRegion_5A2500())
            {
                zCoordTmp = this->field_6C_cp3;
            }
        }
    }

    this->field_70 = zCoordTmp - this->field_6C_cp3;
    this->field_6C_cp3 += field_70;
    if (a2_ != kFP16Zero_6FE20C)
    {
        field_70 = field_70 / a2_;
    }
}

MATCH_FUNC(0x55B7B0)
void CarPhysics_B0::UpdateZPosition_55B7B0(Fix16 a2)
{
    UpdateZPosition_55B4F0(a2);
}

WIP_FUNC(0x55B7E0)
void CarPhysics_B0::EmitImpactParticles_55B7E0(u8 apply_to_corners_mask)
{
    WIP_IMPLEMENTED;

    //s32 box_idx; // edi
    //u8 box_corner_mask; // bl
    //u32 cornerCount; // [esp+8h] [ebp-20h] BYREF
    //Fix16_Point box_xy; // [esp+14h] [ebp-14h] BYREF
    Fix16_Point box_xy;

    Car_BC* pCar = field_5C_pCar;
    Sprite* pCarSprite = pCar->field_50_car_sprite;
    s32 not_used = 0;
    u32 cornerCount = 0;

    if (!pCar->field_50_car_sprite->IsOnWater_59E1D0() && !field_5C_pCar->IsMaxDamage_40F890())
    {
        if (apply_to_corners_mask == 0)
        {
            apply_to_corners_mask = pCarSprite->CheckCornerZCollisions_5A1CA0(&cornerCount);
        }
        s32 box_idx = 0;
        u8 box_corner_mask = 1;
        do
        {
            if ((apply_to_corners_mask & box_corner_mask) == box_corner_mask)
            {
                box_xy = pCarSprite->GetBoundingBoxCorner_562450(box_idx);
                gParticle_8_6FD5E8->EmitImpactParticles_53FE40(box_xy.x, box_xy.y, field_6C_cp3, field_40_linvel_1.x, field_40_linvel_1.y);
            }
            box_idx++;
            box_corner_mask *= 2;
        } while (box_idx < 4);
    }

    gRozza_C88_66AFE0->Type4_40BC40(pCarSprite);
}

WIP_FUNC(0x55b970)
char_type CarPhysics_B0::ProcessGroundCollisionAndSurfaceType_55B970(char_type* check_mask)
{
    WIP_IMPLEMENTED;

    Sprite* pSprite = this->field_5C_pCar->field_50_car_sprite;
    s32 corner_idx_ = 0;
    this->field_AB_tpa = 0;

    if (IsInAir_55A0B0())
    {
        if (this->field_68_z_pos > -dword_6FE370)
        {
            this->field_68_z_pos -= dword_6FE1AC;
        }
    }

    if (gMap_0x370_6F6268->sub_4E5170(this->field_38_cp1.x, this->field_38_cp1.y, this->field_6C_cp3))
    {
        *check_mask = 0;
        this->field_9C =
            gMap_0x370_6F6268->GetBlockSpec_4E00A0(this->field_38_cp1.x, this->field_38_cp1.y, this->field_6C_cp3 - k_dword_6FE210);
    }
    else
    {
        this->field_9C = 0;
        u32 v29;
        *check_mask = pSprite->CheckCornerZCollisions_5A1CA0(&v29);
        if (v29 == 1 || v29 == 2)
        {
            this->field_AB_tpa = 1;
        }

        if (*check_mask)
        {
            Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
            if (!pTrailer || pTrailer->GetCabOrLoadedCar_407B90(field_5C_pCar)->field_58_physics->field_98_surface_type == 6)
            {
                u8 mask_ = 1;
                do
                {
                    if (((u8)mask_ & (u8)*check_mask) != mask_)
                    {
                        Fix16 v28 = Fix16(0xC8000, 0);
                        Fix16_Point pCorner_ = pSprite->GetBoundingBoxCorner_562450(corner_idx_);
                        Fix16_Point v10 = (pCorner_ - field_30_cm1);
                        Fix16_Point v11 = (v10 / v28);
                        ApplyImpulseWithTrailerRedirect_55FA10(&v11);
                    }
                    ++corner_idx_;
                    mask_ *= 2;
                } while (corner_idx_ < 4);
            }
        }
        else
        {
            if (field_98_surface_type != 1 && field_98_surface_type != 2 && field_98_surface_type != 3 && field_98_surface_type != 4)
            {
                this->field_98_surface_type = 6;
                return 0;
            }

            char_type result = pSprite->IsTouchingSlopeBlock_5A1EB0();
            if (!result)
            {
                this->field_98_surface_type = 6;
                return result;
            }
        }
    }

    Fix16 cp3 = this->field_6C_cp3;
    bool is6 = this->field_98_surface_type == 6;
    if (cp3.GetFracValue() == kFP16Zero_6FE20C)
    {
        cp3 -= k_dword_6FE210;
    }

    s32 cp3_int = cp3.ToInt();
    gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_38_cp1.x.ToInt(), this->field_38_cp1.y.ToInt(), cp3.ToInt());
    gmp_block_info* pBlock_ = pBlock;

    u8 v28;
    u8 gradient_level;
    u8 graident_size;

    if (!pBlock)
    {
        //LABEL_36:
        v28 = 5;
        //goto LABEL_37;
    }
    else
    {
        s32 lid_idx = pBlock->field_8_lid & 0x3FF;
        if (gGtx_0x106C_703DD4->field_6C_spec[pBlock_->field_8_lid & 0x3FF] != 4 || !gGtx_0x106C_703DD4->IsTileRemapped_5AA850(lid_idx))
        {
            //LABEL_31:
            if ((pBlock_->field_B_slope_type & 3) != 0)
            {
                s32 lid_idx_ = pBlock_->field_8_lid & 0x3FF;
                if (gGtx_0x106C_703DD4->field_6C_spec[lid_idx_] != 4 || gGtx_0x106C_703DD4->IsTileRemapped_5AA850(lid_idx_))
                {
                    graident_size = byte_6F5BA8[pBlock_->field_B_slope_type >> 2].field_1_gradient_size;
                    gradient_level = byte_6F5BA8[pBlock_->field_B_slope_type >> 2].field_2_gradient_level;
                    v28 = byte_6F5BA8[pBlock_->field_B_slope_type >> 2].field_0_gradient_direction;
                }
                else
                {
                    v28 = 9;
                }
                //goto LABEL_37;
            }
            else
            {
                v28 = 5;
                //goto LABEL_36;
            }
        }
        else
        {
            /*
            if (!gGtx_0x106C_703DD4->IsTileRemapped_5AA850(lid_idx))
            {
                goto LABEL_31;
            }
            else*/
            {
                v28 = 7;
            }
        }
    }

    //LABEL_37:
    if (v28 != 5)
    {
        this->field_98_surface_type = (u8)v28;
        this->field_A5_current_slope_length = (char)graident_size;
        this->field_A6_current_slope_left_tiles = gradient_level;
        this->field_A7_current_tile_z = cp3_int;
        if (v28 == 7)
        {
            s32 water_mask = pSprite->GetWaterCornerMask_59E250();
            s32 water_mask_ = water_mask;
            if (water_mask == 15)
            {
                this->field_98_surface_type = 8;
            }
            else
            {
                s32 corner_idx = 0;
                s32 mask = 1;
                do
                {
                    if (((u8)mask & (u8)water_mask) != mask)
                    {
                        Fix16 v28_ = Fix16(819200, 0);
                        Fix16_Point pCorner = pSprite->GetBoundingBoxCorner_562450(corner_idx);
                        Fix16_Point v24 = field_30_cm1 - pCorner;
                        Fix16_Point v25 = v24 / v28_;
                        ApplyImpulseWithTrailerRedirect_55FA10(&v25);
                        water_mask = water_mask_;
                    }
                    ++corner_idx;
                    mask *= 2;
                } while (corner_idx < 4);
            }
        }
    }
    return is6;
}

WIP_FUNC(0x55bfe0)
void CarPhysics_B0::ProcessGroundCollisionAndEmitImpactParticles_55BFE0()
{
    WIP_IMPLEMENTED;

    char b1;
    CarPhysics_B0* pPhysics;
    char b2;
    char b1_;
    u8 corner_bits1[4];
    u8 corner_bits2[4];

    b1 = ProcessGroundCollisionAndSurfaceType_55B970((char*)corner_bits1);
    b1_ = b1;
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (!pTrailer)
    {
        if (!b1)
        {
            return;
        }
        goto LABEL_11;
    }
    pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
    pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
    b2 = pPhysics->ProcessGroundCollisionAndSurfaceType_55B970((char*)corner_bits2);
    if (b2)
    {
        if (b1_ || field_98_surface_type == 6)
        {
            pPhysics->EmitImpactParticles_55B7E0(corner_bits2[0]);
        }
    }
    SetCurrentCarInfoAndModelPhysics_562EF0();
    if ((b2 || pPhysics->field_98_surface_type == 6) && b1_)
    {
    LABEL_11:
        EmitImpactParticles_55B7E0(corner_bits1[0]);
    }
}

MATCH_FUNC(0x55c150)
char_type CarPhysics_B0::TestCollision_55C150()
{
    Sprite* pCarSprite = this->field_5C_pCar->field_50_car_sprite;
    if (!pCarSprite->CheckSpriteMovementRegion_5A2500() && !pCarSprite->QuerySpriteCollision_59E7D0(0))
    {
        Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            pCarSprite = pTrailer->field_C_pCarOnTrailer->field_50_car_sprite;
            if (!pCarSprite->CheckSpriteMovementRegion_5A2500() && !pCarSprite->QuerySpriteCollision_59E7D0(0))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    gRozza_679188.field_24 = pCarSprite;
    return 1;
}

WIP_FUNC(0x55c3b0)
char_type CarPhysics_B0::SweepTestMovementForCollision_55C3B0(Fix16* outHitStep, Fix16* outNoHitStep)
{
    WIP_IMPLEMENTED;

    save_state_55A600();

    Fix16 movement = ComputeRequiredSweepSteps_55A6A0();

    *outHitStep = kFP16Zero_6FE20C;
    *outNoHitStep = kFP16Zero_6FE20C;

    // Variables used in the sweep
    Fix16 stepSize = kFP16Zero_6FE20C;
    Fix16 accumulated = kFP16Zero_6FE20C;
    s32 numSteps = 0;
    s32 stepIndex = 0;

    // TODO: Fix control flow

    // Determine whether we need to sweep
    if (movement > k_dword_6FE210)
    {
        stepSize = k_dword_6FE210 / movement;
        numSteps = movement.ToInt();
        stepIndex = 0;
    }

    // If movement is tiny or numSteps <= 0 skip sweep
    if (movement <= k_dword_6FE210 || numSteps <= 0)
    {
        restore_state_55A550();
        UpdateCarAndTrailerSpriteFromPhysics_5636C0();

        if (TestCollision_55C150())
        {
            *outHitStep = k_dword_6FE210;
            return 1;
        }

        *outNoHitStep = k_dword_6FE210;
        return 0;
    }

    // Swept movement loop
    while (true)
    {
        accumulated += stepSize;

        restore_saved_physics_state_55A400();
        ApplyMovementStep_560F20(accumulated);

        if (TestCollision_55C150())
        {
            *outHitStep = accumulated;
            return 1;
        }

        stepIndex++;
        *outNoHitStep = accumulated;

        if (stepIndex >= numSteps)
        {
            // No collision in sweep test final state
            restore_state_55A550();
            UpdateCarAndTrailerSpriteFromPhysics_5636C0();

            if (TestCollision_55C150())
            {
                *outHitStep = k_dword_6FE210;
                return 1;
            }

            *outNoHitStep = k_dword_6FE210;
            return 0;
        }
    }
}

MATCH_FUNC(0x55c560)
void CarPhysics_B0::BinarySearchCollisionTime_55C560(Fix16& a2, Fix16& a3)
{
    for (s32 i = 0; i < 3; i++)
    {
        restore_saved_physics_state_55A400();
        Fix16 total = (a3 + a2) / 2;
        ApplyMovementStep_560F20(total);
        if (TestCollision_55C150())
        {
            a2 = total;
        }
        else
        {
            a3 = total;
        }
    }
}

STUB_FUNC(0x55c5c0)
void CarPhysics_B0::HandleMapBoundaryCollisionY_55C5C0(Fix16_Point& a2, Ang16& a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55c820)
void CarPhysics_B0::HandleMapBoundaryCollisionX_55C820(Fix16_Point& a2, Ang16& a3)
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x4A4170
WIP_FUNC(0x55ca70)
void CarPhysics_B0::DispatchCollision_55CA70(Fix16_Point& a2, Ang16 a3)
{
    WIP_IMPLEMENTED;

    Fix16_Point arg0;
    u8 hitType;
    //v7 = 0;
    switch (gRozza_679188.field_0_type)
    {
        case 1:
            HandleMapBoundaryCollisionY_55C5C0(a2, a3);
            break;
        case 2:
            HandleMapBoundaryCollisionX_55C820(a2, a3);
            break;
        case 3:
            // TODO: Likely wrong arguments here
            stru_6FE1A0 = *field_5C_pCar->field_50_car_sprite->FindCollisionIntersectionPoint_5A2710(&arg0,
                                                                                                     gRozza_679188.field_20_pSprite,
                                                                                                     a2,
                                                                                                     a3,
                                                                                                     &byte_6FDFC4,
                                                                                                     &byte_6FDFCC,
                                                                                                     &hitType);

            Car_BC* pCar = gRozza_679188.field_20_pSprite->AsCar_40FEB0();
            if (pCar)
            {
                HandleCarCollision_55FF20(pCar);
            }
            else
            {
                Char_B4* pB4 = gRozza_679188.field_20_pSprite->AsCharB4_40FEA0();
                if (pB4)
                {
                    ProcessPedImpact_560B40(pB4, hitType);
                }
                else
                {
                    Object_2C* p2C = gRozza_679188.field_20_pSprite->As2C_40FEC0();
                    HandleObjectCollision_5606C0(p2C, byte_6FDFC4);
                }
            }
            break;
    }

    gRozza_C88_66AFE0->OtherType_40BBA0(field_5C_pCar->field_50_car_sprite, dword_6FE33C);
}

// https://decomp.me/scratch/0TpGe
WIP_FUNC(0x55cbb0)
void CarPhysics_B0::ReplayAndDispatchCollision_55CBB0(Fix16 a2, Fix16 a3)
{
    WIP_IMPLEMENTED;
    Car_BC* pCar;
    if (gRozza_679188.field_24->field_30_sprite_type_enum == sprite_types_enum::car)
    {
        pCar = gRozza_679188.field_24->field_8_car_bc_ptr;
    }
    else
    {
        pCar = NULL;
    }

    CarPhysics_B0* pPhysics = pCar->field_58_physics;
    CarPhysics_B0::restore_saved_physics_state_55A400();
    CarPhysics_B0::ApplyMovementStep_560F20(a2);

    Fix16_Point point(pPhysics->field_38_cp1.x, pPhysics->field_38_cp1.y);

    CarPhysics_B0::restore_saved_physics_state_55A400();
    CarPhysics_B0::ApplyMovementStep_560F20(a3);
    if (pPhysics == this)
    {
        CarPhysics_B0::DispatchCollision_55CA70(point, pPhysics->field_58_theta);
    }
    else
    {
        CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
        CarPhysics_B0::DispatchCollision_55CA70(point, pPhysics->field_58_theta);
        CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
    }
}

WIP_FUNC(0x55D490)
EXPORT s32 __stdcall get_skid_obj_type_55D490(s32 surface, Fix16 box_idx)
{
    WIP_IMPLEMENTED;

    int result; // eax

    if (box_idx > dword_6FE270)
    {
        if (box_idx > dword_6FDFD4)
        {
            if (box_idx > dword_6FE178)
            {
                switch (surface)
                {
                    case 0:
                        result = 147;
                        break;
                    case 1:
                        result = 144;
                        break;
                    case 2:
                        result = 146;
                        break;
                    case 3:
                        result = 145;
                        break;
                    default:
                    LABEL_13:
                        result = 117;
                        break;
                }
            }
            else
            {
                switch (surface)
                {
                    case 0:
                        result = 250;
                        break;
                    case 1:
                        result = 253;
                        break;
                    case 2:
                        result = 249;
                        break;
                    case 3:
                        result = 124;
                        break;
                    default:
                        goto LABEL_13;
                }
            }
        }
        else
        {
            switch (surface)
            {
                case 0:
                    result = 120;
                    break;
                case 1:
                    result = 121;
                    break;
                case 2:
                    result = 119;
                    break;
                case 3:
                    result = 125;
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\physics.cpp", 2331, surface);
            }
        }
    }
    else
    {
        switch (surface)
        {
            case 1:
                result = 118;
                break;
            case 2:
                result = 116;
                break;
            case 3:
                result = 126;
                break;
            default:
                goto LABEL_13;
        }
    }
    return result;
}

// 9.6f 0x4A0120
WIP_FUNC(0x55d200)
void CarPhysics_B0::SpawnSkidSegment_55D200(s32 box_idx, Fix16_Point arg_4, s32 surface)
{
    WIP_IMPLEMENTED;

    arg_4.RotateByAngle_40F6B0(field_58_theta);
    arg_4 += this->field_38_cp1;

    s32 map_ret = gMap_0x370_6F6268->sub_4E52A0(arg_4.x, arg_4.y, field_6C_cp3);
    if (map_ret == 5 || surface == 3 && map_ret != 7)
    {
        Fix16_Point* pBoxCorner_ = &this->field_10[(u8)box_idx];
        pBoxCorner_->clear_41E1E0();
    }
    else
    {
        Fix16_Point* pBoxCorner = &this->field_10[(u8)box_idx];
        if (!pBoxCorner->IsNull_420360())
        {
            Fix16_Point v13 = (arg_4 - *pBoxCorner);
            Fix16_Point t;
            t.x = v13.x;
            t.y = v13.y;
            box_idx = 2;
            Fix16_Point v14 = (*pBoxCorner + arg_4);
            //LOBYTE(seh) = 3;
            Fix16_Point v15 = (v14 / box_idx);
            Fix16 obj_x = v15.x;
            Fix16 obj_y = v15.y;

            //LOBYTE(seh) = 2;
            Ang16 r = t.atan2_40F790();
            Fix16 len = t.GetLength_2();
            if (len > kFP16Zero_6FE20C)
            {
                s32 obj_type = get_skid_obj_type_55D490(surface, len);
                Object_2C* pObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(obj_type, obj_x, obj_y, field_6C_cp3, r);
                if (pObj)
                {
                    if (pObj->field_4->sub_5A19C0())
                    {
                        pObj->sub_5290A0();
                    }
                }
            }
        }
        pBoxCorner->x = arg_4.x; // setting field_10
        pBoxCorner->y = arg_4.y;
    }
}

WIP_FUNC(0x55dc00)
void CarPhysics_B0::UpdateWheelSkidEffects_55DC00()
{
    WIP_IMPLEMENTED;

    if (!field_5C_pCar->IsOnScreenForAnyPlayer_43B730())
    {
        this->field_10[0].x = 0;
        this->field_10[0].y = 0;
        this->field_10[1].x = 0;
        this->field_10[1].y = 0;
        this->field_10[2].x = 0;
        this->field_10[2].y = 0;
        this->field_10[3].x = 0;
        this->field_10[3].y = 0;
        //goto LABEL_32;
    }
    else
    {
        s32 b_d9C;
        if (field_9C == 2 || (b_d9C = 0, field_9C == 10))
        {
            b_d9C = 2;
        }

        Car_BC* pCar = this->field_5C_pCar;
        Fix16 w_adjusted = (pCar->field_50_car_sprite->field_C_sprite_4c_ptr->field_0_width * dword_6FE004);

        Fix16 v9;
        if (pCar->field_68 == k_dword_6FE210)
        {
            v9 = gCarInfo_2C_6FE0E4->field_8_rear_wheel_offset;
        }
        else
        {
            v9 = gCarInfo_2C_6FE0E4->field_8_rear_wheel_offset * pCar->field_68;
        }

        Fix16 front_wheel_offset_;
        if (field_5C_pCar->field_68 != k_dword_6FE210)
        {
            front_wheel_offset_ = gCarInfo_2C_6FE0E4->field_4_front_wheel_offset * field_5C_pCar->field_68;
        }
        else
        {
            front_wheel_offset_ = gCarInfo_2C_6FE0E4->field_4_front_wheel_offset;
        }

        if (field_98_surface_type == 7 || field_98_surface_type == 8 || field_98_surface_type == 9)
        {
            //v21 = -w_adjusted;
            //v22 = v9;
            Fix16_Point spawn_pos;
            spawn_pos.y = v9;
            spawn_pos.x = -w_adjusted;
            SpawnSkidSegment_55D200(0, spawn_pos, 3); // spawns the skid obj?
            //v22 = v9;
            //v21 = w_adjusted;
            Fix16_Point t;
            t.x = v9;
            t.y = w_adjusted;
            SpawnSkidSegment_55D200(1, t, 3);
            //goto LABEL_20;
        }
        else
        {
            if ((this->field_88_rear_skid >= gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2 ||
                 this->field_AC_drive_wheels_locked_q && gCarInfo_2C_6FE0E4->field_20_front_drive_bias > kFP16Zero_6FE20C) &&
                field_98_surface_type != 6)
            {
                //v21 = -w_adjusted;
                //v22 = v9;
                Fix16_Point spawn_pos_;
                spawn_pos_.y = v9;
                spawn_pos_.x = -w_adjusted;
                SpawnSkidSegment_55D200(0, spawn_pos_, b_d9C);
                //v22 = v9;
                //v21 = w_adjusted;
                Fix16_Point t;
                t.x = v9;
                t.y = w_adjusted;
                SpawnSkidSegment_55D200(1, t, b_d9C);
                //LABEL_20:
                //goto LABEL_21;
            }
            else
            {
                this->field_10[0].x = 0;
                this->field_10[0].y = 0;
                this->field_10[1].x = 0;
                this->field_10[1].y = 0;
            }
        }

        //LABEL_21:

        if (field_98_surface_type == 7 || field_98_surface_type == 8 || field_98_surface_type == 9)
        {
            //v22 = front_wheel_offset_;
            //v21 = -w_adjusted;
            Fix16_Point spawn_pos__;
            spawn_pos__.y = front_wheel_offset_;
            spawn_pos__.x = -w_adjusted;
            SpawnSkidSegment_55D200(3, spawn_pos__, 3);
            //v21 = w_adjusted;
            //v22 = front_wheel_offset_;
            Fix16_Point t;
            t.x = front_wheel_offset_;
            t.y = w_adjusted;
            SpawnSkidSegment_55D200(2, t, 3);
        }
        else if (this->field_84_front_skid < gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1 &&
                     (!this->field_AC_drive_wheels_locked_q || gCarInfo_48_6FE258->field_8_front_drive_bias <= kFP16Zero_6FE20C) ||
                 field_98_surface_type == 6)
        {
            this->field_10[3].x = 0;
            this->field_10[3].y = 0;
            this->field_10[2].x = 0;
            this->field_10[2].y = 0;
        }
        else
        {
            //v21 = -w_adjusted;
            //v22 = front_wheel_offset_;
            Fix16_Point spawn_pos___;
            spawn_pos___.y = front_wheel_offset_;
            spawn_pos___.x = -w_adjusted;
            SpawnSkidSegment_55D200(3, spawn_pos___, b_d9C);
            //v21 = w_adjusted;
            //v22 = front_wheel_offset_;
            Fix16_Point t;
            t.x = front_wheel_offset_;
            t.y = w_adjusted;
            SpawnSkidSegment_55D200(2, t, b_d9C);
        }
    }

    //LABEL_32:
    if (field_AC_drive_wheels_locked_q)
    {
        field_AC_drive_wheels_locked_q--;
    }
}

MATCH_FUNC(0x55e260)
void CarPhysics_B0::DoSkidmarks_55E260()
{
    if (!bSkip_skidmarks_67D585)
    {
        CarPhysics_B0::UpdateWheelSkidEffects_55DC00();
        Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
            pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
            pPhysics->field_84_front_skid = kFP16Zero_6FE20C;
            pPhysics->field_88_rear_skid = kFP16Zero_6FE20C;
            pPhysics->UpdateWheelSkidEffects_55DC00();
            CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
        }
    }
}

WIP_FUNC(0x55e470)
char_type CarPhysics_B0::StepMovementAndCollisions_55E470()
{
    WIP_IMPLEMENTED;

    s32 sprites_array_idx = 0;
    char_type ret_val = 0;
    s32 k2Counter = 2;

    if (dword_6FE198 < dword_6FE370)
    {
        return ret_val;
    }

    Sprite* sprites_array[4];
    Sprite** pSpriteIter = sprites_array;

    Fix16 a2;
    Fix16 a3;
    s32 i;

    while (1)
    {
        gRozza_679188.sub_4637B0();
        this->field_70 = 0; // fp 0
        save_physics_state_55A4B0();
        ApplyMovementStep_560F20(k_dword_6FE210);
        UpdateTrailerPhysicsFromTowingCar_559A40();
        if (SweepTestMovementForCollision_55C3B0(&a2, &a3))
        {
            break;
        }

    LABEL_17:
        dword_6FE198 = (dword_6FE198 * (k_dword_6FE210 - a3));
        *pSpriteIter = gRozza_679188.field_20_pSprite;
        ++sprites_array_idx;
        ++pSpriteIter;

        if ((gRozza_679188.IsCharB4_49EF20() || gRozza_679188.IsObj2C_477A10()) && k2Counter < 4)
        {
            ++k2Counter;
        }

        ProcessGroundCollisionAndEmitImpactParticles_55BFE0();

        if (sprites_array_idx >= k2Counter || dword_6FE198 < dword_6FE370)
        {
            return ret_val;
        }
    }

    ret_val = 1;

    if (!gRozza_679188.field_20_pSprite || (i = 0, sprites_array_idx <= 0))
    {
    LABEL_9:
        BinarySearchCollisionTime_55C560(a2, a3);
        if (field_5C_pCar->IsTrainModel_403BA0() && !field_40_linvel_1.IsNull_420360())
        {
            a3 = kFP16Zero_6FE20C;
        }
        ReplayAndDispatchCollision_55CBB0(a2, a3);
        goto LABEL_17;
    }

    Sprite** pIter = sprites_array;
    while (*pIter != gRozza_679188.field_20_pSprite)
    {
        ++i;
        ++pIter;
        if (i >= sprites_array_idx)
        {
            goto LABEL_9;
        }
    }

    restore_saved_physics_state_55A400();
    UpdateCarAndTrailerSpriteFromPhysics_5636C0();
    ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
    return 1;
}

WIP_FUNC(0x55eb80)
char_type CarPhysics_B0::CheckAndHandleCarAndTrailerCollisions_55EB80()
{
    WIP_IMPLEMENTED;

    gCar_6C_677930->field_68 = 0;

    const char_type bCollision = gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_5C_pCar->field_50_car_sprite);
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        char_type bTrailerCollision =
            gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(pTrailer->field_C_pCarOnTrailer->field_50_car_sprite);
        return bTrailerCollision | bCollision;
    }

    return bCollision;
}

WIP_FUNC(0x55ec30)
void CarPhysics_B0::ApplyForwardEngineForce_55EC30()
{
    WIP_IMPLEMENTED;

    Ang16 theta = this->field_58_theta;
    if (this->field_94_is_backward_gas_on)
    {
        theta += k_word_6FE12A;
    }

    Fix16 global_val;

    if (stru_6FE1F0.x <= kFP16Zero_6FE20C)
    {
        if (theta >= word_6FE00C)
        {
            if (theta >= k_word_6FE12A)
            {
                goto LABEL_13;
            }
            global_val = k_dword_6FDFA4;
        }
        else
        {
            global_val = -k_dword_6FDFA4;
        }
        goto LABEL_12;
    }

    if (theta <= word_6FE154)
    {
        if (theta <= k_word_6FE12A)
        {
            goto LABEL_13;
        }
        global_val = -k_dword_6FDFA4;
    LABEL_12:
        ApplyAngularImpulse_55F970(global_val);
        goto LABEL_13;
    }

    ApplyAngularImpulse_55F970(k_dword_6FDFA4);

LABEL_13:
    Fix16_Point v4 = stru_6FE1F0.NormalizeSafe_442AD0();
    //v9 = 0;
    Fix16_Point v5 = (v4 * stru_6FDF80);
    //LOBYTE(v9) = 1;
    ApplyForceScaledByMass_55F9A0(v5);

    this->field_AA_sbw = 1;
}

WIP_FUNC(0x55ef20)
void CarPhysics_B0::ApplyReverseEngineForce_55EF20()
{
    WIP_IMPLEMENTED;

    Ang16 theta = this->field_58_theta;
    if (field_94_is_backward_gas_on)
    {
        theta += k_word_6FE12A;
    }

    if (stru_6FE1F0.y > kFP16Zero_6FE20C)
    {
        if (theta > word_6FE00C && theta >= k_word_6FE12A)
        {
            if (theta < k_word_6FE12A)
            {
                ApplyAngularImpulse_55F970(k_dword_6FDFA4);
            }
            else if (theta > k_word_6FE12A && theta < word_6FE154)
            {
                ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
            }
        }
        else
        {
            if (theta > k_word_6FE12A && theta < word_6FE154)
            {
                ApplyAngularImpulse_55F970(k_dword_6FDFA4);
            }
        }
    }
    else
    {
        // Moving backwards or stationary
        if (theta < word_6FE00C)
        {
            ApplyAngularImpulse_55F970(k_dword_6FDFA4);
        }
        else if (theta > word_6FE154)
        {
            ApplyAngularImpulse_55F970(-k_dword_6FDFA4);
        }
    }

    ApplyForceScaledByMass_55F9A0(stru_6FE1F0.NormalizeSafe_442AD0() * stru_6FDF80);
    this->field_AA_sbw = 1;
}

WIP_FUNC(0x55f020)
void CarPhysics_B0::ApplyTurningForce_55F020()
{
    WIP_IMPLEMENTED;

    Fix16 v17 = dword_6FE358;

    Object_2C* pObj = gRozza_679188.field_20_pSprite->As2C_40FEC0();
    Fix16 v4;
    if (pObj && pObj->field_18_model == 166)
    {
        v4 = k_dword_6FDFA4;
        v17 = stru_6FDF80;
    }
    else
    {
        Ped* pDriver = this->field_5C_pCar->field_54_driver;
        if (pDriver && pDriver->field_15C_player)
        {
            v4 = dword_6FE078;
            v17 = dword_6FE390;
        }
        else
        {
            v4 = dword_6FE080;
            v17 = dword_6FE358;
        }
    }

    Fix16_Point v6 = (this->field_38_cp1 - stru_6FE1A0);
    Fix16 var_14 = v6.x;
    Fix16 v7 = v6.y;

    Ang16 v6_ = -this->field_58_theta;

    Fix16 v21 = var_14;
    Fix16 v19 = (Ang16::sine_40F500(v6_) * v7);
    Fix16 v10 = (var_14 * Ang16::cosine_40F520(v6_));
    var_14 = (v10 + v19);
    Fix16 v15 = (v7 * Ang16::cosine_40F520(v6_));
    Fix16 v11 = -v21;
    Fix16 v12 = (v11 * Ang16::sine_40F500(v6_));
    var_14 = (v12 + v15);

    if (var_14 >= kFP16Zero_6FE20C)
    {
        v4 = -v4;
    }

    ApplyAngularImpulse_55F970(v4);
    Fix16_Point v13 = stru_6FE1F0.NormalizeSafe_442AD0();
    Fix16_Point v14 = (v13 * v17);
    ApplyForceScaledByMass_55F9A0(v14);
}

MATCH_FUNC(0x55f240)
char_type CarPhysics_B0::ApplyMovementCommand_55F240()
{
    switch (gRozza_679188.field_0_type)
    {
        case 1:
            ApplyReverseEngineForce_55EF20();
            return 1;
        case 2:
            ApplyForwardEngineForce_55EC30();
            return 1;
        case 3:
            ApplyTurningForce_55F020();
            field_AA_sbw = 0;
            break;
    }
    return 0;
}

MATCH_FUNC(0x55f280)
char_type CarPhysics_B0::ProcessCollisionAndClampVelocity_55F280()
{
    char_type movementApplied = 0;

    Fix16_Point_POD cm1 = this->field_30_cm1;

    char_type stepResult;
    while (1)
    {
        dword_6FE198 = k_dword_6FE210;
        stepResult = StepMovementAndCollisions_55E470();
        const char_type prevStepResult = stepResult;

        if (movementApplied)
        {
            if (dword_6FE198 == k_dword_6FE210)
            {
                this->field_40_linvel_1.reset();
                this->field_74_ang_vel_rad = kFP16Zero_6FE20C;
            }
            break;
        }

        this->field_AA_sbw = 0;

        if (dword_6FE198 != k_dword_6FE210 || !stepResult)
        {
            break;
        }

        if (IsAccelerationOrReverseOn_55A180())
        {
            movementApplied = ApplyMovementCommand_55F240();

            if (movementApplied)
            {
                UpdateLinearAndAngularAccel_560EB0();
                IntegrateAndClampVelocities_5610B0();
                continue;
            }
        }

        stepResult = prevStepResult;
        break;
    }

    this->field_0_vel_read_only.x = this->field_30_cm1.x - cm1.x;
    this->field_0_vel_read_only.y = this->field_30_cm1.y - cm1.y;

    return stepResult;
}

MATCH_FUNC(0x55f330)
void CarPhysics_B0::StepPhysics_55F330()
{
    dword_6FE198 = k_dword_6FE210;
    save_physics_state_55A4B0();
    ApplyMovementStep_560F20(k_dword_6FE210);
}

MATCH_FUNC(0x55f360)
char_type CarPhysics_B0::CheckPendingCollision_55F360()
{
    if ((this->field_5C_pCar->field_78_flags & 0x2000) != 0)
    {
        gCar_6C_677930->field_60 = 1;
        if (gPurpleDoom_1_679208->CheckAndHandleAllCollisionsForSprite_477C30(field_5C_pCar->field_50_car_sprite, 2))
        {
            return 1;
        }
        this->field_5C_pCar->field_78_flags &= ~0x2000u;
    }
    return 0;
}

WIP_FUNC(0x55f740)
void CarPhysics_B0::ApplyForceWithTrailerRedirect_55F740(Fix16_Point* a2, Fix16_Point* a3)
{
    WIP_IMPLEMENTED;

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer && pTrailer->field_C_pCarOnTrailer == field_5C_pCar)
    {
        CarPhysics_B0* pB0 = pTrailer->field_8_truck_cab->field_58_physics;
        pB0->SetCurrentCarInfoAndModelPhysics_562EF0();
        pB0->ApplyForceAndIntegrate_55F7A0(a2, *a3);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        ApplyForceAndIntegrate_55F7A0(a2, *a3);
    }
}

MATCH_FUNC(0x55f7a0)
void CarPhysics_B0::ApplyForceAndIntegrate_55F7A0(Fix16_Point* a2, Fix16_Point a3)
{
    ApplyForceAtPoint_55F800(a2, &a3, 0);
    UpdateLinearAndAngularAccel_560EB0();
    IntegrateAndClampVelocities_5610B0();
}

// 9.6f 0x4A0850
WIP_FUNC(0x55f800)
void CarPhysics_B0::ApplyForceAtPoint_55F800(Fix16_Point* a2, Fix16_Point* a3, s32 bRotate)
{
    WIP_IMPLEMENTED;

    Fix16_Point_POD point(a2->x, a2->y);

    if (bRotate == 1)
    {
        point.RotateByAngle_40F6B0(field_58_theta);
        point = point + field_38_cp1;
    }

    field_48 = field_48 + *a3;

    Fix16 v11 = (point.y - field_30_cm1.y) * a3->x;
    Fix16 v9 = (point.x - field_30_cm1.x) * a3->y;
    Fix16 v10 = v9 - v11;
    field_7C = field_7C + v10;
}

MATCH_FUNC(0x55f930)
void CarPhysics_B0::AccumulateImpulse_55F930(Fix16_Point* a2)
{
    field_48 += (*a2 * gCarInfo_48_6FE258->field_4_mass);
}

MATCH_FUNC(0x55f970)
void CarPhysics_B0::ApplyAngularImpulse_55F970(Fix16 a2)
{
    this->field_7C -= (gCarInfo_2C_6FE0E4->field_0_moment_of_inertia * a2);
}

MATCH_FUNC(0x55f9a0)
void CarPhysics_B0::ApplyForceScaledByMass_55F9A0(Fix16_Point_POD& pForce)
{
    field_48 += pForce.Multiply_438FE0(CarPhysics_B0::CalculateMass_559FF0());
}

WIP_FUNC(0x55fa10)
void CarPhysics_B0::ApplyImpulseWithTrailerRedirect_55FA10(Fix16_Point* a2)
{
    WIP_IMPLEMENTED;

    Car_BC* pCar = this->field_5C_pCar;
    Trailer* pTrailer = pCar->field_64_pTrailer;
    if (pTrailer && pTrailer->field_C_pCarOnTrailer == pCar)
    {
        // We are on the trailer so apply impulse to the truck cab instead
        CarPhysics_B0* pPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pPhysics->AccumulateImpulse_55F930(a2);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        AccumulateImpulse_55F930(a2);
    }
}

WIP_FUNC(0x55fa60)
Fix16 CarPhysics_B0::ApplyImpactForcesAndDamage_55FA60(Fix16_Point* a2, Fix16_Point* a4, s32 base_dmg)
{
    WIP_IMPLEMENTED;

    Fix16 v17 = a4->GetLength_2();
    Fix16 v10 = CalculateMass_559FF0();

    if ((v17 / v10) <= dword_6FE37C)
    {
        return v17;
    }
    else
    {
        Fix16_Point a3;
        a3.x = a4->x;
        a3.y = a4->y;

        if ((field_5C_pCar->field_78_flags & 0x800) != 0)
        {
            if (!field_5C_pCar->field_54_driver || !field_5C_pCar->field_54_driver->field_15C_player)
            {
                Fix16 v17_ = field_0_vel_read_only.GetLength_41E260();
                if (v17_ <= dword_6FE1D4 || this->field_92_is_hand_brake_on)
                {
                    a3 = (*a4 / dword_6FE218);
                }
            }
        }

        field_5C_pCar->ApplyVisualDamage_43A9F0();

        if ((this->field_5C_pCar->field_78_flags & 2) != 0)
        {
            return v17;
        }
        else
        {
            ApplyForceWithTrailerRedirect_55F740(a2, &a3);
            s32 v14 = base_dmg + rng_dword_67AB34->field_0_rng;
            if (v14 > this->field_8_total_damage_q)
            {
                this->field_8_total_damage_q = v14;
            }

            if (!field_5C_pCar->field_54_driver || !field_5C_pCar->field_54_driver->field_15C_player)
            {
                this->field_92_is_hand_brake_on = 0;
            }
            return v17;
        }
    }
}

WIP_FUNC(0x55fc30)
void CarPhysics_B0::AccumulateImpulse_55FC30(Fix16_Point* arg0, s32 base_dmg)
{
    WIP_IMPLEMENTED;

    if (!field_5C_pCar->IsTrainModel_403BA0())
    {
        Fix16_Point a2;
        if (this->field_92_is_hand_brake_on)
        {
            a2 = (*arg0 / dword_6FE214);
        }
        else
        {
            a2 = *arg0;
        }

        ApplyImpulseWithTrailerRedirect_55FA10(&a2);

        u32 rng_damage = base_dmg + rng_dword_67AB34->field_0_rng;
        if (rng_damage > this->field_8_total_damage_q)
        {
            this->field_8_total_damage_q = rng_damage;
        }

        Ped* pDriver = this->field_5C_pCar->field_54_driver;
        if (!pDriver || !pDriver->field_15C_player)
        {
            this->field_92_is_hand_brake_on = 0;
        }
    }
}

STUB_FUNC(0x55fd00)
s32 CarPhysics_B0::HandleWorldCollision_55FD00(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ff20)
CarAI_78* CarPhysics_B0::HandleCarCollision_55FF20(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5606c0)
void CarPhysics_B0::HandleObjectCollision_5606C0(Object_2C* a2, char_type a3)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x560680)
EXPORT Fix16 __stdcall DotProduct_560680(const Fix16_Point& a2, const Fix16_Point& a3)
{
    WIP_IMPLEMENTED;

    return (a3.x * a2.x) + (a3.y * a2.y);
}

// TODO: Probably move
WIP_FUNC(0x55F3B0)
EXPORT Fix16_Point __stdcall ComputeLineLineIntersection_55F3B0(Fix16 a2,
                                                                Fix16 a3,
                                                                Fix16_Point* a4,
                                                                Fix16_Point* a5,
                                                                Fix16_Point* a6,
                                                                Fix16_Point* a7,
                                                                Fix16_Point* a8,
                                                                Fix16 a9,
                                                                Fix16 a10,
                                                                Fix16 a11)
{
    WIP_IMPLEMENTED;

    //Fix16 v36 = 0;

    //    v47 = 2;
    if (a4->IsNull_420360() || a5->IsNull_420360())
    {
        return stru_6FE300;
    }

    Fix16 _a5 = ((k_dword_6FE210) / a2);
    Fix16_Point v14 = (*a6 - *a7);
    //      LOBYTE(v47) = 3;
    Fix16_Point v44 = v14.Rotate90CCW_5605E0();
    //  LOBYTE(v47) = 2;
    Fix16_Point v43 = a5->NormalizeSafe_442AD0();
    Fix16 v31 = (a4->y * v43.y);
    Fix16 v15 = (a4->x * v43.x);

    Fix16 _a4 = (v15 + v31);

    Fix16 v35 = (-(k_dword_6FE210 + a11) * _a4);
    if (a3 == dword_6FDF1C)
    {
        Fix16 v32 = (v44.y * v43.y);
        Fix16 v16 = (v44.x * v43.x);
        Fix16 __a4 = (v16 + v32);
        Fix16 _a7 = (__a4 * __a4);
        Fix16 v17 = (_a7) / a9;

        Fix16 v33 = (v43.y * v43.y);
        Fix16 v18 = (v43.x * v43.x);
        Fix16 _a6 = (v18 + v33);
        a2 = v17 + ((_a5 * _a6));
    }
    else
    {
        Fix16 _a6 = ((k_dword_6FE210) / a3);
        Fix16_Point v19 = (*a6 - *a8);
        //LOBYTE(v47) = 4;
        Fix16_Point v19r = v19.Rotate90CCW_5605E0();

        //LOBYTE(v47) = 6;
        Fix16 v20 = DotProduct_560680(v19r, v43);
        Fix16 __a4 = (v20 * v20);

        Fix16 v21 = DotProduct_560680(v44, v43);
        Fix16 _a7 = (v21 * v21);

        Fix16 v34 = (__a4 / a10);
        Fix16 v30 = (_a7 / a9);
        Fix16 v29 = (_a5 + _a6);
        Fix16 v22 = DotProduct_560680(v43, v43);
        Fix16 v23 = (v22 * v29);
        Fix16 v24 = (v23 + v30);
        a2 = (v24 + v34);
        //LOBYTE(v47) = 2;
    }

    Fix16_Point v25 = (v43 * (v35 / a2));
    return v25;
    //a1->x = v25.x;
    //a1->y = v25.y;
    //return *a1;
}

WIP_FUNC(0x560b40)
void CarPhysics_B0::ProcessPedImpact_560B40(Char_B4* pCharB4, u8 hitType)
{
    WIP_IMPLEMENTED;

    Sprite* pSprite = pCharB4->field_80_sprite_ptr;

    Fix16_Point sprite_xy;
    sprite_xy.x = pSprite->field_14_xy.x;
    sprite_xy.y = pSprite->field_14_xy.y;

    Fix16_Point combinedCentreOfmass = ComputeCombinedCenterOfMass_559EC0();
    Fix16_Point relativePointVel = ComputeRelativePointVelocity_561130(&stru_6FE1A0);

    stru_6FE1F0 = combinedCentreOfmass - stru_6FE1A0;

    Fix16 effectiveMomentOfInertia = GetEffectiveMomentOfInertia_55A050();
    Fix16 carMass = CalculateMass_559FF0();

    Fix16_Point pIntersection = ComputeLineLineIntersection_55F3B0(carMass,
                                                                   dword_6FE2F8,
                                                                   &relativePointVel,
                                                                   &stru_6FE1F0,
                                                                   &stru_6FE1A0,
                                                                   &combinedCentreOfmass,
                                                                   &sprite_xy,
                                                                   effectiveMomentOfInertia,
                                                                   dword_6FE070,
                                                                   dword_6FE3DC);

    Fix16_Point intersect_abs;
    intersect_abs.x = pIntersection.x;
    intersect_abs.y = pIntersection.y;

    u8 bUnknown =
        field_98_surface_type == 6 && pCharB4->field_80_sprite_ptr->field_1C_zpos != field_5C_pCar->field_50_car_sprite->field_1C_zpos ||
        hitType == 0;

    dword_6FE33C = pIntersection.GetLength_2();

    Car_BC* pCar = this->field_5C_pCar;

    Fix16_Point v15 = -intersect_abs;
    Fix16_Point v16 = v15 / dword_6FE2F8;

    Ped* pCarDriver = field_5C_pCar->field_54_driver;
    if (pCarDriver)
    {
        pCharB4->field_7C_pPed->field_204_killer_id = pCarDriver->field_200_id;
        pCharB4->field_7C_pPed->field_264 = 50;

        Ped* pPed = pCharB4->field_7C_pPed;
        if (pPed->field_140 == this->field_5C_pCar)
        {
            pPed->field_290 = 3;
        }
        else
        {
            pPed->field_290 = 1;
        }
    }
    else
    {
        Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            // is_on_trailer_421720 ?
            Car_BC* pCarOnTrailer = pTrailer->field_C_pCarOnTrailer;
            if (pCarOnTrailer)
            {
                if (pCarOnTrailer == field_5C_pCar)
                {
                    Car_BC* pTruckCab = pTrailer->field_8_truck_cab;
                    if (pTruckCab)
                    {
                        pCar = pTruckCab;
                        Ped* pTruckCabDriver = pTruckCab->field_54_driver;
                        if (pTruckCabDriver)
                        {
                            pCharB4->field_7C_pPed->field_204_killer_id = pTruckCabDriver->field_200_id;
                            pCharB4->field_7C_pPed->field_264 = 50;

                            Ped* pPed = pCharB4->field_7C_pPed;
                            if (pPed->field_140 == this->field_5C_pCar->field_64_pTrailer->field_8_truck_cab)
                            {
                                pPed->field_290 = 3;
                            }
                            else
                            {
                                pPed->field_290 = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    pCharB4->HandleCarImpact_5538A0(pCar, bUnknown, v16.x, v16.y);
}

MATCH_FUNC(0x560eb0)
void CarPhysics_B0::UpdateLinearAndAngularAccel_560EB0()
{
    field_50 = field_48.Divide_442CB0(CarPhysics_B0::CalculateMass_559FF0());
    field_80 = -field_7C / CarPhysics_B0::GetEffectiveMomentOfInertia_55A050();
}

WIP_FUNC(0x560f20)
void CarPhysics_B0::ApplyMovementStep_560F20(Fix16 a2)
{
    WIP_IMPLEMENTED;

    Fix16 v3 = (dword_6FE198 * a2);

    if (v3 != kFP16Zero_6FE20C)
    {
        Ang16 tmp = Ang16::Fix16_To_Ang16_40F540(v3 * field_74_ang_vel_rad);
        this->field_58_theta += tmp;

        this->field_30_cm1 = field_30_cm1 + (field_40_linvel_1 * v3);

        UpdateCp1FromCm1_563280();

        this->field_6C_cp3 += (g_f70_6FDFE0 * a2);

        UpdateTrailerAlignment_559B40();

        if (field_5C_pCar->IsTrainModel_403BA0())
        {
            UpdateZPosition_55B7B0(a2);
            UpdateCarAndTrailerSpriteFromPhysics_5636C0();
            return;
        }
        SyncZWithTrailer_55B3F0(a2);
    }

    UpdateCarAndTrailerSpriteFromPhysics_5636C0();
}

MATCH_FUNC(0x5610b0)
void CarPhysics_B0::IntegrateAndClampVelocities_5610B0()
{
    // Integrate linear and angular velocity
    this->field_40_linvel_1 += this->field_50;
    this->field_74_ang_vel_rad += this->field_80;

    ResetForceAccumulators_55A840();

    field_40_linvel_1.ApplyDeadZone_49E3C0();
    field_74_ang_vel_rad = field_74_ang_vel_rad.ApplyDeadZone_482730(field_74_ang_vel_rad);
}

WIP_FUNC(0x561130)
Fix16_Point CarPhysics_B0::ComputeRelativePointVelocity_561130(Fix16_Point* a3)
{
    WIP_IMPLEMENTED;

    Fix16_Point v11 = (*a3 - field_38_cp1);
    v11.RotateByAngle_40F6B0(-field_58_theta);
    v11 = v11 - gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;

    Ang16 v6 = Ang16::Fix16_To_Ang16_40F540(field_74_ang_vel_rad) + field_58_theta;
    v11.RotateByAngle_40F6B0(v6);

    v11 = v11 + (field_30_cm1 + field_40_linvel_1);
    return v11 - *a3;
}

MATCH_FUNC(0x561350)
Fix16_Point CarPhysics_B0::GetPointVelocity_561350(Fix16_Point* a3)
{
    SetCarInfoGlobal_562ED0();
    return ComputeRelativePointVelocity_561130(a3);
}

WIP_FUNC(0x561380)
Fix16_Point CarPhysics_B0::ComputePointVelocity_561380(Fix16_Point& point)
{
    WIP_IMPLEMENTED;

    Fix16_Point v4 = (point - gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset);

    Fix16_Point v12;
    v12.x = v4.x;
    v12.y = v4.y;

    v12.RotateByAngle_40F6B0(field_58_theta);
    v12 = v12 + field_30_cm1;

    Fix16_Point v13;
    v13.x = v4.x;
    v13.y = v4.y;

    Ang16 v7 = Ang16::Fix16_To_Ang16_40F540(field_74_ang_vel_rad);
    v13.RotateByAngle_40F6B0(field_58_theta + v7);

    Fix16_Point v9 = field_30_cm1 + field_40_linvel_1;
    v13 = v13 + v9;
    return v13 - v12;
}

// 9.6f 0x4A0D40
WIP_FUNC(0x5615d0)
Fix16 CarPhysics_B0::ApplyDriveForce_5615D0(Fix16_Point& a3, Ang16 a4, Fix16_Point& a5, Fix16 a6)
{
    WIP_IMPLEMENTED;

    Fix16_Point a3_;

    if (field_95)
    {
        a3_.SetXY_432860(kFP16Zero_6FE20C, kFP16Zero_6FE20C);
    }
    else
    {
        Fix16_Point v10 = ComputePointVelocity_561380(a3);

        Fix16_Point v40;
        v40.x = v10.x;
        v40.y = v10.y;
        v40.RotateByAngle_40F6B0(-a4); // ang16::negate_401C80

        a3_.x = (v40.x * -a5.x);
        a3_.y = (v40.y * -a5.y);
    }

    Fix16 y_abs = Fix16::Abs(a3_.y);
    Fix16 x_abs = Fix16::Abs(a3_.x);

    if (this->field_8C_state == 2 && this->field_5C_pCar->field_84_car_info_idx != car_model_enum::TANK)
    {
        if (y_abs > dword_6FE15C || Fix16::Abs(a6) < dword_6FE3B4)
        {
            if (this->field_92_is_hand_brake_on)
            {
                char_type tmp = this->field_AC_drive_wheels_locked_q;
                if ((tmp && y_abs > kFP16Zero_6FE20C || y_abs >= dword_6FE2F4) && (u8)tmp < 2u)
                {
                    this->field_AC_drive_wheels_locked_q = 2;
                }
            }
        }
        else
        {
            Fix16 v19 = Fix16::Abs(a6);
            this->field_AC_drive_wheels_locked_q = (((v19) / dword_6FE3B4) * 8).ToInt();
        }
    }

    a3_.y += a6;
    a3_.RotateByAngle_40F6B0(a4);
    ApplyForceAtPoint_55F800(&a3, &a3_, 1);

    return x_abs;
}

MATCH_FUNC(0x561940)
bool CarPhysics_B0::get_revs_561940()
{
    return gCarInfo_48_6FE258->field_1_turbo && this->field_60_gas_pedal >= k_dword_6FE1B8;
}

// https://decomp.me/scratch/0MzjM
// 9.6f 0x4A0F30
WIP_FUNC(0x561970)
Fix16 CarPhysics_B0::ComputeEngineTorque_561970()
{
    WIP_IMPLEMENTED;

    if (this->field_5C_pCar->field_9C == 3 && field_98_surface_type != 7 && field_98_surface_type != 8)
    {
        if (this->field_8C_state == 2)
        {
            // TODO: SquareRoot getting inlined when we don't want it to be
            Fix16 vel_len = field_40_linvel_1.GetLength_2();

            if (field_94_is_backward_gas_on)
            {
                if (vel_len != kFP16Zero_6FE20C || !this->field_92_is_hand_brake_on)
                {
                    return (-(gCarInfo_2C_6FE0E4->field_14_half_thrust + ComputeTorqueUnknown_49E8E0()) *
                            gCarInfo_48_6FE258->field_34_gear1_multiplier);
                }
            }
            else if (field_93_is_forward_gas_on)
            {
                if (vel_len != kFP16Zero_6FE20C || !this->field_92_is_hand_brake_on)
                {
                    if (vel_len <= gCarInfo_48_6FE258->field_44_gear3_speed)
                    {
                        if (vel_len <= gCarInfo_48_6FE258->field_40_gear2_speed)
                        {
                            // Gear 1
                            return ((ComputeTorqueUnknown_49E8E0()) * gCarInfo_48_6FE258->field_34_gear1_multiplier);
                        }
                        else
                        {
                            // Gear 2
                            return ((inline_ComputeTorqueFromThrottle_561DD0()) * gCarInfo_48_6FE258->field_38_gear2_multiplier);
                        }
                    }
                    else
                    {
                        // Gear 3
                        return ((inline_ComputeTorqueFromThrottle_561DD0()) * gCarInfo_48_6FE258->field_3C_gear3_multiplier);
                    }
                }
            }
        }
        else
        {
            if (this->field_93_is_forward_gas_on)
            {
                return ComputeTorqueFromThrottle_561DD0();
            }

            if (this->field_94_is_backward_gas_on)
            {
                return -ComputeTorqueFromThrottle_561DD0();
            }
        }
    }
    return kFP16Zero_6FE20C;
}

MATCH_FUNC(0x561dd0)
Fix16 CarPhysics_B0::ComputeTorqueFromThrottle_561DD0()
{
    if (get_revs_561940() != 0)
    {
        return gCarInfo_2C_6FE0E4->field_14_half_thrust +
            ((field_60_gas_pedal * ((dword_6FE348 * gCarInfo_2C_6FE0E4->field_18_fith_thrust)))) * 2;
    }
    else
    {
        return gCarInfo_2C_6FE0E4->field_14_half_thrust +
            ((field_60_gas_pedal * ((dword_6FE348 * gCarInfo_2C_6FE0E4->field_18_fith_thrust))));
    }
}

// https://decomp.me/scratch/46zAM
WIP_FUNC(0x561e50)
Fix16 CarPhysics_B0::CalculateFrontWheelForce_561E50()
{
    WIP_IMPLEMENTED;
    Fix16_Point point(Fix16(0), gCarInfo_2C_6FE0E4->field_4_front_wheel_offset);

    if (CarPhysics_B0::IsInAir_55A0B0())
    {
        return kFP16Zero_6FE20C;
    }
    else
    {
        Fix16 v6;
        Fix16 lodword_v5;
        Fix16 hidword_v5 = CarPhysics_B0::ComputeEngineTorque_561970() * gCarInfo_48_6FE258->field_8_front_drive_bias;

        if (field_AD_turn_direction)
        {
            v6 = k_dword_6FE210 + gCarInfo_48_6FE258->field_14_turn_in;
        }
        else
        {
            v6 = k_dword_6FE210;
        }

        Fix16 pointing_ang_rad;
        Fix16 v9;
        Fix16 v10;

        switch (field_A0)
        {
            case 0:
                v9 = v6 * dword_6FE3D0;
                v10 = dword_6FE3D4;
                lodword_v5 = dword_6FE0D8 * dword_6FE320;
                pointing_ang_rad = this->field_78_pointing_ang_rad;
                break;
            case 1:
            case 2:
                lodword_v5 = kFP16Zero_6FE20C;
                if (hidword_v5 == kFP16Zero_6FE20C)
                {
                    pointing_ang_rad = this->field_78_pointing_ang_rad;
                    v9 = kFP16Zero_6FE20C;
                    v10 = kFP16Zero_6FE20C;
                }
                else
                {
                    v10 = dword_6FE3D4;
                    v9 = v6 * dword_6FE3D0;
                    if (field_A0 == 1)
                    {
                        lodword_v5 = kFP16Zero_6FE20C;
                        pointing_ang_rad = this->field_78_pointing_ang_rad + dword_6FE3C4 * 30;
                    }
                    else
                    {
                        pointing_ang_rad = this->field_78_pointing_ang_rad - dword_6FE3C4 * 30;
                        lodword_v5 = kFP16Zero_6FE20C;
                    }
                }
                break;
            default:
                lodword_v5 = v6;
                pointing_ang_rad = field_78_pointing_ang_rad;
                break;
        }

        Fix16_Point point2(v9, v10 + lodword_v5);

        Ang16 rotation = Ang16::Fix16_To_Ang16_40F540(pointing_ang_rad);

        Ang16 final_rotation = rotation + field_58_theta;

        return CarPhysics_B0::ApplyDriveForce_5615D0(point, final_rotation, point2, hidword_v5);
    }
}

WIP_FUNC(0x5620d0)
Fix16 CarPhysics_B0::CalculateRearWheelForce_5620D0()
{
    WIP_IMPLEMENTED;

    Fix16_Point wheel_point;
    wheel_point.x = 0;
    wheel_point.y = gCarInfo_2C_6FE0E4->field_8_rear_wheel_offset;

    if (IsInAir_55A0B0())
    {
        return kFP16Zero_6FE20C;
    }

    Fix16 v5 = gCarInfo_2C_6FE0E4->field_20_front_drive_bias * ComputeEngineTorque_561970();
    Fix16 v7 = k_dword_6FE210;

    if (field_AD_turn_direction)
    {
        v7 = k_dword_6FE210 - gCarInfo_48_6FE258->field_14_turn_in;
    }

    Fix16 pointing_ang_rad;
    Fix16 brake_force1;
    Fix16 brake_force2;
    Fix16 brake_force3;
    Fix16 new_x;

    if (field_A0)
    {
        if (field_A0 <= 0 || field_A0 > 2)
        {
            // wtf ??
            //new_x = pOut;
            //brake_force2 = pOut;
            //brake_force3 = pOut;
            //pointing_ang_rad = pOut;
        }
        else
        {
            brake_force2 = kFP16Zero_6FE20C;
            if (v5 == kFP16Zero_6FE20C)
            {
                pointing_ang_rad = this->field_78_pointing_ang_rad;
                new_x = kFP16Zero_6FE20C;
                brake_force1 = kFP16Zero_6FE20C;
                brake_force3 = kFP16Zero_6FE20C;
                this->field_A8_hand_brake_force = 0;
            }
            else
            {
                new_x = (v7 * dword_6FE3D0);
                brake_force1 = dword_6FE3D4;
                Fix16 v13 = dword_6FE3C4 * 3;
                if (field_A0 == 1)
                {
                    pointing_ang_rad = (this->field_78_pointing_ang_rad - dword_6FE3C4 * 30);
                    brake_force3 = kFP16Zero_6FE20C;
                    this->field_A8_hand_brake_force = 0;
                }
                else // 2 ?
                {
                    pointing_ang_rad = this->field_78_pointing_ang_rad;
                    this->field_A8_hand_brake_force = 0;
                    pointing_ang_rad = (pointing_ang_rad + v13 * 10);
                    brake_force3 = brake_force2;
                }
            }
        }
    }
    else
    {
        brake_force1 = dword_6FE3D4;
        brake_force2 = (dword_6FE0D8 * dword_6FE2B0);

        if (field_92_is_hand_brake_on)
        {
            if (field_A8_hand_brake_force != (char)0x80)
            {
                field_A8_hand_brake_force++;
            }
            new_x = (gCarInfo_48_6FE258->field_20_handbrake_slide_value * (v7 * dword_6FE3D0));
            pointing_ang_rad = this->field_78_pointing_ang_rad;
            brake_force3 = (gCarInfo_48_6FE258->field_10_brake_friction * this->field_A8_hand_brake_force) / (128);
        }
        else
        {
            this->field_A8_hand_brake_force = 0;
            Fix16 v16 = (v7 * dword_6FE3D0);
            pointing_ang_rad = this->field_78_pointing_ang_rad;
            new_x = v16;
            brake_force3 = kFP16Zero_6FE20C;
        }
    }

    Fix16_Point v25;
    v25.x = new_x;
    v25.y = brake_force1 + brake_force2 + brake_force3;
    v25.x *= gCarInfo_48_6FE258->field_1C_rear_end_stability;
    v25.y *= gCarInfo_48_6FE258->field_1C_rear_end_stability;

    Fix16 vec_len = field_40_linvel_1.GetLength_2();
    Fix16 v21 = (dword_6FE228 * dword_6FE340);

    Ang16 t = Ang16::Fix16_To_Ang16_40F540(((dword_6FE228 - vec_len) / v21) * pointing_ang_rad);
    Ang16 tt = this->field_58_theta - t;

    return ApplyDriveForce_5615D0(wheel_point, tt, v25, v5);
}

WIP_FUNC(0x562480)
void CarPhysics_B0::ApplyThrottleInput_562480()
{
    WIP_IMPLEMENTED;

    if (this->field_93_is_forward_gas_on)
    {
        Fix16 gasT1 = k_dword_6FE3A0 + this->field_60_gas_pedal;
        this->field_60_gas_pedal = gasT1;
        if (gasT1 > k_dword_6FDEFC)
        {
            this->field_60_gas_pedal = k_dword_6FDEFC;
        }
    }
    else if (this->field_94_is_backward_gas_on)
    {
        this->field_60_gas_pedal += k_dword_6FE3A0;
        if (this->field_60_gas_pedal > k_dword_6FDF88)
        {
            this->field_60_gas_pedal = k_dword_6FDF88;
        }
    }
    else
    {
        Fix16 gasT2 = this->field_60_gas_pedal - k_dword_6FE364;
        this->field_60_gas_pedal = gasT2;
        if (gasT2 < k_dword_6FE290)
        {
            this->field_60_gas_pedal = k_dword_6FE290;
        }
    }
}

MATCH_FUNC(0x5624f0)
void CarPhysics_B0::ApplyBrakePhysics_5624F0()
{
    if (!field_91_is_foot_brake_on || field_98_surface_type == 7 || field_98_surface_type == 8)
    {
        field_64 = dword_6FE200;
        dword_6FE0D8 = kFP16Zero_6FE20C; // final value used in skid calcs
    }
    else
    {
        field_64 += dword_6FE2AC;
        if (field_64 > dword_6FE1C0)
        {
            field_64 = dword_6FE1C0; // 0x4000 fp16
        }
        dword_6FE0D8 = field_64 * gCarInfo_48_6FE258->field_10_brake_friction;
    }
}

// https://decomp.me/scratch/f2UpJ
WIP_FUNC(0x562560)
void CarPhysics_B0::UpdateSteeringAngle_562560()
{
    WIP_IMPLEMENTED;
    if (field_5C_pCar->field_7C_uni_num != 2)
    {
        field_78_pointing_ang_rad = CarPhysics_B0::GetTrailerAwareTurnRatio_55A100() * field_AD_turn_direction;
    }
    else
    {
        Fix16 v6 = dword_6FE228 - field_40_linvel_1.GetLength_2();
        if (v6 < dword_6FE374)
        {
            v6 = dword_6FE374;
        }
        field_78_pointing_ang_rad =
            CarPhysics_B0::GetTrailerAwareTurnRatio_55A100() * field_AD_turn_direction * (v6 / (dword_6FE228 * dword_6FE104));
    }
}

MATCH_FUNC(0x5626a0)
bool CarPhysics_B0::IsGasPedalPressedEnough_5626A0()
{
    Fix16 t = MinGasPedalPressure_5626C0();
    return field_60_gas_pedal >= t;
}

MATCH_FUNC(0x5626c0)
Fix16 CarPhysics_B0::MinGasPedalPressure_5626C0()
{
    if (field_5C_pCar->field_7C_uni_num == 2) // ??
    {
        return dword_6FDFB0;
    }
    else
    {
        return kFP16Zero_6FE20C;
    }
}

MATCH_FUNC(0x5626f0)
void CarPhysics_B0::ApplyArrowSteerAssist_5626F0()
{
    Fix16 theta_fp = Ang16::Ang16_to_Fix16(field_58_theta);
    dword_6FE0B0 = kFP16Zero_6FE20C;
    CarAI_78* pAi = this->field_5C_pCar->field_5C;
    if ((!pAi || (pAi->field_24_flags & 0x2000) != 0) && this->field_78_pointing_ang_rad == kFP16Zero_6FE20C)
    {
        if (IsGasPedalPressedEnough_5626A0())
        {
            if (IsVelocityAlignedWithHeading_40F840() && !this->field_40_linvel_1.IsNull() && !this->field_A0)
            {
                gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(this->field_38_cp1.x.ToInt(),
                                                                             this->field_38_cp1.y.ToInt(),
                                                                             (this->field_6C_cp3 - k_dword_6FE210).ToInt());
                if (pBlock)
                {
                    if ((pBlock->field_B_slope_type & 3) == 1)
                    {
                        if ((pBlock->field_A_arrows & 0x33) != 0)
                        {
                            if (theta_fp > dword_6FE10C - k_dword_6FE134 && theta_fp < k_dword_6FE134 + dword_6FE10C)
                            {
                                dword_6FE0B0 = dword_6FE10C - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > dword_6FE278 - k_dword_6FE134 && theta_fp < dword_6FE278 + k_dword_6FE134)
                                {
                                    dword_6FE0B0 = dword_6FE278 - theta_fp;
                                }
                            }
                        }
                        else if ((pBlock->field_A_arrows & 0xCC) != 0)
                        {
                            if (theta_fp > k_dword_6FE260 - k_dword_6FE134 && theta_fp < k_dword_6FE260 + k_dword_6FE134)
                            {
                                dword_6FE0B0 = k_dword_6FE260 - theta_fp;
                            }
                            else
                            {
                                if (theta_fp > k_dword_6FE314 - k_dword_6FE134)
                                {

                                    dword_6FE0B0 = k_dword_6FE314 - theta_fp;
                                }
                                else
                                {
                                    if (theta_fp < k_dword_6FE134)
                                    {
                                        dword_6FE0B0 = -theta_fp;
                                    }
                                }
                            }
                        }

                        if (dword_6FE0B0 != kFP16Zero_6FE20C)
                        {
                            theta_fp = field_5C_pCar->sub_440510();
                            if (dword_6FE0B0 > kFP16Zero_6FE20C)
                            {
                                if (dword_6FE0B0 > theta_fp)
                                {
                                    dword_6FE0B0 = theta_fp;
                                }
                            }
                            else
                            {
                                if (dword_6FE0B0 < -theta_fp)
                                {
                                    dword_6FE0B0 = -theta_fp;
                                }
                            }

                            if (bDo_show_instruments_67D64C)
                            {
                                Ped* pDriver = this->field_5C_pCar->field_54_driver;
                                if (pDriver)
                                {
                                    Player* pPlayer = pDriver->field_15C_player;
                                    if (pPlayer)
                                    {
                                        if (pPlayer->IsUser_41DC70())
                                        {
                                            gHud_2B00_706620->field_650.sub_5D1F50(L"snap", 0, 64, word_706600, 1);
                                        }
                                    }
                                }
                            }

                            this->field_74_ang_vel_rad += dword_6FE0B0;
                        }
                    }
                }
            }
        }
    }
}

// https://decomp.me/scratch/vdIqi
WIP_FUNC(0x562910)
void CarPhysics_B0::StabilizeVelocityAtSpeed_562910()
{
    WIP_IMPLEMENTED;
    if (CarPhysics_B0::IsInAir_55A0B0())
    {
        if (Fix16::Abs(field_40_linvel_1.x) <= dword_6FDFDC && Fix16::Abs(field_40_linvel_1.y) <= dword_6FDFDC)
        {
            field_40_linvel_1.MultiplyByFix16_49E3A0(dword_6FE334);
        }
        else
        {
            field_40_linvel_1.RotateByAngle_40F6B0(-field_58_theta);
            field_40_linvel_1.x = field_40_linvel_1.x * dword_6FE334;
            if (!field_5C_pCar->field_64_pTrailer)
            {
                field_40_linvel_1.y = field_40_linvel_1.y * dword_6FE330;
            }
            else
            {
                field_40_linvel_1.y = field_40_linvel_1.y * dword_6FE240;
            }
            field_40_linvel_1.RotateByAngle_40F6B0(field_58_theta);
            field_74_ang_vel_rad = field_74_ang_vel_rad * dword_6FDF18;
        }
    }
    else
    {
        if (Fix16::Abs(field_40_linvel_1.x) <= dword_6FDFDC && Fix16::Abs(field_40_linvel_1.y) <= dword_6FDFDC)
        {
            field_40_linvel_1.MultiplyByFix16_49E3A0(dword_6FE100);
        }
        else
        {
            field_40_linvel_1.RotateByAngle_40F6B0(-field_58_theta);
            field_40_linvel_1.x = field_40_linvel_1.x * dword_6FE100;
            if (!field_5C_pCar->field_64_pTrailer)
            {
                field_40_linvel_1.y = field_40_linvel_1.y * dword_6FE0FC;
            }
            else
            {
                field_40_linvel_1.y = field_40_linvel_1.y * dword_6FDFBC;
            }
            field_40_linvel_1.RotateByAngle_40F6B0(field_58_theta);
            field_74_ang_vel_rad = field_74_ang_vel_rad * dword_6FE318;
        }
    }
}

// TODO: Actually Fix16_Point method its RotateByAngle_40F6B0
MATCH_FUNC(0x562c20)
void CarPhysics_B0::RotateVelocity_562C20(const Ang16& angle)
{
    const Fix16 sin = Ang16::sine_40F500(angle);
    const Fix16 cos = Ang16::cosine_40F520(angle);

    const Fix16 x_old = field_0_vel_read_only.x;

    field_0_vel_read_only.x = (sin * field_0_vel_read_only.y) + (cos * field_0_vel_read_only.x);
    field_0_vel_read_only.y = (cos * field_0_vel_read_only.y) + ((-x_old) * sin);
}

// https://decomp.me/scratch/0X4pK
// 9.6f 0x4A1B20
WIP_FUNC(0x562d00)
void CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00()
{
    WIP_IMPLEMENTED;

    Fix16_Point polar;
    if (!IsInAir_55A0B0())
    {
        Fix16 radius;
        if (IsVelocityAlignedWithHeading_40F840())
        {
            radius = gCarInfo_48_6FE258->field_28_max_speed;
        }
        else
        {
            radius = gCarInfo_48_6FE258->field_40_gear2_speed;
        }

        if (field_40_linvel_1.GetLength_2() > radius)
        {
            polar.FromPolar_41E210(radius, field_40_linvel_1.atan2_40F790());
            field_40_linvel_1.ClampTowardsZero_49E480(polar);
        }
    }
}

MATCH_FUNC(0x562eb0)
void CarPhysics_B0::SetModelPhysicsGlobal_562EB0()
{
    gCarInfo_48_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_5C_pCar->GetCarModelForPhysics_43A850());
}

MATCH_FUNC(0x562ed0)
void CarPhysics_B0::SetCarInfoGlobal_562ED0()
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());
    gCarInfo_2C_6FE0E4 = pInfo;
}

MATCH_FUNC(0x562ef0)
void CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0()
{
    u8 info_idx_remapped = field_5C_pCar->GetCarModelForPhysics_43A850();
    gCarInfo_2C_6FE0E4 = gCarInfo_808_678098->GetInfoAtIdx_454840(info_idx_remapped);
    gCarInfo_48_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(info_idx_remapped);
}

MATCH_FUNC(0x562f30)
void CarPhysics_B0::ApplyInputsAndIntegratePhysics_562F30()
{
    dword_6FE348 = field_5C_pCar->GetDamageFactorOnSpeed_439EE0();
    ApplyThrottleInput_562480();
    ApplyBrakePhysics_5624F0();
    field_84_front_skid = CalculateFrontWheelForce_561E50();
    field_88_rear_skid = CalculateRearWheelForce_5620D0();
    HandleGravityOnSlope_55AA00();
    UpdateLinearAndAngularAccel_560EB0();
    IntegrateAndClampVelocities_5610B0();
}

MATCH_FUNC(0x562fa0)
char_type CarPhysics_B0::UpdateLastMovementTimer_562FA0()
{
    if (IsNotMoving_5599D0())
    {
        if (field_90_timer_since_last_move < 255)
        {
            field_90_timer_since_last_move++;
        }
        if (field_90_timer_since_last_move >= 20u)
        {
            return true;
        }
    }
    else
    {
        field_90_timer_since_last_move = 0;
    }
    return false;
}

MATCH_FUNC(0x562fe0)
bool CarPhysics_B0::ProcessCarPhysicsStateMachine_562FE0()
{
    char carModel; // al
    char flag; // bl

    SetCurrentCarInfoAndModelPhysics_562EF0();
    carModel = field_5C_pCar->GetCarModelForPhysics_43A850();
    if (carModel != this->field_A9_car_model)
    {
        this->field_A9_car_model = carModel;
        UpdateReferencePoint_563460();
    }

    this->field_84_front_skid = kFP16Zero_6FE20C;
    this->field_88_rear_skid = kFP16Zero_6FE20C;

    switch (field_8C_state)
    {
        case 1:
            ResetPoint6FDF50();
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyForcedSteering_559DD0();
            EnforceTrailerControlLimits_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            Field40Add();
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            Field40Subtract();
            Field74Subtract();
            break;
        case 2:
            ResetPoint6FDF50();
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            EnforceTrailerControlLimits_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            Field40Add();
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            Field40Subtract();
            Field74Subtract();
            break;
        case 3:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyInputsAndIntegratePhysics_562F30();
            ScarePedsOnDrivingFast_559C30();
            CheckPendingCollision_55F360();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 0:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ScarePedsOnDrivingFast_559C30();
            flag = ProcessCollisionAndClampVelocity_55F280() | flag;
            flag = CheckPendingCollision_55F360() | flag;
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        case 4:
            flag = CheckAndHandleCarAndTrailerCollisions_55EB80();
            StepPhysics_55F330();
            ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
            DoSkidmarks_55E260();
            if (this->field_5C_pCar->is_driven_by_player())
            {
                SetField8C_to_2();
            }
            else
            {
                SetField8C_to_1();
            }
            break;
        default:
            //bCol4 = v14;
            break;
    }

    return (UpdateLastMovementTimer_562FA0() && !flag && field_98_surface_type != 7 && field_98_surface_type != 8 &&
            field_98_surface_type != 6);
}

// https://decomp.me/scratch/Uxers
WIP_FUNC(0x563280)
void CarPhysics_B0::UpdateCp1FromCm1_563280()
{
    WIP_IMPLEMENTED;

    Fix16_Point point(-gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset.x, -gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset.y);
    point.RotateByAngle_40F6B0(field_58_theta);

    field_38_cp1 = field_30_cm1 + point;
}

WIP_FUNC(0x563350)
void CarPhysics_B0::UpdateCenterOfMassPoint_563350()
{
    WIP_IMPLEMENTED;

    const CarInfo_2C* info = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());

    Fix16_Point point = info->field_C_center_of_mass_offset;
    point.RotateByAngle_40F6B0(field_58_theta);

    field_30_cm1 = field_38_cp1 + point;
}

// 0x49EDC0 9.6f
// https://decomp.me/scratch/xDPiP
WIP_FUNC(0x563460)
void CarPhysics_B0::UpdateReferencePoint_563460()
{
    WIP_IMPLEMENTED;

    Fix16_Point point = gCarInfo_2C_6FE0E4->field_C_center_of_mass_offset;

    point.RotateByAngle_40F6B0(field_58_theta);

    field_30_cm1 = field_38_cp1 + point;
}

MATCH_FUNC(0x563560)
void CarPhysics_B0::SetSprite_563560(Sprite* a2)
{
    field_38_cp1.x = a2->field_14_xy.x;
    field_38_cp1.y = a2->field_14_xy.y;
    field_6C_cp3 = a2->field_1C_zpos;
    field_58_theta = a2->field_0;
    field_78_pointing_ang_rad = 0;
    UpdateCenterOfMassPoint_563350();
}

MATCH_FUNC(0x563590)
void CarPhysics_B0::SnapVelocityToSpriteDirection_563590(Sprite* pSprt)
{
    field_40_linvel_1.sub_41E210(field_40_linvel_1.GetLength_2(), pSprt->field_0);
    CarPhysics_B0::SetSprite_563560(pSprt);
}

MATCH_FUNC(0x563670)
void CarPhysics_B0::UpdateSpriteFromPhysics_563670()
{
    Sprite* car_sprite = field_5C_pCar->field_50_car_sprite;
    car_sprite->set_xyz_lazy_420600(field_38_cp1.x, field_38_cp1.y, field_6C_cp3);
    car_sprite->set_ang_lazy_420690(field_58_theta);
}

MATCH_FUNC(0x5636c0)
void CarPhysics_B0::UpdateCarAndTrailerSpriteFromPhysics_5636C0()
{
    UpdateSpriteFromPhysics_563670();

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->field_C_pCarOnTrailer->field_58_physics->UpdateSpriteFromPhysics_563670();
    }
}

MATCH_FUNC(0x5636e0)
bool CarPhysics_B0::IsNearlyStopped_5636E0()
{
    if (field_40_linvel_1.GetLength_2() < dword_6FE0A8)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x5637a0)
void CarPhysics_B0::Init_5637A0()
{
    field_40_linvel_1.reset();
    field_74_ang_vel_rad = Fix16(0);
    field_70 = kFP16Zero_6FE20C;
    ResetForceAccumulators_55A840();
    field_91_is_foot_brake_on = 0;
    field_92_is_hand_brake_on = 0;
    field_93_is_forward_gas_on = 0;
    field_94_is_backward_gas_on = 0;
    field_95 = 0;
    field_AD_turn_direction = 0;
    field_A9_car_model = -1;
    field_A8_hand_brake_force = 0;
    field_90_timer_since_last_move = 0;
    set_field_A0_559B90(0);
    field_A4 = 0;
    field_98_surface_type = 0;
    field_9C = 0;
    field_A5_current_slope_length = 0;
    field_A6_current_slope_left_tiles = 0;
    field_10[0].reset();
    field_10[1].reset();
    field_10[2].reset();
    field_10[3].reset();
    field_8C_state = 1;
    field_8_total_damage_q = 0;
    field_60_gas_pedal = k_dword_6FE290;
    field_AC_drive_wheels_locked_q = 0;
    field_64 = dword_6FE200;
    field_68_z_pos = kFP16Zero_6FE20C;
    field_84_front_skid = kFP16Zero_6FE20C;
    field_88_rear_skid = kFP16Zero_6FE20C;
    field_AA_sbw = 0;
    field_AB_tpa = 0;
}

MATCH_FUNC(0x563890)
void CarPhysics_B0::PoolAllocate()
{
    field_30_cm1.reset();
    field_58_theta = DAT_0066AC08;
    field_38_cp1.reset();
    field_5C_pCar = NULL;
    Init_5637A0();
    field_0_vel_read_only.reset();
}

MATCH_FUNC(0x447010)
EXPORT Fix16_Point CarPhysics_B0::get_linvel_447010()
{
    return field_40_linvel_1;
}

MATCH_FUNC(0x5638c0)
void CarPhysics_B0::SetCar_5638C0(Car_BC* pCar)
{
    this->field_5C_pCar = pCar;
    Ped* pDriver = pCar->field_54_driver;
    if (pDriver && pDriver->field_15C_player)
    {
        this->field_8C_state = 2;
    }
    else
    {
        this->field_8C_state = 1;
    }
}

MATCH_FUNC(0x563900)
CarPhysics_B0::CarPhysics_B0()
{
    mpNext = NULL;
    PoolAllocate();
}