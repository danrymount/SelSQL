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
#include "../Nodes/JoinNodes/FullJoinNode.h"
#include "../Nodes/JoinNodes/JoinNode.h"
#include "../Nodes/JoinNodes/LeftJoinNode.h"
#include "../Nodes/JoinNodes/RightJoinNode.h"
#include "../Nodes/JoinNodes/SourceJoinNode.h"
#include "../Nodes/TableNode.h"
#include "TreeVisitor.h"
typedef std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> JoinRecord;
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
            tableName = std::move(curValue);
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
    }

    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>>
    addRecord(std::string aliasName, std::shared_ptr<Cursor> cursor) {
        std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> records;
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

    void startExecuteJoin(BaseJoinNode* node) {
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
    }

    void endExecuteJoin() {
        allrecords.emplace_back(records);
        records.clear();
        firstRecords.clear();
        secondRecords.clear();
    }

    void visit(LeftJoinNode* node) override {
        startExecuteJoin(node);
        records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : secondRecords) {
                auto joinRecords = first;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), second.begin(), second.end());
                    records.emplace_back(joinRecords);
                    flag = 1;
                }
            }
            if (!flag) {
                auto joinRecords = first;
                for (auto& rec : secondRecords[0]) {
                    auto tempRec = rec;
                    tempRec.second.clear();
                    joinRecords.emplace_back(tempRec);
                }
                records.emplace_back(joinRecords);
            }
        }
        endExecuteJoin();
    }

    void visit(RightJoinNode* node) override {
        startExecuteJoin(node);
        records.clear();
        for (auto& first : secondRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : firstRecords) {
                auto joinRecords = second;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), first.begin(), first.end());
                    records.emplace_back(joinRecords);
                    flag = 1;
                }
            }
            if (!flag) {
                auto joinRecords = firstRecords[0];
                for (auto& joinRec : joinRecords) {
                    joinRec.second.clear();
                }
                for (auto& rec : first) {
                    joinRecords.emplace_back(rec);
                }
                records.emplace_back(joinRecords);
            }
        }
        endExecuteJoin();
    }

    void visit(FullJoinNode* node) override {
        startExecuteJoin(node);
        records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : secondRecords) {
                auto joinRecords = first;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), second.begin(), second.end());
                    records.emplace_back(joinRecords);
                    flag = 1;
                }
            }
            if (!flag) {
                auto joinRecords = first;
                for (auto& rec : secondRecords[0]) {
                    auto tempRec = rec;
                    tempRec.second.clear();
                    joinRecords.emplace_back(tempRec);
                }
                records.emplace_back(joinRecords);
            }
        }
        for (auto& first : secondRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : firstRecords) {
                auto joinRecords = second;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), first.begin(), first.end());
                    records.emplace_back(joinRecords);
                    flag = 1;
                }
            }
            if (!flag) {
                auto joinRecords = firstRecords[0];
                for (auto& joinRec : joinRecords) {
                    joinRec.second.clear();
                }
                for (auto& rec : first) {
                    joinRecords.emplace_back(rec);
                }
                records.emplace_back(joinRecords);
            }
        }
        endExecuteJoin();
    }

    void visit(JoinNode* node) override {
        startExecuteJoin(node);
        node->getExpr()->accept(this);
        if (countEq == 2) {
            hashJoin(node);
            countEq = 0;
            curExpr.clear();
        } else {
            nestedLoopsJoin(node);
        }
        endExecuteJoin();
    }

    void visit(ExprNode* node) override { node->getChild()->accept(this); }

    void visit(IndentExprNode* node) override {
        countEq++;
        curExpr.emplace_back(node->getAliasname(), node->getBaseValue());
    }

    void visit(EqualsNode* node) override {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    static bool compareForHash(std::pair<std::pair<std::string, std::string>, std::string>& val) {
        auto curName = std::find_if(curExpr.begin(), curExpr.end(),
                                    [val](const std::pair<std::string, std::string>& value) {
                                        return value.second == val.first.second;
                                    });
        if (curName == curExpr.end()) {
            return false;
        }

        for (int i = 0; i < curExpr.size(); i++) {
            auto exp = curExpr[i];
            if (exp.first == val.first.first || exp.first.empty()) {
                if (val.first.second == exp.second) {
                    id = i;
                    return true;
                }
            }
        }

        return false;
    }

    void
    addRec(JoinRecord small, JoinRecord large,
           std::unordered_map<std::string, std::vector<std::pair<std::pair<std::string, std::string>, std::string>>>
                                                                                                               vals) {
        for (auto& rec : small) {
            auto ident = std::find_if(rec.begin(), rec.end(), compareForHash);
            auto record = vals.find(ident->second);
            if (record != vals.end()) {
                if (record->second == rec) {
                    for (auto& newrecord : large) {
                        auto joinRecords = rec;
                        joinRecords.insert(joinRecords.end(), newrecord.begin(), newrecord.end());
                        records.emplace_back(joinRecords);
                    }
                } else {
                    auto joinRecords = rec;
                    joinRecords.insert(joinRecords.end(), record->second.begin(), record->second.end());
                    records.emplace_back(joinRecords);
                }
            }
        }
        if (id >= 0) {
            curExpr.erase(curExpr.begin() + id);
        }
    }

    void hashJoin(JoinNode* node) {
        std::unordered_map<std::string, std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> vals;

        auto large = firstRecords;
        auto small = secondRecords;
        if (firstRecords.size() < secondRecords.size()) {
            large = secondRecords;
            small = firstRecords;
        }

        if (!small.empty() && !large.empty()) {
            auto ident = std::find_if(small[0].begin(), small[0].end(), compareForHash);
            if (ident == small[0].end()) {
                ident = std::find_if(large[0].begin(), large[0].end(), compareForHash);
                if (ident == large[0].end()) {
                    message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
                    return;
                } else {
                    for (auto& rec : large) {
                        ident = std::find_if(rec.begin(), rec.end(), compareForHash);
                        vals.emplace(std::make_pair(ident->second, rec));
                    }
                    if (id >= 0) {
                        curExpr.erase(curExpr.begin() + id);
                    }
                }
            } else {
                for (auto& rec : small) {
                    ident = std::find_if(rec.begin(), rec.end(), compareForHash);
                    vals.emplace(std::make_pair(ident->second, rec));
                }
                if (id >= 0) {
                    curExpr.erase(curExpr.begin() + id);
                }
            }

            ident = std::find_if(large[0].begin(), large[0].end(), compareForHash);
            if (ident == large[0].end()) {
                ident = std::find_if(small[0].begin(), small[0].end(), compareForHash);
                if (ident == small[0].end()) {
                    message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
                    return;
                } else {
                    addRec(small, large, vals);
                }
            } else {
                addRec(large, small, vals);
            }
        }
    }

    void nestedLoopsJoin(JoinNode* node) {
        records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            for (auto& second : secondRecords) {
                auto joinRecords = first;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), second.begin(), second.end());
                    records.emplace_back(joinRecords);
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
    JoinRecord firstRecords;
    JoinRecord secondRecords;

    JoinRecord records;
    std::vector<JoinRecord> allrecords;

    std::vector<std::pair<std::string, std::string>> columns;
    BaseExprNode* expr;
    BaseNode* source;
    ExpressionVisitor* expressionVisitor;
    int countEq = 0;
    inline static int id = -1;
    inline static std::vector<std::pair<std::string, std::string>> curExpr;
};

#endif  // SELSQL_SELECTVISITOR_H
