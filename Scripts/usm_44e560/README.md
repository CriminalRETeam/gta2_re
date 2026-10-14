# UpdateStateMachine_44E560 experiment kit

`CarAI_78::UpdateStateMachine_44E560` is still WIP. This kit reproduces the closest state found so far without
touching `Source/`: the repo version plus `experiment.patch`, which holds the changes that only make sense
together with the WIP (see `docs/match_attempts.md`, "UpdateStateMachine_44E560 (Oct 14)").

```bash
python3 Scripts/inline_budget/setup.py              # once: build_vc6/inline_c2
Scripts/usm_44e560/score.sh                         # stock compiler: 186
python3 Scripts/usm_44e560/c2cap.py 0               # build_vc6/inline_c2_cap0 (stock 0x400 limit)
python3 Scripts/usm_44e560/c2pred.py P644 644       # needs keystone-engine
Scripts/usm_44e560/score.sh build_vc6/inline_c2_P644   # 0: every byte matches
```

- `experiment.patch`: `Fix16` `operator-/+/*` without their named local and `operator+=`, all at their current
  front-end sizes (the `(void)0;` keep the sizes; they stand for whatever the original had there), the two
  `char.cpp` rewrites those operators need to keep matching, an unused `s32 unused;` in
  `Sprite::set_xyz_lazy_420600` and `Fix16::operator<` (+35 optimizer entries), the two cluster-C spellings,
  and 29 `(void)0;` at the end of the function (the original is 58 front-end nodes bigger).
  With these headers and `char.cpp`, a full build still verifies all MATCH functions.
- `c2cap.py K`: C2 that tracks 0x400 + K entries per function (the global optimizer limit).
- `c2pred.py NAME LINE`: C2 that leaves the 0 of the compare at IL line `LINE` as an immediate. Line 644 is
  `if (pOwner != 0)` after `FindDriverInState_4748A0` in follow_behind; that single compare is the whole
  remaining difference.
