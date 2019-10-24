//
// Created by quiks on 13.10.2019.
//
#include "WinClient.h"
#include <iostream>
Client::Client() {
    WORD wV = MAKEWORD(2, 2);
    WSADATA d;
    WSAStartup(wV, &d);
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
int Client::sendMessage(std::string message) {
    message.resize(MESSAGE_SIZE);
    char m[MESSAGE_SIZE];
    memcpy(m,message.c_str(),MESSAGE_SIZE);
    server_connection = send(client_socket, m, MESSAGE_SIZE, 0);
    if (server_connection <= 0) {
        std::cerr << "Send error" << std::endl;
        throw ClientException();
    }
    /* закрываем соединения для посылки данных */
    closesocket(server_connection);
    return 0;
}
int Client::getMessage() {
    char rec_message[MESSAGE_SIZE];
    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(client_socket, &allreads);
    for (;;) {
        readmask = allreads;
        if (select(client_socket, &readmask, NULL, NULL, NULL) <= 0) {
            //            throw ClientException();
        }
        if (FD_ISSET(client_socket, &readmask)) {
            memset(rec_message, 0, sizeof(rec_message));
            int result = recv(client_socket, rec_message, MESSAGE_SIZE, 0);
            if (result < 0) {
                throw ClientException();
            }
            if (result == 0) {
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
    response = rec_message;
    return 0;
}

int Client::execRequest(std::string request) {
    sendMessage(request);
    getMessage();
    return 0;
}

Client::~Client() {
    closesocket(server_connection);
    closesocket(client_socket);
    WSACleanup();
}

#include "WinClient.h"
