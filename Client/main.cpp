//
// Created by quiks on 13.10.2019.
//
#include <iostream>
#include "Client.h"
#include "Exception.h"
int main() {
    std::string message;
    try {
        Client client;
        while (true) {
            std::getline(std::cin, message);
            client.execRequest(message);
            std::cout << client.response << std::endl;
        }
    } catch (ClientException) {
        std::cout << "ERROR" << std::endl;
    }

    return 0;
}