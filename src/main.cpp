
#include "Logic/Headers/MainLogic.h"
#include "parser.cpp"

int main() {
    std::string str;
    Response result;
    while (true) {
        std::getline(std::cin, str);
        result = parse_request(str.c_str());
        if (result.code)
            std::cerr << result.errorMsg << std::endl;
        else {
            result = MainLogic::executeRequest(result);
            if (result.code)
                std::cerr << result.errorMsg << std::endl;
        }
        printf("ENTER NEW COMMAND\n");
    }
}