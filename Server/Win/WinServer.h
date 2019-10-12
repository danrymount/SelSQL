//
// Created by quiks on 13.10.2019.
//

#ifndef SERVER_WINSERVER_H
#define SERVER_WINSERVER_H

#include <winsock2.h>
#include <string>
#include "../Exception.h"
static const int MESSAGE_SIZE = 256;

class Server {
    int server_socket;
    int communication_socket;
    sockaddr_in addr;

   public:
    char recieved_message[256];
    explicit Server();
    int ListenSocket();
    int SendMessage(std::string response);
};

#endif  // SERVER_WINSERVER_H
