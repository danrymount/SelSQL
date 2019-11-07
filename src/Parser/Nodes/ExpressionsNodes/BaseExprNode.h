//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_BASEEXPRNODE_H
#define SELSQL_BASEEXPRNODE_H
#include "../BaseNode.h"
class BaseExprNode : public BaseNode {
   public:
    BaseExprNode() = default;
    BaseExprNode(BaseExprNode* _left, BaseExprNode* _right) : left(_left), right(_right) {}
    BaseExprNode(BaseExprNode* _node) : left(_node) {}
    virtual std::string getBaseValue() { return std::string(); }

    int getResult() { return result; }

    void setResult(int _res) { result = _res; }

    BaseExprNode* getLeft() { return left; }

    BaseExprNode* getRight() { return right; }

   private:
    int result = -1;
    BaseExprNode* left = nullptr;
    BaseExprNode* right = nullptr;
};
#endif  // SELSQL_BASEEXPRNODE_H
