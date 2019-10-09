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
        response.error = Error(ErrorConstants::ERR_SAME_FIELD_NAME);
    }
}

void ParserLogic::addConstraint(char* name) {
    int error = response.ddlData.table.addConstraint(parserUtils.stringToConstraint(string(name)));
    if (error) {
        response.error = Error(ErrorConstants::ERR_SAME_CONSTRAINT);
    }
}

BigResponse ParserLogic::finish() { return response; }

void ParserLogic::addActionName(char* name) { response.action = parserUtils.stringToAction(string(name)); }

void ParserLogic::addSelectColumn(char* name) { response.dqlData.columns.emplace_back(string(name)); }

void ParserLogic::addCondition(char* name, char* sign, char* value) {
    if (response.action == SELECT) {
        response.dqlData.conditions.insert(std::make_pair(string(name), Condition(parserUtils.stringToCmp(string(sign)),
                                                                                  string(value))));
        return;
    }
    response.dmlData.conditions.insert(std::make_pair(string(name),
                                                      Condition(parserUtils.stringToCmp(string(sign)), string(value))));
}

void ParserLogic::addColumn(char* name) {
    response.dmlData.columns.emplace_back(string(name));
    std::cout << response.dmlData.columns.size() << std::endl;
}

void ParserLogic::addValue(char* value) {
    response.dmlData.values.emplace_back(string(value));
    // std::cout << response.dmlData.values[0] << std::endl;
}

void ParserLogic::addTableName(char* name) {
    response.tableName = string(name);
    response.ddlData.table.name = response.tableName;
}

void ParserLogic::start() {
    BigResponse _response;
    response = _response;
    checkName.erase(checkName.begin(), checkName.end());
}