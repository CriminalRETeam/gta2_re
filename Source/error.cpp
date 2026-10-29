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

DEFINE_GLOBAL(char_type, bDestroyed_6F5B70, 0x6F5B70);
DEFINE_GLOBAL(HWND, gHwnd_707F04, 0x707F04);

MATCH_FUNC(0x4D9470)
void ErrorLog::Open_4D9470(const char_type* path, s32 truncate_mode)
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
    this->field_3C_pLen = fileNameLen;

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
    static_cast<ofstream&>(field_0_ofstr).open(path, mode, filebuf::openprot);
#endif

    log_timestamp_4D9540();
}

// The member ofstream's constructor call and EH frame come from the real ofstream member
// (error.hpp, VC6 only)
MATCH_FUNC(0x4D94E0)
ErrorLog::ErrorLog(const char_type* FileName, s32 truncate_mode)
{
    Open_4D9470(FileName, truncate_mode);
}

// This func matches but for some reason it's crashing the patched version
MATCH_FUNC(0x4D9690)
EXPORT void __cdecl log_on_line_written_cb_4D9690(void* pStream)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type*>(pStream)->flush();
#endif
}

MATCH_FUNC(0x4D9620)
void ErrorLog::Write_4D9620(const char_type* pMsg)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type&>(this->field_0_ofstr) << pMsg << endl << flush;
#endif
}

MATCH_FUNC(0x4D9650)
void ErrorLog::Write_Log_4D9650(const char_type* buffer)
{
#if !defined(__clang__) && (_MSC_VER <= 1200)
    static_cast<ostream_type&>(this->field_0_ofstr) << buffer << flush;
#endif
}

MATCH_FUNC(0x4D9540)
EXPORT void ErrorLog::log_timestamp_4D9540()
{
    time_t curTime = time(NULL);
    char_type* buffer = ctime(&curTime);
    buffer[strlen(buffer) - 1] = 0; // remove the new line it  adds
    sprintf(gTmpBuffer_67C598, "\n------ %s ------", buffer);
    Write_4D9620(gTmpBuffer_67C598);
}

MATCH_FUNC(0x4D9670)
void* ErrorLog::log_on_line_written_4D9670(TLogLineCallback pCallBack)
{
    pCallBack(this);
    return this;
}

// Logs the intro line again on the way out, then closes the file
MATCH_FUNC(0x4A3610)
ErrorLog::~ErrorLog()
{
    log_intro_4D95A0();
#if !defined(__clang__) && (_MSC_VER <= 1200)
    field_0_ofstr.close();
#endif
    delete field_3C_pLen;
}

MATCH_FUNC(0x4D95A0)
void ErrorLog::log_intro_4D95A0()
{
    sprintf(gTmpBuffer_67C598, "Coop v%d.%d", gGTA2VersionMajor_708280, gGTA2VersionMinor_708284);
    Write_4D9620(gTmpBuffer_67C598);

    time_t curTime = time(NULL);
    char_type* buffer = ctime(&curTime);
    buffer[strlen(buffer) - 1] = 0; // remove the new line it  adds
    sprintf(gTmpBuffer_67C598, "\n------ %s ------", buffer);
    Write_4D9620(gTmpBuffer_67C598);
}

DEFINE_GLOBAL_INIT(ErrorLog, gErrorLog_67CF58, ErrorLog("test\\error.log", 1), 0x67CF58);
DEFINE_GLOBAL_INIT(ErrorLog, gErrorLog_67C530, ErrorLog("test\\warning.log", 1), 0x67C530);
DEFINE_GLOBAL(ErrorLog, gMiss2Log_6F7698, 0x6F7698);

DEFINE_GLOBAL_ARRAY(char_type, gTmpBuffer_67C598, 256, 0x67C598);
DEFINE_GLOBAL_ARRAY(char_type, gErrStr_67C29C, 256, 0x67C29C);
DEFINE_GLOBAL_ARRAY(char_type, gErrStr2_67C3A8, 256, 0x67C3A8);
DEFINE_GLOBAL_ARRAY(char_type, gGlobalFileName_67C6AC, 256, 0x67C6AC);

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
EXPORT void __stdcall Error_SetName_4A0770(const char_type* pFileName)
{
    strcpy(gGlobalFileName_67C6AC, pFileName);
}

MATCH_FUNC(0x4A07A0)
EXPORT const char_type* __stdcall SourceFileNameFromPath_4A07A0(const char_type* pPath)
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

#define err_a1_int(msg, arg) sprintf(gTmpBuffer_67C598, msg, arg);

