# This file is part of prog1-beleg
# Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
# See README for details

# Configure CMake
cmake_minimum_required(VERSION 3.3)
project(prog1_beleg)
include(FindPkgConfig)
enable_testing()

# Configure compiler
add_definitions(-Wall)
set(CMAKE_C_STANDARD 99)

# Set global include directory
include_directories(include/)

# Add GTK3
pkg_check_modules(GTK gtk+-3.0)

# Define core library target
set(CORE_SOURCE_FILES
        src/core.c
        src/common/error.c
        src/data/database.c
        src/data/query.c
        src/data/image.c
        src/data/material_item.c
        src/logic/csv_export.c
        src/logic/items_query.c
)
add_library(pb-core SHARED ${CORE_SOURCE_FILES})
target_include_directories(pb-core PUBLIC)
target_link_libraries(pb-core sqlite3)

# Define frontend target
set(FRONTEND_SOURCE_FILES
        src/frontend/main.c
        src/frontend/error.c
        src/frontend/globals.c
        src/frontend/welcome.c
)
add_executable(pb-frontend ${FRONTEND_SOURCE_FILES})
target_include_directories(pb-frontend PUBLIC ${GTK_INCLUDE_DIRS})
target_link_libraries(pb-frontend ${GTK_LIBRARIES} pb-core)
configure_file(ui/prog1-beleg.glade prog1-beleg.glade COPYONLY)

# Define tests
add_executable(test-custom-error tests/common/custom_error.c tests/test_utils.c)
add_test(test-custom-error test-custom-error)
target_link_libraries(test-custom-error pb-core)

add_executable(test-database tests/data/database.c tests/test_utils.c)
add_test(test-database test-database)
target_link_libraries(test-database pb-core)

add_executable(test-image tests/data/image.c tests/test_utils.c)
add_test(test-image test-image)
target_link_libraries(test-image pb-core)

add_executable(test-material-item tests/data/material_item.c tests/test_utils.c)
add_test(test-material-item test-material-item)
target_link_libraries(test-material-item pb-core)

add_executable(test-items-query tests/logic/items_query.c tests/test_utils.c)
add_test(test-items-query test-items-query)
target_link_libraries(test-items-query pb-core)