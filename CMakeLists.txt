cmake_minimum_required(VERSION 3.15)
project(aoc_bench CXX)

# Require AOC_DAY to be defined
if(NOT DEFINED AOC_DAY)
    message(FATAL_ERROR "AOC_DAY is not defined. Please specify with -DAOC_DAY=<day>.")
endif()

# Set the source directory based on AOC_DAY
set(SRC_DIR "${CMAKE_SOURCE_DIR}/${AOC_DAY}")

# Set the source file
set(SRC_FILE "${SRC_DIR}/main.cpp")

add_executable(aoc_bench "${SRC_FILE}")

# Check the compiler is clang, warn if not
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING "CMAKE_CXX_COMPILER is not set to clang. Clang is used for AoCBench, so this may lead to unexpected results.")
else()
    # Strip off the inclusive O3 from Release flags and set custom flags
    string(REGEX REPLACE "\-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    # Set custom optimisations incl LTO, and set C++23
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -static -O3 -march=native -mtune=native -flto")
    # Ensure static linking (and LTO)
    set_target_properties(aoc_bench PROPERTIES LINK_SEARCH_START_STATIC ON)
    set_target_properties(aoc_bench PROPERTIES LINK_SEARCH_END_STATIC ON)
    target_link_options(aoc_bench PRIVATE -static -flto)
endif()
