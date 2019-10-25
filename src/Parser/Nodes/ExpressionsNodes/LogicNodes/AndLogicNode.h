//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ANDLOGICNODE_H
#define SELSQL_ANDLOGICNODE_H
class AndLogicNode : public BaseExprNode {
   public:
    AndLogicNode(BaseExprNode* _left, BaseExprNode* _right) : left(_left), right(_right) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    BaseExprNode* getLeft() { return left; }

    BaseExprNode* getRight() { return right; }

   private:
    BaseExprNode* left;
    BaseExprNode* right;
};

#endif  // SELSQL_ANDLOGICNODE_H
