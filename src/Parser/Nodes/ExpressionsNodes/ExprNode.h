//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_EXPRNODE_H
#define SELSQL_EXPRNODE_H

#include "../BaseNode.h"
#include "BaseExprNode.h"
class ExprNode : public BaseExprNode {
   public:
    explicit ExprNode(BaseExprNode* _child) : child(_child) {}

    explicit ExprNode(BaseNode* _child) { child = static_cast<BaseExprNode*>(_child); }

    void accept(TreeVisitor* v) override { v->visit(this); }

    BaseExprNode* getChild() { return child; }

   private:
    BaseExprNode* child;
};

#endif  // SELSQL_EXPRNODE_H
