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
    int sendMessage(std::string message);
    int getMessage();

   public:
    std::string response;
    int execRequest(std::string request);
    explicit Client();
    ~Client();
};

#endif  // CLIENT_WINCLIENT_H
