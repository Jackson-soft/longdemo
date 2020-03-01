#!/bin/bash
set -u

dir="build"

if [ -d $dir ]; then
    ninja -C $dir -j 6
    exit 0
fi

mkdir -p $dir

cmake -B$dir -H. -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

ninja -C $dir -j 6

lnFile="compile_commands.json"

if [ ! -f $lnFile ]; then
    ln -s $dir/$lnFile .
fi
