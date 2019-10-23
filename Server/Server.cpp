//
// Created by quiks on 23.10.2019.
//

#include "Server.h"
#include "parser.cpp"
std::mutex m;
std::string executeRequest(std::string request) {
    std::lock_guard<std::mutex> guard(m);
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
    if (DEBUG) {
        std::cout << "Client " << id + 1 << " connected" << std::endl;
    }

    while (true) {
        std::string message;
        int err = server->ListenSocket(id);
        if (err == 1) {
            if (DEBUG) {
                std::cout << "Client " << id + 1 << " disconnected" << std::endl;
            }

            server->AcceptSocket(id);
            if (DEBUG) {
                std::cout << "Client " << id + 1 << " connected" << std::endl;
            }

            continue;
        }
        if (DEBUG) {
            std::cout << "Got message from Client " << id + 1 << " :" << std::endl;
            std::cout << "\t" << server->recieved_message << std::endl;
        }

        message = executeRequest(std::string(server->recieved_message));
        if (DEBUG) {
            std::cout << "Send message to Client " << id + 1 << " :" << std::endl;
            std::cout << "\t" << message << std::endl;
        }

        server->SendMessage(message, id);
    }
}

void runServer() {
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
};
