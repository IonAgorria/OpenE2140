#Set project with C++14
project('opene2140', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++14'])

#Vars
opene2140_deps = []

#Load SDL2
sdl2_dep = dependency('sdl2')
opene2140_deps += [sdl2_dep]

#Load OpenGL
opengl_dep = dependency('gl')
opene2140_deps += [opengl_dep]

#Load spdlog
#threads_dep = subproject('spdlog')
#opene2140_deps += [sdl2_dep]

#Load Lua
lua_dep = dependency('lua')
opene2140_deps += [lua_dep]

#Load boost libs
Boost_dep = dependency('boost', modules: ['system', 'filesystem'])

#Set source files
opene2140_src = [
    'src/core/main.cpp',
    'src/core/errorpossible.cpp',
    'src/core/utils.cpp',
    'src/core/luavm.cpp',
    'src/graphics/window.cpp',
    'src/graphics/palette.cpp',
    'src/graphics/image.cpp',
    'src/math/rectangle.cpp',
    'src/math/vector2.cpp',
    'src/io/log.cpp',
    'src/io/file.cpp',
    'src/assets/asset.cpp',
    'src/assets/manager.cpp',
    'src/assets/assetimage.cpp',
    'src/assets/assetpalette.cpp',
    'src/gui/eventhandler.cpp',
    'src/core/game.cpp',
    'src/simulation/simulation.cpp',
    'src/graphics/renderer.cpp'
]

#Include headers
opene2140_incs = include_directories(
    'src',
    'lib/spdlog/include',
    'lib/stb',
)

#Create executable, set definitions and link it
opene2140_exe = executable(
    'opene2140',
    opene2140_src,
    include_directories: opene2140_incs,
    dependencies: opene2140_deps,
)
