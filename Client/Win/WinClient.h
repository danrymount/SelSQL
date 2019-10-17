//
// Created by quiks on 13.10.2019.
//

#ifndef CLIENT_WINCLIENT_H
#define CLIENT_WINCLIENT_H

#include <sys/types.h>
#include <winsock2.h>
#include <memory>

#include "../Exception.h"

static const int MESSAGE_SIZE = 1024;
class Client {
    SOCKET client_socket;
    sockaddr_in peer;
    SOCKET server_connection;

   public:
    explicit Client();
    int sendMessage(std::string message);
    int getMessage();
    char recieved_message[MESSAGE_SIZE];
};

#endif  // CLIENT_WINCLIENT_H
