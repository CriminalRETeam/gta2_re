#include "Monster_808.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

EXPORT_VAR Monster_808* gMonster_808_678098;
GLOBAL(gMonster_808_678098, 0x678098);

EXPORT_VAR char file_name_677EC4[64];
GLOBAL(file_name_677EC4, 0x677EC4);

EXPORT_VAR u32 processed_input_676260;
GLOBAL(processed_input_676260, 0x676260);

EXPORT_VAR u32 input_size_675F94;
GLOBAL(input_size_675F94, 0x675F94);

EXPORT_VAR u8* input_data_676170;
GLOBAL(input_data_676170, 0x676170);

EXPORT_VAR u32 found_open_brackets_67626C;
GLOBAL(found_open_brackets_67626C, 0x67626C);

EXPORT_VAR s32 line_number_676258;
GLOBAL(line_number_676258, 0x676258);

EXPORT_VAR u32 processed_output_676250;
GLOBAL(processed_output_676250, 0x676250);

EXPORT_VAR s32 output_size_675F90;
GLOBAL(output_size_675F90, 0x675F90);

EXPORT_VAR u8* output_ptr_675F98;
GLOBAL(output_ptr_675F98, 0x675F98);

EXPORT_VAR Fix16 DAT_5e5514;
GLOBAL(DAT_5e5514, 0x5e5514);

EXPORT_VAR Fix16 DAT_5e54a0;
GLOBAL(DAT_5e54a0, 0x5e54a0);

EXPORT_VAR Fix16 DAT_6761A4;
GLOBAL(DAT_6761A4, 0x6761a4);

MATCH_FUNC(0x454680)
void Monster_48::sub_454680()
{
    field_4_mass = field_4_mass * (DAT_5e54a0 + DAT_5e5514);
}

MATCH_FUNC(0x430b10)
s32 __stdcall Monster_808::sub_430b10(char* param_1)
{
    s32 iVar2 = 0;

    *param_1 = 0;
    while (255 > iVar2)
    {
        u32 uVar1 = 0;

        // Get the next character
        if (processed_input_676260 == input_size_675F94)
        {
            uVar1 = -1;
        }
        else
        {
            uVar1 = *input_data_676170;
            input_data_676170++;
            processed_input_676260++;
            if (uVar1 == '\n')
            {
                line_number_676258++;
            }
        }

        switch (found_open_brackets_67626C)
        {
            case 0:
                if (uVar1 == -1)
                {
                    return 1;
                }
                if (uVar1 == '{')
                {
                    found_open_brackets_67626C = 1;
                }
                else if (uVar1 != ' ' && uVar1 != '\t' && uVar1 != '\n' && uVar1 != ')' && uVar1 != '(' && uVar1 != '\r')
                {
                    found_open_brackets_67626C = 2;
                    param_1[iVar2] = (char)uVar1;
                    iVar2++;
                }
                break;
            case 1:
                if (uVar1 == -1)
                {
                    return -4;
                }
                if (uVar1 == '}')
                {
                    found_open_brackets_67626C = 0;
                }
                break;
            case 2:
                if (uVar1 == -1)
                {
                    param_1[iVar2] = 0;
                    return 1;
                }
                if (uVar1 == '{')
                {
                    param_1[iVar2] = 0;
                    found_open_brackets_67626C = 1;
                    return 0;
                }
                if (uVar1 == ' ' || uVar1 == '\t' || uVar1 == '\n' || uVar1 == ')' || uVar1 == '(' || uVar1 == '\r')
                {
                    param_1[iVar2] = 0;
                    found_open_brackets_67626C = 0;
                    return 0;
                }
                param_1[iVar2] = (char)uVar1;
                iVar2++;

                break;
        }
    }
    return -5;
}

STUB_FUNC(0x430a30)
char* __stdcall Monster_808::parse_gci_file_430A30(void* input,
                                                   size_t input_size,
                                                   Monster_48* output,
                                                   size_t output_size,
                                                   u32* next_position)
{
    return NULL;
}

