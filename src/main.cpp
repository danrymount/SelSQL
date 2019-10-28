
#include "Parser/Headers/TreeVisitor.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string request;
    auto visitor = new TreeVisitor();
    while (true) {
        std::getline(std::cin, request);
        auto parser_msg = new std::string();

        RootNode *tree = parse_request(request.c_str(), parser_msg);
        if (tree == nullptr) {
            std::cout << *parser_msg << std::endl;
        } else {
            tree->accept(visitor);
            auto message = visitor->getMessage();
            std::cout << message.getMsg() << std::endl;
        }
        printf("ENTER NEW COMMAND\n");
    }
}