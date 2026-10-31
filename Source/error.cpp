#include "error.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "enums.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"
#include "winmain.hpp"
#include <stdio.h>
#include <time.h>
#include <windows.h>

DEFINE_GLOBAL(char_type, bDestroyed, 0x6F5B70);
DEFINE_GLOBAL(HWND, gHwnd, 0x707F04);

MATCH_FUNC(0x4D9470)
void ErrorLog::Open(const char_type* path, s32 truncate_mode)
{
    u8* fileNameLen = new u8;
    if (fileNameLen)
    {
        *fileNameLen = static_cast<u8>(strlen(path));
    }
    else
    {
        fileNameLen = NULL;
    }
    this->mpLen = fileNameLen;

    s32 mode;
    switch (truncate_mode)
    {
        case 0:
            mode = 8; // ios::app
            break;

        case 1:
            mode = 16; // ios::trunc
            break;

        default:
            mode = 8; // ios::app
            break;
    }

#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ofstream&>(mOfstr).open(path, mode, filebuf::openprot);
#endif

    log_timestamp();
}

// The member ofstream's constructor call and EH frame come from the real ofstream member
// (error.hpp, VC6 only)
MATCH_FUNC(0x4D94E0)
ErrorLog::ErrorLog(const char_type* FileName, s32 truncate_mode)
{
    Open(FileName, truncate_mode);
}

// This func matches but for some reason it's crashing the patched version
MATCH_FUNC(0x4D9690)
EXPORT void __cdecl log_on_line_written_cb(void* pStream)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type*>(pStream)->flush();
#endif
}

MATCH_FUNC(0x4D9620)
void ErrorLog::Write(const char_type* pMsg)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type&>(this->mOfstr) << pMsg << endl << flush;
#endif
}

MATCH_FUNC(0x4D9650)
void ErrorLog::Write_Log(const char_type* buffer)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type&>(this->mOfstr) << buffer << flush;
#endif
}

MATCH_FUNC(0x4D9540)
EXPORT void ErrorLog::log_timestamp()
{
    time_t curTime = time(NULL);
    char_type* buffer = ctime(&curTime);
    buffer[strlen(buffer) - 1] = 0; // remove the new line it  adds
    sprintf(gTmpBuffer, "\n------ %s ------", buffer);
    Write(gTmpBuffer);
}

MATCH_FUNC(0x4D9670)
void* ErrorLog::log_on_line_written(TLogLineCallback pCallBack)
{
    pCallBack(this);
    return this;
}

// Logs the intro line again on the way out, then closes the file
MATCH_FUNC(0x4A3610)
ErrorLog::~ErrorLog()
{
    log_intro();
#if !defined(__clang__) && (_MSC_VER <= 1200)
    mOfstr.close();
#endif
    delete mpLen;
}

MATCH_FUNC(0x4D95A0)
void ErrorLog::log_intro()
{
    sprintf(gTmpBuffer, "Coop v%d.%d", gGTA2VersionMajor, gGTA2VersionMinor);
    Write(gTmpBuffer);

    time_t curTime = time(NULL);
    char_type* buffer = ctime(&curTime);
    buffer[strlen(buffer) - 1] = 0; // remove the new line it  adds
    sprintf(gTmpBuffer, "\n------ %s ------", buffer);
    Write(gTmpBuffer);
}

DEFINE_GLOBAL_INIT(ErrorLog, gErrorLog_67CF58, ErrorLog("test\\error.log", 1), 0x67CF58);
DEFINE_GLOBAL_INIT(ErrorLog, gErrorLog_67C530, ErrorLog("test\\warning.log", 1), 0x67C530);
DEFINE_GLOBAL(ErrorLog, gMiss2Log, 0x6F7698);

DEFINE_GLOBAL_ARRAY(char_type, gTmpBuffer, 256, 0x67C598);
DEFINE_GLOBAL_ARRAY(char_type, gErrStr, 256, 0x67C29C);
DEFINE_GLOBAL_ARRAY(char_type, gErrStr2, 256, 0x67C3A8);
DEFINE_STATIC_GLOBAL_ARRAY(char_type, sGlobalFileName, 256, 0x67C6AC);

const char_type* gListTypes_61AB70[30] = {"objects",
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
                                          "audio_info_space"};

MATCH_FUNC(0x4A0770)
EXPORT void __stdcall Error_SetName(const char_type* pFileName)
{
    strcpy(sGlobalFileName, pFileName);
}

