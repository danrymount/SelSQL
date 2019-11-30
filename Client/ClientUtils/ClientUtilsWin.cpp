//
// Created by User on 10.11.2019.
//
#include <winsock2.h>
#include "ClientUtils.h"

void ClientUtils::startClient() {
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
}

void ClientUtils::close(int server_connection) { closesocket(server_connection); }

void ClientUtils::clientClose(int server_connection, int client_socket) {
    closesocket(server_connection);
    closesocket(client_socket);
    WSACleanup();
}