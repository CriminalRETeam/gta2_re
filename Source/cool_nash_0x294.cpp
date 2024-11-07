#include "cool_nash_0x294.hpp"
#include "angry_lewin_0x85C.hpp"

// TODO: Move and fully stub
class Car_BC
{
public:
    void sub_43A600();

    char sub_43B360();
};

STUB_FUNC(0x43A600)
void Car_BC::sub_43A600()
{
}

STUB_FUNC(0x43B360)
char Car_BC::sub_43B360()
{
    return 0;
}

// =======

STUB_FUNC(0x45ae70)
cool_nash_0x294::cool_nash_0x294()
{
}

STUB_FUNC(0x45af00)
cool_nash_0x294::~cool_nash_0x294()
{
}

STUB_FUNC(0x45afc0)
char_type cool_nash_0x294::sub_45AFC0()
{
    return 0;
}

STUB_FUNC(0x45b440)
s32 cool_nash_0x294::sub_45B440()
{
    return 0;
}

STUB_FUNC(0x45b4e0)
char_type cool_nash_0x294::sub_45B4E0()
{
    return 0;
}

STUB_FUNC(0x45b520)
u32 *cool_nash_0x294::sub_45B520(u32 *a2)
{
    return 0;
}

MATCH_FUNC(0x45b550)
void cool_nash_0x294::sub_45B550()
{
    field_26A = 2;
}

MATCH_FUNC(0x45b560)
void cool_nash_0x294::sub_45B560(angry_lewin_0x85C *a2, char_type a3)
{
    field_15C_player_weapons = a2;
    field_200 = a2->field_2E_idx + 1;
    if (a3)
    {
        field_200 += 6;
    }
}

MATCH_FUNC(0x45b590)
bool cool_nash_0x294::sub_45B590()
{
    // TODO: was probably a switch case rather than checking a "between" on occupation?
    if (field_240_occupation >= 23 && (field_240_occupation <= 27 || field_240_occupation == 37))
    {
        return true;
    }
    return false;
}

STUB_FUNC(0x45b5b0)
s32 cool_nash_0x294::sub_45B5B0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45bbf0)
Car_BC *cool_nash_0x294::sub_45BBF0()
{
    return 0;
}

STUB_FUNC(0x45bc10)
s32 cool_nash_0x294::sub_45BC10(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x45bc70)
void cool_nash_0x294::sub_45BC70()
{
}

STUB_FUNC(0x45bd20)
char_type cool_nash_0x294::sub_45BD20(Car_BC *a2)
{
    return 0;
}

STUB_FUNC(0x45be30)
s32 cool_nash_0x294::sub_45BE30()
{
    return 0;
}

STUB_FUNC(0x45be70)
void cool_nash_0x294::sub_45BE70()
{
}

STUB_FUNC(0x45be90)
void cool_nash_0x294::sub_45BE90()
{
}

STUB_FUNC(0x45bec0)
void cool_nash_0x294::sub_45BEC0()
{
}

STUB_FUNC(0x45bfb0)
void cool_nash_0x294::sub_45BFB0(cool_nash_0x294 *a1, s32 a2)
{
}

MATCH_FUNC(0x45bfd0)
void cool_nash_0x294::sub_45BFD0()
{
    field_21C |= 0x2000000u;
    sub_45C010();
}

STUB_FUNC(0x45bfe0)
u32 cool_nash_0x294::sub_45BFE0()
{
    return 0;
}

STUB_FUNC(0x45c010)
void cool_nash_0x294::sub_45C010()
{
}

MATCH_FUNC(0x45c040)
void cool_nash_0x294::sub_45C040()
{
    field_208_invulnerability = 9999;
    sub_45C070();
}

STUB_FUNC(0x45c050)
void cool_nash_0x294::sub_45C050()
{
}

STUB_FUNC(0x45c070)
void cool_nash_0x294::sub_45C070()
{
}

MATCH_FUNC(0x45c090)
void cool_nash_0x294::sub_45C090()
{
    Car_BC *pBc = this->field_16C_car;
    if (pBc)
    {
        pBc->sub_43A600();
    }
    else if (field_278 != 9)
    {
        field_216_health = 100;
    }
}

