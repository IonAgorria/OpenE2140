#!/bin/sh
set -e

#Clean out
rm -rf build/*

#Generate build files
meson build

#MAKE!
cd build
ninja
