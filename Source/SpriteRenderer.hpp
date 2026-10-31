#pragma once

#include "Function.hpp"

class Sprite;

// Node of a binary tree of sprites sorted by depth: mpLeft is drawn before the node, mpRight after it
class SpriteTreeNode
{
  public:
    Sprite* mpSprite; // 0x0
    SpriteTreeNode* mpLeft; // 0x4
    SpriteTreeNode* mpRight; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeNode, 0xC);

EXTERN_GLOBAL(class SpriteTreeStack*, gSpriteTreeStack);


// Explicit stack used by SpriteTree::Draw for the in-order walk of a sprite tree
class SpriteTreeStack
{
  public:
    enum
    {
        k_max_nodes = 1000
    };

    SpriteTreeStack() // inlined 4C4BD0
    {
        mpTop = mStack;
    }

    ~SpriteTreeStack()
    {
    }

    // 9.6f inline
    void Push_4C4B80(SpriteTreeNode* pToPush)
    {
        SpriteTreeNode*** ppTop = &this->mpTop;
        *(*ppTop)++ = pToPush;
    }

    // 9.6f inline
    bool IsEnd_4C4BC0() const
    {
        return this->mpTop == mStack;
    }

    // 9.6f inline
    SpriteTreeNode* Pop_4C4BA0()
    {
        SpriteTreeNode*** ppTop = &gSpriteTreeStack->mpTop;
        (*ppTop)--;
        return *this->mpTop;
    }

    SpriteTreeNode* mStack[k_max_nodes]; // 0x0
    SpriteTreeNode** mpTop; // 0xFA0
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeStack, 0xFA4);

// Fixed pool of tree nodes, reset every frame
class SpriteTreeNodePool
{
  public:

    inline SpriteTreeNode* Alloc_4C4B40()
    {
        if (mNextFreeIdx >= SpriteTreeStack::k_max_nodes)
        {
            return NULL;
        }
        else
        {
            SpriteTreeNode* pReturn = &mEntries[mNextFreeIdx];
            mNextFreeIdx++;
            return pReturn;
        }
    }

    // 9.6f 0x4C4B70
    inline void Reset_4C4B70()
    {
        mNextFreeIdx = 0;
    }

    EXPORT SpriteTreeNodePool();
    EXPORT ~SpriteTreeNodePool();
    SpriteTreeNode mEntries[SpriteTreeStack::k_max_nodes]; // 0x0
    u32 mNextFreeIdx; // 0x2EE0
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteTreeNodePool, 0x2EE4);

// One draw layer: a binary tree of the sprites added this frame
class SpriteTree
{
  public:
    EXPORT void AddSprite(Sprite* pSprite);
    EXPORT void Draw();
    EXPORT void Reset();
    EXPORT SpriteTree();
    EXPORT ~SpriteTree();
    SpriteTreeNode* mpRoot; // 0x0
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

    EXPORT void ResetAll();
    EXPORT void DisplayAdd(Sprite* pSprite);
    EXPORT void Draw(s32 layer);
    EXPORT SpriteRenderer();
    EXPORT ~SpriteRenderer();
    SpriteTree* mLayers[k_num_layers]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteRenderer, 0x1C);

EXTERN_GLOBAL(SpriteRenderer*, gSpriteRenderer);

EXTERN_GLOBAL(SpriteTreeNodePool*, gSpriteTreeNodePool);