STUB_FUNC(0x45c0c0)
void cool_nash_0x294::sub_45C0C0(gmp_map_zone *a2)
{
}

STUB_FUNC(0x45c310)
s32 cool_nash_0x294::sub_45C310()
{
    return 0;
}

STUB_FUNC(0x45c350)
void cool_nash_0x294::sub_45C350(gmp_map_zone *a2)
{
}

STUB_FUNC(0x45c410)
s32 cool_nash_0x294::sub_45C410()
{
    return 0;
}

STUB_FUNC(0x45c4b0)
s32 cool_nash_0x294::sub_45C4B0()
{
    return 0;
}

STUB_FUNC(0x45c500)
s32 cool_nash_0x294::sub_45C500(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45c540)
s32 cool_nash_0x294::sub_45C540(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x45c5a0)
void cool_nash_0x294::sub_45C5A0()
{
    field_278 = field_280;
    field_27C = field_284;
}

STUB_FUNC(0x45c5c0)
void cool_nash_0x294::sub_45C5C0()
{
}

STUB_FUNC(0x45c730)
s32 cool_nash_0x294::sub_45C730(Car_BC *a2)
{
    return 0;
}

MATCH_FUNC(0x45c7a0)
void cool_nash_0x294::sub_45C7A0(Car_BC *a2)
{
    sub_463830(0, 9999);
    sub_463570(35, 9999);
    field_248_enter_car_as_passenger = 1;
    field_150_target_objective_car = a2;
    field_24C_target_car_door = a2->sub_43B360() - 1;
}

STUB_FUNC(0x45c7f0)
Char_8 *cool_nash_0x294::sub_45C7F0(Car_BC *a2)
{
    return 0;
}

STUB_FUNC(0x45c830)
char_type cool_nash_0x294::sub_45C830(s32 xpos, s32 ypos, s32 zpos)
{
    return 0;
}

STUB_FUNC(0x45c900)
s16 *cool_nash_0x294::sub_45C900(s16 *a2)
{
    return 0;
}

STUB_FUNC(0x45c920)
s32 *cool_nash_0x294::sub_45C920(s32 *a2)
{
    return 0;
}

STUB_FUNC(0x45c960)
s16 *cool_nash_0x294::sub_45C960(s16 *a2)
{
    return 0;
}

STUB_FUNC(0x45c9b0)
s32 *cool_nash_0x294::sub_45C9B0(s32 *a2)
{
    return 0;
}

STUB_FUNC(0x45c9d0)
s16 *cool_nash_0x294::sub_45C9D0(s16 *a2)
{
    return 0;
}

STUB_FUNC(0x45caa0)
void cool_nash_0x294::sub_45CAA0()
{
}

MATCH_FUNC(0x45ce50)
void cool_nash_0x294::sub_45CE50(s16 damage)
{
    if (field_208_invulnerability <= 0)
    {
        field_216_health -= damage;
        if (field_216_health <= 0)
        {
            if (field_278 != 8)
            {
                field_216_health = 0;
                sub_46F9D0();
            }
            else
            {
                if (field_216_health < 0)
                {
                    field_216_health = 0;
                }
            }
        }
    }
}

STUB_FUNC(0x45cf20)
void cool_nash_0x294::sub_45CF20(s32 a2)
{
}

STUB_FUNC(0x45d000)
char_type cool_nash_0x294::sub_45D000(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45dd30)
char_type cool_nash_0x294::sub_45DD30(s32 a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x45de80)
char_type cool_nash_0x294::sub_45DE80(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45e080)
void cool_nash_0x294::sub_45E080()
{
}

STUB_FUNC(0x45e4a0)
void cool_nash_0x294::sub_45E4A0()
{
}

STUB_FUNC(0x45ea00)
void cool_nash_0x294::sub_45EA00()
{
}

STUB_FUNC(0x45eb60)
void cool_nash_0x294::sub_45EB60()
{
}

STUB_FUNC(0x45edc0)
char_type cool_nash_0x294::sub_45EDC0()
{
    return 0;
}

MATCH_FUNC(0x45ede0)
bool cool_nash_0x294::sub_45EDE0(s32 a2)
{
    return field_238 == a2 ? true : false;
}

