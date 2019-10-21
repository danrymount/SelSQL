//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_FLOATVALUENODE_H
#define SELSQL_FLOATVALUENODE_H
#include "BaseValueNode.h"
class FloatValueNode : public BaseValueNode {
   public:
    explicit FloatValueNode(double _value) : value(_value) {}

    double getValue() { return value; };

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    double value;
};
#endif  // SELSQL_FLOATVALUENODE_H
