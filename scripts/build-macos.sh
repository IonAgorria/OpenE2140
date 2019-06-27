#!/bin/sh
set -e

#Clean out
rm -rf build/*

#Generate build files
meson --prefix=$PWD/build/opene2140.app --bindir=Contents/MacOS build

#Copy extra files
cp Info.plist build/Info.plist

#MAKE!
cd build
ninja
