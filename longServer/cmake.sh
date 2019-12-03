#!/bin/bash

dir="build"

if [ ! -d $dir ]; then
    mkdir $dir
fi

cd $dir

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

make
