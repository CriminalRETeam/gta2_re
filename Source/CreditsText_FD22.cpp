#include "CreditsText_FD22.hpp"
#include "text_0x14.hpp"
#include "credits_line_category.hpp"

MATCH_FUNC(0x483E70)
CreditsText_FD22::CreditsText_FD22() // 483E70
{
    field_0_line_count = 0;
}

MATCH_FUNC(0x483EA0)
CreditsText_FD22::~CreditsText_FD22() // 483EA0
{
}

MATCH_FUNC(0x483EC0)
wchar_t CreditsText_FD22::ReadNextNonBlankChar_483EC0(const wchar_t* pStr, u16* pStartPos, bool bStopAtSpace)
{
    wchar_t cur_wchar = pStr[(*pStartPos)++];
    while ((cur_wchar == ' ' && bStopAtSpace) || cur_wchar == '\n' || cur_wchar == '\t')
    {
        cur_wchar = pStr[(*pStartPos)++];
    }
    return cur_wchar;
}

MATCH_FUNC(0x483F20)
void CreditsText_FD22::LoadCredits_483F20()
{
    s16 y_gap = 0;
    u16 read_pos = 0;
    wchar_t* pCreditsText = gText_0x14_704DFC->Find_5B5F90("credits");
    wchar_t line_char = CreditsText_FD22::ReadNextNonBlankChar_483EC0(pCreditsText, &read_pos, 1);
    u16 line_idx = 0;

    while (line_char != 0)
    {
        CreditsLine_6C* pLine = &this->field_2_lines[line_idx];
        if (line_char == '*')
        {
            y_gap += 20;
        }
        else
        {
            if (line_char == 35)
            {
                wchar_t tag_char = CreditsText_FD22::ReadNextNonBlankChar_483EC0(pCreditsText, &read_pos, 1);
                if (tag_char != 'W')
                {
                    if (tag_char == 'B')
                    {
                        pLine->field_6_string_category = credits_line_category::dev_names_2;
                    }
                    else if (tag_char == 'G')
                    {
                        pLine->field_6_string_category = credits_line_category::department_3;
                    }
                    else if (tag_char == 'C')
                    {
                        pLine->field_6_string_category = credits_line_category::game_name_4;
                    }
                    else
                    {
                        pLine->field_6_string_category = credits_line_category::unknown_1;
                    }
                }
                else
                {
                    pLine->field_6_string_category = credits_line_category::normal_0;
                }
            }
            else
            {
                read_pos--;
                pLine->field_6_string_category = credits_line_category::normal_0;
            }

            pLine->field_4_y_gap = y_gap;
            pLine->field_2_unused = 300;

            u16 i = 0;
            while (1)
            {
                wchar_t content_char = CreditsText_FD22::ReadNextNonBlankChar_483EC0(pCreditsText, &read_pos, 0);
                if (content_char == '*')
                {
                    break;
                }
                pLine->field_8_text[i++] = content_char;
            }
            read_pos--;
            pLine->field_8_text[i] = 0;
            pLine->field_0_bLoaded = true;
            ++field_0_line_count;
            y_gap = 0;
            line_idx++;
        }
        line_char = CreditsText_FD22::ReadNextNonBlankChar_483EC0(pCreditsText, &read_pos, 1);
        if (line_char == 0)
        {
            return;
        }
    }
}