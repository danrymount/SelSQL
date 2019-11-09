//
// Created by User on 10.11.2019.
//

#include "ServerUtilsLin.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void ServerUtilsLin::startServer() {}

void ServerUtilsLin::closeServer(int server_socket) { shutdown(server_socket, SHUT_RDWR); }