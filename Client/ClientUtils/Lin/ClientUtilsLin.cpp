//
// Created by User on 10.11.2019.
//

#include "ClientUtilsLin.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void ClientUtils::startClient() {}

void ClientUtils::close(int server_connection) { shutdown(server_connection, SHUT_RD); }

void ClientUtils::clientClose(int server_connection, int client_socket) {
    shutdown(client_socket, SHUT_RDWR);
    shutdown(server_connection, SHUT_RDWR);
}