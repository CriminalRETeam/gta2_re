#include "CarPhysics_B0.hpp"
#include "CarInfo_808.hpp"
#include "Globals.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0, 0x6FE3E0);
DEFINE_GLOBAL(CarInfo_2C*, dword_6FE0E4, 0x6FE0E4);
DEFINE_GLOBAL(Ang16, DAT_0066AC08, 0x66AC08);
DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FE20C, Fix16(0), 0x6FE20C);
DEFINE_GLOBAL(Fix16, k_dword_6FE290, 0x6FE290);
DEFINE_GLOBAL(Fix16, k_dword_6FE3A0, 0x6FE3A0);
DEFINE_GLOBAL(Fix16, k_dword_6FDEFC, 0x6FDEFC);
DEFINE_GLOBAL(Fix16, k_dword_6FDF88, 0x6FDF88);
DEFINE_GLOBAL(Fix16, k_dword_6FE364, 0x6FE364);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE200, kFP16Zero_6FE20C, 0x6FE200);
DEFINE_GLOBAL(ModelPhysics_48*, dword_6FE258, 0x6FE258);
DEFINE_GLOBAL(Fix16, dword_6FE1B0, 0x6FE1B0);
DEFINE_GLOBAL(Fix16, dword_6FE348, 0x6FE348);
DEFINE_GLOBAL(Fix16, dword_6FDFB0, 0x6FDFB0);
DEFINE_GLOBAL(Fix16, dword_6FE0D8, 0x6FE0D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFD4, Fix16(0x1000, 0), 0x6FDFD4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE2AC, dword_6FDFD4, 0x6FE2AC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FE210, Fix16(1), 0x6FE210);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE1C0, k_dword_6FE210, 0x6FE1C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDFE4, Fix16(0x1333, 0), 0x6FDFE4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE0A8, dword_6FDFE4, 0x6FE0A8);

DEFINE_GLOBAL(Fix16_Point, g_cm1_6FDF10, 0x6FDF10);
DEFINE_GLOBAL(Fix16, g_cp3_6FDF08, 0x6FDF08);
DEFINE_GLOBAL(Ang16, g_theta_6FE344, 0x6FE344);
DEFINE_GLOBAL(Fix16_Point, g_cp1_6FDF00, 0x6FDF00);
DEFINE_GLOBAL(Fix16, g_f70_6FDFE0, 0x6FDFE0);
DEFINE_GLOBAL(Fix16, g_ZPos_6FE0AC, 0x6FE0AC);

DEFINE_GLOBAL(Fix16_Point, g_trailer_cm1_6FE068, 0x6FE068);
DEFINE_GLOBAL(Fix16, gTrailer_cp3_6FE1B4, 0x6FE1B4);
DEFINE_GLOBAL(Ang16, gTrailer_theta_6FE018, 0x6FE018);
DEFINE_GLOBAL(Fix16_Point, gTrailer_cp1_6FE3A8, 0x6FE3A8);
DEFINE_GLOBAL(Fix16, gTrailer_f70_6FE194, 0x6FE194);
DEFINE_GLOBAL(Fix16, gTrailer_ZPos_6FE354, 0x6FE354);

DEFINE_GLOBAL(Fix16, dword_6FDF3C, 0x6FDF3C);
DEFINE_GLOBAL(Fix16, dword_6FDF7C, 0x6FDF7C);
DEFINE_GLOBAL(Fix16, dword_6FE07C, 0x6FE07C);

DEFINE_GLOBAL(Fix16, FastCarMinVelocity_6FE1CC, 0x6FE1CC);
DEFINE_GLOBAL(Fix16, dword_6FE198, 0x6FE198);
DEFINE_GLOBAL(Fix16, k_dword_6FE1B8, 0x6FE1B8);

DEFINE_GLOBAL(Fix16_Point, stru_6FDF50, 0x6FDF50);
DEFINE_GLOBAL(Fix16, dword_6FE0B0, 0x6FE0B0);

