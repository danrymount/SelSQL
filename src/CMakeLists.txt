cmake_minimum_required(VERSION 3.14)

project(SelSQL)

SET(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/Parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(SelSQL main.cpp ${FLEX_Lexer_OUTPUTS}
        Logic/Headers/APILogic.h Logic/MainLogic.cpp Logic/Headers/MainLogic.h
        Utils/ParserUtils.cpp Utils/Headers/ParserUtils.h Utils/CommonUtils.cpp Utils/Headers/CommonUtils.h
        Tests/Tests.cpp)
target_link_libraries(SelSQL gtest gtest_main)