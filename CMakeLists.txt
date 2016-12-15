# This file is part of prog1-beleg
# Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
# See README for details

# Configure CMake
cmake_minimum_required(VERSION 3.3)
project(prog1_beleg)
include(FindPkgConfig)
enable_testing()

# Configure compiler
set(CMAKE_C_STANDARD 99)

# Set global include directory
include_directories(include/)

# Add GTK3
pkg_check_modules(GTK gtk+-3.0)

# Define core library target
set(CORE_SOURCE_FILES src/core.c src/common/error.c)
add_library(pb-core SHARED ${CORE_SOURCE_FILES})
target_include_directories(pb-core PUBLIC)

# Define frontend target
set(FRONTEND_SOURCE_FILES main.c)
add_executable(pb-frontend ${FRONTEND_SOURCE_FILES})
target_include_directories(pb-frontend PUBLIC ${GTK_INCLUDE_DIRS})
target_link_libraries(pb-frontend ${GTK_LIBRARIES})

# Define tests
add_executable(pb-custom-error tests/common/custom_error.c tests/test_utils.c)
add_test(pb-custom-error pb-custom-error)
target_link_libraries(pb-custom-error pb-core)