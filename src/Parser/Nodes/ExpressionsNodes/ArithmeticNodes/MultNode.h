//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_MULTNODE_H
#define SELSQL_MULTNODE_H
class MultNode : public ArithmeticNode {
   public:
    MultNode(BaseExprNode* _left, BaseExprNode* _right) : ArithmeticNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_MULTNODE_H
