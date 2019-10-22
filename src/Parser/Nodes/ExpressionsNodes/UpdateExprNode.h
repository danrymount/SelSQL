//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_UPDATEEXPRNODE_H
#define SELSQL_UPDATEEXPRNODE_H
#include <utility>

#include "BaseExprNode.h"
class UpdateExprNode : public BaseExprNode {
   public:
    explicit UpdateExprNode(std::vector<BaseExprNode*> _children) : children(std::move(_children)) {}

    std::vector<BaseExprNode*> getChildren() { return children; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::vector<BaseExprNode*> children;
};
#endif  // SELSQL_UPDATEEXPRNODE_H
