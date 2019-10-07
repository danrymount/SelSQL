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
    std::map<std::string, Type> mapOfStringToType = {{"int", INT},
                                                     {"float", FLOAT},
                                                     {"boolean", BOOLEAN},
                                                     {"char", CHAR}};
    std::map<std::string, Constraint> mapOfStringToConstraint = {{"not", NOT_NULL},
                                                                 {"unique", UNIQUE},
                                                                 {"primary", PRIMARY_KEY}};

    std::map<std::string, Action> mapOfAction = {{"create", CREATE}, {"show", SHOW_CREATE}, {"drop", DROP}};

    std::map<std::string, Cmp> mapOfCmp = {{"=", EQUALS},  {"<", LOWER},          {"<=", LOWEREQUALS},
                                           {">", GREATER}, {">=", GREATEREQUALS}, {"<>", NOEQUALS}};

    std::map<Constraint, std::string> mapOfConstraintToString = {{NOT_NULL, "NOT NULL"},
                                                                 {UNIQUE, "UNIQUE"},
                                                                 {PRIMARY_KEY, "PRIMARY KEY"}};
    std::map<Type, std::string> mapOfTypeToString = {{INT, "INT"},
                                                     {FLOAT, "FLOAT"},
                                                     {BOOLEAN, "BOOLEAN"},
                                                     {CHAR, "CHAR"}};

   public:
    static std::string chrToString(char *name);
    std::string typeToString(Type type);
    std::string constraintToString(Constraint constraint);
    Type stringToType(std::string name);
    Constraint stringToConstraint(std::string name);
    Action stringToAction(std::string name);
    Cmp stringToCmp(const std::string& name);
};

#endif  // SELSQL_PARSERUTILS_H
