#include "CarInfo_808.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "crt_stubs.hpp"
#include "enums.hpp"

DEFINE_GLOBAL(CarInfo_808*, gCarInfo_808_678098, 0x678098);
UnknownList dword_6F6850; //DEFINE_GLOBAL(UnknownList, dword_6F6850, 0x6F6850);
DEFINE_GLOBAL_ARRAY(char, file_name_677EC4, 64, 0x677EC4);
DEFINE_GLOBAL(u32, processedGciData_676260, 0x676260);
DEFINE_GLOBAL(u32, gciDataLen_675F94, 0x675F94);
DEFINE_GLOBAL(u8*, input_data_676170, 0x676170);
DEFINE_GLOBAL(u32, found_open_brackets_67626C, 0x67626C);
DEFINE_GLOBAL(s32, line_number_676258, 0x676258);
DEFINE_GLOBAL(u32, processed_output_676250, 0x676250);
DEFINE_GLOBAL(s32, modelPhyArrLen_675F90, 0x675F90);
DEFINE_GLOBAL(u8*, modelPhyArrPtr_675F98, 0x675F98);
DEFINE_GLOBAL(Fix16, dword_677F54, 0x677F54);
DEFINE_GLOBAL(Fix16, dword_677D74, 0x677D74);
DEFINE_GLOBAL(Fix16, DAT_6761A4, 0x6761a4);
DEFINE_GLOBAL_ARRAY(char, Buffer_675FD4, 80, 0x675FD4);
DEFINE_GLOBAL_ARRAY(char, byte_676024, 256, 0x676024);
DEFINE_GLOBAL_ARRAY_INIT(char*, 
                         error_table_61A6D4, 
                         13, 
                         0x61A6D4, 
                         "Invalid integer",
                         "Number too big",
                         "Write error",
                         "EOF in comment",
                         "Token too big",
                         "Input file not found",
                         "Can't open output file",
                         "Invalid long integer",
                         "Invalid hex integer",
                         "Invalid hex long integer",
                         "Invalid f32",
                         "Output too big",
                         "Unknown error");

MATCH_FUNC(0x454680)
void ModelPhysics_48::sub_454680()
{
    field_4_mass = field_4_mass * (dword_677D74 + dword_677F54);
}

MATCH_FUNC(0x430b10)
s32 __stdcall CarInfo_808::sub_430b10(char* pOut)
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

        switch (found_open_brackets_67626C)
        {
            case 0:
                if (currentChar == -1)
                {
                    return 1;
                }
                if (currentChar == '{')
                {
                    found_open_brackets_67626C = 1;
                }
                else if (currentChar != ' ' && currentChar != '\t' && currentChar != '\n' && currentChar != ')' && currentChar != '(' && currentChar != '\r')
                {
                    found_open_brackets_67626C = 2;
                    pOut[i] = (char)currentChar;
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
                    found_open_brackets_67626C = 0;
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
                    found_open_brackets_67626C = 1;
                    return 0;
                }
                if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == ')' || currentChar == '(' || currentChar == '\r')
                {
                    pOut[i] = 0;
                    found_open_brackets_67626C = 0;
                    return 0;
                }
                pOut[i] = (char)currentChar;
                i++;

                break;
        }
    }
    return -5;
}

