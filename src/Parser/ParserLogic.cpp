//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ParserLogic.h"
#include "../Utils/Headers/ParserUtils.h"

void ParserLogic::addColumn(char* name, char* type) {
    if (checkName.find(ParserUtils::chrToString(name)) == checkName.end()) {
        response.ddlData.table.addField(string(name), parserUtils.stringToType(string(type)));
        checkName[string(name)] = 1;
    } else {
        response.error.errorCode = 1;
        response.error.errorMsg = Constants::ERR_SAME_FIELD_NAME;
    }
}

void ParserLogic::addConstraint(char* name) {
    response.error.errorCode = response.ddlData.table.addConstraint(parserUtils.stringToConstraint(string(name)));
    if (response.error.errorCode)
        response.error.errorMsg = Constants::ERR_SAME_CONSTRAINT;
}

BigResponse ParserLogic::finish() { return response; }

void ParserLogic::addTableName(char* name) {
    response.clear();
    checkName.erase(checkName.begin(), checkName.end());

    response.tableName = string(name);
    response.ddlData.table.name = response.tableName;
}

void ParserLogic::addActionName(char* name) { response.action = parserUtils.stringToAction(string(name)); }