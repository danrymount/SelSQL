//
// Created by quiks on 13.10.2019.
//

#ifndef SERVER_WINSERVER_H
#define SERVER_WINSERVER_H

#ifdef __WIN32
#include <winsock2.h>
#elif __linux
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#include <memory>
#include <string>
#include <vector>
#include "Exception.h"

static const int MESSAGE_SIZE = 90000;

class Server {
    int server_socket;
    std::vector<int> communication_socket;
    sockaddr_in addr;

   public:
    char recieved_message[MESSAGE_SIZE];
    explicit Server(int max_connection);
    int ListenSocket(int id);
    void SendMessage(std::string response, int id);
    int AcceptSocket(int id);
    ~Server();
};

#endif  // SERVER_WINSERVER_H