// https://decomp.me/scratch/YniJR
STUB_FUNC(0x430C70)
s32 __stdcall sub_430C70(char_type* pStr)
{
    NOT_IMPLEMENTED;
    s32 type;
    s32 error_ret;
    s32 v9;
    Fix16 fix16_num;
    s32 v11;
    s16 v8s;
    s16 tmp;

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
                {

                    if (bHex == 1)
                    {
                        error_ret = CarInfo_808::HexStr2Int_430F30(pCurr, (s16*)&tmp);
                    }
                    else
                    {
                        error_ret = CarInfo_808::StrToInt_431080(pCurr, (s16*)&tmp);
                    }

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            s16 v8s = (char)(-(char)tmp);
                            if ((s16)v8s < -127)
                            {
                                return -2;
                            }
                            error_ret = CarInfo_808::sub_430E60(&v8s, sizeof(BYTE));
                        }
                        else
                        {
                            if ((s16)tmp > 0xFFu)
                            {
                                return -2;
                            }
                            error_ret = CarInfo_808::sub_430E60(&tmp, sizeof(BYTE));
                        }

                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;
                }

                case DataType::word:

                {
                    if (bHex == 1)
                    {
                        error_ret = CarInfo_808::HexStr2Int_430F30(pCurr, &tmp);
                    }
                    else
                    {
                        error_ret = CarInfo_808::StrToInt_431080(pCurr, &tmp);
                    }
                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            tmp = -tmp;
                        }
                        error_ret = CarInfo_808::sub_430E60(&tmp, sizeof(WORD));

                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;
                }

                case DataType::dword:
                    if (bHex == 1)
                    {
                        error_ret = CarInfo_808::HexStr2Int_430EC0(pCurr, (s32*)&v9);
                    }
                    else
                    {
                        error_ret = CarInfo_808::StrToInt_430FA0(pCurr, &v9);
                    }

                    if (error_ret == 0)
                    {
                        if (bNegate)
                        {
                            v11 = -(s16)v9;
                            error_ret = CarInfo_808::sub_430E60(&v11, sizeof(DWORD));
                        }
                        else
                        {
                            error_ret = CarInfo_808::sub_430E60(&v9, sizeof(DWORD));
                        }
                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;

                case DataType::fix16_float:
                    error_ret = CarInfo_808::FloatStrToFix16_431000((char*)pCurr, fix16_num);

                    if (!error_ret)
                    {
                        if (bNegate)
                        {
                            fix16_num = -fix16_num;
                        }
                        error_ret = CarInfo_808::sub_430E60(&fix16_num, sizeof(Fix16));
                        if (error_ret >= 0)
                        {
                            return 0;
                        }
                    }
                    return error_ret;

                default:
                    return 0;

            } // end switch

        } // end if (*pCurr)
    }
    return 0;
}

MATCH_FUNC(0x430a30)
char* __stdcall CarInfo_808::parse_gci_file_430A30(void* pGciData,
                                                   size_t gciDataLen,
                                                   ModelPhysics_48* pModelPhyArr,
                                                   size_t modelPhyArrLen,
                                                   u32* next_position)
{
    input_data_676170 = (u8*)pGciData;
    line_number_676258 = 1;
    processedGciData_676260 = 0;
    gciDataLen_675F94 = gciDataLen;
    modelPhyArrPtr_675F98 = (u8*)pModelPhyArr;
    processed_output_676250 = 0;
    modelPhyArrLen_675F90 = modelPhyArrLen;

    s32 v6;
    s32 v5 = 0;
    while (v5 == 0)
    {
        v5 = sub_430b10(byte_676024);
        if (v5 < 0)
        {
            return SetErr_430AC0(v5);
        }

        v6 = sub_430C70(byte_676024);
        if (v6 < 0)
        {
            return SetErr_430AC0(v6);
        }
    }

    *next_position = processed_output_676250;
    return NULL;
}

MATCH_FUNC(0x430AC0)
char* __stdcall CarInfo_808::SetErr_430AC0(s32 a1)
{
    if (a1 > 0 || a1 < -GTA2_COUNTOF_S(error_table_61A6D4) - 1)
    {
        a1 = -GTA2_COUNTOF(error_table_61A6D4);
    }

    sprintf(Buffer_675FD4, "%s at line %d", error_table_61A6D4[-a1], line_number_676258);
    return Buffer_675FD4;
}

MATCH_FUNC(0x430e60)
s32 __stdcall CarInfo_808::sub_430E60(void* pSrc, u32 size)
{
    processed_output_676250 += size;
    if (processed_output_676250 > modelPhyArrLen_675F90)
    {
        return -12;
    }

    memcpy(modelPhyArrPtr_675F98, pSrc, size);

    modelPhyArrPtr_675F98 = modelPhyArrPtr_675F98 + size;
    return 0;
}

