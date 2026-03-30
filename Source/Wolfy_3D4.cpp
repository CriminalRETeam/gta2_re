#include "Wolfy_3D4.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Particle_4C.hpp"
#include "Particle_8.hpp"
#include "PurpleDoom.hpp"
#include "rng.hpp"
#include "debug.hpp"

DEFINE_GLOBAL(Wolfy_7A8*, gWolfy_7A8_6FD5F0, 0x6FD5F0);
DEFINE_GLOBAL(Wolfy_3D4*, gWolfy_3D4_6FD5EC, 0x6FD5EC);

EXTERN_GLOBAL(u16, gParticleInstCount_6FD5F4);

DEFINE_GLOBAL(Fix16, dword_6FD49C, 0x6FD49C);
DEFINE_GLOBAL(s16, word_6FD5D4, 0x6FD5D4);

DEFINE_GLOBAL(Ang16, word_6FD3EE, 0x6FD3EE);
DEFINE_GLOBAL(Fix16, dword_6FD330, 0x6FD330);

DEFINE_GLOBAL_INIT(s16, gWolfyId_40_pool_623F18, 1, 0x623F18);

STUB_FUNC(0x543690)
s32 Wolfy_7A8::sub_543690()
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x543800)
Wolfy_30* Wolfy_7A8::New_40_543800()
{
    WIP_IMPLEMENTED;

    s32 k20Idx = 0;
    s32 k40Idx;
    s32 _20IdxCopy = 0;
    s32 idx_to_use;

    while (1)
    {
        idx_to_use = _20IdxCopy;
        if (!this->field_780_bUsed[_20IdxCopy])
        {
            // Found a free entry
            break;
        }

        _20IdxCopy = ++k20Idx;
        if (k20Idx >= 20u)
        {
            sub_543690();
            k20Idx = 0;
            k40Idx = 0;
            while (1)
            {
                idx_to_use = k40Idx;
                if (!this->field_780_bUsed[k40Idx])
                {
                    break;
                }
                k40Idx = ++k20Idx;
                if (k20Idx >= 40u)
                {
                    return 0;
                }
            }
            break;
        }
    }

    Wolfy_30* pNew = &this->field_0[idx_to_use];
    pNew->Init_543650();
    pNew->field_4_idx = k20Idx;
    pNew->field_6_id = gWolfyId_40_pool_623F18;
    pNew->field_0_bIn20Pool = 0;
    gWolfyId_40_pool_623F18++;
    this->field_780_bUsed[idx_to_use] = 1;
    return pNew;
}

MATCH_FUNC(0x5438b0)
Wolfy_7A8::Wolfy_7A8()
{
    for (u8 i = 0; i < 40; i++)
    {
        field_0[i].field_4_idx = i;
        field_780_bUsed[i] = 0;
    }

    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5438f0)
Wolfy_7A8::~Wolfy_7A8()
{
}

MATCH_FUNC(0x5408f0)
Wolfy_30::Wolfy_30()
{
    this->field_C = 0;
    this->field_20 = 0;
    this->field_22 = 0;
    this->field_4_idx = 0;
    this->field_10_type_or_state = 0;
    this->field_14 = 0;
    this->field_18 = 0;
    this->field_1A = 0;
    this->field_8 = dword_6FD49C;
    this->field_20 = word_6FD5D4;
    this->field_22 = word_6FD5D4;
    this->field_24 = dword_6FD49C;
    this->field_28 = dword_6FD49C;
    this->field_C = word_6FD5D4;
    this->field_1C = 0;
    this->field_6_id = 0;
    this->field_2C_ped_id = 0;
}

MATCH_FUNC(0x540a10)
Wolfy_30::~Wolfy_30()
{
    field_14 = 0;
    field_1C = 0;
}

STUB_FUNC(0x540a40)
void Wolfy_30::sub_540A40()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x540d30)
void Wolfy_30::state_3_12_540D30(Fix16 a3, Ang16 a2)
{
    WIP_IMPLEMENTED;

    Ang16 v6 = word_6FD3EE + a2;

    Fix16 v32 = ((a3 * gCos_table_669260[v6.rValue]) + (a3 * gSin_table_667A80[v6.rValue]));
    Fix16 v13 = ((-a3 * gSin_table_667A80[v6.rValue]) + (a3 * a3));

    this->field_8 = a3;
    this->field_C = a2;

    if (field_18)
    {
        field_18--;
    }
    else
    {
        //a3 = (int)&v27;
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(v32, v13, dword_6FD330, v32, v13, 0);
        if (!pParticle)
        {
            return;
        }
        Sprite* pNext = pParticle->field_30_pNext;
        pParticle->field_40_pUnknown = this;
        pParticle->field_20 = a3; // F16
        pParticle->field_44 = field_6_id;
        pParticle->field_24 = a2;
        pParticle->field_34 = 0;
        pParticle->field_46_sub_state = 0;
        pParticle->field_2C_counter = 32;
        pParticle->field_2E = 32;
        pNext->field_30_sprite_type_enum = 8;
        pNext->sub_59E960();
        pParticle->field_38_state = 3;

        Sprite* v19 = pParticle->field_30_pNext;

        v19->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 96);

        pParticle->field_30_pNext->field_2C |= 4u;
        Sprite* v21 = pParticle->field_30_pNext;
        Sprite* v22 = this->field_14->field_4;

        v21->set_xyz_lazy_420600(v22->field_14_xy.x, v22->field_14_xy.y, v22->field_1C_zpos);

        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
        field_18 = stru_6F6784.get_int_4F7AE0(2);
    }
}

