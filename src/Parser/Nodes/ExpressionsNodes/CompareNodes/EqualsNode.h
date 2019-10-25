//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_EQUALSNODE_H
#define SELSQL_EQUALSNODE_H
#include "CmpNode.h"
class EqualsNode : public CmpNode {
   public:
    EqualsNode() = default;
    EqualsNode(BaseExprNode* left, BaseExprNode* right) : CmpNode(left, right) {}
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_EQUALSNODE_H
