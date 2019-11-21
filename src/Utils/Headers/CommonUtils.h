//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
enum Type { TYPE_INT, TYPE_FLOAT, TYPE_CHAR };

enum Constraint { NOT_NULL, PRIMARY_KEY, UNIQUE };

enum Cmp { GREATEREQUALS, GREATER, NOEQUALS, EQUALS, LOWER, LOWEREQUALS };

struct Condition {
    Condition(Cmp _sign, std::string _value) : sign(_sign), value(_value) {}
    Condition() = default;
    Cmp sign;
    std::string value;
};

static int DEBUG_MODE = 1;

template <typename T>
void print(const T &elem) {
    if (DEBUG_MODE) {
        std::cout << elem << std::endl;
    }
}

typedef std::vector<std::string> vecString;
typedef std::vector<std::pair<std::pair<std::string, std::string>, std::string>> RecordsFull;
typedef std::pair<std::pair<std::string, std::string>, std::string> RecordColumn;

#endif  // SELSQL_COMMONUTILS_H
