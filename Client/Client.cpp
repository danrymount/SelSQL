//
// Created by quiks on 13.10.2019.
//
#include "Client.h"

#include <cstring>
#include <iostream>
Client::Client() {
#ifdef __WIN32
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
#endif
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
    message.resize(MESSAGE_SIZE);
    server_connection = send(client_socket, message.c_str(), MESSAGE_SIZE, 0);
    if (server_connection <= 0) {
        std::cerr << "Send error" << std::endl;
        throw ClientException();
    }
    /* закрываем соединения для посылки данных */
#ifdef __WIN32
    closesocket(server_connection);
#elif __linux
    shutdown(server_connection, SHUT_RD);
#endif
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

Client::~Client() {
#ifdef __WIN32
    closesocket(server_connection);
    closesocket(client_socket);
    WSACleanup();
#elif __linux
    shutdown(client_socket, SHUT_RDWR);
    shutdown(server_connection, SHUT_RDWR);
#endif
}
