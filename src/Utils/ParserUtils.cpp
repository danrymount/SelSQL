//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/ParserUtils.h"

std::string ParserUtils::chrToString(char *name) {
    if (name != nullptr) {
        return std::string(name);
    }
    return nullptr;
}

TYPE ParserUtils::stringToType(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    return mapOfType[name];
}

CONSTRAINT ParserUtils::stringToConstraint(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    return mapOfConstraint[name];
}
