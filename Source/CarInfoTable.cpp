#include "CarInfoTable.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Forward declarations: the functions below are in address order
EXPORT Fix16 __stdcall ComputeCarMassAndInertia_454410(Fix16 width, Fix16 height, Fix16 mass, Fix16 frontMassBias, Fix16* outCgHeight);
EXPORT Fix16 __stdcall ComputeThrustWithTurbo_5618F0(Fix16 half_thrust, Fix16 thrust_div5, s8 bTurbo);

EXTERN_GLOBAL(Fix16, kFpOne_6FE210);
EXTERN_GLOBAL(Fix16, kFpHalf_6FDFEC);
EXTERN_GLOBAL(Fix16, kFpFive_6FDEFC);

DEFINE_GLOBAL(CarInfoTable*, gCarInfo_808_678098, 0x678098);
DEFINE_GLOBAL_ARRAY(char, file_name_677EC4, 64, 0x677EC4);
DEFINE_GLOBAL(u32, processedGciData_676260, 0x676260);
DEFINE_GLOBAL(u32, gciDataLen_675F94, 0x675F94);
DEFINE_GLOBAL(u8*, input_data_676170, 0x676170);
DEFINE_GLOBAL(u32, gTokenizerState_67626C, 0x67626C);
DEFINE_GLOBAL(s32, line_number_676258, 0x676258);
DEFINE_GLOBAL(u32, processed_output_676250, 0x676250);
DEFINE_GLOBAL(s32, modelPhyArrLen_675F90, 0x675F90);
DEFINE_GLOBAL(u8*, modelPhyArrPtr_675F98, 0x675F98);
DEFINE_GLOBAL_INIT(Fix16, kOne_677F54, Fix16(1), 0x677F54);
DEFINE_GLOBAL_INIT(Fix16, kQuarter_677D78, Fix16(0x1000, 0), 0x677D78);
DEFINE_GLOBAL_INIT(Fix16, kOneTenth_677D74, Fix16(0x666, 0), 0x677D74);
DEFINE_GLOBAL_INIT(Fix16, kZero_6761A4, Fix16(0), 0x6761a4);
DEFINE_GLOBAL_ARRAY(char, Buffer_675FD4, 80, 0x675FD4);
DEFINE_GLOBAL_ARRAY(char, token_buffer_676024, 256, 0x676024);
DEFINE_GLOBAL_ARRAY_INIT(char*,
                         error_table_61A6D4,
                         13,
                         0x61A6D4,
                         "Invalid integer" COMMA
                         "Number too big" COMMA
                         "Write error" COMMA
                         "EOF in comment" COMMA
                         "Token too big" COMMA
                         "Input file not found" COMMA
                         "Can't open output file" COMMA
                         "Invalid long integer" COMMA
                         "Invalid hex integer" COMMA
                         "Invalid hex long integer" COMMA
                         "Invalid f32" COMMA
                         "Output too big" COMMA
                         "Unknown error");

MATCH_FUNC(0x430a30)
char* __stdcall CarInfoTable::parse_gci_file_430A30(void* pGciData,
                                                   size_t gciDataLen,
                                                   ModelPhysics* pModelPhyArr,
                                                   size_t modelPhyArrLen,
                                                   u32* next_position)
{
    input_data_676170 = static_cast<u8*>(pGciData);
    line_number_676258 = 1;
    processedGciData_676260 = 0;
    gciDataLen_675F94 = gciDataLen;
    modelPhyArrPtr_675F98 = reinterpret_cast<u8*>(pModelPhyArr);
    processed_output_676250 = 0;
    modelPhyArrLen_675F90 = modelPhyArrLen;

    s32 parse_result;
    s32 token_result = 0;
    while (token_result == 0)
    {
        token_result = ReadToken_430B10(token_buffer_676024);
        if (token_result < 0)
        {
            return SetErr_430AC0(token_result);
        }

        parse_result = ParseTokenAndPush_430C70(token_buffer_676024);
        if (parse_result < 0)
        {
            return SetErr_430AC0(parse_result);
        }
    }

    *next_position = processed_output_676250;
    return NULL;
}

