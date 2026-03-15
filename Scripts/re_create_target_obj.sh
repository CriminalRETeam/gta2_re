# bin/bash

cd ..
source venv/bin/activate
python Scripts/generate_target_asm_for_objs.py Cranes.cpp
cd Scripts/asm
./make_objs.sh
cd ..
cd ..