MATCH_FUNC(0x4A07C0)
EXPORT void FatalError_4A07C0(s32 code, const char_type* pFileName, s32 lineNo, ...)
{
    va_list va;

    va_start(va, lineNo);

    sprintf(gTmpBuffer_67C598, "Fatal Error %d in %s(%d)", code, SourceFileNameFromPath_4A07A0(pFileName), lineNo);
    gErrorLog_67CF58.Write_4D9620(gTmpBuffer_67C598);

    switch (code)
    {
        case Gta2Error::DMAVideoInitFail:
            sprintf(gTmpBuffer_67C598, "Can't Init DMAVideo.");
            break;

        case Gta2Error::SecurityFail:
            sprintf(gTmpBuffer_67C598, "security failure");
            break;

        case Gta2Error::SetWindowModeFail:
            sprintf(gTmpBuffer_67C598, "Failed to set WindowMode");
            break;

        case Gta2Error::GetValidHDCFail:
            sprintf(gTmpBuffer_67C598, "Failed to get a valid HDC");
            break;

        case Gta2Error::SetWindowPosFail:
            sprintf(gTmpBuffer_67C598, "Failed to SetWindowPos properly");
            break;

        case Gta2Error::DirectInputInitFail:
            sprintf(gTmpBuffer_67C598, "Failed to Initialise DirectInput");
            break;

        case Gta2Error::DirectInputSetDataFormatFail:
            sprintf(gTmpBuffer_67C598, "Failed to set Data Format for DirectInput");
            break;

        case Gta2Error::DirectInputSetCooperativeLevelFail:
            sprintf(gTmpBuffer_67C598, "Failed to set Cooperative Level for DirectInput");
            break;

        case Gta2Error::DirectInputNoObject:
            sprintf(gTmpBuffer_67C598, "No DirectInput Object");
            break;

        case Gta2Error::DirectInputCreateFail:
            sprintf(gTmpBuffer_67C598, "Failed to create DirectInput object");
            break;

        case Gta2Error::ZeroBytesAlloc:
            strcpy(gTmpBuffer_67C598, "Attempting to allocate zero bytes of memory");
            break;

        case Gta2Error::FtellError:
            sprintf(gTmpBuffer_67C598, "Ftell error in file: %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileReadFailure:
            sprintf(gTmpBuffer_67C598, "Read failure in file: %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FreeloaderEpisodeUnknown:
            sprintf(gTmpBuffer_67C598,
                    "Install Next freeloader episode please. [Temp message].Unable to open file : %s",
                    gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileCloseError:
            sprintf(gTmpBuffer_67C598, "Unable to close file: %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileBiggerThanBuffer:
            sprintf(gTmpBuffer_67C598, "Trying to a load a file too big for buffer: %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::WritingZeroBytesToFile:
            sprintf(gTmpBuffer_67C598, "Writing zero bytes to file : %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileWriteFailure:
            sprintf(gTmpBuffer_67C598, "Write failure on file : %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileReadingNotOpenedFile:
            strcpy(gTmpBuffer_67C598, "Attempt to read from not opened file");
            break;

        case Gta2Error::ungetcFailrue:
            sprintf(gTmpBuffer_67C598, "ungetc failure in file %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileInvalidInteger:
            sprintf(gTmpBuffer_67C598, "invalid integer encountered in file %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::FileEndOfFileInTextString:
            sprintf(gTmpBuffer_67C598, "End of file in text string in file %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::TextStringTooLong:
            strcpy(gTmpBuffer_67C598, "Text string too long");
            break;

        case Gta2Error::OutOfMemoryNewOperator:
            strcpy(gTmpBuffer_67C598, "Out of memory - new operator failed");
            break;

        case Gta2Error::FileOverflow:
            strcpy(gTmpBuffer_67C598, "file overflow");
            break;

        case Gta2Error::DataNotLoaded:
            strcpy(gTmpBuffer_67C598, "data not loaded");
            break;

        case Gta2Error::InvalidStringInParameterList:
            sprintf(gTmpBuffer_67C598, "Invalid string in parameter list: %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::RepeatedFileInParameterList:
            sprintf(gTmpBuffer_67C598, "Repeated file in parameter list : %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::InvalidZoneData:
            strcpy(gTmpBuffer_67C598, "Invalid zone data");
            break;

        case Gta2Error::InvalidMapObjectData:
            strcpy(gTmpBuffer_67C598, "Invalid map object data");
            break;

        case Gta2Error::InvalidMapLightData:
            strcpy(gTmpBuffer_67C598, "Invalid map light data");
            break;

        case Gta2Error::InvalidLoadedObjectInfoData:
            strcpy(gTmpBuffer_67C598, "Invalid loaded object info data");
            break;

        case Gta2Error::CloseRegistryKeyFail:
            strcpy(gTmpBuffer_67C598, "unable to close registry key");
            break;

        case Gta2Error::CreateRegistryKeyFail:
            strcpy(gTmpBuffer_67C598, "unable to create registry key");
            break;

        case Gta2Error::NoPlayers:
            strcpy(gTmpBuffer_67C598, "No players");
            break;

        case Gta2Error::ClassNotAllocated:
            strcpy(gTmpBuffer_67C598, "Class not allocated");
            break;

        case Gta2Error::SetRegistryValueFail:
            strcpy(gTmpBuffer_67C598, "Unable to set registry value");
            break;

        case Gta2Error::LoopInTree:
            strcpy(gTmpBuffer_67C598, "Loop in tree");
            break;

        case Gta2Error::DisplayTreeOutOfSpace:
            strcpy(gTmpBuffer_67C598, "Out of space for display tree");
            break;

        case Gta2Error::FileWriteByteFail:
            sprintf(gTmpBuffer_67C598, "Failed writing a byte to file %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::UsingUninitialisedSprite:
            strcpy(gTmpBuffer_67C598, "Using uninitialised sprite");
            break;

        case Gta2Error::InvalidCorners:
            sprintf(gTmpBuffer_67C598, "Invalid corners");
            break;

        case Gta2Error::MissionProcessAllocMemFail:
            sprintf(gTmpBuffer_67C598, "Failed to allocate memory for new mission process");
            break;

        case Gta2Error::MissionProcessOutOfSlots:
            sprintf(gTmpBuffer_67C598, "No more process slots available for mission processes");
            break;

        case Gta2Error::CheckWallsSlopeInvalidSlope:
            strcpy(gTmpBuffer_67C598, "check walls slope invalid slope");
            break;

        case Gta2Error::ScriptfileTooMany_PLAYER_PED_Declarations:
            sprintf(gTmpBuffer_67C598, "Too many PLAYER_PED declarations in scriptfile");
            break;

        case Gta2Error::ScriptfileNo_LEVELSTART_Declaration:
            sprintf(gTmpBuffer_67C598, "No LEVELSTART declaration in scriptfile");
            break;

        case Gta2Error::ScriptfileFailedToCreatePed:
            sprintf(gTmpBuffer_67C598, "Failed to create a ped in scriptfile");
            break;

        case Gta2Error::MapChangeOverflowBlocks:
            strcpy(gTmpBuffer_67C598, "map change overflow - too many new blocks");
            break;

        case Gta2Error::MapChangeOverflowColumns:
            strcpy(gTmpBuffer_67C598, "map change overflow - too many new columns");
            break;

        case Gta2Error::MissionStackAllocFail:
            strcpy(gTmpBuffer_67C598, "Failed to allocate memory for mission stack");
            break;

        case Gta2Error::MissionStackPushNullItem:
            strcpy(gTmpBuffer_67C598, "Tried to push NULL item onto mission stack");
            break;

        case Gta2Error::MissStackItemGrabNoStackSpaceLeftFail:
            strcpy(gTmpBuffer_67C598, "Failed grabbing miss_stack_item - no stack space left");
            break;

        case Gta2Error::RemoveCharFromCharlistFail:
            strcpy(gTmpBuffer_67C598, "can't remove character from charlist");
            break;

        case Gta2Error::InvalidDoor:
            strcpy(gTmpBuffer_67C598, "Invalid door");
            break;

        case Gta2Error::ScriptfileCreateCarFail:
            strcpy(gTmpBuffer_67C598, "Failed to create a car in scriptfile.");
            break;

        case Gta2Error::ObjectHasDataWhenItShouldnt:
            strcpy(gTmpBuffer_67C598, "Object has data when it shouldn't");
            break;

        case Gta2Error::SidNegativeReferences:
            sprintf(gTmpBuffer_67C598, "Sid has negative references");
            break;

        case Gta2Error::SidTooManyReferences:
            sprintf(gTmpBuffer_67C598, "Sid has too many references");
            break;

        case Gta2Error::NoFreeTiles:
            strcpy(gTmpBuffer_67C598, "No free tiles");
            break;

        case Gta2Error::InvalidFileType:
            sprintf(gTmpBuffer_67C598, "Invalid file type in %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::InvalidFileVersion:
            sprintf(gTmpBuffer_67C598, "Invalid file version in %s", gGlobalFileName_67C6AC);
            break;

        case Gta2Error::CharacterHasNoPedPointer_1:
        case Gta2Error::CharacterHasNoPedPointer_2:
            sprintf(gTmpBuffer_67C598, "Character has no ped pointer, bad ");
            break;

        case Gta2Error::InvalidObjectiveCoordinates:
            sprintf(gTmpBuffer_67C598, "Invalid objective coordinates");
            break;

        case Gta2Error::NoPlayerPointer:
            sprintf(gTmpBuffer_67C598, "No Player pointer");
            break;

        case Gta2Error::CharacterHasNoCarObjectivePointer_1:
        case Gta2Error::CharacterHasNoCarObjectivePointer_2:
            sprintf(gTmpBuffer_67C598, "Character has no car objective pointer,very bad");
            break;

        case Gta2Error::CharacterHasNoInCarPointer:
            sprintf(gTmpBuffer_67C598, "Character has no in_car pointer, bad");
            break;

        case Gta2Error::CharacterDefaultCase:
            sprintf(gTmpBuffer_67C598, "Character Default Case error");
            break;

        case Gta2Error::PedDefaultCase:
            sprintf(gTmpBuffer_67C598, "Ped Default Case error");
            break;

        case Gta2Error::PedSpriteZIs0:
            sprintf(gTmpBuffer_67C598, "Ped Sprite Z is 0, not good");
            break;

        case Gta2Error::PedsCarInfoIsNULL:
            sprintf(gTmpBuffer_67C598, "Peds Car Info is NULL , error indeed");
            break;

        case Gta2Error::GroupMemberNotRemoved:
            sprintf(gTmpBuffer_67C598, "Group member not removed");
            break;
        case Gta2Error::TooManyGroupMembers:
            sprintf(gTmpBuffer_67C598, "Too many members for the group to manage");
            break;
        case Gta2Error::GroupCreateTooManyGroupsInUse:
            sprintf(gTmpBuffer_67C598, "Cant create a group too many in use");
            break;

        case Gta2Error::CharacterNoFootOrCarPointer:
            sprintf(gTmpBuffer_67C598, "Character not got a foot or car pointer, very bad");
            break;

        case Gta2Error::CreateMedicalUnitFail:
            sprintf(gTmpBuffer_67C598, "Unable to Create A Medical Unit");
            break;

        case Gta2Error::TooManyTrafficLights:
            strcpy(gTmpBuffer_67C598, "Too many traffic lights");
            break;

        case Gta2Error::TooManyInfoAndNavigationZonesInMap:
            strcpy(gTmpBuffer_67C598, "Too many information & navigation zones in map");
            break;

        case Gta2Error::InvalidZoneForInfoSetting:
            strcpy(gTmpBuffer_67C598, "Invalid zone for information setting");
            break;

        case Gta2Error::FailedToCreateCarInRecycle:
            strcpy(gTmpBuffer_67C598, "Failed to create car in recycle.");
            break;

        case Gta2Error::EmptyStringPassedToFunction:
            strcpy(gTmpBuffer_67C598, "Empty String passed into function");
            break;

        case Gta2Error::FirstPlayerIsNULL:
            strcpy(gTmpBuffer_67C598, "Got NULL ptr for first player");
            break;

        case Gta2Error::NextPlayerIsNULL:
            strcpy(gTmpBuffer_67C598, "Got NULL ptr for next player");
            break;

        case Gta2Error::DivideByZero:
            strcpy(gTmpBuffer_67C598, "divide by zero");
            break;

        case Gta2Error::NullBlockPassedToCheckForArrowsInRoutefinderCode:
            strcpy(gTmpBuffer_67C598, "NULL block passed to check for arrows in routefinder code");
            break;

        case Gta2Error::TooManyJunctionsInMapForRoutefinder:
            strcpy(gTmpBuffer_67C598, "Too many junctions in map for routefinder!");
            break;

        case Gta2Error::GswTrainMovingAcrossSlope:
            strcpy(gTmpBuffer_67C598, "gsw - train moving across slope");
            break;

        case Gta2Error::GswTrainUnknownDirection:
            strcpy(gTmpBuffer_67C598, "gsw - train : unknown direction");
            break;

        case Gta2Error::GswTrainUnknownSlope:
            strcpy(gTmpBuffer_67C598, "gsw - train : unknown slope");
            break;

        case Gta2Error::CarTryingSuicideDidntDealWithDriver:
            strcpy(gTmpBuffer_67C598, "Car is trying to suicide and has not dealt with the driver");
            break;

        case Gta2Error::AIObjectiveRequiredByCharNoLongerActive:
            strcpy(gTmpBuffer_67C598, "AI objective required by a character is no longer active");
            break;

        case Gta2Error::InvalidCarRating:
            strcpy(gTmpBuffer_67C598, "Invalid Car Rating in Car Model Info");
            break;

        case Gta2Error::NoMoreSpaceIn_simp_text_DisplayBuffer:
            strcpy(gTmpBuffer_67C598, "No more space in simp_text display buffer");
            break;

        case Gta2Error::TooManyScoreTextures:
            strcpy(gTmpBuffer_67C598, "Too many score textures");
            break;

        case Gta2Error::TryingToGetUnitOfZeroVector:
            strcpy(gTmpBuffer_67C598, "Trying to get unit of zero vector");
            break;

        case Gta2Error::AssertionFail:
            sprintf(gTmpBuffer_67C598, "assertion failure in %s(line %d)", SourceFileNameFromPath_4A07A0(pFileName), lineNo);
            break;

        case Gta2Error::TryingToExpandAnEmptyNodeInRouterfinder:
            strcpy(gTmpBuffer_67C598, "Trying to expand an empty node in routefinder.");
            break;

        case Gta2Error::NoValidBackcarForAnCarRelatedStruct:
            strcpy(gTmpBuffer_67C598, "No valid backcar for an car related struct.");
            break;

        case Gta2Error::Gta2ManagerLaunchError:
            strcpy(gTmpBuffer_67C598, "Failed to launch 'GTA2 Manager' correctly.");
            break;

        case Gta2Error::MessageTextTooLongToWrap:
            strcpy(gTmpBuffer_67C598, "Message text too long to wrap");
            break;

        case Gta2Error::TooManyTimers:
            strcpy(gTmpBuffer_67C598, "Too many timers");
            break;

        case Gta2Error::TooManyArrows:
            strcpy(gTmpBuffer_67C598, "Too many arrows");
            break;

        case Gta2Error::TooManyMessages:
            strcpy(gTmpBuffer_67C598, "Too many messages");
            break;

        case Gta2Error::NoRestartZone:
            strcpy(gTmpBuffer_67C598, "No Restart Zone");
            break;

        case Gta2Error::MapNotCompressed:
            strcpy(gTmpBuffer_67C598, "Map not compressed");
            break;

        case Gta2Error::InvalidSmybolDataInSymboltableBuffer:
            strcpy(gTmpBuffer_67C598, "Invalid symbol data in symboltable buffer");
            break;

        case Gta2Error::InvalidMissionScriptFileSpecifiedInBob:
            sprintf(gTmpBuffer_67C598, "Invalid mission script file specified in Bob");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidCharacter:
            strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid character");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidCar:
            strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid car");
            break;

        case Gta2Error::PointingArrowAtANoLongerValidObject:
            strcpy(gTmpBuffer_67C598, "Pointing arrow at a no longer valid object");
            break;

        case Gta2Error::CarRoutefindNoRoutfinderDataInMap:
            sprintf(gTmpBuffer_67C598, "Error doing car routefind: no routefinder data in map.");
            break;

        case Gta2Error::IllegalTrainStationNoPlatformZone:
            sprintf(gTmpBuffer_67C598, "Illegal Train Station: no platform zone");
            break;

        case Gta2Error::NoMoreTrainSpace:
            sprintf(gTmpBuffer_67C598, "Unable to create train: no more space! The game has too many trains/carriages/buses.");
            break;

        case Gta2Error::BaseChangeLogOverflow:
            strcpy(gTmpBuffer_67C598, "Base change log overflow");
            break;

        case Gta2Error::KeyboardTypeNotHandled:
            strcpy(gTmpBuffer_67C598, "Keyboard type not handled");
            break;

        case Gta2Error::KeyboardTypeFileOpenError:
            strcpy(gTmpBuffer_67C598, "Couldn't open file for keyboard type");
            break;

        case Gta2Error::SeqFileNotFound:
            strcpy(gTmpBuffer_67C598, "Couldn't find a .seq file in the data directory");
            break;

        case Gta2Error::MultipleSeqFilesFound:
            strcpy(gTmpBuffer_67C598, "There is more than one .seq file in the data directory");
            break;

        case Gta2Error::SeqFileOpenError:
            strcpy(gTmpBuffer_67C598, "Couldn't open .seq file in the data directory");
            break;

        case Gta2Error::TooManyMainBlocks:
            strcpy(gTmpBuffer_67C598, "The .seq file has too many MAIN blocks defined");
            break;

        case Gta2Error::MainBlockMustPrecedeBonus:
            strcpy(gTmpBuffer_67C598, "There must be a MAIN block before any BONUS block in .seq file");
            break;

        case Gta2Error::TooManyBonusBlocks:
            strcpy(gTmpBuffer_67C598, "The .seq file has too many BONUS blocks defined for a particular city");
            break;

        case Gta2Error::InvalidBlockType:
            strcpy(gTmpBuffer_67C598, "Each block in .seq file must be MAIN or BONUS");
            break;

        case Gta2Error::InvalidFirstLineCharacter:
            strcpy(gTmpBuffer_67C598, "First character of line in .seq should be a letter");
            break;

        case Gta2Error::LineInterruptedByNewline:
            strcpy(gTmpBuffer_67C598, "Line in .seq has been interrupted by newline before end");
            break;

        case Gta2Error::LabelTooLong:
            strcpy(gTmpBuffer_67C598, "Label in .seq file is too long");
            break;

        case Gta2Error::UndefinedLabel:
            strcpy(gTmpBuffer_67C598, "Undefined label in code for loading .seq file");
            break;

        case Gta2Error::LineDataTooLong:
            strcpy(gTmpBuffer_67C598, "Data for a line in .seq file is too long");
            break;

        case Gta2Error::UnexpectedLabel:
            strcpy(gTmpBuffer_67C598, "Unexpected label in .seq file - Check order and spelling");
            break;

        case Gta2Error::NoValidMenuOptions:
            strcpy(gTmpBuffer_67C598, "The menu contains no valid options");
            break;

        case Gta2Error::GmpFilenameMismatch:
            strcpy(gTmpBuffer_67C598, ".gmp filename in save game file does not match .gmp filename in .seq for same level");
            break;

        case Gta2Error::StyFilenameMismatch:
            strcpy(gTmpBuffer_67C598, ".sty filename in save game file does not match .sty filename in .seq for same level");
            break;

        case Gta2Error::ScrFilenameMismatch:
            strcpy(gTmpBuffer_67C598, ".scr filename in save game file does not match .scr filename in .seq for same level");
            break;

        case Gta2Error::BinkBufferOpenError:
            strcpy(gTmpBuffer_67C598, "Couldn't open bink buffer for playing movie");
            break;

        case Gta2Error::BinkOpenError:
            strcpy(gTmpBuffer_67C598, "Couldn't open bink for playing movie");
            break;

        case Gta2Error::CreditsLineTooLong:
            strcpy(gTmpBuffer_67C598, "Too many characters in credits line (max is 50)");
            break;

        case Gta2Error::TooManyCreditsLines:
            strcpy(gTmpBuffer_67C598, "Too many lines in credits (max is 600)");
            break;

        case Gta2Error::InvalidLevelAdvancement:
            strcpy(gTmpBuffer_67C598, "Shouldn't be allowed to move to next level if already on last level");
            break;

        case Gta2Error::LevelNotOpened:
            strcpy(gTmpBuffer_67C598, "Shouldn't be allowed to move to next level if it isn't opened");
            break;

        case Gta2Error::InvalidBackgroundImageSize:
            strcpy(gTmpBuffer_67C598, "Unexpected size of frontend background image");
            break;

        case Gta2Error::InvalidCreditTextColor:
            strcpy(gTmpBuffer_67C598, "Invalid colour for line of credit text");
            break;

        case Gta2Error::InvalidMultiplayerGameType:
            strcpy(gTmpBuffer_67C598, "Multiplayer game type should be frag, tag or score (but isn't)");
            break;

        case Gta2Error::UnableToHandleAttachedPed:
            strcpy(gTmpBuffer_67C598, "Unable to handle attached ped");
            break;

        case Gta2Error::EmptyReplayFile:
            strcpy(gTmpBuffer_67C598, "Attract mode replay file has no contents");
            break;

        case Gta2Error::ReplaySyncError:
            strcpy(gTmpBuffer_67C598, "Replay sync error");
            break;

        case Gta2Error::TargaMemoryAllocationError:
            strcpy(gTmpBuffer_67C598, "Couldn't allocate memory for targa file");
            break;

        case Gta2Error::TargaFileInvalidImage:
            strcpy(gTmpBuffer_67C598, "Targa file does not contain an image");
            break;

        case Gta2Error::TargaFileInvalidFormat:
            strcpy(gTmpBuffer_67C598, "Wrong format for targa file - expecting 16 bit uncompressed");
            break;

        case Gta2Error::TargaFileInvalidBitsPerPixel:
            strcpy(gTmpBuffer_67C598, "Targa file does not have 16 bits per pixel");
            break;

        case Gta2Error::TargaFileHasPalette:
            strcpy(gTmpBuffer_67C598, "Targa file has a palette - not expected");
            break;

        case Gta2Error::TargaFileInvalidOrigin:
            strcpy(gTmpBuffer_67C598, "Origin for targa file image not at (0,0)");
            break;

        case Gta2Error::TargaFileZeroWidthOrHeight:
            strcpy(gTmpBuffer_67C598, "Width or height of targa file image is 0");
            break;

        case Gta2Error::TargaFileIdReadError:
            strcpy(gTmpBuffer_67C598, "Couldn't read id from targa file");
            break;

        case Gta2Error::TargaFileImageReadError:
            strcpy(gTmpBuffer_67C598, "Error in reading image from targa file");
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
            sprintf(gTmpBuffer_67C598, "File too large: %s exceeds max size by %d bytes", gGlobalFileName_67C6AC, va_1);
            break;
        }

        case Gta2Error::OutOfMemory:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Out of memory allocating %d bytes", va_1);
            break;
        }

        case Gta2Error::InvalidChunkSize:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid chunk size : %d", va_1);
            break;
        }

        case Gta2Error::InvalidParameterListSwitch:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid switch in parameter list: %c", static_cast<char>(va_1));
            break;
        }

        case Gta2Error::InvalidLineInfo:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid line (info:%d)", va_1);
            break;
        }

        case Gta2Error::ReplayFileTooLarge:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Replay File larger than maximum: size %d", va_1);
            break;
        }

        case Gta2Error::InvalidVideomode:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid Videomode to search for: %d", va_1);
            break;
        }

        case Gta2Error::FullscreenVideomodeSetError:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to set fullscreen videomode: %d", va_1);
            break;
        }

        case Gta2Error::TooManyLoadedObjectInfos:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many loaded object infos at %d", va_1);
            break;
        }

        case Gta2Error::TooManyPlayers:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many players at %d", va_1);
            break;
        }

        case Gta2Error::InvalidPlayerNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid player number : %d", va_1);
            break;
        }

        case Gta2Error::InvalidGridWidth:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid grid width : %d", va_1);
            break;
        }

        case Gta2Error::InvalidGridHeight:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid grid height : %d", va_1);
            break;
        }

        case Gta2Error::InvalidZCoordinate:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid z coordinate : %d", va_1);
            break;
        }

        case Gta2Error::UnprocessedEvent:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Event left unprocessed : %08X", va_1);
            break;
        }

        case Gta2Error::ListTypeOutOfSpace:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Out of space for list type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::NoCollisionBoxInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "No collision box in sprite %d", va_1);
            break;
        }

        case Gta2Error::InvalidCollisionBoxInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Collision box invalid in sprite %d", va_1);
            break;
        }

        case Gta2Error::InvalidObjectModelNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid object model number : %d", va_1);
            break;
        }

        case Gta2Error::ObjectModelNumberAlreadyAdded:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "object model number %d already added", va_1);
            break;
        }

        case Gta2Error::NoSpaceForObjectModelNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "no space for object model number %d", va_1);
            break;
        }

        case Gta2Error::CheckWallsSlopeSizeTooBig:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "check walls slope size too big : %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateObjectInScriptfile:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create an object in scriptfile. Object type: %d", va_1);
            break;
        }

        case Gta2Error::AttemptToKillDeadMissionThread:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Attempt to kill an already dead mission thread. Debug line %d", va_1);
            break;
        }

        case Gta2Error::TooManyCarDoors:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Car model %d has too many doors", va_1);
            break;
        }

        case Gta2Error::ReleasingNullPtrInReleasePassive:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Releasing null ptr in release_passive. List type %s", gListTypes_61AB70[va_1]);
            break;
        }
        case Gta2Error::TryingToPacifyNullListElement:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Trying to 'pacify' a NULL list element. List type %s", gListTypes_61AB70[va_1]);
            break;
        }
        case Gta2Error::ObjectBackPointerMissingInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Object back pointer missing in sprite %d", va_1);
            break;
        }

        case Gta2Error::NoRigidBodyInCar:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "No rigid body in car ID %d", va_1);
            break;
        }
        case Gta2Error::NoFreeSidForCharacter:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "No free Sid for character %d", va_1);
            break;
        }
        case Gta2Error::InvalidDoorInfoNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid door info number : %d", va_1);
            break;
        }
        case Gta2Error::IllegalAngleValue:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal Value for an angle: %d", va_1);
            break;
        }
        case Gta2Error::InvalidTileTexture:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid tile texture: %d", va_1);
            break;
        }
        case Gta2Error::InvalidCacheNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid cache number : %d", va_1);
            break;
        }
        case Gta2Error::InvalidScoreTextureNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid score texture number : %d", va_1);
            break;
        }

        case Gta2Error::InvalidZoneInformation:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "Setting invalid zone information : %s", va_1);
            break;
        }

        case Gta2Error::CounterOutOfBoundsInternalCount:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Counter has out of bounds internal count: %d", va_1);
            break;
        }

        case Gta2Error::OutOfRangeRouteId:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Out of range route id: %d", va_1);
            break;
        }

        case Gta2Error::InvalidScorePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid score position: %d", va_1);
            break;
        }

        case Gta2Error::InvalidPlayerId:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid player ID: %d", va_1);
            break;
        }

        case Gta2Error::TooManyCharactersInScore:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too characters in score : %d", va_1);
            break;
        }

        case Gta2Error::InvalidTextFile:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "Invalid text file : %s", va_1);
            break;
        }

        case Gta2Error::StringStartsWithEndOfLine:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "String starts with end-of-line : %s", va_1);
            break;
        }

        case Gta2Error::CarInCollisionWithWallAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "car ID %d in collision with wall at safe position", va_1);
            break;
        }

        case Gta2Error::ObjectInCollisionWithWallAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "object ID %d in collision with wall at safe position", va_1);
            break;
        }

        case Gta2Error::IllegalDeviceCode:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal device code for this machine: %d", va_1);
            break;
        }

        case Gta2Error::ActivatingNullPtrInActivate:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Activating null ptr in activate. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::ActivatingItemTwice:
        {
            u32 va_1 = va_arg(va, u32);
            sprintf(gTmpBuffer_67C598, "Activating item twice. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::IllegalTurntableIndex:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal index into turntable: %d", va_1);
            break;
        }

        case Gta2Error::TooManyYRoadSegments:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many Y road segments. Num: %d", va_1);
            break;
        }

        case Gta2Error::TooManyXRoadSegments:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many X road segments. Num: %d", va_1);
            break;
        }
        case Gta2Error::InvalidTimerHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid timer handle : %d", va_1);
            break;
        }

        case Gta2Error::InvalidArrowHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid arrow handle : %d", va_1);
            break;
        }

        case Gta2Error::UnallocatedArrowHandle:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unallocated arrow handle : %d", va_1);
            break;
        }

        case Gta2Error::CarAlreadyDoingRoutefind:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Car already doing routefind, but requested another. Car ID: %d", va_1);
            break;
        }

        case Gta2Error::CounterCheckFailed:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Counter check failed for list type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::InvalidCarModelStyleData:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Car model %d has invalid style data", va_1);
            break;
        }

        case Gta2Error::ReleasingTwiceInReleasePassive:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Releasing twice in release_passive. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::ReleasingTwiceInRelease:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Releasing twice in release. List type %s", gListTypes_61AB70[va_1]);
            break;
        }

        case Gta2Error::AssertionFailure:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "assertion failure in %s(line %d): %s", SourceFileNameFromPath_4A07A0(pFileName), lineNo, va_1);
            break;
        }
        case Gta2Error::TooManySymbols:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many symbols at %d (max is 999)", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCharacterInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create_a_character in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::FailedToFindUidInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to find a uid in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::IllegalZoneIdInSymbolTable:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal zone id in symbol table: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateArrowInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create arrow in script: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCarThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing car that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCharacterThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing character that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingObjectThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing object that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCounterThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing counter that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCharacterWithNoGroup:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing character with no group when it should have group: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingTriggerThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing trigger that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingCraneThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing crane that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineOrderingDriverOutOfCarButCharIsNotDriver:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line attempting to order driver out of car, but char isn't a driver!: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingArrowThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing arrow that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::AttemptingToDoSetGangInfoOnInvalidGangName:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Attempting to do 'SET_GANG_INFO' on invalid gang name in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCraneInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create a crane in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateConveyorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create a conveyor in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateGeneratorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create a generator in script line: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateDestructorInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create a destructor in script line: %d", va_1);
            break;
        }

        case Gta2Error::AttemptingToPos32ArrowAtLogicalItem:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Attempted to point arrow at a 'logical' item, not a physical item. Script line: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingDoorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing door that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::FailedToCreateCrusherInScriptLine:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Failed to create a crusher in script line: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingConveyorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing conveyor that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingGeneratorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing generator that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::InvalidSpecialTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid special tile number : %d", va_1);
            break;
        }

        case Gta2Error::RepeatedSpecialTileNumber:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Repeated special tile number : %d", va_1);
            break;
        }

        case Gta2Error::LineAccessingDestructorThatHasntBeenCreated:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Line accessing destructor that hasn't been created: uid: %d", va_1);
            break;
        }

        case Gta2Error::CheckingPhoneInMissionscriptWithoutExecutingAnswerPhone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Checking phone in missionscript without executing ANSWER_PHONE: uid: %d", va_1);
            break;
        }

        case Gta2Error::MapHasTooManyColumns:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "map has too many columns by %d words", va_1);
            break;
        }

        case Gta2Error::MapHasTooManyBlocks:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "map has %d blocks too many ", va_1);
            break;
        }

        case Gta2Error::StyleFileContainsUnexpectedNumberOfFonts:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "style file contains unexpected number of fonts %d", va_1);
            break;
        }

        case Gta2Error::AttemptingIsItemOnscreenOnIllegalType:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Attempting 'is_item_onscreen' on an illegal type: uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToCreateCarToPlaceOnTopOfTrailerInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to create car to place on top of trailer in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameToSetupInfoForInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find zonename to 'setup' info for in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameInSetGangInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find zonename in 'set_gang_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZonenameInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find zonename in 'set_station_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindPlatformInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find platform in 'set_station_info' in script. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindTrainstationAssociatedWithPlatformInSetStationInfoInScript:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598,
                    "Unable to find trainstation associated with platform in 'set_station_info' in script. uid: %d",
                    va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInSetGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find gang matching name of gangone in a 'set gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find gang matching name of gangtwo in a 'set gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangnameInAlterGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find gangname in an 'alter gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::TryingToChangeGangRespectOnNonPlayerControlledCharacter:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Trying to change gangrespect on a non-player controlled character. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInCheckGangRespect:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find gang matching name in a 'check gang respect'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZoneMatchingNameInIsCharInGangzone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find zone matching name in a 'is char in gangzone'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindZoneMatchingNameInIsCharInZone:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find zone matching name in a 'is char in zone'. uid: %d", va_1);
            break;
        }

        case Gta2Error::UnableToFindGangMatchingNameInAddCharToGang:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Unable to find gang matching name in a 'add char to gang'. uid: %d", va_1);
            break;
        }

        case Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Mission script is referring to a 'text id' that doesn't exist. ID: %d", va_1);
            break;
        }

        case Gta2Error::ErrorWith1Param:
        {
            s32 va_1 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Error with 1 param: %d", va_1);
            break;
        }

        case Gta2Error::InvalidRelativeSpriteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "Invalid relative sprite number %d in %s", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidRelativeVirtualPaletteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "Invalid relative virtual palette number %d in %s", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidRelativeFontSpriteNumber:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid relative font sprite number %d in font %d", va_2, va_2);
            break;
        }

        case Gta2Error::IncorrectAspectRatioInScreenMode:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Incorrect aspect ratio in screen mode : %d x %d", va_2, va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtGrab:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "List type %s corrupted at grab at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtRelease:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "List type %s corrupted at release at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtTraverse:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "List type %s corrupted at traverse at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtActivate:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "List type %s corrupted at activate at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::ListTypeCorruptedAtPacify:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "List type %s corrupted at pacify at %d elements", gListTypes_61AB70[va_2], va_2);
            break;
        }

        case Gta2Error::CantDeleteElementFromList:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Can't delete element %d from list type %s", va_2, gListTypes_61AB70[va_2]);
            break;
        }

        case Gta2Error::InvalidZsInSprite:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "invalid zs in sprite %d : %d", va_2, va_2);
            break;
        }

        case Gta2Error::DoorTooCloseInCarModel:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Door %d of car model %d too close", va_2, va_2);
            break;
        }

        case Gta2Error::InvalidSpriteTypeAndNumber:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid sprite type %d number %d", va_2, va_2);
            break;
        }

        case Gta2Error::TrafficLightsTooCloseToEdgeOfWorld:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "traffic lights at (%d,%d) are too close to the edge of the world", va_2, va_2);
            break;
        }

        case Gta2Error::InvalidFrameInObjectModel:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid frame %d in object model %d", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalCoordinate:
        {
            Fix16_Point_POD c;
            c.x = *va_arg(va, s32*);
            c.y = *va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Illegal coordinate: (%.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat());

            break;
        }

        case Gta2Error::IllegalGoodBadRatioForCarModelChoice:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal good/bad ratio for car model choice: good %d, bad %d", va_2, va_2);
            break;
        }

        case Gta2Error::ModelRepeatedInFile:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "model %d repeated in %s", reinterpret_cast<s32>(va_2), SourceFileNameFromPath_4A07A0(va_2));
            break;
        }

        case Gta2Error::BinmakeError:
            sprintf(gTmpBuffer_67C598, "%s binmake error : %s", SourceFileNameFromPath_4A07A0(gErrStr_67C29C), gErrStr2_67C3A8);
            break;

        case Gta2Error::InvalidSize:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "%s invalid size at %d bytes", SourceFileNameFromPath_4A07A0(va_2), reinterpret_cast<s32>(va_2));
            break;
        }

        case Gta2Error::CarInCollisionWithSpriteAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "car ID %d in collision with %s sprite at safe position", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::ObjectInCollisionWithSpriteAtSafePosition:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "objecct ID %d in collision with %s sprite at safe position", reinterpret_cast<s32>(va_2), va_2);
            break;
        }

        case Gta2Error::InvalidCraneTarget:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Invalid crane target : (%d,%d)", va_2, va_2);
            break;
        }

        case Gta2Error::TooManyDoorInfosDeclaredInScript:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Too many door infos declared in script: curr door: closedframe %d openframe %d", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoEntryZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no entry zone", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoExitZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no exit zone", va_2, va_2);
            break;
        }

        case Gta2Error::IllegalTrainStationNoStopZone:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Illegal Train Station (%d, %d): no stop zone", va_2, va_2);
            break;
        }

        case Gta2Error::ErrorWith2Params:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Error with 2 param: %d,%d", va_2, va_2);
            break;
        }

        case Gta2Error::IncorrectReplayVersion:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "incorrect replay version : %d.%d", va_2, va_2);
            break;
        }

        case Gta2Error::SyncErrorRandom:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Sync error (random) remote=%d local=%d", va_2, va_2);
            break;
        }

        case Gta2Error::SyncErrorMission:
        {
            s32 va_1 = va_arg(va, s32);
            s32 va_2 = va_arg(va, s32);
            sprintf(gTmpBuffer_67C598, "Sync error (mission) remote=%d local=%d", va_2, va_2);
            break;
        }

        case Gta2Error::VideomodeNotAvailable:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Videomode %dx%dx%d is not available", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::FailedToSetVideomode:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Failed to set videomode %dx%dx%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::CantDeleteSpriteFromCollisionList:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Can't delete sprite %d from collision list at : %d,%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AddingSpriteToCollisionListTwice:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Adding sprite %d to collision list twice at : %d,%d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidMapAccess:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "invalid map access : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidMapChangeAccess:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "invalid map change access : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidPaletteTypeForSpriteTypeAndNumber:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Invalid palette type %d for sprite type %d, number %d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidSpriteTypeAndNumberRemap:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Invalid sprite type %d number %d remap %d", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::ModelCountMismatchInStyleFile:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598,
                    "%d models in %s but %d models in style",
                    reinterpret_cast<s32>(va_3),
                    SourceFileNameFromPath_4A07A0(reinterpret_cast<const char_type*>(va_3)),
                    reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::InvalidBlockInTrainCode:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Invalid block in train code : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::TrainOnBlockWithIncorrectArrow:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Train on a block with incorrect arrow : (%d,%d,%d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AttemptingToDisableRoadAtIllegalCoordinates:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Attempting to disable road at illegal coordinates: check Z! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::AttemptingToEnableRoadAtIllegalCoordinates:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Attempting to enable road at illegal coordinates: check Z! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::IllegalBlockForTrainCreation:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Illegal block for train creation. Coordinates: check stopzone! ( %d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::FailedToCreateCarriage:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Failed to create carriage at: (%d, %d, %d)", reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3), reinterpret_cast<s32>(va_3));
            break;
        }

        case Gta2Error::ErrorInSetupOfParkCommand:
        {
            s32 va_1 = va_arg(va, s32);
            const char_type* va_2 = va_arg(va, const char_type*);
            s32* va_3 = va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598,
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

            sprintf(gTmpBuffer_67C598,
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
            sprintf(gTmpBuffer_67C598,
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
            sprintf(gTmpBuffer_67C598,
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
            sprintf(gTmpBuffer_67C598, "Placing gang phone when no gang at: (%.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat());
            break;
        }

        case Gta2Error::IllegalCoordinate3D:
        {
            const char_type* va_1 = va_arg(va, const char_type*);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Illegal coordinate: (%.4f, %.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat(), c.z.AsFloat());
            break;
        }

        case Gta2Error::InvalidDiagonalBlock:
        {
            const char_type* va_1 = va_arg(va, const char_type*);

            Fix16_Vec c;
            c.x = *reinterpret_cast<const s32*>(va_arg(va, const char_type*));
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598, "Invalid diagonal block at (%.4f, %.4f, %.4f)", c.x.AsFloat(), c.y.AsFloat(), c.z.AsFloat());
            break;
        }

        case Gta2Error::NoDirectionArrowFoundInBlockWithSlope:
        {
            s32* va_1 = va_arg(va, s32*);
            Fix16_Vec c;
            c.x = *va_arg(va, s32*);
            c.y = *va_arg(va, s32*);
            c.z = *va_arg(va, s32*);
            sprintf(gTmpBuffer_67C598,
                    "No direction arrow found in block (with slope!!). ( %f, %f, %f)",
                    c.x.AsFloat(),
                    c.y.AsFloat(),
                    c.z.AsFloat());
            break;
        }

        case Gta2Error::DirectXError:
        {
            const char_type* va_1 = va_arg(va, const char_type*);
            sprintf(gTmpBuffer_67C598, "DirectX Error: %s", va_1);
            break;
        }

        case Gta2Error::TooManyPhonesForGang:
            sprintf(gTmpBuffer_67C598, "Too many %s phones for %s gang", gErrStr_67C29C, gErrStr2_67C3A8);
            break;
        case Gta2Error::TooManyLinesInBriefing:

            sprintf(gTmpBuffer_67C598, "Too many lines in briefing %s", gErrStr_67C29C);
            break;

        case Gta2Error::CouldntFindAttractModeFile:
            sprintf(gTmpBuffer_67C598, "Couldn't find attract mode file %s", gErrStr_67C29C);
            break;

        case Gta2Error::TextIdentifierNotFound:
            sprintf(gTmpBuffer_67C598, "text identifier not found : %s", gErrStr_67C29C);
            break;

        case Gta2Error::BriefingTooLong:
            sprintf(gTmpBuffer_67C598, "Briefing %s too long", gErrStr_67C29C);
            break;

        case Gta2Error::BriefingTooLongToWrap:
            sprintf(gTmpBuffer_67C598, "Briefing %s too long to wrap", gErrStr_67C29C);
            break;

        case Gta2Error::InvalidFraction:
        {
            Fix16 value = *va_arg(va, Fix16*);
            sprintf(gTmpBuffer_67C598, "Invalid fraction : %f", value.AsFloat());
            break;
        }

        case Gta2Error::InvalidAmbientLightValue:
        {
            Fix16 value = *va_arg(va, Fix16*);
            sprintf(gTmpBuffer_67C598, "Invalid ambient light value : %f", value.AsFloat());
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

            sprintf(gTmpBuffer_67C598,
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
EXPORT void FatalError_4A38C0(s32 Code, const char_type* pSourceFile, s32 lineNo, ...)
{
    if (Code == Gta2Error::FreeloaderEpisodeUnknown)
    {
        const char_type destroyed = bDestroyed_6F5B70;
        if (!destroyed)
        {
            Shutdown_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();
            // 16 was using esi instead of push 10h without the 0x6F5B70 bool
            // being cached in both branches of the if/else
            FatalError_4A07C0(Gta2Error::FreeloaderEpisodeUnknown, pSourceFile, lineNo);
        }
        DestroyWindow(gHwnd_707F04);
        exit(16);
    }
    else
    {
        const char_type destroyed = bDestroyed_6F5B70;
        if (!destroyed)
        {
            Shutdown_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();

            va_list va;
            va_start(va, lineNo);

            if (Code < 1000)
            {
                FatalError_4A07C0(Code, pSourceFile, lineNo);
            }
            else if (Code < 2000)
            {
                s32 arg1 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg1);
            }
            else if (Code < 3000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg2, arg2);
            }
            else if (Code < 4000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                s32 arg3 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg3, arg3, arg3);
            }
            else if (Code < 5000)
            {
                s32 arg1 = va_arg(va, s32);
                s32 arg2 = va_arg(va, s32);
                s32 arg3 = va_arg(va, s32);
                s32 arg4 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg1, arg2, arg3, arg4);
            }
            else if (Code < 6000)
            {
                s32 arg1 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg1);
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
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
                s32 arg1 = va_arg(va, s32);
                FatalError_4A07C0(Code, pSourceFile, lineNo, arg1);
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
// Called by diutil.cpp (InputRecorder_67F8B0.cpp) when a DirectInput call fails. The cases are in the original's
// body order. The empty DI_OK case is needed: without it VC6 splits the switch differently (compares instead of
// the byte-indexed table for 0x8007000E-0x80070057), which shifts the register rotation and so which cases
// cross-jump into the default's tail.
MATCH_FUNC(0x4A3CF0)
EXPORT void __stdcall FatalDXError_4A3CF0(HRESULT hr, const char_type* pSourceFile, s32 lineNo)
{
    switch (hr)
    {
        case 0x80004001: // E_NOTIMPL / DIERR_UNSUPPORTED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "the interface is not implemented");
            break;

        case 0x8000000A: // E_PENDING
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "pending");
            break;

        case 0x80004002: // E_NOINTERFACE / DIERR_NOINTERFACE
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "the interface is not supported");
            break;

        case 0x80004005: // E_FAIL / DIERR_GENERIC
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "undetermined failure");
            break;

        case 0x80040110: // CLASS_E_NOAGGREGATION
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "object does not support aggregation");
            break;

        case 0x80040202: // DIERR_MOREDATA
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "not all requested information fitted into the buffer");
            break;

        case 0x80040201: // DIERR_DEVICEFULL
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device is full");
            break;

        case 0x80040154: // REGDB_E_CLASSNOTREG
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "class is not registered");
            break;

        case 0x80040203: // DIERR_NOTDOWNLOADED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect is not downloaded");
            break;

        case 0x80040208: // DIERR_EFFECTPLAYING
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect is still playing");
            break;

        case 0x80040204: // DIERR_HASEFFECTS
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device still has effects attached");
            break;

        case 0x80040206: // DIERR_INCOMPLETEEFFECT
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect could not be downloaded because essential information is missing");
            break;

        case 0x80040207: // DIERR_NOTBUFFERED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device is not buffered");
            break;

        case 0x80040205: // DIERR_NOTEXCLUSIVEACQUIRED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device must be acquired in exclusive mode");
            break;

        case 0x80070002: // DIERR_OBJECTNOTFOUND
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "object does not exist");
            break;

        case 0x80070005: // E_ACCESSDENIED / DIERR_OTHERAPPHASPRIO
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "access denied");
            break;

        case 0x8007000C: // DIERR_NOTACQUIRED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device must be acquired");
            break;

        case 0x80070057: // E_INVALIDARG
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "invalid argument");
            break;

        case 0x8007000E: // E_OUTOFMEMORY
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "out of memory");
            break;

        case 0x8007001E: // DIERR_INPUTLOST
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "access to the input device has been lost");
            break;

        case 0x80070015: // DIERR_NOTINITIALIZED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "object has not been initialized");
            break;

        case 0x80070077: // DIERR_BADDRIVERVER
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "incompatible driver version or mismatched or incomplete driver components");
            break;

        case 0x80070481: // DIERR_BETADIRECTINPUTVERSION
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "application was written for an unsupported prerelease version of DirectInput");
            break;

        case 0x8007047E: // DIERR_OLDDIRECTINPUTVERSION
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "requires a newer version of DirectInput");
            break;

        case 0x800700AA: // DIERR_ACQUIRED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device is acquired");
            break;

        case 0x800704DF: // DIERR_ALREADYINITIALIZED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "object is already initialized");
            break;

        case 0: // DI_OK: returns without an error (the jump table entry goes to the ret)
            break;

        case 1: // S_FALSE / DI_NOEFFECT
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "the function had no effect");
            break;

        case 3: // DI_DOWNLOADSKIPPED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect could not be downloaded");
            break;

        case 4: // DI_EFFECTRESTARTED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect has been restarted");
            break;

        case 2: // DI_POLLEDDEVICE
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "device is a polled device");
            break;

        case 8: // DI_TRUNCATED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect parameters have been truncated");
            break;

        case 12: // DI_TRUNCATEDANDRESTARTED
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "effect parameters have been truncated and effect has been restarted");
            break;

        default:
            FatalError_4A38C0(Gta2Error::DirectXError, pSourceFile, lineNo, "unknown error");
            break;
    }
}