STUB_FUNC(0x559E90)
Fix16 CarPhysics_B0::ComputeZPosition_559E90()
{
    NOT_IMPLEMENTED;
    return 0;
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
void CarPhysics_B0::sub_5597B0()
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

STUB_FUNC(0x559a40)
void CarPhysics_B0::sub_559A40()
{
    NOT_IMPLEMENTED;
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
void CarPhysics_B0::sub_559B90(const s32& a2)
{
    field_A0 = a2;
}

STUB_FUNC(0x559ba0)
u32 CarPhysics_B0::SpinOutOnOil_559BA0()
{
    NOT_IMPLEMENTED;
    return 0;
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
                if (!field_5C_pCar->is_train_model())
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
            CarPhysics_B0::sub_559B90(0);
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

STUB_FUNC(0x559e20)
u32 CarPhysics_B0::sub_559E20(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x559ec0)
s32* CarPhysics_B0::sub_559EC0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x55a1d0)
s32 CarPhysics_B0::sub_55A1D0(s32 a2, s32 a3, s32 a4, u32* a5)
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x55a550)
void CarPhysics_B0::sub_55A550()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a600)
void CarPhysics_B0::sub_55A600()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a6a0)
u32* CarPhysics_B0::sub_55A6A0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x55a860)
char_type CarPhysics_B0::HandleUserInputs_55A860(char_type bForwardGasOn,
                                                 char_type bFootBrakeOn,
                                                 char_type a4,
                                                 char_type a5,
                                                 char_type bHandBrakeOn)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55aa00)
void CarPhysics_B0::HandleGravityOnSlope_55AA00()
{
    Fix16_Point_POD force;

    // On a slope and no brake inputs
    if (field_A5_current_slope_length != 1 || field_92_is_hand_brake_on || field_91_is_foot_brake_on)
    {
        return;
    }

    // Trains ignore slope gravity
    if (field_5C_pCar->is_train_model())
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
s32 CarPhysics_B0::sub_55B4F0(Sprite_4C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55b970)
char_type CarPhysics_B0::sub_55B970(char_type* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55bfe0)
void CarPhysics_B0::sub_55BFE0()
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x55c3b0)
char_type CarPhysics_B0::sub_55C3B0(Sprite_4C** a2, Sprite_4C** a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x55c560)
void CarPhysics_B0::sub_55C560(Fix16 a2, Fix16 a3)
{
    WIP_IMPLEMENTED;

    s32 i = 3;
    do
    {
        restore_saved_physics_state_55A400();
        Fix16 total = (a3 + a2) / 2;
        sub_560F20(total);
        if (sub_55C150())
        {
            a2 = total;
        }
        else
        {
            a3 = total;
        }
        --i;
    } while (i);
}

STUB_FUNC(0x55c5c0)
s32 CarPhysics_B0::sub_55C5C0(u32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55c820)
s32 CarPhysics_B0::sub_55C820(u32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ca70)
void CarPhysics_B0::sub_55CA70(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55cbb0)
void CarPhysics_B0::sub_55CBB0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55d200)
void CarPhysics_B0::sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55dc00)
char_type CarPhysics_B0::sub_55DC00()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x55e260)
void CarPhysics_B0::DoSkidmarks_55E260()
{
    if (!bSkip_skidmarks_67D585)
    {
        CarPhysics_B0::sub_55DC00();
        Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            CarPhysics_B0* pPhysics = pTrailer->field_C_pCarOnTrailer->field_58_physics;
            pPhysics->SetCurrentCarInfoAndModelPhysics_562EF0();
            pPhysics->field_84_front_skid = kFP16Zero_6FE20C;
            pPhysics->field_88_rear_skid = kFP16Zero_6FE20C;
            pPhysics->sub_55DC00();
            CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0();
        }
    }
}

