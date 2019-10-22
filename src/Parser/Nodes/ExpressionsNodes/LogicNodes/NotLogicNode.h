//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_NOTLOGICNODE_H
#define SELSQL_NOTLOGICNODE_H

#include "../ExprNode.h"
class NotLogicNode : public ExprNode {
   public:
    explicit NotLogicNode(BaseExprNode* _child) : ExprNode(_child) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_NOTLOGICNODE_H
