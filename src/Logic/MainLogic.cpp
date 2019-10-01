//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include <string>
void MainLogic::addColumn(char* name, char* type) {
    printf("1\n");
    if (currentVariable.type != none) {
        table.fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    currentVariable.name = parserUtils.chrToString(name);
    currentVariable.type = parserUtils.stringToType(parserUtils.chrToString(type));
    // printf("VALNAME = %s, TYPE = %s\n", name, type);
}

void MainLogic::addConstraint(char* name) {
    CONSTRAINT tempConstraint = parserUtils.stringToConstraint(parserUtils.chrToString(name));
    int flag = 0;
    for (const auto& elem : currentVariable.constraints) {
        if (elem == tempConstraint) {
            flag = 1;
            showError();
        }
    }
    if (!flag)
        currentVariable.constraints.push_back(tempConstraint);
    // printf("CONSTRAINT = %s\n", name);
}
void MainLogic::showError() { printf("ERROR CONSTRAINT"); }

void MainLogic::finish() {
    if (currentVariable.type != none) {
        table.fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    parserUtils.sendToEngine(table);
    // printf("SUCCESS %d\n", count);
}
void MainLogic::addTableName(char* name) {
    table.name.erase();
    table.fields.clear();

    table.name = parserUtils.chrToString(name);
    // printf("TABLENAME = %s\n", name);
}
void MainLogic::addActionName(char* name) {
    parserUtils.chrToString(name);
    // printf("ACTION = %s\n", name);
}
