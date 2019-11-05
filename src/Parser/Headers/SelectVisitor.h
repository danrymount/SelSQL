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

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    void visit(ColumnNode* node) override {
        if (node->getAlias() == nullptr) {
            columns.emplace_back(std::make_pair("", node->getColumn()->getBaseValue()));
        } else {
            columns.emplace_back(std::make_pair(node->getAlias()->getBaseValue(), node->getColumn()->getBaseValue()));
        }
    }

    void visit(SourceJoinNode* node) override {
        node->getSource()->accept(this);
        if (!curValue.empty()) {
            auto tableName = std::move(curValue);
            std::string alias = tableName;
            if (node->getAlias() != nullptr) {
                node->getAlias()->accept(this);
                alias = std::move(curValue);
            }
            auto cursor = engine.GetCursor(tableName);
            if (cursor.first->name.empty()) {
                message = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
                return;
            } else {
                std::vector<std::pair<std::string, std::string>> columnValues;

                for (auto& col : columns) {
                    columnValues.emplace_back(std::make_pair(col.second, ""));
                }
                message = ActionsUtils::checkFieldsExist(cursor.first, columnValues);
                if (message.getErrorCode()) {
                    return;
                }
                allrecords.emplace_back(addRecord(alias, cursor.second));
            }
        }
        //        else {
        //            node->getAlias()->accept(this);
        //            auto alias = std::move(curValue);
        //            if (firstRecordAndAlias.empty()) {
        //                firstRecordAndAlias = records;
        //                std::cout << alias << std::endl;
        //            } else {
        //                secondRecordAndAlias = records;
        //            }
        //        }
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
        if (message.getErrorCode()) {
            return;
        }
        node->getSecondSource()->accept(this);
        if (message.getErrorCode()) {
            return;
        }
        secondRecords = allrecords.back();
        allrecords.pop_back();
        firstRecords = allrecords.back();
        allrecords.pop_back();
        doubleCycleJoin(node);
        allrecords.emplace_back(records);
        firstRecords.clear();
        secondRecords.clear();
    }

    void doubleCycleJoin(JoinNode* node) {
        records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            for (auto& second : secondRecords) {
                auto f = first;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getResult()) {
                    f.insert(f.end(), second.begin(), second.end());
                    records.emplace_back(f);
                }
            }
        }
    }

    std::vector<std::pair<std::string, std::string>> getColumns() { return columns; }


    BaseNode* getSource() { return source; }

    BaseExprNode* getExpr() { return expr; }

    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> getRecords() {
        return allrecords[0];
    }

    void setExpressionVisitor(ExpressionVisitor* visitor) { expressionVisitor = visitor; }

    std::string getTableName() { return tableName; }

   private:
    MainEngine engine;
    std::string curValue;
    std::string tableName;
    typedef std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> JoinRecord;
    JoinRecord firstRecords;
    JoinRecord secondRecords;

    JoinRecord records;
    std::vector<JoinRecord> allrecords;

    std::vector<std::pair<std::string, std::string>> columns;
    BaseExprNode* expr;
    BaseNode* source;
    ExpressionVisitor* expressionVisitor;
};

#endif  // SELSQL_SELECTVISITOR_H