STUB_FUNC(0x55e470)
char_type CarPhysics_B0::sub_55E470()
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x55ec30)
s32 CarPhysics_B0::ApplyForwardEngineForce_55EC30()
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x55f280)
s32 CarPhysics_B0::ProcessCollisionAndClampVelocity_55F280()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x55f330)
void CarPhysics_B0::sub_55F330()
{
    dword_6FE198 = k_dword_6FE210;
    save_physics_state_55A4B0();
    sub_560F20(k_dword_6FE210);
}

MATCH_FUNC(0x55f360)
char_type CarPhysics_B0::sub_55F360()
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

STUB_FUNC(0x55f800)
s32 CarPhysics_B0::sub_55F800(Fix16_Point* a2, Fix16_Point* a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f930)
s32 CarPhysics_B0::sub_55F930(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
    return 0;
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
void CarPhysics_B0::sub_5606C0(s32 a2, char_type a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x560b40)
void CarPhysics_B0::sub_560B40(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x560eb0)
void CarPhysics_B0::UpdateLinearAndAngularAccel_560EB0()
{
    field_50 = field_48.Divide_442CB0(CarPhysics_B0::CalculateMass_559FF0());
    field_80 = -field_7C / CarPhysics_B0::sub_55A050();
}

STUB_FUNC(0x560f20)
void CarPhysics_B0::sub_560F20(Fix16 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5610b0)
void CarPhysics_B0::IntegrateAndClampVelocities_5610B0()
{
    // Integrate linear and angular velocity
    this->field_40_linvel_1.x += this->field_50.x;
    this->field_40_linvel_1.y += this->field_50.y;
    this->field_74_ang_vel_rad += this->field_80;

    ResetForceAccumulators_55A840();

    Fix16 y_abs = (field_40_linvel_1.y <= 0) ? -field_40_linvel_1.y : field_40_linvel_1.y;
    Fix16 x_abs = (field_40_linvel_1.x <= 0) ? -field_40_linvel_1.x : field_40_linvel_1.x;

    if (x_abs + y_abs < dword_6FE07C)
    {
        this->field_40_linvel_1.x = kFP16Zero_6FE20C;
        this->field_40_linvel_1.y = kFP16Zero_6FE20C;
    }

    Fix16 ang_vel_rad_abs = (field_74_ang_vel_rad <= 0) ? -field_74_ang_vel_rad : field_74_ang_vel_rad;

    // Clamp angular velocity if below threshold
    if (ang_vel_rad_abs < dword_6FE07C)
    {
        ang_vel_rad_abs = kFP16Zero_6FE20C;
    }

    field_74_ang_vel_rad = ang_vel_rad_abs;
}

STUB_FUNC(0x561130)
u32* CarPhysics_B0::sub_561130(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561350)
u32* CarPhysics_B0::sub_561350(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561380)
u32* CarPhysics_B0::sub_561380(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5615d0)
s32* CarPhysics_B0::sub_5615D0(s32* a2, s32* a3, s32 a4, u32* a5, s32 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x561940)
s32 CarPhysics_B0::get_revs_561940()
{
    return dword_6FE258->field_1_turbo && this->field_60_gas_pedal >= k_dword_6FE1B8;
}

STUB_FUNC(0x561970)
u32* CarPhysics_B0::sub_561970(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561dd0)
u32* CarPhysics_B0::sub_561DD0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561e50)
Fix16 CarPhysics_B0::CalculateFrontSkid_561E50()
{
    NOT_IMPLEMENTED;
    return Fix16(0);
}

STUB_FUNC(0x5620d0)
Fix16 CarPhysics_B0::CalculateRearSkid_5620D0()
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

