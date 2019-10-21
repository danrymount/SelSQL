//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_CHARVALUENODE_H
#define SELSQL_CHARVALUENODE_H
#include <utility>

#include "BaseValueNode.h"
class CharValueNode : public BaseValueNode {
   public:
    CharValueNode(std::string _value, int _size) : value(std::move(_value)), size(_size) {}

    std::string getValue() { return value; };

    int getSize() { return size; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string value;
    int size;
};

#endif  // SELSQL_CHARVALUENODE_H
