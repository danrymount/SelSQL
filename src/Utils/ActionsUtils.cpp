//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"
#include "Headers/ParserUtils.h"
std::string ActionsUtils::makeRequestCreateFromTable(Table& table) {
    const char space = ' ';
    const char semicolon = ';';
    const char comma = ',';
    std::string str = "CREATE TABLE ";
    str += table.name + '(';
    for (auto& field : table.fields) {
        str += field.name + space;
        str += parserUtils.typeToString(field.type) + space;
        for (auto& constraint : field.constraints) str += parserUtils.constraintToString(constraint) + space;
        if (table.fields[table.fields.size() - 1].name != field.name)
            str += comma;
    }
    str += ')';
    str += semicolon;
    return str;
}
