//
// Created by quiks on 13.10.2019.
//

#ifndef UNTITLED6_LINUXSERVER_H
#define UNTITLED6_LINUXSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <memory>

#include <netinet/in.h>
#include <vector>
#include "../Exception.h"

static const int MESSAGE_SIZE = 1024;

class Server {
   public:
    int server_socket;
    std::vector<int> communication_socket;
    sockaddr_in addr;

    char recieved_message[MESSAGE_SIZE];
    explicit Server(int max_connection);
    int ListenSocket(int id);
    int SendMessage(std::string response, int id);
    int AcceptSocket(int id);
    ~Server();
};

#endif  // UNTITLED6_LINUXSERVER_H
