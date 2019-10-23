//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_CMPNODE_H
#define SELSQL_CMPNODE_H
#include "../../ValuesNodes/BaseValueNode.h"
#include "../BaseExprNode.h"
class CmpNode : public BaseExprNode {
   public:
    CmpNode() = default;
    CmpNode(BaseExprNode* _left, BaseExprNode* _right) : left(_left), right(_right) {}

    void addChilds(BaseExprNode* _left, BaseExprNode* _right) {
        left = _left;
        right = _right;
    }

    BaseExprNode* getLeft() { return left; }

    BaseExprNode* getRight() { return right; }

   private:
    BaseExprNode* left = nullptr;
    BaseExprNode* right = nullptr;

};
#endif  // SELSQL_CMPNODE_H
