#include "jolly_poitras_0x2BC0.hpp"
#include "error.hpp"
#include "file.hpp"
#include <io.h>

jolly_poitras_0x2BC0* gJolly_poitras_0x2BC0_6FEAC0;

wchar_t word_67DC8C[50];

jolly_poitras_0x2BC0::jolly_poitras_0x2BC0()
{
    // todo
    int* tmp = field_1878;
    int* pIter1 = this->field_1884;
    struc_221* pIter2 = this->field_1800;

    int i;
    for (i = 0; i < 3; i++)
    {
      //  ++pIter1;
        memset(pIter2, 0, sizeof(struc_221));
        *pIter1 = 0;
        pIter1++;

        *tmp = 0;
        tmp++;

//        *(pIter1 - 4) = 0;
  //      *(pIter1 - 1) = 0;
        ++pIter2;
    }

    for (i = 0; i < 8; i++)
    {
        if (PlySlotDatExists_56B940(i))
        {
            sub_56B990(i);
        }
        else
        {
            sub_56BA60(i);
        }
    }

    if (HiScoreHscExists_56BCA0())
    {
        sub_56BE50();
    }
    else
    {
        sub_56C1D0();
        sub_56BF20();
    }

    sub_56BD20();
}

jolly_poitras_0x2BC0::~jolly_poitras_0x2BC0()
{

}

// match
void jolly_poitras_0x2BC0::create_56C2C0()
{
    if (!gJolly_poitras_0x2BC0_6FEAC0)
    {
        gJolly_poitras_0x2BC0_6FEAC0 = new jolly_poitras_0x2BC0();
        if (!gJolly_poitras_0x2BC0_6FEAC0)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\plydat.cpp", 1269);
        }
    }
}

// match
void jolly_poitras_0x2BC0::destroy_56C340()
{
    if (gJolly_poitras_0x2BC0_6FEAC0)
    {
        delete gJolly_poitras_0x2BC0_6FEAC0;
        gJolly_poitras_0x2BC0_6FEAC0 = 0;
    }
}

void jolly_poitras_0x2BC0::sub_56BB10(angry_lewin_0x85C *a2)
{
    // todo
}

void jolly_poitras_0x2BC0::sub_56C010()
{
    // todo
}

// match
void jolly_poitras_0x2BC0::GetPlySlotDatName_56B8A0(unsigned __int16 a1, char *a2)
{
    char Buffer[8];
    _itoa(a1, Buffer, 10);
    strcpy(a2, "player\\plyslot");
    strcat(a2, Buffer);
    strcat(a2, ".dat");
}

// match
char jolly_poitras_0x2BC0::PlySlotDatExists_56B940(int a1)
{
    char FileName[356];
    GetPlySlotDatName_56B8A0(a1, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }
    _findclose(hFind);
    return 1;
}

// match
void jolly_poitras_0x2BC0::GetHiScoreHscFileName_56BCF0(char *pName)
{
    strcpy(pName, "player\\hiscores.hsc");
}

// match
char jolly_poitras_0x2BC0::HiScoreHscExists_56BCA0()
{
    char FileName[356];
    GetHiScoreHscFileName_56BCF0(FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }
    _findclose(hFind);
    return 1;
}

/*
 char FileName[356];

    unsigned __int16 idx = (unsigned __int16)slotIdx;

    wchar_t* pStr = field_26A0[idx].field_90_str;
    GetPlySlotDatName_56B8A0(idx, FileName);
    File::Global_Open_4A7060(FileName);



    int i = 9;
    do
    {
        int readLen = 2;
        File::Global_Read_4A71C0(pStr, &readLen);
        pStr++;
        i--;
    } while (i);


    int j;
    int k;
    for (k = 0; k < 3; k++)
    {
        for (j = 0; j < 4; j++)
        {
            int len_read = 1;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_0, &len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_4, &len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_8, &len_read);
        }
    }
    File::Global_Close_4A70C0();
*/

void jolly_poitras_0x2BC0::sub_56B990(int slotIdx)
{

    char FileName[356];
    unsigned __int16 idx = (unsigned __int16)slotIdx;
  
    wchar_t  * pStr = field_26A0[idx].field_90_str; // todo: This instruction is too early lea     esi, [ebx+2730h]
    GetPlySlotDatName_56B8A0(idx, FileName);
    File::Global_Open_4A7060(FileName);


    int i = 9;
    do
    {
        int readLen = 2;
        File::Global_Read_4A71C0(pStr, &readLen);
        pStr++;
        i--;
    } while (i);


    int j;
    int k;
    for (k = 0; k < 3; k++)
    {
        for (j = 0; j < 4; j++)
        {
            int len_read = 1;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_0, &len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_4, &len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0[idx].field_0[k][j].field_8, &len_read);
        }
    }
    File::Global_Close_4A70C0();
}

void jolly_poitras_0x2BC0::sub_56BA60(__int16 a2)
{
    // todo
}

