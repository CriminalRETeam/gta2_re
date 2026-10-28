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

void BotDebugDraw::Box(float cx, float cy, float ax, float ay, float hl, float hw, unsigned col)
{
    const float px = -ay;
    const float py = ax;
    float xs[4];
    float ys[4];
    xs[0] = cx + ax * hl + px * hw;
    ys[0] = cy + ay * hl + py * hw;
    xs[1] = cx + ax * hl - px * hw;
    ys[1] = cy + ay * hl - py * hw;
    xs[2] = cx - ax * hl - px * hw;
    ys[2] = cy - ay * hl - py * hw;
    xs[3] = cx - ax * hl + px * hw;
    ys[3] = cy - ay * hl + py * hw;
    for (int i = 0; i < 4; i++)
    {
        Line(xs[i], ys[i], xs[(i + 1) % 4], ys[(i + 1) % 4], col);
    }
}

void BotDebugDraw::TileOutline(float x, float y, unsigned col)
{
    const float tx = static_cast<float>(static_cast<int>(x));
    const float ty = static_cast<float>(static_cast<int>(y));
    Line(tx, ty, tx + 1.0f, ty, col);
    Line(tx + 1.0f, ty, tx + 1.0f, ty + 1.0f, col);
    Line(tx + 1.0f, ty + 1.0f, tx, ty + 1.0f, col);
    Line(tx, ty + 1.0f, tx, ty, col);
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
