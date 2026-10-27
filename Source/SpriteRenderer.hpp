#pragma once

#include "Function.hpp"

class Sprite;

// Node of a binary tree of sprites sorted by depth: field_4_pLeft is drawn before the node, field_8_pRight after it
class SpriteTreeNode
{
  public:
    Sprite* field_0_pSprite;
    SpriteTreeNode* field_4_pLeft;
    SpriteTreeNode* field_8_pRight;
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeNode, 0xC);

EXTERN_GLOBAL(class SpriteTreeStack*, gSpriteTreeStack_705BC0);


// Explicit stack used by SpriteTree::Draw_5C5DF0 for the in-order walk of a sprite tree
class SpriteTreeStack
{
  public:
    enum
    {
        k_max_nodes = 1000
    };

    SpriteTreeStack() // inlined 4C4BD0
    {
        field_FA0_pTop = field_0_stack;
    }

    ~SpriteTreeStack()
    {
    }

    // 9.6f inline
    void Push_4C4B80(SpriteTreeNode* pToPush)
    {
        SpriteTreeNode*** ppTop = &this->field_FA0_pTop;
        *(*ppTop)++ = pToPush;
    }

    // 9.6f inline
    bool IsEnd_4C4BC0() const
    {
        return this->field_FA0_pTop == field_0_stack;
    }

    // 9.6f inline
    SpriteTreeNode* Pop_4C4BA0()
    {
        SpriteTreeNode*** ppTop = &gSpriteTreeStack_705BC0->field_FA0_pTop;
        (*ppTop)--;
        return *this->field_FA0_pTop;
    }

    SpriteTreeNode* field_0_stack[k_max_nodes];
    SpriteTreeNode** field_FA0_pTop;
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeStack, 0xFA4);

// Fixed pool of tree nodes, reset every frame
class SpriteTreeNodePool
{
  public:

    inline SpriteTreeNode* Alloc_4C4B40()
    {
        if (field_2EE0_next_free_idx >= SpriteTreeStack::k_max_nodes)
        {
            return NULL;
        }
        else
        {
            SpriteTreeNode* pReturn = &field_0_entries[field_2EE0_next_free_idx];
            field_2EE0_next_free_idx++;
            return pReturn;
        }
    }

    // 9.6f 0x4C4B70
    inline void Reset_4C4B70()
    {
        field_2EE0_next_free_idx = 0;
    }

    EXPORT SpriteTreeNodePool();
    EXPORT ~SpriteTreeNodePool();
    SpriteTreeNode field_0_entries[SpriteTreeStack::k_max_nodes];
    u32 field_2EE0_next_free_idx;
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeNodePool, 0x2EE4);

// One draw layer: a binary tree of the sprites added this frame
class SpriteTree
{
  public:
    EXPORT void AddSprite_5C5CF0(Sprite* pSprite);
    EXPORT void Draw_5C5DF0();
    EXPORT void Reset_5C5E50();
    EXPORT SpriteTree();
    EXPORT ~SpriteTree();
    SpriteTreeNode* field_0_pRoot;
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTree, 0x4);


// The 7 draw layers (by sprite z), sprites are added with DisplayAdd and drawn layer by layer from MapRenderer
class SpriteRenderer
{
  public:
    enum
    {
        k_num_layers = 7
    };

    EXPORT void ResetAll_4954F0();
    EXPORT void DisplayAdd_495510(Sprite* pSprite);
    EXPORT void Draw_495560(s32 layer);
    EXPORT SpriteRenderer();
    EXPORT ~SpriteRenderer();
    SpriteTree* field_0_layers[k_num_layers];
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteRenderer, 0x1C);

EXTERN_GLOBAL(SpriteRenderer*, gSpriteRenderer_67B580);

EXTERN_GLOBAL(SpriteTreeNodePool*, gSpriteTreeNodePool_705BBC);