MATCH_FUNC(0x430e60)
s32 __stdcall Monster_808::sub_430E60(void* param_1, u32 param_2)
{
    processed_output_676250 += param_2;
    if (processed_output_676250 > output_size_675F90)
    {
        return -12;
    }

    memcpy(output_ptr_675F98, param_1, param_2);

    output_ptr_675F98 = output_ptr_675F98 + param_2;
    return 0;
}

MATCH_FUNC(0x430EC0)
s32 __stdcall Monster_808::HexStr2Int_430EC0(const char* param_1, s32* param_2)
{
    *param_2 = 0;
    s32 iVar5 = 1;
    s32 iVar4 = strlen(param_1);

    while (--iVar4 >= 0)
    {
        s8 cVar1 = param_1[iVar4];
        if ((cVar1 < '0' || '9' < cVar1) && (cVar1 < 'A' || 'F' < cVar1))
        {
            return 0xfffffff6;
        }

        s32 sVar2;
        if (cVar1 < '0' || '9' < cVar1)
        {
            sVar2 = (cVar1 - 0x37) * iVar5;
        }
        else
        {
            sVar2 = (cVar1 - 0x30) * iVar5;
        }

        *param_2 += sVar2;
        iVar5 = iVar5 << 4;
    }

    return 0;
}

MATCH_FUNC(0x430f30)
s32 __stdcall Monster_808::HexStr2Int_430F30(const char* param_1, s16* param_2)
{
    *param_2 = 0;
    s32 iVar5 = 1;
    s32 iVar4 = strlen(param_1);

    while (--iVar4 >= 0)
    {
        s8 cVar1 = param_1[iVar4];
        if ((cVar1 < '0' || '9' < cVar1) && (cVar1 < 'A' || 'F' < cVar1))
        {
            return 0xfffffff7;
        }

        s32 sVar2;
        if (cVar1 < '0' || '9' < cVar1)
        {
            sVar2 = (cVar1 - 0x37) * iVar5;
        }
        else
        {
            sVar2 = (cVar1 - 0x30) * iVar5;
        }

        *param_2 += sVar2;
        iVar5 = iVar5 << 4;
    }

    return 0;
}

MATCH_FUNC(0x430fa0)
s32 __stdcall Monster_808::StrToInt_430FA0(const char* param_1, s32* param_2)
{
    *param_2 = 0;
    s32 iVar5 = 1;
    s32 iVar4 = strlen(param_1);

    while (--iVar4 >= 0)
    {
        s8 cVar1 = param_1[iVar4];
        if (cVar1 < '0' || '9' < cVar1)
        {
            return 0xfffffff8;
        }

        s32 sVar2 = (cVar1 - 0x30) * iVar5;
        *param_2 += sVar2;
        iVar5 = iVar5 * 10;
    }

    return 0;
}

MATCH_FUNC(0x431000)
s32 __stdcall Monster_808::FloatStrToFix16_431000(char* param_1, Fix16 &param_2)
{
    param_2 = DAT_6761A4;
    bool bVar2 = false;
    s32 iVar6 = 1;
    s32 iVar5 = strlen(param_1);

    while (--iVar5 >= 0)
    {
        char cVar1 = param_1[iVar5];
        if (cVar1 < '0' || '9' < cVar1)
        {
            if (cVar1 != '.' || bVar2)
            {
                return -11;
            }
            param_2.mValue /= iVar6;
            iVar6 = 1;
            bVar2 = true;
        }
        else
        {
            param_2.mValue += (cVar1 - 0x30) * iVar6 * 0x4000;
            iVar6 = iVar6 * 10;
        }
    }
    return 0;
}

MATCH_FUNC(0x431080)
s32 __stdcall Monster_808::StrToInt_431080(const char* param_1, s16* param_2)
{
    *param_2 = 0;
    s32 iVar5 = 1;
    s32 iVar4 = strlen(param_1);

    while (--iVar4 >= 0)
    {
        s8 cVar1 = param_1[iVar4];
        if (cVar1 < '0' || '9' < cVar1)
        {
            return 0xffffffff;
        }

        s32 sVar2 = (cVar1 - 0x30) * iVar5;
        *param_2 += sVar2;
        iVar5 = iVar5 * 10;
    }

    return 0;
}