STUB_FUNC(0x45ee00)
char_type cool_nash_0x294::sub_45EE00(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45ee70)
gmp_map_zone *cool_nash_0x294::sub_45EE70()
{
    return 0;
}

STUB_FUNC(0x45f360)
void cool_nash_0x294::sub_45F360()
{
}

STUB_FUNC(0x45ff60)
void cool_nash_0x294::sub_45FF60()
{
}

STUB_FUNC(0x460820)
void cool_nash_0x294::sub_460820()
{
}

STUB_FUNC(0x461290)
void cool_nash_0x294::sub_461290()
{
}

STUB_FUNC(0x461530)
char_type cool_nash_0x294::sub_461530()
{
    return 0;
}

STUB_FUNC(0x461630)
void cool_nash_0x294::sub_461630()
{
}

STUB_FUNC(0x4619f0)
char_type cool_nash_0x294::sub_4619F0()
{
    return 0;
}

STUB_FUNC(0x461a60)
s16 cool_nash_0x294::sub_461A60()
{
    return 0;
}

STUB_FUNC(0x461f20)
void cool_nash_0x294::sub_461F20()
{
}

STUB_FUNC(0x462280)
s16 cool_nash_0x294::sub_462280()
{
    return 0;
}

STUB_FUNC(0x4624a0)
s32 cool_nash_0x294::sub_4624A0()
{
    return 0;
}

STUB_FUNC(0x462510)
s32 cool_nash_0x294::sub_462510()
{
    return 0;
}

STUB_FUNC(0x462550)
s32 cool_nash_0x294::sub_462550()
{
    return 0;
}

STUB_FUNC(0x462590)
void cool_nash_0x294::sub_462590()
{
}

STUB_FUNC(0x462620)
s32 cool_nash_0x294::sub_462620()
{
    return 0;
}

STUB_FUNC(0x4626b0)
char_type cool_nash_0x294::sub_4626B0()
{
    return 0;
}

STUB_FUNC(0x462b80)
void cool_nash_0x294::sub_462B80()
{
}

STUB_FUNC(0x462e70)
char_type cool_nash_0x294::sub_462E70()
{
    return 0;
}

STUB_FUNC(0x4632e0)
void cool_nash_0x294::sub_4632E0()
{
}

STUB_FUNC(0x463300)
s32 cool_nash_0x294::sub_463300(u8 a1)
{
    return 0;
}

STUB_FUNC(0x4633e0)
s32 cool_nash_0x294::sub_4633E0(char_type a2)
{
    return 0;
}

STUB_FUNC(0x463570)
char_type cool_nash_0x294::sub_463570(s32 a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x463830)
void cool_nash_0x294::sub_463830(s32 a2, s16 a3)
{
}

STUB_FUNC(0x463aa0)
void cool_nash_0x294::sub_463AA0()
{
}

STUB_FUNC(0x463fb0)
void cool_nash_0x294::sub_463FB0()
{
}

STUB_FUNC(0x4645b0)
s16 *cool_nash_0x294::sub_4645B0()
{
    return 0;
}

STUB_FUNC(0x465270)
void cool_nash_0x294::sub_465270()
{
}

STUB_FUNC(0x465b20)
char_type cool_nash_0x294::sub_465B20()
{
    return 0;
}

STUB_FUNC(0x465cd0)
bool cool_nash_0x294::sub_465CD0()
{
    return 0;
}

STUB_FUNC(0x465d00)
char_type cool_nash_0x294::sub_465D00(cool_nash_0x294 *a2)
{
    return 0;
}

STUB_FUNC(0x466b90)
cool_nash_0x294 *cool_nash_0x294::sub_466B90(cool_nash_0x294 *a2)
{
    return 0;
}

STUB_FUNC(0x466bb0)
cool_nash_0x294 *cool_nash_0x294::sub_466BB0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x466bd0)
cool_nash_0x294 *cool_nash_0x294::sub_466BD0(cool_nash_0x294 *a2)
{
    return 0;
}

STUB_FUNC(0x466bf0)
cool_nash_0x294 *cool_nash_0x294::sub_466BF0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x466f40)
s32 cool_nash_0x294::sub_466F40(u8 a2)
{
    return 0;
}

