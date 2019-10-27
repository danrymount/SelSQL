//
// Created by quiks on 13.10.2019.
//
//#ifdef __WIN32
//#include "Win/WinServer.h"
//#elif __linux
//#include "Linux/LinuxServer.h"
//#endif
//#include <iostream>
//#include <mutex>
//#include <sstream>
//#include <thread>
//#include "../src/Logic/Headers/MainLogic.h"
//#include "../src/Utils/Structures/BigResponse.h"
//#include "Exception.h"
//#include "parser.cpp"
//#define MAX_CONN 100
//#define DEBUG 0
// std::mutex m;

#include "Server.h"
int main() {
    RunServer();
    return 0;
}
