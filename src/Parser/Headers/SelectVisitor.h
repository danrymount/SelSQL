//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_SELECTVISITOR_H
#define SELSQL_SELECTVISITOR_H
#include <utility>

#include "../Nodes/ActionNodes/SelectNode.h"
#include "../Nodes/ColumnNode.h"
#include "../Nodes/ColumnsAndExprNode.h"
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
#include "../Nodes/JoinNodes/JoinNode.h"
#include "../Nodes/JoinNodes/SourceJoinNode.h"
#include "../Nodes/TableNode.h"
#include "TreeVisitor.h"
class SelectVisitor : public TreeVisitor {
   public:
    void visit(SelectNode* node) override {
        node->getChild()->accept(this);
        source = node->getSource();
    }

    void visit(ColumnsAndExprNode* node) override {
        for (auto& col : node->getColumns()) {
            col->accept(this);
        }
        expr = node->getExpr();
    }

    void visit(TableNode* node) override {
        node->getChild()->accept(this);
        tableName = std::move(curValue);
    }

    void visit(ColumnNode* node) override {
        if (node->getAlias() == nullptr) {
            columns.emplace_back(std::make_pair("*", node->getColumn()->getBaseValue()));
        } else {
            columns.emplace_back(std::make_pair(node->getAlias()->getBaseValue(), node->getColumn()->getBaseValue()));
        }
    }

    void visit(SourceJoinNode* node) override {
        node->getSource()->accept(this);
        if (!curValue.empty()) {
            auto tableName = std::move(curValue);
            std::string alias = "";
            if (node->getAlias() != nullptr) {
                node->getAlias()->accept(this);
                alias = std::move(curValue);
            }
            auto cursor = engine.GetCursor(tableName);
            if (cursor.first->name.empty()) {
                message = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
            } else {
                std::vector<std::pair<std::string, std::string>> columnValues;

                for (auto& col : columns) {
                    columnValues.emplace_back(std::make_pair(col.second, ""));
                }
                message = ActionsUtils::checkFieldsExist(cursor.first, columnValues);

                if (firstRecords.empty()) {
                    firstRecords = addRecord(alias, cursor.second);

                } else {
                    secondRecords = addRecord(alias, cursor.second);
                }
            }
        } else {
            node->getAlias()->accept(this);
            auto alias = std::move(curValue);
            if (firstRecordAndAlias.empty()) {
                firstRecordAndAlias = records;
                std::cout << alias << std::endl;
            } else {
                secondRecordAndAlias = records;
            }
        }
    }

    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>>
    addRecord(std::string aliasName, std::shared_ptr<Cursor> cursor) {
        std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> records;
        // cursor->Reset();
        do {
            auto _record = cursor->Fetch();
            if (_record.empty()) {
                continue;
            }
            std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
            for (auto& col : _record) {
                _newRecord.emplace_back(std::make_pair(std::make_pair(aliasName, col.first), col.second));
            }
            records.emplace_back(_newRecord);
        } while (!cursor->Next());
        return records;
    }

    void visit(JoinNode* node) override {
        node->getFirstSource()->accept(this);
        node->getSecondSource()->accept(this);
        if (!secondRecordAndAlias.empty()) {
            firstRecords = firstRecordAndAlias;
            secondRecords = secondRecordAndAlias;
            firstRecordAndAlias.clear();
            secondRecordAndAlias.clear();
        }
        doubleCycleJoin(node);
        firstRecords.clear();
        secondRecords.clear();
    }

    void doubleCycleJoin(JoinNode* node) {
        records.clear();
        for (auto& first : firstRecords) {
            setFirstValues(first);
            for (auto& second : secondRecords) {
                auto f = first;
                setSecondValues(second);
                node->getExpr()->accept(this);
                if (getResult()) {
                    f.insert(f.end(), second.begin(), second.end());
                    records.emplace_back(f);
                }
            }
        }
    }

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
        for (auto& val : firstValues) {
            if (node->getAliasname() == val.first.first) {
                if (node->getBaseValue() == val.first.second) {
                    curValue = val.second;
                    flag = 1;
                    break;
                }
            }
        }
        if (!flag) {
            for (auto& val : secondValues) {
                if (node->getAliasname() == val.first.first) {
                    if (node->getBaseValue() == val.first.second) {
                        curValue = val.second;
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if (!flag) {
            result = false;
            message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
        }
    }

    void visit(ValueExprNode* node) override { curValue = node->getBaseValue(); }

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    std::vector<std::pair<std::string, std::string>> getColumns() { return columns; }

    bool getResult() { return result; }

    BaseNode* getSource() { return source; }

    BaseExprNode* getExpr() { return expr; }

    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> getRecords() {
        return records;
    }

    void setFirstValues(std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _values) {
        firstValues = std::move(_values);
    }

    void setSecondValues(std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _values) {
        secondValues = std::move(_values);
    }

    std::string getTableName() { return tableName; }

   private:
    MainEngine engine;
    std::string curValue;
    std::string tableName;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> firstRecords;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> secondRecords;

    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> records;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> firstRecordAndAlias;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> secondRecordAndAlias;

    std::vector<std::pair<std::string, std::string>> columns;
    std::vector<std::pair<std::pair<std::string, std::string>, std::string>> firstValues;
    std::vector<std::pair<std::pair<std::string, std::string>, std::string>> secondValues;
    BaseExprNode* expr;
    BaseNode* source;
    bool result = true;
};

#endif  // SELSQL_SELECTVISITOR_H
