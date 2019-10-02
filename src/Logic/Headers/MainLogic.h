//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_MAINLOGIC_H
#define SELSQL_MAINLOGIC_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../../Engine/Headers/MainEngine.h"
#include "../../Utils/Headers/ParserUtils.h"
#include "APILogic.h"

class MainLogic : public APILogic {
    ParserUtils parserUtils;
    MainEngine engineApi;

    Table* table;
    Variable currentVariable;
    Action currentAction;
    std::map<std::string, int> checkName;

    static void showError();
    static void showErrorName();

   public:
    void addColumn(char* name, char* type) override;
    void addConstraint(char* name) override;
    Table* finish() override;
    void addTableName(char* name) override;
    void addActionName(char* name) override;
};

#endif  // SELSQL_MAINLOGIC_H
