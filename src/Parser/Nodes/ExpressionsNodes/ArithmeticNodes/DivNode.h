//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_DIVNODE_H
#define SELSQL_DIVNODE_H
class DivNode : public ArithmeticNode {
   public:
    DivNode(BaseExprNode* _left, BaseExprNode* _right) : ArithmeticNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_DIVNODE_H
