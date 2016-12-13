# This file is part of prog1-beleg
# Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
# See README for details

# Configure CMake
cmake_minimum_required(VERSION 3.3)
project(prog1_beleg)
include(FindPkgConfig)

# Configure compiler
set(CMAKE_C_STANDARD 99)

# Add GTK3
pkg_check_modules(GTK gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})

# Set sources and compile
set(SOURCE_FILES main.c)
add_executable(prog1_beleg ${SOURCE_FILES})

# Link against dependencies
target_link_libraries(prog1_beleg ${GTK_LIBRARIES})