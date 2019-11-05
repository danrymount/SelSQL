//
// Created by toxaxab on 05.11.2019.
//

#ifndef SELSQL_EXPRESSIONVISITOR_H
#define SELSQL_EXPRESSIONVISITOR_H
#include "../../Utils/Headers/ActionsUtils.h"
#include "../../Utils/Headers/CommonUtils.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/AddNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/ArithmeticNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/DivNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/MultNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/SubNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/EqualsNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/NoEqualsNode.h"
#include "../Nodes/ExpressionsNodes/ExprNode.h"
#include "../Nodes/ExpressionsNodes/IndentExprNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
#include "../Nodes/ExpressionsNodes/ValueExprNode.h"
#include "TreeVisitor.h"
class ExpressionVisitor : public TreeVisitor {
   public:
    void visit(ExprNode* node) override {
        if (node->getChild()) {
            node->getChild()->accept(this);
            result = node->getChild()->getResult();
        }
    }

    void visit(NullValueNode* node) override { curValue = "null"; }

    void visit(AndLogicNode* node) override {
        node->getLeft()->accept(this);
        auto a = node->getLeft()->getResult();
        node->getRight()->accept(this);
        auto b = node->getRight()->getResult();
        node->setResult(a and b);
    }

    void visit(OrLogicNode* node) override {
        node->getLeft()->accept(this);
        auto a = node->getLeft()->getResult();
        node->getRight()->accept(this);
        auto b = node->getRight()->getResult();
        node->setResult(a or b);
    }

    void visit(NotLogicNode* node) override {
        node->getChild()->accept(this);
        auto a = node->getChild()->getResult();
        node->setResult(not a);
    }

    void visit(AddNode* node) override {
        node->getLeft()->accept(this);
        auto a = std::move(curValue);
        node->getRight()->accept(this);
        auto b = std::move(curValue);
        curValue = std::to_string(ActionsUtils::calculate[0](std::stod(a), std::stod(b)));
    }

    void visit(DivNode* node) override {
        node->getLeft()->accept(this);
        auto a = std::move(curValue);
        node->getRight()->accept(this);
        auto b = std::move(curValue);
        curValue = std::to_string(ActionsUtils::calculate[3](std::stod(a), std::stod(b)));
    }

    void visit(SubNode* node) override {
        node->getLeft()->accept(this);
        auto a = std::move(curValue);
        node->getRight()->accept(this);
        auto b = std::move(curValue);
        curValue = std::to_string(ActionsUtils::calculate[1](std::stod(a), std::stod(b)));
    }

    void visit(MultNode* node) override {
        node->getLeft()->accept(this);
        auto a = std::move(curValue);
        node->getRight()->accept(this);
        auto b = std::move(curValue);
        curValue = std::to_string(ActionsUtils::calculate[2](std::stod(a), std::stod(b)));
    }

    void visit(MoreNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::GREATER](left, right));
    }

    void visit(EqualsNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::EQUALS](left, right));
    }
    void visit(NoEqualsNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::NOEQUALS](left, right));
    }

    void visit(MoreEqNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::GREATEREQUALS](left, right));
    }

    void visit(LessEqNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::LOWEREQUALS](left, right));
    }

    void visit(LessNode* node) override {
        node->getLeft()->accept(this);
        auto left = std::move(curValue);
        node->getRight()->accept(this);
        auto right = std::move(curValue);
        node->setResult(ActionsUtils::checkSign[Cmp::LOWER](left, right));
    }

    void visit(IndentExprNode* node) override {
        int flag = 0;
        std::vector<std::pair<std::pair<std::string, std::string>, std::string>> values;
        values.reserve(firstValues.size() + secondValues.size());
        values.insert(values.end(), firstValues.begin(), firstValues.end());
        values.insert(values.end(), secondValues.begin(), secondValues.end());
        auto value = node->getBaseValue();
        auto res = std::find_if(values.begin(), values.end(),
                                [value, node](const std::pair<std::pair<std::string, std::string>, std::string>& val) {
                                    return value == val.first.second &&
                                           (node->getAliasname() == val.first.first || node->getAliasname().empty());
                                });
        if (res == values.end()) {
            result = false;
            message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
            return;
        }
        curValue = res->second;
    }

    void visit(ValueExprNode* node) override { curValue = node->getBaseValue(); }

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    void setFirstValues(std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _values) {
        firstValues = std::move(_values);
    }

    void setSecondValues(std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _values) {
        secondValues = std::move(_values);
    }

    bool getResult() { return result; }

    std::string getCurValue() { return curValue; }

   private:
    std::vector<std::pair<std::pair<std::string, std::string>, std::string>> firstValues;
    std::vector<std::pair<std::pair<std::string, std::string>, std::string>> secondValues;
    std::string curValue;
    bool result = true;
};

#endif  // SELSQL_EXPRESSIONVISITOR_H
