cmake_minimum_required(VERSION 3.0)
project(Json)

set(CMAKE_CXX_STANDARD 11)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)
file(GLOB SRC_CPP ${SRC_DIR}/*.cpp)

include_directories(${SRC_DIR})
enable_testing()
add_subdirectory(Test Test)
add_subdirectory(Sample Sample)