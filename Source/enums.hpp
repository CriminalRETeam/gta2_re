#pragma once

namespace car_manager_car_type_enum
{
enum
{
    New_Name = 0,
    New_Name_1 = 1,
    New_Name_2 = 2,
    New_Name_3 = 3,
    ambulance = 4,
    firetruck = 5,
    police_cars = 6
};
// static_assert(sizeof(car_manager_car_type_enum) == 4);
} // namespace car_manager_car_type_enum
namespace car_model_enum
{
enum
{
    alfa = 0,
    allard = 1,
    amdb4 = 2,
    apc = 3,
    bank_van = 4,
    bmw = 5,
    boxcar = 6,
    boxtruck = 7,
    bug = 8,
    CAR9 = 9,
    BUICK = 10,
    BUS = 11,
    COPCAR = 12,
    DART = 13,
    EDSEL = 14,
    CAR15 = 15,
    FIAT = 16,
    FIRETRUK = 17,
    GRAHAM = 18,
    GT24640 = 19,
    CAR20 = 20,
    GTRUCK = 21,
    GUNJEEP = 22,
    HOTDOG = 23,
    HOTDOG_D1 = 24,
    HOTDOG_D2 = 25,
    HOTDOG_D3 = 26,
    ICECREAM = 27,
    ISETLIMO = 28,
    ISETTA = 29,
    JEEP = 30,
    JEFFREY = 31,
    LIMO = 32,
    LIMO2 = 33,
    MEDICAR = 34,
    MERC = 35,
    MESSER = 36,
    MIURA = 37,
    MONSTER = 38,
    MORGAN = 39,
    MORRIS = 40,
    PICKUP = 41,
    RTYPE = 42,
    CAR43 = 43,
    SPIDER = 44,
    SPRITE = 45,
    STINGRAY = 46,
    STRATOS = 47,
    STRATOSB = 48,
    STRIPETB = 49,
    STYPE = 50,
    STYPECAB = 51,
    SWATVAN = 52,
    T2000GT = 53,
    TANK = 54,
    TANKER = 55,
    TAXI = 56,
    TBIRD = 57,
    TOWTRUCK = 58,
    TRAIN = 59,
    TRAINCAB = 60,
    TRAINFB = 61,
    TRANCEAM = 62,
    TRUKCAB1 = 63,
    TRUKCAB2 = 64,
    TRUKCONT = 65,
    TRUKTRNS = 66,
    TVVAN = 67,
    VAN = 68,
    VESPA = 69,
    VTYPE = 70,
    WBTWIN = 71,
    WRECK0 = 72,
    WRECK1 = 73,
    WRECK2 = 74,
    WRECK3 = 75,
    WRECK4 = 76,
    WRECK5 = 77,
    WRECK6 = 78,
    WRECK7 = 79,
    WRECK8 = 80,
    WRECK9 = 81,
    XK120 = 82,
    ZCX5 = 83,
    EDSELFBI = 84,
    HOTDOG_D4 = 85,
    KRSNABUS = 86,
    none = 87
};
// static_assert(sizeof(car_model_enum) == 4);
} // namespace car_model_enum

namespace cop_level_enum
{
enum cop_level_enum
{
    cops_0 = 0,
    cops_1 = 1,
    cops_2 = 2,
    cops_3 = 3,
    cops_4 = 4,
    cops_5 = 5,
    cops_6 = 6,
};
} // namespace cop_level_enum

namespace cop_level_ped_enum
{
enum
{ /* The cop level stored in the PED */
    cop_0_stars = 0,
    cop_1_stars = 600,
    cop_2_stars = 1600,
    cop_3_stars = 3000,
    cop_4_stars = 5000,
    cop_5_stars = 8000,
    cop_6_stars = 12000
};
// static_assert(sizeof(cop_level_ped_enum) == 2);
} // namespace cop_level_ped_enum

namespace gmp_zone_type_enum
{
enum
{
    general_purpose = 0,
    navigation = 1,
    traffic_light = 2,
    unused1 = 3,
    unused2 = 4,
    arrow_blocker = 5,
    railway_station_platform = 6,
    bus_stop_pavement = 7,
    general_trigger = 8,
    unused3 = 9,
    information = 10,
    railway_station_entry_point = 11,
    railway_station_exit_point = 12,
    railway_stop_point = 13,
    gang = 14,
    local_navigation = 15,
    restart = 16,
    unused4 = 17,
    unused5 = 18,
    unsued6 = 19,
    arrest_restart = 20
};
// static_assert(sizeof(gmp_zone_type_enum) == 4);
} // namespace gmp_zone_type_enum

