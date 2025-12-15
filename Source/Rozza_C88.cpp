#include "Rozza_C88.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(Rozza_C88*, gRozza_C88_66AFE0, 0x66AFE0);
DEFINE_GLOBAL(Rozza_28, gRozza_679188, 0x679188);

MATCH_FUNC(0x40B870)
void Rozza_A::set_xyz_40B870(Fix16 x, Fix16 y, Fix16 z)
{
    field_4_x = x;
    field_8_y = y;
    field_C_z = z;
}

MATCH_FUNC(0x40B890)
void Rozza_A::sub_40B890(Car_BC* pCar)
{
    field_10 = pCar;

    switch (gRozza_679188.field_0_type)
    {
        case 1:
            field_0 = 5;
            field_20_map_block_spec =
                gMap_0x370_6F6268->sub_4E0000(gRozza_679188.field_4_mapx_t1, gRozza_679188.field_18_mapy_t1, gRozza_679188.field_1C_mapz);
            break;
        case 2:
            field_0 = 5;
            field_20_map_block_spec =
                gMap_0x370_6F6268->sub_4DFF60(gRozza_679188.field_14_mapx_t2, gRozza_679188.field_C_mapy_t2, gRozza_679188.field_1C_mapz);
            break;
        case 3:
        {
            Car_BC* cBC = gRozza_679188.field_20_pSprite->AsCar_40FEB0();
            if (cBC)
            {
                field_14 = cBC;
                field_0 = 2;
            }
            else if (gRozza_679188.field_20_pSprite->AsCharB4_40FEA0())
            {
                field_0 = 4;
            }
            else
            {
                Object_2C* p2C = gRozza_679188.field_20_pSprite->As2C_40FEC0();

                field_18_model_copy = p2C->get_model_40FEF0();
                if (field_18_model_copy == 166)
                {
                    field_0 = 5;
                    field_20_map_block_spec = p2C->sub_529240();
                }
                else
                {
                    field_0 = 3;
                }
            }
            break;
        }
    }
}

// ================================================================

MATCH_FUNC(0x40bb90)
void Rozza_C88::Reset_40BB90()
{
    field_C84_count = 0;
}

STUB_FUNC(0x40bba0)
void Rozza_C88::sub_40BBA0(Sprite* a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x40bc40)
void Rozza_C88::sub_40BC40(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x40bd10)
void Rozza_C88::sub_40BD10(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x40bdd0)
void Rozza_C88::sub_40BDD0(Sprite* a2, Sprite* a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x40be00)
Rozza_C88::Rozza_C88()
{
    if (bSkip_audio_67D6BE)
    {
        field_0_pSoundObj = NULL;
    }
    else
    {
        field_0_pSoundObj = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 6);
    }
    Reset_40BB90();
}

MATCH_FUNC(0x40be40)
Rozza_C88::~Rozza_C88()
{
    if (field_0_pSoundObj)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_0_pSoundObj);
        field_0_pSoundObj = 0;
    }
}