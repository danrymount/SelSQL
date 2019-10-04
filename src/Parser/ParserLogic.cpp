//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ParserLogic.h"
#include "../Utils/Headers/ParserUtils.h"

void ParserLogic::addColumn(char* name, char* type) {
    if (checkName.find(ParserUtils::chrToString(name)) == checkName.end()) {
        response.table.addField(string(name), parserUtils.stringToType(string(type)));
        checkName[string(name)] = 1;
    } else {
        response.code = 1;
        response.errorMsg = Constants::ERR_SAME_FIELD_NAME;
    }
}

void ParserLogic::addConstraint(char* name) {
    response.code = response.table.addConstraint(parserUtils.stringToConstraint(string(name)));
    if (response.code)
        response.errorMsg = Constants::ERR_SAME_CONSTRAINT;
}

Response ParserLogic::finish() { return response; }

void ParserLogic::addTableName(char* name) {
    response.clear();
    checkName.erase(checkName.begin(), checkName.end());

    response.table.name = string(name);
}

void ParserLogic::addActionName(char* name) { response.action = parserUtils.stringToAction(string(name)); }