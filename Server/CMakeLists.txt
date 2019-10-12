cmake_minimum_required(VERSION 3.15)
project(Server)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    add_executable(Server main.cpp Linux/LinuxServer.h Linux/LinuxServer.cpp)
endif ()

if (WIN32)
    add_executable(Server main.cpp Win/WinServer.cpp Win/WinServer.h)
    target_link_libraries(Server ws2_32)
endif ()
