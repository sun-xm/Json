cmake_minimum_required(VERSION 3.10)
project(Sample)
add_executable(${PROJECT_NAME} ${SRC_CPP} main.cpp Person.cpp JPrint.cpp)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(WORK_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
else()
    set(WORK_DIR "${CMAKE_CURRENT_BINARY_DIR}/")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/person.json" "${WORK_DIR}")