/*
int len;
    int k3Counter;
    int k4Counter;

    char FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    len = 240;
    File::Global_Read_4A71C0(&field_23D0.field_0, &len);


    for (k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            len = 240;
            File::Global_Read_4A71C0(&field_1890[k3Counter][k4Counter].field_0, &len);
        }

        len = 40;
        File::Global_Read_4A71C0(&field_1800[k3Counter], &len); // 3 40 byte objs

        len = 4;
        File::Global_Read_4A71C0(&field_1878[k3Counter], &len);

        len = 4;
        File::Global_Read_4A71C0(&field_1884[k3Counter], &len);
    }

    File::Global_Close_4A70C0();
*/

// todo: wrong stack slots
void jolly_poitras_0x2BC0::sub_56BE50()
{
    char FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    int len = 240;
    File::Global_Read_4A71C0(&field_23D0.field_0, &len);

    for (int k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (int k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            len = 240;
            File::Global_Read_4A71C0(&field_1890[k3Counter][k4Counter].field_0, &len);
        }

        len = 40;
        File::Global_Read_4A71C0(&field_1800[k3Counter], &len); // 3 40 byte objs

        len = 4;
        File::Global_Read_4A71C0(&field_1878[k3Counter], &len);

        len = 4;
        File::Global_Read_4A71C0(&field_1884[k3Counter], &len);
    }

    File::Global_Close_4A70C0();
}

void jolly_poitras_0x2BC0::sub_56C1D0()
{
    // todo
}

void jolly_poitras_0x2BC0::sub_56BF20()
{
    // todo
}

// match
void jolly_poitras_0x2BC0::sub_56BD20()
{
    wcsncpy(this->field_24C0.field_0[0].field_0_str, L"ALAN", 9u);
    this->field_24C0.field_0[0].field_14_score = 1000000;
    wcsncpy(this->field_24C0.field_0[1].field_0_str, L"BRIAN", 9u);
    this->field_24C0.field_0[1].field_14_score = 500000;
    wcsncpy(this->field_24C0.field_0[2].field_0_str, L"COLIN", 9u);
    this->field_24C0.field_0[2].field_14_score = 400000;
    wcsncpy(this->field_24C0.field_0[3].field_0_str, L"DAVE", 9u);
    this->field_24C0.field_0[3].field_14_score = 300000;
    wcsncpy(this->field_24C0.field_0[4].field_0_str, L"ERIC", 9u);
    this->field_24C0.field_0[4].field_14_score = 250000;
    wcsncpy(this->field_24C0.field_0[5].field_0_str, L"FRANK", 9u);
    this->field_24C0.field_0[5].field_14_score = 200000;
    wcsncpy(this->field_24C0.field_0[6].field_0_str, L"GRAEME", 9u);
    this->field_24C0.field_0[6].field_14_score = 100000;
    wcsncpy(this->field_24C0.field_0[7].field_0_str, L"HECTOR", 9u);
    this->field_24C0.field_0[7].field_14_score = 50000;
    wcsncpy(this->field_24C0.field_0[8].field_0_str, L"IMOGEN", 9u);
    this->field_24C0.field_0[8].field_14_score = 25000;
    wcsncpy(this->field_24C0.field_0[9].field_0_str, L"JACKSON", 9u);
    this->field_24C0.field_0[9].field_14_score = 10000;
}

agitated_keldysh_0xF0::agitated_keldysh_0xF0()
{
    Init_56B520();
}

agitated_keldysh_0xF0::~agitated_keldysh_0xF0()
{

}

// match
void agitated_keldysh_0xF0::Init_56B520()
{
    for (int i=0; i<10; i++)
    {
        wcscpy(field_0[i].field_0_str, word_67DC8C);
        field_0[i].field_14_score = 0;
    }
}

char agitated_keldysh_0xF0::sub_56B550(const wchar_t *pFindStr, int findScore)
{
    unsigned __int16 startIdx = 10;
    for (__int16 i = 9; i != -1; --i)
    {
        if (findScore > field_0[i].field_14_score)
        {
            startIdx = i;
        }

        if (findScore == field_0[i].field_14_score &&
            wcscmp(pFindStr, field_0[i].field_0_str) == 0)
        {
            return 0;
        }
    }

    if (startIdx != 10)
    {

        if (startIdx < 9u)
        {
            // ??????
            small_string* pIter = &field_0[8];// .field_14_score;
          //  int* pIter = &field_0[9 - 1].field_14_score;

            int remainderCount = 9 - startIdx;
            do
            {
                wcsncpy((pIter+1)->field_0_str, (pIter)->field_0_str, 9u);
                (pIter+1)->field_14_score = (pIter)->field_14_score;

                pIter--;

                --remainderCount;
            } while (remainderCount);
            //newScore = findScore;
        }

        wcsncpy(field_0[startIdx].field_0_str, pFindStr, 9u);
        field_0[startIdx].field_14_score = findScore;

        return 1;
    }
    return 0;
}

dreamy_clarke_0xA4::dreamy_clarke_0xA4()
{
    sub_56B630();
}

dreamy_clarke_0xA4::~dreamy_clarke_0xA4()
{

}

void dreamy_clarke_0xA4::sub_56B630()
{
    // todo
}
