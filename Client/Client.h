//
// Created by quiks on 13.10.2019.
//

#ifndef CLIENT_WINCLIENT_H
#define CLIENT_WINCLIENT_H

#ifdef __WIN32
#include <winsock2.h>
#elif __linux
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <sys/types.h>
#include <memory>

#include "Exception.h"

static const int MESSAGE_SIZE = 1024 * 1024;
class Client {
    int client_socket;
    sockaddr_in peer;
    int server_connection;

   public:
    std::string response;
    void execRequest(std::string request);
    void sendMessage(std::string message);
    void getMessage();
    explicit Client();
    ~Client();
};

#endif  // CLIENT_WINCLIENT_H
