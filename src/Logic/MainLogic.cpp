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
    if (checkName.find(ParserUtils::chrToString(name)) == checkName.end()) {
        currentVariable.constraints.erase(currentVariable.constraints.begin(), currentVariable.constraints.end());
        checkName[ParserUtils::chrToString(name)] = 1;
        currentVariable.name = ParserUtils::chrToString(name);
        currentVariable.type = parserUtils.stringToType(ParserUtils::chrToString(type));
    } else {
        errorCode = 1;
        showErrorName();
    }
    // printf("VALNAME = %s, Type = %s\n", name, type);
}

void MainLogic::addConstraint(char* name) {
    Constraint tempConstraint = parserUtils.stringToConstraint(ParserUtils::chrToString(name));
    int flag = 0;
    for (const auto& elem : currentVariable.constraints) {
        if (elem == tempConstraint) {
            flag = 1;
            errorCode = 1;
            showError();
        }
    }
    if (!flag)
        currentVariable.constraints.push_back(tempConstraint);
}
void MainLogic::showError() { printf("ERROR Constraint"); }

void MainLogic::showErrorName() { printf("ERROR NAME"); }

Table* MainLogic::finish() {
    if (currentVariable.type != none) {
        table->fields.push_back(currentVariable);
        currentVariable.type = none;
    }
    if (!errorCode) {
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
                std::cout << actionsUtils.makeRequestCreateFromTable(*table) << std::endl;
                break;
            }
            case DROP: {
                errorCode = engineApi.DropTable(table->name);
                if (errorCode) {
                    std::cout << "Table doesnt exist" << std::endl;
                    return nullptr;
                }
                break;
            }
        }
    }

    return table;
}

void MainLogic::addTableName(char* name) {
    table = new Table();
    table->name.erase();
    table->fields.clear();
    checkName.erase(checkName.begin(), checkName.end());

    table->name = ParserUtils::chrToString(name);
}

void MainLogic::addActionName(char* name) { currentAction = parserUtils.stringToAction(ParserUtils::chrToString(name)); }
