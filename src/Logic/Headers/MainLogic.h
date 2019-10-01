//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_MAINLOGIC_H
#define SELSQL_MAINLOGIC_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../../Utils/Headers/ParserUtils.h"
#include "APILogic.h"

class MainLogic : public APILogic {
    int count = 0;
    ParserUtils parserUtils;
    Table table;
    Variable currentVariable;
    ACTION currentAction;
    std::map<std::string, int> checkName;

    static void showError();
    static void showErrorName();

   public:
    void addColumn(char* name, char* type) override;
    void addConstraint(char* name) override;
    Table finish() override;
    void addTableName(char* name) override;
    void addActionName(char* name) override;
};

#endif  // SELSQL_MAINLOGIC_H
