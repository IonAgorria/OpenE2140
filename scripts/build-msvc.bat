@echo off

rem Create/clean out
rmdir /Q /S build
mkdir build

rem Generate build files
cd build
cmake ..

#MAKE!
cmake --build . --target opene2140 -- -j 2
