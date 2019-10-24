//
// Created by sapiest on 24.10.2019.
//

#ifndef SELSQL_ASSIGNUPDATENODE_H
#define SELSQL_ASSIGNUPDATENODE_H
#include <utility>

#include "BaseExprNode.h"
class AssignUpdateNode : public BaseExprNode {
   public:
    AssignUpdateNode(std::string _colName, BaseExprNode* node) : colName(std::move(_colName)), expr(node) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    std::string getBaseValue() override { return colName; }
    std::string getColName() { return colName; }
    BaseExprNode* getExpr() { return expr; }

   private:
    std::string colName;
    BaseExprNode* expr;
};

#endif  // SELSQL_ASSIGNUPDATENODE_H
