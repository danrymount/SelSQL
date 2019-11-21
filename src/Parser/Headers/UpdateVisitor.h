//
// Created by sapiest on 24.10.2019.
//

#ifndef SELSQL_UPDATEVISITOR_H
#define SELSQL_UPDATEVISITOR_H
#include "../Nodes/ActionNodes/UpdateNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/AddNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/ArithmeticNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/DivNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/MultNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/SubNode.h"
#include "../Nodes/ExpressionsNodes/AssignUpdateNode.h"
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
#include "../Nodes/ExpressionsNodes/UpdateExprNode.h"
#include "../Nodes/ExpressionsNodes/ValueExprNode.h"
#include "../Nodes/UpdatesAndExprNode.h"
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "TreeVisitor.h"
class UpdateVisitor : public TreeVisitor {
   public:
    explicit UpdateVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)){};

    void visit(UpdateNode* node) override {
        node->getSource()->accept(this);
        tableName = std::move(curValue);
        node->getChild()->accept(this);
    }

    void visit(UpdatesAndExprNode* node) override {
        node->getUpdates()->accept(this);
        expr = node->getExpr();
    }

    void visit(UpdateExprNode* node) override {
        for (auto& child : node->getChildren()) {
            child->accept(this);
            updateValues.emplace_back(std::move(curUpdateValue));
        }
    }
    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    void visit(AssignUpdateNode* node) override {
        curUpdateValue.first = node->getBaseValue();
        node->getExpr()->accept(expressionVisitor);
        curUpdateValue.second = std::move(node->getExpr()->getBaseValue());
    }

    BaseExprNode* getExpr() { return expr; }

    std::vector<std::pair<std::string, std::string>> getUpdates() { return updateValues; }

    std::string getTableName() { return tableName; }

    void setExpressionVisitor(ExpressionVisitor* visitor) { expressionVisitor = visitor; }

   private:
    std::vector<std::pair<std::string, std::string>> updateValues;
    std::pair<std::string, std::string> curUpdateValue;
    std::string curValue;
    Message error;
    BaseExprNode* expr = nullptr;
    std::string tableName;
    ExpressionVisitor* expressionVisitor = nullptr;
};
#endif  // SELSQL_UPDATEVISITOR_H
