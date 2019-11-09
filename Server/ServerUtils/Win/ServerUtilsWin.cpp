//
// Created by User on 10.11.2019.
//
#include "ServerUtilsWin.h"
#include <winsock2.h>
void ServerUtilsWin::startServer() {
    WSACleanup();
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
}

void ServerUtilsWin::closeServer(int server_socket) { WSACleanup(); }