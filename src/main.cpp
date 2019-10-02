
#include "parser.cpp"
int main() {
    std::string str;
    while (true) {
        std::getline(std::cin, str);
        parse_request(str.c_str());
        printf("ENTER NEW COMMAND\n");
    }
}