#include "CarPhysics_B0.hpp"
#include "CarInfo_808.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0, 0x6FE3E0);
DEFINE_GLOBAL(CarInfo_2C*, dword_6FE0E4, 0x6FE0E4);
DEFINE_GLOBAL(ModelPhysics_48*, dword_6FE258, 0x6FE258);
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
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2EC, Fix16(40), 0x6FE2EC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE21C, Fix16(4), 0x6FE21C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE07C, k_dword_6FE210 / (dword_6FE2EC * dword_6FE21C), 0x6FE07C);

DEFINE_GLOBAL_INIT(Fix16, FastCarMinVelocity_6FE1CC, Fix16(0x1EB, 0), 0x6FE1CC);
DEFINE_GLOBAL(Fix16, dword_6FE198, 0x6FE198);
DEFINE_GLOBAL(Fix16, dword_6FE370, 0x6FE370);

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
void CarPhysics_B0::sub_559B40()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->sub_407CE0();
    }
}

MATCH_FUNC(0x559b50)
void CarPhysics_B0::sub_559B50()
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
void CarPhysics_B0::sub_559DD0()
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
void CarPhysics_B0::sub_559E20(Object_2C* pObj)
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
Fix16 CarPhysics_B0::sub_55A050()
{
    if (field_5C_pCar->field_64_pTrailer)
    {
        return field_5C_pCar->field_64_pTrailer->field_8_truck_cab->sub_43A590() +
            field_5C_pCar->field_64_pTrailer->field_C_pCarOnTrailer->sub_43A590();
    }
    else
    {
        return field_5C_pCar->sub_43A590();
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
        return dword_6FE1B0 * dword_6FE258->field_18_turn_ratio;
    }
    else
    {
        return dword_6FE258->field_18_turn_ratio;
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
    offset.x = pCarInfo->field_C.x;
    offset.y = pCarInfo->field_C.y;
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

STUB_FUNC(0x55a6a0)
Fix16 CarPhysics_B0::sub_55A6A0()
{
    NOT_IMPLEMENTED;
    return Fix16(10, 0);
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

STUB_FUNC(0x55ab50)
s32* CarPhysics_B0::sub_55AB50(s32* a2, Sprite_4C** a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ad90)
s32 CarPhysics_B0::sub_55AD90(Fix16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x55b3f0)
void CarPhysics_B0::sub_55B3F0(Fix16 a2)
{
    WIP_IMPLEMENTED;

    sub_55AD90(a2);

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarOnTrailerPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
        pCarOnTrailerPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pCarOnTrailerPhysics->sub_55AD90(a2);
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

STUB_FUNC(0x55b4f0)
s32 CarPhysics_B0::sub_55B4F0(Fix16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x55B7B0)
void CarPhysics_B0::sub_55B7B0(Fix16 a2)
{
    sub_55B4F0(a2);
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

STUB_FUNC(0x55b970)
char_type CarPhysics_B0::ProcessGroundCollisionAndSurfaceType_55B970(char_type* a2)
{
    NOT_IMPLEMENTED;
    return 0;
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
char_type CarPhysics_B0::sub_55C150()
{
    Sprite* pCarSprite = this->field_5C_pCar->field_50_car_sprite;
    if (!pCarSprite->sub_5A2500() && !pCarSprite->sub_59E7D0(0))
    {
        Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            pCarSprite = pTrailer->field_C_pCarOnTrailer->field_50_car_sprite;
            if (!pCarSprite->sub_5A2500() && !pCarSprite->sub_59E7D0(0))
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

    Fix16 movement = sub_55A6A0();

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

        if (sub_55C150())
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

        if (sub_55C150())
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

            if (sub_55C150())
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
void CarPhysics_B0::sub_55C560(Fix16& a2, Fix16& a3)
{
    for (s32 i = 0; i < 3; i++)
    {
        restore_saved_physics_state_55A400();
        Fix16 total = (a3 + a2) / 2;
        ApplyMovementStep_560F20(total);
        if (sub_55C150())
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
void CarPhysics_B0::sub_55C5C0(Fix16_Point& a2, Ang16& a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55c820)
void CarPhysics_B0::sub_55C820(Fix16_Point& a2, Ang16& a3)
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x4A4170
WIP_FUNC(0x55ca70)
void CarPhysics_B0::sub_55CA70(Fix16_Point a2, Ang16 a3)
{
    WIP_IMPLEMENTED;

    Fix16_Point arg0;
    //v7 = 0;
    switch (gRozza_679188.field_0_type)
    {
        case 1:
            sub_55C5C0(a2, a3);
            break;
        case 2:
            sub_55C820(a2, a3);
            break;
        case 3:
            // TODO: Likely wrong arguments here
            stru_6FE1A0 = *field_5C_pCar->field_50_car_sprite->FindCollisionIntersectionPoint_5A2710(&arg0,
                                                                                                     gRozza_679188.field_20_pSprite,
                                                                                                     a2,
                                                                                                     a3,
                                                                                                     &byte_6FDFC4,
                                                                                                     &byte_6FDFCC);

            Car_BC* pCar = gRozza_679188.field_20_pSprite->AsCar_40FEB0();
            if (pCar)
            {
                sub_55FF20(pCar);
            }
            else
            {
                Char_B4* pB4 = gRozza_679188.field_20_pSprite->AsCharB4_40FEA0();
                if (pB4)
                {
                    sub_560B40(pB4, a3);
                }
                else
                {
                    Object_2C* p2C = gRozza_679188.field_20_pSprite->As2C_40FEC0();
                    sub_5606C0(p2C, byte_6FDFC4);
                }
            }
            break;
    }

    gRozza_C88_66AFE0->OtherType_40BBA0(field_5C_pCar->field_50_car_sprite, dword_6FE33C);
}

// https://decomp.me/scratch/0TpGe
WIP_FUNC(0x55cbb0)
void CarPhysics_B0::sub_55CBB0(Fix16 a2, Fix16 a3)
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
        CarPhysics_B0::sub_55CA70(point, pPhysics->field_58_theta);
    }
    else
    {
        CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
        CarPhysics_B0::sub_55CA70(point, pPhysics->field_58_theta);
        CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
    }
}

STUB_FUNC(0x55d200)
void CarPhysics_B0::sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55dc00)
char_type CarPhysics_B0::UpdateWheelSkidEffects_55DC00()
{
    NOT_IMPLEMENTED;
    return 0;
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
        sub_55C560(a2, a3);
        if (field_5C_pCar->IsTrainModel_403BA0() && !field_40_linvel_1.IsNull_420360())
        {
            a3 = kFP16Zero_6FE20C;
        }
        sub_55CBB0(a2, a3);
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
        sub_55F970(global_val);
        goto LABEL_13;
    }

    sub_55F970(k_dword_6FDFA4);

LABEL_13:
    Fix16_Point v4 = stru_6FE1F0.NormalizeSafe_442AD0();
    //v9 = 0;
    Fix16_Point v5 = (v4 * stru_6FDF80);
    //LOBYTE(v9) = 1;
    ApplyForceScaledByMass_55F9A0(v5);

    this->field_AA_sbw = 1;
}

STUB_FUNC(0x55ef20)
s32 CarPhysics_B0::ApplyReverseEngineForce_55EF20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f020)
s32 CarPhysics_B0::ApplyTurningForce_55F020()
{
    NOT_IMPLEMENTED;
    return 0;
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
void CarPhysics_B0::sub_55F330()
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
void CarPhysics_B0::sub_55F740(Fix16_Point* a2, Fix16_Point* a3)
{
    WIP_IMPLEMENTED;

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer && pTrailer->field_C_pCarOnTrailer == field_5C_pCar)
    {
        CarPhysics_B0* pB0 = pTrailer->field_8_truck_cab->field_58_physics;
        pB0->SetCurrentCarInfoAndModelPhysics_562EF0();
        pB0->sub_55F7A0(a2, *a3);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        sub_55F7A0(a2, *a3);
    }
}

MATCH_FUNC(0x55f7a0)
void CarPhysics_B0::sub_55F7A0(Fix16_Point* a2, Fix16_Point a3)
{
    sub_55F800(a2, &a3, 0);
    UpdateLinearAndAngularAccel_560EB0();
    IntegrateAndClampVelocities_5610B0();
}

// 9.6f 0x4A0850
WIP_FUNC(0x55f800)
void CarPhysics_B0::sub_55F800(Fix16_Point* a2, Fix16_Point* a3, s32 bRotate)
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
void CarPhysics_B0::sub_55F930(Fix16_Point* a2)
{
    field_48 += (*a2 * dword_6FE258->field_4_mass);
}

MATCH_FUNC(0x55f970)
void CarPhysics_B0::sub_55F970(Fix16 a2)
{
    this->field_7C -= (dword_6FE0E4->field_0 * a2);
}

MATCH_FUNC(0x55f9a0)
void CarPhysics_B0::ApplyForceScaledByMass_55F9A0(Fix16_Point_POD& pForce)
{
    field_48 += pForce.Multiply_438FE0(CarPhysics_B0::CalculateMass_559FF0());
}

WIP_FUNC(0x55fa10)
void CarPhysics_B0::sub_55FA10(Fix16_Point* a2)
{
    WIP_IMPLEMENTED;

    Car_BC* pCar = this->field_5C_pCar;
    Trailer* pTrailer = pCar->field_64_pTrailer;
    if (pTrailer && pTrailer->field_C_pCarOnTrailer == pCar)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
        pPhysics->sub_55F930(a2);
        SetCurrentCarInfoAndModelPhysics_562EF0();
    }
    else
    {
        sub_55F930(a2);
    }
}

STUB_FUNC(0x55fa60)
u32* CarPhysics_B0::sub_55FA60(u32* a2, s32* a3, s32* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fc30)
Ped* CarPhysics_B0::sub_55FC30(s32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fd00)
s32 CarPhysics_B0::sub_55FD00(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ff20)
Car_78* CarPhysics_B0::sub_55FF20(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5606c0)
void CarPhysics_B0::sub_5606C0(Object_2C* a2, char_type a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x560b40)
void CarPhysics_B0::sub_560B40(Char_B4* a2, Ang16& a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x560eb0)
void CarPhysics_B0::UpdateLinearAndAngularAccel_560EB0()
{
    field_50 = field_48.Divide_442CB0(CarPhysics_B0::CalculateMass_559FF0());
    field_80 = -field_7C / CarPhysics_B0::sub_55A050();
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

        sub_559B40();

        if (field_5C_pCar->IsTrainModel_403BA0())
        {
            sub_55B7B0(a2);
            UpdateCarAndTrailerSpriteFromPhysics_5636C0();
            return;
        }
        sub_55B3F0(a2);
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

STUB_FUNC(0x561130)
Fix16_Point CarPhysics_B0::sub_561130(Fix16_Point* a3)
{
    NOT_IMPLEMENTED;
    return Fix16_Point();
}

MATCH_FUNC(0x561350)
Fix16_Point CarPhysics_B0::sub_561350(Fix16_Point* a3)
{
    SetCarInfoGlobal_562ED0();
    return sub_561130(a3);
}

STUB_FUNC(0x561380)
Fix16_Point CarPhysics_B0::sub_561380(Fix16_Point* a3)
{
    NOT_IMPLEMENTED;
    return Fix16_Point(Fix16(0), Fix16(0));
}

STUB_FUNC(0x5615d0)
Fix16 CarPhysics_B0::ApplyDriveForce_5615D0(s32* a3, s32 a4, u32* a5, s32 a6)
{
    NOT_IMPLEMENTED;
    return Fix16(0);
}

MATCH_FUNC(0x561940)
s32 CarPhysics_B0::get_revs_561940()
{
    return dword_6FE258->field_1_turbo && this->field_60_gas_pedal >= k_dword_6FE1B8;
}

STUB_FUNC(0x561970)
Fix16 CarPhysics_B0::ComputeEngineTorque_561970()
{
    NOT_IMPLEMENTED;
    return Fix16(0);
}

WIP_FUNC(0x561dd0)
Fix16 CarPhysics_B0::ComputeTorqueFromThrottle_561DD0()
{
    WIP_IMPLEMENTED;

    if (get_revs_561940() != 0)
    {
        // Somehow this is wrong :')
        return dword_6FE0E4->field_14 + Fix16(2, 0) * ((this->field_60_gas_pedal * ((dword_6FE348 * dword_6FE0E4->field_18))));
    }
    else
    {
        return dword_6FE0E4->field_14 + ((this->field_60_gas_pedal * ((dword_6FE348 * dword_6FE0E4->field_18))));
    }
}

STUB_FUNC(0x561e50)
Fix16 CarPhysics_B0::CalculateFrontWheelForce_561E50()
{
    NOT_IMPLEMENTED;
    return Fix16(0);
}

STUB_FUNC(0x5620d0)
Fix16 CarPhysics_B0::CalculateRearWheelForce_5620D0()
{
    NOT_IMPLEMENTED;
    return Fix16(0);
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
        dword_6FE0D8 = field_64 * dword_6FE258->field_10_brake_friction;
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
s32 CarPhysics_B0::IsGasPedalPressedEnough_5626A0()
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

STUB_FUNC(0x5626f0)
char_type CarPhysics_B0::ApplyArrowSteerAssist_5626F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562910)
void CarPhysics_B0::StabilizeVelocityAtSpeed_562910()
{
    NOT_IMPLEMENTED;
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

// 9.6f 0x4A1B20
WIP_FUNC(0x562d00)
void CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00()
{
    WIP_IMPLEMENTED;

    Fix16_Point polar;
    if (!IsInAir_55A0B0())
    {
        Fix16 radius;
        if (sub_40F840())
        {
            radius = dword_6FE258->field_28_max_speed;
        }
        else
        {
            radius = dword_6FE258->field_40_gear2_speed;
        }

        Fix16_Point& linVel = field_40_linvel_1;
        if (linVel.GetLength_2() > radius)
        {
            Ang16 ang = linVel.atan2_40F790();
            polar.FromPolar_41E210(radius, ang);
            linVel.ClampTowardsZero_49E480(polar);
        }
    }
}

MATCH_FUNC(0x562eb0)
void CarPhysics_B0::SetModelPhysicsGlobal_562EB0()
{
    dword_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_5C_pCar->GetCarModelForPhysics_43A850());
}

MATCH_FUNC(0x562ed0)
void CarPhysics_B0::SetCarInfoGlobal_562ED0()
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());
    dword_6FE0E4 = pInfo;
}

MATCH_FUNC(0x562ef0)
void CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0()
{
    u8 info_idx_remapped = field_5C_pCar->GetCarModelForPhysics_43A850();
    dword_6FE0E4 = gCarInfo_808_678098->GetInfoAtIdx_454840(info_idx_remapped);
    dword_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(info_idx_remapped);
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

WIP_FUNC(0x562fe0)
bool CarPhysics_B0::ProcessCarPhysicsStateMachine_562FE0()
{
    WIP_IMPLEMENTED;

    char carModel; // al
    char bCol2; // bl
    char bCol3; // bl
    char bCol4; // bl
    char bCol5; // bl
    char bCol6; // bl
    char bCol1; // bl
    char bCol7; // bl
    Ped* pDriver; // eax

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
        case 0:
            bCol1 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ScarePedsOnDrivingFast_559C30();
            bCol7 = ProcessCollisionAndClampVelocity_55F280() | bCol1;
            bCol4 = CheckPendingCollision_55F360() | bCol7;
            DoSkidmarks_55E260();
            pDriver = this->field_5C_pCar->field_54_driver;
            if (pDriver && pDriver->field_15C_player) // Car_BC::IsDrivenByPlayer_4118D0
            {
                this->field_8C_state = 2; // sub_4212B0
            }
            else
            {
                this->field_8C_state = 1; // sub_4212A0
            }
            break;
        case 1:
            stru_6FDF50.x.mValue = 0;
            stru_6FDF50.y.mValue = 0;
            bCol2 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            sub_559DD0();
            sub_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            this->field_40_linvel_1.x.mValue += stru_6FDF50.x.mValue;
            this->field_40_linvel_1.y.mValue += stru_6FDF50.y.mValue;
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            bCol3 = ProcessCollisionAndClampVelocity_55F280() | bCol2;
            bCol4 = CheckPendingCollision_55F360() | bCol3;
            DoSkidmarks_55E260();
            this->field_40_linvel_1.x.mValue -= stru_6FDF50.x.mValue;
            this->field_40_linvel_1.y.mValue -= stru_6FDF50.y.mValue;
            this->field_74_ang_vel_rad -= dword_6FE0B0;
            break;
        case 2:
            stru_6FDF50.x.mValue = 0;
            stru_6FDF50.y.mValue = 0;
            bCol5 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            sub_559B50();
            UpdateSteeringAngle_562560();
            ApplyInputsAndIntegratePhysics_562F30();
            StabilizeVelocityAtSpeed_562910();
            EnforceGearSensitiveMaxSpeed_562D00();
            this->field_40_linvel_1.x.mValue += stru_6FDF50.x.mValue;
            this->field_40_linvel_1.y.mValue += stru_6FDF50.y.mValue;
            ApplyArrowSteerAssist_5626F0();
            ScarePedsOnDrivingFast_559C30();
            bCol6 = ProcessCollisionAndClampVelocity_55F280() | bCol5;
            bCol4 = CheckPendingCollision_55F360() | bCol6;
            DoSkidmarks_55E260();
            this->field_40_linvel_1.x.mValue -= stru_6FDF50.x.mValue;
            this->field_40_linvel_1.y.mValue -= stru_6FDF50.y.mValue;
            this->field_74_ang_vel_rad -= dword_6FE0B0;
            break;
        case 3:
            bCol1 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyInputsAndIntegratePhysics_562F30();
            ScarePedsOnDrivingFast_559C30();
            CheckPendingCollision_55F360();
            bCol7 = ProcessCollisionAndClampVelocity_55F280() | bCol1;
            bCol4 = CheckPendingCollision_55F360() | bCol7;
            DoSkidmarks_55E260();
            pDriver = this->field_5C_pCar->field_54_driver;
            if (pDriver && pDriver->field_15C_player) // Car_BC::IsDrivenByPlayer_4118D0
            {
                this->field_8C_state = 2; // sub_4212B0
            }
            else
            {
                this->field_8C_state = 1; // sub_4212A0
            }
            break;
        case 4:
            bCol4 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            sub_55F330();
            ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
            DoSkidmarks_55E260();
            pDriver = this->field_5C_pCar->field_54_driver;
            if (pDriver && pDriver->field_15C_player) // Car_BC::IsDrivenByPlayer_4118D0
            {
                this->field_8C_state = 2; // sub_4212B0
            }
            else
            {
                this->field_8C_state = 1; // sub_4212A0
            }
            break;
        default:
            //bCol4 = v14;
            break;
    }

    if (UpdateLastMovementTimer_562FA0())
    {
        if (!bCol4)
        {
            if (field_98_surface_type != 7 && field_98_surface_type != 8 && field_98_surface_type != 6)
            {
                return 1;
            }
        }
    }

    return 0;
}

WIP_FUNC(0x563280)
void CarPhysics_B0::UpdateCp1FromCm1_563280()
{
    WIP_IMPLEMENTED;

    const CarInfo_2C* info = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());

    Fix16_Point point = info->field_C;
    point.RotateByAngle_40F6B0(field_58_theta);

    field_38_cp1 = field_30_cm1 + point;
}

WIP_FUNC(0x563350)
void CarPhysics_B0::UpdateCenterOfMassPoint_563350()
{
    WIP_IMPLEMENTED;

    const CarInfo_2C* info = gCarInfo_808_678098->GetInfoAtIdx_454840(field_5C_pCar->GetCarModelForPhysics_43A850());

    Fix16_Point point = info->field_C;
    point.RotateByAngle_40F6B0(field_58_theta);

    field_30_cm1 = field_38_cp1 + point;
}

// 0x49EDC0 9.6f
// https://decomp.me/scratch/xDPiP
WIP_FUNC(0x563460)
void CarPhysics_B0::UpdateReferencePoint_563460()
{
    WIP_IMPLEMENTED;

    Fix16_Point point = dword_6FE0E4->field_C;

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