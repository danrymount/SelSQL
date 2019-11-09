//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_ARITHMETICNODE_H
#define SELSQL_ARITHMETICNODE_H
#include "../BaseExprNode.h"
class ArithmeticNode : public BaseExprNode {
   public:
    ArithmeticNode(BaseExprNode* _left, BaseExprNode* _right) : BaseExprNode(_left, _right) {}
};

#endif  // SELSQL_ARITHMETICNODE_H
