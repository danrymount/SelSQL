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

FILE(GLOB Engine Engine/*.cpp Engine/Headers/*.h)
FILE(GLOB Logic Logic/*.cpp Logic/Headers/*.h)
FILE(GLOB Utils Utils/*.cpp Utils/Headers/*.h)

add_executable(SelSQL main.cpp ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} Utils/ActionsUtils.cpp Utils/Headers/ActionsUtils.h)