cmake_minimum_required(VERSION 3.0)
project(Runner)

set(CMAKE_CXX_STANDARD 11)

include_directories(Runner Source)
add_executable(${PROJECT_NAME}
               Runner/main.cpp
               Runner/Person.cpp
               Runner/JPrint.cpp
               Source/JParser.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/person.json" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")