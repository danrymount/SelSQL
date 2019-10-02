//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include <string>
void MainLogic::addColumn(char* name, char* type) {
    if (currentVariable.type != none) {
        table->fields.push_back(currentVariable);
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

Table* MainLogic::finish() {
    int errorCode = 0;
    if (currentVariable.type != none) {
        table->fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    switch (currentAction) {
        case CREATE: {
            errorCode = engineApi.CreateTable(table);
            if (errorCode) {
                std::cout << "Table already exists" << std::endl;
                return nullptr;
            }
            break;
        }
        case SHOW_CREATE: {
            table = engineApi.ShowCreateTable(table->name);
            if (table == nullptr) {
                std::cout << "Table doesnt exist" << std::endl;
                return nullptr;
            }
            break;
        }
        case DROP: {
            errorCode = engineApi.DropTable(table->name);
            // parserUtils.sendToEngine(table.name);
            if (errorCode) {
                std::cout << "Table doesnt exist" << std::endl;
                return nullptr;
            }
            break;
        }
    }

    return table;
}

void MainLogic::addTableName(char* name) {
    table = new Table();
    table->name.erase();
    table->fields.clear();
    checkName.erase(checkName.begin(), checkName.end());

    table->name = parserUtils.chrToString(name);
}

void MainLogic::addActionName(char* name) { currentAction = parserUtils.stringToAction(parserUtils.chrToString(name)); }
