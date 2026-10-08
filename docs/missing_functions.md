# Functions with no marker

Functions listed in `Scripts/bin_comp/og_function_data_v105.csv` that have no `MATCH_FUNC`, `WIP_FUNC` or `STUB_FUNC` anywhere in `Source/`. Regenerate this list with the script in the commit that added it.

- Total in the CSV: 4425. With a marker: 3373. Without one: 1069.
- 430 of them (53989 bytes) are at 0x5ED000 or above, which is CRT and iostream library code.
- 639 of them (18803 bytes) are game code. Many are small out-of-line copies of inline methods (COMDAT), static init/dtor thunks, or helpers.

Game functions by size:

| Size | Count |
|---|---|
| <= 16 bytes | 508 |
| 17-64 | 64 |
| 65-256 | 49 |
| > 256 | 18 |

## Game functions without a marker

| Address | Size | Name |
|---|---|---|
| 0x401000 | 72 | `sub_401000` |
| 0x401050 | 41 | `sub_401050` |
| 0x401080 | 94 | `sub_401080` |
| 0x4010E0 | 153 | `sub_4010E0` |
| 0x401180 | 1089 | `sub_401180` |
| 0x4015D0 | 316 | `Matrix_Mult_4015D0` |
| 0x401710 | 42 | `Matrix_PushAndMulti_401710` |
| 0x401740 | 8 | `Matrix_Pop_401740` |
| 0x401750 | 109 | `sub_401750` |
| 0x4017C0 | 217 | `sub_4017C0` |
| 0x4018A0 | 211 | `sub_4018A0` |
| 0x401980 | 1 | `nullsub_44` |
| 0x401990 | 524 | `sub_401990` |
| 0x401BA0 | 117 | `sub_401BA0` |
| 0x401C20 | 211 | `sub_401C20` |
| 0x401D00 | 400 | `sub_401D00` |
| 0x401ED0 | 535 | `sub_401ED0` |
| 0x4020F0 | 280 | `sub_4020F0` |
| 0x402210 | 478 | `sub_402210` |
| 0x4023F0 | 314 | `sub_4023F0` |
| 0x404AA0 | 1 | `nullsub_45` |
| 0x404AE0 | 1 | `nullsub_46` |
| 0x4057B0 | 421 | `sub_4057B0` |
| 0x405DA0 | 118 | `sub_405DA0` |
| 0x405E20 | 95 | `sub_405E20` |
| 0x405E80 | 272 | `sub_405E80` |
| 0x40AC40 | 1 | `Car_8::dtor_40AC40` |
| 0x40AC80 | 42 | `Fix16::subtract_40AC80` |
| 0x40ACB0 | 31 | `Fix16::Negate_40ACB0` |
| 0x40AD20 | 1 | `nullsub_47` |
| 0x40AD60 | 1 | `nullsub_48` |
| 0x40B740 | 1 | `nullsub_49` |
| 0x40B790 | 1 | `nullsub_50` |
| 0x40E3B0 | 1 | `nullsub_51` |
| 0x40E3F0 | 1 | `nullsub_52` |
| 0x40EDB0 | 1 | `nullsub_53` |
| 0x40EE00 | 1 | `nullsub_54` |
| 0x40EF10 | 10 | `root_sound::static_dtor_40EF10` |
| 0x4116D0 | 1 | `nullsub_55` |
| 0x411710 | 1 | `nullsub_56` |
| 0x412110 | 1 | `nullsub_57` |
| 0x412160 | 1 | `nullsub_58` |
| 0x4185C0 | 1 | `nullsub_59` |
| 0x418600 | 1 | `nullsub_60` |
| 0x419A90 | 1 | `nullsub_61` |
| 0x419AE0 | 1 | `nullsub_62` |
| 0x419DF0 | 3 | `sub_419DF0` |
| 0x41B480 | 14 | `Fix16::sub_41B480` |
| 0x41DCF0 | 1 | `nullsub_63` |
| 0x41DD30 | 1 | `nullsub_64` |
| 0x41E6F0 | 1 | `nullsub_65` |
| 0x41E740 | 1 | `nullsub_66` |
| 0x422030 | 1 | `nullsub_67` |
| 0x422070 | 1 | `nullsub_68` |
| 0x422A00 | 1 | `nullsub_69` |
| 0x422A50 | 1 | `nullsub_70` |
| 0x425C20 | 1 | `nullsub_71` |
| 0x425C60 | 1 | `nullsub_72` |
| 0x426620 | 1 | `nullsub_73` |
| 0x426670 | 1 | `nullsub_74` |
| 0x427170 | 10 | `cSampleManager::sub_427170` |
| 0x427330 | 1 | `nullsub_4` |
| 0x4299D0 | 1 | `nullsub_75` |
| 0x429A10 | 1 | `nullsub_76` |
| 0x42A3D0 | 1 | `nullsub_77` |
| 0x42A420 | 1 | `nullsub_78` |
| 0x42CCB0 | 1 | `nullsub_79` |
| 0x42CCF0 | 1 | `nullsub_80` |
| 0x42D6B0 | 1 | `nullsub_81` |
| 0x42D700 | 1 | `nullsub_82` |
| 0x42D9B0 | 1 | `nullsub_83` |
| 0x42FF00 | 1 | `nullsub_84` |
| 0x42FF40 | 1 | `nullsub_85` |
| 0x430900 | 1 | `nullsub_86` |
| 0x430950 | 1 | `nullsub_87` |
| 0x434990 | 1 | `nullsub_88` |
| 0x4349D0 | 1 | `nullsub_89` |
| 0x435360 | 1 | `nullsub_90` |
| 0x4353B0 | 1 | `nullsub_91` |
| 0x4369F0 | 14 | `Fix16::FromInt_4369F0` |
| 0x436A00 | 19 | `Fix16::Subtract_436A00` |
| 0x439070 | 1 | `nullsub_92` |
| 0x4390B0 | 1 | `nullsub_93` |
| 0x439AA0 | 1 | `nullsub_94` |
| 0x439AF0 | 1 | `nullsub_95` |
| 0x4415F0 | 1 | `nullsub_96` |
| 0x441990 | 1 | `nullsub_97` |
| 0x4419D0 | 1 | `nullsub_98` |
| 0x444CE0 | 5 | `NoRefs_444CE0` |
| 0x447170 | 1 | `nullsub_99` |
| 0x447200 | 1 | `nullsub_100` |
| 0x447250 | 1 | `nullsub_101` |
| 0x451670 | 22 | `Fix16::sub_451670` |
| 0x451690 | 22 | `Fix16::sub_451690` |
| 0x4516F0 | 52 | `sub_4516F0` |
| 0x4517B0 | 1 | `nullsub_102` |
| 0x4517F0 | 1 | `nullsub_103` |
| 0x454170 | 1 | `nullsub_104` |
| 0x4541C0 | 1 | `nullsub_105` |
| 0x454940 | 84 | `dtor_454940` |
| 0x4570A0 | 1 | `nullsub_106` |
| 0x4570E0 | 1 | `nullsub_107` |
| 0x457890 | 1 | `nullsub_108` |
| 0x4578E0 | 1 | `nullsub_109` |
| 0x45A200 | 1 | `nullsub_110` |
| 0x45A240 | 1 | `nullsub_111` |
| 0x45ABD0 | 1 | `nullsub_112` |
| 0x45AC20 | 1 | `nullsub_113` |
| 0x45C4E0 | 20 | `Fix16::FromInt_45C4E0` |
| 0x4614E0 | 66 | `sub_4614E0` |
| 0x462450 | 1 | `nullsub_114` |
| 0x462490 | 1 | `nullsub_115` |
| 0x466B70 | 28 | `HasBit25AndGameObject_466B70` |
| 0x467AB0 | 1 | `FleeCharAnyMeansTillSafe_Nullsub` |
| 0x467AC0 | 1 | `FleeCharAnyMeansAlways_Nullsub` |
| 0x467E10 | 1 | `KillCar_Nullsub` |
| 0x4685F0 | 1 | `nullsub_116` |
| 0x468640 | 1 | `nullsub_117` |
| 0x469BC0 | 1 | `Objective50_Nullsub` |
| 0x46A1E0 | 1 | `nullsub_118` |
| 0x46D0A0 | 1 | `State30_Nullsub` |
| 0x4710F0 | 31 | `Char_11944::dtor_4710F0` |
| 0x473C80 | 1 | `nullsub_119` |
| 0x473CC0 | 1 | `nullsub_120` |
| 0x474680 | 1 | `nullsub_121` |
| 0x4746D0 | 1 | `nullsub_122` |
| 0x477280 | 1 | `nullsub_123` |
| 0x4772C0 | 1 | `nullsub_124` |
| 0x47B1D0 | 1 | `nullsub_125` |
| 0x47B210 | 1 | `nullsub_126` |
| 0x47BBD0 | 1 | `nullsub_127` |
| 0x47BC20 | 1 | `nullsub_128` |
| 0x483210 | 1 | `nullsub_129` |
| 0x483250 | 1 | `nullsub_130` |
| 0x483C30 | 1 | `nullsub_131` |
| 0x483C80 | 1 | `nullsub_132` |
| 0x4876B0 | 1 | `nullsub_133` |
| 0x4876F0 | 1 | `nullsub_134` |
| 0x4880B0 | 1 | `nullsub_135` |
| 0x488100 | 1 | `nullsub_136` |
| 0x48AE00 | 1 | `nullsub_137` |
| 0x48AE40 | 1 | `nullsub_138` |
| 0x48B7D0 | 1 | `nullsub_139` |
| 0x48B820 | 1 | `nullsub_140` |
| 0x48B9A0 | 3 | `nullsub_141` |
| 0x48DEF0 | 1 | `nullsub_142` |
| 0x48DF30 | 1 | `nullsub_143` |
| 0x491F10 | 1 | `nullsub_144` |
| 0x491F50 | 1 | `nullsub_145` |
| 0x4926F0 | 14 | `Fix16::FromInt_4926F0` |
| 0x494C70 | 1 | `nullsub_146` |
| 0x494CB0 | 1 | `nullsub_147` |
| 0x497BA0 | 1 | `nullsub_148` |
| 0x497BE0 | 1 | `nullsub_149` |
| 0x4985A0 | 1 | `nullsub_150` |
| 0x4985F0 | 1 | `nullsub_151` |
| 0x49B750 | 1 | `nullsub_152` |
| 0x49B790 | 1 | `nullsub_153` |
| 0x49C1F0 | 1 | `nullsub_154` |
| 0x49C240 | 1 | `nullsub_155` |
| 0x49C330 | 1 | `nullsub_156` |
| 0x49FB20 | 1 | `nullsub_157` |
| 0x49FB60 | 1 | `nullsub_158` |
| 0x4A0520 | 1 | `nullsub_159` |
| 0x4A0570 | 1 | `nullsub_160` |
| 0x4A0710 | 10 | `MissLog2_static_ctor_4A0710` |
| 0x4A0760 | 10 | `log_4A0760` |
| 0x4A3610 | 102 | `MissLog2_4A3610` |
| 0x4A3680 | 20 | `sub_4A3680` |
| 0x4A6310 | 1 | `nullsub_161` |
| 0x4A6350 | 1 | `nullsub_162` |
| 0x4A6BE0 | 152 | `File::sub_4A6BE0` |
| 0x4A6DB0 | 197 | `File::sub_4A6DB0` |
| 0x4A8890 | 55 | `NoRefs_4A8890` |
| 0x4AAE20 | 1 | `nullsub_163` |
| 0x4AAE60 | 1 | `nullsub_164` |
| 0x4AB820 | 1 | `nullsub_165` |
| 0x4AB870 | 1 | `nullsub_166` |
| 0x4AE170 | 93 | `FreeLoader::sub_4AE170` |
| 0x4AE970 | 20 | `Fix16::FromU16_4AE970` |
| 0x4B3AE0 | 1 | `nullsub_167` |
| 0x4B3CB0 | 1 | `nullsub_168` |
| 0x4B7D60 | 161 | `sub_4B7D60` |
| 0x4B80F0 | 389 | `sub_4B80F0` |
| 0x4B8A80 | 1 | `nullsub_169` |
| 0x4B8AD0 | 1 | `nullsub_170` |
| 0x4BBCB0 | 389 | `get_audio_path_4BBCB0` |
| 0x4BBFA0 | 389 | `sub_4BBFA0` |
| 0x4BDD80 | 1 | `nullsub_171` |
| 0x4BDDC0 | 1 | `nullsub_172` |
| 0x4BE950 | 1 | `nullsub_173` |
| 0x4BE9A0 | 1 | `nullsub_174` |
| 0x4C18B0 | 1 | `nullsub_175` |
| 0x4C18F0 | 1 | `nullsub_176` |
| 0x4C4390 | 1 | `nullsub_177` |
| 0x4C43D0 | 1 | `nullsub_178` |
| 0x4C4D90 | 1 | `nullsub_179` |
| 0x4C4DE0 | 1 | `nullsub_180` |
| 0x4C8320 | 1 | `nullsub_181` |
| 0x4C8360 | 1 | `nullsub_182` |
| 0x4C8CF0 | 1 | `nullsub_183` |
| 0x4C8D40 | 1 | `nullsub_184` |
| 0x4CD170 | 1 | `nullsub_186` |
| 0x4CD1B0 | 1 | `nullsub_187` |
| 0x4CDB70 | 1 | `nullsub_188` |
| 0x4CDBC0 | 1 | `nullsub_189` |
| 0x4CFFB0 | 151 | `sub_4CFFB0` |
| 0x4D13C0 | 1 | `nullsub_190` |
| 0x4D1400 | 1 | `nullsub_191` |
| 0x4D1E90 | 1 | `nullsub_192` |
| 0x4D1EE0 | 1 | `nullsub_193` |
| 0x4D5270 | 1 | `nullsub_194` |
| 0x4D52B0 | 1 | `nullsub_195` |
| 0x4D5C40 | 1 | `nullsub_196` |
| 0x4D5C90 | 1 | `nullsub_197` |
| 0x4DB120 | 71 | `sub_4DB120` |
| 0x4DB410 | 40 | `FatalErrorMsg_4DB410` |
| 0x4DDB50 | 1 | `nullsub_198` |
| 0x4DDB90 | 1 | `nullsub_199` |
| 0x4DE5F0 | 1 | `nullsub_200` |
| 0x4DE640 | 1 | `nullsub_201` |
| 0x4E5630 | 1 | `nullsub_202` |
| 0x4E5B20 | 1 | `nullsub_203` |
| 0x4E5D10 | 69 | `sub_4E5D10` |
| 0x4E5D70 | 126 | `sub_4E5D70` |
| 0x4E5E00 | 126 | `sub_4E5E00` |
| 0x4E6280 | 36 | `Fix16_Rect::MakeRect_4E6280` |
| 0x4E9B80 | 1 | `nullsub_204` |
| 0x4E9BD0 | 1 | `nullsub_205` |
| 0x4E9D40 | 1 | `nullsub_207` |
| 0x4F5FC0 | 1 | `nullsub_208` |
| 0x4F6210 | 1 | `nullsub_209` |
| 0x4FA150 | 1 | `nullsub_210` |
| 0x4FA190 | 1 | `nullsub_211` |
| 0x4FD9A0 | 1 | `nullsub_212` |
| 0x4FD9E0 | 1 | `nullsub_213` |
| 0x4FE3A0 | 1 | `nullsub_214` |
| 0x4FE3F0 | 1 | `nullsub_215` |
| 0x5024F0 | 1 | `nullsub_216` |
| 0x502530 | 1 | `nullsub_217` |
| 0x502D50 | 10 | `Miss2Logger::static_dtor_502D50` |
| 0x50A390 | 1 | `nullsub_218` |
| 0x50A3D0 | 1 | `nullsub_219` |
| 0x511710 | 1 | `nullsub_220` |
| 0x511760 | 1 | `nullsub_221` |
| 0x515D00 | 1 | `nullsub_222` |
| 0x515D40 | 1 | `nullsub_223` |
| 0x518D60 | 1 | `nullsub_224` |
| 0x518DA0 | 1 | `nullsub_225` |
| 0x519760 | 1 | `nullsub_226` |
| 0x5197B0 | 1 | `nullsub_227` |
| 0x519B80 | 30 | `Network_20324::vdtor_519B80` |
| 0x5212E0 | 7 | `goofy_thompson:NoRefs_5212E0` |
| 0x523060 | 830 | `sub_523060` |
| 0x525D50 | 53 | `sub_525D50` |
| 0x529700 | 1 | `nullsub_229` |
| 0x529740 | 1 | `nullsub_230` |
| 0x529B20 | 155 | `Object_5C::sub_529B20` |
| 0x52B170 | 1 | `nullsub_231` |
| 0x52B1C0 | 1 | `nullsub_232` |
| 0x531940 | 3 | `sub_531940` |
| 0x533C80 | 1 | `nullsub_233` |
| 0x534260 | 1 | `nullsub_234` |
| 0x5345E0 | 85 | `sub_5345E0` |
| 0x537440 | 1 | `nullsub_235` |
| 0x537480 | 1 | `nullsub_236` |
| 0x537E10 | 1 | `nullsub_237` |
| 0x537E60 | 1 | `nullsub_238` |
| 0x539F90 | 46 | `Fix16::div_539F90` |
| 0x53E860 | 20 | `sub_53E860` |
| 0x53F050 | 3 | `nullsub_239` |
| 0x53FDF0 | 1 | `nullsub_240` |
| 0x53FE30 | 1 | `nullsub_241` |
| 0x540A40 | 594 | `Explosion_30::sub_540A40` |
| 0x543450 | 1 | `nullsub_242` |
| 0x543460 | 1 | `nullsub_243` |
| 0x543470 | 1 | `nullsub_244` |
| 0x543480 | 1 | `nullsub_245` |
| 0x543490 | 1 | `nullsub_246` |
| 0x543900 | 118 | `ExplosionPool_7A8::sub_543900` |
| 0x543D90 | 1 | `nullsub_247` |
| 0x543DE0 | 1 | `nullsub_248` |
| 0x54C6B0 | 1 | `nullsub_249` |
| 0x54C8F0 | 1 | `nullsub_250` |
| 0x551CA0 | 1 | `nullsub_28` |
| 0x5534E0 | 1 | `nullsub_251` |
| 0x553530 | 1 | `nullsub_252` |
| 0x557CC0 | 1 | `nullsub_253` |
| 0x557D00 | 1 | `nullsub_254` |
| 0x5586C0 | 1 | `nullsub_255` |
| 0x558710 | 1 | `nullsub_256` |
| 0x558E70 | 21 | `sub_558E70` |
| 0x558EC0 | 21 | `sub_558EC0` |
| 0x558F90 | 21 | `sub_558F90` |
| 0x558FE0 | 21 | `sub_558FE0` |
| 0x5590B0 | 21 | `sub_5590B0` |
| 0x559100 | 21 | `sub_559100` |
| 0x5591D0 | 21 | `sub_5591D0` |
| 0x559220 | 21 | `sub_559220` |
| 0x559350 | 21 | `sub_559350` |
| 0x5593A0 | 21 | `sub_5593A0` |
| 0x5593F0 | 21 | `sub_5593F0` |
| 0x55EEE0 | 59 | `sub_55EEE0` |
| 0x560630 | 1 | `nullsub_268` |
| 0x560670 | 1 | `nullsub_269` |
| 0x561DB0 | 18 | `Fix16::sub_561DB0` |
| 0x562430 | 29 | `Fix16::sub_562430` |
| 0x5641D0 | 1 | `nullsub_270` |
| 0x564220 | 1 | `nullsub_271` |
| 0x56A680 | 1 | `nullsub_272` |
| 0x56A6C0 | 1 | `nullsub_273` |
| 0x56B3D0 | 1 | `nullsub_274` |
| 0x56B420 | 1 | `nullsub_275` |
| 0x56C170 | 95 | `jolly_poitras_0x2BC0::sub_56C170` |
| 0x56E8B0 | 1 | `nullsub_276` |
| 0x56E8F0 | 1 | `nullsub_277` |
| 0x56F2B0 | 1 | `nullsub_278` |
| 0x56F300 | 1 | `nullsub_279` |
| 0x571340 | 1 | `nullsub_280` |
| 0x5721E0 | 1 | `nullsub_281` |
| 0x575C50 | 1 | `nullsub_282` |
| 0x575C90 | 1 | `nullsub_283` |
| 0x577CF0 | 1 | `nullsub_284` |
| 0x577D40 | 1 | `nullsub_285` |
| 0x57D180 | 1 | `nullsub_286` |
| 0x57D1C0 | 1 | `nullsub_287` |
| 0x57DB50 | 1 | `nullsub_288` |
| 0x57DBA0 | 1 | `nullsub_289` |
| 0x581690 | 1 | `nullsub_290` |
| 0x5816D0 | 1 | `nullsub_291` |
| 0x582090 | 1 | `nullsub_292` |
| 0x5820E0 | 1 | `nullsub_293` |
| 0x585E90 | 1 | `nullsub_294` |
| 0x585ED0 | 1 | `nullsub_295` |
| 0x586890 | 1 | `nullsub_296` |
| 0x5868E0 | 1 | `nullsub_297` |
| 0x5869F0 | 10 | `Registry::sub_5869F0` |
| 0x5875A0 | 64 | `Registry::sub_5875A0` |
| 0x5875E0 | 59 | `Registry::sub_5875E0` |
| 0x587620 | 108 | `Registry::sub_587620` |
| 0x58C7B0 | 1 | `nullsub_298` |
| 0x58C7F0 | 1 | `nullsub_299` |
| 0x58D1F0 | 1 | `nullsub_300` |
| 0x58D240 | 1 | `nullsub_301` |
| 0x58D610 | 1 | `nullsub_302` |
| 0x58E000 | 7 | `cSampleManager::sub_58E000` |
| 0x5910A0 | 1 | `nullsub_303` |
| 0x5910E0 | 1 | `nullsub_304` |
| 0x591AA0 | 1 | `nullsub_305` |
| 0x591AF0 | 1 | `nullsub_306` |
| 0x595B20 | 1 | `nullsub_307` |
| 0x595B60 | 1 | `nullsub_308` |
| 0x596520 | 1 | `nullsub_309` |
| 0x596570 | 1 | `nullsub_310` |
| 0x599840 | 1 | `nullsub_311` |
| 0x599880 | 1 | `nullsub_312` |
| 0x59A2E0 | 1 | `nullsub_313` |
| 0x59A330 | 1 | `nullsub_314` |
| 0x59D660 | 1 | `nullsub_315` |
| 0x59D6A0 | 1 | `nullsub_316` |
| 0x59F940 | 1 | `nullsub_317` |
| 0x5A5690 | 192 | `sub_5A5690` |
| 0x5A5C90 | 1 | `nullsub_318` |
| 0x5A5CD0 | 1 | `nullsub_319` |
| 0x5A67E0 | 1 | `nullsub_320` |
| 0x5A6830 | 1 | `nullsub_321` |
| 0x5A9880 | 1 | `nullsub_322` |
| 0x5A98C0 | 1 | `nullsub_323` |
| 0x5AA280 | 1 | `nullsub_324` |
| 0x5AA2D0 | 1 | `nullsub_325` |
| 0x5B0670 | 1 | `nullsub_326` |
| 0x5B06B0 | 1 | `nullsub_327` |
| 0x5B1040 | 1 | `nullsub_328` |
| 0x5B1090 | 1 | `nullsub_329` |
| 0x5B2610 | 1 | `nullsub_330` |
| 0x5B2620 | 1 | `nullsub_331` |
| 0x5B2630 | 1 | `nullsub_332` |
| 0x5B2750 | 3 | `sub_5B2750` |
| 0x5B2760 | 1 | `nullsub_333` |
| 0x5B4CB0 | 1 | `nullsub_334` |
| 0x5B4CF0 | 1 | `nullsub_335` |
| 0x5B56B0 | 1 | `nullsub_336` |
| 0x5B5700 | 1 | `nullsub_337` |
| 0x5B8620 | 1 | `nullsub_338` |
| 0x5B8660 | 1 | `nullsub_339` |
| 0x5BBF90 | 1 | `nullsub_340` |
| 0x5BBFD0 | 1 | `nullsub_341` |
| 0x5BEA00 | 1 | `nullsub_342` |
| 0x5BEA40 | 1 | `nullsub_343` |
| 0x5C1420 | 1 | `nullsub_344` |
| 0x5C1460 | 1 | `nullsub_345` |
| 0x5C50D0 | 1 | `nullsub_346` |
| 0x5C5110 | 1 | `nullsub_347` |
| 0x5C5AD0 | 1 | `nullsub_348` |
| 0x5C5B20 | 1 | `nullsub_349` |
| 0x5C5CD0 | 26 | `sub_5C5CD0` |
| 0x5C84E0 | 1 | `nullsub_350` |
| 0x5C8520 | 1 | `nullsub_351` |
| 0x5CB040 | 1 | `nullsub_352` |
| 0x5CB080 | 1 | `nullsub_353` |
| 0x5CBA40 | 1 | `nullsub_354` |
| 0x5CBA90 | 1 | `nullsub_355` |
| 0x5CEAF0 | 1 | `nullsub_356` |
| 0x5CEB30 | 1 | `nullsub_357` |
| 0x5CF4F0 | 1 | `nullsub_358` |
| 0x5CF540 | 1 | `nullsub_359` |
| 0x5D6970 | 1 | `nullsub_360` |
| 0x5D69B0 | 1 | `nullsub_361` |
| 0x5D7AC0 | 1 | `nullsub_362` |
| 0x5D7B10 | 1 | `nullsub_363` |
| 0x5D7DD0 | 235 | `sub_5D7DD0` |
| 0x5D98D0 | 9 | `IsFullScreen_5D98D0` |
| 0x5DBEF0 | 1 | `nullsub_364` |
| 0x5DBF30 | 1 | `nullsub_365` |
| 0x5DC9C0 | 1 | `nullsub_366` |
| 0x5DCA10 | 1 | `nullsub_367` |
| 0x5DCD00 | 1 | `nullsub_369` |
| 0x5DCED0 | 25 | `sub_5DCED0` |
| 0x5E40C0 | 30 | `Fix16::sub_5E40C0` |
| 0x5E40E0 | 81 | `Fix16::sub_5E40E0` |
| 0x5E4140 | 43 | `Fix16::sub_5E4140` |
| 0x5E41A0 | 1 | `nullsub_370` |
| 0x5E41E0 | 1 | `nullsub_371` |
| 0x5E4BA0 | 1 | `nullsub_372` |
| 0x5E4BF0 | 1 | `nullsub_373` |
| 0x5E8320 | 1 | `nullsub_374` |
| 0x5E8360 | 1 | `nullsub_375` |
| 0x5E8DF0 | 1 | `nullsub_376` |
| 0x5E8E40 | 1 | `nullsub_377` |
| 0x5E8F20 | 6 | `OutputDebugStringA` |
| 0x5E8F26 | 6 | `FreeLibrary` |
| 0x5E8F2C | 6 | `LoadLibraryA` |
| 0x5E8F32 | 6 | `LocalFree` |
| 0x5E8F38 | 6 | `FormatMessageA` |
| 0x5E8F3E | 6 | `GetLastError` |
| 0x5E8F44 | 6 | `GetDriveTypeA` |
| 0x5E8F4A | 6 | `lstrcatA` |
| 0x5E8F50 | 6 | `CloseHandle` |
| 0x5E8F56 | 6 | `OpenMutexA` |
| 0x5E8F5C | 6 | `Sleep` |
| 0x5E8F62 | 6 | `GetProcAddress` |
| 0x5E8F68 | 6 | `GetVersionExA` |
| 0x5E8F6E | 6 | `WideCharToMultiByte` |
| 0x5E8F74 | 6 | `GetTickCount` |
| 0x5E8F7A | 6 | `GetFileAttributesA` |
| 0x5E8F80 | 6 | `GetPrivateProfileStringA` |
| 0x5E8F86 | 6 | `FindClose` |
| 0x5E8F8C | 6 | `FindNextFileA` |
| 0x5E8F92 | 6 | `FindFirstFileA` |
| 0x5E8F98 | 6 | `CreateEventA` |
| 0x5E8F9E | 6 | `MultiByteToWideChar` |
| 0x5E8FA4 | 6 | `lstrlenA` |
| 0x5E8FAA | 6 | `ReleaseMutex` |
| 0x5E8FB0 | 6 | `CreateMutexA` |
| 0x5E8FB6 | 6 | `DestroyWindow` |
| 0x5E8FBC | 6 | `MessageBoxA` |
| 0x5E8FC2 | 6 | `wvsprintfA` |
| 0x5E8FC8 | 6 | `TranslateMessage` |
| 0x5E8FCE | 6 | `DispatchMessageA` |
| 0x5E8FD4 | 6 | `PeekMessageA` |
| 0x5E8FDA | 6 | `PostMessageA` |
| 0x5E8FE0 | 6 | `ShowWindow` |
| 0x5E8FE6 | 6 | `GetKeyboardLayoutNameA` |
| 0x5E8FEC | 6 | `wsprintfA` |
| 0x5E8FF2 | 6 | `DialogBoxParamA` |
| 0x5E8FF8 | 6 | `EnableWindow` |
| 0x5E8FFE | 6 | `GetDlgItem` |
| 0x5E9004 | 6 | `SendDlgItemMessageA` |
| 0x5E900A | 6 | `SendMessageA` |
| 0x5E9010 | 6 | `EndDialog` |
| 0x5E9016 | 6 | `KillTimer` |
| 0x5E901C | 6 | `SetDlgItemTextA` |
| 0x5E9022 | 6 | `GetWindowLongA` |
| 0x5E9028 | 6 | `GetDlgItemTextA` |
| 0x5E902E | 6 | `SetWindowLongA` |
| 0x5E9034 | 6 | `SetWindowTextA` |
| 0x5E903A | 6 | `CreateWindowExA` |
| 0x5E9040 | 6 | `SetTimer` |
| 0x5E9046 | 6 | `GetWindowTextA` |
| 0x5E904C | 6 | `CallWindowProcA` |
| 0x5E9052 | 6 | `GetParent` |
| 0x5E9058 | 6 | `GetDlgCtrlID` |
| 0x5E905E | 6 | `GetWindowRect` |
| 0x5E9064 | 6 | `UpdateWindow` |
| 0x5E906A | 6 | `SetWindowPos` |
| 0x5E9070 | 6 | `ShowCursor` |
| 0x5E9076 | 6 | `GetClientRect` |
| 0x5E907C | 6 | `ReleaseDC` |
| 0x5E9082 | 6 | `GetDC` |
| 0x5E9088 | 6 | `SetForegroundWindow` |
| 0x5E908E | 6 | `FindWindowExA` |
| 0x5E9094 | 6 | `DefWindowProcA` |
| 0x5E909A | 6 | `PostQuitMessage` |
| 0x5E90A0 | 6 | `GetSystemMetrics` |
| 0x5E90A6 | 6 | `RegisterClassA` |
| 0x5E90AC | 6 | `LoadCursorA` |
| 0x5E90B2 | 6 | `LoadIconA` |
| 0x5E90B8 | 6 | `GetDeviceCaps` |
| 0x5E90BE | 6 | `RegQueryValueExA` |
| 0x5E90C4 | 6 | `RegCloseKey` |
| 0x5E90CA | 6 | `RegOpenKeyA` |
| 0x5E90D0 | 6 | `GetUserNameA` |
| 0x5E90D6 | 6 | `RegCreateKeyExA` |
| 0x5E90DC | 6 | `RegSetValueExA` |
| 0x5E90E2 | 6 | `RegOpenKeyExA` |
| 0x5E90E8 | 6 | `RegDeleteValueA` |
| 0x5E90EE | 6 | `ShellExecuteA` |
| 0x5E90F4 | 6 | `CoCreateInstance` |
| 0x5E90FA | 6 | `CoUninitialize` |
| 0x5E9100 | 6 | `CoInitialize` |
| 0x5E9106 | 6 | `DirectInputCreateA` |
| 0x5E9D34 | 3 | `nullsub_378` |
| 0x5E9D44 | 1 | `nullsub_379` |
| 0x5E9DB4 | 3 | `nullsub_380` |
| 0x5E9DC4 | 1 | `nullsub_381` |
| 0x5EA440 | 6 | `timeGetTime` |
| 0x5EA446 | 6 | `DirectPlayLobbyCreateW` |
| 0x5EA44C | 6 | `DirectPlayCreate` |
| 0x5EA452 | 6 | `VerQueryValueA` |
| 0x5EA458 | 6 | `GetFileVersionInfoA` |
| 0x5EA45E | 6 | `GetFileVersionInfoSizeA` |
| 0x5EA464 | 6 | `_BinkNextFrame@4` |
| 0x5EA46A | 6 | `_BinkBufferBlit@12` |
| 0x5EA470 | 6 | `_BinkBufferUnlock@4` |
| 0x5EA476 | 6 | `_BinkBufferLock@4` |
| 0x5EA47C | 6 | `_BinkCopyToBuffer@28` |
| 0x5EA482 | 6 | `_BinkDoFrame@4` |
| 0x5EA488 | 6 | `_BinkWait@4` |
| 0x5EA48E | 6 | `_BinkClose@4` |
| 0x5EA494 | 6 | `_BinkGetSummary@8` |
| 0x5EA49A | 6 | `_BinkBufferClose@4` |
| 0x5EA4A0 | 6 | `_BinkBufferOpen@16` |
| 0x5EA4A6 | 6 | `_BinkBufferSetDDPrimary@4` |
| 0x5EA4AC | 6 | `_BinkOpen@8` |
| 0x5EA4B2 | 6 | `_BinkSetIOSize@4` |
| 0x5EA4B8 | 6 | `_BinkSetSoundSystem@8` |
| 0x5EA4BE | 6 | `_BinkOpenMiles@4` |
| 0x5EA4C4 | 6 | `_BinkBufferCheckWinPos@12` |
| 0x5EA4CA | 6 | `_BinkBufferSetOffset@12` |
| 0x5EB6CA | 6 | `_AIL_set_digital_master_volume@8` |
| 0x5EB6D0 | 6 | `_AIL_shutdown@0` |
| 0x5EB6D6 | 6 | `_AIL_startup@0` |
| 0x5EB6DC | 6 | `_AIL_waveOutClose@4` |
| 0x5EB6E2 | 6 | `_AIL_mem_alloc_lock@4` |
| 0x5EB6E8 | 6 | `_AIL_waveOutOpen@16` |
| 0x5EB6EE | 6 | `_AIL_set_preference@8` |
| 0x5EB6F4 | 6 | `_AIL_allocate_3D_sample_handle@4` |
| 0x5EB6FA | 6 | `_AIL_3D_provider_attribute@12` |
| 0x5EB700 | 6 | `_AIL_release_3D_sample_handle@4` |
| 0x5EB706 | 6 | `_AIL_set_sample_type@12` |
| 0x5EB70C | 6 | `_AIL_init_sample@4` |
| 0x5EB712 | 6 | `_AIL_allocate_sample_handle@4` |
| 0x5EB718 | 6 | `_AIL_release_sample_handle@4` |
| 0x5EB71E | 6 | `_AIL_mem_free_lock@4` |
| 0x5EB724 | 6 | `_AIL_close_stream@4` |
| 0x5EB72A | 6 | `_AIL_delay@4` |
| 0x5EB730 | 6 | `_AIL_set_stream_volume@8` |
| 0x5EB736 | 6 | `_AIL_stream_volume@4` |
| 0x5EB73C | 6 | `_AIL_set_sample_address@12` |
| 0x5EB742 | 6 | `_AIL_set_sample_volume@8` |
| 0x5EB748 | 6 | `_AIL_set_sample_pan@8` |
| 0x5EB74E | 6 | `_AIL_set_sample_playback_rate@8` |
| 0x5EB754 | 6 | `_AIL_set_sample_loop_block@12` |
| 0x5EB75A | 6 | `_AIL_set_sample_loop_count@8` |
| 0x5EB760 | 6 | `_AIL_sample_status@4` |
| 0x5EB766 | 6 | `_AIL_start_sample@4` |
| 0x5EB76C | 6 | `_AIL_end_sample@4` |
| 0x5EB772 | 6 | `_AIL_set_3D_sample_info@8` |
| 0x5EB778 | 6 | `_AIL_set_3D_sample_volume@8` |
| 0x5EB77E | 6 | `_AIL_set_3D_position@16` |
| 0x5EB784 | 6 | `_AIL_set_3D_sample_float_distances@20` |
| 0x5EB78A | 6 | `_AIL_set_3D_sample_playback_rate@8` |
| 0x5EB790 | 6 | `_AIL_set_3D_sample_loop_block@12` |
| 0x5EB796 | 6 | `_AIL_set_3D_sample_loop_count@8` |
| 0x5EB79C | 6 | `_AIL_3D_sample_status@4` |
| 0x5EB7A2 | 6 | `_AIL_start_3D_sample@4` |
| 0x5EB7A8 | 6 | `_AIL_end_3D_sample@4` |
| 0x5EB7AE | 6 | `_AIL_set_3D_provider_preference@12` |
| 0x5EB7B4 | 6 | `_AIL_open_3D_provider@4` |
| 0x5EB7BA | 6 | `_AIL_close_3D_provider@4` |
| 0x5EB7C0 | 6 | `_AIL_enumerate_3D_providers@12` |
| 0x5EB7C6 | 6 | `_AIL_digital_handle_release@4` |
| 0x5EB7CC | 6 | `_AIL_digital_handle_reacquire@4` |
| 0x5EB7D2 | 6 | `_AIL_stream_status@4` |
| 0x5EB7D8 | 6 | `_AIL_start_stream@4` |
| 0x5EB7DE | 6 | `_AIL_set_stream_loop_count@8` |
| 0x5EB7E4 | 6 | `_AIL_open_stream@12` |
| 0x5EB7EA | 6 | `_AIL_set_stream_playback_rate@8` |
| 0x5EB7F0 | 6 | `_AIL_stream_playback_rate@4` |
| 0x5EB7F6 | 6 | `_AIL_set_stream_ms_position@8` |
| 0x5EB7FC | 6 | `_AIL_stream_ms_position@12` |
| 0x5EC070 | 50 | `??0ios@@IAE@XZ` |
| 0x5EC0A2 | 28 | `??_Gios@@UAEPAXI@Z` |
| 0x5EC0BE | 62 | `??0ios@@QAE@PAVstreambuf@@@Z` |
| 0x5EC0FC | 32 | `??0ios@@IAE@ABV0@@Z` |
| 0x5EC11C | 41 | `??1ios@@UAE@XZ` |
| 0x5EC145 | 47 | `?init@ios@@IAEXPAVstreambuf@@@Z` |
| 0x5EC174 | 59 | `??4ios@@IAEAAV0@ABV0@@Z` |
| 0x5EC1AF | 106 | `?xalloc@ios@@SAHXZ` |
| 0x5EC225 | 134 | `??0ofstream@@QAE@XZ` |
| 0x5EC2AB | 43 | `sub_5EC2AB` |
| 0x5EC2D6 | 183 | `??0ofstream@@QAE@PBDHH@Z` |
| 0x5EC38D | 137 | `??0ofstream@@QAE@H@Z` |
| 0x5EC416 | 143 | `??0ofstream@@QAE@HPADH@Z` |
| 0x5EC4A5 | 19 | `sub_5EC4A5` |
| 0x5EC525 | 60 | `?open@ofstream@@QAEXPBDHH@Z` |
| 0x5EC561 | 47 | `unknown_libname_32` |
| 0x5EC590 | 26 | `??0filebuf@@QAE@XZ` |
| 0x5EC5AA | 28 | `??_Gfilebuf@@UAEPAXI@Z` |
| 0x5EC5C6 | 31 | `??0filebuf@@QAE@H@Z` |
| 0x5EC5E5 | 72 | `??0filebuf@@QAE@HPADH@Z` |
| 0x5EC62D | 69 | `??1filebuf@@UAE@XZ` |
| 0x5EC672 | 49 | `?close@filebuf@@QAEPAV1@XZ` |
| 0x5EC6A3 | 113 | `?overflow@filebuf@@UAEHH@Z` |
| 0x5EC714 | 34 | `?sputc@streambuf@@QAEHH@Z` |
| 0x5EC736 | 144 | `?underflow@filebuf@@UAEHXZ` |
| 0x5EC7C6 | 16 | `?in_avail@streambuf@@QBEHXZ` |
| 0x5EC7D6 | 68 | `?seekoff@filebuf@@UAEJJW4seek_dir@ios@@H@Z` |
| 0x5EC81A | 216 | `?sync@filebuf@@UAEHXZ` |
| 0x5EC8F2 | 63 | `?setbuf@filebuf@@UAEPAVstreambuf@@PADH@Z` |
| 0x5EC931 | 41 | `?opfx@ostream@@QAEHXZ` |
| 0x5EC95A | 128 | `?osfx@ostream@@QAEXXZ` |
| 0x5EC9DA | 41 | `??6ostream@@QAEAAV0@PBD@Z` |
| 0x5ECA03 | 40 | `?flush@ostream@@QAEAAV1@XZ` |
| 0x5ECA2B | 54 | `??0ostream@@IAE@XZ` |
| 0x5ECA61 | 43 | `sub_5ECA61` |
| 0x5ECA8C | 101 | `??0ostream@@QAE@PAVstreambuf@@@Z` |
| 0x5ECAF1 | 110 | `??0ostream@@IAE@ABV0@@Z` |
| 0x5ECB5F | 15 | `sub_5ECB5F` |
| 0x5ECB6E | 134 | `??4ostream@@IAEAAV0@PAVstreambuf@@@Z` |
| 0x5ECBF4 | 91 | `??0ostream_withassign@@QAE@XZ` |
| 0x5ECC4F | 43 | `sub_5ECC4F` |
| 0x5ECC7A | 94 | `??0ostream_withassign@@QAE@PAVstreambuf@@@Z` |
| 0x5ECCD8 | 19 | `sub_5ECCD8` |
| 0x5ECCEB | 364 | `?writepad@ostream@@AAEAAV1@PBD0@Z` |
| 0x5ECE57 | 124 | `??6ostream@@QAEAAV0@E@Z` |
| 0x5ECED3 | 84 | `?attach@filebuf@@QAEPAV1@H@Z` |
| 0x5ECF27 | 329 | `?open@filebuf@@QAEPAV1@PBDHH@Z` |

