cmake_minimum_required(VERSION 3.10)
project(Test)

link_libraries(JLib)

add_executable(JStr  ${SRC_CPP} JStr.cpp)
add_executable(JInt  ${SRC_CPP} JInt.cpp)
add_executable(JNum  ${SRC_CPP} JNum.cpp)
add_executable(JBool ${SRC_CPP} JBool.cpp)
add_executable(JTime ${SRC_CPP} JTime.cpp)
add_executable(JArr  ${SRC_CPP} JArr.cpp)
add_executable(JObj  ${SRC_CPP} JObj.cpp)
add_executable(JVar  ${SRC_CPP} JVar.cpp)

add_test(NAME JStr  COMMAND JStr)
add_test(NAME JInt  COMMAND JInt)
add_test(NAME JNum  COMMAND JNum)
add_test(NAME JBool COMMAND JBool)
add_test(NAME JTime COMMAND JTime)
add_test(NAME JArr  COMMAND JArr)
add_test(NAME JObj  COMMAND JObj)
add_test(NAME JVar  COMMAND JVar)