STUB_FUNC(0x540f90)
void Wolfy_30::state_4_540F90(Fix16 a2, Ang16 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5411e0)
void Wolfy_30::state_13_14_5411E0(Fix16 a2, Ang16 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x541430)
void Wolfy_30::state_5_541430(Fix16 a2, Ang16 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x541680)
u32* Wolfy_30::sub_541680(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x541710)
u32* Wolfy_30::sub_541710(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x541760)
char_type Wolfy_30::sub_541760()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x541850)
void Wolfy_30::sub_541850(u16 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x541d60)
char_type Wolfy_30::sub_541D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x542060)
char_type Wolfy_30::sub_542060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x542340)
char_type Wolfy_30::sub_542340()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x542790)
s16 Wolfy_30::state_18_19_20_32_33_542790()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x542e30)
void Wolfy_30::state_22_23_24_25_542E30(char_type a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x5434a0)
char_type Wolfy_30::Update_5434A0(Fix16 a2, Ang16 a3)
{
    WIP_IMPLEMENTED;

    s16 timer = this->field_1A;
    if (timer != 9999)
    {
        if (timer)
        {
            this->field_1A = timer - 1;
        }
    }

    char_type result;
    if (this->field_1A)
    {
        if (bSkip_particles_67D64D)
        {
            return 1;
        }
        else
        {
            switch (this->field_10_type_or_state)
            {
                case 3:
                case 12:
                    Wolfy_30::state_3_12_540D30( a2, a3);
                    result = 0;
                    break;
                case 4:
                    Wolfy_30::state_4_540F90(a2, a3);
                    result = 0;
                    break;
                case 5:
                    Wolfy_30::state_5_541430(a2, a3);
                    result = 0;
                    break;
                case 13:
                case 14:
                    Wolfy_30::state_13_14_5411E0(a2, a3);
                    result = 0;
                    break;
                case 18:
                case 19:
                case 20:
                case 32:
                case 33:
                    Wolfy_30::state_18_19_20_32_33_542790();
                    result = 0;
                    break;
                case 22:
                    Wolfy_30::state_22_23_24_25_542E30(3);
                    goto LABEL_18;
                case 23:
                    Wolfy_30::state_22_23_24_25_542E30(2);
                    result = 0;
                    break;
                case 24:
                    Wolfy_30::state_22_23_24_25_542E30(0);
                    result = 0;
                    break;
                case 25:
                    Wolfy_30::state_22_23_24_25_542E30(1);
                    result = 0;
                    break;
                default:
                LABEL_18:
                    result = 0;
                    break;
            }
        }
    }
    else
    {
        Wolfy_30::DeInit_543610();
        return 1;
    }
    return result;
}

MATCH_FUNC(0x5435d0)
char_type Wolfy_30::IsState_5435D0()
{
    switch (field_10_type_or_state)
    {
        case 3:
        case 4:
        case 5:
        case 12:
        case 13:
        case 14:
            return 1;
        default:
            return 0;
    }
}

MATCH_FUNC(0x543610)
void Wolfy_30::DeInit_543610()
{
    this->field_6_id = 0;
    if (field_0_bIn20Pool == 0)
    {
        gWolfy_7A8_6FD5F0->field_780_bUsed[this->field_4_idx] = 0;
    }
    else
    {
        gWolfy_3D4_6FD5EC->field_3C0_bUsed[this->field_4_idx] = 0;
    }
}

MATCH_FUNC(0x543650)
void Wolfy_30::Init_543650()
{
    this->field_10_type_or_state = 0;
    this->field_18 = 0;
    this->field_24 = 0;
    this->field_22 = word_6FD5D4;
    this->field_1A = 200;
    this->field_14 = 0;
    this->field_2C_ped_id = 0;
    this->field_0_bIn20Pool = 0;
}

MATCH_FUNC(0x543680)
void Wolfy_30::Set_Obj2C_543680(Object_2C* a2)
{
    this->field_14 = a2;
}

MATCH_FUNC(0x543980)
Wolfy_3D4::Wolfy_3D4()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_3C0_bUsed); i++)
    {
        field_0[i].field_4_idx = i;
        field_3C0_bUsed[i] = 0;
    }
    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5439c0)
Wolfy_3D4::~Wolfy_3D4()
{
}