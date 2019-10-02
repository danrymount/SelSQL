
#include "parser.cpp"
#include "Engine/Headers/MainEngine.h"
int main() {
    std::string request;
    std::getline(std::cin,request);

    parse_request(request.c_str());



//    auto mainEngine = new MainEngine();
//    Table t, c;
//    Variable v;
//    v.name = "ID";
//    v.type = INT;
//    t.name = "TEST";
//    std::vector<Variable> vector = {v};
//    t.fields = vector;
//
//    mainEngine->CreateTable(&t);
//    c = *mainEngine->ShowCreateTable("TEST");

    return 0;
}