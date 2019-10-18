
#include "Logic/Headers/MainLogic.h"
#include "Utils/Structures/BigResponse.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string str;
    BigResponse result;
    while (true) {
        std::getline(std::cin, str);
        auto tree = parse_request(str.c_str());
        std::cout << "SUCCESS" << std::endl;
        //        if (result.error.getErrorCode())
        //            std::cerr << result.error.getErrorMsg() << std::endl;
        //        else {
        //            result = MainLogic::executeRequest(result);
        //            if (result.error.getErrorCode())
        //                std::cerr << result.error.getErrorMsg() << " ERROR: " << result.error.getErrorCode() <<
        //                std::endl;
        //            else if (result.ddlData.returnMsg.size() > 0) {
        //                std::cout << result.ddlData.returnMsg << std::endl;
        //            }
        //        }
        //        printf("ENTER NEW COMMAND\n");
    }
}