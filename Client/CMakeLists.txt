project(Client)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    add_executable(Client main.cpp Client.cpp Client.h ClientUtils/ClientUtilsLin.cpp ClientUtils/ClientUtils.h)
endif ()

if (WIN32)
    add_executable(Client main.cpp Client.cpp Client.h ClientUtils/ClientUtilsWin.cpp ClientUtils/ClientUtils.h)
    target_link_libraries(Client ws2_32)
endif ()