## Excluded: never called

Don't add these: they are dead code. A function counts as called when it is reachable from the entry point or from
a pointer outside any function (vtables, callback and static init tables) through `call`/`jmp` rel32 and 32-bit
pointers anywhere in `10.5.exe`. "Referenced from" lists the callers, which are dead themselves.

- The whole `0x401000`-`0x4025xx` group (`sub_401000` to `sub_4023F0`, including `Matrix_Mult_4015D0`,
  `Matrix_PushAndMulti_401710` and `Matrix_Pop_401740`) only calls itself; its two entry points `sub_401180` and
  `sub_4023F0` are never called. A few members are called from bytes between the listed functions, so the table
  below doesn't show all of them, but none is reachable.
- Import thunks (`0x5E8F20`-`0x5EB7FC`, `jmp [IAT]`; the game calls through the IAT directly) and unused iostream
  members are library code and left out.

Without a marker (not to be added):

| Address | Size | Name | Referenced from |
|---|---|---|---|
| 0x401000 | 72 | `sub_401000` | 0x401180 |
| 0x401050 | 41 | `sub_401050` | 0x401080 |
| 0x401080 | 94 | `sub_401080` | 0x4010E0 |
| 0x4010E0 | 153 | `sub_4010E0` | 0x4010E0, 0x401180 |
| 0x401180 | 1089 | `sub_401180` | (nothing) |
| 0x401750 | 109 | `sub_401750` | 0x4023F0 |
| 0x4017C0 | 217 | `sub_4017C0` | 0x401750, 0x4017C0 |
| 0x4018A0 | 211 | `sub_4018A0` | 0x401990 |
| 0x401990 | 524 | `sub_401990` | 0x401180, 0x401990 |
| 0x401BA0 | 117 | `sub_401BA0` | 0x401BA0 |
| 0x401ED0 | 535 | `sub_401ED0` | 0x402210 |
| 0x402210 | 478 | `sub_402210` | 0x402210, 0x4023F0 |
| 0x4023F0 | 314 | `sub_4023F0` | (nothing) |
| 0x4057B0 | 421 | `sub_4057B0` | (nothing) |
| 0x427170 | 10 | `cSampleManager::sub_427170` | (nothing) |
| 0x444CE0 | 5 | `NoRefs_444CE0` | (nothing) |
| 0x454940 | 84 | `dtor_454940` | (nothing) |
| 0x4A6BE0 | 152 | `File::sub_4A6BE0` | (nothing) |
| 0x4A6DB0 | 197 | `File::sub_4A6DB0` | (nothing) |
| 0x4A8890 | 55 | `NoRefs_4A8890` | (nothing) |
| 0x4AE170 | 93 | `FreeLoader::sub_4AE170` | (nothing) |
| 0x4B80F0 | 389 | `sub_4B80F0` | (nothing) |
| 0x4BBCB0 | 389 | `get_audio_path_4BBCB0` | (nothing) |
| 0x4BBFA0 | 389 | `sub_4BBFA0` | (nothing) |
| 0x5212E0 | 7 | `goofy_thompson:NoRefs_5212E0` | (nothing) |
| 0x523060 | 830 | `sub_523060` | (nothing) |
| 0x525D50 | 53 | `sub_525D50` | 0x523060 |
| 0x529B20 | 155 | `Object_5C::sub_529B20` | (nothing) |
| 0x540A40 | 594 | `Explosion_30::sub_540A40` | (nothing) |
| 0x543900 | 118 | `ExplosionPool_7A8::sub_543900` | (nothing) |
| 0x56C170 | 95 | `jolly_poitras_0x2BC0::sub_56C170` | (nothing) |
| 0x5875A0 | 64 | `Registry::sub_5875A0` | (nothing) |
| 0x5875E0 | 59 | `Registry::sub_5875E0` | (nothing) |
| 0x587620 | 108 | `Registry::sub_587620` | (nothing) |
| 0x58E000 | 7 | `cSampleManager::sub_58E000` | (nothing) |
| 0x5B2750 | 3 | `sub_5B2750` | (nothing) |
| 0x5D98D0 | 9 | `IsFullScreen_5D98D0` | (nothing) |
| 0x5DCED0 | 25 | `sub_5DCED0` | (nothing) |

