#!/bin/sh
# Builds and runs the native bot tests (no game, no wine): the steering choice and the steering calibration.
set -e
cd "$(dirname "$0")"
SRC=../../../Source/Bot
g++ -O1 -Wall -I$SRC -o steer_sim steer_sim.cpp $SRC/DriveSteer.cpp
g++ -O1 -Wall -I$SRC -o calib_sim calib_sim.cpp $SRC/CalibrateCore.cpp
g++ -O1 -Wall -I$SRC -o kite_sim kite_sim.cpp $SRC/KiteCops.cpp
rc=0
./steer_sim || rc=1
./calib_sim || rc=1
./kite_sim || rc=1
exit $rc
