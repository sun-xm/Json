cmake_minimum_required(VERSION 3.10)
project(Json)

if(MSVC AND MSVC_VERSION GREATER_EQUAL 1910)
    add_compile_options(/permissive-)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)

enable_testing()
add_subdirectory(Test Test)
add_subdirectory(Sample Sample)