MATCH_FUNC(0x430AC0)
char* __stdcall CarInfoTable::SetErr_430AC0(s32 error_code)
{
    if (error_code > 0 || error_code < -GTA2_COUNTOF_S(error_table_61A6D4) + 1)
    {
        // The original uses -13 despite having only 13 entries (indices 0..12).
        // The following DWORD is string data ("Unkn"), not another message pointer.
        error_code = -GTA2_COUNTOF_S(error_table_61A6D4);
    }

    sprintf(Buffer_675FD4, "%s at line %d", error_table_61A6D4[-error_code], line_number_676258);
    return Buffer_675FD4;
}

MATCH_FUNC(0x430b10)
s32 __stdcall CarInfoTable::ReadToken_430B10(char* pOut)
{
    s32 i = 0;

    *pOut = 0;
    while (255 > i)
    {
        u32 currentChar = 0;

        // Get the next character
        if (processedGciData_676260 == gciDataLen_675F94)
        {
            currentChar = -1;
        }
        else
        {
            currentChar = *input_data_676170;
            input_data_676170++;
            processedGciData_676260++;
            if (currentChar == '\n')
            {
                line_number_676258++;
            }
        }

        switch (gTokenizerState_67626C)
        {
            case 0:
                if (currentChar == -1)
                {
                    return 1;
                }
                if (currentChar == '{')
                {
                    gTokenizerState_67626C = 1;
                }
                else if (currentChar != ' ' && currentChar != '\t' && currentChar != '\n' && currentChar != ')' && currentChar != '(' &&
                         currentChar != '\r')
                {
                    gTokenizerState_67626C = 2;
                    pOut[i] = static_cast<char>(currentChar);
                    i++;
                }
                break;
            case 1:
                if (currentChar == -1)
                {
                    return -4;
                }
                if (currentChar == '}')
                {
                    gTokenizerState_67626C = 0;
                }
                break;
            case 2:
                if (currentChar == -1)
                {
                    pOut[i] = 0;
                    return 1;
                }
                if (currentChar == '{')
                {
                    pOut[i] = 0;
                    gTokenizerState_67626C = 1;
                    return 0;
                }
                if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == ')' || currentChar == '(' ||
                    currentChar == '\r')
                {
                    pOut[i] = 0;
                    gTokenizerState_67626C = 0;
                    return 0;
                }
                pOut[i] = static_cast<char>(currentChar);
                i++;

                break;
        }
    }
    return -5;
}

