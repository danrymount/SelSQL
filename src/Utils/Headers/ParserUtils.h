//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_PARSERUTILS_H
#define SELSQL_PARSERUTILS_H
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "CommonUtils.h"

class ParserUtils {
    std::map<std::string, TYPE> mapOfType = {{"int", INT},
                                             {"float", FLOAT},
                                             {"boolean", BOOLEAN},
                                             {"char", CHAR}};
    std::map<std::string, CONSTRAINT> mapOfConstraint = {{"not", NOT_NULL},
                                                         {"unique", UNIQUE},
                                                         {"primary", PRIMARY_KEY}};

   public:
    std::string chrToString(char *name);
    TYPE stringToType(std::string name);
    CONSTRAINT stringToConstraint(std::string name);
    void sendToEngine(const Table &);
};

#endif  // SELSQL_PARSERUTILS_H
