//
// Created by sapiest on 24.12.2019.
//

#ifndef SELSQL_EXPRESSIONOPTIMIZERVISITOR_H
#define SELSQL_EXPRESSIONOPTIMIZERVISITOR_H
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
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "TreeVisitor.h"
class ExpressionOptimizerVisitor : public TreeVisitor {
   public:
    void visit(ExprNode* node) override {
        if (node->getChild()) {
            node->getChild()->accept(this);
            result = node->getChild()->getResult() && !isNull;
            isNull = false;
        }
    }

    void visit(NullValueNode* node) override { node->setValue("null"); }

    int executeLeftLogic(BaseExprNode* node) {
        node->getLeft()->accept(this);
        return node->getLeft()->getResult();
    }

    int executeRightLogic(BaseExprNode* node) {
        node->getRight()->accept(this);
        return node->getRight()->getResult();
    }

    std::string executeLeftArith(BaseExprNode* node) {
        node->getLeft()->accept(this);
        return node->getLeft()->getBaseValue();
    }

    std::string executeRightArith(BaseExprNode* node) {
        node->getRight()->accept(this);
        return node->getRight()->getBaseValue();
    }

    void visit(AndLogicNode* node) override {
        auto left = executeLeftLogic(node);
        auto right = executeRightLogic(node);
        if (left != -1 && right != -1) {
            node->setResult(executeLeftLogic(node) and executeRightLogic(node));
        }
    }

    void visit(OrLogicNode* node) override {
        auto left = executeLeftLogic(node);
        auto right = executeRightLogic(node);
        if (left != -1 && right != -1) {
            node->setResult(executeLeftLogic(node) or executeRightLogic(node));
        }
    }

    void visit(NotLogicNode* node) override {
        auto left = executeLeftLogic(node);
        if (left != -1) {
            node->setResult(not executeLeftLogic(node));
        }
    }

    void visit(AddNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        if (!left.empty() and !right.empty()) {
            node->setValue(std::to_string(ActionsUtils::calculate[0](std::stod(left), std::stod(right))));
        }
    }

    void visit(DivNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        if (!left.empty() and !right.empty()) {
            node->setValue(std::to_string(ActionsUtils::calculate[3](std::stod(left), std::stod(right))));
        }
    }

    void visit(SubNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        if (!left.empty() and !right.empty()) {
            node->setValue(std::to_string(ActionsUtils::calculate[1](std::stod(left), std::stod(right))));
        }
    }

    void visit(MultNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        if (!left.empty() and !right.empty()) {
            node->setValue(std::to_string(ActionsUtils::calculate[2](std::stod(left), std::stod(right))));
        }
    }

    void visit(MoreNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::GREATER](left, right));
        }
    }

    void visit(EqualsNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::EQUALS](left, right));
        }
    }
    void visit(NoEqualsNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::NOEQUALS](left, right));
        }
    }

    void visit(MoreEqNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::GREATEREQUALS](left, right));
        }
    }

    void visit(LessEqNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::LOWEREQUALS](left, right));
        }
    }

    void visit(LessNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeLeftArith(node);
        if (!left.empty() && !right.empty()) {
            node->setResult(ActionsUtils::checkSign[Cmp::LOWER](left, right));
        }
    }

    void visit(IndentExprNode* node) override {}

    void visit(ValueExprNode* node) override {}

    void visit(IdentNode* node) override {}

   private:
    bool isNull = false;
    bool result = true;
};
#endif  // SELSQL_EXPRESSIONOPTIMIZERVISITOR_H
