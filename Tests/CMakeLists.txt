cmake_minimum_required(VERSION 3.14)

project(Tests)

SET(CMAKE_CXX_STANDARD 17)





link_libraries(stdc++fs)
if (UNIX)
    add_executable(Tests main_test.cpp TestClass.cpp
            Headers/TestUtils.h . ../Client/Client.cpp ../Client/ClientUtils/Lin/ClientUtilsLin.cpp)
    link_libraries(stdc++fs)
endif ()

if (WIN32)
    add_executable(Tests main_test.cpp TestClass.cpp
            Headers/TestUtils.h . ../Client/Client.cpp ../Client/ClientUtils/Win/ClientUtilsWin.cpp)
    target_link_libraries(Tests ws2_32)
endif ()

enable_testing()
target_link_libraries(Tests gtest gtest_main)