
#include "Engine/Headers/MainEngine.h"
#include "Parser/Headers/TreeVisitor.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string request;
    std::shared_ptr<MainEngine> engine = std::make_shared<MainEngine>(MainEngine());
    auto visitor = new TreeVisitor(engine);
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