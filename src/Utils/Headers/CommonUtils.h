//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
enum Type { INT, FLOAT, CHAR };

enum Constraint { NOT_NULL, PRIMARY_KEY, UNIQUE };

enum Action { SHOW_CREATE, DROP, CREATE, SELECT, DELETE, INSERT, UPDATE };

enum Cmp { GREATEREQUALS = -3, GREATER = -2, NOEQUALS = -1, EQUALS = 1, LOWER, LOWEREQUALS };

struct Condition {
    Condition(Cmp _sign, std::string _value) : sign(_sign), value(_value) {}

    Cmp sign;
    std::string value;
};

#endif  // SELSQL_COMMONUTILS_H
