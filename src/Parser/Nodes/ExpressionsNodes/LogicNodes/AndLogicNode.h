//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ANDLOGICNODE_H
#define SELSQL_ANDLOGICNODE_H
class AndLogicNode : public BaseExprNode {
   public:
    AndLogicNode(BaseExprNode* _left, BaseExprNode* _right) : BaseExprNode(_left, _right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

};

#endif  // SELSQL_ANDLOGICNODE_H
