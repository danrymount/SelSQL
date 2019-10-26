//
// Created by sapiest on 24.10.2019.
//

#ifndef SELSQL_DELETEVISITOR_H
#define SELSQL_DELETEVISITOR_H
#include "../Nodes/ActionNodes/DeleteNode.h"
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
#include "../Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
#include "../Nodes/ExpressionsNodes/ValueExprNode.h"
#include "TreeVisitor.h"
class DeleteVisitor : public TreeVisitor {
   public:
    void visit(DeleteNode* node) override { expr = static_cast<BaseExprNode*>(node->getChild()); }

    void visit(ExprNode* node) override {
        if (node->getChild()) {
            node->getChild()->accept(this);
            result = node->getChild()->getResult();
        }
    }

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
        for (auto& val : values) {
            if (node->getBaseValue() == val.first) {
                curValue = val.second;
                flag = 1;
                break;
            }
        }
        if (!flag) {
            result = false;
            error = Error(ErrorConstants::ERR_NO_SUCH_FIELD);
        }
    }

    void visit(ValueExprNode* node) override { curValue = node->getName(); }

    bool getResult() { return result; }

    Error getError() { return error; }

    BaseExprNode* getExpr() { return expr; }

    void setValues(std::vector<std::pair<std::string, std::string>> _values) { values = _values; }

   private:
    std::vector<std::pair<std::string, std::string>> values;
    std::string curValue;
    Error error;
    BaseExprNode* expr;
    bool result = true;
};
#endif  // SELSQL_DELETEVISITOR_H
