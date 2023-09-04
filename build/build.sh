#!/bin/bash
set -eu

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cppcheck ../src --project=compile_commands.json --std=c++20 --language=c++ --suppress='*:_deps/*' -j4 -v -q
cmake --build .
ctest
