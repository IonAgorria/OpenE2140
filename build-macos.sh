#!/bin/sh
set -e
#NOT TESTED

#Create/clean out
mkdir -p build
rm -rf build/*

#Generate build files
cd build
cmake –G "Xcode" ..

#MAKE!
cmake --build . --target opene2140
