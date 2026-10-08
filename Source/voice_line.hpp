#pragma once

// Argument of root_sound::PlayVoice_40F090 (sound_obj::PlayVocal_418C80): the vocal to play, named after the sample in
// data/audio/Vocals it ends up playing (see sound_obj::ProcessType10_Vocals_418CA0, which maps these to gVocNames_5FEA5C).
// Values that are not mapped there play nothing. Kept in its own header, like car_despawn_status.hpp, so that the enum
// does not land in a header many TUs share.
namespace voice_line
{
enum
{
    insanestunt_1 = 1, // insanestunt.wav
    gta_2 = 2, // gta.wav
    wipeout_3 = 3, // wipeout.wav
    expiditious_4 = 4, // expiditious.wav
    genocide_5 = 5, // genocide.wav
    copkilla_6 = 6, // copkilla.wav
    carjacker_7 = 7, // carjacker.wav
    elvis_8 = 8, // elvis.wav
    accuracyb_9 = 9, // accuracyb.wav
    back2front_10 = 10, // back2front.wav
    medicalemer_11 = 11, // medicalemer.wav
    killfrenzy_12 = 12, // killfrenzy.wav
    busted_17 = 17, // busted.wav
    frenzyfail_18 = 18, // frenzyfail.wav
    frenzypassed_19 = 19, // frenzypassed.wav
    fryingtonite_20 = 20, // fryingtonite.wav
    game_over_21 = 21, // not mapped in ProcessType10_Vocals_418CA0 (plays nothing; gameover.wav is never reached)
    jobcomplete_22 = 22, // jobcomplete.wav
    jobfail_23 = 23, // jobfail.wav
    respectis_24 = 24, // respectis.wav
    shocking_25 = 25, // shocking.wav
    somethingscookin_26 = 26, // somethingscookin.wav
    timesup_27 = 27, // timesup.wav
    toasted_28 = 28, // toasted.wav
    wasted_29 = 29, // wasted.wav
    bombarmed_30 = 30, // bombarmed.wav
    random_laugh_31 = 31, // laugh1..laugh9, lauga..laugd (random)
    random_laugh_32 = 32, // laugh1..laugh9, lauga..laugd (random)
    raceover_33 = 33, // raceover.wav
    second_lap_34 = 34, // 2ndlap.wav
    finallap_35 = 35, // finallap.wav
    raceon_36 = 36, // raceon.wav
    people_down_30_37 = 37, // 30peopledown.wav
    people_down_60_38 = 38, // 60peopledown.wav
    people_down_90_39 = 39, // 90peopledown.wav
    people_down_120_40 = 40, // 120peopledown.wav
    people_down_150_41 = 41, // 150peopledown.wav
    timeout_42 = 42, // timeout.wav
    timeextended_43 = 43, // timeextended.wav
    timesup_44 = 44, // timesup.wav
    sorrydidthathurt_45 = 45, // sorrydidthathurt.wav
    nicework_46 = 46, // nicework.wav
    choctastic_47 = 47, // choctastic.wav
    raspberryripple_48 = 48, // raspberryripple.wav
    youshotyourload_49 = 49, // youshotyourload.wav
    oohdidthathurt_50 = 50, // oohdidthathurt.wav
    deathtoicvans_51 = 51, // deathtoicvans.wav
    crispycritter_52 = 52, // crispycritter.wav
    youretoastbuddy_53 = 53, // youretoastbuddy.wav
    eatleaddeath_54 = 54, // eatleaddeath.wav
    thatsgottahurt_55 = 55, // thatsgottahurt.wav
    sorryaboutthat_56 = 56, // sorryaboutthat.wav
    xinloimyman_57 = 57, // xinloimyman.wav
    damnsundaydrivers_58 = 58, // damnsundaydrivers.wav
    suckitandsee_59 = 59, // suckitandsee.wav
    tastemywrath_60 = 60, // tastemywrath.wav
    hallelujah_61 = 61, // hallelujah.wav
    damnation_62 = 62, // damnation.wav
};
} // namespace voice_line
