//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_COLUMNSANDVALUESNODE_H
#define SELSQL_COLUMNSANDVALUESNODE_H
#include <utility>
#include <vector>
#include "BaseNode.h"
#include "ValuesNodes/BaseValueNode.h"
class ColumnsAndValuesNode : public BaseNode {
   public:
    ColumnsAndValuesNode(std::vector<ColumnNode*> _columns, std::vector<BaseValueNode*> _values)
                                                                                                        : columns(std::move(_columns)),
                                                                                                          values(std::move(_values)) {
    }

    void accept(TreeVisitor* v) override { v->visit(this); }

    std::vector<ColumnNode*> getColumns() { return columns; }

    std::vector<BaseValueNode*> getValues() { return values; }

   private:
    std::vector<ColumnNode*> columns;
    std::vector<BaseValueNode*> values;
};

#endif  // SELSQL_COLUMNSANDVALUESNODE_H
