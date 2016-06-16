## WIP ##
cmake_minimum_required (VERSION 2.8)
project (dpixel)

# The version number
set (dpixel_VERSION_MAJOR 0)
set (dpixel_VERSION_MINOR 1)

set(DPIXEL_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include(${QT_USE_FILE})

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

if(Qt5_FOUND)
    message("Found Qt version ${Qt5Core_VERSION}")
endif()

qt5_wrap_ui( UI_HEADERS MainWindow.ui )
qt5_wrap_ui( UI_HEADERS "${UI_HEADERS}" CheckUpscaleWindow.ui )
qt5_add_resources( QRCS gui.qrc )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

file(GLOB SOURCES  "*.cpp" "*.c")
file(GLOB SOURCES ${SOURCES} "Filters/*.cpp" "Filters/*.c")
file(GLOB SOURCES ${SOURCES} "Depixel/*.cpp")
file(GLOB SOURCES ${SOURCES} "ImageOperations/*.cpp")
file(GLOB SOURCES ${SOURCES} "Math/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${UI_HEADERS} ${QRCS})
#add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets )

message( "Source list: " ${SOURCES})
message( "Ui list: " ${UI_HEADERS})
message( "Resource list: " ${QRCS})
