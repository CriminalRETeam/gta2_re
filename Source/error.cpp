#include "Function.hpp"
#include "error.hpp"
#include "winmain.hpp"
#include "fix16.hpp"
#include <windows.h>
#include <stdio.h>
#include <fstream>

char bDestroyed_6F5B70;
HWND gHwnd_707F04;

class ErrorLog : public std::fstream
{
public:
    ErrorLog& Write_4D9620(const char* pMsg);
};

ErrorLog& ErrorLog::Write_4D9620(const char* pMsg)
{
    return *this;
}

ErrorLog gErrorLog_67CF58;

char gTmpBuffer_67C598[256]; // TODO: Check
char gErrStr_67C29C[256];
char byte_67C3A8[256];
char gGlobalFileName_67C6AC[256];

const char *gListTypes_61AB70[30] =
{
  "objects",
  "corner_space",
  "sprite_space",
  "collision_space",
  "sparse_space",
  "car_space",
  "miss_space",
  "ped_space",
  "character_space",
  "mission_stack_space",
  "charlist_space",
  "animating_object_space",
  "moving_object_space",
  "unused",
  "rigid_body_space",
  "tile_anim_space",
  "sid_space",
  "weapon_space",
  "map_door_space",
  "unused",
  "unused",
  "carcontrol_space",
  "unused",
  "artic_space",
  "sprlist_space",
  "unused",
  "light_space",
  "particle_space",
  "scorefx_space",
  "audio_info_space"
};

// stub
void sub_4DA740()
{

}

MATCH_FUNC(0x4A0770)
void __stdcall Error_SetName_4A0770(const char *pFileName)
{
    strcpy(gGlobalFileName_67C6AC, pFileName);
}

MATCH_FUNC(0x4A07A0)
const char* __stdcall SourceFileNameFromPath_4A07A0(const char *pPath)
{
    // ecx was swapped with edx without using the local pIter
    const char *result = 0;
    const char* pIter = pPath;
    while (*pIter)
    {
        if (*pIter == '\\')
        {
            result = pIter + 1;
        }
        pIter++;
    }
    return result;
}

struct Coord
{
    Fix16 x;
    Fix16 y;
    Fix16 z;
};

struct Coord2
{
    Fix16 x;
    Fix16 y;
};

#define err_a1_int(msg, arg) sprintf(gTmpBuffer_67C598, msg, va_1);

