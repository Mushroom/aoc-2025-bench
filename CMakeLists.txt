cmake_minimum_required(VERSION 3.15)
project(aoc-2025-bench CXX)

# Require AOC_DAY to be defined
if(NOT DEFINED AOC_DAY)
    message(FATAL_ERROR "AOC_DAY is not defined. Please specify with -DAOC_DAY=<day>.")
endif()

# Ensure the compiler is clang
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING "CMAKE_CXX_COMPILER is not set to clang. Clang is used for AoCBench, so this may lead to unexpected results.")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -static -O3 -march=native -mtune=native")
endif()

# Set the source directory based on AOC_DAY
set(SRC_DIR "${CMAKE_SOURCE_DIR}/${AOC_DAY}")

# Set the source file
set(SRC_FILE "${SRC_DIR}/main.cpp")

add_executable(aoc_bench "${SRC_FILE}")
