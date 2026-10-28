#include "BotDebugDraw.hpp"
#include <string.h>

namespace
{
BotDebugDraw::DebugLine sLines[BotDebugDraw::kMaxLines];
BotDebugDraw::DebugMark sMarks[BotDebugDraw::kMaxMarks];
int sLineCount = 0;
int sMarkCount = 0;
} // namespace

void BotDebugDraw::Clear()
{
    sLineCount = 0;
    sMarkCount = 0;
}

void BotDebugDraw::Line(float x0, float y0, float x1, float y1, unsigned col)
{
    if (sLineCount < kMaxLines)
    {
        DebugLine& l = sLines[sLineCount++];
        l.x0 = x0;
        l.y0 = y0;
        l.x1 = x1;
        l.y1 = y1;
        l.col = col;
    }
}

void BotDebugDraw::Mark(float x, float y, float radius, unsigned col, const char* label)
{
    if (sMarkCount < kMaxMarks)
    {
        DebugMark& m = sMarks[sMarkCount++];
        m.x = x;
        m.y = y;
        m.radius = radius;
        m.col = col;
        strncpy(m.label, label ? label : "", sizeof(m.label) - 1);
        m.label[sizeof(m.label) - 1] = 0;
    }
}

int BotDebugDraw::LineCount()
{
    return sLineCount;
}

const BotDebugDraw::DebugLine& BotDebugDraw::LineAt(int i)
{
    return sLines[i];
}

int BotDebugDraw::MarkCount()
{
    return sMarkCount;
}

const BotDebugDraw::DebugMark& BotDebugDraw::MarkAt(int i)
{
    return sMarks[i];
}