// nomatch
void FatalError_4A07C0(int code, const char *pFileName, int lineNo, ...)
{
    va_list va; // [esp+94h] [ebp+34h] BYREF

    va_start(va, lineNo);

    sprintf(gTmpBuffer_67C598, "Fatal Error %d in %s(%d)", code, SourceFileNameFromPath_4A07A0(pFileName), lineNo);
    gErrorLog_67CF58.Write_4D9620(gTmpBuffer_67C598);

    switch (code)
    {
    case 2:
        sprintf(gTmpBuffer_67C598, "Can't Init DMAVideo.");
        break;

    case 72:
        sprintf(gTmpBuffer_67C598, "security failure");
        break;

    case 5:
        sprintf(gTmpBuffer_67C598, "Failed to set WindowMode");
        break;

    case 6:
        sprintf(gTmpBuffer_67C598, "Failed to get a valid HDC");
        break;

    case 7:
        sprintf(gTmpBuffer_67C598, "Failed to SetWindowPos properly");
        break;

    case 8:
        sprintf(gTmpBuffer_67C598, "Failed to Initialise DirectInput");
        break;

    case 9:
        sprintf(gTmpBuffer_67C598, "Failed to set Data Format for DirectInput");
        break;

    case 10:
        sprintf(gTmpBuffer_67C598, "Failed to set Cooperative Level for DirectInput");
        break;

    case 30:
        sprintf(gTmpBuffer_67C598, "No DirectInput Object");
        break;

    case 31:
        sprintf(gTmpBuffer_67C598, "Failed to create DirectInput object");
        break;

    case 28:
        strcpy(gTmpBuffer_67C598, "Attempting to allocate zero bytes of memory");
        break;

    case 13:
        sprintf(gTmpBuffer_67C598, "Ftell error in file: %s", gGlobalFileName_67C6AC);
        break;

    case 15:
        sprintf(gTmpBuffer_67C598, "Read failure in file: %s", gGlobalFileName_67C6AC);
        break;

    case 16:
        sprintf(
            gTmpBuffer_67C598,
            "Install Next freeloader episode please. [Temp message].Unable to open file : %s",
            gGlobalFileName_67C6AC);
        break;

    case 17:
        sprintf(gTmpBuffer_67C598, "Unable to close file: %s", gGlobalFileName_67C6AC);
        break;

    case 18:
        sprintf(gTmpBuffer_67C598, "Trying to a load a file too big for buffer: %s", gGlobalFileName_67C6AC);
        break;

    case 19:
        sprintf(gTmpBuffer_67C598, "Writing zero bytes to file : %s", gGlobalFileName_67C6AC);
        break;

    case 20:
        sprintf(gTmpBuffer_67C598, "Write failure on file : %s", gGlobalFileName_67C6AC);
        break;

    case 21:
        strcpy(gTmpBuffer_67C598, "Attempt to read from not opened file");
        break;

    case 24:
        sprintf(gTmpBuffer_67C598, "ungetc failure in file %s", gGlobalFileName_67C6AC);
        break;

    case 25:
        sprintf(gTmpBuffer_67C598, "invalid integer encountered in file %s", gGlobalFileName_67C6AC);
        break;

    case 26:
        sprintf(gTmpBuffer_67C598, "End of file in text string in file %s", gGlobalFileName_67C6AC);
        break;

    case 27:
        strcpy(gTmpBuffer_67C598, "Text string too long");
        break;

    case 32:
        strcpy(gTmpBuffer_67C598, "Out of memory - new operator failed");
        break;

    case 34:
        strcpy(gTmpBuffer_67C598, "file overflow");
        break;

    case 35:
        strcpy(gTmpBuffer_67C598, "data not loaded");
        break;

    case 36:
        sprintf(gTmpBuffer_67C598, "Invalid string in parameter list: %s", gGlobalFileName_67C6AC);
        break;

    case 37:
        sprintf(gTmpBuffer_67C598, "Repeated file in parameter list : %s", gGlobalFileName_67C6AC);
        break;

    case 39:
        strcpy(gTmpBuffer_67C598, "Invalid zone data");
        break;

    case 40:
        strcpy(gTmpBuffer_67C598, "Invalid map object data");
        break;

    case 51:
        strcpy(gTmpBuffer_67C598, "Invalid map light data");
        break;

    case 41:
        strcpy(gTmpBuffer_67C598, "Invalid loaded object info data");
        break;

    case 42:
        strcpy(gTmpBuffer_67C598, "unable to close registry key");
        break;

    case 43:
        strcpy(gTmpBuffer_67C598, "unable to create registry key");
        break;

    case 44:
        strcpy(gTmpBuffer_67C598, "No players");
        break;

    case 45:
        strcpy(gTmpBuffer_67C598, "Class not allocated");
        break;

    case 46:
        strcpy(gTmpBuffer_67C598, "Unable to set registry value");
        break;

    case 47:
        strcpy(gTmpBuffer_67C598, "Loop in tree");
        break;

    case 48:
        strcpy(gTmpBuffer_67C598, "Out of space for display tree");
        break;

    case 50:
        sprintf(gTmpBuffer_67C598, "Failed writing a byte to file %s", gGlobalFileName_67C6AC);
        break;

    case 53:
        strcpy(gTmpBuffer_67C598, "Using uninitialised sprite");
        break;

    case 54:
        sprintf(gTmpBuffer_67C598, "Invalid corners");
        break;

    case 55:
        sprintf(gTmpBuffer_67C598, "Failed to allocate memory for new mission process");
        break;

    case 56:
        sprintf(gTmpBuffer_67C598, "No more process slots available for mission processes");
        break;

    case 59:
        strcpy(gTmpBuffer_67C598, "check walls slope invalid slope");
        break;

    case 66:
        sprintf(gTmpBuffer_67C598, "Too many PLAYER_PED declarations in scriptfile");
        break;

    case 65:
        sprintf(gTmpBuffer_67C598, "No LEVELSTART declaration in scriptfile");
        break;

    case 69:
        sprintf(gTmpBuffer_67C598, "Failed to create a ped in scriptfile");
        break;

    case 67:
        strcpy(gTmpBuffer_67C598, "map change overflow - too many new blocks");
        break;

    case 68:
        strcpy(gTmpBuffer_67C598, "map change overflow - too many new columns");
        break;

    case 70:
        strcpy(gTmpBuffer_67C598, "Failed to allocate memory for mission stack");
        break;

    case 71:
        strcpy(gTmpBuffer_67C598, "Tried to push NULL item onto mission stack");
        break;

    case 73:
        strcpy(gTmpBuffer_67C598, "Failed grabbing miss_stack_item - no stack space left");
        break;

    case 74:
        strcpy(gTmpBuffer_67C598, "can't remove character from charlist");
        break;

    case 75:
        strcpy(gTmpBuffer_67C598, "Invalid door");
        break;

    case 76:
        strcpy(gTmpBuffer_67C598, "Failed to create a car in scriptfile.");
        break;

    case 77:
        strcpy(gTmpBuffer_67C598, "Object has data when it shouldn't");
        break;

    case 91:
        sprintf(gTmpBuffer_67C598, "Sid has negative references");
        break;

    case 90:
        sprintf(gTmpBuffer_67C598, "Sid has too many references");
        break;

    case 92:
        strcpy(gTmpBuffer_67C598, "No free tiles");
        break;

    case 93:
        sprintf(gTmpBuffer_67C598, "Invalid file type in %s", gGlobalFileName_67C6AC);
        break;

    case 94:
        sprintf(gTmpBuffer_67C598, "Invalid file version in %s", gGlobalFileName_67C6AC);
        break;

    case 61:
    case 81:
        sprintf(gTmpBuffer_67C598, "Character has no ped pointer, bad ");
        break;

    case 64:
        sprintf(gTmpBuffer_67C598, "Invalid objective coordinates");
        break;

    case 60:
        sprintf(gTmpBuffer_67C598, "No Player pointer");
        break;

    case 79:
    case 80:
        sprintf(gTmpBuffer_67C598, "Character has no car objective pointer,very bad");
        break;

    case 97:
        sprintf(gTmpBuffer_67C598, "Character has no in_car pointer, bad");
        break;

    case 62:
        sprintf(gTmpBuffer_67C598, "Character Default Case error");
        break;

    case 85:
        sprintf(gTmpBuffer_67C598, "Ped Default Case error");
        break;

    case 88:
        sprintf(gTmpBuffer_67C598, "Ped Sprite Z is 0, not good");
        break;

    case 89:
        sprintf(gTmpBuffer_67C598, "Peds Car Info is NULL , error indeed");
        break;

    case 82:
        sprintf(gTmpBuffer_67C598, "Group member not removed");
        break;
    case 83:
        sprintf(gTmpBuffer_67C598, "Too many members for the group to manage");
        break;
    case 84:
        sprintf(gTmpBuffer_67C598, "Cant create a group too many in use");
        break;

    case 95:
        sprintf(gTmpBuffer_67C598, "Character not got a foot or car pointer, very bad");
        break;

    case 96:
        sprintf(gTmpBuffer_67C598, "Unable to Create A Medical Unit");
        break;

    case 98:
        strcpy(gTmpBuffer_67C598, "Too many traffic lights");
        break;

    case 99:
        strcpy(gTmpBuffer_67C598, "Too many information & navigation zones in map");
        break;

    case 100:
        strcpy(gTmpBuffer_67C598, "Invalid zone for information setting");
        break;

    case 101:
        strcpy(gTmpBuffer_67C598, "Failed to create car in recycle.");
        break;

    case 102:
        strcpy(gTmpBuffer_67C598, "Empty String passed into function");
        break;

    case 103:
        strcpy(gTmpBuffer_67C598, "Got NULL ptr for first player");
        break;

    case 104:
        strcpy(gTmpBuffer_67C598, "Got NULL ptr for next player");
        break;

    case 49:
        strcpy(gTmpBuffer_67C598, "divide by zero");
        break;

    case 105:
        strcpy(gTmpBuffer_67C598, "NULL block passed to check for arrows in routefinder code");
        break;

    case 106:
        strcpy(gTmpBuffer_67C598, "Too many junctions in map for routefinder!");
        break;

    case 146:
        strcpy(gTmpBuffer_67C598, "gsw - train moving across slope");
        break;

    case 147:
        strcpy(gTmpBuffer_67C598, "gsw - train : unknown direction");
        break;

    case 148:
        strcpy(gTmpBuffer_67C598, "gsw - train : unknown slope");
        break;

    case 107:
        strcpy(gTmpBuffer_67C598, "Car is trying to suicide and has not dealt with the driver");
        break;

    case 108:
        strcpy(gTmpBuffer_67C598, "AI objective required by a character is no longer active");
        break;

    case 149:
        strcpy(gTmpBuffer_67C598, "Invalid Car Rating in Car Model Info");
        break;

    case 109:
        strcpy(gTmpBuffer_67C598, "No more space in simp_text display buffer");
        break;

    case 120:
        strcpy(gTmpBuffer_67C598, "Too many score textures");
        break;

    case 121:
        strcpy(gTmpBuffer_67C598, "Trying to get unit of zero vector");
        break;

    case 123:
        sprintf(gTmpBuffer_67C598, "assertion failure in %s(line %d)", SourceFileNameFromPath_4A07A0(pFileName), lineNo);
        break;

    case 124:
        strcpy(gTmpBuffer_67C598, "Trying to expand an empty node in routefinder.");
        break;

    case 125:
        strcpy(gTmpBuffer_67C598, "No valid backcar for an car related struct.");
        break;

    case 126:
        strcpy(gTmpBuffer_67C598, "Failed to launch 'GTA2 Manager' correctly.");
        break;

    case 127:
        strcpy(gTmpBuffer_67C598, "Message text too long to wrap");
        break;

    case 128:
        strcpy(gTmpBuffer_67C598, "Too many timers");
        break;

    case 130:
        strcpy(gTmpBuffer_67C598, "Too many arrows");
        break;

    case 129:
        strcpy(gTmpBuffer_67C598, "Too many messages");
        break;

    case 131:
        strcpy(gTmpBuffer_67C598, "No Restart Zone");
        break;

    case 132:
        strcpy(gTmpBuffer_67C598, "Map not compressed");
        break;

    case 133:
        strcpy(gTmpBuffer_67C598, "Invalid symbol data in symboltable buffer");
        break;

    case 134:
        sprintf(gTmpBuffer_67C598, "Invalid mission script file specified in Bob");
        break;

    case 136:
        strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid character");
        break;

    case 137:
        strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid car");
        break;

    case 138:
        strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid object");
        break;

    case 139:
        sprintf(gTmpBuffer_67C598, "Error doing car routefind: no routefinder data in map.");
        break;

    case 143:
        sprintf(gTmpBuffer_67C598, "Illegal Train Station: no platform zone");
        break;

    case 144:
        sprintf(
            gTmpBuffer_67C598,
            "Unable to create train: no more space! The game has too many trains/carriages/buses.");
        break;

    case 145:
        strcpy(gTmpBuffer_67C598, "Base change log overflow");
        break;

    case 150:
        strcpy(gTmpBuffer_67C598, "Keyboard type not handled");
        break;

    case 151:
        strcpy(gTmpBuffer_67C598, "Couldn't open file for keyboard type");
        break;

    case 152:
        strcpy(gTmpBuffer_67C598, "Couldn't find a .seq file in the data directory");
        break;

    case 153:
        strcpy(gTmpBuffer_67C598, "There is more than one .seq file in the data directory");
        break;

    case 154:
        strcpy(gTmpBuffer_67C598, "Couldn't open .seq file in the data directory");
        break;

    case 155:
        strcpy(gTmpBuffer_67C598, "The .seq file has too many MAIN blocks defined");
        break;

    case 156:
        strcpy(gTmpBuffer_67C598, "There must be a MAIN block before any BONUS block in .seq file");
        break;

    case 157:
        strcpy(gTmpBuffer_67C598, "The .seq file has too many BONUS blocks defined for a particular city");
        break;

    case 158:
        strcpy(gTmpBuffer_67C598, "Each block in .seq file must be MAIN or BONUS");
        break;

    case 159:
        strcpy(gTmpBuffer_67C598, "First character of line in .seq should be a letter");
        break;

    case 160:
        strcpy(gTmpBuffer_67C598, "Line in .seq has been interrupted by newline before end");
        break;

    case 161:
        strcpy(gTmpBuffer_67C598, "Label in .seq file is too long");
        break;

    case 162:
        strcpy(gTmpBuffer_67C598, "Undefined label in code for loading .seq file");
        break;

    case 163:
        strcpy(gTmpBuffer_67C598, "Data for a line in .seq file is too long");
        break;

    case 164:
        strcpy(gTmpBuffer_67C598, "Unexpected label in .seq file - Check order and spelling");
        break;

    case 165:
        strcpy(gTmpBuffer_67C598, "The menu contains no valid options");
        break;

    case 166:
        strcpy(
            gTmpBuffer_67C598,
            ".gmp filename in save game file does not match .gmp filename in .seq for same level");
        break;

    case 167:
        strcpy(
            gTmpBuffer_67C598,
            ".sty filename in save game file does not match .sty filename in .seq for same level");
        break;

    case 168:
        strcpy(
            gTmpBuffer_67C598,
            ".scr filename in save game file does not match .scr filename in .seq for same level");
        break;

    case 169:
        strcpy(gTmpBuffer_67C598, "Couldn't open bink buffer for playing movie");
        break;

    case 170:
        strcpy(gTmpBuffer_67C598, "Couldn't open bink for playing movie");
        break;

    case 183:
        strcpy(gTmpBuffer_67C598, "Too many characters in credits line (max is 50)");
        break;

    case 184:
        strcpy(gTmpBuffer_67C598, "Too many lines in credits (max is 600)");
        break;

    case 185:
        strcpy(gTmpBuffer_67C598, "Shouldn't be allowed to move to next level if already on last level");
        break;

    case 186:
        strcpy(gTmpBuffer_67C598, "Shouldn't be allowed to move to next level if it isn't opened");
        break;

    case 187:
        strcpy(gTmpBuffer_67C598, "Unexpected size of frontend background image");
        break;

    case 188:
        strcpy(gTmpBuffer_67C598, "Invalid colour for line of credit text");
        break;

    case 189:
        strcpy(gTmpBuffer_67C598, "Multiplayer game type should be frag, tag or score (but isn't)");
        break;

    case 171:
        strcpy(gTmpBuffer_67C598, "Unable to handle attached ped");
        break;

    case 172:
        strcpy(gTmpBuffer_67C598, "Attract mode replay file has no contents");
        break;

    case 173:
        strcpy(gTmpBuffer_67C598, "Replay sync error");
        break;

    case 174:
        strcpy(gTmpBuffer_67C598, "Couldn't allocate memory for targa file");
        break;

    case 175:
        strcpy(gTmpBuffer_67C598, "Targa file does not contain an image");
        break;

    case 176:
        strcpy(gTmpBuffer_67C598, "Wrong format for targa file - expecting 16 bit uncompressed");
        break;

    case 177:
        strcpy(gTmpBuffer_67C598, "Targa file does not have 16 bits per pixel");
        break;

    case 178:
        strcpy(gTmpBuffer_67C598, "Targa file has a palette - not expected");
        break;

    case 179:
        strcpy(gTmpBuffer_67C598, "Origin for targa file image not at (0,0)");
        break;

    case 180:
        strcpy(gTmpBuffer_67C598, "Width or height of targa file image is 0");
        break;

    case 181:
        strcpy(gTmpBuffer_67C598, "Couldn't read id from targa file");
        break;

    case 182:
        strcpy(gTmpBuffer_67C598, "Error in reading image from targa file");
        break;

    case 1001:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid car model : %d", va_1);
        break;
    }

    case 1002:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Sprite %d has multiple delta entries", va_1);
        break;
    }

    case 1003:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Too many sprites with deltas at %d", va_1);
        break;
    }

    case 1004:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid absolute sprite number: %d", va_1);
        break;
    }

    case 1005:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Too many sprites at %d", va_1);
        break;
    }

    case 1006:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid case : %d", va_1);
        break;
    }

    case 1007:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid absolute virtual palette number: %d", va_1);
        break;
    }

    case 1008:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid font number: %d", va_1);
        break;
    }

    case 1009:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid tile number: %d", va_1);
        break;
    }

    case 1010:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid new tile number: %d", va_1);
        break;
    }

    case 1011:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Unable to load DLL: %s", va_1);
        break;
    }

    case 1012:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid physical palette number: %d", va_1);
        break;
    }

    case 1016:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Too many zones at %d (max is 999)", va_1);
        break;
    }

    case 1017:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid zone number: %d", va_1);
        break;
    }

    case 1013:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Zone not found : %s", va_1);
        break;
    }

    case 1014:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("car model %d has invalid gear speeds in gci file", va_1);
        break;
    }

    case 1018:
    {
        const char *va_1 = va_arg(va, const char *);
        err_a1_int("Invalid map object number: %d", va_1);
        break;
    }

    case 1022:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "File too large: %s exceeds max size by %d bytes", gGlobalFileName_67C6AC, va_1);
        break;
    }

    case 1029:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Out of memory allocating %d bytes", va_1);
        break;
    }

    case 1033:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid chunk size : %d", va_1);
        break;
    }

    case 1034:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid switch in parameter list: %c", (char)va_1);
        break;
    }

    case 1073:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid line (info:%d)", va_1);
        break;
    }

    case 1035:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Replay File larger than maximum: size %d", va_1);
        break;
    }

    case 1036:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid Videomode to search for: %d", va_1);
        break;
    }

    case 1037:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to set fullscreen videomode: %d", va_1);
        break;
    }

    case 1020:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too many loaded object infos at %d", va_1);
        break;
    }

    case 1023:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too many players at %d", va_1);
        break;
    }

    case 1024:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid player number : %d", va_1);
        break;
    }

    case 1025:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid grid width : %d", va_1);
        break;
    }

    case 1026:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid grid height : %d", va_1);
        break;
    }

    case 1027:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid z coordinate : %d", va_1);
        break;
    }

    case 1028:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Event left unprocessed : %08X", va_1);
        break;
    }

    case 1040:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Out of space for list type %s", gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1039:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "No collision box in sprite %d", va_1);
        break;
    }

    case 1021:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Collision box invalid in sprite %d", va_1);
        break;
    }

    case 1041:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid object model number : %d", va_1);
        break;
    }

    case 1042:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "object model number %d already added", va_1);
        break;
    }

    case 1043:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "no space for object model number %d", va_1);
        break;
    }

    case 1044:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "check walls slope size too big : %d", va_1);
        break;
    }

    case 1045:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create an object in scriptfile. Object type: %d", va_1);
        break;
    }

    case 1046:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Attempt to kill an already dead mission thread. Debug line %d", va_1);
        break;
    }

    case 1047:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Car model %d has too many doors", va_1);
        break;
    }

    case 1048:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Releasing null ptr in release_passive. List type %s",
            gListTypes_61AB70[(DWORD)va_1]);
        break;
    }
    case 1049:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Trying to 'pacify' a NULL list element. List type %s",
            gListTypes_61AB70[(DWORD)va_1]);
        break;
    }
    case 1050:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Object back pointer missing in sprite %d", va_1);
        break;
    }

    case 1051:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "No rigid body in car ID %d", va_1);
        break;
    }
    case 1052:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "No free Sid for character %d", va_1);
        break;
    }
    case 1053:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid door info number : %d", va_1);
        break;
    }
    case 1054:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal Value for an angle: %d", va_1);
        break;
    }
    case 1055:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid tile texture: %d", va_1);
        break;
    }
    case 1056:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid cache number : %d", va_1);
        break;
    }
    case 1068:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid score texture number : %d", va_1);
        break;
    }

    case 1019:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Setting invalid zone information : %s", va_1);
        break;
    }

    case 1057:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Counter has out of bounds internal count: %d", va_1);
        break;
    }

    case 1058:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Out of range route id: %d", va_1);
        break;
    }

    case 1066:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid score position: %d", va_1);
        break;
    }

    case 1067:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid player ID: %d", va_1);
        break;
    }

    case 1069:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too characters in score : %d", va_1);
        break;
    }

    case 1071:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid text file : %s", va_1);
        break;
    }

    case 1072:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "String starts with end-of-line : %s", va_1);
        break;
    }

    case 1076:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "car ID %d in collision with wall at safe position", va_1);
        break;
    }

    case 1080:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "object ID %d in collision with wall at safe position", va_1);
        break;
    }

    case 1077:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal device code for this machine: %d", va_1);
        break;
    }

    case 1078:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Activating null ptr in activate. List type %s", gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1079:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Activating item twice. List type %s", gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1081:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal index into turntable: %d", va_1);
        break;
    }

    case 1082:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too many Y road segments. Num: %d", va_1);
        break;
    }

    case 1083:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too many X road segments. Num: %d", va_1);
        break;
    }
    case 1084:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid timer handle : %d", va_1);
        break;
    }

    case 1085:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid arrow handle : %d", va_1);
        break;
    }

    case 1086:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unallocated arrow handle : %d", va_1);
        break;
    }

    case 1090:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Car already doing routefind, but requested another. Car ID: %d", va_1);
        break;
    }

    case 1137:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Counter check failed for list type %s", gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1107:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Car model %d has invalid style data", va_1);
        break;
    }

    case 1138:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Releasing twice in release_passive. List type %s",
            gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1139:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Releasing twice in release. List type %s", gListTypes_61AB70[(DWORD)va_1]);
        break;
    }

    case 1095:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "assertion failure in %s(line %d): %s", SourceFileNameFromPath_4A07A0(pFileName), lineNo, va_1);
        break;
    }
    case 1096:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Too many symbols at %d (max is 999)", va_1);
        break;
    }

    case 1098:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create_a_character in script: uid: %d", va_1);
        break;
    }

    case 1099:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to find a uid in script: uid: %d", va_1);
        break;
    }

    case 1100:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal zone id in symbol table: %d", va_1);
        break;
    }

    case 1101:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create arrow in script: uid: %d", va_1);
        break;
    }

    case 1102:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing car that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1103:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing character that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1104:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing object that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1110:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing counter that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1111:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Line accessing character with no group when it should have group: uid: %d",
            va_1);
        break;
    }

    case 1106:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing trigger that hasn't been created: uid: %d", (int)va_1);
        break;
    }

    case 1108:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing crane that hasn't been created: uid: %d", (int)va_1);
        break;
    }

    case 1109:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line attempting to order driver out of car, but char isn't a driver!: %d", (int)va_1);
        break;
    }

    case 1112:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing arrow that hasn't been created: uid: %d", (int)va_1);
        break;
    }

    case 1113:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Attempting to do 'SET_GANG_INFO' on invalid gang name in script line: %d", (int)va_1);
        break;
    }

    case 1114:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create a crane in script line: %d", (int)va_1);
        break;
    }

    case 1115:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create a conveyor in script line: %d", (int)va_1);
        break;
    }

    case 1116:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create a generator in script line: %d", (int)va_1);
        break;
    }

    case 1117:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create a destructor in script line: %d", va_1);
        break;
    }

    case 1118:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Attempted to point arrow at a 'logical' item, not a physical item. Script line: %d",
            va_1);
        break;
    }

    case 1119:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing door that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1120:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Failed to create a crusher in script line: %d", va_1);
        break;
    }

    case 1121:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing conveyor that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1123:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing generator that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1124:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid special tile number : %d", va_1);
        break;
    }

    case 1125:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Repeated special tile number : %d", va_1);
        break;
    }

    case 1122:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Line accessing destructor that hasn't been created: uid: %d", va_1);
        break;
    }

    case 1126:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Checking phone in missionscript without executing ANSWER_PHONE: uid: %d", va_1);
        break;
    }

    case 1127:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "map has too many columns by %d words", va_1);
        break;
    }

    case 1129:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "map has %d blocks too many ", va_1);
        break;
    }

    case 1128:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "style file contains unexpected number of fonts %d", va_1);
        break;
    }

    case 1130:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Attempting 'is_item_onscreen' on an illegal type: uid: %d", va_1);
        break;
    }

    case 1131:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to create car to place on top of trailer in script. uid: %d", va_1);
        break;
    }

    case 1132:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find zonename to 'setup' info for in script. uid: %d", va_1);
        break;
    }

    case 1133:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find zonename in 'set_gang_info' in script. uid: %d", va_1);
        break;
    }

    case 1134:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find zonename in 'set_station_info' in script. uid: %d", va_1);
        break;
    }

    case 1135:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find platform in 'set_station_info' in script. uid: %d", va_1);
        break;
    }

    case 1136:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Unable to find trainstation associated with platform in 'set_station_info' in script. uid: %d",
            va_1);
        break;
    }

    case 1140:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Unable to find gang matching name of gangone in a 'set gang respect'. uid: %d",
            va_1);
        break;
    }

    case 1141:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Unable to find gang matching name of gangtwo in a 'set gang respect'. uid: %d",
            va_1);
        break;
    }

    case 1142:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find gangname in an 'alter gang respect'. uid: %d", va_1);
        break;
    }

    case 1143:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Trying to change gangrespect on a non-player controlled character. uid: %d",
            va_1);
        break;
    }

    case 1144:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find gang matching name in a 'check gang respect'. uid: %d", va_1);
        break;
    }

    case 1145:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find zone matching name in a 'is char in gangzone'. uid: %d", va_1);
        break;
    }

    case 1146:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find zone matching name in a 'is char in zone'. uid: %d", va_1);
        break;
    }

    case 1147:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Unable to find gang matching name in a 'add char to gang'. uid: %d", va_1);
        break;
    }

    case 1148:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Mission script is referring to a 'text id' that doesn't exist. ID: %d", va_1);
        break;
    }

    case 1400:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Error with 1 param: %d", va_1);
        break;
    }

    case 2001:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid relative sprite number %d in %s", va_2, va_2);
        break;
    }

    case 2002:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid relative virtual palette number %d in %s", va_2, va_2);
        break;
    }

    case 2003:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid relative font sprite number %d in font %d", va_2, va_2);
        break;
    }

    case 2004:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Incorrect aspect ratio in screen mode : %d x %d", va_2, va_2);
        break;
    }

    case 2024:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "List type %s corrupted at grab at %d elements",
            gListTypes_61AB70[(DWORD)va_2],
            va_2);
        break;
    }

    case 2025:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "List type %s corrupted at release at %d elements",
            gListTypes_61AB70[(DWORD)va_2],
            va_2);
        break;
    }

    case 2026:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "List type %s corrupted at traverse at %d elements",
            gListTypes_61AB70[(DWORD)va_2],
            va_2);
        break;
    }

    case 2030:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "List type %s corrupted at activate at %d elements",
            gListTypes_61AB70[(DWORD)va_2],
            va_2);
        break;
    }

    case 2031:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "List type %s corrupted at pacify at %d elements",
            gListTypes_61AB70[(DWORD)va_2],
            va_2);
        break;
    }

    case 2005:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Can't delete element %d from list type %s",
            va_2,
            gListTypes_61AB70[(DWORD)va_2]);
        break;
    }

    case 2006:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "invalid zs in sprite %d : %d", va_2, va_2);
        break;
    }

    case 2007:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Door %d of car model %d too close", va_2, va_2);
        break;
    }

    case 2008:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid sprite type %d number %d", va_2, va_2);
        break;
    }

    case 2009:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "traffic lights at (%d,%d) are too close to the edge of the world", va_2, va_2);
        break;
    }

    case 2010:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid frame %d in object model %d", va_2, va_2);
        break;
    }

    case 2011:
    {
        Coord2 c;
        c.x = *va_arg(va, int*);
        c.y = *va_arg(va, int*);
        sprintf(
            gTmpBuffer_67C598,
            "Illegal coordinate: (%.4f, %.4f)",
            c.x.AsFloat(),
            c.y.AsFloat());

        break;
    }

    case 2012:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal good/bad ratio for car model choice: good %d, bad %d", va_2, va_2);
        break;
    }

    case 2019:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "model %d repeated in %s", va_2, SourceFileNameFromPath_4A07A0(va_2));
        break;
    }

    case 2014:
        sprintf(gTmpBuffer_67C598, "%s binmake error : %s", SourceFileNameFromPath_4A07A0(gErrStr_67C29C), byte_67C3A8);
        break;

    case 2018:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "%s invalid size at %d bytes", SourceFileNameFromPath_4A07A0(va_2), va_2);
        break;
    }

    case 2015:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "car ID %d in collision with %s sprite at safe position", va_2, va_2);
        break;
    }

    case 2016:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "objecct ID %d in collision with %s sprite at safe position", va_2, va_2);
        break;
    }

    case 2017:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid crane target : (%d,%d)", va_2, va_2);
        break;
    }

    case 2020:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Too many door infos declared in script: curr door: closedframe %d openframe %d",
            va_2,
            va_2);
        break;
    }

    case 2021:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no entry zone", va_2, va_2);
        break;
    }

    case 2022:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no exit zone", va_2, va_2);
        break;
    }

    case 2023:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no stop zone", va_2, va_2);
        break;
    }

    case 2401:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Error with 2 param: %d,%d", va_2, va_2);
        break;
    }

    case 2027:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "incorrect replay version : %d.%d", va_2, va_2);
        break;
    }

    case 2028:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Sync error (random) remote=%d local=%d", va_2, va_2);
        break;
    }

    case 2029:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Sync error (mission) remote=%d local=%d", va_2, va_2);
        break;
    }

    case 3003:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Videomode %dx%dx%d is not available", va_3, va_3, va_3);
        break;
    }

    case 3004:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Failed to set videomode %dx%dx%d", va_3, va_3, va_3);
        break;
    }

    case 3005:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Can't delete sprite %d from collision list at : %d,%d", va_3, va_3, va_3);
        break;
    }

    case 3006:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Adding sprite %d to collision list twice at : %d,%d", va_3, va_3, va_3);
        break;
    }

    case 3007:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "invalid map access : (%d,%d,%d)", va_3, va_3, va_3);
        break;
    }

    case 3008:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "invalid map change access : (%d,%d,%d)", va_3, va_3, va_3);
        break;
    }

    case 3009:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Invalid palette type %d for sprite type %d, number %d", va_3, va_3, va_3);
        break;
    }

    case 3010:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Invalid sprite type %d number %d remap %d", va_3, va_3, va_3);
        break;
    }

    case 3013:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "%d models in %s but %d models in style", va_3, SourceFileNameFromPath_4A07A0((const char *)va_3), va_3);
        break;
    }

    case 3011:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Invalid block in train code : (%d,%d,%d)", va_3, va_3, va_3);
        break;
    }

    case 3012:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Train on a block with incorrect arrow : (%d,%d,%d)", va_3, va_3, va_3);
        break;
    }

    case 3014:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Attempting to disable road at illegal coordinates: check Z! ( %d, %d, %d)",
            va_3,
            va_3,
            va_3);
        break;
    }

    case 3015:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Attempting to enable road at illegal coordinates: check Z! ( %d, %d, %d)",
            va_3,
            va_3,
            va_3);
        break;
    }

    case 3016:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Illegal block for train creation. Coordinates: check stopzone! ( %d, %d, %d)",
            va_3,
            va_3,
            va_3);
        break;
    }

    case 3017:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(gTmpBuffer_67C598, "Failed to create carriage at: (%d, %d, %d)", va_3, va_3, va_3);
        break;
    }

    case 3018:
    {
        int va_1 = va_arg(va, int);
        const char *va_2 = va_arg(va, const char *);
        int *va_3 = va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Error in set-up of park command. Walls intruding on the park's triggerspace! Door coords: (%d, %d, %d)",
            va_3,
            va_3,
            va_3);
        break;
    }

    case 4001:
    {
        int *va_1 = va_arg(va, int *);

        Coord c;

        c.x = *(int*)va_arg(va, int *);
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);

        sprintf(
            gTmpBuffer_67C598,
            "Invalid x co-ord in sprite type %d at (%f,%f,%f)",
            va_1,
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
    }
        break;

    case 4002:
    {
        const char *va_1 = va_arg(va, const char *);


        Coord c;
        c.x = *(int*)va_arg(va, const char *);
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Invalid y co-ord in sprite type %d at (%f,%f,%f)",
            va_1,
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
        break;
    }

    case 4003:
    {
        const char *va_1 = va_arg(va, const char *);

        Coord c;
        c.x = *(int*)va_arg(va, const char *);
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Invalid z co-ord in sprite type %d at (%f,%f,%f)",
            va_1,
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
        break;
    }

    case 4007:
    {
        const char *va_1 = va_arg(va, const char *);

        Coord2 c;
        c.x = *(int*)va_arg(va, const char *);
        c.y = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Placing gang phone when no gang at: (%.4f, %.4f)",
            c.x.AsFloat(),
            c.y.AsFloat());
        break;
    }

    case 4004:
    {
        const char *va_1 = va_arg(va, const char *);

        Coord c;
        c.x = *(int*)va_arg(va, const char *);
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Illegal coordinate: (%.4f, %.4f, %.4f)",
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
        break;
    }

    case 4008:
    {
        const char *va_1 = va_arg(va, const char *);

        Coord c;
        c.x = *(int*)va_arg(va, const char *);
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "Invalid diagonal block at (%.4f, %.4f, %.4f)",
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
        break;
    }

    case 4005:
    {
        int *va_1 = va_arg(va, int *);
        Coord c;
        c.x = *(int*)va_arg(va, int *); // NOTE: No match without casting here
        c.y = *(int*)va_arg(va, int *);
        c.z = *(int*)va_arg(va, int *);
        sprintf(
            gTmpBuffer_67C598,
            "No direction arrow found in block (with slope!!). ( %f, %f, %f)",
            c.x.AsFloat(),
            c.y.AsFloat(),
            c.z.AsFloat());
        break;
    }

    case 9011:
    {
        const char *va_1 = va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "DirectX Error: %s", va_1);
        break;
    }

    case 8001:
        sprintf(gTmpBuffer_67C598, "Too many %s phones for %s gang", gErrStr_67C29C, byte_67C3A8);
        break;
    case 7004:

        sprintf(gTmpBuffer_67C598, "Too many lines in briefing %s", gErrStr_67C29C);
        break;

    case 7006:
        sprintf(gTmpBuffer_67C598, "Couldn't find attract mode file %s", gErrStr_67C29C);
        break;

    case 7001:
        sprintf(gTmpBuffer_67C598, "text identifier not found : %s", gErrStr_67C29C);
        break;

    case 7002:
        sprintf(gTmpBuffer_67C598, "Briefing %s too long", gErrStr_67C29C);
        break;

    case 7003:
        sprintf(gTmpBuffer_67C598, "Briefing %s too long to wrap", gErrStr_67C29C);
        break;

    case 5001:
    {
        Coord2 c;
        c.x = *(int *)va_arg(va, const char *);
        sprintf(gTmpBuffer_67C598, "Invalid fraction : %f", c.x.AsFloat());
        break;
    }

    case 5002:
    {
        Coord2 c;
        c.x = *(int *)va_arg(va, const char *);
        sprintf(
            gTmpBuffer_67C598,
            "Invalid ambient light value : %f",
            c.x.AsFloat());
        break;
    }
    case 6001:
    {
        // TODO: Stack access is wrong here
        int va_1 = va_arg(va, int );
        int va_2 = va_arg(va, int );

        int coords[3];
        int coords1[3];
        coords[0] = *va_arg(va, int *);
        coords[1] = *va_arg(va, int *);
        coords[2] = *va_arg(va, int *);

       
        coords1[0] = *va_arg(va, int *);
        coords1[1] = *(int *)va_arg(va, int *);
        coords1[2] = *va_arg(va, int *);

        sprintf(
            gTmpBuffer_67C598,
            "Unable to find a route from (%.4f, %.4f, %.4f) to (%.4f, %.4f, %.4f) in car id: %d model: %d",
            coords[0] / 16384.0f,
            coords[1] / 16384.0f,
            coords[2] / 16384.0f,
            coords1[0] / 16384.0f,
            coords1[1] / 16384.0f,
            coords1[2] / 16384.0f,
            va_1,
            va_2);
    }
        break;

    case 666: // moved case
        sprintf(gTmpBuffer_67C598, "List Error see Kivlin");
        break;

    default:
        sprintf(gTmpBuffer_67C598, "No Error Message Declared");
        break;
    }

    ErrorMsgBox_5E4EC0(gTmpBuffer_67C598);
    gErrorLog_67CF58.Write_4D9620(gTmpBuffer_67C598);
}

