//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_INSERTVISITOR_H
#define SELSQL_INSERTVISITOR_H

#include "../Nodes/ActionNodes/InsertNode.h"
#include "../Nodes/ColumnNode.h"
#include "../Nodes/ColumnsAndValuesNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "../Nodes/ValuesNodes/CharValueNode.h"
#include "../Nodes/ValuesNodes/FloatValueNode.h"
#include "../Nodes/ValuesNodes/IntValueNode.h"
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "TreeVisitor.h"
class InsertVisitor : public TreeVisitor {
   public:
    void visit(InsertNode* node) override {
        node->getSource()->accept(this);
        node->getChild()->accept(this);
    }

    void visit(ColumnsAndValuesNode* node) override {
        for (auto& col : node->getColumns()) {
            col->accept(this);
        }
        for (auto& val: node->getValues()) {
            val->accept(this);
        }
    }

    void visit(IdentNode* node) override { tableName = node->getBaseValue(); }

    void visit(ColumnNode* node) override { columns.emplace_back(node->getColumn()->getBaseValue()); }

    void visit(IntValueNode* node) override {
        values.emplace_back(std::to_string(node->getValue()));
    }

    void visit(CharValueNode* node) override {
        values.emplace_back(node->getValue());
    }

    void visit(FloatValueNode* node) override {
        values.emplace_back(std::to_string(node->getValue()));
    }

    void visit(NullValueNode* node) override {
        values.emplace_back("null");
    }

    std::vector<std::string> getValues() { return values; }

    std::vector<std::string> getColumns() { return columns; }

    std::string getTableName() { return tableName; }

   private:
    std::vector<std::string> values;
    std::vector<std::string> columns;
    std::string tableName;
};
#endif  // SELSQL_INSERTVISITOR_H
