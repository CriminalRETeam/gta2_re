#include "Monster_808.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

EXPORT_VAR char file_name_677EC4[64];
GLOBAL(file_name_677EC4, 0x677EC4);

STUB_FUNC(0x430a30)
char* __stdcall Monster_808::parse_gci_file_430A30(void* input,
                                                   size_t input_size,
                                                   Monster_48* output,
                                                   size_t output_size,
                                                   u32* next_position)
{
    return NULL;
}

STUB_FUNC(0x4546b0)
Monster_48* Monster_808::sub_4546B0(u8 a2)
{
    return 0;
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

STUB_FUNC(0x454840)
Monster_2C* Monster_808::sub_454840(u8 idx)
{
    return 0;
}

STUB_FUNC(0x454850)
s32 Monster_808::sub_454850()
{
    return 0;
}

STUB_FUNC(0x4549c0)
void Monster_808::sub_4549C0()
{
}

MATCH_FUNC(0x454a00)
void Monster_808::sub_454A00(const char_type* pGciFilePath)
{
    strcpy(&file_name_677EC4[0], pGciFilePath);

    sub_4546D0();
    sub_4549C0();
    sub_454850();
}

STUB_FUNC(0x454a50)
s32 Monster_808::sub_454A50()
{
    return 0;
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

STUB_FUNC(0x454aa0)
void Monster_808::sub_454AA0()
{
}

MATCH_FUNC(0x454b00)
Monster_808::Monster_808()
{
    field_400_raw_data = NULL;
    field_804_raw_data = NULL;
    sub_454A80();
}

STUB_FUNC(0x454b20)
Monster_808::~Monster_808()
{
}