STUB_FUNC(0x466f60)
s32 cool_nash_0x294::sub_466F60(u8 a2)
{
    return 0;
}

STUB_FUNC(0x466fb0)
s32 cool_nash_0x294::sub_466FB0()
{
    return 0;
}

STUB_FUNC(0x467090)
char_type cool_nash_0x294::sub_467090()
{
    return 0;
}

STUB_FUNC(0x467280)
s32 cool_nash_0x294::sub_467280()
{
    return 0;
}

STUB_FUNC(0x4672e0)
char_type cool_nash_0x294::sub_4672E0(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x4678e0)
char_type cool_nash_0x294::sub_4678E0()
{
    return 0;
}

STUB_FUNC(0x467960)
char_type cool_nash_0x294::sub_467960()
{
    return 0;
}

STUB_FUNC(0x467a20)
char_type cool_nash_0x294::sub_467A20()
{
    return 0;
}

STUB_FUNC(0x467ad0)
void cool_nash_0x294::sub_467AD0()
{
}

STUB_FUNC(0x467bd0)
void cool_nash_0x294::sub_467BD0()
{
}

STUB_FUNC(0x467ca0)
void cool_nash_0x294::sub_467CA0()
{
}

STUB_FUNC(0x467e20)
void cool_nash_0x294::sub_467E20()
{
}

STUB_FUNC(0x467fb0)
char_type cool_nash_0x294::sub_467FB0()
{
    return 0;
}

STUB_FUNC(0x467fd0)
char_type cool_nash_0x294::sub_467FD0()
{
    return 0;
}

STUB_FUNC(0x468040)
s16 cool_nash_0x294::sub_468040()
{
    return 0;
}

STUB_FUNC(0x4682a0)
void cool_nash_0x294::sub_4682A0()
{
}

STUB_FUNC(0x468310)
void cool_nash_0x294::sub_468310()
{
}

STUB_FUNC(0x4686c0)
void cool_nash_0x294::sub_4686C0()
{
}

STUB_FUNC(0x468820)
void cool_nash_0x294::sub_468820()
{
}

STUB_FUNC(0x468930)
void cool_nash_0x294::sub_468930()
{
}

STUB_FUNC(0x468a00)
void cool_nash_0x294::sub_468A00()
{
}

STUB_FUNC(0x468bd0)
void cool_nash_0x294::sub_468BD0()
{
}

STUB_FUNC(0x468c70)
char_type cool_nash_0x294::sub_468C70()
{
    return 0;
}

STUB_FUNC(0x468de0)
char_type cool_nash_0x294::sub_468DE0()
{
    return 0;
}

STUB_FUNC(0x468e80)
char_type cool_nash_0x294::sub_468E80()
{
    return 0;
}

STUB_FUNC(0x469010)
s32 cool_nash_0x294::sub_469010()
{
    return 0;
}

STUB_FUNC(0x469030)
char_type cool_nash_0x294::sub_469030()
{
    return 0;
}

STUB_FUNC(0x469060)
void cool_nash_0x294::sub_469060()
{
}

STUB_FUNC(0x469bd0)
void cool_nash_0x294::sub_469BD0()
{
}

STUB_FUNC(0x469bf0)
char_type cool_nash_0x294::sub_469BF0()
{
    return 0;
}

STUB_FUNC(0x469d60)
s32 cool_nash_0x294::sub_469D60()
{
    return 0;
}

STUB_FUNC(0x469e10)
void cool_nash_0x294::sub_469E10()
{
}

STUB_FUNC(0x469e30)
s32 cool_nash_0x294::sub_469E30()
{
    return 0;
}

STUB_FUNC(0x469e50)
s32 cool_nash_0x294::sub_469E50()
{
    return 0;
}

STUB_FUNC(0x469f30)
s32 cool_nash_0x294::sub_469F30()
{
    return 0;
}

STUB_FUNC(0x469fc0)
s32 cool_nash_0x294::sub_469FC0()
{
    return 0;
}

STUB_FUNC(0x469fe0)
void cool_nash_0x294::sub_469FE0()
{
}

STUB_FUNC(0x46a1f0)
void cool_nash_0x294::sub_46A1F0()
{
}

