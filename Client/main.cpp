//
// Created by quiks on 13.10.2019.
//
#ifdef __WIN32
#include "Win/WinClient.h"
#elif __linux
#include "Linux/LinuxClient.h"
#endif
#include <iostream>
#include "Exception.h"
int main() {
    std::string message;
    try {
        Client client;
        while (true) {
            std::getline(std::cin, message);
            client.sendMessage(message);
            client.getMessage();
            std::cout << client.recieved_message << std::endl;
        }
    } catch (ClientException) {
        std::cout << "ERROR" << std::endl;
    }

    return 0;
}