namespace palette_types_enum
{
enum
{
    wrong_type = 0,
    tiles = 1,
    sprites = 2,
    car_remaps = 3,
    ped_remaps = 4,
    code_obj_remaps = 5,
    map_obj_remaps = 6,
    user_remaps = 7,
    font_remaps = 8,
};
// static_assert(sizeof(palette_types_enum) == 4);
} // namespace palette_types_enum

namespace ped_bit_status_enum
{
enum
{
    k_ped_0x00000001 = 0x00000001,
    k_ped_0x00000002 = 0x00000002,
    k_ped_0x00000004 = 0x00000004,
    k_ped_0x00000008 = 0x00000008,
    k_ped_0x00000010 = 0x00000010,
    k_ped_0x00000020 = 0x00000020,
    k_ped_0x00000040 = 0x00000040,
    k_ped_0x00000080 = 0x00000080,
    k_ped_0x00000100 = 0x00000100,
    k_ped_0x00000200 = 0x00000200,
    k_ped_0x00000400 = 0x00000400,
    k_ped_has_weapon = 0x00000800,
    k_ped_0x00001000 = 0x00001000,
    k_ped_0x00002000 = 0x00002000,
    k_ped_0x00004000 = 0x00004000,
    k_ped_0x00008000 = 0x00008000,
    k_ped_0x00010000 = 0x00010000,
    k_ped_0x00020000 = 0x00020000,
    k_ped_0x00040000 = 0x00040000,
    k_ped_0x00080000 = 0x00080000,
    k_ped_0x00100000 = 0x00100000,
    k_ped_0x00200000 = 0x00200000,
    k_ped_0x00400000 = 0x00400000,
    k_ped_0x00800000 = 0x00800000,
    k_ped_in_flames = 0x01000000,
    k_ped_invisible = 0x02000000,
    k_ped_0x04000000 = 0x04000000,
    k_ped_0x08000000 = 0x08000000,
    k_ped_0x10000000 = 0x10000000,
    k_ped_0x20000000 = 0x20000000,
    k_ped_0x40000000 = 0x40000000,
    k_ped_electro_fingers = 0x40000000,
    k_ped_0x80000000 = 0x80000000,
};
// static_assert(sizeof(ped_bit_status_enum) == 0x4);
} // namespace ped_bit_status_enum

namespace ped_major_state
{
enum
{
    ped_major_New_Name = 0,
    ped_major_walking = 1,
    ped_major_New_Name_1 = 2,
    ped_major_New_Name_2 = 3,
    ped_major_staying = 4,
    ped_major_driving = 5,
    ped_major_entering_a_car = 6,
    ped_major_getting_out_a_car = 7
};
// static_assert(sizeof(ped_major_state) == 0x1);
} // namespace ped_major_state

namespace ped_ocupation_enum
{
enum
{ /* The ocupation that a PED can have */
    player = 0,
    empty = 1,
    unknown_1 = 2,
    dummy = 3,
    unknown_2 = 4,
    driver = 5,
    unknown_3 = 6,
    unknown_4 = 7,
    unknown_5 = 8,
    unknown_6 = 9,
    driver_2 = 10,
    unknown_7 = 11,
    unknown_8 = 12,
    unknown_9 = 13,
    psycho = 14,
    mugger = 15,
    car_thief = 16,
    bank_robber = 17,
    criminal = 18,
    unknown_10 = 19,
    unknown_11 = 20,
    unknown_12 = 21,
    elvis = 22,
    unknown_13 = 23,
    police = 24,
    swat = 25,
    fbi = 26,
    army_army = 27,
    guard = 28,
    unknown_14 = 29,
    unknown_15 = 30,
    unknown_16 = 31,
    guard_against_player = 32,
    criminal_type_1 = 33,
    criminal_type_2 = 34,
    special_groups_member = 35,
    tank_driver = 36,
    unknown_17 = 37,
    fireman = 38,
    road_block_tank_man = 39,
    unknown_18 = 40,
    drone = 41,
    unknown_19 = 42,
    stand_still_bloke = 43,
    elvis_leader = 44,
    refugees = 45,
    any_law_enforcement = 46,
    any_emergency_service_man = 47,
    any_gang_member = 48,
    any_elvis = 49,
    driver_3 = 50,
    no_occupation = 51,
};
// static_assert(sizeof(ped_ocupation_enum) == 0x4);
} // namespace ped_ocupation_enum

