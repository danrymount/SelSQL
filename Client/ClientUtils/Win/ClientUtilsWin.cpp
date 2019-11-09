//
// Created by User on 10.11.2019.
//
#include "ClientUtilsWin.h"
#include <winsock2.h>

void ClientUtilsWin::startClient() {
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
}

void ClientUtilsWin::close(int server_connection) { closesocket(server_connection); }

void ClientUtilsWin::clientClose(int server_connection, int client_socket) {
    closesocket(server_connection);
    closesocket(client_socket);
    WSACleanup();
}