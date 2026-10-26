#include "Car_10.hpp"
#include "Ped.hpp"
#include "car_door_state.hpp"

const u8 kDoorFirstDeltaIdx_61A808[] = {7, 24, 11, 28};

// Read only in CarAI.cpp, defined here: see the comment there.
DEFINE_GLOBAL_INIT(Ang16, kAng4_677CE2, Ang16(0x10), 0x677CE2);
DEFINE_GLOBAL_INIT(Ang16, kAng0_677CE8, Ang16(0), 0x677CE8);
DEFINE_GLOBAL_INIT(Ang16, kAng12_677A2E, Ang16(0x30), 0x677A2E);

// 9.6f: the bit ops are BitSet32::clear_bit/set_bit (0x420DE0/0x420DC0) on a maybe_flags, here a u32*
MATCH_FUNC(0x439CD0)
void Car_Door::AnimateOpening_439CD0(u32* pRet)
{
    if (field_0_animation_frame != 4)
    {
        if (field_1_frame_delay == 0)
        {
            if (field_0_animation_frame != 0)
            {
                *pRet &= ~(1 << (field_2_first_delta_idx + field_0_animation_frame - 1));
            }
            *pRet |= 1 << (field_0_animation_frame + field_2_first_delta_idx);
            field_1_frame_delay = 2;
            field_0_animation_frame++;
        }
        else
        {
            field_1_frame_delay--;
        }
    }

    if (field_0_animation_frame == 4)
    {
        field_4_state = car_door_state::open_1;
    }
}

// 9.6f: the bit ops are BitSet32::clear_bit/set_bit (0x420DE0/0x420DC0) on a maybe_flags, here a u32*
MATCH_FUNC(0x439D40)
void Car_Door::AnimateClosing_439D40(u32* pDeltaMask)
{
    if (field_0_animation_frame)
    {
        *pDeltaMask &= ~(1 << (field_2_first_delta_idx + field_0_animation_frame - 1));
        field_0_animation_frame--;
    }

    if (!field_0_animation_frame)
    {
        field_4_state = car_door_state::closed_6;
    }
    else
    {
        *pDeltaMask |= 1 << (field_2_first_delta_idx + field_0_animation_frame - 1);
    }
}

MATCH_FUNC(0x439DA0)
void Car_Door::Service_439DA0(u32* pDeltaMask)
{
    switch (field_4_state)
    {
        case car_door_state::opening_2:
            AnimateOpening_439CD0(pDeltaMask);
            break;

        case car_door_state::closing_3:
            AnimateClosing_439D40(pDeltaMask);
            break;

        case car_door_state::close_requested_5:
            field_4_state = car_door_state::about_to_close_4;
            break;

        case car_door_state::about_to_close_4:
            field_4_state = car_door_state::closing_3;
            break;

        case car_door_state::open_1:
            if (field_C_stay_open_timer > 0)
            {
                field_C_stay_open_timer--;
            }
            else
            {
                Close_439EA0();
            }
            break;
        default:
            break;
    }

    Ped* pPed = field_8_pObj;
    if (pPed)
    {
        if (pPed->isDead_403B60() || !pPed->field_21C_bf.bLeftVehicle)
        {
            field_8_pObj = NULL;
        }
    }
}

MATCH_FUNC(0x439E40)
void Car_Door::Init_439E40(u8 door_idx)
{
    field_4_state = car_door_state::closed_6;
    field_0_animation_frame = 0;
    field_2_first_delta_idx = kDoorFirstDeltaIdx_61A808[door_idx];
}

MATCH_FUNC(0x439E60)
void Car_Door::Open_439E60()
{
    switch (field_4_state)
    {
        case car_door_state::open_1:
        case car_door_state::opening_2:
            field_C_stay_open_timer = 50;
            break;
        case car_door_state::closing_3:
        case car_door_state::about_to_close_4:
        case car_door_state::close_requested_5:
        case car_door_state::closed_6:
            field_1_frame_delay = 4;
            field_4_state = car_door_state::opening_2;
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x439EA0)
void Car_Door::Close_439EA0()
{
    if (field_4_state > car_door_state::inactive_0 && field_4_state <= car_door_state::opening_2)
    {
        field_4_state = car_door_state::close_requested_5;
    }
}

MATCH_FUNC(0x447330)
Car_Door::Car_Door()
{
    field_8_pObj = NULL;
    field_2_first_delta_idx = 0;
    field_0_animation_frame = 0;
    field_1_frame_delay = 3;
    field_4_state = car_door_state::inactive_0;
    field_C_stay_open_timer = 0;
}

MATCH_FUNC(0x447350)
Car_Door::~Car_Door()
{
    field_8_pObj = NULL;
}