cmake_minimum_required(VERSION 3.20)
project(S VERSION 0.0.1 LANGUAGES CXX)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include every important files
include_directories(${PROJECT_SOURCE_DIR}/src)

# Lists src files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Creates executable
add_executable(s ${SOURCES})