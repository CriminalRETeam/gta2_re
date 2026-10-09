#pragma once

#include "Function.hpp"

class Sprite;

// Node of a binary tree of sprites sorted by depth: field_4_pLeft is drawn before the node, field_8_pRight after it
class SpriteTreeNode_C
{
  public:
    Sprite* field_0_pSprite;
    SpriteTreeNode_C* field_4_pLeft;
    SpriteTreeNode_C* field_8_pRight;
};

EXTERN_GLOBAL(class SpriteTreeStack_FA4*, gSpriteTreeStack_705BC0);


// Explicit stack used by SpriteTree_4::Draw_5C5DF0 for the in-order walk of a sprite tree
class SpriteTreeStack_FA4
{
  public:
    enum
    {
        k_max_nodes = 1000
    };

    SpriteTreeStack_FA4() // inlined 4C4BD0
    {
        field_FA0_pTop = field_0_stack;
    }

    ~SpriteTreeStack_FA4()
    {
    }

    // 9.6f inline
    void Push_4C4B80(SpriteTreeNode_C* pToPush)
    {
        SpriteTreeNode_C*** pOld = &this->field_FA0_pTop;
        *(*pOld)++ = pToPush;
    }

    // 9.6f inline
    bool IsEnd_4C4BC0() const
    {
        return this->field_FA0_pTop == field_0_stack;
    }

    // 9.6f inline
    SpriteTreeNode_C* Pop_4C4BA0()
    {
        SpriteTreeNode_C*** pOld = &gSpriteTreeStack_705BC0->field_FA0_pTop;
        (*pOld)--;
        return *this->field_FA0_pTop;
    }

    SpriteTreeNode_C* field_0_stack[k_max_nodes];
    SpriteTreeNode_C** field_FA0_pTop;
};

// Fixed pool of tree nodes, reset every frame
class SpriteTreeNodePool_2EE4
{
  public:

    inline SpriteTreeNode_C* Alloc_4C4B40()
    {
        if (field_2EE0_next_free_idx >= SpriteTreeStack_FA4::k_max_nodes)
        {
            return NULL;
        }
        else
        {
            SpriteTreeNode_C* pReturn = &field_0_entries[field_2EE0_next_free_idx];
            field_2EE0_next_free_idx++;
            return pReturn;
        }
    }

    // 9.6f 0x4C4B70
    inline void Reset_4C4B70()
    {
        field_2EE0_next_free_idx = 0;
    }

    EXPORT SpriteTreeNodePool_2EE4();
    EXPORT ~SpriteTreeNodePool_2EE4();
    SpriteTreeNode_C field_0_entries[SpriteTreeStack_FA4::k_max_nodes];
    u32 field_2EE0_next_free_idx;
};

// One draw layer: a binary tree of the sprites added this frame
class SpriteTree_4
{
  public:
    EXPORT void AddSprite_5C5CF0(Sprite* pSprite);
    EXPORT void Draw_5C5DF0();
    EXPORT void Reset_5C5E50();
    EXPORT SpriteTree_4();
    EXPORT ~SpriteTree_4();
    SpriteTreeNode_C* field_0_pRoot;
};


// The 7 draw layers (by sprite z), sprites are added with DisplayAdd and drawn layer by layer from MapRenderer
class SpriteRenderer_1C
{
  public:
    enum
    {
        k_num_layers = 7
    };

    EXPORT void ResetAll_4954F0();
    EXPORT void DisplayAdd_495510(Sprite* pSprite);
    EXPORT void Draw_495560(s32 layer);
    EXPORT SpriteRenderer_1C();
    EXPORT ~SpriteRenderer_1C();
    SpriteTree_4* field_0_layers[k_num_layers];
};

EXTERN_GLOBAL(SpriteRenderer_1C*, gSpriteRenderer_67B580);

EXTERN_GLOBAL(SpriteTreeNodePool_2EE4*, gSpriteTreeNodePool_705BBC);
