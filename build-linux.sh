#!/bin/sh
set -e

#Create/clean out
mkdir -p out
rm -rf out/*

#Link assets dir just in case the executable is run inside
if [ -d assets ]; then
    ln -s assets out/assets
fi

#Generate build files
cd out
cmake -G "Unix Makefiles" ..

#MAKE!
cmake --build . --target opene2140
