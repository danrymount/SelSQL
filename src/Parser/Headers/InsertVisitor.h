//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_INSERTVISITOR_H
#define SELSQL_INSERTVISITOR_H

#include "../Nodes/ValuesNodes/IntValueNode.h"
#include "../Nodes/ValuesNodes/CharValueNode.h"
#include "../Nodes/ValuesNodes/FloatValueNode.h"
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "../Nodes/ColumnNode.h"
#include "../Nodes/ColumnsAndValuesNode.h"
#include "TreeVisitor.h"
class InsertVisitor : public TreeVisitor {
   public:
    void visit(ColumnsAndValuesNode* node) override {
        for (auto& col : node->getColumns()) {
            col->accept(this);
        }
        for (auto& val: node->getValues()) {
            val->accept(this);
        }
    }

    void visit(ColumnNode* node) override {
        columns.emplace_back(node->getName());
    }

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

   private:
    std::vector<std::string> values;
    std::vector<std::string> columns;
};
#endif  // SELSQL_INSERTVISITOR_H
