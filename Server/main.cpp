//
// Created by quiks on 13.10.2019.
//
#ifdef __WIN32
#include "WinServer.h"
#elif __linux
#include "Linux/LinuxServer.h"
#endif
#include <iostream>
#include "Exception.h"

int main() {
    try {
        Server server;
        while (true) {
            server.ListenSocket();
            std::cout << server.recieved_message << std::endl;
            server.SendMessage("Success");
        }
    } catch (ServerException) {
        std::cerr << "Error" << std::endl;
    }

    return 0;
}
