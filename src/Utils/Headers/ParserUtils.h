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
    std::map<std::string, Type> mapOfType = {{"int", INT}, {"float", FLOAT}, {"boolean", BOOLEAN}, {"char", CHAR}};
    std::map<std::string, Constraint> mapOfConstraint = {{"not", NOT_NULL},
                                                         {"unique", UNIQUE},
                                                         {"primary", PRIMARY_KEY}};

    std::map<std::string, Action> mapOfAction = {{"create", CREATE}, {"show", SHOW_CREATE}, {"drop", DROP}};

   public:
    std::string chrToString(char *name);
    Type stringToType(std::string name);
    Constraint stringToConstraint(std::string name);
    Action stringToAction(std::string name);
    void sendToEngine(const Table &);
};

#endif  // SELSQL_PARSERUTILS_H
