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

STUB_FUNC(0x454680)
void Monster_48::sub_454680()
{
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

STUB_FUNC(0x454850)
void Monster_808::sub_454850()
{
    const u32 count = gGtx_0x106C_703DD4->field_5C_cari->field_400_count;
    field_400_raw_data = new Monster_2C[count];

    for (s32 i = 0; i < 256; i++)
    {
        if (gGtx_0x106C_703DD4->field_5C_cari->field_0[i])
        {
            field_0_ptr_array[i] = &field_400_raw_data[i];
            field_400_raw_data[i].sub_4542A0(i);
        }
    }
}

STUB_FUNC(0x4549c0)
void Monster_808::sub_4549C0()
{
    // stupid meme function - doing some strange backwards looping
    car_info_container* container = gGtx_0x106C_703DD4->field_5C_cari;
    Monster_48* pIter = field_804_raw_data;
    for (s32 i = 0; i < container->field_400_count; i++)
    {
        pIter->sub_454680();
        pIter++;
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