MATCH_FUNC(0x430EC0)
s32 __stdcall CarInfo_808::HexStr2Int_430EC0(const char* pStr, s32* pOut)
{
    *pOut = 0;
    s32 iVar5 = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if ((currentChar < '0' || '9' < currentChar) && (currentChar < 'A' || 'F' < currentChar))
        {
            return 0xfffffff6;
        }

        s32 sVar2;
        if (currentChar < '0' || '9' < currentChar)
        {
            sVar2 = (currentChar - 0x37) * iVar5;
        }
        else
        {
            sVar2 = (currentChar - 0x30) * iVar5;
        }

        *pOut += sVar2;
        iVar5 = iVar5 << 4;
    }

    return 0;
}

MATCH_FUNC(0x430f30)
s32 __stdcall CarInfo_808::HexStr2Int_430F30(const char* pStr, s16* pOut)
{
    *pOut = 0;
    s32 iVar5 = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if ((currentChar < '0' || '9' < currentChar) && (currentChar < 'A' || 'F' < currentChar))
        {
            return 0xfffffff7;
        }

        s32 sVar2;
        if (currentChar < '0' || '9' < currentChar)
        {
            sVar2 = (currentChar - 0x37) * iVar5;
        }
        else
        {
            sVar2 = (currentChar - 0x30) * iVar5;
        }

        *pOut += sVar2;
        iVar5 = iVar5 << 4;
    }

    return 0;
}

MATCH_FUNC(0x430fa0)
s32 __stdcall CarInfo_808::StrToInt_430FA0(const char* pStr, s32* pOut)
{
    *pOut = 0;
    s32 iVar5 = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            return 0xfffffff8;
        }

        s32 sVar2 = (currentChar - 0x30) * iVar5;
        *pOut += sVar2;
        iVar5 = iVar5 * 10;
    }

    return 0;
}

MATCH_FUNC(0x431000)
s32 __stdcall CarInfo_808::FloatStrToFix16_431000(char* pStr, Fix16& pOut)
{
    pOut = DAT_6761A4;
    bool bVar2 = false;
    s32 iVar6 = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        char currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            if (currentChar != '.' || bVar2)
            {
                return -11;
            }
            pOut.mValue /= iVar6;
            iVar6 = 1;
            bVar2 = true;
        }
        else
        {
            pOut.mValue += (currentChar - 0x30) * iVar6 * 0x4000;
            iVar6 = iVar6 * 10;
        }
    }
    return 0;
}

MATCH_FUNC(0x431080)
s32 __stdcall CarInfo_808::StrToInt_431080(const char* pStr, s16* pOut)
{
    *pOut = 0;
    s32 iVar5 = 1;
    s32 strLen = strlen(pStr);

    while (--strLen >= 0)
    {
        s8 currentChar = pStr[strLen];
        if (currentChar < '0' || '9' < currentChar)
        {
            return 0xffffffff;
        }

        s32 sVar2 = (currentChar - 0x30) * iVar5;
        *pOut += sVar2;
        iVar5 = iVar5 * 10;
    }

    return 0;
}

MATCH_FUNC(0x4549A0)
CarInfo_2C::CarInfo_2C()
{
}

MATCH_FUNC(0x4549B0)
CarInfo_2C::~CarInfo_2C()
{
}

STUB_FUNC(0x4542A0)
void CarInfo_2C::sub_4542A0(s32 idx)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4546b0)
ModelPhysics_48* CarInfo_808::GetModelPhysicsFromIdx_4546B0(u8 model_idx)
{
    return field_404_model_physics_array[model_idx];
}

