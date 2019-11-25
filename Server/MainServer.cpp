//
// Created by quiks on 23.10.2019.
//

#include "MainServer.h"
#include "../src/Parser/Headers/TreeVisitor.h"
#include "parser.cpp"

std::mutex m;

static std::shared_ptr<MainEngine> engine = std::make_shared<MainEngine>(MainEngine());

std::string ExecuteRequest(const std::string &request, int id) {
    std::lock_guard<std::mutex> guard(m);
    std::string parser_msg = "Success";

    RootNode *tree = parse_request(request.c_str(), &parser_msg, engine, id);
    if (tree == nullptr) {
        return parser_msg;
    } else {
        auto visitor = new TreeVisitor(engine);
        tree->accept(visitor);
        auto message = visitor->getMessage();
        std::string res = "Success";
        if (!visitor->getMessage().getMsg().empty()) {
            res = visitor->getMessage().getMsg();
        }
        delete visitor;
        return res;
    }
}

int ListenClient(int id, Server *server) {
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

        message = ExecuteRequest(std::string(server->recieved_message), id);
        if (DEBUG) {
            std::cout << "Send message to Client " << id + 1 << " :" << std::endl;

            std::cout << "\t" << message << std::endl;
            //            std::cout<<message.size()<<std::endl;
        }
        server->SendMessage(message, id);
    }
}

void RunServer() {
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
    }
};
