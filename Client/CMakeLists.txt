cmake_minimum_required(VERSION 3.15)
project(Client)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    add_executable(Client main.cpp Linux/LinuxClient.cpp Linux/LinuxClient.h main.cpp Exception.h)
endif ()

if (WIN32)
    add_executable(Client main.cpp Client.cpp Client.h)
    target_link_libraries(Client ws2_32)
endif ()

