//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_INTVALUENODE_H
#define SELSQL_INTVALUENODE_H
#include "BaseValueNode.h"
class IntValueNode : public BaseValueNode {
   public:
    explicit IntValueNode(int _value) : value(_value) {}

    int getValue() { return value; };

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    int value;
};
#endif  // SELSQL_INTVALUENODE_H
