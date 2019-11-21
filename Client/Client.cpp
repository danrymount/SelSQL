//
// Created by quiks on 13.10.2019.
//

#include "Client.h"
#ifdef __WIN32
#include "ClientUtils/Win/ClientUtilsWin.h"
#elif __linux
#include "ClientUtils/Lin/ClientUtilsLin.h"
#endif

#include <cstring>
#include <iostream>
Client::Client() {
    ClientUtils::startClient();

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0) {
        std::cerr << "Unable to create server socket" << std::endl;
        throw ClientException();
    }

    peer.sin_family = AF_INET;
    peer.sin_port = htons(18666);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    server_connection = connect(client_socket, (struct sockaddr *)&peer, sizeof(peer));
    if (server_connection) {
        std::cerr << "Unable to connect with server" << std::endl;
        throw ClientException();
    }
}
void Client::sendMessage(std::string message) {
    if (message == "")
        message = " ";
    server_connection = send(client_socket, message.c_str(), size(message), 0);
    if (server_connection <= 0) {
        std::cerr << "Send error" << std::endl;
        throw ClientException();
    }
    ClientUtils::close(server_connection);
}
void Client::getMessage() {
    char rec_message[MESSAGE_SIZE];
    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(client_socket, &allreads);
    while (true) {
        readmask = allreads;
        if (FD_ISSET(client_socket, &readmask)) {
            memset(rec_message, 0, sizeof(rec_message));
            int result = recv(client_socket, rec_message, MESSAGE_SIZE, 0);
            if (result <= 0) {
                std::cerr << "Server disconnected" << std::endl;
                throw ClientException();
            }

            break;
        }
        if (FD_ISSET(0, &readmask)) {
            std::cerr << "No server response" << std::endl;
            throw ClientException();
        }
    }
    response = std::string(rec_message);
}

void Client::execRequest(std::string request) {
    sendMessage(request);
    getMessage();
}

Client::~Client() { ClientUtils::clientClose(server_connection, client_socket); }
