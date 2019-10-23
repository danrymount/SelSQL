//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_COLUMNSANDEXPRNODE_H
#define SELSQL_COLUMNSANDEXPRNODE_H

#include <utility>

#include "BaseNode.h"
#include "ExpressionsNodes/BaseExprNode.h"
class ColumnsAndExprNode : public BaseNode {
   public:
    explicit ColumnsAndExprNode(std::vector<ColumnNode*> _columns, BaseExprNode* _expr)
                                                                                                        : columns(std::move(_columns)),
                                                                                                          expr(_expr){};
    void accept(TreeVisitor* v) override { v->visit(this); }

    std::vector<ColumnNode*> getColumns() { return columns; }

    BaseExprNode* getExpr() { return expr; }

   private:
    std::vector<ColumnNode*> columns;
    BaseExprNode* expr;
};
#endif  // SELSQL_COLUMNSANDEXPRNODE_H
