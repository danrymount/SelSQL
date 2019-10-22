//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ADDNODE_H
#define SELSQL_ADDNODE_H
class AddNode : public ArithmeticNode {
   public:
    AddNode(BaseExprNode* _left, BaseExprNode* _right) : ArithmeticNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_ADDNODE_H
