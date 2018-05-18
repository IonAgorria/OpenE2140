#!/bin/sh
set -e

#Create/clean out
mkdir -p out
rm -rf out/*

#Generate build files
cd out
cmake -G "Unix Makefiles" ..

#MAKE!
cmake --build . --target opene2140