namespace ped_remap_enum
{
enum
{
    ped_remap_blue_police = 0,
    ped_remap_green_police = 1,
    ped_remap_red_police = 2,
    ped_remap_yellow_police = 3,
    ped_remap_khaki_police = 4,
    ped_remap_red_head_redneck = 5,
    ped_remap_blond_head_redneck = 6,
    ped_remap_yellow_scientist = 7,
    ped_remap_zaibatsu = 8,
    ped_remap_kristna = 9,
    ped_remap_russian = 10,
    ped_remap_lonny = 11,
    ped_remap_elvis = 12,
    ped_remap_yakuza = 13,
    ped_remap_fireman = 14,
    ped_remap_green_shorts = 15,
    ped_remap_medic = 16,
    ped_remap_mugger = 17,
    ped_remap_blue_dummy = 18,
    ped_remap_light_blue_dummy = 19,
    ped_remap_tshirt_and_shorts_dummy = 20,
    ped_remap_short_sleeve = 21,
    ped_remap_prison_uniform = 22,
    ped_remap_hulk1_normal = 23,
    ped_remap_hulk2_green = 24,
    ped_remap_player = 25,
    ped_remap_naked_dummy = 26,
};
// static_assert(sizeof(ped_remap_enum) == 0x1);
} // namespace ped_remap_enum

namespace ped_state1_enum
{
enum
{
    ped_walking = 0,
    unused1 = 1,
    ped_going_to_car = 2,
    ped_entering_a_car = 3,
    ped_getting_out_a_car = 4,
    unused2 = 5,
    unused3 = 6,
    ped_staying = 7,
    ped_fall_on_ground = 8,
    ped_wasted = 9,
    ped_driving = 10,
};
// static_assert(sizeof(ped_state1_enum) == 0x4);
} // namespace ped_state1_enum

namespace ped_state2_enum
{
enum
{
    ped2_walking = 0,
    New_Name = 1,
    New_Name_1 = 2,
    New_Name_2 = 3,
    ped2_following_a_car = 4,
    New_Name_3 = 5,
    ped2_entering_a_car = 6,
    ped2_getting_out_a_car = 7,
    New_Name_4 = 8,
    New_Name_5 = 9,
    ped2_driving = 10,
    ped2_staying = 14,
    ped_unknown = 15
};
// static_assert(sizeof(ped_state2_enum) == 0x4);
} // namespace ped_state2_enum

namespace ped_type_enum
{
enum
{
    New_Name = 0,
    New_Name_1 = 1,
    ped_player = 2,
    New_Name_2 = 3,
    ped_elvis = 4,
    New_Name_3 = 5,
    ped_car_thief = 6,
};
// static_assert(sizeof(ped_type_enum) == 0x4);
} // namespace ped_type_enum

namespace spec_surface_type_enum
{
enum
{
    spec_grass = 2,
    spec_road_special = 3,
    spec_water = 4,
    spec_electrified = 5,
    spec_electrified_platform = 6,
    spec_wood_floor = 7,
    spec_metal_floor = 8,
    spec_metal_wall = 9,
    spec_grass_dirt_wall = 10,
};
// static_assert(sizeof(spec_surface_type_enum) == 4);
} // namespace spec_surface_type_enum

namespace sprite_types_enum
{
enum
{
    unknown_0 = 0, // At least in the function sprite_FUN_004b9aa0, but don't know what represents.
    unknown_1 = 1, // At least in the function sprite_FUN_004b9aa0, but don't know what represents.
    car = 2,
    ped = 3,
    code_obj1 = 4,
    map_obj = 5,
    user = 6,
    font = 7,
    code_obj2 = 8,
};
// static_assert(sizeof(sprite_types_enum) == 4);
} // namespace sprite_types_enum

