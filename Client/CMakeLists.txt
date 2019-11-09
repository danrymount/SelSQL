cmake_minimum_required(VERSION 3.15)
project(Client)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    add_executable(Client main.cpp Client.cpp Client.h ClientUtils/Lin/ClientUtilsLin.cpp ClientUtils/Lin/ClientUtilsLin.h)
endif ()

if (WIN32)
    add_executable(Client main.cpp Client.cpp Client.h ClientUtils/Win/ClientUtilsWin.cpp ClientUtils/Win/ClientUtilsWin.h)
    target_link_libraries(Client ws2_32)
endif ()

