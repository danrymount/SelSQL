//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/ParserUtils.h"
#include <iostream>

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

void ParserUtils::sendToEngine(const Table &table) {
    std::cout << "Table name = " << table.name << std::endl;
    for (const auto &field : table.fields) {
        std::cout << "val name = " << field.name << "," << field.type << std::endl;
        for (const auto &constraint : field.constraints) {
            std::cout << "Constraint = " << constraint << std::endl;
        }
    }
    std::cout << std::endl;
}
