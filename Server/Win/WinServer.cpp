//
// Created by quiks on 13.10.2019.
//

#include "WinServer.h"
#include <iostream>

Server::Server() {
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Unable to create server socket" << std::endl;
        throw ServerException();
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(18666);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Unable to bind server socket" << std::endl;
        throw ServerException();
    }

    if (listen(server_socket, 5)) {
        std::cerr << "Unable to listen" << std::endl;
        throw ServerException();
    }
}
int Server::ListenSocket() {
    communication_socket = accept(server_socket, NULL, NULL);
    if (communication_socket < 0) {
        std::cerr << "Unable to create communication socket" << std::endl;
        throw ServerException();
    }
    /*читаем данные из сокета*/

    for (;;) {
        memset(recieved_message, 0, sizeof(char) * MESSAGE_SIZE);
        /*следует помнить, что данные поступают неравномерно*/
        int rc = recv(communication_socket, recieved_message, MESSAGE_SIZE - 1, 0);
        if (rc == MESSAGE_SIZE - 1) {
            return 0;
        }
        if (rc < 0) {
            /*чтение может быть прервано системным вызовом, это нормально*/
            if (errno == EINTR)
                continue;
            std::cerr << "Can't receive data." << std::endl;
            throw ServerException();
        }
        if (rc == 0)
            break;
    }

    return 0;
}
int Server::SendMessage(std::string response) {
    if (sendto(communication_socket, response.c_str(), response.size(), 0, (struct sockaddr *)&addr, sizeof(addr)) <
        0) {
        std::cerr << "Send error" << std::endl;
        throw ServerException();
    }
}
