cmake_minimum_required(VERSION 3.14)
project(SelSQL)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test/googletest-master)
add_subdirectory(src)
include_directories(test/googletest-master/googletest/include)
include_directories(test/googletest-master/googlemock/include)

