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
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "TreeVisitor.h"
class ExpressionVisitor : public TreeVisitor {
   public:
    void visit(ExprNode* node) override {
        if (node->getChild()) {
            int res = node->getChild()->getResult();
            if (res == -1) {
                node->getChild()->accept(this);
                res = node->getChild()->getResult();
                node->getChild()->setResult(-1);
            }
            result = res && !isNull;
            isNull = false;
        }
    }

    void visit(NullValueNode* node) override { node->setValue("null"); }

    int executeLeftLogic(BaseExprNode* node) {
        if (node->getLeft()->getResult() == -1) {
            node->getLeft()->accept(this);
            auto res = node->getLeft()->getResult();
            node->getLeft()->setResult(-1);
            return res;
        }
        return node->getLeft()->getResult();
    }

    int executeRightLogic(BaseExprNode* node) {
        if (node->getRight()->getResult() == -1) {
            node->getRight()->accept(this);
            auto res = node->getRight()->getResult();
            node->getRight()->setResult(-1);
            return res;
        }
        return node->getRight()->getResult();
    }

    std::string executeLeftArith(BaseExprNode* node) {
        if (node->getLeft()->getBaseValue().empty()) {
            node->getLeft()->accept(this);
            auto res = node->getLeft()->getBaseValue();
            node->getLeft()->setValue("");
            return res;
        }
        return node->getLeft()->getBaseValue();
    }

    std::string executeRightArith(BaseExprNode* node) {
        if (node->getRight()->getBaseValue().empty()) {
            node->getRight()->accept(this);
            auto res = node->getRight()->getBaseValue();
            node->getRight()->setValue("");
            return res;
        }
        return node->getRight()->getBaseValue();
    }

    void visit(AndLogicNode* node) override { node->setResult(executeLeftLogic(node) and executeRightLogic(node)); }

    void visit(OrLogicNode* node) override { node->setResult(executeLeftLogic(node) or executeRightLogic(node)); }

    void visit(NotLogicNode* node) override { node->setResult(not executeLeftLogic(node)); }

    void visit(AddNode* node) override {
        node->setValue(std::to_string(ActionsUtils::calculate[0](std::stod(executeLeftArith(node)),
                                                                 std::stod(executeRightArith(node)))));
    }

    void visit(DivNode* node) override {
        node->setValue(std::to_string(ActionsUtils::calculate[3](std::stod(executeLeftArith(node)),
                                                                 std::stod(executeRightArith(node)))));
    }

    void visit(SubNode* node) override {
        node->setValue(std::to_string(ActionsUtils::calculate[1](std::stod(executeLeftArith(node)),
                                                                 std::stod(executeRightArith(node)))));
    }

    void visit(MultNode* node) override {
        node->setValue(std::to_string(ActionsUtils::calculate[2](std::stod(executeLeftArith(node)),
                                                                 std::stod(executeRightArith(node)))));
    }

    void visit(MoreNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::GREATER](executeLeftArith(node), executeRightArith(node)));
    }

    void visit(EqualsNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::EQUALS](executeLeftArith(node), executeRightArith(node)));
    }
    void visit(NoEqualsNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::NOEQUALS](executeLeftArith(node), executeRightArith(node)));
    }

    void visit(MoreEqNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::GREATEREQUALS](executeLeftArith(node), executeRightArith(node)));
    }

    void visit(LessEqNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::LOWEREQUALS](executeLeftArith(node), executeRightArith(node)));
    }

    void visit(LessNode* node) override {
        node->setResult(ActionsUtils::checkSign[Cmp::LOWER](executeLeftArith(node), executeRightArith(node)));
    }

    static std::string findValue(RecordsFull values, const std::string& value, const std::string& alias) {
        auto res = std::find_if(values.begin(), values.end(), [value, alias](const RecordColumn& val) {
            return value == val.first.second && (alias == val.first.first || alias.empty());
        });
        if (res != values.end()) {
            return res->second;
        }
        return std::string();
    }

    void visit(IndentExprNode* node) override {
        auto res = findValue(firstValues, node->getName(), node->getAliasName());
        if (res.empty()) {
            res = findValue(secondValues, node->getName(), node->getAliasName());
            if (res.empty()) {
                result = false;
                message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
                return;
            }
        }
        if (res == "null") {
            isNull = true;
        }
        node->setValue(res);
    }

    void visit(ValueExprNode* node) override {}

    void visit(IdentNode* node) override {}

    void setFirstValues(RecordsFull _values) { firstValues = std::move(_values); }

    void setSecondValues(RecordsFull _values) { secondValues = std::move(_values); }

    bool getResult() { return result; }

   private:
    RecordsFull firstValues;
    RecordsFull secondValues;
    bool isNull = false;
    bool result = true;
};

#endif  // SELSQL_EXPRESSIONVISITOR_H