MATCH_FUNC(0x4A38C0)
void FatalError_4A38C0(int Code, const char *pSourceFile, int lineNo, ...)
{
    if (Code == 16)
    {
        const char destroyed = bDestroyed_6F5B70;
        if (!destroyed)
        {
            sub_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();
            // 16 was using esi instead of push 10h without the 0x6F5B70 bool
            // being cached in both branches of the if/else
            FatalError_4A07C0(16, pSourceFile, lineNo);
        }
        DestroyWindow(gHwnd_707F04);
        exit(16);
    }
    else
    {
        const char destroyed = bDestroyed_6F5B70;
        if (!destroyed)
        {
            sub_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();

            va_list va;
            va_start(va, lineNo);

            if (Code < 1000)
            {
                FatalError_4A07C0(Code, pSourceFile, lineNo);
            }
            else if (Code < 2000)
            {
                int a33 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a33);
            }
            else if (Code < 3000)
            {
                int a33 = va_arg(va, int);
                int a44 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a44, a44);
            }
            else if (Code < 4000)
            {
                int a33 = va_arg(va, int);
                int a44 = va_arg(va, int);
                int a55 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a55, a55, a55);
            }
            else if (Code < 5000)
            {
                int a33 = va_arg(va, int);
                int a44 = va_arg(va, int);
                int a55 = va_arg(va, int);
                int a66 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a33, a44, a55, a66);
            }
            else if (Code < 6000)
            {
                int a33 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a33);
            }
            else if (Code < 7000)
            {
                int a33 = va_arg(va, int);
                int a44 = va_arg(va, int);
                int a55 = va_arg(va, int);
                int a66 = va_arg(va, int);
                int a77 = va_arg(va, int);
                int a88 = va_arg(va, int);
                int a99 = va_arg(va, int);
                int a10 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a33, a44, a55, a66, a77, a88, a99, a10);
            }
            else if (Code < 8000)
            {
                FatalError_4A07C0(Code, pSourceFile, lineNo);
            }
            else if (Code < 9000)
            {
                FatalError_4A07C0(Code, pSourceFile, lineNo);
            }
            else if (Code == 9011)
            {
                int a33 = va_arg(va, int);
                FatalError_4A07C0(Code, pSourceFile, lineNo, a33);
            }
            else
            {
                FatalError_4A07C0(Code, pSourceFile, lineNo);
            }
        }
        DestroyWindow(gHwnd_707F04);
        exit(Code);
    }
}
