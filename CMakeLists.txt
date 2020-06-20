# cmake
cmake_minimum_required(VERSION 2.8)

# project
set(TARGET_NAME checkers)
project(${TARGET_NAME})

# options
#add_definitions(-DSOUFLER)

# compilations
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 11)

# sources
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)
add_executable(${TARGET_NAME}
    src/checkers.cc
    src/board.cc
    src/engine.cc
    src/error.cc
)

# libs
target_link_libraries(${TARGET_NAME})
