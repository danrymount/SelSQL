//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include <string>
void MainLogic::addColumn(char* name, char* type) {
    if (currentVariable.type != none) {
        table.fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    if (checkName.find(parserUtils.chrToString(name)) == checkName.end()) {
        currentVariable.constraints.erase(currentVariable.constraints.begin(), currentVariable.constraints.end());
        checkName[parserUtils.chrToString(name)] = 1;
        currentVariable.name = parserUtils.chrToString(name);
        currentVariable.type = parserUtils.stringToType(parserUtils.chrToString(type));
    } else {
        showErrorName();
    }
    // printf("VALNAME = %s, Type = %s\n", name, type);
}

void MainLogic::addConstraint(char* name) {
    Constraint tempConstraint = parserUtils.stringToConstraint(parserUtils.chrToString(name));
    int flag = 0;
    for (const auto& elem : currentVariable.constraints) {
        if (elem == tempConstraint) {
            flag = 1;
            showError();
        }
    }
    if (!flag)
        currentVariable.constraints.push_back(tempConstraint);
}
void MainLogic::showError() { printf("ERROR Constraint"); }

void MainLogic::showErrorName() { printf("ERROR NAME"); }

Table MainLogic::finish() {
    if (currentVariable.type != none) {
        table.fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    switch (currentAction){
        case CREATE:{
            //engineApi.CreateTable(table);
        }
        case SHOW_CREATE:{
            //engineApi.CreateTable(table.name);
        }
        case DROP:{
            //parserUtils.sendToEngine(table.name);
        }
    }
    return table;
}

void MainLogic::addTableName(char* name) {
    table.name.erase();
    table.fields.clear();
    checkName.erase(checkName.begin(), checkName.end());

    table.name = parserUtils.chrToString(name);
}

void MainLogic::addActionName(char* name) { currentAction = parserUtils.stringToAction(parserUtils.chrToString(name)); }
