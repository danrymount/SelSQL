cmake_minimum_required(VERSION 3.14)
project(SelSQL)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src)

add_subdirectory(Tests/Tests_CRUD)
add_subdirectory(Tests/Tests_JUI)
add_subdirectory(Tests/Tests_Stress)
add_subdirectory(Tests/Tests_Thread)
add_subdirectory(Tests/Tests_Error)

add_subdirectory(Server)
add_subdirectory(Client)


add_subdirectory(lib/googletest-master)
