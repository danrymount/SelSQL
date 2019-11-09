//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_BASEEXPRNODE_H
#define SELSQL_BASEEXPRNODE_H
#include <utility>

#include "../BaseNode.h"
class BaseExprNode : public BaseNode {
   public:
    BaseExprNode() = default;
    BaseExprNode(BaseExprNode* _left, BaseExprNode* _right) : left(_left), right(_right) {}
    explicit BaseExprNode(BaseExprNode* _node) : left(_node) {}
    virtual std::string getBaseValue() { return curValue; }

    int getResult() { return result; }

    void setResult(int _res) { result = _res; }

    void setValue(std::string _val) { curValue = std::move(_val); }

    BaseExprNode* getLeft() { return left; }

    BaseExprNode* getRight() { return right; }

    void addLeft(BaseExprNode* _left) { left = _left; }

    void addRight(BaseExprNode* _right) { right = _right; }

   private:
    int result = -1;
    std::string curValue;
    BaseExprNode* left = nullptr;
    BaseExprNode* right = nullptr;
};
#endif  // SELSQL_BASEEXPRNODE_H
