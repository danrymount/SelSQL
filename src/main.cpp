
#include "Logic/Headers/MainLogic.h"
#include "Utils/Structures/BigResponse.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string str;
    BigResponse result;
    auto visitor = new TreeVisitor();
    while (true) {
        std::getline(std::cin, str);
        auto tree = parse_request(str.c_str());
        tree->accept(visitor);
        auto result = visitor->getResponse();
        if (result->error.getErrorCode())
            std::cerr << result->error.getErrorMsg() << " ERROR: " << result->error.getErrorCode() << std::endl;
        else if (result->ddlData.returnMsg.size() > 0) {
            std::cout << result->ddlData.returnMsg << std::endl;
        }
        printf("ENTER NEW COMMAND\n");
    }
    delete visitor;
}