MATCH_FUNC(0x4546d0)
void CarInfo_808::sub_4546D0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    u32 local_1c;
    size_t file_size;
    void* file_content = File::ReadFileToBuffer_4A6C80(file_name_677EC4, &file_size);

    field_804_raw_data = new ModelPhysics_48[number_of_cars];

    char* pErrorMsg = CarInfo_808::parse_gci_file_430A30(file_content,
                                                      file_size,
                                                      field_804_raw_data,
                                                      number_of_cars * sizeof(ModelPhysics_48),
                                                      &local_1c);

    crt::free(file_content);

    if (pErrorMsg != NULL)
    {
        strcpy(gErrStr_67C29C, file_name_677EC4);
        strcpy(byte_67C3A8, pErrorMsg);

        FatalError_4A38C0(Gta2Error::BinmakeError, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbc, gErrStr_67C29C, byte_67C3A8);
    }

    if (local_1c % sizeof(ModelPhysics_48))
    {
        FatalError_4A38C0(Gta2Error::InvalidSize, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbd, file_name_677EC4, local_1c);
    }

    if (local_1c != number_of_cars * sizeof(ModelPhysics_48))
    {
        FatalError_4A38C0(Gta2Error::ModelCountMismatchInStyleFile,
                          "C:\\Splitting\\Gta2\\Source\\carinfo.cpp",
                          0xbe,
                          local_1c / sizeof(ModelPhysics_48),
                          file_name_677EC4,
                          number_of_cars);
    }

    for (u32 i = 0; i < number_of_cars; i++)
    {
        ModelPhysics_48* pMVar5 = &field_804_raw_data[i];
        field_404_model_physics_array[pMVar5->field_0_model] = pMVar5;
    }
}

MATCH_FUNC(0x454840)
CarInfo_2C* CarInfo_808::sub_454840(u8 idx)
{
    return field_0_ptr_array[idx];
}

// This function has a full match, but it's waiting until sub_4542A0 has matched.
// Or moved to a different file. While it's empty, this match will fail because of a single intruction
STUB_FUNC(0x454850)
void CarInfo_808::sub_454850()
{
    NOT_IMPLEMENTED;
    const u32 count = gGtx_0x106C_703DD4->get_number_of_cars();
    field_400_raw_data = new CarInfo_2C[count];

    for (u32 i = 0, j = 0; i < 256; i++)
    {
        if (gGtx_0x106C_703DD4->does_car_exist(i))
        {
            field_0_ptr_array[i] = &field_400_raw_data[j];
            field_400_raw_data[j].sub_4542A0(i);
            j++;
        }
    }
}

MATCH_FUNC(0x4549c0)
void CarInfo_808::sub_4549C0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    for (u32 i = 0; i < number_of_cars; i++)
    {
        field_804_raw_data[i].sub_454680();
    }
}

MATCH_FUNC(0x454a00)
void CarInfo_808::sub_454A00(const char_type* pGciFilePath)
{
    strcpy(&file_name_677EC4[0], pGciFilePath);

    sub_4546D0();
    sub_4549C0();
    sub_454850();
}

MATCH_FUNC(0x454a50)
void CarInfo_808::sub_454A50()
{
    sub_454AA0();
    sub_454A80();
    sub_4546D0();
    sub_4549C0();
    sub_454850();
}

MATCH_FUNC(0x454a80)
void CarInfo_808::sub_454A80()
{
    for (int i = 0; i < 0x100; i++)
    {
        field_404_model_physics_array[i] = NULL;
        field_0_ptr_array[i] = NULL;
    }
}

MATCH_FUNC(0x454aa0)
void CarInfo_808::sub_454AA0()
{
    delete[] field_400_raw_data;
    field_400_raw_data = 0;

    if (field_804_raw_data)
    {
        delete[] field_804_raw_data;
    }
    field_804_raw_data = 0;
}

MATCH_FUNC(0x454b00)
CarInfo_808::CarInfo_808()
{
    field_400_raw_data = NULL;
    field_804_raw_data = NULL;
    sub_454A80();
}

MATCH_FUNC(0x454b20)
CarInfo_808::~CarInfo_808()
{
    sub_454AA0();
}