Already in `Source/` with a marker, but also dead (kept, listed for reference):

| Address | Size | Name | Referenced from |
|---|---|---|---|
| 0x407BD0 | 266 | `Trailer::sub_407BD0` | (nothing) |
| 0x43A1F0 | 18 | `Car_BC::is_bus_43A1F0` | (nothing) |
| 0x43B420 | 285 | `Car_BC::GetDoorWorldPos_43B420` | (nothing) |
| 0x441600 | 203 | `Car_BC::NoRefs_441600` | (nothing) |
| 0x454A50 | 38 | `CarInfo_808::Reload_454A50` | (nothing) |
| 0x477BA0 | 40 | `PurpleDoom::DebugLogAll_477BA0` | (nothing) |
| 0x478950 | 157 | `PurpleDoom::DebugLog_478950` | 0x477BA0 |
| 0x4A6BB0 | 44 | `File::IsCdRomDrive_4A6BB0` | 0x4B80F0, 0x4BBCB0, 0x4BBFA0 |
| 0x4B5270 | 243 | `Frontend::DrawSavedStage_4B5270` | (nothing) |
| 0x4C9240 | 94 | `PedGroup::KillEntireGroup_4C9240` | (nothing) |
| 0x4DF3E0 | 239 | `Map_0x370::sub_4DF3E0` | (nothing) |
| 0x4E0120 | 14 | `Map_0x370::sub_4E0120` | (nothing) |
| 0x4E4820 | 242 | `Map_0x370::sub_4E4820` | (nothing) |
| 0x4E7E90 | 120 | `Map_0x370::FindFirstPavementCoord_4E7E90` | (nothing) |
| 0x4FF990 | 70 | `Mike_A80::sub_4FF990` | (nothing) |
| 0x4FF9F0 | 71 | `Mike_A80::sub_4FF9F0` | (nothing) |
| 0x4FFA50 | 50 | `Mike_A80::sub_4FFA50` | (nothing) |
| 0x4FFA90 | 758 | `Mike_A80::sub_4FFA90` | (nothing) |
| 0x51E140 | 361 | `NetPlay::CreateTcpIpAddress_51E140` | (nothing) |
| 0x51E2B0 | 405 | `NetPlay::CreateModemAddress_51E2B0` | (nothing) |
| 0x51E450 | 354 | `NetPlay::CreateSerialAddress_51E450` | (nothing) |
| 0x520EA0 | 1 | `NetPlay::NoRefs_null_520EA0` | (nothing) |
| 0x5215B0 | 119 | `NetPlay::CopyConnection_5215B0` | (nothing) |
| 0x521BE0 | 155 | `NetPlay::NoRefs_Send_521BE0` | (nothing) |
| 0x521C80 | 151 | `NetPlay::NoRefs_Send_521C80` | (nothing) |
| 0x5455F0 | 8 | `Char_B4::KillPed_5455F0` | (nothing) |
| 0x56B680 | 33 | `player_stats_0xA4::GetTotalLatestScore_56B680` | 0x56C170 |
| 0x571150 | 235 | `PoliceCrew_38::SpawnFBI_nonused_571150` | (nothing) |
| 0x5872A0 | 145 | `Registry::Set_Binary_5872A0` | 0x5875E0 |
| 0x589210 | 158 | `RouteFinder::NoRefs_589210` | (nothing) |
| 0x5AA8C0 | 4 | `gtx_0x106C::GetTiles_5AA8C0` | (nothing) |
| 0x5B1170 | 5280 | `NoRefs_sub_5B1170` | (nothing) |
