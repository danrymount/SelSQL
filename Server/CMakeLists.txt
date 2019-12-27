project(Server)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/../src/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/../src/Parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})


SET(SOURCE_DIR ../src/)

FILE(GLOB Engine ${SOURCE_DIR}/Engine/*.cpp ${SOURCE_DIR}/Engine/Headers/*.h)
FILE(GLOB Logic ${SOURCE_DIR}/Logic/Actions/*.cpp ${SOURCE_DIR}/Logic/Actions/Headers/*.h)
FILE(GLOB Utils ${SOURCE_DIR}/Utils/*.cpp ${SOURCE_DIR}/Utils/Headers/*.h)
FILE(GLOB Parser ${SOURCE_DIR}/Parser/*.cpp ${SOURCE_DIR}/Parser/Headers/**.h)
FILE(GLOB ServerLin *.cpp *.h ServerUtils/*.h ServerUtils/ServerUtilsLin.cpp)
FILE(GLOB ServerWin *.cpp *.h ServerUtils/*.h ServerUtils/ServerUtilsWin.cpp)
if (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_executable(Server ${ServerLin} ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} ${Parser})

endif ()

if (WIN32)
    add_executable(Server main.cpp Server.cpp Server.h ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} ${Parser} MainServer.cpp MainServer.h ServerUtils/ServerUtilsWin.cpp ServerUtils/ServerUtils.h)
    target_link_libraries(Server ws2_32)
endif ()
