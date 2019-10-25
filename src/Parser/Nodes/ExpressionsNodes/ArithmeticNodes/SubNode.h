//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_SUBNODE_H
#define SELSQL_SUBNODE_H
class SubNode : public ArithmeticNode {
   public:
    SubNode(BaseExprNode* _left, BaseExprNode* _right) : ArithmeticNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_SUBNODE_H
