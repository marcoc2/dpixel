cmake_minimum_required (VERSION 2.8)
project (dpixel)

# The version number
set (dpixel_VERSION_MAJOR 0)
set (dpixel_VERSION_MINOR 1)

find_package(Qt5 COMPONENTS Widgets OpenGL)

if(Qt5_FOUND)
    message("Found Qt version ${Qt5Core_VERSION}")
endif()

add_executable(Main main.cpp)
