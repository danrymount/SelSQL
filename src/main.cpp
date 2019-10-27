
#include "Parser/Headers/TreeVisitor.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string str;
    auto visitor = new TreeVisitor();
    while (true) {
        std::getline(std::cin, str);
        RootNode *tree = parse_request(str.c_str());
        if (tree == nullptr) {
            std::cerr << "PARSE ERROR" << std::endl;
        } else {
            tree->accept(visitor);
            auto message = visitor->getMessage();
            std::cout << message.getMsg() << std::endl;
        }
        printf("ENTER NEW COMMAND\n");
    }
}