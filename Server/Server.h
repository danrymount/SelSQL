//
// Created by quiks on 23.10.2019.
//

#ifndef SELSQL_SERVER_H
#define SELSQL_SERVER_H

#ifdef __WIN32
#include "Win/WinServer.h"
#elif __linux
#include "Linux/LinuxServer.h"
#endif
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include "../src/Logic/Headers/MainLogic.h"
#include "../src/Utils/Structures/BigResponse.h"
#include "Exception.h"

#define MAX_CONN 1
#define DEBUG 1

void runServer();
int ListenClient(int id, Server* server);
std::string executeRequest(std::string request);

#endif  // SELSQL_SERVER_H
