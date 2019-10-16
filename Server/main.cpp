//
// Created by quiks on 13.10.2019.
//
#ifdef __WIN32
#include "Win/WinServer.h"
#elif __linux
#include "Linux/LinuxServer.h"
#endif
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include "../src/Logic/Headers/MainLogic.h"
#include "../src/Utils/Structures/BigResponse.h"
#include "Exception.h"
#include "parser.cpp"
#define MAX_CONN 100
// std::mutex m;

std::string executeRequest(std::string request) {
    //    std::lock_guard<std::mutex> guard(m);
    BigResponse result = parse_request(request.c_str());
    std::string message = "Success";
    if (result.error.getErrorCode())
        message = result.error.getErrorMsg();
    else {
        result = MainLogic::executeRequest(result);

        if (!result.select_message.empty()) {
            message = result.select_message;
        }

        if (result.error.getErrorCode())
            message = result.error.getErrorMsg() + std::string(" ERROR: ") +
                      std::to_string(result.error.getErrorCode());
        else if (result.ddlData.returnMsg.size() > 0) {
            message = result.ddlData.returnMsg;
        }
    }
    return message;
}

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

        message = executeRequest(std::string(server->recieved_message));
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
