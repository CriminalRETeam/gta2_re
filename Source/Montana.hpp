#pragma once

#include "Function.hpp"

#include <stddef.h>

class Sprite;

// SpriteLayerRecord ?
class Montana_C
{
  public:
    Sprite* field_0_sprt;
    Montana_C* mpNext;
    Montana_C* field_8;
};

EXTERN_GLOBAL(class Montana_FA4*, gMontana_FA4_705BC0);


// Something to do with the order that sprites are drawn? Not sure almost seems redundant
// SpriteLayerSorter ?
class Montana_FA4
{
  public:
    Montana_FA4() // inlined 4C4BD0
    {
        field_FA0 = field_0;
    }

    ~Montana_FA4()
    {
    }

    // 9.6f inline
    void Push_4C4B80(Montana_C* pToPush)
    {
        Montana_C*** pOld = &this->field_FA0;
        *(*pOld)++ = pToPush;
    }

    // 9.6f inline
    bool IsEnd_4C4BC0() const
    {
        return this->field_FA0 == field_0;
    }

    // 9.6f inline
    Montana_C* Pop_4C4BA0()
    {
        Montana_C*** pOld = &gMontana_FA4_705BC0->field_FA0;
        (*pOld)--;
        return *this->field_FA0;
    }

    Montana_C* field_0[1000];
    Montana_C** field_FA0;
};

// SpriteList ?
class Montana_2EE4
{
  public:

    inline Montana_C* sub_4C4B40()
    {
        if (field_2EE0_free_indx >= 1000)
        {
            return NULL;
        }
        else
        {
            Montana_C* pReturn = &field_0[field_2EE0_free_indx];
            field_2EE0_free_indx++;
            return pReturn;
        }
    }

    EXPORT Montana_2EE4();
    EXPORT ~Montana_2EE4();
    Montana_C field_0[1000];
    s32 field_2EE0_free_indx;
};

// SpriteLayer ?
class Montana_4
{
  public:
    EXPORT void AddSprite_5C5CF0(Sprite* pSprite);
    EXPORT void Draw_5C5DF0();
    EXPORT void Reset_5C5E50();
    EXPORT Montana_4();
    EXPORT ~Montana_4();
    Montana_C* field_0_pFirst;
};


// SpriteRenderer ?
class Montana
{
  public:
    EXPORT void ResetAll_4954F0();
    EXPORT void DisplayAdd_495510(Sprite* pSprite);
    EXPORT void Draw_495560(s32 col_idx);
    EXPORT Montana();
    EXPORT ~Montana();
    Montana_4* field_0_cols[7];
};

EXTERN_GLOBAL(Montana*, gMontana_67B580);

EXTERN_GLOBAL(Montana_2EE4*, gMontana_2EE4_705BBC);
