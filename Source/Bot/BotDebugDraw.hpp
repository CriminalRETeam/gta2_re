#pragma once

// What the bot wants to show on the game view: lines and marks in world tiles that the actions add every frame (the route,
// the aim point, the steering arc, cars to keep clear of) and the overlay draws on top of the picture, see BotUi.cpp.
// Colours are 0xAABBGGRR, as ImGui takes them.

enum
{
    kDebugColCyan = 0xFFE6E600,
    kDebugColYellow = 0xFF00E6FF,
    kDebugColMagenta = 0xFFDC00FF,
    kDebugColGreen = 0xFF50E650,
    kDebugColRed = 0xFF3030FF,
    kDebugColWhite = 0xFFFFFFFF,
    kDebugColOrange = 0xFF1090FF
};

class BotDebugDraw
{
  public:
    enum
    {
        kMaxLines = 400,
        kMaxMarks = 48
    };
    struct DebugLine
    {
        float x0, y0, x1, y1;
        unsigned col;
    };
    struct DebugMark
    {
        float x, y;
        float radius; // tiles
        unsigned col;
        char label[20];
    };
    static void Clear();
    static void Line(float x0, float y0, float x1, float y1, unsigned col);
    // a ring of the given radius (in tiles) with a label
    static void Mark(float x, float y, float radius, unsigned col, const char* label);
    static int LineCount();
    static const DebugLine& LineAt(int i);
    static int MarkCount();
    static const DebugMark& MarkAt(int i);
};
