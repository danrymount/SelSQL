//
// Created by quiks on 23.10.2019.
//

#ifndef SELSQL_SERVER_H
#define SELSQL_SERVER_H

#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include "Exception.h"
#include "Server.h"

#define MAX_CONN 100
#define DEBUG 1

void RunServer();
int ListenClient(int id, Server* server);
std::string ExecuteRequest(const std::string& request, int id);

#endif  // SELSQL_SERVER_H
