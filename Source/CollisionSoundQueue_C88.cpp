#include "CollisionSoundQueue_C88.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "collision_event_type.hpp"

DEFINE_GLOBAL(CollisionSoundQueue_C88*, gCollisionSoundQueue_66AFE0, 0x66AFE0);
DEFINE_GLOBAL(CollisionTarget_28, gCollisionTarget_679188, 0x679188);
DEFINE_GLOBAL(Fix16, kFpZero_66AE98, 0x66AE98);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_66AE9C, Fix16(0x4000, 0), 0x66AE9C);

MATCH_FUNC(0x40B870)
void CollisionEvent_28::SetPosition_40B870(Fix16 x, Fix16 y, Fix16 z)
{
    field_4_x = x;
    field_8_y = y;
    field_C_z = z;
}

MATCH_FUNC(0x40B890)
void CollisionEvent_28::SetupForCar_40B890(Car_BC* pCar)
{
    field_10_car = pCar;

    switch (gCollisionTarget_679188.field_0_type)
    {
        case collision_target_type::horizontal_edge_1:
            field_0_type = collision_event_type::car_wall_5;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetTopEdgeSpec_4E0000(gCollisionTarget_679188.field_4_hseg_x_min, gCollisionTarget_679188.field_18_hseg_y, gCollisionTarget_679188.field_1C_z);
            break;

        case collision_target_type::vertical_edge_2:
            field_0_type = collision_event_type::car_wall_5;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetLeftEdgeSpec_4DFF60(gCollisionTarget_679188.field_14_vseg_x, gCollisionTarget_679188.field_C_vseg_y_min, gCollisionTarget_679188.field_1C_z);
            break;

        case collision_target_type::sprite_3:
        {
            Car_BC* cBC = gCollisionTarget_679188.field_20_pHitSprite->AsCar_40FEB0();
            if (cBC)
            {
                field_14_other_car = cBC;
                field_0_type = collision_event_type::car_car_2;
            }
            else if (gCollisionTarget_679188.field_20_pHitSprite->AsCharB4_40FEA0())
            {
                field_0_type = collision_event_type::car_ped_4;
            }
            else
            {
                Object_2C* p2C = gCollisionTarget_679188.field_20_pHitSprite->As2C_40FEC0();

                field_18_object_model = p2C->get_model_40FEF0();
                if (field_18_object_model == objects::diagonal_wall_collision_obj_166)
                {
                    field_0_type = collision_event_type::car_wall_5;
                    field_20_map_block_spec = p2C->sub_529240();
                }
                else
                {
                    field_0_type = collision_event_type::car_object_3;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x40B980)
void CollisionEvent_28::SetupForPed_40B980()
{
    switch (gCollisionTarget_679188.field_0_type)
    {
        case collision_target_type::horizontal_edge_1:
            field_0_type = collision_event_type::ped_wall_8;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetTopEdgeSpec_4E0000(gCollisionTarget_679188.field_4_hseg_x_min, gCollisionTarget_679188.field_18_hseg_y, gCollisionTarget_679188.field_1C_z);
            break;

        case collision_target_type::vertical_edge_2:
            field_0_type = collision_event_type::ped_wall_8;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetLeftEdgeSpec_4DFF60(gCollisionTarget_679188.field_14_vseg_x, gCollisionTarget_679188.field_C_vseg_y_min, gCollisionTarget_679188.field_1C_z);
            break;

        case collision_target_type::sprite_3:
        {
            Car_BC* cBC = gCollisionTarget_679188.field_20_pHitSprite->AsCar_40FEB0();
            if (cBC)
            {
                field_10_car = cBC;
                field_0_type = collision_event_type::car_ped_4;
            }
            else if (gCollisionTarget_679188.field_20_pHitSprite->AsCharB4_40FEA0())
            {
                field_0_type = collision_event_type::ped_ped_6;
            }
            else
            {
                Object_2C* p2C = gCollisionTarget_679188.field_20_pHitSprite->As2C_40FEC0();
                field_18_object_model = p2C->get_model_40FEF0();
                if (field_18_object_model == objects::diagonal_wall_collision_obj_166)
                {
                    field_0_type = collision_event_type::ped_wall_8;
                    field_20_map_block_spec = p2C->sub_529240();
                }
                else
                {
                    field_0_type = collision_event_type::ped_object_7;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x40BA60)
bool CollisionEvent_28::SetupForObject_40BA60(Object_2C* pObj)
{
    if (!pObj->GetDefField63_40FF00())
    {
        return 0;
    }

    field_18_object_model = pObj->field_18_model;

    switch (gCollisionTarget_679188.field_0_type)
    {
        case collision_target_type::horizontal_edge_1:
            field_0_type = collision_event_type::object_wall_10;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetTopEdgeSpec_4E0000(gCollisionTarget_679188.field_4_hseg_x_min, gCollisionTarget_679188.field_18_hseg_y, gCollisionTarget_679188.field_1C_z);

            break;

        case collision_target_type::vertical_edge_2:
            field_0_type = collision_event_type::object_wall_10;
            field_20_map_block_spec =
                gMap_0x370_6F6268->GetLeftEdgeSpec_4DFF60(gCollisionTarget_679188.field_14_vseg_x, gCollisionTarget_679188.field_C_vseg_y_min, gCollisionTarget_679188.field_1C_z);

            break;

        case collision_target_type::sprite_3:
        {
            Car_BC* cBC = gCollisionTarget_679188.field_20_pHitSprite->AsCar_40FEB0();
            if (cBC)
            {
                field_10_car = cBC;
                field_0_type = collision_event_type::car_object_3;
                return 1;
            }
            else if (gCollisionTarget_679188.field_20_pHitSprite->AsCharB4_40FEA0())
            {
                field_0_type = collision_event_type::ped_object_7;
                return 1;
            }
            else
            {
                Object_2C* o2c = gCollisionTarget_679188.field_20_pHitSprite->As2C_40FEC0();
                if (!o2c->GetDefField63_40FF00())
                {
                    return 0;
                }

                field_1C_other_object_model = o2c->get_model_40FEF0();
                if (field_1C_other_object_model == objects::diagonal_wall_collision_obj_166)
                {
                    field_0_type = collision_event_type::object_wall_10;
                    field_20_map_block_spec = o2c->sub_529240();
                }
                else
                {
                    field_0_type = collision_event_type::object_object_9;
                }
            }
            break;
        }
    }

    return 1;
}

MATCH_FUNC(0x40bb90)
void CollisionSoundQueue_C88::Reset_40BB90()
{
    field_C84_count = 0;
}

// ================================================================

MATCH_FUNC(0x40bba0)
void CollisionSoundQueue_C88::AddCollision_40BBA0(Sprite* pSprite, Fix16 impact_strength)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent_28* pEvent = &field_4_events[field_C84_count];
        Car_BC* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->SetupForCar_40B890(pCar);
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->SetupForPed_40B980();
        }
        else
        {
            Object_2C* p2c = pSprite->As2C_40FEC0();
            if (!pEvent->SetupForObject_40BA60(p2c))
            {
                return;
            }
        }

        pEvent->SetPosition_40B870(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);
        pEvent->SetImpactStrength_40FF10(impact_strength);
        field_C84_count++;
    }
}

MATCH_FUNC(0x40bc40)
void CollisionSoundQueue_C88::AddFloorCollision_40BC40(Sprite* pSprite)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent_28* pEvent = &field_4_events[field_C84_count];
        pEvent->SetPosition_40B870(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);
        pEvent->SetImpactStrength_40FF10(kFpZero_66AE98);

        Car_BC* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->field_0_type = collision_event_type::car_floor_12;
            pEvent->field_10_car = pCar;
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->field_0_type = collision_event_type::ped_floor_11;
        }
        else
        {
            Object_2C* p2c = pSprite->As2C_40FEC0();
            pEvent->field_0_type = collision_event_type::object_floor_1;
            pEvent->field_18_object_model = p2c->get_model_40FEF0();
        }
        pEvent->field_20_map_block_spec = gMap_0x370_6F6268->GetBlockSpec_4E00A0(pSprite->field_14_xy.x,
                                                                              pSprite->field_14_xy.y,
                                                                              pSprite->field_1C_zpos - kFpOne_66AE9C);
        field_C84_count++;
    }
}

MATCH_FUNC(0x40bd10)
void CollisionSoundQueue_C88::AddBlockCollision_40BD10(Sprite* pSprite)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent_28* pEvent = &this->field_4_events[this->field_C84_count];
        pEvent->SetPosition_40B870(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);
        pEvent->SetImpactStrength_40FF10(kFpZero_66AE98);

        Car_BC* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->field_0_type = collision_event_type::car_floor_12;
            pEvent->field_10_car = pCar;
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->field_0_type = collision_event_type::ped_floor_11;
        }
        else
        {
            Object_2C* p2c = pSprite->As2C_40FEC0();
            pEvent->field_0_type = collision_event_type::object_floor_1;
            pEvent->field_18_object_model = p2c->get_model_40FEF0();
        }
        pEvent->field_20_map_block_spec =
            gMap_0x370_6F6268->GetBlockSpec_4E00A0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);
        field_C84_count++;
    }
}

