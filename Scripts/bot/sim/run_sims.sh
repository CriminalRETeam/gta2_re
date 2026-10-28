#!/bin/sh
# Builds and runs the native bot tests (no game, no wine): the steering choice and the steering calibration.
set -e
cd "$(dirname "$0")"
SRC=../../../Source/Bot
g++ -O1 -Wall -I$SRC -o steer_sim steer_sim.cpp $SRC/DriveSteer.cpp
g++ -O1 -Wall -I$SRC -o calib_sim calib_sim.cpp $SRC/CalibrateCore.cpp
g++ -O1 -Wall -I$SRC -o shoot_police_crowd_sim shoot_police_crowd_sim.cpp $SRC/ShootPoliceCrowd.cpp
g++ -O1 -Wall -I$SRC -o walk_car_jump_sim walk_car_jump_sim.cpp $SRC/WalkCarJump.cpp
g++ -O1 -Wall -I$SRC -o car_switch_sim car_switch_sim.cpp $SRC/CarSwitch.cpp
g++ -O1 -Wall -I$SRC -o walk_route_sim walk_route_sim.cpp
rc=0
./steer_sim || rc=1
./calib_sim || rc=1
./shoot_police_crowd_sim || rc=1
./walk_car_jump_sim || rc=1
./car_switch_sim || rc=1
./walk_route_sim || rc=1
exit $rc