MATCH_FUNC(0x4549A0)
Monster_2C::Monster_2C()
{
}

MATCH_FUNC(0x4549B0)
Monster_2C::~Monster_2C()
{
}

STUB_FUNC(0x4542A0)
void Monster_2C::sub_4542A0(s32 idx)
{

}

MATCH_FUNC(0x4546b0)
Monster_48* Monster_808::sub_4546B0(u8 a2)
{
    return field_404_ptr_array[a2];
}

MATCH_FUNC(0x4546d0)
void Monster_808::sub_4546D0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    u32 local_1c;
    size_t file_size;
    void* file_content = File::ReadFileToBuffer_4A6C80(file_name_677EC4, &file_size);

    field_804_raw_data = new Monster_48[number_of_cars];

    char* pcVar3 = Monster_808::parse_gci_file_430A30(file_content, file_size, field_804_raw_data, number_of_cars * 0x48, &local_1c);
    free(file_content);

    if (pcVar3 != NULL)
    {
        strcpy(gErrStr_67C29C, file_name_677EC4);
        strcpy(byte_67C3A8, pcVar3);

        FatalError_4A38C0(0x7de, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbc, gErrStr_67C29C, byte_67C3A8);
    }

    if (local_1c % 0x48)
    {
        FatalError_4A38C0(0x7e2, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbd, file_name_677EC4, local_1c);
    }

    if (local_1c != number_of_cars * 0x48)
    {
        FatalError_4A38C0(0xbc5, "C:\\Splitting\\Gta2\\Source\\carinfo.cpp", 0xbe, local_1c / 0x48, file_name_677EC4, number_of_cars);
    }

    for (u32 i = 0; i < number_of_cars; i++)
    {
        Monster_48* pMVar5 = field_804_raw_data + i;
        field_404_ptr_array[pMVar5->field_0_model] = pMVar5;
    }
}

MATCH_FUNC(0x454840)
Monster_2C* Monster_808::sub_454840(u8 idx)
{
    return field_0_ptr_array[idx];
}

// This function has a full match, but it's waiting until sub_4542A0 has matched.
// Or moved to a different file. While it's empty, this match will fail because of a single intruction
STUB_FUNC(0x454850)
void Monster_808::sub_454850()
{
    const u32 count = gGtx_0x106C_703DD4->get_number_of_cars();
    field_400_raw_data = new Monster_2C[count];

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
void Monster_808::sub_4549C0()
{
    u32 number_of_cars = gGtx_0x106C_703DD4->get_number_of_cars();
    for (u32 i = 0; i < number_of_cars; i++)
    {
        field_804_raw_data[i].sub_454680();
    }
}

MATCH_FUNC(0x454a00)
void Monster_808::sub_454A00(const char_type* pGciFilePath)
{
    strcpy(&file_name_677EC4[0], pGciFilePath);

    sub_4546D0();
    sub_4549C0();
    sub_454850();
}

MATCH_FUNC(0x454a50)
void Monster_808::sub_454A50()
{
    sub_454AA0();
    sub_454A80();
    sub_4546D0();
    sub_4549C0();
    sub_454850();
}

MATCH_FUNC(0x454a80)
void Monster_808::sub_454A80()
{
    for (int i = 0; i < 0x100; i++)
    {
        field_404_ptr_array[i] = NULL;
        field_0_ptr_array[i] = NULL;
    }
}

MATCH_FUNC(0x454aa0)
void Monster_808::sub_454AA0()
{
    delete[] field_400_raw_data;
    field_400_raw_data = 0;

    if (field_804_raw_data)
    {
        delete field_804_raw_data;
    }
    field_804_raw_data = 0;
}

MATCH_FUNC(0x454b00)
Monster_808::Monster_808()
{
    field_400_raw_data = NULL;
    field_804_raw_data = NULL;
    sub_454A80();
}

MATCH_FUNC(0x454b20)
Monster_808::~Monster_808()
{
    sub_454AA0();
}
