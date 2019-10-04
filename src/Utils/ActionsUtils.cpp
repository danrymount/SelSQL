//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"
std::string ActionsUtils::makeRequestCreateFromTable(Table& table) {
    const char space = ' ';
    const char semicolon = ';';
    const char comma = ',';
    std::string str = "CREATE TABLE ";
    str += table.name + '(';
    for (auto& field : table.getFields()) {
        str += field.first + space;
        str += parserUtils.typeToString(field.second.type) + space;
        for (auto& constraint : field.second.getConstraints())
            str += parserUtils.constraintToString(constraint) + space;
        if ((*(table.getFields().end())).first != field.first && table.getFields().size() > 1)
            str += comma;
    }
    str += ')';
    str += semicolon;
    return str;
}
