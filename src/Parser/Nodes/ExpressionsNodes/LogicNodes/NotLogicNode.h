//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_NOTLOGICNODE_H
#define SELSQL_NOTLOGICNODE_H

#include "../ExprNode.h"
class NotLogicNode : public BaseExprNode {
   public:
    explicit NotLogicNode(BaseExprNode* _child) : BaseExprNode(_child) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_NOTLOGICNODE_H
