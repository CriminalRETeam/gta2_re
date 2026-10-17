# UpdateStateMachine_44E560 experiment kit

`CarAI_78::UpdateStateMachine_44E560` is still WIP. This kit reproduces the closest state found so far without
touching `Source/`: the repo version plus `experiment.patch`, which holds the changes that only make sense
together with the WIP (see `docs/match_attempts.md`, "UpdateStateMachine_44E560 (Oct 14)").

```bash
python3 Scripts/inline_budget/setup.py              # once: build_vc6/inline_c2
Scripts/usm_44e560/score.sh                         # stock compiler: 186
python3 Scripts/usm_44e560/c2cap.py 0               # build_vc6/inline_c2_cap0 (stock 0x400 limit)
python3 Scripts/usm_44e560/c2pred.py P659 659       # needs keystone-engine
Scripts/usm_44e560/score.sh build_vc6/inline_c2_P659   # 0: every byte matches
```

- `experiment.patch`: `NOT_IMPLEMENTED` removed, an unused `s32 unused;` in `Sprite::set_xyz_lazy_420600` and
  `Fix16::operator<` (+35 optimizer entries, 1 scope marker per expansion, 0 nodes), and the two cluster-C
  spellings at the left alongside site. The compiled-out asserts in `Source/` (29 in the function, two per
  `Fix16` operator) are part of the match: each costs 2 front-end nodes in the inline budget and nothing else.
  With these headers a full build verifies all MATCH functions.
- `c2cap.py K`: C2 that tracks 0x400 + K entries per function (the global optimizer limit).
- `c2pred.py NAME LINE`: C2 that leaves the 0 of the compare at IL line `LINE` as an immediate. Line 659 is
  `if (pOwner != 0)` after `FindDriverInState_4748A0` in follow_behind (relative to the function's first line, in
  the patched file); that single compare is the whole remaining difference.
