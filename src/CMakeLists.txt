project(SelSQL)

SET(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h COMPILE_FLAGS -t)
FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/Parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB Engine Engine/*.cpp Engine/Headers/*.h)
FILE(GLOB Logic Logic/*.cpp Logic/Headers/*.h)
FILE(GLOB Utils Utils/*.cpp Utils/Headers/*.h)

SET(SOURCE_DIR ./)

FILE(GLOB Engine ${SOURCE_DIR}/Engine/*.cpp ${SOURCE_DIR}/Engine/Headers/*.h)
FILE(GLOB Logic ${SOURCE_DIR}/Logic/Actions/*.cpp ${SOURCE_DIR}/Logic/Actions/Headers/*.h)
FILE(GLOB Utils ${SOURCE_DIR}/Utils/*.cpp ${SOURCE_DIR}/Utils/Headers/*.h)
FILE(GLOB Parser ${SOURCE_DIR}/Parser/*.cpp ${SOURCE_DIR}/Parser/Headers/**.h)

link_libraries(stdc++fs)
add_executable(SelSQL main.cpp ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} ${Parser})