namespace weapon_type
{
enum
{
    pistol = 0,
    smg = 1,
    rocket = 2,
    shocker = 3,
    molotov = 4,
    grenade = 5,
    shotgun = 6,
    electro_batton = 7,
    flamethrower = 8,
    silence_smg = 9,
    dual_pistol = 10,
    weapon_11 = 11,
    weapon_12 = 12,
    weapon_13 = 13,
    weapon_14 = 14,
    car_bomb = 15,
    oil_stain = 16,
    car_mines = 17,
    car_smg = 18,
    tank_main_gun = 19,
    fire_truck_gun = 20,
    fire_truck_flamethrower = 21,
    army_gun_jeep = 22,
    weapon_0x17 = 23,
};
// static_assert(sizeof(weapon_type) == 0x4);
} // namespace weapon_type

namespace objects
{
enum
{
    object_0 = 0,
    bin_lid_1 = 1,
    bollard_2 = 2,
    cone_3 = 3,
    boxes_4 = 4,
    blaster_5 = 5,
    rubbish_6 = 6,
    bin_7 = 7,
    animating_oil_8 = 8,
    oil_9 = 9,
    mine_10 = 10,
    bush_11 = 11,
    crate_12 = 12,
    footy_13 = 13,
    hardbox_14 = 14,
    newsdis_15 = 15,
    oildrum_16 = 16,
    tyre_17 = 17,
    hydrant_lid_18 = 18,
    hydrant_19 = 19,
    hydrant_unlid_20 = 20,
    roadblock_21 = 21,
    bench_22 = 22,
    package_23 = 23,
    object_24 = 24,
    tower_25 = 25,
    object_26 = 26,
    object_27 = 27,
    object_28 = 28,
    object_29 = 29,
    object_30 = 30,
    object_31 = 31,
    object_32 = 32,
    object_33 = 33,
    object_34 = 34,
    object_35 = 35,
    object_36 = 36,
    object_37 = 37,
    object_38 = 38,
    object_39 = 39,
    object_40 = 40,
    explode_medium_41 = 41,
    object_42 = 42,
    object_43 = 43,
    object_44 = 44,
    object_45 = 45,
    object_46 = 46,
    moving_collect_00_96 = 96,
    moving_collect_01_97 = 97,
    moving_collect_02_98 = 98,
    moving_collect_03_99 = 99,
    moving_collect_04_100 = 100,
    moving_collect_05_101 = 101,
    moving_collect_06_102 = 102,
    moving_collect_07_103 = 103,
    moving_collect_08_104 = 104,
    moving_collect_09_105 = 105,
    moving_collect_10_106 = 106,
    moving_collect_11_107 = 107,
    moving_collect_12_108 = 108,
    moving_collect_13_109 = 109,
    moving_collect_14_110 = 110,
    moving_collect_15_111 = 111,
    moving_collect_16_112 = 112,
    moving_collect_17_113 = 113,
    moving_collect_18_114 = 114,
    moving_collect_19_115 = 115,
    moving_collect_20_116 = 116,
    moving_collect_21_117 = 117,
    moving_collect_22_118 = 118,
    moving_collect_23_119 = 119,
    moving_collect_24_120 = 120,
    moving_collect_25_121 = 121,
    moving_collect_26_122 = 122,
    moving_collect_27_123 = 123,
    moving_collect_28_124 = 124,
    moving_collect_29_125 = 125,
    moving_collect_30_126 = 126,
    moving_collect_31_127 = 127,
    moving_collect_32_128 = 128,
    moving_collect_33_129 = 129,
    moving_collect_34_130 = 130,
    moving_collect_35_131 = 131,
    moving_collect_36_132 = 132,
    moving_collect_37_133 = 133,
    moving_collect_38_134 = 134,
    moving_collect_39_135 = 135,
    moving_collect_40_136 = 136,
    moving_collect_41_137 = 137,
    moving_collect_42_138 = 138,
    moving_collect_43_139 = 139,
    moving_collect_44_140 = 140,
    small_arrow_141 = 141,
    object_142 = 142,
    blood_spark_143 = 143,
    object_144 = 144,
    particle_system_145 = 145,
    firejet_146 = 146,
    object_147 = 147,
    small_brown_skid_148 = 148,
    small_grey_skid_149 = 149,
    small_red_skid_150 = 150,
    medium_brown_skid_151 = 151,
    medium_grey_skid_152 = 152,
    medium_red_skid_153 = 153,
    car_crossing_154 = 154,
    car_stop_155 = 155,
    big_white_skid_156 = 156,
    medium_white_skid_157 = 157,
    small_white_skid_158 = 158,
    object_159 = 159,
    rocket_160 = 160,
    bus_stop_marker_161 = 161,
    car_shop_162 = 162,
    busy_car_shop_163 = 163,
    car_bomb_164 = 164,
    object_165 = 165,
    object_166 = 166,
    molotov_moving_167 = 167,
    huge_red_skid_189 = 189,
    huge_white_skid_190 = 190,
    huge_brown_skid_191 = 191,
    huge_grey_skid_192 = 192,
    tanktop_193 = 193,
    antenna_194 = 194,
    object_195 = 195,
    animating_rubbish_196 = 196,
    dead_rubbish_197 = 197,
    moving_cone_198 = 198,
    object_199 = 199,
    remote_200 = 200
};
} // namespace objects

