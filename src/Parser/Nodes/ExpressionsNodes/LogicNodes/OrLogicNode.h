//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ORLOGICNODE_H
#define SELSQL_ORLOGICNODE_H
#include "../BaseExprNode.h"
class OrLogicNode : public BaseExprNode {
   public:
    OrLogicNode(BaseExprNode* _left, BaseExprNode* _right) : BaseExprNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_ORLOGICNODE_H