STUB_FUNC(0x46a290)
s32 cool_nash_0x294::sub_46A290()
{
    return 0;
}

STUB_FUNC(0x46a350)
char_type cool_nash_0x294::sub_46A350()
{
    return 0;
}

STUB_FUNC(0x46a530)
s32 cool_nash_0x294::sub_46A530()
{
    return 0;
}

STUB_FUNC(0x46a5e0)
void cool_nash_0x294::sub_46A5E0()
{
}

STUB_FUNC(0x46a6d0)
s16 cool_nash_0x294::sub_46A6D0()
{
    return 0;
}

STUB_FUNC(0x46a7c0)
void cool_nash_0x294::sub_46A7C0()
{
}

STUB_FUNC(0x46a850)
void cool_nash_0x294::sub_46A850()
{
}

STUB_FUNC(0x46a8f0)
s16 cool_nash_0x294::sub_46A8F0()
{
    return 0;
}

STUB_FUNC(0x46a9c0)
s32 cool_nash_0x294::sub_46A9C0()
{
    return 0;
}

STUB_FUNC(0x46aae0)
char_type cool_nash_0x294::sub_46AAE0()
{
    return 0;
}

STUB_FUNC(0x46ab50)
char_type cool_nash_0x294::sub_46AB50()
{
    return 0;
}

STUB_FUNC(0x46ac20)
char_type cool_nash_0x294::sub_46AC20()
{
    return 0;
}

STUB_FUNC(0x46b170)
s32 cool_nash_0x294::sub_46B170()
{
    return 0;
}

STUB_FUNC(0x46b2f0)
s32 cool_nash_0x294::sub_46B2F0()
{
    return 0;
}

STUB_FUNC(0x46b670)
void cool_nash_0x294::sub_46B670()
{
}

STUB_FUNC(0x46bd30)
void cool_nash_0x294::sub_46BD30()
{
}

STUB_FUNC(0x46bd50)
char_type cool_nash_0x294::sub_46BD50(u8 *a2)
{
    return 0;
}

STUB_FUNC(0x46bdc0)
void cool_nash_0x294::sub_46BDC0()
{
}

STUB_FUNC(0x46c250)
void cool_nash_0x294::sub_46C250()
{
}

STUB_FUNC(0x46c770)
char_type cool_nash_0x294::sub_46C770()
{
    return 0;
}

STUB_FUNC(0x46c7e0)
s32 cool_nash_0x294::sub_46C7E0()
{
    return 0;
}

STUB_FUNC(0x46c8a0)
s32 cool_nash_0x294::sub_46C8A0()
{
    return 0;
}

STUB_FUNC(0x46c910)
void cool_nash_0x294::sub_46C910()
{
}

STUB_FUNC(0x46c9b0)
void cool_nash_0x294::sub_46C9B0()
{
}

STUB_FUNC(0x46ca60)
void cool_nash_0x294::sub_46CA60()
{
}

STUB_FUNC(0x46ca70)
s32 cool_nash_0x294::sub_46CA70()
{
    return 0;
}

STUB_FUNC(0x46cb30)
char_type cool_nash_0x294::sub_46CB30()
{
    return 0;
}

STUB_FUNC(0x46cc70)
char_type cool_nash_0x294::sub_46CC70()
{
    return 0;
}

STUB_FUNC(0x46cdb0)
char_type cool_nash_0x294::sub_46CDB0()
{
    return 0;
}

STUB_FUNC(0x46cef0)
char_type cool_nash_0x294::sub_46CEF0()
{
    return 0;
}

STUB_FUNC(0x46d030)
void cool_nash_0x294::sub_46D030()
{
}

STUB_FUNC(0x46d0b0)
void cool_nash_0x294::sub_46D0B0()
{
}

STUB_FUNC(0x46d0d0)
void cool_nash_0x294::sub_46D0D0()
{
}

STUB_FUNC(0x46d240)
void cool_nash_0x294::sub_46D240()
{
}

STUB_FUNC(0x46d300)
void cool_nash_0x294::sub_46D300()
{
}

STUB_FUNC(0x46d460)
void cool_nash_0x294::sub_46D460(char_type a2)
{
}