// https://decomp.me/scratch/YniJR
MATCH_FUNC(0x430C70)
s32 __stdcall ParseTokenAndPush_430C70(char_type* pStr)
{
    s32 type;
    s32 error_ret;
    s16 negated;
    s32 parsed_value;
    Fix16 fix16_num;
    s32 negated_value;
    s16 parsed_short;

    char_type* pCurr = pStr;
    BOOL bHex = false;
    BOOL bNegate = false;

    if (*pCurr)
    {
        if (*pCurr == '-')
        {
            bNegate = 1;
            pCurr++;
        }

        if (*pCurr)
        {
            if (*pCurr == 'b')
            {
                type = DataType::byte;
                ++pCurr;
            }
            else if (*pCurr == 'w')
            {
                type = DataType::word;
                ++pCurr;
            }
            else if (*pCurr == 'l')
            {
                type = DataType::dword;
                ++pCurr;
            }
            else if (*pCurr == 'f')
            {
                type = DataType::fix16_float;
                ++pCurr;
            }
            else
            {
                type = DataType::byte;
            }

            if (*pCurr == 'h')
            {
                bHex = 1;
                ++pCurr;
            }
            if (*pCurr == '-')
            {
                bNegate = !bNegate;
                ++pCurr;
            }

            switch (type)
            {
                case DataType::byte:
                    if (bHex == 1)
                    {
                        error_ret = CarInfoTable::HexStr2Int_430F30(pCurr, &parsed_short);
                    }
                    else
                    {
                        error_ret = CarInfoTable::StrToInt_431080(pCurr, &parsed_short);
                    }

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            negated = static_cast<char>(-static_cast<char>(parsed_short));
                            if (negated < -127)
                            {
                                return -2;
                            }
                            error_ret = CarInfoTable::PushData_430E60(&negated, sizeof(BYTE));
                        }
                        else
                        {
                            if (static_cast<u16>(parsed_short) > 0xFFu)
                            {
                                return -2;
                            }
                            error_ret = CarInfoTable::PushData_430E60(&parsed_short, sizeof(BYTE));
                        }
                        // Shares the word case's tail; VC6 doesn't merge identical case tails by itself
                        goto check_push_result;
                    }
                    return error_ret;

                case DataType::word:
                    if (bHex == 1)
                    {
                        error_ret = CarInfoTable::HexStr2Int_430F30(pCurr, &parsed_short);
                    }
                    else
                    {
                        error_ret = CarInfoTable::StrToInt_431080(pCurr, &parsed_short);
                    }

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            negated = -parsed_short;
                            error_ret = CarInfoTable::PushData_430E60(&negated, sizeof(WORD));
                        }
                        else
                        {
                            error_ret = CarInfoTable::PushData_430E60(&parsed_short, sizeof(WORD));
                        }

                    check_push_result:
                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;

                case DataType::dword:
                    if (bHex == 1)
                    {
                        error_ret = CarInfoTable::HexStr2Int_430EC0(pCurr, &parsed_value);
                    }
                    else
                    {
                        error_ret = CarInfoTable::StrToInt_430FA0(pCurr, &parsed_value);
                    }

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            negated_value = -static_cast<s16>(parsed_value);
                            error_ret = CarInfoTable::PushData_430E60(&negated_value, sizeof(DWORD));
                        }
                        else
                        {
                            error_ret = CarInfoTable::PushData_430E60(&parsed_value, sizeof(DWORD));
                        }

                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;

                case DataType::fix16_float:
                    error_ret = CarInfoTable::FloatStrToFix16_431000(static_cast<char*>(pCurr), fix16_num);

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            fix16_num = -fix16_num;
                        }

                        error_ret = CarInfoTable::PushData_430E60(&fix16_num, sizeof(Fix16));
                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;

                default:
                    return 0;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x430e60)
s32 __stdcall CarInfoTable::PushData_430E60(void* pSrc, u32 size)
{
    processed_output_676250 += size;
    if (processed_output_676250 > static_cast<u32>(modelPhyArrLen_675F90))
    {
        return -12;
    }

    memcpy(modelPhyArrPtr_675F98, pSrc, size);

    modelPhyArrPtr_675F98 = modelPhyArrPtr_675F98 + size;
    return 0;
}

MATCH_FUNC(0x430EC0)
s32 __stdcall CarInfoTable::HexStr2Int_430EC0(const char* pStr, s32* pOut)
{
    *pOut = 0;
    s32 place_value = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if ((currentChar < '0' || '9' < currentChar) && (currentChar < 'A' || 'F' < currentChar))
        {
            return 0xfffffff6;
        }

        s32 digit_value;
        if (currentChar < '0' || '9' < currentChar)
        {
            digit_value = (currentChar - 0x37) * place_value;
        }
        else
        {
            digit_value = (currentChar - 0x30) * place_value;
        }

        *pOut += digit_value;
        place_value = place_value << 4;
    }

    return 0;
}

MATCH_FUNC(0x430f30)
s32 __stdcall CarInfoTable::HexStr2Int_430F30(const char* pStr, s16* pOut)
{
    *pOut = 0;
    s32 place_value = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if ((currentChar < '0' || '9' < currentChar) && (currentChar < 'A' || 'F' < currentChar))
        {
            return 0xfffffff7;
        }

        s32 digit_value;
        if (currentChar < '0' || '9' < currentChar)
        {
            digit_value = (currentChar - 0x37) * place_value;
        }
        else
        {
            digit_value = (currentChar - 0x30) * place_value;
        }

        *pOut += digit_value;
        place_value = place_value << 4;
    }

    return 0;
}

