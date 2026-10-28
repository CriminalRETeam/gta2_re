#!/bin/sh
# Builds and runs the native bot tests (no game, no wine): the steering choice and the steering calibration.
set -e
cd "$(dirname "$0")"
SRC=../../../Source/Bot
g++ -O1 -Wall -I$SRC -o steer_sim steer_sim.cpp $SRC/DriveSteer.cpp $SRC/LocalPlan.cpp $SRC/GridRoute.cpp
g++ -O1 -Wall -I$SRC -o calib_sim calib_sim.cpp $SRC/CalibrateCore.cpp
g++ -O1 -Wall -I$SRC -o shoot_police_crowd_sim shoot_police_crowd_sim.cpp $SRC/ShootPoliceCrowd.cpp
g++ -O1 -Wall -I$SRC -o walk_car_jump_sim walk_car_jump_sim.cpp $SRC/WalkCarJump.cpp
g++ -O1 -Wall -I$SRC -o car_switch_sim car_switch_sim.cpp $SRC/CarSwitch.cpp
g++ -O1 -Wall -I$SRC -o walk_route_sim walk_route_sim.cpp
g++ -O1 -Wall -I$SRC -o local_plan_sim local_plan_sim.cpp $SRC/LocalPlan.cpp $SRC/GridRoute.cpp $SRC/DriveSteer.cpp
g++ -O1 -Wall -I$SRC -o blast_avoid_sim blast_avoid_sim.cpp $SRC/BlastAvoid.cpp
g++ -O1 -Wall -I$SRC -o turn_around_sim turn_around_sim.cpp $SRC/TurnAround.cpp
g++ -O1 -Wall -I$SRC -o local_plan_real_sim local_plan_real_sim.cpp $SRC/LocalPlan.cpp $SRC/GridRoute.cpp $SRC/DriveSteer.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o object_touch_sim object_touch_sim.cpp $SRC/ObjectTouch.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o pickup_reach_sim pickup_reach_sim.cpp $SRC/PickupReach.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o drop_off_sim drop_off_sim.cpp $SRC/DropOff.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o jam_switch_sim jam_switch_sim.cpp $SRC/JamSwitch.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o car_intercept_sim car_intercept_sim.cpp $SRC/CarIntercept.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o car_pick_sim car_pick_sim.cpp $SRC/CarPickRule.cpp $SRC/CarIntercept.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o self_defence_sim self_defence_sim.cpp $SRC/SelfDefence.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o horn_sim horn_sim.cpp $SRC/Horn.cpp
g++ -O1 -Wall -std=c++11 -I$SRC -o first_road_sim first_road_sim.cpp $SRC/CarPickRule.cpp $SRC/CarIntercept.cpp
g++ -O1 -Wall -I$SRC -o grid_route_sim grid_route_sim.cpp $SRC/GridRoute.cpp
rc=0
./steer_sim -local || rc=1
./calib_sim || rc=1
./shoot_police_crowd_sim || rc=1
./walk_car_jump_sim || rc=1
./car_switch_sim || rc=1
./walk_route_sim || rc=1
./local_plan_sim || rc=1
./blast_avoid_sim || rc=1
./local_plan_real_sim || rc=1
./turn_around_sim || rc=1
./object_touch_sim || rc=1
./pickup_reach_sim || rc=1
./drop_off_sim || rc=1
./jam_switch_sim || rc=1
./car_intercept_sim || rc=1
./car_pick_sim || rc=1
./self_defence_sim || rc=1
./horn_sim || rc=1
./first_road_sim || rc=1
./grid_route_sim || rc=1
exit $rc
