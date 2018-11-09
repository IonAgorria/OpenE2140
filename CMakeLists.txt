cmake_minimum_required(VERSION 3.1)
project(opene2140)

# Set C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14 -rdynamic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# Fixes SDL2 trailing space bug
if(POLICY CMP0004)
    cmake_policy(SET CMP0004 OLD)
endif()

# Get sources
set(LINK_LIBRARIES)
include_directories("src")
set(SOURCE_FILES
        src/core/config.h
        src/core/main.cpp
        src/core/errorpossible.cpp
        src/core/utils.cpp
        src/core/luavm.cpp
        src/graphics/window.cpp
        src/graphics/palette.cpp
        src/graphics/image.cpp
        src/math/rectangle.cpp
        src/math/vector2.cpp
        src/io/log.cpp
        src/io/file.cpp
        src/assets/asset.cpp
        src/assets/manager.cpp
        src/assets/assetimage.cpp
        src/assets/assetpalette.cpp
        src/gui/eventhandler.cpp
        src/core/game.cpp
        src/simulation/simulation.cpp
        src/graphics/renderer.cpp src/graphics/renderer.h)

# Load SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(LINK_LIBRARIES ${LINK_LIBRARIES} ${SDL2_LIBRARIES})

# Load GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LINK_LIBRARIES ${LINK_LIBRARIES} ${GLEW_LIBRARIES})

# Load OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LINK_LIBRARIES ${LINK_LIBRARIES} ${OPENGL_gl_LIBRARY})

# Load spdlog
add_subdirectory(lib/spdlog REQUIRED)
find_package(Threads)
set(LINK_LIBRARIES ${LINK_LIBRARIES} spdlog ${CMAKE_THREAD_LIBS_INIT})

# Load Lua
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
set(LINK_LIBRARIES ${LINK_LIBRARIES} ${LUA_LIBRARIES})

# Load boost libs
set(HAS_BOOST 0)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66.0) # Boost stacktrace was introduced in 1.65 but is bugged
if(Boost_FOUND)
    set(HAS_BOOST 1)
    # Add boost libraries if found
    include_directories(${Boost_INCLUDE_DIRS})
    set(LINK_LIBRARIES ${LINK_LIBRARIES} boost_system boost_filesystem)
    # dl is required for dladdr in stacktrace,
    set(LINK_LIBRARIES ${LINK_LIBRARIES} dl)
endif()

# Create executable, set definitions and link it
add_executable(opene2140 ${SOURCE_FILES})
target_compile_definitions(opene2140 PRIVATE HAS_BOOST=${HAS_BOOST})
target_link_libraries(opene2140 ${LINK_LIBRARIES})