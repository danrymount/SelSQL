//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/ParserUtils.h"
#include <iostream>

std::string ParserUtils::chrToString(char* name) { return std::string(name); }

Type ParserUtils::stringToType(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    return mapOfStringToType[name];
}

Constraint ParserUtils::stringToConstraint(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    return mapOfStringToConstraint[name];
}

Cmp ParserUtils::stringToCmp(const std::string& name) { return mapOfCmp[name]; }

std::string ParserUtils::typeToString(Type type) { return mapOfTypeToString[type]; }

std::string ParserUtils::constraintToString(Constraint constraint) { return mapOfConstraintToString[constraint]; }