MATCH_FUNC(0x4A07A0)
EXPORT const char_type* __stdcall SourceFileNameFromPath(const char_type* pPath)
{
    // ecx was swapped with edx without using the local pIter
    const char_type* result = NULL;
    const char_type* pIter = pPath;
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

#define err_a1_int(msg, arg) sprintf(gTmpBuffer, msg, arg);

MATCH_FUNC(0x4A07C0)
EXPORT void ReportFatalError(s32 code, const char_type* pFileName, s32 lineNo, ...)
{
    va_list va;

    va_start(va, lineNo);

    sprintf(gTmpBuffer, "Fatal Error %d in %s(%d)", code, SourceFileNameFromPath(pFileName), lineNo);
    gErrorLog_67CF58.Write(gTmpBuffer);

    switch (code)
    {
        case Gta2Error::DMAVideoInitFail:
            sprintf(gTmpBuffer, "Can't Init DMAVideo.");
            break;

        case Gta2Error::SecurityFail:
            sprintf(gTmpBuffer, "security failure");
            break;

        case Gta2Error::SetWindowModeFail:
            sprintf(gTmpBuffer, "Failed to set WindowMode");
            break;

        case Gta2Error::GetValidHDCFail:
            sprintf(gTmpBuffer, "Failed to get a valid HDC");
            break;

        case Gta2Error::SetWindowPosFail:
            sprintf(gTmpBuffer, "Failed to SetWindowPos properly");
            break;

        case Gta2Error::DirectInputInitFail:
            sprintf(gTmpBuffer, "Failed to Initialise DirectInput");
            break;

        case Gta2Error::DirectInputSetDataFormatFail:
            sprintf(gTmpBuffer, "Failed to set Data Format for DirectInput");
            break;

        case Gta2Error::DirectInputSetCooperativeLevelFail:
            sprintf(gTmpBuffer, "Failed to set Cooperative Level for DirectInput");
            break;

        case Gta2Error::DirectInputNoObject:
            sprintf(gTmpBuffer, "No DirectInput Object");
            break;

        case Gta2Error::DirectInputCreateFail:
            sprintf(gTmpBuffer, "Failed to create DirectInput object");
            break;

        case Gta2Error::ZeroBytesAlloc:
            strcpy(gTmpBuffer, "Attempting to allocate zero bytes of memory");
            break;

        case Gta2Error::FtellError:
            sprintf(gTmpBuffer, "Ftell error in file: %s", sGlobalFileName);
            break;

        case Gta2Error::FileReadFailure:
            sprintf(gTmpBuffer, "Read failure in file: %s", sGlobalFileName);
            break;

        case Gta2Error::FileOpenError:
            sprintf(gTmpBuffer,
                    "Install Next freeloader episode please. [Temp message].Unable to open file : %s",
                    sGlobalFileName);
            break;

        case Gta2Error::FileCloseError:
            sprintf(gTmpBuffer, "Unable to close file: %s", sGlobalFileName);
            break;

        case Gta2Error::FileBiggerThanBuffer:
            sprintf(gTmpBuffer, "Trying to a load a file too big for buffer: %s", sGlobalFileName);
            break;

        case Gta2Error::WritingZeroBytesToFile:
            sprintf(gTmpBuffer, "Writing zero bytes to file : %s", sGlobalFileName);
            break;

        case Gta2Error::FileWriteFailure:
            sprintf(gTmpBuffer, "Write failure on file : %s", sGlobalFileName);
            break;

        case Gta2Error::FileReadingNotOpenedFile:
            strcpy(gTmpBuffer, "Attempt to read from not opened file");
            break;

        case Gta2Error::ungetcFailrue:
            sprintf(gTmpBuffer, "ungetc failure in file %s", sGlobalFileName);
            break;

        case Gta2Error::FileInvalidInteger:
            sprintf(gTmpBuffer, "invalid integer encountered in file %s", sGlobalFileName);
            break;

        case Gta2Error::FileEndOfFileInTextString:
            sprintf(gTmpBuffer, "End of file in text string in file %s", sGlobalFileName);
            break;

        case Gta2Error::TextStringTooLong:
            strcpy(gTmpBuffer, "Text string too long");
            break;

        case Gta2Error::OutOfMemoryNewOperator:
            strcpy(gTmpBuffer, "Out of memory - new operator failed");
            break;

        case Gta2Error::FileOverflow:
            strcpy(gTmpBuffer, "file overflow");
            break;

        case Gta2Error::DataNotLoaded:
            strcpy(gTmpBuffer, "data not loaded");
            break;

        case Gta2Error::InvalidStringInParameterList:
            sprintf(gTmpBuffer, "Invalid string in parameter list: %s", sGlobalFileName);
            break;

        case Gta2Error::RepeatedFileInParameterList:
            sprintf(gTmpBuffer, "Repeated file in parameter list : %s", sGlobalFileName);
            break;

        case Gta2Error::InvalidZoneData:
            strcpy(gTmpBuffer, "Invalid zone data");
            break;

        case Gta2Error::InvalidMapObjectData:
            strcpy(gTmpBuffer, "Invalid map object data");
            break;

        case Gta2Error::InvalidMapLightData:
            strcpy(gTmpBuffer, "Invalid map light data");
            break;

        case Gta2Error::InvalidLoadedObjectInfoData:
            strcpy(gTmpBuffer, "Invalid loaded object info data");
            break;

        case Gta2Error::CloseRegistryKeyFail:
            strcpy(gTmpBuffer, "unable to close registry key");
            break;

        case Gta2Error::CreateRegistryKeyFail:
            strcpy(gTmpBuffer, "unable to create registry key");
            break;

        case Gta2Error::NoPlayers:
            strcpy(gTmpBuffer, "No players");
            break;

        case Gta2Error::ClassNotAllocated:
            strcpy(gTmpBuffer, "Class not allocated");
            break;

        case Gta2Error::SetRegistryValueFail:
            strcpy(gTmpBuffer, "Unable to set registry value");
            break;

        case Gta2Error::LoopInTree:
            strcpy(gTmpBuffer, "Loop in tree");
            break;

        case Gta2Error::DisplayTreeOutOfSpace:
            strcpy(gTmpBuffer, "Out of space for display tree");
            break;

        case Gta2Error::FileWriteByteFail:
            sprintf(gTmpBuffer, "Failed writing a byte to file %s", sGlobalFileName);
            break;

        case Gta2Error::UsingUninitialisedSprite:
            strcpy(gTmpBuffer, "Using uninitialised sprite");
            break;

        case Gta2Error::InvalidCorners:
            sprintf(gTmpBuffer, "Invalid corners");
            break;

        case Gta2Error::MissionProcessAllocMemFail:
            sprintf(gTmpBuffer, "Failed to allocate memory for new mission process");
            break;

        case Gta2Error::MissionProcessOutOfSlots:
            sprintf(gTmpBuffer, "No more process slots available for mission processes");
            break;

        case Gta2Error::CheckWallsSlopeInvalidSlope:
            strcpy(gTmpBuffer, "check walls slope invalid slope");
            break;

        case Gta2Error::ScriptfileTooMany_PLAYER_PED_Declarations:
            sprintf(gTmpBuffer, "Too many PLAYER_PED declarations in scriptfile");
            break;

        case Gta2Error::ScriptfileNo_LEVELSTART_Declaration:
            sprintf(gTmpBuffer, "No LEVELSTART declaration in scriptfile");
            break;

        case Gta2Error::ScriptfileFailedToCreatePed:
            sprintf(gTmpBuffer, "Failed to create a ped in scriptfile");
            break;

        case Gta2Error::MapChangeOverflowBlocks:
            strcpy(gTmpBuffer, "map change overflow - too many new blocks");
            break;

        case Gta2Error::MapChangeOverflowColumns:
            strcpy(gTmpBuffer, "map change overflow - too many new columns");
            break;

        case Gta2Error::MissionStackAllocFail:
            strcpy(gTmpBuffer, "Failed to allocate memory for mission stack");
            break;

        case Gta2Error::MissionStackPushNullItem:
            strcpy(gTmpBuffer, "Tried to push NULL item onto mission stack");
            break;

        case Gta2Error::MissStackItemGrabNoStackSpaceLeftFail:
            strcpy(gTmpBuffer, "Failed grabbing miss_stack_item - no stack space left");
            break;

        case Gta2Error::RemoveCharFromCharlistFail:
            strcpy(gTmpBuffer, "can't remove character from charlist");
            break;

        case Gta2Error::InvalidDoor:
            strcpy(gTmpBuffer, "Invalid door");
            break;

        case Gta2Error::ScriptfileCreateCarFail:
            strcpy(gTmpBuffer, "Failed to create a car in scriptfile.");
            break;

        case Gta2Error::ObjectHasDataWhenItShouldnt:
            strcpy(gTmpBuffer, "Object has data when it shouldn't");
            break;

        case Gta2Error::SidNegativeReferences:
            sprintf(gTmpBuffer, "Sid has negative references");
            break;

        case Gta2Error::SidTooManyReferences:
            sprintf(gTmpBuffer, "Sid has too many references");
            break;

        case Gta2Error::NoFreeTiles:
            strcpy(gTmpBuffer, "No free tiles");
            break;

        case Gta2Error::InvalidFileType:
            sprintf(gTmpBuffer, "Invalid file type in %s", sGlobalFileName);
            break;

        case Gta2Error::InvalidFileVersion:
            sprintf(gTmpBuffer, "Invalid file version in %s", sGlobalFileName);
            break;

        case Gta2Error::CharacterHasNoPedPointer_1:
        case Gta2Error::CharacterHasNoPedPointer_2:
            sprintf(gTmpBuffer, "Character has no ped pointer, bad ");
            break;

        case Gta2Error::InvalidObjectiveCoordinates:
            sprintf(gTmpBuffer, "Invalid objective coordinates");
            break;

        case Gta2Error::NoPlayerPointer:
            sprintf(gTmpBuffer, "No Player pointer");
            break;

        case Gta2Error::CharacterHasNoCarObjectivePointer_1:
        case Gta2Error::CharacterHasNoCarObjectivePointer_2:
            sprintf(gTmpBuffer, "Character has no car objective pointer,very bad");
            break;

        case Gta2Error::CharacterHasNoInCarPointer:
            sprintf(gTmpBuffer, "Character has no in_car pointer, bad");
            break;

        case Gta2Error::CharacterDefaultCase:
            sprintf(gTmpBuffer, "Character Default Case error");
            break;

        case Gta2Error::PedDefaultCase:
            sprintf(gTmpBuffer, "Ped Default Case error");
            break;

        case Gta2Error::PedSpriteZIs0:
            sprintf(gTmpBuffer, "Ped Sprite Z is 0, not good");
            break;

        case Gta2Error::PedsCarInfoIsNULL:
            sprintf(gTmpBuffer, "Peds Car Info is NULL , error indeed");
            break;

        case Gta2Error::GroupMemberNotRemoved:
            sprintf(gTmpBuffer, "Group member not removed");
            break;
        case Gta2Error::TooManyGroupMembers:
            sprintf(gTmpBuffer, "Too many members for the group to manage");
            break;
        case Gta2Error::GroupCreateTooManyGroupsInUse:
            sprintf(gTmpBuffer, "Cant create a group too many in use");
            break;

        case Gta2Error::CharacterNoFootOrCarPointer:
            sprintf(gTmpBuffer, "Character not got a foot or car pointer, very bad");
            break;

        case Gta2Error::CreateMedicalUnitFail:
            sprintf(gTmpBuffer, "Unable to Create A Medical Unit");
            break;

        case Gta2Error::TooManyTrafficLights:
            strcpy(gTmpBuffer, "Too many traffic lights");
            break;

        case Gta2Error::TooManyInfoAndNavigationZonesInMap:
            strcpy(gTmpBuffer, "Too many information & navigation zones in map");
            break;

        case Gta2Error::InvalidZoneForInfoSetting:
            strcpy(gTmpBuffer, "Invalid zone for information setting");
            break;

        case Gta2Error::FailedToCreateCarInRecycle:
            strcpy(gTmpBuffer, "Failed to create car in recycle.");
            break;

        case Gta2Error::EmptyStringPassedToFunction:
            strcpy(gTmpBuffer, "Empty String passed into function");
            break;

        case Gta2Error::FirstPlayerIsNULL:
            strcpy(gTmpBuffer, "Got NULL ptr for first player");
            break;

        case Gta2Error::NextPlayerIsNULL:
            strcpy(gTmpBuffer, "Got NULL ptr for next player");
            break;

        case Gta2Error::DivideByZero:
            strcpy(gTmpBuffer, "divide by zero");
            break;

        case Gta2Error::NullBlockPassedToCheckForArrowsInRoutefinderCode:
            strcpy(gTmpBuffer, "NULL block passed to check for arrows in routefinder code");
            break;

        case Gta2Error::TooManyJunctionsInMapForRoutefinder:
            strcpy(gTmpBuffer, "Too many junctions in map for routefinder!");
            break;

        case Gta2Error::GswTrainMovingAcrossSlope:
            strcpy(gTmpBuffer, "gsw - train moving across slope");
            break;

        case Gta2Error::GswTrainUnknownDirection:
            strcpy(gTmpBuffer, "gsw - train : unknown direction");
            break;

        case Gta2Error::GswTrainUnknownSlope:
            strcpy(gTmpBuffer, "gsw - train : unknown slope");
            break;

        case Gta2Error::CarTryingSuicideDidntDealWithDriver:
            strcpy(gTmpBuffer, "Car is trying to suicide and has not dealt with the driver");
            break;

        case Gta2Error::AIObjectiveRequiredByCharNoLongerActive:
            strcpy(gTmpBuffer, "AI objective required by a character is no longer active");
            break;

        case Gta2Error::InvalidCarRating:
            strcpy(gTmpBuffer, "Invalid Car Rating in Car Model Info");
            break;

        case Gta2Error::NoMoreSpaceIn_simp_text_DisplayBuffer:
            strcpy(gTmpBuffer, "No more space in simp_text display buffer");
            break;

        case Gta2Error::TooManyScoreTextures:
            strcpy(gTmpBuffer, "Too many score textures");
            break;

        case Gta2Error::TryingToGetUnitOfZeroVector:
            strcpy(gTmpBuffer, "Trying to get unit of zero vector");
            break;

        case Gta2Error::AssertionFail:
            sprintf(gTmpBuffer, "assertion failure in %s(line %d)", SourceFileNameFromPath(pFileName), lineNo);
            break;

        case Gta2Error::TryingToExpandAnEmptyNodeInRouterfinder:
            strcpy(gTmpBuffer, "Trying to expand an empty node in routefinder.");
            break;

        case Gta2Error::NoValidBackcarForAnCarRelatedStruct:
            strcpy(gTmpBuffer, "No valid backcar for an car related struct.");
            break;

        case Gta2Error::Gta2ManagerLaunchError:
            strcpy(gTmpBuffer, "Failed to launch 'GTA2 Manager' correctly.");
            break;

        case Gta2Error::MessageTextTooLongToWrap:
            strcpy(gTmpBuffer, "Message text too long to wrap");
            break;

        case Gta2Error::TooManyTimers:
            strcpy(gTmpBuffer, "Too many timers");
            break;

        case Gta2Error::TooManyArrows:
            strcpy(gTmpBuffer, "Too many arrows");
            break;

        case Gta2Error::TooManyMessages:
            strcpy(gTmpBuffer, "Too many messages");
            break;

        case Gta2Error::NoRestartZone:
            strcpy(gTmpBuffer, "No Restart Zone");
            break;

        case Gta2Error::MapNotCompressed:
            strcpy(gTmpBuffer, "Map not compressed");
            break;

        case Gta2Error::InvalidSmybolDataInSymboltableBuffer:
            strcpy(gTmpBuffer, "Invalid symbol data in symboltable buffer");
            break;

        case Gta2Error::InvalidMissionScriptFileSpecifiedInBob:
            sprintf(gTmpBuffer, "Invalid mission script file specified in Bob");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidCharacter:
            strcpy(gTmpBuffer, "Pointing arrow at a no longer valid character");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidCar:
            strcpy(gTmpBuffer, "Pointing arrow at a no longer valid car");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidObject:
            strcpy(gTmpBuffer, "Pointing arrow at a no longer valid object");
            break;

        case Gta2Error::CarRoutefindNoRoutfinderDataInMap:
            sprintf(gTmpBuffer, "Error doing car routefind: no routefinder data in map.");
            break;

        case Gta2Error::IllegalTrainStationNoPlatformZone:
            sprintf(gTmpBuffer, "Illegal Train Station: no platform zone");
            break;

        case Gta2Error::NoMoreTrainSpace:
            sprintf(gTmpBuffer, "Unable to create train: no more space! The game has too many trains/carriages/buses.");
            break;

        case Gta2Error::BaseChangeLogOverflow:
            strcpy(gTmpBuffer, "Base change log overflow");
            break;

        case Gta2Error::KeyboardTypeNotHandled:
            strcpy(gTmpBuffer, "Keyboard type not handled");
            break;

        case Gta2Error::KeyboardTypeFileOpenError:
            strcpy(gTmpBuffer, "Couldn't open file for keyboard type");
            break;

        case Gta2Error::SeqFileNotFound:
            strcpy(gTmpBuffer, "Couldn't find a .seq file in the data directory");
            break;

        case Gta2Error::MultipleSeqFilesFound:
            strcpy(gTmpBuffer, "There is more than one .seq file in the data directory");
            break;

        case Gta2Error::SeqFileOpenError:
            strcpy(gTmpBuffer, "Couldn't open .seq file in the data directory");
            break;

        case Gta2Error::TooManyMainBlocks:
            strcpy(gTmpBuffer, "The .seq file has too many MAIN blocks defined");
            break;

        case Gta2Error::MainBlockMustPrecedeBonus:
            strcpy(gTmpBuffer, "There must be a MAIN block before any BONUS block in .seq file");
            break;

        case Gta2Error::TooManyBonusBlocks:
            strcpy(gTmpBuffer, "The .seq file has too many BONUS blocks defined for a particular city");
            break;

        case Gta2Error::InvalidBlockType:
            strcpy(gTmpBuffer, "Each block in .seq file must be MAIN or BONUS");
            break;

        case Gta2Error::InvalidFirstLineCharacter:
            strcpy(gTmpBuffer, "First character of line in .seq should be a letter");
            break;

        case Gta2Error::LineInterruptedByNewline:
            strcpy(gTmpBuffer, "Line in .seq has been interrupted by newline before end");
            break;

        case Gta2Error::LabelTooLong:
            strcpy(gTmpBuffer, "Label in .seq file is too long");
            break;

        case Gta2Error::UndefinedLabel:
            strcpy(gTmpBuffer, "Undefined label in code for loading .seq file");
            break;

        case Gta2Error::LineDataTooLong:
            strcpy(gTmpBuffer, "Data for a line in .seq file is too long");
            break;

        case Gta2Error::UnexpectedLabel:
            strcpy(gTmpBuffer, "Unexpected label in .seq file - Check order and spelling");
            break;

        case Gta2Error::NoValidMenuOptions:
            strcpy(gTmpBuffer, "The menu contains no valid options");
            break;

        case Gta2Error::GmpFilenameMismatch:
            strcpy(gTmpBuffer, ".gmp filename in save game file does not match .gmp filename in .seq for same level");
            break;

        case Gta2Error::StyFilenameMismatch:
            strcpy(gTmpBuffer, ".sty filename in save game file does not match .sty filename in .seq for same level");
            break;

        case Gta2Error::ScrFilenameMismatch:
            strcpy(gTmpBuffer, ".scr filename in save game file does not match .scr filename in .seq for same level");
            break;

        case Gta2Error::BinkBufferOpenError:
            strcpy(gTmpBuffer, "Couldn't open bink buffer for playing movie");
            break;

        case Gta2Error::BinkOpenError:
            strcpy(gTmpBuffer, "Couldn't open bink for playing movie");
            break;

        case Gta2Error::CreditsLineTooLong:
            strcpy(gTmpBuffer, "Too many characters in credits line (max is 50)");
            break;

        case Gta2Error::TooManyCreditsLines:
            strcpy(gTmpBuffer, "Too many lines in credits (max is 600)");
            break;

        case Gta2Error::InvalidLevelAdvancement:
            strcpy(gTmpBuffer, "Shouldn't be allowed to move to next level if already on last level");
            break;

        case Gta2Error::LevelNotOpened:
            strcpy(gTmpBuffer, "Shouldn't be allowed to move to next level if it isn't opened");
            break;

        case Gta2Error::InvalidBackgroundImageSize:
            strcpy(gTmpBuffer, "Unexpected size of frontend background image");
            break;

        case Gta2Error::InvalidCreditTextColor:
            strcpy(gTmpBuffer, "Invalid colour for line of credit text");
            break;

        case Gta2Error::InvalidMultiplayerGameType:
            strcpy(gTmpBuffer, "Multiplayer game type should be frag, tag or score (but isn't)");
            break;

        case Gta2Error::UnableToHandleAttachedPed:
            strcpy(gTmpBuffer, "Unable to handle attached ped");
            break;

        case Gta2Error::EmptyReplayFile:
            strcpy(gTmpBuffer, "Attract mode replay file has no contents");
            break;

        case Gta2Error::ReplaySyncError:
            strcpy(gTmpBuffer, "Replay sync error");
            break;

        case Gta2Error::TargaMemoryAllocationError:
            strcpy(gTmpBuffer, "Couldn't allocate memory for targa file");
            break;

        case Gta2Error::TargaFileInvalidImage:
            strcpy(gTmpBuffer, "Targa file does not contain an image");
            break;

        case Gta2Error::TargaFileInvalidFormat:
            strcpy(gTmpBuffer, "Wrong format for targa file - expecting 16 bit uncompressed");
            break;

        case Gta2Error::TargaFileInvalidBitsPerPixel:
            strcpy(gTmpBuffer, "Targa file does not have 16 bits per pixel");
            break;

        case Gta2Error::TargaFileHasPalette:
            strcpy(gTmpBuffer, "Targa file has a palette - not expected");
            break;

        case Gta2Error::TargaFileInvalidOrigin:
            strcpy(gTmpBuffer, "Origin for targa file image not at (0,0)");
            break;

        case Gta2Error::TargaFileZeroWidthOrHeight:
            strcpy(gTmpBuffer, "Width or height of targa file image is 0");
            break;

        case Gta2Error::TargaFileIdReadError:
            strcpy(gTmpBuffer, "Couldn't read id from targa file");
            break;

        case Gta2Error::TargaFileImageReadError:
            strcpy(gTmpBuffer, "Error in reading image from targa file");
            break;

        case Gta2Error::InvalidCarModel:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid car model : %d", va_1);
            break;
        }

        case Gta2Error::MultipleSpriteDeltas:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Sprite %d has multiple delta entries", va_1);
            break;
        }

        case Gta2Error::TooManySpritesWithDeltas:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Too many sprites with deltas at %d", va_1);
            break;
        }

        case Gta2Error::InvalidAbsoluteSpriteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid absolute sprite number: %d", va_1);
            break;
        }

        case Gta2Error::TooManySprites:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Too many sprites at %d", va_1);
            break;
        }

        case Gta2Error::InvalidCase:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid case : %d", va_1);
            break;
        }

        case Gta2Error::InvalidAbsoluteVirtualPaletteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid absolute virtual palette number: %d", va_1);
            break;
        }

        case Gta2Error::InvalidFontNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid font number: %d", va_1);
            break;
        }

        case Gta2Error::InvalidTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid tile number: %d", va_1);
            break;
        }

        case Gta2Error::InvalidNewTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid new tile number: %d", va_1);
            break;
        }

        case Gta2Error::DllLoadError:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            err_a1_int("Unable to load DLL: %s", va_1);
            break;
        }

        case Gta2Error::InvalidPhysicalPaletteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid physical palette number: %d", va_1);
            break;
        }

        case Gta2Error::TooManyZones:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Too many zones at %d (max is 999)", va_1);
            break;
        }

        case Gta2Error::InvalidZoneNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid zone number: %d", va_1);
            break;
        }

        case Gta2Error::ZoneNotFoundError:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            err_a1_int("Zone not found : %s", va_1);
            break;
        }

        case Gta2Error::InvalidCarModelGearSpeeds:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("car model %d has invalid gear speeds in gci file", va_1);
            break;
        }

        case Gta2Error::InvalidMapObjectNumber:
        {
            s32 va_1 = va_arg(va, s32);
            err_a1_int("Invalid map object number: %d", va_1);
            break;
        }

        case Gta2Error::FileTooLarge:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "File too large: %s exceeds max size by %d bytes", sGlobalFileName, va_1);
            break;
        }

        case Gta2Error::OutOfMemory:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Out of memory allocating %d bytes", va_1);
            break;
        }

        case Gta2Error::InvalidChunkSize:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid chunk size : %d", va_1);
            break;
        }

        case Gta2Error::InvalidParameterListSwitch:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid switch in parameter list: %c", static_cast<char>(va_1));
            break;
        }

        case Gta2Error::InvalidLineInfo:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid line (info:%d)", va_1);
            break;
        }

        case Gta2Error::ReplayFileTooLarge:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Replay File larger than maximum: size %d", va_1);
            break;
        }

        case Gta2Error::InvalidVideomode:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid Videomode to search for: %d", va_1);
            break;
        }

        case Gta2Error::FullscreenVideomodeSetError:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to set fullscreen videomode: %d", va_1);
            break;
        }

        case Gta2Error::TooManyLoadedObjectInfos:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many loaded object infos at %d", va_1);
            break;
        }

        case Gta2Error::TooManyPlayers:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many players at %d", va_1);
            break;
        }

        case Gta2Error::InvalidPlayerNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid player number : %d", va_1);
            break;
        }

        case Gta2Error::InvalidGridWidth:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid grid width : %d", va_1);
            break;
        }

        case Gta2Error::InvalidGridHeight:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid grid height : %d", va_1);
            break;
        }

        case Gta2Error::InvalidZCoordinate:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid z coordinate : %d", va_1);
            break;
        }

        case Gta2Error::UnprocessedEvent:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Event left unprocessed : %08X", va_1);
            break;
        }

        case Gta2Error::ListTypeOutOfSpace:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Out of space for list type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::NoCollisionBoxInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "No collision box in sprite %d", va_1);
            break;
        }

        case Gta2Error::InvalidCollisionBoxInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Collision box invalid in sprite %d", va_1);
            break;
        }

        case Gta2Error::InvalidObjectModelNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid object model number : %d", va_1);
            break;
        }

        case Gta2Error::ObjectModelNumberAlreadyAdded:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "object model number %d already added", va_1);
            break;
        }

        case Gta2Error::NoSpaceForObjectModelNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "no space for object model number %d", va_1);
            break;
        }

        case Gta2Error::CheckWallsSlopeSizeTooBig:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "check walls slope size too big : %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateObjectInScriptfile:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create an object in scriptfile. Object type: %d", va_1);
            break;
        }

        case Gta2Error::AttemptToKillDeadMissionThread:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Attempt to kill an already dead mission thread. Debug line %d", va_1);
            break;
        }

        case Gta2Error::TooManyCarDoors:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Car model %d has too many doors", va_1);
            break;
        }

        case Gta2Error::ReleasingNullPtrInReleasePassive:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Releasing null ptr in release_passive. List type %s", gListTypes_61AB70[va_1]);
            break;
        }
        case Gta2Error::TryingToPacifyNullListElement:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Trying to 'pacify' a NULL list element. List type %s", gListTypes_61AB70[va_1]);
            break;
        }
        case Gta2Error::ObjectBackPointerMissingInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Object back pointer missing in sprite %d", va_1);
            break;
        }

        case Gta2Error::NoRigidBodyInCar:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "No rigid body in car ID %d", va_1);
            break;
        }
        case Gta2Error::NoFreeSidForCharacter:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "No free Sid for character %d", va_1);
            break;
        }
        case Gta2Error::InvalidDoorInfoNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid door info number : %d", va_1);
            break;
        }
        case Gta2Error::IllegalAngleValue:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal Value for an angle: %d", va_1);
            break;
        }
        case Gta2Error::InvalidTileTexture:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid tile texture: %d", va_1);
            break;
        }
        case Gta2Error::InvalidCacheNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid cache number : %d", va_1);
            break;
        }
        case Gta2Error::InvalidScoreTextureNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid score texture number : %d", va_1);
            break;
        }

        case Gta2Error::InvalidZoneInformation:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "Setting invalid zone information : %s", va_1);
            break;
        }

        case Gta2Error::CounterOutOfBoundsInternalCount:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Counter has out of bounds internal count: %d", va_1);
            break;
        }

        case Gta2Error::OutOfRangeRouteId:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Out of range route id: %d", va_1);
            break;
        }

        case Gta2Error::InvalidScorePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid score position: %d", va_1);
            break;
        }

        case Gta2Error::InvalidPlayerId:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid player ID: %d", va_1);
            break;
        }

        case Gta2Error::TooManyCharactersInScore:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too characters in score : %d", va_1);
            break;
        }

        case Gta2Error::InvalidTextFile:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "Invalid text file : %s", va_1);
            break;
        }

        case Gta2Error::StringStartsWithEndOfLine:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "String starts with end-of-line : %s", va_1);
            break;
        }

        case Gta2Error::CarInCollisionWithWallAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "car ID %d in collision with wall at safe position", va_1);
            break;
        }

        case Gta2Error::ObjectInCollisionWithWallAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "object ID %d in collision with wall at safe position", va_1);
            break;
        }

        case Gta2Error::IllegalDeviceCode:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal device code for this machine: %d", va_1);
            break;
        }

        case Gta2Error::ActivatingNullPtrInActivate:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Activating null ptr in activate. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::ActivatingItemTwice:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer, "Activating item twice. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::IllegalTurntableIndex:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal index into turntable: %d", va_1);
            break;
        }

        case Gta2Error::TooManyYRoadSegments:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many Y road segments. Num: %d", va_1);
            break;
        }

        case Gta2Error::TooManyXRoadSegments:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many X road segments. Num: %d", va_1);
            break;
        }
        case Gta2Error::InvalidTimerHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid timer handle : %d", va_1);
            break;
        }

        case Gta2Error::InvalidArrowHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid arrow handle : %d", va_1);
            break;
        }

        case Gta2Error::UnallocatedArrowHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unallocated arrow handle : %d", va_1);
            break;
        }

        case Gta2Error::CarAlreadyDoingRoutefind:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Car already doing routefind, but requested another. Car ID: %d", va_1);
            break;
        }

        case Gta2Error::CounterCheckFailed:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Counter check failed for list type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::InvalidCarModelStyleData:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Car model %d has invalid style data", va_1);
            break;
        }

        case Gta2Error::ReleasingTwiceInReleasePassive:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Releasing twice in release_passive. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::ReleasingTwiceInRelease:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Releasing twice in release. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::AssertionFailure:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "assertion failure in %s(line %d): %s", SourceFileNameFromPath(pFileName), lineNo, va_1);
            break;
        }
        case Gta2Error::TooManySymbols:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many symbols at %d (max is 999)", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCharacterInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create_a_character in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::FailedToFindUidInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to find a uid in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::IllegalZoneIdInSymbolTable:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal zone id in symbol table: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateArrowInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create arrow in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCarThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing car that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCharacterThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing character that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingObjectThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing object that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCounterThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing counter that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCharacterWithNoGroup:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing character with no group when it should have group: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingTriggerThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing trigger that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCraneThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing crane that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineOrderingDriverOutOfCarButCharIsNotDriver:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line attempting to order driver out of car, but char isn't a driver!: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingArrowThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing arrow that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::AttemptingToDoSetGangInfoOnInvalidGangName:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Attempting to do 'SET_GANG_INFO' on invalid gang name in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCraneInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create a crane in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateConveyorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create a conveyor in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateGeneratorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create a generator in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateDestructorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create a destructor in script line: %d", va_1);
            break;
        }

        case Gta2Error::AttemptingToPos32ArrowAtLogicalItem:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Attempted to point arrow at a 'logical' item, not a physical item. Script line: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingDoorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing door that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCrusherInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Failed to create a crusher in script line: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingConveyorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing conveyor that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingGeneratorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing generator that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::InvalidSpecialTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid special tile number : %d", va_1);
            break;
        }

        case Gta2Error::RepeatedSpecialTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Repeated special tile number : %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingDestructorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Line accessing destructor that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::CheckingPhoneInMissionscriptWithoutExecutingAnswerPhone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Checking phone in missionscript without executing ANSWER_PHONE: uid: %d", va_1);
            break;
        }

        case Gta2Error::MapHasTooManyColumns:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "map has too many columns by %d words", va_1);
            break;
        }

        case Gta2Error::MapHasTooManyBlocks:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "map has %d blocks too many ", va_1);
            break;
        }

        case Gta2Error::StyleFileContainsUnexpectedNumberOfFonts:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "style file contains unexpected number of fonts %d", va_1);
            break;
        }

        case Gta2Error::AttemptingIsItemOnscreenOnIllegalType:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Attempting 'is_item_onscreen' on an illegal type: uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToCreateCarToPlaceOnTopOfTrailerInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to create car to place on top of trailer in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameToSetupInfoForInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find zonename to 'setup' info for in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameInSetGangInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find zonename in 'set_gang_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find zonename in 'set_station_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindPlatformInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find platform in 'set_station_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindTrainstationAssociatedWithPlatformInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer,
                    "Unable to find trainstation associated with platform in 'set_station_info' in script. uid: %d",
                    va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInSetGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find gang matching name of gangone in a 'set gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find gang matching name of gangtwo in a 'set gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangnameInAlterGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find gangname in an 'alter gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::TryingToChangeGangRespectOnNonPlayerControlledCharacter:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Trying to change gangrespect on a non-player controlled character. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInCheckGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find gang matching name in a 'check gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZoneMatchingNameInIsCharInGangzone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find zone matching name in a 'is char in gangzone'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZoneMatchingNameInIsCharInZone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find zone matching name in a 'is char in zone'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInAddCharToGang:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Unable to find gang matching name in a 'add char to gang'. uid: %d", va_1);
            break;
        }

        case Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Mission script is referring to a 'text id' that doesn't exist. ID: %d", va_1);
            break;
        }

        case Gta2Error::ErrorWith1Param:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Error with 1 param: %d", va_1);
            break;
        }

        case Gta2Error::InvalidRelativeSpriteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "Invalid relative sprite number %d in %s", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidRelativeVirtualPaletteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "Invalid relative virtual palette number %d in %s", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidRelativeFontSpriteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid relative font sprite number %d in font %d", va_2, va_2);
            break;
        }

        case Gta2Error::IncorrectAspectRatioInScreenMode:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Incorrect aspect ratio in screen mode : %d x %d", va_2, va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtGrab:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "List type %s corrupted at grab at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtRelease:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "List type %s corrupted at release at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtTraverse:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "List type %s corrupted at traverse at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtActivate:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "List type %s corrupted at activate at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtPacify:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "List type %s corrupted at pacify at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::CantDeleteElementFromList:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Can't delete element %d from list type %s", va_2, gListTypes_61AB70[va_2]);
            break;
        }

        case Gta2Error::InvalidZsInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "invalid zs in sprite %d : %d", va_2, va_2);
            break;
        }

        case Gta2Error::DoorTooCloseInCarModel:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Door %d of car model %d too close", va_2, va_2);
            break;
        }

        case Gta2Error::InvalidSpriteTypeAndNumber:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid sprite type %d number %d", va_2, va_2);
            break;
        }

        case Gta2Error::TrafficLightsTooCloseToEdgeOfWorld:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "traffic lights at (%d,%d) are too close to the edge of the world", va_2, va_2);
            break;
        }

        case Gta2Error::InvalidFrameInObjectModel:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid frame %d in object model %d", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalCoordinate:
        {
            Fix16_Point_POD c;
            c.x = *va_arg(va, s32*);
            c.y = *va_arg(va, s32*);
            sprintf(gTmpBuffer, "Illegal coordinate: (%.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat());

            break;
        }

        case Gta2Error::IllegalGoodBadRatioForCarModelChoice:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal good/bad ratio for car model choice: good %d, bad %d", va_2, va_2);
            break;
        }

        case Gta2Error::ModelRepeatedInFile:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "model %d repeated in %s", reinterpret_cast<s32>(va_2), SourceFileNameFromPath(va_2));
            break;
        }

        case Gta2Error::BinmakeError:
            sprintf(gTmpBuffer, "%s binmake error : %s", SourceFileNameFromPath(gErrStr), gErrStr2);
            break;

        case Gta2Error::InvalidSize:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "%s invalid size at %d bytes", SourceFileNameFromPath(va_2), reinterpret_cast<s32>(va_2));
            break;
        }

        case Gta2Error::CarInCollisionWithSpriteAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "car ID %d in collision with %s sprite at safe position", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::ObjectInCollisionWithSpriteAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "objecct ID %d in collision with %s sprite at safe position", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidCraneTarget:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Invalid crane target : (%d,%d)", va_2, va_2);
            break;
        }

        case Gta2Error::TooManyDoorInfosDeclaredInScript:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Too many door infos declared in script: curr door: closedframe %d openframe %d", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoEntryZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal Train Station (%d, %d): no entry zone", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoExitZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal Train Station (%d, %d): no exit zone", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoStopZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Illegal Train Station (%d, %d): no stop zone", va_2, va_2);
            break;
        }

        case Gta2Error::ErrorWith2Params:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Error with 2 param: %d,%d", va_2, va_2);
            break;
        }

        case Gta2Error::IncorrectReplayVersion:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "incorrect replay version : %d.%d", va_2, va_2);
            break;
        }

        case Gta2Error::SyncErrorRandom:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Sync error (random) remote=%d local=%d", va_2, va_2);
            break;
        }

        case Gta2Error::SyncErrorMission:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer, "Sync error (mission) remote=%d local=%d", va_2, va_2);
            break;
        }

        case Gta2Error::VideomodeNotAvailable:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Videomode %dx%dx%d is not available", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::FailedToSetVideomode:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Failed to set videomode %dx%dx%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::CantDeleteSpriteFromCollisionList:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Can't delete sprite %d from collision list at : %d,%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AddingSpriteToCollisionListTwice:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Adding sprite %d to collision list twice at : %d,%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidMapAccess:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "invalid map access : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidMapChangeAccess:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "invalid map change access : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidPaletteTypeForSpriteTypeAndNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Invalid palette type %d for sprite type %d, number %d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidSpriteTypeAndNumberRemap:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Invalid sprite type %d number %d remap %d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::ModelCountMismatchInStyleFile:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer,
                    "%d models in %s but %d models in style",
                    reinterpret_cast<s32>(va_3),
                    SourceFileNameFromPath(reinterpret_cast<const char_type*>(va_3)),
                    reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidBlockInTrainCode:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Invalid block in train code : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::TrainOnBlockWithIncorrectArrow:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Train on a block with incorrect arrow : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AttemptingToDisableRoadAtIllegalCoordinates:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Attempting to disable road at illegal coordinates: check Z! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AttemptingToEnableRoadAtIllegalCoordinates:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Attempting to enable road at illegal coordinates: check Z! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::IllegalBlockForTrainCreation:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Illegal block for train creation. Coordinates: check stopzone! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::FailedToCreateCarriage:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer, "Failed to create carriage at: (%d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::ErrorInSetupOfParkCommand:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer,
                    "Error in set-up of park command. Walls intruding on the park's triggerspace! Door coords: (%d, %d, %d)",
                    reinterpret_cast<s32>(va_3),
                    reinterpret_cast<s32>(va_3),
                    reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidXCoordinateInSpriteType:
        {
            s32 va_1 = va_arg(va, s32);

            Fix16_Vec c;

            c.x = *va_arg(va, s32*);
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);

            sprintf(gTmpBuffer,
                    "Invalid x co-ord in sprite type %d at (%f,%f,%f)",
                    va_1,
                    c.x.AsFloat(),
                    c.y.AsFloat(),
                    c.z.AsFloat());
        }
        break;

        case Gta2Error::InvalidYCoordinateInSpriteType:
        {
            s32 va_1 = va_arg(va, s32);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer,
                    "Invalid y co-ord in sprite type %d at (%f,%f,%f)",
                    va_1,
                    c.x.AsFloat(),
                    c.y.AsFloat(),
                    c.z.AsFloat());
            break;
        }

        case Gta2Error::InvalidZCoordinateInSpriteType:
        {
            s32 va_1 = va_arg(va, s32);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer,
                    "Invalid z co-ord in sprite type %d at (%f,%f,%f)",
                    va_1,
                    c.x.AsFloat(),
                    c.y.AsFloat(),
                    c.z.AsFloat());
            break;
        }

        case Gta2Error::PlacingGangPhoneWhenNoGang:
        {
            const char_type* va_1 = va_arg(va, const char_type*);

            Fix16_Point_POD c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            sprintf(gTmpBuffer, "Placing gang phone when no gang at: (%.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat());
            break;
        }

        case Gta2Error::IllegalCoordinate3D:
        {
            const char_type* va_1 = va_arg(va, const char_type*);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer, "Illegal coordinate: (%.4f, %.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat(), c.z.AsFloat());
            break;
        }

        case Gta2Error::InvalidDiagonalBlock:
        {
            const char_type* va_1 = va_arg(va, const char_type*);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer, "Invalid diagonal block at (%.4f, %.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat(), c.z.AsFloat());
            break;
        }

        case Gta2Error::NoDirectionArrowFoundInBlockWithSlope:
        {
            s32* va_1 = va_arg(va, s32*);
            Fix16_Vec c;
            c.x = *va_arg(va, s32*);
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer,
                    "No direction arrow found in block (with slope!!). ( %f, %f, %f)",
                    c.x.AsFloat(),
                    c.y.AsFloat(),
                    c.z.AsFloat());
            break;
        }

        case Gta2Error::DirectXError:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer, "DirectX Error: %s", va_1);
            break;
        }

        case Gta2Error::TooManyPhonesForGang:
            sprintf(gTmpBuffer, "Too many %s phones for %s gang", gErrStr, gErrStr2);
            break;
        case Gta2Error::TooManyLinesInBriefing:

            sprintf(gTmpBuffer, "Too many lines in briefing %s", gErrStr);
            break;

        case Gta2Error::CouldntFindAttractModeFile:
            sprintf(gTmpBuffer, "Couldn't find attract mode file %s", gErrStr);
            break;

        case Gta2Error::TextIdentifierNotFound:
            sprintf(gTmpBuffer, "text identifier not found : %s", gErrStr);
            break;

        case Gta2Error::BriefingTooLong:
            sprintf(gTmpBuffer, "Briefing %s too long", gErrStr);
            break;

        case Gta2Error::BriefingTooLongToWrap:
            sprintf(gTmpBuffer, "Briefing %s too long to wrap", gErrStr);
            break;

        case Gta2Error::InvalidFraction:
        {
            Fix16 value = *va_arg(va, Fix16*);
            sprintf(gTmpBuffer, "Invalid fraction : %f", value.AsFloat());
            break;
        }

        case Gta2Error::InvalidAmbientLightValue:
        {
            Fix16 value = *va_arg(va, Fix16*);
            sprintf(gTmpBuffer, "Invalid ambient light value : %f", value.AsFloat());
            break;
        }
        case Gta2Error::UnableToFindRoute:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);

            // One array with the "to" half stored back to front: this gives the original's stack slots.
            // Two arrays or six scalars get laid out differently whatever the declaration order.
            s32 coords[6];
            coords[0] = *va_arg(va, s32*);
            coords[1] = *va_arg(va, s32*);
            coords[2] = *va_arg(va, s32*);

            coords[5] = *va_arg(va, s32*);
            coords[4] = *va_arg(va, s32*);
            coords[3] = *va_arg(va, s32*);

            sprintf(gTmpBuffer,
                    "Unable to find a route from (%.4f, %.4f, %.4f) to (%.4f, %.4f, %.4f) in car id: %d model: %d",
                    coords[0] / 16384.0f,
                    coords[1] / 16384.0f,
                    coords[2] / 16384.0f,
                    coords[5] / 16384.0f,
                    coords[4] / 16384.0f,
                    coords[3] / 16384.0f,
                    va_1,
                    va_2);
        }
        break;

        case Gta2Error::ListError: // moved case
            sprintf(gTmpBuffer, "List Error see Kivlin");
            break;

        default:
            sprintf(gTmpBuffer, "No Error Message Declared");
            break;
    }

    ErrorMsgBox(gTmpBuffer);
    gErrorLog_67CF58.Write(gTmpBuffer);
}

