//
// Created by User on 10.11.2019.
//
#include "ServerUtilsWin.h"
#include <winsock2.h>
void ServerUtils::startServer() {
    WSACleanup();
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
}

void ServerUtils::closeServer(int server_socket) { WSACleanup(); }