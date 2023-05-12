#!/bin/bash
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cmake --build .
