#include "xenodochial_morse.hpp"
#include "text_0x14.hpp"

MATCH_FUNC(0x483E70)
xenodochial_morse::xenodochial_morse() // 483E70
{
    field_0 = 0;
}

MATCH_FUNC(0x483EA0)
xenodochial_morse::~xenodochial_morse() // 483EA0
{
}

MATCH_FUNC(0x483EC0)
wchar_t xenodochial_morse::sub_483EC0(const wchar_t* pStr, u16* pStartPos, bool bStopAtSpace)
{
    wchar_t cur_wchar = pStr[(*pStartPos)++];
    while ((cur_wchar == ' ' && bStopAtSpace) || cur_wchar == '\n' || cur_wchar == '\t')
    {
        cur_wchar = pStr[(*pStartPos)++];
    }
    return cur_wchar;
}

MATCH_FUNC(0x483F20)
void xenodochial_morse::sub_483F20()
{
    s16 v2 = 0;
    u16 a4 = 0;
    wchar_t* _5B5F90 = gText_0x14_704DFC->Find_5B5F90("credits");
    wchar_t v4 = xenodochial_morse::sub_483EC0(_5B5F90, &a4, 1);
    u16 v12 = 0;

    while (v4 != 0)
    {
        sleepy_stonebraker_0x6C* v5 = &this->field_2[v12];
        if (v4 == '*')
        {
            v2 += 20;
        }
        else
        {
            if (v4 == 35)
            {
                wchar_t v6 = xenodochial_morse::sub_483EC0(_5B5F90, &a4, 1);
                if (v6 != 'W')
                {
                    if (v6 == 'B')
                    {
                        v5->field_6 = 2;
                    }
                    else if (v6 == 'G')
                    {
                        v5->field_6 = 3;
                    }
                    else if (v6 == 'C')
                    {
                        v5->field_6 = 4;
                    }
                    else
                    {
                        v5->field_6 = 1;
                    }
                }
                else
                {
                    v5->field_6 = 0;
                }
            }
            else
            {
                a4--;
                v5->field_6 = 0;
            }

            v5->field_4 = v2;
            v5->field_2 = 300;

            u16 i = 0;
            while (1)
            {
                wchar_t v8 = xenodochial_morse::sub_483EC0(_5B5F90, &a4, 0);
                if (v8 == '*')
                {
                    break;
                }
                v5->field_8_strBuf[i++] = v8;
            }
            a4--;
            v5->field_8_strBuf[i] = 0;
            v5->field_0 = 1;
            ++field_0;
            v2 = 0;
            v12++;
        }
        v4 = xenodochial_morse::sub_483EC0(_5B5F90, &a4, 1);
        if (v4 == 0)
        {
            return;
        }
    }
}