MATCH_FUNC(0x40bdd0)
void CollisionSoundQueue_C88::AddSpriteCollision_40BDD0(Sprite* pSprite, Sprite* pHitSprite)
{
    gCollisionTarget_679188.field_20_pHitSprite = pHitSprite;
    gCollisionTarget_679188.field_0_type = collision_target_type::sprite_3;
    AddCollision_40BBA0(pSprite, kFpZero_66AE98);
}

MATCH_FUNC(0x40be00)
CollisionSoundQueue_C88::CollisionSoundQueue_C88()
{
    if (bSkip_audio_67D6BE)
    {
        field_0_pSoundObj = NULL;
    }
    else
    {
        field_0_pSoundObj = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::CollisionSoundQueue_6);
    }
    Reset_40BB90();
}

MATCH_FUNC(0x40be40)
CollisionSoundQueue_C88::~CollisionSoundQueue_C88()
{
    if (field_0_pSoundObj)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_0_pSoundObj);
        field_0_pSoundObj = 0;
    }
}

MATCH_FUNC(0x477A10)
bool CollisionTarget_28::IsObj2C_477A10()
{
    if (field_0_type == collision_target_type::sprite_3)
    {
        Object_2C* p2c = field_20_pHitSprite->As2C_40FEC0();
        if (p2c)
        {
            if (p2c->field_8->field_4C == 3)
            {
                return 1;
            }
        }
    }
    return 0;
}