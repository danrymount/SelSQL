//
// Created by quiks on 13.10.2019.
//

#ifndef UNTITLED6_LINUXSERVER_H
#define UNTITLED6_LINUXSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <memory>

#include <netinet/in.h>
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

#endif  // UNTITLED6_LINUXSERVER_H
