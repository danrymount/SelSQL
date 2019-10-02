cmake_minimum_required(VERSION 3.14)

project(Tests)

SET(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

BISON_TARGET(Parser ../src/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Lexer ../src/Parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB Engine ../src/Engine/*.cpp ../src/Engine/Headers/*.h)
FILE(GLOB Logic ../src/Logic/*.cpp ../src/Logic/Headers/*.h)
FILE(GLOB Utils ../src/Utils/*.cpp ../src/Utils/Headers/*.h)

add_executable(Tests main_test.cpp ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} TestClass.cpp)
enable_testing()
target_link_libraries(Tests gtest gtest_main)