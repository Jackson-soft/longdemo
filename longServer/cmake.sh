#!/bin/bash
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

make