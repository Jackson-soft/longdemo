#!/bin/bash
set -u

dir="build"

if [ ! -d $dir ]; then
    mkdir -p $dir
fi

cmake -B$dir -H. -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

ninja -C $dir -j 6

lnFile="compile_commands.json"

if [ ! -f $lnFile ]; then
    ln -s $dir/$lnFile .
fi
