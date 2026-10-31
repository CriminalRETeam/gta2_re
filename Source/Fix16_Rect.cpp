#include "Fix16_Rect.hpp"
#include "map_0x370.hpp"
#include "SpriteGrid.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL_INIT(Fix16, kSmallWidthEpslion_703450, Fix16(0xCCC, 0), 0x703450);
DEFINE_GLOBAL_INIT(Fix16, kCollisionPrismHalfHeight_6771E4, Fix16(0x800, 0), 0x6771E4);

MATCH_FUNC(0x4E6280)
void Fix16_Rect::MakeRect(Fix16 x, Fix16 y, Fix16 w, Fix16 h)
{
    mLeft = x;
    mTop = y;
    mRight = x + w;
    mBottom = y + h;
}

MATCH_FUNC(0x59dd60)
void Fix16_Rect::DoSetCurrentRect()
{
    gSpriteGrid_left_6F5FD4 = mLeft.ToInt();
    gSpriteGrid_right_6F5B80 = mRight.ToInt();
    gSpriteGrid_top_6F6108 = mTop.ToInt();
    gSpriteGrid_bottom_6F5F38 = mBottom.ToInt();
}

// 0x4BA6C0 9.6f
MATCH_FUNC(0x59ddf0)
bool Fix16_Rect::IntersectsSpriteRenderingRect(Sprite* pSprite)
{
    Fix16_Point* pCorners = pSprite->mpCollisionShape->mCorners;

    return (IsPointInRect_4B9FD0(&pCorners[0]) || IsPointInRect_4B9FD0(&pCorners[1]) 
        || IsPointInRect_4B9FD0(&pCorners[2]) || IsPointInRect_4B9FD0(&pCorners[3]));
}

MATCH_FUNC(0x59de80)
char_type Fix16_Rect::IsRectBlockedByMap()
{
    DoSetCurrentRect();
    s32 x = ((this->mLeft + this->mRight) / 2).ToInt();
    s32 y = ((this->mTop + this->mBottom) / 2).ToInt();
    s32 z = ((this->mLowZ + this->mHighZ) / 2).ToInt();
    return gMap_0x370_6F6268->IsMovementRegionBlocked(x, y, z, NULL, 1024);
}

MATCH_FUNC(0x59dee0)
void Fix16_Rect::ExpandToIncludePoint(Fix16 x, Fix16 y)
{
    if (x < mLeft)
    {
        mLeft = x;
    }
    else
    {
        if (x > mRight)
        {
            mRight = x;
        }
    }

    if (y < mTop)
    {
        mTop = y;
    }
    else
    {
        if (y > mBottom)
        {
            mBottom = y;
        }
    }
}

MATCH_FUNC(0x59df20)
bool Fix16_Rect::RectOverlapsZone(u8 zone_type)
{
    u8 zone_y = mTop.ToUInt8();
    u8 zone_x;

    while (zone_y <= mBottom.ToUInt8())
    {
        zone_x = mLeft.ToUInt8();
        while (zone_x <= mRight.ToUInt8())
        {
            if (gMap_0x370_6F6268->GetZoneAtPosOfType(zone_x, zone_y, zone_type))
            {
                return true;
            }
            ++zone_x;
        }
        ++zone_y;
    }
    return false;
}

MATCH_FUNC(0x59dfb0)
bool Fix16_Rect::EveryTileMatchesArrowType(s32 arrow_type)
{
    u8 y_pos = mTop.ToUInt8();
    u8 x_pos;

    bool bMatches = true;

    while (y_pos <= mBottom.ToUInt8())
    {
        x_pos = mLeft.ToUInt8();
        while (x_pos <= mRight.ToUInt8())
        {
            if (!bMatches)
            {
                return false;
            }

            gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock(x_pos, y_pos, mLowZ.ToInt());
            if (pBlockAtPos)
            {
                
                switch (arrow_type)
                {
                    case 1:
                        bMatches = (pBlockAtPos->mArrows & block_arrow::green_mask) == block_arrow::green_up_4;  //  green arrow up
                        break;
                    case 2:
                        bMatches = (pBlockAtPos->mArrows & block_arrow::green_mask) == block_arrow::green_down_8;  //  green arrow down
                        break;
                    case 3:
                        bMatches = (pBlockAtPos->mArrows & block_arrow::green_mask) == block_arrow::green_right_2;  //  green arrow right
                        break;
                    case 4:
                        bMatches = (pBlockAtPos->mArrows & block_arrow::green_mask) == block_arrow::green_left_1;  //  green arrow left
                        break;
                    default:
                        break;
                }
            }
            else
            {
                bMatches = false;
            }
            ++x_pos;
        }
        ++y_pos;
    }
    return true;
}

MATCH_FUNC(0x5A5E30)
void Fix16_Rect::SetRect_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom)
{
    this->mLeft = left;
    this->mRight = right;
    this->mTop = top;
    this->mBottom = bottom;
}