cmake_minimum_required(VERSION 3.0)
project(Runner)

set(CMAKE_CXX_STANDARD 11)

include_directories(Runner Source)
add_executable(${PROJECT_NAME}
               Runner/main.cpp
               Runner/Person.cpp
               Runner/JPrint.cpp
               Source/JParser.cpp)