
#include "parser.cpp"
int main() {
    char request[] = "CREATE TABLE name(ID INT);";
    parse_request(request);

    return 0;
}