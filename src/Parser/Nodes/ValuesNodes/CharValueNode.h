//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_CHARVALUENODE_H
#define SELSQL_CHARVALUENODE_H
#include <utility>

#include "BaseValueNode.h"
class CharValueNode : public BaseValueNode {
   public:
    explicit CharValueNode(std::string _value) : value(std::move(_value)) {}

    std::string getValue() { return value; };

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string value;
};

#endif  // SELSQL_CHARVALUENODE_H
