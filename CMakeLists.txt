cmake_minimum_required(VERSION 3.14)
project(SelSQL)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src)

add_subdirectory(Tests)

add_subdirectory(Server)
add_subdirectory(Client)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
