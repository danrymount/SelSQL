//
// Created by toxaxab on 15.10.2019.
//

#ifndef SELSQL_EXPRESION_H
#define SELSQL_EXPRESION_H

#include <string>
#include <vector>
#include "../../Headers/CommonUtils.h"
#include "../../Headers/ParserUtils.h"

class Expression {
   public:
    std::map<std::string, Condition> getResult(std::vector<std::string> columnValues);

    void addColumn(char* name, char* sign);
    void addOperand(char* name);
    void addOperator(char* name);
    void addLogicOperator(char* name);
    std::pair<Expr, vecString> endUnion();

   private:
    ParserUtils parserUtils;
    vecString operators;
    // std::vector<std::string> columns;
    vecString operands;
    vecString logicOperators;

    Expr expr;
    std::pair<Expr, vecString> fullExpression;
    // std::array<std::string, 5> getOperationNumber = {};
};

#endif  // SELSQL_EXPRESION_H
