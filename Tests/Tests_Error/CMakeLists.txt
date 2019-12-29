project(Tests_Error)

SET(CMAKE_CXX_STANDARD 17)

FILE(GLOB Utils ${SOURCE_DIR}/Utils/*.cpp ${SOURCE_DIR}/Utils/Headers/*.h)

link_libraries(stdc++fs)
if (UNIX)
    add_executable(Tests_Error main_test.cpp TestClass.cpp
            ../Headers/TestUtils.h . ../../Client/Client.cpp ../../Client/ClientUtils/ClientUtilsLin.cpp ../RQG/Generator.h ../RQG/Generator.cpp)
    link_libraries(stdc++fs)
endif ()

if (WIN32)
    add_executable(Tests_Error main_test.cpp TestClass.cpp
            ../Headers/TestUtils.h . ../../Client/Client.cpp ../../Client/ClientUtils/ClientUtilsWin.cpp ../RQG/Generator.h ../RQG/Generator.cpp)
    target_link_libraries(Tests_Error ws2_32)
endif ()

enable_testing()
target_link_libraries(Tests_Error gtest gtest_main)