MATCH_FUNC(0x430fa0)
s32 __stdcall CarInfoTable::StrToInt_430FA0(const char* pStr, s32* pOut)
{
    *pOut = 0;
    s32 place_value = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            return 0xfffffff8;
        }

        s32 digit_value = (currentChar - 0x30) * place_value;
        *pOut += digit_value;
        place_value = place_value * 10;
    }

    return 0;
}

MATCH_FUNC(0x431000)
s32 __stdcall CarInfoTable::FloatStrToFix16_431000(char* pStr, Fix16& pOut)
{
    pOut = kZero_6761A4;
    bool bPastDecimalPoint = false;
    s32 decimal_divisor = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        char currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            if (currentChar != '.' || bPastDecimalPoint)
            {
                return -11;
            }
            pOut.mValue /= decimal_divisor;
            decimal_divisor = 1;
            bPastDecimalPoint = true;
        }
        else
        {
            pOut.mValue += (currentChar - 0x30) * decimal_divisor * 0x4000;
            decimal_divisor = decimal_divisor * 10;
        }
    }
    return 0;
}

MATCH_FUNC(0x431080)
s32 __stdcall CarInfoTable::StrToInt_431080(const char* pStr, s16* pOut)
{
    *pOut = 0;
    s32 place_value = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            return 0xffffffff;
        }

        s32 digit_value = (currentChar - 0x30) * place_value;
        *pOut += digit_value;
        place_value = place_value * 10;
    }

    return 0;
}

MATCH_FUNC(0x440860)
Fix16 PixelsToFix16Table::SignedPixelsToFix16_440860(s8 pixels)
{
    if (pixels < 0)
    {
        return -list[-pixels];
    }
    return list[pixels];
}

MATCH_FUNC(0x4542A0)
void CarInfo_2C::CalculateCarInfo_4542A0(s32 idx)
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(idx);
    ModelPhysics* pModelPhysics = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(idx);
    Fix16 gear2_speed = pModelPhysics->field_40_gear2_speed;
    Fix16 gear3_speed = pModelPhysics->field_44_gear3_speed;
    Fix16 max_speed;
    if (gear2_speed > gear3_speed || (max_speed = pModelPhysics->field_28_max_speed, gear3_speed > max_speed) || gear2_speed > max_speed)
    {
        FatalError_4A38C0(0x3F6, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 91, static_cast<u8>(idx));
    }

    s8 front_wheel_offset = pCarInfo->front_wheel_offset;
    Fix16 new_front_wheel_offset;
    if (front_wheel_offset < 0)
    {
        new_front_wheel_offset = -gPixelsToFix16_6F6850.list[-front_wheel_offset];
    }
    else
    {
        new_front_wheel_offset = gPixelsToFix16_6F6850.list[front_wheel_offset];
    }
    this->field_4_front_wheel_offset = new_front_wheel_offset;
    s8 rear_wheel_offset = pCarInfo->rear_wheel_offset;
    Fix16 new_rear_wheel_offset;
    if (rear_wheel_offset < 0)
    {
        new_rear_wheel_offset = -gPixelsToFix16_6F6850.list[-rear_wheel_offset];
    }
    else
    {
        new_rear_wheel_offset = gPixelsToFix16_6F6850.list[rear_wheel_offset];
    }
    this->field_8_rear_wheel_offset = new_rear_wheel_offset;
    Fix16 outY;
    this->field_0_moment_of_inertia = ComputeCarMassAndInertia_454410(gPixelsToFix16_6F6850.list[pCarInfo->w],
                                                                      gPixelsToFix16_6F6850.list[pCarInfo->h],
                                                                      pModelPhysics->field_4_mass,
                                                                      pModelPhysics->field_C_front_mass_bias,
                                                                      &outY);
    this->field_C_center_of_mass_offset.x = 0;
    this->field_C_center_of_mass_offset.y = outY; // out val of func above
    this->field_14_half_thrust = pModelPhysics->field_24_thrust / 2;
    this->field_18_fith_thrust = pModelPhysics->field_24_thrust / 5;
    this->field_1C_max_thrust_with_turbo =
        ComputeThrustWithTurbo_5618F0(field_14_half_thrust, field_18_fith_thrust, pModelPhysics->field_1_turbo);
    this->field_20_rear_drive_bias = kOne_677F54 - pModelPhysics->field_8_front_drive_bias;
    this->field_24_skid_threshhold_1 = (pModelPhysics->field_30_sked_threshold * (kOne_677F54 - kOneTenth_677D74));
    this->field_28_skid_threshhold_2 = (pModelPhysics->field_30_sked_threshold * (kOne_677F54 + kOneTenth_677D74));
}

