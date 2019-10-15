//
// Created by toxaxab on 15.10.2019.
//
#include "Structures/Data/Expresion.h"

std::map<std::string, Condition> Expression::getResult(std::vector<std::string> columnValues) {
    return std::map<std::string, Condition>();
}
void Expression::addColumn(char* name, char* sign) {
    expr.emplace_back(std::make_pair(std::string(name), parserUtils.stringToCmp(std::string(sign))),
                      std::make_pair(operands, operators));
    operands.clear();
    operators.clear();
    // columns.emplace_back(std::string(name));
}

void Expression::addOperand(char* name) { operands.emplace_back(std::string(name)); }

void Expression::addOperator(char* name) { operators.emplace_back(std::string(name)); }

void Expression::addLogicOperator(char* name) { logicOperators.emplace_back(std::string(name)); }
std::pair<Expr, vecString> Expression::endUnion() {
    fullExpression = std::make_pair(expr, logicOperators);
    return fullExpression;
}