MATCH_FUNC(0x4A38C0)
EXPORT void FatalError(s32 Code, const char_type* pSourceFile, s32 lineNo, ...)
{
    if (Code == Gta2Error::FileOpenError)
    {
        const char_type destroyed = bDestroyed;
        if (!destroyed)
        {
            ShutdownGame();
            GBH_Graphis_DMA_Video_Free();
            // 16 was using esi instead of push 10h without the 0x6F5B70 bool
            // being cached in both branches of the if/else
            ReportFatalError(Gta2Error::FileOpenError, pSourceFile, lineNo);
        }
        DestroyWindow(gHwnd);
        exit(16);
    }
    else
    {
        const char_type destroyed = bDestroyed;
        if (!destroyed)
        {
            ShutdownGame();
            GBH_Graphis_DMA_Video_Free();

            va_list va;
            va_start(va, lineNo);

            if (Code < 1000)
            {
                ReportFatalError(Code, pSourceFile, lineNo);
            }
            else if (Code < 2000)
            {
                s32 arg1 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg1);
            }
            else if (Code < 3000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg2, arg2);
            }
            else if (Code < 4000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                s32 arg3 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg3, arg3, arg3);
            }
            else if (Code < 5000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                s32 arg3 = va_arg(va, s32);
                s32 arg4 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg1, arg2, arg3, arg4);
            }
            else if (Code < 6000)
            {
                s32 arg1 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg1);
            }
            else if (Code < 7000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                s32 arg3 = va_arg(va, s32);
                s32 arg4 = va_arg(va, s32);
                s32 arg5 = va_arg(va, s32);
                s32 arg6 = va_arg(va, s32);
                s32 arg7 = va_arg(va, s32);
                s32 arg8 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            }
            else if (Code < 8000)
            {
                ReportFatalError(Code, pSourceFile, lineNo);
            }
            else if (Code < 9000)
            {
                ReportFatalError(Code, pSourceFile, lineNo);
            }
            else if (Code == 9011)
            {
                s32 arg1 = va_arg(va, s32);
                ReportFatalError(Code, pSourceFile, lineNo, arg1);
            }
            else
            {
                ReportFatalError(Code, pSourceFile, lineNo);
            }
        }
        DestroyWindow(gHwnd);
        exit(Code);
    }
}
// Called by diutil.cpp (InputRecorder_67F8B0.cpp) when a DirectInput call fails. The cases are in the original's
// body order. The empty DI_OK case is needed: without it VC6 splits the switch differently (compares instead of
// the byte-indexed table for 0x8007000E-0x80070057), which shifts the register rotation and so which cases
// cross-jump into the default's tail.
MATCH_FUNC(0x4A3CF0)
EXPORT void __stdcall FatalDXError(HRESULT hr, const char_type* pSourceFile, s32 lineNo)
{
    switch (hr)
    {
        case 0x80004001: // E_NOTIMPL / DIERR_UNSUPPORTED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "the interface is not implemented");
            break;

        case 0x8000000A: // E_PENDING
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "pending");
            break;

        case 0x80004002: // E_NOINTERFACE / DIERR_NOINTERFACE
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "the interface is not supported");
            break;

        case 0x80004005: // E_FAIL / DIERR_GENERIC
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "undetermined failure");
            break;

        case 0x80040110: // CLASS_E_NOAGGREGATION
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "object does not support aggregation");
            break;

        case 0x80040202: // DIERR_MOREDATA
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "not all requested information fitted into the buffer");
            break;

        case 0x80040201: // DIERR_DEVICEFULL
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device is full");
            break;

        case 0x80040154: // REGDB_E_CLASSNOTREG
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "class is not registered");
            break;

        case 0x80040203: // DIERR_NOTDOWNLOADED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect is not downloaded");
            break;

        case 0x80040208: // DIERR_EFFECTPLAYING
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect is still playing");
            break;

        case 0x80040204: // DIERR_HASEFFECTS
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device still has effects attached");
            break;

        case 0x80040206: // DIERR_INCOMPLETEEFFECT
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect could not be downloaded because essential information is missing");
            break;

        case 0x80040207: // DIERR_NOTBUFFERED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device is not buffered");
            break;

        case 0x80040205: // DIERR_NOTEXCLUSIVEACQUIRED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device must be acquired in exclusive mode");
            break;

        case 0x80070002: // DIERR_OBJECTNOTFOUND
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "object does not exist");
            break;

        case 0x80070005: // E_ACCESSDENIED / DIERR_OTHERAPPHASPRIO
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "access denied");
            break;

        case 0x8007000C: // DIERR_NOTACQUIRED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device must be acquired");
            break;

        case 0x80070057: // E_INVALIDARG
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "invalid argument");
            break;

        case 0x8007000E: // E_OUTOFMEMORY
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "out of memory");
            break;

        case 0x8007001E: // DIERR_INPUTLOST
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "access to the input device has been lost");
            break;

        case 0x80070015: // DIERR_NOTINITIALIZED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "object has not been initialized");
            break;

        case 0x80070077: // DIERR_BADDRIVERVER
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "incompatible driver version or mismatched or incomplete driver components");
            break;

        case 0x80070481: // DIERR_BETADIRECTINPUTVERSION
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "application was written for an unsupported prerelease version of DirectInput");
            break;

        case 0x8007047E: // DIERR_OLDDIRECTINPUTVERSION
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "requires a newer version of DirectInput");
            break;

        case 0x800700AA: // DIERR_ACQUIRED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device is acquired");
            break;

        case 0x800704DF: // DIERR_ALREADYINITIALIZED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "object is already initialized");
            break;

        case 0: // DI_OK: returns without an error (the jump table entry goes to the ret)
            break;

        case 1: // S_FALSE / DI_NOEFFECT
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "the function had no effect");
            break;

        case 3: // DI_DOWNLOADSKIPPED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect could not be downloaded");
            break;

        case 4: // DI_EFFECTRESTARTED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect has been restarted");
            break;

        case 2: // DI_POLLEDDEVICE
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "device is a polled device");
            break;

        case 8: // DI_TRUNCATED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect parameters have been truncated");
            break;

        case 12: // DI_TRUNCATEDANDRESTARTED
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "effect parameters have been truncated and effect has been restarted");
            break;

        default:
            FatalError(Gta2Error::DirectXError, pSourceFile, lineNo, "unknown error");
            break;
    }
}