MATCH_FUNC(0x454410)
EXPORT Fix16 __stdcall ComputeCarMassAndInertia_454410(Fix16 width, Fix16 height, Fix16 mass, Fix16 frontMassBias, Fix16* outCgHeight)
{
    // The const refs (bound to the product temporaries) give the original's stack slots and the
    // __allmul operand order of the two inertia products (value locals swap them)
    const Fix16& inertiaBase = ((((height * height) * kQuarter_677D78) + (width * width)) / 12);
    const Fix16& heightXConstant = (height * kQuarter_677D78);
    const Fix16& frontMass = (mass * frontMassBias);
    Fix16 frontI = (frontMass * inertiaBase);
    // Negated by hand: one inline expansion less, so both front (cg - h) subtractions stay inline (budget)
    Fix16 nh;
    nh.mValue = -height.mValue;
    Fix16 negHeightXConstant = (nh * kQuarter_677D78);

    const Fix16& rearMass = (mass * (kOne_677F54 - frontMassBias));
    Fix16 rearI = (rearMass * inertiaBase);

    *outCgHeight = (((heightXConstant * frontMass) + (negHeightXConstant * rearMass)) / mass);

    Fix16 frontTotal;
    frontTotal = frontI + frontMass * (*outCgHeight - heightXConstant) * (*outCgHeight - heightXConstant);
    Fix16 rearTotal;
    rearTotal = rearI + rearMass * (*outCgHeight - negHeightXConstant) * (*outCgHeight - negHeightXConstant);
    return frontTotal + rearTotal;
}

MATCH_FUNC(0x454680)
void ModelPhysics::ConvertMass_454680()
{
    field_4_mass = field_4_mass * (kOneTenth_677D74 + kOne_677F54);
}

MATCH_FUNC(0x4546b0)
ModelPhysics* CarInfoTable::GetModelPhysicsFromIdx_4546B0(u8 model_idx)
{
    return field_404_model_physics_array[model_idx];
}

MATCH_FUNC(0x4546d0)
void CarInfoTable::LoadModelPhysics_4546D0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    u32 data_size;
    size_t file_size;
    void* file_content = File::ReadFileToBuffer_4A6C80(file_name_677EC4, &file_size);

    field_804_raw_data = new ModelPhysics[number_of_cars];

    char* pErrorMsg = CarInfoTable::parse_gci_file_430A30(file_content,
                                                         file_size,
                                                         field_804_raw_data,
                                                         number_of_cars * sizeof(ModelPhysics),
                                                         &data_size);

    crt::free(file_content);

    if (pErrorMsg != NULL)
    {
        strcpy(gErrStr_67C29C, file_name_677EC4);
        strcpy(gErrStr2_67C3A8, pErrorMsg);

        FatalError_4A38C0(Gta2Error::BinmakeError, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbc, gErrStr_67C29C, gErrStr2_67C3A8);
    }

    if (data_size % sizeof(ModelPhysics))
    {
        FatalError_4A38C0(Gta2Error::InvalidSize, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbd, file_name_677EC4, data_size);
    }

    if (data_size != number_of_cars * sizeof(ModelPhysics))
    {
        FatalError_4A38C0(Gta2Error::ModelCountMismatchInStyleFile,
                          "C:\\Splitting\\Gta2\\Source\\carinfo.cpp",
                          0xbe,
                          data_size / sizeof(ModelPhysics),
                          file_name_677EC4,
                          number_of_cars);
    }

    for (u32 i = 0; i < number_of_cars; i++)
    {
        ModelPhysics* pPhysics = &field_804_raw_data[i];
        field_404_model_physics_array[pPhysics->field_0_model] = pPhysics;
    }
}

