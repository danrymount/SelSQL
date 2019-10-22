//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ARITHMETICNODE_H
#define SELSQL_ARITHMETICNODE_H
class ArithmeticNode : public BaseExprNode {
   public:
    ArithmeticNode(BaseExprNode* _left, BaseExprNode* _right) : left(_left), right(_right) {}

    BaseExprNode* getLeft() { return left; }

    BaseExprNode* getRight() { return right; }

   private:
    BaseExprNode* left;
    BaseExprNode* right;
};

#endif  // SELSQL_ARITHMETICNODE_H
