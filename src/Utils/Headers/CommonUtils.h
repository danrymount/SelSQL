//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
enum Type { none, INT, FLOAT, BOOLEAN, CHAR };

enum Constraint { NOT_NULL, PRIMARY_KEY, UNIQUE };

enum Action { SHOW_CREATE, DROP, CREATE };

struct Variable {
    Type type;
    std::string name;
    std::vector<Constraint> constraints;
};

struct Table {
    std::string name;
    // std::map<std::string, Variable> fields;
    std::vector<Variable> fields;
};

#endif  // SELSQL_COMMONUTILS_H