MATCH_FUNC(0x454840)
CarInfo_2C* CarInfoTable::GetInfoAtIdx_454840(u8 idx)
{
    return field_0_ptr_array[idx];
}

MATCH_FUNC(0x454850)
void CarInfoTable::CalculateAllCarInfo_454850()
{
    const u32 count = gGtx_0x106C_703DD4->get_number_of_cars();
    field_400_raw_data = new CarInfo_2C[count];

    for (u32 i = 0, j = 0; i < 256; i++)
    {
        if (gGtx_0x106C_703DD4->does_car_exist(i))
        {
            field_0_ptr_array[i] = &field_400_raw_data[j];
            field_400_raw_data[j].CalculateCarInfo_4542A0(i);
            j++;
        }
    }
}

MATCH_FUNC(0x4549A0)
CarInfo_2C::CarInfo_2C()
{
}

MATCH_FUNC(0x4549B0)
CarInfo_2C::~CarInfo_2C()
{
}

MATCH_FUNC(0x4549c0)
void CarInfoTable::ConvertAllMass_4549C0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    for (u32 i = 0; i < number_of_cars; i++)
    {
        field_804_raw_data[i].ConvertMass_454680();
    }
}

MATCH_FUNC(0x454a00)
void CarInfoTable::LoadFromGciFile_454A00(const char_type* pGciFilePath)
{
    strcpy(&file_name_677EC4[0], pGciFilePath);

    LoadModelPhysics_4546D0();
    ConvertAllMass_4549C0();
    CalculateAllCarInfo_454850();
}

MATCH_FUNC(0x454a50)
void CarInfoTable::Reload_454A50()
{
    Free_454AA0();
    Clear_454A80();
    LoadModelPhysics_4546D0();
    ConvertAllMass_4549C0();
    CalculateAllCarInfo_454850();
}

MATCH_FUNC(0x454a80)
void CarInfoTable::Clear_454A80()
{
    for (int i = 0; i < 0x100; i++)
    {
        field_404_model_physics_array[i] = NULL;
        field_0_ptr_array[i] = NULL;
    }
}

MATCH_FUNC(0x454aa0)
void CarInfoTable::Free_454AA0()
{
    delete[] field_400_raw_data;
    field_400_raw_data = NULL;

    if (field_804_raw_data)
    {
        delete[] field_804_raw_data;
    }
    field_804_raw_data = NULL;
}

MATCH_FUNC(0x454b00)
CarInfoTable::CarInfoTable()
{
    field_400_raw_data = NULL;
    field_804_raw_data = NULL;
    Clear_454A80();
}

MATCH_FUNC(0x454b20)
CarInfoTable::~CarInfoTable()
{
    Free_454AA0();
}

MATCH_FUNC(0x5618F0)
EXPORT Fix16 __stdcall ComputeThrustWithTurbo_5618F0(Fix16 half_thrust, Fix16 thrust_div5, s8 bTurbo)
{
    Fix16 thrust;
    if (bTurbo)
    {
        thrust = (kFpOne_6FE210 + kFpHalf_6FDFEC) * ((thrust_div5 * kFpFive_6FDEFC));
    }
    else
    {
        thrust = thrust_div5 * kFpFive_6FDEFC;
    }
    thrust = thrust + half_thrust;
    return thrust;
}