namespace threat_reaction_enum
{
enum
{
    no_reaction_0 = 0,
    react_as_emergency_1 = 1,
    react_as_normal_2 = 2,
    run_away_3 = 3,
};
} // namespace threat_reaction_enum

namespace objectives_enum
{
enum
{
    no_obj_0 = 0,
    flee_on_foot_till_safe_1 = 1,
    flee_char_on_foot_till_safe_2 = 2,
    flee_char_on_foot_always_3 = 3,
    objective_4 = 4,
    objective_5 = 5,
    objective_6 = 6,
    objective_7 = 7,
    objective_8 = 8,
    objective_9 = 9,
    objective_10 = 10,
    objective_11 = 11,
    goto_area_on_foot_12 = 12,
    objective_13 = 13,
    goto_area_in_car_14 = 14,
    objective_15 = 15,
    goto_char_on_foot_16 = 16,
    objective_17 = 17,
    objective_18 = 18,
    kill_char_any_means_19 = 19,
    kill_char_on_foot_20 = 20,
    objective_21 = 21,
    objective_22 = 22,
    objective_23 = 23,
    guard_spot_24 = 24,
    guard_area_25 = 25,
    wait_on_foot_26 = 26,
    wait_in_car_27 = 27,
    objective_28 = 28,
    objective_29 = 29,
    objective_30 = 30,
    objective_31 = 31,
    objective_32 = 32,
    objective_33 = 33,
    objective_34 = 34,
    enter_car_as_driver_35 = 35,
    leave_car_36 = 36,
    objective_37 = 37,
    objective_38 = 38,
    objective_39 = 39,
    objective_40 = 40,
    objective_41 = 41,
    objective_42 = 42,
    objective_43 = 43,
    objective_44 = 44,
    objective_45 = 45,
    objective_46 = 46,
    objective_47 = 47,
    objective_48 = 48,
    objective_49 = 49,
    follow_car_in_car_50 = 50,
    follow_car_on_foot_with_offset_51 = 51,
    fire_at_object_from_vehicle_52 = 52,
    destroy_object_53 = 53,
    destroy_car_54 = 54,
};
} // namespace objectives_enum

namespace threat_search_enum
{
enum
{
    no_threats_0 = 0,
    line_of_sight_1 = 1,
    area_2 = 2,
    area_player_threat_only_3 = 3,
    line_of_sight_player_threat_only_4 = 4,
    area_player_only_5 = 5,
    line_of_sight_player_only_6 = 6,
};
} // namespace threat_search_enum

namespace CarDamageAreas
{
enum
{
    FrontLeft_0 = 0x0,
    FrontRight_1 = 0x1,
    BackLeft_2 = 0x2,
    BackRight_3 = 0x3,
    Window_4 = 0x4,
};
} // namespace CarDamageAreas

namespace CarDamageAreasFlags
{
enum
{
    FlagsBackRight_1 = 0x1,
    FlagsBackLeft_2 = 0x2,
    FlagsFrontLeft_4 = 0x4,
    FlagsFrontRight_8 = 0x8,
    FlagsWindow_10 = 0x10,
};
} // namespace CarDamageAreasFlags