STUB_FUNC(0x562560)
s32 CarPhysics_B0::UpdateSteeringAngle_562560()
{
    NOT_IMPLEMENTED;
    return 0;
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

// TODO: Actually Fix16_Point method
MATCH_FUNC(0x562c20)
void CarPhysics_B0::RotateVelocity_562C20(const Ang16& angle)
{
    const Fix16 sin = Ang16::sine_40F500(angle);
    const Fix16 cos = Ang16::cosine_40F520(angle);

    const Fix16 x_old = field_0_vel_read_only.x;

    field_0_vel_read_only.x = (sin * field_0_vel_read_only.y) + (cos * field_0_vel_read_only.x);
    field_0_vel_read_only.y = (cos * field_0_vel_read_only.y) + ((-x_old) * sin);
}

STUB_FUNC(0x562d00)
void CarPhysics_B0::EnforceGearSensitiveMaxSpeed_562D00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x562eb0)
ModelPhysics_48* CarPhysics_B0::sub_562EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562ed0)
CarInfo_2C* CarPhysics_B0::sub_562ED0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x562ef0)
void CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0()
{
    u8 info_idx_remapped = field_5C_pCar->sub_43A850();
    dword_6FE0E4 = gCarInfo_808_678098->sub_454840(info_idx_remapped);
    dword_6FE258 = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(info_idx_remapped);
}

MATCH_FUNC(0x562f30)
void CarPhysics_B0::ApplyInputsAndIntegratePhysics_562F30()
{
    dword_6FE348 = field_5C_pCar->GetDamageFactorOnSpeed_439EE0();
    ApplyThrottleInput_562480();
    ApplyBrakePhysics_5624F0();
    field_84_front_skid = CalculateFrontSkid_561E50();
    field_88_rear_skid = CalculateRearSkid_5620D0();
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
    carModel = field_5C_pCar->sub_43A850();
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
            bCol4 = sub_55F360() | bCol7;
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
            bCol4 = sub_55F360() | bCol3;
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
            bCol4 = sub_55F360() | bCol6;
            DoSkidmarks_55E260();
            this->field_40_linvel_1.x.mValue -= stru_6FDF50.x.mValue;
            this->field_40_linvel_1.y.mValue -= stru_6FDF50.y.mValue;
            this->field_74_ang_vel_rad -= dword_6FE0B0;
            break;
        case 3:
            bCol1 = CheckAndHandleCarAndTrailerCollisions_55EB80();
            ApplyInputsAndIntegratePhysics_562F30();
            ScarePedsOnDrivingFast_559C30();
            sub_55F360();
            bCol7 = ProcessCollisionAndClampVelocity_55F280() | bCol1;
            bCol4 = sub_55F360() | bCol7;
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
            sub_55BFE0();
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
void CarPhysics_B0::sub_563280()
{
    WIP_IMPLEMENTED;

    const CarInfo_2C* info = gCarInfo_808_678098->sub_454840(field_5C_pCar->sub_43A850());

    Fix16_Point point = info->field_C;
    point.RotateByAngle_40F6B0(field_58_theta);

    field_38_cp1 = field_30_cm1 + point;
}

WIP_FUNC(0x563350)
void CarPhysics_B0::UpdateCenterOfMassPoint_563350()
{
    WIP_IMPLEMENTED;

    const CarInfo_2C* info = gCarInfo_808_678098->sub_454840(field_5C_pCar->sub_43A850());

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
    field_38_cp1.x = a2->field_14_xpos.x;
    field_38_cp1.y = a2->field_14_xpos.y;
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
void CarPhysics_B0::sub_563670()
{
    Sprite* car_sprite = field_5C_pCar->field_50_car_sprite;
    car_sprite->sub_420600(field_38_cp1.x, field_38_cp1.y, field_6C_cp3);
    car_sprite->sub_420690(field_58_theta);
}

MATCH_FUNC(0x5636c0)
void CarPhysics_B0::sub_5636C0()
{
    sub_563670();

    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->field_C_pCarOnTrailer->field_58_physics->sub_563670();
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
    sub_559B90(0);
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