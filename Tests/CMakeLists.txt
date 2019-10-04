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

SET(SOURCE_DIR ../src)

FILE(GLOB Engine ${SOURCE_DIR}/Engine/*.cpp ${SOURCE_DIR}/Engine/Headers/*.h)
FILE(GLOB Logic ${SOURCE_DIR}/Logic/*.cpp ${SOURCE_DIR}/Logic/Headers/*.h ${SOURCE_DIR}/Logic/Actions/*.cpp ${SOURCE_DIR}/Logic/Actions/Headers/*.h)
FILE(GLOB Utils ${SOURCE_DIR}/Utils/*.cpp ${SOURCE_DIR}/Utils/Headers/*.h)
FILE(GLOB Parser ${SOURCE_DIR}/Parser/*.cpp ${SOURCE_DIR}/Parser/Headers/**.h)

add_executable(Tests main_test.cpp ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} ${Parser} TestClass.cpp
        Headers/TestUtils.h)
enable_testing()
target_link_libraries(Tests gtest gtest_main)