STUB_FUNC(0x46db60)
void cool_nash_0x294::sub_46DB60()
{
}

STUB_FUNC(0x46db70)
void cool_nash_0x294::sub_46DB70()
{
}

STUB_FUNC(0x46db80)
void cool_nash_0x294::sub_46DB80()
{
}

STUB_FUNC(0x46df50)
Car_3C *cool_nash_0x294::sub_46DF50()
{
    return 0;
}

STUB_FUNC(0x46df70)
s32 cool_nash_0x294::sub_46DF70(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x46e020)
bool cool_nash_0x294::sub_46E020(Mouze_44 *a2)
{
    return 0;
}

STUB_FUNC(0x46e080)
s32 cool_nash_0x294::sub_46E080(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x46e200)
u8 cool_nash_0x294::sub_46E200(u8 a2)
{
    return 0;
}

STUB_FUNC(0x46ef00)
u8 cool_nash_0x294::get_wanted_star_count_46EF00()
{
    return 0;
}

STUB_FUNC(0x46ef40)
void cool_nash_0x294::set_wanted_level_46EF40(u16 wanted)
{
}

STUB_FUNC(0x46f070)
void cool_nash_0x294::set_wanted_star_count_46F070(u8 star_count)
{
}

STUB_FUNC(0x46f100)
s32 cool_nash_0x294::sub_46F100(u8 a2)
{
    return 0;
}

STUB_FUNC(0x46f110)
keen_bhaskara_0x30 *cool_nash_0x294::sub_46F110()
{
    return 0;
}

STUB_FUNC(0x46f1e0)
void cool_nash_0x294::sub_46F1E0(keen_bhaskara_0x30 *a2)
{
}

STUB_FUNC(0x46f390)
void cool_nash_0x294::sub_46F390()
{
}

STUB_FUNC(0x46f490)
s32 cool_nash_0x294::sub_46F490()
{
    return 0;
}

STUB_FUNC(0x46f600)
char_type cool_nash_0x294::sub_46F600(s32 a2)
{
    return 0;
}

STUB_FUNC(0x46f650)
u32 *cool_nash_0x294::sub_46F650(s32 a2)
{
    return 0;
}

STUB_FUNC(0x46f680)
void cool_nash_0x294::sub_46F680(cool_nash_0x294 *a2)
{
}

STUB_FUNC(0x46f720)
void cool_nash_0x294::sub_46F720()
{
}

STUB_FUNC(0x46f9d0)
void cool_nash_0x294::sub_46F9D0()
{
}

STUB_FUNC(0x46fc70)
u32 *cool_nash_0x294::sub_46FC70()
{
    return 0;
}

STUB_FUNC(0x46fc90)
void cool_nash_0x294::sub_46FC90(s32 a2, s32 a3)
{
}

STUB_FUNC(0x46fe20)
char_type cool_nash_0x294::sub_46FE20(s32 a2)
{
    return 0;
}

STUB_FUNC(0x46ff00)
void cool_nash_0x294::sub_46FF00(s32 a2, s32 a3, s32 a4)
{
}

STUB_FUNC(0x46fff0)
void cool_nash_0x294::sub_46FFF0(s32 a2)
{
}

STUB_FUNC(0x470050)
s16 cool_nash_0x294::sub_470050()
{
    return 0;
}

STUB_FUNC(0x470160)
void cool_nash_0x294::add_wanted_points_470160(s16 wanted_amount)
{
}

STUB_FUNC(0x4701d0)
bool cool_nash_0x294::sub_4701D0()
{
    return 0;
}

STUB_FUNC(0x470200)
s32 cool_nash_0x294::sub_470200(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4702d0)
void cool_nash_0x294::sub_4702D0(cool_nash_0x294 *pPed)
{
}

STUB_FUNC(0x470300)
s32 cool_nash_0x294::sub_470300()
{
    return 0;
}

STUB_FUNC(0x470f00)
s32 cool_nash_0x294::sub_470F00()
{
    return 0;
}

STUB_FUNC(0x470f90)
cool_nash_0x294 *cool_nash_0x294::sub_470F90(cool_nash_0x294 *pSrc)
{
    return 0;
}
