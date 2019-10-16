//
// Created by quiks on 13.10.2019.
//
#ifdef __WIN32
#include "Win/WinServer.h"
#elif __linux
#include "Linux/LinuxServer.h"
#endif
#include <iostream>
#include <sstream>
#include <thread>
#include "Exception.h"

#define MAX_CONN 5

int ListenClient(int id, Server* server) {
    server->AcceptSocket(id);
    std::cout << "Client " << id + 1 << " connected" << std::endl;
    while (true) {
        std::string message;
        int err = server->ListenSocket(id);
        if (err == 1) {
            std::cout << "Client " << id + 1 << " disconnected" << std::endl;
            server->AcceptSocket(id);
            std::cout << "Client " << id + 1 << " connected" << std::endl;
        }
        std::cout << "Got message from Client " << id + 1 << " :" << std::endl;
        std::cout << "\t" << server->recieved_message << std::endl;

        message = "GET";
        std::cout << "Send message to Client " << id + 1 << " :" << std::endl;
        std::cout << "\t" << message << std::endl;
        server->SendMessage(message, id);
    }
}

int main() {
    std::vector<std::thread> threads;

    try {
        Server server(MAX_CONN);

        for (size_t i = 0; i < MAX_CONN; ++i) {
            threads.emplace_back(std::thread(ListenClient, i, &server));
        }
        for (size_t i = 0; i < MAX_CONN; ++i) {
            threads[i].join();
        }
    } catch (ServerException) {
        std::cerr << "Error" << std::endl;
    }
    return 0;
}
