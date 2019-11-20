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
    inline static std::map<std::string, Type> mapOfStringToType = {{"int", TYPE_INT},
                                                                   {"float", TYPE_FLOAT},

                                                                   {"char", TYPE_CHAR}};
    inline static std::map<std::string, Constraint> mapOfStringToConstraint = {{"not", NOT_NULL},
                                                                               {"unique", UNIQUE},
                                                                               {"primary", PRIMARY_KEY}};

    inline static std::map<std::string, Cmp> mapOfCmp = {{"=", EQUALS},  {"<", LOWER},          {"<=", LOWEREQUALS},
                                                         {">", GREATER}, {">=", GREATEREQUALS}, {"<>", NOEQUALS}};

    inline static std::map<Constraint, std::string> mapOfConstraintToString = {{NOT_NULL, "NOT NULL"},
                                                                               {UNIQUE, "UNIQUE"},
                                                                               {PRIMARY_KEY, "PRIMARY KEY"}};
    inline static std::map<Type, std::string> mapOfTypeToString = {{TYPE_INT, "INT"},
                                                                   {TYPE_FLOAT, "FLOAT"},
                                                                   {TYPE_CHAR, "CHAR"}};

   public:
    static std::string chrToString(char* name);
    static std::string typeToString(Type type);
    static std::string constraintToString(Constraint constraint);
    static Type stringToType(std::string name);
    static Constraint stringToConstraint(std::string name);
    static Cmp stringToCmp(const std::string& name);
};

#endif  // SELSQL_PARSERUTILS_H
