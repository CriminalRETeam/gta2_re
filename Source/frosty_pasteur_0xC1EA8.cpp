#include "Function.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "file.hpp"
#include "error.hpp"
#include "debug.hpp"
#include "memory.hpp"
#include "map_0x370.hpp"

// nomatch
void frosty_pasteur_0xC1EA8::Load_512330(const char *pScrName)
{
    if (!bSkip_mission_67D4E5)
    {
        strcpy(field_35C_full_scr_file_name, pScrName);
        Error_SetName_4A0770(field_35C_full_scr_file_name);
        File::Global_Open_4A7060(field_35C_full_scr_file_name);
        unsigned int len = sizeof(field_46C_base_pointers);
        File::Global_Read_4A71C0(field_46C_base_pointers, &len);
        len = sizeof(field_334C_script_data);
        File::Global_Read_4A71C0(field_334C_script_data, &len);
        unsigned __int16 tableSize;
        len = sizeof(tableSize);
        File::Global_Read_4A71C0(&tableSize, &len);
        len = 5118;
        File::GetRemainderSize_4A7250(field_1334C_strings, &len);
        File::Global_Close_4A70C0();
        LoadStringTbl_5121E0(tableSize);
        GetScrFileName_5122D0();
        LoadSubScripts_5125F0();
    }
}


// nomatch
void frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0(unsigned __int16 tableSize)
{
    unsigned int total_str_length = 0;
    BYTE* pStringDataIter1 = (BYTE*)this->field_1334C_strings;
    while (total_str_length < tableSize)
    {
        int str_length = pStringDataIter1[8] + 9;
        total_str_length += str_length;
        pStringDataIter1 += str_length;
    }

    this->field_13350_pStringTbl = reinterpret_cast<str_table_normalized*>(Memory::malloc_4FE4D0(sizeof(str_table_normalized)));
    memset(field_13350_pStringTbl, 0, sizeof(str_table_normalized));

    str_table_entry *pStringDataIter2 = this->field_1334C_strings;
  
   // int offset; // ebp
    if (tableSize)
    {
        //offset = 4;
        unsigned int total_str_length_ = 0;
        unsigned __int16 str_count = 0;

        do
        {
            pStringDataIter2->field_2_zone_idx = gMap_0x370_6F6268->zone_idx_by_name_4DF050(
                (char *)&pStringDataIter2[1],
                strlen((const char *)&pStringDataIter2[1]));
            //offset += 4;
            field_13350_pStringTbl->field_4[str_count] = pStringDataIter2;
            int str_length_ = pStringDataIter2->field_8_length + 9;
            total_str_length_ += str_length_;
            pStringDataIter2 = (str_table_entry *)((char *)pStringDataIter2 + str_length_);
            ++str_count;
        } while (total_str_length_ < tableSize);

        field_13350_pStringTbl->field_0_string_count = str_count;
    }
    else
    {
        this->field_13350_pStringTbl->field_0_string_count = 0;
    }
}

MATCH_FUNC(0x5122D0)
void frosty_pasteur_0xC1EA8::GetScrFileName_5122D0()
{
    const char * pSlashPos = strrchr(field_35C_full_scr_file_name, '\\');
    if (pSlashPos)
    {
        pSlashPos++;

        memset(field_45C_scr_file_name, 0, sizeof(field_45C_scr_file_name));

        unsigned int lenAfterSlash = strlen(pSlashPos);
        while (lenAfterSlash)
        {
            if (pSlashPos[lenAfterSlash] == '.')
            {
                strncpy(field_45C_scr_file_name, pSlashPos, lenAfterSlash);
                return;
            }
            lenAfterSlash--;
        }
    }
}

// stub
void frosty_pasteur_0xC1EA8::LoadSubScripts_5125F0()
{

}

MATCH_FUNC(0x503080)
str_table_entry* frosty_pasteur_0xC1EA8::FindStringById_503080(__int16 stringId)
{
    unsigned short int idx = 0;
    str_table_entry *result = field_13350_pStringTbl->field_4[idx];
    while (result)
    {
        if (result->field_0_str_id == stringId)
        {
            return result;
        }
        idx++;
        result = field_13350_pStringTbl->field_4[idx];
    }
    return 0;
}
