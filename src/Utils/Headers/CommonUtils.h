//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
enum TYPE { INT, FLOAT, BOOLEAN, CHAR };

enum CONSTRAINT { NOT_NULL, PRIMARY_KEY, UNIQUE };

struct Variable {
    TYPE type;
    std::vector<CONSTRAINT> constraints;
};

struct Table {
    std::string name;
    std::map<std::string, Variable> fields;
};

#endif  // SELSQL_COMMONUTILS_H
